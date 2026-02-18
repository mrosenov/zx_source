#include "facbase_mall.h"
#include <timer.h>
#include "gmatrix.h"

facbase_mall::facbase_mall( int fid ) : facbase_id( fid ), mall_lock( 0 )
{}

facbase_mall::facbase_mall( int fid, const std::set<int>& item_list ) : facbase_id( fid ), mall_lock( 0 )
{
	SyncMallItems( item_list );
}

void facbase_mall::SyncMallItems( const std::set<int>& item_list )
{
	spin_autolock keeper( &mall_lock );
	active_item_list = item_list;
}

bool facbase_mall::IsMallItemActivate( int idx )
{
	spin_autolock keeper( &mall_lock );
	return ( active_item_list.find( idx ) != active_item_list.end() );
}

std::set<int> facbase_mall::GetFacbaseMallItemInfo()
{
	spin_autolock keeper( &mall_lock );
	return active_item_list;
}

/*
void facbase_mall::ActiveBidItem( const facbase_bid_item& item, bool which_list )
{
	if( which_list )
	{
		bid_item_list[ item.tid ] = item;
	}
	else
	{
		finished_bid_item_list[ item.tid ] = item;
	}
}

void facbase_mall::ActiveBidItem( facbase_bid_item* item_list, int cnt )
{
	for( int i = 0; i < cnt; ++i )
	{
		int time = item_list[i].expire_time;
		item_list[i].expire_time += g_timer.get_systime();
		ActiveBidItem( item_list[i], time > 0 );
	}
}

bool facbase_mall::IsBidItemInList( int idx, bool which_list ) const
{
	if( which_list )
	{
		return ( bid_item_list.find( idx ) != bid_item_list.end() );
	}
	else
	{
		return ( finished_bid_item_list.find( idx ) != finished_bid_item_list.end() );
	}
	return false;
}
*/
