#ifndef __GNET_GTMANAGER_H
#define __GNET_GTMANAGER_H

#include <vector>
#include <map>
#include <list>

#include "thread.h"
#include "localmacro.h"
#include "taskqueue.h"
#include "userqueryagent2.h"
#include "factionqueryagent.h"
#include "simplequery.h"
#include "msglimiter.h"
#include "roleinfobean"
#include "rolestatusbean"
#include "gfriendinfo"
#include "ggroupinfo"

namespace GNET
{
class RoleStatusUpdate;
class FactionInfoReq;
class RoleStatusReq;
class RoleStatusResp;
class RoleListResp;
class RoleStatusBean;
class GGateRoleInfo;
class GGateRoleGTStatus;
class GFactionInfo;
class GFamily;
class RoleGroupBean;
class GGroupInfo;
class GFriendInfo;
class RoleInfo;
class RoleRelationResp;
class DBGateRoleRelationRes;
class RoleMsgBean;
class PrivateChat;
class GateFactionChat;

class GTManager : public IntervalTimer::Observer
{
	static const int UPDATE_INTERVAL = 300;
	static const int MAX_RECORD_NUM_IN_OUTGOING_ROLESTATUSRESP = 500;
	bool gt_open;
	GTRoleListReqQueue rolelist_queue;
	GTRoleRelationReqQueue rolerelation_queue;
	GTFactionInfoReqQueue factioninfo_queue;
	GTFamilyInfoReqQueue familyinfo_queue;
	GTRoleInfoReqQueue roleinfo_queue;

	typedef MsgLimiter<10,5,30> FactionChatLimit;//5
	FactionChatLimit faction_chat_limit;
	typedef MsgLimiter<10,8,30> PrivateChatLimit;//8
	PrivateChatLimit private_chat_limit;
	typedef NoCooldownCounterPolicy<5,500> GlobalFactionChatLimit;//5,500
	GlobalFactionChatLimit global_faction_chat_limit;
	typedef NoCooldownCounterPolicy<5,2500> GlobalPrivateChatLimit;//5,2500 //include offlinechat
	GlobalPrivateChatLimit global_private_chat_limit;
	typedef NoCooldownCounterPolicy<5,1000> GlobalGTStatusUpdateLimit;//5,1000
	GlobalGTStatusUpdateLimit global_gtstatus_chat_limit;

	typedef std::map<int,UserQueryAgent> UserQueryMAP;
	UserQueryMAP user_query_map;
	typedef std::map<int,FactionQueryAgent> FactionQueryMAP;
	FactionQueryMAP faction_query_map;

	enum GT_PLATFORM_FORMAT_STATUS
	{
		STATUS_OFFLINE=0,
		STATUS_ONLINE_GAME=1,
		STATUS_ONLINE_IM=2,
		STATUS_ONLINE_WEB=3,
		STATUS_INVISIABLE=128,
	};
	GTManager():gt_open(false){}
public:
	virtual ~GTManager(){} 
	static GTManager* Instance(){static GTManager instance;	return &instance;}
	void SetQueryLimit(int faction,int user,int normal){
		factioninfo_queue.set_frequency_limit(faction);
		rolelist_queue.set_frequency_limit(user);
		rolerelation_queue.set_frequency_limit(normal);
		familyinfo_queue.set_frequency_limit(normal);
		roleinfo_queue.set_frequency_limit(normal);
	}
	void OpenGT(){gt_open = true;}
	bool IsGTOpen(){return gt_open;} 
	bool Initialize(){IntervalTimer::Attach(this,1000000/IntervalTimer::Resolution()); return true;} 
	bool Update();
	bool Check_64to32_Cast(int64_t number64,int& number32){
		if((number64&0xFFFFFFFF00000000LL) != 0LL){
			number32=0;
			DEBUG_PRINT("Check_64to32_Cast err: number64=%lld", number64);
			return false;
		}
		number32 = number64;
		return true;
	}

	//game online update
	int GameLogin(unsigned int roleid,int localsid, int linksid,const GGateRoleInfo& data );
	int GameLogout(unsigned int roleid, const GGateRoleInfo& data);
	int GameRoleCreate(int roleid, int userid, const GGateRoleInfo & data);

	//gt online update
	int OnRoleStatusResp(const std::map<int64_t,RoleStatusBean>& rolestatus);
	int OnRoleStatusUpdate(const RoleStatusUpdate& tsk);

	//complicated game data query
	int OnRoleListReq(int64_t gtuserid,int pt_type);
	int OnFactionInfoReq(int64_t localuid,int ftype,int64_t gt_facid, int pt_type){
		int fid = gt_facid;//truncate
		FactionReq req(localuid,fid,pt_type);
		if(ftype == FACTIONTYPE)//faction
			OnFactionInfoReq(req);
		else if(ftype == FAMILYTYPE)//family
			OnFamilyInfoReq(req);
		else//else error
			return -1;
		return 0;	
	}
	//auxiliary funcs for complicated game data query 
	int OnFactionInfoReq(FactionReq req);
	int TryFactionQuery(FactionReq req);
	int TryUserQuery(UserQuery req);
	int OnDBGateGetUser(int userid, unsigned int rolelist, unsigned int logicuid, int pt_type);
	int OnDBGateGetRole(const GGateRoleInfo& roleinfo,int userid,int64_t localuid, int pt_type);
	int OnDBFactionGet(int factionid, const GFactionInfo& info, int pt_type);
	int OnDBFamilyGet(int factionid, const GFamily& family,int64_t localuid, int pt_type);
	int FamilyMemberSetData(FactionInfoResp & resp,const std::vector<GNET::GFolk>& info,int reserve_num);
	int OnDBFactionGetErr(int factionid,int64_t localuid,int retcode, int pt_type);
	int OnDBFamilyGetErr(int factionid, int familyid,int64_t localuid,int retcode, int pt_type);
	int OnDBGateGetRoleErr(int userid,int64_t localuid,int roleid,int retcode,int pt_type);
	//DBGetUser error is fine,just call OnDBGateGetUser with a null rolelist

	//simple query
	int OnRoleRelationReq(int64_t gtuserid,int64_t gtroleid,int pt_type);
	int OnDBGateRoleRelation(RoleRelationResp& re,DBGateRoleRelationRes* res,int pt_type);
	int OnRoleInfoReq(int64_t localuid,int64_t gtroleid);
	int OnFamilyInfoReq(FactionReq req);
	int OnDBGateRoleRelationErr(int userid,int roleid,int retcode, int pt_type);

	//online status query
	RoleStatusBean RoleStatusQuery(int roleid);
	bool GameOnline(int roleid);
	bool GTOnline(int roleid);
	//game online query
	int OnRoleStatusReq(const std::vector<int64_t>& rolelist,int64_t localrid);
	//gt online query
	int OnGetGTRoleStatus(const std::vector<int> & rolelist,std::vector<GGateRoleGTStatus>& statuslist);
	
	//chat forwardding
	int OnGTRoleMsg(int64_t receiver,const RoleMsgBean& msg);
	int OnGamePrivateChat(const PrivateChat& chat);
	int OnGTFactionMsg(int64_t factionid,int ftype,const RoleMsgBean& msg);
	int OnGameFactionChat(const GateFactionChat& chat);
	int OnGTOfflineRoleMsg(int64_t receiver,const std::vector<RoleMsgBean>& msgs);

	//gt activate or deactivate
	int OnRoleActivation(int64_t gtroleid,unsigned char operation);

	//role relation update and facion/family update handled in hpp files

	//sync
	int SyncGameOnlineTOGT();
	int OnGameGateDisconnect();
	int OnGameGateSync();
private:
	//decode status send from gt platform
	bool IS_GT_OFFLINE(unsigned char status){return (status&0xFE) == 0;}
	bool IS_GAME_OFFLINE(unsigned char status){return (status&0x01) == 0;}
	//get status in gameclient format from gt platorm format
	bool IS_GTOnline_In_Client_Eyes(unsigned char status){return (status&0x06) != 0;}
	//get status in gt platform format from RoleInfo format
	unsigned char ROLEINTO_TO_GTSTATUS(bool gameonline,unsigned char gtstatus){
		if(gameonline)gtstatus|=0x01;return gtstatus;
	}
	bool IS_Hide_To_Offline(unsigned char newgtstatus,unsigned char oldgtstatus){
		return ((newgtstatus&0xFE) == 0) && ( (oldgtstatus&0x80) !=0);
	}
	int UpdateGTRoleData(unsigned int roleid, const GGateRoleInfo& olddata, const GGateRoleInfo& data);
	int SendGTRoleData(unsigned int roleid, const GGateRoleInfo& data);
	int UpdateStatusTOGT(int roleid,const RoleInfo* role);
	int UpdateRoleStatusCache(int64_t gtroleid,const RoleStatusBean& status);
	int UpdateClientRoleStatus(int64_t gtroleid,const RoleStatusBean& status,const std::vector<int64_t>& friends);
	int FriendSetData(std::vector<RoleGroupBean> & ret,const GFriendInfoVector& friends,const GGroupInfoVector& group);
	int FamilyChangeTitle(FactionInfoResp & resp);
};

};
#endif
