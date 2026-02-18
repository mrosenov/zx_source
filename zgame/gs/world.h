#ifndef __ONLINEGAME_GS_WORLD_H__
#define __ONLINEGAME_GS_WORLD_H__

#include <map>
#include <vector>
#include <hashtab.h>
#include <amemory.h>
#include <timer.h>
#include <threadpool.h>
#include <common/types.h>
#include <set>
#include <glog.h>

#include "gimp.h"
#include "grid.h"
#include "msgqueue.h"
#include "terrain.h"
#include "template/itemdataman.h"
#include "worldmanager.h"
#include "npcgenerator.h"
#include "gmatrix.h"

extern abase::timer	g_timer;
class MsgDispatcher;
class GSvrPool;
class CNPCGenMan;
/*
 *	NPCºÍÎïÆ·µÄID×ª»»º¯Êı
 */
inline int ID2WIDX(int id) { return 0;}
inline int ID2IDX(int id) { return id & 0x0FFFFFFF; }
inline int MKOBJID(int worldid,int idx)
{
	int tmp = (idx & 0x0FFFFFFF);
	return tmp;
}

inline void MAKE_ID(XID & id, int u_id)
{
	id.id = u_id;
	if((u_id & 0x80000000) == 0)
	{
		id.type = GM_TYPE_PLAYER;
	}
	else if((u_id & 0x40000000) == 0)
	{
		id.type = GM_TYPE_NPC;
	}
	else if((u_id & 0x20000000) == 0)
	{
		id.type = GM_TYPE_MATTER;
	}
	else if((u_id & 0x10000000) == 0) //·ÖÉí
	{
		id.type = GM_TYPE_NPC;
	}
}

//Í¨¹ıattack message ×¨ÓÃID×ª»»£¬ ±ÜÃâµÄ³èÎïµÄÅĞ¶¨ ÏÖÔÚÕâ¸öÅĞ¶¨¾Í¿ÉÒÔÊ¹ÓÃ
inline bool IS_HUMANSIDE(const XID & id) { return id.type == GM_TYPE_PLAYER;}

class gclonenpc;

template<typename T> inline int MERGE_ID(int oldid);
template<> inline int MERGE_ID<gnpc>(int oldid) { return oldid | 0x80000000;}
template<> inline int MERGE_ID<gclonenpc>(int oldid) { return oldid | 0xE0000000;}
template<> inline int MERGE_ID<gmatter>(int oldid) { return oldid | 0xC0000000;}
template<> inline int MERGE_ID<gplayer>(int oldid) { return oldid;}

class  world_data_ctrl
{
//Õâ¸öÀàÓÃÓÚÊÀ½çµÄÊı¾İ¿ØÖÆ£¬»áÌí¼ÓÒ»ÏµÁĞµÄĞéº¯Êı
	public:
	virtual ~world_data_ctrl() {}
	virtual world_data_ctrl * Clone() = 0;
	virtual void Reset() = 0;
	virtual void Tick(world * pPlane) = 0;
	virtual void BattleFactionSay(int faction ,const void * buf, size_t size) = 0;
	virtual void BattleSay(const void * buf, size_t size) = 0;
};

class world
{
	typedef abase::hashtab<int,int,abase::_hash_function,abase::fast_alloc<> >	query_map;//ÓÃ»§µÄ²éÑ¯±í

	grid 	w_grid;
	int 	w_index;	//ÊÀ½çÇøÓòµÄË÷Òı
	int     w_tag;      //ÊÀ½çTag
	query_map w_player_map;	//Íæ¼ÒµÄ²éÕÒ±í
//	query_map w_npc_map;	//ÔÚ±¾µØµÄÍâ²¿npc²éÑ¯±í
	int	w_pmap_lock;	//player µÄuserid -> index idµÄmapËø
	int	w_nmap_lock;	//npc Íâ²¿npc-->±¾µØÁ¬½ÓµÄ²éÑ¯±í
	int 	w_message_counter[GM_MSG_MAX];	//ÏûÏ¢ÊıÄ¿µÄ¼ÆÊı±í
	npc_generator 	w_npc_gen; //¹ÖÎïÉú³É¹ÜÀíÆ÷
public:
	struct off_node_t{
		int idx_off;
		int x_off;
		int z_off;
		off_node_t(grid & grid,int offset_x,int offset_y):x_off(offset_x),z_off(offset_y)
		{
			idx_off = offset_y*grid.reg_column + offset_x;
		}
		bool operator==(const off_node_t & rhs) const 
		{
			return rhs.idx_off == idx_off;
		}
	};
	abase::vector<off_node_t,abase::fast_alloc<> > w_off_list;
	int	w_near_vision;		//ÔİÊ±Ã»ÓĞÊ¹ÓÃ£¬½üµãµÄË÷Òı
	int	w_far_vision;		//×îÔ¶µÄ¾àÀëËùº­¸ÇµÄ·¶Î§
	int	w_true_vision;		//ÍêÈ«¿ÉÊÓµÄ·¶Î§,ÔİÊ±Ã»ÓĞÊ¹ÓÃ
	int	w_plane_index;		//ÔÚÊÀ½çÖĞµÄÎ»ÃæË÷Òı
	int	w_player_count;		//±¾ÊÀ½çÖĞÍæ¼ÒµÄÊıÄ¿
	float	w_vision;		//ÊÓÒ°·¶Î§£¬ºÍfar_vision¶ÔÓ¦µÄ¾àÀë
	world_manager * w_world_man;
	int 	w_obsolete;		//ÓÉmanagerÊ¹ÓÃÕâ¸ö±äÁ¿
	instance_hash_key w_ins_key;	//¸±±¾Ê¹ÓÃµÄinstance_key £¬ ÓÉmanagerÊ¹ÓÃÕâ¸ö±äÁ¿
	int	w_activestate;		//¼¤»î×´Ì¬ 0:Î´¼¤»î 1:¼¤»î 2:ÀäÈ´  ÓÉmanagerÀ´¿ØÖÆ
	int	w_index_in_man;		//ÔÚ¹ÜÀíÆ÷ÖĞµÄË÷Òı£¬ÎªMsgQueue2ºÍmanagerËùÊ¹ÓÃ
	int	w_create_timestamp;	//´´½¨µÄÊ±¼ä´Á£¬ÓÉmanagerÊ¹ÓÃ 
	int	w_destroy_timestamp;	//É¾³ıµÄÊ±¼ä´Á£¬ÓÉmanagerÊ¹ÓÃ ²»ÊÇËùÓĞµÄ¸±±¾Õâ¸ö¶¼ÓĞĞ§µÄ
	int	w_ins_kick;		//ÊÇ·ñÌß³öinstance key²»·ûºÏµÄÍæ¼Ò
	int	w_battle_result;	//¸øÕ½³¡ÓÃµÄ Õ½³¡½á¹û
	int	w_offense_goal;		//Õ½³¡¹¥·½Ä¿±ê
	int	w_offense_cur_score;	//Õ½³¡¹¥·½µÃ·Ö
	int	w_defence_goal;		//Õ½³¡ÊØ·½Ä¿±ê
	int	w_defence_cur_score;	//Õ½³¡ÊØ·½µÃ·Ö
	int 	w_end_timestamp; 	//Õ½³¡½áÊøÊ±¼ä£¬µ½´ËÊ±¼äÊ±£¬ËùÓĞÍæ¼Ò¶¼½«±»×Ô¶¯Ìß³ö 
					//Ö»ÓĞbatle_resultÓĞĞ§ºó,´ËÖµ²Å»á±»Ê¹ÓÃ

public:
//³õÊ¼»¯º¯Êı
	world();
	~world();
	bool 	Init(int world_index, int world_tag);
	void 	InitManager(world_manager * man) { w_world_man = man;}
	bool 	InitNPCGenerator(CNPCGenMan & npcgen);
	bool	TriggerSpawn(int condition);
	bool 	ClearSpawn(int condition);
	void 	InitTimerTick();
	bool 	CreateGrid(int row,int column,float step,float startX,float startY);
	int	BuildSliceMask(float near,float far);			//´´½¨¾àÀë²éÑ¯ËùĞèÒªµÄmask

	void 	DuplicateWorld(world * dest) const;	//¸´ÖÆÊÀ½ç£¬³ıÁËNPCÉú³ÉÆ÷ .....
	
	inline world_manager * GetWorldManager() { return w_world_man;}
	inline int GetTag() { return w_tag; }
	inline int GetWorldIndex() { return w_index; }
	//·ÖÅäÒ»¸öNPCÊı¾İ£¬·µ»ØÒ»¸öÉÏÁËËøÁËNPC½á¹¹
	inline gnpc 	*AllocNPC() 
	{ 
		gnpc *pNPC = gmatrix::AllocNPC(); 
		if(pNPC) pNPC->plane = this;
		return pNPC;
	}
	inline void 	FreeNPC(gnpc* pNPC) 
	{ 
		ASSERT(pNPC->plane == this);
		pNPC->plane = NULL;
		return gmatrix::FreeNPC(pNPC); 
	}

	inline bool CheckPlayerDropCondition()
	{
		return gmatrix::CheckPlayerDropCondition();
	}

	//·ÖÅäÒ»¸öMatterÊı¾İ£¬·µ»ØÒ»¸öÉÏÁËËøµÄMatter½á¹¹
	inline gmatter *AllocMatter() 
	{ 
		gmatter * pMatter = gmatrix::AllocMatter(); 
		 if(pMatter) pMatter->plane = this;
		return pMatter;
	}
	inline void 	FreeMatter(gmatter *pMatter) 
	{
		ASSERT(pMatter->plane == this);
		pMatter->plane = NULL;
		return gmatrix::FreeMatter(pMatter); 
	}

	//	Ôö¼Ó/É¾³ıÍæ¼ÒºÍ¶ÔÏóµÄº¯Êı
	//·ÖÅäÒ»¸öÍæ¼ÒÊı¾İ£¬²¢·µ»ØÒ»¸öËø¶¨µÄÍæ¼Ò½á¹¹
	inline gplayer *AllocPlayer() 
	{ 
		gplayer * pPlayer = gmatrix::AllocPlayer(); 
		if(pPlayer) 
		{
			pPlayer->plane = this;
			interlocked_increment(&w_player_count);
		}
		return pPlayer;
	}

	inline void 	FreePlayer(gplayer * pPlayer)
	{
		if(w_world_man)
		{
			w_world_man->PlayerLeaveThisWorld(w_plane_index,pPlayer->ID.id);
		}
		interlocked_decrement(&w_player_count);
		ASSERT(pPlayer->plane == this);
		pPlayer->plane = NULL;
		return gmatrix::FreePlayer(pPlayer);
	}

	inline int GetPlayerInWorld() 
	{
		return w_player_count; 
	}

	inline void InsertPlayerToMan(gplayer *pPlayer) 
	{ 
		gmatrix::InsertPlayerToMan(pPlayer);
	}
	inline void RemovePlayerToMan(gplayer *pPlayer) 
	{ 	
		gmatrix::RemovePlayerToMan(pPlayer);
	}

	int InsertPlayer(gplayer *);		//¸ù¾İÎ»ÖÃ£¬²åÈëÒ»¸ö¶ÔÏóµ½ÊÀ½çÖĞ£¬·µ»Ø²åÈëµÄÇøÓòË÷ÒıºÅ
	int InsertNPC(gnpc*);			//¸ù¾İÎ»ÖÃ£¬²åÈëÒ»¸ö¶ÔÏóµ½ÊÀ½çÖĞ£¬·µ»Ø²åÈëµÄÇøÓòË÷ÒıºÅ
	int InsertMatter(gmatter *);		//¸ù¾İÎ»ÖÃ£¬²åÈëÒ»¸ö¶ÔÏóµ½ÊÀ½çÖĞ£¬·µ»Ø²åÈëµÄÇøÓòË÷ÒıºÅ
	
	void RemovePlayer(gplayer *pPlayer); 	//´ÓÊÀ½çÖĞÒÆ³öÒ»¸ö¶ÔÏó£¬²»free
	void RemoveNPC(gnpc *pNPC);		//´ÓÊÀ½çÖĞÒÆ³öÒ»¸ö¶ÔÏó£¬²»free
	void RemoveMatter(gmatter *pMatter);	//´ÓÊÀ½çÖĞÒÆ³öÒ»¸ö¶ÔÏó£¬²»free

	//´Ó¹ÜÀíÆ÷ÖĞÒÆ³öNPC£¬ÓÃÓÚ²»ÔÚ³¡¾°ÖĞµÄnpc
	inline void RemoveNPCFromMan(gnpc * pNPC)
	{
		gmatrix::RemoveNPCFromMan(pNPC);
	}

	inline void RemoveMatterFromMan(gmatter * pMatter)
	{
		gmatrix::RemoveMatterFromMan(pMatter);
	}

	void Release();

public:
//	È¡µÃÊôĞÔµÄinlineº¯Êı
	inline gmatter * GetMatterByIndex(size_t index) const  { return gmatrix::GetMatterByIndex(index);}
	inline gplayer*  GetPlayerByIndex(size_t index) const   {return gmatrix::GetPlayerByIndex(index);}
	inline gnpc* 	 GetNPCByIndex(size_t index) const   { return gmatrix::GetNPCByIndex(index);}
	inline size_t GetPlayerIndex(gplayer *pPlayer)  const  { return gmatrix::GetPlayerIndex(pPlayer);}
	inline size_t GetMatterIndex(gmatter *pMatter)  const  { return gmatrix::GetMatterIndex(pMatter);}
	inline size_t GetNPCIndex(gnpc *pNPC)  const  { return gmatrix::GetNPCIndex(pNPC);}
	inline grid&	 GetGrid() { return w_grid;}
	inline const rect & GetLocalWorld() { return w_grid.local_region;}
	inline bool PosInWorld(const A3DVECTOR & pos)
	{
		return w_grid.IsLocal(pos.x,pos.z);
	}

	inline bool MapPlayer(int uid,int index) { 
		spin_autolock alock(w_pmap_lock); 
		return w_player_map.put(uid,index);
	}
	
	inline int UnmapPlayer(int uid) {
		spin_autolock alock(w_pmap_lock);
		return w_player_map.erase(uid);
	}

	inline int FindPlayerIndex(int uid)
	{
		return gmatrix::FindPlayer(uid);
	}
	
	inline gplayer *  GetPlayerByID(int uid) {
		int index = gmatrix::FindPlayer(uid);
		if(index < 0) return NULL;
		gplayer * pPlayer = GetPlayerByIndex(index);
		if(pPlayer->plane == this) 
			return pPlayer;
		else 
			return NULL;
	}

	inline int GetPlayerCount()
	{
		spin_autolock alock(w_pmap_lock); 
		return w_player_map.size();
	}

	//
	enum
	{
		QUERY_OBJECT_STATE_ACTIVE = 0x01,
		QUERY_OBJECT_STATE_ZOMBIE = 0x02,
		QUERY_OBJECT_STATE_DISCONNECT = 0x04,
	};
	struct object_info
	{
		int state;
		int tag;
		A3DVECTOR pos;
		float body_size;
		short cls;
		short gender;
		int faction;
		int level;
		int hp;
		int mp;
		int tid;	//npc or matter
		int invisible_rate;
		int anti_invisible_rate;
	};

	bool QueryObject(const XID & id,object_info & info,bool other_plane = false);	//²éÑ¯Ò»¸öÆäËû¶ÔÏóµÄ×´Ì¬
	
	void GetColumnPlayers(std::vector<XID>& player_list, const A3DVECTOR& src, const A3DVECTOR& target, int self, float fRadius, float height);


public:
	void RunTick();		//ÓÉmanager¿ØÖÆµ÷ÓÃ»òÕßÄÚ²¿×Ô¶¯µ÷ÓÃ
	void ResetWorld();	//ÖØÖÃÊÀ½ç£¬Ö»ÓĞ¸±±¾²Å»áµ÷ÓÃÕâ¸ö£¨Õâ¸ö²Ù×÷»áÖØÉúËùÓĞµÄ¹ÖÎïµÈ£©
	void DumpMessageCount();

	void SetWorldCtrl(world_data_ctrl * ctrl);
	world_data_ctrl * w_ctrl;
private:
	friend class MsgQueue;

	int HandlePlaneMessage(const MSG & msg);
public:


	/*
	 *	¹ã²¥ÏûÏ¢£¬°´ÕÕ¾àÀë½«°ü×ª·¢¸øÖÜÎ§µÄËùÓĞ¶ÔÏ
	 *	msg.target µÄÀàĞÍ¾ö¶¨ÁËÊÕµ½Õß£¬msg.targetµÄID±ØĞëÎª-1
	 *	maskÓÃÓÚ¹ıÂËÏûÏ¢½ÓÊÕ¶ÔÏó
	 *	ÔÚ×ùÕâ¸ö¹ã²¥Ê±£¬»á×Ô¶¯ÅĞ¶ÏÊÇ·ñÒª×ª·¢µ½ÆäËûµÄ·şÎñÆ÷ÉÏ
	 *	msg.source²»»áÊÕµ½Õâ¸ö¹ã²¥ÏûÏ¢
	 */
	int BroadcastMessage(const MSG & message,float fRadius,int mask); 		

	/*
	 *	¹ã²¥ºĞĞÎÏûÏ¢£¬°´ÕÕ¾àÀë½«°ü×ª·¢¸øÖÜÎ§µÄËùÓĞ¶ÔÏó
	 *	msg.target µÄÀàĞÍ¾ö¶¨ÁËÊÕµ½Õß£¬msg.targetµÄID±ØĞëÎª-1
	 *	msg.source²»»áÊÕµ½Õâ¸ö¹ã²¥ÏûÏ¢
	 */
	int BroadcastLocalBoxMessage(const MSG & message,const rect & rt);

	/*
	 *	¹ã²¥ÇòĞÎÏûÏ¢£¬°´ÕÕ¾àÀë½«°ü×ª·¢¸øÖÜÎ§µÄËùÓĞ¶ÔÏó
	 *	msg.target µÄÀàĞÍ¾ö¶¨ÁËÊÕµ½Õß£¬msg.targetµÄID±ØĞëÎª-1
	 *	maskÓÃÓÚ¹ıÂËÏûÏ¢½ÓÊÕ¶ÔÏó(ÒÑ¾­±»È¡Ïû)
	 *	ÔÚ×ùÕâ¸ö¹ã²¥Ê±£¬»á×Ô¶¯ÅĞ¶ÏÊÇ·ñÒª×ª·¢µ½ÆäËûµÄ·şÎñÆ÷ÉÏ
	 *	msg.source²»»áÊÕµ½Õâ¸ö¹ã²¥ÏûÏ¢
	 */
	int BroadcastSphereMessage(const MSG & message,const A3DVECTOR & target, float fRadius,size_t max_count, std::vector<exclude_target>& targets_exclude);

	/*
	 *	¹ã²¥ÖùĞÎÏûÏ¢£¬°´ÕÕ¾àÀë½«°ü×ª·¢¸øÖÜÎ§µÄËùÓĞ¶ÔÏó,¸Ã¶ÔÏó±ØĞëÔÚÖùÖĞ
	 *	ÖùµÄÆğÊ¼×ø±êÔÚmessageÖĞ£¬ÖÕµãÊÇtarget
	 *	msg.target µÄÀàĞÍ¾ö¶¨ÁËÊÕµ½Õß£¬msg.targetµÄID±ØĞëÎª-1
	 *	maskÓÃÓÚ¹ıÂËÏûÏ¢½ÓÊÕ¶ÔÏó
	 *	ÔÚ×ùÕâ¸ö¹ã²¥Ê±£¬»á×Ô¶¯ÅĞ¶ÏÊÇ·ñÒª×ª·¢µ½ÆäËûµÄ·şÎñÆ÷ÉÏ
	 *	msg.source²»»áÊÕµ½Õâ¸ö¹ã²¥ÏûÏ¢
	 */
	int BroadcastCylinderMessage(const MSG & message,const A3DVECTOR & target, float fRadius,size_t max_count, std::vector<exclude_target>& targets_exclude);

	/*
	 *	¹ã²¥×µĞÎÏûÏ¢£¬ÔÚ×µÖĞµÄ¶ÔÏó»áÊÕµ½Õâ¸öÏûÏ¢
	 *	Ô²×¶µÄÔ²ĞÄ¼´ÎªÏûÏ¢µÄ·¢³öµã
	 *	msg.target µÄÀàĞÍ¾ö¶¨ÁËÊÕµ½Õß£¬msg.targetµÄID±ØĞëÎª-1
	 *	maskÓÃÓÚ¹ıÂËÏûÏ¢½ÓÊÕ¶ÔÏó
	 *	ÔÚ×ùÕâ¸ö¹ã²¥Ê±£¬»á×Ô¶¯ÅĞ¶ÏÊÇ·ñÒª×ª·¢µ½ÆäËûµÄ·şÎñÆ÷ÉÏ
	 *	msg.source²»»áÊÕµ½Õâ¸ö¹ã²¥ÏûÏ¢
	 */
	int BroadcastTaperMessage(const MSG & message,const A3DVECTOR & target,float fRadius,float cos_halfangle,size_t max_count,std::vector<exclude_target>& targets_exclude);


	/*
	 *	Í¬BroadcastMessage£¬Î¨Ò»µÄÇø±ğÊÇÖ»ÔÚ±¾µØ×öÏàÓ¦µÄ×ª·¢²Ù×÷
	 *	BroadcastMessageµÄ±¾µØ·¢ËÍÊÇÍ¨¹ıµ÷ÓÃ±¾º¯ÊıÍê³ÉµÄ
	 */
	int BroadcastLocalMessage(const MSG & message,float fRadius,int mask);

	/*
	 *	¹ã²¥ÇòĞÎÏûÏ¢£¬°´ÕÕ¾àÀë½«°ü×ª·¢¸øÖÜÎ§µÄËùÓĞ¶ÔÏó
	 *	msg.target µÄÀàĞÍ¾ö¶¨ÁËÊÕµ½Õß£¬msg.targetµÄID±ØĞëÎª-1
	 *	maskÓÃÓÚ¹ıÂËÏûÏ¢½ÓÊÕ¶ÔÏó
	 *	ÔÚ×ùÕâ¸ö¹ã²¥Ê±£¬»á×Ô¶¯ÅĞ¶ÏÊÇ·ñÒª×ª·¢µ½ÆäËûµÄ·şÎñÆ÷ÉÏ
	 *	msg.source²»»áÊÕµ½Õâ¸ö¹ã²¥ÏûÏ¢
	 */
	int BroadcastLocalSphereMessage(const MSG & message,const A3DVECTOR & target, float fRadius,size_t max_count, std::vector<exclude_target>& targets_exclude);

	/*
	 *	¹ã²¥ÖùĞÎÏûÏ¢£¬°´ÕÕ¾àÀë½«°ü×ª·¢¸øÖÜÎ§µÄËùÓĞ¶ÔÏó,¸Ã¶ÔÏó±ØĞëÔÚÖùÖĞ
	 *	ÖùµÄÆğÊ¼×ø±êÔÚmessageÖĞ£¬ÖÕµãÊÇtarget
	 *	msg.target µÄÀàĞÍ¾ö¶¨ÁËÊÕµ½Õß£¬msg.targetµÄID±ØĞëÎª-1
	 *	maskÓÃÓÚ¹ıÂËÏûÏ¢½ÓÊÕ¶ÔÏó
	 *	ÔÚ×ùÕâ¸ö¹ã²¥Ê±£¬»á×Ô¶¯ÅĞ¶ÏÊÇ·ñÒª×ª·¢µ½ÆäËûµÄ·şÎñÆ÷ÉÏ
	 *	msg.source²»»áÊÕµ½Õâ¸ö¹ã²¥ÏûÏ¢
	 */
	int BroadcastLocalCylinderMessage(const MSG & message,const A3DVECTOR & target, float fRadius,size_t max_count, std::vector<exclude_target>& targets_exclude);

	/*
	 *	¹ã²¥×µĞÎÏûÏ¢£¬ÔÚ×µÖĞµÄ¶ÔÏó»áÊÕµ½Õâ¸öÏûÏ¢
	 *	Ô²×¶µÄÔ²ĞÄ¼´ÎªÏûÏ¢µÄ·¢³öµã
	 *	msg.target µÄÀàĞÍ¾ö¶¨ÁËÊÕµ½Õß£¬msg.targetµÄID±ØĞëÎª-1
	 *	maskÓÃÓÚ¹ıÂËÏûÏ¢½ÓÊÕ¶ÔÏó
	 *	ÔÚ×ùÕâ¸ö¹ã²¥Ê±£¬»á×Ô¶¯ÅĞ¶ÏÊÇ·ñÒª×ª·¢µ½ÆäËûµÄ·şÎñÆ÷ÉÏ
	 *	msg.source²»»áÊÕµ½Õâ¸ö¹ã²¥ÏûÏ¢
	 */
	int BroadcastLocalTaperMessage(const MSG & message,const A3DVECTOR & target,float fRadius,float cos_halfangle,size_t max_count,std::vector<exclude_target>& targets_exclude);


	int DispatchPlaneMessage(const MSG & message);
	
	/*
	 *	¼ì²éÒ»¸ö×ø±ê¼ÓÖµºóÊÇ·ñÈÔÈ»ÔÚÕıÈ··¶Î§ÄÚ,ÄÚ²¿Ê¹ÓÃµÄº¯Êı.
	 */
	inline static bool check_index(const grid * g,int x,int z, const world::off_node_t &node)
	{
		int nx = x + node.x_off;
		if(nx < 0 || nx >= g->reg_column) return false;
		int nz = z + node.z_off;
		if(nz < 0 || nz >= g->reg_row) return false;
		return true;
	}

	int GetRegionPlayerCount(int id, A3DVECTOR & pos, float & radius, std::vector<exclude_target>& targets_excludes);
	int GetRegionPlayer(abase::vector<gobject*,abase::fast_alloc<> > &list, int id, const A3DVECTOR & target, float & fRadius, std::vector<exclude_target>& targets_exclude);

	void BattleFactionSay(int faction, const void * msg, size_t size);
	void BattleSay(const void * msg, size_t size);


public:
//Ä£°åº¯Êı½Ó¿Ú
	/*
	 *	µ±Ò»¸ö¶ÔÏóÔÚÁ½¸ö¸ñ×Ó¼äÒÆ¶¯,ÅĞ¶Ï¸Ã¶ÔÏóÀë¿ªÁËÄÄĞ©¸ñ×ÓµÄÊÓÒ°,»áµ÷ÓÃÏàÓ¦µÄenterºÍleaveº¯Êı¶ÔÏó
	 */
	template <typename ENTER,typename LEAVE>
	inline void MoveBetweenSlice(slice * pPiece, slice * pNewPiece,ENTER enter,LEAVE leave)
	{
		int i;
		grid * pGrid = &GetGrid();
		int ox,oy,nx,ny;
		pGrid->GetSlicePos(pPiece,ox,oy);
		pGrid->GetSlicePos(pNewPiece,nx,ny);
		float vision = w_vision + pGrid->slice_step - 1e-3;
		float dis = pNewPiece->Distance(pPiece);
		if(dis > vision)
		{
			//±¾¸ñµÄÎŞ·¨¿´¼û ËùÒÔÒª½øĞĞÀë¿ª±¾¸ñµÄ²Ù×÷,ºóÃæµÄÑ­»·²¢Ã»ÓĞÅĞ¶Ï±¾¸ñ
			leave(pPiece);
			enter(pNewPiece);
			if(dis > vision*2)
			{
				for(i = 0; i < w_far_vision; i ++)
				{
					const world::off_node_t &node = w_off_list[i];
					slice * pTmpPiece = pPiece + node.idx_off;
					leave(pTmpPiece);
				}

				for(i = 0; i < w_far_vision; i ++)
				{
					const world::off_node_t &node = w_off_list[i];
					slice * pTmpPiece = pNewPiece + node.idx_off;
					enter(pTmpPiece);
				}
				return ;
			}
		}

		for(i = 0; i < w_far_vision; i ++)
		{
			const world::off_node_t &node = w_off_list[i];
			if(check_index(pGrid,ox,oy,node)) 
			{
				slice * pTmpPiece = pPiece + node.idx_off;
				if(pTmpPiece->Distance(pNewPiece) > vision && pTmpPiece->IsInWorld())
				{
					leave(pTmpPiece);
					//Àë¿ªÁËÕâ¸öslice
				}
			}

			if(check_index(pGrid,nx,ny,node))
			{
				slice * pTmpPiece = pNewPiece + node.idx_off;
				if(pTmpPiece->Distance(pPiece) > vision && pTmpPiece->IsInWorld())
				{
					enter(pTmpPiece);
				}
			}
		}
	}

	/*
	 *	É¨Ãè¸½½üËùÓĞµÄĞ¡¸ñ×Ó,°´ÕÕÔ¤¶¨µÄ·¶Î§À´É¨Ãè,ÕâÀï²»½øĞĞ¸ñ×ÓÊÇ·ñÔÚ±¾·şÎñÆ÷µÄÅĞ¶Ï 
	 */
	template <typename FUNC >
	inline void ForEachSlice(slice * pStart, FUNC func,int vlevel = 0)
	{
		int total = vlevel?w_near_vision:w_far_vision;
		int slice_x,slice_z;
		GetGrid().GetSlicePos(pStart,slice_x,slice_z);
		for(int i = 0; i <total; i ++)
		{
			off_node_t &node = w_off_list[i]; 
			int nx = slice_x + node.x_off; 
			int nz = slice_z + node.z_off; 
			if(nx < 0 || nz < 0 || nx >= GetGrid().reg_column || nz >= GetGrid().reg_row) continue;
			slice * pNewPiece = pStart+ node.idx_off;
			func(i,pNewPiece);
		}
	}

	/*
	 * °´ÕÕÎ»ÖÃºÍ·¶Î§É¨Ãè¸½½üËùÓĞµÄĞ¡¸ñ,²¢ÇÒÒÀ´Îµ÷ÓÃÏàÓ¦µÄ´¦Àíº¯Êı¶ÔÏó
	 * ÕâÀï»áÊ×ÏÈÅĞ¶Ï¸ñ×ÓÊÇ·ñÔÚµ±Ç°·şÎñÆ÷ÖĞ,·ñÔò²»»á·¢ËÍµ½funcº¯ÊıÖĞ
	 */
	template <typename FUNC>
	inline void ForEachSlice(const A3DVECTOR &pos, float fRadius, FUNC func)
	{
		grid * pGrid = &GetGrid();
		float fx = pos.x - pGrid->grid_region.left;
		float fz = pos.z - pGrid->grid_region.top;
		float inv_step = pGrid->inv_step;
		int ofx1 = (int)((fx - fRadius) * inv_step);
		int ofx2 = (int)((fx + fRadius) * inv_step);
		int ofz1 = (int)((fz - fRadius) * inv_step);
		int ofz2 = (int)((fz + fRadius) * inv_step);
		if(ofx1 < 0) ofx1 = 0;
		if(ofx2 >= pGrid->reg_column) ofx2 = pGrid->reg_column -1;
		if(ofz1 < 0) ofz1 = 0;
		if(ofz2 >= pGrid->reg_row) ofz2 = pGrid->reg_row - 1;

		slice * pPiece = pGrid->GetSlice(ofx1,ofz1);
		for(int i = ofz1;i <= ofz2; i ++,pPiece += pGrid->reg_column)
		{
			slice * pStart = pPiece;
			for(int j = ofx1; j <= ofx2; j ++, pStart++)
			{
				if(pStart->IsInWorld()) func(pStart,pos);
			}
		}
	}

	template <typename FUNC>
	inline void ForEachSlice(const A3DVECTOR &pos, const rect & rt, FUNC func)
	{
		grid * pGrid = &GetGrid();
		
		float inv_step = pGrid->inv_step;
		int ofx1 = (int)((rt.left   - pGrid->grid_region.left) * inv_step);
		int ofx2 = (int)((rt.right  - pGrid->grid_region.left) * inv_step);
		int ofz1 = (int)((rt.top    - pGrid->grid_region.top ) * inv_step);
		int ofz2 = (int)((rt.bottom - pGrid->grid_region.top ) * inv_step);
		if(ofx1 < 0) ofx1 = 0;
		if(ofx2 >= pGrid->reg_column) ofx2 = pGrid->reg_column -1;
		if(ofz1 < 0) ofz1 = 0;
		if(ofz2 >= pGrid->reg_row) ofz2 = pGrid->reg_row - 1;

		slice * pPiece = pGrid->GetSlice(ofx1,ofz1);
		for(int i = ofz1;i <= ofz2; i ++,pPiece += pGrid->reg_column)
		{
			slice * pStart = pPiece;
			for(int j = ofx1; j <= ofx2; j ++, pStart++)
			{
				if(pStart->IsInWorld()) func(pStart,pos);
			}
		}
	}

	template <typename FUNC>
	inline void ForEachSlice(const A3DVECTOR &pos1, const A3DVECTOR & pos2, FUNC func)
	{
		rect rt(pos1,pos2);
		ForEachSlice(pos1,rt,func);
	}

	template <int foo>
		inline static void InspirePieceNPC(slice * pPiece,int tick)
		{
			int timestamp = pPiece->idle_timestamp;
			if(tick - timestamp < 40)
			{
				return;
			}
			pPiece->Lock();
			if(tick - pPiece->idle_timestamp < 40)	//ÓÉpiece¾ö¶¨Ã¿Á½ÃëÉèÖÃÒ»´Î
			{
				pPiece->Unlock();
				return;
			}
			pPiece->idle_timestamp = tick;
			gnpc * pNPC = (gnpc*)(pPiece->npc_list);
			while(pNPC)
			{
				pNPC->idle_timer = NPC_IDLE_TIMER;
				pNPC = (gnpc*)(pNPC->pNext);
			}
			pPiece->Unlock();
		}
	/*
	 *	É¨Ãè¸½½üËùÓĞµÄĞ¡¸ñ×Ó,°´ÕÕÔ¤¶¨µÄ·¶Î§À´É¨Ãè,ÕâÀï²»½øĞĞ¸ñ×ÓÊÇ·ñÔÚ±¾·şÎñÆ÷µÄÅĞ¶Ï 
	 */
	template <int foo>
	inline void InspireNPC(slice * pStart, int vlevel = 0)
	{
		int total = vlevel?w_near_vision:w_far_vision;
		int slice_x,slice_z;
		GetGrid().GetSlicePos(pStart,slice_x,slice_z);
		int tick = g_timer.get_tick();
		InspirePieceNPC<0>(pStart,tick);

		for(int i = 0; i <total; i ++)
		{
			off_node_t &node = w_off_list[i]; 
			int nx = slice_x + node.x_off; 
			int nz = slice_z + node.z_off; 
			if(nx < 0 || nz < 0 || nx >= GetGrid().reg_column || nz >= GetGrid().reg_row) continue;
			slice * pNewPiece = pStart+ node.idx_off;
			InspirePieceNPC<0>(pNewPiece,tick);
		}
	}

private:
	void CheckGSvrPoolUpdate();						//¼ì²éµ±Ç°ÔÚÏßµÄÓÎÏ··şÎñÆ÷ÁĞ±í
	void ConnectGSvr(int index, const char * ipaddr, const char * unixaddr);	//Á¬½ÓÁíÍâÒ»Ì¨·şÎñÆ÷ 

	/*
	 * Õâ¸öÀàÊÇ¶¨ÆÚË¢ĞÂÓÎÏ··şÎñÆ÷µÄÀà,Ëü½«×÷ÎªÏß³Ì³ØµÄÒ»¸öÈÎÎñÀ´½øĞĞ
	 * Éú³ÉËüµÄµØ·½ÊÇÏà¹ØµÄ¶¨Ê±Æ÷º¯Êı
	 */

	int _message_handle_count;				//¼ÇÂ¼µ±Ç°message´¦ÀíµÄÇ¶Ì× 

private:
	//ÏûÏ¢·¢ËÍÆ÷µÄº¯Êı£¬ÕâÁ½¸öº¯ÊıÖ»ÓĞÏûÏ¢·¢ËÍÆ÷Á¬½ÓÉÏºÍ¶Ï¿ªÊ±µ÷ÓÃ
};

template <typename WRAPPER>
inline int WrapObject(WRAPPER & wrapper,controller * ctrl, gobject_imp * imp, dispatcher * runner)
{
	ctrl->SaveInstance(wrapper);
	imp->SaveInstance(wrapper);
	runner->SaveInstance(wrapper);
	return 0;
}

template <typename WRAPPER,typename OBJECT>
inline int RestoreObject(WRAPPER & wrapper,OBJECT *obj,world * pPlane)
{
	controller * ctrl =NULL;
	gobject_imp * imp = NULL;
	dispatcher * runner = NULL;

	ctrl = substance::DynamicCast<controller>(substance::LoadInstance(wrapper));
	if(ctrl) imp = substance::DynamicCast<gobject_imp>(substance::LoadInstance(wrapper));
	if(imp) runner = substance::DynamicCast<dispatcher>(substance::LoadInstance(wrapper));
	if(!ctrl || !runner ||!imp) 
	{
		delete imp;
		delete ctrl;
		return -1;
	}
	obj->imp = imp;
	imp->_runner = runner;
	imp->_commander = ctrl;
	imp->Init(pPlane,obj);
	ctrl->Init(imp);
	runner->init(imp);
	//ÒÔºó»¹ĞèÒªµ÷ÓÃReInit
	return 0;
}


#endif

