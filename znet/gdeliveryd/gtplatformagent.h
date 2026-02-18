#ifndef __GNET_GTPLATFORMAGENT_H
#define __GNET_GTPLATFORMAGENT_H
#include "localmacro.h"
#include "familymanager.h"
#include "gfriendinfo"
namespace GNET
{
class GFactionInfo;
class GFamily;
class GFolk;
class GGateRoleGTStatus;
class GateFactionChat;
class GRoleInfo;

class GTPlatformAgent
{
	GTPlatformAgent(){}
	static const int MAX_RECORD_NUM_IN_GATEONLINELIST=500;
	static bool gtopen;
public:
//	static GTPlatformAgent* Instance(){static GTPlatformAgent instance; return &instance;}
	~GTPlatformAgent(){}
	static void OpenGT(){gtopen=true;}
	static bool IsGTOpen(){return gtopen;}
	static int ModifyFriendGroup(GroupOperation oper,int roleid,char groupid,const Octets& groupname);
	static int ModifyFriend(FriendOperation oper,int roleid,int friendid,char groupid);
	static int DeleteRole(int userid,int roleid);
	static int CreateRole(int roleid, int userid, const GRoleInfo & info);
	static int DeleteFaction(FType type,int fid);//and family
	static int FactionDelMember(int factionid,const GFamily& family,const RoleContainer& members);
	static int FactionAddMember(const GFactionInfo& faction,const GFamily& family,const RoleContainer& members);
	static int FactionModMember(FType type,int fid,int roleid,unsigned char title);//and family
	static int FactionUpdate(FType type,int fid,const Octets& data, int updatetype);//and family
	static int FamilyAddMember(int familyid,const GFolk& role,int reborn,int cultivation,unsigned char gender,int factionid);
	static int FamilyDelMember(int familyid,int roleid,int factionid);
	static int SyncTOGate(int startrole,bool start);
	static int UpdateClientGTStatus(const std::vector<int>& rolelist,const GGateRoleGTStatus & status);
	static int OnGateFactionChat(const GateFactionChat& gchat);
	static int GetFriendStatus(int roleid,const GFriendInfoVector& flist);
	static int GetFriendStatus(int roleid,int friendid);
	static int OnPlayerLogout(int roleid,int occupation,int level);
	static int OnPlayerLogin(int roleid,const GRoleInfo& info);
};

};
#endif
