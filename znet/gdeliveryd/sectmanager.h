#ifndef __GNET_SECTMANAGER_H
#define __GNET_SECTMANAGER_H

#include <vector>
#include <map>
#include <list>

#include "thread.h"
#include "hashstring.h"
#include "localmacro.h"
#include "gsect"
#include "sectlist_re.hpp"

namespace GNET
{

class GSectInfo
{
public:
	Octets name;
	std::vector<GDiscipleInfo> disciples;
	std::vector<GSectSkill> skills;
	int cooldown;
	int reputation;
	int uptime;
	bool dirty;
};

class SectManager : public IntervalTimer::Observer
{
	typedef std::map<unsigned int,GSectInfo*>  Map;
	unsigned int cursor;
	bool open;
	Map sects;

	SectManager() : cursor(0), open(false) {}  
public:
	~SectManager() {}

	static SectManager* Instance() 
	{ 
		static SectManager instance; 
		return &instance;
	}
	void Initialize();
	int PreRecruit(int master, int invitee);
	bool Update();
	bool GetSect(int sectid, SectList_Re& re, int roleid);
	GSectInfo *FindSect(int sectid);
	bool CheckSect(int sectid, int roleid);
	void OnLoad(int sectid, GSect& sect);
	void OnUpdate(int sectid, GSect& sect);
	void OnLogout(int sectid, int roleid, char level, char occupation);
	void OnLogin(int sectid, int roleid, int gameid);
	void OnJoin(int sectid, GDisciple& disciple);
	void OnQuit(int sectid, int roleid, Octets& name, char reason);
	bool GetSkill(int sectid, std::vector<GSectSkill>& skills);
	int  DoExpel(int sectid, int roleid);
	int  Quit(int sectid, int roleid, char reason );
	void Broadcast(int sectid, GSectInfo* info, Protocol& data, unsigned int &localsid);
	bool CheckCapacity(int sectid, unsigned int& capacity);
	void UpdateSect(int sectid, unsigned int reputation, std::vector<GSectSkill>& skils);
	unsigned int  GetCapacity(int reputation);
	void OnRolenameChange(int sectid, int roleid, const Octets & oldname, const Octets & newname);
};

};
#endif

