#ifndef __ONLINE_FACBASE_MALL_H__
#define __ONLINE_FACBASE_MALL_H__
#include <set>

/*
#include <map>
struct facbase_bid_item
{
	facbase_bid_item() : tid( 0 ), expire_time( 0 ), player_id( 0 ){}
	facbase_bid_item(int id, int time, int player) : tid( id ), expire_time( time ), player_id( player ){}
	int tid;
	int expire_time;
	int player_id;
};
*/

class facbase_mall
{
private:
	int facbase_id;
	std::set<int> active_item_list;
	int mall_lock;
	/*
	std::map<int, facbase_bid_item> bid_item_list;
	std::map<int, facbase_bid_item> finished_bid_item_list;
	*/
	
public:
	facbase_mall( int fid );
	facbase_mall( int fid, const std::set<int>& active_items );
	//void ActiveMallItem( int* item_list, int cnt );
	void SyncMallItems( const std::set<int>& item_list );
	bool IsMallItemActivate( int idx );
	std::set<int> GetFacbaseMallItemInfo();
	
	/*
	void ActiveBidItem( const facbase_bid_item& item, bool which_list );
	void ActiveBidItem( facbase_bid_item* item_list, int cnt );
	bool IsBidItemInList( int idx, bool which_list ) const;
	*/
};

#endif
