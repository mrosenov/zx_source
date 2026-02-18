#ifndef __ONLINEGAME_GS_OBJECT_H__
#define __ONLINEGAME_GS_OBJECT_H__

#include <spinlock.h>
#include <common/types.h>
#include <common/packetwrapper.h>
#include <ASSERT.h>

#include "config.h"


class world;
class gobject_imp;
struct slice;
struct gobject
{
	enum 
	{
		MSG_MASK_PLAYER_MOVE  	= 0x0001,
//		MSG_MASK_NPC_MOVE	= 0x0002,
//		MSG_MASK_MATTER_DROP	= 0x0004,
//		MSG_MASK_PLAYER_DEATH	= 0x0008,
//		MSG_MASK_ACTIVE		= 0x0010,
		MSG_MASK_ATTACK		= 0x0020,
		MSG_MASK_ENCHANT	= 0x0040,
		MSG_MASK_CRY_FOR_HELP	= 0x0080,
	};

	int	spinlock;	//自旋锁
#ifdef _DEBUG
	int 	cur_msg;	//最近一条执行的消息号
#endif
	unsigned int crc;	//可能的crc校验值
	bool 	b_chk_state;	//表示该对象是否使用中的检查锁
	bool 	b_zombie;	//是否僵尸状态
	bool	b_disconnect;	//是否已经断线，只有player使用
	unsigned char dir;	//对象所处的方向
	int	msg_mask;	//接收消息的标志
	gobject * pPrev;	//前一个指针
	gobject * pNext;	//后一个指针
	slice 	* pPiece;	//所属的块指针
	int	collision_id;	//碰撞标志位
	int	tag;		//对象所处世界ID
	int	w_serial;	//对象所处的世界序列号
	world   * plane;	//对象所处的世界指针
	A3DVECTOR pos;		//对象所处的位置
	XID 	ID;		//id,对于用户，使用uid,对于其他物品和NPC，使用 2位标志 | 14位 world_index | 16位索引
	gobject_imp * imp;	//实际的具体实现
	float	body_size;	//对象的大小尺度 
public:
	inline void Lock() {mutex_spinlock(&spinlock);}
	inline void Unlock() {mutex_spinunlock(&spinlock);}
	inline int  TryLock() { return mutex_spinset(&spinlock);}
	inline bool IsEmpty() { return !b_chk_state;}
	inline bool IsActived() { return b_chk_state;}
	inline bool IsZombie() { return b_zombie;}
	inline void SetActive() { b_chk_state = true;}
	inline void ClrActive() { b_chk_state = false;}
	inline void Clear()
	{
		crc = 0;
		b_chk_state = false; 
		b_zombie = false;
		dir = 0;
		msg_mask = 0;
		ID.id = -1;
		ID.type = -1;
		imp = NULL;
		body_size = 0.f;
		b_disconnect = false;
		collision_id = 0;
	}

	inline void SetWorld(int world_tag, int world_serial)
	{
		ASSERT(spinlock);
		tag		= world_tag;
		w_serial 	= world_serial;
	}
};

struct object_base_info
{
	short cls;			//职业
	short gender;			//性别
	int faction;			//派系
	int level;			//级别
	int hp;				//hp
	int max_hp;			//mp
	int mp;				//法术 

	static bool CheckCls(int c, int64_t mask1, int64_t mask2)
	{
		int cc = c & 0x7F;
		return (cc <= 0x3F) ? (((int64_t)1 << cc) & mask1) : (((int64_t)1 << (cc - 0x40)) & mask2);
	}
};


struct gactive_object : public gobject
{
	object_base_info base_info;
	unsigned int object_state;	//这个表示对象的状态
	unsigned char shape_form;	//变身类型
	unsigned char emote_form;	//表情类型
	unsigned char effect_count;
	unsigned char npc_reborn_flag;	//对NPC而言，代表重生时需要重新生成
	int64_t extend_state;
	int 	invisible_rate;		//隐身能力
	int 	anti_invisible_rate;	//反隐身能力

	int extend_dir_state;			//带面向的玩家效果, 最多32个
	short extend_dir_state_dirs[32];	//带面向的玩家效果的方向，最多32个

	enum 
	{
		MALE = 0,
		FEMALE = 1,
	};

	template<typename WRAPPER>
	WRAPPER & Export(WRAPPER & wrapper)
	{
		wrapper << object_state << shape_form << effect_count << extend_state << invisible_rate << anti_invisible_rate << extend_dir_state;
		wrapper.push_back(extend_dir_state_dirs, sizeof(extend_dir_state_dirs));
		return wrapper;
	}

	template<typename WRAPPER>
	WRAPPER & Import(WRAPPER & wrapper)
	{
		wrapper >> object_state >> shape_form >> effect_count >> extend_state << invisible_rate << anti_invisible_rate;
		wrapper.pop_back(extend_dir_state_dirs, sizeof(extend_dir_state_dirs));
		return wrapper;
	}

	void Clear()
	{
		object_state = 0;
		shape_form = 0;
		emote_form = 0;//移动的时候不用存盘，因为此时不会有相应的表情
		effect_count = 0;
		extend_state = 0;;
		base_info.faction = 0;
		invisible_rate = 0;
		anti_invisible_rate = 0;
		extend_dir_state = 0;
		memset(extend_dir_state_dirs, 0, sizeof(extend_dir_state_dirs));
		gobject::Clear();
	}
	enum		//这里的枚举量是放到object_state上的
	{	
	//考虑可以将player专属内容放到和NPC专属内容平行的位置上
		STATE_SHAPE		= 0x00000001,   //是否变身状态
		STATE_EMOTE		= 0x00000002,   //是否正在做表情
		STATE_INVADER 		= 0x00000004,   //是否粉名
		STATE_FLY 		= 0x00000008,   //是否飞行
		STATE_FAMILY		= 0x00000010,   //是否家族
		STATE_SITDOWN		= 0x00000020,   //是否坐下
		STATE_EXTEND_PROPERTY	= 0x00000040,   //是否有扩展状态
		STATE_ZOMBIE		= 0x00000080,	//是否尸体

		STATE_TEAM		= 0x00000100,   //是否队员
		STATE_TEAMLEADER	= 0x00000200,   //是否队长
		STATE_ADV_MODE		= 0x00000400,   //是否有广告内容
		STATE_MAFIA		= 0x00000800,   //是否帮派成员
		STATE_MARKET		= 0x00001000,	//是否正在摆摊
		STATE_FASHION_MODE	= 0x00002000,	//是否时装模式
		STATE_GAMEMASTER	= 0x00004000,	//后面存在着GM特殊状态
		STATE_PVP_NOPROTECTED	= 0x00008000,	//是否不再进行PVP保护

		STATE_EFFECT		= 0x00010000,	//是否有特殊效果
		STATE_IN_PVP_COMBAT	= 0x00020000,	//是否正在PVP中
		STATE_IN_DUEL_MODE	= 0x00040000,	//是否正在决斗中
		STATE_MOUNT		= 0x00080000,	//正在骑乘中
		STATE_IN_BIND		= 0x00100000,	//和别人绑在一起
		STATE_BATTLE_OFFENSE	= 0x00200000,	//战争攻击方
		STATE_BATTLE_DEFENCE	= 0x00400000,	//战争防守方
		STATE_TITLE		= 0x00800000,	//具有称谓

		STATE_SPOUSE		= 0x01000000,	//具有配偶
		STATE_PET_COMBINE1	= 0x02000000,	//宠物通灵
		STATE_PET_COMBINE2	= 0x04000000,	//宠物御宝
		STATE_SECT_MASTER_ID	= 0x08000000,	//具有师傅
		STATE_IN_KILLINGFIELD	= 0x10000000,	//是否在杀戮战场里面
		STATE_IN_VIP		= 0x20000000,	//处于VIP状态
		STATE_BATTLE_VISITOR	= 0x40000000,	//战场观战方
		STATE_TRANSFORM		= 0x80000000,	//是否处于变身状态

		STATE_STATE_CORPSE	= 0x00000008,	//NPC是否出于尸体残留，这个和ZOMBIE不一样，非尸体残留的也可能ZOMBIE
		STATE_NPC_ADDON1	= 0x00000100,	//NPC附加属性位1 
		STATE_NPC_ADDON2	= 0x00000200,	//NPC附加属性位2 
		STATE_NPC_ADDON3	= 0x00000400,	//NPC附加属性位3 
		STATE_NPC_ADDON4	= 0x00000800,	//NPC附加属性位4 
		STATE_NPC_ALLADDON	= 0x00000F00,	//NPC附加属性位 
		STATE_NPC_PET		= 0x00001000,	//NPC是一个PET，后面跟随PET的主人ID
		STATE_NPC_NAME		= 0x00002000,	//NPC有独特的名字，后面跟随一字节char和名字内容
		STATE_NPC_SUMMON	= 0x00004000,	//NPC召唤怪,后面跟主人的ID
		STATE_NPC_OWNER		= 0x00008000,	//NPC有归属
		STATE_NPC_CLONE		= 0x00010000,	//NPC是分身
		STATE_NPC_DIM		= 0x00020000,	//NPC朦胧状态
		STATE_NPC_FROZEN	= 0x00040000,	//NPC冰冻状态
		STATE_DIR_EXT_STATE 	= 0x00080000,	//NPC带方向的扩展状态
		STATE_NPC_INVISIBLE	= 0x00100000,	//NPC隐身
		STATE_NPC_TELEPORT1	= 0x00200000,	//传送类NPC(祝福)
		STATE_NPC_TELEPORT2	= 0x00400000,	//传送类NPC(诅咒)
		STATE_PHASE		= 0x00800000,	//相位
	};
	
	inline bool IsFemale() const
	{
		return base_info.gender == FEMALE;
	}

	inline short GetClass() const 
	{
		return base_info.cls;
	}

	inline void SetClassGender(short cls, bool is_female)
	{
		base_info.cls = cls;
		base_info.gender = is_female?FEMALE:MALE;
	}

	inline void SetObjectState(int state)
	{
		object_state |= state;
	}

	inline void ClrObjectState(int state)
	{
		object_state &= ~state;
	}

	inline bool CheckObjectState(int state) const
	{
		return object_state & state;
	}

	inline bool IsDuelMode() const
	{
		return CheckObjectState(STATE_IN_DUEL_MODE);
	}

	inline bool IsMountMode() const
	{
		return CheckObjectState(STATE_MOUNT);
	}

	inline bool IsFlyMode() const
	{
		return CheckObjectState(STATE_FLY);
	}

	inline void SetBattleOffense() 
	{
		ClrObjectState(STATE_BATTLE_DEFENCE);
		SetObjectState(STATE_BATTLE_OFFENSE);
	}

	inline void SetBattleDefence()
	{
		ClrObjectState(STATE_BATTLE_OFFENSE);
		SetObjectState(STATE_BATTLE_DEFENCE);
	}

	inline void ClrBattleMode()
	{
		ClrObjectState(STATE_BATTLE_OFFENSE | STATE_BATTLE_DEFENCE);
	}

	inline bool IsBattleOffense() const
	{
		return CheckObjectState(STATE_BATTLE_OFFENSE);
	}

	inline bool IsBattleDefence() const
	{
		return CheckObjectState(STATE_BATTLE_DEFENCE);
	}

	inline bool IsBattleVisitor() const
	{
		return CheckObjectState(STATE_BATTLE_VISITOR);
	}

	inline bool IsVIPState() const
	{
		return CheckObjectState(STATE_IN_VIP);
	}
	inline bool IsTransformMode() const
	{
		return CheckObjectState(STATE_TRANSFORM);
	}


};

struct gowner
{
	enum _owner_type
	{
		OWNER_NONE,		//无
		OWNER_SELF,		//自己
		OWNER_MASTER,		//师傅
		OWNER_APPRENTICE,	//徒弟
		OWNER_SPOUSE,		//夫妻
		OWNER_TEAM,		//队伍
		OWNER_TEAM_OR_SELF,	//队伍或自己
		OWNER_TEAM_AND_SELF,	//队伍和自己
		OWNER_ANY,		//怪的名字属于刷怪的人，但是没有特殊的攻击逻辑
		OWNER_FAMILY,		//家族 (保留)
		OWNER_MAFIA,		//帮派 (保留)
	};

	gowner()
	{
		_owner_type = OWNER_NONE;
		_owner_id1 = 0;	//roleid
		_owner_id2 = 0;  //team id, master id, family id, mafia id
	}

	void Clear()
	{
		_owner_type = OWNER_NONE;
		_owner_id1 = 0;	//roleid
		_owner_id2 = 0;  //team id, master id, family id, mafia id
	}

	char GetOwnerType()
	{
		return _owner_type;
	}

	void SetOwnerType(char type)
	{
		_owner_type = type;
	}

	void SetOwnerID(int id1, int id2)
	{
		_owner_id1 = id1;
		_owner_id2 = id2;
	}

	bool IsOwner(int roleid, int team_id, int master_id)
	{
		switch(_owner_type)
		{
			case OWNER_NONE:
				return true;

			case OWNER_SELF:
			case OWNER_MASTER:
			case OWNER_SPOUSE:
			{
				return _owner_id1 == roleid; 
			}
			break;

			case OWNER_APPRENTICE:
			{
				return _owner_id2 == master_id;
			}
			break;

			case OWNER_TEAM:
			{
				return _owner_id2 == team_id;
			}
			break;

			case OWNER_TEAM_OR_SELF:
			{
				return (_owner_id1 == roleid || _owner_id2 == team_id) ;
			}
			break;

			case OWNER_TEAM_AND_SELF:
		      	{
				return (_owner_id1 == roleid && _owner_id2 == team_id);
			}	
			break;

			case OWNER_ANY:
			{
				return true;
			}
			break;
		}
		return false;
	}

	int GetNameID()
	{
		switch(_owner_type)
		{
			case OWNER_SELF:
				return _owner_id1;
			case OWNER_MASTER:
				return _owner_id1;
			case OWNER_APPRENTICE:
				return _owner_id2;
			case OWNER_SPOUSE:
				return _owner_id1;
			case OWNER_TEAM:
			case OWNER_TEAM_AND_SELF:
				return _owner_id2;
			case OWNER_TEAM_OR_SELF:
				return (_owner_id2 !=0 ) ?  _owner_id2 : _owner_id1;
			case OWNER_ANY:
				return _owner_id1;
			default:
				return 0;

		}
		return 0;
	}

	char _owner_type;
	int _owner_id1;
	int _owner_id2;
};

struct gnpc: public gactive_object
{
	size_t spawn_index;
	int native_state;
	int tid;		//实际的tid
	int vis_tid;		//可见的tid
	int monster_faction;	//怪物的小类
	int cruise_timer;	//闲逛时的计数器
	int idle_timer;
	int idle_timer_count;	//用于在idle状态计时
	int master_id;		//主人ID，只有处于PET状态的NPC才会有此状态
	unsigned char pet_star; //宠物的星级信息
	unsigned char pet_shape;//宠物的模型
	unsigned char pet_face; //宠物脸型
	unsigned char teleport_count; //萝莉传送机关的数量(用于判断是否可传送)
	int npc_idle_hearbeat;	//npcidle时的心跳控制 若此值非0 则用之
	short name_size;	//NPC的自动一名字长度，只有设置了名字标志的npc此状态才有效 
	char npc_name[20];	//NPC的自定义名字，只有设置了名字标志的npc此状态才有效
	unsigned short phase_id;
	gowner owner;

	enum
	{
		TYPE_NORMAL ,
		TYPE_NATIVE ,
		TYPE_EXTERN_NATIVE,
		TYPE_FREE,
	};
	inline gnpc* get_next() { return (gnpc*)pNext;}
	inline gnpc* get_prev() { return (gnpc*)pPrev;}
	void Clear()
	{
		tid = 0;
		vis_tid = 0;
		master_id = 0;
		pet_star = 0;
		name_size = 0;
		native_state = TYPE_NORMAL;
		npc_idle_hearbeat = 0;
		phase_id = 0;
		teleport_count = 0;
		owner.Clear();
		gactive_object::Clear();
	}
	bool IsNative()
	{
		return native_state == TYPE_NATIVE;
	}
	
	void SetOwnerType(char type)
	{
		owner.SetOwnerType(type);
	}
	
	void SetOwnerID(int owner_id1, int owner_id2)
	{
		owner.SetOwnerID(owner_id1, owner_id2);
	}
	
	bool IsNPCOwner(int roleid, int team_id, int master_id)
	{
		return owner.IsOwner(roleid, team_id, master_id);
	}
	
	template<typename WRAPPER>
	WRAPPER & Import(WRAPPER & wrapper)
	{
		gactive_object::Import(wrapper); 
		wrapper >> ID >> pos >> msg_mask >> tid >> vis_tid ;
		wrapper.pop_back(&base_info,sizeof(base_info));
		wrapper >> monster_faction >> body_size >> master_id >> pet_star >> pet_shape >> pet_face >> teleport_count >> name_size;
		return wrapper.pop_back(npc_name,sizeof(npc_name));
	}
	
	template<typename WRAPPER>
	WRAPPER & Export(WRAPPER & wrapper)
	{
		gactive_object::Export(wrapper); 
		wrapper << ID << pos << msg_mask << tid ;
		wrapper.push_back(&base_info,sizeof(base_info));
		wrapper << monster_faction << body_size << master_id << pet_star << pet_shape << pet_face << teleport_count << name_size;
		return wrapper.push_back(npc_name,sizeof(npc_name));
	}
};

struct gmatter : public gobject
{
	unsigned char dir1;
	unsigned char rad;
	unsigned char matter_state;	
	unsigned char battle_flag;				//0 中立 1 攻方  2 守方
	unsigned short phase_id;
	int combine_state;  // Youshuang add
	gowner owner;

	enum 
	{
		BATTLE_FLAG_NEUTRAL,
		BATTLE_FLAG_ATTACKER,
		BATTLE_FLAG_DEFENDER
	};
	enum
	{
		STATE_MASK_NORMAL_OBJECT	= 0x00,		//普通物品 矿物 
		STATE_MASK_DYN_OBJECT 		= 0x01,		//表示本物品为动态物品，其ID应从动态ID中寻找
		STATE_MASK_BATTLE_FLAG		= 0x02,		//表示本物品为战场争夺物
		STATE_MASK_OWNER_MATTER		= 0x04,
		STATE_MASK_PHASE		= 0x10,		//相位
		STATE_MASK_COMBINE_MINE		= 0x20,		// combine mine

		BATTLE_FLAG_WHITE		= 0,
		BATTLE_FLAG_RED			= 0,
		BATTLE_FLAG_BLUE		= 0,
	};
	int matter_type;
	int spawn_index;		//只对矿产有用
	int name_id;			//名字归属ID

	inline gmatter* get_next() { return (gmatter*)pNext;}
	inline gmatter* get_prev() { return (gmatter*)pPrev;}

	inline bool IsAttackerFlag()
	{
		return battle_flag == BATTLE_FLAG_ATTACKER;
	}

	inline bool IsDefenderFlag()
	{
		return battle_flag == BATTLE_FLAG_DEFENDER;
	}

	inline bool IsNeutralFlag()
	{
		return battle_flag == BATTLE_FLAG_NEUTRAL;
	}
	void SetDirUp(unsigned char d, unsigned char d1, unsigned char r)
	{
		dir = d;
		dir1 = d1;
		rad = r;
	}
	inline void Clear()
	{
		gobject::Clear();
		dir1 = rad = matter_state = battle_flag = 0;
		owner.Clear();
	}
	
	void SetOwnerType(char type)
	{
		owner.SetOwnerType(type);
	}
	
	void SetOwnerID(int owner_id1, int owner_id2)
	{
		owner.SetOwnerID(owner_id1, owner_id2);
	}
	
	bool IsMineOwner(int roleid, int master_id, int team_id)
	{
		return owner.IsOwner(roleid, master_id, team_id);
	}
};

#endif

