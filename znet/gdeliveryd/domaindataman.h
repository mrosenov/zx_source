#ifndef _DOMAINDATAMAN_H_
#define _DOMAINDATAMAN_H_

#include <vector>

// data and methods for domain
enum DOMAIN_TYPE
{
	DOMAIN_TYPE_NULL = 0,
	DOMAIN_TYPE_3RD_CLASS,//¸ß¼¶
	DOMAIN_TYPE_2ND_CLASS,
	DOMAIN_TYPE_1ST_CLASS,//µÍ¼¶
};

typedef struct _DOMAIN_INFO_SERV
{
	int						id;				// id of the domain
	DOMAIN_TYPE				type;			// type of the domain
	int					item_lowerlimit;	
	int					item_upperlimit;	
	std::vector<int>		neighbours;		// neighbours of this domain

} DOMAIN_INFO_SERV;

typedef struct _BATTLETIME_SERV
{
	int						nDay;
	int						nHour;
	int						nMinute;
	_BATTLETIME_SERV(int day = 0, int hour = 0, int min = 0):nDay(day),nHour(hour),nMinute(min){}
} BATTLETIME_SERV;

///////////////////////////////////////////////////////////////////////////////
// load data from a config file
// return	0 if succeed
//			-1 if failed.
///////////////////////////////////////////////////////////////////////////////
int domain_data_load();

// get the data of a domain by id
// return NULL if not found
DOMAIN_INFO_SERV * domain_data_getbyid(int id);

// get the data of a domain by index
// return NULL if not found
DOMAIN_INFO_SERV * domain_data_getbyindex(int index);

// get the number of domain data
int domain_data_getcount();

// get battle time list
const std::vector<BATTLETIME_SERV>& getbattletimelist();
// get number of battle at the same time
int getbattletimemax();

#endif//_DOMAINDATAMAN_H_

