#include "kingdommanager.h"
#include "kingdombattlestart.hpp"
#include "kingdombattlestop.hpp"
#include "topmanager.h"
#include "dbkingdomload.hrp"
#include "gproviderserver.hpp"
#include "playerchangegs.hpp"
#include "maplinkserver.h"
#include "chatbroadcast.hpp"
#include "factionmanager.h"
#include "kingdominfosync.hpp"
#include "kingdompointsync.hpp"
#include "dbkingdomsetwinner.hrp"
#include "dbkingdomappoint.hrp"
#include "dbkingdomdischarge.hrp"
#include "dbkingdomsave.hrp"
#include "kingdomtitlechange.hpp"
#include "kingdomtitlesync.hpp"
#include "querygameserverattr_re.hpp"
#include "gmsetgameattri.hrp"
#include "kingdomgettitle_re.hpp"
#include "kingdompointchange.hpp"
#include "kingdomkingchange.hpp"
#include "kingguardinvite.hrp"
#include "queenopenbathinvite.hrp"
#include "queenopenbath.hpp"
#include "queenclosebath.hpp"
#include "dbkingdomtaskissue.hrp"
#include "dbkingdomtaskclose.hrp"
#include "dbkingdomtaskend.hrp"
#include "dbkinggetreward.hrp"
#include "commondata.h"

namespace GNET
{
KingdomBattleField::STUBMAP KingdomBattleField::stubmap;
MainKingdomBattle MainKingdomBattle::stub(KINGDOM_FIELD_MAIN, 0, 0);
SubKingdomBattle SubKingdomBattle::stub(KINGDOM_FIELD_SUB, 0, 0);

unsigned int KingdomBattle::ATTACKER_COUNT = 4;
int KingdomBattle::PREPARE_TIME = 300;

bool KingdomBattleField::SendStart(int _def, const Octets & def_name, const std::set<int> _attackers)
{
	std::vector<int> _attack(_attackers.begin(), _attackers.end());
	if (GProviderServer::GetInstance()->DispatchProtocol(GetGS(), 
			KingdomBattleStart(GetType(), GetTag(), _def,
				_attack, def_name)))
	{
		Log::formatlog("kingdom", "battle send start, type=%d tag=%d gs=%d defender=%d", GetType(), GetTag(), GetGS(), _def);
		std::vector<int>::iterator it, ite = _attack.end();
		for (it = _attack.begin(); it != ite; ++it)
			Log::formatlog("kingdom", "battle attacker %d", *it);
		//gs·µ»Ø³É¹¦Ê±ÔÙ SetBattleSide
//		SetBattleSide(_def, def_name, _attackers); //onfieldbegin();
		StateChange(BSTATE_SEND_OPEN);
		return true;
	}
	else
	{
		StateChange(BSTATE_OPEN_FAILED);
		return false;
	}
}

void KingdomBattleField::SendStop()
{
	GProviderServer::GetInstance()->DispatchProtocol(GetGS(), 
			KingdomBattleStop(GetType(), GetTag()));
	LOG_TRACE("kingdom battle send stop, type=%d tag=%d gs=%d",
			GetType(), GetTag(), GetGS());
	StateChange(BSTATE_IDLE);
	OnFieldEnd(-1); //Òì³£½áÊø »òÕßÕý³£Í¨Öª¶þ¼¶Õ½³¡½áÊø  virtual
}

bool KingdomBattleField::StartFight()
{
	if (b_state != BSTATE_PREPARE)
	{
		Log::log(LOG_ERR, "StartFight state %d invalid, tag %d",
				b_state, tagid);
		return false;
	}
	StateChange(BSTATE_FIRST_HALF);
	return true;
}

void KingdomBattleField::Update(time_t now)
{
	LOG_TRACE("kingdom battlefield state %d", b_state);
}

void KingdomBattleField::OnStart(int ret, int tag, int def, const std::vector<int> & att, const Octets & def_name)
{
	if (tag != tagid)
	{
		Log::log(LOG_ERR, "BattleField on start tagid %d(%d) not match, ret %d",
				tag, tagid, ret);
		return;
	}
	if (b_state != BSTATE_SEND_OPEN)
	{
		Log::log(LOG_ERR, "BattleField on start state %d invalid, tag %d ret %d",
				b_state, tag, ret);
		return;
	}
	if (ret == ERR_SUCCESS)
	{
		StateChange(BSTATE_PREPARE);
		SetBattleSide(def, def_name, att);
		Log::formatlog("kingdom", "battle start success, type=%d tag=%d gs=%d defender=%d", GetType(), GetTag(), GetGS(), def);
	}
	else
	{
		StateChange(BSTATE_OPEN_FAILED);
		Log::formatlog("kingdom", "battle start err %d, type=%d tag=%d gs=%d defender=%d", ret, GetType(), GetTag(), GetGS(), def);
	}
}

bool KingdomBattleField::OnDisconnect()
{
	Log::log(LOG_ERR, "kingdom battle gs disconnect, tag %d gs %d", GetTag(), GetGS());
	StateChange(BSTATE_IDLE);
	OnFieldEnd(-1);
	return dominant; // game over or not
}

void MainKingdomBattle::OnFieldEnd(int win_attacker)
{
	//Ò²ÓÐ¿ÉÄÜÊÇÒì³£Ê§°ÜÇé¿öµ¼ÖÂ½áÊø 
//	SecondHalfEndAnnounce(win_attacker);
	ClearAttackerFacs();
}

void SubKingdomBattle::OnFieldEnd(int win_attacker)
{
	battleroles.clear();
}

bool KingdomBattleField::OnEnd(int win_attacker)
{
	/*
	if (tag != tagid)
	{
		Log::log(LOG_ERR, "OnEnd tagid %d(%d) not match",
				tag, tagid);
		return false;
	}
	*/
//	Log::formatlog("onkingdombattleend", "tag %d gs %d winner %d", GetTag(),
//			GetGS(), winner);
	if (GetState() != BSTATE_PREPARE && GetState() != BSTATE_FIRST_HALF && GetState() != BSTATE_SECOND_HALF)
	{
		Log::log(LOG_ERR, "OnEnd tagid %d state %d invalid", GetTag(), GetState());
		return false;
	}
	OnFieldEnd(win_attacker);//virtual Õý³£½áÊø
	StateChange(BSTATE_IDLE);
	Log::formatlog("kingdom", "battle end, tagid %d", GetTag());
	return dominant; // game over
}

void MainKingdomBattle::FirstHalfEndAnnounce(char res)
{
	ChatBroadCast chat;
	chat.channel = GP_CHAT_SYSTEM;
	chat.srcroleid = MSG_KINGDOM_FIRSTHALF_END;
	Marshal::OctetsStream msg;
	/*
	Octets defender_name;
	if (defender != 0)
		FactionManager::Instance()->GetName(defender, defender_name);
		*/
	msg<<res<<defender<<def_fac_name;
	Octets attacker_name;
	std::map<int, BattleSide>::iterator ait, aite = attackers.end();
	for (ait = attackers.begin(); ait != aite; ++ait)
	{
		FactionManager::Instance()->GetName(ait->first, attacker_name);
		msg<<attacker_name;
	}
	chat.msg = msg;
	LOG_TRACE("Kingdom Battle End Announce res %d defender %d defendername.size %d", res, defender, def_fac_name.size());
	LinkServer::GetInstance().BroadcastProtocol(chat);
}

/*
void MainKingdomBattle::SecondHalfEndAnnounce(int win_attacker)
{
	ChatBroadCast chat;
	chat.channel = GP_CHAT_SYSTEM;
	chat.srcroleid = MSG_KINGDOM_SECONDHALF_END;
	Marshal::OctetsStream msg;
	Octets win_fac_name, new_king_name;
	FactionManager::Instance()->GetName(win_attacker, win_fac_name);
	int warning;
	msg<<win_fac_name<<new_king_name;
	chat.msg = msg;
	LOG_TRACE("Kingdom Battle End Announce win_fac_name.size %d new_king_name.size %d", win_fac_name.size(), new_king_name.size());
	LinkServer::GetInstance().BroadcastProtocol(chat);
}
*/

void KingdomInfo::NewWinnerAnnounce(const Octets & win_fac_name, const Octets & new_king_name)
{
	ChatBroadCast chat;
	chat.channel = GP_CHAT_SYSTEM;
	chat.srcroleid = MSG_KINGDOM_NEW_WINNER;
	Marshal::OctetsStream msg;
	msg<<win_fac_name<<new_king_name;
	chat.msg = msg;
	LOG_TRACE("Kingdom Battle End Announce win_fac_name.size %d new_king_name.size %d", win_fac_name.size(), new_king_name.size());
	LinkServer::GetInstance().BroadcastProtocol(chat);
}

bool MainKingdomBattle::OnHalf(int tag, char res, std::vector<int> _failers)
{
	if (tag != GetTag())
	{
		Log::log(LOG_ERR, "OnHalf tagid %d(%d) not match",
				tag, GetTag());
		return false;
	}
	if (GetState() != BSTATE_PREPARE && GetState() != BSTATE_FIRST_HALF)
	{
		Log::log(LOG_ERR, "OnHalf tagid %d state %d invalid", GetTag(), GetState());
		return false;
	}
	Log::formatlog("onkingdombattlehalf", "tag %d gs %d res %d defender %d", GetTag(),
			GetGS(), res, defender);
	FirstHalfEndAnnounce(res);
	if (res == KINGDOM_DEFENDER_WIN)
	{
		OnFieldEnd(defender);//ClearAttackerFacs();
		StateChange(BSTATE_IDLE);
		return true;
	}
	else if (res == KINGDOM_ATTACKER_WIN)
	{
		//Ë«ÖØÑéÖ¤Ê§°ÜµÄÌôÕ½°ï
		std::vector<int>::const_iterator it, ite = _failers.end();
		for (it = _failers.begin(); it != ite; ++it)
			OnAttackerFail(tag, *it);
			//attackers.erase(*it);
		defenderside.fail = true;
		StateChange(BSTATE_SECOND_HALF);
		LOG_TRACE("kingdom half, attacker win, fail acttacker num %d", _failers.size());
		return true;
	}
	Log::log(LOG_ERR, "kingdom half, unknown res %d", res);
	return false;
}

int MainKingdomBattle::Enter(PlayerInfo * pinfo)
{
	if (GetState() != BSTATE_PREPARE && GetState() != BSTATE_FIRST_HALF && GetState() != BSTATE_SECOND_HALF)
		return ERR_KINGDOM_BATTLE_STATE;
	if (IsRoleIn(pinfo->roleid))
		return ERR_KINGDOM_BATTLE_STILL_IN;
	if (pinfo->factionid <= 0)
		return ERR_KINGDOM_INVALID_FAC;
	if ((int)pinfo->factionid == defender)
	{
		if (defenderside.fail)
			return ERR_KINGDOM_ALREADY_FAIL;
		defenderside.roles.insert(pinfo->roleid);
		if (defenderside.roles.size() > DEFENDER_LIMIT)
		{
			defenderside.roles.erase(pinfo->roleid);
			return ERR_KINGDOM_ROLE_LIMIT;
		}
		GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, 
				PlayerChangeGS(pinfo->roleid, pinfo->localsid, GetGS(), GetTag()));
		return ERR_SUCCESS;
	}
	std::map<int, BattleSide>::iterator it = attackers.find(pinfo->factionid);
	if (it == attackers.end())
		return ERR_KINGDOM_INVALID_FAC;
	if (it->second.fail)
		return ERR_KINGDOM_ALREADY_FAIL;
	it->second.roles.insert(pinfo->roleid);
	if (it->second.roles.size() > ATTACKER_LIMIT)
	{
		it->second.roles.erase(pinfo->roleid);
		return ERR_KINGDOM_ROLE_LIMIT;
	}
	GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, 
			PlayerChangeGS(pinfo->roleid, pinfo->localsid, GetGS(), GetTag()));
	return ERR_SUCCESS;
}

int MainKingdomBattle::TryLeave(int tag, int roleid)
{
	if (tag != GetTag())
	{
		Log::log(LOG_ERR, "Role %d Leave kingdom battle, tagid %d(%d) not match",
				roleid, tag, GetTag());
		return ERR_KINGDOM_INVALID_TAG;
	}
	if (!IsRoleIn(roleid))
		return ERR_KINGDOM_NOTIN;
	return ERR_SUCCESS;
}

void MainKingdomBattle::DelRole(int roleid)
{
	if (defenderside.roles.erase(roleid) > 0)
		return;
	std::map<int, BattleSide>::iterator it, ite = attackers.end();
	for (it = attackers.begin(); it != ite; ++it)
	{
		if (it->second.roles.erase(roleid) > 0)
			return;
	}
}

void SubKingdomBattle::DelRole(int roleid)
{
	battleroles.erase(roleid);
}

bool MainKingdomBattle::IsRoleIn(int roleid)
{
	if (defenderside.roles.find(roleid) != defenderside.roles.end())
		return true;
	std::map<int, BattleSide>::iterator it, ite = attackers.end();
	for (it = attackers.begin(); it != ite; ++it)
	{
		if (it->second.roles.find(roleid) != it->second.roles.end())
			return true;
	}
	return false;
}

bool SubKingdomBattle::IsRoleIn(int roleid)
{
	return battleroles.find(roleid) != battleroles.end();
}

void MainKingdomBattle::Update(time_t now)
{
	KingdomBattleField::Update(now);
	if (GetState() == BSTATE_PREPARE || GetState() == BSTATE_FIRST_HALF || GetState() == BSTATE_SECOND_HALF)
	{
		LOG_TRACE("MainKingdomBattle defender %d roles %d",
				defender, defenderside.roles.size());
		std::map<int/*factionid*/, BattleSide>::const_iterator it, ite = attackers.end();
		for (it = attackers.begin(); it != ite; ++it)
			LOG_TRACE("attacker %d roles %d", it->first, it->second.roles.size());
	}
	for (std::multimap<int, int>::iterator it=offlineroles.begin(); it!=offlineroles.end();)
	{
		if (it->first >= now)
			break;
		LOG_TRACE("kingdom battle erase offline role %d", it->second);
		DelRole(it->second);
		offlineroles.erase(it++);
	}
}

void SubKingdomBattle::Update(time_t now)
{
	KingdomBattleField::Update(now);
	if (GetState() == BSTATE_PREPARE || GetState() == BSTATE_FIRST_HALF || GetState() == BSTATE_SECOND_HALF)
	{
		LOG_TRACE("SubKingdomBattle roles %d", battleroles.size());
	}
}

void KingdomBattleField::EnterFail(int roleid)
{
	DelRole(roleid);
}

void KingdomBattleField::Leave(int roleid)
{
	DelRole(roleid);
}

void MainKingdomBattle::OnLogout(int roleid)
{
	time_t now = Timer::GetTime();
	LOG_TRACE("kingdom battle role %d offline", roleid);
	offlineroles.insert(std::make_pair(now + PUNISH_TIME, roleid));
}

void SubKingdomBattle::OnLogout(int roleid)
{
	DelRole(roleid);
}

int SubKingdomBattle::Enter(PlayerInfo * pinfo)
{
	if (/*GetState() != BSTATE_PREPARE &&*/ GetState() != BSTATE_FIRST_HALF)
		return ERR_KINGDOM_BATTLE_STATE;
	if (IsRoleIn(pinfo->roleid))
		return ERR_KINGDOM_BATTLE_STILL_IN;
	if (battleroles.size() >= BATTLE_LIMIT)
		return ERR_KINGDOM_SUB_LIMIT;
	battleroles.insert(pinfo->roleid);
	GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, 
			PlayerChangeGS(pinfo->roleid, pinfo->localsid, GetGS(), GetTag()));
	return ERR_SUCCESS;
}
//////////////////////////////
void KingdomBattle::Register(int gsid, char type, int tag)
{
	KingdomBattleField * field = fields[type];
	if (field != NULL)
	{
		Log::log(LOG_ERR, "kingdom battle field already exists, gsid=%d, tagid=%d, type=%d, newgsid=%d, newtag=%d",	field->GetGS(), field->GetTag(), type, gsid, tag);
		return;
	}
	field = KingdomBattleField::Create(type, gsid, tag);
	if (field == NULL)
	{
		Log::log(LOG_ERR, "unknown kingdom battle type %d, gsid %d tag %d",
				type, gsid, tag);
		return;
	}
	fields[type] = field;
	LOG_TRACE("register kingdom battle, type %d gsid %d tag %d", type, gsid, tag);
}

void KingdomBattle::OnKingdomReady()
{
	StateChange(STATE_WAIT_OPEN);
}

time_t KingdomBattle::GetNextSundayNight(time_t now)
{
	time_t timestamp = 0;
	struct tm dt;
	localtime_r(&now, &dt);
	dt.tm_sec = 0;
	dt.tm_min = 0;
	dt.tm_hour = 0;
	time_t t_base = mktime(&dt);
	if (t_base == -1)
		return 0;
	//ÖÜÈÕÁè³¿Áãµã
	t_base -= 86400*dt.tm_wday;
	if (now < t_base + START_TIME)
		timestamp = t_base + START_TIME;
	else
		timestamp = t_base + START_TIME + WEEK_SECONDS;
	return timestamp;
}

bool KingdomBattle::IsLastSundayOfMon(time_t sunday) //²ÎÊýÎªÄ³¸öÖÜÈÕÍí8:00µÄÊ±¼ä´Á ÅÐ¶Ï¸ÃÖÜÈÕÊÇ·ñÎªµ±ÔÂ×îºóÒ»¸ö ÖÜÈÕ
{
	struct tm dt;
	localtime_r(&sunday, &dt);
	
	int this_mon = dt.tm_mon; 
	dt.tm_mday += 7;
	time_t weeklater = mktime(&dt);
	if (weeklater == -1)
		return false;
	localtime_r(&weeklater, &dt);
	return (dt.tm_mon != this_mon);
}

void KingdomBattle::StateChange(int st)
{
	time_t now = Timer::GetTime();
	int old_state = state;
	time_t old_timeout = state_timeout;
	switch(st)
	{
	case STATE_WAIT_OPEN:
	{
		/*
		time_t t_base = 0;
		struct tm dt;
		localtime_r(&now, &dt);
		dt.tm_sec = 0;
		dt.tm_min = 0;
		dt.tm_hour = 0;
		//ÖÜÈÕÁè³¿Áãµã
		t_base = mktime(&dt)-86400*dt.tm_wday;
		if (now < t_base + START_TIME)
			state_timeout = t_base + START_TIME;
		else
			state_timeout = t_base + START_TIME + 604800;
			*/
		if (special_start_time != 0 && now < special_start_time)
			state_timeout = special_start_time;
		else
			state_timeout = GetNextSundayNight(now);
	}
	break;
	case STATE_SEND_OPEN:
	{
		state_timeout = now + SWITCH_TIME;
	}
	break;
	case STATE_PREPARE:
	{
		state_timeout = now + PREPARE_TIME;
	}
	break;
	case STATE_FIGHTING:
	{
		state_timeout = now + FIGHTING_MAX_TIME;
	}
	break;
	default:
	{
		Log::log(LOG_ERR, "unknown kingdom state to %d,cur_state %d cur_timeout %ld",
				st, old_state, old_timeout);
		return;
	}
	break;
	}
	state = st;
	LOG_TRACE("kingdom state change from %d timeout %d to newstate %d timeout %s",
			old_state, old_timeout, state, ctime(&state_timeout));
}

bool KingdomBattle::SendStart()
{
	if (!parent->IsReady())
	{
		Log::log(LOG_ERR, "kingdom try start battle, but kingdom data not load");
		return false;
	}
	if (!GameDBClient::GetInstance()->IsConnect())
	{
		Log::log(LOG_ERR, "kingdom try start battle, but gamedbd is disconnected");
		return false;
	}
	std::set<int> attackers;
	GTopTable table;
	if (debug_attackers.size())
		attackers = debug_attackers;
	else
	{
		TopManager::Instance()->GetDailyTable(TOP_FACTION_LEVEL, table);
		std::vector<GTopItem>::const_iterator tit, tite = table.items.end();
		for (tit = table.items.begin(); tit != tite && attackers.size() < ATTACKER_COUNT; tit++)
		{
			if ((int)tit->id != parent->GetKingFaction())
				attackers.insert(tit->id);
		}
	}
	if (attackers.size() != ATTACKER_COUNT)
	{
		Log::log(LOG_ERR, "kingdom try start battle, but attacker count %d invalid, top count %d",
				attackers.size(), table.items.size());
		return false;
	}
	for (char t = KINGDOM_FIELD_MAIN; t < KINGDOM_FIELD_COUNT; t++)
	{
		if (fields[t] == NULL)
		{
			Log::log(LOG_ERR, "kingdom try start battle, but field type %d unregistered",	t);
			return false;
		}
	}
	FieldList::iterator it, ite = fields.end();
	for (it = fields.begin(); it != ite; ++it)
	{
		if (it->second != NULL) //×¢ÒâÅÐ¶ÏÖ¸Õë²»Îª NULL
		{
			if (!it->second->SendStart(parent->GetKingFaction(), parent->GetKingFactionName(), attackers))
				return false;
		}
	}
	return true;
}

void KingdomBattle::StartFight()
{
	FieldList::iterator it, ite = fields.end();
	for (it = fields.begin(); it != ite; ++it)
	{
		if (it->second != NULL)
			it->second->StartFight();
	}
}

void KingdomBattle::SendStop(char exceptiontype)
{
	FieldList::iterator it, ite = fields.end();
	for (it = fields.begin(); it != ite; ++it)
	{
		if (it->second != NULL && it->second->GetType() != exceptiontype) //×¢ÒâÅÐ¶ÏÖ¸Õë²»Îª NULL
			it->second->SendStop();
	}
}

void KingdomBattle::Update(time_t now)
{
	LOG_TRACE("kingdom state %d timeout %s", state, ctime(&state_timeout));
	FieldList::iterator it, ite = fields.end();
	for (it = fields.begin(); it != ite; ++it)
	{
		if (it->second != NULL)
			it->second->Update(now);
	}
	if (state_timeout != 0 && now > state_timeout)
	{
		LOG_TRACE("kingdom state %d timeout", state);
		switch(state)
		{
		case STATE_WAIT_OPEN:
		{
			if (debug_start || special_start_time != 0 || IsLastSundayOfMon(now))
			{
				if (SendStart())
					StateChange(STATE_SEND_OPEN);
				else
				{
					SendStop();
					StateChange(STATE_WAIT_OPEN);
				}
				debug_start = false;
				special_start_time = 0;
			}
			else
			{
				state_timeout = GetNextSundayNight(now);
				LOG_TRACE("not last sunday of mon, adjust state_timeout to %s", ctime(&state_timeout));
			}
		}
		break;
		case STATE_SEND_OPEN:
		{
			Log::log(LOG_ERR, "kingdom send start timeout");
			if (CheckFieldsReady())
				StateChange(STATE_PREPARE);
			else
			{
				SendStop();
				StateChange(STATE_WAIT_OPEN);
			}
		}
		break;
		case STATE_PREPARE:
		{
			StartFight();
			StateChange(STATE_FIGHTING);
		}
		break;
		case STATE_FIGHTING:
		{
			Log::log(LOG_ERR, "kingdom fighting timeout");
			SendStop();
			StateChange(STATE_WAIT_OPEN);
		}
		break;
		default:
		{
			Log::log(LOG_ERR, "unknown kingdom cur_state %d cur_timeout %ld",
				state, state_timeout);
		}
		break;
		}
//		LOG_TRACE("kingdom state trans to %d", state);
	}
	else if (state_timeout == 0)
		Log::log(LOG_ERR, "kingdom state %d timeout is 0", state);
}

bool KingdomBattle::CheckFieldsReady()
{
	FieldList::iterator it, ite = fields.end();
	for (it = fields.begin(); it != ite; ++it)
	{
		if (it->second != NULL) //×¢ÒâÅÐ¶ÏÖ¸Õë²»Îª NULL
		{
			if (!it->second->CheckReady())
				return false;
		}
	}
	return true;
}

void KingdomBattle::OnBattleStart(int ret, char fieldtype, int tag, int def, const std::vector<int> & attackers, const Octets & def_name)
{
	KingdomBattleField * field = fields[fieldtype];
	if (field == NULL)
	{
		Log::log(LOG_ERR, "OnKingdomBattleStart but fieldtype %d is empty,ret %d tag %d", fieldtype, ret, tag);
		return;
	}
	field->OnStart(ret, tag, def, attackers, def_name);
	if (CheckFieldsReady()) //¼ì²éÊÇ·ñÈ«²¿Õ½³¡ÒÑ¾­¿ªÆô³É¹¦
		StateChange(STATE_PREPARE);
}

void KingdomBattle::OnBattleAttackerFail(char fieldtype, int tag, int failer)
{
	KingdomBattleField * field = fields[fieldtype];
	if (field == NULL)
	{
		Log::log(LOG_ERR, "OnKingdomBattleAttackerFail but fieldtype %d is empty,tag %d", fieldtype, tag);
		return;
	}
	field->OnAttackerFail(tag, failer);
}

/*
void KingdomBattle::EndAnnounce(char res, int newkingfaction)
{
	ChatBroadCast chat;
	chat.channel = GP_CHAT_SYSTEM;
	chat.srcroleid = MSG_KINGDOM_BATTLE_END;
	Marshal::OctetsStream msg;
	Octets winner_name;
	if (newkingfaction != 0)
		FactionManager::Instance()->GetName(newkingfaction, winner_name);
	msg<<res<<newkingfaction<<winner_name;
	chat.msg = msg;
	LOG_TRACE("Kingdom Battle End Announce res %d winner %d winnername.size %d", res, newkingfaction, winner_name.size());
	LinkServer::GetInstance().BroadcastProtocol(chat);
}
*/
bool KingdomBattle::OnBattleHalf(char fieldtype, int tag, char res, std::vector<int> _failers)
{
	KingdomBattleField * field = fields[fieldtype];
	if (field == NULL)
	{
		Log::log(LOG_ERR, "OnKingdomBattleHalf but fieldtype %d is empty,tag %d", fieldtype, tag);
		return false;
	}
	if (!field->OnHalf(tag, res, _failers))
		return false;
	//Ö÷Õ½³¡µÚÒ»½×¶Î½áÊøÖ®ºó ¼´¿É¹Ø±ÕËùÓÐ´Î¼¶Õ½³¡
	SendStop(fieldtype);
	if (res == KINGDOM_DEFENDER_WIN)
		StateChange(STATE_WAIT_OPEN);
	return true;
}

bool KingdomBattle::OnBattleEnd(char fieldtype, int tag, int winner)
{
	KingdomBattleField * field = fields[fieldtype];
	if (field == NULL)
	{
		Log::log(LOG_ERR, "OnKingdomBattleEnd but fieldtype %d is empty,tag %d", fieldtype, tag);
		return false;
	}
	if (!field->OnEnd(winner))
		return false;
	//SendStop(fieldtype); //Ö÷Õ½³¡µÚÒ»½×¶Î½áÊøÊ±ÒÑ¾­Í¨Öª´Î¼¶Õ½³¡¹Ø±Õ ´Ë´¦ÎÞÐëÔÙ
	//		EndAnnounce(MainKingdomBattle::ATTACKER_WIN, winner);
	StateChange(STATE_WAIT_OPEN);
	return true;
}

int KingdomBattle::TryEnterBattle(char type, PlayerInfo * pinfo)
{
	if (state != STATE_PREPARE && state != STATE_FIGHTING)
		return ERR_KINGDOM_BATTLE_STATE;
	//¿Í»§¶Ë·¢À´µÄÊý¾Ý ÑÏ¸ñ¼ì²é£¡
	FieldList::iterator it = fields.find(type);
	if (it == fields.end())
		return ERR_KINGDOM_FIELD_UNREGISTER;
	return it->second->Enter(pinfo);
}

int KingdomBattle::TryLeaveBattle(char type, int tag, int roleid )
{
	FieldList::iterator it = fields.find(type);
	if (it == fields.end())
		return ERR_KINGDOM_FIELD_UNREGISTER;
	return it->second->TryLeave(tag, roleid);
}

void KingdomBattle::OnChangeGS(int roleid, int retcode, int old_gs, int old_map, int new_gs, int new_map)
{
	FieldList::iterator it, ite = fields.end();
	for (it = fields.begin(); it != ite; ++it)
	{
		if (it->second == NULL)
			continue;
		if (it->second->GetGS()==old_gs && it->second->GetTag()==old_map)
		{
			//³ö¹úÕ½
			if (retcode == ERR_SUCCESS)
				it->second->Leave(roleid);
			break;
		}
		if (it->second->GetGS()==new_gs && it->second->GetTag()==new_map)
		{
			//½ø¹úÕ½
			if (retcode != ERR_SUCCESS)
				it->second->EnterFail(roleid);
			break;
		}
	}
}

void KingdomBattle::OnLoginBattle(int roleid, int retcode, int old_gs, int old_map, int new_gs, int new_map)
{
	FieldList::iterator it, ite = fields.end();
	for (it = fields.begin(); it != ite; ++it)
	{
		if (it->second == NULL)
			continue;
		if (it->second->GetGS()==new_gs && it->second->GetTag()==new_map)
		{
			//½ø¹úÕ½
			if (retcode != ERR_SUCCESS)
				it->second->EnterFail(roleid);
			break;
		}
	}
}

void KingdomBattle::OnLogout(int roleid, int gsid, int map)
{
	FieldList::iterator it, ite = fields.end();
	for (it = fields.begin(); it != ite; ++it)
	{
		if (it->second == NULL)
			continue;
		if (it->second->GetGS()==gsid && it->second->GetTag()==map)
		{
			it->second->OnLogout(roleid);
			break;
		}
	}
}

void KingdomBattle::OnDisconnect(int gsid)
{       
	bool gameover = false;
	FieldList::iterator it, ite = fields.end();
	for (it = fields.begin(); it != ite; ++it)
	{
		if (it->second == NULL)
			continue;
		if (it->second->GetGS() == gsid)
		{
			if (it->second->OnDisconnect())
				gameover = true;
			delete it->second;
			it->second = NULL;
		}
	}
	if (gameover)
	{
		SendStop();
		StateChange(STATE_WAIT_OPEN);
	}
}

void KingdomBattle::DebugAddAttacker(int fid)
{
	if (debug_attackers.size() < ATTACKER_COUNT)
	{
		debug_attackers.insert(fid);
	}
	LOG_TRACE("debugaddattacker fid=%d total count %d", fid, debug_attackers.size());
}

void KingdomBattle::DebugClearAttacker()
{
	debug_attackers.clear();
	LOG_TRACE("debugclearattacker");
}

void KingdomBattle::DebugStartBattle()
{
	LOG_TRACE("debugstartbattle");
	if (state == STATE_WAIT_OPEN)
	{
		state_timeout = Timer::GetTime();
		debug_start = true;
	}
	else
		Log::log(LOG_ERR, "debugstartbattle, but battle is ongoing, state %d",
				state);
}

/////////////////////////////////

void KingdomInfo::CloseTask()
{
	if (task_status != KINGDOM_TASK_OPEN)
		return;
	Log::formatlog("kingdom", "try close task type %d time %d", task_type, task_issue_time);
	DBKingdomTaskClose * rpc = (DBKingdomTaskClose *)Rpc::Call(RPC_DBKINGDOMTASKCLOSE, DBKingdomTaskCloseArg(0));
	rpc->type = task_type;
	rpc->time = task_issue_time;
	GameDBClient::GetInstance()->SendProtocol(rpc);

	task_status = KINGDOM_TASK_WAIT_BALANCE;
	task_type = 0;
	SyncKingdomInfo();
}

void KingdomInfo::EndTask()
{
	if (task_status != KINGDOM_TASK_WAIT_BALANCE)
		return;
	CommonDataMan * pcd = CommonDataMan::Instance();
	int _task_points = 0;
	pcd->SimpleGet(COMMON_DATA_KINGTASK_WINPOINTS, _task_points);
	Log::formatlog("kingdom", "try end task type %d time %d win_points %d", task_type, task_issue_time, _task_points);
	DBKingdomTaskEnd * rpc = (DBKingdomTaskEnd *)Rpc::Call(RPC_DBKINGDOMTASKEND, DBKingdomTaskEndArg(0));
	rpc->type = task_type;
	rpc->time = task_issue_time;
	rpc->win_points = _task_points;
	GameDBClient::GetInstance()->SendProtocol(rpc);

	if (_task_points > KINGDOM_TASK_POINTS_LIMIT)
		_task_points = KINGDOM_TASK_POINTS_LIMIT;
	if (_task_points > 0)
	{
		OnPointChange(_task_points);
		SetDirty(true);
	}
	task_status = KINGDOM_TASK_NONE;
	task_points = 0;
	pcd->SimplePut(COMMON_DATA_KINGTASK_WINPOINTS, 0, true);

	//		SyncKingdomInfo();
	//int warning; //ÊÇ·ñÐèÒªÐÞ¸ÄÈ«¾ÖissuetimeÈ«¾Ö±äÁ????
	//		SetGettingTaskPoint(true);
}

void KingdomInfo::Update(time_t now)
{
	//ÒòÎªºÏ·þÊ±Ã»ÓÐ´¦ÀíÈ«¾Ö±äÁ¿ Ö»´¦ÀíÁËÁË kingdom ±í£¬ËùÒÔÐèÒªÃ¿´ÎÆô¶¯Ê± ¸ù¾Ý Kingdom ±íÊý¾ÝÀ´Í¬²½È«¾Ö±äÁ¿Öµ
	CommonDataMan * pcd = CommonDataMan::Instance();
	if (!IsReady()) //µÈ´ý: 1 Êý¾Ý¿â¼ÓÔØ; 2 ½«ÈÎÎñÏà¹ØÊý¾ÝÍ¬²½ÖÁÈ«¾Ö±äÁ¿
	{
		if (IsLoad() && pcd->IsOpen())
		{
			Log::formatlog("kingdom", "sync task_type %d task_issue_time %d task_points %d", task_type, task_issue_time, task_points);
			pcd->SimplePut(COMMON_DATA_KINGTASK_ISSUETIME, task_issue_time, true);
			pcd->SimplePut(COMMON_DATA_KINGTASK_WINPOINTS, task_points, true);
			db_status |= SK_SYNC_COMMONDATA;
			battle.OnKingdomReady();
			SyncKingdomInfo();
		}
		return;
	}

	battle.Update(now);

	if (task_status != KINGDOM_TASK_NONE)
	{
		if (task_status == KINGDOM_TASK_OPEN && now > task_issue_time + TASK_OPEN_PERIOD)
		{
			CloseTask();
		}
		else if (task_status == KINGDOM_TASK_WAIT_BALANCE && now > task_issue_time + TASK_LAST_PERIOD
				&& pcd->IsOpen())
		{
			EndTask();
		}
	}

	std::map<int, KingdomAttrSet *>::iterator it, ite = game_attr.end();
	for (it = game_attr.begin(); it != ite; ++it)
	{
		if (it->second != NULL)
			it->second->Update(now);
	}

	if (pcd->IsOpen()) 
	{
		int win_points = 0;
		if (pcd->SimpleGet(COMMON_DATA_KINGTASK_WINPOINTS, win_points) && task_points != win_points)
		{
			LOG_TRACE("kingdom sync task_points from %d to %d", task_points, win_points);
			task_points = win_points;
			SetDirty(true);
			if (task_status == KINGDOM_TASK_NONE && task_points != 0)
				Log::log(LOG_ERR, "kingdom invalid task_points %d", task_points);
		}
	}

	if (IsDirty())
	{
		DBKingdomSaveArg arg;
		GetSaveInfo(arg.info);
		DBKingdomSave * rpc = (DBKingdomSave *)Rpc::Call(RPC_DBKINGDOMSAVE, arg);
		if (GameDBClient::GetInstance()->SendProtocol(rpc))
			SetDirty(false);
		LOG_TRACE("save kingdom,points=%d task_points=%d", points, task_points);
	}
}

void KingdomInfo::SyncKingdomInfo()
{
	LOG_TRACE("SyncKingdomInfo to all gs");
	if (IsReady())
	{
		KingdomInfoSync sync;
		GetInfoForGS(sync.info);
		GProviderServer::GetInstance()->BroadcastProtocol(sync);
	}
}

void KingdomInfo::SyncKingdomInfo(unsigned int sid)
{
	LOG_TRACE("SyncKingdomInfo to gs, sid=%d", sid);
	if (IsReady())
	{
		KingdomInfoSync sync;
		GetInfoForGS(sync.info);
		GProviderServer::GetInstance()->Send(sid, sync);
	}
}

void KingdomInfo::OnPointChange(int delta)
{
	if (!IsReady())
	{
		Log::log(LOG_ERR, "kingdompointchange delta=%d, but data is not load", delta);
		return;
	}
	points += delta;
	SetDirty(true);
	Log::formatlog("kingdompointchange", "value is %d after delta %d", points, delta);
	if (points < 0)
		Log::log(LOG_ERR, "kingdompointchange, invalid value %d after delta %d", points, delta);

	GProviderServer::GetInstance()->BroadcastProtocol(KingdomPointSync(points));
	LinkServer::GetInstance().BroadcastProtocol(KingdomPointChange(points, 0));
}

void KingdomInfo::OnGSConnect(unsigned int sid)
{
	SyncKingdomInfo(sid);
}

void KingdomInfo::SetNewWinner(char res, int win_fac, int _task_points)
{
	LOG_TRACE("kingdom battle end res %d win_fac %d _task_points %d", res, win_fac, _task_points);
	DBKingdomSetWinner * rpc = (DBKingdomSetWinner *)Rpc::Call(RPC_DBKINGDOMSETWINNER,
			DBKingdomSetWinnerArg(res, win_fac, _task_points));
	GameDBClient::GetInstance()->SendProtocol(rpc);
}

const KingdomFunctionary * KingdomInfo::FindFunc(int roleid) const
{
	if (roleid == 0)
		return NULL;
	if (roleid == king.roleid)
		return &king;
	if (roleid == queen.roleid)
		return &queen;
	std::map<int, KingdomFunctionary>::const_iterator it = functionaries.find(roleid);
	if (it == functionaries.end())
		return NULL;
	return &(it->second);
}

KingdomFunctionary * KingdomInfo::FindFunc(int roleid)
{
	return const_cast<KingdomFunctionary *>(static_cast<const KingdomInfo &>(*this).FindFunc(roleid));
}

//Èç¹ûÊÇ¹¥·½Ê¤Àû »á¸ø gamedbd ·¢Á½´Îsetwinner
//µÚÒ»´Î newwiner Îª0 µÚ¶þ´Î newwiner ÎªÐÂ¹úÍõ°ï
void KingdomInfo::OnSetWinner(char result, const GKingdomInfo & newinfo)
{
	//Óëgamedbd DBKingdomSetWinner Âß¼­±£³ÖÒ»ÖÂ
	if (result == KINGDOM_DEFENDER_WIN)
	{
		if (king_faction != 0)
			win_times++;
	}
	else if (result == KINGDOM_ATTACKER_WIN)
	{
		if (GetKingFaction()!=0 && newinfo.kingfaction==0)
		{
			CommonDataMan::Instance()->SimplePut(COMMON_DATA_KINGTASK_WINPOINTS, 0, true);
			//task_points »áÔÚ OnSet()ÖÐ±»ÖÃÎª 0
			ClearTitles();
		}
		OnSet(newinfo);
		if (newinfo.kingfaction)
		{
			GenerateTitles();
			NewWinnerAnnounce(king_fac_name, king.rolename);
		}
		BroadcastNewKing(newinfo.king.roleid);
		SyncKingdomInfo();
	}
	else
		Log::log(LOG_ERR,"kingdominfo OnSetWinner invalid res %d", result);
}

void KingdomInfo::ClearTitles()
{
	if (king.roleid)
		UpdateTitle(king.roleid, KINGDOM_TITLE_NONE);
	if (queen.roleid)
		UpdateTitle(queen.roleid, KINGDOM_TITLE_NONE);
	std::map<int, KingdomFunctionary>::const_iterator it, ite = functionaries.end();
	for (it = functionaries.begin(); it != ite; ++it)
		UpdateTitle(it->second.roleid, KINGDOM_TITLE_NONE);
	if (king_faction)
	{
		KingdomTitleChange notice(KINGDOM_TITLE_NONE, 0);
		FactionManager::Instance()->Broadcast(king_faction, notice, notice.localsid);
		KingdomTitleSync sync(0, KINGDOM_TITLE_NONE);
		FactionManager::Instance()->BroadcastGS(king_faction, sync, sync.roleid);
	}
}

void KingdomInfo::GenerateTitles()
{
	//×¢ÒâË³Ðò
	if (king_faction)
	{
		KingdomTitleChange notice(KINGDOM_TITLE_MEMBER, 0);
		FactionManager::Instance()->Broadcast(king_faction, notice, notice.localsid);
		KingdomTitleSync sync(0, KINGDOM_TITLE_MEMBER);
		FactionManager::Instance()->BroadcastGS(king_faction, sync, sync.roleid);
	}
	std::map<int, KingdomFunctionary>::const_iterator it, ite = functionaries.end();
	for (it = functionaries.begin(); it != ite; ++it)
		UpdateTitle(it->second.roleid, it->second.title);
	if (queen.roleid)
		UpdateTitle(queen.roleid, KINGDOM_TITLE_QUEEN);
	if (king.roleid)
		UpdateTitle(king.roleid, KINGDOM_TITLE_KING);
}

void KingdomInfo::BroadcastNewKing(int newking)
{
	LinkServer::GetInstance().BroadcastProtocol(KingdomKingChange(newking, 0));
}

int KingdomInfo::Announce(int roleid, const Octets & ann)
{
	if (!IsReady())
		return ERR_KINGDOM_NOT_INIT;
	if (!CheckPrivilege(roleid, KINGDOM_PRIVILEGE_ANNOUNCE))
		return ERR_KINGDOM_PRIVILEGE;
	if (ann.size() > 100)
		return ERR_KINGDOM_LOGIC;
	/*
	if (Matcher::GetInstance()->Match((char*)ann.begin(),ann.size()))
		return ERR_KINGDOM_INVALID_CHAR;
		*/
	if (!CheckPoints(ANNOUNCE_POINTS))
		return ERR_KINGDOM_POINTS_NOTENOUGH;
	
	OnPointChange(ANNOUNCE_POINTS * (-1));
	announce = ann;
	SetDirty(true);

	ChatBroadCast chat;
	chat.channel = GP_CHAT_SYSTEM;
	chat.srcroleid = MSG_KINGDOM_ANNOUNCE;
	Marshal::OctetsStream msg;
	chat.msg = ann;
	LinkServer::GetInstance().BroadcastProtocol(chat);

	return ERR_SUCCESS;
}

int KingdomInfo::Appoint(int roleid, const Octets & candidate_name, char title)
{
	if (!IsReady())
		return ERR_KINGDOM_NOT_INIT;
	if (!CheckPrivilege(roleid, KINGDOM_PRIVILEGE_APPOINT))
		return ERR_KINGDOM_PRIVILEGE;
	if (title != KINGDOM_TITLE_GENERAL && title != KINGDOM_TITLE_MINISTER && title != KINGDOM_TITLE_GUARD)
		return ERR_KINGDOM_LOGIC;
	if (!candidate_name.size())
		return ERR_KINGDOM_LOGIC;	
	if (!CheckFuncCount(title))
		return ERR_KINGDOM_FUNC_SIZE;
	DBKingdomAppointArg arg(candidate_name, title);
	DBKingdomAppoint * rpc = (DBKingdomAppoint *)Rpc::Call(RPC_DBKINGDOMAPPOINT, arg);
	rpc->roleid = roleid;
	GameDBClient::GetInstance()->SendProtocol(rpc);
	return ERR_SUCCESS;
}

void KingdomInfo::UpdateTitle(int candidate_id, char title)
{
	PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(candidate_id);
	if (pinfo)
	{
		if (title == -1) //ÔÚ pinfo->factionid ²»±ä ¹ú¼ÒÍ·ÏÎ½â³ýµÄÇé¿öÏÂÊ¹ÓÃ -1 ²ÎÊý
			title = (pinfo->factionid==(unsigned int)king_faction)?
				KINGDOM_TITLE_MEMBER:KINGDOM_TITLE_NONE;
		GDeliveryServer::GetInstance()->Send(pinfo->linksid, KingdomTitleChange(title, pinfo->localsid));
		GProviderServer::GetInstance()->DispatchProtocol(pinfo->gameid, KingdomTitleSync(candidate_id, title));
	LOG_TRACE("Update player %d title %d", candidate_id, title);
	}
}

void KingdomInfo::OnDBAppoint(char title, int candidate_id, const Octets & candidate_name)
{
	if (functionaries.insert(std::make_pair(candidate_id, KingdomFunctionary(candidate_id, title, candidate_name))).second == false)
	{
		Log::log(LOG_ERR, "dup functionary title %d roleid %d", title, candidate_id);
		return;
	}
	func_count[title]++;
	UpdateTitle(candidate_id, title);
	SyncKingdomInfo();

	ChatBroadCast chat;
	chat.channel = GP_CHAT_SYSTEM;
	chat.srcroleid = MSG_KINGDOM_APPOINT;
	Marshal::OctetsStream msg;
	msg << title << candidate_name;
	chat.msg = msg;
	LinkServer::GetInstance().BroadcastProtocol(chat);
}

int KingdomInfo::Discharge(int roleid, int candidate, char title)
{
	if (!IsReady())
		return ERR_KINGDOM_NOT_INIT;
	if (!CheckPrivilege(roleid, KINGDOM_PRIVILEGE_APPOINT))
		return ERR_KINGDOM_PRIVILEGE;
	if (title != KINGDOM_TITLE_GENERAL && title != KINGDOM_TITLE_MINISTER && title != KINGDOM_TITLE_GUARD)
		return ERR_KINGDOM_LOGIC;

	std::map<int, KingdomFunctionary>::const_iterator it = functionaries.find(candidate);
	if (it == functionaries.end() || it->second.title != title)
		return ERR_KINGDOM_LOGIC;

	DBKingdomDischargeArg arg(candidate, title);
	DBKingdomDischarge * rpc = (DBKingdomDischarge *)Rpc::Call(RPC_DBKINGDOMDISCHARGE, arg);
	rpc->roleid = roleid;
	GameDBClient::GetInstance()->SendProtocol(rpc);
	return ERR_SUCCESS;

}

void KingdomInfo::OnDBDischarge(int candidate_id, char oldtitle)
{
	if (functionaries.erase(candidate_id) == 0)
	{
		Log::log(LOG_ERR, "functionary roleid %d not exist", candidate_id);
		return;
	}
	func_count[oldtitle]--;
	UpdateTitle(candidate_id, -1);
	SyncKingdomInfo();
}

bool KingCallGuardsLimit::CheckCD()
{
	if (limit.last_timestamp != 0)
	{
		int64_t time_off = GDeliveryServer::GetInstance()->zone_off;
		if ((limit.last_timestamp+time_off)/86400 != (Timer::GetTime()+time_off)/86400)
		{
			limit.period_times = 0;
			return true;
		}
		return limit.period_times < GetTimesLimit();
	}
	return true;
}

void KingCallGuardsLimit::SetCD()
{
	limit.func_type = GetCDType();
	limit.period_type = KingdomLimit::PERIOD_DAY;
	limit.period_times++;
	limit.last_timestamp = Timer::GetTime();
}

KingdomAttrSet * KingdomInfo::FindAttrSet(int flag)
{
	std::map<int, KingdomAttrSet *>::iterator it = game_attr.find(flag);
	if (it == game_attr.end())
		return NULL;
	return it->second;
}

KingdomLimit * KingdomInfo::FindLimit(int flag)
{
	std::map<int, KingdomLimit *>::iterator it = limits.find(flag);
	if (it == limits.end())
		return NULL;
	return it->second;
}

int KingdomInfo::SetGameAttr(int roleid, int flag)
{
	if (!IsReady())
		return ERR_KINGDOM_NOT_INIT;
	if (!CheckPrivilege(roleid, KINGDOM_PRIVILEGE_SET_GAMEATTR))
		return ERR_KINGDOM_PRIVILEGE;
	KingdomAttrSet * attr = FindAttrSet(flag);
	if (attr == NULL)
		return ERR_KINGDOM_LOGIC;
	if (!CheckPoints(attr->Cost()))
		return ERR_KINGDOM_POINTS_NOTENOUGH;
	int ret = attr->Set();
	if (ret)
		return ret;
	OnPointChange(attr->Cost() * (-1));
	SetDirty(true);
	ChatBroadCast chat;
	chat.channel = GP_CHAT_SYSTEM;
	chat.srcroleid = MSG_KINGDOM_SET_ATTR;
	Marshal::OctetsStream msg;
	msg << flag << attr->Cost();
	chat.msg = msg;
	LinkServer::GetInstance().BroadcastProtocol(chat);

	return ERR_SUCCESS;
}

int KingdomInfo::GetTitle(int roleid, KingdomGetTitle_Re & re) const
{
	if (!IsReady())
		return ERR_KINGDOM_NOT_INIT;
	const KingdomFunctionary * func = FindFunc(roleid);
	if (func != NULL)
		re.self_title = func->title;
	else
	{
		PlayerInfo * pinfo = UserContainer::GetInstance().FindRole(roleid);
		if (pinfo && king_faction > 0 && (int)pinfo->factionid==king_faction)
			re.self_title = KINGDOM_TITLE_MEMBER;
	}
	re.king = king.roleid;
	re.points = points;
	return ERR_SUCCESS;
}

void KingdomInfo::UpdateQueen(const GKingdomFunctionary & qu)
{
	if (queen.roleid)
		UpdateTitle(queen.roleid, -1);
	queen = LoadFunc(qu);
	if (qu.roleid)
	{

		std::map<int, KingdomFunctionary>::iterator it = functionaries.find(qu.roleid);
		if (it != functionaries.end()) 
		{
			func_count[it->second.title]--;
			functionaries.erase(qu.roleid);
			UpdateTitle(qu.roleid, -1);
		}

		UpdateTitle(qu.roleid, KINGDOM_TITLE_QUEEN);
	}
	SyncKingdomInfo();
}
void KingdomInfo::OnRolenameChange(int roleid, const Octets & newname)
{
	KingdomFunctionary * func = FindFunc(roleid);
	if (func)
		func->rolename = newname;
}
void KingdomInfo::OnFacNameChange(int fid, const Octets & newname)
{
	if (fid == king_faction)
		king_fac_name = newname;
}
void KingdomInfo::OnFactionChange(int oldfid, int fid, int roleid)
{
	if (oldfid != 0 && oldfid == king_faction)
	{//Àë¿ª¹úÍõ°ï
		KingdomFunctionary * func = FindFunc(roleid);
		if (!func)
			UpdateTitle(roleid, KINGDOM_TITLE_NONE);
	}
	else if (fid != 0 && fid == king_faction)
	{//¼ÓÈë¹úÍõ°ï
		KingdomFunctionary * func = FindFunc(roleid);
		if (!func)
			UpdateTitle(roleid, KINGDOM_TITLE_MEMBER);
	}
}
void KingdomInfo::OnBattleHalf(char type, int tag, char res, std::vector<int> _failers)
{
	if (!GetBattle().OnBattleHalf(type, tag, res, _failers))
		return;
	//Èç¹ûÊÇ½ø¹¥·½Ó®ÇÒnewwinner=0 ±íÊ¾ÊØ·½Ê§°Ü ÐÂ¹úÍõ²úÉúÖ®Ç°¶ÌÔÝµÄ¹ú¼ÒÎÞÖ÷×´Ì¬
	int win_points = 0;
	CommonDataMan::Instance()->SimpleGet(COMMON_DATA_KINGTASK_WINPOINTS, win_points);
	if (win_points > KINGDOM_TASK_POINTS_LIMIT)
		win_points = KINGDOM_TASK_POINTS_LIMIT;
	else if (win_points < 0)
		win_points = 0;
	SetNewWinner(res, 0, win_points);
}

void KingdomInfo::DebugSetWinner(int res, int new_winner)
{
	int win_points = 0;
	CommonDataMan::Instance()->SimpleGet(COMMON_DATA_KINGTASK_WINPOINTS, win_points);
	if (win_points > KINGDOM_TASK_POINTS_LIMIT)
		win_points = KINGDOM_TASK_POINTS_LIMIT;
	else if (win_points < 0)
		win_points = 0;
	SetNewWinner(res, new_winner, win_points);
}

void KingdomInfo::OnBattleEnd(char type, int tag, int winner)
{
	if (!GetBattle().OnBattleEnd(type, tag, winner))
		return;
	if (winner != 0)
		SetNewWinner(KINGDOM_ATTACKER_WIN, winner, 0);
	else
		Log::log(LOG_ERR, "onkingdombattleend but winner is %d", winner);
}

int KingdomInfo::TryCallGuards(int roleid, int lineid, int mapid, float x, float y, float z, int & remain_times)
{
	if (!IsReady())
		return ERR_KINGDOM_NOT_INIT;
	if (roleid == 0 || roleid != GetKing())
		return ERR_KINGDOM_LOGIC;
	KingdomLimit * limit = FindLimit(KingdomLimit::CALL_GUARDS);
	if (limit == NULL)
		return ERR_KINGDOM_LOGIC;
	if (!limit->CheckCD())
		return ERR_KINGDOM_CALLGUARDS_CD;
	bool somebody_online = false;
	std::map<int, KingdomFunctionary>::const_iterator it, ite = functionaries.end();
	for (it = functionaries.begin(); it != ite; ++it)
	{
		if (it->second.title == KINGDOM_TITLE_GUARD)
		{
			PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(it->second.roleid);
			if (pinfo)
			{
				KingGuardInvite * rpc = (KingGuardInvite *)Rpc::Call(RPC_KINGGUARDINVITE, KingGuardInviteArg(GetKingName(), pinfo->localsid));
				rpc->lineid = lineid;
				rpc->mapid = mapid;
				rpc->posx = x;
				rpc->posy = y;
				rpc->posz = z;
				rpc->guard_roleid = it->second.roleid;
				GDeliveryServer::GetInstance()->Send(pinfo->linksid, rpc);
				somebody_online = true;
			}
		}
	}
	if (!somebody_online)
		return ERR_KINGDOM_NOGUARD_ONLINE;
	limit->SetCD();
	remain_times = limit->GetRemainTimes();
	SetDirty(true);
	return ERR_SUCCESS;
}

int KingdomInfo::TryOpenBath(int roleid)
{
	if (!IsReady())
		return ERR_KINGDOM_NOT_INIT;
	if (roleid == 0 || roleid != GetQueen())
		return ERR_KINGDOM_LOGIC;
	KingdomAttrSet * attr = FindAttrSet(KingdomLimit::OPEN_BATH);
	if (attr == NULL)
		return ERR_KINGDOM_LOGIC;
	int ret = attr->CheckOpen();
	if (ret)
		return ret;
	if (!CheckPoints(attr->Cost()))
		return ERR_KINGDOM_POINTS_NOTENOUGH;
	if (GetKing() == 0)
		return ERR_KINGDOM_KING_NOTONLINE;
	PlayerInfo * pinfo = UserContainer::GetInstance().FindRoleOnline(GetKing());
	if (pinfo == NULL)
		return ERR_KINGDOM_KING_NOTONLINE;
	QueenOpenBathInvite * rpc = (QueenOpenBathInvite *)Rpc::Call(RPC_QUEENOPENBATHINVITE, QueenOpenBathInviteArg(pinfo->localsid));
	rpc->queen_roleid = roleid;
	GDeliveryServer::GetInstance()->Send(pinfo->linksid, rpc);
	return ERR_SUCCESS;
}

int KingdomInfo::OpenBath()
{
	if (!IsReady())
		return ERR_KINGDOM_NOT_INIT;
	KingdomAttrSet * attr = FindAttrSet(KingdomLimit::OPEN_BATH);
	if (attr == NULL)
		return ERR_KINGDOM_LOGIC;
	if (!CheckPoints(attr->Cost()))
		return ERR_KINGDOM_POINTS_NOTENOUGH;
	int ret = attr->Set();
	if (ret)
		return ret;
	OnPointChange(attr->Cost()*(-1));
	SetDirty(true);

	ChatBroadCast chat;
	chat.channel = GP_CHAT_SYSTEM;
	chat.srcroleid = MSG_KINGDOM_OPEN_BATH;
	Marshal::OctetsStream msg;
	msg << GetQueenName();
	chat.msg = msg;
	LinkServer::GetInstance().BroadcastProtocol(chat);
	return ERR_SUCCESS;
}

int KingdomInfo::IssueTask(int roleid, int task_type)
{
	if (!IsReady() || !CommonDataMan::Instance()->IsOpen())
		return ERR_KINGDOM_NOT_INIT;
	if (roleid == 0 || roleid != GetKing())
		return ERR_KINGDOM_LOGIC;
//	if (task_issue_time != 0 && Timer::GetTime() < task_issue_time + TASK_ISSUE_CD)
//		return ERR_KINGDOM_TASK_CD;
	if (task_status != KINGDOM_TASK_NONE)
		return ERR_KINGDOM_TASK_REPEAT; //µ±Ç°ÈÎÎñÃ»ÓÐ½áËãÍê±Ï£¬½ûÖ¹·¢²¼ÐÂÈÎÎñ
	DBKingdomTaskIssue * rpc = (DBKingdomTaskIssue *)Rpc::Call(RPC_DBKINGDOMTASKISSUE, DBKingdomTaskIssueArg(roleid, GetKingFaction(), task_type, Timer::GetTime()));
	GameDBClient::GetInstance()->SendProtocol(rpc);
	return ERR_SUCCESS;
}

int KingdomInfo::GetReward(int roleid)
{
	if (!IsReady())
		return ERR_KINGDOM_NOT_INIT;
	if (roleid == 0 || roleid != GetKing())
		return ERR_KINGDOM_LOGIC;
	DBKingGetReward * rpc = (DBKingGetReward *)Rpc::Call(RPC_DBKINGGETREWARD, DBKingGetRewardArg(roleid));
	GameDBClient::GetInstance()->SendProtocol(rpc);
	return ERR_SUCCESS;
}

void KingdomInfo::OnGetReward(int mask)
{
	reward_mask = mask;
}

void KingdomInfo::OnDBTaskIssue(int type, int time)
{
	task_status = KINGDOM_TASK_OPEN;
	task_type = type;
	task_issue_time = time;
	CommonDataMan::Instance()->SimplePut(COMMON_DATA_KINGTASK_ISSUETIME, time, true);
	task_points = 0;
	//´Ë´¦Ç¿ÖÆÔÙ°ÑÀÛ»ýÈ«¾Ö±äÁ¿ÇåÒ»´Î 0 ·ÀÖ¹¹úÍõÁìÈ¡»ý·ÖµÄÊ±ºòÃ»Çåµô
	CommonDataMan::Instance()->SimplePut(COMMON_DATA_KINGTASK_WINPOINTS, 0, true);

	ChatBroadCast chat;
	chat.channel = GP_CHAT_SYSTEM;
	chat.srcroleid = MSG_KINGDOM_ISSUE_TASK;
	Marshal::OctetsStream msg;
	msg << GetKingName() << task_type;
	chat.msg = msg;
	LinkServer::GetInstance().BroadcastProtocol(chat);

	SyncKingdomInfo();
}

int KingdomInfo::GetTask(int roleid, int & type, int & times, int & _status, int & _time)
{
	if (!IsReady() || !CommonDataMan::Instance()->IsOpen())
		return ERR_KINGDOM_NOT_INIT;
	type = task_type;
	_status = task_status;
	_time = task_issue_time;
	if (task_type > 0 && roleid != 0 && roleid == GetKing())
	{
		int win_points = 0;
		CommonDataMan::Instance()->SimpleGet(COMMON_DATA_KINGTASK_WINPOINTS, win_points);
		times = MapTaskCompleteTime(task_type, win_points);
	}
	return ERR_SUCCESS;
}

void KingdomInfo::DebugClearLimits()
{
	std::map<int, KingdomLimit *>::const_iterator it, ite = limits.end();
	for (it = limits.begin(); it != ite; ++it)
		if (it->second != NULL)
			it->second->ClearCD();
}

int KingdomAttrSet::Set()
{
	if (IsOnGoing())
		return ERR_KINGDOM_SVR_ATTR_REPEAT;
	if (!CheckCD())
		return ERR_KINGDOM_SVR_ATTR_CD;
	int ret = OnSet();
	if (ret != ERR_SUCCESS)
		return ret;
	ongoing = true;
	if (LastTime() > 0)
		timeout = Timer::GetTime() + LastTime();
	SetCD();
	return ERR_SUCCESS;
}

void KingdomAttrSet::Update(time_t now)
{
	if (ongoing)
	{
		LOG_TRACE("kingdom attr set type %d timeout:%d", GetCDType(), timeout);
		if (now > timeout)
		{
			OnCancel();
			ongoing = false;
			timeout = 0;
		}
	}
}

bool KingdomAttrSet::SetAttrGeneral(GameAttrMap::attri_t attr, const GameAttrMap::value_t & value)
{
	if (!GameAttrMap::IsValid(attr, value))
	{
		Log::log(LOG_ERR, "King try set attr %d not valid", attr);
		return false;
	}
	if (!GameAttrMap::Put(attr, value))
	{
		Log::log(LOG_ERR, "King try put attr %d fail", attr);
		return false;
	}
	QueryGameServerAttr_Re qgsa_re;
	qgsa_re.attr.push_back(GameAttr(attr, value));
	GProviderServer::GetInstance()->BroadcastProtocol(qgsa_re);
	GMSetGameAttri::UpdateServerAttr(attr, value);
	Log::formatlog("kingdom", "set attr %d", attr);
	return true;
}

bool KingdomTripleExp::CheckCD()
{
	if (limit.last_timestamp != 0)
	{
		int64_t time_off = GDeliveryServer::GetInstance()->zone_off;
		if ((limit.last_timestamp+time_off)/86400 != (Timer::GetTime()+time_off)/86400)
		{
			limit.period_times = 0;
			return true;
		}
		return limit.period_times < GetTimesLimit();
	}
	return true;
}

void KingdomTripleExp::SetCD()
{
	limit.func_type = GetCDType();
	limit.period_type = KingdomLimit::PERIOD_DAY;
	limit.period_times++;
	limit.last_timestamp = Timer::GetTime();
}

int KingdomTripleExp::OnSet()
{
	if (GDeliveryServer::GetInstance()->serverAttr.GetMultipleExp() > 1
			|| GDeliveryServer::GetInstance()->serverAttr.GetDoubleSP() != 0)
	{
		Log::log(LOG_ERR, "King try enable triple exp, but attr %lld already set",
				GDeliveryServer::GetInstance()->serverAttr.GetAttr());
		return ERR_KINGDOM_SVR_ATTR_COND;
	}
	if (SetAttrGeneral(Privilege::PRV_MULTIPLE_EXP,
			Marshal::OctetsStream()<<(unsigned char)3))
		return ERR_SUCCESS;
	else
		return ERR_KINGDOM_SVR_ATTR_COND;
}

void KingdomTripleExp::OnCancel()
{
	SetAttrGeneral(Privilege::PRV_MULTIPLE_EXP,
			Marshal::OctetsStream()<<(unsigned char)MIN_OF_MULTIPLEEXP);
}

bool KingdomDoubleDrop::CheckCD()
{
	if (limit.last_timestamp != 0)
	{
		int64_t time_off = GDeliveryServer::GetInstance()->zone_off;
		if ((limit.last_timestamp+time_off)/86400 != (Timer::GetTime()+time_off)/86400)
		{
			limit.period_times = 0;
			return true;
		}
		return limit.period_times < GetTimesLimit();
	}
	return true;
}

void KingdomDoubleDrop::SetCD()
{
	limit.func_type = GetCDType();
	limit.period_type = KingdomLimit::PERIOD_DAY;
	limit.period_times++;
	limit.last_timestamp = Timer::GetTime();
}

int KingdomDoubleDrop::OnSet()
{
	if (GDeliveryServer::GetInstance()->serverAttr.GetDoubleObject() != 0)
	{
		Log::log(LOG_ERR, "King try enable double drop, but attr %lld already set",
				GDeliveryServer::GetInstance()->serverAttr.GetAttr());
		return ERR_KINGDOM_SVR_ATTR_COND;
	}
	if (SetAttrGeneral(Privilege::PRV_DOUBLEOBJECT,
			Marshal::OctetsStream()<<(unsigned char)GameAttrMap::_ATTR_ENABLE))
		return ERR_SUCCESS;
	else
		return ERR_KINGDOM_SVR_ATTR_COND;
}

void KingdomDoubleDrop::OnCancel()
{
	SetAttrGeneral(Privilege::PRV_DOUBLEOBJECT,
			Marshal::OctetsStream()<<(unsigned char)GameAttrMap::_ATTR_DISABLE);
}

KingdomOpenBath::KingdomOpenBath()
{
	time_t now = Timer::GetTime();
	time_t timestamp = 0;
	struct tm dt;
	localtime_r(&now, &dt);
	dt.tm_sec = 0;
	dt.tm_min = 0;
	dt.tm_hour = 0;
	timestamp = mktime(&dt);
	if (timestamp != -1)
	{
		//ÖÜÈÕÁè³¿Áãµã
		t_base = timestamp - 86400*dt.tm_wday;
	}
}

void KingdomOpenBath::Update(time_t now)
{
	if (t_base && now - t_base > WEEK_SECONDS)
		t_base += WEEK_SECONDS;
	if (!ongoing)
	{
		if (timeout != 0 && now > timeout)
		{
			OnSet();
			ongoing = true;
			timeout = now + LastTime();
		}
	}
	KingdomAttrSet::Update(now);
	if (!ongoing && timeout == 0)
	{
		if (now < t_base + SYS_START_TIME)
			timeout = t_base + SYS_START_TIME;
		else
			timeout = t_base + SYS_START_TIME + WEEK_SECONDS;
	}
	LOG_TRACE("KingdomOpenBath t_base %s", ctime(&t_base));
	LOG_TRACE("KingdomOpenBath ongoing %d timeout %s", ongoing, ctime(&timeout));
}

bool KingdomOpenBath::CheckCD()
{
	if (limit.last_timestamp != 0)
	{
		time_t now = Timer::GetTime();
		//ÏÈÅÐ¶ÏÀäÈ´ ·ÀÖ¹ gs »¹Ã»ÓÐ³É¹¦¹Ø±ÕÓÖ·¢Æð¿ªÆô
		if (now < limit.last_timestamp + LastTime() + 600)
			return false;
		int64_t time_off = GDeliveryServer::GetInstance()->zone_off;
		if ((limit.last_timestamp+time_off)/86400 != (now+time_off)/86400)
		{
			limit.period_times = 0;
			return true;
		}
		return limit.period_times < GetTimesLimit();
	}
	return true;
}

void KingdomOpenBath::SetCD()
{
	limit.func_type = GetCDType();
	limit.period_type = KingdomLimit::PERIOD_DAY;
	limit.period_times++;
	limit.last_timestamp = Timer::GetTime();
}
int KingdomOpenBath::OnSet()
{
	/*
	time_t now = Timer::GetTime();
	if (now >= t_base + FORBID_BEGIN_TIME && now < t_base + FORBID_END_TIME)
		return ERR_KINGDOM_SYS_BATH_TIME;
		*/
	//ÒÆµ½ CheckOpen ½øÐÐÅÐ¶Ï ·ñÔòÏµÍ³¿ªÆôµ÷ÓÃ OnSet ½«²»ÄÜÍ¨¹ý
	Log::formatlog("kingdom", "send open bath");
	GProviderServer::GetInstance()->BroadcastProtocol(QueenOpenBath(0));
	return ERR_SUCCESS;
}

void KingdomOpenBath::OnCancel()
{
	Log::formatlog("kingdom", "send close bath");
	GProviderServer::GetInstance()->BroadcastProtocol(QueenCloseBath(0));
}

int KingdomOpenBath::CheckOpen()
{
	if (IsOnGoing())
		return ERR_KINGDOM_SVR_ATTR_REPEAT;
	if (!CheckCD())
		return ERR_KINGDOM_SVR_ATTR_CD;
	time_t now = Timer::GetTime();
	if (now >= t_base + FORBID_BEGIN_TIME && now < t_base + FORBID_END_TIME)
		return ERR_KINGDOM_SYS_BATH_TIME;
	return ERR_SUCCESS;
}
/////////////////////////////new
bool KingdomManager::LoadConf(time_t & special_time)
{
	Conf *conf = Conf::GetInstance();
	std::string special_date = conf->find(GDeliveryServer::GetInstance()->Identification(), "kingdom_battle_date");
	if (!special_date.empty())
	{
		int year, mon, day;
		year = mon = day = 0;
		if (3 != sscanf(special_date.c_str(), "%d-%d-%d", &year, &mon, &day))
		{
			Log::log(LOG_ERR, "invalid kingdom_battle_date %s", special_date.c_str());
			return false;
		}
		struct tm dt = {};
		dt.tm_year = year-1900;
		dt.tm_mon = mon-1;
		dt.tm_mday = day;
		time_t t = mktime(&dt);
		if (t == -1)
		{
			Log::log(LOG_ERR, "invalid kingdom_battle_date %s, mktime error", special_date.c_str());
			return false;
		}
		t += KingdomBattle::START_TIME;
		if (Timer::GetTime() < t)
			special_time = t;
	}
	return true;
}

bool KingdomManager::Initialize()
{
	time_t special_time = 0;
	if (!LoadConf(special_time))
		return false;
	kingdom.Init(special_time);
	IntervalTimer::Attach( this,UPDATE_INTERVAL*1000000/IntervalTimer::Resolution());
//	status |= ST_OPEN;
	return true;
}

bool KingdomManager::OnDBConnect(Protocol::Manager *manager, int sid)
{
//	if(/*(db_status&ST_OPEN) &&*/ !(db_status&ST_DATAREADY))
	{
		DBKingdomLoadArg arg;
		manager->Send(sid, Rpc::Call(RPC_DBKINGDOMLOAD, arg));
	}
	return true;
}

void KingdomManager::RegisterField(int gsid, const KingdomBattleInfoVector & fields)
{
	KingdomBattleInfoVector::const_iterator it, ite = fields.end();
	for (it = fields.begin(); it != ite; ++it)
		kingdom.GetBattle().Register(gsid, it->fieldtype, it->tagid);
}

void KingdomManager::LoadKingdom(const GKingdomInfo & info)
{
	kingdom.Load(info);
}

bool KingdomManager::Update()
{
	time_t now = Timer::GetTime();
	kingdom.Update(now);
	return true;
}

void KingdomManager::OnBattleStart(int ret, char fieldtype, int tag, int def, const std::vector<int> & attackers, const Octets & def_name)
{
	kingdom.GetBattle().OnBattleStart(ret, fieldtype, tag, def, attackers, def_name);
}

void KingdomManager::OnBattleAttackerFail(char type, int tag, int failer)
{
	kingdom.GetBattle().OnBattleAttackerFail(type, tag, failer);
}

void KingdomManager::OnBattleHalf(char type, int tag, char res, std::vector<int> _failers)
{
	kingdom.OnBattleHalf(type, tag, res, _failers);
}

void KingdomManager::OnBattleEnd(char type, int tag, int winner)
{
	kingdom.OnBattleEnd(type, tag, winner);
}

int KingdomManager::TryEnterBattle(char type, PlayerInfo *pinfo)
{
	return kingdom.GetBattle().TryEnterBattle(type, pinfo);
}

void KingdomManager::OnChangeGS(int roleid, int retcode, int old_gs, int old_map, int new_gs, int new_map)
{
	kingdom.GetBattle().OnChangeGS(roleid, retcode, old_gs, old_map, new_gs, new_map);
}

void KingdomManager::OnLoginBattle(int roleid, int retcode, int old_gs, int old_map, int new_gs, int new_map)
{
	kingdom.GetBattle().OnLoginBattle(roleid, retcode, old_gs, old_map, new_gs, new_map);
}

int KingdomManager::TryLeaveBattle(char type, int roleid, int tag)
{
	return kingdom.GetBattle().TryLeaveBattle(type, roleid, tag);
}

void KingdomManager::OnDisconnect(int gsid)
{       
	kingdom.GetBattle().OnDisconnect(gsid);
}

void KingdomManager::OnLogout(int roleid, int gsid, int map)
{
	kingdom.GetBattle().OnLogout(roleid, gsid, map);
}

void KingdomManager::DebugAddAttacker(int fid)
{
	kingdom.GetBattle().DebugAddAttacker(fid);
}

void KingdomManager::DebugClearAttacker()
{
	kingdom.GetBattle().DebugClearAttacker();
}

void KingdomManager::DebugStartBattle()
{
	kingdom.GetBattle().DebugStartBattle();
}
void KingdomManager::DebugClearLimits()
{
	kingdom.DebugClearLimits();
}
void KingdomManager::DebugSetWinner(int res, int new_winner)
{
	kingdom.DebugSetWinner(res, new_winner);
}
void KingdomManager::OnPointChange(int delta)
{
	kingdom.OnPointChange(delta);
}

void KingdomManager::OnGSConnect(unsigned int sid)
{
	kingdom.OnGSConnect(sid);
}

void KingdomManager::OnSetWinner(char result, const GKingdomInfo & newinfo)
{
	kingdom.OnSetWinner(result, newinfo);
}

int KingdomManager::Announce(int roleid, const Octets & announce)
{
	return kingdom.Announce(roleid, announce);
}

int KingdomManager::Appoint(int roleid, const Octets & candidate_name, char title)
{
	return kingdom.Appoint(roleid, candidate_name, title);
}

void KingdomManager::OnDBAppoint(char title, int candidate_id, const Octets & candidate_name)
{
	kingdom.OnDBAppoint(title, candidate_id, candidate_name);
}

int KingdomManager::Discharge(int roleid, int candidate, char oldtitle)
{
	return kingdom.Discharge(roleid, candidate, oldtitle);
}

void KingdomManager::OnDBDischarge(int candidate, char oldtitle)
{
	kingdom.OnDBDischarge(candidate, oldtitle);
}

int KingdomManager::SetGameAttr(int roleid, int flag)
{
	return kingdom.SetGameAttr(roleid, flag);
}

int KingdomManager::GetClientInfo(int roleid, KingdomClientInfo & client) const
{
	return kingdom.GetClientInfo(roleid, client);
}

int KingdomManager::GetTitle(int roleid, KingdomGetTitle_Re & re)
{
	return kingdom.GetTitle(roleid, re);
}

void KingdomManager::UpdateQueen(const GKingdomFunctionary & qu)
{
	kingdom.UpdateQueen(qu);
}
void KingdomManager::OnRolenameChange(int roleid, const Octets & rolename)
{
	kingdom.OnRolenameChange(roleid, rolename);
}
void KingdomManager::OnFacNameChange(int fid, const Octets & newname)
{
	kingdom.OnFacNameChange(fid, newname);
}
void KingdomManager::OnFactionChange(int oldfid, int fid, int roleid)
{
	kingdom.OnFactionChange(oldfid, fid, roleid);
}
int KingdomManager::TryCallGuards(int roleid, int lineid, int mapid, float x, float y, float z, int & remain_times)
{
	return kingdom.TryCallGuards(roleid, lineid, mapid, x, y, z, remain_times);
}
int KingdomManager::TryOpenBath(int roleid)
{
	return kingdom.TryOpenBath(roleid);
}
int KingdomManager::OpenBath()
{
	return kingdom.OpenBath();
}
int KingdomManager::IssueTask(int roleid, int type)
{
	return kingdom.IssueTask(roleid, type);
}
int KingdomManager::GetReward(int roleid)
{
	return kingdom.GetReward(roleid);
}
void KingdomManager::OnGetReward(int mask)
{
	return kingdom.OnGetReward(mask);
}
void KingdomManager::OnDBTaskIssue(int type, int time)
{
	kingdom.OnDBTaskIssue(type, time);
}
int KingdomManager::GetTask(int roleid, int & type, int & complete_times, int & _status, int & _time)
{
	return kingdom.GetTask(roleid, type, complete_times, _status, _time);
}
void KingdomManager::DebugCloseTask()
{
	kingdom.CloseTask();
}
void KingdomManager::DebugEndTask()
{
	kingdom.EndTask();
}
};//end of GNET

