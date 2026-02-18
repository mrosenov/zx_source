#include "policy.h"
#include "policy_loader.h"
#include "../aitrigger.h"
#include "../worldmanager.h"
#include "../gmatrix.h"
#include <ASSERT.h>

using namespace ai_trigger;

namespace
{
	expr * ConvertExpr (void *pTree);
	
	condition * ConvertCondition(void * pTree)
	{
		CTriggerData::_s_tree_item * temp = (CTriggerData::_s_tree_item*)pTree;
		switch(temp->mConditon.iType) 
		{
			case CTriggerData::c_time_come:
				return new cond_timer(((C_TIME_COME*)temp->mConditon.pParam)->uID);
			case CTriggerData::c_hp_less:
				return new cond_hp_less(((C_HP_LESS*)temp->mConditon.pParam)->fPercent);
			case CTriggerData::c_random:
				return new cond_random(((C_RANDOM*)temp->mConditon.pParam)->fProbability);

			case CTriggerData::c_and:
				{
					condition * left = ConvertCondition(temp->pLeft);
					condition * right = ConvertCondition(temp->pRight);
					return new cond_and(left,right);
				}
			case CTriggerData::c_not:
				{
					condition * right = ConvertCondition(temp->pRight);
					return new cond_not(right);
				}
				
			case CTriggerData::c_or:
				{
					condition * left = ConvertCondition(temp->pLeft);
					condition * right = ConvertCondition(temp->pRight);
					return new cond_or(left,right);
				}

			case CTriggerData::c_kill_player:
				return new cond_kill_target();
			case CTriggerData::c_start_attack:
				return new cond_start_combat();

			case CTriggerData::c_died:
				return new cond_on_death();
			
			case CTriggerData::c_path_end_point:
				return new cond_path_end(((C_PATH_END_POINT*)temp->mConditon.pParam)->uID);

			case CTriggerData::c_enmity_reach:
				return new cond_aggro_count(((C_ENMITY_REACH*)temp->mConditon.pParam)->nPlayer);

			case CTriggerData::c_distance_reach:
				return new cond_battle_range(((C_DISTANCE_REACH*)temp->mConditon.pParam)->fDistance);

			case CTriggerData::c_less:
				{
					expr * left = ConvertExpr(temp->pLeft);
					expr * right = ConvertExpr(temp->pRight);
					return new cond_compare_less (left, right);
				}
			case CTriggerData::c_great:
				{
					expr * left = ConvertExpr(temp->pLeft);
					expr * right = ConvertExpr(temp->pRight);
					return new cond_compare_greater (left, right);
				}
			case CTriggerData::c_equ:
				{
					expr * left = ConvertExpr(temp->pLeft);
					expr * right = ConvertExpr(temp->pRight);
					return new cond_compare_equal (left, right);
				}

			case CTriggerData::c_born:
				return new cond_on_born();

			case CTriggerData::c_attack_by_skill:
				return new cond_attack_by_skill(((C_ATTACK_BY_SKILL*)temp->mConditon.pParam)->uID);

			default:
			ASSERT(false);
			break;
		}
		return NULL;
	}

	expr * ConvertExpr (void *pTree)
	{
		//事实上这部分是表达式树，不是条件树
		CTriggerData::_s_tree_item * temp = (CTriggerData::_s_tree_item*)pTree;
		switch(temp->mConditon.iType) 
		{
			case CTriggerData::c_plus:
				{
					expr * left = ConvertExpr(temp->pLeft);
					expr * right = ConvertExpr(temp->pRight);
					return new expr_plus (left, right);
				}
			case CTriggerData::c_minus:
				{
					expr * left = ConvertExpr(temp->pLeft);
					expr * right = ConvertExpr(temp->pRight);
					return new expr_minus (left, right);
				}
			case CTriggerData::c_multiply:
				{
					expr * left = ConvertExpr(temp->pLeft);
					expr * right = ConvertExpr(temp->pRight);
					return new expr_multiply (left, right);
				}
			case CTriggerData::c_divide:
				{
					expr * left = ConvertExpr(temp->pLeft);
					expr * right = ConvertExpr(temp->pRight);
					return new expr_divide (left, right);
				}
			case CTriggerData::c_constant:
				{
					return new expr_constant(((C_CONSTANT*)temp->mConditon.pParam)->iValue);
				}
			case CTriggerData::c_var:
				{
					return new expr_common_data(((C_VAR*)temp->mConditon.pParam)->iID);
				}
			case CTriggerData::c_rank_level:
				{
					return new expr_rank_level(((C_RANK_LEVEL*)temp->mConditon.pParam)->iValue);
				}
				
			default:
			ASSERT(false);
			break;
		}
		return NULL;
	}
	target * ConvertTarget(const CTriggerData::_s_target & mTarget)
	{
		//目标类型，目前只有t_occupation_list有参数
		switch(mTarget.iType)
		{
			case CTriggerData::t_hate_first:
				return new target_aggro_first();
			case CTriggerData::t_hate_second:
				return new target_aggro_second();
			case CTriggerData::t_hate_others:
				return new target_aggro_second_rand();
			case CTriggerData::t_most_hp:
				return new target_most_hp();
			case CTriggerData::t_most_mp:
				return new target_most_mp();
			case CTriggerData::t_least_hp:
				return new target_least_hp();
			case CTriggerData::t_self:
				return new target_self();
			case CTriggerData::t_occupation_list:
				{
					int bit =  ((T_OCCUPATION*)mTarget.pParam)->uBit;
					return new target_class_combo(bit);
				}
		}
		return NULL;
	}

	trigger * ConvertTrigger(CPolicyData *pPolicyData, CTriggerData *pTriggerData);
	operation * ConvertOperation(CPolicyData *pPolicyData, CTriggerData::_s_operation *pOperation)
	{
		target * tar = ConvertTarget(pOperation->mTarget);
		operation * pOP = NULL;

		#define OPARAM(x) ((x*)pOperation->pParam)

		//操作类型
		switch(pOperation->iType) 
		{
			case CTriggerData::o_attact:
				pOP = new op_attack(OPARAM(O_ATTACK_TYPE)->uType);
				break;
			case CTriggerData::o_use_skill:
				pOP = new op_skill(OPARAM(O_USE_SKILL)->uSkill,OPARAM(O_USE_SKILL)->uLevel);
				break;
			case CTriggerData::o_talk:
				pOP = new op_say(OPARAM(O_TALK_TEXT)->szData,OPARAM(O_TALK_TEXT)->uSize);
				break;
			case CTriggerData::o_reset_hate_list:
				pOP = new op_reset_aggro();
				break;
			case CTriggerData::o_run_trigger:
				{
					int idx = pPolicyData->GetIndex(OPARAM(O_RUN_TRIGGER)->uID);
					CTriggerData *pTriggerData = pPolicyData->GetTriggerPtr(idx);
					trigger * pT = ConvertTrigger(pPolicyData,pTriggerData);
					pOP = new op_exec_trigger(pT);
				}
				break;
			case CTriggerData::o_stop_trigger:
				pOP = new op_enable_trigger(OPARAM(O_STOP_TRIGGER)->uID,false);
				break;
			case CTriggerData::o_active_trigger:
				pOP = new op_enable_trigger(OPARAM(O_ACTIVE_TRIGGER)->uID,true);
				break;
			case CTriggerData::o_create_timer:
				pOP = new op_create_timer(OPARAM(O_CREATE_TIMER)->uID,OPARAM(O_CREATE_TIMER)->uPeriod,OPARAM(O_CREATE_TIMER)->uCounter);
				break;
			case CTriggerData::o_kill_timer:
				pOP = new op_remove_timer(OPARAM(O_KILL_TIMER)->uID);
				break;

			case CTriggerData::o_flee:
				pOP = new op_flee();
				break;
			case CTriggerData::o_set_hate_to_first:
				pOP = new op_be_taunted();
				break;
			case CTriggerData::o_set_hate_to_last:
				pOP = new op_fade_target();
				break;
			case CTriggerData::o_set_hate_fifty_percent:
				pOP = new op_aggro_fade();
				break;
			case CTriggerData::o_skip_operation:
				pOP = new op_break();
				break;
			case CTriggerData::o_active_controller:
				pOP = new op_active_spawner(OPARAM(O_ACTIVE_CONTROLLER)->uID,OPARAM(O_ACTIVE_CONTROLLER)->bStop);
				break;

			case CTriggerData::o_summon:
				{
				const O_SUMMON * pSum = (O_SUMMON*)(pOperation->pParam);
				const unsigned short * p = std::find(pSum->szName, pSum->szName + 16, (unsigned short)0);
				size_t name_len = (p-pSum->szName)*sizeof(short);
				pOP = new op_create_minors(pSum->uMounsterID,pSum->uBodyMounsterID,pSum->uLife, pSum->bFollow,
								pSum->bDispear,pSum->uMounsterNum,(void*)pSum->szName, name_len,
								pSum->fRange);
				}
				break;
			case CTriggerData::o_trigger_task:
				pOP = new op_task_trigger(OPARAM(O_TRIGGER_TASK)->uTaskID);
				break;

			case CTriggerData::o_change_path:
				pOP = new op_switch_path(OPARAM(O_CHANGE_PATH)->uPathID, OPARAM(O_CHANGE_PATH)->iType);
				break;

			case CTriggerData::o_dispear:
				pOP = new op_disappear();
				break;

			case CTriggerData::o_sneer_monster:
				pOP = new op_cajole_mobs(OPARAM(O_SNEER_MONSTER)->fRange);
				break;
			case CTriggerData::o_use_range_skill:  
				////$$$$$$$$$$$$$$$$$$$$$
				ASSERT(false);
				break;

			case CTriggerData::o_reset:
				pOP = new op_combat_reset();
				break;

			case CTriggerData::o_set_global:
				{
					O_SET_GLOBAL * pSG = (O_SET_GLOBAL*)(pOperation->pParam);
					pOP = new op_set_common_data (pSG->iID, pSG->iValue);
				}
				break;

			case CTriggerData::o_revise_global:
				{
					O_REVISE_GLOBAL * pRG = (O_REVISE_GLOBAL*)(pOperation->pParam);
					pOP = new op_add_common_data (pRG->iID, pRG->iValue);
				}
				break;
			
			case CTriggerData::o_assign_global:
				{
					O_ASSIGN_GLOBAL * pAG = (O_ASSIGN_GLOBAL*)(pOperation->pParam);
					pOP = new op_assign_common_data (pAG->iIDSrc, pAG->iIDDst);
				}
				break;

			case CTriggerData::o_summon_mineral:
				{
					O_SUMMON_MINERAL *pSM = (O_SUMMON_MINERAL*)(pOperation->pParam);
					pOP = new op_summon_mineral(pSM->uMineralID, pSM->uMineralNum, pSM->uHP, pSM->fRange, pSM->bBind);
				}
				break;

			case CTriggerData::o_drop_item:
				{
					O_DROP_ITEM *pDI = (O_DROP_ITEM*)(pOperation->pParam);
					pOP = new op_drop_item(pDI->uItemID, pDI->uItemNum, pDI->uExpireDate);
				}
				break;

			case CTriggerData::o_change_hate:
				{
					O_CHANGE_HATE * pCH = (O_CHANGE_HATE*)(pOperation->pParam);
					pOP = new op_change_aggro(pCH->iHateValue);
				}
				break;

			case CTriggerData::o_start_event:
				{
					O_START_EVENT * pSE = (O_START_EVENT*)(pOperation->pParam);
					pOP = new op_start_event(pSE->iId);
				}
				break;
			
			case CTriggerData::o_stop_event:
				{
					O_STOP_EVENT * pSE = (O_STOP_EVENT*)(pOperation->pParam);
					pOP = new op_stop_event(pSE->iId);
				}
				break;

			default://...
				ASSERT(false);
				break;
		}
		
		if(pOP->RequireTarget())
		{
			pOP->SetTarget(tar);
		}
		else
		{
			delete tar;
		}
		return pOP;
		#undef OPARAM
	}

	trigger * ConvertTrigger(CPolicyData *pPolicyData, CTriggerData *pTriggerData)
	{
		CTriggerData::_s_tree_item* root = pTriggerData->GetConditonRoot();
		condition * cond = ConvertCondition(root);

		trigger * pTri = new trigger();
		pTri->SetBattleEnable(pTriggerData->GetRunCondition());
		pTri->SetData(pTriggerData->GetID(),cond);

		int numOperation = pTriggerData->GetOperaionNum();
		for( int j = 0; j < numOperation; ++j)
		{
			CTriggerData::_s_operation *pOperation = pTriggerData->GetOperaion(j);
			operation *pOP = ConvertOperation(pPolicyData,pOperation);
			pTri->AddOp(pOP);
		}
		return pTri;
	}
}

bool LoadAIPolicy(const char * path)
{
	CPolicyDataManager man;
	if(!man.Load(path))
	{
		printf("Failed to Load AIPolicy '%s'\n",path);
		return false;
	}
	int numPolicy = man.GetPolicyNum();
	for(int i = 0; i < numPolicy; i ++)
	{
		CPolicyData *pPolicyData = man.GetPolicy(i);
		policy * pPolicy = new policy(pPolicyData->GetID());

		//加入各个触发器
		int numTrigger = pPolicyData->GetTriggerPtrNum();
		for(int j = 0; j < numTrigger; j ++)
		{
			CTriggerData *pTriggerData = pPolicyData->GetTriggerPtr(j);
			if(pTriggerData->IsRun()) continue;
			trigger * pTrigger = ConvertTrigger(pPolicyData,pTriggerData);
			pTrigger->SetDefaultEnable(pTriggerData->IsActive());
			pPolicy->AddTrigger(pTrigger);
		}

		//将策略加入到管理器中
		gmatrix::GetTriggerMan().AddPolicy(pPolicy);
	}
	man.Release();
	return true;
}

