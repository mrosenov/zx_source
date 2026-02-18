#include "domaindataman.h"

static std::vector<DOMAIN_INFO_SERV>		domain_infos_server;
static std::vector<BATTLETIME_SERV>			battletime_server;
static int									battletime_max;

///////////////////////////////////////////////////////////////////////////////
// load data from a config file
// return	0 if succeed
//			-1 if failed.
///////////////////////////////////////////////////////////////////////////////
int domain_data_load()
{
	int	count;
	FILE * fpServer;

	fpServer = fopen("domain.sev", "rb");
	if( !fpServer )	return -1;
	fread(&count, 1, sizeof(int), fpServer);
	for(int i=0; i<count; i++)
	{
		DOMAIN_INFO_SERV domain_info;
		fread(&domain_info.id, 1, sizeof(int), fpServer);
		fread(&domain_info.type, 1, sizeof(int), fpServer);
		fread(&domain_info.item_lowerlimit, 1, sizeof(int), fpServer);
		fread(&domain_info.item_upperlimit, 1, sizeof(int), fpServer);
		int nNumNeighbours;
		fread(&nNumNeighbours, 1, sizeof(int), fpServer);
		for(int j=0; j<nNumNeighbours; j++)
		{
			int idNeighbour;
			fread(&idNeighbour, 1, sizeof(int), fpServer);
			domain_info.neighbours.push_back(idNeighbour);
		}
		domain_infos_server.push_back(domain_info);
	}
	// load time list
	fread(&count, 1, sizeof(int), fpServer);
	for(int i=0; i<count; i++)
	{
		BATTLETIME_SERV theTime;
		fread(&theTime, sizeof(BATTLETIME_SERV), 1, fpServer);
		battletime_server.push_back(theTime);
	}
	fread(&battletime_max, sizeof(int), 1, fpServer);
	//城战时间段写死
	battletime_server.clear();
	battletime_server.push_back(BATTLETIME_SERV(5, 20, 0));
	battletime_server.push_back(BATTLETIME_SERV(6, 20, 0));
	battletime_server.push_back(BATTLETIME_SERV(4, 20, 0));
	battletime_server.push_back(BATTLETIME_SERV(5, 14, 0));
	battletime_server.push_back(BATTLETIME_SERV(6, 14, 0));
	battletime_server.push_back(BATTLETIME_SERV(5, 9, 0));
	battletime_server.push_back(BATTLETIME_SERV(6, 9, 0));
	battletime_max = 7;//默认每个时段安排7场
	fclose(fpServer);
	return 0;
}

// get the data of a domain by id
DOMAIN_INFO_SERV * domain_data_getbyid(int id)
{
	int n = domain_infos_server.size();
	for(int i=0; i<n; i++)
	{
		if( domain_infos_server[i].id == id )
			return &domain_infos_server[i];
	}

	return NULL;
}

// get the data of a domain by index
DOMAIN_INFO_SERV * domain_data_getbyindex(int index)
{
	int n = domain_infos_server.size();
	if( index >= n )
		return NULL;

	return &domain_infos_server[index];
}

// get the number of domain data
int domain_data_getcount()
{
	return domain_infos_server.size();
}

const std::vector<BATTLETIME_SERV>& getbattletimelist()
{
	return battletime_server;
}

int getbattletimemax()
{
	return battletime_max;
}
