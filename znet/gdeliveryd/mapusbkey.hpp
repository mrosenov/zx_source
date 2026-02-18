#ifndef _GNET_USBKEY_HPP
#define _GNET_USBKEY_HPP

namespace GNET
{
class UsbKeyInfo
{
	Octets _seed;
	Octets _pin;
	int _rtime;
public:
	UsbKeyInfo()
	{
	}

	UsbKeyInfo(const Octets &seed, const Octets &pin, int rtime) : _seed(seed), _pin(pin), _rtime(rtime)
	{
	}

	UsbKeyInfo(const UsbKeyInfo &rhs) : _seed(rhs._seed), _pin(rhs._pin), _rtime(rhs._rtime)
	{
	}

	void GetUsbKeyInfo(Octets &seed, Octets &pin, int &rtime)
	{
		seed = _seed;
		pin = _pin;
		rtime = _rtime;
	}
};

class UsbKeyCache
{
private:
	typedef std::map<int/*userid*/, UsbKeyInfo> UsbMap;
	typedef UsbMap::iterator Iterator;
	UsbMap _usbmap;
	Thread::Mutex _locker;
	static int time_diff;

	UsbKeyCache() : _locker("UsbKeyCache::locker")
	{
	}
public:
	static UsbKeyCache &GetInstance()
	{
		static UsbKeyCache instance;
		return instance;
	}

	bool GetUsbKeyInfo(int userid, Octets &seed, Octets &pin, int &rtime, int &currenttime)
	{
		Thread::Mutex::Scoped l(_locker);
		Iterator it = _usbmap.find(userid);
		if(it != _usbmap.end())
		{
			it->second.GetUsbKeyInfo(seed, pin, rtime);
			currenttime = time_diff + (int)time(NULL);
			return true;
		}
		return false;
	}

	void SetUsbKeyInfo(int userid, Octets &seed, Octets &pin, int rtime, int currenttime)
	{
		Thread::Mutex::Scoped l(_locker);
		time_diff = currenttime - (int)time(NULL);
		_usbmap[userid] = UsbKeyInfo(seed, pin, rtime);
	}

	void RmvUsbKeyInfo(int userid)
	{
		_usbmap.erase(userid);
	}
};

};
#endif
