#include "commondata.h"
#include "gsp_if.h"
#include "dbgprt.h"
#include "glog.h"
#include "commondata_if.h"
#include "amemory.h"
#include "threadpool.h"


void CommonData::DataHandler (const Octets &data)
{
	DataStream is(data);

	//加写锁
	RWLock::Keeper l(_lock);
	l.LockWrite();

	try {
		size_t size;
		is >> size;

		for (; size; size--)
		{
			data_atomic_action action;
			is >> action._key >> action._op >> action._value;

			//对于client,收到的全是SET操作
			_map[action._key] = action._value;
			//__PRINTF ("Common Data Set: key (%d) value(%d)\n", action._key, action._value.ToInteger());
		} 
		SetConsistent();
	}
	catch (DataStream::Exception& e)
	{
		__PRINTF ("Common data decode error.\n");
	}
}
bool CommonData::GetValue (int key, Value& value)
{
	//由调用者去加锁
	ASSERT (_lock.CheckReadLock());

	std::map<int, Value>::iterator it = _map.find (key);
	if (it != _map.end())
	{
		value = it->second;
		return true;
	}
	return false;
}
void CommonData::ChangeValue (int key, int op, Value& value)
{
	//由调用者去加锁
	ASSERT (_lock.CheckWriteLock());

	Value & old_value = _map[key];
	if (old_value.IsNull())
	{
		old_value.ChangeType (value.GetType());
	}
	else if (old_value.GetType() != value.GetType())
	{
		__PRINTF("CommonValue: ChangeType, from %d to %d. data_type=%d key=%d.\n",
				old_value.GetType(), value.GetType(), _data_type, key);
		old_value.ChangeType (value.GetType());
	}
	old_value.Operate(op, value);

	//__PRINTF ("Common Data Updated: key (%d) value(%d)\n", key, old_value.ToInteger());
}
void CommonData::ForEach (CommonDataInterface::Iterator &cit)
{
	ASSERT (_lock.CheckReadLock());
	DATA_MAP::iterator it;
	for (it = _map.begin(); it != _map.end(); ++it)
	{
		if ((it->second).GetType() == Value::TYPE_INT)
		{
			cit(_data_type, it->first, (it->second).ToInteger());
		}
		else
		{
			__PRINTF ("Common Data Iterate: Found Non integer,key=%d\n", it->first);
		}
	}
}


///////////////////////////Common Data Manager /////////////////////////////////////////////////


CommonDataMan CommonDataMan::Instance;
CommonDataMan::CommonDataMan():_session_key(0)
{

}
CommonDataMan::~CommonDataMan()
{
	std::map<int, CommonData*>::iterator it = _map.begin();
	std::map<int, CommonData*>::iterator ie = _map.end();
	for (; it != ie; ++it)
	{
		delete it->second;
	}
	_map.clear();
}

void CommonDataMan::DispatchData (int session_key, int data_type, const Octets& data)
{
	CommonData *pcd;
	UpdateSessionKey (session_key);

	std::map<int, CommonData*>::iterator it;
	it = _map.find (data_type);
	if (it != _map.end())
	{
		pcd = it->second;
	}
	else
	{
		__PRINTF ("Unknown Common Data type %d.\n", data_type);
		return;
	}
	pcd->DataHandler (data);
}
void CommonDataMan::SyncDelivery (int data_type, Octets& data)
{
	//向Delivery发消息。
	GMSV::SendCommonData (_session_key, data_type, data.begin(), data.size());
}
CommonData* CommonDataMan::Create (int data_type, int flag)
{
	RWLock::Keeper l(_map_lock);
	l.LockWrite();

	std::map<int, CommonData*>::iterator it;
	it = _map.find (data_type);
	if (it != _map.end())
	{
		__PRINTF ("Common Data type %d already created.\n", data_type);
		return it->second;
	}
	CommonData *pcd = new CommonData(data_type, flag);
	_map.insert (std::make_pair (data_type, pcd));
	return pcd;
}
void CommonDataMan::Destroy(int data_type)
{
	RWLock::Keeper l(_map_lock);
	l.LockWrite();

	std::map<int, CommonData*>::iterator it;
	it = _map.find (data_type);
	if (it != _map.end())
	{
		__PRINTF ("Common Data type %d already destroyed.\n", data_type);
		delete it->second;
		_map.erase(it);
	}
}

CommonData* CommonDataMan::Get (int data_type)
{
	RWLock::Keeper l(_map_lock);
	l.LockRead();

	std::map<int, CommonData*>::iterator it;
	it = _map.find (data_type);
	return it == _map.end()? NULL: it->second;
}
void CommonDataMan::OnDeliveryDisConnect()
{
	_session_key = 0;
}

void CommonDataMan::SendRpcCall(int data_type, int key, const Value& diff_value,
		const Value& range_low, const Value& range_high, int op, CommonDataRpcObject *obj)
{
	class RpcSendFail: public ONET::Thread::Runnable, public abase::ASmallObject
	{
		CommonDataRpcObject *_obj;
		int _reason;
	public:
		RpcSendFail(CommonDataRpcObject *obj, int reason): _obj(obj),_reason(reason){}
		void Run ()
		{
			_obj->OnGetResult(0, _reason);
			delete this;
		}
	};
	CommonData* pcd = Get(data_type);
	if (pcd == NULL || pcd->IsLocal ())
	{
		//obj->OnGetResult(0, CommonDataRpcObject::RESULT_DEST_IS_LOCAL);
		ONET::Thread::Pool::AddTask( new RpcSendFail(obj, CommonDataRpcObject::RESULT_DEST_IS_LOCAL));
		return;
	}
	DataStream os;
	os << key << diff_value << range_low << range_high << op;

	GMSV::SendCommonDataRpcCall (_session_key,data_type, os.GetData().begin(), os.GetData().size(),obj);
}

void CommonDataMan::UpdateSessionKey (int session_key)
{
	if (_session_key != session_key)
	{
		_session_key = session_key;

		std::map<int, CommonData*>::iterator it;
		for (it=_map.begin(); it!=_map.end(); ++it)
		{
			RWLock::Keeper ll(it->second->_lock);
			ll.LockWrite();
			it->second->_map.clear();
		}
	}
}

void CommonDataMan::OnRecvRpcCallResult(int session_key, void *p, size_t size,  int result, void* user_data)
{
	UpdateSessionKey (session_key);

	CommonDataRpcObject *obj = (CommonDataRpcObject*) user_data;

	DataStream is(p, size);
	Value value;

	try {
		is >> value;
		obj->OnGetResult(value.ToInteger(), result);
	}
	catch (const DataStream::Exception &e)
	{
		obj->OnGetResult(0, result);
	}
}

void OnRecvCommonDataRpcCallResult(int session_key, void *p, size_t size, int result, void*user_data)
{
	CommonDataMan& man = CommonDataMan::GetInstance();
	man.OnRecvRpcCallResult (session_key,p,size, result,user_data);
}

///////////////////////////SyncTransaction /////////////////////////////////////////////////

SyncTransaction::SyncTransaction(int data_type):
	_assoc_data(NULL), _update_local(true), _submitted(false)
{
	_assoc_data = CommonDataMan::GetInstance().Get(data_type);

	ASSERT (_assoc_data && "call CommonDataSpaceCreate frist\n");
}
SyncTransaction::SyncTransaction(CommonData* pcd): _assoc_data(pcd), 
	_update_local(true), _submitted(false)
{
}

void SyncTransaction::Submit ()
{
	if (_queue.empty()) return;

	std::vector<data_atomic_action>::iterator it,ie;
	if (_update_local)
	{
		RWLock::Keeper l (_assoc_data->_lock);
		l.LockWrite();
		it = _queue.begin();
		ie = _queue.end ();
		for (;it != ie; ++it)
		{
			_assoc_data->ChangeValue (it->_key, it->_op, it->_value);
		}
	}
	if (!_assoc_data->IsLocal())
	{
		DataStream os;
		os << _queue.size();

		it = _queue.begin();
		ie = _queue.end ();

		for (;it != ie; ++it)
		{
			os << it->_key << it->_op << it->_value;
		}
		os << _assoc_data->GetFlag();
		CommonDataMan::GetInstance().SyncDelivery (_assoc_data->GetDataType(), os.GetData());
	}
	_queue.clear();
	_submitted = true;
}

int SyncTransaction::ValueInc (int key, Value::Type t)		//自增
{
	_queue.push_back (data_atomic_action(key,OP_MODE_INC,Value::GetStub(t)));
	return 0;
}
int SyncTransaction::ValueDec (int key, Value::Type t)		//自减
{
	_queue.push_back (data_atomic_action(key,OP_MODE_DEC,Value::GetStub(t)));
	return 0;
}
int SyncTransaction::ValueAdd (int key, const Value& value)  //增量更新
{
	_queue.push_back (data_atomic_action(key,OP_MODE_ADD,value));
	return 0;
}
int SyncTransaction::ValueSub (int key, const Value& value)
{
	_queue.push_back (data_atomic_action(key,OP_MODE_SUB,value));
	return 0;
}
int SyncTransaction::ValueSet (int key, const Value& value)  //设置
{
	_queue.push_back (data_atomic_action(key,OP_MODE_SET,value));
	return 0;
}
void SyncTransaction::Cancel()
{
	_queue.clear();
}

void CommonDataHandler (int session_key, int data_type, void *p, size_t len)
{
	Octets data(p,len);
	CommonDataMan::GetInstance().DispatchData (session_key, data_type,data);
}

