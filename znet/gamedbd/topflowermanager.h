
#ifndef __TOPFLOWER_MANAGER_H
#define __TOPFLOWER_MANAGER_H

#include "rpcdefs.h"
//#include "callid.hxx"
//#include "state.hxx"

#include "algorithm"
#include "dbgettopflowerdataarg"
#include "dbgettopflowerdatares"
#include "gtabletopflowerdata"

namespace GNET
{
#define TOPFLOWER_CLIENT_COUNT 500 // 客户端榜
#define TOPFLOWER_ALL_COUNT 600 // 总榜单榜单人,多出100，防止角色被删
//#define __TEST__

//inline bool operator > (const GTopFlowerData& lhs, const GTopFlowerData& rhs)
//bool operator > (GTopFlowerData& lhs, GTopFlowerData& rhs)
inline bool TopFlower_Cmp(GTopFlowerData& lhs, GTopFlowerData& rhs)
{
	return lhs.count > rhs.count ? true : (lhs.count == rhs.count ? lhs.stamp < rhs.stamp : false);
}

typedef std::vector<GTopFlowerData> TTopFlowerData;
typedef std::map<int, GTableTopFlowerData> TTopFlowerFixedData;
class ListTopFlowerDataQuery : public StorageEnv::IQuery
{
public:
	friend class ListTopFlowerDataManager;
	TTopFlowerData m_vRecvFlowerData;
	int recv_total;
	TTopFlowerData m_vSendFlowerData;
	int send_total;
	TTopFlowerFixedData m_vFixedData;
	
	typedef std::map<int, int> TFixedMap;
	TFixedMap recv_fixed_map;
	TFixedMap send_fixed_map;
	bool isFixed;
	bool isExit;
public:
	ListTopFlowerDataQuery() : recv_total(0), send_total(0), isFixed(false), isExit(false)
	{ 
	}
	
	void Release()
	{
		m_vFixedData.clear();
		m_vRecvFlowerData.clear();
		recv_total = 0;
		m_vSendFlowerData.clear();
		send_total = 0;
	}

	void SetFixed(bool fixed) { isFixed = fixed; }
	bool GetFixed() { return isFixed; }
	void Split(const char * line, std::vector<std::string> & sub_strings, char div = ',')
	{
			int len = strlen(line);
			char * s = const_cast<char*>(line);
			while (len > 0)
			{
				int i = 0;
				for (; i < len && s[i] != div; ++i) {}
				if (i > 0)
				{
					sub_strings.push_back(std::string(s, i));
				}
				++i;
				len -= i;
				s += i;
			}
	}
	
	bool LoadFixedFile(const char * useridfile)
	{
		if (access(useridfile, R_OK) != 0)
                {
                        Log::log(LOG_ERR, "Can't access %s\n", useridfile);
                        return false;
                } 

		std::ifstream ifs(useridfile);
		string line;
		while (std::getline(ifs, line))
		{
			std::vector<std::string> sub_strings;
			Split(line.c_str(), sub_strings);

			if(sub_strings.size() != 3)
			{
				Log::log(LOG_ERR, "sub_strings size:%d != 3\n", sub_strings.size());
				continue;
			}
			std::string s_roleid = sub_strings[0];
			std::string s_isrecv = sub_strings[1];
			std::string s_count = sub_strings[2];

			//if(isdigit(s_roleid) || isdigit(s_count))
			//{
			//	Log::log(LOG_ERR, "s_roleid:%s, s_count:%s \n", s_roleid.c_str(), s_count.c_str());
			//	continue;
			//}
			
			int roleid = atoi(s_roleid.c_str());
			if(roleid == 0) continue; // 如果读入的文本非数字，则返回0

			int count = atoi(s_count.c_str());

			if(0 == s_isrecv.compare("1"))
			{
				recv_fixed_map.insert(std::make_pair(roleid, count));
			}
			else if(0 == s_isrecv.compare("0"))
			{
				send_fixed_map.insert(std::make_pair(roleid, count));
			}
			else
			{
				Log::log(LOG_ERR, "roleid:%d, count:%d \n", roleid, count);
			}
		}

		return true;
	}

	int RecvFixedCount(int roleid)
	{
		TFixedMap::iterator _it = recv_fixed_map.find(roleid);
		if(_it != recv_fixed_map.end())
		{
			return _it->second;
		}

		return 0;
	}

	int SendFixedCount(int roleid)
	{
		TFixedMap::iterator _it = send_fixed_map.find(roleid);
		if(_it != send_fixed_map.end())
		{
			return _it->second;
		}

		return 0;
	}


	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			int _roleid;
			Marshal::OctetsStream(key) >> _roleid;
	
			if(_roleid == 0) return true;// 0是标志已经修复

			StorageEnv::Storage * pbase = StorageEnv::GetStorage("base");
			StorageEnv::AtomTransaction txn_base;
			
			int _userid = 0;
			GRoleBase base;
			try
			{
				Marshal::OctetsStream key_base, value_base;
				key_base << _roleid;
				if(pbase->find(key_base, value_base, txn_base))
				{
					value_base >> base;
					_userid = base.userid;
				}
				else
				{
					//Log::log( LOG_INFO,"ListTopFlowerDataQuery, no found GRoleBase for roleid %d.", _roleid );
				}

			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}

			GTableTopFlowerData _topFlowerData;
			GTopFlowerData _recvFlowerData;
			GTopFlowerData _sendFlowerData;
			Marshal::OctetsStream(value) >> _topFlowerData; 
			if(_topFlowerData.recv_count > 0)
			{
				_recvFlowerData.userid = _userid;
				_recvFlowerData.roleid = _roleid;
				_recvFlowerData.rolename = _topFlowerData.rolename;
				_recvFlowerData.stamp = _topFlowerData.recv_stamp;
				_recvFlowerData.gender = _topFlowerData.gender;
				_recvFlowerData.count = _topFlowerData.recv_count;
				_recvFlowerData.isGiftTake = _topFlowerData.recv_isGiftTake;
				recv_total++;
			
				if(!GetFixed())
				{
					int old_count = _recvFlowerData.count;
					int reduce_count = RecvFixedCount(_roleid);
					int new_count = old_count - reduce_count;
					if(new_count < 0)
					{
						//Log::log( LOG_INFO,"ListTopFlowerDataQuery, recv, roleid:%d,old_count:%d,reduce_count:%d,new_count:%d.",_roleid,old_count,reduce_count,new_count);
						//exit(0);
						isExit = true;
					}
					else
					{
						_topFlowerData.recv_count = new_count;
						_recvFlowerData.count = new_count;
						//if(reduce_count>0) Log::log( LOG_INFO,"ListTopFlowerDataQuery, recv, roleid:%d,old_count:%d,reduce_count:%d,new_count:%d.",_roleid,old_count,reduce_count,new_count);
					}
					
				}
				m_vRecvFlowerData.push_back(_recvFlowerData);
			}
			if(_topFlowerData.send_count > 0)
			{
				_sendFlowerData.userid = _userid;
				_sendFlowerData.roleid = _roleid;
				_sendFlowerData.rolename = _topFlowerData.rolename;
				_sendFlowerData.stamp = _topFlowerData.send_stamp;
				_sendFlowerData.gender = _topFlowerData.gender;
				_sendFlowerData.count = _topFlowerData.send_count;
				_sendFlowerData.isGiftTake = _topFlowerData.send_isGiftTake;
				send_total++;
			
				if(!GetFixed())
				{
					int old_count = _sendFlowerData.count;
					int reduce_count = SendFixedCount(_roleid);
					int new_count = old_count - reduce_count;
					if(new_count < 0)
					{
						//Log::log( LOG_ERR,"ListTopFlowerDataQuery, send, roleid:%d,old_count:%d,reduce_count:%d,new_count:%d.",_roleid,old_count,reduce_count,new_count);
						//exit(0);
						isExit = true;
					}
					else
					{
						_topFlowerData.send_count = new_count;
						_sendFlowerData.count = new_count;
						//if(reduce_count>0) Log::log( LOG_INFO,"ListTopFlowerDataQuery, send, roleid:%d,old_count:%d,reduce_count:%d,new_count:%d.",_roleid,old_count,reduce_count,new_count);
					}
					
				}
				m_vSendFlowerData.push_back(_sendFlowerData);
			}
			
			if(!GetFixed())
				m_vFixedData.insert(std::make_pair(_roleid, _topFlowerData));

			Log::log( LOG_INFO,"flower print roleid:%d, send_count:%d, recv_count:%d", _roleid, _sendFlowerData.count, _recvFlowerData.count);
			//if(_topFlowerData.recv_count > 9999 || _topFlowerData.send_count > 9999)
			//Log::log( LOG_INFO,"ListTopFlowerDataQuery update,_roleid:%d,_topFlowerData.recv_count:%d, _topFlowerData.send_count:%d,send_isGiftTake:%d,userid:%d", _roleid,_topFlowerData.recv_count,_topFlowerData.send_count, _topFlowerData.send_isGiftTake, _userid);
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListTopFlowerDataQuery, error unmarshal.");
		}
		
		return true;
	}
	
	void FixedTable()
	{
		if(isFixed)
		{
			return;
		}

		for(TTopFlowerFixedData::iterator it = m_vFixedData.begin(); it != m_vFixedData.end(); ++it)
		{
			try
			{
				StorageEnv::Storage * pflower = StorageEnv::GetStorage("topflower");
				StorageEnv::AtomTransaction txn;
				try
				{
					if(pflower)
					{
						
						Marshal::OctetsStream key, value_new;
						key << it->first;
						

						value_new << it->second;

						pflower->insert(key, value_new, txn);
					
					}
				}
				catch ( DbException e ) { throw; }
				catch ( ... )
				{
					DbException ee( DB_OLD_VERSION );
					txn.abort( ee );
					throw ee;
				}
			}
			catch ( DbException e )
			{
				
			}
		}
		
		try
		{
			StorageEnv::Storage * pflower = StorageEnv::GetStorage("topflower");
			StorageEnv::AtomTransaction txn;
			try
			{
				if(pflower)
				{
					int _id = 0;
					Marshal::OctetsStream key, value_new;
					key << _id;
					
					GTableTopFlowerData new_data;
					new_data.send_count = 0;
					new_data.recv_count = 0;
					value_new << new_data;
					pflower->insert(key, value_new, txn);
				
					isFixed = true;
				}
			}
			catch ( DbException e ) { throw; }
			catch ( ... )
			{
				DbException ee( DB_OLD_VERSION );
				txn.abort( ee );
				throw ee;
			}
		}
		catch ( DbException e )
		{
			
		}

		Log::log( LOG_INFO,"FixedTable size:%d, ", m_vFixedData.size());


	}

	int GetCount(bool isRecv) { return isRecv ? recv_total : send_total; }
};

class ListBaseDataQuery : public StorageEnv::IQuery
{
public:
	int count;
	friend class ListTopFlowerDataManager;
	TTopFlowerData m_vRecvFlowerData;
	int recv_total;
	TTopFlowerData m_vSendFlowerData;
	int send_total;
	
public:
	ListBaseDataQuery() : recv_total(0), send_total(0)
	{ 
		count = 5000;
	}
	
	void Release()
	{
		m_vRecvFlowerData.clear();
		recv_total = 0;
		m_vSendFlowerData.clear();
		send_total = 0;
	}


	bool Update( StorageEnv::Transaction& txn, Octets& key, Octets& value)
	{
		try
		{
			GRoleBase base;
			Marshal::OctetsStream(value) >> base;
			
			StorageEnv::Storage * pflower = StorageEnv::GetStorage("topflower");
			StorageEnv::AtomTransaction txn;
			Marshal::OctetsStream value_new;

			int _roleid;
			//GTableTopFlowerData _topFlowerData;
			GTopFlowerData _recvFlowerData;
			GTopFlowerData _sendFlowerData;
			Marshal::OctetsStream(key) >> _roleid;
			if(count-- > 0)
			{
				_recvFlowerData.roleid = _roleid;
				_recvFlowerData.rolename = base.name;
				_recvFlowerData.stamp = 1;
				_recvFlowerData.gender = base.gender;
				_recvFlowerData.count = count;
				_recvFlowerData.isGiftTake = 0;
				m_vRecvFlowerData.push_back(_recvFlowerData);
				recv_total++;
				
				_sendFlowerData.roleid = _roleid;
				_sendFlowerData.rolename = base.name;
				_sendFlowerData.stamp = 1;
				_sendFlowerData.gender = base.gender;
				_sendFlowerData.count = count;
				_sendFlowerData.isGiftTake = 0;
				m_vSendFlowerData.push_back(_sendFlowerData);
				send_total++;
		
				GTableTopFlowerData new_data;
				new_data.rolename = base.name;
				new_data.recv_stamp = 1;
				new_data.send_stamp = 1;
				new_data.gender = base.gender;
				new_data.recv_isGiftTake = 0;
				new_data.send_isGiftTake = 0;
				new_data.recv_count = _recvFlowerData.count;
				new_data.send_count = _sendFlowerData.count;

				value_new << new_data;
				pflower->insert(key, value_new, txn);

				Log::log( LOG_INFO,"ListBaseQuery update,_roleid:%d,_topFlowerData.recv_count:%d, _topFlowerData.send_count:%d,isGiftTake:%d, name.size:%d", _roleid,_recvFlowerData.count,_sendFlowerData.count, _sendFlowerData.isGiftTake, base.name.size());
			}
			
		} catch ( Marshal::Exception & ) {
			Log::log( LOG_ERR, "ListBaseQuery, error unmarshal.");
		}
		return true;
	}
	
	int GetCount(bool isRecv) { return isRecv ? recv_total : send_total; }
};


//inline bool TopFlowerComp(GTopFlowerData& a, GTopFlowerData& b) { return a > b; }

class ListTopFlowerDataManager
{
	ListBaseDataQuery baseq;
	ListTopFlowerDataQuery topq;
	
	Thread::RWLock locker;
	bool isInit;
	bool isFlowerTest;

public:
	ListTopFlowerDataManager() : locker("listtopflowermanager::locker"), isInit(false), isFlowerTest(false)
	{
		Conf* conf=Conf::GetInstance();
		std::string db_flower_test = conf->find("GameDBServer", "db_flower_test");
		
		if(db_flower_test == "true")
			isFlowerTest = true;
		else
			isFlowerTest = false;
		LOG_TRACE("db_flower_test:%s", db_flower_test.c_str());
	}
	~ListTopFlowerDataManager(){}

	Thread::RWLock & GetLocker() { return locker; }

	bool Init()
	{
		if(!GetInit())// 需要初始化，遍历数据库
		{
			if(isFlowerTest)
			{
				Marshal::OctetsStream key, value;
				try
				{
					StorageEnv::AtomTransaction     txn;
					StorageEnv::Storage * ptable = StorageEnv::GetStorage("base");
					try{
						StorageEnv::Storage::Cursor cursor = ptable->cursor( txn );
						cursor.walk(baseq);
						SetInit();
						Log::log( LOG_INFO,"ListTopFlowerDataManager, recv_count=%d, send_count=%d", baseq.recv_total, baseq.send_total);
					}
					catch ( DbException e ) { throw; }
					catch ( ... )
					{
						DbException e( DB_OLD_VERSION );
						txn.abort( e );
						throw e;
					}
				}
				catch ( DbException e )
				{
					Log::log( LOG_ERR, "DBGetTopFlowerData, what=%s.\n", e.what() );
					//res->retcode = TOPFLOWER_LOAD_TOP_ERR;
				}
			}
			else
			{
				try
				{
					StorageEnv::AtomTransaction     txn;
					StorageEnv::Storage * ptable = StorageEnv::GetStorage("topflower");
					try{
						int _isFixed = 0;
						Marshal::OctetsStream key, value_old;
						key << _isFixed;
					
						//ptable->del(key, txn);
						if(ptable->find(key, value_old, txn))
						{
							topq.SetFixed(true);// 已经修复过
						}
						
						if(!topq.GetFixed())
						{
							if(!topq.LoadFixedFile("fakeflowernumbers"))
							{
								Log::log( LOG_ERR, "LoadFixedFile fakeflowernumbers fail." );
								topq.SetFixed(true);// 不用再走修复逻辑
								return false;
							}
						}

						if(topq.isExit)
						{
							Log::log( LOG_ERR, "isExit == true ." );
							return false;
						}
						StorageEnv::Storage::Cursor cursor = ptable->cursor( txn );
						cursor.walk(topq);
						topq.FixedTable();
						PartialSort(true);
						PartialSort(false);
						SetInit();
						//LOG_TRACE("DBGetTopFlowerData, size=%d", res->data.size());
					}
					catch ( DbException e ) { throw; }
					catch ( ... )
					{
						DbException e( DB_OLD_VERSION );
						txn.abort( e );
						throw e;
					}
				}
				catch ( DbException e )
				{
					Log::log( LOG_ERR, "DBGetTopFlowerData, what=%s.\n", e.what() );
					//res->retcode = TOPFLOWER_LOAD_TOP_ERR;
				}
			}
		}

		return true;
	}
	
	inline void SetInit() { isInit = true; }
	inline bool GetInit() { return isInit; }
	void Release()
	{
		baseq.Release();
		topq.Release();
		isInit = false;
	}

	static ListTopFlowerDataManager* Instance() { static ListTopFlowerDataManager instance; return &instance;}  

	void PartialSort(bool isRecv)// 部分排序
	{
		if(isFlowerTest)
		{
			TTopFlowerData& _flowerData = isRecv ? baseq.m_vRecvFlowerData : baseq.m_vSendFlowerData;
			int _offsize = (_flowerData.size() <= TOPFLOWER_ALL_COUNT) ? _flowerData.size() : TOPFLOWER_ALL_COUNT; 
			std::partial_sort(_flowerData.begin(), _flowerData.begin()+_offsize, _flowerData.end(), TopFlower_Cmp);

		}
		else
		{
			TTopFlowerData&	_flowerData = isRecv ? topq.m_vRecvFlowerData : topq.m_vSendFlowerData;
			int _offsize = (_flowerData.size() <= TOPFLOWER_ALL_COUNT) ? _flowerData.size() : TOPFLOWER_ALL_COUNT; 
			std::partial_sort(_flowerData.begin(), _flowerData.begin()+_offsize, _flowerData.end(), TopFlower_Cmp);
		}
		//LOG_TRACE("PartialSort, _flowerData.size:%d, _offsize:%d", _flowerData.size(), _offsize);
	}

	bool CopyFrom(bool isRecv, int startIndex, int endIndex, TTopFlowerData& data)
	{
		LOG_TRACE("recv DBGetTopFlowerData CopyFrom, startIndex=%d, endIndex=%d", startIndex, endIndex);
		TTopFlowerData _flowerData;
		if(isFlowerTest)
			_flowerData = isRecv ? baseq.m_vRecvFlowerData : baseq.m_vSendFlowerData;
		else
			_flowerData = isRecv ? topq.m_vRecvFlowerData : topq.m_vSendFlowerData;

		int _start = 0;
		int _end = 0;
		bool _finished;
		if(startIndex == 0 && endIndex == 0)
		{
			_finished = true;
		}
		else if((int)_flowerData.size() >= startIndex && (int)_flowerData.size() < endIndex)
		{
			_start = startIndex;
			_end = _flowerData.size();
			_finished = true;
		}
		else if((int)_flowerData.size() >= startIndex && (int)_flowerData.size() >= endIndex)
		{
			_start = startIndex;
			_end = endIndex;
			_finished = false;
		}
		else
		{
			_finished = true;
		}
	
		int i = 0;
		for(i=_start; i<_end; ++i)
		{
			if(_flowerData[i].count >= 999)
				LOG_TRACE("CopyFrom count:%d, i:%d", _flowerData[i].count, i);
			data.push_back(_flowerData[i]);
		}

		LOG_TRACE("recv DBGetTopFlowerData CopyFrom,_flowerData.size:%d, copy date:%d _start=%d, _end=%d,_isFinished:%d", _flowerData.size(), i, _start, _end, _finished);
		
		return _finished;
	}
};

};
#endif
