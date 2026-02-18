#include <octets.h> 
#include <set>

namespace GNET
{
// Youshuang add for new equip essence
enum
{
	EQUIP_ESSENCE_VERSION_OLD = 0,
	EQUIP_ESSENCE_VERSION_SPIRIT = 1,
	EQUIP_ESSENCE_VERSION_GEM = 2,
	EQUIP_ESSENCE_VERSION_ASTROLOGY = 3,
	MAX_ENCHANT_SLOT = 5,
	MAX_GEM_SLOT = 3,
};

#pragma pack(1)
struct astrology_prop
{
        int id; 
        int value;
};

struct astrology_slot 
{
        int astrology_id;
        int level;
        int score;
        int energy_consume;
        astrology_prop prop[6];
        bool is_lock;                   //保留星座的锁定信息
        int reserver1;
        int reserver2;
};

struct equip_essence_new
{
	int version;
	
	// old essence
	int rf_stone;
	short rf_stone_level;
	int pr_stone;
	short pr_stone_level;
	int sk_stone;
	unsigned int sk_stone_level; 

	// add spirit
	int master_id;				//主人ID
	char master_name[20];			//主人名称
	int reserved1;
	int reserved2;
	int cur_spirit_power;			//当前魄力
	int max_spirit_power;			//魄力上限
	int soul_used_spirit;			//器魄已经占用的魂力
	int soul[MAX_ENCHANT_SLOT];		//金木水火土5个器魄
	int soul_rune_word;			//器魄组合属性的CRC，只有非0才有效

	// add gem
	char gem_slot_shape[MAX_GEM_SLOT]; 	//装备宝石槽形状, -1为该槽还没有被鉴定
	int gem_embeded[MAX_GEM_SLOT];		//已经镶嵌的宝石，-1表示该槽还没有镶嵌宝石
	int colorant_id;
	int reserved3;
	int reserved4;
	
	// add astrology
	int cur_hole;				//当前的孔数
	int reserved5;
	int reserved6;
	astrology_slot hole[3];

	int reserved[32];
};
#pragma pack()

//Note: 使用这个以前应该确保gs中关于下面字段的定义没有改变
#define TALISMAN_ESSENCE_SIZE 92		//法宝装备结构大小
#define PETBEDGE_OLD_ESSENCE_SIZE 236 		//老宠物数据结构
#define PETBEDGE_ESSENCE_SIZE 450 		//新宠物数据结构
#define EQUIP_OLD_ESSENCE_SIZE 12		//基本的装备结构大小
#define EQUIP_SPIRIT_ESSENCE_SIZE 80		//带血祭版本的装备结构大小
#define EQUIP_GEM_ESSENCE_SIZE 107		//宝石版本的装备结构大小
#define EQUIP_ASTROLOGY_ESSENCE_SIZE 335 	//星座版本的装备结构大小
#define TRANS_STONE_ESSENCE_SIZE 128 		//幻灵石(幻灵石装备结构大小)
#define ITEM_BASE_SIZE 9			//item结构的基本大小(9个int)
#define ITEM_UNLOCKING_ID 21119			//解锁种的物品
#define ITEM_DESTROY_ID	4544			//摧毁中的物品
#define ITEM_BROKEN_ID	21695			//死亡破碎物品	
	
// 等同于Gs中的Item.h的枚举
enum
{
	EQUIP_INDEX_WEAPON		= 0,
	EQUIP_INDEX_HEAD		= 1,
	EQUIP_INDEX_BODY		= 2,
	EQUIP_INDEX_FOOT		= 3,
	EQUIP_INDEX_NECK		= 4,
	EQUIP_INDEX_FINGER1		= 5,
	EQUIP_INDEX_FINGER2		= 6,
	EQUIP_INDEX_FASHION_EYE		= 7,	
	EQUIP_INDEX_MAGIC		= 8, // 幻灵石
	EQUIP_INDEX_FASHION_LIP		= 9, // 法身
	EQUIP_INDEX_FASHION_HEAD	= 10,
	EQUIP_INDEX_FASHION_BODY	= 11,
	EQUIP_INDEX_FASHION_LEG		= 12,
	EQUIP_INDEX_FASHION_FOOT	= 13,
	EQUIP_INDEX_FASHION_BACK	= 14,		// 护符
	EQUIP_INDEX_WING		= 15,
	EQUIP_INDEX_TALISMAN1		= 16,		// 法宝
	EQUIP_INDEX_TALISMAN2		= 17,
	EQUIP_INDEX_TALISMAN3		= 18,
	EQUIP_INDEX_BUGLE		= 19,		// 嘻哈猴
	EQUIP_INDEX_XITEM1		= 20,		//锦囊
	EQUIP_INDEX_XITEM2		= 21,		//玺绶
	EQUIP_INDEX_XITEM3		= 22,		//罡气
	EQUIP_INDEX_RUNE		= 23,		//元魂
	EQUIP_INDEX_XITEM4		= 24,
	EQUIP_INDEX_XITEM5		= 25,
	EQUIP_INDEX_FASHION_WEAPON	= 26,		// Youshuang add for fashion weapon
	EQUIP_INVENTORY_COUNT,

	EQUIP_VISUAL_START	= EQUIP_INDEX_WEAPON,
	//EQUIP_VISUAL_END	= EQUIP_INDEX_RUNE + 1, 
	EQUIP_VISUAL_END	= EQUIP_INDEX_FASHION_WEAPON + 1,  // Youshuang add

	EQUIP_MASK_ALL  	= 0xFFFFFF,
};

struct SEquipUnit
{
	unsigned int id;
	short rf_stone_level;
};

struct SEquipsRole
{
	SEquipUnit equip_head; // 帽子
	SEquipUnit equip_body; // 衣服
	SEquipUnit equip_foot; // 鞋子
	SEquipUnit equip_weapon; // 武器

	unsigned int talisman1_id; 	// 法宝
	unsigned int neck_id; // 项链
	unsigned int finger1_id; //	左戒指
	unsigned int finger2_id; //	右戒指
	unsigned int xitem3_id; //	罡气
	unsigned int back_id;//	护符
	unsigned int xitem2_id; //	玺绶
	unsigned int xitem1_id; //	锦囊
	unsigned int lip_id;//	金身/法身
/*
	unsigned int eye_id;
	unsigned int lip_id;
	unsigned int xitem4_id;
	unsigned int xitem5_id;
*/
};

//typedef std::map<int /*pos*/, SEquipRoleScan> TEquipRoleScanMap;

enum ITEM_TYPE{ITEMTYPE_EQUIPMENT=0, ITEMTYPE_TALISMAN, ITEMTYPE_PETBADGE, ITEMTYPE_TRANS_STONE, ITEMTYPE_DELETE, ITEMTYPE_MAX};

int GetOwnerID(int item_type, const Octets & item_data);
bool SetOwnerID(int item_type, Octets & item_data, int owner_id);
bool SetOwnerName(int item_type, Octets & item_data, Octets & owner_name);
int GetSpecialItemID(int item_id, const Octets & item_data);
int GetSpecialItemOwnerID(int item_id, int special_item_type, const Octets & item_data);
bool SetSpecialItemOwnerName(int item_id, int special_item_type, Octets & item_data, Octets & owner_name);
bool SetSpecialItemOwnerID(int item_id, int special_item_type, Octets & item_data, int owner_id);
bool TestTalismanData(const Octets & item_data);
bool TestPetbedgeData(const Octets & item_data, char & data_type);
bool TestTransStoneData(const Octets & item_data);

class ItemIdMan
{
private:
	std::set<int> m_setItemId[ITEMTYPE_MAX];
public:
	bool LoadItemId(const char *filename);
	bool LoadItemId();
	int GetItemType(int itemid)
	{
		int item_type = -1;
		for (int i = 0; i < ITEMTYPE_MAX; ++i)
		{
			if (m_setItemId[i].find(itemid) != m_setItemId[i].end())
			{
				item_type = i;
				break;
			}
		}
		return item_type;
	}
};
};
