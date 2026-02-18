#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "world.h"
#include "player.h"
#include "player_imp.h"
#include "config.h"
#include "userlogin.h"
#include "usermsg.h"
#include "clstab.h"
#include "playertemplate.h"

#include <deque>
#include <db_if.h>
#include "task/taskman.h"
#include <base64.h>
#include "gmatrix.h"

delivery_state g_d_state;

void do_player_login(const A3DVECTOR & ppos, const GDB::base_info * pInfo, const GDB::vecdata * data,const userlogin_t &user, char trole, char flag)
{
	gplayer * _player = user._player;
	world * _plane = user._plane;
	
	_player->login_state = gplayer::WAITING_ENTER;
	_player->pos = ppos; 
	//判定一下玩家的位置是否低于地面
	float hy = _plane->GetWorldManager()->GetTerrain().GetHeightAt(_player->pos.x,_player->pos.z);
	if(_player->pos.y < hy) _player->pos.y = hy;
	
	_player->pPiece = NULL;


	//创建玩家类
	world_manager::player_cid cid;
	_plane->GetWorldManager()->GetPlayerCid(cid);
	_player->imp = CF_Create(cid.cid[0],cid.cid[2],cid.cid[1],_plane,_player);

	_player->body_size = PLAYER_BODYSIZE;		//现在暂时是固定值
	
	//初始化玩家属性
	gplayer_imp * pImp = (gplayer_imp *)_player->imp;
	pImp->SetPlayerClass(pInfo->cls,pInfo->gender);
	pImp->SetBasicData(pInfo->level,pInfo->deity_level,pInfo->exp,pInfo->deity_exp,pInfo->hp,pInfo->mp,pInfo->dp,pInfo->pp,pInfo->money,pInfo->battlescore, pInfo->userid, user._loginip);
	pImp->SetPlayerName(data->user_name.data,data->user_name.size);
	pImp->SetDBTimeStamp(pInfo->timestamp);
	pImp->SetRecipeData((const short*)data->recipes.data , data->recipes.size / sizeof(short));
	pImp->SetSpouse(pInfo->spouse);
	pImp->SetSectID(pInfo->sectid);
	pImp->SetSectInitLevel(pInfo->initiallevel);
	pImp->SetCultivation(pInfo->cultivation);
	
	//设置帮派ID
	pImp->UpdateMafiaInfo(pInfo->factionid, pInfo->familyid, pInfo->factionrole);
	//设置帮派贡献度 必须在帮派数据后面之后执行
	pImp->SetMafiaContribution(pInfo->contribution);
	//设置加入帮派时间
	pImp->SetMafiaJoinTime(pInfo->jointime);
	pImp->SetFactionCoupon( pInfo->fac_coupon ); // Youshuang add, 设置帮派金券
	pImp->SetFactionCouponAdd( pInfo->fac_coupon_add ); // Youshuang add
	//raw_wrapper newyear_award_info_ar(data->newyear_award_info.data,data->newyear_award_info.size);
	//pImp->LoadNewYearAwardInfo( newyear_award_info_ar ); // Youshuang add for new year award

	//设置家族贡献度
	pImp->SetFamilyContribution(pInfo->devotion);

	//设置圈子信息
	pImp->UpdateCircleInfo(pInfo->circleid, pInfo->circletitlemask);

	//设置zoneid
	pImp->SetZoneId(pInfo->src_zoneid);

	//设置防沉迷数据      $$$$$$$$$$$$$$$
	pImp->SetWallowData(NULL,0);

	//设置玩家的角色创建时间
	pImp->SetRoleCreateTime(pInfo->create_time);

	//设置转生
	pImp->InitRebornInfo(data->reborndata.data, data->reborndata.size);

	pImp->SetTalismanValue(pInfo->talismanscore);
	pImp->SetRuneRefineValue(pInfo->runescore);

	raw_wrapper propadd_ar(data->propadd.data, data->propadd.size);
	pImp->LoadPropAdd(propadd_ar);

	//这个要在后面，因为这时才设置了级别 转生数据等
	player_template::InitPlayerData(pInfo->cls,pImp);

	//设置debug模式
	gplayer_controller * pCtrl = (gplayer_controller*)pImp->_commander;
	pCtrl->SetDebugMode(player_template::GetDebugMode());

	pCtrl->SetPrivilege(user._auth_data,user._auth_size);
	if(user._auth_data)
	{
		int ad[256];
		size_t len = user._auth_size;
		if(len > sizeof(ad)) len = sizeof(ad);
		memset(ad,0,sizeof(ad));
		memcpy(ad,user._auth_data,len);
		GLog::log(GLOG_INFO,"用户%d拥有GM权限%08x%08x%08x%08x%08x%08x%08x%08x%08x%08x",user._uid,ad[0],ad[1],ad[2],ad[3],ad[4],ad[5],ad[6],ad[7],ad[8],ad[9]);
	}

	//设置多倍经验
	raw_wrapper multi_exp_ar(data->multi_exp.data,data->multi_exp.size);
	pImp->LoadMultiExp(multi_exp_ar);

	//设置双倍点卡信息(双倍经验已经被多倍经验替代, 但是需要读入老数据, 这个的读取需要放到多倍经验的读取后面)
	pImp->SetRestParam(data->dbltime_data.data,data->dbltime_data.size);

	//设置点卡信息
	pImp->MallInfo().SetInfo(pInfo->cash,pInfo->cash_used, pInfo->cash_delta, pInfo->cash_serial, pInfo->cash_add2);
	//设置web商城购买信息
	pImp->MallInfo().SetWebOrder(data->weborders);

	//设置鸿利信息
	pImp->BonusInfo().SetInfo(pInfo->bonus_withdraw, pInfo->bonus_reward, pInfo->bonus_used);


	pImp->SetPlayEd(g_timer.get_systime(),pInfo->time_used);

	//设置称谓列表
	pImp->SetPlayerTitleList((const short *)data->title_list.data,data->title_list.size);
	pImp->SelectPlayerTitle(pInfo->cur_title);
	pImp->SetUsedTitle(pInfo->used_title); // Youshuang add

	//设置区域友好度
	pImp->SetRegionReputation(data->region_reputation.data,data->region_reputation.size);

	//设置古卷挖宝数据
	raw_wrapper treasure_ar(data->treasure_info.data,data->treasure_info.size);
	pImp->SetTreasureInfo(treasure_ar);
	
	raw_wrapper littlepet_ar(data->littlepet.data,data->littlepet.size);
	pImp->SetLittlePetInfo(littlepet_ar);

	//设置相位信息
	raw_wrapper phase_ar(data->phase.data,data->phase.size);
	pImp->LoadPhaseInfo(phase_ar);


	pImp->SetFlagMask(pInfo->flag_mask);

	pImp->SetRegionReputation(data->region_reputation.data,data->region_reputation.size);

	int tu = pInfo->time_used;
	GLog::formatlog("formatlog:playerlogin:userid=%d:level=%d:money=%d:trashmoney=%d:cash=%d:cash_add2=%d",user._uid,pInfo->level,pInfo->money, pInfo->trash_money,pInfo->cash + pInfo->cash_delta,
			pInfo->cash_add2);
	GLog::log(GLOG_INFO,"用户%d已经游戏了%d小时%02d分%02d秒",user._uid,tu/3600, (tu/60)%60, tu % 60);
	GLog::log(GLOG_INFO,"用户%d包裹金钱%d,仓库金钱%d ", user._uid, pInfo->money, pInfo->trash_money);

	//初始化玩家副本key
	pImp->InitInstanceKey(pInfo->create_time, data->instancekeylist.data,data->instancekeylist.size);

	//设置玩家红名粉名数据 
	pImp->SetRawPKValue(pInfo->pkvalue);

	pImp->SetProduceSkill(pInfo->produceskill,pInfo->produceexp);

	//设置玩家动态数据临时
	if(data->var_data.size >= sizeof(int))
	{
		player_var_data::SetData(_player,pImp,data->var_data.data,data->var_data.size);
	}

	//设置帮派战斗的统计结果 被杀次数在 player_var_data 的SetData中完成 两个参数的初始值均为0
	pImp->SetMafiaDuelKills(pInfo->combatkills);

	//设置玩家的冷却时间数据
	pImp->SetCoolDownData(data->coolingtime.data,data->coolingtime.size);

	//设置玩家的初始修真级别
	_player->level = (unsigned char ) pInfo->level;
	_player->dt_level = (unsigned char ) pInfo->deity_level;

	pImp->SetReputation(pInfo->reputation);

	//重新装载需要存盘的filter, 这个要在读包裹数据前完成
	if(data->filter_data.size)
	{
		raw_wrapper ar(data->filter_data.data,data->filter_data.size);
		pImp->_filters.Load(ar);
		pImp->_filters.DBInit(pImp);
	}

	//初始化技能 
	pImp->InitSkillData(data->skill_data.data, data->skill_data.size);

	//初始化组合技技能
	pImp->InitCombineSkillData(data->composkills.data, data->composkills.size);
	

	//保存本次下线时间
	int offline_time  = g_timer.get_systime() - pInfo->updatetime;
	if(offline_time < 0) offline_time = 0;
	pImp->_last_db_save_time = pInfo->updatetime;
	
	pImp->SetReferID(pInfo->referrer);

	//初始化物品
	int bagsize = pInfo->bagsize;
	if(bagsize < ITEM_LIST_BASE_SIZE) bagsize = ITEM_LIST_BASE_SIZE;
	if(bagsize > ITEM_LIST_MAX_SIZE) bagsize = ITEM_LIST_MAX_SIZE;
	
	pImp->GetInventory().SetSize(bagsize);
	pImp->GetInventory().InitFromDBData(data->inventory);
	pImp->GetEquipInventory().InitFromDBData(data->equipment);
	pImp->GetTaskInventory().InitFromDBData(data->task_inventory);
	pImp->GetFashionInventory().InitFromDBData(data->fashion);

	int mountwingsize = pInfo->mountwingsize;
	if(mountwingsize < MOUNT_WING_INVENTORY_BASE_SIZE) mountwingsize = MOUNT_WING_INVENTORY_BASE_SIZE;
	if(mountwingsize > MOUNT_WING_INVENTORY_MAX_SIZE) mountwingsize = MOUNT_WING_INVENTORY_MAX_SIZE;
	pImp->GetMountWingInventory().SetSize(mountwingsize);
	pImp->GetMountWingInventory().InitFromDBData(data->mountwing);

	pImp->GetGiftInventory().InitFromDBData(data->gifts);
	pImp->GetFuwenInventory().InitFromDBData(data->fuwen);
	
	int pocketsize = pInfo->pocketsize;
	if(pocketsize < POCKET_INVENTORY_BASE_SIZE) pocketsize = POCKET_INVENTORY_BASE_SIZE; 
	if(pocketsize > POCKET_INVENTORY_MAX_SIZE) pocketsize = POCKET_INVENTORY_MAX_SIZE;
	pImp->GetPocketInventory().SetSize(pocketsize);	
	pImp->GetPocketInventory().InitFromDBData(data->pocket);
	
	int db_trashbox_size = pInfo->storesize;
	if(db_trashbox_size < TRASHBOX_BASE_SIZE) db_trashbox_size = TRASHBOX_BASE_SIZE;
	if(db_trashbox_size > TRASHBOX_MAX_SIZE) db_trashbox_size = TRASHBOX_MAX_SIZE;
	pImp->_trashbox.SetTrashBoxSize(db_trashbox_size);

	int db_trashbox_size2 = pInfo->storesize2;
	if(db_trashbox_size2 < MAFIA_TRASHBOX_BASE_SIZE) db_trashbox_size2 = MAFIA_TRASHBOX_BASE_SIZE;
	if(db_trashbox_size2 > TRASHBOX_MAX_SIZE) db_trashbox_size2 = TRASHBOX_MAX_SIZE;
	pImp->_trashbox.SetMafiaTrashBoxSize(db_trashbox_size2);
	if(pInfo->trashbox_active)
	{
		//读取普通仓库
		pImp->GetTrashBoxInventory().InitFromDBData(data->trash_box);
		pImp->_trashbox.GetMoney() = pInfo->trash_money;

		//读取帮派仓库
		pImp->GetTrashBoxInventoryMafia().InitFromDBData(data->trash_box2);
		
		//清除仓库修改标志
		pImp->ClearTrahsBoxWriteFlag();
	}
	if(data->trashbox_passwd.data)
	{
		pImp->_trashbox.SetPasswordMD5((const char *)data->trashbox_passwd.data, data->trashbox_passwd.size);
	}
	//SetDBData 一定要在InitFromDBData前面
	pImp->GetPetMan().SetDBData(pImp,data->petdata.data,data->petdata.size);
	pImp->GetPetBedgeInventory().InitFromDBData(data->petbadge);
	pImp->GetPetEquipInventory().InitFromDBData(data->petequip);

	//初始化时装衣橱快接键信息
	pImp->SetFashionHotkey(data->fashion_hotkey.data, data->fashion_hotkey.size);

	pImp->LoadRaidCountersDB(data->raid_data.data, data->raid_data.size);
	
	//成就
	pImp->GetAchievementManager().InitMap(data->achieve_map.data, data->achieve_map.size);
	raw_wrapper ar(data->achieve_active.data, data->achieve_active.size);
	pImp->GetAchievementManager().LoadActiveAchievement(ar);
	object_interface oif(pImp);
	pImp->GetAchievementManager().CheckAchievements(oif);
	pImp->LoadSpecialAchievementInfo(data->achieve_spec_info.data, data->achieve_spec_info.size);
	pImp->GetAchievementManager().InitAwardMap(data->achieve_award_map.data, data->achieve_award_map.size);  // Youshuang add

	//VIP奖励
	pImp->LoadVipAwardData(data->vipaward.data, data->vipaward.size);

	//在线倒计时奖励
	pImp->LoadOnlineAwardData(data->onlineaward.data, data->onlineaward.size);

	//设置消费值
	pImp->LoadConsumptionValue(pInfo->comsumption);

	//pImp->LoadFiveAnniDataDB(data->five_year.data, data->five_year.size);

	pImp->LoadTowerData(data->tower_raid.data, data->tower_raid.size);

	pImp->LoadPlayerBetData(data->five_year.data, data->five_year.size);

	//设置UI Transfer
	raw_wrapper ui_transfer_ar(data->ui_transfer.data, data->ui_transfer.size);
	pImp->LoadUITransfer(ui_transfer_ar);

	//设置Liveness
	raw_wrapper liveness_ar(data->liveness_info.data, data->liveness_info.size);
	pImp->LoadLiveness(liveness_ar);

	//设置台历
	raw_wrapper sale_promotion_ar(data->sale_promotion_info.data, data->sale_promotion_info.size);
	pImp->LoadSalePromotion(sale_promotion_ar);

	//设置星座信息
	raw_wrapper astrology_ar(data->astrology_info.data, data->astrology_info.size);
	pImp->LoadAstrologyData(astrology_ar);
	
	//设置6V6战场信息 LoadCollisionData和LoadExchange6V6AwardInfo函数的顺序不能乱
	raw_wrapper collision_ar(data->collision_info.data, data->collision_info.size);
	pImp->LoadCollisionData(collision_ar);

	raw_wrapper exchange_6v6_award_info(data->award_info_6v6.data, data->award_info_6v6.size);
	pImp->LoadExchange6V6AwardInfo(exchange_6v6_award_info);

	raw_wrapper hide_and_seek_ar(data->hide_and_seek_info.data,data->hide_and_seek_info.size);
	pImp->LoadHideAndSeekData(hide_and_seek_ar);

	//设置国王职位信息
	pImp->SetKingdomInfo();

	//加载符文信息
	raw_wrapper fuwen_compose_ar(data->fuwen_info.data,data->fuwen_info.size);
	pImp->LoadFuwenComposeInfo(fuwen_compose_ar);

	//初始化任务
	size_t buf_size = data->task_data.size;
	if(buf_size > pImp->_active_task_list.size()) buf_size = pImp->_active_task_list.size();
	memcpy(pImp->_active_task_list.begin(),data->task_data.data,buf_size);
	pImp->VerifyActiveTaskData(buf_size);
	
	buf_size = data->finished_task_data.size;
	if(buf_size > pImp->_finished_task_list.size()) buf_size = pImp->_finished_task_list.size();
	memcpy(pImp->_finished_task_list.begin(),data->finished_task_data.data,buf_size);

	buf_size = data->finished_time_task_data.size;
	if(buf_size > pImp->_finished_time_task_list.size()) buf_size = pImp->_finished_time_task_list.size();
	memcpy(pImp->_finished_time_task_list.begin(),data->finished_time_task_data.data,buf_size);

	PlayerTaskInterface task_if(pImp);
	task_if.InitActiveTaskList();

	

	//设置玩家角色状态 这里可能会操作包裹栏，所以应当在初始化包裹栏之后
	//这里还可能会添加filter 所以还应当放在filter读取之后
	pImp->SetPlayerCharMode(data->charactermode.data,data->charactermode.size);

	//重新计算玩家的所有数据
	property_policy::UpdatePlayer(pInfo->cls,pImp);

	//将玩家放入世界
	_plane->InsertPlayerToMan(_player);
	
	//发送登录成功消息给连接服务器
	GMSV::SendLoginRe(_player->cs_index,_player->ID.id,_player->cs_sid,0,flag,
			_plane->GetTag(), _player->pos.x,_player->pos.y,_player->pos.z,
			pInfo->level, pInfo->cls, pInfo->factionid,pInfo->factionrole,pInfo->contribution,
			pInfo->familyid, pInfo->devotion, pInfo->jointime, pInfo->battlescore, pInfo->circleid,
			_plane->GetWorldManager()->GetClientTag());   // login ok

	if(pImp->IsSectMaster())
	{
		int sectskill[32];
		int sectssize = 32;
		pImp->_skill.GetSectSkill(sectskill,sectssize);
		GMSV::UpdateSect(_player->ID.id, pImp->GetRegionReputation(SECT_MASTER_REP_INDEX) , sectskill,sectssize);
	}

	//现在除了加入网格之外无其他操作
	//用户若干时间内如果仍然不enter 则自动将玩家踢出的操作
	//现在link会在2分钟内进行断线操作
	GLog::log(GLOG_INFO,"用户%d连接成功，等待用户发送enter_world",_player->ID.id);

	//gplayer_imp 登录后的自定义处理
	pImp->PostLogin(user, trole, flag);

	//注意，因为还没有进入世界，所以player应该还没有开始心跳和种种其他操作，这些是通过
	//Login state完成了特殊消息的转发而做到的。
}


void user_enter_world(int cs_index,int sid,int uid, int locktime, int maxlocktime)
{
	GLog::log(GLOG_INFO,"用户%d进入世界",uid);
	int world_index;
	gplayer * pPlayer = gmatrix::FindPlayer(uid,world_index);
	if(!pPlayer)
	{
		GLog::log(GLOG_ERR,"用户%d进入世界时不在服务器内",uid);
		//该用户不在本服务器 这种错误不应该出现，记录日志即可
		return ;
	}
	spin_autolock alock(pPlayer->spinlock);
	if(pPlayer->login_state != gplayer::WAITING_ENTER || pPlayer->pPiece
		|| !pPlayer->IsActived() || pPlayer->cs_index != cs_index 
		|| pPlayer->cs_sid != sid || pPlayer->ID.id != uid || !pPlayer->imp)
	{
		//处于错误的状态，发送日志
		GLog::log(GLOG_WARNING,"enter_world:用户%d已经不在服务器里了",uid);
		return;
	}
	world * pPlane = pPlayer->imp->_plane;
	if(pPlayer->imp->_plane != pPlane)
	{
		GLog::log(GLOG_WARNING,"enter_world:用户%d已经不在服务器里了",uid);
		return ;
	}
	if(!pPlane || !pPlane->GetWorldManager() || !pPlane->GetWorldManager()->CanEnterWorld())
	{
		//如果世界不存在或者已经不能进入，不再等待10分钟直接清除
		int cs_index = pPlayer->cs_index;
		int uid = pPlayer->ID.id;
		int sid = pPlayer->cs_sid;
		pPlayer->imp->_commander->Release();
		GMSV::SendDisconnect(cs_index,uid,sid,0);
		GLog::log(GLOG_WARNING,"enter_world:世界已经处于非运行状态，用户%d无法进入世界",uid);
		return ;
	}
	//将player放入世界
	pPlane->InsertPlayer(pPlayer);
	MSG msg;
	BuildMessage2(msg,GM_MSG_ENTER_WORLD,pPlayer->ID,pPlayer->ID,pPlayer->pos, locktime, maxlocktime);
	pPlayer->imp->DispatchMessage(msg);
}

void    user_save_data(gplayer * pPlayer,GDB::Result * callback, int priority,int mask
		, bool spec_save , int spec_tag , const A3DVECTOR & spec_pos )
{
	//应该已经完成锁定
	ASSERT(pPlayer->spinlock);
	if(!pPlayer->IsActived() || 
			pPlayer->login_state == gplayer::WAITING_LOGIN ||
			pPlayer->login_state == gplayer::WAITING_ENTER)
	{
		GLog::log(GLOG_WARNING,"save_data:用户已经不再服务器里了,或者本次登陆尚未完成");
		return ;
	}

	gplayer_imp * pImp = (gplayer_imp *)pPlayer->imp;
	pImp->VerifyWhileSave(); 

	GDB::base_info info;
	GDB::vecdata data;
	memset(&info,0,sizeof(info));
	memset(&data,0,sizeof(data));
	info.id = pPlayer->ID.id;
	info.cls = pImp->GetObjectClass();
	
	pImp->GetBasicData(info.level,info.deity_level,info.exp,info.deity_exp,info.hp,info.mp,info.dp,info.pp,info.money,info.battlescore, info.userid, info.loginip);

	pImp->GetPlayerClass(info.cls,info.gender);
	info.produceskill = pImp->GetProduceLevel();
	info.produceexp = pImp->GetProduceExp();

	int tag = 1;
	A3DVECTOR pos;
	if(spec_save)
	{
		tag = spec_tag;
		pos = spec_pos;
	}
	else
	{
		pImp->GetWorldManager()->GetLogoutPos(pImp,tag,pos);
	}
	info.worldtag = tag;
	info.posx = pos.x;
	info.posy = pos.y;
	info.posz = pos.z;
	info.trashbox_active = pImp->IsTrashBoxChanged();  //设置是否保存仓库数据
	info.trash_money = pImp->_trashbox.GetMoney();
	info.factionid = pPlayer->id_mafia;
	info.factionrole = pPlayer->rank_mafia;
	
	// Youshuang add
	info.fac_coupon = pImp->GetFactionCoupon();
	info.fac_coupon_add = pImp->GetFactionCouponAdd();
	pImp->SendFactionCouponAdd();
	
	//raw_wrapper newyear_award_info_ar;
	// pImp->SaveNewYearAwardInfo(newyear_award_info_ar);
	// data.newyear_award_info.data = newyear_award_info_ar.data();
	// data.newyear_award_info.size = newyear_award_info_ar.size();
	// end
	
	info.jointime = pImp->GetMafiaJoinTime();
	info.contribution = pImp->GetMafiaContribution();
	info.reputation = pImp->GetReputation();
	info.cur_title = pPlayer->title_id;
	info.used_title = pImp->GetUsedTitle();
	info.pkvalue = pImp->GetRawPKValue();
	info.combatkills = pImp->GetMafiaDuelKills();
	info.devotion = pImp->GetFamilyContribution();
	info.cultivation = pImp->GetCultivation();
	
	info.talismanscore = pImp->GetTalismanValue();
	info.runescore = pImp->GetRuneScore();

	//保存玩家消费值
	info.comsumption = pImp->GetConsumptionValue();

	//计算玩家游戏时间并存盘
	info.time_used = pImp->GetPlayEd();

	//保存玩家点数
	pImp->MallInfo().GetInfo(info.cash,info.cash_used, info.cash_delta, info.cash_serial);
	info.cash_active = pImp->MallInfo().NeedSave();		//察看点卡数据是否改变并且需要存盘
	abase::vector<GDB::shoplog, abase::fast_alloc<> > mall_order_list;
	pImp->MallInfo().GetOrder(mall_order_list);
	pImp->MallInfo().GetWebOrder(data.processed_weborders);

	
	pImp->BonusInfo().GetInfo(info.bonus_withdraw, info.bonus_reward, info.bonus_used);
/*
	if((data.logs.count = mall_order_list.size()))
	{
		data.logs.list = &mall_order_list[0];
	}   //$$$$$$$$$$$$$不存这个
	*/
	
	//设置玩家配方列表
	data.recipes.data = pImp->GetRecipeBuf();
	data.recipes.size = pImp->GetRecipeCount() * sizeof(unsigned short);

	//设置称谓列表
	data.title_list.data = pImp->GetPlayerTitleList(data.title_list.size);
	data.title_list.size *= sizeof(short);

	//设置区域友好度
	data.region_reputation.data = pImp->GetRegionReputationData(data.region_reputation.size);
	data.region_reputation.size *= sizeof(int);

	raw_wrapper treasure_data;
	pImp->GetTreasureInfo(treasure_data);
	data.treasure_info.data = treasure_data.data();
	data.treasure_info.size = treasure_data.size();
	
	raw_wrapper littlepet_data;
	pImp->GetLittlePetInfo(littlepet_data);
	data.littlepet.data = littlepet_data.data();
	data.littlepet.size = littlepet_data.size();

	raw_wrapper multiexp_data;
	pImp->SaveMultiExp(multiexp_data);
	data.multi_exp.data = multiexp_data.data();
	data.multi_exp.size = multiexp_data.size();

	raw_wrapper fuwen_compose_info;
	pImp->SaveFuwenComposeInfo(fuwen_compose_info);
	data.fuwen_info.data = fuwen_compose_info.data();
	data.fuwen_info.size = fuwen_compose_info.size();

	raw_wrapper phase_data;
	pImp->SavePhaseInfo(phase_data);
	data.phase.data = phase_data.data();
	data.phase.size = phase_data.size();

	pImp->GetFlagMask(info.flag_mask);
	
	//设置玩家冷却时间的数据
	pImp->ClearExpiredCoolDown();
	raw_wrapper cooling_data(512);
	pImp->GetCoolDownData(cooling_data);
	data.coolingtime.data = cooling_data.data();
	data.coolingtime.size = cooling_data.size();
		
	//设置包裹栏数据	
	info.bagsize = pImp->GetInventory().Size();
	bool bRst = pImp->GetInventory().MakeDBData(data.inventory);
	ASSERT(bRst);
	bRst = pImp->GetEquipInventory().MakeDBData(data.equipment);
	ASSERT(bRst);
	bRst = pImp->GetTaskInventory().MakeDBData(data.task_inventory);
	ASSERT(bRst);
	bRst = pImp->GetFashionInventory().MakeDBData(data.fashion);
	ASSERT(bRst);
	info.mountwingsize = pImp->GetMountWingInventory().Size();
	bRst = pImp->GetMountWingInventory().MakeDBData(data.mountwing);
	ASSERT(bRst);
	bRst = pImp->GetGiftInventory().MakeDBData(data.gifts);
	ASSERT(bRst);
	bRst = pImp->GetFuwenInventory().MakeDBData(data.fuwen);
	ASSERT(bRst);

	info.storesize = pImp->_trashbox.GetTrashBoxSize();
	info.storesize2 = pImp->_trashbox.GetMafiaTrashBoxSize();
	if(info.trashbox_active)
	{
		bRst = pImp->GetTrashBoxInventory().MakeDBData(data.trash_box);
		ASSERT(bRst);

		bRst = pImp->GetTrashBoxInventoryMafia().MakeDBData(data.trash_box2);
		ASSERT(bRst);
	}
	bRst = pImp->GetPetBedgeInventory().MakeDBData(data.petbadge);
	ASSERT(bRst);
	bRst = pImp->GetPetEquipInventory().MakeDBData(data.petequip);
	ASSERT(bRst);
	data.petdata.data = pImp->GetPetMan().GetData();
	data.petdata.size = pImp->GetPetMan().GetSize();

	info.pocketsize = pImp->GetPocketInventory().Size();
	bRst = pImp->GetPocketInventory().MakeDBData(data.pocket);
	ASSERT(bRst);

	//设置转生数据
	data.reborndata.data = pImp->GetRebornInfo(&data.reborndata.size); 

	raw_wrapper raid_count_ar;
	pImp->SaveRaidCountersDB(raid_count_ar);
	data.raid_data.data = raid_count_ar.data();
	data.raid_data.size = raid_count_ar.size();

	//设置技能数据
	raw_wrapper ar;
	pImp->_skill.StoreDatabase(ar);
	data.skill_data.data = ar.data();
	data.skill_data.size = ar.size();

	//设置组合技能数据
	raw_wrapper ar2;
	pImp->_skill.SaveSkillElems(ar2);
	data.composkills.data = ar2.data();
	data.composkills.size = ar2.size();

	//设置任务数据
	PlayerTaskInterface task_if(pImp);
	task_if.BeforeSaveData();
	data.task_data.data = task_if.GetActiveTaskList();
	data.task_data.size = task_if.GetActLstDataSize();
	data.finished_task_data.data = task_if.GetFinishedTaskList(); 
	data.finished_task_data.size = task_if.GetFnshLstDataSize();
	data.finished_time_task_data.data = task_if.GetFinishedTimeList();
	data.finished_time_task_data.size = task_if.GetFnshTimeLstDataSize();


	//设置filter数据
	raw_wrapper filter_data;
	pImp->_filters.SaveSpecFilters(filter_data,filter::FILTER_MASK_SAVE_DB_DATA); 

	data.filter_data.data = filter_data.data();
	data.filter_data.size = filter_data.size();

	//设置自定义数据
	player_var_data var_data;
	var_data.MakeData(pPlayer,pImp);
	data.var_data.data = &var_data;
	data.var_data.size = sizeof(var_data);
	data.trashbox_passwd.data = (void*) pImp->_trashbox.GetPassword(data.trashbox_passwd.size);

	//设置效果数据 这个东西只写盘不用读 因为实际上是给界面上用的
	data.custom_status.data = pPlayer->effect_list;
	data.custom_status.size = pPlayer->effect_count * sizeof(short);

	//设置玩家角色状态
	raw_wrapper char_data;
	pImp->GetPlayerCharMode(char_data);
	data.charactermode.data = char_data.data();
	data.charactermode.size = char_data.size();

	//保存玩家的所有副本数据
	raw_wrapper ins_data;
	pImp->SaveInstanceKey(ins_data);
	data.instancekeylist.data = ins_data.data();
	data.instancekeylist.size = ins_data.size();

	//保存玩家VIP奖励信息
	raw_wrapper vipaward_data;
	pImp->SaveVipAwardData(vipaward_data);
	data.vipaward.data		  = vipaward_data.data();
	data.vipaward.size		  = vipaward_data.size();

	//保存玩家在线倒计时奖励信息
	raw_wrapper onlineaward_data;
	pImp->SaveOnlineAwardData(onlineaward_data);
	data.onlineaward.data	  = onlineaward_data.data();
	data.onlineaward.size	  = onlineaward_data.size();
	
	//保存玩家成就数据
	data.achieve_map.data = pImp->GetAchievementManager().GetMap().data(data.achieve_map.size);
	raw_wrapper achieve_data;
	pImp->GetAchievementManager().SaveActiveAchievement(achieve_data);
	data.achieve_active.data = achieve_data.data();
	data.achieve_active.size = achieve_data.size();
	data.achieve_spec_info.data = pImp->GetSpecialAchievementInfo(data.achieve_spec_info.size);
	data.achieve_award_map.data = pImp->GetAchievementManager().SaveAchievementAward( data.achieve_award_map.size );  // Youshuang add

	//raw_wrapper five_year_data;
	//pImp->SaveFiveAnniDataDB(five_year_data);
	//data.five_year.data = five_year_data.data();
	//data.five_year.size = five_year_data.size();

	raw_wrapper tower_data;
	pImp->SaveTowerData(tower_data);
	data.tower_raid.data = tower_data.data();
	data.tower_raid.size = tower_data.size();

	raw_wrapper bet_data;
	pImp->SavePlayerBetData(bet_data);
	data.five_year.data = bet_data.data();
	data.five_year.size = bet_data.size();

	raw_wrapper ui_transfer_data;
	pImp->SaveUITransfer(ui_transfer_data);
	data.ui_transfer.data = ui_transfer_data.data();
	data.ui_transfer.size = ui_transfer_data.size();
	
	raw_wrapper astrology_data; 
	pImp->SaveAstrologyData(astrology_data);
	data.astrology_info.data = astrology_data.data();
	data.astrology_info.size = astrology_data.size();
	
	raw_wrapper collision_data; 
	pImp->SaveCollisionData(collision_data);
	data.collision_info.data = collision_data.data();
	data.collision_info.size = collision_data.size();

	raw_wrapper hide_and_seek_data;
	pImp->SaveHideAndSeekData(hide_and_seek_data);
	data.hide_and_seek_info.data = hide_and_seek_data.data();
	data.hide_and_seek_info.size = hide_and_seek_data.size();
	
	raw_wrapper exchange_cs6v6_award_data; 
	pImp->SaveExchange6V6AwardInfo(exchange_cs6v6_award_data);
	data.award_info_6v6.data = exchange_cs6v6_award_data.data();
	data.award_info_6v6.size = exchange_cs6v6_award_data.size();

	raw_wrapper propadd_data;
	pImp->SavePropAdd(propadd_data);
	data.propadd.data = propadd_data.data();
	data.propadd.size = propadd_data.size();

	raw_wrapper liveness_data; 
	pImp->SaveLiveness(liveness_data);
	data.liveness_info.data = liveness_data.data();
	data.liveness_info.size = liveness_data.size();

	//设置台历
	raw_wrapper sale_promotion_data;
	pImp->SaveSalePromotion(sale_promotion_data);
	data.sale_promotion_info.data = sale_promotion_data.data();
	data.sale_promotion_info.size = sale_promotion_data.size();

	info.timestamp = pImp->OI_InceaseDBTimeStamp();

	//保存时装包裹快捷键
	data.fashion_hotkey.data = pImp->GetFashionHotkey(&data.fashion_hotkey.size); 
/*---------------------------发出保存请求------------------------------------*/
	GDB::put_role(gmatrix::GetServerIndex(), info.id,&info,&data,callback,priority,mask);
/*---------------------------发出信息给gdelivery------------------------------------*/
	GMSV::SyncRoleInfo(&info,&data);
/*-------------------------发出保存请求完成----------------------------------*/

	//释放包裹栏分配的内存
	pImp->GetInventory().ReleaseDBData(data.inventory);
	pImp->GetEquipInventory().ReleaseDBData(data.equipment);
	pImp->GetTaskInventory().ReleaseDBData(data.task_inventory);
	pImp->GetFashionInventory().ReleaseDBData(data.fashion);
	pImp->GetMountWingInventory().ReleaseDBData(data.mountwing);
	pImp->GetGiftInventory().ReleaseDBData(data.gifts);
	pImp->GetFuwenInventory().ReleaseDBData(data.fuwen);
	if(info.trashbox_active)
	{
		pImp->GetTrashBoxInventory().ReleaseDBData(data.trash_box);
		pImp->GetTrashBoxInventoryMafia().ReleaseDBData(data.trash_box2);
	}
	pImp->GetPetBedgeInventory().ReleaseDBData(data.petbadge);
	pImp->GetPetEquipInventory().ReleaseDBData(data.petequip);
	pImp->GetPocketInventory().ReleaseDBData(data.pocket);

	//恢复任务数据
	task_if.InitActiveTaskList();

	GLog::log(GLOG_INFO,"save_data:%d发送保存用户%d数据请求 %s包含仓库信息 等级%d 金钱%u",
			pImp->GetWorldTag(),
			pPlayer->ID.id,
			info.trashbox_active?"":"不",
			info.level,
			pImp->GetMoney() + pImp->GetTrashBox().GetMoney());
}


extern int64_t __allow_login_class_mask;
extern int64_t __allow_login_class_mask1;
namespace 
{
class LoginTask :  public abase::ASmallObject , public GDB::Result
{
	gplayer * _player;
	int _uid;
	int _cs_index;
	int _cs_sid;
	void * _auth_data;
	size_t _auth_size;
	bool _use_spec_pos;
	A3DVECTOR _spec_pos;
	int 	  _spec_tag;
	int	  _shutdown_counter;
	int	  _loginip;
	char 	  _trole;
	char	  _flag;
public:
	LoginTask(int counter, gplayer * pPlayer,int uid,const void * auth_data , size_t auth_size, int loginip, bool use_spec_pos, const A3DVECTOR & spec_pos,int spec_tag, char trole, char flag)
		:_player(pPlayer),_uid(uid),_cs_index(pPlayer->cs_index),_cs_sid(pPlayer->cs_sid),
		_auth_data(NULL),_auth_size(auth_size),_use_spec_pos(use_spec_pos), _spec_pos(spec_pos),_spec_tag(spec_tag), _trole(trole),_flag(flag)
		{
			if(auth_size)
			{
				_auth_data = abase::fastalloc(auth_size);
				memcpy(_auth_data,auth_data,auth_size);
			}
			_loginip = loginip;
			_shutdown_counter = counter;
		}
	~LoginTask()
	{
		if(_auth_data)
		{
			abase::fastfree(_auth_data,_auth_size);
		}
	}

	void Failed(int retcode = -1)
	{
		if(retcode) GMSV::SendLoginRe(_cs_index,_uid,_cs_sid,retcode,_flag);	// login failed
		//对申请的player对象进行释放
		spin_autolock alock(_player->spinlock);
		ASSERT(_player->login_state == gplayer::WAITING_LOGIN && _uid == _player->ID.id);
		if(_player->is_waitting_login() && _uid == _player->ID.id) 
		{
			gmatrix::UnmapPlayer(_uid);
			gmatrix::FreePlayer(_player);
		}
		delete this;
	}
public:
	virtual void OnTimeOut()
	{
		GLog::log(GLOG_ERR,"用户%d从数据库取得数据超时",_uid);
		Failed();
	}
	
	virtual void OnFailed()
	{
		GLog::log(GLOG_ERR,"用户%d从数据库取得数据失败",_uid);
		Failed();
	}
	virtual void OnGetRole(int id,const GDB::base_info * pInfo, const GDB::vecdata * data,const GNET::GRoleDetail* pRole);
};

void 
LoginTask::OnGetRole(int id,const GDB::base_info * pInfo, const GDB::vecdata * data, const GNET::GRoleDetail * pRole)
{
	//检测登录控制  只有被允许的职业才能够登入
	if(!object_base_info::CheckCls(pInfo->cls, __allow_login_class_mask, __allow_login_class_mask1))
	{
		GMSV::SendLoginRe(_cs_index,_uid,_cs_sid,1,_flag);	// login failed
		//此时按照错误来处理
		GLog::log(GLOG_ERR,"用户%d由于职业%d被禁止登入，登录失败",id,pInfo->cls & 0x7F);
		Failed(0);
		return;
	}


	if(!do_login_check_data(pInfo,data))
	{
		GMSV::SendLoginRe(_cs_index,_uid,_cs_sid,1,_flag);	// login failed
		//此时按照错误来处理
		GLog::log(GLOG_ERR,"用户%d数据异常，无法登录",id);
		Failed(0);
		return;
	}

	int tag = pInfo->worldtag;
	if(_use_spec_pos) tag = _spec_tag;
	//确定坐标和进行坐标判断
	A3DVECTOR pos(pInfo->posx, pInfo->posy, pInfo->posz);
	if(tag == 0 || tag == 1) 
	{
		tag = 401;
		pos = A3DVECTOR(368, 481, 348);
	}

	//2013年12月10日,数据安全部错误的将gs32添加到了原服的配置里面, 12月24日更新出错, 需要维护将这个图去掉
	//如果不是跨服，且在gs32这个图的玩家强行设置河阳
        if(!gmatrix::IsZoneServer() && tag == 32)
	{
		tag = 401;
		pos = A3DVECTOR(368, 481, 348);
	}

	world_manager * pManager = gmatrix::FindWorld(tag);
	if(pManager == NULL || pManager->IsIdle())
	{
		GLog::log(GLOG_ERR,"用户%d的tag%d无法找到对应的世界",id,tag);
		Failed(-1);
		return ;
	}

	world * pPlane = pManager->GetWorldLogin(id, pInfo, data);
	
	if(pPlane == NULL)
	{
		GLog::log(GLOG_ERR,"用户%d的tag%d无法创建对应的Plane",id,tag);
		Failed(-1);
		return ;
	}


	if(_use_spec_pos) pos = _spec_pos;
	if( !pPlane->PosInWorld(pos))
	{
		GLog::log(GLOG_ERR,"用户%d登录时坐标不满足要求(%f,%f,%f)",id, pos.x,pos.y,pos.z);
		Failed(-1);
		return;
	}

	int rst = pManager->OnPlayerLogin(pInfo, data, _auth_data && _auth_size );
	if(0 != rst)
	{
		//manager不让玩家登录 
		GLog::log(GLOG_INFO,"用户 %d 登录Manager返回失败(errcode: %d)",id,rst);
		//不同的manager有不同的限制
		Failed(-1);
		return;
	}
	//宠物数据检查
	rst = pet_manager::CheckDBData(data->petdata.data,data->petdata.size);
	if(0 != rst)
	{
		//宠物数据错误
		GLog::log(GLOG_ERR,"用户 %d 登录时宠物数据检测出错(errcode: %d)",id,rst);
		Failed(-1);
		return;
	}

	//使用数据库取得的数据
	char name_base64[64] ="未知";
	if(data->user_name.data)
	{
		size_t name_len = data->user_name.size;
		if(name_len > 32) name_len = 32;
		base64_encode((unsigned char*)(data->user_name.data),name_len,name_base64);
	}

	GLog::log(GLOG_INFO,"用户%d从数据库取得数据，职业%d,级别%d 名字'%s'",_uid,pInfo->cls,pInfo->level,name_base64);
	spin_autolock alock(_player->spinlock);

	//delivery是否断过线，所以无法登录
	if(_shutdown_counter != g_d_state.GetShutDownCounter())
	{
		alock.detach();
		GMSV::SendLoginRe(_cs_index,_uid,_cs_sid,7,_flag);	// login failed
		Failed(0);
		return;
	}

	if(!_player->is_waitting_login() || _uid != _player->ID.id) 
	{	
		//已经不是登录状态了 这是一个奇怪的错误
		ASSERT(false);
		Failed(1);
		return;
	}

	if(_player->b_disconnect)
	{
		GMSV::SendDisconnect(_cs_index, _uid, _cs_sid,0);
		gmatrix::UnmapPlayer(_uid);
		gmatrix::FreePlayer(_player);
		delete this;
		return;
	}

	//在这里赋值plane值 在insert的时候赋值是否更好？
	_player->plane = pPlane;

	userlogin_t user;
	memset(&user,0,sizeof(user));
	user._player = _player;
	user._plane = pPlane;
	user._uid = _uid;
	user._auth_data = _auth_data;
	user._auth_size = _auth_size;
	user._origin_tag = pInfo->worldtag;
	user._origin_pos = A3DVECTOR(pInfo->posx, pInfo->posy, pInfo->posz);
	user._loginip = _loginip;

	do_player_login(pos, pInfo,data,user, _trole,_flag);

	pManager->PostPlayerLogin(_player);
	//删除自身
	delete this;
}
}

void	user_login(int cs_index,int cs_sid,int uid,const void * auth_data, size_t auth_size, int loginip, bool use_spec_pos, int ftag, float px,float py,float pz, char trole, char flag)
{
	if(!g_d_state.CanLogin())
	{
		GMSV::SendLoginRe(cs_index,uid,cs_sid,6,flag);	// login failed
		GLog::log(GLOG_WARNING,"用户%d无法登录(%d,%d) 由于delivery尚未准备完成",uid,cs_index,cs_sid);
		return ;
	}

	int rindex;
	if((rindex = gmatrix::FindPlayer(uid)) >=0)
	{
		//发送已经有人登录的消息
		GMSV::SendLoginRe(cs_index,uid,cs_sid,3,flag);	// login failed
		GLog::log(GLOG_WARNING,"用户%d已经登录(%d,%d)(%d)",uid,cs_index,cs_sid,gmatrix::GetPlayerByIndex(rindex)->login_state);
		return ;
	}
	int counter = g_d_state.GetShutDownCounter();
	gplayer *pPlayer = gmatrix::AllocPlayer();
	if(pPlayer == NULL)
	{
		//发送没有物理空间可以容纳Player的信息
		GMSV::SendLoginRe(cs_index,uid,cs_sid,2,flag);	// login failed
		GLog::log(GLOG_WARNING,"用户达到物理限制最大值 uid:%d",uid);
		return;
	}

	GLog::log(GLOG_INFO,"用户%d从%d开始登录,gs_id=%d",uid,cs_index, gmatrix::GetServerIndex());
	pPlayer->cs_sid = cs_sid;
	pPlayer->cs_index = cs_index;
	pPlayer->ID.id = uid;
	pPlayer->ID.type = GM_TYPE_PLAYER; 
	pPlayer->login_state = gplayer::WAITING_LOGIN; 
	pPlayer->pPiece = NULL;
	if(!gmatrix::MapPlayer(uid,gmatrix::GetPlayerIndex(pPlayer)))
	{
		//map player 失败，表示在这一瞬间有人加入
		gmatrix::FreePlayer(pPlayer);
		mutex_spinunlock(&pPlayer->spinlock);
		GMSV::SendLoginRe(cs_index,uid,cs_sid,4,flag);	// login failed
		return;
	}

	ASSERT(pPlayer->imp == NULL);
	pPlayer->imp = NULL; 
	mutex_spinunlock(&pPlayer->spinlock);

	GDB::get_role(gmatrix::GetServerIndex(), uid, new LoginTask(counter,pPlayer,uid,auth_data,auth_size, loginip, use_spec_pos,A3DVECTOR(px,py,pz),ftag, trole,flag));
	return ;
}

bool 	do_login_check_data(const GDB::base_info * pInfo, const GDB::vecdata * data)
{
	//本来是检查用户数据是否合法，目前无需检测
	return true;
}

void
delivery_state::OnConnect()
{
	spin_autolock keeper(_lock);
	if(STATE_NO_CONNECTION == _state)
	{
		_state = STATE_OK;
		_can_login = true;
	}
	_connected = true;
}

void
delivery_state::OnDisconnect()
{
	spin_autolock keeper(_lock);
	if(STATE_OK == _state)
	{
		_state = STATE_DISCONNECT_WAITING;
		IncShutDownCounter();
		ONET::Thread::Pool::AddTask(new WriteTask(this));
	}
	_can_login = false;
	_connected = false;
}

bool delivery_state::ReadyWriteBack()
{
	spin_autolock keeper(_lock);
	if(STATE_DISCONNECT_WAITING == _state)
	{
		_state = STATE_DISCONNECT_WRITING;
		return true;
	}
	return false;
}

void
delivery_state::WriteBackDone()
{
	spin_autolock keeper(_lock);
	if(STATE_DISCONNECT_WRITING == _state)
	{
		_state = STATE_NO_CONNECTION;
		if(_connected)
		{
			_state = STATE_OK;
			_can_login = true;
		}
	}
}

void 
delivery_state::WriteTask::Run()
{
	if(_state->ReadyWriteBack())
	{
		_state->IncShutDownCounter();
		gmatrix::Instance()->WriteAllUserBack();
		_state->IncShutDownCounter();
		sleep(15);	//等待15秒 反正这里是 什么都做不了 这里是线程任务，同时没有锁定任何东西，不用担心死锁或其他
		_state->WriteBackDone();
	}
	delete this;
}

