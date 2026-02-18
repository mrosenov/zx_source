
#ifndef __GNET_MATRIXRESPONSE_HPP
#define __GNET_MATRIXRESPONSE_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "matrixfailure.hpp"

namespace GNET
{

class MatrixResponse : public GNET::Protocol
{
	#include "matrixresponse"

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		bool result = true;
		unsigned int checkertype = ALGORITHM_NONE;

		GLinkServer *lsm = GLinkServer::GetInstance();
		Thread::RWLock::RDScoped l(lsm->locker_map);
		SessionInfo * sinfo = lsm->GetSessionInfo(sid);

		Log::trace("user(%d) receive matrixresponse response %d algo %d", sinfo->userid,
					response, sinfo->algorithm);
		if (sinfo && sinfo->checker)
		{
			result = sinfo->checker->Verify(response);
			checkertype = sinfo->checker->getClass();
			if(!result)
			{
				MatrixFailure mf(sinfo->checker->GetUid(), sinfo->checker->GetIp(), 1);
				GDeliveryClient::GetInstance()->SendProtocol(mf);
			}
			else
			{
				if ((sinfo->algorithm & ALGORITHM_MASK_HIGH) == ALGORITHM_PHONETOKEN)
				{
					char num[6];
					int i = 0, j = 100000;
					while (i < 6 && j > 0)
					{
						num[i] = response / j + '0';
						response = response%j;
						i++;
						j /= 10;
					}
					sinfo->elec_num.replace(num, sizeof(num));
				}
			}
			delete sinfo->checker;
			sinfo->checker = NULL;
		}
		if(!result)
		{
			lsm->SessionError(sid, ERR_MATRIXFAILURE, "Matrix check failed.");
			return;
		}

		lsm->ChangeState(sid, &state_GKeyExchgSend);
		KeyExchange keyexchange(PROTOCOL_KEYEXCHANGE);
		keyexchange.Setup(sinfo->identity, sinfo->response, lsm, sid);
		lsm->Send(sid, keyexchange);

		Octets& username = sinfo->identity;
		Log::login(username, sinfo->userid, sid, inet_ntoa(((const struct sockaddr_in*)sinfo->GetPeer())->sin_addr), sinfo->mid);
	}
};

};

#endif
