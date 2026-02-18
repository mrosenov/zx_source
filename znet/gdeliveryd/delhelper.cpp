
#include "delhelper.h"
#include "dbdeleterole.hrp"
#include "dbdeletefaction.hrp"
#include "dbdeletefamily.hrp"
#include "localmacro.h"
#include "gamedbclient.hpp"
namespace GNET
{

	void DeleteHelper::DeleteNext() 
	{
		if (cursor >= todel.size())
		{
			todel.clear();
			cursor = 0;
			return;
		}
		WaitDelKey & next = todel[cursor++];
		switch (next.type)
		{
			case TYPE_ROLE:
				{
					DBDeleteRoleArg arg(next.id, false);
					DBDeleteRole* rpc=(DBDeleteRole*) Rpc::Call(RPC_DBDELETEROLE, arg);
					rpc->isbatch = true;
					GameDBClient::GetInstance()->SendProtocol(rpc);
				}
				break;
			case TYPE_FACTION:
				{
					DBDeleteFactionArg arg(next.id, false);
					DBDeleteFaction* rpc=(DBDeleteFaction*) Rpc::Call(RPC_DBDELETEFACTION, arg);
					rpc->isbatch = true;
					GameDBClient::GetInstance()->SendProtocol(rpc);
				}
				break;
			case TYPE_FAMILY:
				{
					DBDeleteFamilyArg arg(next.id, false);
					DBDeleteFamily* rpc=(DBDeleteFamily*) Rpc::Call(RPC_DBDELETEFAMILY, arg);
					rpc->isbatch = true;
					GameDBClient::GetInstance()->SendProtocol(rpc);
				}
				break;
		}
		return;
	}

	void DeleteHelper::AddDeleteList(WaitDelKeyVector& _todel)
	{
		if (_todel.size() == 0)
			return;

		if (todel.size() == 0)
		{
			todel.insert(todel.end(), _todel.begin(), _todel.end());
			cursor = 0;
			DeleteNext();
		}
		else
			todel.insert(todel.end(), _todel.begin(), _todel.end());
	}
};


