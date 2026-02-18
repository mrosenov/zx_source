#ifndef __GNET_MATRIXCHECKER_H
#define __GNET_MATRIXCHECKER_H

namespace GNET
{

class MatrixChecker
{
	unsigned int uid;
	unsigned int ip;
public:
	MatrixChecker() {}
	MatrixChecker( unsigned int _uid, unsigned int _ip ) : uid(_uid), ip(_ip) {}
	MatrixChecker( const MatrixChecker& r) : uid(r.uid), ip(r.ip) {} 
	virtual ~MatrixChecker() {}
	virtual unsigned int Challenge() { return 0; } 
	virtual unsigned int getClass() const = 0;
	virtual bool Verify( unsigned int response) { return true; }
	unsigned int GetUid() { return uid; }
	unsigned int GetIp()  { return ip; }
};

}

#endif

