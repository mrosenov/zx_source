#include "debug_gs_cmd.h"
#include "gpdatatype.h"
#include "gamedatasend.hpp"
#include "glinkclient.h"
#include <memory.h>
#include <stdarg.h>

#define MAXNUM_TOKEN 18


DebugGSCmd DebugGSCmd::_instance;

DebugGSCmd::DebugGSCmd()
{

}

DebugGSCmd::~DebugGSCmd()
{

}

void
DebugGSCmd::ParseCommandLine(const char *szcmd)
{
	int inumcmdtoken = 0;
	const char * pch = szcmd;
	_tbuf.clear();              

	//Skip space
	while(*pch && *pch <= 32)
		pch++;

	while(*pch && inumcmdtoken < MAXNUM_TOKEN)
	{
		char *pend = (char *)pch;

		//Get token 
		while(*pend > 32)
			pend++;

		*(pend) = '\0';
		_tbuf.push_back(atoi(pch));

		pch = ++pend;

		//Skip space
		while (*pch && *pch <= 32)
			pch++;
	}
}

void 
DebugGSCmd::Send(const char *pbuf)
{
	if(strlen(pbuf) >= MAX_DEBUGCMD_LEN)
	{
		fprintf(stderr, "send debug_gs_cmd error: cmd_len > MAX_LEN\n");
	}

	char tmpbuf[MAX_DEBUGCMD_LEN];
	memset(&tmpbuf, 0, MAX_DEBUGCMD_LEN);
	memcpy(&tmpbuf, pbuf, strlen(pbuf));

	//parse cmd line
	ParseCommandLine(tmpbuf);

	int inumparams = _tbuf.size() - 1;
	switch(inumparams)
	{
		case 0:
			SendDebugCmd(_tbuf[0], inumparams);
			break;

		case 1:
			SendDebugCmd(_tbuf[0], inumparams, _tbuf[1]);
			break;

		case 2:
			SendDebugCmd(_tbuf[0], inumparams, _tbuf[1], _tbuf[2]);
			break;
		
		case 3:
			SendDebugCmd(_tbuf[0], inumparams, _tbuf[1], _tbuf[2], _tbuf[3]);
			break;

		case 4:
			SendDebugCmd(_tbuf[0], inumparams, _tbuf[1], _tbuf[2], _tbuf[3], _tbuf[4]);
			break;

		case 5:
			SendDebugCmd(_tbuf[0], inumparams, _tbuf[1], _tbuf[2], _tbuf[3], _tbuf[4], _tbuf[5]);
			break;

		case 6:
			SendDebugCmd(_tbuf[0], inumparams, _tbuf[1], _tbuf[2], _tbuf[3], _tbuf[4], _tbuf[5], _tbuf[6]);
			break;

		case 7:
			SendDebugCmd(_tbuf[0], inumparams, _tbuf[1], _tbuf[2], _tbuf[3], _tbuf[4], _tbuf[5], _tbuf[6], _tbuf[7]);
			break;

		case 8:
			SendDebugCmd(_tbuf[0], inumparams, _tbuf[1], _tbuf[2], _tbuf[3], _tbuf[4], _tbuf[5], _tbuf[6], _tbuf[7], _tbuf[8]);
			break;

		default:
			fprintf(stderr, "error cmd param size = %d\n", _tbuf.size());
			break;
	}
}


void
DebugGSCmd::SendDebugCmd(int icmd, int inumparams, ...)
{
	using namespace C2S;

	int isize   = sizeof(C2S::cmd_header) + inumparams * sizeof(int);
	char abuf[MAX_DEBUGCMD_LEN];
	memset(&abuf, 0, MAX_DEBUGCMD_LEN);
	char * ptmpbuf = (char *)&abuf;

	((C2S::cmd_header *)ptmpbuf)->cmd = icmd;

	if(inumparams)
	{
		va_list valist;
		va_start(valist, inumparams);

		int *pdata = (int *)(ptmpbuf + sizeof(C2S::cmd_header));
		for(int i = 0; i < inumparams; i++)
		{
			*pdata++ = va_arg(valist, int);
		}

		va_end(valist);
	}

	GLinkClient::GetInstance()->SendProtocol(GamedataSend(Octets(ptmpbuf, isize)));
}
