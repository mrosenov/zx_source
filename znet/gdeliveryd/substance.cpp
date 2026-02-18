#include "substance.h"
#include "hometowndef.h"

namespace GNET
{
	ClassInfo Substance::_classSubstance(HT_CLSID_SUBSTANCE, "Substance", NULL, (Substance *(*)())Substance::CreateObject);
};
