#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <common/protocol_imp.h>
#include <common/packetwrapper.h>
#include "world.h"
#include "config.h"
#include "netmsg.h"
#include "usermsg.h"
#include "userlogin.h"
#include "task/taskman.h"
#include "player_imp.h"
#include <factionlib.h>
#include "instance/battleground_manager.h"
#include "battleground/bg_world_manager.h"
#include "battlefield/bf_world_manager.h"
#include "territory/tr_world_manager.h"
#include "raid/raid_world_manager.h"
#include "kingdom/kingdom_common.h"
#include "commondata.h"
#include "playertemplate.h"
#include "forbid_manager.h"
#include "facbase_mall.h"

inline static bool check_player(gplayer *pPlayer,int cs_index,int sid,int uid)
{
	return pPlayer->IsActived()     && pPlayer->cs_index == cs_index 
		&& pPlayer->cs_sid == sid 
		&& pPlayer->ID.id == uid;
}

inline static void single_trade_end(int trade_id, int role, bool need_read,int reason)
{	
	int index;
	//gplayer *pPlayer = world_manager::GetInstance()->FindPlayer(role,index);
	gplayer *pPlayer = gmatrix::FindPlayer(role,index);
	if(!pPlayer) return ;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != role || !pPlayer->IsActived() || !pPlayer->imp)  
	{
		return;
	}
	pPlayer->imp->TradeComplete(trade_id,reason,need_read);
}

void trade_end(int trade_id, int role1,int role2,bool need_read1,bool need_read2)
{
	ASSERT(role1 != role2 && role1 >0 && role2 > 0);
	if(role1 == role2 || role1 <= 0 ||  role2 <= 0) return;
	single_trade_end(trade_id,role1,need_read1,0);
	single_trade_end(trade_id,role2,need_read2,0);
}

void trade_start(int trade_id, int role1,int role2, int localid1,int localid2)
{
	ASSERT(role1 != role2 && role1 >0 && role2 > 0);
	try
	{
		if(role1 == role2 || role1 <= 0 ||  role2 <= 0) throw -1001;
		if(gmatrix::GetWorldParam().forbid_trade) throw -1002;

		int index1,index2;
		gplayer *pPlayer1 = gmatrix::FindPlayer(role1,index1);
		gplayer *pPlayer2 = gmatrix::FindPlayer(role2,index2);
		if(!pPlayer1 || !pPlayer2 || index1 != index2)
		{
			//目标不存在或者不在同一个世界
			throw -1;
		}
		spin_doublelock keeper(pPlayer1->spinlock,pPlayer2->spinlock);

		if(pPlayer1->ID.id != role1 || pPlayer1->cs_sid != localid1) throw -3;
		if(pPlayer2->ID.id != role2 || pPlayer2->cs_sid != localid2) throw -4;
		if(pPlayer1->tag != pPlayer2->tag) throw -14;
		if(!pPlayer1->IsActived() || pPlayer1->IsZombie() || pPlayer1->login_state != gplayer::LOGIN_OK) throw -5;
		if(!pPlayer2->IsActived() || pPlayer2->IsZombie() || pPlayer2->login_state != gplayer::LOGIN_OK) throw -6;

		if(pPlayer1->pos.squared_distance(pPlayer2->pos) > 10.f*10.f) throw -7;
		if(!pPlayer1->imp || !pPlayer2->imp) throw -10;

		if(!pPlayer1->imp->CanTrade(pPlayer2->ID)) throw -8;
		if(!pPlayer2->imp->CanTrade(pPlayer1->ID)) throw -9;
		pPlayer1->imp->StartTrade(trade_id,pPlayer2->ID);
		pPlayer2->imp->StartTrade(trade_id,pPlayer1->ID);
		GLog::log(GLOG_INFO,"玩家交易准备开始 用户%d <--> 用户%d, 交易id:%d",role1,role2,trade_id);

	}catch(int)
	{
		GMSV::ReplyTradeRequest(trade_id, role1,localid1,false);
	}
}

void player_not_online(int user_id, int link_id, int sid) 
{
	GLog::log(GLOG_INFO,"收到用户%d不在线的消息 %d %d",user_id, link_id, sid);
	//遇到这种情况的处理就是强行下线，而且不进行存盘操作
	int index1;
	gplayer * pPlayer = gmatrix::FindPlayer(user_id,index1);
	if(!pPlayer)
	{
		GLog::log(GLOG_INFO,"异常情况出现，Deliveryd速度好慢，去买彩票吧");
		//没有找到 合适的用户
		//正常情况下，这个用户应该存在的
		return;
	}
	spin_autolock keeper(pPlayer->spinlock);

	if(!pPlayer->IsActived() || pPlayer->ID.id != user_id  ||  
			pPlayer->cs_index != link_id || pPlayer->cs_sid != sid || !pPlayer->imp)
	{
		//这个用户可能正好已经消失了，所以直接返回，不进行处理
		return;
	}
	pPlayer->imp->PlayerForceOffline();
}

namespace
{
	static int _leave_lock = 0;
	static int _leave_map_size = 0;
	static abase::hash_map<int, int , abase::_hash_function, abase::fast_alloc<> > _leave_map(100000);
	static bool test_err_command(int id)
	{
		spin_autolock keeper(_leave_lock);
		int t = _leave_map[id];
		int t1 = g_timer.get_systime();
		if(t == 0 || t1 - t > 5)
		{
			return false;
		}
		return true;
	}
}

void record_leave_server(int uid)
{
	spin_autolock keeper(_leave_lock);
	_leave_map[uid] = g_timer.get_systime();
	_leave_map_size = _leave_map.size();
}

int  total_login_player_number()
{
	return _leave_map_size;
}

size_t handle_chatdata(int uid, const void * aux_data, size_t size, void * buffer, size_t len)
{       
	//考虑根据具体数据来助理
	if(size < sizeof(short)) return 0;

	//简单的聊天处理策略
	int index;
	gplayer * pPlayer = gmatrix::FindPlayer(uid,index);
	if(!pPlayer) return 0;

	spin_autolock keeper(&pPlayer->spinlock);
	if(!pPlayer->IsActived() || pPlayer->ID.id != uid || pPlayer->b_disconnect || !pPlayer->imp)
	{
		//这种特殊情况直接忽略
		return 0;
	}
	gplayer_imp * pImp = ((gplayer_imp*)pPlayer->imp);
	return pImp->TransformChatData(aux_data,size, buffer, len);
}       


void handle_user_chat(int cs_index,int sid, int uid, const void * msg, size_t size, const void * aux_data, size_t size2, char channel, int item_idx)
{
	//简单的聊天处理策略
	int index;
	gplayer * pPlayer = gmatrix::FindPlayer(uid,index);
	if(!pPlayer)
	{
		if(!test_err_command(uid)) 
		{
			//要不要做特殊处理，因为它可能在另外一台服务器上
			GLog::log(GLOG_INFO,"user_msg::用户%d已经不在服务器中，断线处理",uid);

			//现在先做下线处理
			GMSV::SendDisconnect(cs_index, uid, sid,0);
		}
		return ;
	}

	spin_autolock keeper(&pPlayer->spinlock);
	if(!pPlayer->IsActived() || pPlayer->cs_index != cs_index
			|| pPlayer->ID.id != uid
			|| pPlayer->cs_sid != sid
			|| pPlayer->b_disconnect
			|| !pPlayer->imp)

	{
		//这种特殊情况直接忽略
		return ;
	}
	slice * pPiece = pPlayer->pPiece;
	if(pPiece == NULL)
	{
		//这种情况也直接忽略
		return ;
	}
	gplayer_imp * pImp = ((gplayer_imp*)pPlayer->imp);

	char buffer[1024];
	int dsize = 0;
	if(size2) dsize = pImp->TransformChatData(aux_data,size2, buffer, sizeof(buffer));

	switch(channel)
	{
		case GMSV::CHAT_CHANNEL_LOCAL:
		case GMSV::CHAT_CHANNEL_TRADE:
			break;

		case GMSV::CHAT_CHANNEL_FARCRY:
			pImp->SendFarCryChat(channel,msg,size, buffer, dsize);
			return;
		
		case GMSV::CHAT_CHANNEL_SPEAKER:
			pImp->SendGlobalChat(channel,msg,size, buffer, dsize, item_idx);	//大喇叭
			return;
		
		case GMSV::CHAT_CHANNEL_RUMOR:
			pImp->SendRumorChat(channel,msg,size, buffer, dsize, item_idx);	//流言
			return;

		case GMSV::CHAT_CHANNEL_WISPER:
		case GMSV::CHAT_CHANNEL_FACTION:
			return ;

		case GMSV::CHAT_CHANNEL_TEAM:
			pImp->SendTeamChat(channel,msg,size,buffer, dsize);
			return;

		case GMSV::CHAT_CHANNEL_ZONE:
			pImp->SendZoneChat(channel,msg,size, buffer, dsize);
			return;
	}

	pImp->SendNormalChat(channel,msg,size,buffer, dsize);
	return ;
}

static long co(timeval & t1, timeval & t2)
{
	return (t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec);
}

void handle_user_cmd(int cs_index,int sid,int uid,const void * buf, size_t size)
{
	if(!g_d_state.CanLogin())
	{
		//当前不允许万家登录，也自然不允许用户做任何操作
		return;
	}
	if(size < sizeof(C2S::cmd_header))
	{
		//用户数据大小有错误的
		//GLog::log(GLOG_WARNING,"用户%d发来错误的数据",uid);
		GMSV::ReportCheater2Gacd(uid, 777 , NULL,0);
		GMSV::SendDisconnect(cs_index, uid, sid,0);
		return ;
	}
	timeval tv,tv1,tv2,tv3,tv4;
	gettimeofday(&tv,NULL);

	int index;
	gplayer * pPlayer = gmatrix::FindPlayer(uid,index);
	if(!pPlayer) 
	{
		if(!test_err_command(uid))
		{
			//要不要做特殊处理，因为它可能在另外一台服务器上
			GLog::log(GLOG_INFO,"user_cmd::用户%d已经不在服务器中，断线处理",uid);

			//现在先做下线处理
			GMSV::SendDisconnect(cs_index, uid, sid,0);
		}
		return ;
	}
	gettimeofday(&tv1,NULL);
	int cmd = ((const C2S::cmd_header*)buf) -> cmd;

	pPlayer->Lock();
	gettimeofday(&tv2,NULL);
	if(!pPlayer->IsActived() || pPlayer->cs_index != cs_index 
				 || pPlayer->ID.id != uid 
				 || pPlayer->cs_sid != sid
				 || pPlayer->imp == NULL
				 || pPlayer->b_disconnect)
	{
		pPlayer->Unlock();
		return ;
	}
		
	if(pPlayer->login_state != gplayer::LOGIN_OK)
	{
		int login_state = pPlayer->login_state;
		//验证一下是否可以取数据
		pPlayer->Unlock();
		if(login_state <= gplayer::WAITING_ENTER)
		{
			GLog::log(GLOG_ERR,"user_cmd:在非符合的状态收到了客户端发来的数据 %d(外挂)",uid);
		}
		return;
	}
	gettimeofday(&tv3,NULL);
	if(pPlayer->imp->DispatchCommand(cmd , buf, size) == 0)
	{
		ASSERT(pPlayer->spinlock && "锁怎么会被人乱解开呢？");
		if(!pPlayer->spinlock)
		{
			GLog::log(GLOG_ERR,"user_cmd:线程锁被意外解开了%d",uid);
		}
		//如果命令返回0才需要进行解锁
		pPlayer->Unlock();
	}
	else
	{
		//返回非0值就不需要解锁了,因为在DispatchCommand里面已经解开了锁
		ASSERT(pPlayer->spinlock == 0 && "概率很低但是更可能是错误的情况");
	}
	gettimeofday(&tv4,NULL);
	__PRINTF("player %6d command %4d use ---------------%6ld %6ld %6ld %6ld start:%ld.%06ld\n",uid, cmd,
		co(tv,tv1),co(tv1,tv2),co(tv2,tv3),co(tv3,tv4),tv.tv_sec,tv.tv_usec);
	//GLog::log(GLOG_INFO,"pcmd %2d [%d] start:%06ld",cmd,uid,tv.tv_usec);
	return;
}

void get_task_data_reply(int taskid, int uid, const void * env_data, size_t env_size, const void * task_data, size_t task_size)
{
	int world_index;
	gplayer * pPlayer = gmatrix::FindPlayer(uid,world_index);
	ASSERT(task_size == TASK_GLOBAL_DATA_SIZE);
	if(pPlayer)
	{
		spin_autolock keeper(pPlayer->spinlock);
		if(pPlayer->ID.id == uid && pPlayer->IsActived() && 
				pPlayer->login_state == gplayer::LOGIN_OK && pPlayer->imp)
		{
			//这是是否还要根据player_imp的其他状态作出一些判定...
			PlayerTaskInterface task_if(((gplayer_imp*)pPlayer->imp));
			OnTaskReceivedGlobalData(&task_if,taskid,(unsigned char*)task_data,env_data,env_size);
		}
	}
	//收到任务数据的回应
}

void get_task_data_timeout(int taskid, int uid, const void * env_data, size_t env_size)
{
	__PRINTF("收到全局任务数据的超时 \n");
}

void psvr_ongame_notify(int * start , size_t size,size_t step)
{
//	GLog::log(GLOG_INFO,"收到用户进入游戏的消息(%d):%d",*start,size);
	__PRINTF("收到用户进入游戏的消息(%d):%d\n",*start,size);
//	world_manager::GetInstance()->BatchSetPlayerServer(start,size,step);
	
}

void psvr_offline_notify(int * start , size_t size,size_t step)
{
//	GLog::log(GLOG_INFO,"收到用户离开的消息(%d):%d",*start,size);
	__PRINTF("收到用户离开的消息(%d):%d\n",*start,size);
	ASSERT(size == 1);
//	world_manager::GetInstance()->RemovePlayerServerIdx(*start);
	
}

//发送寄售角色失败信息并记录log
void send_player_consignrole_failed(int cs_index, int roleid, int sid, int result)
{
	GMSV::SendPlayerConsignFailedOfflineRe(cs_index, roleid, sid, result);
	GLog::log(GLOG_INFO, "玩家角色寄售offline_re发送，result=%d", result);
}

template <int OFFLINE_CODE>
static void user_offline(int cs_index, int sid, int uid)
{
	GLog::log(GLOG_INFO,"用户断线了(%d):%d",OFFLINE_CODE,uid);
	int index;
	gplayer * pPlayer = gmatrix::FindPlayer(uid,index);
	if(pPlayer == NULL)
	{
		if(gplayer_imp::PLAYER_OFF_CONSIGNROLE == OFFLINE_CODE)
		{
			send_player_consignrole_failed(cs_index, uid, sid, gplayer_imp::ERR_CONSIGN_ROLE_OFFLINE_1);
		}
		else
		{
			//如果已经不存在了，则同意断线要求
			GMSV::SendOfflineRe(cs_index,uid,sid,0);	// offline re
		}
		return;
	}

	spin_autolock alock(pPlayer->spinlock);
	if(!check_player(pPlayer,cs_index,sid,uid))
	{
		GLog::log(GLOG_WARNING,"lost_connect(%d):用户%d已经不再服务器里了",OFFLINE_CODE,uid);

		if(gplayer_imp::PLAYER_OFF_CONSIGNROLE == OFFLINE_CODE)
		{
			send_player_consignrole_failed(cs_index, uid, sid, gplayer_imp::ERR_CONSIGN_ROLE_OFFLINE_2);
		}
		else
		{
			GMSV::SendOfflineRe(cs_index,uid,sid,0);	// offline re
		}
		return ;
	}
	if(pPlayer->login_state == gplayer::WAITING_LOGIN)
	{	
		//让player断线，不进行数据保存，直接释放对象
		ASSERT(pPlayer->pPiece == NULL);

		//这里不能够进行断线处理，因为数据库还没有处理完，一旦数据回来就可能会出错
		//所以只是设置一个断线标志
		pPlayer->b_disconnect = true;
		return ;
	}
	else if (pPlayer->login_state == gplayer::WAITING_ENTER)
	{
		//正在等待进入的阶段，直接退出
		__PRINTF("在enter_world阶段断线了%d\n",uid);
		ASSERT(pPlayer->pPiece == NULL);
		pPlayer->imp->_commander->Release();
		if(gplayer_imp::PLAYER_OFF_CONSIGNROLE == OFFLINE_CODE)
		{
			send_player_consignrole_failed(cs_index, uid, sid, gplayer_imp::ERR_CONSIGN_ROLE_OFFLINE_3);
		}
		else
		{
			GMSV::SendOfflineRe(cs_index,uid,sid,0);	// offline re
		}

		return ;
	}

	
	ASSERT(pPlayer->imp);
	//发来了断线消息，按照正常情况处理
	((gplayer_imp*)(pPlayer->imp))->LostConnection(OFFLINE_CODE);
}

void	user_lost_connection(int cs_index,int sid,int uid)
{
	user_offline<gplayer_imp::PLAYER_OFF_OFFLINE>(cs_index,sid,uid);
}

void	user_kickout(int cs_index,int sid,int uid)
{
	user_offline<gplayer_imp::PLAYER_OFF_KICKOUT>(cs_index,sid,uid);
}

void user_offline_consignrole(int cs_index, int sid, int uid)
{
	user_offline<gplayer_imp::PLAYER_OFF_CONSIGNROLE>(cs_index, sid, uid);
}

void	user_reconnect(int cs_index,int sid,int roleid)
{
	GLog::log(GLOG_INFO,"[reconect]: receive the reconnect request from delivery, cs_index=%d, sid=%d, roleid=%d", cs_index, sid, roleid);

	int player_index;
	gplayer *pPlayer = gmatrix::FindPlayer(roleid, player_index);
	if(!pPlayer) 
	{
		GMSV::SendReconnectRe(roleid, 0, -1); 
		return;
	}

	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != (int)roleid || !pPlayer->IsActived() || !pPlayer->imp)  
	{
		GMSV::SendReconnectRe(roleid, 0, -1); 
		return ;
	}

	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->TryReconnect();
}

void
switch_server_timeout(int link_id,int user_id,int localsid)
{
	GLog::log(GLOG_ERR,"用户%d转移服务器超时",user_id);
	//现在先不处理
	//ASSERT(false);
}

void switch_server_request(int link_id,int user_id, int localsid, int source,const void * key_buf, size_t key_size)
{
	ASSERT(false && "不应该出现");
}

void switch_server_cancel(int link_id,int user_id, int localsid, int )
{
	ASSERT(false && "不应该出现");
}


void faction_trade_lock(int trade_id, int roleid)
{
	try
	{
		int index;
		gplayer *pPlayer = gmatrix::FindPlayer(roleid,index);
		if(!pPlayer) 
		{
			//目标不存在
			throw -1;
		}
		spin_autolock keeper(pPlayer->spinlock);

		if(pPlayer->ID.id != roleid ) throw -2;
		if(!pPlayer->IsActived() || pPlayer->IsZombie() || pPlayer->login_state!=gplayer::LOGIN_OK) throw -3;
		if(!pPlayer->imp) throw -4;
		if(!pPlayer->imp->StartFactionTrade(trade_id,0,DBMASK_PUT_SYNC_TIMEOUT)) throw -5;
	}catch(int)
	{
		GNET::syncdata_t data(0,0);
//BW		GNET::SendFactionLockResponse(-1,trade_id,roleid,data);
	}
}

inline void faction_trade_end(int trade_id, int role, const GNET::syncdata_t & data)
{	
	int index;
	gplayer *pPlayer = gmatrix::FindPlayer(role,index);
	if(!pPlayer) return ;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != role||!pPlayer->IsActived()||!pPlayer->imp||pPlayer->login_state!=gplayer::LOGIN_OK)
	{
		return;
	}
	pPlayer->imp->FactionTradeComplete(trade_id,data);
}

void player_end_sync(int role, unsigned int money, GDB::itemlist const& item_change,bool is_write_trashbox, bool is_write_shoplog)
{
	__PRINTF("%d通知内部交易结束收到\n",role);
	int index;
	gplayer *pPlayer = gmatrix::FindPlayer(role,index);
	if(!pPlayer) return ;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != role||!pPlayer->IsActived()||!pPlayer->imp||pPlayer->login_state!=gplayer::LOGIN_OK)
	{
		return;
	}

	pPlayer->imp->SyncTradeComplete(0,money, item_change,is_write_trashbox,true,is_write_shoplog);
}

void player_cancel_sync(int role)
{
	__PRINTF("内部交易取消\n");
	GDB::itemlist item_change;
	memset(&item_change, 0,sizeof(item_change));
	int index;
	gplayer *pPlayer = gmatrix::FindPlayer(role,index);
	if(!pPlayer) return ;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != role||!pPlayer->IsActived()||!pPlayer->imp||pPlayer->login_state!=gplayer::LOGIN_OK)
	{
		return;
	}
	
	GLog::log(GLOG_INFO,"用户%d外部操作取消",pPlayer->ID.id);
	pPlayer->imp->SyncTradeComplete(0,0, item_change,false,false,false);
}

void player_cosmetic_result(int role, int ticket_inv_idx, int ticket_id, int result, unsigned int crc)
{	
	//现在已经不会整容了
}

void notify_player_reward(int role, int reward, int param, int fill_info_mask)
{
	//donothing
	__PRINTF("recv player reward %d %d %d\n",role, reward, param);
	int index;
	gplayer *pPlayer = gmatrix::FindPlayer(role,index);
	if(!pPlayer) return ;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != role||!pPlayer->IsActived()||!pPlayer->imp
			|| (pPlayer->login_state!=gplayer::LOGIN_OK && pPlayer->login_state != gplayer::WAITING_ENTER))
	{
		return;
	}
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->SetSpecailTaskAward(reward,param,fill_info_mask);
	if(reward)
	{
		GLog::log(GLOG_INFO,"用户%d存在任务奖%d %d",role,reward,param);
	}
}

namespace GNET
{
void FactionLockPlayer(unsigned int tid,int roleid)
{
	__PRINTF("faction lock player\n");
	faction_trade_lock(tid,roleid);
}

void FactionUnLockPlayer(unsigned int tid,int roleid,const syncdata_t& syncdata)
{
	__PRINTF("faction unlock player\n");
	faction_trade_end(tid, roleid,syncdata);
}

void ReceivePlayerFactionInfo(int roleid,unsigned int faction_id, unsigned int family_id , char faction_role,void * faction_name, size_t name_len)
{
	__PRINTF("recv player faction info %d, %d, %d\n",roleid,faction_id,faction_role);
	int index;
	gplayer *pPlayer = gmatrix::FindPlayer(roleid,index);
	if(!pPlayer) return ;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != roleid || !pPlayer->IsActived() || !pPlayer->imp || pPlayer->login_state!=gplayer::LOGIN_OK)
	{
		return;
	}

	if (name_len > 0)
	{
		gplayer_imp * pImp = (gplayer_imp *)pPlayer->imp;
		pImp->SetMafiaName(faction_name, name_len);
	}

	if(pPlayer->id_family != (int)family_id ||
			pPlayer->id_mafia != (int)faction_id || 
			pPlayer->rank_mafia != faction_role)
	{
		//帮派信息有变
		if(family_id == (unsigned int)-1 ) family_id = pPlayer->id_family;
		pPlayer->imp->UpdateMafiaInfo(faction_id, family_id , faction_role);
	}
}
};

void gm_shutdown_server()
{
	gmatrix::Instance()->ShutDown();
}

void battleground_start( int battle_world_tag, int battle_id, int end_time, int apply_count, bool is_instance_field, char battle_type)
{
	bg_param param;
	memset(&param,0,sizeof(param));
	param.battle_world_tag = battle_world_tag;
	param.battle_id = battle_id;
	param.end_timestemp = end_time;
	param.apply_count = apply_count;
	world_manager* manager = gmatrix::Instance()->FindWorld( battle_world_tag );
	if( !manager || manager->IsIdle())
	{
		__PRINTF( "战场开启失败，原因：非法的worldtag,"
			"battle_world_tag=%d, battle_id=%d  end_time=%d\n",
			battle_world_tag, battle_id, end_time );
		GLog::log( GLOG_INFO, "战场开启失败，原因：非法的worldtag,"
			"battle_world_tag=%d, battle_id=%d  end_time=%d\n",
			battle_world_tag, battle_id, end_time );

		battleground_start_response(is_instance_field, -1, battle_world_tag, battle_id, battle_type);
		return;
	}
	if( !manager->IsBattleWorld() )
	{
		__PRINTF( "战场开启失败，原因：worldtag指定的world不是战场world,"
			"battle_world_tag=%d, battle_id=%d, end_time=%d\n",
			battle_world_tag, battle_id, end_time );
		GLog::log( GLOG_INFO, "战场开启失败，原因：worldtag指定的world不是战场world,"
			"battle_world_tag=%d, battle_id=%d, end_time=%d\n",
			battle_world_tag, battle_id, end_time );
		battleground_start_response(is_instance_field, -1, battle_world_tag, battle_id, battle_type);
		return;
	}
	// 以下的log在bg_world_manager里做
	if( !manager->CreateBattleGround( param ) )
	{
		battleground_start_response(is_instance_field, -1, battle_world_tag, battle_id, battle_type);
		return;
	}
	battleground_start_response(is_instance_field, 0, battle_world_tag, battle_id, battle_type);
	return;
}

void battleground_start_response(bool is_instance_field, int retcode, int battle_world_tag, int battle_id, char battle_type)
{
	if(!is_instance_field)
	{
		GNET::ResponseBattleStart( retcode, gmatrix::Instance()->GetServerIndex(), battle_world_tag, battle_id, battle_type);
	}
	else
	{
		GNET::ResponseInstanceStart( retcode, gmatrix::Instance()->GetServerIndex(), battle_id );
	}
}

void territory_start(int territory_id, int map_id, unsigned int defender_id, unsigned int attacker_id, int end_timestamp)
{
	world_manager* manager = gmatrix::Instance()->FindWorld( map_id);
	if( !manager || manager->IsIdle())
	{
		__PRINTF( "领土战战场开启失败，原因：非法的worldtag, battle_world_tag=%d, territory_id=%d  end_time=%d\n",
			map_id, territory_id, end_timestamp );
		GLog::log( GLOG_INFO, "领土战战场开启失败，原因：非法的worldtag, battle_world_tag=%d, territory_id=%d  end_time=%d\n",
			map_id, territory_id, end_timestamp );

		territory_start_response(-1, territory_id);
		return;
	}
	// 以下的log在bg_world_manager里做
	if( !manager->CreateBattleTerritory(territory_id, map_id, defender_id, attacker_id, end_timestamp ) )
	{
		territory_start_response(-1, territory_id);
		return;
	}
	territory_start_response(0, territory_id);
	return;

}

void territory_start_response(int retcode, int territory_id)
{
	GNET::ResponseTerritoryStart( retcode,  territory_id);
}


void kingdom_start(char fieldtype, int tag_id, int defender, std::vector<int> &  attacker_list, void * defender_name, size_t name_len)
{
	world_manager* manager = gmatrix::Instance()->FindWorld( tag_id);
	if( !manager || manager->IsIdle())
	{
		GLog::log( GLOG_INFO, "国王战战场开启失败，原因：非法的worldtag, type=%d, world_tag=%d, defender=%d\n",
			fieldtype, tag_id, defender);

		kingdom_start_response(-1, fieldtype, tag_id, defender, attacker_list, defender_name, name_len);
		return;
	}
	
	if( !manager->CreateKingdom(fieldtype, tag_id, defender, attacker_list ) )
	{
		kingdom_start_response(-1, fieldtype, tag_id, defender, attacker_list, defender_name, name_len);
		return;
	}
	kingdom_start_response(0, fieldtype, tag_id, defender, attacker_list, defender_name, name_len);
	return;
}

void kingdom_start_response(int retcode, char fieldtype, int tag_id, int defender, std::vector<int> &  attacker_list, void * defender_name, size_t name_len) 
{
	GNET::ResponseKingdomStart(retcode, fieldtype, tag_id, defender, attacker_list, defender_name, name_len);
}


void kingdom_battle_stop(char fieldtype, int tag_id)
{
	world_manager* manager = gmatrix::Instance()->FindWorld( tag_id);
	if( !manager || manager->IsIdle())
	{
		return;
	}

	manager->StopKingdom(fieldtype, tag_id);
}

void kingdom_point_sync(int point)
{
	gmatrix::SyncKingdomPoint(point);	
}

void kingdom_info_sync(int mafia_id, int point, std::map<int, int> & kingdom_title, int task_type)
{
	gmatrix::SyncKingdomInfo(mafia_id, point, kingdom_title, task_type);
}

void kingdom_title_sync(int roleid, char title)
{
	int player_index;
	gplayer *pPlayer = gmatrix::FindPlayer((int)roleid, player_index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != (int)roleid || !pPlayer->IsActived() || !pPlayer->imp)  
		return;

	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	return pImp->SyncKingdomTitle(title);
	
}

bool player_enter_battleground( int role_id, int battle_world_tag, int battle_id, int battle_faction, int battle_type )
{
	__PRINTF( "收到delivery发来的player_enter_battleground,  role_id=%d, battle_world_tag=%d, battle_id=%d,battle_faction=%d\n",
		role_id, battle_world_tag, battle_id, battle_faction );
	int index;
	gplayer *pPlayer = gmatrix::FindPlayer( role_id, index );
	if( !pPlayer ) return false; // 玩家不存在
	spin_autolock keeper( pPlayer->spinlock );
	if( pPlayer->ID.id != role_id || !pPlayer->IsActived() || !pPlayer->imp || gplayer::LOGIN_OK != pPlayer->login_state )
	{
		__PRINTF( "玩家状态不对\n" );
		return false;
	}
	// 对battle_id 和battle_world_tag做校验
	world_manager * manager = gmatrix::Instance()->FindWorld( battle_world_tag );
	if( !manager || manager->IsIdle())
	{
		__PRINTF( "非法的battle_world_tag=%d\n", battle_world_tag );
		GLog::log( GLOG_INFO, "非法的battle_world_tag=%d\n", battle_world_tag );
		return false;
	}
	if( !manager->IsBattleWorld() )
	{
		__PRINTF( "目标battle_world_tag=%d 不是战场地图\n", battle_world_tag );
		GLog::log( GLOG_INFO, "目标battle_world_tag=%d 不是战场地图\n", battle_world_tag );
		return false;
	}
	if( manager->GetBattleID() != battle_id )
	{
		__PRINTF( "battle_id=%d 与 battle_world_tag=%d 不匹配\n", battle_id, battle_world_tag );
		GLog::log( GLOG_INFO, "battle_id=%d 与 battle_world_tag=%d 不匹配\n", battle_id, battle_world_tag );
		return false;
	}
	// 通过delivery正常途径进入的玩家必须有battle_faction
	if( battle_faction != BF_NONE && battle_faction != BF_ATTACKER && battle_faction != BF_DEFENDER 
			&& battle_faction != BF_VISITOR && battle_faction != BF_MELEE)
	{
		__PRINTF( "阵营不对 battle_faction=%d\n", battle_faction );
		 GLog::log( GLOG_INFO, "阵营不对 battle_faction=%d\n", battle_faction );
		return false;
	}

	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	return pImp->EnterBattleground( battle_world_tag, battle_faction, battle_type );
}


void OnDeliveryConnected()
{
	g_d_state.OnConnect();
	std::vector<battle_field_info> b_info;
	std::vector<instance_field_info> i_info;
	std::vector<territory_field_info> t_info;
	std::vector<raid_world_info> r_info;
	std::vector<kingdom_field_info> k_info;

	size_t i = 0;
	for( i = 0; i < gmatrix::MAX_WORLD_TAG; ++i )
	{
		world_manager* p = gmatrix::Instance()->FindWorld( i );
		if( p && !p->IsIdle())
		{
			p->OnDeliveryConnected(b_info);
			p->OnDeliveryConnected(i_info);
			p->OnDeliveryConnected(t_info);
			p->OnDeliveryConnected(k_info);
		}
	}

	gmatrix::GetRaidWorldInfos(r_info);
	
	// 收集战场信息后进行校验
	// 校验正确发送给delivery注册信息
	// 等delivery更新完了,并且有战场地图数据了这个才会发出去
	if(b_info.size())
	{
		GNET::SendServerRegister(gmatrix::Instance()->GetServerIndex(), &b_info[0], b_info.size()*sizeof(battle_field_info) );
	}
	if(i_info.size())
	{
		GNET::SendInstanceRegister(gmatrix::Instance()->GetServerIndex(), &i_info[0], i_info.size()*sizeof(instance_field_info) );
	}
	if(t_info.size())
	{
		GNET::SendTerritoryRegister(gmatrix::Instance()->GetServerIndex(), &t_info[0], t_info.size()*sizeof(territory_field_info) );
	}
	if(r_info.size())
	{
		GNET::SendRaidServerRegister(gmatrix::Instance()->GetServerIndex(), &r_info[0], r_info.size()*sizeof(raid_world_info) );
	}	
	if(k_info.size())
	{
		GNET::SendKingdomRegister(gmatrix::Instance()->GetServerIndex(), &k_info[0], k_info.size()*sizeof(kingdom_field_info) );
	}
	gmatrix::facbase_field_info & fbase_info = gmatrix::GetFacBaseInfo();
	if (fbase_info.world_tag > 0 && fbase_info.max_instance > 0)
	{
		GNET::SendFacBaseRegister(gmatrix::Instance()->GetServerIndex(), fbase_info.max_instance);
	}

	std::vector<player_template::count_drop_item> items;
	player_template::GetAllCountDrops(items);
	if(items.size())
	{
		GNET::SendGlobalCountRegister(&items[0], items.size() * sizeof(player_template::count_drop_item));
	}
}

void OnDeliveryDisconnect()
{
	g_d_state.OnDisconnect();
	CommonDataMan::GetInstance().OnDeliveryDisConnect();
	gmatrix::WorldManagerOnDisconnect(); //释放多实例 worldmanager 如 副本，帮派基地
	gmatrix::CloseAllFlowBattleManager();
}

void OnDeliveryDisconnect2()
{
	g_d_state.OnDisconnect();
}

static void single_divorce(int role)
{
	int index;
	gplayer *pPlayer = gmatrix::FindPlayer(role,index);
	if(!pPlayer) return ;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != role||!pPlayer->IsActived()||!pPlayer->imp)
	{
		return;
	}
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	if(pImp->IsMarried())
	{
		pImp->SetSpouse(0);
		if(pPlayer->login_state == gplayer::LOGIN_OK 
				&& pImp->_plane) {
			pImp->_runner->player_change_spouse(0);
		}
	}
}

void player_on_divorce(int id1, int id2)
{
	single_divorce(id1);
	single_divorce(id2);
}

void player_cash_notify(int role, int cash_plus_used)
{
	int index;
	gplayer *pPlayer = gmatrix::FindPlayer(role,index);
	if(!pPlayer) return ;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != role||!pPlayer->IsActived()||!pPlayer->imp)
	{
		return;
	}
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->DeliveryNotifyCash(cash_plus_used);
}

namespace
{

class GetCashTotalTask : public abase::ASmallObject , public GDB::Result
{
	gplayer * _player;
	int _userid;
	int _roleid;

public:
	GetCashTotalTask(): _player(NULL), _userid(0), _roleid(0)
	{}

	~GetCashTotalTask()
	{
		_userid = 0;
		_roleid = 0;
		_player = NULL;
	}

	GetCashTotalTask(int userid, int roleid, gplayer *player)
	{
		_userid = userid;
		_roleid = roleid;
		_player = player;
	}

	virtual void OnGetCashTotal(int cash_total, int cash_add2, int userid); 

	virtual void OnTimeOut()
	{
		GLog::log(GLOG_ERR,"用户u=%d, r=%d从数据库取得ct数据超时", _userid, _roleid);
		Failed();
	}
	
	virtual void OnFailed()
	{
		GLog::log(GLOG_ERR,"用户u=%d, r=%d从数据库取得ct数据失败", _userid, _roleid);
		Failed();
	}
	
private:
	GetCashTotalTask(const GetCashTotalTask &rhs);
	GetCashTotalTask & operator =(const GetCashTotalTask &rhs);

	void Failed(int errcode = -1)
	{
		//删除自身
		delete this;
	}
};

void 
GetCashTotalTask::OnGetCashTotal(int cash_total, int cash_add2, int userid)
{
	if(!_player) 
	{
		Failed();
		return ;
	}

	if(userid != _userid)
	{
		GLog::log(GLOG_ERR,"用户u=%d, r=%d. 与回调的用户u=%d不一致,读数据库ct数据", _userid, _roleid, userid);
		Failed();
		return ;
	}

	spin_autolock keeper(_player->spinlock);
	if(_player->ID.id != _roleid || !_player->IsActived() || !_player->imp)
	{
		Failed();
		return;
	}

	gplayer_imp * pImp = (gplayer_imp*)_player->imp;
	if( !(pImp->IsWaitFactionTradeState()) )
	{
		Failed();
		return;
	}

	pImp->DeliveryNotifyCashAdd(cash_total, cash_add2);
	pImp->TradeCashUnLockPlayer();	

	//删除自身
	delete this;
}

}


//元宝变化即时通知，变化后会去get一次cash_total
void player_get_cash_total(int roleid)
{
	int index;
	gplayer *pPlayer = gmatrix::FindPlayer(roleid, index);
	if(!pPlayer) return ;

	//lock
	spin_autolock keeper( pPlayer->spinlock );

	if(pPlayer->ID.id != roleid || !pPlayer->IsActived() || !pPlayer->imp)
	{
		return;
	}
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	int userid = pImp->GetDBMagicNumber();

	if(pImp->TradeCashLockPlayer(0, DBMASK_PUT_SYNC_TIMEOUT) == 0)
	{
		GetCashTotalTask *ptemptask = new GetCashTotalTask(userid, roleid, pPlayer);
		if( NULL == ptemptask || !(GDB::get_cash_total(roleid, ptemptask)) ) 
		{
			//fail
			if(ptemptask) 
			{
				delete ptemptask;
				ptemptask = NULL;
			}
			pImp->TradeCashUnLockPlayer();
		}
	}
}


void faction_battle_start(int attacker, int defender, int end_time)
{
	gmatrix::Instance()->GetBattleCtrl().InsertBattle(attacker,defender,end_time);
}

namespace  
{
class ReadScriptOutput : public ONET::Thread::Runnable
{
	int _fd;
public:
	ReadScriptOutput(int fd):_fd(fd){}
	~ReadScriptOutput()
	{
		if(_fd >= 0) close(_fd);
	}
	

	virtual void Run()
	{
		FILE * file = fdopen(_fd,"r");
		try 
		{
			std::map<int , std::vector<int> > toplist;
			int count;
			char buf[8192];
			if(fgets(buf, sizeof(buf), file) == NULL) throw std::string("-1");
			count = atoi(buf);
			if(count < 0) throw std::string("2");
			if(count == 0 && buf[0] != '0') throw std::string("-2 ") + buf;
			for(int i = 0; i < count; i ++)
			{
				if(fgets(buf, sizeof(buf),file) == NULL) throw std::string("3");
				char * str= strtok(buf," \t\r\n");
				int userid = atoi(buf);
				if(userid <= 0 ) throw std::string("4");
				std::vector<int> a1;
				do
				{
					str = strtok(NULL," \t\r\n");
					if(str == NULL) break;
					a1.push_back(atoi(str));
				}while(1);
				toplist[userid].swap(a1);
			}
			//成功了
			//处理数据
			gmatrix::InitTopTitle(toplist);
			__PRINTF("解析排行榜对应称谓完成\n");
		}
		catch(std::string e)
		{
			__PRINTF("解析出错 %s\n",e.c_str());
			GLog::log(GLOG_ERR,"排行榜转换称谓列表时解析脚本内容出错 %s\n", e.c_str());
		}
		fclose(file);
		_fd = -1;
		delete this;
	}
};
}

void handle_toplist()
{
	int fds_i[2];
	int fds_o[2];
	if(pipe(fds_i)) return;
	if(pipe(fds_o)) return;
	sigset_t sig_set;
	sigemptyset(&sig_set);
	sigaddset(&sig_set,SIGPIPE);
	sigprocmask(SIG_BLOCK,&sig_set,NULL);

	if(fork() == 0)
	{
		if(fork() == 0)
		{
			//要fork两次才对
			//子进程 组织标准输入输出 并且关闭无关文件和连接
			close(fds_i[1]);
			close(fds_o[0]);
			close(0);
			close(1);
			dup2(fds_i[0], 0);
			dup2(fds_o[1], 1);
			for(int i = 3; i < getdtablesize(); i ++)
			{
				close(i);
			}
			const char * str = gmatrix::GetTopListScript();
			execl(str,str,NULL);
			//这里应该报告错误
			fprintf(stdout,"execute error %d, %s\n",errno, strerror(errno));
			fprintf(stderr,"execute error %d, %s\n",errno, strerror(errno));
			_exit(0);
		}
		else
		{
			_exit(0);
		}
	}
	close(fds_i[0]);
	close(fds_o[1]);
	wait(NULL);

	ONET::Thread::Pool::CreateThread(new ReadScriptOutput(fds_o[0]));
	GMSV::TopList2Title(fds_i[1]);

}


void player_change_gs( int localsid, int roleid, int target_gs , int tag, float x, float y, float z, int reason)
{
	__PRINTF( "in function player_change_gs, localsid=%d, roleid=%d, gslineid=%d,tag=%d, pos=(%f,%f,%f)\n" ,
			localsid, roleid, target_gs, tag, x,y,z);
	//这里进行判断	

	int world_index = 0;
	gplayer * pPlayer = gmatrix::FindPlayer( roleid, world_index );
	if( !pPlayer )
	{
		__PRINTF( "用户%d切换gs时不在服务器内, pPlayer is NULL\n", roleid );
		GMSV::SendChangeGsRe( GNET::ERR_CHGS_NOTINSERVER, roleid, localsid, target_gs, tag, x, y, z, reason);
		return;
	}
	spin_autolock alock( pPlayer->spinlock );
	if( gplayer::LOGIN_OK != pPlayer->login_state || NULL == pPlayer->imp || pPlayer->cs_sid != localsid )
	{
		__PRINTF( "用户%d切换gs时状态不为LOGIN_OK\n", roleid );
		GMSV::SendChangeGsRe( GNET::ERR_CHGS_STATUSINVALID, roleid, localsid, target_gs, tag, x, y, z, reason);
		return;
	}
	gplayer_imp* pImp = (gplayer_imp*)pPlayer->imp;
	A3DVECTOR pos(x,y,z);
	//gs取当前位置
	if(tag == -1)
	{
		pImp->GetWorldManager()->GetLogoutPos(pImp,tag,pos);
	}

	//进行转移逻辑，这里可能会不成功
	pImp->ChangePlayerGameServer(target_gs, tag, pos, reason);
}

namespace GMSV
{
void OnSyncFamilyData(int uid, int familyid, const int * list,size_t count)
{
	int world_index;
	gplayer * pPlayer = gmatrix::FindPlayer(uid,world_index);
	if(!pPlayer)
	{
		//用户不在
		return ;
	}
	spin_autolock alock(pPlayer->spinlock);
	if(pPlayer->login_state != gplayer::LOGIN_OK || !pPlayer->pPiece
		|| !pPlayer->IsActived() || pPlayer->ID.id != uid || !pPlayer->imp)
	{
		//错误的状态 离开
		return;
	}
	
	gplayer_imp *pImp = (gplayer_imp *) pPlayer->imp;
	pImp->SetFamilySkill(list, count);
	
}

void RecvTaskAsyncData(int uid, int result, const void * buf, size_t size)
{
	//收到任务数据的回应
	int world_index;
	gplayer * pPlayer = gmatrix::FindPlayer(uid,world_index);
	if(pPlayer)
	{
		spin_autolock keeper(pPlayer->spinlock);
		if(pPlayer->ID.id == uid && pPlayer->IsActived() && 
				pPlayer->login_state == gplayer::LOGIN_OK && pPlayer->imp)
		{
			//这是是否还要根据player_imp的其他状态作出一些判定...
			PlayerTaskInterface task_if(((gplayer_imp*)pPlayer->imp));
			OnTaskReceiveFamilyData(&task_if,buf,size,result);
		}
	}
}


void RecvCertChallengeResult(int result, const void *response, size_t size)
{
	__PRINTF ("收到CertChallengeResult结果，尚未实现的函数.\n");
}

void OnSyncFacMultiExp(int roleid, int multi, int end_time)
{
	int world_index;
	gplayer * pPlayer = gmatrix::FindPlayer(roleid, world_index);
	if (!pPlayer)
	{
		//用户不在
		return ;
	}
	spin_autolock alock(pPlayer->spinlock);
	if(pPlayer->login_state != gplayer::LOGIN_OK || !pPlayer->pPiece
		|| !pPlayer->IsActived() || pPlayer->ID.id != roleid || !pPlayer->imp)
	{
		//错误的状态 离开
		return;
	}
	gplayer_imp * pImp = (gplayer_imp *) pPlayer->imp;
	pImp->SetFacMultiExp(multi, end_time);
}
}

void family_exp_delivery(int uid, int family_id, int64_t exp)
{
	//受到家族分享经验
	int world_index;
	gplayer * pPlayer = gmatrix::FindPlayer(uid,world_index);
	if(pPlayer)
	{
		spin_autolock keeper(pPlayer->spinlock);
		if(pPlayer->ID.id == uid && pPlayer->IsActived() && 
				pPlayer->login_state == gplayer::LOGIN_OK && pPlayer->imp)
		{
			gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
			pImp->ReceiveShareExp(exp);
		}
	}
}


void report_cheater(int roleid, int cheattype, const void *cheatinfo, size_t size)
{
	int index;
	gplayer * pPlayer = gmatrix::FindPlayer( roleid, index );
	if(!pPlayer) return ;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != roleid||!pPlayer->IsActived()||!pPlayer->imp || pPlayer->login_state != gplayer::LOGIN_OK)
	{
		return;
	}
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->FindCheater(cheattype,true);
}

void wallow_control(int userid, int rate, int msg, std::map<int,int> & data)
{
	__PRINTF("recv player %d wallow control %d\n", userid, rate);
	int index;
	gplayer * pPlayer = gmatrix::FindPlayer( userid, index );
	if(!pPlayer) return ;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != userid ||!pPlayer->IsActived()||!pPlayer->imp || pPlayer->login_state != gplayer::LOGIN_OK)
	{
		return;
	}
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->WallowControl(rate, msg, data[1], data[2], data[3]);
	
}

void acquestion_ret(int userid, int ret) // ret: 0 正确, 1 错误, 2 超时
{
	if(ret != 0) return ;//正确才处理
	int index;
	gplayer * pPlayer = gmatrix::FindPlayer( userid, index );
	if(!pPlayer) return ;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != userid ||!pPlayer->IsActived()||!pPlayer->imp || pPlayer->login_state != gplayer::LOGIN_OK)
	{
		return;
	}
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->QuestionBonus();
}

void sync_faction_hostiles_info( unsigned int faction_id, std::vector<unsigned int>& hostiles )
{
	__PRINTF( "sync 敌对帮派数据, faction_id=%d", faction_id );
	for( std::vector<unsigned int>::iterator it = hostiles.begin(); it != hostiles.end(); ++ it )
	{
		gmatrix::Instance()->GetFHManager().SyncFactionHostile( faction_id, *it );
		__PRINTF( ", %d", *it );
	}
	__PRINTF( "\n" );
}

void add_faction_hostile( unsigned int faction_id, unsigned int hostile )
{
	__PRINTF( "add 敌对帮派数据, faction_id=%d, hostile=%d\n", faction_id, hostile );
	gmatrix::Instance()->GetFHManager().AddFactionHostile( faction_id, hostile );
}

void del_faction_hostile( unsigned int faction_id, unsigned int hostile )
{
	__PRINTF( "del 敌对帮派数据, faction_id=%d, hostile=%d\n", faction_id, hostile );
	gmatrix::Instance()->GetFHManager().DelFactionHostile( faction_id, hostile );
}

void battlefield_start(int battle_id,int world_tag,int attacker,int defender,int attacker_assistant,int end_timestamp,bool force)
{
	int last_time = end_timestamp - (int)g_timer.get_systime();
	GLog::log(GLOG_INFO,"城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d)开启请求(attacker= %d,defender= %d,attacker_assistant= %d,last_time= %d,force: %d)",
		gmatrix::GetServerIndex(),battle_id,world_tag,attacker,defender,attacker_assistant,last_time,force);
	__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d)开启请求(attacker= %d,defender= %d,attacker_assistant= %d,last_time= %d,force: %d)\n",
		gmatrix::GetServerIndex(),battle_id,world_tag,attacker,defender,attacker_assistant,last_time,force);
	if(last_time < 600)
	{
		GLog::log(GLOG_INFO,"城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d)开启失败(原因: 持续时间过短 last_time: %d)",
			gmatrix::GetServerIndex(),battle_id,world_tag,last_time);
		__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d)开启失败(原因: 持续时间过短 last_time: %d)\n",
			gmatrix::GetServerIndex(),battle_id,world_tag,last_time);
		GNET::SendBattleFieldStartRe(gmatrix::GetServerIndex(),battle_id,world_tag,bf_world_manager_base::CBFR_LAST_TIME_TOO_SHORT);
		return;
	}
	bf_param param;
	memset(&param,0,sizeof(param));
	param.battle_id = battle_id;
	param.world_tag = world_tag;
	param.attacker_mafia_id = attacker;
	param.defender_mafia_id = defender;
	param.attacker_assistant_mafia_id = attacker_assistant;
	param.end_timestamp = end_timestamp;
	world_manager* manager = gmatrix::FindWorld(world_tag);
	if(!manager || manager->IsIdle())
	{
		GLog::log(GLOG_INFO,"城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d)开启失败(原因: 非法的world_tag)",
			gmatrix::GetServerIndex(),battle_id,world_tag);
		__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d)开启失败(原因: 非法的world_tag)\n",
			gmatrix::GetServerIndex(),battle_id,world_tag);
		GNET::SendBattleFieldStartRe(gmatrix::GetServerIndex(),battle_id,world_tag,bf_world_manager_base::CBFR_INVALID_WORLD_TAG);
		return;
	}       
	if(!gmatrix::IsBattleFieldServer())
	{
		GLog::log(GLOG_INFO,"城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d)开启失败(原因: 服务器不是战场服务器)",
			gmatrix::GetServerIndex(),battle_id,world_tag);
		__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d)开启失败(原因: 服务器不是战场服务器)\n",
			gmatrix::GetServerIndex(),battle_id,world_tag);
		GNET::SendBattleFieldStartRe(gmatrix::GetServerIndex(),battle_id,world_tag,bf_world_manager_base::CBFR_NOT_BATTLE_SERVER);
		return;
	}
	int rst = manager->CreateBattleField(param,force);
	if(0 != rst)
	{
		//失败了直接返回结果,成功的话需要等待进一步处理,从数据库拿东西
		GLog::log(GLOG_INFO,"城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d)开启失败(ret_code: %d)",
			gmatrix::GetServerIndex(),battle_id,world_tag,rst);
		__PRINTF("城战信息:   城战(gs_id: %d,battle_id: %d,world_tag: %d)开启失败(ret_code: %d)\n",
			gmatrix::GetServerIndex(),battle_id,world_tag,rst);
		GNET::SendBattleFieldStartRe(gmatrix::GetServerIndex(),battle_id,world_tag,rst);
	}
}

void sect_add_member(int master, int disciple)
{
	int index;
	gplayer * pPlayer = gmatrix::FindPlayer( disciple, index );
	if(!pPlayer) return ;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != disciple ||!pPlayer->IsActived()||!pPlayer->imp || pPlayer->login_state != gplayer::LOGIN_OK)
	{
		return;
	}
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->UpdateSectID(master);	//对于师傅，也会收到这个协议，只是master == disciple
}

void sect_del_member(int master, int disciple)
{
	if(master == disciple) return;
	int index;
	gplayer * pPlayer = gmatrix::FindPlayer( disciple, index );
	if(!pPlayer) return ;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != disciple ||!pPlayer->IsActived()||!pPlayer->imp || pPlayer->login_state != gplayer::LOGIN_OK)
	{
		return;
	}
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->SetSectID(0);
	if(pPlayer->pPiece) pImp->_runner->sect_become_disciple(0);
}

void  sect_update_skill(int roleid, const int *skill_list, int count)
{
	int index;
	gplayer * pPlayer = gmatrix::FindPlayer( roleid, index );
	if(!pPlayer) return ;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != roleid ||!pPlayer->IsActived()||!pPlayer->imp || pPlayer->login_state != gplayer::LOGIN_OK)
	{
		return;
	}
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->_skill.SetSectSkill((int*)skill_list, count,pImp);
}


void player_contest_result(int role_id,int score,int place,int right_amount)
{
	__PRINTF("recv player contest result (roleid: %d score: %d place: %d right_amount: %d\n",role_id,score,place,right_amount);
	int index;
	gplayer* pPlayer = gmatrix::FindPlayer(role_id,index);
	if(!pPlayer) return ;
	spin_autolock keeper(pPlayer->spinlock);
	if(pPlayer->ID.id != role_id ||!pPlayer->IsActived()||!pPlayer->imp || pPlayer->login_state != gplayer::LOGIN_OK)
	{
		return;
	}
	gplayer_imp* pImp = (gplayer_imp*)pPlayer->imp;
	pImp->PlayerContestResult(score,place,right_amount);
}

void handle_message_result(int role_id, char message_type, char op_type, int charm)
{
	int index;
	gplayer* pPlayer = gmatrix::FindPlayer(role_id, index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);
	
	if(pPlayer->ID.id != role_id ||!pPlayer->IsActived()||!pPlayer->imp || pPlayer->login_state != gplayer::LOGIN_OK)
	{
		return;
	}
	gplayer_imp* pImp = (gplayer_imp*)pPlayer->imp;
	pImp->PlayerHandleMessageResult(message_type, op_type, charm);
}


bool player_billing_approved(int role_id, int itemid, int itemnum, int expire_time, int total_cash, int order_count)
{
	int index;
	gplayer* pPlayer = gmatrix::FindPlayer(role_id, index);
	if(!pPlayer) return false;
	spin_autolock keeper(pPlayer->spinlock);
	
	if(pPlayer->ID.id != role_id ||!pPlayer->IsActived()||!pPlayer->imp || pPlayer->login_state != gplayer::LOGIN_OK)
	{
		return false;
	}
	gplayer_imp* pImp = (gplayer_imp*)pPlayer->imp;
	return pImp->PlayerDoBillingShoppingStep2(itemid, itemnum , expire_time, total_cash, order_count);
}

void player_billing_cash_notify(int role_id, int total_cash)
{
	int index;
	gplayer *pPlayer = gmatrix::FindPlayer(role_id,index);
	if(!pPlayer) return ;
	spin_autolock keeper(pPlayer->spinlock);
	
	if(pPlayer->ID.id != role_id ||!pPlayer->IsActived()||!pPlayer->imp || pPlayer->login_state != gplayer::LOGIN_OK)
	{
		return;
	}
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->DeliveryNotifyBillingCash(total_cash);
}

void ref_add_bonus(int role_id, int inc)
{
	int index;
	gplayer* pPlayer = gmatrix::FindPlayer(role_id, index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);
	
	if(pPlayer->ID.id != role_id ||!pPlayer->IsActived()||!pPlayer->imp || pPlayer->login_state != gplayer::LOGIN_OK)
	{
		return;
	}
	
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->RefAddBonus(inc); //线上活动返回的鸿利	
	pImp->_runner->player_bonus(pImp->BonusInfo().GetBonus());
}

void ref_add_exp(int role_id, int64_t inc)
{
	int index;
	gplayer* pPlayer = gmatrix::FindPlayer(role_id, index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);
	
	if(pPlayer->ID.id != role_id ||!pPlayer->IsActived()||!pPlayer->imp || pPlayer->login_state != gplayer::LOGIN_OK)
	{
		return;
	}
	
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->RefAddExp(inc); //线上活动返回的经验	
}

void player_change_style(int role_id, unsigned char faceid, unsigned char hairid, unsigned char earid, unsigned char tailid, unsigned char fashionid)
{
	int index;
	gplayer* pPlayer = gmatrix::FindPlayer(role_id, index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);
	
	if(pPlayer->ID.id != role_id ||!pPlayer->IsActived()||!pPlayer->imp || pPlayer->login_state != gplayer::LOGIN_OK)
	{
		return;
	}
	
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->HandleChangeStyle(faceid, hairid, earid, tailid, fashionid);	
}

void handle_netbar_reward(int role_id, int netbar_level, int award_type)
{
	int index;
	gplayer* pPlayer = gmatrix::FindPlayer(role_id, index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);
	
	if(pPlayer->ID.id != role_id ||!pPlayer->IsActived()||!pPlayer->imp || pPlayer->login_state != gplayer::LOGIN_OK)
	{
		return;
	}
	
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->HandleNetbarReward(netbar_level, award_type);	
}

static int mall_lock = 0;
void change_shop_sale(unsigned int new_scheme)
{
	spin_autolock keeper(mall_lock);
	g_config.mall_prop = g_config.mall_prop & 0xFF00FFFF | ((new_scheme << 16) & 0x00FF0000);

}

void change_shop_discount(unsigned int new_discount)
{
	spin_autolock keeper(mall_lock);
	g_config.mall_prop = g_config.mall_prop & 0x00FFFFFF | ((new_discount<< 24) & 0xFF000000);
}

void sync_territory_owner(int territoryid, int territory_type, unsigned int owner)
{
	gmatrix::SyncTerritoryOwner(territoryid, territory_type, owner);
}

void territory_item_get(int retcode, int roleid, int itemid, int itemcount, int money)
{
	int index;
	gplayer* pPlayer = gmatrix::FindPlayer(roleid, index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);
	
	if(pPlayer->ID.id != roleid ||!pPlayer->IsActived()||!pPlayer->imp || pPlayer->login_state != gplayer::LOGIN_OK)
	{
		return;
	}
	
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->TerritoryItemGet(retcode, itemid, itemcount, money);	
}


void unique_bid_result(int retcode, int roleid, int money_upperbound, int money_lowerbound) 
{
	int index;
	gplayer* pPlayer = gmatrix::FindPlayer(roleid, index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);
	
	if(pPlayer->ID.id != roleid ||!pPlayer->IsActived()||!pPlayer->imp || pPlayer->login_state != gplayer::LOGIN_OK)
	{
		return;
	}
	
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->PlayerUniqueBidResult(retcode, money_upperbound, money_lowerbound); 
}	

void unique_bid_reward(int retcode, int roleid, int itemid)
{
	int index;
	gplayer* pPlayer = gmatrix::FindPlayer(roleid, index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);
	
	if(pPlayer->ID.id != roleid ||!pPlayer->IsActived()||!pPlayer->imp || pPlayer->login_state != gplayer::LOGIN_OK)
	{
		return;
	}
	
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->PlayerUniqueBidReward(retcode, itemid); 
}

void update_circle_info(int roleid, unsigned int circle_id, unsigned char circle_mask)
{
	int index;
	gplayer* pPlayer = gmatrix::FindPlayer(roleid, index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);
	
	if(pPlayer->ID.id != roleid ||!pPlayer->IsActived()||!pPlayer->imp || pPlayer->login_state != gplayer::LOGIN_OK)
	{
		return;
	}
	
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->UpdateCircleInfo(circle_id, circle_mask);
}

void recv_deliveryd_exp(int roleid, int64_t bonus_exp, unsigned int award_type)
{
	int index;
	gplayer* pPlayer = gmatrix::FindPlayer(roleid, index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);
	
	if(pPlayer->ID.id != roleid ||!pPlayer->IsActived()||!pPlayer->imp || pPlayer->login_state != gplayer::LOGIN_OK)
	{
		return;
	}
	
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->ReceiveDeliverydExp(bonus_exp, award_type);
}

void recv_deliveryd_item(int roleid, unsigned int award_type, unsigned int item_id)
{
	int index;
	gplayer* pPlayer = gmatrix::FindPlayer(roleid, index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);
	
	if(pPlayer->ID.id != roleid ||!pPlayer->IsActived()||!pPlayer->imp || pPlayer->login_state != gplayer::LOGIN_OK)
	{
		return;
	}
	
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->ReceiveDeliverydItem(award_type, item_id);
}

//获取delivery发过来的vip信息
void recv_vipaward_info_notify(int roleid, int cur_time, int start_time, int end_time, int vipaward_level, int recent_total_cash)
{
	int index;
	gplayer* pPlayer = gmatrix::FindPlayer(roleid, index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);
	
	if(pPlayer->ID.id != roleid ||!pPlayer->IsActived()||!pPlayer->imp || pPlayer->login_state != gplayer::LOGIN_OK)
	{
		return;
	}

	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->ReceiveVipAwardInfoNotify(cur_time, start_time, end_time, vipaward_level, recent_total_cash);
}

void recv_friend_num(int roleid, int taskid, int number)
{
	int index;
	gplayer* pPlayer = gmatrix::FindPlayer(roleid, index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);
	
	if(pPlayer->ID.id != roleid ||!pPlayer->IsActived()||!pPlayer->imp || pPlayer->login_state != gplayer::LOGIN_OK)
	{
		return;
	}
	
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->ReceiveFriendNum(taskid, number);
}

void recv_circle_async_data(int uid, int result, const void * buf, size_t size)
{
	//收到任务数据的回应
	int world_index;
	gplayer * pPlayer = gmatrix::FindPlayer(uid,world_index);
	if(pPlayer)
	{
		spin_autolock keeper(pPlayer->spinlock);
		if(pPlayer->ID.id == uid && pPlayer->IsActived() && 
				pPlayer->login_state == gplayer::LOGIN_OK && pPlayer->imp)
		{
			//这是是否还要根据player_imp的其他状态作出一些判定...
			PlayerTaskInterface task_if(((gplayer_imp*)pPlayer->imp));
			OnTaskReceiveCircleGroupData(&task_if,buf,size,result);
		}
	}
}

void player_change_ds(int roleid, char flag)
{
	int index;
	gplayer* pPlayer = gmatrix::FindPlayer(roleid, index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);
	
	if(pPlayer->ID.id != roleid ||!pPlayer->IsActived()||!pPlayer->imp || pPlayer->login_state != gplayer::LOGIN_OK)
	{
		return;
	}
	
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->PlayerChangeDS(flag);
}

void consign_end(int roleid, char need_read)
{
	ASSERT(roleid > 0);
	if(roleid <= 0) return;
	
	int index;
	gplayer *pPlayer = gmatrix::FindPlayer(roleid,index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != roleid || !pPlayer->IsActived() || !pPlayer->imp)  
	{
		return;
	}
	pPlayer->imp->CompleteConsign(0,need_read);
	printf("Consign End: need_read:%s\n", need_read ? "true" : "false");
}

//角色寄售delivery检查失败时会收到这个协议
void consign_end_role(int roleid, int retcode)
{
	ASSERT(roleid > 0);
	if(roleid <= 0) return;
	
	int index;
	gplayer *pPlayer = gmatrix::FindPlayer(roleid,index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != roleid || !pPlayer->IsActived() || !pPlayer->imp)  
	{
		return;
	}

	//清除player身上的角色寄售标志
	pPlayer->imp->SetConsignRoleFlag(false);
	GLog::log(GLOG_INFO,"玩家角色寄售错误 用户%d delivery_retcode=%d", roleid, retcode);
	__PRINTF("Consign End Role: retcode=%d\n", retcode);
}

//副本Delivery发送的接口, add by Houjun 2011-7-14
void raid_start(int map_id, int raidroom_id, const std::vector<int>& roleid_list, int raid_type, const void* buf, size_t size, char can_vote, char difficulty)
{
	__PRINTF("Delivery发送的副本%d创建消息, raidroom_id=%d, roleid_list size=%d\n, ", map_id, raidroom_id, (int)roleid_list.size());
	int rst = -1;
	int world_tag = -1;
	if((rst = gmatrix::CreateRaidWorldManager(map_id, raidroom_id, roleid_list, world_tag, buf, size, difficulty)))
	{
		__PRINTF("副本%d创建失败, %d\n, ", map_id, rst);
	}
	else
	{
		printf("副本%d创建成功, world_tag=%d\n", map_id, world_tag);
	}
	GNET::SendRaidStartRe(rst, gmatrix::GetServerIndex(), map_id, raidroom_id, world_tag);
}

void raid_close(int room_id, int map_id, int index, int reason)
{
	__PRINTF("Delivery发送的副本%d关闭消息, room_id=%d, world_tag=%d, 原因:%d\n", map_id, room_id, index, reason);
	if(gmatrix::ReleaseRaidWorldManager(index))
	{
		__PRINTF("副本%d实例world_tag=%d关闭失败，原因没有找到世界管理器\n", map_id, index);
	}
}

//原GS在传送前进行最后的检查，是否满足进副本的要求
void raid_enter(int role_id, int map_id, int raid_template_id, int index, int raid_type, const void* buf, size_t size)
{
	int player_index;
	gplayer *pPlayer = gmatrix::FindPlayer(role_id,player_index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != role_id || !pPlayer->IsActived() || !pPlayer->imp)  
	{
		return;
	}
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	GNET::SendRaidEnterRe(role_id, gmatrix::GetServerIndex(), pImp->CheckEnterRaid(map_id, raid_template_id, raid_type, buf, size) ? 0 : -1, map_id, index, raid_type);
}

void raid_member_change(int role_id, int gs_id, int map_id, int index, unsigned char operation)
{
}

void raid_kickout(int role_id, int index)
{
	int player_index;
	gplayer *pPlayer = gmatrix::FindPlayer(role_id,player_index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != role_id || !pPlayer->IsActived() || !pPlayer->imp)  
	{
		return;
	}
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->PlayerLeaveRaid(CHG_GS_LEAVE_BY_VOTE);
}
//Add end

void change_name_response(int role_id, char * name, size_t name_len)
{
	int player_index;
	gplayer *pPlayer = gmatrix::FindPlayer(role_id,player_index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != role_id || !pPlayer->IsActived() || !pPlayer->imp)  
	{
		return;
	}
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->PlayerChangeName(name, name_len);
}

void change_faction_name(int role_id, int fid, char type, char * name, size_t name_len)
{
	int player_index;
	gplayer *pPlayer = gmatrix::FindPlayer(role_id,player_index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != role_id || !pPlayer->IsActived() || !pPlayer->imp)  
	{
		return;
	}
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->PlayerChangeFactionName(role_id, fid, type, name, name_len);
}

void battle_flag_buff_scope(const std::vector<int>& role_list, int flagid, int worldtag, int remaintime)
{
	for(size_t i = 0; i < role_list.size(); i ++)
	{
		int role_id = role_list[i];
		int player_index;
		gplayer *pPlayer = gmatrix::FindPlayer(role_id,player_index);
		if(!pPlayer) continue;
		spin_autolock keeper(pPlayer->spinlock);

		if(pPlayer->ID.id != role_id || !pPlayer->IsActived() || !pPlayer->imp)  
		{
			continue;
		}
		gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
		if(pImp->GetWorldManager()->GetWorldTag() != worldtag)
		{
			continue;
		}
		pImp->AddBattleFlag(flagid, remaintime);
	}
}

void get_coupons_req(int userid, int toaid, int tozoneid, int roleid, std::vector<int> & task_ids)
{
	int player_index;
	gplayer *pPlayer = gmatrix::FindPlayer(roleid,player_index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != roleid || !pPlayer->IsActived() || !pPlayer->imp)  
	{
		return;
	}
	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->PlayerGetCouponsInfo(userid, toaid, tozoneid, roleid, task_ids);
}

void OnRoleEnterVoiceChannel(long roleid) 
{
	int player_index;
	gplayer *pPlayer = gmatrix::FindPlayer((int)roleid, player_index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != (int)roleid || !pPlayer->IsActived() || !pPlayer->imp)  
		return;

	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->HandleAddGTReward();
}

void OnRoleLeaveVoiceChannel(long roleid)
{
	int player_index;
	gplayer *pPlayer = gmatrix::FindPlayer((int)roleid, player_index);
	if(!pPlayer) return;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != (int)roleid || !pPlayer->IsActived() || !pPlayer->imp)  
		return;

	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->HandleRemoveGTReward();
}

// 发放奖励
bool OnFlowerSendAward(int roleid, int award_type, int award_id) 
{
	int player_index;
	gplayer *pPlayer = gmatrix::FindPlayer((int)roleid, player_index);
	if(!pPlayer) return false;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != (int)roleid || !pPlayer->IsActived() || !pPlayer->imp)  
		return false;

	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	return pImp->FlowerSendAwardByDelivery(award_type, award_id);
}
// 扣除鲜花
bool OnFlowerTakeoff(int roleid, int count)
{
	int player_index;
	gplayer *pPlayer = gmatrix::FindPlayer((int)roleid, player_index);
	if(!pPlayer) return false;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != (int)roleid || !pPlayer->IsActived() || !pPlayer->imp)  
		return false;

	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	return pImp->FlowerTakeoffByDelivery(count);
}
// 激活控制器 
void OnFlowerActiveCtrl(int ctrl_id)
{
	gmatrix::TriggerSpawn(ctrl_id, true);
}

//亲卫传送(普通地图)
bool guard_call_by_king(int roleid, int lineid, int map_id, float pos_x, float pos_y, float pos_z)
{
	int player_index;
	gplayer *pPlayer = gmatrix::FindPlayer((int)roleid, player_index);
	if(!pPlayer) return false;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != (int)roleid || !pPlayer->IsActived() || !pPlayer->imp)  
		return false;

	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	return pImp->GuardCallByKing(roleid, lineid, map_id, pos_x, pos_y, pos_z);
}

//亲卫传送(国王战地图和帮战副本地图)
bool king_guard_invite_reply(int roleid, int lineid, int map_id, float pos_x, float pos_y, float pos_z)
{
	int player_index;
	gplayer *pPlayer = gmatrix::FindPlayer((int)roleid, player_index);
	if(!pPlayer) return false;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != (int)roleid || !pPlayer->IsActived() || !pPlayer->imp)  
		return false;

	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	return pImp->KingCallGuardReply(lineid, map_id, pos_x, pos_y, pos_z);
}

bool touch_point_exchange(int retcode, int roleid, unsigned int flag, unsigned int count)
{
	int player_index;
	gplayer *pPlayer = gmatrix::FindPlayer((int)roleid, player_index);
	if(!pPlayer) return false;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != (int)roleid || !pPlayer->IsActived() || !pPlayer->imp)  
		return false;

	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	return pImp->PlayerExchangeTouchPoint(retcode, flag, count);
}

void queen_open_bath()
{
	gmatrix::OpenBath();
}

void queen_close_bath()
{
	gmatrix::CloseBath();
}

void sync_bath_count(int roleid, int count)
{
	gmatrix::SyncBathCount(roleid, count);
}

void king_deliver_task_response(int retcode, int roleid, int task_type)
{
	int player_index;
	gplayer *pPlayer = gmatrix::FindPlayer((int)roleid, player_index);
	if(!pPlayer) return ;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != (int)roleid || !pPlayer->IsActived() || !pPlayer->imp)  
		return ;

	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->KingDeliverTaskResponse(retcode, task_type); 
}

void flow_battle_clear_leave(int world_tag, int role_id, char faction)
{
	world_manager* manager = gmatrix::Instance()->FindWorld( world_tag );
	if( !manager || manager->IsIdle() || !manager->IsBattleWorld() || !manager->IsFlowBattle())
	{
		__PRINTF( "无法找到流水席战场清除玩家%d, faction=%d，非法的world_tag=%d\n", role_id, faction, world_tag);
		GLog::log( GLOG_INFO, "无法找到流水席战场清除玩家%d, faction=%d，非法的world_tag=%d", role_id, faction, world_tag);
		return;
	}
	manager->FlowBattlePlayerLeave(role_id, faction);
}

void close_flow_battle(int gs_id, int map_id)
{
	world_manager* manager = gmatrix::Instance()->FindWorld(map_id);
	if( !manager || manager->IsIdle() || !manager->IsBattleWorld() || !manager->IsFlowBattle())
	{
		__PRINTF( "战场关闭失败，原因：非法的worldtag, battle_world_tag=%d\n", map_id );
		GLog::log(GLOG_INFO, "战场关闭失败，原因：非法的worldtag, battle_world_tag=%d", map_id );
		return;
	}
	__PRINTF( "流水席战场关闭, gs_id=%d, flow_battle_world_tag=%d\n", gs_id, map_id );
	GLog::log(GLOG_INFO, "流水席战场关闭, gs_id=%d, flow_battle_world_tag=%d", gs_id, map_id );
	bg_world_manager* bgManager = (bg_world_manager*)manager;
	bgManager->ForceClose();
	return;
}

void forbid_sync(int type, std::vector<int> & ids)
{
	forbid_manager::SyncForbidInfo(type, ids);
}


void get_cash_avail(int roleid, int & cash_avail)
{
	int player_index;
	gplayer *pPlayer = gmatrix::FindPlayer((int)roleid, player_index);
	if(!pPlayer) return ;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != (int)roleid || !pPlayer->IsActived() || !pPlayer->imp)  
		return ;

	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	cash_avail = pImp->GetCashAvail();
}


void add_web_order(int roleid, std::vector<GMSV::weborder> & list)
{
	int player_index;
	gplayer *pPlayer = gmatrix::FindPlayer((int)roleid, player_index);
	if(!pPlayer) return ;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != (int)roleid || !pPlayer->IsActived() || !pPlayer->imp)  
		return ;

	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->AddWebOrder(list);
}

void facbase_start(int fid)
{
	__PRINTF("Delivery发送的基地开启消息 fid=%d\n", fid);
	int rst = -1;
	if((rst = gmatrix::CreateFacBaseManager(fid)))
	{
		__PRINTF("帮派 %d 基地创建失败, rst %d\n, ", fid, rst);
		GNET::SendFacBaseStartRe(rst, fid, gmatrix::GetServerIndex(), 0, std::set<int>());
	}
}

void king_get_reward_response(int retcode, int roleid)
{
	int player_index;
	gplayer *pPlayer = gmatrix::FindPlayer((int)roleid, player_index);
	if(!pPlayer) return ;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != (int)roleid || !pPlayer->IsActived() || !pPlayer->imp)  
		return ;

	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->KingGetRewardResponse(retcode);
}

void facbase_prop_change(int noti_roleid, int base_tag, int fid, int prop_type, int delta)
{
	world_manager * manager = gmatrix::Instance()->FindWorld(base_tag);
	if( !manager || manager->IsIdle())
	{
		GLog::log(GLOG_ERR, "基地属性同步修改失败 基地 tag %d 非法", base_tag);
		return;
	}
	manager->OnFacBasePropChange(noti_roleid, fid, prop_type, delta);
}

void facbase_building_progress(int base_tag, int fid, int task_id, int value)
{
	world_manager * manager = gmatrix::Instance()->FindWorld(base_tag);
	if( !manager || manager->IsIdle())
	{
		GLog::log(GLOG_ERR, "基地建筑建设进度增加 基地 tag %d 非法", base_tag);
		return;
	}
	manager->OnFacBaseBuildingProgress(fid, task_id, value);
}

void facbase_task_put_auction(int base_tag, int roleid, char * rolename, int name_len, int itemid)
{
	world_manager * manager = gmatrix::Instance()->FindWorld(base_tag);
	if( !manager || manager->IsIdle())
	{
		GLog::log(GLOG_ERR, "facbase_task_put_auction, tag %d invalid", base_tag);
		return;
	}
	manager->FacBaseTaskPutAuction(roleid, name_len, rolename, itemid);
}

void get_cs6v6_info(int roleid, int role_score, int team_score, int team_rank, int award_timestamp, int award_type, int award_level)
{
	int player_index;
	gplayer *pPlayer = gmatrix::FindPlayer((int)roleid, player_index);
	if(!pPlayer) return ;
	spin_autolock keeper(pPlayer->spinlock);

	if(pPlayer->ID.id != (int)roleid || !pPlayer->IsActived() || !pPlayer->imp)  
		return ;

	gplayer_imp * pImp = (gplayer_imp*)pPlayer->imp;
	pImp->PlayerGetCS6V6Award(award_type, award_level, role_score, team_score, team_rank, award_timestamp);
}

// Youshuang add
void facbase_mall_change( int fid, const std::set<int>& mall_indexes )
{
	GLog::log(GLOG_INFO, "收到基地商城变化消息 fid %d size %d", fid, mall_indexes.size());
	__PRINTF("收到基地商城变化消息 fid %d size %d\n", fid, mall_indexes.size());

	gmatrix::ModFacbaseMall( fid, mall_indexes );
}

void facbase_mall_sync( const std::map<int, std::set<int> >& malls )
{
	GLog::log(GLOG_INFO, "收到基地商城同步消息 malls.size %d", malls.size());
	__PRINTF("收到基地商城同步消息 malls.size %d\n", malls.size());

	for( std::map< int, std::set<int> >::const_iterator iter = malls.begin(); iter != malls.end(); iter++ )
	{
		gmatrix::ModFacbaseMall( iter->first, iter->second );
	}
}
// end
//此处的cmd命令是client经由deliver转发过来的
void handle_facbase_cmd(int linkid, int localsid,int roleid, int base_tag, const void * buf, size_t size)
{
	if(!g_d_state.CanLogin())
	{
		//当前不允许万家登录，也自然不允许用户做任何操作
		return;
	}
	if(size < sizeof(C2S::cmd_header))
	{
		//用户数据大小有错误的
		//GLog::log(GLOG_WARNING,"用户%d发来错误的数据",uid);
		//GMSV::ReportCheater2Gacd(uid, 777 , NULL,0);
		//GMSV::SendDisconnect(cs_index, uid, sid,0);
		return ;
	}
	world_manager * manager = gmatrix::Instance()->FindWorld(base_tag);
	if( !manager || manager->IsIdle())
	{
		GLog::log(GLOG_ERR, "handle_facbase_cmd 基地 tag %d", base_tag);
		return;
	}
	manager->HandleFBaseClientCMD(roleid, buf, size, linkid, localsid);
}
//此cmd命令是普通gs经由deliver转发过来的
void handle_facbase_deliver_cmd(int roleid, int base_tag, const void * buf, size_t size)
{
	if(!g_d_state.CanLogin())
		return;
	if(size < sizeof(C2S::cmd_header))
		return ;
	world_manager * manager = gmatrix::Instance()->FindWorld(base_tag);
	if( !manager || manager->IsIdle())
	{
		GLog::log(GLOG_ERR, "handle_facbase_deliver_cmd 基地 tag %d", base_tag);
		return;
	}
	manager->HandleFBaseDeliverCMD(roleid, buf, size);
}

void facbase_stop(int fid, int base_tag)
{
	__PRINTF("Delivery发送的基地关闭消息 fid=%d tag=%d\n", fid, base_tag);
	world_manager * manager = gmatrix::Instance()->FindWorld(base_tag);
	if( !manager/* || manager->IsIdle()*/)
	{
		GLog::log(GLOG_ERR, "facbase_stop tag %d invalid", base_tag);
		return;
	}
	manager->OnFacBaseStop(fid);
}

