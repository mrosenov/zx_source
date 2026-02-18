#include "commondata.h"
#include "commondatasync.hpp"
#include "gproviderserver.hpp"
#include "dbgetcommondataarg"
#include "dbputcommondataarg"
#include "dbputcommondata.hrp"
#include "gamedbclient.hpp"

#include "commondataauto.h"
//#include "astro.h"
//#include "business.h"


//从GS代码中拷来的关于数据空间的属性。实际只用到了CMN_DATA_NEED_DB。
enum {
        CMN_DATA_LOCAL   = 0,  //内部的使用的，非共享
        CMN_DATA_SHARE   = 1,  //在GS间共享
        CMN_DATA_NEED_DB = 2,  //需要存盘
        CMN_DATA_DEFAULT = 3,  //默认 (CMN_DATA_SHARE | CMN_DATA_NEED_DB)
};

void CommonData::DataHandler (const Octets &data)
{
	SyncTransaction t(this);
	DataStream is(data);
	{
		RWLock::WRScoped l(_lock);
		try {
			size_t size;
			is >> size;

			for (; size; size--)
			{
				data_atomic_action action;
				is >> action._key >> action._op >> action._value;
				Value& new_value= ChangeValue (action._key, action._op, action._value);
				t.ValueSet(action._key, new_value);
			}
			if (!is.eos())
			{
				int flag;
				is >> flag;
				SetNeedGameDB(flag & CMN_DATA_NEED_DB);
			}
		}
		catch (DataStream::Exception& e)
		{
			DEBUG_PRINT ("Common data decode error.\n");
			t.Cancel();
		}
	}
	t.Submit();
}
bool CommonData::GetValue (int key, Value& value) const
{
	std::map<int, SvValue>::const_iterator it = _map.find (key);
	if (it != _map.end())
	{
		value = it->second;
		return true;
	}
	return false;
}
Value& CommonData::ChangeValue (int key, int op, Value& value)
{
	Value & OldValue = _map[key];
	if (OldValue.IsNull())
	{
		OldValue.ChangeType (value.GetType());
	}
	else if (OldValue.GetType() != value.GetType())
	{
		DEBUG_PRINT("CommonValue: ChangeType, from %d to %d. data_type=%d key=%d.\n",
				OldValue.GetType(), value.GetType(), _data_type, key);
		OldValue.ChangeType (value.GetType());
	}
	OldValue.Operate (op, value);

	(static_cast<SvValue&>(OldValue)).SetDirty(true);

	if (OldValue.GetType() == Value::TYPE_INT)
	{
		DEBUG_PRINT ("Common Data Updated: data_type (%d) key (%d) value(%d)\n", 
		_data_type,key, OldValue.ToInteger());
	}
	return OldValue;
}
void CommonData::OperateAllValue (int op, Value& value)
{
	SyncTransaction trans(this);
	trans.SetUpdateLocal (true);
 	for (DATA_MAP::iterator it = _map.begin(); it != _map.end(); ++it)
	{
		Value new_value = ChangeValue (it->first, op, value);
		trans.ValueSet(it->first, new_value);
	}	
}
void CommonData::OnLoadValue (int key, SvValue& v)
{
	v.SetDirty(false);
	_map[key] = v;

	if (v.GetType() == Value::TYPE_INT)
		DEBUG_PRINT ("CommonData:Load data_type(%d) key(%d) value(%d).\n", _data_type, key, v.ToInteger());
}
void CommonData::OnDBPutFailed (const DBPutCommonDataArg& arg)
{
	//失败的时候，目前只是又将数据设为脏
	const std::vector<GCommonData>& vt = arg.data;
	std::vector<GCommonData>::const_iterator it;

	for ( it = vt.begin(); it != vt.end(); ++it)
	{
		_map[it->key].SetDirty(true);
	}
}
void CommonData::Heartbeat ()
{
	if (!_need_gamedb)
	{
		return;
	}
	DBPutCommonDataArg arg;

	RWLock::WRScoped l(_lock);
	std::map<int,SvValue>::iterator it;

	size_t total_size =0;
	for (it = _map.begin(); it != _map.end(); ++it)
	{
		if (total_size > 409600)
		{
		   break;
		}
		if (it->second.IsDirty())
		{
			DataStream os;
			os << static_cast<Value&>(it->second);
			DEBUG_PRINT ("Put data_type(%d), key (%d), value_size(%d)\n",
					_data_type, it->first, os.GetData().size());
			//os.GetData().dump();
			GCommonData e (_data_type,it->first, os.GetData());
			arg.data.push_back (e);

			total_size += (os.GetData().size() + 12);

			it->second.SetDirty(false);
		}
	}
	if (arg.data.size() !=0)
	{
		if (CommonDataMan::Instance()->GetStatus() != CommonDataMan::STATUS_OPEN)
			Log::log(LOG_ERR, "DBPutCommonData but status invalid");
		DBPutCommonData * rpc = (DBPutCommonData*) Rpc::Call(RPC_DBPUTCOMMONDATA, arg);
		GameDBClient::GetInstance()->SendProtocol (rpc);
	}
}

void CommonData::RpcDataHandler (const Octets &data, int &rpc_result, Value& ret_value)
{
	SyncTransaction t(this);
	DataStream is(data);
	{
		//加写锁
		RWLock::WRScoped l(_lock);

		try {
			int key, op_mode;
			Value d, low, high;
			is >> key >> d >> low >> high >> op_mode;

			Value cv;
			if (!GetValue (key, cv))
			{
				cv.ChangeType(d.GetType());
			}
			Value OldValue(cv), diff (d);

			cv.Operate (op_mode, d);

			if (cv.IsInRange(low,high))
			{
				ret_value = ChangeValue (key, op_mode, diff);
				t.ValueSet(key, ret_value);

				if (op_mode== OP_MODE_XCHG)
					ret_value = diff;
				rpc_result = 0;
			}
			else
			{
				ret_value = OldValue;
				rpc_result = 1;	//越界
			}
		}
		catch (DataStream::Exception& e)
		{
			DEBUG_PRINT ("Common data decode error.\n");
			t.Cancel();
		}
	}
	t.Submit();
}

CommonDataMan CommonDataMan::_Instance;

CommonDataMan::CommonDataMan():_session_key(time(0)),status(STATUS_INIT)
{
	//可以考虑在load成功数据后，再设置_session_key
}
CommonDataMan::~CommonDataMan()
{
	RWLock::WRScoped l(_lock);
	std::map<int, CommonData*>::iterator it = _map.begin();
	std::map<int, CommonData*>::iterator ie = _map.end();
	for (; it != ie; ++it)
	{
		delete it->second;
	}
	_map.clear();
}
bool CommonDataMan::Initialize()
{
	IntervalTimer::Attach(this,6000000/IntervalTimer::Resolution());
	return true;
}

void CommonDataMan::DispatchData (int session_key, int data_type, const Octets& data)
{
	if (status != STATUS_OPEN)
	{
		Log::log(LOG_ERR, "CommonDataSync status %d invalid", status);
		return;
	}
	CommonData *pcd = PreDispatch (session_key, data_type);
	if (pcd == NULL)  return;

	pcd->DataHandler (data);
}

void CommonDataMan::SyncGs(int gs_sid)
{
	bool empty = true;

	RWLock::RDScoped l(_lock);
	std::map<int, CommonData*>::iterator It,Ie;
	It = _map.begin();
	Ie = _map.end();

	for (; It!=Ie; ++It)
	{
		SyncTransaction t(It->second);
		t.SetGsSid(gs_sid);
		{
			CommonData* pCD = It->second;
			RWLock::RDScoped ll(pCD->_lock);

			std::map<int,SvValue>::iterator it;
			for (it = pCD->_map.begin(); it != pCD->_map.end(); ++it)
			{
				t.ValueSet (it->first, it->second);
				empty = false;
			}
		}
		t.Submit();
	}
	if (empty)
	{
		DataStream os;
		os << 0;
		SyncGs(gs_sid, 0, os.GetData());
	}
}
void CommonDataMan::SyncGs (int gs_id, int data_type, Octets& data)
{
	//向gs发消息。
	CommonDataSync cds(_session_key, data_type, data);
	GProviderServer::GetInstance()->Send(gs_id,cds);
}
void CommonDataMan::SyncAllGs (int data_type, Octets& data)
{
	//向gs广播
	CommonDataSync cds(_session_key, data_type, data);
	GProviderServer::GetInstance()->BroadcastProtocol(cds);
}
CommonData* CommonDataMan::Create (int data_type)
{
	RWLock::WRScoped l(_lock);
	std::map<int, CommonData*>::iterator it;
	it = _map.find (data_type);
	if (it != _map.end())
	{
		return it->second;
	}
	CommonData *pcd = new CommonData(data_type);
	_map.insert (std::make_pair (data_type, pcd));
	return pcd;
}
CommonData* CommonDataMan::Get (int data_type)
{
	RWLock::RDScoped l(_lock);
	std::map<int, CommonData*>::iterator it;
	it = _map.find (data_type);
	return it == _map.end()? NULL: it->second;
}

bool  CommonDataMan::Update()
{
	RWLock::RDScoped l(_lock);
	std::map<int, CommonData*>::iterator it;
	for (it = _map.begin(); it != _map.end(); ++it)
	{
		it->second->Heartbeat();
	}
	return true;
}

CommonData* CommonDataMan::PreDispatch(int session_key, int data_type)
{
	if (session_key ==0)
	{
		return NULL;
	}
	RWLock::WRScoped l(_lock);
	std::map<int, CommonData*>::iterator it;
	if (_session_key != session_key)
	{
		//client和server已经不一致了。
		printf ("invalid session_key\n");
		return NULL;
	}
	CommonData *pcd;

	it = _map.find (data_type);
	if (it == _map.end())
	{
		pcd = new CommonData(data_type);
		_map.insert (std::make_pair (data_type, pcd));
	}
	else
	{
		pcd = it->second;
	}
	return pcd;
}

void CommonDataMan::DispatchRpcData(int session_key, int data_type, const Octets& in_data,
		int& rpc_result, Octets &out_data)
{
	if (status != STATUS_OPEN)
	{
		Log::log(LOG_ERR, "CommonDataChange status %d invalid", status);
		return;
	}
	CommonData *pcd = PreDispatch (session_key, data_type);
	if (pcd == NULL)  return;

	Value ret_value;
	pcd->RpcDataHandler (in_data, rpc_result, ret_value);

	DataStream os;
	os << ret_value;
	out_data.swap (os.GetData());
}
void CommonDataMan::OnDBConnect(Protocol::Manager *manager, int sid)
{
	//掉线后gamedb中的数据是老的，此时以delivery为准。
	if (status == STATUS_INIT)
	{
		manager->Send(sid,Rpc::Call(RPC_DBGETCOMMONDATA,DBGetCommonDataArg()));
	}
}
bool CommonDataMan::OnLoad(std::vector<GCommonData>& data)
{
	if (status != STATUS_INIT)
		return false;
	std::vector<GCommonData>::iterator it;
	for (it = data.begin(); it!= data.end(); ++it)
	{
		CommonData* pcd = Get (it->scole);
		if (pcd == NULL)
		{
			pcd = Create (it->scole);
		}
		//it->value.dump();
		DataStream is(it->value.begin(), it->value.size());
		SvValue value;
		try {
			is >> static_cast<Value&>(value);
		}
		catch (DataStream::Exception &e)
		{
			DEBUG_PRINT ("Decode value from db error. key =%d\n", it->key);
			return true;
		}
		pcd->OnLoadValue (it->key, value);
	}
	return true;
}
void CommonDataMan::Open()
{
//	assert (status == STATUS_INIT);
	if (status != STATUS_INIT)
		return;
	status = STATUS_OPEN;

	//astro::Instance()->Start();		//天象
	//business::GetInstance().Start();	//跑商

	GProviderServer *gps = GProviderServer::GetInstance();
	GProviderServer::GameServerMap::iterator it;
	Thread::RWLock::RDScoped l(gps->locker_gameservermap);
	for (it = gps->gameservermap.begin(); it!= gps->gameservermap.end(); ++it)
	{
		SyncGs (it->second.sid);
	}
	CommonDataAuto::Instance()->Start();
}
void CommonDataMan::OnDBPutFailed (DBPutCommonDataArg& arg)
{
	CommonData *pcd = Get(arg.data[0].scole);
	pcd->OnDBPutFailed (arg);
}

bool CommonDataMan::SimpleGet(int key, int & value)
{
	if (status != STATUS_OPEN)
	{
		Log::log(LOG_ERR, "CommonData SimpleGet status %d invalid, key=%d", status, key);
		return false;
	}
	CommonData * pcd = Get(0);
	if (pcd == NULL)
	{
		Log::log(LOG_ERR, "CommonData SimpleGet data_type not ready, key=%d", key);
		return false;
	}
	Value v;
	if (!pcd->GetValue(key, v))
		return false;
	if (v.GetType() != Value::TYPE_INT)
	{
		LOG_TRACE("CommonData SimpleGet type invalid, key=%d", key);
		return false;
	}
	value = v.ToInteger();
	LOG_TRACE("commondata simple get key %d value %d", key, value);
	return true;
}
bool CommonDataMan::SimplePut(int key, int value, bool need_db)
{
	if (status != STATUS_OPEN)
	{
		Log::log(LOG_ERR, "CommonData SimplePut status %d invalid, key=%d value=%d", status, key, value);
		return false;
	}
	CommonData *pcd = PreDispatch (_session_key, 0);
	if (pcd == NULL)
		return false;
	Value v(value);
	pcd->ChangeValue(key, OP_MODE_SET, v);
	pcd->SetNeedGameDB(need_db);
	DataStream os;
	os << 1;
	os << key << OP_MODE_SET << v;
	SyncAllGs(pcd->GetDataType(), os.GetData());
	LOG_TRACE("commondata simple put key %d value %d", key, value);
	return true;
}

SyncTransaction::SyncTransaction(int data_type):
	_assoc_data(NULL), _update_local(false), _submitted(false),_silent(false),_broadcast(true)
{
	_assoc_data = CommonDataMan::Instance()->Get(data_type);
	ASSERT (_assoc_data != NULL);
}
SyncTransaction::SyncTransaction(CommonData* pcd): _assoc_data(pcd), 
	_update_local(false), _submitted(false), _silent(false), _broadcast(true)
{
}

void SyncTransaction::Submit ()
{
	if (_queue.empty()) return;

	DataStream os;
	os << _queue.size();

	std::vector<data_atomic_action>::iterator it,ie;
	it = _queue.begin();
	ie = _queue.end ();
	for (;it != ie; ++it)
	{
		os << it->_key << it->_op << it->_value;
	}

	if (_update_local)
	{
		RWLock::WRScoped l (_assoc_data->_lock);
		it = _queue.begin();
		ie = _queue.end ();
		for (;it != ie; ++it)
		{
			_assoc_data->ChangeValue (it->_key, it->_op, it->_value);
		}
	}
	_queue.clear();
	_submitted = true;

	if (_silent)
	{
		return;	
	}

	if (_broadcast)
	{
		CommonDataMan::Instance()->SyncAllGs(_assoc_data->GetDataType(), os.GetData());
	}
	else
	{
		CommonDataMan::Instance()->SyncGs(gs_sid, _assoc_data->GetDataType(), os.GetData());
	}
}

int SyncTransaction::ValueInc (int key)		//自增
{
	_queue.push_back (data_atomic_action(key,OP_MODE_INC,Value(0)));
	return 0;
}
int SyncTransaction::ValueDec (int key)		//自减
{
	_queue.push_back (data_atomic_action(key,OP_MODE_DEC,Value(0)));
	return 0;
}
int SyncTransaction::ValueAdd (int key, const Value& value)  //增量更新
{
	_queue.push_back (data_atomic_action(key,OP_MODE_ADD,value));
	return 0;
}
int SyncTransaction::ValueDel (int key, const Value& value)
{
	_queue.push_back (data_atomic_action(key,OP_MODE_SUB,value));
	return 0;
}
int SyncTransaction::ValueSet (int key, const Value& value)  //设置
{
	_queue.push_back (data_atomic_action(key,OP_MODE_SET,value));
	return 0;
}
int SyncTransaction::ValueOp (int key, int op, const Value& value)
{
	_queue.push_back (data_atomic_action(key,op,value));
	return 0;
}
void SyncTransaction::Cancel()
{
	_queue.clear();
}

