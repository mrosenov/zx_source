#ifndef __GNET_GLOBALDROPMANAGER_H__
#define	__GNET_GLOBALDROPMANAGER_H__

#include "globalcounterinfo"
namespace GNET
{

class GlobalDropManager : public IntervalTimer::Observer
{
	std::map<int64_t,GlobalCounterInfo> register_map;
	std::multimap<int/*time*/,int64_t/*key*/> future_counter_map;
public:
	void RegisterNewCounter(const std::vector<GlobalCounterInfo>& newcounter);
	GlobalDropManager(){}
	bool Update();
	~GlobalDropManager(){}
	static GlobalDropManager* GetInstance() { static GlobalDropManager instance; return &instance; }
	bool Initialize();
private:
	void HandleNewCounter(int64_t key, const GlobalCounterInfo & item);
	void AddCounterTask(const GlobalCounterInfo & item);
};

}

#endif

