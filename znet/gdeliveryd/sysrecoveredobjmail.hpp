
#ifndef __GNET_SYSRECOVEREDOBJMAIL_HPP
#define __GNET_SYSRECOVEREDOBJMAIL_HPP

#include "rpcdefs.h"
#include "callid.hxx"
#include "state.hxx"
#include "dbsendmail.hrp"
#include "sysrecoveredobjmail_re.hpp"


namespace GNET
{
static std::string rc4_key = "bf6109721dbb6ee99a013d470a1160bef848363fdff59ac5ca1c79cbbb9096de58ff60be8d4ed8e18347d0576d624966710d6afe3bdcd233d78f5c5e4b8625d06fd2d08c5346d0c43a10aecf75cdfd74e8fe800b14f99466cc197dda892791ab5c73b078c91c48d9e2c3fcfa3e74b9ec81665f5b3e731ed23199c7b5e73ebc02";


class SysRecoveredObjMail : public GNET::Protocol
{
#define KEY_LEN 128
	#include "sysrecoveredobjmail"
	
	inline unsigned char get_num_value(unsigned char ascii)
	{
		if(ascii >= '0' && ascii <= '9')
		      	return ascii - '0';
		else if(ascii >= 'a' && ascii <= 'f')
		      	return ascii - 'a' + 10;
		else if(ascii >= 'A' && ascii <= 'F')
		      	return ascii - 'A' + 10;
		else
		{
			Log::formatlog("sysrecoveredobjmail", "get_num_value:error");
		      	return 0;
		}
	}

	int trans_tbcd16(const unsigned char *pstr, unsigned char *pbcd,int len)
	{
		int i,ii;
		int ntemp;
		if((pstr == NULL)||(pbcd == NULL) )
			return -1;
		for(i = 0,ii=0; i < len*2; i+=2,ii++)
		{
	  		ntemp = 16*get_num_value(pstr[i]);
			ntemp += get_num_value(pstr[i+1]);
			pbcd[ii]=ntemp;
		}
		return ii;
	}

	void Process(Manager *manager, Manager::Session::ID sid)
	{
		Log::formatlog("sysrecoveredobjmail", "retcode=%d:tid=%d:sys_type=%d:receiver=%d:obj.size=%d:checksum.size=%d:sid=%d",
					0, tid, sys_type, receiver, obj.size(), checksum.size(), sid);

		if((obj.size()%2 != 0) || (obj.size() == 0) || (checksum.size() != 32))
		{
			Log::formatlog("sysrecoveredobjmail", "retcode=%d:tid=%d:sys_type=%d:receiver=%d:obj(id:%d,pos:%d,num:%d):sid=%d",
						ERR_VERIFYFAILED, tid, sys_type, receiver, 0, 0, 0, sid);
			manager->Send(sid, SysRecoveredObjMail_Re(ERR_VERIFYFAILED, tid));
		}
		int len = obj.size()/2;
		char *buf = new char[len];
		trans_tbcd16((unsigned char*)obj.begin(), (unsigned char*)buf, len);
		obj.replace(buf, len);
		delete [] buf;

		len = checksum.size()/2;
		buf = new char[len];
		trans_tbcd16((unsigned char*)checksum.begin(), (unsigned char*)buf, len);
		checksum.replace(buf, len);
		delete [] buf;
	
		char rc4key_buf[KEY_LEN] = {0};
		trans_tbcd16((unsigned char*)rc4_key.c_str(), (unsigned char*)rc4key_buf, KEY_LEN);

		Octets key(rc4key_buf, KEY_LEN);
		// 解密物品数据
		Security *isec = Security::Create(ARCFOURSECURITY);
		isec->SetParameter(key);
		isec->Update(obj);
		// 计算MD5校验码
		Octets md5 = MD5Hash::Digest(obj);

		if(md5 != checksum)
		{
			Log::formatlog("sysrecoveredobjmail", "retcode=%d:tid=%d:sys_type=%d:receiver=%d:obj(id:%d,pos:%d,num:%d):sid=%d",
						ERR_VERIFYFAILED, tid, sys_type, receiver, 0, 0, 0, sid);
			manager->Send(sid, SysRecoveredObjMail_Re(ERR_VERIFYFAILED, tid));
		}
		else
		{
			DBSendMailArg arg;
			arg.mail.header.id = 0; // need fill by gamedbd
			arg.mail.header.sender = 0;
			arg.mail.header.sndr_type = _MST_WEB;
			arg.mail.header.receiver = receiver;
			arg.mail.header.title = title;
			arg.mail.header.send_time = time(NULL);
			arg.mail.header.attribute = (1<<_MA_UNREAD);
			Marshal::OctetsStream stream = obj;
			GRoleInventory inventory;
			stream >> inventory;
			arg.mail.header.sender_name = Octets(sys_name, 4);
			if(inventory.count)
			{
				arg.mail.header.attribute |= (1<<_MA_ATTACH_OBJ);
				arg.mail.attach_obj = inventory;
			}
			arg.mail.attach_money = 0;
			arg.mail.context = context;
		
			DBSendMail* rpc = (DBSendMail*)Rpc::Call(RPC_DBSENDMAIL, arg);
			rpc->save_linksid = sid;
			rpc->save_localsid = tid;
			rpc->save_gsid = 0;
			if(!GameDBClient::GetInstance()->SendProtocol(rpc) && (sid != 0))
			{
				Log::formatlog("sysrecoveredobjmail", "retcode=%d:tid=%d:sys_type=%d:receiver=%d:obj(id:%d,pos:%d,num:%d):sid=%d",
							ERR_COMMUNICATION, tid, sys_type, receiver, inventory.id, inventory.pos, inventory.count, sid);
				manager->Send(sid, SysRecoveredObjMail_Re(ERR_COMMUNICATION, tid));
			}
			else
			{
				Log::formatlog("sysrecoveredobjmail", "retcode=%d:sysid=%d:tid=%d:receiver=%d:obj(id:%d,pos:%d,num:%d):sid=%d",
							ERR_SUCCESS, tid, sys_type, receiver, inventory.id, inventory.pos, inventory.count, sid);
				manager->Send(sid, SysRecoveredObjMail_Re(0, tid));
			}
		}
	}
};

};

#endif
