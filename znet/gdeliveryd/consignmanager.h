#ifndef __GNET_CONSIGNMANAGER_H__
#define	__GNET_CONSIGNMANAGER_H__

#include <map>
#include <set>
#include "itimer.h"
#include "gconsigndb"
#include "gconsignitem"
#include "gconsignlistnode"
#include "gconsignrole"
#include "localmacro.h"
#include "gwebtradeclient.hpp"
#include "gconsignlistrolenode"

namespace GNET
{
class GConsignStart;
class PlayerInfo;
class ConsignManager;
class GConsignStartRole;
class Post;

class ConsignInfo
{
public:
	enum ConsignClientState 	//在客户端显示的状态
	{
		STATE_PRE_POST,					//预寄售,未与平台同步
		STATE_POST,					//寄售成功,与平台同步完毕,下架
		STATE_PRE_CANCEL_POST,				//预取消寄售,未与平台同步
		STATE_SHOW,						//公示
		STATE_SELL,						//上架
		STATE_SOLD,
		STATE_EXPIRE,
		STATE_POST_FAIL,
		STATE_POST_CANCEL,
		STATE_POST_FORCE_CANCEL,
	};
public:
	ConsignInfo(const GConsignDB& detail)
		:busy(false),userid(detail.seller_userid),roleid(detail.seller_roleid),\
		buyeruid(detail.buyer_userid),buyerrid(detail.buyer_roleid),state(detail.info.state),\
		rolename(detail.seller_name),obj(detail.item),itcache(detail.info),\
		commodityid(detail.commodityid),shelf_time(detail.shelf_time)
	{
		last_web_timestamp = (detail.largest_web_timestamp==0)? 0:detail.largest_web_timestamp;
		if(state == STATE_PRE_POST || state == STATE_PRE_CANCEL_POST)
			busy = true;
	}
	~ConsignInfo(){}
	bool CheckStateChange(int target_state);
	bool IsBusy(){ return busy; }
	bool SetBusy(){
		if(busy)return false; 
		return busy=true;
	}
	bool ClearBusy(){
		if(!busy)return false;
		busy=false;
		return true;
	}
	int CheckTimestamp(int64_t t){ 
		if(t > last_web_timestamp) return 1;
		else if(t == last_web_timestamp) return 0;
		else return -1;
	}
	void UpdateTimestamp(int64_t t){ last_web_timestamp = t; }
	void TimeoutUpdateState(int new_state){state = new_state;}//SELL -> POST

private:
	bool busy;
	int64_t last_web_timestamp;//成功操作的最大时戳
	int userid;
	int roleid;
	int buyeruid;	//specified buyer userid
	int buyerrid;	//specified buyer roleid
	int state;
	Octets rolename; 	//seller name
	GRoleInventory obj;
	GConsignItem itcache;
	int commodityid;
	int shelf_time;
friend class ConsignManager;
friend class ConsignNeedMail;
friend class ConsignShelf;
};

class ConsignClientRqst
{
	enum ClientRqstType{
		POST,
		POST_CANCEL,
		POSTROLE,
	};
	static const int CLIENT_REQUEST_RESEND_INTERVAL=60;
	int timeout;
	ClientRqstType rtype;
	GConsignDB* post_info; //info to construct Post msg
	int userid;
	int roleid;
	int64_t sn;
	int game_timestamp;
	GConsignRole* role_data;
	void SendMsg();
public:
	bool Update(int elapse_time)//send msg to web when timeout
	{
		timeout-=elapse_time;
		if(timeout<=0)
		{
			SendMsg();
			timeout=CLIENT_REQUEST_RESEND_INTERVAL;
		}
		return true;
	}
	ConsignClientRqst(ClientRqstType type,int uid, int rid, int64_t consign_sn,int stamp,const GConsignDB* pc=NULL):
		rtype(type),post_info(NULL),userid(uid),roleid(rid),sn(consign_sn),game_timestamp(stamp),role_data(NULL)
	{
		timeout = CLIENT_REQUEST_RESEND_INTERVAL;
		if(type == POST || type == POSTROLE)
		{
			if(pc==NULL)
			{
				Log::log(LOG_ERR,"ConsignClientRqst construct err, GConsignDB pointer is null");
				return;
			}
			post_info = new GConsignDB(*pc);
		}
	}
	~ConsignClientRqst()
	{
		if(post_info) delete post_info;
		if(role_data) delete role_data;
	}
	void SetRoleData(const GConsignRole& rdata)
	{
		role_data = new GConsignRole(rdata);
	}
friend class ConsignManager;		
private:
	ConsignClientRqst(const ConsignClientRqst& rqst);
};

class ConsignNeedMail
{
	static const int MAIL_REQUEST_RESEND_INTERVAL=900;
	int timeout;
	int64_t sn;
	void SendMailMsg();
public:
	bool Update(int elapse_time)//send msg to db when timeout
	{
		timeout-=elapse_time;
		if(timeout<=0)
		{
			SendMailMsg();
			timeout=MAIL_REQUEST_RESEND_INTERVAL;
		}
		return true;
	}
	ConsignNeedMail(int64_t consign_sn):sn(consign_sn)
	{
		timeout = MAIL_REQUEST_RESEND_INTERVAL;
	}
	~ConsignNeedMail(){}
friend class ConsignManager;		
};

class ConsignShelf
{
	typedef std::multimap<int/*shelf_time*/,ConsignInfo*> ShelfMap;
	typedef ShelfMap::iterator Sheit;
	typedef ShelfMap::reverse_iterator Sherit;
	ShelfMap shelfmap;
	const int itemperpage;
	const int firstpageindex;
	static const int SHELF_AUOTUPDATE_INTERVAL=300;
	int updatetime;
	int tick;
public:
	ConsignShelf(int ipp,int fpi):itemperpage(ipp),firstpageindex(fpi),updatetime(0),tick(0){}
	bool EraseItem(int shelf_time,int64_t sn);
	bool InsertItem(int shelf_time,ConsignInfo* pcinfo);
	void GetPage(std::vector<GConsignListNode>& nodes,int page);
	//page number of shelf, empty shelf has 0 page
	int TotalPage(){return (shelfmap.size()>0)?((shelfmap.size()-1)/itemperpage +1):0;}
	int LastPageIndex(){return TotalPage()>0?(firstpageindex-1+TotalPage()):firstpageindex;}
	int GetUpdateTime(){return updatetime;}
	int size(){return shelfmap.size();}
	void UpdateShelf();
	void UpdateMe(int delta){
		tick+=delta;if(tick==SHELF_AUOTUPDATE_INTERVAL){tick=0;UpdateShelf();}
	}
//friend class ConsignManager;
};

class ConsignManager : public IntervalTimer::Observer
{
public:
	struct FinishedRecord
	{
		bool exist; //历史记录中是否有该 sn 
		int64_t orderid;
		FinishedRecord(bool _e = false, int64_t _o = 0) : exist(_e), orderid(_o) {}
	};
//	typedef std::map<int64_t,ConsignInfo*>	ConsignMap;		//sn--交易物品  查询表
	typedef std::set<int64_t>		SNSet;	
	typedef std::map<int/*roleid*/,SNSet>	RoleConsignMap;		//roleid->consign sn of role
//	typedef std::map<int64_t,GConsignDB*> PrePostMap;	//client msg do not need timestamp
//	typedef std::map<int64_t,ConsignMap::Iterator> PrePostCancelMap;//client msg do not need timestamp
	typedef std::map<int64_t,ConsignClientRqst*> ClientRqstMap;	//client requestes would send to web
	typedef std::map<int64_t,ConsignNeedMail> MailRqstMap;         	//done trade, need mail
	typedef std::map<int64_t, FinishedRecord> SoldMap; 				//缓存历史sn列表 不一定都是成功交易的
	typedef std::multimap<int/*category*/,ConsignInfo*> CategoryMap;//category--交易物品  查询表
	typedef std::map<int64_t,CategoryMap::iterator>	ConsignMap;	//sn--交易物品  查询表
	typedef CategoryMap::reverse_iterator Cate_rit;
	typedef CategoryMap::iterator Cate_it;
	enum Consign_Manager_State
	{
		ST_UNINIT,
		ST_INITED,
		ST_INITING,//正在从db加载
	};
	enum direction
	{
		FORWARD,
		BACKWARD,
	};
	struct PageInfo
	{
		Cate_it next;//iterator to last entry of current page, +1 
		bool finish;//has traversed entire category
		int retcode;
		PageInfo(Cate_it it,bool fin,int ret):next(it),finish(fin),retcode(ret){}
	};
private:
	static const int CONSIGN_MANAGER_UPDATE_INTERVAL=10;//seconds
	static const int CONSIGN_MANAGER_HEARTBEAT_INTERVAL=30;
	static const int CONSIGN_ITEM_MAX  = 20;
	static const int DEFAULT_SHOW_PERIOD = 30;
	static const int CONSIGN_MONEY_MAX=2000000000;
	static const int CONSIGN_MARGIN_MAX=200000000;
	static const int CONSIGN_PRICE_MAX=100000;
	static const int PAGECOUNT=10;
	static const int LASTPAGE=-1;
	static const int FIRSTPAGE=0;
	static const int64_t NO_SHORTCUT=0;
//	static const unsigned int MONEY_MIN;	//游戏币寄售下限
	int	status;// is manager inited
	int	tick;//how many ticks have passed after last Update()
	int	aid;		//游戏id
	int 	zoneid;		//服务器id
	bool 	consign_open;   //启用寄售功能
	bool 	consignrole_open;   //启用寄售role功能
	ConsignMap		consign_map;
	RoleConsignMap		role_consign_map;
	ClientRqstMap		client_rqst_map;
	std::set<int>		consign_starting_set;//roleids whose GConsignStart requests is processing
	SoldMap 		sold_map;
	MailRqstMap  		mail_rqst_map;	
	CategoryMap 		category_map;
	ConsignShelf 		shelfcache;	
private:
	ConsignManager():status(ST_UNINIT),consign_open(false),consignrole_open(false),shelfcache(PAGECOUNT,FIRSTPAGE){}
	ConsignInfo& GetCInfo(CategoryMap::iterator it){return *(it->second);}
public:	 
	bool Initialize();	//delivery启动时调用
	bool LoadConfig();	//读取配置
	void OpenConsign(){consign_open=true;}
	void OpenConsignRole(){consignrole_open=true;}
	void OnDBConnect(Protocol::Manager * manager, int sid);	//连接上DB时调用，读取已有交易
	void OnDBLoad(std::vector<GConsignDB>& list, bool finish);	//加载已有交易
	bool Update();
	~ConsignManager();
	static ConsignManager* GetInstance() { static ConsignManager instance; return &instance; }
//	static int GetPageCount() {return PAGECOUNT;}
	bool ClearBusy(int64_t sn);
	bool SetBusy(int64_t sn);
	void SendPost(const GConsignDB& detail);
	void SendPostRole(const GConsignDB& detail,const GConsignRole& roledetail);
	void SendCancelPost(int uid,int rid, int64_t sn,int stamp);
	static int GetWebRqstRetcode(int errcode);//translate inner errcode to retcode defined in 寻宝网 protocols
	static int GetClientRqstRetcode(int errcode);//translate inner errcode to retcode defined for gclient
	void ClearConsignStarting(int roleid){
		if(consign_starting_set.erase(roleid)==0)
			Log::log(LOG_ERR,"ClearConsignStarting,role not in consign_starting_set. roleid=%d",roleid);
	}
	ConsignMap::iterator CheckEntry( int64_t sn,int& retcode);
	//check if sn is used
	int SN_notfound(int64_t sn);
	int GetFinishedRecord(int64_t sn, FinishedRecord & record);
	//modify used sn cache
	bool OnQueryBackup(int retcode, int64_t sn, const Octets & detail_os);
	//client rqst
	//phase 1, write rqst to db
	int TryPrePost(const GConsignStart& consign, int ip,PlayerInfo& info);
	int TryPreCancelPost(int roleid, int64_t sn, PlayerInfo& ui);
	int TryPrePostRole(const GConsignStartRole& consign, int ip,PlayerInfo& info);//role
	//phase 2, add to ClientRqst Map
	int OnDBPrePost(const GConsignDB & detail);
	int OnDBPrePostRole(const GConsignDB & detail,const GConsignRole& role);//role,call by roletask
	int OnDBPreCancelPost(int roleid, int64_t sn,int stamp);
	//phase 3, get web acknowledge, del from ClientRqst Map, write new state to db
	int RecvPostRe(bool success, int userid, int64_t sn, int postperiod, int showperiod, int sellperiod, int commodity_id);
	int RecvCancelPostRe(bool success, int userid, int64_t sn);
	//phase 4, modify deliver state
	int OnDBPost(int roleid, int64_t sn, int reason,const GConsignItem &item,int commodityid);
	//and OnDBConsignFail
	
	//client query
	bool GetRoleConsignList(int roleid,std::vector<GConsignItem>& items,int localsid);
	void OnDBConsignQuery(int roleid,std::vector<GConsignItem>& items);
	int GetItemObj(int64_t sn,GRoleInventory& obj);
	int GetConsignListRoleWrap(int roleid,std::vector<GConsignListRoleNode>& nodes,int& page,char direction,int category,int64_t oldsn);
	int GetConsignListWrap(int roleid,std::vector<GConsignListNode>& nodes,int& page,char direction,int category,int64_t oldsn);
	template <class T>
	int GetConsignList(int roleid,std::vector<T>& nodes,int& page,char direction,int category,int64_t oldsn);
	int GetConsignListLargeCategory(std::vector<GConsignListNode>& nodes,int& page,int& timestamp);

	//web rqst
	//对于trade server的操作，收到时验证sn,roleid,timestamp，DB返回成功后更新物品状态。
	//phase 1, write new state to db
	int DoShelf(int userid, int roleid, int64_t sn, int price, int64_t actiontime, int showperiod, int sellperiod, int buyerroleid, int64_t messageid, int64_t timestamp);
	int DoShelfCancel(int userid, int roleid, int64_t sn, int64_t messageid, int64_t timestamp);
	int DoWebPostCancel(int userid, int roleid, int64_t sn, int ctype, int64_t messageid, int64_t timestamp);	
	int DoPostExpire(int userid, int roleid, int64_t sn, int64_t messageid, int64_t timestamp);	
	int DoSold(int zoneid, int userid, int roleid, int64_t sn, int buyeruserid, int buyerroleid, int64_t orderid, int stype, int64_t timestamp);//role ,modify
	//phase 2, modify deliver state
	int OnDBShelf(int roleid, int64_t sn, int buyer_roleid, int buyer_userid, int64_t timestamp,const GConsignItem &item, int shelf_time);
	int OnDBCancelShelf(int roleid, int64_t sn, int64_t timestamp);
	int OnDBSold(int roleid, int64_t sn, int64_t timestamp);
	int OnDBSoldRole(int roleid, int64_t sn, int64_t timestamp);//role,call by roletask
	int OnDBConsignFail(int64_t sn,int state, int64_t timestamp);
	int OnDBConsignFailRole(int64_t sn,int state, int64_t timestamp);//role
//	bool OnDBPostExpire(int roleid, int64_t sn);
//	bool OnDBCancelPost(int roleid, int64_t sn);

	//mail sent
	bool OnDBMail(int roleid, int64_t sn){ return RemoveEntry(roleid,sn);}
	void RemoveRoleEntry(int roleid, int64_t sn){
		RoleConsignMap::iterator rit=role_consign_map.find(roleid);
		if(rit!=role_consign_map.end())
			rit->second.erase(sn);
	}
	//clear item cache
	void OnRoleLogout(int roleid);
	//set role data for selling role
	bool SetRoleData(int64_t sn,const GConsignRole& role);
	void OnRolenameChange(int roleid, const Octets & rolename);
private:
	bool IsInited()	{ return status == ST_INITED; }
	bool IsConsignRoleOpen() {return consignrole_open == true;}
	int GetAttendListNum(int roleid);
	void UpdateCacheEntry(const GConsignItem& item,ConsignInfo& info){
		if(item.sn!=0)
			info.itcache = item;
	}
	void AddClientRqst(int state,const ConsignInfo& info,int64_t sn, int stamp, const GConsignDB* pdetail=NULL);
	bool RemoveClientRqst(int64_t sn);
	void AddMailRqst(int64_t sn);
	bool RemoveMailRqst(int64_t sn);
	void AddEntry(const GConsignDB& detail);
	bool RemoveEntry(int roleid, int64_t sn);
	ConsignMap::iterator CheckWebRqst( int64_t sn,int64_t timestamp,int state,int& retcode);
	ConsignMap::iterator CheckRqst( int64_t sn,int state,int& retcode);
	ConsignMap::iterator CheckDBRes( int64_t sn,int64_t timestamp,int state,int& retcode,bool check_timestamp=true);
	void SetToClientState(std::vector<GConsignItem>& items);
	static bool ListAllSetToClientState(GConsignItem& items,int now);
	static bool ClientCanSee(ConsignInfo& info,int now);
	PageInfo GetNextPage(std::vector<Cate_it>& nodes,Cate_it beginning,Cate_it end,int now);
	template <class T>
	int GetPrevPage(std::vector<T>& nodes,Cate_rit beginning,Cate_rit end,int now);
/*	static GConsignListNode GetListNode(ConsignInfo* pinfo){
		return GConsignListNode(pinfo->itcache,pinfo->roleid,pinfo->rolename,pinfo->buyerrid,pinfo->commodityid);
	}*/
	static void GetListNode(GConsignListNode& node,ConsignInfo* pinfo){
		node = GConsignListNode(pinfo->itcache,pinfo->roleid,pinfo->rolename,pinfo->buyerrid,pinfo->commodityid);
		return;
	}
	static void GetListNode(GConsignListRoleNode& node,ConsignInfo* pinfo){
		node = GConsignListRoleNode(pinfo->itcache,pinfo->roleid,pinfo->rolename,pinfo->buyerrid,pinfo->commodityid,pinfo->itcache.money,pinfo->itcache.item_id>>16,pinfo->itcache.item_id&0xff,pinfo->itcache.item_cnt);
		//see gamedbd/dbconsignprepostrole.hrp, use money field as level
		return;
	}
	void BuildPost(Post & p,const GConsignDB& detail);
friend class ConsignShelf;	
};

}
#endif



