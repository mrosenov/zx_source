#ifndef __ONLINEGAME_COMMON_MESSAGE_H__
#define __ONLINEGAME_COMMON_MESSAGE_H__

#include <stdlib.h>
#include <amemory.h>
#include <ASSERT.h>
#include "types.h"
struct MSG 
{
	int 	message;	//ÏûÏ¢µÄÀàĞÍ
	struct XID target;	//ÊÕÏûÏ¢µÄÄ¿±ê£¬¿ÉÄÜÊÇ·şÎñÆ÷£¬Íæ¼Ò£¬ÎïÆ·£¬NPCµÈ
	struct XID source;	//´ÓÄÄÀï·¢¹ıÀ´µÄ£¬¿ÉÄÜµÄidºÍÉÏÃæÒ»Ñù
	A3DVECTOR pos;		//ÏûÏ¢·¢³öÊ±µÄÎ»ÖÃ£¬ÓĞµÄÏûÏ¢¿ÉÄÜÎ»ÖÃ²¢ÎŞ×÷ÓÃ
	int	param;		//Ò»¸ö²ÎÊı£¬Èç¹ûÕâ¸ö²ÎÊı¹»ÓÃ£¬ÄÇÃ´¾ÍÊ¹ÓÃÕâ¸ö²ÎÊı
	int 	param2;		//µÚ¶ş²ÎÊı£¬ÒòÎª³£³£·¢ÏÖĞèÒªÕâ¸ö²ÎÊı
	size_t 	content_length;	//ÏûÏ¢µÄ¾ßÌåÊı¾İ³¤¶È
	const void * content;	//ÏûÏ¢µÄ¾ßÌåÊı¾İ ÍøÂçÉÏ´«²¥Ê±Õâ¸ö×Ö¶ÎÎŞĞ§
private:
	enum {FAST_ALLOC_LEN = 128};
	friend void * SerializeMessage(const MSG &);
	friend void FreeMessage(MSG *);
};

inline void * SerializeMessage(const MSG & msg)
{
	void * buf;
	size_t length = msg.content_length;
	if(length <= MSG::FAST_ALLOC_LEN)
	{
	//	printf("%d %dalloced\n",sizeof(MSG) + length,msg.message);
		buf = abase::fast_allocator::align_alloc(sizeof(MSG) + length);		//±ØĞë¶ÔÆë£¬¿¼ÂÇ¶à¸ömsg
		memcpy(buf,&msg,sizeof(MSG));
		if(length)
		{
			memcpy((char*)buf + sizeof(MSG),msg.content,length);
		}
	}
	else
	{
		buf = abase::fast_allocator::raw_alloc(sizeof(MSG) + length);
		memcpy(buf,&msg,sizeof(MSG));
		memcpy((char*)buf + sizeof(MSG),msg.content,msg.content_length);
	}
	return buf;
}

inline MSG * DupeMessage(const MSG & msg)
{
	MSG * pMsg = (MSG*)SerializeMessage(msg);
	pMsg->content = ((char*)pMsg) + sizeof(MSG);
	return pMsg;
}

inline void FreeMessage(MSG * pMsg)
{
	ASSERT(pMsg->content == ((char*)pMsg) + sizeof(MSG));
	size_t length = pMsg->content_length;
	if(length <= MSG::FAST_ALLOC_LEN)
	{
		abase::fast_allocator::align_free(pMsg, sizeof(MSG) + length);
	}
	else
	{
		abase::fast_allocator::raw_free(pMsg);
	}
}
inline void BuildMessage(MSG & msg, int message, const XID &target, const XID & source,
			const A3DVECTOR & pos,int param = 0,
			const void * content = NULL,size_t content_length = 0)
{
	msg.message = message;
	msg.target = target;
	msg.source = source;
	msg.pos = pos;
	msg.param = param;
	msg.param2 = 0;
	msg.content_length = content_length;
	msg.content = content;
}

inline void BuildMessage2(MSG & msg, int message, const XID &target, const XID & source,
			const A3DVECTOR & pos,int param = 0,int param2 = 0,
			const void * content = NULL,size_t content_length = 0)
{
	msg.message = message;
	msg.target = target;
	msg.source = source;
	msg.pos = pos;
	msg.param = param;
	msg.param2= param2;
	msg.content_length = content_length;
	msg.content = content;
}

enum
{
//	normal message
	GM_MSG_NULL,				//¿ÕÏûÏ¢
	GM_MSG_FORWARD_USERBC,			//×ª·¢µÄÓÃ»§¹ã²¥
	GM_MSG_FORWARD,				//×ª·¢µÄÏûÏ¢£¬ÄÚÈİÓ¦¸Ã×÷ÎªÒ»ÌõĞÂµÄÏûÏ¢ÄÚÈİÀ´½âÊÍ
	GM_MSG_FORWARD_BROADCAST,		//×ª·¢µÄÏûÏ¢¹ã²¥ÏûÏ¢,contentÊÇÁíÍâÒ»ÌõÍêÕûµÄÏûÏ¢
	GM_MSG_USER_GET_INFO,			//ÓÃ»§È¡µÃ±ØÒªµÄÊı¾İ

//5
	GM_MSG_IDENTIFICATION,			//·şÎñÆ÷¸æÖª×Ô¼ºµÄÉí·İ,Ô­µÄÀàĞÍ±ØĞëÊÇserver²¢ÇÒidÊÇËûµÄ·ûºÅ
	GM_MSG_SWITCH_GET,			//È¡µÃÓÃ»§Êı¾İ,·şÎñÆ÷ÇĞ»»£¬È¡µÃÓÃ»§Êı¾İ paramÊÇ tag,contentÊÇkey
	GM_MSG_SWITCH_START,			//ÓÃ»§Í¨Öª¹ÜÀíÆ÷×Ô¼º¿ªÊ¼×ªÒÆ
	GM_MSG_SWITCH_NPC,			//NPCÇĞ»»·şÎñÆ÷
	GM_MSG_USER_MOVE_OUTSIDE,		//ÓÃ»§ÔÚ±ß½çÒÆ¶¯

//10	
	GM_MSG_USER_NPC_OUTSIDE,		//NPCÔÚ±ß½ç´¦ÒÆ¶¯£¬²»Í¬Ö®´¦ÔÚÓÚNPC²»ĞèÒªÈ¡µÃĞÂ¿´µ½ÇøÓòµÄ¶ÔÏó
	GM_MSG_ENTER_WORLD,			//¸øcontrollerµÄ£¬±íÊ¾ÓÃ»§ÒÑ¾­½øÈëÁËÊÀ½ç
	GM_MSG_ATTACK,				//Ä¿±êºÍÔ´¶¼±ØĞëÊÇ¸öÌå
	GM_MSG_SKILL,				//Ä¿±êºÍÔ´¶¼±ØĞëÊÇ¸öÌå
	GM_MSG_PICKUP,				//¼ğÆğÎïÆ·,Ä¿±êÒ»°ãÊÇÎïÆ· contentÊÇmsg_pickup_t paramÊÇÊÇ·ñÂÖÁ÷·ÖÅä

//15
	GM_MSG_FORCE_PICKUP,			//Ç¿ÖÆ¼ñÆğÎïÆ·£¬²»Ğ£ÑéËùÊéÕßIDºÍ×é¶ÓID
	GM_MSG_PICKUP_MONEY,			//ÎïÆ·Í¨ÖªÓÃ»§¼ğµ½Ç® paramÊÇÇ®Êı  contentÊÇË­¶ªÆúµÄ
	GM_MSG_PICKUP_TEAM_ITEM,		//ÎïÆ·Í¨Öª×é¶Ó¼ñµ½ÎïÆ·  param ÊÇ¸ÃÎïÆ·µÄworld_tag
	GM_MSG_RECEIVE_MONEY,			//Í¨ÖªÍæ¼ÒµÃµ½×é¶ÓÊ±¹ÖÎïµôÂäµÄ½ğÇ® ĞèÒª¿¼ÂÇ½ğÇ®¼Ó³É
	GM_MSG_PICKUP_ITEM,			//ÎïÆ·Í¨ÖªÓÃ»§¼ğµ½ÎïÆ· paramÊÇ palyer_id | 0x80000000(Èç¹û×é¶Ó£©

//20
	GM_MSG_ERROR_MESSAGE,			//ÈÃplayer·¢ËÍÒ»¸öerror message
	GM_MSG_NPC_SVR_UPDATE,			//NPC·¢ÉúÁË·şÎñÆ÷ÇĞ»»£¬Õâ¸öÏûÏ¢Ö»·¢¸ø´¦ÓÚÒÆ×ß×´Ì¬µÄÔ­ÉúNPC
	GM_MSG_EXT_NPC_DEAD,			//Íâ²¿µÄNPCµÄËÀÍöÏûÏ¢(ÕæÕıÉ¾³ı)£¬Õâ¸öÏûÏ¢Ö»·¢¸ø´¦ÓÚÒÆ×ß×´Ì¬µÄÔ­ÉúNPC
	GM_MSG_EXT_NPC_HEARTBEAT,		//Íâ²¿NPCµÄĞÄÌø£¬ÓÃÓÚÅĞ¶ÏÊÇ·ñ³¬Ê± 
	GM_MSG_WATCHING_YOU,			//Ö÷¶¯¹ÖÎï¼¤»îµÄÏûÏ¢,ÓÉÍæ¼Ò»ònpc·¢³ö£¬ºóÃæÊÇÒ»¸öwatching_tµÄ½á¹¹

//25
//	AGGRO  message 
	GM_MSG_GEN_AGGRO,			//Éú³Éaggro£¬ºóÃæ¸½¼ÓÁËÒ»¸öaggro_info_tµÄ½á¹¹
	GM_MSG_TRANSFER_AGGRO,			//aggroµÄ´«ËÍ Ä¿Ç°Ö»´«ËÍµÚÒ»Î» contentÊÇÒ»¸öXID,Èç¹û¸ÃXIDµÄidÎª-1    ÔòÇå¿Õ³ğºŞÁĞ±í paramÊÇ¸ÃÈË³ğºŞÖµ
	GM_MSG_AGGRO_ALARM,			//aggro¾¯±¨£¬µ±ÊÜµ½¹¥»÷Ê±»á·¢ËÍ£¬ºóÃæ¸½¼ÓÁËÒ»¸öaggro_alarm_tÎ´Ê¹ÓÃ
	GM_MSG_AGGRO_WAKEUP,			//aggro¾¯±¨£¬½«ĞİÃßµÄ¹ÖÎï¾ªĞÑ,ºóÃæ¸½¼ÓÁËÒ»¸öaggro_alarm_tÎ´Ê¹ÓÃ
	GM_MSG_AGGRO_TEST,			//aggro²âÊÔ,Ö»ÓĞµ±·¢ËÍÕßÔÚaggroÁĞ±íÖĞ£¬²Å»áÒı·¢ĞÂµÄaggro£¬ºóÃæ¸½¼ÓÁËÒ»¸öaggro_info_tÎ´Ê¹ÓÃ
	
//30
	GM_MSG_OBJ_SESSION_END,			//¶ÔÏóµÄsessionÍê³É
	GM_MSG_OBJ_SESSION_REPEAT,		//±íÊ¾sessionÒª¼ÌĞøÖ´ĞĞ 
	GM_MSG_OBJ_ZOMBIE_END,			//±íÊ¾Òª½áÊø½©Ê¬×´Ì¬
	GM_MSG_EXPERIENCE,			//µÃµ½¾­ÑéÖµ	content ÊÇÒ»¸ömsg_exp_t
	GM_MSG_GROUP_EXPERIENCE,		//µÃµ½×é¶Ó¾­ÑéÖµ conennt ÊÇ¶à¸ömsg_grp_exp_t , param Ôì³ÉµÄ×ÜÉËº¦
	
//35
	GM_MSG_TEAM_EXPERIENCE,			//µÃµ½×é¶Ó¾­ÑéÖµ conennt ÊÇmsg_exp_t ³¬¹ı¾àÀë¾­ÑéÖµ»á±»ºöÂÔ param ÊÇÉ±ËÀµÄnpcid Èç¹Îª0Ôò²»ÊÇ±¾¶ÓÎéÉ±ËÀµÄ
	GM_MSG_QUERY_OBJ_INFO00,		//È¡µÃ¶ÔÏóµÄinfo00 paramÊÇ·¢ËÍÕßµÄsid ,contentÊÇÒ»¸öint´ú±ícs_index
	GM_MSG_HEARTBEAT,			//·¢¸ø×Ô¼ºµÄĞÄÌøÏûÏ¢  ²ÎÊıÊÇÕâ´ÎHeartbeatµÄÃëÊı
	GM_MSG_HATE_YOU,
	GM_MSG_TEAM_INVITE,			//ÇëÇóÄ³ÈË¼ÓÈë¶ÓÎéparamÊÇteamseq, contentÊÇÒ»¸öint ±íÊ¾pickup_flag param2ÊÇfaction

//40	
	GM_MSG_TEAM_AGREE_INVITE,		//±»ÑûÇëÈËÍ¬Òâ¼ÓÈë¶ÓÎé contentÊÇÒ»¸öint(±íÊ¾Ö°Òµ)+ team_mutable_prop
	GM_MSG_TEAM_REJECT_INVITE,		//¾Ü¾ø¼ÓÈëÑûÇë
	GM_MSG_JOIN_TEAM,			//¶Ó³¤Í¬ÒâÄ³ÈË¼ÓÈë¶ÓÎé param¸ßÎ»ÊÇ¼ñÈ¡·½Ê½ paramµÍÎ»ÊÇ¶ÓÔ±¸öÊı£¬contentÊÇmember_entryµÄ±í 
	GM_MSG_JOIN_TEAM_FAILED,		//¶ÔÏóÎŞ·¨¼ÓÈë¶ÓÎé£¬Ó¦¸Ã´Ó¶ÓÎéÖĞÈ¥³ı
	GM_MSG_MEMBER_NOTIFY_DATA,		//×é¶Ó³ÉÔ±Í¨ÖªÆäËûÈË×Ô¼ºµÄ»ù´¡ĞÅÏ¢ content ÊÇÒ»¸öteam_mutable_prop

//45	
	GM_MSG_NEW_MEMBER,			//leaderÍ¨ÖªĞÂ³ÉÔ±¼ÓÈë£¬contentÊÇÒ»¸ömember_entry list paramÊÇÊıÁ¿
	GM_MSG_LEAVE_PARTY_REQUEST,
	GM_MSG_LEADER_CANCEL_PARTY,
	GM_MSG_MEMBER_NOT_IN_TEAM,
	GM_MSG_LEADER_KICK_MEMBER,

//50	
	GM_MSG_MEMBER_LEAVE,
	GM_MSG_LEADER_UPDATE_MEMBER,
	GM_MSG_GET_MEMBER_POS,			//ÒªÇó¶ÓÓÑ·¢ËÍÎ»ÖÃ paramÊÇ·¢ËÍÕßµÄsid ,contentÊÇÒ»¸öint´ú±ícs_index
	GM_MSG_QUERY_PLAYER_EQUIPMENT,		//È¡µÃÌØ¶¨Íæ¼ÒµÄÊı¾İ£¬ÒªÇóÆ½Ãæ¾àÀëÔÚÒ»¶¨·¶Î§Ö®ÄÚparamÊÇ·¢ËÍÕßµÄsid ,contentÊÇÒ»¸öint´ú±ícs_index
	GM_MSG_TEAM_PICKUP_NOTIFY,		//¶ÓÓÑ·ÖÅäµ½ÎïÆ·Í¨Öª£¬ param ÊÇ type, content ÊÇcount

//55	
	GM_MSG_TEAM_CHAT___,			//×é¶ÓÁÄÌì param ÊÇchannel, content ÊÇÄÚÈİ ÒÑ¾­×÷·Ï£¬ÏÖÔÚÖ±½Ó·¢ËÍ
	GM_MSG_SERVICE_REQUEST,			//playerÒªÇó·şÎñµÄÏûÏ¢ param ÊÇ·şÎñÀàĞÍ content ÊÇ¾ßÌåÊı¾İ ¾
	GM_MSG_SERVICE_DATA,			//·şÎñµÄÊı¾İµ½´ï param ÊÇ·şÎñÀàĞÍ  content ÊÇ ¾ßÌåÊı¾İ
	GM_MSG_SERVICE_HELLO,			//player Ïò·şÎñÉÌÎÊºÃ  param ÊÇ player×Ô¼ºµÄfaction
	GM_MSG_SERVICE_GREETING,		//·şÎñÉÌ½øĞĞ»Ø»° ¿ÉÄÜĞèÒªÔÚÀïÃæ·µ»Ø·şÎñÁĞ±í$$$$(ÏÖÔÚÎ´×ö)

//60	
	GM_MSG_SERVICE_QUIERY_CONTENT,		//È¡µÃ·şÎñÄÚÈİ 	 param ÊÇ·şÎñÀàĞÍ, content¿É¿´×÷pair<cs_index,sid>
	GM_MSG_EXTERN_OBJECT_APPEAR_N,		//content ÊÇextern_object_manager::object_appear [×÷·Ï]
	GM_MSG_EXTERN_OBJECT_DISAPPEAR_N,	//ÏûÊ§»òÕß    [×÷·Ï]
	GM_MSG_EXTERN_OBJECT_REFRESH_N,		//¸üĞÂÎ»ÖÃºÍÑªÖµ£¬paramÖĞ±£´æµÄÊÇÑªÖµ  [×÷·Ï]
	GM_MSG_USER_APPEAR_OUTSIDE,		//ÓÃ»§ÔÚÍâÃæ³öÏÖ£¬Òª·¢ËÍ±ØÒªµÄÊı¾İ¸ø¸ÃÍæ¼Ò£¬content ÀïÊÇsid,paramÊÇlinkd id

//65
	GM_MSG_FORWARD_BROADCAST_SPHERE_N,	//×ª·¢µÄÏûÏ¢¹ã²¥ÏûÏ¢,contentÊÇÁíÍâÒ»ÌõÍêÕûµÄÏûÏ¢ [×÷·Ï]
	GM_MSG_FORWARD_BROADCAST_CYLINDER_N,	//×ª·¢µÄÏûÏ¢¹ã²¥ÏûÏ¢,contentÊÇÁíÍâÒ»ÌõÍêÕûµÄÏûÏ¢ [×÷·Ï]
	GM_MSG_PRODUCE_MONSTER_DROP,		//Í¨ÖªÏµÍ³²úÉú¹ÖÎïµôÂäÎïÆ·ºÍ½ğÇ®£¬ ·¢ËÍÔ´ÊÇËùÊôÕß£¬paramÊÇmoney£¬ content ÊÇ struct { int team_id; int team_seq;int npc_id;int item_count; int item[];}
	GM_MSG_ENCHANT,				//Ê¹ÓÃ¸¨ÖúÄ§·¨
	GM_MSG_ENCHANT_ZOMBIE,			//Ê¹ÓÃ¸¨ÖúÄ§·¨,×¨ÃÅ¸øËÀÈËÓÃµÄ

//70
	GM_MSG_OBJ_SESSION_REP_FORCE,		//±íÊ¾sessionÒªrepeat £¬ºóÃæ¼´Ê¹ÓĞÈÎÎñÒ²Òª¼ÌĞøÖ´ĞĞ
	GM_MSG_NPC_BE_KILLED,			//ÏûÏ¢·¢¸øÉ±ËÀnpcµÄÍæ¼Ò£¬param ±íÊ¾±»É±ËÀnpcµÄÀàĞÍ contentÊÇNPCµÄ¼¶±ğ
	GM_MSG_NPC_CRY_FOR_HELP,		//npc ½øĞĞÇó¾È²Ù×÷
	GM_MSG_PLAYER_TASK_TRANSFER,		//ÈÎÎñÔÚplayerÖ®¼ä½øĞĞ´«ËÍºÍÍ¨Ñ¶µÄº¯Êı
	GM_MSG_PLAYER_BECOME_INVADER,		//³ÉÎª·ÛÃû msg.param ÊÇÔö¼ÓµÄÊ±¼ä

//75
	GM_MSG_PLAYER_KILL_PLAYER,		//É±ËÀÍæ¼ÒµÄ±êÖ¾ msg.param´ú±í¶Ô·½µÄPKÖµ
	GM_MSG_FORWARD_CHAT_MSG,		//×ª·¢µÄÓÃ»§ÁÄÌìĞÅÏ¢,paramÊÇrlevel,sourceÊÇXID(-channel,self_id)
	GM_MSG_QUERY_SELECT_TARGET,		//È¡µÃ¶ÓÓÑÑ¡ÔñµÄ¶ÔÏó
	GM_MSG_NOTIFY_SELECT_TARGET,		//È¡µÃ¶ÓÓÑÑ¡ÔñµÄ¶ÔÏó
	GM_MSG_SUBSCIBE_TARGET,			//ÒªÇó¶©ÔÄÒ»¸ö¶ÔÏó

//80
	GM_MSG_UNSUBSCIBE_TARGET,		//ÒªÇó¶©ÔÄÒ»¸ö¶ÔÏó
	GM_MSG_SUBSCIBE_CONFIRM,		//È·ÈÏ¶©ÔÄÊÇ·ñ´æÔÚ
	GM_MSG_MONSTER_MONEY,			//Í¨ÖªÍæ¼ÒÊÕµ½¹ÖÎïµôÂäµÄ½ğÇ®	paramÊÇ½ğÇ® ĞèÒª¿¼ÂÇ½ğÇ®¼Ó³É
	GM_MSG_MONSTER_GROUP_MONEY,		//Í¨ÖªÍæ¼ÒÊÕµ½¶ÓÎé½ğÇ®  paramÊÇ½ğÇ® 
	GM_MSG_GATHER_REQUEST,			//ÇëÇóÊÕ¼¯Ô­ÁÏ£¬  param ÊÇÍæ¼ÒµÄfaction, content ·Ö±ğÊÇÍæ¼Ò¼¶±ğ¡¢²É¼¯¹¤¾ßºÍÈÎÎñID

//85
	GM_MSG_GATHER_REPLY,			//Í¨Öª¿ÉÒÔ½øĞĞ²É¼¯  param ÊÇ²É¼¯ĞèÒªµÄÊ±¼ä
	GM_MSG_GATHER_CANCEL,			//È¡Ïû²É¼¯
	GM_MSG_GATHER,				//½øĞĞ²É¼¯£¬ÒªÇóÈ¡µÃÎïÆ·
	GM_MSG_GATHER_RESULT,			//²É¼¯Íê³É£¬param ÄÚÊÇ²É¼¯µ½µÄÎïÆ·id, contentÊÇÊıÁ¿ ºÍ¿ÉÄÜ¸½¼ÓµÄÈÎÎñID
	GM_MSG_EXTERN_HEAL,			//¸øÄ³Ä³¶ÔÏó¼ÓÑªµÄÏûÏ¢

//90
	GM_MSG_INSTANCE_SWITCH_GET,		//È¡µÃÓÃ»§Êı¾İ,·şÎñÆ÷ÇĞ»»£¬È¡µÃÓÃ»§Êı¾İ ÓÃÓÚ¸±±¾¼äµÄÇĞ»» paramÊÇkey
	GM_MSG_INSTANCE_SWITCH_USER_DATA,	//ÓÃ»§Êı¾İ,SWITCH_SWITCH_GETµÄ»ØÓ¦
	GM_MSG_EXT_AGGRO_FORWARD,		//Í¨ÖªÔ­Éúnpc½øĞĞ³ğºŞ×ª·¢ param ÊÇrage´óĞ¡£¬ contentÊÇ²úÉú³ğºŞµÄid
	GM_MSG_TEAM_APPLY_PARTY,		//ÉêÇë½øÈë¶ÓÎéÑ¡Ïî      param2ÊÇfaction
	GM_MSG_TEAM_APPLY_REPLY,		//ÉêÇë³É¹¦»Ø¸´ ÆäÖĞµÄparamÊÇseq	

//95
	GM_MSG_QUERY_INFO_1,			//²éÑ¯INFO1£¬¿ÉÒÔ·¢¸øÍæ¼Ò»òÕßNPC,paramµÄÄÚÈİÊÇcs_index,contentÊÇsid
	GM_MSG_CON_EMOTE_REQUEST_NULL,		//½øĞĞĞ­Í¬¶¯×÷µÄÇëÇó param ÊÇ action      ÒÑ¾­×÷·Ï
	GM_MSG_CON_EMOTE_REPLY_NULL,		//½øĞĞĞ­Í¬¶¯×÷µÄ»ØÓ¦ param ÊÇactionºÍÍ¬ÒâÓë·ñµÄÁ½¸ö×Ö½ÚµÄ ÒÑ¾­×÷·Ï
	GM_MSG_TEAM_CHANGE_TO_LEADER,		//Í¨Öª±ğÈËÒª³ÉÎªleader
	GM_MSG_TEAM_LEADER_CHANGED,		//Í¨Öª¶ÓÓÑ¶Ó³¤µÄ¸Ä±ä

//100
	GM_MSG_OBJ_ZOMBIE_SESSION_END,		//ËÀÍöºó½øĞĞsessionµÄ²Ù×÷£¬ÆäËû¶¨ÒåºÍÕı³£µÄsession²Ù×÷Ò»Ñù
	GM_MSG_QUERY_MARKET_NAME,		//È¡µÃ°ÚÌ¯µÄÃû×Ö£¬paramÊÇ·¢ËÍÕßµÄsid ,contentÊÇÒ»¸öint´ú±ícs_index
	GM_MSG_HURT,				//¶ÔÏó²úÉúÉËº¦ contentÊÇattacker_info_1, param1ÊÇdamage, param2×éºÏÁË¹¥»÷Ä£Ê½ºÍÊÇ·ñ·Ç·¨¹¥»÷
	GM_MSG_DEATH,				//Ç¿ĞĞÈÃ¶ÔÏóËÀÍö
	GM_MSG_PLANE_SWITCH_REQUEST,		//ÇëÇó¿ªÊ¼´«ËÍ£¬contentÊÇkey£¬Èç¹û½øĞĞ´«ËÍ£¬Ôò·µ»Ø SWITCH_REPLAY

//105
	GM_MSG_PLANE_SWITCH_REPLY,		//´«ËÍÇëÇó±»È·ÈÏ£¬contentÊÇkey
	GM_MSG_SCROLL_RESURRECT,		//¾íÖá¸´»î  param±íÊ¾¸´»îÕßÊÇ·ñ¿ªÆôÁËpvpÄ£Ê½1±íÊ¾¿ªÆôÁË
	GM_MSG_LEAVE_COSMETIC_MODE,		//ÍÑÀëÕûÈİ×´Ì¬
	GM_MSG_DBSAVE_ERROR,			//Êı¾İ¿â±£´æ´íÎó
	GM_MSG_SPAWN_DISAPPEAR,			//Í¨ÖªNPCºÍÎïÆ·ÏûÊ§ paramÊÇcondition

//110
	GM_MSG_PET_CTRL_CMD,			//Íæ¼Ò·¢À´µÄ¿ØÖÆÏûÏ¢»áÓÃÕâ¸öÏûÏ¢·¢¸ø³èÎï
	GM_MSG_ENABLE_PVP_DURATION,		//¼¤»îPVP×´Ì¬
	GM_MSG_PLAYER_KILLED_BY_NPC,		//Íæ¼Ò±»NPCÉ±ËÀºóNPCµÄÂß¼­
	GM_MSG_PLAYER_DUEL_REQUEST,             //Íæ¼Ò·¢³öÒªÇóduelµÄÇëÇó
	GM_MSG_PLAYER_DUEL_REPLY,               //Íæ¼Ò»ØÓ¦duelµÄÇëÇó£¬paramÊÇÊÇ·ñ´ğÓ¦duel

//115
	GM_MSG_PLAYER_DUEL_PREPARE,      	//¾ö¶·×¼±¸¿ªÊ¼ 3Ãëµ¹¼ÆÊ±ºó¿ªÊ¼
	GM_MSG_PLAYER_DUEL_START,               //¾ö¶·¿ªÊ¼ 
	GM_MSG_PLAYER_DUEL_CANCEL,		//Í£Ö¹¾ö¶·
	GM_MSG_PLAYER_DUEL_STOP,		//¾ö¶·½áÊø
	GM_MSG_DUEL_HURT,			//PVP¶ÔÏó²úÉúÉËº¦content ±»ºöÂÔ

//120
	GM_MSG_PLAYER_BIND_REQUEST,		//ÇëÇóÆïÔÚ±ğÈËÉíÉÏ
	GM_MSG_PLAYER_BIND_INVITE,		//ÑûÇë±ğÈËÆïÔÚ×Ô¼ºÉíÉÏ
	GM_MSG_PLAYER_BIND_REQ_REPLY,		//ÇëÇóÆïµÄ»ØÓ¦
	GM_MSG_PLAYER_BIND_INV_REPLY,		//ÑûÇëÆïµÄ»ØÓ¦
	GM_MSG_PLAYER_BIND_PREPARE,		//×¼±¸¿ªÊ¼Á¬½Ó

//125
	GM_MSG_PLAYER_BIND_LINK,		//Á¬½Ó¿ªÊ¼
	GM_MSG_PLAYER_BIND_STOP,		//Í£Ö¹Á¬½Ó
	GM_MSG_PLAYER_BIND_FOLLOW,		//ÒªÇóÍæ¼Ò¸úËæ
	GM_MSG_QUERY_EQUIP_DETAIL,		//param Îªfaction, content Îªcs_index ºÍcs_sid
	GM_MSG_PLAYER_RECALL_PET,		//ÈÃÍæ¼ÒÇ¿ÖÆÏû³ıÕÙ»½×´Ì¬

//130
	GM_MSG_CREATE_BATTLEGROUND,		//ÒªÇóÕ½³¡·şÎñÆ÷´´½¨Ò»¸öÕ½³¡µÄÏûÏ¢£¬Ö÷ÒªÓÃÓÚ²âÊÔ
	GM_MSG_BECOME_TURRET_MASTER,		//³ÉÎª¹¥³Ç³µµÄmaster,paramÊÇtid, content ÊÇfaction
	GM_MSG_REMOVE_ITEM,			//É¾³ıÒ»¸öÎïÆ·µÄÏûÏ¢£¬ÓÃÓÚ¹¥³Ç³µ¿ØÖÆºóµÄÎïÆ·¼õÉÙ paramÊÇtid
	GM_MSG_NPC_TRANSFORM,			//NPC±äĞÎĞ§¹û£¬contentÀï±£´æ ÖĞ¼ä×´Ì¬£¬ÖĞ¼äÊ±¼ä ÖĞ¼ä±êÖ¾ ×îºó×´Ì¬
	GM_MSG_NPC_TRANSFORM2,			//NPC±äĞÎĞ§¹û2£¬param ÊÇÄ¿±êID Èç¹û±¾À´¾ÍºÍÄ¿±êIDÒ»ÖÂÁË£¬ÄÇÃ´¾Í²»±äĞÎÁË

//135
	GM_MSG_TURRET_NOTIFY_LEADER,		//¹¥³Ç³µÍ¨Öªleader×Ô¼º´æÔÚ£¬ÈÃÆäÎŞ·¨ÔÙ´Î½øĞĞÕÙ»½
	GM_MSG_PET_RELOCATE_POS,		//³èÎïÒªÇóÖØĞÂ¶¨Î»×ø±ê
	GM_MSG_PET_CHANGE_POS,			//Ö÷ÈËĞŞ¸ÄÁË³èÎïµÄ×ø±ê
	GM_MSG_PET_DISAPPEAR,			//Êı¾İ²»ÕıÈ·,»òÕßÆäËüÇé¿ö,Ö÷ÈËÒªÇó³èÎïÏûÊ§
	GM_MSG_PET_NOTIFY_HP_VP,		//³èÎïÍ¨ÖªÖ÷ÈË,¸æÖª×Ô¼ºµÄÑªÁ¿ ¾«Á¦

//140
	GM_MSG_PET_NOTIFY_DEATH,		//³èÎïÍ¨ÖªÖ÷ÈË×Ô¼ºµÄËÀÍö
	GM_MSG_MASTER_INFO,			//Ö÷ÈËÍ¨Öª³èÎï×Ô¼ºµÄÊı¾İ
	GM_MSG_PET_LEVEL_UP,			//Ö÷ÈËÍ¨Öª³èÎïÉı¼¶ÁË ,contentÊÇ level
	GM_MSG_PET_HONOR_MODIFY,		//Ö÷ÈËÍ¨Öª³èÎïµÄÖÒ³Ï¶È·¢Éú±ä»¯
	GM_MSG_MASTER_ASK_HELP,			//Ö÷ÈËÒªÇó³èÎï°ïÖú
	
//145
	GM_MSG_REPU_CHG_STEP_1,			//ÍşÍû×ª»»µÚ1²½ ±»É±Õß´«µİ×Ô¼ºµÄµ±Ç°ÍşÍû
	GM_MSG_REPU_CHG_STEP_2,			//ÍşÍû×ª»»µÚ2²½ É±ÈËÕß´«»Ø½á¹û  ±»É±ÕßĞŞ¸Ä×Ô¼ºµÄÍşÍû
	GM_MSG_REPU_CHG_STEP_3,			//ÍşÍû×ª»»µÚ3²½ ±»É±Õß¼õÍêÍşÍûºó Í¨ÖªÉ±ÈËÕßĞŞ¸ÄÍşÍû
	GM_MSG_TEAM_MEMBER_LVLUP,		//¶Ó³¤ÊÕµ½£¬µÍµÈ¼¶¶ÓÔ±Éı¼¶Ê±Îª¸ø¶Ó³¤Ôö¼ÓÍşÍûµÄĞ­Òé paramÎªÉı¼¶Ç°¼¶±ğ
	GM_MSG_KILL_PLAYER_IN_BATTLEGROUND,	// µ±Íæ¼ÒÔÚÕ½³¡É±ËÀµĞ·½Íæ¼ÒÊ±£¬»áÊÕµ½ËÀÕß·¢À´Õâ¸öÏûÏ¢

//150
	GM_MSG_MODIFIY_BATTLE_DEATH,		// Íæ¼ÒĞèÒªĞŞ¸Ä×Ô¼ºµÄËÀÍöÊıĞÅÏ¢£¬·¢ËÍ¸øworld
	GM_MSG_MODIFIY_BATTLE_KILL,		// Íæ¼ÒĞèÒªĞŞ¸Ä×Ô¼ºµÄÉ±ÈËÊıĞÅÏ¢£¬·¢ËÍ¸øworld
	GM_MSG_PET_SET_COOLDOWN,		//³èÎï¼¼ÄÜÀäÈ´
	GM_MSG_PET_SET_AUTO_SKILL,		//³èÎï¼¼ÄÜ×Ô¶¯Ê©·Å
	GM_MSG_FEED_PET,			//Ö÷ÈË¸ø³èÎï³ÔÒ©

//155
	GM_MSG_PET_CAST_SKILL,			//³èÎïÊ¹ÓÃ¼¼ÄÜ
	GM_MSG_PET_HONOR_LEVEL_CHANGED,		//³èÎïµÄÇ×ÃÜ¶È¼¶±ğ±äÁË
	GM_MSG_PET_HUNGER_LEVEL_CHANGED,	//³èÎïµÄ±¥Ê³¶È¼¶±ğ±äÁË
	GM_MSG_MASTER_START_ATTACK,			//Ö÷ÈË¹¥»÷
	GM_MSG_PET_INFO_CHANGED,		//³èÎïĞÅÏ¢±ä»¯

//160
	GM_MSG_NPC_DISAPPEAR,			//NPCÏûÊ§
	GM_MSG_TASK_AWARD_TRANSFOR,
	GM_MSG_PLAYER_KILL_PET,			//É±ËÀÍæ¼Ò³èÎï
	GM_MSG_PLAYER_LINK_RIDE_INVITE,
	GM_MSG_PLAYER_LINK_RIDE_INV_REPLY,

//165
	GM_MSG_PLAYER_LINK_RIDE_START,
	GM_MSG_PLAYER_LINK_RIDE_STOP,
	GM_MSG_PLAYER_LINK_RIDE_MEMBER_JOIN,
	GM_MSG_PLAYER_LINK_RIDE_FOLLOW,
	GM_MSG_PLAYER_LINK_RIDE_LEAVE_REQUEST,

//170
	GM_MSG_PLAYER_LINK_RIDE_MEMBER_LEAVE,
	GM_MSG_PLAYER_LINK_RIDE_LEADER_LEAVE,
	GM_MSG_PLAYER_LINK_RIDE_KICK_MEMBER,
	GM_MSG_KILL_MONSTER_IN_BATTLEGROUND,	//Õ½³¡ÀïÃæ¹ÖÎï±»Íæ¼ÒÉ±ËÀ
	GM_MSG_PLAYER_CATCH_PET,		//Íæ¼Ò×¥³èÎï

//175
	GM_MSG_NPC_BE_CATCHED,			//¹ÖÎï±»×¥ÁË
	GM_MSG_NPC_BE_CATCHED_CONFIRM,
	GM_MSG_SYNC_BATTLE_INFO,
	GM_MSG_QUERY_ACHIEVEMENT,		//²éÑ¯ÆäËûÍæ¼Ò³É¾ÍÁĞ±í
	GM_MSG_BATTLE_INFO_CHANGE,		//Õ½³¡ĞÅÏ¢±ä»¯

//180
	GM_MSG_PLAYER_KILL_SUMMON,		//É±ËÀÍæ¼ÒÕÙ»½¹Ö
	GM_MSG_SUMMON_NOTIFY_DEATH,		//³èÎïÍ¨ÖªÖ÷ÈË×Ô¼ºµÄËÀÍö
	GM_MSG_SUMMON_RELOCATE_POS,
	GM_MSG_SUMMON_CHANGE_POS,
	GM_MSG_EXTERN_ADD_MANA,			//Í¨ÖªÄ¿±ê¼ÓÀ¶

//185
	GM_MSG_PLAYER_ENTER_CARRIER,		//Íæ¼ÒÇëÇóÉÏ´¬
	GM_MSG_PLAYER_LEAVE_CARRIER,		//Íæ¼ÒÇëÇóÏÂ´¬
	GM_MSG_ENTER_CARRIER,			//Í¨ÖªÍæ¼ÒÉÏ´¬
	GM_MSG_LEAVE_CARRIER,			//Í¨ÖªÍæ¼ÒÏÂ´¬
	GM_MSG_CARRIER_SYNC_POS,		//´¬ÒÆ¶¯Ê±Í¬²½×Ô¼ºµÄÎ»ÖÃºÍ·½Ïò¸ø´¬ÉÏµÄ¶ÔÏó

//190	
	GM_MSG_BATTLE_NPC_DISAPPEAR,		//Õ½³¡NPCÏûÊ§,ÓÃÓÚÕ½³¡ÇåÀí
	GM_MSG_SUMMON_HEARTBEAT,		//ÕÙ»½ÊŞ¸øÍæ¼Ò·¢ËÍĞÄÌøĞÅÏ¢
	GM_MSG_NPC_BORN,			//¹ÖÎï´´½¨£¬ÓÃÓÚ²ßÂÔ 
	GM_MSG_MONSTER_TRANSFORM2,		//¶Ô¹ÖÎïÊ¹ÓÃÕÕÑı¾µ,Ô­ĞÍÎª¹Ö
	GM_MSG_MINE_TRANSFORM2,			//¶Ô¹ÖÎïÊ¹ÓÃÕÕÑı¾µ,Ô­ĞÍÎª¿óÎï

//195
	GM_MSG_USE_COMBO_SKILL,			//Á¬Ğø¼¼
	GM_MSG_TRY_CLEAR_AGGRO,			//Çå³ş¹ÖÎï³ğºŞ
	GM_MSG_DEC_HP,				//¿Û³ı¶Ô·½µÄHP, ²»»á¼Ó³ğºŞ
	GM_MSG_EXCHANGE_STATUS,			//½»»»hp,mp
	GM_MSG_EXCHANGE_POS,			//½»»»Î»ÖÃ

//200
	GM_MSG_HEAL_CLONE,			//¸ø·ÖÉí¼Óhp
	GM_MSG_MANA_CLONE,			//¸ø·ÖÉí¼Ómp
	GM_MSG_EXCHANGE_SUBSCIBE,		//½»»»¶©ÔÄÁĞ±í¸ø
	GM_MSG_MINE_DISAPPEAR,			//Çå³ıËùÓĞ¿óÎï
	GM_MSG_QUERY_CLONE_EQUIPMENT,		//²éÑ¯·ÖÉíÖ÷ÈËµÄ×°±¸ĞÅÏ¢

//205
	GM_MSG_PROTECTED_NPC_NOTIFY,		//Í¨Öª±£»¤µÄNPC×´Ì¬
	GM_MSG_SYNC_BATTLE_INFO_TO_PLAYER,	//Í¬²½Õ½³¡ĞÅÏ¢¸øÍæ¼Ò
	GM_MSG_MASTER_DUEL_STOP,		//Í¨Öª³èÎïºÍÕÙ»½ÊŞÖ÷ÈË¾ö¶·½áÊø
	GM_MSG_TRANSFER_ATTACK,			//×ªÒÆ¹¥»÷ÏûÏ¢
	GM_MSG_ROUND_START_IN_BATTLE,   //Õ½³¡ÖĞÒ»Ğ¡¾ÖµÄ¿ªÊ¼

//210
	GM_MSG_ROUND_END_IN_BATTLE,   //Õ½³¡ÖĞÒ»Ğ¡¾ÖµÄ½áÊø
	GM_MSG_ATTACK_FEED_BACK,	  //Ïò¹¥»÷Õß·¢ËÍ¹¥»÷½á¹û·´À¡, paramÎªmask£¬param2Îª±»¹¥»÷ÕßµÈ¼¶(FEEDBACK_KILL),  »òÉËº¦ÊıÁ¿(FEEDBACK_DAMAGE)
	GM_MSG_CIRCLE_OF_DOOM_PREPARE,  //Õó·¨¼¼ÄÜÊ¹ÓÃ£¬¿ªÊ¼ÊÕ¼¯ÈËÊı
	GM_MSG_CIRCLE_OF_DOOM_STARTUP,  //Õó·¨½áÕó³É¹¦£¬·¢ÏûÏ¢¸øÕóÄÚµÄ³ÉÔ±
	GM_MSG_CIRCLE_OF_DOOM_STOP,     //Õó·¨ÖÕÖ¹¡¢½áÊø

//215
	GM_MSG_CIRCLE_OF_DOOM_ENTER,    //³ÉÔ±Í¬Òâ½øÈëÕó·¨
	GM_MSG_CIRCLE_OF_DOOM_LEAVE,    //³ÉÔ±Àë¿ªÕó·¨
	GM_MSG_CIRCLE_OF_DOOM_QUERY,    //²éÑ¯Õó·¨ĞÅÏ¢	
	GM_MSG_REMOVE_PERMIT_CYCLE_AREA,//Çå³ı·¢ËÍÏûÏ¢ÕßËùÊ©¼ÓµÄÒÆ¶¯ÏŞÖÆÇøÓò
	GM_MSG_CANCEL_BE_PULLED,		//½â³ı±»À­³¶×´Ì¬

//220
	GM_MSG_CANCEL_BE_CYCLE,         //½â³ıÕó·¨Ğ§¹û
	GM_MSG_QUERY_BE_SPIRIT_DRAGGED,	//²éÑ¯±»Ç£ÒıĞÅÏ¢
	GM_MSG_QUERY_BE_PULLED,			//²éÑ¯±»À­³¶ĞÅÏ¢
	GM_MSG_TASK_SHARE_NPC_BE_KILLED,//ÏûÏ¢·¢¸øÉ±ËÀnpcµÄÍæ¼Ò£¬param ±íÊ¾±»É±ËÀnpcµÄÀàĞÍ contentÊÇNPCµÄ¼¶±ğ
	GM_MSG_TASK_CHECK_STATE,	//°ÚÌ¯»òÕß½»Ò×½áÊøºó¼ì²éÊÇ·ñÓĞ±»¿¨×¡µÄÈÎÎñ

//225
	GM_MSG_SUMMON_CAST_SKILL,		//ÕÙ»½ÊŞÊ¹ÓÃ¼¼ÄÜ
	GM_MSG_SPIRIT_SESSION_END,		//ĞùÔ¯ÁéÊ¹ÓÃµÄsession½áÊø
	GM_MSG_SPIRIT_SESSION_REPEAT,	//ĞùÔ¯ÁéÊ¹ÓÃµÄsessionÖØ¸´
	GM_MSG_SPIRIT_SESSION_REP_FORCE,//ĞùÔ¯ÁéÊ¹ÓÃµÄ±íÊ¾sessionÒªrepeat £¬ºóÃæ¼´Ê¹ÓĞÈÎÎñÒ²Òª¼ÌĞøÖ´ĞĞ
	GM_MSG_TALISMAN_SKILL,			//¼¼ÄÜÃüÖĞºóÍ¨ÖªÊ©·¨ÕßÊÍ·Å·¨±¦¼¼ÄÜ

//230	
	GM_MSG_KINGDOM_BATTLE_HALF,	//¹úÍõÕ½°ë³¡½áÊø
	GM_MSG_KINGDOM_UPDATE_KEY_NPC,	//¹úÍõÕ½¸¨ÖúÕ½³¡É±ËÀNPC
	GM_MSG_REQUEST_BUFFAREA_BUFF,	//ÉêÇëBUFFÇøÓòBUFF
	GM_MSG_KINGDOM_BATTLE_END,
	GM_MSG_LEVEL_RAID_INFO_CHANGE,	//levelÀà¸±±¾ÖĞ¸±±¾ĞÅÏ¢·¢Éú¸Ä±ä

//235
	GM_MSG_LEVEL_RAID_START,
	GM_MSG_LEVEL_RAID_END,
	GM_MSG_KINGDOM_SYNC_KEY_NPC,
	GM_MSG_KINGDOM_CHANGE_KEY_NPC,
	GM_MSG_KINGDOM_QUERY_KEY_NPC,

//240
	GM_MSG_KINGDOM_KEY_NPC_INFO,
	GM_MSG_KINGDOM_CHANGE_NPC_FACTION,
	GM_MSG_KINGDOM_CALL_GUARD,
	GM_MSG_KILL_PLAYER_IN_CSFLOW,
	GM_MSG_SYNC_CSFLOW_PLAYER_INFO,

//245
	GM_MSG_PLAYER_LINK_BATH_INVITE,
	GM_MSG_PLAYER_LINK_BATH_INV_REPLY,
	GM_MSG_PLAYER_LINK_BATH_START,
	GM_MSG_PLAYER_LINK_BATH_STOP,
	GM_MSG_PLAYER_LINK_BATH_LEAVE_REQUEST,


//250
	GM_MSG_PLAYER_LINK_BATH_MEMBER_LEAVE,
	GM_MSG_PLAYER_LINK_BATH_LEADER_LEAVE,
	GM_MSG_FORBID_NPC,
	GM_MSG_FLOW_TEAM_SCORE,
	GM_MSG_NPC_TELEPORT_INFO,

//255
	GM_MSG_SUMMON_TELEPORT_REQUEST,
	GM_MSG_SUMMON_TELEPORT_REPLY,
	GM_MSG_SUMMON_TELEPORT_SYNC,
	GM_MSG_SUMMON_TRY_TELEPORT,
	GM_MSG_SUMMON_AGREE_TELEPORT,

//260
	GM_MSG_MOB_ACTIVE_START,
	GM_MSG_MOB_ACTIVE_STATE_START,
	GM_MSG_MOB_ACTIVE_STATE_FINISH,
	GM_MSG_MOB_ACTIVE_STATE_CANCEL,
	GM_MSG_MOB_ACTIVE_TELL_POS,

//265
	GM_MSG_MOB_ACTIVE_COUNTER_SUC,
	GM_MSG_MOB_ACTIVE_SYNC_POS,
	GM_MSG_PLAYER_LINK_QILIN_INVITE,
	GM_MSG_PLAYER_LINK_QILIN_INV_REPLY,
	GM_MSG_PLAYER_LINK_QILIN_START,

//270
	GM_MSG_PLAYER_LINK_QILIN_STOP,
	GM_MSG_PLAYER_LINK_QILIN_FOLLOW,
	GM_MSG_PLAYER_LINK_QILIN_LEAVE_REQUEST,
	GM_MSG_PLAYER_LINK_QILIN_MEMBER_LEAVE,
	GM_MSG_PLAYER_LINK_QILIN_LEADER_LEAVE,

//275
	GM_MSG_FAC_BUILDING_COMPLETE,
	GM_MSG_PLAYER_ACTIVE_EMOTE_INVITE,
	GM_MSG_PLAYER_ACTIVE_EMOTE_INV_REPLY,
	GM_MSG_PLAYER_ACTIVE_EMOTE_STOP,
	GM_MSG_PLAYER_ACTIVE_EMOTE_LINK,

//280
	GM_MSG_GET_RAID_TRANSFORM_TASK, //¸±±¾±äÉí
	GM_MSG_PET_SAVE_COOLDOWN,
	GM_MSG_MOB_ACTIVE_START_MOVE,
	GM_MSG_MOB_ACTIVE_STOP_MOVE,
	GM_MSG_GET_STEP_RAID_TASK,

//285	
	GM_MSG_MOB_ACTIVE_RECLAIM,
	GM_MSG_MOB_ACTIVE_PATH_END,
	GM_MSG_MASTER_GET_PET_PROP,
	GM_MSG_CS6V6_CHEAT_INFO,
	GM_MSG_CONTROL_TRAP,

//290	
	GM_MSG_SEEK_PREPARE,
	GM_MSG_SEEK_START,
	GM_MSG_SEEK_STOP,
	GM_MSG_SEEK_TRANSFORM,
	GM_MSG_SEEK_RAID_KILL,

//295	
	GM_MSG_SEEK_RAID_SKILL_LEFT,
	GM_MSG_GET_CAPTURE_MONSTER_AWARD,
	GM_MSG_CLEAN_FLAG_TRANSFORM,
	GM_MSG_CAPTURE_FORBID_MOVE,
	GM_MSG_CAPTURE_ALLOW_MOVE,

//300
	GM_MSG_CAPTURE_SYNC_FLAG_POS,

//GMËù²ÉÓÃµÄÏûÏ¢	
	GM_MSG_GM_GETPOS=600,			//È¡µÃÖ¸¶¨Íæ¼ÒµÄ×ø±ê param ÊÇ cs_index, content ÊÇsid
	GM_MSG_GM_MQUERY_MOVE_POS,		//GMÒªÇó²éÑ¯×ø±ê ÓÃÓÚÏÂÒ»²½Ìø×ªµ½Íæ¼Ò´¦ 
	GM_MSG_GM_MQUERY_MOVE_POS_RPY,		//GMÒªÇó²éÑ¯×ø±êµÄ»ØÓ¦,ÓÃÓÚGMµÄÌø×ªÃüÁî contentÊÇµ±Ç°µÄinstance key
	GM_MSG_GM_RECALL,			//GMÒªÇó½øĞĞÌø×ª
	GM_MSG_GM_CHANGE_EXP,			//GMÔö¼Óexp ºÍsp , param ÊÇ exp , content ÊÇsp
	GM_MSG_GM_ENDUE_ITEM,			//GM¸øÓëÁËÈô¸ÉÎïÆ· £¬param ÊÇitem id, content ÊÇÊıÄ¿ 
	GM_MSG_GM_ENDUE_SELL_ITEM,		//GM¸øÓëÁËÉÌµêÀïÂôµÄÎïÆ·£¬ÆäËûÍ¬ÉÏ
	GM_MSG_GM_REMOVE_ITEM,			//GMÒªÇóÉ¾³ıÄ³Ğ©ÎïÆ·£¬param ÊÇitem id, content ÊÇÊıÄ¿
	GM_MSG_GM_ENDUE_MONEY,			//GMÔö¼Ó»òÕß¼õÉÙ½ğÇ®
	GM_MSG_GM_RESURRECT,			//GMÒªÇó¸´»î
	GM_MSG_GM_OFFLINE,			//GMÒªÇóÏÂÏß 
	GM_MSG_GM_DEBUG_COMMAND,		//GMÒªÇóÏÂÏß 
	GM_MSG_GM_RESET_PP,			//GM½øĞĞÏ´µã²Ù×÷

	GM_MSG_MAX,

};

struct msg_usermove_t	//ÓÃ»§ÒÆ¶¯²¢ÇÒ¿çÔ½±ß½çµÄÏûÏ¢
{
	int cs_index;
	int cs_sid;
	int user_id;
	A3DVECTOR newpos;	//ÏûÏ¢ÀïÃæÓĞoldpos
	size_t leave_data_size;	//Àë¿ª·¢ËÍµÄÏûÏ¢´óĞ¡£¨¸ÃÏûÏ¢¸½¼ÓÔÚºóÃæ)
	size_t enter_data_size;	//Àë¿ª·¢ËÍµÄÏûÏ¢´óĞ¡£¨¸ÃÏûÏ¢¸½¼ÓÔÚºóÃæ)
};

struct msg_aggro_info_t
{
	XID source;		//Ë­Éú³ÉÁËÕâĞ©³ğºŞ
	int aggro;		//³ğºŞµÄ´óĞ¡
	int aggro_type;		//³ğºŞµÄÀàĞÍ
	int faction;		//¶Ô·½ËùÊôµÄÅÉÏµ
	int level;		//¶Ô·½µÄ¼¶±ğ
};

struct msg_watching_t
{
	int faction;		//Ô´µÄÅÉÏµ
	int invisible_rate;	//ÒşÉíÄÜÁ¦
	int family_id;		//¼Ò×åid
	int mafia_id;		//°ïÅÉid
	int zone_id;		//·şÎñÆ÷id
};

struct msg_aggro_list_t
{
	int count;
	struct 
	{
		XID id;
		int aggro;
	}list[1];
};

struct msg_cry_for_help_t
{
	XID attacker;
	int lamb_faction;
	int helper_faction;
};

struct msg_aggro_alarm_t
{
	XID attacker;	//¹¥»÷Õß
	int rage;	
	int faction;	//·¢ËÍÕßµÄÅÉÏµ
	int target_faction;	//Ä¿±êµÄ½ÓÊÜÇó¾ÈÀàĞÍ
};

struct msg_exp_t
{
	int level;
	int64_t exp;
	bool no_exp_punish;
};

struct msg_grp_exp_t
{
	int level;
	int64_t exp;
	float rand;
};

struct msg_grpexp_info
{
	int64_t exp;
	int64_t team_damage;
	int team_seq;
	int npc_level;
	int npc_id;
	int npc_tag;
	float r;
	bool no_exp_punish;
};

struct msg_grpexp_t
{
	XID who;
	int64_t damage;
};

struct gather_reply
{
	int can_be_interrupted;
	int eliminate_tool;	//ÏûºÄ¹¤¾ßµÄID
};

struct gather_result
{
	int amount;
	int task_id;
	int eliminate_tool;		//Èç¹ûÉ¾³ıÎïÆ·Ôò¸½¼Ó´ËID
};

struct msg_pickup_t
{
	XID who;
	int team_id;
	int team_seq;
};

struct msg_gen_money
{
	int team_id;
	int team_seq;
};

struct msg_npc_transform
{
	int id_in_build;
	int time_use;
	int flag;
	int id_buildup;
	enum 
	{
		FLAG_DOUBLE_DMG_IN_BUILD = 1,
	};
};

struct msg_player_kill_player
{
	bool bpKilled;		//ÊÇ·ñÕı³£É±ËÀ£¬ÈçÊÇ£¬ÔòÓ¦½øĞĞPKÖµºÍÆäËû·½ÃæµÄĞŞÕı
	bool noDrop;		//ÊÇ·ñ²»µôÂäËÀÍö(°ïÕ½¹æÔò²Å»áÈç´Ë) 
	int mafia_id;		//±»É±ËÀÕßµÄ°ïÅÉ ÓÃÓÚ°ïÕ½Ê±
	int pkvalue;		//±»É±ËÀÕßµÄPKVALUE
	
};

struct msg_task_transfor_award
{
	int gold;
	int64_t exp;
	int reputation;
	int region_rep_idx;
	int region_rep_val;
	int common_item_id;
	int common_item_count;
	bool common_item_bind;
	int common_item_period;
	int task_item_id;
	int task_item_count;
	int title;
public:
	msg_task_transfor_award():	gold(0),exp(0),reputation(0),region_rep_idx(0),region_rep_val(0),
					common_item_id(0),common_item_count(0),common_item_bind(0),common_item_period(0),
					task_item_id(0),task_item_count(0),title(0)
	{}

	void SetGold(int __gold) { gold = __gold;}
	void SetExp(int64_t __exp) { exp = __exp;}
	void SetReputation(int __rep) { reputation = __rep;}
	void SetRegionReputation(int __idx,int __rep) { region_rep_idx = __idx; region_rep_val = __rep;}
	void SetCommonItem(int __id, int __count, bool __bind, int __period) { common_item_id = __id; common_item_count = __count; common_item_bind = __bind; common_item_period = __period;}
	void SetTaskItem(int __id, int __count) { task_item_id = __id; task_item_count = __count;}
	void SetTitle(int __title) { title = __title;}

};

struct msg_catch_pet
{
	int monster_id;
	int monster_level;
	int monster_raceinfo;
	int monster_catchdifficulty;
	float monster_hp_ratio;	
	
};

struct msg_player_enter_carrier
{
	A3DVECTOR rpos;
	unsigned char rdir;
};

struct msg_player_leave_carrier
{
	A3DVECTOR pos;
	unsigned char dir;
};

struct msg_enter_carrier
{
	A3DVECTOR rpos;
	unsigned char rdir;
	A3DVECTOR carrier_pos;
	unsigned char carrier_dir;
};

struct msg_leave_carrier
{
	A3DVECTOR pos;
	unsigned char dir;
	unsigned char reason;	//0 - Ö÷¶¯ÇëÒª  1 - ±»Ìß
};

struct msg_apply_party
{
	int faction;
	int family_id;
	int mafia_id;
	int level;
	int sectid;
	int referid;
};

struct msg_team_invite
{
	int pickup_flag;
	int family_id;
	int mafia_id;
	int zone_id;
	int level;
	int sectid;
	int referid;
};

struct msg_combo
{
	int combo_type;
	int combo_color;
	int combo_color_num;
};

struct msg_get_clone_equipment
{
	int cs_sid;
	int cs_index;
};

struct msg_transfer_attack
{
	char force_attack;
	int dmg_plus;
	int radius;
	int total;
};

struct msg_summon_cast_skill
{
	XID target;
	int skillid;
	int skilllevel;
};

struct msg_talisman_skill
{	
	int level;
	float range;
	char force;
	int skill_var[16];
	short skillList[8];
};

struct msg_level_raid_info
{
	char level;
	short matter_cnt;
	int start_time;
};

struct msg_kingdom_update_key_npc
{
	int cur_hp;
	int max_hp;
	int cur_op_type;
	int change_hp;
};

struct msg_kingdom_sync_key_npc
{
	int cur_hp;
	int max_hp;
	int hp_add;
	int hp_dec;
};

struct msg_kingdom_key_npc_info
{
	int cur_hp;
	int max_hp;
};


struct msg_kingdom_call_guard
{
	char kingname[20];
	size_t kingname_len;
	int line_id;
	int map_id;	
	A3DVECTOR pos;
};

struct msg_sync_flow_info 
{
	int score;
	int c_kill_cnt;
	int kill_cnt;
	int death_cnt;
	int monster_kill_cnt;
	int max_ckill_cnt;
	int max_mkill_cnt;
	bool battle_result_sent;
};

struct msg_npc_teleport_info
{
	int npc_id;
	A3DVECTOR pos;
	int lifetime;
};

struct msg_fac_building_complete
{
	int index;
	int tid;
	int level;
};

struct msg_master_get_pet_prop
{
	int cs_index;
	int uid;
	int sid;
	int pet_index;
};

struct msg_cs6v6_cheat_info
{
	bool is_cheat;
	int cheat_counter;
};

#endif

