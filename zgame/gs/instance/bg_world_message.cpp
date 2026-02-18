#include "battleground_manager.h"
#include "../world.h"
#include "../player_imp.h"
#include "../usermsg.h"
#include "../aei_filter.h"
#include "battleground_ctrl.h"

/*
 *	´¦ÀíÊÀ½çÏûÏ¢
 */
int
battleground_world_message_handler::RecvExternMessage(int msg_tag, const MSG & msg)
{
	//¶ÔÓÚ¸±±¾£¬Ö»½ÓÊÜ¸øÍò¼ÒµÄÏûÏ¢
	if(msg.target.type != GM_TYPE_PLAYER && msg.target.type != GM_TYPE_MANAGER ) return 0;
	if(msg_tag != _manager->GetWorldTag())
	{
		//¸ô¶ÏÄ³Ğ©ÏûÏ¢.......
	}

	//»¹ĞèÒªÒª¸ô¶ÏÄ³Ğ©ÏûÏ¢
	//ÕâÀï»¹Ó¦¸ÃÖ±½Ó´¦ÀíÄ³Ğ©ÏûÏ¢
	//Êı¾İ×ª·¢ĞèÒª¾­¹ıÅĞ¶¨

	//ÓĞĞ©ÏûÏ¢ĞèÒª¾­¹ıÌØÊâ×ª·¢
	switch(msg.message)
	{
		/*
		case GM_MSG_SWITCH_USER_DATA:
			{
				if(msg.content_length < sizeof(instance_key)) return 0;
				instance_key * key = (instance_key*)msg.content;
				//ÏûÏ¢µÄÍ·²¿±ØĞëÊÇinstance_key
				//ºóÃæ¸½¼Ó×ÅÍæ¼ÒÊı¾İ
				ASSERT(key->target.key_level1 == msg.source.id);
				instance_hash_key hkey;
				_manager->TransformInstanceKey(key->target, hkey);
				int index = _manager->GetWorldByKey(hkey);
				if(index < 0) return 0;
				return 0;
				//return _manager->GetWorldByIndex(index)->DispatchMessage(msg); $$$$$ BW
			}
			*/

		case GM_MSG_PLANE_SWITCH_REQUEST:
		//È·¶¨ÇĞ»»ÇëÇó 
		//¼ì²é¸±±¾ÊÀ½çÊÇ·ñ´æÔÚ£¬Èç¹û²»´æÔÚ£¬Ôò·ÅÈëµÈ´ıÁĞ±í£
		//Èç¹ûÒÑ¾­´æÔÚ£¬ÔòË¢ĞÂÒ»ÏÂ·şÎñÆ÷µÄÊ±¼ä±êÖ¾£¬²¢·µ»Ø³É¹¦±êÖ¾
		//Õâ¸ö²Ù×÷Òª½øĞĞËø¶¨£¬ÒÔÃâÉ¾³ıÊÀ½ç 
		{
			if(msg.content_length != sizeof(instance_key)) 
			{
				ASSERT(false);
				return 0;
			}
			instance_key * key = (instance_key*)msg.content;
			int rst = _manager->CheckPlayerSwitchRequest(msg.source,key,msg.pos,msg.param);
			if(rst == 0)
			{
				//½øĞĞ»ØÀ¡ÏûÏ¢
				MSG nmsg = msg;
				nmsg.target = msg.source;
				nmsg.source = msg.target;
				nmsg.message = GM_MSG_PLANE_SWITCH_REPLY;
				gmatrix::SendMessage(nmsg);
			}
			else if(rst > 0)
			{
				MSG nmsg;
				BuildMessage(nmsg,GM_MSG_ERROR_MESSAGE,msg.source,msg.target,msg.pos,rst);
				gmatrix::SendMessage(nmsg);
			}
			//Èç¹ûrstĞ¡ÓÚ0£¬±íÃ÷µ±Ç°ÎŞ·¨È·¶¨ÊÇ·ñÄÜ¹»´´½¨ÊÀ½ç£¬ĞèÒªµÈ´ı ËùÒÔ·´¶øÊ²Ã´¶¼²»×÷
		}
		return 0;

		case GM_MSG_CREATE_BATTLEGROUND:
		{
			//battle_ground_param &param = *(battle_ground_param*) msg.content;
			//_manager->CreateBattleGround(param);
		}
		return 0;

		default:
		if(msg.target.type == GM_TYPE_PLAYER)
		{
			int index = _manager->GetPlayerWorldIdx(msg.target.id);
			if(index < 0) return 0;
			return 0;
			//return _manager->GetWorldByIndex(index)->DispatchMessage(msg);
		}
		//·şÎñÆ÷ÏûÏ¢ÉĞÎ´´¦Àí ..........
	}
	return 0;
}

void
battleground_world_message_handler::SetInstanceFilter(gplayer_imp * pImp,instance_key & ikey)
{
	//¼ÓÈë¸±±¾key¼ì²âfilter,Õâ¸öfilterÔÚÇĞ»»·şÎñÆ÷Ê±²»»á½øĞĞ±£´æºÍ»Ö¸´
	pImp->_filters.AddFilter(new aebf_filter(pImp,FILTER_CHECK_INSTANCE_KEY,ikey.target.key_level3));
}

void 
battleground_world_message_handler::PlayerPreEnterServer(gplayer * pPlayer, gplayer_imp * pImp,instance_key & ikey)
{	
	world * pPlane = pImp->_plane;
	
	//¸ù¾İÍæ¼ÒµÄ°ïÅÉÉè¶¨¹¥·½ºÍÊØ·½
	//Ôö¼ÓÈËÊıµÄ²Ù×÷ÔÚ Íæ¼ÒµÄEnterServer²Ù×÷ÖĞÍê³É
	battleground_ctrl * pCtrl = (battleground_ctrl*)(pPlane->w_ctrl);

	int id = pPlayer->id_mafia;
	if(id)
	{
		if(id == pCtrl->_data.faction_attacker)
		{
			//¹¥·½
			pPlayer->SetBattleOffense();
			//¼ì²éÈËÊıÉÏÏŞ ×¢Òâ¼õÉÙÈËÊıÉÏÏŞÊÇÔÚplayer_battleÀïµÄPlayerLeaveWorldÀï×öµÄ
			if(!pCtrl->AddAttacker())
			{
				//ÈËÊıÒÑÂú,Çå³ıÀïÃæµÄ°ïÅÉÄÚÈİ
				ikey.target.key_level3 = -1;

				//Çå³ıÕ½³¡µÄ±êÖ¾(ºóÃæÓĞÓÃ)
				pPlayer->ClrBattleMode();
			}
		}
		else
		if(id == pCtrl->_data.faction_defender)
		{	
			//ÊØ·½
			pPlayer->SetBattleDefence();
			//¼ì²éÈËÊıÉÏÏŞ ×¢Òâ¼õÉÙÈËÊıÉÏÏŞÊÇÔÚplayer_battleÀïµÄPlayerLeaveWorldÀï×öµÄ
			if(!pCtrl->AddDefender())
			{
				//ÈËÊıÒÑÂú,Çå³ıÀïÃæµÄ°ïÅÉÄÚÈİ
				ikey.target.key_level3 = -1;

				//Çå³ıÕ½³¡µÄ±êÖ¾(ºóÃæÓĞÓÃ)
				pPlayer->ClrBattleMode();
			}
		}
	}
}

int
battleground_world_message_handler::HandleMessage(const MSG & msg)
{
	//ÓĞĞ©ÏûÏ¢²Ù×÷¿ÉÄÜ»á±È½Ï·ÑÊ±¼ä£¬ÊÇ·ñ¿ÉÒÔ¿¼ÂÇTaskÍê³É£¬²»¹ıÓÃÏß³ÌµÄ»°¾ÍÒª¿¼ÂÇmsgµÄÊı¾İÎÊÌâÁË¡£

	switch(msg.message)
	{

		case GM_MSG_PLANE_SWITCH_REQUEST:
			//È·¶¨ÇĞ»»ÇëÇó 
			//¼ì²é¸±±¾ÊÀ½çÊÇ·ñ´æÔÚ£¬Èç¹û²»´æÔÚ£¬Ôò·ÅÈëµÈ´ıÁĞ±í£
			//Èç¹ûÒÑ¾­´æÔÚ£¬ÔòË¢ĞÂÒ»ÏÂ·şÎñÆ÷µÄÊ±¼ä±êÖ¾£¬²¢·µ»Ø³É¹¦±êÖ¾
			//Õâ¸ö²Ù×÷Òª½øĞĞËø¶¨£¬ÒÔÃâÉ¾³ıÊÀ½ç 
			{
				if(msg.content_length != sizeof(instance_key)) 
				{
					ASSERT(false);
					return 0;
				}
				instance_key * key = (instance_key*)msg.content;
				int rst = _manager->CheckPlayerSwitchRequest(msg.source,key,msg.pos,msg.param);
				if(rst == 0)
				{
					//½øĞĞ»ØÀ¡ÏûÏ¢
					MSG nmsg = msg;
					nmsg.target = msg.source;
					nmsg.source = msg.target;
					nmsg.message = GM_MSG_PLANE_SWITCH_REPLY;
					gmatrix::SendMessage(nmsg);
				}
				else if(rst > 0)
				{
					MSG nmsg;
					BuildMessage(nmsg,GM_MSG_ERROR_MESSAGE,msg.source,msg.target,msg.pos,rst);
					gmatrix::SendMessage(nmsg);
				}
				//Èç¹ûrstĞ¡ÓÚ0£¬±íÃ÷µ±Ç°ÎŞ·¨È·¶¨ÊÇ·ñÄÜ¹»´´½¨ÊÀ½ç£¬ĞèÒªµÈ´ı ËùÒÔ·´¶øÊ²Ã´¶¼²»×÷
			}
			return 0;
		default:
			return world_message_handler::HandleMessage(msg);

	}
	return 0;
}

