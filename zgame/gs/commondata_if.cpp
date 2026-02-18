#include "commondata_if.h"
#include "commondata.h"

CommonDataInterface::CommonDataInterface(int data_type)
{
	_pcd = CommonDataMan::GetInstance().Get(data_type);
	ASSERT (_pcd != NULL && "请先创建数据空间");
}
bool CommonDataInterface::IsConsistent()
{
	return _pcd->IsConsistent();
}
bool CommonDataInterface::GetData(int index, int& val)
{
	Value v; 

	RWLock::Keeper l(_pcd->_lock); 
	l.LockRead();
	if (_pcd->GetValue(index, v))
	{
		val = v.ToInteger();
		return true;
	}
	return false;
}
void CommonDataInterface::SetData(int index, int val)
{
	SyncTransaction t(_pcd);
	t.ValueSet (index, Value(val));
}

bool CommonDataInterface::GetUserData(int index, void *data, int *size)
{
	Value v; 

	RWLock::Keeper l(_pcd->_lock); 
	l.LockRead();
	if (_pcd->GetValue(index, v))
	{
		memcpy (data, v.GetData().begin(), v.GetData().size());
		*size = v.GetData().size();
		return true;
	}
	return false;
}
void CommonDataInterface::SetUserData(int index, void *data, int size)
{
	SyncTransaction t(_pcd);
	t.ValueSet (index, Value(data,size));
}

void CommonDataInterface::IncData (int index)
{
	SyncTransaction t(_pcd);
	t.ValueInc (index);
}
void CommonDataInterface::DecData (int index)
{
	SyncTransaction t(_pcd);
	t.ValueDec (index);
}

void CommonDataInterface::AddData (int index, int diff_val)
{
	SyncTransaction t(_pcd);
	t.ValueAdd (index, Value(diff_val));
}
void CommonDataInterface::SubData (int index, int diff_val)
{
	SyncTransaction t(_pcd);
	t.ValueSub (index, Value(diff_val));
}
void CommonDataInterface::ForEach(CommonDataInterface::Iterator &it)
{
	RWLock::Keeper l(_pcd->_lock); 
	l.LockRead();
	_pcd->ForEach (it);
}

void SendCommonDataRpcObject (CommonDataRpcObject *obj, const CommonDataRpcArg& arg)
{
	CommonDataMan& man =CommonDataMan::GetInstance();
	obj->SetArg(arg);

	ASSERT (man.Get(arg._data_type) != NULL && "请先创建数据空间");

	ASSERT (arg._range_low <= arg._range_high);

	man.SendRpcCall(arg._data_type,arg._index, Value(arg._diff_val), Value(arg._range_low),
			Value(arg._range_high),obj->GetOp(),obj);
}
bool CommonDataSpaceCreate (int data_type, int flag)
{
	CommonDataMan& man =CommonDataMan::GetInstance();

	ASSERT (man.Get(data_type) == NULL && "数据空间已存在");

	CommonData * data = man.Create (data_type, flag);
	ASSERT(data != NULL);
       	if (data == NULL)
		return false;
	if (flag == CMN_DATA_LOCAL) 
		data->SetConsistent();
	return true;
}

bool CommonDataSpaceDestroy(int data_type)
{
	CommonDataMan& man =CommonDataMan::GetInstance();
	man.Destroy(data_type);
	return true;
}
