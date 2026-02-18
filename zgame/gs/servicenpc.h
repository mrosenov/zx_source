#ifndef __ONLINEGAME_GS_SERVICE_NPC_H__
#define __ONLINEGAME_GS_SERVICE_NPC_H__

#include "npc.h"
#include "serviceprovider.h"

class service_npc : public gnpc_imp
{
	typedef provider_list<MAX_PROVIDER_PER_NPC>  LIST;
	LIST _service_list;
	float _tax_rate;
	bool _need_domain;
	int  _domain_test_time;
	int  _domain_mafia;
	int  _attack_rule;
	int  _id_territory;
public:
DECLARE_SUBSTANCE(service_npc);

public:
	service_npc():_tax_rate(0),_need_domain(0),_domain_test_time(0),_domain_mafia(0),_attack_rule(0),_id_territory(0){}
	~service_npc();
	virtual void Reborn()
	{
		gnpc_imp::Reborn();
		//这里好像还没有什么可做的，列表应该依然可用
		//也有可能列表需要重新刷新，在相应服务加入后再处理
	}

	void SetTaxRate(float taxrate)
	{
		_tax_rate = taxrate;
	}
	
	float GetTaxRate() 
	{ 
		return _tax_rate;
	}

	void SetAttackRule(int rule)
	{
		_attack_rule = rule;
	}

	void SetTerritoryID(int id)
	{
		_id_territory = id;
	}

	int GetTerritoryID() const
	{
		return _id_territory;
	}
	
	
	int GetCurIDMafia();

	void SetNeedDomain(bool need_domain)
	{
		_need_domain = need_domain;
	}
	
	void AddProvider(service_provider * provider,  const void * buf, size_t size)
	{
		if(provider->Init(this,buf,size))
		{
			_service_list.AddProvider(provider);
		}
		else
		{
			delete provider;
		}
	}

	virtual int MessageHandler(const MSG & msg);

	bool Save(archive & ar);
	bool Load(archive & ar);
	virtual int DoAttack(const XID & target, char force_attack);
	virtual void FillAttackMsg(const XID & target, attack_msg & attack);
	virtual void FillEnchantMsg(const XID & target, enchant_msg & enchant);
	virtual int OI_GetTerritoryID() const { return _id_territory;}
};

#endif

