#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../world.h"
#include "../player.h"
#include "../player_imp.h"
#include "../config.h"
#include "../userlogin.h"
#include "../usermsg.h"
#include "../clstab.h"
#include "../playertemplate.h"
#include "../aei_filter.h"

#include <deque>
#include <db_if.h>
#include "../task/taskman.h"
#include "instance_manager.h"
#include <base64.h>

extern int __allow_login_class_mask;
extern int __allow_login_class_mask1;
namespace {
class LoginTask :  public abase::ASmallObject , public GDB::Result
{
	int _uid;
	int _cs_index;
	int _cs_sid;
	void * _auth_data;
	size_t _auth_size;
	char _flag;
public:
	LoginTask(int uid,int cs_index, int cs_sid, const void * auth_data , size_t auth_size, char flag)
		:_uid(uid),_cs_index(cs_index),_cs_sid(cs_sid),_auth_data(NULL),_auth_size(auth_size),_flag(flag)
		{
			if(auth_size)
			{
				_auth_data = abase::fastalloc(auth_size);
				memcpy(_auth_data,auth_data,auth_size);
			}
		}
	~LoginTask()
	{
		if(_auth_data)
		{
			abase::fastfree(_auth_data,_auth_size);
		}
	}

	void Failed()
	{
		GMSV::SendLoginRe(_cs_index,_uid,_cs_sid,-1,_flag);	// login failed
		delete this;
	}
public:
	virtual void OnTimeOut()
	{
		GLog::log(GLOG_ERR,"ÓÃ»§%d´ÓÊı¾İ¿âÈ¡µÃÊı¾İ³¬Ê±",_uid);
		Failed();
	}
	
	virtual void OnFailed()
	{
		GLog::log(GLOG_ERR,"ÓÃ»§%d´ÓÊı¾İ¿âÈ¡µÃÊı¾İÊ§°Ü",_uid);
		Failed();
	}
	virtual void OnGetRole(int id,const GDB::base_info * pInfo, const GDB::vecdata * data,const GNET::GRoleDetail* pRole);
};


void 
LoginTask::OnGetRole(int id,const GDB::base_info * pInfo, const GDB::vecdata * data, const GNET::GRoleDetail * pRole)
{
	//¼ì²âµÇÂ¼¿ØÖÆ  
	//if(!(__allow_login_class_mask & (1 << (pInfo->cls & 0x7F))))
	if(object_base_info::CheckCls(pInfo->cls, __allow_login_class_mask, __allow_login_class_mask1))
	{               
		GMSV::SendLoginRe(_cs_index,_uid,_cs_sid,1,_flag);    // login failed
		//´ËÊ±°´ÕÕ´íÎóÀ´´¦Àí
		OnFailed();
		return;
	}

	if(!do_login_check_data(pInfo,data))
	{
		GMSV::SendLoginRe(_cs_index,_uid,_cs_sid,1,_flag);    // login failed
		//´ËÊ±°´ÕÕ´íÎóÀ´´¦Àí
		GLog::log(GLOG_ERR,"ÓÃ»§%dÊı¾İÒì³££¬ÎŞ·¨µÇÂ¼",id);
		OnFailed();
		return; 
	}


	//´ÓÊı¾İ¿â¶ËÈ¡µÃÊı¾İ
	char name_base64[64] ="Î´Öª";
	if(data->user_name.data)
	{
		size_t name_len = data->user_name.size;
		if(name_len > 32) name_len = 32;
		base64_encode((unsigned char*)(data->user_name.data),name_len,name_base64);
	}
	
	GLog::log(GLOG_INFO,"ÓÃ»§%d´ÓÊı¾İ¿âÈ¡µÃÊı¾İ£¬Ö°Òµ%d,¼¶±ğ%d Ãû×Ö'%s'",_uid,pInfo->cls,pInfo->level,name_base64);

	//´ÓÊı¾İ¿âÖĞÈ¡µÃinstance key
	// BW instance_world_manager * pManager = (instance_world_manager*)world_manager::GetInstance();
	instance_world_manager * pManager = NULL;
	instance_hash_key ikey;
	player_var_data::GetInstanceKey(data->var_data.data,data->var_data.size,ikey);


	//½øĞĞÊÀ½çµÄ·ÖÅä
	int world_index;
	world * pPlane = pManager->AllocWorld(ikey,world_index);
	if(!pPlane)
	{
		//Éú³ÉÊÀ½çÊ§°ÜÁË£¬Ó¦¸ÃÊÇÃ»ÓĞ×ã¹»µÄ¸±±¾
		GMSV::SendLoginRe(_cs_index,_uid,_cs_sid,5,_flag); 
		delete this;
		return;
	}
	gplayer * pPlayer = pPlane->AllocPlayer();
	if(pPlayer == NULL)
	{
		//·¢ËÍÃ»ÓĞÎïÀí¿Õ¼ä¿ÉÒÔÈİÄÉPlayerµÄĞÅÏ¢
		__PRINTF("ÓÃ»§´ïµ½ÎïÀí×î´óÏŞÖÆÖµ\n");
		GMSV::SendLoginRe(_cs_index,_uid,_cs_sid,4,_flag); 
		return;
	}
	GLog::log(GLOG_INFO,"ÓÃ»§%d´Ó%d¿ªÊ¼µÇÂ¼",_uid,_cs_index);
	pPlayer->cs_sid = _cs_sid;
	pPlayer->cs_index = _cs_index;
	pPlayer->ID.id = _uid;
	pPlayer->ID.type = GM_TYPE_PLAYER;
	pPlayer->login_state = gplayer::WAITING_LOGIN;
	pPlayer->pPiece = NULL;
	
	if(!pPlane->MapPlayer(_uid,pPlane->GetPlayerIndex(pPlayer)))
	{
		//map player Ê§°Ü£¬±íÊ¾ÔÚÕâÒ»Ë²¼äÓĞÈË¼ÓÈë
		pPlane->FreePlayer(pPlayer);
		mutex_spinunlock(&pPlayer->spinlock);
		GMSV::SendLoginRe(_cs_index,_uid,_cs_sid,4,_flag);       // login failed
		return;
	}
	ASSERT(pPlayer->imp == NULL);
	pPlayer->imp = NULL; 

	userlogin_t user;
	memset(&user,0,sizeof(user));
	user._player = pPlayer;
	user._plane = pPlane;
	user._uid = _uid;
	user._auth_data = _auth_data;
	user._auth_size = _auth_size;

	//ÉèÖÃ¸±±¾¶ÔÓ¦µÄ±êÖ¾
	pManager->SetPlayerWorldIdx(_uid,world_index);

	//¼ì²éÊÀ½çÊÇ·ñºÍ´æÅÌÊı¾İÒ»ÖÂ£¬Èç¹û²»Ò»ÖÂ£¬Ôò½«×ø±êÉèÖÃµ½Æğµã 
	int last_instance_timestamp;
	int last_instance_tag;
	A3DVECTOR last_instance_pos;
	player_var_data::GetLastInstance<0>(data->var_data.data,data->var_data.size,last_instance_tag,last_instance_pos,last_instance_timestamp);

	A3DVECTOR login_pos = A3DVECTOR(pInfo->posx, pInfo->posy, pInfo->posz);

	if(last_instance_tag == pManager->GetWorldTag())
	{
		if(last_instance_timestamp > 0)
		{
			if(last_instance_timestamp == pPlane->w_create_timestamp)
			{	
				//´æÅÌ¸±±¾ÏÖÔÚÒÀÈ»´æÔÚ£¬×ø±êÎŞĞè×ª»» Ê²Ã´¶¼²»ĞèÒª±ä
			}
			else
			{
				//µ±Ç°¸±±¾ÒÑ¾­²»´æÔÚ£¬Ê¹ÓÃ´æÅÌÎ»ÖÃ  ²¢¸üĞÂÊ±¼ä´Á
				login_pos = last_instance_pos;
				last_instance_timestamp = pPlane->w_create_timestamp;
			}
		}
		else
		{
			last_instance_tag = -1;
		}
	}
	else
	{
		last_instance_tag = -1;
	}
	//Èç¹ûÃ»ÓĞÍê³ÉÎ»ÖÃµÄ±ê¶¨£¬ÄÇÃ´Íæ¼ÒµÄÊı¾İ½«»áÊÇ´íÎóµÄ£¬ÕâÊ±¸ÃÈçºÎ£¿¿¼ÂÇÊ¹ÓÃÅäÖÃÎÄ¼ş¸ø³öµÄÄ¬ÈÏÈë¿Ú
	if(last_instance_tag <= 0)
	{
		//ÊÔÍ¼Ê¹ÓÃÅäÖÃÎÄ¼şÄÚ¿ÉÄÜ´æÔÚµÄÎ»ÖÃ
		const struct world_pos & point = pManager->GetSavePoint();
		if(point.tag == pManager->GetWorldTag())
		{
			//±ØĞëtagÂú×ãÒªÇó²ÅÄÜÊ¹ÓÃµ±Ç°µÄÉèÖÃ
			last_instance_tag = pManager->GetWorldTag();
			last_instance_pos = point.pos;

			//½«µÇÂ¼Î»ÖÃÒ²¸ÄÎª±£´æÎ»ÖÃ
			login_pos = last_instance_pos;
		}
	}
	
	//½øĞĞÊÀ½çµÄ¼ÓÈë¹¤×÷
	do_player_login(login_pos, pInfo,data,user);

	if(pPlayer->imp)
	{
		gplayer_imp *pImp =(gplayer_imp*)pPlayer->imp;
		pImp->_filters.AddFilter(new aei_filter(pImp,FILTER_CHECK_INSTANCE_KEY));

		//ÉèÖÃ¸±±¾Èë¿Ú×ø±ê
		pImp->SetLastInstancePos(last_instance_tag, last_instance_pos, last_instance_timestamp);
	}

	//½â¿ªÍæ¼ÒµÄËø
	//µÈ´ıÍæ¼Ò·¢À´EnterWorld
	mutex_spinunlock(&pPlayer->spinlock);

	//É¾³ı×ÔÉí
	delete this;
}

}

void	instance_user_login(int cs_index,int cs_sid,int uid,const void * auth_data, size_t auth_size)
{
	ASSERT(false);
	/*
	//¸±±¾²»ÔÊĞíÖ±½ÓµÇÂ¼
	int world_index;
	//BW if(world_manager::GetInstance()->FindPlayer(uid,world_index))
	if(gmatrix::FindPlayer(uid,world_index))
	{
		//·¢ËÍÒÑ¾­ÓĞÈËµÇÂ¼µÄÏûÏ¢
		GMSV::SendLoginRe(cs_index,uid,cs_sid,3);	// login failed
		GLog::log(GLOG_WARNING,"ÓÃ»§%dÒÑ¾­µÇÂ¼(%d,%d)(ÔÚÊÀ½ç±%d)",uid , cs_index,cs_sid,world_index);
		return ;
	}

	//´ÓÊı¾İ¿âÈ¡³öÊı¾İ£¬²¢½øĞĞËùÓĞµÄµÇÂ¼²Ù×÷
	GDB::get_role(uid, new LoginTask(uid,cs_index,cs_sid,auth_data,auth_size));
	*/
}
