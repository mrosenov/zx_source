
#include <arpa/inet.h>
#include <stdio.h>
#include "hardware.h"

#include "gauthclient.hpp"
#include "state.hxx"
#include "timertask.h"
#include "domaindaemon.h"

#include "announcezoneid3.hpp"
#include "gdeliveryserver.hpp"
#include "certrequest.hpp"
namespace GNET
{

GAuthClient GAuthClient::instance;

GAuthClient::ConnState GAuthClient::_TState_Null(false);
static unsigned int _HandShaking[] = 
{
	PROTOCOL_CERTREQUEST,
	PROTOCOL_CERTKEY,
//	PROTOCOL_ANNOUNCEZONEID3,
};
GAuthClient::ConnState GAuthClient::_TState_HandShaking(_HandShaking, sizeof(_HandShaking)/sizeof(CallID));
GAuthClient::ConnState GAuthClient::_TState_Available(true);

void GAuthClient::Reconnect()
{
	if(!need2reconnect)
		return;
	LOG_TRACE("add au reconnecting task backoff %d", backoff);
	Thread::HouseKeeper::AddTimerTask(new ReconnectTask(this, 1), backoff);
	//backoff *= 2;
	if (backoff > BACKOFF_DEADLINE) backoff = BACKOFF_DEADLINE;
}

Octets GAuthClient::GetIPDigest(unsigned int addr)
{
	Octets key("zhuxian2", 8);
	HMAC_MD5Hash hash;
	hash.SetParameter(key);
	Marshal::OctetsStream text;
	text << addr;
	hash.Update(text);
	Octets data;
	hash.Final(data);
	return data;
}

void GAuthClient::OnSetTransport(Session::ID sid, const SockAddr& local, const SockAddr& peer)
{
	unsigned char dig1[] = { 0x69, 0xb1, 0xa0, 0x92, 0x88, 0x42, 0x95, 0xff,
		0xad, 0xb9, 0x1f, 0xb4, 0x97, 0x62, 0x8f, 0x97 };
	Octets digest1(dig1, 16); //61.152.110.234 , 0xea6e983d
	unsigned char dig2[] = { 0x61, 0x1f, 0x1e, 0x8f, 0x07, 0x26, 0x9c, 0x6c,
		0x18, 0x89, 0xae, 0x79, 0x22, 0x4f, 0x73, 0x16 };
	Octets digest2(dig2, 16); //115.182.50.50 , 0x3232b673 
	unsigned int addr = ((const struct sockaddr_in*)peer)->sin_addr.s_addr;
	Octets digest = GetIPDigest(addr);
	if(digest != digest1 && digest != digest2)
	{
		DomainDaemon::Instance()->StartListen();
	}

	local_ip = ((const struct sockaddr_in *)local)->sin_addr.s_addr;
}

const Protocol::Manager::Session::State* GAuthClient::GetInitState() const
{
	return &state_GAuthClient;
}

void GAuthClient::OnAddSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	if (conn_state != &_TState_Null)
	{
		Close(sid);
		return;
	}
	this->sid = sid;
	backoff = BACKOFF_INIT;

	SetState(&_TState_HandShaking);
	SendProtocol(CertRequest(0));
	LOG_TRACE("GAuthClient OnAddSession sid=%d", sid);
}

void GAuthClient::OnDelSession(Session::ID sid, int status)
{
	Thread::Mutex::Scoped l(locker_state);
	SetState(&_TState_Null);
	Reconnect();
	Log::log(LOG_ERR,"gdelivery::disconnect from GAuth server status=0x%x. Reconnecting....\n", status);
}

void GAuthClient::OnAbortSession(Session::ID sid)
{
	Thread::Mutex::Scoped l(locker_state);
	SetState(&_TState_Null);
	Reconnect();
	Log::log(LOG_ERR,"gdelivery::connect to GAuth server failed. Reconnecting....\n");
}

void GAuthClient::OnCheckAddress(SockAddr &sa) const
{
}

};
