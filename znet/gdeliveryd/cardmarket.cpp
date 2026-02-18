#include "cardmarket.h"
#include "macros.h"
#include "errcode.h"
#include "gamedbclient.hpp"
#include "dbselltimeout.hrp"
#include "mapremaintime.h"
#include "mapforbid.h"
#include <vector>
#include <algorithm> //for set_difference
#include <iterator>  //for back_insert_iterator
#include <utility> //for make_pair

#define MIN_REMAIN_TIME 135000 // 30 Yuan
#define MIN_POINT_UNIT  135000 // 30 Yuan
#define NONOPER_TIME    300   // if (expire_time - now)<NONOPER_TIME, no operation(cancel) can be execute

#define min(a,b) ( (a)<(b) ? (a) : (b) )
#define max(a,b) ( (a)>(b) ? (a) : (b) )
namespace GNET
{

char sellsys[32] = "\xb9\x70\x61\x53\xa4\x4e\x13\x66\xfb\x7c\xdf\x7e"; //点卡销售系统

// init process finished
bool CardMarket::InitMarket( const SellList& slist,bool blFinish )
{
	if ( m_blInit_ ) return m_blInit_;
	Thread::Mutex::Scoped l(m_lock_);
	for ( size_t i=0;i<slist.size();++i )
		_update_maps( slist[i] );
	return m_blInit_=blFinish;
}
// Add new sell, if sell exist, synchronize it
int CardMarket::AddSell( const SellPointInfo& si )
{
	int retcode=ERR_SUCCESS;
	if ( !m_blInit_ ) return ERR_SP_NOT_INIT;
	Thread::Mutex::Scoped l(m_lock_);
	_update_maps( si );
	return retcode;
}
// sync sell info of a role with gamedbd
bool CardMarket::SyncSell( roleid_t roleid,const SellList& slist )
{
	if ( !m_blInit_ ) return false;
	std::set<sellid_t>    dbd_set;
	std::vector<sellid_t> diff_set;
	{
		Thread::Mutex::Scoped l(m_lock_);
		for ( size_t i=0;i<slist.size();++i )
		{
			dbd_set.insert( slist[i].sellid );
			_update_maps( slist[i] );
		}
		// find sellid, which exist in cache, but not exist in gamedbd
		Seller &seller=m_mapSeller_[ roleid ];
		std::set_difference( 
				seller.attendlist.begin(),seller.attendlist.end(),
				dbd_set.begin(),dbd_set.end(),
				std::back_insert_iterator< std::vector<sellid_t> >(diff_set)
			);
	}
	//remove a sell, if it does not exist in DBD
	for ( size_t i=0;i<diff_set.size();++i )
		RmvSell( diff_set[i] );
	return true;
}
// remove a sell, only in case of Cancel or Expire 
int CardMarket::RmvSell( sellid_t sellid )
{
	return ERR_SUCCESS;
}
// Mark Selling status
bool CardMarket::MarkSelling( sellid_t sellid )
{
	if ( !m_blInit_ ) return false;
	Thread::Mutex::Scoped l(m_lock_);
	SellInfoMap::iterator it=m_mapSellInfo_.find( sellid );
	if ( it!=m_mapSellInfo_.end() )
	{
		if ( (*it).second->status==_PST_SOLD ) return false;
		(*it).second->status=_PST_SELLING;
		return true;
	}
	else
		return false;
}

// find a serial of sell by sellid
bool CardMarket::FindSell( int startid,SellList& list,bool forward )
{
	if ( !m_blInit_ ) return false;
	
	struct timeval now; 
    IntervalTimer::GetTime( &now );
	Thread::Mutex::Scoped l(m_lock_);
	int count=0;
	int sell_count=m_mapSellInfo_.size();
	int start_p= forward ? min(startid,sell_count ? sell_count-1:0) : 
			               max(startid-_HANDLE_PAGESIZE,0);
	SellInfoMap::iterator it=m_mapSellInfo_.begin();
	std::advance(it,start_p);
	for ( ;it!=m_mapSellInfo_.end() && count<=_HANDLE_PAGESIZE;++it,++count )
	{
		if ( (*it).second->etime-now.tv_sec>NONOPER_TIME )
			list.push_back( it->second );
	}
	return true;
}
// get sell info of a role
bool CardMarket::GetSellList( roleid_t roleid,SellList& list )
{
	if ( !m_blInit_ ) return false;
    struct timeval now; 
    IntervalTimer::GetTime( &now );
	Thread::Mutex::Scoped l(m_lock_);
	Seller &seller=m_mapSeller_[ roleid ];
	SellInfoMap::iterator it;
	std::set<int>::iterator ita=seller.attendlist.begin(),itae=seller.attendlist.end();
	for ( ;ita!=itae;++ita )
	{
		it=m_mapSellInfo_.find( (*ita) );
		if ( it!=m_mapSellInfo_.end() )
		{
			if ( (*it).second->etime-now.tv_sec>NONOPER_TIME )
				list.push_back( it->second );
		}
	}
	return true;
}

// timer observer's update      
bool CardMarket::Update()
{
	if ( !m_blInit_ ) return true; //means put it to timer list again.
	Thread::Mutex::Scoped l(m_lock_);
	//check expired sell, and inform gamedbd to remove it
	//Maybe some sellid does not exist, because these sells is removed from m_mapSellInfo_by RmvSell() 
	//but not be removed from m_mapTimer_ if a user Canceled a sell manually
	//1. check whether sellid exists in m_mapSellInfo_. if not, just ignore
	//2. if DO exists, Mark this sell as expired(prevent some operations on it), and inform GameDBD( if informing gamedbd failed,just resend )
	//3. when GameDBD returned, Remove it
	SellInfoMap::iterator itsi;
	std::vector<TimerMap::iterator> expire_set;
	TimerMap::iterator it=m_mapTimer_.begin(),ite=m_mapTimer_.end();
	struct timeval now;
	IntervalTimer::GetTime( &now );
	for ( ;it!=ite;++it )
		if ( it->first<=now.tv_sec ) expire_set.push_back( it );
	for ( size_t i=0;i<expire_set.size();++i )
	{
		sellid_t sellid=expire_set[i]->second;
		itsi=m_mapSellInfo_.find( sellid );
		if ( itsi!=m_mapSellInfo_.end() && (*itsi).second->status==_PST_NOTSELL )
		{
			(*itsi).second.GetAttr().expired=1;
			_announce_expired( (*itsi).second->roleid,(*itsi).second->sellid );
		}
		m_mapTimer_.erase( expire_set[i] );
	}
	return true;
}
// reschedule sell, with random backdraw
void CardMarket::RescheduleSell( sellid_t sellid,bool blBackDraw )
{
	if ( !m_blInit_ ) return;
	Thread::Mutex::Scoped l(m_lock_);
	SellInfoMap::iterator it=m_mapSellInfo_.find( sellid );
	if ( it==m_mapSellInfo_.end() ) return;
	if ( blBackDraw )
	{
		(*it).second->etime+=_random_backdraw();
		(*it).second.GetAttr().expired=0;
	}
	m_mapTimer_.insert( std::make_pair((*it).second->etime,sellid) );
}
//whether expired                                                   
bool CardMarket::ExpirePolicy( sellid_t sellid )
{
	if ( !m_blInit_ ) return false; 
	Thread::Mutex::Scoped l(m_lock_);
	SellInfoMap::iterator it=m_mapSellInfo_.find( sellid );
	if ( it==m_mapSellInfo_.end() ) return false;
	struct timeval now;
	IntervalTimer::GetTime( &now );
	return (*it).second->etime-now.tv_sec<NONOPER_TIME || (*it).second.GetAttr().expired;
}	
//whether permit sell, check remain_time,sell point num                                                   
int  CardMarket::CreatePolicy( roleid_t roleid,int point,int price ) 	
{
	return ERR_SUCCESS;
}
//whether sellid belongs to roleid
bool CardMarket::VerifyCardOwner( roleid_t roleid,sellid_t sellid )
{
	return true;
}

///////////////////////////////////////
///  Below is private functions     ///
///////////////////////////////////////
// update maps with NEW sell info
void CardMarket::_update_maps( const SellPointInfo& si )
{
}
//announce gamedbd, expired
void CardMarket::_announce_expired( roleid_t roleid,sellid_t sellid )
{
	GameDBClient::GetInstance()->SendProtocol(
			Rpc::Call( RPC_DBSELLTIMEOUT,SellID(roleid,sellid) )
		);
}
// get random backdraw time
int  CardMarket::_random_backdraw()
{
	double ratio=(double)rand()/(double)RAND_MAX;
	return (int) (ratio*NONOPER_TIME);
}

}
