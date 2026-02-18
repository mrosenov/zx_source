#include "traceman.h"
#include "BrushMan.h"
#include "CDBrush.h"
#include "SMTree.h"
#include "NmdChd.h"
#include "A3DQuaternion.h"
#include <ASSERT.h>

SvrCD::CNmdChd *trace_manager::_nmd_element = NULL;

trace_manager::trace_manager()
{
	_brush_man = NULL;
	_sm_tree = NULL;
	_sm_ref = 0;
	
	_element_count = 0;
	attach = false;
}

trace_manager::~trace_manager()
{
	Release();
}

void trace_manager::Release()
{
	if(_brush_man) 
	{
		_brush_man->Release();
		delete _brush_man;
		_brush_man = 0;
	}

	if(!_sm_ref && _sm_tree)
	{
		if(!attach)
		{
			_sm_tree->Release();
			delete _sm_tree;
		}
		_sm_tree = 0;
		attach = false;
	}
}

void trace_manager::Attach(trace_manager * rhs)
{
	ASSERT(_sm_tree == NULL);
	_sm_tree = rhs->_sm_tree;
	attach = true;
}

bool 
trace_manager::Load(const char * filename)
{
	if(!_sm_tree)
	{
		_sm_tree = new SvrCD::CSMTree();
		if(!_sm_tree->Load(filename))
		{
			delete _sm_tree;
			return false;
		}
	}

	_brush_man = new SvrCD::CBrushMan();
	_brush_man->SetSmTree(_sm_tree);

	return true;
}

bool 
trace_manager::LoadElement(const char * filename)
{
	if(_nmd_element) return false;
	_nmd_element = new SvrCD::CNmdChd();
	if(!_nmd_element->Load(filename))
	{
		delete _nmd_element;
		_nmd_element = NULL;
		return false;
	}
	return true;
}

void 
trace_manager::ReleaseElement()
{
	if(!_nmd_element) return;
	delete _nmd_element;
	_nmd_element = NULL;
}

bool 
trace_manager::AABBTrace(const A3DVECTOR & start, const A3DVECTOR & offset, const A3DVECTOR & ext, bool & in_solid, float & ratio)
{
	if(!_brush_man)  return false;
	
	SvrCD::BrushTraceInfo info;
	info.Init( A3DVECTOR3(start.x,start.y + ext.y ,start.z), A3DVECTOR3(offset.x,offset.y,offset.z), A3DVECTOR3(ext.x,ext.y,ext.z));
	if(!_brush_man->Trace(&info)) return false;
	in_solid = info.bStartSolid;
	ratio = info.fFraction;
	return true;
}


int 
trace_manager::RegisterElement(int tid, int mid, const A3DVECTOR & pos, float dir0, float dir1, float rad)
{
	if(!_brush_man) return 0;
	A3DVECTOR3 target(pos.x, pos.y, pos.z);

	A3DVECTOR3 dir,up;
	if(dir1 <1e-3 && rad <1e-3)
	{
		dir.x = (float)cos(dir0);
		dir.y = 0;
		dir.z = (float)sin(dir0);

		up.x = 0;
		up.y = 1;
		up.z = 0;
	}
	else
	{
		//¼ÆËãDirºÍUP
		A3DVECTOR3 vAxis;
		float p = (float)sin(dir1);
		vAxis.x = p * (float)cos(dir0);
		vAxis.z = p * (float)sin(dir0);
		vAxis.y = (float)cos(dir1);

		A3DQUATERNION q(vAxis, rad *256.0f/255.0f);
		A3DMATRIX4 matTran;
		q.ConvertToMatrix(matTran);

		dir = matTran.GetRow(2);
		up  = matTran.GetRow(1);
	}

	if(tid)
	{
		_brush_man->AddNpcMine(_nmd_element,tid, _element_count + 1, target, up, dir);
//		_brush_man2->AddNpcMine(tid, _element_count + 1, target, up, dir);
		_element_count ++;
	}
	else if(mid)
	{
		_brush_man->AddDynObj(_nmd_element,mid, _element_count + 1, target,  up, dir);
//		_brush_man2->AddDynObj(mid, _element_count + 1, target,  up, dir);
		_element_count ++;
	}
	else
	{
		return 0;
	}
	return _element_count;
}

void trace_manager::EnableElement(int cid, bool active)
{
	if(!_brush_man) return ;
	_brush_man->EnableNmd(cid,active);
}

void trace_manager::Compare()
{
	if(!_brush_man) return;
//	SvrCD::CompareBMan(_brush_man, _brush_man2);
}



