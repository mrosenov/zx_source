#ifndef __GNET_IWEBCMD_H
#define __GNET_IWEBCMD_H

#include "serviceforbidcmd"
#include "base64.h"
#include "dbgetserviceforbidcmd.hrp"
#include "forbidservice.hpp"
namespace GNET
{
	class IwebCmd
	{
		std::vector<ServiceForbidCmd> cmdlist;
		bool data_ready;
		int service_forbid_version;
		IwebCmd() : data_ready(false), service_forbid_version(0) {}
	public:
		static IwebCmd * GetInstance() { static IwebCmd instance; return &instance; }
		bool Initialize()
		{
			const char * conf = "serviceforbidver";
			if (access(conf, R_OK) == -1)
				return false;
			std::ifstream ifs(conf);
			std::string line;
			int ver = 0;
			while (std::getline(ifs, line))
			{
				if (line.empty()) continue;
				const char c = line[0];
				if (c == '#' || c == ';'|| c == '\r' || c=='\n') continue;
				if (sscanf(line.c_str(), "ver=%d", &ver) == 1)
					break;
			}
			LOG_TRACE("IwebCmd read serviceforbidver from conf %d", ver);
			SetServiceForbidVer(ver);
			return true;
		}
		void SetServiceForbidVer(int ver)
		{
			service_forbid_version = ver;
		}
		int GetServiceForbidVer()
		{
			return service_forbid_version;
		}
		void OnDBConnect(Protocol::Manager *manager, int sid)
		{
			if (data_ready)
				return;
			manager->Send(sid, Rpc::Call(RPC_DBGETSERVICEFORBIDCMD, DBGetServiceForbidCmdArg(GetServiceForbidVer())));
		}
		void NotifyCmdToGS(const std::vector<ServiceForbidCmd> & list)
		{
			if (list.empty())
				return;
			LOG_TRACE("broadcast serviceforbidcmdlist to gs, cmdlist.size %d", list.size());
			ForbidService notify(list);
			GProviderServer::GetInstance()->BroadcastProtocol(notify);
		}
		void NotifyCmdToGS(unsigned int sid, const std::vector<ServiceForbidCmd> & list)
		{
			if (list.empty())
				return;
			LOG_TRACE("notify serviceforbidcmdlist to gssid %d, cmdlist.size %d", sid, list.size());
			ForbidService notify(list);
			GProviderServer::GetInstance()->Send(sid, notify);
		}
		void OnGSConnect(unsigned int sid)
		{
			if (data_ready)
				NotifyCmdToGS(sid, cmdlist);
		}
		static std::string MakeCmdStr(const std::vector<ServiceForbidCmd> & list)
		{
			std::string str;
			std::vector<ServiceForbidCmd>::const_iterator it, ite = list.end();
			for (it = list.begin(); it != ite; ++it)
			{
				char buf[1024];
				memset(buf, 0, sizeof(buf));
				snprintf(buf, sizeof(buf)-1, " cmdtype %d args:", it->cmdtype);
				str += buf;
				std::vector<int>::const_iterator it2, ite2 = it->arglist.end();
				for (it2 = it->arglist.begin(); it2 != ite2; ++it2)
				{
					memset(buf, 0, sizeof(buf));
					snprintf(buf, sizeof(buf)-1, " %d", *it2);
					str += buf;
				}
				str += ";";
			}
			return str;
		}
		void LoadCmd(const std::vector<ServiceForbidCmd> & list)
		{
			if (data_ready)
				return;
			cmdlist = list;
			LOG_TRACE("load serviceforbidcmdlist size %d cmd:%s", list.size(), GetCmdStr().c_str());
			data_ready = true;

			NotifyCmdToGS(cmdlist);
			LocalProcess(list);
		}
		static bool MakeCmd(const std::vector<int> & cmdandargs, ServiceForbidCmd &cmd)
		{
			if (cmdandargs.empty())
				return false;
			cmd.cmdtype = cmdandargs.front();
			cmd.timestamp = Timer::GetTime();
			if (cmd.cmdtype <= 0)
				return false;
			std::vector<int> tmp(cmdandargs.begin()+1, cmdandargs.end());
			cmd.arglist.swap(tmp);
			return true;
		}
		bool IsReady() { return data_ready; }
		int GetCmdList(const Octets & code, int & timestamp, std::vector<ServiceForbidCmd> & list)
		{
			Octets plain_text, cmd_text, cmd_digest;
			Base64Decoder::Convert(plain_text, code);
			try
			{
				Marshal::OctetsStream(plain_text) >> timestamp >> cmd_text >> cmd_digest;
			}
			catch(...)
			{
				Log::log(LOG_ERR, "operationcmd, plain_text unmarshal error, plain_text.size %d",
						plain_text.size());
				return ERR_IWEBCMD_UNMARSHAL;
			}
			if (cmd_text.size()==0 || cmd_digest.size()==0)
			{
				Log::log(LOG_ERR, "operationcmd, size error, cmd_text.size %d cmd_digest.size %d",
						cmd_text.size(), cmd_digest.size());
				return ERR_IWEBCMD_CMDSIZE;
			}
			Octets digest2;
			MD5Hash hash;
			hash.Update(cmd_text);
			hash.Final(digest2);
			if (cmd_digest != digest2)
			{
				Log::log(LOG_ERR, "operationcmd, code is not integrated");
				return ERR_IWEBCMD_INTEGRITY;
			}
			char * cmd_str = new char[cmd_text.size()+1];
			memcpy(cmd_str, (char*)cmd_text.begin(), cmd_text.size());
			cmd_str[cmd_text.size()] = 0;
			LOG_TRACE("operationcmd, get cmd timestmp %d str:%s", timestamp, cmd_str);
			char * delim = "#";
			char * outer_ptr = NULL;
			char * token = strtok_r(cmd_str, delim, &outer_ptr);
			bool parse_success = true;
			while (NULL != token)
			{
				char * inner_delim = ":";
				char * inner_ptr = NULL;
				std::vector<int> cmdandargs;
				char * cmdorarg = strtok_r(token, inner_delim, &inner_ptr);
				while (NULL != cmdorarg)
				{
					cmdandargs.push_back(atoi(cmdorarg));
					cmdorarg = strtok_r(NULL, inner_delim, &inner_ptr);
				}
				token = strtok_r(NULL, delim, &outer_ptr);

				ServiceForbidCmd cmd;
				if (MakeCmd(cmdandargs, cmd))
					list.push_back(cmd);
				else
				{
					parse_success = false;
					Log::log(LOG_ERR, "operationcmd, makecmd fail, str:%s", token);
				}
			}
			delete [] cmd_str;
			return (parse_success && !list.empty()) ? ERR_IWEBCMD_SUCCESS : ERR_IWEBCMD_PARSE;
		}
		void HandleCmd(const std::vector<ServiceForbidCmd> & list)
		{
			NotifyCmdToGS(list);
			cmdlist.insert(cmdlist.end(), list.begin(), list.end());
			LocalProcess(list);
		}
		void DebugClearCmdList()
		{
			cmdlist.clear();
		}
		std::string GetCmdStr()
		{
			return MakeCmdStr(cmdlist);
		}
		void LocalProcess(const std::vector<ServiceForbidCmd> & l)
		{
			std::vector<ServiceForbidCmd>::const_iterator it, ite = l.end();
			for (it = l.begin(); it != ite; ++it)
			{
				if (it->cmdtype == SERVICE_FORBID_DELIVERY_PRO)
				{
					LOG_TRACE("operationcmd delivery forbid pro.size %d", it->arglist.size());
					std::set<Protocol::Type> s(it->arglist.begin(), it->arglist.end());
					GDeliveryServer::GetInstance()->InsertIgnore(s);
				}
			}
		}
	};//end of class IwebCmd
}; //end of GNET
#endif
