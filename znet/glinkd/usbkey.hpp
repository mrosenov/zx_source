#ifndef _GNET_USBKEY_HPP
#define _GNET_USBKEY_HPP

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

class UsbKey
{
private:
	Octets _seed;
	Octets _pin;
	int _rtime;
public:
	UsbKey()
	{
	}

	UsbKey(const Octets &seed, const Octets &pin, int rtime) : _seed(seed), _pin(pin), _rtime(rtime)
	{
	}

	UsbKey(const UsbKey &rhs) : _seed(rhs._seed), _pin(rhs._pin), _rtime(rhs._rtime)
	{
	}

	bool CheckPasswd(Octets &account, Octets &challenge, const Octets &passwd, Octets &elec_number)
	{
		if(_seed.size() != 16)
		{
			Log::log(LOG_ERR, "USBKEY2's seed.size invalid,size=%d", _seed.size());
			return false;
		}

		if(_pin.size() != 4 && _pin.size() != 0)
		{
			Log::log(LOG_ERR, "USBKEY2's pin.size invalid,size=%d", _pin.size());
			return false;
		}

		int rtimes[3] = {_rtime, _rtime - 1, _rtime + 1};
		Octets temp, passwdmd5, response;
		for(int i = 0; i < 3; i++)
		{
			temp.clear();
			passwdmd5.clear();
			response.clear();

			if(_pin.size() == 4)
			{
				temp.insert(temp.end(), _pin.begin(), _pin.size());
			}
			unsigned char ret[6];
			GetElecNumber(ret, rtimes[i]);
			for(int j = 0; j < 6; ++j)
			{
				ret[j] += '0';
			}
			temp.insert(temp.end(), ret, sizeof(ret));

			MD5Hash md5;
			md5.Update(account);
			md5.Update(temp);
			md5.Final(passwdmd5);

			HMAC_MD5Hash hash;
			hash.SetParameter(passwdmd5);
			hash.Update(challenge);
			hash.Final(response);
			if(response == passwd)
			{
				elec_number.replace(ret, sizeof(ret));
				return true;
			}
		}
		return false;
	}
private:
	void GetElecNumber(unsigned char ret[6], int rtime) // ret[0] is the first elec number
	{
		unsigned char text[8];
		((int *)text)[1] = ((int *)text)[0] = gd_byteorder_32(rtime); // use Little-Endian(intel-linux is LE)
		Octets re(text, 8);
		HMAC_MD5Hash hash;
		Octets os;
		hash.SetParameter(_seed);
		hash.Update(re);
		hash.Final(os);

		const unsigned char *p = (const unsigned char *)os.begin();
		unsigned char pos = p[15] & 0xf;
		if(pos > 11)
			pos = 11;
		const unsigned char *p2 = p + pos;
		for(int i = 0; i < 3; ++i)
		{
			unsigned char c = p2[i];
			unsigned char c1 = (c & 0xf) & 7;
			if(p[14] & (1 << (2*i)))
				++c1;
			if(p[13] & (1 << (2*i)))
				++c1;
			unsigned char c2 = ((c >> 4) & 0xf) & 7;
			if(p[14] & (1 << (2*i + 1)))
				++c2;
			if(p[13] & (1 << (2*i + 1)))
				++c2;

			ret[i*2] = c1;
			ret[i*2 + 1] = c2;
		}
	}
};

};
#endif
