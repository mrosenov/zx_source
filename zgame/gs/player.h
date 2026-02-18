#ifndef __ONLINEGAME_GS_PLAYER_H__
#define __ONLINEGAME_GS_PLAYER_H__

#include <time.h>
#include "object.h"
#include <common/packetwrapper.h>
#include "config.h"

struct gplayer  : public gactive_object
{
	enum{
		EMPTY 		= 0,
		WAITING_LOGIN 	= 1,
		WAITING_ENTER	= 2,
		LOGIN_OK	= 3,	//登陆的正常状态
		WAITING_LOGOUT	= 4,
		DISCONNECTED 	= 5,
		WAITING_SWITCH  = 6,
	};
	enum
	{
		TITLE_FREEMAN    = 0,
		TITLE_SYSTEM     = 1,
		TITLE_MASTER     = 2, //帮主
		TITLE_VICEMASTER = 3, //副帮主
		TITLE_CAPTAIN    = 4, //长老
		TITLE_HEADER     = 5, //舵主
		TITLE_MEMBER     = 6,
	};
	inline gplayer * get_next() { return (gplayer*)pNext;}
	inline gplayer * get_prev() { return (gplayer*)pPrev;}
	int	cs_index;		//连接服务器索引
	int 	cs_sid;			//连接服务器session ID
	int	login_state;		//当前登录状态，在未生成imp时使用
	int 	id_mafia;		//帮派id
	int 	id_family;		//家族id
	int 	id_zone;		//服务器id
	int	extra_state;		//扩展object中的object_state
	int	extra_state2;		//扩展object中的object_state
	short	effect_list[MAX_PLAYER_EFFECT_COUNT];		//这个标记的数目在gactive_object里声明了
	unsigned char rank_mafia;	//帮派内角色( 2-帮主 3-副帮主 4-长老 5-舵主)
	unsigned char market_id;	//当前摊号
	unsigned char invisible;	//是否隐身
	unsigned char rank;		//军衔, 仅在战场里有效
	unsigned char vip_type;		//VIP的类型, 默认是0
	unsigned char rdir;		//对象所处的相对方向
	unsigned char circle_title;	//圈子职位
	unsigned char vipaward_level; //vip奖励等级
	char	is_hide_vip_level;		//是否隐藏vip等级
	int		market_item_id;			//使用道具摆摊时，客户端使用来创建不同的形象
	char	sect_flag;
	char	cultivation;		//造化 仙魔佛的组合
	char 	mount_type;		//骑乘的类型
	int	sect_id;
	int	circle_id;		//圈子id
	int 	adv_data1;
	int 	adv_data2;
	char 	pk_level;
	char	level;			//玩家级别级别
	char 	dt_level;		//玩家封神等级
	char 	bind_type;		//绑定类型 1下面 2 上面 
	char	mount_level;		//骑的马的级别
	char	level2;			//转生次数
	char 	bind_pos;		//多人绑定时候的position
	unsigned char wing_color;	//飞剑颜色
	bool bind_disconnect;		//绑定状态处于断开的状态
	int	bind_target;		//绑定的人 是谁
	int 	mount_id;		//骑的马是什么类型
	int	title_id;		//具有称谓 
	int	reputation;		//声望
	int	spouse_id;		//配偶
	int	team_id;		//队伍id
	int	fashion_mask;		//时装状态
	int 	transform_id;		//变身模板ID
	int	carrier_id;		//交通工具ID
	A3DVECTOR rpos;			//对象所处的相对位置
	A3DVECTOR before_mobactive_pos;     //互动前位置

	char darklight_state;	//客户端显示用轩辕光暗状态0； 光；1；暗
	char darklight_spirits[MAX_XUANYUAN_SPIRITS];	//轩辕灵体
	char mirror_image_cnt;	//镜像数量

	char start_fly_type;	//0: 飞剑; 1:技能

	char weapon_effect_level;

	short flow_battle_ckill;
	char puppet_id;		//傀儡id
	int 	mobactive_id;		//互动物品id 
	char mobactive_pos; 
	char mobactive_state;

	char active_emote_first_player;
	int active_emote_id;
	int active_emote_type;
	int travel_vehicle_id;
	
	bool	is_waitting_login() { return login_state == WAITING_LOGIN;}
	void Clear()
	{
		cs_sid = -1;
		cs_index = -1;
		login_state =  EMPTY;
		id_mafia = 0;
		rank_mafia = 0;
		id_family = 0;
		id_zone = 0;
		extra_state = 0;
		extra_state2 = 0;
		market_id = 0;
		ID.id = 0;
		base_info.cls = -1;
		base_info.gender = 0;
		invisible = 0;
		adv_data1 = adv_data2 = 0;
		bind_type  =0;
		level = 0;
		dt_level = 0;
		mount_level = 0;
		mount_type = 0;
		bind_target = 0;
		bind_disconnect = 0;
		bind_pos = 0;
		mount_id = 0;
		title_id = 0;
		reputation = 0;
		spouse_id = 0;
		team_id = 0;
		cultivation = 0;
		fashion_mask = 0;
		rank = 0;
		vip_type = 0;
		transform_id = 0;
		carrier_id = 0;
		rdir = 0;
		circle_id = 0;
		sect_id = 0;
		circle_title = 0;
		vipaward_level = 0;
		is_hide_vip_level = 0;
		market_item_id = -1;
		darklight_state = 0;
		wing_color = 0;
		memset(darklight_spirits, 0, sizeof(darklight_spirits));
		mirror_image_cnt = 0;
		start_fly_type = -1;
		weapon_effect_level = 0;
		flow_battle_ckill = 0;
		puppet_id = 0;
		mobactive_id = 0;
		mobactive_pos = 0;
		mobactive_state = 0;
		active_emote_id = 0;
		active_emote_first_player = 0;
		active_emote_type = -1;
		travel_vehicle_id = 0;
		gactive_object::Clear();
	}

	template <typename WRAPPER>
	WRAPPER &Export(WRAPPER & wrapper)
	{
		gactive_object::Export(wrapper);
		wrapper << ID << pos << msg_mask << crc << 
		id_mafia << rank_mafia << id_family << id_zone << market_id << invisible;
		wrapper.push_back(&base_info,sizeof(base_info));
		wrapper<< body_size << 
		cs_sid << cs_index << login_state << 
		adv_data1 << adv_data2 << mount_type << sect_id << circle_id << level << dt_level << 
		bind_type << mount_level << bind_pos << bind_disconnect <<
		bind_target << mount_id << title_id <<reputation << spouse_id<< team_id <<  
		rank << vip_type << transform_id << carrier_id <<  extra_state << extra_state2 << rdir << rpos << before_mobactive_pos <<
		circle_title << vipaward_level << is_hide_vip_level << darklight_state << mirror_image_cnt 
		<< market_item_id << wing_color << start_fly_type << weapon_effect_level << flow_battle_ckill << puppet_id << 
		mobactive_id << mobactive_pos << mobactive_state << active_emote_first_player << active_emote_id << active_emote_type << travel_vehicle_id;
		wrapper.push_back(effect_list,sizeof(effect_list));
		return wrapper.push_back(darklight_spirits,sizeof(darklight_spirits));
	}

	template <typename WRAPPER>
	WRAPPER & Import(WRAPPER & wrapper)
	{
		gactive_object::Import(wrapper);
		wrapper >> ID >> pos >> msg_mask >> crc >> 
		id_mafia >> rank_mafia >> id_family >> id_zone >> market_id >> invisible ;
		wrapper.pop_back(&base_info,sizeof(base_info));
		wrapper >> body_size >>
		cs_sid >> cs_index >> login_state >> 
		adv_data1 >> adv_data2 >> mount_type >> sect_id >> circle_id >> level >> dt_level >>
		bind_type >> mount_level >> bind_pos >> bind_disconnect >>
		bind_target >> mount_id >> title_id >> reputation >> spouse_id >> team_id >>  
		rank >> vip_type >> transform_id >> carrier_id >>  extra_state >> extra_state2 >>  rdir >> rpos >> before_mobactive_pos >>
	       	circle_title >> vipaward_level >> is_hide_vip_level >> darklight_state >> mirror_image_cnt 
		>> market_item_id >> wing_color >> start_fly_type >> weapon_effect_level >> flow_battle_ckill >>puppet_id >>  
		mobactive_id >> mobactive_pos >> mobactive_state >> active_emote_first_player >>  active_emote_id >> active_emote_type >> travel_vehicle_id;
		wrapper.pop_back(effect_list,sizeof(effect_list));
		return wrapper.pop_back(darklight_spirits,sizeof(darklight_spirits));
	}

	//这些状态仅仅保存在player身上
	enum
	{
		STATE_CARRIER		= 0x00000001,	//乘坐交通工具
		STATE_ANONYMOUS		= 0x00000002,	//匿名状态
		STATE_INVISIBLE		= 0x00000004,	//影遁状态
		STATE_DIM		= 0x00000008,	//朦胧状态
		STATE_CLONE		= 0x00000010,	//分身状态
		STATE_CIRCLE		= 0x00000020,	//圈子状态
		STATE_CROSSZONE		= 0x00000040,	//跨服状态
		STATE_IGNITE		= 0x00000080,	//燃烧状态
		STATE_FROZEN		= 0x00000100,	//冰封状态
		STATE_FURY		= 0x00000200,	//神圣之怒状态
		STATE_DARK		= 0x00000400,	//黑暗状态
		STATE_COLDINJURE	= 0x00000800,	//冻伤状态
		STATE_CIRCLEOFDOOM  = 0x00001000,   //阵法状态，只有在阵法发起者身上才有
		STATE_TURNBUFF		= 0x00002000,	//逆转正面
		STATE_TURNDEBUFF	= 0x00004000,	//逆转负面
		STATE_BLOODTHIRSTY	= 0x00008000,	//嗜血状态
		STATE_BESPIRITDRAGGED= 0x00010000,	//被牵引状态
		STATE_BEPULLED		= 0x00020000,	//被拉扯状态
		STATE_VIPAWARD		= 0x00040000,   //VIP奖励状态
		STATE_IN_COLLISION_RAID = 0x00080000,	//在碰撞战场
		STATE_XY_DARKLIGHT_STATE = 0x00100000, //轩辕的光暗状态
		STATE_XY_SPIRITS	= 0x00200000,	//轩辕灵体
		STATE_MIRROR_IMAGE	= 0x00400000,	//镜像
		STATE_WING_COLOR	= 0x00800000,	//飞剑颜色
		STATE_DIR_EXT_STATE = 0x01000000,	//带方向的扩展状态
		STATE_EXTRA_EQUIP_EFFECT = 0x02000000,	//装备额外光效
		STATE_FLOW_BATTLE	= 0x04000000,	//流水席战场状态
		STATE_PUPPET		= 0x08000000,	//傀儡姿态	
		STATE_QILIN		= 0x10000000,	//麒麟姿态
		STATE_MOBACTIVE 	= 0x20000000,   //互动物品状态
		STATE_ACTIVE_EMOTE      = 0x40000000,   //互动表情
		STATE_TRAVEL		= 0x80000000,   //自由飞行
	};

	enum        
	{   
		STATE_SEEKRAID_DIE = 0x00000001,
		STATE_CAPTURE_RAID_FLAG = 0x00000002,
	};  

	//碰撞战场相关的STATE
	enum
	{
		STATE_COLLISION_ASHILL	= 0x00000001,	//不动如山
		STATE_COLLISION_NOMOVE	= 0x00000002,	//定身
		STATE_COLLISION_CHAOS	= 0x00000004,	//混乱
		STATE_COLLISION_VOID    = 0x00000008,	//虚无

	};
	enum
	{
		DLTYPE_LIGHT = 0,
		DLTYPE_DARK,
	};
	
	enum
	{
		SPIRIT_TYPE_NONE = 0,
		SPIRIT_TYPE_DARK,
		SPIRIT_TYPE_LIGHT,
		SPIRIT_TYPE_DARKLIGHT,
		SPIRIT_TYPE_NUM,
	};

	inline void SetExtraState(int state)
	{
		extra_state |= state;
	}

	inline void ClrExtraState(int state)
	{
		extra_state &= ~state;
	}

	inline bool CheckExtraState(int state) const
	{
		return extra_state & state;
	}

        inline void SetExtraState2(int state)
        {   
                extra_state2 |= state;
        }   

        inline void ClrExtraState2(int state)
        {   
                extra_state2 &= ~state;
        }   

        inline bool CheckExtraState2(int state) const
        {   
                return extra_state2 & state;
        }   
	
	inline bool IsCarrierMode() const
	{
		return CheckExtraState(STATE_CARRIER);
	}

	inline bool IsAnonymousMode() const
	{
		return CheckExtraState(STATE_ANONYMOUS);
	}

	inline bool IsInvisible() const
	{
		return CheckExtraState(STATE_INVISIBLE);
	}

	inline bool IsCloneMode() const
	{
		return CheckExtraState(STATE_CLONE);
	}

	inline bool IsIgniteState() const
	{
		return CheckExtraState(STATE_IGNITE);
	}

	inline bool IsFrozenState() const
	{
		return CheckExtraState(STATE_FROZEN);
	}
	
	inline bool IsColdInjureState() const
	{
		return CheckExtraState(STATE_COLDINJURE);
	}

	inline bool IsFuryState() const
	{
		return CheckExtraState(STATE_FURY);
	}

	inline bool IsTurnBuffState() const
	{
		return CheckExtraState(STATE_TURNBUFF);
	}

	inline bool IsTurnDebuffState() const
	{
		return CheckExtraState(STATE_TURNDEBUFF);
	}

	inline bool IsBloodThirstyState() const
	{
		return CheckExtraState(STATE_BLOODTHIRSTY);
	}

	inline bool IsBePulledState() const
	{
		return CheckExtraState(STATE_BEPULLED);
	}

	inline bool IsBeSpiritDraggedState() const
	{
		return CheckExtraState(STATE_BESPIRITDRAGGED);
	}

	inline bool IsXYDarkLightState() const
	{
		return CheckExtraState(STATE_XY_DARKLIGHT_STATE);
	}
	
	inline bool IsXYSpiritsState() const
	{
		return CheckExtraState(STATE_XY_SPIRITS);
	}
	
	inline bool IsMirrorImageState() const
	{
		return CheckExtraState(STATE_MIRROR_IMAGE);
	}

	inline bool IsFlowBattleState() const
	{
		return CheckExtraState(STATE_FLOW_BATTLE);
	}

	inline bool IsPuppetState() const
	{
		return CheckExtraState(STATE_PUPPET);
	}

	inline bool IsQilinState() const
	{
		return CheckExtraState(STATE_QILIN);
	}

	inline bool IsMobAactiveState() const
	{
		return CheckExtraState(STATE_MOBACTIVE);
	}



public:
	
};

inline bool make_link_sid(gplayer * dest, link_sid & id)
{
	id.cs_id = dest->cs_index;
	id.cs_sid = dest->cs_sid;
	id.user_id = dest->ID.id;
	return id.cs_id >= 0;
}

//副本或者其他世界的key，在进入副本世界时需要此种key
struct  instance_key
{
	struct key_essence
	{
		int key_level1;			//一级key， 有些副本需要这个 标准key是自己的ID
		struct 
		{
			int first;
			int second;
		}key_level2;			//二级key，有些副本需要这个  标准key是自己的队伍ID和seq
		int key_level3;			//三级key，有些副本需要这个  标准key是自己的帮派ID
		int key_level4;			//四级key，有些副本需要这个，这是为专有副本准备的（比如城战）
	}; 
	
	key_essence essence;
	key_essence target;
};

template <typename WRAPPER >
WRAPPER & operator << (WRAPPER & wrapper ,const instance_key & rhs)
{
	return wrapper.push_back(&rhs, sizeof(rhs));
}

template <typename WRAPPER >
WRAPPER & operator >>  (WRAPPER & wrapper ,instance_key & rhs)
{
	return wrapper.pop_back(&rhs, sizeof(rhs));
}

struct instance_hash_key
{
	int key1;
	int key2;
	bool operator ==(const instance_hash_key & rhs) const
	{
		return key1 == rhs.key1 && key2 == rhs.key2;
	}
};

struct instance_hash_function
{
	inline long operator()(const instance_hash_key & key) const
	{
		return key.key1 ^ key.key2;
	}
};

struct player_var_data
{
	enum 
	{
		BASIC_VAR_DATA_SIZE = 16,
		CUR_VERSION	= 1,
	};
	int version;
	int mafia_death;	//帮战被杀次数累计
	char dead_flag;		//是否死亡状态(3种)
	bool resurrect_state;	//复活状态(是否可原地复活)
	float resurrect_exp_reduce; //复活状态时损失的exp（可原地复活后才有效)
	instance_hash_key ins_key;  //上次退出时所在的副本hash key
	int last_instance_timestamp;//最后一次进入副本的时间戳
	int last_instance_tag;	    //最后一次进入副本的ID
	A3DVECTOR last_instance_pos;//最后一次进入副本的坐标
	int dir;		    //玩家下线时的方向

	template <typename PLAYER_IMP>
	void MakeData(gplayer * pPlayer, PLAYER_IMP * pImp)
	{
		version = CUR_VERSION;
		mafia_death = pImp->GetMafiaDuelDeaths();
		dead_flag =  pImp->GetDeadFlag();
		float hp_recover = 0.1f;	//这个值就不存盘了
		resurrect_state = pImp->GetResurrectState(resurrect_exp_reduce, hp_recover);
		ins_key = pImp->_plane->w_ins_key;
		pImp->GetLastInstancePos( last_instance_tag, last_instance_pos,last_instance_timestamp);
		dir = pPlayer->dir;

	}

	template <typename PLAYER_IMP>
	static void SetData(gplayer * pPlayer, PLAYER_IMP * pImp, const void * buf, size_t size)
	{
		player_var_data * pVar = (player_var_data*)buf;
		switch(pVar->version)
		{
			case CUR_VERSION:
				{
					pImp->SetDeadFlag(pVar->dead_flag);
					pImp->SetResurrectState(pVar->resurrect_state,pVar->resurrect_exp_reduce);
					pPlayer->dir = pVar->dir;
					pImp->SetMafiaDuelDeaths(pVar->mafia_death);
				}
				break;
		}
	}

	template <typename INSTANCE_KEY>
	static void GetInstanceKey(const void * buf, size_t size,INSTANCE_KEY & key)
	{
		memset(&key,0,sizeof(key));
		if(size < sizeof(int)) 
		{
			key.key1 = 0;
			key.key2 = time(NULL);
			return;
		}
		player_var_data * pVar = (player_var_data*)buf;
		switch(pVar->version)
		{
			case CUR_VERSION:
				{
					key = pVar->ins_key;
				}
				return ;
		}
		key.key1 = 0;
		key.key2 = time(NULL);
		return;
	}

	template <int >
	static void GetLastInstance(const void * buf, size_t size,int &last_ins_tag, A3DVECTOR & pos,int & create_ts)
	{
		if(size < sizeof(int)) 
		{
			last_ins_tag = -1;
			create_ts = -1;
			return;
		}
		player_var_data * pVar = (player_var_data*)buf;
		switch(pVar->version)
		{
			case CUR_VERSION:
				{
					last_ins_tag = pVar->last_instance_tag;
					pos = pVar->last_instance_pos;
					create_ts = pVar->last_instance_timestamp;
				}
				return ;
		}
		last_ins_tag = -1;
		create_ts = -1;
		return;
	}

	enum 
	{
		ALIVE 	= 0,
		KILLED_BY_PLAYER = 1,
		KILLED_BY_NPC = 2,
	};

};
enum
{
	PLAYER_MONSTER_TYPE = 256
};

#endif

