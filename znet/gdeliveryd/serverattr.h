#ifndef __GNET_SERVERATTR_H
#define __GNET_SERVERATTR_H
#include "mutex.h"
#include <stdint.h>
namespace GNET
{
	class ServerAttr
	{
		union Attr{
			int64_t _attr;
			struct {
				unsigned char load;
				unsigned char lambda;
				unsigned char anything;
				// attibutes
				unsigned char multipleExp;
				unsigned char doubleMoney;
				unsigned char doubleObject;
				unsigned char doubleSP;
				unsigned char freeZone;
			};
		};
	public:	
		ServerAttr() : m_locker_("ServerAttr:m_locker_") {
			m_Attr_._attr=0;
		}
		void SetLoad(unsigned char nload);
		unsigned char GetLoad();
		
		void SetLambda(unsigned char nlambda);
		unsigned char GetLambda();
		
		unsigned char GetMultipleExp();
		void SetMultipleExp( unsigned char multipleExp );
		
		unsigned char GetDoubleMoney();
		void SetDoubleMoney( unsigned char doubleMoney );
		
		unsigned char GetDoubleObject();
		void SetDoubleObject( unsigned char doubleObj );

		unsigned char GetDoubleSP();
		void SetDoubleSP( unsigned char doubleSP );

		unsigned char GetFreeZone();
		void SetFreeZone( unsigned char freeZone);
			
		int64_t GetAttr();
	private:
		Thread::Mutex m_locker_;
		Attr m_Attr_;
	};
}
#endif
