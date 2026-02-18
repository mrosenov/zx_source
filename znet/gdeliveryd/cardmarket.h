/* @file: cardmarket.h
   @description: manage all card Sell's informations,this information must consistent with gamedbd
 */

#ifndef __GNET_POINTSELLER_H
#define __GNET_POINTSELLER_H
#include <map>
#include <set>
#include "sellpointinfo"
#include "itimer.h"
#include "mutex.h"
#include "stdlib.h" //for rand
namespace GNET
{
	extern char sellsys[];

	class SellInfo
	{
	public:
		union attr_t {
			int value;
			struct {
				int handling:1; /* sell is handled by gamedbd, no result return yet */
				int expired:1;  /* sell is expired */
			};
		};
		SellInfo(const SellPointInfo& si) : m_sellinfo_(si) { m_attr_.value=0; }
		operator SellPointInfo& () { return m_sellinfo_; }
		SellPointInfo* operator ->() { return &m_sellinfo_; }
		attr_t&   GetAttr() { return m_attr_; }
	private:
		SellPointInfo m_sellinfo_;		
		attr_t        m_attr_;
	};
	struct Seller
	{
		int roleid;
		int forsale_point;
		int forsale_price;
		std::set<int> attendlist;
		Seller( int r=0,int p=0,int m=0) : roleid(r),forsale_point(p),forsale_price(m) { }
	};
	class CardMarket : public IntervalTimer::Observer
	{
	public:	
		typedef int  roleid_t;
		typedef int  userid_t;
		typedef int  sellid_t;
		typedef std::map<userid_t,int>           UserPointMap; //map userid to total point this user sell
		typedef std::map<roleid_t,Seller>        SellerMap;
		typedef std::map<sellid_t,SellInfo>      SellInfoMap;
		typedef std::multimap<time_t,sellid_t>   TimerMap;
	private:
		typedef std::vector<SellPointInfo>       SellList;
		CardMarket() : m_blInit_(false),m_lock_("CardMarket::m_lock_") { 
			//m_maxSellID_=0;
			srand(time(NULL)); 
			IntervalTimer::Attach( this,1*1000000/IntervalTimer::Resolution() ); 
		}	
	public:
		static CardMarket& GetInstance() {
			static CardMarket instance;
			return instance;
		}
		// Is init process finished
		bool IsMarketOpen() { return m_blInit_; }
		// Get all sell info from GameDBD
		bool InitMarket( const SellList& slist,bool blFinish=false );
		// Add new sell, if sell exist, synchronize it
		int AddSell( const SellPointInfo& si );
		// sync sell info of a role with gamedbd
		bool SyncSell( roleid_t roleid,const SellList& slist ); 
		// remove a sell
		int RmvSell( sellid_t sellid );
		// Mark Selling status
		bool MarkSelling( sellid_t sellid );
		//whether expired
		bool ExpirePolicy( sellid_t sellid );
		//whether permit sell, success return 0,else return errcode
		int CreatePolicy( roleid_t roleid,int point,int price );
		//whether sellid belongs to roleid
		bool VerifyCardOwner( roleid_t roleid,sellid_t sellid );

		// find a serial of sell by sellid
		bool FindSell( int startid,SellList& list,bool forward=true );
		// get sell info of a role
		bool GetSellList( roleid_t roleid,SellList& list );
		// generate a new sellid, -1 means invalid id
		//int GenerateSellID() { return m_blInit_ ? ++m_maxSellID_: -1; };

		// timer observer's update, ONLY status==NOTSELL can expire
		bool Update();
		// reschedule sell, with random backdraw
		void RescheduleSell( sellid_t sellid,bool blBackDraw=true );
	private:
		//below is tool functions
		void _update_maps( const SellPointInfo& si );
		//announce gamedbd, expired
		void _announce_expired( roleid_t roleid,sellid_t sellid );
		// get random backdraw time
		int  _random_backdraw();
	private:
		bool m_blInit_;	
		//int  m_maxSellID_;
		Thread::Mutex m_lock_;
		UserPointMap  m_mapUserPoint_;
		SellerMap     m_mapSeller_;
		SellInfoMap   m_mapSellInfo_;
		TimerMap      m_mapTimer_;
	};
}
#endif
