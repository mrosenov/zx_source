#ifndef _REFERENCEMANAGER_H_o
#define _REFERENCEMANAGER_H_o

#include <map>
#include <set>
#include <vector>
#include "itimer.h"

namespace GNET
{
class GReferral;
class GReferrer;
class ReferralBrief;
class RefTrans;

class CountedDirty
{
	int dirty;
public:
	CountedDirty():dirty(0) {}
	void SetDirty(bool _dirty)
	{
		if (_dirty) 
			++dirty;
		else if (dirty > 0)
			--dirty;
	}
	bool IsDirty() { return dirty>0; }
};

class Referral					//下线信息
{
	int roleid;
	int referrer;				//上线roleid
	int bonus_total1; 		//由于本人消耗元宝对上线的历史总贡献鸿利值
	int bonus_total2; 		//由于本人下线消耗元宝对本人上线的历史总贡献鸿利值
	int bonus_withdraw; 		//上线从该玩家已经提取的鸿利值
	int64_t exp_total1; 			//由于本人升级对上线的历史总贡献经验
	int64_t exp_total2; 			//由于本人下线升级对上线的历史总贡献经验
	int64_t exp_withdraw; 		//上线从该玩家已经提取的经验值
	CountedDirty dirty;
	bool logout;
	friend class ReferenceManager;
	friend class WithdrawExpTrans;
	friend class WithdrawBonusTrans;

public:
	Referral():roleid(0),referrer(0),bonus_total1(0),bonus_total2(0),bonus_withdraw(0),
				exp_total1(0),exp_total2(0),exp_withdraw(0),logout(false)
	{
	}
	void OnLoad(const GReferral &referral, int _referrer);
	void SetDirty(bool _dirty) { dirty.SetDirty(_dirty); }
	bool IsDirty() { return dirty.IsDirty(); }
	void SetLogout(bool _logout) { logout = _logout; }
	bool IsLogout() { return logout; }
	void OnUseCash(int bonus_cont);
	void OnLevelUp(int64_t exp_cont);
	void SyncDB();
	void ToGReferral(GReferral &gref);
};

class Referrer					//上线信息
{
	typedef std::set<int> ReferralSet;
	ReferralSet referrals;
	int roleid;
	int bonus_withdraw;			//上线历史上从所有下线提取的总的鸿利值
	int64_t exp_withdraw_today; //本日该玩家从所有下线身上已经提取的总经验值
	int exp_withdraw_time; //最后一次从下线领取经验的时间
	bool loaded;				//上线信息未加载
	CountedDirty dirty;
	friend class ReferenceManager;
	friend class WithdrawExpTrans;
	friend class WithdrawBonusTrans;
	
public:
	Referrer():roleid(0), bonus_withdraw(0), exp_withdraw_today(0), exp_withdraw_time(0), loaded(false)
	{
	}
	void OnLoad(const GReferrer &referrer);
	void AddReferral(int _roleid) { referrals.insert(_roleid); }
	void DelReferral(int _roleid) { referrals.erase(_roleid); }
	bool IsEmpty() { return referrals.empty(); }
	void SetLoaded(bool _loaded) { loaded = _loaded; }
	bool IsLoaded() { return loaded; }
	void SetDirty(bool _dirty) { dirty.SetDirty(_dirty); }
	bool IsDirty() { return dirty.IsDirty(); }
	void SyncDB();
	void OnLogin();
	void ToGReferrer(GReferrer &gref);
private:
	void CheckExpWithdrawToday();
};

class ReferenceManager : public IntervalTimer::Observer
{
	enum{
		CHECKNUM_ONUPDATE = 10,
		WITHDRAW_MAX = 250, //每次至多从250个下线中提取鸿利或经验
	};
	typedef std::map<int, Referrer> ReferrerMap;
	ReferrerMap referrers;
	typedef std::map<int, Referral> ReferralMap;
	ReferralMap referrals;
	typedef std::map<int, RefTrans *> WithdrawTransMap;		//以上线roleid索引的提取事务map
	WithdrawTransMap transmap;
	int referrer_cursor;
	int referral_cursor;
	int trans_cursor;
	time_t double_time_begin;
	time_t double_time_end;
	bool open;
	static ReferenceManager instance;

	ReferenceManager():referrer_cursor(0),referral_cursor(0),trans_cursor(0),double_time_begin(0),double_time_end(0), open(false){ }

public:
	static ReferenceManager *GetInstance() { return &instance; }

	bool Initialize();
	void OnLogin(int roleid);
	void OnLogout(int roleid);
	void OnLoadReferrer(const GReferrer &referrer);
	void OnLoadReferral(const GReferral &referral);
	void OnReferralUseCash(int roleid, int bonus_cont);
	void OnReferralLevelUp(int roleid, int64_t exp, int level, int reborn_cnt);
	int  ListReferrals(int roleid, int start_index, int &total, int &bonus_avail_today, int64_t &exp_avail_today, std::vector<ReferralBrief> &referrals);
	void OnDBUpdateReferrer(int roleid);
	void OnDBUpdateReferral(int roleid);
	void OnDBWithdrawConfirm(int roleid);
	void OnDBWithdrawRollback(int roleid);
	bool Update();
	bool IsInTransaction(int referrer) { return transmap.find(referrer)!=transmap.end(); }
	Referrer *GetReferrer(int roleid)
	{
		ReferrerMap::iterator it = referrers.find(roleid);
		if (it != referrers.end())
			return &it->second;
		else 
			return NULL;
	}
	Referral *GetReferral(int roleid)
	{
		ReferralMap::iterator it = referrals.find(roleid);
		if (it != referrals.end())
			return &it->second;
		else
			return NULL;
	}
	int WithdrawExp(int roleid);
	int WithdrawBonus(int roleid);
	bool IsOpen() { return open; }
};

};
#endif
