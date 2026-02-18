#include <stdlib.h>
#include "gmatrix.h"
#include "filter.h"
#include "filter_man.h"
#include "actobject.h"
#include "playertemplate.h"
#include "clstab.h"
#include "sfilterdef.h"

#ifdef FILTER_USE_SYSTIME
#include <time.h>
#define SYS_TIME  time(NULL)
#else
#include <timer.h>
extern abase::timer g_timer;
#define SYS_TIME  g_timer.get_systime()
#endif

DEFINE_SUBSTANCE_ABSTRACT(filter, substance, CLS_FILTER)

bool filter::ResetTimeout()
{
	if(_timeout_t == 0 || !_active || _is_deleted) return false;    //无超时或者未激活的不设置超时
	_timeout_t = SYS_TIME + GetTimeout();
	return true;
}

int filter::GetTimeLeft()
{
	if(_timeout_t == 0 || !_active || _is_deleted) return 0;
	return _timeout_t - SYS_TIME;
}

bool timeout_filter::ResetTimeout()
{
	if(_time == 0 || _is_deleted) return false;
	_timeout = _time;
	return true;
}

bool timeout_filter::Overlay()
{
	if(!_parent.IsFilterExist(_filter_id))
	{
		return true;
	}
	_parent.ModifyFilter(_filter_id, FMID_OVERLAY, this, sizeof(this));
	return _attach;
}

void timeout_filter::OnModify(int ctrlname, void * ctrlval,size_t ctrllen)
{
	if(ctrlname == FMID_OVERLAY_CNT)
	{
		*((int*)ctrlval) = _overlay_cnt;
		return;
	}
	else if(ctrlname == FMID_OVERLAY)
	{
		timeout_filter* pfilter = (timeout_filter*)ctrlval;
		if(pfilter->GetFilterID() == _filter_id && _overlay_cnt >= 1)
		{
			_is_deleted = true;
			pfilter->_overlay_cnt = _overlay_cnt < _max_overlay_cnt ? _overlay_cnt + 1 : _overlay_cnt;
			pfilter->SetAttach();
		}
	}
}

