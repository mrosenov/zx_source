
#ifndef __ONLINEGAME_GAME_GS_COMMON_DATA_H_
#define __ONLINEGAME_GAME_GS_COMMON_DATA_H_

#include "commonvalue.h"
#include <map>
#include "mutex.h"

#include "gcommondata"
#include "dbputcommondataarg"

using GNET::Thread::RWLock;

class SvValue: public Value
{
	bool _dirty;
public:
	SvValue():Value(),_dirty(true) {}
	~SvValue() {}
	explicit SvValue(const int &v):Value(v),_dirty(true) {}
	explicit SvValue(const Octets &v):Value(v),_dirty(true) {}

	SvValue(const SvValue& rhs):Value(rhs),_dirty(true) {}
	SvValue(void *p,size_t size):Value(p,size) {}

	SvValue& operator=(const SvValue& rhs) {
		if (this == &rhs)
			return *this;
		type = rhs.type;
		data = rhs.data;
		_dirty = rhs._dirty;
		return *this;
	}
	
	bool IsDirty () const { return _dirty;}
	void SetDirty(bool dirty) {_dirty = dirty;}
};

using GNET::DBPutCommonDataArg;
class CommonData
{
	friend class CommonDataMan;
public:
	RWLock _lock;	//对整个数据map的读写锁

	void ReadLock() {_lock.RDLock();}
	void ReadUnlock() {_lock.UNLock();}
	void WriteLock() {_lock.WRLock();}
	void WriteUnlock() {_lock.UNLock();}

	//修改某个KEY对应的Value. 调用前请加锁
	Value& ChangeValue (int key, int op, Value& value);
	//获得某个KEY对应的Value. 调用前请加锁
	bool GetValue (int key, Value& value) const;

	int GetDataType() const {return _data_type;}

	void OnLoadValue (int key, SvValue& value);

	bool IsNeedGameDB() const {return _need_gamedb;}
	void SetNeedGameDB(bool b) {_need_gamedb = b;}

	void OperateAllValue (int op, Value& value);
private:
	int _data_type;
	typedef std::map<int, SvValue> DATA_MAP;	//数据
	DATA_MAP _map;

	bool _need_gamedb;	//是否需要保存至数据库

	CommonData(int data_type):_data_type(data_type),_need_gamedb(true) {}
	~CommonData() {}

	void DataHandler (const Octets &data);
	void RpcDataHandler (const Octets &data, int &result, Value& new_value);
	void Heartbeat ();
	void OnDBPutFailed (const DBPutCommonDataArg& arg);
};


using GNET::GCommonData;
class CommonDataMan : public IntervalTimer::Observer
{
	std::map<int, CommonData*> _map;
	RWLock _lock;	//对_map的读写锁

	int _session_key;
	CommonDataMan();
	~CommonDataMan();

	int status;
	static CommonDataMan _Instance;
public:
	static CommonDataMan* Instance ()
	{
		return &_Instance;
	}

	enum {
		STATUS_INIT,
		STATUS_OPEN,
	};

	bool Initialize();

	//创建、销毁和获取CommonData空间
	CommonData* Create(int data_type);
	CommonData* Get(int data_type);

	//从网络过来的数据，进行分发
	void DispatchData (int session_key, int data_type, const Octets& data);
	CommonData* PreDispatch(int session_key, int data_type);
	//向网络发送，同步数据
	void SyncAllGs (int data_type, Octets&data);
	void SyncGs (int gs_id, int data_type, Octets&data);
	void SyncGs (int gs_id);


	//分发RPC数据, Rpc数据是用于精确控制的
	void DispatchRpcData(int session_key, int data_type, const Octets& in_data,
			int& result, Octets &out_data);

	bool Update ();

	//和数据库打交道的部分
        void OnDBConnect(Protocol::Manager *manager, int sid);
	void OnDBDisConnect();
	bool OnLoad(std::vector<GCommonData>& data);
	void OnDBPutFailed (DBPutCommonDataArg& arg);
	void Open();
	int GetStatus() { return status; }

	bool IsOpen() { return status == STATUS_OPEN; }
	bool SimpleGet(int key, int & value);		 //针对整数全局变量的简单存取接口
	bool SimplePut(int key, int value, bool need_db);
};

//一个Transaction的修改操作原子的，不可打断的。
class SyncTransaction
{
	CommonData* _assoc_data;
	std::vector<data_atomic_action> _queue;
	bool _update_local;
	bool _submitted;

	bool _silent;
	bool _broadcast;
	int gs_sid; 
public:
	SyncTransaction (int data_type);
	SyncTransaction (CommonData*);
	~SyncTransaction () { if (!_submitted) Submit();}

	//数据的相关操作
	int ValueInc (int key);		//自增
	int ValueDec (int key);		//自减
	int ValueAdd (int key, const Value& value);  //增量更新
	int ValueDel (int key, const Value& value);
	int ValueSet (int key, const Value& value);  //设置

	int ValueOp (int key, int op, const Value& value);

	void Cancel();
	void Submit();

	void SetGsSid(int sid) {gs_sid = sid;_broadcast=false;}
	void SetSilent(bool silent) {_silent=silent;}
	void SetUpdateLocal (bool update) {_update_local = update;}
};


#endif
