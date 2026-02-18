#include "world.h"
#include "playerteam.h"
#include "player_imp.h"
#include <common/protocol.h>
#include <arandomgen.h>
#include <map>
#include <algorithm>
#include "playertemplate.h"
#include "task/taskman.h"
#include <gsp_if.h>
#include "global_drop.h"
#include "usermsg.h"

int player_team::team_t::team_gt_id_seed = 0; 
int player_team::team_t::team_gt_id_lock = 0; 
team_cache team_cache::instance;

void OnGTConnected() 
{
	team_cache::instance.SyncToGT();	
}

void team_cache::UpdateTeam(int team_id, const team_cache_entry& entry)
{
	team_cache_entry old;
	old.team_size = -1;
	{ // 锁保护，更新
		spin_autolock keeper(lock);

		std::map<int, team_cache_entry>::iterator it = teams.find(team_id);
		if (it != teams.end()) {
			old = it->second;
			teams.erase(it);
		}
		teams.insert(std::make_pair(team_id, entry));
	}

	int64_t	gt_team_id = MakeGTTeamId(team_id);  
	// 通知gt
	if (old.team_size <= 0) { // 新创建通告
		GMSV::gt_team_info info;
		info.team_id = gt_team_id;
		info.captain = entry.leader_id;	
		copy(entry.members, entry.members + entry.team_size, back_inserter(info.members));
		GMSV::SendGTTeamCreate(info);	
	} else { // 更新通告
		if (old.leader_id != entry.leader_id) { // 队长变化
			std::vector<int> members;
			members.push_back(entry.leader_id);
			GMSV::SendGTTeamMemberUpdate(gt_team_id, members, (char)2); // 2: team_cap		
		}	
		
		std::set<int> old_members;
		std::set<int> cur_members;
		copy(old.members, old.members + old.team_size, inserter(old_members, old_members.begin()));
		copy(entry.members, entry.members + entry.team_size, inserter(cur_members, cur_members.begin()));
		std::vector<int> added_members;
		std::vector<int> leaved_members;
		set_difference(old_members.begin(), old_members.end(), cur_members.begin(), cur_members.end(), back_inserter(leaved_members));
		set_difference(cur_members.begin(), cur_members.end(), old_members.begin(), old_members.end(), back_inserter(added_members));
		if (!added_members.empty()) {
			GMSV::SendGTTeamMemberUpdate(gt_team_id, added_members, 0); // 0: team_add
		}
		if (!leaved_members.empty()) {
			GMSV::SendGTTeamMemberUpdate(gt_team_id, leaved_members, 1); // 1: team_del
		}
	}
}
void team_cache::RemoveTeam(int team_id)
{
	int removed_size = 0;
	{ // 锁保护，更新
		spin_autolock keeper(lock);

		removed_size = teams.erase(team_id);
	}
	
	if (removed_size)
	{
		GMSV::SendGTTeamDismiss(MakeGTTeamId(team_id));	
	}
}

void team_cache::SyncToGT()
{
	std::vector<GMSV::gt_team_info> gt_infos;
	{
		spin_autolock keeper(lock);
		GMSV::gt_team_info gt_info;
		for (std::map<int, team_cache_entry>::iterator it = teams.begin(); it != teams.end(); ++it) {
			gt_info.members.clear();
			gt_info.team_id = MakeGTTeamId(it->first);
			gt_info.captain = it->second.leader_id;	
			copy(it->second.members, it->second.members + it->second.team_size, back_inserter(gt_info.members));
			gt_infos.push_back(gt_info);
		}
	}	

	GMSV::SendGTSyncTeams(gt_infos);
}

class std_team_control : public player_team::team_control
{
	private:
		//客户端操作
		virtual void CliInviteOther(player_team * pTeam,const XID & member)
		{
			return ;
		}

		virtual void CliAgreeInvite(player_team * pTeam,const XID & leader,int seq)
		{
			return ;
		}

		virtual void CliRejectInvite(player_team * pTeam, const XID & member)
		{
			ASSERT(false);
		}

		virtual bool CliLeaveParty(player_team * pTeam) 
		{
			return false;
		}

		virtual void MsgJoinTeam(player_team *pTeam, const XID& leader,const player_team::member_entry*,size_t ,int pickup_flag,const void * ins_key , size_t ik_size, int team_gt_id)
		{
			//发送不能加入队伍的消息
			gplayer_imp * pImp = GetData(pTeam).imp;
			pImp->SendTo<0>(GM_MSG_JOIN_TEAM_FAILED,leader,0);
		}

		virtual void MsgJoinTeamFailed(player_team *pTeam, const XID & leader)
		{
			return;
		}

		virtual void MsgMemberUpdateData(player_team *,const XID & member, const A3DVECTOR & pos,const team_mutable_prop & data)
		{
			ASSERT(false);
		}

		virtual void MsgLeaderUpdateData(player_team *,const XID & leader,const player_team::member_entry * list,size_t count)
		{
			ASSERT(false);
		}

		virtual void MsgInvite(player_team *pTeam, const XID & leader,int seq,int pickup_flag, int familyid, int mafiaid, int level, int sectid, int referid)
		{
			ASSERT(false);
		}

		virtual void CliKickMember(player_team *pTeam, const XID & member)
		{
			ASSERT(false);
		}

		virtual void MsgMemberLeaveRequest(player_team * pTeam, const XID & member)
		{
			ASSERT(false);
		}

		void MsgNotifyMemberLeave(player_team * pTeam, const XID & leader,const XID & member,int type) 
		{
			ASSERT(false);
		}

		virtual void MsgNewMember(player_team*,const XID& leader,const player_team::member_entry * list, size_t count)
		{
			ASSERT(false);
		}
		
		virtual void MsgLeaderCancelParty(player_team *,const XID & leader,int seq)
		{
			ASSERT(false);
		}

		virtual bool PickupTeamMoney(player_team * pTeam, int tag, const A3DVECTOR & pos, int amount)
		{
			ASSERT(false);
			return false;
		}
		
		virtual void LostConnection(player_team * pTeam)
		{
			// do nothing
		}
		
		virtual void Logout(player_team * pTeam)
		{
			// do nothing
		}

		virtual void CliAgreeApply(player_team *, int id, bool result)
		{
			//do nothing
		}

		virtual void MsgApplyPartyReply(player_team*, const XID & leader, int seq)
		{
			//do nothing
		}
		
		virtual void CliChangeLeader(player_team *, const XID & new_leader)
		{
			//do nothing 
		}

		virtual void MsgChangeToLeader(player_team*, const XID & leader)
		{
			//do nothing
		}
		
		virtual void MsgLeaderChanged(player_team*, const XID & new_leader)
		{
			//do nothing
		}

};

class team_control_normal : public  std_team_control
{
	private:
		virtual void OnHeartbeat(player_team * pTeam) 
		{
			//负责清除超时的邀请
			long t = g_timer.get_systime();
			gplayer_imp * pImp = GetData(pTeam).imp;
			player_team::INVITE_MAP & map = GetData(pTeam).invite_map;
			player_team::INVITE_MAP::iterator it = map.end();
			for(;it != map.begin(); )
			{
				--it;
				if(it->second.timestamp < t)
				{
					pImp->_runner->team_invite_timeout(it->second.id.id);
					map.erase(it);
				}
			}
		}

		//客户端操作
		virtual void CliInviteOther(player_team * pTeam,const XID & member)
		{
			player_team::INVITE_MAP & map = GetData(pTeam).invite_map;
			gplayer_imp * pImp = GetData(pTeam).imp;
			if(map.size() >= player_team::INVITE_MAP::CAPACITY)
			{
				//队列满 发送现在不能邀请的要求
				pImp->_runner->error_message(S2C::ERR_TEAM_CANNOT_INVITE);
				return ;
			}

			if(map.find(member.id) != map.end())
			{
				//队列满 发送现在不能邀请的要求
				pImp->_runner->error_message(S2C::ERR_TEAM_ALREADY_INVITE);
				return ;
			}

			player_team::invite_t it;
			it.timestamp = g_timer.get_systime() + TEAM_INVITE_TIMEOUT;
			it.id = member;
			map[member.id] = it;

			//发送消息 请求玩家加入队伍
			msg_team_invite team_invite;
			int pickup_flag = GetData(pTeam).pickup_flag;
			team_invite.pickup_flag = pickup_flag;
			team_invite.family_id = pImp->GetParent()->id_family; 
			team_invite.mafia_id = pImp->GetParent()->id_mafia;
			team_invite.zone_id = pImp->GetParent()->id_zone;

			// Youshuang add
			team_invite.level = ( pImp->GetRebornCount() == 0 ) ? pImp->GetParent()->level : 0;
			team_invite.sectid = pImp->GetParent()->sect_id;
			team_invite.referid = pImp->GetReferID();
			// end

			pImp->SendTo2<0>(GM_MSG_TEAM_INVITE,member,GetData(pTeam).self_seq, pImp->GetFaction(),  &team_invite, sizeof(team_invite));
		}

		void MsgApplyPartyReply(player_team* pTeam, const XID & leader, int seq)
		{
			player_team::team_entry & data = GetData(pTeam);
			data.last_invite_time  = g_timer.get_systime() + 10;
			CliAgreeInvite(pTeam,leader,seq);
		}
		
		//只有normal有这个能力处理这个操作
		virtual void CliAgreeInvite(player_team * pTeam,const XID & leader,int seq)
		{
			player_team::team_entry & data = GetData(pTeam);
			gplayer_imp * pImp = GetData(pTeam).imp;
			if(data.last_invite_time < g_timer.get_systime())
			{
				//邀请超时了
				pImp->_runner->error_message(S2C::ERR_TEAM_INVITE_TIMEOUT);
				return;
			}

			//进入等待状态
			data.invite_map.clear();
			ChangeState(pTeam,player_team::TEAM_WAIT);
			data.time_out = TEAM_WAIT_TIMEOUT;
			GetTeam(pTeam).leader = leader;
			GetTeam(pTeam).team_seq = seq;

			player_team::agree_invite_entry prop;
			gplayer * pPlayer = (gplayer*)pImp->_parent;
			prop.data.Init(pImp);
			prop.cs_index = pPlayer->cs_index;
			prop.cs_sid = pPlayer->cs_sid;
			prop.cls = pPlayer->base_info.cls;
			prop.gender = pPlayer->base_info.gender;

			//发出消息给leader
			pImp->SendTo<0>(GM_MSG_TEAM_AGREE_INVITE,leader,seq,&prop,sizeof(prop));

		}

		virtual void CliRejectInvite(player_team * pTeam, const XID & leader)
		{
			player_team::team_entry & data = GetData(pTeam);
			if(data.last_invite_time < g_timer.get_systime())
			{
				//邀请超时了，直接返回
				return;
			}

			//这里不能清除邀请标记和时间戳，因为同时可以有多个邀请

			//发出消息给leader
			gplayer_imp * pImp = data.imp;
			pImp->SendTo<0>(GM_MSG_TEAM_REJECT_INVITE,leader,0);
		}

		//消息
		void MsgAgreeInvite(player_team * pTeam,const XID & member,const A3DVECTOR & pos,const player_team::agree_invite_entry& prop,int seq)
		{
			//受到某个玩家发来了同意加入的消息
			//检查该玩家是否在invite列表中
			player_team::team_entry & data = GetData(pTeam);
			player_team::INVITE_MAP::iterator it = data.invite_map.find(member.id);
			if(data.invite_map.end() == it || seq != data.self_seq)
			{
				//未找到合适的 让它超时
				return;
			}
			data.invite_map.erase(it);

			//组成队伍, 进入组队模式
			//将队员加入到队伍中
			data.min_timer_counter = 0;
			pTeam->BecomeLeader(member,prop,pos);
			data.time_out = TEAM_LEADER_UPDATE_INTERVAL;
			pTeam->_team.team_seq = data.self_seq;
			pTeam->_team.pickup_flag = data.pickup_flag;


			//发送消息给该队员
			gplayer_imp * pImp = data.imp;
			pImp->_runner->join_team(pTeam->_team.leader,data.pickup_flag);
			int count = pTeam->_team.member_count;
			int pickup_flag = pTeam->_team.pickup_flag;
			size_t buf_size = sizeof(player_team::member_entry) * count + pImp->_cur_ins_key_list.size()*(sizeof(int)*3);
			char * buf = (char*)malloc(buf_size);
			size_t buf_header = sizeof(player_team::member_entry) * count;
			memcpy(buf,pTeam->_team.member_list,buf_header);
			bool bRst = pImp->GetInstanceKeyBuf(buf + buf_header, buf_size - buf_header);
			ASSERT(bRst);
			pImp->SendTo2<0>(GM_MSG_JOIN_TEAM,member,(count & 0x7FFF) |(pickup_flag << 16),
					pTeam->_team.team_gt_id, // add by sunjunbo 2012.10.29  param2表示队伍的唯一id
					buf,buf_size);
			free(buf);

			//发送副本数据给该队员 注意可能会在组队消息之前到.....
			
			//进行任务相关的调用
			PlayerTaskInterface  task_if(pImp);
			OnTeamSetup(&task_if);

			//刷新组队标志，用于发送
//			pImp->SendTeamVisibleStateToOther(member.id,prop.cs_index,prop.cs_sid); 

			pImp->NotifyMasterInfo();

			//队长更新活跃度，进入队伍
			pImp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_TEAM);

			//通知GT
			team_cache::instance.UpdateTeam(pTeam->_team.team_gt_id, team_cache_entry(pTeam->_team));
		}

		void MsgRejectInvite(player_team *pTeam,const XID & member)
		{
			//受到某个玩家发来了不同意加入的消息
			//检查该玩家是否在invite列表中
			player_team::team_entry & data = GetData(pTeam);
			player_team::INVITE_MAP::iterator it = data.invite_map.find(member.id);
			if(data.invite_map.end() == it)
			{
				//没有找到,直接返回
				return;
			}
			data.invite_map.erase(it);

			//发送拒绝消息
			data.imp->_runner->player_reject_invite(member);
		}

		void MsgInvite(player_team *pTeam, const XID & leader,int seq,int pickup_flag, int familyid, int mafiaid, int level, int sectid, int referid)
		{
			//收到了一个人发来的邀请消息
			player_team::team_entry & data = GetData(pTeam);

			data.last_invite_time = g_timer.get_systime() + TEAM_INVITE_TIMEOUT2;
			data.imp->_runner->leader_invite(leader,seq,pickup_flag, familyid, mafiaid, level, sectid, referid);
		}
		
		void MsgApplyParty(player_team* pTeam, const XID & who, int faction, int familyid, int mafiaid, int level, int sectid, int referid)
		{
			//do nothing
		}
};

class team_control_wait : public  std_team_control
{
	private:
		virtual void OnHeartbeat(player_team * pTeam) 
		{
			if(--(GetData(pTeam).time_out) <= 0)
			{
				//超时了,回到normal状态
				ChangeState(pTeam,player_team::TEAM_NORMAL);
				pTeam->_data.imp->_runner->error_message(S2C::ERR_TEAM_JOIN_FAILED);
			}
		}

		//客户端操作
		virtual void CliInviteOther(player_team * pTeam,const XID & member)
		{
			ASSERT(false);
		}

		virtual void CliAgreeInvite(player_team * pTeam,const XID & leader,int seq)
		{
			ASSERT(false);
		}

		void CliRejectInvite(player_team * pTeam, const XID & member)
		{
			ASSERT(false);
		}

		//消息

		void MsgRejectInvite(player_team *pTeam,const XID & member)
		{
			//不处理,自己就要加入别的队伍了
			return ;
		}


		void MsgJoinTeam(player_team *pTeam, const XID & leader, const player_team::member_entry * list,size_t count, int pickup_flag,const void * ins_key , size_t ik_size, int team_gt_id)
		{
			player_team::team_t & team =  GetTeam(pTeam);
			gplayer_imp * pImp = GetData(pTeam).imp;
			if(!(team.leader == leader))
			{
				//不匹配的队长
				//返回错误消息
				pImp->SendTo<0>(GM_MSG_JOIN_TEAM_FAILED,leader,0);
				return ;
			}

			//处理加入队伍的操作,现在不组织队伍列表,因为根本就没有数据
			team.member_count = 0;
			team.pickup_flag = pickup_flag;

			GetData(pTeam).min_timer_counter = 0;
			
			//进入队员状态 ,建立队伍结构
			pTeam->BecomeMember(leader,list,count);
			pTeam->_team.team_gt_id = team_gt_id;
			pTeam->_data.time_out = TEAM_LEADER_TIMEOUT;
			__PRINTF("enter team: timeout%d\n",pTeam->_data.time_out);

			//建立和队长一样的instance key
			pImp->SetInstanceKeyBuf(ins_key,ik_size);

			//通知player进入操作状态
			pImp->_runner->join_team(leader,pickup_flag);

			//发送所有的组队可见数据给队友
			pImp->SendTeamDataToMembers();

			//队员更新活跃度，进入队伍
			pImp->EventUpdateLiveness(gplayer_imp::LIVENESS_UPDATE_TYPE_SPECIAL, gplayer_imp::LIVENESS_SPECIAL_TEAM);

			pImp->NotifyMasterInfo();

		}

		void MsgAgreeInvite(player_team * pTeam, const XID & member,const A3DVECTOR &,const player_team::agree_invite_entry & prop,int seq)
		{
			//什么都不作，忽略掉
			//因为只有leader处理
		}

		void MsgApplyParty(player_team* pTeam, const XID & who, int faction, int familyid, int mafiaid, int level, int sectid, int referid)
		{
			//do nothing
		}
};

class team_control_member : public  std_team_control
{
	private:
		virtual void OnHeartbeat(player_team * pTeam) 
		{
			gplayer_imp * pImp = GetData(pTeam).imp;
			//检测如果 leader长时间不发送notify的话,进行超时处理
			//这个超时可以考虑在30秒左右
			if((--GetData(pTeam).time_out) <= 0)
			{
				__PRINTF("队员发现队长超时，自动离开队伍\n");
				//超时了，执行离开队伍逻辑
				MsgNotifyMemberLeave(pTeam, pTeam->_team.leader,pImp->_parent->ID,0);
				return ;
			}
			if((GetData(pTeam).min_timer_counter++) > 30)
			{
				//每30秒计算最大级别
				pTeam->CalcExactMaxLevel();
				GetData(pTeam).min_timer_counter = 0;

			}
		
			//发送队伍数据给客户端
			pTeam->SendTeamDataToClient();

			//首先需要是否要发送数据
			team_mutable_prop data(pImp);
			if(pImp->GetRefreshState())
			{
				pTeam->SendSelfDataToMember(data);
			}
			else if(pTeam->_data.time_out & 0x01)	//每两秒发送一次数据
			{
				player_team::team_t & team =  GetTeam(pTeam);
				pImp->SendTo<0>(GM_MSG_MEMBER_NOTIFY_DATA,team.leader,0,&data,sizeof(data));
			}
		}


		//客户端操作
		virtual void CliInviteOther(player_team * pTeam,const XID & member)
		{
			ASSERT(false);
		}

		virtual void CliAgreeInvite(player_team * pTeam,const XID & leader,int seq)
		{
			ASSERT(false);
		}

		void CliRejectInvite(player_team * pTeam, const XID & leader)
		{
			ASSERT(false);
		}

		bool CliLeaveParty(player_team * pTeam)
		{
			gplayer_imp * pImp = GetData(pTeam).imp;
			player_team::team_t & team =  GetTeam(pTeam);
			//发出离开队伍消息 队长同意后即离开队伍
			pImp->SendTo<0>(GM_MSG_LEAVE_PARTY_REQUEST,team.leader,0);
			return true;
		}

		//消息
		void MsgAgreeInvite(player_team * pTeam, const XID & member,const A3DVECTOR &,const player_team::agree_invite_entry & prop,int seq)
		{
			//此状态不处理此操作,让它超时
			return;
		}

		void MsgRejectInvite(player_team *pTeam,const XID & member)
		{
			//不处理
			return ;
		}

		//成员更新自己的数据
		void MsgMemberUpdateData(player_team * pTeam,const XID & member, const A3DVECTOR &pos,const team_mutable_prop & data)
		{
			pTeam->UpdateMemberData(member,pos,data);
		}

		//队长更新大家的数据
		void MsgLeaderUpdateData(player_team *pTeam,const XID & leader,const player_team::member_entry * list,size_t count)
		{
			pTeam->LeaderUpdateMembers(leader,list,count);
			//更新队长的时间戳
			GetData(pTeam).time_out = TEAM_LEADER_TIMEOUT;

			GetData(pTeam).imp->NotifyMasterInfo();
		}


		//收到leader发来的新人加入消息
		void MsgNewMember(player_team* pTeam,const XID& leader,const player_team::member_entry * list, size_t count)
		{
			ASSERT(count >= 2);
			if(leader != pTeam->_team.leader)  return ;
			pTeam->LeaderUpdateMembers(leader,list,count);
			const player_team::member_entry & ent = list[count-1];
			pTeam->_data.imp->_runner->new_member(ent.id);
			//更新队长的时间戳
			GetData(pTeam).time_out = TEAM_LEADER_TIMEOUT;
//			pTeam->_data.imp->SendTeamVisibleStateToOther(ent.id.id,ent.cs_index,ent.cs_sid);

                        gplayer* player = (gplayer*)pTeam->_data.imp->_parent;
			if(player->IsInvisible()) pTeam->_data.imp->SendAppearToTeamMember(ent.id.id,ent.cs_index,ent.cs_sid);
			return ;
		}

		void MsgNotifyMemberLeave(player_team * pTeam, const XID & leader,const XID & member,int type) 
		{
			//收到leader发出的踢出某member的消息
			if(leader != pTeam->_team.leader)  return ;
			gplayer_imp * pImp = pTeam->_data.imp;
			if(member == pImp->_parent->ID)
			{

				//调用任务系统的接口
				PlayerTaskInterface task_if(pImp);
				OnTeamMemberLeave(&task_if);
			
				//是自己,直接离开队伍
				pImp->_runner->leave_party(leader,type);
				pTeam->BecomeNormal();

				//不需要重建所有的副本key
				//因为自身的key依然存在

				return ;
			}
			
			player_team::member_entry * pEntry = pTeam->FindMember(member);
			if(pEntry)
			{
				//删除该队员
				pTeam->MemberRemoveMember(pEntry);
				pImp->_runner->member_leave(leader,member,type);
			}
			pImp->NotifyMasterInfo();
		}

		//收到leader发来来的取消组队的消息
		virtual void MsgLeaderCancelParty(player_team *pTeam,const XID & leader,int seq)
		{
			if(leader != pTeam->_team.leader || seq != pTeam->_team.team_seq)  return;
			
			//调用任务系统的接口
			PlayerTaskInterface task_if(pTeam->_data.imp);
			OnTeamMemberLeave(&task_if);


			pTeam->BecomeNormal();
			ChangeState(pTeam,player_team::TEAM_NORMAL);
			//	pTeam->_data.imp->_runner->leader_cancel_party(leader);
			pTeam->_data.imp->_runner->leave_party(leader,2);
			pTeam->_data.imp->NotifyMasterInfo();

		}

		virtual void LostConnection(player_team * pTeam)
		{
			CliLeaveParty(pTeam);
		}

		virtual void Logout(player_team * pTeam)
		{
			LostConnection(pTeam);
		}

		void MsgApplyParty(player_team* pTeam, const XID & who, int faction, int familyid, int mafiaid, int level, int sectid, int referid)
		{
			//转发组队申请到队长身上
			const XID & leader = pTeam->GetLeader();
			MSG msg;
			msg_apply_party msg_apply;
			msg_apply.faction = faction;
			msg_apply.family_id = familyid;
			msg_apply.mafia_id = mafiaid;
			// Youshuang add
			msg_apply.level = level;
			msg_apply.sectid = sectid;
			msg_apply.referid = referid;
			// end
			BuildMessage(msg, GM_MSG_TEAM_APPLY_PARTY, leader, who,A3DVECTOR(0,0,0), 0,  &msg_apply, sizeof(msg_apply)); 
			SendMessage<0>(pTeam->_data.imp->_plane,msg);
		}

		void MsgChangeToLeader(player_team* pTeam, const XID & leader)
		{
			if(leader != GetTeam(pTeam).leader)  return ;
			//消息的发来者的确是leader
			//进行状态和逻辑的转换，将自己变成leader，并发送转换消息给所有的队友 
			//并且调用player的队长转换逻辑
			if(pTeam->FromMemberToLeader(leader))
			{
				MSG msg;
				BuildMessage(msg, GM_MSG_TEAM_LEADER_CHANGED, XID(-1,-1),GetData(pTeam).imp->_parent->ID,A3DVECTOR(0,0,0));
				//考虑附加所有的队员列表
				pTeam->SendGroupMessage(msg);
				
				GetData(pTeam).time_out = 1;	//一秒钟后即进行所有的数据更新
				//给客户端发送消息
				gplayer_imp * pImp = pTeam->_data.imp;
				pImp->_runner->change_team_leader(leader,pImp->_parent->ID);
			}
		}

		void MsgLeaderChanged(player_team* pTeam, const XID & new_leader)
		{
			if(!pTeam->IsMember(new_leader))
			{
				//该人不在队伍中
				return ;
			}
			
			//这时需广播一个消息通知周围的物品修改其组队权限
			//或者考虑最近30秒有无杀死的怪物（是否得到过组队经验）， 如果有再进行物品区域消息广播
			//这样，物品的所属权里必须加入team/seq作为判定标准
			//现在不考虑这个问题了，反正也并非主要问题
			
			XID old_leader = pTeam->_team.leader;
			pTeam->ChangeLeader(new_leader);
			GetData(pTeam).time_out = TEAM_LEADER_TIMEOUT;

			//给客户端发送消息
			gplayer_imp * pImp = pTeam->_data.imp;
			pImp->_runner->change_team_leader(old_leader,new_leader);
		}

		bool PickupTeamMoney(player_team * pTeam, int tag, const A3DVECTOR & pos, int amount)
		{
			return pTeam->DispatchTeamMoney(tag, pos, amount);
		}

};

class team_control_leader: public std_team_control
{
	private:

		virtual void OnHeartbeat(player_team * pTeam) 
		{
			long ts = g_timer.get_systime();
			gplayer_imp * pImp = GetData(pTeam).imp;
			XID self = pImp->_parent->ID;

			//负责清除超时的邀请
			player_team::INVITE_MAP & map = GetData(pTeam).invite_map;
			player_team::INVITE_MAP::iterator it = map.end();
			for(;it != map.begin(); )
			{
				--it;
				if(it->second.timestamp < ts)
				{
					pImp->_runner->team_invite_timeout(it->second.id.id);
					map.erase(it);
				}
			}

			//负责清除超时的申请
			player_team::INVITE_MAP & apply_map = GetData(pTeam).apply_map;
			it = apply_map.end();
			for(;it != apply_map.begin(); )
			{
				--it;
				if(it->second.timestamp < ts)
				{
					apply_map.erase(it);
				}
			}
			
			//清除超时的改变队长操作
			if(GetData(pTeam).change_leader_timeout)
			{
				if( (-- GetData(pTeam).change_leader_timeout) <= 0)
				{
					pImp->RebuildInstanceKey();
					__leave_party(pTeam);
					return;
				}
			}

			//检测如果 member 长时间不发送notify的话,进行超时处理
			//这个超时可以考虑在10秒左右(或者更加长)
			if(GetData(pTeam).time_out & 0x01)
			{
				MSG msg;
				MSG * pMsg = NULL;
				for(size_t i = 0 ; i < pTeam->_team.member_count; )
				{
					player_team::member_entry & member = pTeam->_team.member_list[i];
					if((member.timeout -= 2 ) <= 0 && member.id != self)
					{	
						//超时,删除之
						if(!pMsg)
						{
							BuildMessage(msg,GM_MSG_MEMBER_LEAVE,XID(-1,-1),
									self,pImp->_parent->pos,member.id.id);
							pMsg = &msg;
						}
						SendGroupMessage(pTeam,*pMsg);
						pImp->_runner->member_leave(self,member.id,0);
						__PRINTF("队长发现队员%d超时,更新队伍列表\n",member.id.id);
						
						int member_id = member.id.id;
						//去除队员		
						pTeam->LeaderRemoveMember(&member);
						if(pTeam->_team.member_count > 1)
						{
							//如果组队人数少于一个，后面会调用__leave_party逻辑
							//通知任务接口
							PlayerTaskInterface task_if(pImp);
							OnTeamDismissMember(&task_if,member_id);
						}


					}
					else
					{
						i ++;
					}
				}
			}

			//如果只剩下自己，则退出队伍
			if(pTeam->_team.member_count <= 1)
			{
				__leave_party(pTeam);
				return;
			}

			//试着发送队伍数据给客户端
			pTeam->SendTeamDataToClient();

			//每30秒计算最大级别
			if((GetData(pTeam).min_timer_counter++) > 30)
			{
				pTeam->CalcExactMaxLevel();
				GetData(pTeam).min_timer_counter = 0;

			}

			//更新自己的位置 
			pTeam->_team.member_list[0].pos = pImp->_parent->pos;

			//是否要发送数据
			if((--GetData(pTeam).time_out) <= 0)
			{
				//发送所有member数据到所有其他玩家
				GetData(pTeam).time_out = TEAM_LEADER_UPDATE_INTERVAL;

				//发送队伍消息给所有队员
				int count = pTeam->_team.member_count;
				MSG msg;
				BuildMessage(msg,GM_MSG_LEADER_UPDATE_MEMBER,XID(-1,-1),
						self,pImp->_parent->pos,
						count,pTeam->_team.member_list,
						sizeof(player_team::member_entry)*count);
				SendGroupMessage(pTeam,msg);
				pTeam->CalcMaxLevel();

				//设置全部更新标志，使得自己也可以收到所有数据
				pTeam->_team.change_flag = player_team::team_t::TEAM_ALL_CHANGED;
			}
			else
			if(pImp->GetRefreshState())
			{
				team_mutable_prop data(pImp);
				//更新自己的血值等参数
				pTeam->_team.member_list[0].pos = pImp->_parent->pos;
				pTeam->_team.member_list[0].data = data;
				pTeam->SendSelfDataToMember(data);
			}
		}

		//客户端操作
		virtual void CliInviteOther(player_team * pTeam,const XID & member)
		{
			player_team::INVITE_MAP & map = GetData(pTeam).invite_map;
			gplayer_imp * pImp = GetData(pTeam).imp;
			if(map.size() + GetTeam(pTeam).member_count >= TEAM_MEMBER_CAPACITY)
			{
				//队列满 发送现在不能邀请的要求
				pImp->_runner->error_message(S2C::ERR_TEAM_CANNOT_INVITE);
				return ;
			}

			if(map.find(member.id) != map.end())
			{
				//发送过邀请了
				pImp->_runner->error_message(S2C::ERR_TEAM_ALREADY_INVITE);
				return ;
			}

			player_team::invite_t it;
			it.timestamp = g_timer.get_systime() + TEAM_INVITE_TIMEOUT;
			it.id = member;
			map[member.id] = it;

			//发送消息 请求玩家加入队伍
			msg_team_invite team_invite;
			int pickup_flag = GetTeam(pTeam).pickup_flag;	//从当前的队伍中取得
			team_invite.pickup_flag = pickup_flag;
			team_invite.family_id = pImp->GetParent()->id_family; 
			team_invite.mafia_id = pImp->GetParent()->id_mafia;
			team_invite.zone_id = pImp->GetParent()->id_zone;

			// Youshuang add
			team_invite.level = ( pImp->GetRebornCount() == 0 ) ? pImp->GetParent()->level : 0;
			team_invite.sectid = pImp->GetParent()->sect_id;
			team_invite.referid = pImp->GetReferID();
			// end
			
			pImp->SendTo2<0>(GM_MSG_TEAM_INVITE,member,GetTeam(pTeam).team_seq, pImp->GetFaction(), &team_invite, sizeof(team_invite));
		}

		virtual void CliAgreeInvite(player_team * pTeam,const XID & leader,int seq)
		{
			ASSERT(false);
		}

		void CliRejectInvite(player_team * pTeam, const XID & leader)
		{
			ASSERT(false);
		}


		inline 	void __leave_party(player_team * pTeam)
		{
			//通知任务接口 ,必须在真正队伍解散之前完成 放在这里是不是合适?
			PlayerTaskInterface task_if(pTeam->_data.imp);
			OnTeamDisband(&task_if);

			if(GetData(pTeam).change_leader_timeout > 0)
			{
				//如果处于等待更改队长的冷却期，则重建副本key
				pTeam->_data.imp->RebuildInstanceKey();
			}

			pTeam->_data.imp->_runner->leave_party(pTeam->_team.leader,0);
			//转到正常状态
			GetData(pTeam).self_seq ++;
			pTeam->BecomeNormal();

			//通告GT队伍解散
			team_cache::instance.RemoveTeam((int)pTeam->_team.team_gt_id);
		}

		bool CliLeaveParty(player_team * pTeam)
		{
			//处理客户端发来的离开队伍的命令
			//这样的要求即是解散队伍
			gplayer_imp * pImp = GetData(pTeam).imp;
			//发出队伍解散消息
			MSG msg;
			BuildMessage(msg,GM_MSG_LEADER_CANCEL_PARTY,XID(-1,-1),
					pImp->_parent->ID,pImp->_parent->pos,
					pTeam->_team.team_seq);
			SendGroupMessage(pTeam,msg);

			__leave_party(pTeam);
			return true;
		}

		void CliKickMember(player_team *pTeam, const XID & member)
		{
			player_team::member_entry * pEntry = pTeam->FindMember(member);
			if(pEntry)
			{
				//给该队员发出踢出的消息，离开的方式也存在
				MSG msg;
				gplayer_imp *pImp = pTeam->_data.imp;
				XID self = pImp->_parent->ID;
				BuildMessage(msg,GM_MSG_LEADER_KICK_MEMBER,XID(-1,-1),
						self,pImp->_parent->pos,member.id);
				SendGroupMessage(pTeam,msg);

				//删除该队员
				pTeam->LeaderRemoveMember(pEntry);
				pImp->_runner->member_leave(self,member,1);

				//考虑剩下了几个队员 
				//如果是一个，那么解散队伍
				if(pTeam->_team.member_count <= 1)
				{
					__leave_party(pTeam);
					return ;
				}

				//通知任务接口
				PlayerTaskInterface task_if(pImp);
				OnTeamDismissMember(&task_if,member.id);
			}
		}

//消息
		void MsgAgreeInvite(player_team * pTeam, const XID & member,const A3DVECTOR & member_pos,const player_team::agree_invite_entry & prop,int seq)
		{
			//受到某个玩家发来了同意加入的消息
			//检查该玩家是否在invite列表中
			player_team::team_entry & data = GetData(pTeam);
			player_team::INVITE_MAP::iterator it = data.invite_map.find(member.id);
			if( pTeam->_team.member_count >= TEAM_MEMBER_CAPACITY || data.invite_map.end() == it || seq != GetTeam(pTeam).team_seq)
			{
				//未找到合适的 让它超时
				return;
			}
			data.invite_map.erase(it);

			//将队员加入到队伍中
			pTeam->LeaderAddMember(member,prop,member_pos);


			player_team::member_entry * list = pTeam->_team.member_list;
			int count = pTeam->_team.member_count;
			//发送队伍消息给所有队员
			gplayer_imp * pImp = data.imp;
			const A3DVECTOR & pos = pImp->_parent->pos;
			XID self= pImp->_parent->ID;
			MSG msg;
			int pickup_flag = pTeam->_team.pickup_flag;

			size_t buf_size = sizeof(player_team::member_entry) * count + pImp->_cur_ins_key_list.size()*(sizeof(int)*3);
			char * buf = (char*)malloc(buf_size);
			size_t buf_header = sizeof(player_team::member_entry) * count;
			memcpy(buf,list,buf_header);
			bool bRst = pImp->GetInstanceKeyBuf(buf + buf_header, buf_size - buf_header);
			ASSERT(bRst);
			BuildMessage2(msg,GM_MSG_JOIN_TEAM,member,self,pos,(count & 0x7FFF) |(pickup_flag << 16),
					pTeam->_team.team_gt_id, // add by sunjunbo 2012.10.29  param2表示队伍的唯一id
					buf,buf_size);
			SendMessage<0>(pImp->_plane,msg);
			free(buf);

			msg.content_length = buf_header;
			msg.content = list;

			//最后一个队员肯定是新队员
			ASSERT(list[count-1].id == member);
			//给所有其他队员发送有新人进入的消息 
			msg.param = count;
			msg.message = GM_MSG_NEW_MEMBER;
			for(int i = 0; i < count - 1 ; i ++)
			{
				msg.target = list[i].id;
				SendMessage<0>(pImp->_plane,msg);
			}

			//重新设置更新时间（因为已经给所有人发送了更新消息）
			data.time_out = TEAM_LEADER_UPDATE_INTERVAL;
			pTeam->_data.imp->_runner->new_member(list[count-1].id);

//			pImp->SendTeamVisibleStateToOther(member.id,prop.cs_index,prop.cs_sid);

			//通知任务系统
			PlayerTaskInterface task_if(pImp);
			Task_OnTeamAddMember(&task_if,count - 1);

			//通知GT
			team_cache::instance.UpdateTeam((int)pTeam->_team.team_gt_id, team_cache_entry(pTeam->_team));
		}


		virtual void MsgJoinTeamFailed(player_team *pTeam, const XID & leader)
		{
			//队员加入组队失败
			//将该队员删除，并通知其他队员
			return;
		}
		
		void MsgRejectInvite(player_team *pTeam,const XID & member)
		{
			//受到某个玩家发来了不同意加入的消息
			//检查该玩家是否在invite列表中
			player_team::team_entry & data = GetData(pTeam);
			player_team::INVITE_MAP::iterator it = data.invite_map.find(member.id);
			if(data.invite_map.end() == it)
			{
				//没有找到,直接返回
				return;
			}
			data.invite_map.erase(it);

			//发送拒绝消息
			data.imp->_runner->player_reject_invite(member);
		}

		void SendGroupMessage(player_team *pTeam,MSG & msg)
		{
			player_team::member_entry * list = pTeam->_team.member_list;
			int count = pTeam->_team.member_count;
			for(int i = 0; i < count; i++)
			{
				if(list[i].id == msg.source) continue;

				msg.target = list[i].id;
				gmatrix::SendMessage(msg);
			}
		}


		void MsgMemberLeaveRequest(player_team * pTeam, const XID & member)
		{
			player_team::member_entry * pEntry = pTeam->FindMember(member);
			if(pEntry)
			{
				gplayer_imp *pImp = pTeam->_data.imp;
				//通知所有队员该队员离开，离开者也使用这个消息来离开队伍
				MSG msg;
				BuildMessage(msg,GM_MSG_MEMBER_LEAVE,XID(-1,-1),
						pImp->_parent->ID,pImp->_parent->pos,member.id);
				SendGroupMessage(pTeam,msg);

				pImp->_runner->member_leave(pImp->_parent->ID,member,0);

				// 删除该队员
				pTeam->LeaderRemoveMember(pEntry);

				//考虑剩下了几个队员 
				//如果是一个，那么解散队伍
				if(pTeam->_team.member_count <= 1)
				{
					__leave_party(pTeam);
					return ;
				}

				//通知任务接口
				PlayerTaskInterface task_if(pImp);
				OnTeamDismissMember(&task_if,member.id);

				//通知GT
				team_cache::instance.UpdateTeam((int)pTeam->_team.team_gt_id, team_cache_entry(pTeam->_team));
			}
		}

		void MsgMemberUpdateData(player_team *pTeam, const XID & member,const A3DVECTOR & pos, const team_mutable_prop & prop)
		{
			//队员更新了数据
			player_team::member_entry * pEntry = pTeam->UpdateMemberData(member,pos,prop);
			if(pEntry)
			{
				//更新队员的时间戳
				pEntry->timeout = TEAM_MEMBER_TIMEOUT;
			}
			else
			{
				gplayer_imp * pImp = pTeam->_data.imp;
				//返回一个错误消息
				pImp->SendTo<0>(GM_MSG_MEMBER_NOT_IN_TEAM,member,0);
			}
		}

		void MsgLeaderUpdateData(player_team *, const XID & leader, const player_team::member_entry *, size_t count)
		{
			ASSERT(false);
			return ;
		}

		bool PickupTeamMoney(player_team * pTeam, int tag, const A3DVECTOR & pos, int amount)
		{
			return pTeam->DispatchTeamMoney(tag, pos, amount);
		}

		void LostConnection(player_team * pTeam)
		{
			//断线了,需要将队长交给第二位
			if(pTeam->_team.member_count >= 2)
			{
				XID new_leader = pTeam->_team.member_list[1].id;
				CliChangeLeader(pTeam, new_leader);
				pTeam->_data.imp->LazySendTo<0>(GM_MSG_LEAVE_PARTY_REQUEST,new_leader,0,37);
			}
			//重新生成key
			pTeam->_data.imp->RebuildInstanceKey();
		}

		void Logout(player_team * pTeam)
		{
			//登出不更改玩家的instance_key,肯定属于自己的
			CliLeaveParty(pTeam);
		}
		
		void MsgApplyParty(player_team* pTeam, const XID & who, int faction, int familyid, int mafiaid, int level, int sectid, int referid)
		{
			if(pTeam->_team.member_count >= TEAM_MEMBER_CAPACITY) 
			{
				//考虑是否发出无法加入的申请
				return;
			}

			//自己处理申请
			player_team::INVITE_MAP & map = GetData(pTeam).apply_map;
			gplayer_imp * pImp = GetData(pTeam).imp;
			if(map.size() >= player_team::INVITE_MAP::CAPACITY) return;

			player_team::invite_t it;
			it.timestamp = g_timer.get_systime() + TEAM_INVITE_TIMEOUT;
			it.id = who;
			map[who.id] = it;

			pImp->_runner->send_party_apply(who.id, familyid, mafiaid, level, sectid, referid);
		}

		void CliAgreeApply(player_team *pTeam, int id, bool result)
		{
			//自己处理申请
			player_team::INVITE_MAP & map = GetData(pTeam).apply_map;
			player_team::INVITE_MAP & invite_map = GetData(pTeam).invite_map;
			gplayer_imp * pImp = GetData(pTeam).imp;
			if(map.find(id) == map.end())
			{
				//未找到申请人 
				return;
			}

			if(!result) 
			{
				//发送拒绝数据给申请者
				pImp->SendTo<0>(GM_MSG_ERROR_MESSAGE,XID(GM_TYPE_PLAYER,id),S2C::ERR_TEAM_REFUSE_APPLY);
				map.erase(id);
				return;
			}

			if(pTeam->_team.member_count >= TEAM_MEMBER_CAPACITY 
					|| invite_map.size() >= player_team::INVITE_MAP::CAPACITY)
			{
				//考虑是否发出无法加入的申请
				return;
			}

			//发送同意申请的消息给玩家
			//在邀请列表里加入一项
			//发出邀请请求
			player_team::invite_t it;
			it.timestamp = g_timer.get_systime() + TEAM_INVITE_TIMEOUT;
			it.id = XID(GM_TYPE_PLAYER,id);
			invite_map[id] = it;
			pImp->SendTo<0>(GM_MSG_TEAM_APPLY_REPLY,XID(GM_TYPE_PLAYER,id),pTeam->_team.team_seq);
		}

		virtual void CliChangeLeader(player_team * pTeam, const XID & new_leader)
		{
			//检测该目标是否成员
			if(GetData(pTeam).change_leader_timeout) return;
			if(new_leader == pTeam->_team.leader) return;
			if(GetData(pTeam).change_leader_timeout) return;
			if(pTeam->IsMember(new_leader))
			{
				gplayer_imp * pImp = GetData(pTeam).imp;
				pImp->SendTo<0>(GM_MSG_TEAM_CHANGE_TO_LEADER,new_leader,0);
				pTeam->_data.change_leader_timeout = 25;
			}
		}

		void MsgLeaderChanged(player_team* pTeam, const XID & new_leader)
		{
			if(!pTeam->IsMember(new_leader))
			{
				//该人不在队伍中
				return ;
			}
			GetData(pTeam).change_leader_timeout = 0;
			pTeam->FromLeaderToMember(new_leader);
			GetData(pTeam).time_out = TEAM_LEADER_TIMEOUT;
			//重新生成原始key 避免副本重用
			pTeam->_data.imp->RebuildInstanceKey();


			//给客户端发送消息
			gplayer_imp * pImp = pTeam->_data.imp;
			pImp->_runner->change_team_leader(pImp->_parent->ID,new_leader);

			//通知GT
			team_cache::instance.UpdateTeam((int)pTeam->_team.team_gt_id, team_cache_entry(pTeam->_team));
		}
};

player_team::team_control * player_team::_team_ctrl[player_team::TEAM_STATE_NUM] =
{
	new team_control_normal,
	new team_control_wait,
	new team_control_member,
	new team_control_leader
};

void
player_team::Init(gplayer_imp * pPlayer)
{
	_data.imp = pPlayer;
	_data.self_seq = g_timer.get_systime();
}

void 
player_team::NotifyTeamPickup(const A3DVECTOR & pos, int type, int count)
{	
	//这里怎么个通知法....如果保存了所有的队友的cs_index和sid的话，可以直接发送数据，也未尝不可
	//这样效率可能比较高   另外组队聊天的话也可以考虑这种方式
	//现在先用消息系统来进行，如果要保存cs_index和sid话，需要做较大变动

	MSG msg;
	BuildMessage(msg,GM_MSG_TEAM_PICKUP_NOTIFY,XID(-1,-1),_data.imp->_parent->ID,pos,type,&count,sizeof(count));
	SendGroupMessage(msg);
}
void            
player_team::TeamChat(char channel, char emote_id, const void * buf, size_t len, int srcid, const void * aux_data, size_t dsize )
{                       
	GMSV::chat_msg msg;
	int self = srcid;
	msg.speaker = self;
	msg.msg = buf;
	msg.size = len; 
	msg.data = aux_data;
	msg.dsize = dsize;
	msg.channel = channel;
	msg.emote_id = emote_id;
	size_t count = _team.member_count;
	for(size_t i = 0;i < count; i ++)
	{
		const member_entry & member = _team.member_list[i];
		GMSV::SendChatMsg(member.cs_index, member.id.id, member.cs_sid,msg);
	}               
}

void 
player_team::SendMessage(const MSG & msg, float range,bool norangelimit, int max_count)
{
	size_t count = _team.member_count;
	if(max_count > 0 && (size_t)max_count < count)
	{
		count = (int)max_count;
	}
	size_t index = 0;
	XID  list[TEAM_MEMBER_CAPACITY];
	ASSERT(msg.source == _data.imp->_parent->ID);
	range *= range;
	world *pPlane = _data.imp->_plane;
	for(size_t i = 0;i < count; i ++)
	{
		const XID & member =_team.member_list[i].id;
		if(!norangelimit)
		{
			world::object_info info;
			if(msg.source == member  || 
					!pPlane->QueryObject(member,info) || 
					info.pos.squared_distance(msg.pos) >= range)
			{
				continue;
			}
		}
		list[index++] = member;
	}
	if(index) gmatrix::SendMessage(list,list + index, msg);
}

void 
player_team::SendAllMessage(const MSG & msg, float range,bool norangelimit, int max_count)
{
	size_t count = _team.member_count;
	if(max_count > 0 && (size_t)max_count < count)
	{
		count = (int)max_count;
	}
	size_t index = 0;
	XID  list[TEAM_MEMBER_CAPACITY];
	ASSERT(msg.source == _data.imp->_parent->ID);
	range *= range;
	world *pPlane = _data.imp->_plane;
	for(size_t i = 0;i < count; i ++)
	{
		const XID & member =_team.member_list[i].id;
		if(!norangelimit)
		{
			world::object_info info;
			if(msg.source != member  &&
					(!pPlane->QueryObject(member,info) || 
					 info.pos.squared_distance(msg.pos) >= range))
			{
				continue;
			}
		}
		list[index++] = member;
	}
	if(index) gmatrix::SendMessage(list,list + index, msg);
}


bool player_team::Save(archive & ar)
{
	ar << _team_state << _data.time_out << _data.self_seq << _data.pickup_flag << _data.cur_max_level;
	ar << _data.invite_map.size();
	INVITE_MAP::iterator it = _data.invite_map.begin();
	long t = g_timer.get_systime();
	for(; it != _data.invite_map.end(); it ++)
	{
		ar << it->first << (int)(it->second.timestamp - t) << it->second.id;	
	}

	ar << _team.leader << _team.team_seq << _team.member_count << _team.change_flag << _team.pickup_flag;
	for(size_t i = 0; i < _team.member_count; i ++)
	{
		member_entry & ent = _team.member_list[i];
		ar << ent.id << ent.data << ent.cls << ent.gender << ent.cs_index << ent.cs_sid  << ent.pos << ent.timeout << ent.is_changed;
	}
	return true;
}

void 
player_team::Swap(player_team & rhs)
{
#define Set(var,cls) var = cls.var
	Set(_team_state , rhs);
	Set(_data.time_out,rhs);
	Set(_data.self_seq,rhs);
	Set(_data.pickup_flag,rhs);
	Set(_data.cur_max_level,rhs);
	Set(_data.cur_wallow_level,rhs);
	Set(_item_roll,rhs);
	INVITE_MAP::iterator it = rhs._data.invite_map.begin();
	_data.invite_map.clear();
	for(; it != rhs._data.invite_map.end(); it ++)
	{
		_data.invite_map[it->first] = it->second;
	}

	Set(_team.leader ,rhs);
	Set(_team.team_seq ,rhs);
	Set(_team.member_count ,rhs);
	Set(_team.change_flag ,rhs);
	Set(_team.pickup_flag ,rhs);

	for(size_t i = 0; i < _team.member_count; i ++)
	{
		_team.member_list[i] = rhs._team.member_list[i];
	}
#undef Set
}

bool player_team::Load(archive & ar)
{
	ar >> _team_state >> _data.time_out >> _data.self_seq >> _data.pickup_flag >> _data.cur_max_level;
	size_t size;
	ar >> size;
	_data.invite_map.clear();
	long t = g_timer.get_systime();
	for(size_t i = 0; i < size; i ++)
	{
		int id;
		invite_t inv;
		ar >> id >> (int&)(inv.timestamp) >> inv.id;
		inv.timestamp += t;
		_data.invite_map[id] = inv;
	}

	ar >> _team.leader >> _team.team_seq >> _team.member_count >> _team.change_flag >> _team.pickup_flag;
	for(size_t i = 0; i < _team.member_count; i ++)
	{
		member_entry & ent = _team.member_list[i];
		ar >> ent.id >> ent.data >> ent.cls >> ent.gender >> ent.cs_index >> ent.cs_sid  >> ent.pos >> ent.timeout >> ent.is_changed;
	}
	return true;
}

void 
player_team::DispatchExp(const A3DVECTOR &pos,int * list ,size_t count, int64_t exp,int level,int total_level, int max_level ,int min_level,int npcid,float r, bool no_exp_punish)
{
	ASSERT(IsLeader());

	//按照最高级别进行惩罚
	float exp_adj = 0;
	gplayer_imp * pImp = _data.imp;
	bool bReborn = pImp->GetRebornCount() > 0 ? true: false;
	player_template::GetExpPunishment(max_level - level,&exp_adj, bReborn, no_exp_punish);
	//提供队伍加成
	//如果超过20级没有加成
	if(max_level - min_level < 30)
	{
		//这个操作会在比例的基础值上进行调整 
		player_template::SetTeamBonus(count,_data.cls_count,&exp_adj);
	}
	exp = (int64_t)(exp * exp_adj + 0.5f);

	//进行经验分配
	MSG msg;
	msg_grp_exp_t mexp;
	mexp.level = level;
	mexp.rand = r;
	BuildMessage(msg,GM_MSG_TEAM_EXPERIENCE,XID(-1,-1),_team.leader,pos,npcid,&mexp,sizeof(mexp));

	//total_level里面已经考虑了不得小于20级的限制
	float factor = 1/(float)total_level;

	for(size_t i = 0;i < count; i ++)
	{
		int index = list[i];
		int level = _team.member_list[index].data.level;
		float factor2 = level * factor;
		mexp.exp = (int64_t)(exp * factor2 + 0.5f);
		msg.target = _team.member_list[index].id;
		SendMessage<0>(_data.imp->_plane,msg);
	}
}

void player_team::CalcRaceCount()
{
	_data.cls_count = 0;
	if(!IsInTeam()) return;
	size_t member_count = _team.member_count;
	int cls_mask = 0;
	int cls_mask1 = 0;
	for(size_t i = 0; i < member_count ; i++)
	{
		int cls = _team.member_list[i].cls & 0x7F;
		if(cls <= 0x3F)
		{
			int mask = (1 << cls);
			if((cls_mask & mask) == 0)
			{
				cls_mask |= mask;
				_data.cls_count ++;
			}
		}
		else
		{
			int mask = (1 << (cls - 0x40));
			if((cls_mask1 & mask) == 0)
			{
				cls_mask1 |= mask;
				_data.cls_count ++;
			}
		}
	}
}

int player_team::GetNearMemberCount(int tag, const A3DVECTOR & pos)
{
	int mlist_count = 0;
	//确定附近人数
	for(int i = 0; i < GetMemberNum(); i ++)
	{
		const player_team::member_entry &ent = GetMember(i);
		if(ent.pos.squared_distance(pos) > TEAM_EXP_DISTANCE*TEAM_EXP_DISTANCE || tag  !=ent.data.world_tag)
		{
			continue;
		}
		mlist_count ++;
	}
	return mlist_count;
}

void player_team::CalcMaxLevel()
{
	if(!IsInTeam()) return;
	CalcExactMaxLevel();
}

void 
player_team::CalcExactMaxLevel()
{
	int self = _data.imp->_parent->ID.id;
	A3DVECTOR pos = _data.imp->_parent->pos;
	size_t count = _team.member_count;
	world::object_info info;

	int tag = _data.imp->_plane->GetTag();

	int max_level = 0;
	int wallow_level = 0;
	for(size_t i = 0; i < count; i ++)
	{
		const member_entry & member = _team.member_list[i];
		if(self == member.id.id)
		{
			int level = _data.imp->_basic.level;
			if(level > max_level)  max_level = level;

			int wallow = _data.imp->GetWallowLevel();
			if(wallow > wallow_level) wallow_level = wallow;
			continue;
		}
		if(member.data.world_tag != tag)
		{
			continue;
		}
		if(member.pos.squared_distance(pos) >= TEAM_ITEM_DISTANCE*TEAM_ITEM_DISTANCE)
		{
			continue;
		}
		int level = member.data.level;
		if(level > max_level)  max_level = level;

		int wallow = member.data.wallow_level;
		if(wallow > wallow_level) wallow_level = wallow;
	}
	_data.cur_max_level = max_level;
	_data.cur_wallow_level = wallow_level;

}

void 
player_team::LeaderRemoveMember(member_entry * pEntry)
{
	gplayer * pPlayer = (gplayer*)(_data.imp->_parent);
	if(pPlayer->IsInvisible()) _data.imp->SendDisappearToTeamMember(pEntry->id.id,pEntry->cs_index,pEntry->cs_sid);

	DelRollMember(pEntry->id);
	size_t offset = (_team.member_list + _team.member_count) - pEntry;
	memmove(pEntry,pEntry + 1, offset * sizeof(member_entry));
	_team.member_count --;
	CalcRaceCount();
	CalcMaxLevel();
	_data.imp->NotifyMasterInfo();
}

void
player_team::MemberRemoveMember(member_entry * pEntry)
{
	gplayer * pPlayer = (gplayer*)(_data.imp->_parent);
	if(pPlayer->IsInvisible()) _data.imp->SendDisappearToTeamMember(pEntry->id.id,pEntry->cs_index,pEntry->cs_sid);
	size_t offset = (_team.member_list + _team.member_count) - pEntry;
	memmove(pEntry,pEntry + 1, offset * sizeof(member_entry));
	_team.member_count --;
}


void 
player_team::AddRollMember(const XID & who)
{
	size_t n = _item_roll.tail;
	if(n >= TEAM_MEMBER_CAPACITY) return;
	for(size_t i = 0; i < n; i ++)
	{
		if(_item_roll.list[i] == who) return;
	}
	_item_roll.list[n] = who;
	_item_roll.tail ++;
}

void 
player_team::DelRollMember(const XID & who)
{
	size_t n = _item_roll.tail;
	for(size_t i = 0; i < n; i ++)
	{
		if(_item_roll.list[i] == who)
		{
			for(size_t j = i; j < n - 1; j ++)
			{
				_item_roll.list[j] = _item_roll.list[j +1];
			}
			_item_roll.tail --;
			return ;
		}
	}
}

void player_team::ClearRollMember()
{
	_item_roll.tail = 0;
}

XID 
player_team::RollItem(const A3DVECTOR & pos, int tag)
{
	size_t n = _item_roll.tail;
	ASSERT(n > 0);
	XID self = _data.imp->_parent->ID;
	world * pPlane = _data.imp->_plane;
	world::object_info info;
	for(size_t i = 0; i < n ; i ++)
	{	
		XID who = _item_roll.list[i];
		if(self == who)
		{
			if(_data.imp->GetWorldTag() != tag) continue;
			if(_data.imp->_parent->pos.squared_distance(pos) >= TEAM_ITEM_DISTANCE*TEAM_ITEM_DISTANCE) continue;
		}
		else
		{
			if(!pPlane->QueryObject(who,info,true) || 
					info.pos.squared_distance(pos) >= TEAM_ITEM_DISTANCE*TEAM_ITEM_DISTANCE)
			{
				continue;
			}
			if(tag != info.tag) 
			{
				continue;
			}
			if(info.state & (world::QUERY_OBJECT_STATE_ZOMBIE|world::QUERY_OBJECT_STATE_DISCONNECT))
			{
				continue;
			}
		}

		//找到了第一个合适的 将其移动到最后的位置
		for(size_t j = i; j < n - 1; j ++)
		{
			_item_roll.list[j] = _item_roll.list[j +1];
		}
		_item_roll.list[n - 1] =  who;
		return who;
	}
	return XID(0,0);
}

int
player_team::GetWallowLevel()
{
	if(IsInTeam())
	{
		return _data.cur_wallow_level;
	}
	return _data.imp->GetWallowLevel();

}

bool
player_team::DispatchTeamMoney(int tag, const A3DVECTOR & pos, int amount)
{
	if(amount <=0) return false;

	XID mlist[TEAM_MEMBER_CAPACITY];
	int mlist_count = 0;
	gplayer_imp * pImp = _data.imp;
	XID self = pImp->_parent->ID;

	//确定能分钱的玩家数量
	for(int i = 0; i < GetMemberNum(); i ++)
	{
		const player_team::member_entry &ent = GetMember(i);
		if(ent.pos.squared_distance(pos) > TEAM_EXP_DISTANCE*TEAM_EXP_DISTANCE || tag  !=ent.data.world_tag)
		{
			continue;
		}
		mlist[mlist_count] = ent.id;
		mlist_count ++;
	}

	size_t count = mlist_count;
	if(count < 1) return false;

	//开始分发金钱
	size_t acount = (size_t)((double)amount / (double)count);
	if(acount > MONEY_CAPACITY_BASE) acount = MONEY_CAPACITY_BASE;

	amount -= acount * count;
	int ex_id = -1;
	if(amount > 0)
	{
		ex_id = abase::Rand(0,count -1);
	}
	
	MSG msg;
	BuildMessage(msg,GM_MSG_RECEIVE_MONEY,XID(-1,-1),pImp->_parent->ID,pos,acount);

	if(acount > 0)
	{
		//发送金钱数据给相关队员
		for(size_t i =0; i < count; i ++)
		{
			msg.target = mlist[i];
			msg.param = acount;
			if(ex_id == (int)i)
			{
				msg.param += amount;
			}
			gmatrix::SendMessage(msg);
		}
	}
	else if(amount > 0 || ex_id >= 0)
	{
		//钱很少， 发送相关数据给特定队员
		msg.target = mlist[ex_id];
		msg.param = amount;
		gmatrix::SendMessage(msg);
	}
	return true;
}

bool player_team::BecomeLeader(const XID & first_member, const agree_invite_entry & prop,const A3DVECTOR &pos)
{
	if(_team_state != TEAM_NORMAL)
	{
		ASSERT(false);
		return false;
	}
	_team.leader = _data.imp->_parent->ID;
	_team.team_gt_id = team_t::GenerateTeamId(); // by sunjunbo 2012.10.30, 生成组队唯一id
	_team.member_count = 1;
	_team.member_list[0].data = team_mutable_prop(_data.imp);
	_team.member_list[0].id = _team.leader;
	_team.member_list[0].is_changed = false;
	_team.member_list[0].timeout  = 0;
	_team.member_list[0].pos = _data.imp->_parent->pos;
	gplayer * pPlayer = (gplayer*)(_data.imp->_parent);
	_team.member_list[0].cls = pPlayer->base_info.cls;
	_team.member_list[0].gender = pPlayer->base_info.gender;
	_team.member_list[0].cs_index = pPlayer->cs_index;
	_team.member_list[0].cs_sid  = pPlayer->cs_sid;

	ClearRollMember();
	AddRollMember(_team.leader);

	_data.change_leader_timeout = 0;
	_data.cur_max_level = _data.imp->_basic.level;
	_data.cur_wallow_level = _data.imp->GetWallowLevel();
	_team_state = TEAM_LEADER;
	LeaderAddMember(first_member,prop,pos);
	_team.change_flag = team_t::TEAM_ALL_CHANGED;
	pPlayer->object_state |= gactive_object::STATE_TEAM | gactive_object::STATE_TEAMLEADER;
	pPlayer->team_id = _team.leader.id;
	_data.imp->_runner->player_in_team(1);
	//使用自己的个人key作为组队副本列表
	_data.imp->_team_ins_key_list = _data.imp->_cur_ins_key_list;

	GLog::log(GLOG_INFO,"用户%d建立了队伍(%d,%d)", _team.leader.id, _team.leader.id,_team.team_seq);
	return true;
}

bool player_team::FromMemberToLeader(const XID & leader)
{
	if(_team_state != TEAM_MEMBER)
	{
		ASSERT(false);
		return false;
	}
	XID self = _data.imp->_parent->ID;

	_data.change_leader_timeout = 0;
	for(size_t i = 1; i < _team.member_count; i ++)
	{
		if(_team.member_list[i].id == self)
		{
			member_entry ent = _team.member_list[0];
			_team.member_list[0] = _team.member_list[i];
			ent.timeout = TEAM_MEMBER_TIMEOUT;
			_team.member_list[i] = ent;
			_team.leader = self;
			_team_state = TEAM_LEADER;
			_team.change_flag = team_t::TEAM_ALL_CHANGED;
			CalcRaceCount();
			CalcMaxLevel();
			gplayer * pPlayer = (gplayer*)(_data.imp->_parent);
			pPlayer->object_state |= gactive_object::STATE_TEAM | gactive_object::STATE_TEAMLEADER;
			pPlayer->team_id = _team.leader.id;
			//用组队列表覆盖个人列表 , 这样这些列表就彻底属于本玩家了
			_data.imp->_cur_ins_key_list = _data.imp->_team_ins_key_list;
			_data.imp->_runner->player_in_team(1);

			GLog::log(GLOG_INFO,"用户%d升为队长(%d,%d)", self.id, leader.id, _team.team_seq);


			//更新roll list 考虑采用随机方式
			ClearRollMember();
			XID mlist[TEAM_MEMBER_CAPACITY];
			size_t mcount = _team.member_count;
			for(size_t i = 0; i < mcount; i ++)
			{
				mlist[i] = _team.member_list[i].id;
			}

			while(mcount != 0)
			{
				int index = abase::Rand(0,(int)(mcount - 1));
				AddRollMember(mlist[index]);
				abase::swap(mlist[index],mlist[mcount-1]);
				mcount --;
			}
			return true;
		}
	}
	return false;
}

bool player_team::ChangeLeader(const XID & new_leader)
{
	if(_team_state != TEAM_MEMBER)
	{
		ASSERT(false);
		return false;
	}
	bool bRst = false;
	for(size_t i = 1; i < _team.member_count; i ++)
	{
		if(_team.member_list[i].id == new_leader)
		{
			member_entry ent = _team.member_list[0];
			_team.member_list[0] = _team.member_list[i];
			_team.member_list[i] = ent;
			_team.leader = new_leader;
			_team.change_flag = team_t::TEAM_ALL_CHANGED;
			gplayer * pPlayer = (gplayer *)(_data.imp->_parent);
			pPlayer->team_id = _team.leader.id;
			bRst = true;
		}
		_team.member_list[i].timeout =TEAM_MEMBER_TIMEOUT;
	}
	return bRst;
}

bool player_team::FromLeaderToMember(const XID & new_leader)
{
	if(_team_state != TEAM_LEADER)
	{
		ASSERT(false);
		return false;
	}
	for(size_t i = 1; i < _team.member_count; i ++)
	{
		if(_team.member_list[i].id == new_leader)
		{
			member_entry ent = _team.member_list[0];
			_team.member_list[0] = _team.member_list[i];
			_team.member_list[i] = ent;
			_team.leader = new_leader;
			_team_state = TEAM_MEMBER;
			_team.change_flag = team_t::TEAM_ALL_CHANGED;
			gplayer * pPlayer = (gplayer*)(_data.imp->_parent);
			pPlayer->object_state &= ~(gactive_object::STATE_TEAM | gactive_object::STATE_TEAMLEADER);
			pPlayer->object_state |= gactive_object::STATE_TEAM;
			pPlayer->team_id = _team.leader.id;
			_data.imp->_runner->player_in_team(2);

			return true;
		}
	}
	return false;
}

bool player_team::BecomeMember(const XID & leader, const member_entry * list, size_t count)
{
	if(_team_state != TEAM_WAIT)
	{
		ASSERT(false);
		return false;
	}
	ASSERT(leader == _team.leader);
	_team.member_count = 0;
	_team_state = TEAM_MEMBER;
	_data.cur_max_level = _data.imp->_basic.level;
	_data.cur_wallow_level = _data.imp->GetWallowLevel();
	LeaderUpdateMembers(leader,list,count);

	gplayer * pPlayer = (gplayer*)(_data.imp->_parent);
	pPlayer->object_state &= ~gactive_object::STATE_TEAMLEADER;
	pPlayer->object_state |= gactive_object::STATE_TEAM;
	pPlayer->team_id = _team.leader.id;
	_data.imp->_runner->player_in_team(2);
	GLog::log(GLOG_INFO,"用户%d成为队员(%d,%d)",pPlayer->ID.id,leader.id,_team.team_seq);

	if(pPlayer->IsInvisible())_data.imp->SendAppearToTeam();
	return true;
}

void player_team::BecomeNormal()
{
	gplayer * pPlayer = (gplayer*)(_data.imp->_parent);
	GLog::log(GLOG_INFO,"用户%d脱离队伍(%d,%d)",pPlayer->ID.id,_team.leader.id,_team.team_seq);
	if(pPlayer->IsInvisible())_data.imp->SendDisappearToTeam();

	_team_state = TEAM_NORMAL;
	_team.leader = XID(-1,-1);
	_team.member_count = 0;
	_team.team_seq = 0;
	_data.time_out = 0;
	_data.invite_map.clear();
	_data.apply_map.clear();
	pPlayer->object_state &= ~(gactive_object::STATE_TEAMLEADER | gactive_object::STATE_TEAM);
	pPlayer->team_id = 0;
	_data.imp->_runner->player_in_team(0);
}

bool player_team::LeaderAddMember(const XID & member,const agree_invite_entry  &prop,const A3DVECTOR &pos)
{
	if(_team_state != TEAM_LEADER) return false;
	if(_team.member_count < TEAM_MEMBER_CAPACITY)
	{
		gplayer* player = (gplayer*)_data.imp->_parent;
		if(player->IsInvisible()) _data.imp->SendAppearToTeamMember(member.id,prop.cs_index,prop.cs_sid);

		int index = _team.member_count;
		_team.member_list[index].id = member;
		_team.member_list[index].is_changed = true;
		_team.member_list[index].timeout =TEAM_MEMBER_TIMEOUT;
		_team.member_list[index].data = prop.data;
		_team.member_list[index].pos = pos ;
		_team.member_list[index].cls = prop.cls ;
		_team.member_list[index].gender = prop.gender ;
		_team.member_list[index].cs_index = prop.cs_index ;
		_team.member_list[index].cs_sid = prop.cs_sid ;
		_team.member_count ++;
		if(_data.cur_max_level < prop.data.level) _data.cur_max_level = prop.data.level;
		if(_data.cur_wallow_level < prop.data.wallow_level) _data.cur_wallow_level = prop.data.wallow_level;
		_team.change_flag = team_t::TEAM_PART_CHANGED;
		CalcRaceCount();
		CalcMaxLevel();
		AddRollMember(member);
		return true;
	}
	return false;
}

void player_team::SendSelfDataToMember(const team_mutable_prop & data)
{
	if(!IsInTeam()) return;
	MSG msg;
	BuildMessage(msg,GM_MSG_MEMBER_NOTIFY_DATA,XID(-1,-1),_data.imp->_parent->ID,_data.imp->_parent->pos,0,&data,sizeof(data));
	SendGroupMessage(msg);
}

void player_team::SendTeamDataToClient()
{
	switch(_team.change_flag)
	{
		case team_t::TEAM_NO_CHANGE:
			return;

		case team_t::TEAM_PART_CHANGED:
			{
				size_t member_count = _team.member_count;
				abase::vector<const member_entry *,abase::fast_alloc<> > list;
				list.reserve(member_count);
				for(size_t i = 0; i < member_count ; i++)
				{
					if(_team.member_list[i].is_changed)
					{
						list.push_back(_team.member_list + i);
						_team.member_list[i].is_changed = false;
					}
				}
				if(list.size())
				{
					_data.imp->SendTeamData(_team.leader,member_count,list.size(),list.begin());
				}
				_team.change_flag = team_t::TEAM_NO_CHANGE;
			}
			return;

		case team_t::TEAM_ALL_CHANGED:
			{
				size_t member_count = _team.member_count;
				if(member_count)
				{
					_data.imp->SendTeamData(_team.leader,member_count,_team.member_list);
				}

				for(size_t i = 0; i < member_count ; i++)
				{
					_team.member_list[i].is_changed = false;
				}
				_team.change_flag = team_t::TEAM_NO_CHANGE;
			}
			return;

		default:
			ASSERT(false);
			break;
	}

}

void player_team::SendGroupData(const void * buf, size_t size)
{
	size_t count = _team.member_count;
	int self = _data.imp->_parent->ID.id;
	for(size_t i = 0;i < count; i ++)
	{
		const member_entry &  ent = _team.member_list[i];
		if(self == ent.id.id) continue;
		send_ls_msg(ent.cs_index, ent.id.id, ent.cs_sid,buf,size);
	}
}

