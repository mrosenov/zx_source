#ifndef __GNET_MAPGAMEATTR_H
#define __GNET_MAPGAMEATTR_H
#include <map>
#include "privilege.hxx"
#include "mutex.h"
#include "gameattr"
#include <utility>
#include "marshal.h"
namespace GNET
{
	class GameAttrMap
	{
	public:	
		enum {
			_ATTR_DISABLE=0,
			_ATTR_ENABLE=1,
		};
#define MAX_OF_MULTIPLEEXP 6
#define MIN_OF_MULTIPLEEXP 1
		typedef unsigned char attri_t;
		typedef Octets        value_t;
	private:	
	   	typedef std::map<attri_t,value_t> AttrMap;
		GameAttrMap() {
			m_mapAttr_[Privilege::PRV_LAMBDA] = Marshal::OctetsStream()<<(unsigned char) 0;
			m_mapAttr_[Privilege::PRV_NOTRADE] = Marshal::OctetsStream()<<(unsigned char) _ATTR_DISABLE;
			m_mapAttr_[Privilege::PRV_NOAUCTION] = Marshal::OctetsStream()<<(unsigned char) _ATTR_DISABLE;
			m_mapAttr_[Privilege::PRV_NOMAIL]=Marshal::OctetsStream()<<(unsigned char) _ATTR_DISABLE;
			m_mapAttr_[Privilege::PRV_NOFACTION]=Marshal::OctetsStream()<<(unsigned char) _ATTR_DISABLE;
			m_mapAttr_[Privilege::PRV_DOUBLESP]=Marshal::OctetsStream()<<(unsigned char) _ATTR_DISABLE;
			m_mapAttr_[Privilege::PRV_NOSELLPOINT]=Marshal::OctetsStream()<<(unsigned char) _ATTR_DISABLE;
			
			// 修改原来的双倍为多倍，value_t的值不再为0，1.可以根据具体的倍数进行设置,数值范围为1-6
			m_mapAttr_[Privilege::PRV_MULTIPLE_EXP] = Marshal::OctetsStream()<<(unsigned char)MIN_OF_MULTIPLEEXP;
			m_mapAttr_[Privilege::PRV_DOUBLEMONEY]=Marshal::OctetsStream()<<(unsigned char) _ATTR_DISABLE;
			m_mapAttr_[Privilege::PRV_DOUBLEOBJECT]=Marshal::OctetsStream()<<(unsigned char) _ATTR_DISABLE;
		}
		static GameAttrMap& _instance() {
			static GameAttrMap instance;
			return instance;
		}	
	public:
		~GameAttrMap() { }
		static bool Put(attri_t attr,value_t value) {
			Thread::Mutex::Scoped l(_instance().m_locker_);
			AttrMap::iterator it=_instance().m_mapAttr_.find(attr);
			if ( it!=_instance().m_mapAttr_.end() )
			{
				(*it).second=value;
				return true;
			}
			return false;
		}
		static bool IsValid(attri_t attr, value_t value) {
			bool res = true;
			if(Privilege::PRV_MULTIPLE_EXP == attr)
			{
				try
				{
					Marshal::OctetsStream os(value);
					unsigned char cvalue = 0;
					os >> cvalue;
					if((cvalue < MIN_OF_MULTIPLEEXP) || (cvalue > MAX_OF_MULTIPLEEXP))
					{
						res = false;
					}
				}
				catch (Marshal::Exception e)
				{
					res = false;
				}
			}
			return res;
		}
		static value_t Get(attri_t attr) {
			Thread::Mutex::Scoped l(_instance().m_locker_);
			AttrMap::iterator it=_instance().m_mapAttr_.find(attr);
			return it==_instance().m_mapAttr_.end() ? Octets() : (*it).second;
		}
		static void Get( GameAttrVector& attrs ) {
			Thread::Mutex::Scoped l(_instance().m_locker_);
			AttrMap::iterator it=_instance().m_mapAttr_.begin(),ite=_instance().m_mapAttr_.end();
			for ( ;it!=ite;++it )
				attrs.push_back( GameAttr((*it).first,(*it).second) );
		}
	private:	
		Thread::Mutex m_locker_;
		AttrMap m_mapAttr_;
	};
}
#endif
