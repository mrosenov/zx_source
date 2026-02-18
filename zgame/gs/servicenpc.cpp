#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <common/protocol.h>
#include <arandomgen.h>
#include "servicenpc.h"
#include "world.h"
#include "clstab.h"
#include "npcgenerator.h"
#include "faction.h"
#include "towerbuild_filter.h"
#include <gsp_if.h>
#include "template/city_region.h"
#include "ainpc.h"

DEFINE_SUBSTANCE(service_npc,gnpc_imp,CLS_SERVICE_NPC_IMP)

int 
service_npc::GetCurIDMafia()
{
	if(!_need_domain) return 0;

	int t = g_timer.get_systime();
	if(t > _domain_test_time + 60) 
	{
		_domain_test_time = t;
		int domain_id = GetWorldManager()->GetWorldTag();
		if(domain_id > 0)
		{
			_domain_mafia = GMSV::GetCityOwner(domain_id);
			if(_domain_mafia <= 0) _domain_mafia = -1;
		}
		else
		{
			_domain_mafia = -1;
			
		}
	}
	return _domain_mafia;
}

int 
service_npc::MessageHandler(const MSG & msg)
{
	switch(msg.message)
	{
		case GM_MSG_SERVICE_REQUEST:
		{
			float s = 6 + _parent->body_size;
			if(msg.pos.squared_distance(_parent->pos) > s*s )	//6米限制
			{
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_SERVICE_UNAVILABLE);
				return 0;
			}
			//对服务的请求到来(要求服务)
			service_provider * provider = _service_list.GetProvider(msg.param);
			if(provider)
			{
				provider->PayService(msg.source,msg.content,msg.content_length);
			}
			else
			{
				//报告错误
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_SERVICE_UNAVILABLE);
			}
		}
		return 0;
		
		case GM_MSG_SERVICE_HELLO:
		{
			float s = 6 + _parent->body_size;
			if(msg.pos.squared_distance(_parent->pos) < s*s)	//6米限制
			{
				int faction = msg.param & (~FACTION_PARIAH);
				if(!(faction & GetEnemyFaction()))
				{
					gnpc_ai * pAI = ((gnpc_controller*)_commander)->GetAI();
					if(pAI)
					{
						ai_policy * pP = pAI->GetAICtrl();
						if(pP)
						{
							pP->OnGreeting();
						}
					}

					//检查帮派信息
					int id_mafia = GetCurIDMafia();
					SendTo<0>(GM_MSG_SERVICE_GREETING,msg.source,id_mafia);
				}
			}
		}
		return 0;


		case GM_MSG_SERVICE_QUIERY_CONTENT:
		{
			float s = 6 + _parent->body_size;
			if(msg.pos.squared_distance(_parent->pos) > s*s)	//6米限制
			{
				SendTo<0>(GM_MSG_ERROR_MESSAGE,msg.source,S2C::ERR_SERVICE_UNAVILABLE);
			}
		}
		return 0;

		//现在npc暂时没有服务数据回传，以后会有的
		if(msg.content_length == sizeof(int) * 2)
		{
			int cs_index = *(int*)msg.content;
			int sid = *((int*)msg.content + 1);
			service_provider * provider = _service_list.GetProvider(msg.param);
			if(provider)
			{
				provider->GetServiceContent(msg.source, cs_index, sid);
			}
		}
		else
		{
			ASSERT(false);
		}
		return 0;

		case GM_MSG_ATTACK:
		{
			if(_attack_rule == 0) return 0;
			ASSERT(msg.content_length >= sizeof(attack_msg));
			attack_msg & amsg = *(attack_msg*)msg.content;
			if(_player_cannot_attack && amsg.ainfo.attacker.IsPlayerClass()) return 0;
			if(msg.source.IsPlayerClass() && amsg.is_flying) return 0;
			if(!IsNPCOwner(amsg.ainfo.attacker.id, amsg.ainfo.team_id, amsg.ainfo.master_id)) return 0;
			//不能攻击自己和队友
			bool IsInvader = false;
			if(msg.source.IsPlayerClass())
			{
				if(!(amsg.attacker_mode & attack_msg::PVP_ENABLE))
				{
					return 0;
				}
				else
				{
					IsInvader = true;
				}
				if(!amsg.force_attack) 
				{
					IsInvader = false;
					if((amsg.target_faction & GetFaction()) == 0) return 0;
				}
			}
			
			//这里由于都要进行复制操作，有一定的耗费存在
			attack_msg ack_msg = *(attack_msg*)msg.content;
			ack_msg.is_invader =  IsInvader;
			HandleAttackMsg(msg,&ack_msg);
		}
		return 0;

		case GM_MSG_ENCHANT:
		{
			ASSERT(msg.content_length >= sizeof(enchant_msg));
			enchant_msg ech_msg = *(enchant_msg*)msg.content;
			if(_player_cannot_attack && ech_msg.ainfo.attacker.IsPlayerClass()) return 0;
			if(msg.source.IsPlayerClass() && ech_msg.is_flying) return 0;
			if(!IsNPCOwner(ech_msg.ainfo.attacker.id, ech_msg.ainfo.team_id, ech_msg.ainfo.master_id)) return 0;
			if(!ech_msg.helpful)
			{
				if(_attack_rule == 0) return 0;
				//有害法术的攻击判定类似攻击
				bool IsInvader = false;
				if(msg.source.IsPlayerClass())
				{
					if(!(ech_msg.attacker_mode & attack_msg::PVP_ENABLE))
					{
						return 0;
					}
					else
					{
						IsInvader = true;
					}
					if(!ech_msg.force_attack) return 0;
				}
				ech_msg.is_invader = IsInvader;
			}
			HandleEnchantMsg(msg,&ech_msg);
		}
		return 0;

		case GM_MSG_NPC_TRANSFORM:
		{
			ASSERT(msg.content_length == sizeof(msg_npc_transform));
			if(msg.content_length == sizeof(msg_npc_transform)) 
			{
				msg_npc_transform * data = (msg_npc_transform*)msg.content;
				
				gnpc_imp * __this = TransformNPC(data->id_in_build, 0);
				if(__this == NULL) 
				{
					GLog::log(GLOG_ERR,"NPC在转换的时候发生错误");
					return 0;
				}
				

				//加入定时控制filtero
				filter * pFilter = new towerbuild_filter(__this,FILTER_INDEX_TOWERBUILD,
									data->id_buildup,data->time_use);
				__this->_filters.AddFilter(pFilter);

				//发送更换状态数据
				__this->_runner->disappear();
				__this->_runner->enter_world();
				
			}
		}
		return 0;


	default:
		return gnpc_imp::MessageHandler(msg);
	}
	return 0;
}

service_npc::~service_npc()
{
}

bool 
service_npc::Save(archive & ar)
{
	gnpc_imp::Save(ar);
	ar << _tax_rate;
	size_t size = _service_list.size();
	ar << size;

	for(LIST::iterator it = _service_list.begin();it != _service_list.end(); ++it)
	{
		ar << it->second->GetProviderType();
		it->second->Save(ar);
	}
	return true;
}

bool 
service_npc::Load(archive & ar)
{
	gnpc_imp::Load(ar);
	ar >> _tax_rate;
	size_t size;
	ar >> size;
	for(size_t i = 0;i < size; i ++)
	{
		int provider_id;
		ar >> provider_id;
		service_provider * provider = service_manager::CreateProviderInstance(provider_id);
		ASSERT(provider);
		provider->Load(ar);
		provider->ReInit(this);
		_service_list.AddProvider(provider);
	}
	return true;
}

int
service_npc::DoAttack(const XID & target,char force_attack)
{
	attack_msg attack;
	MakeAttackMsg(attack,force_attack);
	FillAttackMsg(target,attack);

	//设置PVP强制攻击
//	attack.attacker_mode = attack_msg::PVP_ENABLE;
//	attack.force_attack = true;
	attack.target_faction = 0xFFFFFFFF;

	MSG msg;
	BuildMessage(msg,GM_MSG_ATTACK,target,_parent->ID,_parent->pos,0,&attack,sizeof(attack));
	gmatrix::SendMessage(msg);

	return 0;
}

void    
service_npc::FillAttackMsg(const XID & target, attack_msg & attack)
{       
	gactive_imp::FillAttackMsg(target,attack);
	//设置PVP强制攻击
//	attack.attacker_mode = attack_msg::PVP_ENABLE;
//	attack.force_attack = true;
}

void    
service_npc::FillEnchantMsg(const XID & target, enchant_msg & enchant)
{       
	gactive_imp::FillEnchantMsg(target, enchant);
	//设置PVP强制攻击
//	enchant.attacker_mode = attack_msg::PVP_ENABLE;
//	enchant.force_attack = true;
}

