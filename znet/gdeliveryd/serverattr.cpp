#include "serverattr.h"
namespace GNET
{
	void ServerAttr::SetLoad(unsigned char nload) {
		Thread::Mutex::Scoped l(m_locker_);
		m_Attr_.load=nload;
	}
	unsigned char ServerAttr::GetLoad() {
		Thread::Mutex::Scoped l(m_locker_);
		return m_Attr_.load;
	}
	void ServerAttr::SetLambda(unsigned char nlambda) {
		Thread::Mutex::Scoped l(m_locker_);
		m_Attr_.lambda=nlambda;
	}
	unsigned char ServerAttr::GetLambda() {
		Thread::Mutex::Scoped l(m_locker_);
		return m_Attr_.lambda;
	}
	unsigned char ServerAttr::GetMultipleExp() {
		Thread::Mutex::Scoped l(m_locker_);
		return m_Attr_.multipleExp;
	}
	void ServerAttr::SetMultipleExp( unsigned char multipleExp ) {
		Thread::Mutex::Scoped l(m_locker_);
		m_Attr_.multipleExp=multipleExp;
	}

	unsigned char ServerAttr::GetDoubleMoney() {
		Thread::Mutex::Scoped l(m_locker_);
		return m_Attr_.doubleMoney;
	}	
	void ServerAttr::SetDoubleMoney( unsigned char doubleMoney ) {
		Thread::Mutex::Scoped l(m_locker_);
		m_Attr_.doubleMoney=doubleMoney;
	}
	
	unsigned char ServerAttr::GetDoubleObject() {
		Thread::Mutex::Scoped l(m_locker_);
		return m_Attr_.doubleObject;
	}
	void ServerAttr::SetDoubleObject( unsigned char doubleObj ) {
		Thread::Mutex::Scoped l(m_locker_);
		m_Attr_.doubleObject=doubleObj;
	}

	unsigned char ServerAttr::GetDoubleSP() {
		Thread::Mutex::Scoped l(m_locker_);
		return m_Attr_.doubleSP;
	}
	void ServerAttr::SetDoubleSP( unsigned char doubleSP ) {
		Thread::Mutex::Scoped l(m_locker_);
		m_Attr_.doubleSP=doubleSP;
	}
    unsigned char ServerAttr::GetFreeZone() {
		Thread::Mutex::Scoped l(m_locker_);
		return m_Attr_.freeZone;
	}
    void ServerAttr::SetFreeZone( unsigned char freeZone) {
		Thread::Mutex::Scoped l(m_locker_);
		m_Attr_.freeZone=freeZone;
	}
	int64_t ServerAttr::GetAttr() {
		Thread::Mutex::Scoped l(m_locker_);
		return m_Attr_._attr;
	}

}
