#include "referencemanager.h"
#include "greferral"
#include "referralbrief"
#include "greferrer"
#include "mapuser.h"
#include "timer.h"
#include <time.h>
#include "dbrefgetreferral.hrp"
#include "dbrefupdatereferral.hrp"
#include "dbrefgetreferrer.hrp"
#include "dbrefupdatereferrer.hrp"
#include "gamedbclient.hpp"
#include "gproviderserver.hpp"
#include "sendrefaddbonus.hpp"
#include "sendrefaddexp.hpp"
#include <limits.h>
#include "localmacro.h"
#include "reftrans.h"
#include "conf.h"


namespace GNET
{
ReferenceManager ReferenceManager::instance;

void Referral::OnLoad(const GReferral &referral, int _referrer)
{
	roleid = referral.roleid;
	referrer = _referrer;
	bonus_total1 = referral.bonus_total1;
	bonus_total2 = referral.bonus_total2;
	bonus_withdraw = referral.bonus_withdraw;
	exp_total1 = referral.exp_total1;
	exp_total2 = referral.exp_total2;
	exp_withdraw = referral.exp_withdraw;
}

void Referral::SyncDB()
{
	DBRefUpdateReferral *rpc = (DBRefUpdateReferral *)Rpc::Call(RPC_DBREFUPDATEREFERRAL, 
								DBRefUpdateReferralArg(
									GReferral(roleid, bonus_total1, bonus_total2, 
									bonus_withdraw, exp_total1, exp_total2, exp_withdraw)));
	GameDBClient::GetInstance()->SendProtocol(rpc);
}

void Referral::ToGReferral(GReferral &gref)
{
	gref.roleid = roleid;
	gref.bonus_total1 = bonus_total1;
	gref.bonus_total2 = bonus_total2;
	gref.bonus_withdraw = bonus_withdraw;
	gref.exp_total1 = exp_total1;
	gref.exp_total2 = exp_total2;
	gref.exp_withdraw = exp_withdraw;
}

void Referrer::ToGReferrer(GReferrer &gref)
{
	gref.roleid = roleid;
	gref.bonus_withdraw = bonus_withdraw;
	gref.exp_withdraw_today = exp_withdraw_today;
	gref.exp_withdraw_time = exp_withdraw_time;
}

void Referrer::OnLoad(const GReferrer &referrer)
{
	SetLoaded(true);
	roleid = referrer.roleid;
	bonus_withdraw = referrer.bonus_withdraw;
	exp_withdraw_today = referrer.exp_withdraw_today;
	exp_withdraw_time = referrer.exp_withdraw_time;
	CheckExpWithdrawToday();
}

void Referrer::OnLogin()
{
	if (IsLoaded())
		CheckExpWithdrawToday();
}

void Referrer::CheckExpWithdrawToday()
{
	if (exp_withdraw_today > 0)
	{
		struct tm tmnow;
		time_t now = Timer::GetTime();
		localtime_r(&now, &tmnow);
		tmnow.tm_sec = 0;
		tmnow.tm_min = 0;
		tmnow.tm_hour = 0;
		time_t midnight = mktime(&tmnow);

		if (exp_withdraw_time < midnight)
		{
			exp_withdraw_today = 0;
			SetDirty(true);
			return;
		}
	}
}

void Referrer::SyncDB()
{
	DBRefUpdateReferrer *rpc = (DBRefUpdateReferrer *)Rpc::Call(RPC_DBREFUPDATEREFERRER, 
								DBRefUpdateReferrerArg(
									GReferrer(roleid, bonus_withdraw, exp_withdraw_today, exp_withdraw_time)));
	GameDBClient::GetInstance()->SendProtocol(rpc);
}

bool ReferenceManager::Initialize()
{
	const string &double_conf = Conf::GetInstance()->find("GDeliveryServer", "double_bonus_time");
	if (!double_conf.empty())
	{
		int year_begin, year_end;
		int month_begin, month_end;
		int day_begin, day_end;
		if (6==sscanf(double_conf.c_str(), "%d.%d.%d-%d.%d.%d", &year_begin, &month_begin, &day_begin, &year_end, &month_end, &day_end))
		{
			struct tm tmtime = {};
			tmtime.tm_year = year_begin-1900;
			tmtime.tm_mon = month_begin-1;
			tmtime.tm_mday = day_begin;
			time_t time_begin = mktime(&tmtime);
			tmtime.tm_year = year_end-1900;
			tmtime.tm_mon = month_end-1;
			tmtime.tm_mday = day_end;
			time_t time_end = mktime(&tmtime);
			if (time_begin!=-1 && time_end!=-1)
			{
				double_time_begin = time_begin;
				double_time_end = time_end;
			}
		}
	}
	IntervalTimer::Attach(this,10000000/IntervalTimer::Resolution());
	open = true;
	return true;
}

void ReferenceManager::OnLogin(int roleid)
{
	if (!open)
		return;
	ReferrerMap::iterator itr = referrers.find(roleid);
	if (itr != referrers.end())
	{
		itr->second.OnLogin();
	}
	
//	PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
	PlayerInfo *pinfo = UserContainer::GetInstance().FindRole(roleid);
	if (pinfo==NULL || pinfo->user==NULL) return;

	ReferralMap::iterator it = referrals.find(roleid);
	if (it == referrals.end())
	{
		if (pinfo->user->real_referrer > 0)
		{
			Referrer &referrer = referrers[pinfo->user->real_referrer];
			if (!referrer.IsLoaded())
			{
				DBRefGetReferrer *rpc = (DBRefGetReferrer *)Rpc::Call(RPC_DBREFGETREFERRER, RoleId(pinfo->user->real_referrer));
				GameDBClient::GetInstance()->SendProtocol(rpc);
			}
			DBRefGetReferral *rpc = (DBRefGetReferral *)Rpc::Call(RPC_DBREFGETREFERRAL, RoleId(roleid));
			GameDBClient::GetInstance()->SendProtocol(rpc);
		}
	}
	else
	{
		it->second.SetLogout(false);
		if (pinfo->user->real_referrer > 0)
		{
			Referrer &referrer = referrers[pinfo->user->real_referrer];
			referrer.AddReferral(roleid);
			if (!referrer.IsLoaded())
			{
				DBRefGetReferrer *rpc = (DBRefGetReferrer *)Rpc::Call(RPC_DBREFGETREFERRER, RoleId(pinfo->user->real_referrer));
				GameDBClient::GetInstance()->SendProtocol(rpc);
			}
		}
	}
}

void ReferenceManager::OnLogout(int roleid)
{
	if (!open)
		return;
	ReferralMap::iterator itr = referrals.find(roleid);
	if (itr != referrals.end())
	{
		ReferrerMap::iterator it = referrers.find(itr->second.referrer);
		if (it != referrers.end()) it->second.DelReferral(roleid);
		if (itr->second.IsDirty())
			itr->second.SetLogout(true);
		else
			referrals.erase(itr);
	}
}

void ReferenceManager::OnLoadReferral(const GReferral &gref)
{
//	PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(gref.roleid);
	PlayerInfo *pinfo = UserContainer::GetInstance().FindRole(gref.roleid);
//	GRoleInfo *role = RoleInfoCache::Instance().Get(gref.roleid);
	if (pinfo==NULL || pinfo->user==NULL /*|| role==NULL*/)
		return;

	int referrer = pinfo->user->real_referrer;
	if (referrer > 0)
	{
		referrals[gref.roleid].OnLoad(gref, referrer);
		referrers[referrer].AddReferral(gref.roleid);
	}
}

void ReferenceManager::OnLoadReferrer(const GReferrer &referrer)
{
	ReferrerMap::iterator it = referrers.find(referrer.roleid);
	if (it != referrers.end())
		it->second.OnLoad(referrer);
}

bool ReferenceManager::Update()
{
	if (!referrals.empty())
	{
		ReferralMap::iterator it = referrals.lower_bound(referral_cursor);
		ReferralMap::iterator ie = referrals.end();
		int i = 0; 

		while (it!=ie && i < CHECKNUM_ONUPDATE)
		{
			if (it->second.IsDirty())
			{
				it->second.SyncDB();
				++it;
			}
			else if (it->second.IsLogout())
			{
				ReferrerMap::iterator itr = referrers.find(it->second.referrer);
				if (itr != referrers.end())
					itr->second.DelReferral(it->first);
				referrals.erase(it++);
			}
			else 
				++it;
			++i;
		}

		if (it != ie)
			referral_cursor = it->first;
		else
			referral_cursor = 0;
	}

	if (!referrers.empty())
	{
		ReferrerMap::iterator it = referrers.lower_bound(referrer_cursor);
		ReferrerMap::iterator ie = referrers.end();

		int i = 0;
		while (it!=ie && i < CHECKNUM_ONUPDATE)
		{
			if (it->second.IsDirty())
			{
				it->second.SyncDB();
				++it;
			}
			else if (it->second.IsEmpty())
				referrers.erase(it++);
			else
				++it;
			++i;
		}

		if (it != ie)
			referrer_cursor = it->first;
		else
			referrer_cursor = 0;
	}

	if (!transmap.empty())
	{
		WithdrawTransMap::iterator it = transmap.lower_bound(trans_cursor);
		WithdrawTransMap::iterator ie = transmap.end();

		int i = 0; 
		while (it!=ie && i < CHECKNUM_ONUPDATE)
		{
			if (referrers.find(it->first) == referrers.end())
			{
				if (it->second)
				{
					it->second->Rollback();
					delete it->second;
				}
				transmap.erase(it++);
			}
			else
				++it;
			++i;
		}

		if (it != ie)
			trans_cursor = it->first;
		else
			trans_cursor = 0;
	}
	return true;
}

void Referral::OnUseCash(int bonus_cont)
{
	if (bonus_total1 > INT_MAX-bonus_cont)
		bonus_total1 = INT_MAX;
	else
		bonus_total1 += bonus_cont;
	SetDirty(true);
}

void Referral::OnLevelUp(int64_t exp_cont)
{
	exp_total1 += exp_cont;
	SetDirty(true);
}

void ReferenceManager::OnReferralUseCash(int roleid, int bonus_cont)
{
	if (!open)
		return;
	ReferralMap::iterator it = referrals.find(roleid);
	if (it != referrals.end()) 
	{
		time_t now = Timer::GetTime();
		if (now>=double_time_begin && now<=double_time_end)
		{
			if (bonus_cont <= INT_MAX/2)
				bonus_cont *= 2;
			else
				bonus_cont = INT_MAX;
		}
		it->second.OnUseCash(bonus_cont);
	}
}

void ReferenceManager::OnReferralLevelUp(int roleid, int64_t exp, int level, int reborn_cnt)
{
	if (!open)
		return;
	ReferralMap::iterator it = referrals.find(roleid);
	if (it != referrals.end()) 
	{
		it->second.OnLevelUp(exp);
//		GRoleInfo *referral_info = RoleInfoCache::Instance().Get(roleid);
//		if (referral_info!=NULL && referral_info->level>=120)
		if (level >= 120)
		{
			int bonus_cont = (5*level-500)*(reborn_cnt+1);
			time_t now = Timer::GetTime();
			if (now>=double_time_begin && now<=double_time_end)
			{
				if (bonus_cont <= INT_MAX/2)
					bonus_cont *= 2;
				else
					bonus_cont = INT_MAX;
			}
			it->second.OnUseCash(bonus_cont);
		}
	}
}

#define BONUS_WITHDRAW_LIMIT(LEVEL) ((int)((LEVEL)<=90?60*(LEVEL):0.0002*(LEVEL)*(LEVEL)*(LEVEL)*(LEVEL)+5.23*(LEVEL)*(LEVEL)-798*(LEVEL)+32250))
#define EXP_WITHDRAW_LIMIT_NOREBORN(LEVEL) ((int64_t)((LEVEL)<75?(0.45*(LEVEL)*(LEVEL)+12)*(int64_t)10000:(8*(LEVEL)*(LEVEL)-1156*(LEVEL)+44300)*(int64_t)10000))
#define EXP_WITHDRAW_LIMIT_REBORN(LEVEL) ((int64_t)((LEVEL)<75?(0.45*(LEVEL)*(LEVEL)+12)*(int64_t)100000:(8*(LEVEL)*(LEVEL)-1156*(LEVEL)+44300)*(int64_t)100000))
#define EXP_WITHDRAW_LIMIT(LEVEL, REBORN_CNT) ((REBORN_CNT)<=0?EXP_WITHDRAW_LIMIT_NOREBORN(LEVEL):EXP_WITHDRAW_LIMIT_REBORN(LEVEL))
int ReferenceManager:: ListReferrals(int roleid, int start_index, int &total, int &bonus_avail_today, int64_t &exp_avail_today, std::vector<ReferralBrief> &referral_briefs)
{
	if (!open)
		return REF_ERR_REFERRERLOGOUT;
	RoleInfoCache &cache = RoleInfoCache::Instance();
	GRoleInfo *referrer_role = cache.Get(roleid);
	if (referrer_role == NULL) return REF_ERR_REFERRERLOGOUT;

	ReferrerMap::iterator it = referrers.find(roleid);
	if (it == referrers.end())
		return REF_ERR_NOREFERRALFOUND;	
	else if (!it->second.IsLoaded())
		return REF_ERR_REFERRERNOTLOADED;
	else if (IsInTransaction(roleid))
		return REF_ERR_REFERRERINTRANSACTION;
	else
	{
		Referrer &ref = it->second;
		//if (ref.IsEmpty()) return REF_ERR_NOREFERRALFOUND;
		Referrer::ReferralSet &referral_list = ref.referrals;
		total = (int)referral_list.size();
		Referrer::ReferralSet::const_iterator filter_it, filter_ite = referral_list.end();
		for (filter_it = referral_list.begin(); filter_it != filter_ite; ++filter_it)
		{
			//过滤掉在跨服中的下线 非Online状态 并且rinfo可能不在roleinfocache中
			//GRoleInfo * rinfo = cache.Get(*filter_it);
			PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(*filter_it);
			if (/*rinfo==NULL ||*/ pinfo==NULL)
				total--;
		}
		if (total <= 0)
			return REF_ERR_NOREFERRALFOUND;
		int list_num = 0;
		if (total > start_index)
		{
			list_num = REF_LIMIT_REFERRALPERPAGE;
			if (total-start_index < REF_LIMIT_REFERRALPERPAGE)
				list_num = total-start_index;
			referral_briefs.resize(list_num);
		}

		Referrer::ReferralSet::iterator itr = referral_list.begin(), ite = referral_list.end();
		int index = 0;
		int64_t total_exp_left = 0;
		bonus_avail_today = 0;
		for (int i = 0; itr != ite; ++i, ++itr)
		{
			GRoleInfo *referral_info = cache.Get(*itr);
			PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(*itr);
			ReferralMap::iterator itr2 = referrals.find(*itr);
			if (referral_info!=NULL && itr2!=referrals.end() && pinfo!=NULL)
			{
				int referral_level = referral_info->level+referral_info->reborn_cnt*200;
				int bonus_withdraw_limit = BONUS_WITHDRAW_LIMIT(referral_level);
				if (bonus_withdraw_limit > itr2->second.bonus_total1+itr2->second.bonus_total2)
					bonus_withdraw_limit = itr2->second.bonus_total1+itr2->second.bonus_total2;
				int referral_bonus_avail = bonus_withdraw_limit-itr2->second.bonus_withdraw; 
				if(referral_bonus_avail<0)
					referral_bonus_avail = 0;
				bonus_avail_today += referral_bonus_avail;

				int64_t referral_exp_left = itr2->second.exp_total1+itr2->second.exp_total2-itr2->second.exp_withdraw; 
				total_exp_left += referral_exp_left;

				if (index<list_num && i>=start_index)
				{
					ReferralBrief &brief = referral_briefs[index++];
					brief.rolename = referral_info->name;
					brief.level = referral_info->level;
					brief.sub_level = referral_info->reborn_cnt;
					brief.roleid = itr2->second.roleid;
					brief.bonus_total1 = itr2->second.bonus_total1;
					brief.bonus_total2 = itr2->second.bonus_total2;
					brief.bonus_left = itr2->second.bonus_total1+itr2->second.bonus_total2-itr2->second.bonus_withdraw;
					brief.bonus_avail = referral_bonus_avail;
					brief.exp_total1 = itr2->second.exp_total1;
					brief.exp_total2 = itr2->second.exp_total2;
					brief.exp_left = referral_exp_left;
				}
			}
		}

		int64_t exp_withdraw_limit = EXP_WITHDRAW_LIMIT(referrer_role->level, referrer_role->reborn_cnt);
		exp_withdraw_limit -= ref.exp_withdraw_today;
		exp_avail_today = exp_withdraw_limit>total_exp_left ? total_exp_left : exp_withdraw_limit;

		return REF_ERR_SUCCESS;
	}
}

void ReferenceManager::OnDBUpdateReferrer(int roleid)
{
	ReferrerMap::iterator it = referrers.find(roleid);
	if (it != referrers.end()) it->second.SetDirty(false);
}

void ReferenceManager::OnDBUpdateReferral(int roleid)
{
	ReferralMap::iterator it = referrals.find(roleid);
	if (it != referrals.end()) it->second.SetDirty(false);
}

int ReferenceManager::WithdrawExp(int roleid)
{
	GRoleInfo *referrer_role = RoleInfoCache::Instance().Get(roleid);
	if (referrer_role == NULL) return REF_ERR_REFERRERLOGOUT;

	ReferrerMap::iterator it = referrers.find(roleid);
	if (it == referrers.end())
		return REF_ERR_NOREFERRALFOUND;	
	else if (!it->second.IsLoaded())
		return REF_ERR_REFERRERNOTLOADED;
	else if (IsInTransaction(roleid))
		return REF_ERR_REFERRERINTRANSACTION;
	else
	{
		Referrer &ref = it->second;
		if (ref.IsEmpty()) return REF_ERR_NOREFERRALFOUND;
		int64_t exp_withdraw_limit = EXP_WITHDRAW_LIMIT(referrer_role->level, referrer_role->reborn_cnt);
		exp_withdraw_limit -= ref.exp_withdraw_today;

		Referrer::ReferralSet &referral_list = ref.referrals;
		Referrer::ReferralSet::iterator itr, ite = referral_list.end();
		WithdrawExpTrans *ptrans = new WithdrawExpTrans();
		int64_t total_exp_withdraw = 0;
		int withdraw_count = 0;
		for (itr = referral_list.begin(); itr!=ite && total_exp_withdraw<exp_withdraw_limit && withdraw_count < WITHDRAW_MAX; ++itr)
		{
			ReferralMap::iterator itr2 = referrals.find(*itr);
			PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(*itr);
			if (itr2 != referrals.end() && pinfo != NULL)
			{
				int64_t exp_to_withdraw = itr2->second.exp_total1+itr2->second.exp_total2-itr2->second.exp_withdraw;
				if (exp_to_withdraw > 0)
				{
					if (total_exp_withdraw+exp_to_withdraw > exp_withdraw_limit) 
						exp_to_withdraw = exp_withdraw_limit-total_exp_withdraw;

					total_exp_withdraw += exp_to_withdraw;
					ptrans->referral_withdraws[*itr] = itr2->second.exp_withdraw;
					itr2->second.exp_withdraw += exp_to_withdraw;
					itr2->second.SetDirty(true);
					withdraw_count++;
				}
			}
		}

		if (total_exp_withdraw > 0)
		{
			ptrans->referrer = roleid;
			ptrans->total_withdraw = ref.exp_withdraw_today;
			ptrans->to_withdraw = total_exp_withdraw;
			ref.exp_withdraw_today += total_exp_withdraw;
			ref.exp_withdraw_time = Timer::GetTime();
			ref.SetDirty(true);

			ReferralMap::iterator itr2 = referrals.find(roleid);
			if (itr2 != referrals.end())
			{
				ptrans->total2 = itr2->second.exp_total2;
				itr2->second.exp_total2 += (int64_t)(total_exp_withdraw*0.3);
				itr2->second.SetDirty(true);
			}

			if (ptrans->TrySubmit())
			{
				Log::formatlog("reference", "role(id=%d) withdraw exp %lld\n", roleid, total_exp_withdraw);
				transmap[roleid] = ptrans;
				return REF_ERR_SUCCESS;
			}
			else
			{
				ptrans->Rollback();
				delete ptrans;
				return REF_ERR_SUBMITTODB;
			}
		}
		else
		{
			delete ptrans;
			return REF_ERR_NOEXPAVAILABLE;
		}
	}
}

int ReferenceManager::WithdrawBonus(int roleid)
{
	RoleInfoCache &cache = RoleInfoCache::Instance();
	GRoleInfo *referrer_role = cache.Get(roleid);
	if (referrer_role == NULL) return REF_ERR_REFERRERLOGOUT;

	ReferrerMap::iterator it = referrers.find(roleid);
	if (it == referrers.end())
		return REF_ERR_NOREFERRALFOUND;	
	else if (!it->second.IsLoaded())
		return REF_ERR_REFERRERNOTLOADED;
	else if (IsInTransaction(roleid))
		return REF_ERR_REFERRERINTRANSACTION;
	else
	{
		Referrer &ref = it->second;
		if (ref.IsEmpty()) return REF_ERR_NOREFERRALFOUND;
		Referrer::ReferralSet &referral_list = ref.referrals;
		Referrer::ReferralSet::iterator itr, ite = referral_list.end();
		WithdrawBonusTrans *ptrans = new WithdrawBonusTrans();
		int total_bonus_withdraw = 0;
		int withdraw_count = 0;
		for (itr = referral_list.begin(); itr != ite && withdraw_count < WITHDRAW_MAX; ++itr)
		{
			GRoleInfo *referral_info = cache.Get(*itr);
			ReferralMap::iterator itr2 = referrals.find(*itr);
			PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(*itr);
			if (referral_info!=NULL && itr2!=referrals.end() && pinfo!=NULL)
			{
				int referral_level = referral_info->level+referral_info->reborn_cnt*200;
				int bonus_withdraw_limit = BONUS_WITHDRAW_LIMIT(referral_level);
				if (bonus_withdraw_limit > itr2->second.bonus_total1+itr2->second.bonus_total2)
					bonus_withdraw_limit = itr2->second.bonus_total1+itr2->second.bonus_total2;
				int referral_bonus_avail = bonus_withdraw_limit-itr2->second.bonus_withdraw; 

				if (referral_bonus_avail > 0)
				{
					total_bonus_withdraw += referral_bonus_avail;
					ptrans->referral_withdraws[*itr] = itr2->second.bonus_withdraw;
					itr2->second.bonus_withdraw += referral_bonus_avail;
					itr2->second.SetDirty(true);
					withdraw_count++;
				}
			}
		}

		if (total_bonus_withdraw > 0)
		{
			ptrans->referrer = roleid;
			ptrans->total_withdraw = ref.bonus_withdraw;
			ptrans->to_withdraw = total_bonus_withdraw;
			ref.bonus_withdraw += total_bonus_withdraw;
			ref.SetDirty(true);

			ReferralMap::iterator itr2 = referrals.find(roleid);
			if (itr2 != referrals.end())
			{
				ptrans->total2 = itr2->second.bonus_total2;
				itr2->second.bonus_total2 += total_bonus_withdraw/3;
				itr2->second.SetDirty(true);
			}

			if (ptrans->TrySubmit())
			{
				Log::formatlog("reference", "role(id=%d) withdraw bonus %d\n", roleid, total_bonus_withdraw);
				transmap[roleid] = ptrans;
				return REF_ERR_SUCCESS;
			}
			else
			{
				ptrans->Rollback();
				delete ptrans;
				return REF_ERR_SUBMITTODB;
			}
		}
		else
		{
			delete ptrans;
			return REF_ERR_NOBONUSAVAILABLE;
		}
	}
}

void ReferenceManager::OnDBWithdrawConfirm(int roleid)
{
	WithdrawTransMap::iterator it = transmap.find(roleid);
	if (it != transmap.end())
	{
		if (it->second) 
		{
			it->second->Confirm();
			delete it->second;
		}
		transmap.erase(it);
	}
}

void ReferenceManager::OnDBWithdrawRollback(int roleid)
{
	WithdrawTransMap::iterator it = transmap.find(roleid);
	if (it != transmap.end())
	{
		if (it->second) 
		{
			it->second->Rollback();
			delete it->second;
		}
		transmap.erase(it);
	}
}

void WithdrawExpTrans::Rollback()
{
	ReferenceManager *refman = ReferenceManager::GetInstance();
	Referrer *pref = refman->GetReferrer(referrer);
	if (pref != NULL) 
	{
		pref->exp_withdraw_today = total_withdraw;
		pref->SetDirty(false);
	}

	Referral *pfal = refman->GetReferral(referrer);
	if (pfal != NULL) 
	{
		pfal->exp_total2 = total2;
		pfal->SetDirty(false);
	}

	ReferralOldWithdrawMap::iterator it, ie = referral_withdraws.end();
	for (it = referral_withdraws.begin(); it != ie; ++it)
	{
		pfal = refman->GetReferral(it->first);
		if (pfal != NULL) 
		{
			pfal->exp_withdraw = it->second;
			pfal->SetDirty(false);
		}
	}
}

void WithdrawBonusTrans::Rollback()
{
	ReferenceManager *refman = ReferenceManager::GetInstance();
	Referrer *pref = refman->GetReferrer(referrer);
	if (pref != NULL) 
	{
		pref->bonus_withdraw = total_withdraw;
		pref->SetDirty(false);
	}

	Referral *pfal = refman->GetReferral(referrer);
	if (pfal != NULL) 
	{
		pfal->bonus_total2 = total2;
		pfal->SetDirty(false);
	}

	ReferralOldWithdrawMap::iterator it, ie = referral_withdraws.end();
	for (it = referral_withdraws.begin(); it != ie; ++it)
	{
		pfal = refman->GetReferral(it->first);
		if (pfal != NULL) 
		{
			pfal->bonus_withdraw = it->second;
			pfal->SetDirty(false);
		}
	}
}

void WithdrawExpTrans::OnConfirm()
{
	PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(referrer);
	if (pinfo != NULL)
		GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, SendRefAddExp(referrer, to_withdraw));
}

void WithdrawBonusTrans::OnConfirm()
{
	PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(referrer);
	if (pinfo != NULL)
		GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, SendRefAddBonus(referrer, to_withdraw));
}

};
