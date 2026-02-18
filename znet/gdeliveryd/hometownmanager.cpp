#include "hometownmanager.h"
#include "hometownprotocol.h"
#include "hometownproto_imp.h"
#include "hometowndef.h"
#include "dbhometowngetinfo.hrp"
#include "dbhometownputinfo.hrp"
#include "gamedbclient.hpp"
#include "dbhometownexchgmoney.hrp"
#include "hometownexchgmoney_re.hpp"
#include "gmailendsync.hpp"
#include "gdeliveryserver.hpp"
#include "mapuser.h"
#include "gproviderserver.hpp"
#include "hometownitembody.h"
#include "hometownitemfunc.h"

namespace GNET
{
	bool HometownManager::Init()
	{
		//Init template
		if (HometownTemplate::GetInstance()->load_data("hometowndata") == -1) return false;
		//下面两个顺序不能颠倒
		HometownItemFuncMan::GetInstance()->Init();
		HometownItemBodyMan::GetInstance()->Init();

		//Init components
		FarmCompoManager::GetInstance()->Init();

		IntervalTimer::Attach(this,1000000/IntervalTimer::Resolution());
		_config = HometownTemplate::GetInstance()->get_hometown_config();
		return _config!=NULL;
	}

	void HometownManager::LoadPlayer(int roleid)
	{
		HometownPlayerRef player;
		if (FindPlayer(roleid, player) && player->IsLoaded())
		{
			_online_map[roleid] = player;
			std::set<int>::iterator it = player->_friend_list.begin(), ie = player->_friend_list.end();
			for (; it != ie; ++it)
				LoadFriend(player, *it);
		}
		else
		{
			HometownPlayerRef newplayer(new HometownPlayer());
			newplayer->_roleid = roleid;
			_player_map[roleid] = newplayer;
			_online_map[roleid] = newplayer;

			DBHometownGetInfo *rpc = (DBHometownGetInfo *)Rpc::Call(RPC_DBHOMETOWNGETINFO, DBHometownGetInfoArg(roleid, roleid));
			GameDBClient::GetInstance()->SendProtocol(rpc);
			newplayer->SetLoading();
		}
	}

	void HometownManager::LoadFriend(const HometownPlayerRef &sponsor, int roleid)
	{
		if (sponsor->_roleid == roleid) return;

		PlayerMap::iterator it = _player_map.find(roleid);
		if (it == _player_map.end())
		{
			it = _player_map.insert(PlayerMap::value_type(roleid, HometownPlayerRef(new HometownPlayer()))).first;
			it->second->_roleid = roleid;
		}

		HometownPlayerRef &friend_player = it->second;
		if (friend_player->IsUnload())
		{
			DBHometownGetInfo *rpc = (DBHometownGetInfo *)Rpc::Call(RPC_DBHOMETOWNGETINFO, DBHometownGetInfoArg(sponsor->_roleid, roleid));
			GameDBClient::GetInstance()->SendProtocol(rpc);
			friend_player->SetLoading();
		}

		sponsor->_friend_ref_list[roleid] = friend_player;
	}

	void HometownManager::OnAddFriend(int srcroleid, int dstroleid)
	{
		HometownPlayerRef srcplayer;
		if (FindPlayerOnline(srcroleid, srcplayer))
		{
			srcplayer->AddFriend(dstroleid);
			LoadFriend(srcplayer, dstroleid);
		}
	}

	void HometownManager::OnDelFriend(int srcroleid, int dstroleid)
	{
		HometownPlayerRef srcplayer;
		if (FindPlayerOnline(srcroleid, srcplayer))
		{
			srcplayer->DelFriend(dstroleid);
		}
	}

	bool HometownManager::GetPlayerName(int roleid, Octets &name)
	{
		HometownPlayerRef player;
		if (FindPlayer(roleid, player))
		{
			name = player->_name;
			return true;
		}
		return false;
	}

	bool HometownManager::GetPlayerName(int roleid, char *buf, char &buflen)
	{
		HometownPlayerRef player;
		if (FindPlayer(roleid, player))
		{
			buflen = player->_name.size()>(size_t)buflen ? buflen : (char)player->_name.size();
			if (buflen <= 0) return false;
			memcpy(buf, player->_name.begin(), buflen);
			return true;
		}
		return false;
	}

	void HometownManager::OnPlayerLogin(int roleid)
	{
		LoadPlayer(roleid);
	}

	void HometownManager::OnPlayerLogout(int roleid)
	{
		HometownPlayerRef player;
		if (FindPlayerOnline(roleid, player))
		{
			player->_friend_ref_list.clear();		//删除对好友的引用
			_online_map.erase(roleid);				//删除对自身的引用
		}
	}

	void HometownManager::OnLoad(int sponsor, int roleid, const Octets &name, const GHometownInfo &ht_info, const std::set<int> &friend_list, const std::map<int, Octets> &compo_info)
	{
		HometownPlayerRef player;
		if (FindPlayer(roleid, player))
		{
			if (!player->IsLoaded())
			{
				const GHometownPlayer &gplayer = ht_info.player;
				player->_roleid = roleid;
				player->_name = name;
				player->_money = gplayer.money;
				player->_last_exchgmoney_time = gplayer.last_exchgmoney_time;
				player->_compo_list = gplayer.compo_list;
				player->_friend_list = friend_list;
				player->SetLoaded();

				std::map<int, Octets>::const_iterator it, ie = compo_info.end();
				for (it = compo_info.begin(); it != ie; ++it)
				{
					BaseCompoManager *compo = GetCompo(it->first);
					if (compo != NULL)
						compo->OnLoad(roleid, it->second);
				}
			}

			if (sponsor == roleid)
			{
				player->_friend_list = friend_list;
				std::set<int>::iterator it = friend_list.begin(), ie = friend_list.end();
				for (; it != ie; ++it)
					LoadFriend(player, *it);
			}
		}
	}

	void HometownManager::OnLoadFail(int sponsor, int roleid)
	{
//		HometownPlayerRef ref;
//		if (FindPlayer(roleid, ref)) ref->SetLoaded();
		PlayerMap::iterator it = _player_map.find(roleid);
		if (it != _player_map.end())
			_player_map.erase(roleid);
		it = _online_map.find(roleid);
		if (it != _online_map.end())
			_online_map.erase(roleid);
	}

	bool HometownManager::CheckCmd(void *data, int size)
	{
		using HOMETOWN::C2S::hometown_c2s_cmd_header;
		return size>=(int)sizeof(hometown_c2s_cmd_header);
	}

	void HometownManager::HandleCmd(int roleid, void *data, int size, HOMETOWN::WRAPPER &response)
	{
		using namespace HOMETOWN::S2C;
		using HOMETOWN::C2S::hometown_c2s_cmd_header;

		if (!CheckCmd(data, size)) return;
		hometown_c2s_cmd_header *ph = (hometown_c2s_cmd_header *)data;
		Make<hometown_s2c_cmd_header>::From(response, HT_ERR_SUCCESS, ph->cmd_type, ph->compo_id);
		switch (ph->cmd_type)
		{
			case HOMETOWN::COMPO_LIST:
				{
					using HOMETOWN::C2S::CMD::compo_list;
					if (size != sizeof(hometown_c2s_cmd_header)+sizeof(compo_list))
					{
						response.set_retcode(HT_ERR_PACKETSIZE);
						return;
					}
					response.push_back(ph+1, sizeof(compo_list));
					compo_list *pkt = (compo_list *)(ph+1);
					if (pkt->roleid!=roleid && !IsFriend(pkt->roleid, roleid))
					{
						response.set_retcode(HT_ERR_NOTFRIEND);
						return;
					}
					//如果玩家数据没有成功load，禁止一切请求 20091230
					HometownPlayerRef sponsor, player;
					if (FindPlayerOnline(roleid, sponsor) && FindPlayer(pkt->roleid, player) && sponsor->IsLoaded() && player->IsLoaded())
					{
						response << sponsor->_money;
						response << player->_compo_list;
					}
					else
						response.set_retcode(HT_ERR_PLAYEROFFLINE);
				}
				break;
			case HOMETOWN::COMPO_ADD:
				{
					HometownPlayerRef player;
					if (!FindPlayerOnline(roleid, player) || !player->IsLoaded())
					{
						response.set_retcode(HT_ERR_PLAYEROFFLINE);
						return;
					}
					if (player->HasCompo(ph->compo_id))
					{
						response.set_retcode(HT_ERR_ALREADYADDCOMPO);
						return;
					}

					BaseCompoManager *compo = GetCompo(ph->compo_id);
					if (compo != NULL)
						compo->HandleCmd(roleid, HOMETOWN::COMPO_ADD, ph+1, size-sizeof(hometown_c2s_cmd_header), response);
					else
						response.set_retcode(HT_ERR_INVALIDCOMPOID);
					
					if (response.get_retcode() == HT_ERR_SUCCESS)
						player->AddCompo(ph->compo_id);
				}
				break;
			default:
				{
					HometownPlayerRef player;
					if (!FindPlayerOnline(roleid, player) || !player->IsLoaded())
					{
						response.set_retcode(HT_ERR_PLAYEROFFLINE);
						return;
					}
					if (!player->HasCompo(ph->compo_id))
					{
						response.set_retcode(HT_ERR_NOTADDCOMPO);
						return;
					}
					BaseCompoManager *compo = GetCompo(ph->compo_id);
					if (compo != NULL)
						compo->HandleCmd(roleid, ph->cmd_type, ph+1, size-sizeof(hometown_c2s_cmd_header), response);
					else
						response.set_retcode(HT_ERR_INVALIDCOMPOID);
				}
				break;
		}
	}

	bool HometownManager::Update()
	{
		{
			//check player
			PlayerMap::iterator it = _player_map.lower_bound(_update_cursor);
			PlayerMap::iterator ie = _player_map.end();
			for (int i = 0; it!=ie && i<50; ++i)
			{
				int roleid = it->first;
				bool dirty = it->second->IsDirty();
				if (dirty)
				{
					DBHometownPutInfoArg arg;
					arg.roleid = it->first;
					arg.info.player.money = it->second->_money;
					arg.info.player.last_exchgmoney_time = (int)it->second->_last_exchgmoney_time;
					arg.info.player.compo_list = it->second->_compo_list;
					DBHometownPutInfo *rpc = (DBHometownPutInfo *)Rpc::Call(RPC_DBHOMETOWNPUTINFO, arg);
					GameDBClient::GetInstance()->SendProtocol(rpc);
					it->second->SetDirty(false);
				}
				std::set<int>::iterator sit = it->second->_compo_list.begin();
				std::set<int>::iterator sie = it->second->_compo_list.end();
				for (; sit != sie; ++sit)
				{
					BaseCompoManager *compo = GetCompo(*sit);
					if (compo != NULL)
					{
						if (compo->IsDirty(roleid))
						{
							dirty = true;
							compo->SyncToDB(roleid);
						}
					}
				}
				if (!dirty && it->second.get_use_count()==1)
				{
					for (sit = it->second->_compo_list.begin(); sit != sie; ++sit)
					{
						BaseCompoManager *compo = GetCompo(*sit);
						if (compo != NULL)
							compo->ErasePlayer(roleid);
					}
					_player_map.erase(it++);
					LOG_TRACE("HometownManager:: erase role %d with refcnt==1\n", roleid);
				}
				else
					++it;
			}
			if (it != ie) 
				_update_cursor = it->first;
			else
				_update_cursor = 0;
		}

		{
			//update compo
			CompoMap::iterator it = _compo_map.begin(), ie = _compo_map.end();
			for (; it != ie; ++it)
			{
				it->second->Update();
			}
		}


		return true;
	}

	void HometownManager::OnSyncCompo(int retcode, int compo_id, int roleid)
	{
		BaseCompoManager *compo = GetCompo(compo_id);
		if (compo != NULL)
		{
			compo->OnSyncToDB(retcode, roleid);
		}
	}
	void HometownManager::OnSyncHometown(int retcode, int roleid)
	{
		if (retcode != ERR_SUCCESS)
		{
			HometownPlayerRef player;
			if (FindPlayer(roleid, player))
				player->SetDirty(true);
		}
	}

	void HometownManager::ExchgMoney(int roleid, int gs_sid, int amount, const GMailSyncData &syncdata)
	{
		GProviderServer *gsm = GProviderServer::GetInstance();

		int retcode = ERR_SUCCESS;
		if (amount <= 0)
			retcode = HT_ERR_MONEYEXCHGAMOUNT;

		//游戏币兑通用币冷却7天，而且每次兑换数量必须小于等于500
		if (amount > 500)
			amount = 500;

		HometownPlayerRef ht_player;
		if (retcode == ERR_SUCCESS)
		{
			if (syncdata.inventory.money/10000<(unsigned int)amount)
				retcode = HT_ERR_MONEYNOTENOUGH;

			if (retcode==ERR_SUCCESS && !FindPlayerOnline(roleid, ht_player))
				retcode = HT_ERR_PLAYEROFFLINE;
		}
		if (retcode == ERR_SUCCESS)
		{
			if (Timer::GetTime()-ht_player->_last_exchgmoney_time<7*24*3600)
			{
				retcode = HT_ERR_COOLDOWN;
			}
		}
		if (retcode != ERR_SUCCESS)
		{
			gsm->Send(gs_sid, GMailEndSync(0, retcode, roleid));
			PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
			if (pinfo != NULL) GDeliveryServer::GetInstance()->Send(pinfo->linksid, HometownExchgMoney_Re(retcode, roleid, 0, pinfo->localsid));
			return;
		}
		DBHometownExchgMoney *rpc = (DBHometownExchgMoney *)Rpc::Call(RPC_DBHOMETOWNEXCHGMONEY, DBHometownExchgMoneyArg(roleid, amount, ht_player->_money, syncdata));
		rpc->gs_sid = gs_sid;
		GameDBClient::GetInstance()->SendProtocol(rpc);
	}

	void HometownManager::OnExchgMoney(int retcode, int roleid, int gs_sid, int amount, const GMailSyncData &syncdata)
	{
		GProviderServer *gsm = GProviderServer::GetInstance();
		GDeliveryServer *dsm = GDeliveryServer::GetInstance();

		PlayerInfo *pinfo = UserContainer::GetInstance().FindRoleOnline(roleid);
		if (pinfo == NULL) 
		{
			gsm->Send(gs_sid, GMailEndSync(0, ERR_FC_OFFLINE, roleid));
			return;
		}
		if (retcode != ERR_SUCCESS)
		{
			gsm->Send(gs_sid, GMailEndSync(0, retcode, roleid));
			dsm->Send(pinfo->linksid, HometownExchgMoney_Re(retcode, roleid, 0, pinfo->localsid));
			return;
		}

		HometownPlayerRef ht_player;
		if (!FindPlayerOnline(roleid, ht_player))
		{
			gsm->Send(gs_sid, GMailEndSync(0, ERR_FC_OFFLINE, roleid));
			dsm->Send(pinfo->linksid, HometownExchgMoney_Re(HT_ERR_PLAYEROFFLINE, roleid, 0, pinfo->localsid));
			return;
		}

		ht_player->_last_exchgmoney_time = Timer::GetTime();
		ht_player->_money += amount;
		gsm->Send(gs_sid, GMailEndSync(0, ERR_SUCCESS, roleid, syncdata));
		dsm->Send(pinfo->linksid, HometownExchgMoney_Re(HT_ERR_SUCCESS, roleid, ht_player->_money, pinfo->localsid));
	}
};
