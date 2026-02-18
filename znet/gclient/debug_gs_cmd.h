#ifndef __DEBUG_GS_CMD_H
#define __DEBUG_GS_CMD_H

#include <vector>

#define MAX_DEBUGCMD_LEN (256 + sizeof(C2S::cmd_header))

class DebugGSCmd
{
public:
	static DebugGSCmd *GetInstance() { return &_instance; }

	void Send(const char *pbuf);


private:
	DebugGSCmd();
	~DebugGSCmd();
	void ParseCommandLine(const char *szcmd);
	void SendDebugCmd(int icmd, int inumparams, ...);
	
	static DebugGSCmd _instance;
	std::vector<int> _tbuf;
};


#endif
