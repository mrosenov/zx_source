#ifndef __ONLINEGAME_GS_FACBASE_PLAYER_H
#define __ONLINEGAME_GS_FACBASE_PLAYER_H

#include "../player_imp.h"

class facbase_player_imp: public gplayer_imp
{
protected:
	/*
	bool _is_leaving;		//是否已经在离开状态，等待Delivery通知换线
	bool _success_entered;
	int _old_world_tag;		//进入战场前的tag	
	A3DVECTOR _old_pos;		//进入战场前的位置
	*/
	enum
	{
		LEAVE_TIMEOUT = 20,
	};
	int _leave_timeout;

	bool InSelfBase();
public:
	DECLARE_SUBSTANCE( facbase_player_imp )
	facbase_player_imp()
	{
		/*
		_is_leaving = false;
		_success_entered = false;
		_old_world_tag = 0;
		_old_pos = A3DVECTOR(0,0,0);
		*/
		_leave_timeout = 0;
	}
	virtual int MessageHandler( const MSG& msg );
	virtual void OnHeartbeat( size_t tick );
	virtual void PostLogin(const userlogin_t& user, char trole, char loginflag);
	virtual void PlayerEnterWorld();
//	virtual void PlayerEnter();
	virtual void PlayerLeaveWorld();	
//	virtual void PlayerLeave();
	virtual void PlayerEnterMap( int old_tag, const A3DVECTOR& old_pos );
	virtual void PlayerLeaveMap();
//	virtual const A3DVECTOR& GetLogoutPos( int &world_tag );

	virtual void AddFacBuilding(int fid, int index, int tid);
	virtual void UpgradeFacBuilding(int fid, int index, int tid, int cur_lev);
	virtual void RemoveFacBuilding(int fid, int index, int tid);
	virtual void PlayerGetFacBase();
	virtual void FacBaseTransfer(int index);

	virtual int GetFacBuildingLevel(int iBuildId);
	virtual int GetFBaseGrass();
	virtual void DeductFBaseGrass(int num);
	virtual void DeliverFBaseGrass(int num);

	virtual int GetFBaseMine();
	virtual void DeductFBaseMine(int num);
	virtual void DeliverFBaseMine(int num);

	virtual int GetFBaseMonsterFood();
	virtual void DeductFBaseMonsterFood(int num);
	virtual void DeliverFBaseMonsterFood(int num);

	virtual int GetFBaseMonsterCore();
	virtual void DeductFBaseMonsterCore(int num);
	virtual void DeliverFBaseMonsterCore(int num);

	virtual int GetFBaseCash();
	virtual void DeductFBaseCash(int num);
	virtual void DeliverFBaseCash(int num);
	virtual void OnFBaseBuildingProgress(unsigned long task_id, int value);
	virtual void DebugChangeBaseProp(int type, int delta);
	virtual bool GetFacbaseCashItemsInfo();
	virtual void BuyFacbaseCashItem( int item_idx );
	FBASE_CMD_RET CheckPostFacBaseMsg(int fid, char message[], unsigned int len);
	FBASE_CMD_RET DoBidFromFacbaseMall( int idx, int item_id, int coupon, int name_len, char * player_name);
	virtual void WithdrawFacBaseAuction(int item_index);
	virtual void WithdrawFacBaseCoupon();
	virtual bool CheckEnterBase() { return false; } //重载函数但什么也不干 因为已经在基地了怎么能又进入?
	virtual bool LeaveFacBase();
	virtual int GetFacInBuildingLevel();
	virtual void DeliveryFBaseAuction(int itemid);
	virtual void OnUpdateMafiaInfo();
};

#endif

