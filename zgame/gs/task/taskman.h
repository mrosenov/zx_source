#ifndef __ONLINEGAME_GS_TASK_MAN_H__
#define __ONLINEGAME_GS_TASK_MAN_H__

#include "TaskServer.h"
#include <common/types.h>

struct gplayer;
class elementdataman;
bool InitQuestSystem(const char * filename,const char * filename2, const char * filename3, const char * filename4, elementdataman * pMan);


class gplayer_imp;
class PlayerTaskInterface : public TaskInterface
{
gplayer_imp * _imp;
public:
	virtual unsigned long GetPlayerLevel();
	virtual void* GetActiveTaskList();
	virtual void* GetFinishedTaskList();
	virtual void* GetFinishedTimeList();
	virtual unsigned long* GetTaskMask();
	virtual void DeliverGold(unsigned long ulGoldNum,unsigned long ulPlayerID = 0);
	virtual void DeliverExperience(int64_t ulExp, unsigned long ulPlayerID = 0);
	virtual void DeliverSP(unsigned long ulSP,unsigned long ulPlayerID = 0);
	virtual void DeliverReputation(long lReputation, unsigned long ulPlayerID = 0);
	virtual void DeliverRegionReputation(int index, long lValue,unsigned long ulPlayerID = 0);
	virtual int GetTaskItemCount(unsigned long ulTaskItem);
	virtual int GetCommonItemCount(unsigned long ulCommonItem);
	virtual int GetReinforceItemCount(unsigned long ulItemId, unsigned long ulLevel, unsigned char cond_type);
	virtual bool IsInFaction();
	virtual unsigned long GetGoldNum();
	virtual void TakeAwayGold(unsigned long ulNum);
	virtual void TakeAwayTaskItem(unsigned long ulTemplId, unsigned long ulNum);
	virtual void TakeAwayCommonItem(unsigned long ulTemplId, unsigned long ulNum);
	virtual void TakeAwayReinforceItem(unsigned long ulTemplId, unsigned long ulItemLevel, unsigned char cond_type);
	virtual long GetReputation();
	virtual long GetRegionReputation(int index);
	virtual unsigned long GetCurPeriod();
	virtual void SetCurPeriod(unsigned long per);
	virtual unsigned long GetPlayerId();
	virtual unsigned long GetPlayerRace();
	virtual unsigned long GetPlayerOccupation();
	virtual bool CanDeliverCommonItem(unsigned long ulItemTypes);
	virtual bool CanDeliverTaskItem(unsigned long ulItemTypes);
	virtual int GetItemSlotCnt(unsigned long ulItemTemplID, unsigned long ulItemNum);
//	virtual bool CanDeliverCommonItem(unsigned long ulItem,unsigned long count);
//	virtual bool CanDeliverTaskItem(unsigned long ulItem,unsigned long count);
	virtual void DeliverCommonItem(unsigned long ulItem,unsigned long count,bool bind, long lPeriod,unsigned long ulPlayerID = 0);
	virtual void DeliverReinforceItem(unsigned long ulItem, unsigned long level, bool bind, long lPeriod);
	virtual void DeliverTaskItem(unsigned long ulItem,unsigned long count,unsigned long ulPlayerID = 0);
	virtual void NotifyClient(const void* pBuf, size_t sz);
	virtual float UnitRand();
	virtual bool HasLivingSkill(unsigned long ulSkill); 
	virtual int GetPetCon();
	virtual int GetPetCiv();
	virtual unsigned long GetPlayerLifeAgainCnt();
	virtual unsigned long GetPlayerLifeAgainOccup(int nLifeAgainIndex);
	virtual long GetLivingSkillProficiency(unsigned long ulSkill);
	virtual void SetLifeAgainOccup(unsigned long ulLifeAgainFaction);
	virtual void SetNewRelayStation(unsigned long ulStationId);
	virtual void SetStorehouseSize(unsigned long ulSize);
	virtual void SetFuryUpperLimit(unsigned long ulValue);
	virtual void TransportTo(unsigned long ulWorldId, const float pos[3],long );
	virtual void SetFactionStorehouseSize(unsigned long ulSize);

	virtual int GetTeamMemberNum();
	virtual void NotifyPlayer(unsigned long ulPlayerId, const void* pBuf, size_t sz);
	virtual void GetTeamMemberInfo(int nIndex, task_team_member_info* pInfo);
	virtual bool IsDeliverLegal();
	virtual unsigned long GetTeamMemberId(int nIndex);
	virtual bool IsInTeam();
	virtual bool IsCaptain();
	virtual bool IsMale();
	virtual unsigned long GetPos(float pos[3]);
	virtual unsigned long GetTeamMemberPos(int nIndex, float pos[3]);
	virtual void GetSpecailAwardInfo(special_award* p);
	virtual void SetPetInventorySize(unsigned long ulSize);

	virtual int GetProduceSkillLev();
	virtual void SetProduceSkillFirstLev();
	virtual int GetProduceSkillExp();
	virtual void AddProduceSkillExp(int nExp);
	virtual void SetNewProfession(unsigned long ulProfession);
	virtual void GiveOneTitle(long lTitle, unsigned long ulPlayerID =0);
	virtual void TakeAwayTitle(short title);
	virtual void SetInventorySize(long lSize);
	virtual void SetMountInventorySize(unsigned long ulSize); 
	virtual long GetPKValue();
	virtual bool IsGM();
	virtual bool HasTitle(short title);
	virtual void DeliverPKValue(long lVal);
	virtual void ResetPKValue();
	virtual long GetFactionContribution();
	virtual void DeliverFactionContribution(long lVal);
	virtual bool IsMarried();
	virtual void CheckTeamRelationship(int nReason);
	virtual int GetFactionLev();
	virtual int GetFactionRole();
	virtual void Divorce();
	virtual void SetMonsterController(long id, bool bTrigger);
	virtual void DeliverBuff(long skill, long level);
	virtual int GetTalismanValue();
	virtual void SetDoubleExpTime(unsigned long t);

	/* 家族信息 */
	virtual void QueryFamilyData(const void*, int);
	virtual int GetFamilyID();
	virtual bool IsInFamily();
	virtual int GetFamilySkillLevel(int nIndex);
	virtual int GetFamilySkillProficiency(int nIndex);
	virtual void DeliverFamilySkillProficiency(int nIndex, int nValue);
	virtual void UpgradeFamilySkill(int nIndex, int nNewLevel);
	virtual void DeliverFamilyContribution(int nValue);
	virtual int GetFamilyContribution();
	virtual void SendMessage(unsigned long task_id, int channel, int param, long lVariables[3]);
	virtual bool ClearSomeIllegalStates();

	// 战场相关
	virtual int GetBattleScore();
	virtual void DeliverBattleScore( int socre );

	// 宋金战场	
	virtual int GetSJBattleScore();
	virtual void DeliverSJBattleScore(int nValue);
	virtual void ResetSJBattleScore();

	virtual bool IsMaster();
	virtual bool IsPrentice();
	virtual void DeviateMaster();

	virtual void DeliverFamilyExperience(int nFamilyID, int64_t ulExp);
	virtual unsigned long GetCultivation(); 		//仙魔佛的组合mask
	virtual void SetCultivation(unsigned long ulCult); 	//设置仙魔佛的组合mask
	virtual void ClearCultSkill(unsigned long ulCult);	//清除仙魔佛的技能

	virtual void ResetRegionReputation(int index) { DeliverRegionReputation(index, - GetRegionReputation(index));}

	virtual void ClearNormalSkill();
	virtual void ClearTalentSkill();
	virtual unsigned long GetInMasterLev();
	virtual int GetTreasureConsumed();
	virtual unsigned long GetAchievePoint(); // 获取成就点数
	virtual int GetTotalCaseAdd();	   //充值元宝数

	virtual void OutMaster();

	virtual long GetGlobalValue(long lKey);
	virtual void PutGlobalValue(long lKey, long lValue);
	virtual void ModifyGlobalValue(long lKey, long lValue);
	virtual bool IsMaxInvSize();
	virtual void SetPocketSize(long lSize);

	virtual void OnTaskComplete(unsigned long task_id, unsigned short uFinishCount);
	virtual bool IsAchievementFinish(unsigned short id);
	virtual void FinishAchievement(unsigned short id);
	virtual void UnfinishAchievement(unsigned short id);

	virtual int GetTransformID();
	virtual int GetTransformLevel();
	virtual int GetTransformExpLevel();
	virtual void SetTransform(int iID, int iDuration, int iLevel, int iExpLevel, bool bCover);	

	virtual void AddTeamTask(int teamid, int taskid);
	virtual void DelTeamTask(int teamid, int taskid);
	virtual int GetTeamTaskCount(int teamid, int taskid);

	// 召唤任务归属怪
	// 归属模式：0:默认的普通怪 1:自己 2:师傅 3:徒弟 4:仙侣 5:队伍 6:队伍或自己 7:队伍与自己 8:不限
	virtual void SummonMonster(unsigned long ulTemplId, bool bIsMonster, unsigned long ulNum, unsigned long ulMapId, const float pos[3], unsigned long ulPeriod, unsigned long ulSummonMode);
	
	virtual int GetCircleGroupId();				// 获取圈子id
	virtual unsigned long GetLastLogoutTime();	// 获取上次下线时间

	// 更新圈子积分
	virtual void UpdateCircleGroupPoint(unsigned long ulPoint);
	// 查询圈子信息
	virtual void QueryCircleGroupData(const void* pData, int nSize);
	// 查询好友人数
	virtual void QueryFriendNum(unsigned long ulTaskId);

	// 保护特定NPC
	virtual void ProtectNPC(unsigned long ulTaskId, unsigned long ulNPCTemplId, unsigned long ulPeriod);
	
	virtual unsigned long GetTerritoryScore();	// 获取领土战积分
	
	virtual bool CheckNPCAppearInTask(unsigned long ulTaskId, unsigned long ulNPC);
	virtual bool GetTaskNPCPos(unsigned long ulNPC, A3DVECTOR & pos, int & tag);
	
	virtual bool IsInCrossServer();
	
	/* 封神 */
	virtual unsigned long GetFengshenLevel();
	virtual bool IsFengshenExpFull();
	virtual void DeliverFengshenExp(unsigned long ulExp, unsigned long ulPlayerID = 0);
	virtual void DeliverFengshenDujie(unsigned long ulPlayerID = 0);

	//鸿利
	virtual void DeliverBonus(int inc);

	//获得角色创建时间距离当前时间的长度
	virtual unsigned long GetCreateRoleTimeDuration();

	/* 国战 */
	virtual bool HasKing();		// 是否有国王
	virtual bool IsKing();		// 玩家是否国王
	virtual bool IsQueen();		// 玩家是否王后
	virtual bool IsGeneral();	// 玩家是否将军
	virtual bool IsOfficial();	// 玩家是否大臣
	virtual bool IsMember();	// 玩家是否国王帮成员
	virtual bool IsGuard();		// 玩家是否亲卫
	virtual int  GetKingScore();// 获取国王积分
	virtual void CostKingScore(int nValue);
	virtual void DeliverKingScore(int nValue);
	virtual void ClearXpSkillCD();

	// 检查任务是否动态关闭
	virtual bool IsDynamicForbiddenTask(int ulTaskId); 
	
	virtual void DeliverOpenSoulEquip();	//任务开启角色元魂装备位 Added 2012-04-18.
	//开启关闭相位
	virtual void SetPhase(int id, bool bTrigger);
	
	int GetBuildingLevel(int iBuildId); //获取指定建筑ID的等级
	int GetFactionGoldNote(); //帮派金券数量
	void DeliverFactionGoldNote(int ulGoldNoteNum); //任务奖励发功帮派金券奖励
	void TakeAwayFactionGoldNote(int ulGoldNoteNum); //任务完成扣除帮派金券

	int GetFactionGrass(); //获取帮派的灵草数量
	void DeliverFactionGrass(int ulGrassNum); //任务奖励发放的帮派灵草数量
	void TakeAwayFactionGrass(int ulGrassNum); //任务完成扣除的帮派灵草数量

	int GetFactionMine(); //获取帮派的矿石数量
	void DeliverFactionMine(int ulMineNum); //任务奖励发放的帮派矿石数量
	void TakeAwayFactionMine(int ulMineNum); //任务完成扣除的帮派矿石数量

	int GetFactionMonsterFood(); //获取帮派的兽粮数量
	void DeliverFactionMonsterFood(int ulMonsterFoodNum); //任务奖励发放的帮派兽粮数量
	void TakeAwayFactionMosnterFood(int ulMonsterFoodNum); //任务完成扣除的帮派兽粮数量

	int GetFactionMonsterCore(); //获取帮派兽核的数量
	void DeliverFactionMonsterCore(int ulMonsterCoreNum); //任务奖励方法的帮派兽核数量
	void TakeAwayFactionMonsterCore(int ulMonsterCoreNum); //任务完成扣除的帮派兽核数量
	int GetFactionMoney();//帮派元宝
	void DeliverFactionMoney(int ulMonsterCoreNum);
	void TakeAwayFactionMoney(int ulMonsterCoreNum);
	//帮派基地 增加基地建筑的建设进度
	void OnFBaseBuildingProgress(unsigned long task_id, int value);
	bool	InInterAction(int iInterObjId);
	void TravelRound(int iTravelItemId, int iTravelTime, float fSpeed, int iTravelPath);
	virtual void DeliverFactionAuctionItem(int idItem, float prob);
	virtual int GetBuildLevelInConstruct();
	
public:
	PlayerTaskInterface(gplayer_imp * imp):_imp(imp)
	{}

};

struct PlayerTaskTeamInterface : public TaskTeamInterface
{
	int marriage_op;
	int couple[2];
	PlayerTaskTeamInterface()
	{
		marriage_op = 0;
		couple[0] =  couple[1] = 0;
	}

	virtual void SetMarriage(int nPlayer);

	void Execute(gplayer ** list, size_t count);
};

//加入成员的包装函数 ;
inline void Task_OnTeamAddMember(TaskInterface* pTask, int index)
{
	task_team_member_info info;
	pTask->GetTeamMemberInfo(index,&info);
	OnTeamAddMember(pTask,&info);
}

#endif

