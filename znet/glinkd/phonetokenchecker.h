#ifndef __GNET_PHONETOKENCHECKER_H
#define	__GNET_PHONETOKENCHECKER_H
#include "matrixchecker.h"

namespace GNET
{
#ifdef __GNUC__
	#define gd_byteorder_32(x) (x)
#elif defined BYTE_ORDER_BIG_ENDIAN
	inline unsigned int gd_byteorder_32(unsigned int x)
	{
		register unsigned int v;
		__asm__ ("bswap %0" : "=r="(v) : "0"(x));
		return v;
	}
#endif

	class PhoneTokenChecker : public MatrixChecker
	{
		Octets	seed;
		int		rtime;//when use cache,consider the time-diff between au and delivery
		public:
		PhoneTokenChecker() {}	
		PhoneTokenChecker(Octets & _seed, int _rtime) : seed(_seed),rtime(_rtime) {}
		PhoneTokenChecker(const PhoneTokenChecker& r) : MatrixChecker(r), seed(r.seed), rtime(r.rtime) {}

		PhoneTokenChecker(unsigned int uid, unsigned int ip, Octets& _seed, int _rtime) : MatrixChecker(uid,ip), seed(_seed), rtime(_rtime)
		{

		}

		unsigned int GetElecNumber()
		{
			char ret[6];/*ret[0] is the first elec number*/
			unsigned char text[8];
			((int*)text)[1] = ((int*)text)[0] = gd_byteorder_32(rtime);//use Little-Endian(intel-linux is LE)
			Octets re(text,8);
			HMAC_MD5Hash hash;
			Octets os;
			hash.SetParameter(seed);
			hash.Update(re); 
			hash.Final(os);

			const unsigned char *p = (const unsigned char*)os.begin();
			unsigned char pos = p[15] & 0xf;
			if(pos > 11)
				pos = 11;
			const unsigned char *p2 = p + pos;
			for(int i = 0; i < 3 ; ++i)
			{
				unsigned char c = p2[i];
				unsigned char c1 = (c & 0xf) & 7;
				if( p[14] & (1 << (2*i)) ) ++c1;
				if( p[13] & (1 << (2*i)) ) ++c1;
				unsigned char c2 = ((c >> 4) & 0xf) & 7;
				if( p[14] & (1 << (2*i + 1)) ) ++c2;
				if( p[13] & (1 << (2*i + 1)) ) ++c2;

				ret[i*2] = c1;
				ret[i*2+1] = c2;
			}
			/*for(int i=0;i<6;++i)
			  {
			  printf(" %d",ret[i]);
			  }
			  printf("\n");
			 */
			return ret[0]*100000+ret[1]*10000+ret[2]*1000+ret[3]*100+ret[4]*10+ret[5];
		}

		unsigned int getClass() const
		{
			return ALGORITHM_PHONETOKEN;
		}

		unsigned int Challenge()
		{
			return 0;
		}

		bool Verify( unsigned int response )
		{
			if(seed.size() != 16)
			{
				Log::log(LOG_ERR,"PhoneToken's seed.size invalid,size=%d",seed.size());
				return false;
			}

			unsigned int num = GetElecNumber();
			if(response != num)
			{
				this->rtime -= 1;
				num = GetElecNumber();
				if(response != num)
				{
					this->rtime += 2;
					num = GetElecNumber();
					if(response != num)
					{
						this->rtime -= 3;
						num = GetElecNumber();
						if(response != num)
						{
							this->rtime += 4;
							num = GetElecNumber();
							if(response != num)
								return false;
						}
					}
				}
			}
			return true;
		}
	};
};
#endif
