
#include "gproviderclient.hpp"
#include "state.hxx"
#include "timertask.h"
#include "conf.h"
#include "log.h"
#include "announceproviderid.hpp"
#include "isconnalive.hpp"

void OnDeliveryDisconnect();
void OnDeliveryDisconnect2();
namespace GNET
{

GProviderClient::ClientArray 	GProviderClient::g_clientarray;
GProviderClient* GProviderClient::g_clients[MAX_PROVIDERID];
std::string GProviderClient::g_name;
std::string GProviderClient::g_edition;
int GProviderClient::g_lineid;	
int GProviderClient::g_pksetting;


//检查gs与delivery的连接是否断开
class IsConnectTask: public Thread::Runnable
{
	GProviderClient * manager;
public:
	IsConnectTask(GProviderClient * man, unsigned int priority=1) : Runnable(priority), manager(man) { } 
	~IsConnectTask() { }
	void Run()
	{
		if(manager) 
		{
			manager->OnCheckConnect();
		}
	}	
};


	bool GProviderClient::Connect(int lineid,const char* name,const char* edition,int server_attr, int pksetting)
	{
		g_lineid = lineid;
		g_name = name;
		g_edition = edition;
		g_pksetting = pksetting;
		
		int count=std::atoi( Conf::GetInstance()->find( "ProviderServers" , "count" ).c_str() );
		if (count<2)
		{
			Log::log(LOG_ERR,"gamed :: missing provider, check with gmserver.conf file");
			return false;
		}
		char buf[128];
		for (;count--;)
		{
			sprintf(buf,"GProviderClient%d",count);
			GProviderClient* manager = new GProviderClient(buf);
			manager->server_attr = server_attr;
			g_clientarray.push_back(manager);
			LOG_TRACE("gamed:: start to connect %s\n",manager->Identification().c_str());
			Protocol::Client(manager);
			manager->SetAccumulate(atoi(Conf::GetInstance()->find(manager->Identification(), "accumulate").c_str()));
		}


		{ // gt provider 100, add by sunjunbo 2012.10.31
			std::vector<std::string> gt_conf_keys;
			Conf::GetInstance()->getkeys("GProviderClient100", gt_conf_keys);
			if (!gt_conf_keys.empty()) { // 配置了GT
				GProviderClient* manager = new GProviderClient("GProviderClient100");
				manager->server_attr = server_attr;
				LOG_TRACE("gamed:: start to connect %s\n",manager->Identification().c_str());
				Protocol::Client(manager);
				manager->SetAccumulate(atoi(Conf::GetInstance()->find(manager->Identification(), "accumulate").c_str()));
			}
		}
		Thread::HouseKeeper::AddTimerTask(new KeepAliveTask(30),0); 
		PollIO::WakeUp();
		return true;
	}

	bool GProviderClient::DispatchProtocol(int providerid,const Protocol *protocol)
	{
		return DispatchProtocol(providerid,*protocol);
	}

	bool GProviderClient::DispatchProtocol(int providerid,const Protocol &protocol)
	{
		if(providerid>=MAX_PROVIDERID)
			return false;
		GProviderClient* manager = g_clients[providerid]; 
		return manager && manager->SendProtocol(protocol);
	}

	void GProviderClient::BroadcastProtocol(const Protocol &protocol)
	{
		GProviderClient::ClientArray::const_iterator it=g_clientarray.begin(), ie=g_clientarray.end();
		for (;it!=ie;++it)
			(*it)->SendProtocol(protocol);
	}

	bool GProviderClient::Attach(int providerid,GProviderClient* manager)
	{
		if(providerid>=MAX_PROVIDERID)
		{
			Log::log(LOG_ERR,"gamed :: invalid provider id,  %d >= MAX_PROVIDERID", providerid);
			return false;
		}
		if(g_clients[providerid]==manager)
			return true;
		if(g_clients[providerid])
		{
			Log::log(LOG_ERR,"gamed :: duplicated provider id=%d found", providerid);
			return false;
		}
		g_clients[providerid] = manager;
		manager->m_providerid = providerid;

		//delivery
                if(providerid == 0)
		{
			LOG_TRACE("add add connect");
			Thread::HouseKeeper::AddTimerTask(new IsConnectTask(manager),0); 
		}
		return true;
	}

	void GProviderClient::Reconnect()
	{
		Thread::HouseKeeper::AddTimerTask(new ReconnectTask(this, 1), backoff);
		//backoff *= 2;
		if (backoff > BACKOFF_DEADLINE) backoff = BACKOFF_DEADLINE;
	}

	const Protocol::Manager::Session::State* GProviderClient::GetInitState() const
	{
		return &state_GProviderClient;
	}

	void GProviderClient::OnAddSession(Session::ID sid)
	{
		LOG_TRACE("gamed(%d):: OnAddSession %d\n",g_lineid,sid);
		DEBUG_PRINT( "GProviderClient::OnAddSession, sid=%d, lineid=%d, name=%s, pksetting=%d\n", sid, g_lineid, g_name.c_str(), g_pksetting );
		LOG_TRACE("add add session");

		Thread::Mutex::Scoped l(locker_state);
		if (conn_state)
		{
			Close(sid);
			return;
		}
		conn_state = true;
		this->sid = sid;
		backoff = BACKOFF_INIT;
		LineInfo info(g_lineid);
		info.name.replace(g_name.c_str(), g_name.size());
		info.attribute.resize(4);
		*(int*)(info.attribute.begin()) = g_pksetting;
		SendProtocol( AnnounceProviderID( g_lineid, server_attr, info, 0, Octets(g_edition.c_str(), g_edition.size())));

		PollIO::WakeUp();
	}

	void GProviderClient::OnDelSession(Session::ID sid)
	{
		Log::log(LOG_ERR, "GProviderClient::OnDelSession, sid=%d, lineid=%d\n", sid, g_lineid );
                if(this==g_clients[0])
                        OnDeliveryDisconnect();
		Thread::Mutex::Scoped l(locker_state);
		conn_state = false;
		Reconnect();
		void OnDeliveryDisconnect();
	}

	void GProviderClient::OnAbortSession(Session::ID sid)
	{
		Thread::Mutex::Scoped l(locker_state);
		conn_state = false;
		Reconnect();
		LOG_TRACE("gamed(%d):: OnAbortSession %d\n",g_lineid,sid);
	}

	void GProviderClient::OnCheckAddress(SockAddr &sa) const
	{
	}

	void GProviderClient::OnCheckConnect()
	{
		Thread::Mutex::Scoped l(locker_state);
		if(!conn_state) return;

		int send_counter = GetSenderCounter();
		int recv_counter = GetReceiveCounter();
		if(send_counter > recv_counter + 6)
		{
                        OnDeliveryDisconnect2();
			Close(sid);
			GLog::log(LOG_INFO, "close connect counter=%d recv_counter=%d, sid=%d", send_counter, recv_counter, sid);
			GLog::log(LOG_ERR,"gs disconnect from gdelivery, close the connection, send_counter=%d, recv_counter=%d, sid=%d", send_counter, recv_counter, sid);
		}
		else
		{
			GProviderClient::DispatchProtocol(0, IsConnAlive(send_counter)); 
			Thread::HouseKeeper::AddTimerTask(new IsConnectTask(this),30); 
			PollIO::WakeUp();

			GLog::log(LOG_INFO, "send connect counter=%d recv_counter=%d, sid=%d", send_counter, recv_counter, sid);
		}
	}


};
