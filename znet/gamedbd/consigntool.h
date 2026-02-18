#ifndef __GNET_CONSIGNTOOL_H
#define __GNET_CONSIGNTOOL_H
#include "gconsigndb"
#include "localmacro.h"
namespace GNET
{
	namespace CONSIGN
	{
		int CheckStateChange(const GConsignDB& detail,ConsignState target_state);
		int UpdateState( GConsignDB& detail );
	};
	using CONSIGN::CheckStateChange;
	using CONSIGN::UpdateState;
};
#endif
