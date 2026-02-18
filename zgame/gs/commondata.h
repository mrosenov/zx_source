
#ifndef __ONLINEGAME_GAME_GS_COMMON_DATA_H_
#define __ONLINEGAME_GAME_GS_COMMON_DATA_H_

#include "commonvalue.h"
#include <map>
#include <set>
#include "rwlock.h"
#include "commondata_if.h"	//for CMN_DATA_SHARE


class CommonData	//gs、delivery间公共的数据，包含了key-value对应的数据
{
	friend class CommonDataMan;
	friend class SyncTransaction;
public:
	RWLock _lock;

	void ReadLock() {_lock.ReadLock();}
	void ReadUnlock() {_lock.ReadUnlock();}
	void WriteLock() {_lock.WriteLock();}
	void WriteUnlock() {_lock.WriteUnlock();}

	//获得某个KEY对应的Value. 调用前请加锁
	bool GetValue (int key, Value& value);

	//修改数据请用SyncTransaction

	int GetDataType() const {return _data_type;}
	int GetFlag() const {return _flag;}
	bool IsLocal() const {return !(_flag & CMN_DATA_SHARE);}//是否只在GS局部使用，不与其它GS共享
	bool IsNeedDB() const {return (_flag & CMN_DATA_NEED_DB);}//需要数据库？
	void SetConsistent() {if (!_consistent) _consistent=true;}
	bool IsConsistent() const {return _consistent;}

	void ForEach (CommonDataInterface::Iterator &it);
private:
	int _data_type;
	typedef std::map<int, Value> DATA_MAP;
	DATA_MAP _map;

	int _flag;	//数据空间的一些属性
	bool _consistent;  //数据是否完整

	CommonData(int data_type, int flag):_data_type(data_type),_flag(flag),_consistent(false) {}
	~CommonData() {}

	void DataHandler (const Octets &data);

	//修改某个KEY对应的Value. 调用前请加锁
	void ChangeValue (int key, int op, Value& value);

};

class CommonDataRpcObject;
class CommonDataMan
{
	//读写锁用来保护对_map结构的多线程访问(副本可以动态创建，所以这一块需要锁保护)
	RWLock _map_lock;
	std::map<int, CommonData*> _map;
	int _session_key;

	CommonDataMan();
	~CommonDataMan();

	static CommonDataMan Instance;
public:
	static CommonDataMan& GetInstance()
	{
		return Instance;
	}

	//创建、销毁和获取
	CommonData* Create(int data_type, int flag);
	//void Destroy(CommonData *);
	void Destroy(int data_type);
	CommonData* Get(int data_type);

	//从网络过来的数据，进行分发
	void DispatchData (int session_key, int data_type, const Octets& data);
	//向网络发送，同步数据
	void SyncDelivery (int data_type, Octets& data);

	void OnDeliveryDisConnect();

	void SendRpcCall(int data_type, int key, const Value& diff_value,
			 const Value& range_low, const Value& range_high, int op, CommonDataRpcObject *obj);
        void OnRecvRpcCallResult(int session_key, void *data,size_t size, int result, void *user_data);

private:
        void UpdateSessionKey (int session_key);
};

//一个Transaction的修改操作原子的，不可打断的。
class SyncTransaction
{
	CommonData* _assoc_data;
	std::vector<data_atomic_action> _queue;
	bool _update_local;
	bool _submitted;
public:
	explicit SyncTransaction (int data_type);
	SyncTransaction (CommonData*);
	~SyncTransaction () { if (!_submitted) Submit();}

	//数据的相关操作
	int ValueInc (int key, Value::Type t = Value::TYPE_INT);	
	int ValueDec (int key, Value::Type t = Value::TYPE_INT);	

	int ValueAdd (int key, const Value& value);  //增量更新
	int ValueSub (int key, const Value& value);
	int ValueSet (int key, const Value& value);  //设置

	void Cancel();
	void Submit();
};

#endif
