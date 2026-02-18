#include "gconsignstart.hpp"
#include "dbconsigntableload.hrp"
#include "dbconsignmail.hrp"
#include "dbconsignprepost.hrp"
#include "dbconsignprecancel.hrp"
#include "dbconsignpost.hrp"
#include "dbconsignfail.hrp"
#include "dbconsignquery.hrp"
#include "dbconsignshelf.hrp"
#include "dbconsignshelfcancel.hrp"
#include "dbconsignsold.hrp"
#include "dbconsignquerybackup.hrp"
#include "gconsignstartrole.hpp"
#include "dbconsigngetrole.hrp"
#include "dbconsignrolefail.hrp"
#include "dbconsignprepostrole.hrp"

#include "post.hpp"
#include "gamepostcancel.hpp"
#include "gdeliveryserver.hpp"
#include "gwebtradeclient.hpp"
#include "newkeepalive.hpp"
#include "mapuser.h"
#include "consignmanager.h"
#include "localmacro.h"
#include "postoffice.h"
#include "blockedrole.h"
#include "mapforbid.h"
#include "forbid.hxx"
#include "sectmanager.h"
#include "namemanager.h"
#include "announceforbidinfo.hpp"
#include "crssvrteamsmanager.h"

namespace GNET
{
bool ConsignInfo::CheckStateChange(int target_state)
{
	static int init=0;
	static int matrix[10][10];
	
	if(init == 0)
	{
	memset(matrix,CHANGE_FORBID,sizeof(matrix));
	//permit from->to
	matrix[DSTATE_POST][DSTATE_SELL]=CHANGE_PERMIT;
	matrix[DSTATE_POST][DSTATE_EXPIRE]=CHANGE_PERMIT;
	matrix[DSTATE_POST][DSTATE_POST_WEB_CANCEL]=CHANGE_PERMIT;
	matrix[DSTATE_POST][DSTATE_PRE_CANCEL_POST]=CHANGE_PERMIT;
	matrix[DSTATE_POST][DSTATE_POST_FORCE_CANCEL]=CHANGE_PERMIT;
	matrix[DSTATE_POST][DSTATE_PRE_CANCEL_POST]=CHANGE_PERMIT;

//	matrix[DSTATE_PRE_POST][DSTATE_POST]=CHANGE_PERMIT;
	matrix[DSTATE_PRE_POST][DSTATE_SELL]=CHANGE_PERMIT;
	matrix[DSTATE_PRE_POST][DSTATE_POST_FAIL]=CHANGE_PERMIT;
	matrix[DSTATE_PRE_CANCEL_POST][DSTATE_POST_GAME_CANCEL]=CHANGE_PERMIT;

	matrix[DSTATE_SELL][DSTATE_POST_FORCE_CANCEL]=CHANGE_PERMIT;
	matrix[DSTATE_SELL][DSTATE_POST]=CHANGE_PERMIT;
	matrix[DSTATE_SELL][DSTATE_EXPIRE]=CHANGE_PERMIT;
	matrix[DSTATE_SELL][DSTATE_SOLD]=CHANGE_PERMIT;
	matrix[DSTATE_SELL][DSTATE_POST_WEB_CANCEL]=CHANGE_PERMIT;
	//tolerate web err
	matrix[DSTATE_POST_GAME_CANCEL][DSTATE_EXPIRE]=CHANGE_PERMIT;
	matrix[DSTATE_POST_GAME_CANCEL][DSTATE_POST_WEB_CANCEL]=CHANGE_PERMIT;
	matrix[DSTATE_POST_GAME_CANCEL][DSTATE_POST_FORCE_CANCEL]=CHANGE_PERMIT;
	matrix[DSTATE_POST_WEB_CANCEL][DSTATE_POST_GAME_CANCEL]=CHANGE_PERMIT;
	matrix[DSTATE_POST_FORCE_CANCEL][DSTATE_POST_GAME_CANCEL]=CHANGE_PERMIT;
	matrix[DSTATE_EXPIRE][DSTATE_POST_GAME_CANCEL]=CHANGE_PERMIT;

	matrix[DSTATE_POST_WEB_CANCEL][DSTATE_POST_WEB_CANCEL]=CHANGE_PERMIT;
	matrix[DSTATE_POST_FORCE_CANCEL][DSTATE_POST_FORCE_CANCEL]=CHANGE_PERMIT;
	matrix[DSTATE_EXPIRE][DSTATE_EXPIRE]=CHANGE_PERMIT;
	matrix[DSTATE_SELL][DSTATE_SELL]=CHANGE_PERMIT;//sellperiod end,check in db
	matrix[DSTATE_SOLD][DSTATE_SOLD]=CHANGE_PERMIT;//must be timestamp equal and orderid equal,check in db
	//forced
	matrix[DSTATE_PRE_CANCEL_POST][DSTATE_POST]=CHANGE_FORCED;
	
	init=1;
	}
        int ret=matrix[state][target_state];	
	if(ret != CHANGE_PERMIT && state == target_state)
		ret = CHANGE_REPEAT;
	return ret;
}

void ConsignClientRqst::SendMsg()
{
	if(rtype==POST || rtype==POSTROLE)
	{
		if(post_info==NULL)
		{
			Log::log(LOG_ERR,"ConsignClientRqst sendmsg err, GConsignDB pointer is null");
			return;
		}		      
	       	if(rtype==POST)
		{
			ConsignManager::GetInstance()->SendPost(*post_info);
		}
		else 
		{
			if(role_data != NULL)
			{
				ConsignManager::GetInstance()->SendPostRole(*post_info,*role_data);
			}
			else
			{

				DBConsignGetRole * rpc = (DBConsignGetRole *)Rpc::Call(
					RPC_DBCONSIGNGETROLE,
					DBConsignGetRoleArg(
						post_info->info.sn,
						post_info->seller_roleid
						)
					);		
				GameDBClient::GetInstance()->SendProtocol(rpc);
			}
		}
	}
	else if(rtype==POST_CANCEL)
	{
		//no post_info data
	//	if(post_info->info.consign_type != CONSIGNTYPE_ROLE)
	//	{
			ConsignManager::GetInstance()->SendCancelPost(userid,roleid,sn,game_timestamp);
	//	}
	//	else
	//	{
	//		Log::log(LOG_ERR,"ConsignClientRqst msg err, rtype=POST_CANCEL consign_type=CONSIGNTYPE_ROLE");
	//		return;
	//	}
	}
}

void ConsignNeedMail::SendMailMsg()
{
	int retcode;
	ConsignManager::ConsignMap::iterator itcon = ConsignManager::GetInstance()->CheckEntry( sn, retcode);
	ConsignManager::CategoryMap::iterator it = itcon->second;
	if(retcode != ERR_SUCCESS)
		return;
	if(!it->second->SetBusy())
	{
		DEBUG_PRINT("ConsignNeedMail::SendMailMsg err sn=%lld busy",sn);
		return;
	}
	DBConsignMail * rpc = (DBConsignMail *)Rpc::Call(
		RPC_DBCONSIGNMAIL,
		DBConsignMailArg(
			sn,
			it->second->state,
			it->second->roleid
			)
		);		
	GameDBClient::GetInstance()->SendProtocol(rpc);
	DEBUG_PRINT("ConsignNeedMail::SendMailMsg,resent sn=%lld",sn);
}

ConsignManager::~ConsignManager()
{
	for(CategoryMap::iterator it=category_map.begin(),ie=category_map.end();it!=ie;it++)
		delete it->second;
	category_map.clear();
	consign_map.clear();

	for(ClientRqstMap::iterator it=client_rqst_map.begin(),ie=client_rqst_map.end();it!=ie;it++)
		delete it->second;
	client_rqst_map.clear();
}

bool ConsignManager::Initialize()
{
	if(!LoadConfig())
	{
		Log::log(LOG_ERR, "ConsignManager load Config failed!");					
		return false;
	}
	aid = GDeliveryServer::GetInstance()->aid;
	zoneid = GDeliveryServer::GetInstance()->zoneid;
	IntervalTimer::AddTimer(this,CONSIGN_MANAGER_UPDATE_INTERVAL);	
	return true;
}

bool ConsignManager::LoadConfig()
{
	//从conf中读取游戏币寄售下限、公示期时长、寄售时长、保证金数量等
	return true;
}

void ConsignManager::OnDBConnect(Protocol::Manager * manager, int sid)
{
	/*
	if(status == ST_UNINIT && consign_open==true)
		manager->Send(sid,Rpc::Call(RPC_DBCONSIGNTABLELOAD,DBConsignTableLoadArg()));//init key=0
		*/
}

int ConsignManager::SN_notfound(int64_t sn)
{
	SoldMap::iterator it =sold_map.find(sn);
	if(it==sold_map.end())	
	{
		DBConsignQueryBackup * rpc = (DBConsignQueryBackup *)Rpc::Call(
			RPC_DBCONSIGNQUERYBACKUP,
			DBConsignQueryBackupArg(sn,0)
		);
		GameDBClient::GetInstance()->SendProtocol(rpc);
		return -1;//dont know the result yet
	}
	else
	{
	 	if(it->second.exist)
			return 2;//sn exist
		else
			return 1;//sn not exist
	}
}

int ConsignManager::GetFinishedRecord(int64_t sn, FinishedRecord & record)
{
	SoldMap::iterator it =sold_map.find(sn);
	if(it==sold_map.end())	
	{
		DBConsignQueryBackup * rpc = (DBConsignQueryBackup *)Rpc::Call(
			RPC_DBCONSIGNQUERYBACKUP,
			DBConsignQueryBackupArg(sn,0)
		);
		GameDBClient::GetInstance()->SendProtocol(rpc);
		return -1;//dont know the result yet
	}
	else
	{
	 	if(it->second.exist)
		{
			record = it->second;
			return ERR_SUCCESS;
		}
		else
			return ERR_WT_ENTRY_NOT_FOUND;//sn not exist
	}
}

bool ConsignManager::OnQueryBackup(int retcode, int64_t sn, const Octets & detail_os)
{
	if(retcode == ERR_SUCCESS){
		GConsignDB detail;
		try {
			Marshal::OctetsStream(detail_os) >> detail;
		}
		catch (...)
		{
			Log::log(LOG_ERR, "unmarshal his sn %lld error,os size %d", sn, detail_os.size());
		}
		sold_map[sn]=FinishedRecord(true, detail.orderid);//exist
	}
	else{
		sold_map[sn]=FinishedRecord(false, 0);	//sn not exist
	}
	return true;
}

void ConsignManager::AddClientRqst(int state,const ConsignInfo& info,int64_t sn, int stamp,const GConsignDB* pdetail)
{
	ConsignClientRqst::ClientRqstType type = (state == DSTATE_PRE_CANCEL_POST) ? ConsignClientRqst::POST_CANCEL : \
						 ConsignClientRqst::POST;
	if(info.itcache.consign_type == CONSIGNTYPE_ROLE)
		type = ConsignClientRqst::POSTROLE;

	//need delete when remove client_rqst_map entry
	ConsignClientRqst* prqst = new ConsignClientRqst(type,info.userid,info.roleid,sn,stamp,pdetail);
	client_rqst_map.insert(std::make_pair(sn,prqst));
}

void ConsignManager::AddMailRqst(int64_t sn)
{
	mail_rqst_map.insert(std::make_pair(sn,ConsignNeedMail(sn)));
}

bool ConsignManager::RemoveClientRqst(int64_t sn)
{
	ClientRqstMap::iterator it = client_rqst_map.find(sn);
	if(it == client_rqst_map.end())
		return false;
	delete it->second;
	client_rqst_map.erase(it);
	return true;
}

bool ConsignManager::RemoveMailRqst(int64_t sn)
{
	MailRqstMap::iterator it = mail_rqst_map.find(sn);
	if(it == mail_rqst_map.end())
		return false;
	mail_rqst_map.erase(it);
	return true;
}

void ConsignManager::AddEntry(const GConsignDB& detail)
{
//	DEBUG_PRINT("ConsignManager::AddEntry sn=%lld",detail.info.sn);
	ConsignInfo* pcinfo = new ConsignInfo(detail);//need delete when remove consign_map entry
	int64_t sn = detail.info.sn;
	int category = detail.category;
	CategoryMap::iterator cat_ret = category_map.insert(std::make_pair(category,pcinfo));
	std::pair<ConsignMap::iterator,bool> tmp = consign_map.insert(std::make_pair(sn, cat_ret));

	if(tmp.second==false)//record with equal sn in consign_map, should delete both 
	{
		Log::log( LOG_ERR, "Consign AddEntry error, dup sn,roleid=%d sn=%lld.\n",detail.seller_roleid,sn);
		delete pcinfo;
		category_map.erase(cat_ret);
		RemoveEntry(GetCInfo(tmp.first->second).roleid,sn);
		return;
	}

	if(detail.info.state != DSTATE_SOLD || detail.mail_status != SELLER_SENDED)
		role_consign_map[pcinfo->roleid].insert(sn);
	int state = detail.info.state;
	const GConsignDB* pdetail = &detail;
	if(state == DSTATE_PRE_CANCEL_POST || state == DSTATE_PRE_POST)
		AddClientRqst(detail.info.state,*pcinfo,sn,detail.game_timestamp,pdetail);
	else if(state >= DSTATE_SOLD)//tricky,all finish state be put behind this one
	{
		if(detail.info.consign_type != CONSIGNTYPE_ROLE)
			AddMailRqst(sn);
		else
			Log::log( LOG_ERR, "Consign AddEntry error, role in state=%d,roleid=%d sn=%lld.\n",state,detail.seller_roleid,sn);
	}
	if(state == DSTATE_SELL && detail.info.consign_type != CONSIGNTYPE_ROLE)
		shelfcache.InsertItem(detail.shelf_time,pcinfo);
}

bool ConsignManager::RemoveEntry(int roleid, int64_t sn)
{
	ConsignMap::iterator it = consign_map.find(sn);
	if(it == consign_map.end()) 
	{
		Log::log( LOG_ERR, "RemoveEntry not found, roleid=%d sn=%lld.\n",roleid,sn);
		return false;
	}
	ConsignInfo &info = GetCInfo(it->second);
	if(info.roleid != roleid)
	{	
		Log::log( LOG_ERR, "RemoveEntry roleid unmatch, roleid=%d info.roleid=%d,sn=%lld.\n",roleid,info.roleid,sn);
		//return false;
	}
	else
		RemoveRoleEntry(roleid,sn);
//	RemoveCacheEntry(info.itcache);
	shelfcache.EraseItem(info.shelf_time,info.itcache.sn);
	CategoryMap::iterator itc = it->second;
	delete itc->second;
	category_map.erase(itc);
	consign_map.erase(it);
	RemoveMailRqst(sn);
	return true;
}

void ConsignManager::OnRoleLogout(int roleid)
{
/*	if(status == ST_UNINIT) return;
	DEBUG_PRINT("ConsignManager::OnRoleLogout: roleid=%d", roleid);
	RoleConsignMap::iterator rit = role_consign_map.find(roleid);
	if(rit == role_consign_map.end())
		return;
	const SNSet& set = rit->second; 
	SNSet::iterator it,ite=set.end();
	for(it=set.begin();it!=ite;it++)
	{
		RemoveCacheEntry(*it);
	}
*/
}

void ConsignManager::OnDBLoad(std::vector<GConsignDB>& list, bool finish)
{
	if(status != ST_INITING) return;
	

	std::vector<GConsignDB>::iterator ci,cend=list.end();
	for(ci=list.begin(); ci!=cend; ++ci)
	{
		NameManager::GetInstance()->FindName(ci->seller_roleid, ci->seller_name);
		NameManager::GetInstance()->FindName(ci->buyer_roleid, ci->buyer_name);
		AddEntry(*ci);
	}
	
	if(finish)
	{
		Log::formatlog("consign","init consign manager: total=%d, roleconsignmap size=%d", consign_map.size(),\
			role_consign_map.size()); 
		status = ST_INITED;	
	}	
}

bool ConsignManager::Update()
{
	if(!IsInited())
	{
		if (status == ST_UNINIT && GameDBClient::GetInstance()->IsConnect()
				&& consign_open==true && NameManager::GetInstance()->IsInit())
		{
			GameDBClient::GetInstance()->SendProtocol(Rpc::Call(RPC_DBCONSIGNTABLELOAD,DBConsignTableLoadArg()));//init key=0
			status = ST_INITING;
		}
		LOG_TRACE("ConsignManager Update status %d", status);
		return true;
	}
	int delta=CONSIGN_MANAGER_UPDATE_INTERVAL;
	tick+=delta;
	if(tick >= CONSIGN_MANAGER_HEARTBEAT_INTERVAL)
	{
		tick = 0;
		DEBUG_PRINT("send NewKeepAlive");
		GWebTradeClient::GetInstance()->SendProtocol(NewKeepAlive(0));
	}
	shelfcache.UpdateMe(delta);
	ClientRqstMap::iterator it,ite=client_rqst_map.end();
	for(it=client_rqst_map.begin();it!=ite;++it)
		it->second->Update(delta);

	MailRqstMap::iterator itm,itme=mail_rqst_map.end();
	for(itm=mail_rqst_map.begin();itm!=itme;++itm)
		itm->second.Update(delta);

	return true;	
}

int ConsignManager::GetAttendListNum(int roleid)
{
	RoleConsignMap::iterator it = role_consign_map.find(roleid);	
	return (it==role_consign_map.end() ? 0 : it->second.size());
}

bool ConsignManager::ClearBusy(int64_t sn)
{
	if(!IsInited()) return false;

	ConsignMap::iterator it = consign_map.find(sn);
	if(it == consign_map.end()) 
	{
		Log::log(LOG_ERR, "ConsignManager ClearBusy() sn not found! sn(%lld)",sn);
		return false;
	}
	ConsignInfo & info = GetCInfo(it->second);
	if(!info.ClearBusy())
	{
		Log::log(LOG_INFO, "ConsignManager ClearBusy() while not busy! sn(%lld) roleid(%d) state(%d)",sn,info.roleid,info.state);
		return false;
	}
	return true;
}

bool ConsignManager::SetBusy(int64_t sn)
{
	if(!IsInited()) return false;

	ConsignMap::iterator it = consign_map.find(sn);
	if(it == consign_map.end()) 
	{
		Log::log(LOG_ERR, "ConsignManager SetBusy() sn not found! sn(%lld)",sn);
		return false;
	}
	ConsignInfo & info = GetCInfo(it->second);
	if(!info.SetBusy())
	{
		Log::log(LOG_ERR, "ConsignManager SetBusy() while busy! sn(%lld) roleid(%d) state(%d)",sn,info.roleid,info.state);
		return false;
	}
	return true;
}

int ConsignManager::TryPrePost(const GConsignStart& consign,int ip,PlayerInfo& info)
{
	if(!IsInited())	return ERR_WT_UNOPEN;

	//todo检查封禁
	
/*	if(consign.money > CONSIGN_MONEY_MAX || consign.margin > CONSIGN_MARGIN_MAX || consign.margin<0 || consign.money<0 \
			consign.price > CONSIGN_PRICE_MAX || consign.price<0)
		return ERR_WT_MONEY_OR_MARGIN_ERR;*/
		
	std::set<int>::iterator it=consign_starting_set.find(consign.roleid);
	if(it!=consign_starting_set.end())	
		return ERR_WT_GCONSIGNSTART_PROCESSING;

	if(GetAttendListNum(consign.roleid) >= CONSIGN_ITEM_MAX)
		return ERR_WT_TOO_MANY_ATTEND_SELL;
	
	if(PostOffice::GetInstance().GetMailBoxSize(consign.roleid) >= 64)//sys mail limit
		return ERR_WT_MAILBOX_FULL;
	
	//物品本身是否可交易在gamedbd检查
	DBConsignPrePost* rpc=(DBConsignPrePost*)Rpc::Call( 
		RPC_DBCONSIGNPREPOST,
		DBConsignPrePostArg(
			consign.roleid,
			info.userid,
			info.name,
			consign.margin,
			consign.item_id,
			consign.item_cnt,
			consign.item_idx,
			consign.money,
			consign.price*100,//change to cents
			consign.shelf_period,
			consign.sell_to_rolename,
			consign.consign_type,
			ip,
			consign.category
		)
	);
	rpc->save_linksid=info.linksid;
	rpc->save_localsid=info.localsid;
	rpc->save_gsid=info.gameid;
	GameDBClient::GetInstance()->SendProtocol(rpc);
	consign_starting_set.insert(consign.roleid);
	DEBUG_PRINT("ConsignManager::TryPrePost ok, roleid=%d",consign.roleid);
	return ERR_SUCCESS;
}

int ConsignManager::TryPrePostRole(const GConsignStartRole& consign,int ip,PlayerInfo& info)
{
	if(!IsInited())	return ERR_WT_UNOPEN;
	if(!IsConsignRoleOpen()) return ERR_WT_UNOPEN;
	//检查封禁
	GRoleForbid     forbid;                        
	if( ForbidLogin::GetInstance().GetForbidLogin( info.userid, forbid ) )
		return ERR_WT_SELL_ROLE_WHILE_FORBID;
	if( ForbidRoleLogin::GetInstance().GetForbidRoleLogin( consign.roleid, forbid ) )
		return ERR_WT_SELL_ROLE_WHILE_FORBID;
	if( ForbidConsign::GetInstance().GetForbidConsign( consign.roleid, forbid ) )
	{
		GDeliveryServer::GetInstance()->Send(info.linksid,AnnounceForbidInfo(info.userid, info.localsid, forbid));
		return ERR_WT_SELL_ROLE_WHILE_FORBID;
	}
	ForbidUserTalk::GetInstance().GetForbidUserTalk(info.userid, forbid);
	if(forbid.type == Forbid::FBD_FORBID_TALK)
		return ERR_WT_SELL_ROLE_WHILE_FORBID;

	//check family,faction,spouse,sect
	if(info.familyid || info.factionid)
		return ERR_WT_SELL_ROLE_HAS_FAC_RELATION;

	int role_teamid = CrssvrTeamsManager::Instance()->GetRoleTeamId(consign.roleid);
	if(role_teamid > 0)
		return ERR_WT_SELL_ROLE_HAS_CRSSVRTEAMS_RELATION;

	if(info.sectid)
	{
		if(info.sectid != consign.roleid)
			return ERR_WT_SELL_ROLE_HAS_SECT_RELATION;
		else
		{
			GSectInfo *sectinfo = NULL;	
			sectinfo = SectManager::Instance()->FindSect(info.sectid);
			if(sectinfo && sectinfo->disciples.size()>0)
				return ERR_WT_SELL_ROLE_HAS_SECT_RELATION;
		}
	}
	GRoleInfo* pinfo = RoleInfoCache::Instance().Get(consign.roleid);
	if(!pinfo || pinfo->spouse!=0)
		return ERR_WT_SELL_ROLE_HAS_SPOUSE_RELATION; 

	//check if has item consigning
	if(GetAttendListNum(consign.roleid) > 0)
		return ERR_WT_SELL_ROLE_WHILE_ITEM_SELLING;
	
	std::set<int>::iterator it=consign_starting_set.find(consign.roleid);
	if(it!=consign_starting_set.end())	
		return ERR_WT_GCONSIGNSTART_PROCESSING;

	
/*	DBConsignPrePost* rpc=(DBConsignPrePost*)Rpc::Call( 
		RPC_DBCONSIGNPREPOST,
		DBConsignPrePostArg(
			consign.roleid,
			info.userid,
			info.name,
			consign.margin,
			0,//consign.item_id,
			0,//consign.item_cnt,
			0,//consign.item_idx,
			0,//consign.money,
			consign.price*100,//change to cents
			consign.shelf_period,
			consign.sell_to_rolename,
			consign.consign_type,
			ip,
			0//consign.category
		)
	);*/
		DBConsignPrePostRoleArg arg(
			consign.roleid,
			info.userid,
			info.name,
			consign.margin,
			consign.token_item_id,
			consign.token_item_cnt,
			consign.token_item_idx,
			//0,//consign.money,
			consign.price*100,//change to cents
			consign.shelf_period,
			consign.sell_to_rolename,
			consign.consign_type,
			ip,
			consign.category,
			GConsignGsRoleInfo()
		);
//	rpc->save_linksid=info.linksid;
//	rpc->save_localsid=info.localsid;
//	rpc->save_gsid=info.gameid;

	if(RoleTaskManager::GetInstance()->ConsignRole(arg)!=0)
		return ERR_WT_STATEERR;

	consign_starting_set.insert(consign.roleid);
	DEBUG_PRINT("ConsignManager::TryPrePostRole roleid=%d",consign.roleid);
	return ERR_SUCCESS;
}

int ConsignManager::OnDBPrePost(const GConsignDB& detail)
{
	if(!IsInited()){
		DEBUG_PRINT("ConsignManager::OnDBPrePost err sn=%lld,manager not inited",detail.info.sn);
		return ERR_WT_UNOPEN;
	}
	ClearConsignStarting(detail.seller_roleid);
/*	if(consign_starting_set.erase(detail.seller_roleid)==0)
	{
		Log::log(LOG_ERR,"OnDBPrePost,role not in consign_starting_set. sn=%lld,roleid=%d",\
				detail.info.sn,detail.seller_roleid);
	}*/
	AddEntry(detail);
	SendPost(detail);
	DEBUG_PRINT("ConsignManager::OnDBPrePost ok, roleid=%d,sn=%lld",detail.seller_roleid,detail.info.sn);
	return ERR_SUCCESS;
}

int ConsignManager::TryPreCancelPost(int roleid, int64_t sn, PlayerInfo& ui)
{
	int errcode;
        ConsignMap::iterator it = CheckEntry(sn,errcode);
        if(errcode!=ERR_SUCCESS)
                return errcode;
	it = CheckRqst(sn,DSTATE_PRE_CANCEL_POST,errcode);
	if(errcode!=ERR_SUCCESS)
		return errcode;
	ConsignInfo& info = GetCInfo(it->second);
	if(info.itcache.consign_type == CONSIGNTYPE_ROLE)
	{
		info.ClearBusy();
		return ERR_WT_INVALID_ARGUMENT;
	}

//	if(PostOffice::GetInstance().GetMailBoxSize(roleid) >= SYS_MAIL_LIMIT) return ERR_WT_MAILBOX_FULL;
	if(info.roleid != roleid)
		return ERR_WT_SN_ROLEID_MISMATCH;

	DBConsignPreCancel * rpc = (DBConsignPreCancel *)Rpc::Call(
			RPC_DBCONSIGNPRECANCEL,
			DBConsignPreCancelArg(
				sn,
				roleid//,0
			)
		);	
	rpc->save_linksid=ui.linksid;
	rpc->save_localsid=ui.localsid;
	GameDBClient::GetInstance()->SendProtocol(rpc);
	DEBUG_PRINT("ConsignManager::TryPreCancelPost ok sn=%lld,roleid=%d",sn,roleid);
	return ERR_SUCCESS;	
}

int ConsignManager::OnDBPreCancelPost(int roleid, int64_t sn,int stamp)
{
	int state = DSTATE_PRE_CANCEL_POST;
	int errcode;
	ConsignMap::iterator it = CheckDBRes(sn,0,state,errcode,false);
	if(errcode!=ERR_SUCCESS)
		return errcode;
	ConsignInfo& info = GetCInfo(it->second);
	//if(info.roleid != roleid) return false;

	info.state = state;
	AddClientRqst(state,info,sn,stamp);
	SendCancelPost(info.userid,info.roleid,sn,stamp);
	DEBUG_PRINT("ConsignManager::OnDBPreCancelPost ok sn=%lld",sn);
	return ERR_SUCCESS;
}

bool ConsignManager::ClientCanSee(ConsignInfo& info,int now)
{
//	return true;//to delete
	if(info.buyerrid==0 && info.state == DSTATE_SELL)
	{	
		if(info.itcache.show_endtime > now || info.itcache.sell_endtime > now)
			return true;
		else
			info.TimeoutUpdateState(DSTATE_POST);//update ConsignMap
	}
	return false;
}

bool ConsignShelf::EraseItem(int shelf_time,int64_t sn)
{
	DEBUG_PRINT("ConsignShelf::EraseItem shelf_time=%d sn=%lld",shelf_time,sn);
	std::pair<Sheit,Sheit> pit=shelfmap.equal_range(shelf_time);
	for(Sheit it=pit.first;it!=pit.second;++it)
	{
		if(it->second->itcache.sn==sn)
		{	
			updatetime=Timer::GetTime();
			shelfmap.erase(it);
			DEBUG_PRINT("ConsignShelf::EraseItem success");
			return true;
		}
	}
	return false;
}

void ConsignShelf::UpdateShelf()
{
	int now=Timer::GetTime();
	DEBUG_PRINT("ConsignShelf::UpdateShelf now=%d",now);
	Sheit sit,site=shelfmap.end();
	for(sit=shelfmap.begin();sit!=site;)
	{
		if(sit->second->state!=DSTATE_SELL||sit->second->itcache.sell_endtime<now)
		{
	//		DEBUG_PRINT("ConsignShelf::UpdateShelf erase item sell_endtime=%d state=%d now=%d",sit->second->itcache.sell_endtime,sit->second->state,now);
			shelfmap.erase(sit++);
		}
		else
			++sit;
	}
	updatetime=now;
	return;
}

bool ConsignShelf::InsertItem(int shelf_time,ConsignInfo* pcinfo)
{
	int now=Timer::GetTime();
	DEBUG_PRINT("ConsignShelf::InsertItem shelf_time=%d old=%d sn=%lld",shelf_time,pcinfo->shelf_time,pcinfo->itcache.sn);
	if( ConsignManager::ClientCanSee(*pcinfo,now))
	{
		updatetime = now;
		if(EraseItem(pcinfo->shelf_time,pcinfo->itcache.sn))
			DEBUG_PRINT("ConsignShelf::InsertItem erase old shelf_time=%d sn=%lld",pcinfo->shelf_time,pcinfo->itcache.sn);
		shelfmap.insert(std::make_pair(shelf_time,pcinfo));
	}
	return true;
}

void ConsignShelf::GetPage(std::vector<GConsignListNode>& nodes,int page)
{
	if(page<firstpageindex || page>LastPageIndex())
		return;
	Sherit sit=shelfmap.rbegin();
	std::advance(sit,itemperpage*page);
	int now=Timer::GetTime();
	Sherit site=shelfmap.rend();
	GConsignListNode tmp;
	for(int i=0;sit!=site && i<itemperpage;++sit,i++)
	{
		ConsignManager::GetListNode(tmp,sit->second);
		nodes.push_back(tmp);
//		nodes.push_back(ConsignManager::GetListNode(sit->second));
		if(!ConsignManager::ListAllSetToClientState(nodes.back().info,now))
			;//DEBUG_PRINT("ConsignShelf::GetPage ListAllSetToClientState err");
	}
	return;
}
/*
int ConsignManager::GetConsignListRole(std::vector<GConsignListRoleNode>& nodes,int& page)
{
	if(!IsInited()) return ERR_WTC_UNOPEN;
	
	if(page<FIRSTPAGE && page!=LASTPAGE)
		return ERR_WTC_LISTALL_ARG_ERR;
	
//	timestamp = shelfcache.GetUpdateTime();
	int totalpage = shelfcache.TotalPage();
	if(totalpage==0)
	{
		page=FIRSTPAGE;
		return ERR_WTC_LISTALL_FINISH;
	}
	
	int ret = ERR_SUCCESS; 
	int lastpageindex = shelfcache.LastPageIndex();
	if(page==LASTPAGE || page>lastpageindex)
	{
		page=lastpageindex;
		ret = ERR_WTC_LISTALL_FINISH;
	}
	else if(page==lastpageindex)
	{
		ret = ERR_WTC_LISTALL_FINISH;
	}
	shelfcache.GetPage(nodes,page);
	DEBUG_PRINT("ConsignManager::GetConsignListRole ret=%d,page=%d,itemnum=%d",ret,page,nodes.size());
	return ret;
}
*/
int ConsignManager::GetConsignListLargeCategory(std::vector<GConsignListNode>& nodes,int& page,int& timestamp)
{
	if(!IsInited()) return ERR_WTC_UNOPEN;
	
	if(page<FIRSTPAGE && page!=LASTPAGE)
		return ERR_WTC_LISTALL_ARG_ERR;
//	DEBUG_PRINT("ConsignManager::GetConsignListLargeCategory,page=%d,shelfsize=%d",page,shelfcache.size());
	
	timestamp = shelfcache.GetUpdateTime();
	int totalpage = shelfcache.TotalPage();
	if(totalpage==0)
	{
		page=FIRSTPAGE;
		return ERR_WTC_LISTALL_FINISH;
	}
	
	int ret = ERR_SUCCESS; 
	int lastpageindex = shelfcache.LastPageIndex();
	if(page==LASTPAGE || page>lastpageindex)
	{
		page=lastpageindex;
		ret = ERR_WTC_LISTALL_FINISH;
	}
	else if(page==lastpageindex)
	{
		ret = ERR_WTC_LISTALL_FINISH;
	}
	shelfcache.GetPage(nodes,page);
	DEBUG_PRINT("ConsignManager::GetConsignListLargeCategory ret=%d,page=%d,itemnum=%d",ret,page,nodes.size());
	return ret;
}

int ConsignManager::GetConsignListRoleWrap(int roleid,std::vector<GConsignListRoleNode>& nodes,int& page,char direction,int category,int64_t oldsn)
{
	if(!IsConsignRoleOpen()) return ERR_WT_UNOPEN;
	return GetConsignList(roleid,nodes,page,direction,category,oldsn);
}

int ConsignManager::GetConsignListWrap(int roleid,std::vector<GConsignListNode>& nodes,int& page,char direction,int category,int64_t oldsn)
{
	return GetConsignList(roleid,nodes,page,direction,category,oldsn);
}

template <class T>
int ConsignManager::GetConsignList(int roleid,std::vector<T>& nodes,int& page,char direction,int category,int64_t oldsn)
{
	if(!IsInited()) return ERR_WTC_UNOPEN;
	
	if((direction != FORWARD && direction != BACKWARD) || page <LASTPAGE)
		return ERR_WTC_LISTALL_ARG_ERR;

	//get searching range 
	Cate_it cat_begin=category_map.lower_bound(category);
	Cate_it cat_end=category_map.upper_bound(category);
	if(cat_begin==cat_end)
	{
		page=FIRSTPAGE;
		return ERR_WTC_LISTALL_FINISH;
	}
	PageInfo re(cat_begin,false,-1);
	std::vector<Cate_it> nodesit;
	nodesit.reserve(PAGECOUNT);
	std::vector<Cate_it>::iterator vit,vite;
	int now=Timer::GetTime();

	//use a shortcut to the start point
	ConsignMap::iterator it;
	bool shortcut=false;
	if(page!=FIRSTPAGE && page!=LASTPAGE && oldsn!=NO_SHORTCUT)
	{
		it= consign_map.find(oldsn);
		if(it!=consign_map.end() && (it->second)!=category_map.end() && (it->second)->first==category)
			shortcut=true;
	}
	if(shortcut)
	{
		DEBUG_PRINT("ConsignManager::GetConsignList shortcut oldsn=%lld,roleid=%d",oldsn,roleid);	
		if(direction == FORWARD)
		{
			Cate_it start=it->second;
			start++;
			re=GetNextPage(nodesit,start,cat_end,now);
			T tmp;	
			for(vit=nodesit.begin(),vite=nodesit.end();vit!=vite;vit++)
			{
				GetListNode(tmp,(*vit)->second);
				nodes.push_back(tmp);
	//			nodes.push_back(GetListNode((*vit)->second));
				if(!ListAllSetToClientState(nodes.back().info,now))
					DEBUG_PRINT("ConsignManager::GetConsignList ListAllSetToClientState err");
			}
			if(re.finish)
				return ERR_WTC_LISTALL_FINISH;
			else
				return ERR_SUCCESS;
		}
		else //(direction == BACKWARD)
		{
			Cate_rit rit(it->second);//yes, just from the node before oldsn
			Cate_rit rite(cat_begin);
			GetPrevPage(nodes,rit,rite,now);
		}
		return ERR_SUCCESS;
	}

	//no shortcut, calc the page
	int cur=-1;
	do{
		nodesit.clear();
		re=GetNextPage(nodesit,re.next,cat_end,now);
		cur++;
	}while(re.finish!=true && cur!=page);//tricky, cur dodge from LASTPAGE
	page = cur;//tell client lastpage pagenumber if reach category end 
	T tmp;
	for(vit=nodesit.begin(),vite=nodesit.end();vit!=vite;vit++)
	{
		GetListNode(tmp,(*vit)->second);
		nodes.push_back(tmp);
//		nodes.push_back(GetListNode((*vit)->second));
		if(!ListAllSetToClientState(nodes.back().info,now))
			DEBUG_PRINT("ConsignManager::GetConsignList ListAllSetToClientState err");
	}
	if(re.finish)
		return ERR_WTC_LISTALL_FINISH;
	else
		return ERR_SUCCESS;
}

ConsignManager::PageInfo ConsignManager::GetNextPage(std::vector<Cate_it>& nodes,Cate_it beginning,Cate_it end,int now)
{
	PageInfo re(end,true,-1);	
	if(beginning == end)
	{
		re.finish = true;
		return re;
	}
	Cate_it it=beginning;
	for(int count=0;count<PAGECOUNT && it!=end;it++)	
	{
		if(ClientCanSee(*(it->second),now))
		{
			nodes.push_back(it);
			count++;
		}
	}
	if(it==end)
		re.finish = true;
	else
		re.finish = false;
	re.next= it;
	re.retcode = 0;
	return re;
}

template <class T>
int ConsignManager::GetPrevPage(std::vector<T>& nodes,Cate_rit beginning,Cate_rit end,int now)
{
	if(beginning == end)
		return -1;
	Cate_rit rit =beginning;
	std::vector<CategoryMap::reverse_iterator> itvec;
	for(int count=0;count<PAGECOUNT && rit!=end && rit!=category_map.rend();rit++)
	{
		if(ClientCanSee(*(rit->second),now))
		{
			itvec.push_back(rit);
			count++;
		}
	}
	std::vector<Cate_rit>::reverse_iterator vit,vite=itvec.rend();
	T tmp;
	for(vit=itvec.rbegin();vit!=vite;vit++)
	{
		GetListNode(tmp,(*vit)->second);
		nodes.push_back(tmp);
//		nodes.push_back(GetListNode((*vit)->second));
		if(!ListAllSetToClientState(nodes.back().info,now))
			DEBUG_PRINT("ConsignManager::GetPrevPage ListAllSetToClientState err");
	}
	return 0;
}

int ConsignManager::GetItemObj(int64_t sn,GRoleInventory& obj)
{
	if(!IsInited())	return ERR_WTC_UNOPEN;

	ConsignMap::iterator it = consign_map.find(sn);
	if(it != consign_map.end())
	{
		obj = GetCInfo(it->second).obj;		
		return ERR_SUCCESS;
	}
	else
		return ERR_WTC_ENTRY_NOT_FOUND;
}

bool ConsignManager::GetRoleConsignList(int roleid,std::vector<GConsignItem>& items,int localsid)
{
	if(!IsInited())	return false;

	RoleConsignMap::iterator r_it = role_consign_map.find(roleid);
	if(r_it == role_consign_map.end()) return true;
	
	SNSet& sn_set = r_it->second;
	SNSet::iterator s_it = sn_set.begin(),s_ite = sn_set.end();
	std::vector<int64_t> cachemiss;
	for( ; s_it != s_ite; ++s_it)
	{
		ConsignMap::iterator it = consign_map.find(*s_it);
		if(it != consign_map.end())
		{
			items.push_back((GetCInfo(it->second).itcache));
		}
		else
			cachemiss.push_back(*s_it);
	}
	if(cachemiss.size() == 0)
	{
		DEBUG_PRINT("GetRoleConsignList cache hitall,list size=%d",items.size());
		SetToClientState(items);
		return true;
	}
	else
	{
		Log::log( LOG_ERR, "GetRoleConsignList cache miss ,roleid=%d",roleid);
		return false;
	}
}

void ConsignManager::OnDBConsignQuery(int roleid,std::vector<GConsignItem>& items)
{
}

bool ConsignManager::ListAllSetToClientState(GConsignItem& items,int now)
{
	if(items.state!= DSTATE_SELL)
		return false;
	if(items.show_endtime > now)
		items.state = ConsignInfo::STATE_SHOW;
	else if(items.sell_endtime > now)
		items.state = ConsignInfo::STATE_SELL;
	else //sell end
	{
		items.state = ConsignInfo::STATE_SELL;//client display selltime '--'
		return false;
	}
	return true;
}

void ConsignManager::SetToClientState(std::vector<GConsignItem>& items)
{
	int now = Timer::GetTime();
	std::vector<GConsignItem>::iterator it,ie=items.end();
	for(it=items.begin();it!=ie;it++)
	{
		ConsignMap::iterator itcon = consign_map.find(it->sn);
		if(itcon == consign_map.end()) 
		{
			//log err
			DEBUG_PRINT("SetToClientState err, sn=%lld not in consign_map",it->sn);
			continue;
		}
		CategoryMap::iterator itc = itcon->second;
		
		switch(itc->second->state)
		{
		case DSTATE_PRE_POST:					
			it->state = ConsignInfo::STATE_PRE_POST;
			break;
		case DSTATE_POST:					
			it->state = ConsignInfo::STATE_POST;
			it->price = 0;
			break;
		case DSTATE_PRE_CANCEL_POST:				
			it->state = ConsignInfo::STATE_PRE_CANCEL_POST;
			break;
		case DSTATE_SELL:						
			if(it->show_endtime > now)
				it->state = ConsignInfo::STATE_SHOW;
			else if(it->sell_endtime > now)
				it->state = ConsignInfo::STATE_SELL;
			else //sell end
			{
				itc->second->TimeoutUpdateState(DSTATE_POST);//update ConsignMap
				it->state = ConsignInfo::STATE_POST;
			}
			break;
		case DSTATE_SOLD:
			it->state = ConsignInfo::STATE_SOLD;
			break;
		case DSTATE_EXPIRE:
			it->state = ConsignInfo::STATE_EXPIRE;
			break;
		case DSTATE_POST_FAIL:
			it->state = ConsignInfo::STATE_POST_FAIL;
			break;
		case DSTATE_POST_WEB_CANCEL:
			it->state = ConsignInfo::STATE_POST_CANCEL;
			break;
		case DSTATE_POST_GAME_CANCEL:
			it->state = ConsignInfo::STATE_POST_CANCEL;
			break;
		case DSTATE_POST_FORCE_CANCEL:
			it->state = ConsignInfo::STATE_POST_FORCE_CANCEL;
			break;
		default:
			 Log::log( LOG_ERR, "SetToClientState state=%d err,sn=%lld",it->state,it->sn);
		}
	}
}

void ConsignManager::SendPost(const GConsignDB& detail)
{
	DEBUG_PRINT("ConsignManager::SendPost sn=%lld timestamp=%d buyer=%d buyeruid=%d",detail.info.sn,detail.game_timestamp,detail.buyer_roleid,detail.buyer_userid);
	Post p;
	BuildPost(p,detail);
	GWebTradeClient::GetInstance()->SendProtocol(p);
}

void ConsignManager::BuildPost(Post & p,const GConsignDB& detail)
{
	p.aid = aid;
	p.zoneid = zoneid;
	p.seller.roleid = detail.seller_roleid;
	p.seller.userid = detail.seller_userid;
	p.seller.rolename = detail.seller_name;
	p.buyer.roleid = detail.buyer_roleid;
	p.buyer.userid = detail.buyer_userid; 
	p.buyer.rolename = detail.buyer_name;
	p.sn = detail.info.sn;
	p.price = detail.info.price;//cent in web
	p.shelf = 1;//always shelf on post
	p.posttype = detail.info.consign_type;
	p.loginip = detail.loginip;
	int64_t tmp = detail.post_time;
	p.time.actiontime = tmp*1000;
	p.time.showperiod = DEFAULT_SHOW_PERIOD;
	p.time.sellperiod =  detail.info.sell_endtime; //(detail.info.sell_endtime- detail.post_time/1000 )/60;//check it
	////寄售前sell_endtime保存的是	要上架time min
	
	p.time.postperiod = 0;		//非0时由游戏设置寄售时长 0时由平台设置寄售时长
	Marshal::OctetsStream os;
	if(detail.info.consign_type == CONSIGNTYPE_MONEY)
	{
		GRoleInventory m;
		m.id = 0;
		m.pos = detail.category;
		m.count = detail.info.money/10000;
		os << m;
		p.num = detail.info.money/10000;
		p.info.detail = os;
	}
	else if(detail.info.consign_type == CONSIGNTYPE_ITEM)
	{
		GRoleInventory t=detail.item;
		t.pos = detail.category;//to send category to web
		os << t;
		p.num = detail.info.item_cnt;
		p.info.detail = os;
	}
	p.backup = os;
	p.timestamp = detail.game_timestamp; //not need timestamp
}

void ConsignManager::SendPostRole(const GConsignDB& detail,const GConsignRole& roledetail)
{
	DEBUG_PRINT("ConsignManager::SendPostRole sn=%lld timestamp=%d buyer=%d buyeruid=%d",detail.info.sn,detail.game_timestamp,detail.buyer_roleid,detail.buyer_userid);
	Post p;
	BuildPost(p,detail);

	if(detail.info.consign_type == CONSIGNTYPE_ROLE)
	{
		Marshal::OctetsStream os;
		os << roledetail;
		p.info.detail = os;
		p.backup = Octets(0);
		p.num = 1;
	}
	GWebTradeClient::GetInstance()->SendProtocol(p);
}

int ConsignManager::RecvPostRe(bool success, int userid, int64_t sn, int postperiod, int showperiod, int sellperiod,int commodity_id)
{
	if(!IsInited())	return 0;
	
	RemoveClientRqst(sn);
	ConsignMap::iterator it = consign_map.find(sn);
	if(it == consign_map.end()) 
	{
		Log::log(LOG_ERR,"ConsignManager::RecvPostRe sn=%lld not found",sn);
		return -1;
	}

	ConsignInfo& info = GetCInfo(it->second);
	if(info.userid != userid) 
	{
		Log::log(LOG_WARNING,"ConsignManager::RecvPostRe userid inconsist info.userid=%d userid=%d",info.userid,userid);
	//	return -1;
	}
		
	if(info.state != DSTATE_PRE_POST)
	{
		Log::log(LOG_ERR,"ConsignManager::RecvPostRe state=%d err",info.state);
		return -1;
	}

	if(!success)
	{
		if(info.itcache.consign_type != CONSIGNTYPE_ROLE)
		{
			DBConsignFail * rpc = (DBConsignFail *)Rpc::Call(
				RPC_DBCONSIGNFAIL,
				DBConsignFailArg(
					sn,	
					info.roleid,
					DSTATE_POST_FAIL,
					0
				)
			);
			//no need to assign value to rpc's cached variables	
			GameDBClient::GetInstance()->SendProtocol(rpc);
		}
		else
		{
			if(!IsConsignRoleOpen()) return -1;
			DBConsignRoleFail * rpc = (DBConsignRoleFail *)Rpc::Call(
				RPC_DBCONSIGNROLEFAIL,
				DBConsignRoleFailArg(
					sn,	
					info.roleid,
					DSTATE_POST_FAIL,
					0//timestamp no use
				)
			);
			GameDBClient::GetInstance()->SendProtocol(rpc);
			DEBUG_PRINT("ConsignManager::RecvPostRe roleconsign fail\n");
		}
		return 0;	
	}

	if(sellperiod <= 0 || postperiod<=0 || showperiod<0)
	{
		Log::log(LOG_ERR,"ConsignManager::RecvPostRe sellperiod=%d postperiod=%d showperiod=%d err",sellperiod,postperiod,showperiod);
	}
	DBConsignPost * rpc=(DBConsignPost*)Rpc::Call(
			RPC_DBCONSIGNPOST,
			DBConsignPostArg(
				sn,
				info.roleid,
				sellperiod,
				showperiod,
				postperiod,
				commodity_id,
				POST_NORMAL
				)
			);		
	rpc->commodityid=commodity_id;
	rpc->consign_type=info.itcache.consign_type;
	GameDBClient::GetInstance()->SendProtocol(rpc);
	return 0;
}

int ConsignManager::OnDBPost(int roleid, int64_t sn, int reason,const GConsignItem &item,int commodityid)
{

	int state;
	if(reason == POST_NORMAL)
		state=DSTATE_SELL;
	else
		state=DSTATE_POST;
	int errcode;
	ConsignMap::iterator it = CheckDBRes(sn,0,state,errcode,false);
	if(errcode!=ERR_SUCCESS)
		return errcode;
	ConsignInfo& info = GetCInfo(it->second);

//	if(info.roleid != roleid)return false;

	if(!(info.state == state && state == DSTATE_SELL))
	{
		info.state = state;
		info.commodityid=commodityid;
		UpdateCacheEntry(item,info);
		if(reason == POST_NORMAL && info.itcache.consign_type != CONSIGNTYPE_ROLE)
			shelfcache.InsertItem(info.shelf_time,&info);
	}
	return ERR_SUCCESS;
}

void ConsignManager::SendCancelPost(int uid,int rid, int64_t sn,int stamp)
{
	SetBusy(sn);
	DEBUG_PRINT("ConsignManager::SendCancelPost sn=%lld timestamp=%d",sn,stamp);	
	GamePostCancel p;
	p.userid = uid;
	p.roleid = rid;
	p.sn = sn;
	p.timestamp = stamp;//not need
	GWebTradeClient::GetInstance()->SendProtocol(p);
}

int ConsignManager::RecvCancelPostRe(bool success, int userid, int64_t sn)
{
	if(!IsInited())	return ERR_WT_UNOPEN;
	RemoveClientRqst(sn);
	ConsignMap::iterator it = consign_map.find(sn);
	if(it == consign_map.end()) 
	{
		DEBUG_PRINT("ConsignManager::RecvCancelPostRe err sn=%lld,not found",sn);
		return ERR_WT_ENTRY_NOT_FOUND;
	}
	
	ConsignInfo& info = GetCInfo(it->second);
	if(!info.ClearBusy())
		DEBUG_PRINT("ConsignManager::RecvCancelPostRe err sn=%lld,not busy",sn);

	if(info.state != DSTATE_PRE_CANCEL_POST)
	{
		DEBUG_PRINT("ConsignManager::RecvCancelPostRe state=%d err sn=%lld\n",info.state,sn);
	       	return ERR_WT_STATEERR;
	}
	if(info.itcache.consign_type == CONSIGNTYPE_ROLE)
	{
		DEBUG_PRINT("ConsignManager::RecvCancelPostRe consign_type=%d err sn=%lld\n",info.itcache.consign_type,sn);
	       	return ERR_WT_INVALID_ARGUMENT;
	}

	if(!success)
	{
		//return post
		info.SetBusy();
		DBConsignPost * rpc=(DBConsignPost*)Rpc::Call(
				RPC_DBCONSIGNPOST,
				DBConsignPostArg(sn,info.roleid,0,0,0,0,POST_RESTORE)
				);		
		rpc->commodityid=info.commodityid;
		rpc->consign_type=info.itcache.consign_type;
		GameDBClient::GetInstance()->SendProtocol(rpc);
		return ERR_SUCCESS;	
	}
	
	info.SetBusy();
	DBConsignFail * rpc = (DBConsignFail *)Rpc::Call(
		RPC_DBCONSIGNFAIL,
		DBConsignFailArg(
			sn,	
			info.roleid,
			DSTATE_POST_GAME_CANCEL,
			0
			)
		);		
	//no need to assign value to rpc's cached variables	
	GameDBClient::GetInstance()->SendProtocol(rpc);
	return ERR_SUCCESS;
}

int ConsignManager::DoWebPostCancel(int userid, int roleid, int64_t sn, int ctype, int64_t messageid, int64_t timestamp)
{
	int state= (ctype==1) ? DSTATE_POST_FORCE_CANCEL :DSTATE_POST_WEB_CANCEL;//ctype == 1 客服强行取消寄售,退保证金
	int errcode;
	ConsignMap::iterator it = CheckWebRqst(sn,timestamp,state,errcode);
	if(errcode!=ERR_SUCCESS)
		return errcode;
	ConsignInfo& info = GetCInfo(it->second);
	
//	int fintype = (state==DSTATE_POST_FORCE_CANCEL) ?POST_FORCE_CANCEL :POST_WEB_CANCEL ;
	if(info.itcache.consign_type != CONSIGNTYPE_ROLE)
	{
		DBConsignFail * rpc = (DBConsignFail *)Rpc::Call(
			RPC_DBCONSIGNFAIL,
			DBConsignFailArg(
				sn,	
				info.roleid,
				state,//fintype
				timestamp
			)
		);
		rpc->messageid = messageid;
		rpc->timestamp = timestamp;
		rpc->userid = info.userid;
		rpc->userid_mismatch = (info.userid != userid);
		rpc->roleid_mismatch = (info.roleid != roleid);
		GameDBClient::GetInstance()->SendProtocol(rpc);
		DEBUG_PRINT("ConsignManager::DoWebPostCancel ok\n");
	}
	else
	{
		if(!IsConsignRoleOpen()) return ERR_WT_UNOPEN;
		DBConsignRoleFail * rpc = (DBConsignRoleFail *)Rpc::Call(
			RPC_DBCONSIGNROLEFAIL,
			DBConsignRoleFailArg(
				sn,	
				info.roleid,
				state,//fintype
				timestamp
			)
		);
		rpc->messageid = messageid;
		rpc->timestamp = timestamp;
		rpc->userid = info.userid;
		rpc->userid_mismatch = (info.userid != userid);
		rpc->roleid_mismatch = (info.roleid != roleid);
		GameDBClient::GetInstance()->SendProtocol(rpc);
		DEBUG_PRINT("ConsignManager::DoWebPostCancel roleconsign ok\n");
	}
	return ERR_SUCCESS;
}

int ConsignManager::OnDBConsignFailRole(int64_t sn,int state, int64_t timestamp)
{
	DEBUG_PRINT("ConsignManager::OnDBConsignFailRole \n");
	int errcode;
	ConsignMap::iterator it = CheckDBRes(sn,timestamp,state,errcode,state!=DSTATE_POST_GAME_CANCEL && state!=DSTATE_POST_FAIL);
	if(errcode!=ERR_SUCCESS)
		return errcode;
	ConsignInfo& info = GetCInfo(it->second);

	info.state = state;
	if(state!=DSTATE_POST_GAME_CANCEL && state!=DSTATE_POST_FAIL)
		info.UpdateTimestamp(timestamp);
	RoleInfoCache::Instance().SetSelling(info.roleid,false);
	RemoveEntry(info.roleid,sn);
	DEBUG_PRINT("ConsignManager::OnDBConsignFailRole clear selling status ok\n");
	return ERR_SUCCESS;
}

int ConsignManager::OnDBConsignFail(int64_t sn,int state,int64_t timestamp)
{
	int errcode;
	ConsignMap::iterator it = CheckDBRes(sn,timestamp,state,errcode,state!=DSTATE_POST_GAME_CANCEL && state!=DSTATE_POST_FAIL);
	if(errcode!=ERR_SUCCESS)
		return errcode;
	ConsignInfo& info = GetCInfo(it->second);

//	if(info.userid != userid) return false;
	info.state = state;
	if(state!=DSTATE_POST_GAME_CANCEL && state!=DSTATE_POST_FAIL)
		info.UpdateTimestamp(timestamp);
	info.SetBusy();
	DBConsignMail * rpc = (DBConsignMail *)Rpc::Call(
		RPC_DBCONSIGNMAIL,
		DBConsignMailArg(
			sn,
			state,
			info.roleid
			)
		);
	GameDBClient::GetInstance()->SendProtocol(rpc);
	AddMailRqst(sn);
	DEBUG_PRINT("ConsignManager::OnDBConsignFail sn=%lld ok",sn);
	return ERR_SUCCESS;
}

int ConsignManager::DoShelf(int userid, int roleid, int64_t sn, int price, int64_t actiontime, int showperiod, int sellperiod, int buyer_roleid, int64_t messageid, int64_t timestamp)
{
	int state = DSTATE_SELL;
	int errcode;
	ConsignMap::iterator it = CheckWebRqst(sn,timestamp,state,errcode);
	if(errcode!=ERR_SUCCESS)
	{
		return errcode;
	}
	ConsignInfo& info = GetCInfo(it->second);
	int now=Timer::GetTime();
	if(info.state == DSTATE_SELL && info.itcache.sell_endtime > now)
	{
		info.ClearBusy();
		return ERR_WT_STATEERR;
	}

	int show_endtime = 0, sell_endtime = 0;
	if(showperiod > 0)
	{
		show_endtime = int(actiontime/1000) + showperiod*60;
	}
	sell_endtime = int(actiontime/1000) + showperiod*60 + sellperiod*60;
	int shelf_time = int(actiontime/1000);
	DBConsignShelf * rpc = (DBConsignShelf *)Rpc::Call(
			RPC_DBCONSIGNSHELF,
			DBConsignShelfArg(
				sn,
				info.roleid,
				state,
				show_endtime,
				price,//cent in web
				sell_endtime,
				buyer_roleid,
				shelf_time,
				timestamp)
		);
	rpc->messageid = messageid;
	rpc->timestamp = timestamp;
	rpc->userid = info.userid;
	rpc->userid_mismatch = (info.userid != userid);
	rpc->roleid_mismatch = (info.roleid != roleid);
//	rpc->shelf_time = shelf_time;
	GameDBClient::GetInstance()->SendProtocol(rpc);
	DEBUG_PRINT("ConsignManager::DoShelf ok\n");
	return ERR_SUCCESS;
}

int ConsignManager::OnDBShelf(int roleid, int64_t sn, int buyer_roleid, int buyer_userid, int64_t timestamp,const GConsignItem &item,int shelf_time)
{
	int state = DSTATE_SELL;
	int errcode;
	ConsignMap::iterator it = CheckDBRes(sn,timestamp,state,errcode);
	if(errcode!=ERR_SUCCESS)
		return errcode;
	ConsignInfo& info = GetCInfo(it->second);
//	if(info.roleid != roleid) return ;

	info.state = state;
	info.buyerrid = buyer_roleid;
	info.buyeruid = buyer_userid;
	info.UpdateTimestamp(timestamp);
	UpdateCacheEntry(item,info);//update info.itcache first,check in InsertItem
	if(info.itcache.consign_type != CONSIGNTYPE_ROLE)
		shelfcache.InsertItem(shelf_time,&info);//use old info.shelf_time to erase old shelfcache
	info.shelf_time = shelf_time;
	DEBUG_PRINT("ConsignManager::OnDBShelf ok");
	return ERR_SUCCESS;
}

ConsignManager::ConsignMap::iterator ConsignManager::CheckWebRqst( int64_t sn,int64_t timestamp,int state,int& retcode)
{
	ConsignMap::iterator it = CheckEntry(sn,retcode);
	if(retcode!=ERR_SUCCESS)
		return it;
	
	CategoryMap::iterator itcat = it->second;
	int res=itcat->second->CheckTimestamp(timestamp);  
	if(res<0)
	{
		if(state==DSTATE_SELL ||state==DSTATE_POST || state==DSTATE_POST_WEB_CANCEL ||state==DSTATE_POST_GAME_CANCEL|| state==DSTATE_POST_FORCE_CANCEL)
			retcode =  ERR_WT_TIMESTAMP_OLD;
		else
		{
			retcode =ERR_WT_TIMESTAMP_ERR_SEQUENCE;
			Log::log( LOG_ERR,"ConsignManager::CheckWebRqst timestamp err, sn=%lld,timestamp=%lld,oldstamp=%lld",sn,timestamp,itcat->second->last_web_timestamp);
		}
		return it;
	}
	else if(res==0)
	{
		if(state == itcat->second->state)
		{
			retcode = ERR_WT_TIMESTAMP_EQUAL;
			return it;
		}
		else
			Log::log( LOG_WARNING,"ConsignManager::CheckWebRqst timestamp equal with different target state , sn=%lld,oldstate=%d,state=%d",sn,state,itcat->second->state);
	}
	
	CheckRqst(sn,state,retcode);
	return it;
}
ConsignManager::ConsignMap::iterator ConsignManager::CheckEntry( int64_t sn,int& retcode)
{
	if(!IsInited())
	{
		retcode = ERR_WT_UNOPEN;
		DEBUG_PRINT("ConsignManager::CheckEntry err sn=%lld,manager not inited",sn);
		return consign_map.end();
	}
	ConsignMap::iterator it = consign_map.find(sn);
	if(it == consign_map.end()) 
	{
		DEBUG_PRINT("ConsignManager::CheckEntry err sn=%lld,not found",sn);
		retcode = ERR_WT_ENTRY_NOT_FOUND;
		return consign_map.end();
	}
	retcode =  ERR_SUCCESS;
	return it;
}

ConsignManager::ConsignMap::iterator ConsignManager::CheckRqst( int64_t sn,int state,int& retcode)
{
/*	if(!IsInited())
	{
		retcode = ERR_WT_UNOPEN;
		DEBUG_PRINT("ConsignManager::CheckRqst err sn=%lld,manager not inited",sn);
		return consign_map.end();
	}
	ConsignMap::iterator it = consign_map.find(sn);
	if(it == consign_map.end()) 
	{
		DEBUG_PRINT("ConsignManager::CheckRqst err sn=%lld,not found",sn);
		retcode = ERR_WT_ENTRY_NOT_FOUND;
		return consign_map.end();
	}
*/
	ConsignMap::iterator it = consign_map.find(sn);
	
	ConsignInfo& info = GetCInfo(it->second);
	if(!info.SetBusy())
	{
		retcode = ERR_WT_ENTRY_IS_BUSY;
		DEBUG_PRINT("ConsignManager::CheckRqst err sn=%lld busy",sn);
		return consign_map.end();
	}

	int changeres =info.CheckStateChange(state);
	if(changeres == CHANGE_REPEAT)
	{
		retcode = ERR_WT_DUPLICATE_RQST;
		info.ClearBusy();
		return consign_map.end();
	}
	if(changeres != CHANGE_PERMIT)
	{
		retcode = ERR_WT_STATEERR;
		DEBUG_PRINT("ConsignManager::CheckRqst err sn=%lld,state=%d,targetstate=%d",sn,info.state,state);
		info.ClearBusy();
		return consign_map.end();
	}

	retcode =  ERR_SUCCESS;
	return it;
}

ConsignManager::ConsignMap::iterator ConsignManager::CheckDBRes( int64_t sn,int64_t timestamp,int state,int& retcode,bool check_timestamp)
{
	if(!IsInited())
	{
		retcode = ERR_WT_UNOPEN;
		DEBUG_PRINT("ConsignManager::CheckDBRes err sn=%lld,manager not inited",sn);
		return consign_map.end();
	}
	ConsignMap::iterator it = consign_map.find(sn);
	if(it == consign_map.end()) 
	{
		DEBUG_PRINT("ConsignManager::CheckDBRes err sn=%lld,not found",sn);
		retcode = ERR_WT_ENTRY_NOT_FOUND;
		return consign_map.end();
	}
	ConsignInfo& info = GetCInfo(it->second);
	if(!info.ClearBusy())
	{
		//retcode = ERR_WT_ENTRY_IS_NOT_BUSY;
		DEBUG_PRINT("ConsignManager::CheckDBRes warning sn=%lld not busy",sn);
		//return consign_map.end();
	}

/*	int changeres =info.CheckStateChange(state);
//	if(changeres == CHANGE_REPEAT)
//	{
//		retcode = ERR_WT_DUPLICATE_RQST;
//		return consign_map.end();
//	}
	if(changeres != CHANGE_PERMIT)
	{
		retcode = ERR_WT_STATEERR;
		DEBUG_PRINT("ConsignManager::CheckDBRes err sn=%lld,state=%d,targetstate=%d",sn,info.state,state);
		return consign_map.end();
	}*/
	if(check_timestamp && info.CheckTimestamp(timestamp)<0)//permit equal timestamp
	{
		retcode =  ERR_WT_TIMESTAMP_MISMATCH;
		DEBUG_PRINT("ConsignManager::CheckDBRes timestamp err sn=%lld,timestamp=%lld,oldstamp=%lld",sn,timestamp,info.last_web_timestamp);
		return consign_map.end();
	}		
	
	retcode =  ERR_SUCCESS;
	return it;
}

int ConsignManager::DoShelfCancel(int userid, int roleid, int64_t sn, int64_t messageid, int64_t timestamp)
{
	int state = DSTATE_POST;
	int errcode;
	ConsignMap::iterator it = CheckWebRqst(sn,timestamp,state,errcode);
	if(errcode!=ERR_SUCCESS)
	{
		return errcode;
	}
	ConsignInfo& info = GetCInfo(it->second);
	DBConsignShelfCancel * rpc = (DBConsignShelfCancel *)Rpc::Call(
			RPC_DBCONSIGNSHELFCANCEL,
			DBConsignShelfCancelArg(
				sn,
				info.roleid,
				state,
				timestamp)
		);
	rpc->messageid = messageid;
	rpc->timestamp = timestamp;
	rpc->userid = info.userid;
	rpc->userid_mismatch = (info.userid != userid);
	rpc->roleid_mismatch = (info.roleid != roleid);
	GameDBClient::GetInstance()->SendProtocol(rpc);
	DEBUG_PRINT("ConsignManager::DoShelfCancel ok\n");
	return ERR_SUCCESS;
}

int ConsignManager::OnDBCancelShelf(int roleid, int64_t sn, int64_t timestamp)
{
	int state = DSTATE_POST;
	int errcode;
	ConsignMap::iterator it = CheckDBRes(sn,timestamp,state,errcode);
	if(errcode!=ERR_SUCCESS)
		return errcode;
	ConsignInfo& info = GetCInfo(it->second);
//	if(info.roleid != roleid) return false;

	info.state = state;
	info.buyerrid = 0;
	info.buyeruid = 0;
	info.UpdateTimestamp(timestamp);
	shelfcache.EraseItem(info.shelf_time,info.itcache.sn);
	DEBUG_PRINT("ConsignManager::OnDBCancelShelf ok");
	return ERR_SUCCESS;
}

int ConsignManager::DoSold(int _zoneid, int userid, int roleid, int64_t sn, int buyer_userid, int buyer_roleid, int64_t orderid, int stype, int64_t timestamp)
{
	int state = DSTATE_SOLD;
	int errcode;
	ConsignMap::iterator it = CheckWebRqst(sn,timestamp,state,errcode);
	if(errcode!=ERR_SUCCESS && errcode!= ERR_WT_TIMESTAMP_EQUAL)
		return errcode;
	ConsignInfo& info = GetCInfo(it->second);
	//check roleid	
//	if(info.roleid != roleid) 
//		return false;
//	if(info.state!=state)
//	{
//	info.state = state;
	if(info.itcache.consign_type != CONSIGNTYPE_ROLE)
	{
		DBConsignSold* rpc = (DBConsignSold *)Rpc::Call(
				RPC_DBCONSIGNSOLD,
				DBConsignSoldArg(
					sn,
					roleid,
					userid,
					buyer_roleid,
					buyer_userid,
					orderid,
					timestamp)
			);
		rpc->zoneid = _zoneid;
	//	rpc->orderid = orderid;
		rpc->timestamp = timestamp;
		rpc->userid = info.userid;
		rpc->userid_mismatch = (info.userid != userid);
		rpc->roleid_mismatch = (info.roleid != roleid);
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}
//	}
	else
	{
		if(!IsConsignRoleOpen()) return ERR_WT_UNOPEN;
		DBConsignSoldArg arg(
			sn,
			roleid,
			userid,
			buyer_roleid,
			buyer_userid,
			orderid,
			timestamp);
		int ret=RoleTaskManager::GetInstance()->SoldRole(arg,_zoneid,timestamp,info.userid,(info.userid != userid),(info.roleid != roleid));
		if(ret)
			return ret;
	}
	DEBUG_PRINT("ConsignManager::doSold ok,sn=%lld",sn);
	return ERR_SUCCESS;
}

int ConsignManager::OnDBSold(int roleid, int64_t sn, int64_t timestamp)
{
	int state = DSTATE_SOLD;
	int errcode;
	ConsignMap::iterator it = CheckDBRes(sn,timestamp,state,errcode);
	if(errcode!=ERR_SUCCESS)
		return errcode;
	ConsignInfo& info = GetCInfo(it->second);

//	if(info.userid != userid) return false;
	info.state = state;
	info.UpdateTimestamp(timestamp);
	shelfcache.EraseItem(info.shelf_time,info.itcache.sn);
	info.SetBusy();
	DBConsignMail * rpc = (DBConsignMail *)Rpc::Call(
		RPC_DBCONSIGNMAIL,
		DBConsignMailArg(
			sn,
			DSTATE_SOLD,
			info.roleid
			)
		);		
	GameDBClient::GetInstance()->SendProtocol(rpc);
	AddMailRqst(sn);
	DEBUG_PRINT("ConsignManager::OnDBSold ok,sn=%lld",sn);
	return ERR_SUCCESS;
}
int ConsignManager::OnDBSoldRole(int roleid, int64_t sn, int64_t timestamp)
{
	int state = DSTATE_SOLD;
	int errcode;
	ConsignMap::iterator it = CheckDBRes(sn,timestamp,state,errcode);
	if(errcode!=ERR_SUCCESS)
		return errcode;
	ConsignInfo& info = GetCInfo(it->second);

//	if(info.userid != userid) return false;
	info.state = state;
	info.UpdateTimestamp(timestamp);
	shelfcache.EraseItem(info.shelf_time,info.itcache.sn);
	RemoveEntry(roleid,sn);
	RoleInfoCache::Instance().SetSelling(roleid,false);
	DEBUG_PRINT("ConsignManager::OnDBSoldRole ok,sn=%lld",sn);
	return ERR_SUCCESS;
}
int ConsignManager::GetClientRqstRetcode(int errcode)
{
	int ret;
	switch(errcode)
	{
		case ERR_SUCCESS:
			ret = ERR_SUCCESS;
			break;
		case ERR_WT_UNOPEN:
			ret = ERR_WTC_UNOPEN;
			break;
		case ERR_WT_ENTRY_NOT_FOUND:
			ret = ERR_WTC_ENTRY_NOT_FOUND; 
			break;
		case ERR_WT_ENTRY_IS_BUSY:
			ret = ERR_WTC_ENTRY_IS_BUSY;
			break;
		case ERR_WT_SN_ROLEID_MISMATCH:
			ret = ERR_WTC_SN_ROLEID_MISMATCH;
			break;
		case ERR_WT_TOO_MANY_ATTEND_SELL:			
			ret = ERR_WTC_TOO_MANY_ATTEND_SELL;
			break;
		case ERR_WT_MAILBOX_FULL:
			ret = ERR_WTC_MAILBOX_FULL;
			break;
		case ERR_WT_BUYERNAME_INVALID:
			ret = ERR_WTC_BUYERNAME_INVALID;
			break;
		case ERR_WT_DB_FAILURE:
		case ERR_WT_DB_STATEERR:
		case ERR_WT_DB_KEYZERO:
		case ERR_WT_DB_INVALID_ARGUMENT:
//		case ERR_WT_DB_DUPLICATE_RQST:
			ret = ERR_WTC_DB_ERR;
			break;
		case ERR_WT_TIMEOUT:
			ret = ERR_WTC_TIMEOUT;
			break;
		case ERR_WT_GCONSIGNSTART_PROCESSING:
			ret = ERR_WTC_GCONSIGNSTART_PROCESSING;
			break;
		case ERR_WT_STATEERR:
			ret = ERR_WTC_STATEERR;
			break;
		case ERR_WT_DUPLICATE_RQST:
			ret = ERR_WTC_DUPLICATE_RQST;
			break;
		case ERR_WT_PREPOST_ARG_ERR:
			ret = ERR_WTC_PREPOST_ARG_ERR;
			break;
		case ERR_WT_BUYER_SAMEUSER:
			ret = ERR_WTC_BUYER_SAMEUSER;
			break;
		case ERR_WT_SELL_ROLE_WHILE_FORBID:
			ret = ERR_WTC_SELL_ROLE_WHILE_FORBID;
			break;
		case ERR_WT_SELL_ROLE_WHILE_ITEM_SELLING:
			ret = ERR_WTC_SELL_ROLE_WHILE_ITEM_SELLING;
			break;
		case ERR_WT_SELL_ROLE_HAS_FAC_RELATION:
			ret = ERR_WTC_SELL_ROLE_HAS_FAC_RELATION;
			break;
		case ERR_WT_SELL_ROLE_HAS_SECT_RELATION:
			ret = ERR_WTC_SELL_ROLE_HAS_SECT_RELATION;
			break;
		case ERR_WT_SELL_ROLE_HAS_SPOUSE_RELATION:
			ret = ERR_WTC_SELL_ROLE_HAS_SPOUSE_RELATION;
			break;
		case ERR_WT_SELL_ROLE_HAS_CRSSVRTEAMS_RELATION:
			ret = ERR_WTC_SELL_ROLE_HAS_CRSSVRTEAMS_RELATION;
			break;
		default:
			ret = ERR_WTC_INNER;
			break;
	}
	return ret;
}

int ConsignManager::GetWebRqstRetcode(int errcode)
{
	int web_ret;
	switch(errcode)
	{
		default:
		case ERR_WT_UNOPEN:
		case ERR_WT_DUPLICATE_RQST:
		case ERR_WT_TIMESTAMP_ERR_SEQUENCE:	
					web_ret = -1;
					break;
		case ERR_WT_ENTRY_NOT_FOUND:
					web_ret = 1; 
					break;
		case ERR_WT_ENTRY_IS_BUSY:
					web_ret = 6;
					break;
		case ERR_WT_TIMESTAMP_OLD:		//timestamp<= last success timestamp 
		case ERR_WT_TIMESTAMP_EQUAL:			
					web_ret = 0;
					break;
		case ERR_WT_STATEERR:
					web_ret = 3;
					break;
	}
	return web_ret;
}

int ConsignManager::DoPostExpire(int userid, int roleid, int64_t sn, int64_t messageid, int64_t timestamp)
{
	int state = DSTATE_EXPIRE;
	int errcode;
	ConsignMap::iterator it = CheckWebRqst(sn,timestamp,state,errcode);
	if(errcode!=ERR_SUCCESS)
		return errcode;
	ConsignInfo& info = GetCInfo(it->second);
	if(info.itcache.consign_type != CONSIGNTYPE_ROLE)
	{
		DBConsignFail * rpc = (DBConsignFail *)Rpc::Call(
			RPC_DBCONSIGNFAIL,
			DBConsignFailArg(
				sn,	
				info.roleid,
				state,//EXPIRE
				timestamp
			)
		);
	
		rpc->messageid = messageid;
		rpc->timestamp = timestamp;
		rpc->userid = info.userid;
		rpc->userid_mismatch = (info.userid != userid);
		rpc->roleid_mismatch = (info.roleid != roleid);
		GameDBClient::GetInstance()->SendProtocol(rpc);
		DEBUG_PRINT("ConsignManager::DoPostExpire ok\n");
	}
	else
	{
		if(!IsConsignRoleOpen()) return ERR_WT_UNOPEN;
		DBConsignRoleFail * rpc = (DBConsignRoleFail *)Rpc::Call(
			RPC_DBCONSIGNROLEFAIL,
			DBConsignRoleFailArg(
				sn,	
				info.roleid,
				state,//EXPIRE
				timestamp
			)
		);
		rpc->messageid = messageid;
		rpc->timestamp = timestamp;
		rpc->userid = info.userid;
		rpc->userid_mismatch = (info.userid != userid);
		rpc->roleid_mismatch = (info.roleid != roleid);
		GameDBClient::GetInstance()->SendProtocol(rpc);
		DEBUG_PRINT("ConsignManager::DoPostExpire roleconsign ok\n");
	}
	return ERR_SUCCESS;
}

int ConsignManager::OnDBPrePostRole(const GConsignDB & detail,const GConsignRole& role)
{
/*	if(!IsInited()){
		DEBUG_PRINT("ConsignManager::OnDBPrePost err sn=%lld,manager not inited",detail.info.sn);
		return ERR_WT_UNOPEN;
	}*/
//	ClearConsignStarting(detail.seller_roleid);//clear in delete roleconsigntask
	AddEntry(detail);
	SetRoleData(detail.info.sn,role);
	SendPostRole(detail,role);
	DEBUG_PRINT("ConsignManager::OnDBPrePostRole ok, roleid=%d,sn=%lld",detail.seller_roleid,detail.info.sn);
	return ERR_SUCCESS;
}

bool ConsignManager::SetRoleData(int64_t sn,const GConsignRole& role)
{
	ClientRqstMap::iterator it = client_rqst_map.find(sn);
	if(it!=client_rqst_map.end())
	{
		it->second->SetRoleData(role);
		return true;
	}
	return false;
}	

void ConsignManager::OnRolenameChange(int roleid, const Octets & rolename)
{
	RoleConsignMap::iterator r_it = role_consign_map.find(roleid);
	if(r_it == role_consign_map.end())
		return;
	SNSet& sn_set = r_it->second;
	SNSet::iterator s_it = sn_set.begin(),s_ite = sn_set.end();
	for( ; s_it != s_ite; ++s_it)
	{
		ConsignMap::iterator it = consign_map.find(*s_it);
		if(it != consign_map.end())
		{
			if (it->second->second->roleid == roleid)
				it->second->second->rolename = rolename;
		}
	}
}
}
