#include "base_filter.h"
#include "skill.h"

namespace GNET
{

    void base_filter::Init(object_interface object, int time, float ratio, int buffid, float amount, float value, bool tiny_time)
    {
	SetParent(object);
	_time = time;
	_ratio = ratio;
	_buffid = buffid;
	_amount = amount;
	_value = value;
	
	int s_time = tiny_time ? 0 : time;
	
	if(_amount < 0.01)
		_heartbeat_count_max  = 1 + s_time;
	else
		_heartbeat_count_max = 1 + (int)( (s_time*1000) / _amount );
	if (_flag & FLAG_BROADCAST)
	{
	    const SkillStub * stub = SkillStub::GetStub(_skillid);
	    if (stub && (stub->rangetype == RANGE_BROADCAST))
		_flag = _flag | FLAG_BROADCAST;
	    else
		_flag = _flag & ~FLAG_BROADCAST;
	}

	OnInit();
    }

    void base_filter::OnAttach()
    {
	if (CheckBegin())
	    Activate();
	if (_hstate)
	    _parent.UpdateBuff(_filter_id, _hstate, 1, GetTimeout(), _hc->_polyrule==RULE_ISOLATE);
	if (_vstate)
	    _parent.IncVisibleState(_vstate);

	if (_bufftype & BUFF_TYPE_SWITCH)
	{
	    if(_parent.GetSkillWrapper().SwitchOn(_skillid,_filter_id,IsAura()))
		;//_parent.SendClientSkillNotify(SNOTIFY_SWITCHON, _skillid);
	}
    }

    void base_filter::OnSavedAttach()
    {
	if (_is_actived)
	{
	    SavedEffect();
	}
	if (_hstate)
	    _parent.UpdateBuff(_filter_id, _hstate, _polycount+1, GetTimeLeft(), _hc->_polyrule==RULE_ISOLATE);
	if (_vstate)
	    _parent.IncVisibleState(_vstate);

	if (_bufftype & BUFF_TYPE_SWITCH)
	{
	    if(_parent.GetSkillWrapper().SwitchOn(_skillid,_filter_id,IsAura()))
		;//_parent.SendClientSkillNotify(SNOTIFY_SWITCHON, _skillid);
	}
    }

    // 结束时调用
    void base_filter::OnDetach()
    {
	if (_is_actived)
	    UndoEffect();
	if (_hstate)
	    _parent.RemoveBuff(_filter_id, _hc->_polyrule==RULE_ISOLATE);
	if (_vstate)
	    _parent.DecVisibleState(_vstate);

	if (_bufftype & BUFF_TYPE_SWITCH)
	{
	    if (_parent.GetSkillWrapper().SwitchOff(_skillid, _filter_id))
		;//_parent.SendClientSkillNotify(SNOTIFY_SWITCHOFF, _skillid);
	}
    }

};
