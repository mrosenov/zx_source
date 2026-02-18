#include "itemdataman.h"
#include "../clstab.h"
#include "../worldmanager.h"
#include <interlocked.h>
#include <timer.h>

static int guid_counter = 0;
extern abase::timer	g_timer;

void generate_item_guid(item_data * pItem)
{
	unsigned int index = (gmatrix::GetServerIndex() & 0xFF) << 24;
	unsigned int counter = interlocked_increment(&guid_counter) & 0xFFFFFF;
	int t = g_timer.get_systime();
	ASSERT(t);
	pItem->guid.guid1 = t;
	pItem->guid.guid2 = index | counter; 
//	printf("%dÉù³ÆGUID%x,%x\n",id,g1,g2);
}

