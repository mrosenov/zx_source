#include "taskman.h"
#include "../world.h"
#include "../player_imp.h"
#include "../template/itemdataman.h"
#include <arandomgen.h>
#include <gsp_if.h>
#include "../antiwallow.h"
#include <openssl/md5.h>
#include "../general_indexer.h"
#include "../playertransform.h"
#include <map>
#include "TaskTemplMan.h"

class TeamTask
{
	struct TeamTaskData
	{
		int teamid;
		int taskid;
		TeamTaskData(int team_id, int task_id) : teamid(team_id), taskid(task_id){}
	};

	struct TeamTaskCmp
	{
		bool operator()(const TeamTaskData & task1, const TeamTaskData & task2)
		{
			return task1.teamid != task2.teamid ? task1.teamid < task2.teamid : task1.taskid < task2.taskid;
		}
	};

	static int _teamtask_lock; 
	static std::map<TeamTaskData, int, TeamTaskCmp> _teamtask;
public:

	static void AddTeamTask(int teamid, int taskid);
	static void DelTeamTask(int teamid, int taskid);
	static int GetTeamTaskCount(int teamid, int taskid);

};

std::map<TeamTask::TeamTaskData, int, TeamTask::TeamTaskCmp> TeamTask::_teamtask;
int TeamTask::_teamtask_lock = 0;

static int quest_timer = 0;

static void QuestTimerRoutine(int index,void *object,int remain)
{
	quest_timer ++;
	if(quest_timer > 12)
	{
		OnTaskCheckAllTimeLimits(g_timer.get_systime());
		quest_timer = 0; 
	}
}

bool InitQuestSystem(const char * filename,const char * filename2,const char * filename3, const char * filename4, elementdataman * pMan)
{
	bool bRst = LoadTasksFromPack(pMan,filename,filename2, filename3, filename4);
	if(!bRst) return false;
	g_timer.set_timer(5*20,10,0,QuestTimerRoutine,NULL);
	return true;
}



unsigned long 
PlayerTaskInterface::GetPlayerLevel()
{
	return _imp->_basic.level;
}

void* 
PlayerTaskInterface::GetActiveTaskList()
{
	return _imp->_active_task_list.begin();
}

void* 
PlayerTaskInterface::GetFinishedTaskList()
{
	return _imp->_finished_task_list.begin();
}

void* 
PlayerTaskInterface::GetFinishedTimeList()
{
	return _imp->_finished_time_task_list.begin();
}

unsigned long* 
PlayerTaskInterface::GetTaskMask()
{
	return &_imp->_task_mask;
}

void 
PlayerTaskInterface::DeliverGold(unsigned long ulGoldNum,unsigned long ulPlayerID)
{
	int gold = ulGoldNum;
	if(gmatrix::AntiWallow())
	{
		anti_wallow::AdjustTaskMoney(_imp->GetWallowLevel(), gold);
	}
	if(ulPlayerID == 0)
	{
		 _imp->GainMoneyWithDrop(gold);
		 if (gold)
			 GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=1:reason=2:hint=0",_imp->_parent->ID.id,_imp->GetUserID(),gold);
		 _imp->_runner->task_deliver_money((int)gold,_imp->GetMoney());

	}
	else
	{
		msg_task_transfor_award award;
		award.SetGold(gold);
		_imp->SendTo<0>(GM_MSG_TASK_AWARD_TRANSFOR, XID(GM_TYPE_PLAYER, ulPlayerID), 0, &award, sizeof(award));
	}
}

void 
PlayerTaskInterface::DeliverExperience(int64_t ulExp,unsigned long ulPlayerID)
{
	int64_t exp = ulExp; 
	if(gmatrix::AntiWallow())
	{
		anti_wallow::AdjustTaskExp(_imp->GetWallowLevel(), exp);
	}
	if(ulPlayerID == 0)
	{
		_imp->ReceiveTaskExp(exp);
	}
	else
	{
		msg_task_transfor_award award;
		award.SetExp(exp);
		_imp->SendTo<0>(GM_MSG_TASK_AWARD_TRANSFOR, XID(GM_TYPE_PLAYER, ulPlayerID), 0, &award, sizeof(award));
	}
}

void 
PlayerTaskInterface::DeliverSP(unsigned long ulSP,unsigned long ulPlayerID /*$$$$$$$*/)
{
//	_imp->ReceiveTaskExp(0);
}

void 
PlayerTaskInterface::DeliverReputation(long lReputation,unsigned long ulPlayerID)
{
	if(ulPlayerID == 0)
	{
		_imp->ModifyReputation(lReputation);
	}
	else
	{
		msg_task_transfor_award award;
		award.SetReputation(lReputation);
		_imp->SendTo<0>(GM_MSG_TASK_AWARD_TRANSFOR, XID(GM_TYPE_PLAYER, ulPlayerID), 0, &award, sizeof(award));
	}
}

void 
PlayerTaskInterface::DeliverRegionReputation(int index, long lValue, unsigned long ulPlayerID)
{
	if(ulPlayerID == 0)
	{
		_imp->ModifyRegionReputation(index, lValue);
	}
	else
	{
		msg_task_transfor_award award;
		award.SetRegionReputation(index, lValue);
		_imp->SendTo<0>(GM_MSG_TASK_AWARD_TRANSFOR, XID(GM_TYPE_PLAYER, ulPlayerID), 0, &award, sizeof(award));
	}
}


int 
PlayerTaskInterface::GetTaskItemCount(unsigned long ulTaskItem)
{
	item_list & inv = _imp->GetTaskInventory();
	int rst;
	if((rst = inv.Find(0,ulTaskItem)) >=0)
	{
		//是不是应该累计所有位置的数目？
		return inv[rst].count;
	}
	return 0;
}

int 
PlayerTaskInterface::GetCommonItemCount(unsigned long ulCommonItem)
{
	item_list & inv = _imp->GetInventory();
	int rst = 0;
	int count = 0;
	while((rst = inv.Find(rst,ulCommonItem)) >=0)
	{
		//是不是应该累计所有位置的数目？
		count += inv[rst].count;
		rst ++;
	}
	return count;
}

	
bool 
PlayerTaskInterface::IsInFaction()
{
	return _imp->OI_IsMafiaMember();
}

unsigned long 
PlayerTaskInterface::GetGoldNum()
{
	return _imp->GetMoney();
}

void 
PlayerTaskInterface::TakeAwayGold(unsigned long ulNum)
{
	if (ulNum)
		GLog::money("money_change:[roleid=%d,userid=%d]:moneychange=%d:type=2:reason=2:hint=0",_imp->_parent->ID.id,_imp->GetUserID(),ulNum);
	_imp->SpendMoney(ulNum);
	_imp->_runner->spend_money(ulNum);
}

void 
PlayerTaskInterface::TakeAwayTaskItem(unsigned long ulTemplId, unsigned long ulNum)
{
	item_list &inv = _imp->GetTaskInventory();
	int rst = inv.Find(0,ulTemplId);
	if(rst >= 0 && inv[rst].count >= ulNum)
	{
		inv.DecAmount(rst,ulNum);
		//$$$$发出消息
		_imp->_runner->player_drop_item(gplayer_imp::IL_TASK_INVENTORY,rst,ulTemplId,ulNum,S2C::DROP_TYPE_TASK);
	}
}

void 
PlayerTaskInterface::TakeAwayCommonItem(unsigned long ulTemplId, unsigned long ulNum)
{
	item_list &inv = _imp->GetInventory();
	int rst = 0;
	size_t num = ulNum;
	while(num && (rst = inv.Find(rst,ulTemplId)) >= 0)
	{
		size_t count = num;
		if(inv[rst].count < count) count = inv[rst].count;
		//检查是否需要记录消费值
		_imp->CheckSpecialConsumption(inv[rst].type, count);

		_imp->UseItemLog(inv, rst, count);
		inv.DecAmount(rst,count);
		_imp->_runner->player_drop_item(gplayer_imp::IL_INVENTORY,rst,ulTemplId,count,S2C::DROP_TYPE_TASK);
		num -= count;
		rst ++;
	}
}

void
PlayerTaskInterface::TakeAwayReinforceItem(unsigned long ulTemplId, unsigned long ulItemLevel, unsigned char cond_type)
{
	if(ulItemLevel <= 0) return; 

	item_list &inv = _imp->GetInventory();
	int index = -1;
	int level = -1;
	int reinforce_level = 0;
	for(size_t i = 0; i < inv.Size(); ++i)
	{
		if(inv[i].type == (int)ulTemplId && inv[i].GetItemType() == item_body::ITEM_TYPE_EQUIPMENT)
		{
			reinforce_level = inv[i].GetReinforceLevel();
			if(cond_type == 0 && reinforce_level >= (int)ulItemLevel)
			{
				if(reinforce_level < level || level == -1)
				{
					level = reinforce_level;
					index = i;
				}
			}
			else if(cond_type == 1 && reinforce_level <= (int)ulItemLevel)
			{
				if(reinforce_level < level || level == -1)
				{
					level = reinforce_level;
					index = i;
				}
			}
			else if(cond_type == 2 && reinforce_level == (int)ulItemLevel) 
			{
				index = i;
				break;
			}
		}	
	}
	if(index >= 0) inv.Remove(index); 
}

unsigned long 
TaskInterface::GetCurTime()
{
	return g_timer.get_systime();
}

void
TaskInterface::WriteLog(int nPlayerId, int nTaskId, int nType, const char* szLog)
{
	return GLog::tasklog(nPlayerId,nTaskId,nType,szLog);
}

void
TaskInterface::WriteKeyLog(int nPlayerId, int nTaskId, int nType, const char* szLog)
{
	return GLog::task(nPlayerId,nTaskId,nType,szLog);
}

void TaskInterface::WriteTaskAwardLog(int nPlayerID, int nTaskID, unsigned long ulExp, unsigned long ulGold, const abase::vector<Task_Log_AwardRegionRepu>& RegionRepuArr, const abase::vector<Task_Log_AwardItem>& ItemArr)
{
	int mc = RegionRepuArr.size();
	if ((size_t)mc < ItemArr.size()) mc = ItemArr.size();
	for(int i = 0; i <  mc; i ++)
	{
		int rep_idx = -1, rep_val = 0;
		int item_id = 0;
		int item_count = 0;
		if((unsigned long)i < RegionRepuArr.size())
		{
			rep_idx = RegionRepuArr[i].lRegionIndex;
			rep_val = RegionRepuArr[i].lRegionReputation;
		}
		if((unsigned long)i < ItemArr.size())
		{
			item_id = ItemArr[i].ulItemID;
			item_count = ItemArr[i].ulItemCnt;
		}

		GLog::action("taskdeli,rid=%d:tid=%d:exp=%lu:itemid=%d:itemcount=%d:gold=%lu:reputation_index=%d:reputation_value=%d",  nPlayerID, nTaskID,ulExp, item_id, item_count,ulGold, rep_idx,rep_val );
		ulExp = 0;
		ulGold = 0;
	}
}

void 
TaskInterface::WriteTaskDeliverLog(int nPlayerID, int nTaskID)
{
	GLog::action("taskacc,rid=%d:tid=%d",  nPlayerID, nTaskID);
}


long 
PlayerTaskInterface::GetReputation()
{
	return _imp->GetReputation();
}

long 
PlayerTaskInterface::GetRegionReputation(int index)
{
	return _imp->GetRegionReputation(index);
}

unsigned long 
PlayerTaskInterface::GetCurPeriod()
{
	return _imp->_basic.sec_level;
}

void 
PlayerTaskInterface::SetCurPeriod(unsigned long per)
{
	_imp->SetSecLevel(per);
}

unsigned long 
PlayerTaskInterface::GetPlayerId()
{
	return _imp->_parent->ID.id;
}

unsigned long 
PlayerTaskInterface::GetPlayerRace()
{
	return _imp->GetPlayerClass();
}

unsigned long 
PlayerTaskInterface::GetPlayerOccupation()
{
	return _imp->GetPlayerClass();
}

bool 
PlayerTaskInterface::CanDeliverCommonItem(unsigned long ulItemTypes)
{
	return _imp->GetInventory().GetEmptySlotCount() >= ulItemTypes;
}

void 
PlayerTaskInterface::DeliverCommonItem(unsigned long ulItem,unsigned long count, bool bind, long lPeriod,unsigned long ulPlayerID)
{
	element_data::item_tag_t tag = {element_data::IMT_CREATE,0}; 
	if(ulPlayerID == 0)
	{
		while(count > 0)
		{
			item_data * pData = gmatrix::GetDataMan().generate_item(ulItem,&tag,sizeof(tag));
			if(pData == NULL) return;

			if(count > pData->pile_limit)
			{
				pData->count = pData->pile_limit;
			}
			else
			{
				pData->count = count;
			}
			count -= pData->count;

			if(bind)
			{
				item::Bind(pData->proc_type);
			}			

			if(pData->pile_limit == 1)
			{	
				pData->expire_date = (lPeriod <=0?0:lPeriod + g_timer.get_systime());
			}			 

			//返回false表明没有全部放入
			if(_imp->ObtainItem(gplayer_imp::IL_INVENTORY,pData,true, ITEM_INIT_TYPE_TASK)) FreeItem(pData);
		}
	}
	else
	{
		msg_task_transfor_award award;
		award.SetCommonItem(ulItem, count, bind, lPeriod);
		_imp->SendTo<0>(GM_MSG_TASK_AWARD_TRANSFOR, XID(GM_TYPE_PLAYER, ulPlayerID), 0, &award, sizeof(award));
	}
}

void
PlayerTaskInterface::DeliverReinforceItem(unsigned long ulItem, unsigned long level, bool bind, long lPeriod)
{
	_imp->DeliverReinforceItem(ulItem, level, bind, lPeriod, ITEM_INIT_TYPE_TASK);
}

bool 
PlayerTaskInterface::CanDeliverTaskItem(unsigned long ulItemTypes)
{
	return _imp->GetTaskInventory().GetEmptySlotCount() >= ulItemTypes;
}

int
PlayerTaskInterface::GetItemSlotCnt(unsigned long ulItemTemplID, unsigned long ulItemNum)
{
	size_t pile_limit = (size_t)gmatrix::GetDataMan().get_item_pile_limit(ulItemTemplID);
	if(pile_limit <= 0) return 0;
	
	size_t need_slot_count = ulItemNum / pile_limit + ((ulItemNum % pile_limit == 0) ? 0 : 1); 
	return need_slot_count;
}

void 
PlayerTaskInterface::DeliverTaskItem(unsigned long ulItem,unsigned long count,unsigned long ulPlayerID)
{
	element_data::item_tag_t tag = {element_data::IMT_CREATE,0}; 
	if(ulPlayerID == 0)
	{
		while(count > 0)
		{
			item_data * pData = gmatrix::GetDataMan().generate_item(ulItem,&tag,sizeof(tag));
			if(pData == NULL) return;

			if(count > pData->pile_limit)
			{
				pData->count = pData->pile_limit;
			}
			else
			{
				pData->count = count;
			}
			count -= pData->count;

			//返回false表明没有全部放入
			if(_imp->ObtainItem(gplayer_imp::IL_TASK_INVENTORY,pData,true)) FreeItem(pData);
		}
	}
	else
	{
		msg_task_transfor_award award;
		award.SetTaskItem(ulItem, count);
		_imp->SendTo<0>(GM_MSG_TASK_AWARD_TRANSFOR, XID(GM_TYPE_PLAYER, ulPlayerID), 0, &award, sizeof(award));
	}
}

bool PlayerTaskInterface::IsInFamily()
{
	return _imp->OI_GetFamilyID() != 0;
}

int 
PlayerTaskInterface::GetFamilyID()
{
	return _imp->OI_GetFamilyID();
}

void 
PlayerTaskInterface::QueryFamilyData(const void*buf , int size)
{
	GMSV::SendTaskAsyncData(_imp->_parent->ID.id, buf, size);
}

int PlayerTaskInterface::GetFamilyContribution()
{
	return _imp->GetFamilyContribution();
}

void PlayerTaskInterface::DeliverFamilyContribution(int lVal)
{
	_imp->ModifyFamilyContribution(lVal);
}


int PlayerTaskInterface::GetFamilySkillLevel(int nIndex)
{
	return 0;
}

int PlayerTaskInterface::GetFamilySkillProficiency(int nIndex)
{
	return 0;
}

void PlayerTaskInterface::DeliverFamilySkillProficiency(int nIndex, int nValue)
{
}

void PlayerTaskInterface::UpgradeFamilySkill(int nIndex, int nNewLevel)
{
}


void 
PlayerTaskInterface::NotifyClient(const void* pBuf, size_t sz)
{
	_imp->_runner->send_task_var_data(pBuf,sz);
}

float 
PlayerTaskInterface::UnitRand()
{
	return abase::Rand(0.f,1.f);
}

int 
PlayerTaskInterface::GetTeamMemberNum()
{
	return _imp->GetTeamMemberNum();
}

void 
PlayerTaskInterface::NotifyPlayer(unsigned long ulPlayerId, const void* pBuf, size_t sz)
{
	XID id(GM_TYPE_PLAYER,ulPlayerId);
	_imp->SendTo<0>(GM_MSG_PLAYER_TASK_TRANSFER,id,0,pBuf,sz);
}

void 
PlayerTaskInterface::GetTeamMemberInfo(int nIndex, task_team_member_info* pInfo)
{
	const player_team::member_entry &ent = _imp->GetTeamMember(nIndex);
	pInfo->m_ulId = ent.id.id;
	pInfo->m_ulLevel = ent.data.level;
	pInfo->m_ulOccupation = ent.cls;
	pInfo->m_bMale = (ent.gender == gactive_object::MALE);
	pInfo->m_Pos[0] = ent.pos.x;
	pInfo->m_Pos[1] = ent.pos.y;
	pInfo->m_Pos[2] = ent.pos.z;
	pInfo->m_ulWorldId = ent.data.world_tag;
	pInfo->m_nFamilyId = ent.data.family_id;
	pInfo->m_ulMasterID = ent.data.sect_master_id;
	__PRINTF("MemberInfo index:%d id:%d pos:(%f,%f,%f)\n",nIndex,ent.id.id,ent.pos.x,ent.pos.y,ent.pos.z);
}

unsigned long  
PlayerTaskInterface::GetTeamMemberPos(int nIndex, float pos[3])
{
	const player_team::member_entry &ent = _imp->GetTeamMember(nIndex);
	pos[0] = ent.pos.x;
	pos[1] = ent.pos.y;
	pos[2] = ent.pos.z;
	return ent.data.world_tag;
}


bool PlayerTaskInterface::IsDeliverLegal()
{
	return _imp->IsDeliverLegal();
}

bool PlayerTaskInterface::IsCaptain()
{
	return _imp->IsTeamLeader();
}

bool PlayerTaskInterface::IsInTeam()
{
	return _imp->IsInTeam();
}


unsigned long PlayerTaskInterface::GetTeamMemberId(int nIndex)
{
	const player_team::member_entry &ent = _imp->GetTeamMember(nIndex);
	return ent.id.id;
}

bool PlayerTaskInterface::IsMale()
{
	return !_imp->IsPlayerFemale();

}

unsigned long 
PlayerTaskInterface::GetPos(float pos[3])
{
	const A3DVECTOR & selfpos = _imp->_parent->pos;
	pos[0] = selfpos.x;
	pos[1] = selfpos.y;
	pos[2] = selfpos.z;
	/*liuyue-facbase
	if(_imp->GetWorldManager()->IsRaidWorld())
	{
		return _imp->GetWorldManager()->GetRaidID();
	}
	return _imp->GetWorldManager()->GetWorldTag();
	*/
	return _imp->GetClientTag();
}

bool PlayerTaskInterface::HasLivingSkill(unsigned long ulSkill)
{
	return _imp->GetSkillLevel(ulSkill) > 0;
	
}

int PlayerTaskInterface::GetPetCon()
{
	return _imp->GetPetConstruction();
}

int PlayerTaskInterface::GetPetCiv()
{
	return _imp->GetPetCivilization();
}

unsigned long PlayerTaskInterface::GetPlayerLifeAgainCnt()
{
	return _imp->GetRebornCount();
}

void PlayerTaskInterface::SetLifeAgainOccup(unsigned long ulLifeAgainFaction)
{
	_imp->Metempsychosis(ulLifeAgainFaction);
}

long PlayerTaskInterface::GetLivingSkillProficiency(unsigned long ulSkill)
{
	return -1;	
}


void PlayerTaskInterface::SetNewRelayStation(unsigned long ulStationId)
{
	return ;
}

void PlayerTaskInterface::SetStorehouseSize(unsigned long ulSize)
{
	((gplayer_dispatcher*)_imp->_runner)->trashbox_capacity_notify(ulSize);
	return _imp->_trashbox.SetTrashBoxSize(ulSize);
}

void PlayerTaskInterface::SetFactionStorehouseSize(unsigned long ulSize)
{
	((gplayer_dispatcher*)_imp->_runner)->mafia_trashbox_capacity_notify(ulSize);
	return _imp->_trashbox.SetMafiaTrashBoxSize(ulSize);
}

unsigned long PlayerTaskInterface::GetPlayerLifeAgainOccup(int nLifeAgainIndex)
{
	return _imp->GetRebornProf(nLifeAgainIndex);
}

void PlayerTaskInterface::SetFuryUpperLimit(unsigned long ulValue)
{
}

void PlayerTaskInterface::SetDoubleExpTime(unsigned long t)
{
	_imp->ActiveMultiExpTime(2, t);
}

void PlayerTaskInterface::TransportTo(unsigned long ulWorldId, const float pos[3],long)
{
	/*
	if(_imp->GetWorldManager()->IsRaidWorld() && ulWorldId == _imp->GetWorldManager()->GetRaidID())
	{
		_imp->LongJump(A3DVECTOR(pos[0],pos[1],pos[2]));
		return;
	}
	_imp->LongJump(A3DVECTOR(pos[0],pos[1],pos[2]),ulWorldId);
	*/
	//liuyue-facbase 
	if((_imp->GetWorldManager()->IsRaidWorld()||_imp->GetWorldManager()->IsFacBase()) && ulWorldId == _imp->GetClientTag())
	{
		_imp->LongJump(A3DVECTOR(pos[0],pos[1],pos[2]));
		return;
	}
	_imp->LongJump(A3DVECTOR(pos[0],pos[1],pos[2]),ulWorldId);
}

void PlayerTaskInterface::GetSpecailAwardInfo(special_award* p)
{
	_imp->GetSpecailTaskAward(p->id, p->param);
}

void PlayerTaskInterface::SetPetInventorySize(unsigned long ulSize)
{
	_imp->SetPetBedgeInventorySize(ulSize);
	_imp->_runner->player_pet_room_capacity(ulSize);
}

int PlayerTaskInterface::GetProduceSkillLev()
{
	return _imp->GetProduceLevel();
}

void PlayerTaskInterface::SetProduceSkillFirstLev()
{
	if(_imp->GetProduceLevel() == 0)
	{
		_imp->SetProduceSkill(1,0);
		_imp->_runner->player_produce_skill_info(1,0);
	}
}

int PlayerTaskInterface::GetProduceSkillExp()
{
	return _imp->GetProduceExp();
}

void PlayerTaskInterface::AddProduceSkillExp(int nExp)
{
	if(_imp->GetProduceLevel() > 0  && nExp > 0)
	{
		_imp->IncProduceExp(nExp);
	}
}

void PlayerTaskInterface::SetNewProfession(unsigned long ulProfession)
{
	_imp->ChangeClass((int)ulProfession);
}

void PlayerTaskInterface::GiveOneTitle(long lTitle, unsigned long ulPlayerID)
{
	if(ulPlayerID == 0)
	{
		if(_imp->InsertPlayerTitle(lTitle))
		{
			_imp->_runner->player_add_title(lTitle);
		}
	}
	else
	{
		msg_task_transfor_award award;
		award.SetTitle(lTitle);
		_imp->SendTo<0>(GM_MSG_TASK_AWARD_TRANSFOR, XID(GM_TYPE_PLAYER, ulPlayerID), 0, &award, sizeof(award));
	}
}

void PlayerTaskInterface::TakeAwayTitle(short title)
{
	_imp->RemovePlayerTitle(title);
	_imp->_runner->player_del_title(title);
}

void PlayerTaskInterface::SetInventorySize(long lSize)
{
	_imp->ChangeInventorySize(lSize);
}

void PlayerTaskInterface::SetMountInventorySize(unsigned long ulSize)
{
	_imp->ChangeMountWingInventorySize(ulSize);
}

bool PlayerTaskInterface::IsGM()
{
	return _imp->CheckGMPrivilege();
}

bool PlayerTaskInterface::HasTitle(short title)
{
	return _imp->CheckPlayerTitle(title);
}

long 
PlayerTaskInterface::GetPKValue()
{
	return _imp->GetPKValue();
}

void PlayerTaskInterface::DeliverPKValue(long lVal)
{
	_imp->ResetPKValue(abs(lVal), 0);
}

void PlayerTaskInterface::ResetPKValue()
{
	_imp->ResetPKValue(abs(_imp->GetPKValue()), 0);
}

long PlayerTaskInterface::GetFactionContribution()
{
	return _imp->GetMafiaContribution();
}

void PlayerTaskInterface::DeliverFactionContribution(long lVal)
{
	_imp->ModifyMafiaContribution(lVal);
}

bool PlayerTaskInterface::IsMarried()
{
	return _imp->IsMarried();
}

void PlayerTaskInterface::CheckTeamRelationship(int nReason)
{
	return _imp->DoTeamRelationTask(nReason);
}

int
PlayerTaskInterface::GetFactionLev()
{
	int id = _imp->OI_GetMafiaID();
	if(id <= 0) return -1;
	int rst = GMSV::GetFactionLevel(id);
	return rst;
}

int
PlayerTaskInterface::GetFactionRole()
{
	return _imp->GetParent()->rank_mafia;
}


void 
PlayerTaskInterface::Divorce()
{
	if(!_imp->IsMarried()) return;
	GMSV::SetCouple(_imp->_parent->ID.id, _imp->GetSpouse(),0);
	_imp->SetSpouse(0);
	_imp->_skill.ClearSpouseSkill(object_interface(_imp));
	_imp->_runner->player_change_spouse(_imp->GetSpouse());
	_imp->_runner->get_skill_data();
	return ;
}

void 
PlayerTaskInterface::DeliverBuff(long skill, long level)
{
	SKILL::Data data(skill);
	_imp->_skill.CastRune(data,object_interface(_imp), level,  0);
}

int 
PlayerTaskInterface::GetTalismanValue()
{
	return _imp->GetTalismanValue();
}

void 
PlayerTaskInterface::SendMessage(unsigned long task_id, int channel, int param, long var[3])
{
	_imp->TaskSendMessage(task_id, channel, param, var);
}

// 战场相关
int PlayerTaskInterface::GetBattleScore()
{
	return _imp->GetBattleScore();
}

void PlayerTaskInterface::DeliverBattleScore( int score )
{
	if( score > 0 )
	{
		_imp->GainBattleScore( score );
		_imp->_runner->gain_battle_score( score );
	}
	else if( score < 0 )
	{
		_imp->SpendBattleScore( -score, true );
		_imp->_runner->spend_battle_score(-score );
	}
	// score = 0 啥也不用做
}

int PlayerTaskInterface::GetSJBattleScore()
{
	return _imp->GetKillingfieldScore();
}

void PlayerTaskInterface::DeliverSJBattleScore(int nValue)
{
	_imp->ChangeKillingfieldScore(nValue);
}

void PlayerTaskInterface::ResetSJBattleScore()
{
	_imp->ResetKillingfieldScore();
}

// 更新任务全局数据
void TaskUpdateGlobalData( unsigned long ulTaskId, const unsigned char pData[TASK_GLOBAL_DATA_SIZE])
{
	GMSV::SetTaskData((int)ulTaskId,pData,TASK_GLOBAL_DATA_SIZE);
}

void TaskQueryGlobalData( unsigned long ulTaskId, unsigned long ulPlayerId, const void* pPreservedData, size_t size)
{
	GMSV::GetTaskData((int)ulTaskId, (int)ulPlayerId, pPreservedData,size);
}

void 
PlayerTaskTeamInterface::SetMarriage(int nPlayer)
{
	marriage_op = 1;
	if(couple[0] == 0)
	{
		couple[0] = nPlayer;
	}
	else if(couple[1] == 0)
	{
		couple[1] = nPlayer;
	}
}


void PlayerTaskTeamInterface::Execute(gplayer ** list, size_t count)
{
//$$$$$$$$$$日志
	if(count != 2) return;
	gplayer_imp * pImp1 = (gplayer_imp*)list[0]->imp;
	gplayer_imp * pImp2 = (gplayer_imp*)list[1]->imp;
	if((list[0]->ID.id == couple[0] && list[1]->ID.id == couple[1]) ||
			(list[1]->ID.id == couple[0] && list[0]->ID.id == couple[1]))
	{
		//发送消息到数据库和delivery
		GMSV::SetCouple(couple[0],couple[1], 1);
		GDB::set_couple(couple[0],couple[1], 1);
		if(list[0]->ID.id == couple[0])
		{
			pImp1->SetSpouse(couple[1]);
			pImp2->SetSpouse(couple[0]);
		}
		else
		{
			pImp1->SetSpouse(couple[0]);
			pImp2->SetSpouse(couple[1]);
		}
		pImp1->_runner->player_change_spouse(pImp1->GetSpouse());
		pImp2->_runner->player_change_spouse(pImp2->GetSpouse());

		//这里能够存盘的原因是前面检查过了，一定处于normal状态
		ASSERT(pImp1->IsDeliverLegal());
		ASSERT(pImp2->IsDeliverLegal());
		pImp1->ExternSaveDB();
		pImp2->ExternSaveDB();
	}
}

void PlayerTaskInterface::SetMonsterController(long id, bool bTrigger)
{       
	if(id <= 0) return;
	world_manager::ActiveSpawn(_imp->GetWorldManager(), id, bTrigger);
}

bool
TaskInterface::CalcFileMD5(const char* szFile, unsigned char md5[16])
{
	FILE * file = fopen(szFile,"rb");
	if(file == NULL) return false;

	MD5_CTX ctx;
	MD5_Init(&ctx);
	do
	{
		char buf[1024];
		int rst = fread(buf, 1, sizeof(buf), file);
		if(rst <= 0) break;
		MD5_Update(&ctx, buf, rst);
	}while(1);

	MD5_Final(md5, &ctx);
	fclose(file);
	return true;
}


bool
PlayerTaskInterface::IsMaster()
{
	int id = _imp->GetSectID();
	return id && id == _imp->_parent->ID.id;
}


bool
PlayerTaskInterface::IsPrentice()
{
	int id = _imp->GetSectID();
	return id && id != _imp->_parent->ID.id;
}

void PlayerTaskInterface::DeviateMaster()
{
	int id = _imp->GetSectID();
	if(id) GMSV::QuitSect( id, 0, _imp->_parent->ID.id);
}

unsigned long 
PlayerTaskInterface::GetCultivation()
{
	return _imp->_cultivation;
}

void 
PlayerTaskInterface::SetCultivation(unsigned long ulCult)
{
	_imp->SetCultivation(ulCult);
	_imp->_runner->cultivation_notify(ulCult & 0xFF);
}

void 
PlayerTaskInterface::ClearCultSkill(unsigned long ulCult)
{
	_imp->_skill.Forget(_imp, ulCult);
	_imp->_runner->get_skill_data();
}

void PlayerTaskInterface::ClearNormalSkill()
{
	_imp->_skill.Forget(true, _imp);
	_imp->_runner->get_skill_data();
}

void PlayerTaskInterface::ClearTalentSkill()
{
	int rst = _imp->_skill.ForgetTalent(_imp);
	if(rst > 0)
	{
		_imp->ModifyTalentPoint(rst);
	}
	_imp->_runner->get_skill_data();
}

unsigned long PlayerTaskInterface::GetInMasterLev()
{
	return _imp->GetSectInitLevel();
}

void 
PlayerTaskInterface::DeliverFamilyExperience(int nFamilyID, int64_t ulExp)
{
	if(nFamilyID && nFamilyID == _imp->OI_GetFamilyID())
	{
		GMSV::SendFamilyExpShare(_imp->_parent->ID.id,  nFamilyID, _imp->GetObjectLevel() , ulExp);
	}
}

void PlayerTaskInterface::OutMaster()
{
	int id = _imp->GetSectID();
	if(id) GMSV::QuitSect( id, 1, _imp->_parent->ID.id);
}

int PlayerTaskInterface::GetTreasureConsumed()
{
	return _imp->MallInfo().GetCashUsed();
}

unsigned long PlayerTaskInterface::GetAchievePoint()
{
	return _imp->GetAchievePoint();
}

int PlayerTaskInterface::GetTotalCaseAdd()
{
	return _imp->MallInfo().GetCashAdd();
}

bool PlayerTaskInterface::ClearSomeIllegalStates()
{
	return _imp->TaskLeaveAbnormalState();
}


long PlayerTaskInterface::GetGlobalValue(long lKey)
{
	return _imp->OI_GetGlobalValue(lKey);
}

void PlayerTaskInterface::PutGlobalValue(long lKey, long lValue)
{
	_imp->OI_PutGlobalValue(lKey, lValue);
}

void PlayerTaskInterface::ModifyGlobalValue(long lKey, long lValue)
{
	_imp->OI_ModifyGlobalValue(lKey, lValue);
}


bool PlayerTaskInterface::IsMaxInvSize()
{
	return _imp->IsMaxInvSize();
}
void PlayerTaskInterface::SetPocketSize(long lSize)
{
	_imp->ChangePocketSize(lSize);
}


void PlayerTaskInterface::DeliverBonus(int inc)
{
	_imp->OI_AddBonus(inc);
	//-2表示任务发放的红利，由于player_bonus会多处调用，这里填-2表示
	//是任务发放的红利，由于第二个参数的used一直没有使用，这里临时使用这种方法
	//这里确实比较山寨！为了不修改原来的协议
	_imp->_runner->player_bonus(_imp->BonusInfo().GetBonus(), -2);
}

//激活元魂装备位
void PlayerTaskInterface::DeliverOpenSoulEquip()
{
	_imp->ActiveRune();
}


void 
PlayerTaskInterface::OnTaskComplete(unsigned long task_id, unsigned short uFinishCount)
{
	_imp->OnTaskComplete(task_id, uFinishCount);	
}

bool
PlayerTaskInterface::IsAchievementFinish(unsigned short id)
{
	return _imp->IsAchievementFinish(id);
}

void
PlayerTaskInterface::FinishAchievement(unsigned short id)
{
	_imp->FinishAchievement(id);
}

void
PlayerTaskInterface::UnfinishAchievement(unsigned short id)
{
	_imp->UnfinishAchievement(id);
}

int
PlayerTaskInterface::GetTransformID()
{
	return _imp->OI_GetTransformID();
}

int 
PlayerTaskInterface::GetTransformLevel()
{
	return _imp->OI_GetTransformLevel();
	
}

int
PlayerTaskInterface::GetTransformExpLevel()
{
	return _imp->OI_GetTransformExpLevel();
}


void 
PlayerTaskInterface::SetTransform(int iID, int iDuration, int iLevel, int iExpLevel, bool bCover)
{
	if(iID == 0)
	{
		_imp->OI_StopTransform(iID, player_transform::TRANSFORM_TASK);
		return;
	}

	if(!bCover && _imp->IsTransformState()) return;
	if(iID > 0)
	{
		_imp->OI_StartTransform(iID, iLevel, iExpLevel, iDuration, player_transform::TRANSFORM_TASK);
	}
}


int
PlayerTaskInterface::GetReinforceItemCount(unsigned long ulItemId, unsigned long ulLevel, unsigned char cond_type)
{
	if(ulLevel <= 0) return 0;

	int count = 0;
	int reinforce_level = 0;
	item_list &inv = _imp->GetInventory();
	for(size_t i = 0; i < inv.Size(); ++i)
	{
		if(inv[i].type == (int)ulItemId && inv[i].body->GetItemType() == item_body::ITEM_TYPE_EQUIPMENT)
		{
			reinforce_level = inv[i].GetReinforceLevel();
			if(cond_type == 0 && reinforce_level >= (int)ulLevel) count++;
			else if(cond_type == 1 && reinforce_level <= (int)ulLevel) count++;
			else if(cond_type == 2 && reinforce_level == (int)ulLevel) count++;
		}	
	}
	return count;
}

void 
PlayerTaskInterface::AddTeamTask(int teamid, int taskid)
{
	TeamTask::AddTeamTask(teamid, taskid);
}

void 
PlayerTaskInterface::DelTeamTask(int teamid, int taskid)
{
	TeamTask::DelTeamTask(teamid, taskid);
}

int 
PlayerTaskInterface::GetTeamTaskCount(int teamid, int taskid)
{
	return TeamTask::GetTeamTaskCount(teamid, taskid);
}

// 召唤任务归属怪
// 归属模式：0:默认的普通怪 1:自己 2:师傅 3:徒弟 4:仙侣 5:队伍 6:队伍或自己 7:队伍与自己 8:不限
void 
PlayerTaskInterface::SummonMonster(unsigned long ulTemplId, bool bIsMonster, unsigned long ulNum, unsigned long ulMapId, const float pos[3], unsigned long ulPeriod, unsigned long ulSummonMode)
{
	A3DVECTOR summon_pos(pos[0], pos[1], pos[2]);
	object_interface oi((gplayer_imp *)_imp);
	if(bIsMonster)
	{
		oi.CreateMonster(ulTemplId, ulMapId, summon_pos, ulPeriod, ulNum, ulSummonMode);
	}
	else
	{
		oi.CreateMines(ulTemplId, ulMapId, summon_pos, ulPeriod, ulNum, ulSummonMode);
	}
}

int
PlayerTaskInterface::GetCircleGroupId()
{
	return _imp->GetCircleID();
}

unsigned long
PlayerTaskInterface::GetLastLogoutTime()
{
	return _imp->GetLogoutTime();
}

unsigned long
PlayerTaskInterface::GetCreateRoleTimeDuration()
{
	return _imp->GetRoleCreateTimeDuration();
}

void
PlayerTaskInterface::UpdateCircleGroupPoint(unsigned long ulPoint)
{
	return _imp->AddCirclePoint(ulPoint);
}

void
PlayerTaskInterface::QueryCircleGroupData(const void* pData, int nSize)
{
	return _imp->QueryCircleGroupData(pData, nSize);
}

void
PlayerTaskInterface::QueryFriendNum(unsigned long ulTaskId)
{
	return _imp->QueryFriendNum(ulTaskId);
}

unsigned long
PlayerTaskInterface::GetTerritoryScore()
{
	return _imp->GetTerritoryScore();
}

void
PlayerTaskInterface::ProtectNPC(unsigned long ulTaskId, unsigned long ulNPCTemplId, unsigned long ulPeriod)
{
	ASSERT(ulPeriod > 0 && ulPeriod < 3600*6);
	object_interface oi((gplayer_imp *)_imp);
	oi.CreateProtectedNPC(ulNPCTemplId, ulPeriod, ulTaskId);
}

bool
PlayerTaskInterface::CheckNPCAppearInTask(unsigned long ulTaskId, unsigned long ulNPC)
{
	return GetTaskTemplMan()->CheckNPCAppearInTask(ulTaskId, ulNPC);
}

bool
PlayerTaskInterface::IsInCrossServer()
{
	return _imp->IsZoneServer();
}


bool
PlayerTaskInterface::GetTaskNPCPos(unsigned long ulNPC, A3DVECTOR & pos, int & tag)
{
	//查找npc_id 对应的坐标id
	const NPC_INFO * info;
	if( (info = GetTaskTemplMan()->GetTaskNPCInfo(ulNPC)) == NULL)
	{
		return false;
	}

	pos.x = info->x;
	pos.y = info->y;
	pos.z = info->z;
	tag = info->map_id;
	return true;
}

unsigned long 
PlayerTaskInterface::GetFengshenLevel()
{
	return _imp->_basic.dt_level; 
}

bool
PlayerTaskInterface::IsFengshenExpFull()
{
	return _imp->IsDeityExpFull();
}

void
PlayerTaskInterface::DeliverFengshenExp(unsigned long ulExp, unsigned long ulPlayerID)
{
	return _imp->ReceiveTaskDeityExp(ulExp);
}

void
PlayerTaskInterface::DeliverFengshenDujie(unsigned long ulPlayerID)
{
	_imp->DeityDuJie();	//封神渡劫
}

bool
PlayerTaskInterface::HasKing()
{
	return _imp->HasKing();
}

bool
PlayerTaskInterface::IsKing()
{
	return _imp->IsKingdomKing();
}

bool
PlayerTaskInterface::IsQueen()
{
	return _imp->IsKingdomQueen();
}

bool
PlayerTaskInterface::IsGeneral()
{
	return _imp->IsKingdomGeneral();
}

bool
PlayerTaskInterface::IsOfficial()
{
	return _imp->IsKingdomOfficial();
}

bool 
PlayerTaskInterface::IsMember()
{
	return _imp->IsKingdomMember();
}

bool 
PlayerTaskInterface::IsGuard()
{
	return _imp->IsKingdomGuard();
}

int
PlayerTaskInterface::GetKingScore()
{
	return _imp->GetKingdomPoint();
}

void
PlayerTaskInterface::CostKingScore(int value)
{
	_imp->ConsumeKingdomPoint(value);
}

void
PlayerTaskInterface::DeliverKingScore(int nValue)
{
	_imp->ObtainKingdomPoint(nValue);
}

void PlayerTaskInterface::ClearXpSkillCD()
{
	_imp->_skill.ClearXPSkillCoolTime(object_interface(_imp));
}

bool
PlayerTaskInterface::IsDynamicForbiddenTask(int ulTaskId)
{
	return _imp->IsForbidTask(ulTaskId);
}


void
PlayerTaskInterface::SetPhase(int id, bool bTrigger)
{
	_imp->SetPhase(id, bTrigger);
}

int
PlayerTaskInterface::GetBuildingLevel(int iBuildId)
{
	return _imp->GetFacBuildingLevel(iBuildId);
}

int 
PlayerTaskInterface::GetFactionGoldNote()
{
	return _imp->GetFactionCoupon();
}

void 
PlayerTaskInterface::DeliverFactionGoldNote(int ulGoldNoteNum)
{
	_imp->FactionCouponChange(ulGoldNoteNum);
}

void 
PlayerTaskInterface::TakeAwayFactionGoldNote(int ulGoldNoteNum)
{
	_imp->FactionCouponChange(-1*ulGoldNoteNum);
}

int 
PlayerTaskInterface::GetFactionGrass()
{
	return _imp->GetFBaseGrass();
}

void 
PlayerTaskInterface::DeliverFactionGrass(int ulGrassNum)
{
	_imp->DeliverFBaseGrass(ulGrassNum);
}

void 
PlayerTaskInterface::TakeAwayFactionGrass(int ulGrassNum)
{
	_imp->DeductFBaseGrass(ulGrassNum);
}

int 
PlayerTaskInterface::GetFactionMine()
{
	return _imp->GetFBaseMine();
}

void 
PlayerTaskInterface::DeliverFactionMine(int ulMineNum)
{
	_imp->DeliverFBaseMine(ulMineNum);
}

void 
PlayerTaskInterface::TakeAwayFactionMine(int ulMineNum)
{
	_imp->DeductFBaseMine(ulMineNum);
}

int 
PlayerTaskInterface::GetFactionMonsterFood()
{
	return _imp->GetFBaseMonsterFood();
}

void 
PlayerTaskInterface::DeliverFactionMonsterFood(int ulMonsterFoodNum)
{
	_imp->DeliverFBaseMonsterFood(ulMonsterFoodNum);
}

void 
PlayerTaskInterface::TakeAwayFactionMosnterFood(int ulMonsterFoodNum)
{
	_imp->DeductFBaseMonsterFood(ulMonsterFoodNum);
}

int 
PlayerTaskInterface::GetFactionMonsterCore()
{
	return _imp->GetFBaseMonsterCore();
}

void 
PlayerTaskInterface::DeliverFactionMonsterCore(int ulMonsterCoreNum)
{
	_imp->DeliverFBaseMonsterCore(ulMonsterCoreNum);
}
void 
PlayerTaskInterface::TakeAwayFactionMonsterCore(int ulMonsterCoreNum)
{
	_imp->DeductFBaseMonsterCore(ulMonsterCoreNum);
}
int 
PlayerTaskInterface::GetFactionMoney()//帮派元宝
{
	return _imp->GetFBaseCash();
}
void 
PlayerTaskInterface::DeliverFactionMoney(int num)
{
	_imp->DeliverFBaseCash(num);
}
void
PlayerTaskInterface::TakeAwayFactionMoney(int num)
{
	_imp->DeductFBaseCash(num);
}
void 
PlayerTaskInterface::OnFBaseBuildingProgress(unsigned long task_id, int value)
{
	_imp->OnFBaseBuildingProgress(task_id, value);
}

bool
PlayerTaskInterface::InInterAction(int iInterObjID)
{
	int mobactive_tid = _imp->GetMobActiveTid();
	return mobactive_tid == iInterObjID;
}

void
PlayerTaskInterface::TravelRound(int iTravelItemId, int iTravelTime, float fSpeed, int iTravelPath)
{
	_imp->StartTravelAround(iTravelItemId, fSpeed, iTravelPath, iTravelTime);
}

void PlayerTaskInterface::DeliverFactionAuctionItem(int idItem, float prob)
{
	if (abase::Rand(0.0f, 1.0f) < prob)
		_imp->DeliveryFBaseAuction(idItem);
}

int PlayerTaskInterface::GetBuildLevelInConstruct()
{
	return _imp->GetFacInBuildingLevel();
}

void
TeamTask::AddTeamTask(int teamid, int taskid)
{
	spin_autolock keeper(_teamtask_lock);

	std::map<TeamTaskData, int, TeamTaskCmp>::iterator iter;
	for(iter = _teamtask.begin(); iter != _teamtask.end(); ++iter)
	{
		if(iter->first.teamid == teamid && iter->first.taskid == taskid)
		{
			++iter->second;
			return;
		}
	}
	//创建一个新的
	TeamTaskData data(teamid, taskid); 
	_teamtask.insert(std::make_pair(data, 1));
	return;
}

void
TeamTask::DelTeamTask(int teamid, int taskid)
{
	spin_autolock keeper(_teamtask_lock);

	std::map<TeamTaskData, int, TeamTaskCmp>::iterator iter;
	for(iter = _teamtask.begin(); iter != _teamtask.end(); ++iter)
	{
		if(iter->first.teamid == teamid && iter->first.taskid == taskid)
		{
			_teamtask.erase(iter);
			return;
		}
	}
}

int
TeamTask::GetTeamTaskCount(int teamid, int taskid)
{
	spin_autolock keeper(_teamtask_lock);

	std::map<TeamTaskData, int, TeamTaskCmp>::iterator iter;
	for(iter = _teamtask.begin(); iter != _teamtask.end(); ++iter)
	{
		if(iter->first.teamid == teamid && iter->first.taskid == taskid)
		{
			return iter->second;
		}
	}
	return 0;
}

