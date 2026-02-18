#include "pvplimit_filter.h"
#include "clstab.h"
#include "actobject.h"
#include "playertemplate.h"
#include "worldmanager.h"
#include <glog.h>

DEFINE_SUBSTANCE(pvp_limit_filter,filter,CLS_FILTER_PVP_LIMIT)

void 
pvp_limit_filter::OnAttach()
{
	//发送进入安全区的消息
	_parent.GetImpl()->_runner->enter_sanctuary();
	_parent.EnterSanctuary();

	gobject * pObj = _parent.GetImpl()->_parent;
	GLog::log(GLOG_INFO,"用户%d进入安全区(%f,%f)",pObj->ID.id,pObj->pos.x,pObj->pos.z);
}

void 
pvp_limit_filter::OnDetach()
{
	//通知客户端退出了安全区
	_parent.GetImpl()->_runner->leave_sanctuary();
	_parent.LeaveSanctuary();
	
	gobject * pObj = _parent.GetImpl()->_parent;
	GLog::log(GLOG_INFO,"用户%d离开安全区(%f,%f)",pObj->ID.id,pObj->pos.x,pObj->pos.z);
}


void 
pvp_limit_filter::TranslateRecvAttack(const XID & attacker,attack_msg & msg)
{
	msg.attacker_mode &= ~attack_msg::PVP_ENABLE;
	msg.attacker_mode |= attack_msg::PVP_SANCTUARY;
	msg.force_attack = 0;
}       

void 
pvp_limit_filter::TranslateRecvEnchant(const XID & attacker,enchant_msg & msg)
{
/*
	if(!msg.helpful)
	{
		msg.attacker_mode &= ~attack_msg::PVP_ENABLE;
		msg.force_attack = 0;
	}
	*/

	msg.attacker_mode &= ~attack_msg::PVP_ENABLE;
	msg.attacker_mode |= attack_msg::PVP_SANCTUARY;
	msg.force_attack = 0;
}

void 
pvp_limit_filter::TranslateSendAttack(const XID & target,attack_msg & msg, char type, float r, int mc)
{
	msg.attacker_mode &= ~attack_msg::PVP_ENABLE;
	msg.attacker_mode |= attack_msg::PVP_SANCTUARY;
	msg.force_attack = 0;
}

void 
pvp_limit_filter::TranslateSendEnchant(const XID & target,enchant_msg & msg)
{
	msg.attacker_mode &= ~attack_msg::PVP_ENABLE;
	msg.attacker_mode |= attack_msg::PVP_SANCTUARY;
	msg.force_attack = 0;
}


void 
pvp_limit_filter::Heartbeat(int tick)
{
	if((_counter += 1) < 7) return;
	//每隔7秒检查是否该脱离了安全区
	_counter = 0;
	if(!_parent.GetImpl()->GetWorldManager()->GetCityRegion()->IsInSanctuary(_parent.GetPos().x, _parent.GetPos().z))
	{
		_is_deleted = true;
	}
}

