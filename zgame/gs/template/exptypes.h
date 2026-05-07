#ifndef _EXP_TYPES_H_
#define _EXP_TYPES_H_

#include <stdio.h>
#include <stdlib.h>

#ifdef _WINDOWS
#include <memory.h>
#endif

#define ELEMENTDATA_VERSION			0x100000a5

#define SPECIAL_ID_CONFIG_TEMPLID	7	// 689

#ifdef LINUX
#define UInt64  unsigned long long
#else
#define UInt64  unsigned __int64
#endif

enum SERVICE_TYPE
{
	// ��̸����
	NPC_TALK = 0x80000000,
	// ������Ʒ�ķ���
	NPC_SELL,
	// �չ���Ʒ�ķ���
	NPC_BUY,
	// ������Ʒ�ķ���
	NPC_REPAIR,
	// ��Ƕ����
	NPC_INSTALL,
	// �������
	NPC_UNINSTALL,
	// ������ط���,�ַ��������������Լ�����������Ʒ
	NPC_GIVE_TASK,
	NPC_COMPLETE_TASK,
	NPC_GIVE_TASK_MATTER,
	// ������ط���
	NPC_SKILL,
	// ���Ʒ���
	NPC_HEAL,
	// ���ͷ���
	NPC_TRANSMIT,
	// �������
	NPC_TRANSPORT,
	// ���۷���
	NPC_PROXY,
	// �洢��Ʒ����Ǯ
	NPC_STORAGE,
	// ��������
	NPC_MAKE,
	// �ֽ����
	NPC_DECOMPOSE,
	// TALK����
	TALK_RETURN,
	// �����Ի�
	TALK_EXIT,
	// �ֿ�����
	NPC_STORAGE_PASSWORD,
	// ��������
	NPC_IDENTIFY,
	// ��������
	TALK_GIVEUP_TASK,
	// ��ս�����������
	NPC_WAR_TOWERBUILD,
	// ϴ�����
	NPC_RESETPROP,
	// װ���󶨷���
	NPC_EQUIPBIND,
	// װ�����ٷ���
	NPC_EQUIPDESTROY,
	// װ��������ٷ���
	NPC_EQUIPUNDESTROY,
	// ��ս���򹭼��ַ���
	NPC_WAR_BUY_ARCHER,
	// ���ｻ��
	ITEM_TRADE_SERVICE,
	// �����ں�
	EQUIP_SOUL_MELD,
	// ��Ʒ���۷���
	NPC_CONSIGN,
	// ��������
	NPC_TRANSCRIPTION,
};

/* ����index
*/
enum PlayerPropType
{	
	PROP_TYPE_DAMAGE,
	PROP_TYPE_DEF,
	PROP_TYPE_HP,
	PROP_TYPE_MP,
	PROP_TYPE_ATTACK,			// ����
	PROP_TYPE_ARMOR,			// ����
	PROP_TYPE_CRIT_RATE,		// ���� 
	PROP_TYPE_CRIT_ANTI,		// ������
	PROP_TYPE_CRIT_DAMAGE,		// ����
	PROP_TYPE_CRIT_DAMAGE_ANTI,	// ������
	PROP_TYPE_SKILL_ATTACK_RATE,// ��������
	PROP_TYPE_SKILL_ARMOR_RATE,	// ���ܶ���
	PROP_TYPE_RESISTANCE_1,		// ����
	PROP_TYPE_RESISTANCE_2,
	PROP_TYPE_RESISTANCE_3,
	PROP_TYPE_RESISTANCE_4,
	PROP_TYPE_RESISTANCE_5,
	PROP_TYPE_CULT_DEFANCE_1,
	PROP_TYPE_CULT_DEFANCE_2,	
	PROP_TYPE_CULT_DEFANCE_3,
	PROP_TYPE_CULT_ATTACK_1,
	PROP_TYPE_CULT_ATTACK_2,
	PROP_TYPE_CULT_ATTACK_3,
	PROP_TYPE_SKILLED_1,		// ��ͨ
	PROP_TYPE_SKILLED_2,
	PROP_TYPE_SKILLED_3,
	PROP_TYPE_SKILLED_4,
	PROP_TYPE_SKILLED_5,
	PROP_TYPE_TOUGHNESS_1,		// ����
	PROP_TYPE_TOUGHNESS_2,
	PROP_TYPE_TOUGHNESS_3,
	PROP_TYPE_TOUGHNESS_4,
	PROP_TYPE_TOUGHNESS_5,
	PROP_TYPE_MOVE_SPEED,		// �����ƶ��ٶ�
	PROP_TYPE_FLY_SPEED,		// �����ٶ�
	PROP_TYPE_NUM = 40,
};

#pragma pack(push, EXP_TYPES_INC, 4)

#ifdef _ANGELICA22
typedef wchar_t					namechar;
#else
typedef unsigned short			namechar;
#endif // #ifdef _ANGELICA22

///////////////////////////////////////////////////////////////////////////////////////
//
// װ����ģ����Ҫʹ�õĸ������Ե����ݽṹ����
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
struct EQUIPMENT_ADDON
{
	unsigned int	id;							// �������Ե�ID����������ƷʱҪ���14��13λ��ʾ��
												// �������ԵĲ�����������12λ����ռ��
												// ���������ڲ���IDʱ������ʹ��12��13��14����λ

	namechar		name[32];					// �������Ե�����

	int				type;						// �������Ե�����

	int				num_params;					// ���������ԵĲ�������
	int				param1;						// ��1�������������Ǹ�����
	int				param2;						// ��2�������������Ǹ�����
	int				param3;						// ��3�������������Ǹ�����

};


///////////////////////////////////////////////////////////////////////////////////////
//
// װ��ģ�����ݽṹ����
//
//
///////////////////////////////////////////////////////////////////////////////////////

// װ�������
struct EQUIPMENT_MAJOR_TYPE
{
	unsigned int	id;							// װ�������ID
	namechar		name[32];					// װ�����������
};

// װ��С���
struct EQUIPMENT_SUB_TYPE
{
	unsigned int	id;							// װ��С���ID
	namechar		name[32];					// װ��С�������
};

struct EQUIPMENT_ESSENCE
{
	unsigned int	id;
	unsigned int	id_major_type;
	unsigned int	id_sub_type;
	namechar		name[32];
	unsigned int	equip_type;
	unsigned int	equip_mask;
	int				file_model_male;
	int				file_model_female;
	int				file_model_male_armor_xuanyuan;
	int				file_model_female_armor_xuanyuan;
	int				file_model_big_male;
	int				file_model_big_female;
	int				file_model_small_male;
	int				file_model_small_female;
	int				file_model_tiny_male;
	int				file_model_tiny_female;
	int				file_model_tetrapod_male;
	int				file_model_big_male_left;
	int				file_model_big_female_left;
	int				file_model_back_male;
	int				file_model_back_female;
	int				file_model_female_taihao;
	int				file_model_back_female_fash;
	int				file_model_female_taihao_fash;
	int				file_model_big_male_sword;
	int				file_model_big_female_sword;
	int				file_model_big_male_scythe;
	int				file_model_big_female_scythe;
	int				file_model_big_male_bow;
	int				file_model_big_female_bow;
	int				file_model_reserved1;
	int				file_model_reserved2;
	int				file_model_reserved3;
	int				file_model_reserved4;
	int				file_model_reserved5;
	int				file_model_reserved6;
	int				file_model_reserved7;
	int				file_model_reserved8;
	int				file_model_reserved9;
	int				file_model_reserved10;
	int				file_model_reserved11;
	int				file_model_reserved12;
	int				file_matter;
	int				file_icon;
	unsigned int	equip_location;
	unsigned int	action_type;
	namechar		show_level[16];
	int				level;
	int				grade;
	int				unknown_1;
	int				unknown_2;
	int				unknown_3;
	UInt64			character_combo_id;
	UInt64			character_combo_id2;
	UInt64			char_lev_1;
	UInt64			char_lev_1_2;
	UInt64			char_lev_2;
	UInt64			char_lev_2_2;
	UInt64			char_lev_3;
	UInt64			char_lev_3_2;
	unsigned int	require_gender;
	int				require_level;
	unsigned int	sect_mask;
	unsigned int	sect_mask_1;
	unsigned int	sect_mask_2;
	unsigned int	sect_mask_3;
	int				require_race;
	int				require_level2;
	int				renascence_count;
	unsigned int	god_devil_mask;
	unsigned int	nation_position_mask;
	int				title_require;

	struct
	{
		int index;
		int num;
	} require_repu[2];

	int				require_territory;
	int				hp;
	int				mp;
	int				dp;
	int				deity_power;
	int				min_dmg;
	int				max_dmg;
	int				defence;
	int				attack;
	int				armor;
	float			attack_range;
	int				price;
	int				shop_price;
	int				id_addon1;
	int				id_addon2;
	int				id_addon3;
	int				id_addon4;
	int				id_addon5;
	int				id_addon6;
	int				id_addon7;
	int				id_addon8;
	int				id_addon9;
	int				id_addon10;
	int				id_addon11;
	int				fee_estone;
	int				fee_install_pstone;
	int				fee_uninstall_pstone;
	int				fee_install_sstone;
	int				fee_uninstall_sstone;
	unsigned int	id_estone;
	int				ehanced_value[20];

	struct
	{
		int			level;

		struct
		{
			int		id;
			float	probability;
		} prop[5];

	} hidden_prop[2];

	int			blood_sacrifice;
	int			can_punching;
	int			color;
	int			fixed_color;
	int			dyed;
	int			soul_level;

	struct
	{
		int level;
		int addon;
	} hidden_addon[3];

	int				gem_slot_identify;
	int				unknown_4;
	int				unknown_5;
	int				unknown_6;
	int				unknown_7;
	int				pile_num_max;
	unsigned int	proc_type;
	int				unknown_9;
	int				unknown_10;
	int				unknown_11;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// ������������
/////////////////////////////////////////////////////////////////////////////////////////////////
struct TALISMAN_MAINPART_ESSENCE
{
	unsigned int	id;							// ��������ID
	namechar		name[32];					// ������������, ���15������

	int				file_model[7];				// ��������ģ��·��1 - 4
	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��
	int				color_change;				// ��ɫ�Ƿ�����仯��ֻ��Էɽ� 0������ɫ�仯�� 1����ɫ����仯

	// ��������ֻ��װ���ڷ�������λ���ϣ�����ָ��

	namechar		show_level[16];				// ��������Ʒ������
	int				level;						// ���������ȼ�

	int				init_level;					// ��ʼ�ȼ�

	UInt64			character_combo_id;			// ְҵ����, �ӵ�λ����λ�ĺ���Ϊ��0-����, 1-��ʿ, 2-��ʿ, 3-սʿ, 4-��ʿ, 5-��ʦ, 6-��ʦ, 7-����, 8-����ʿ, 9-�̿�, 10-��ʥ, 11-����, 12-��˾, 13-�ط�ʦ, 14-�ٻ�ʹ
	UInt64			character_combo_id2;		// ְҵ���ƣ���չְҵID�����ְҵ���ƣ�Added 2011-07-14. 
	UInt64			char_lev_1;
	UInt64			char_lev_1_2;				// תְҵʱ��ְҵ���ƣ���չְҵID�����ְҵ���ƣ�Added 2011-07-14. 
	UInt64			char_lev_2;
	UInt64			char_lev_2_2;				// תְҵʱ��ְҵ���ƣ���չְҵID�����ְҵ���ƣ�Added 2011-07-14.
	UInt64			char_lev_3;
	UInt64			char_lev_3_2;				// תְҵʱ��ְҵ���ƣ���չְҵID�����ְҵ���ƣ�Added 2011-07-14.
	unsigned int	require_gender;				// �Ա����ƣ�0-�У�1-Ů��2-��Ů����
	int				require_level;				// �ȼ�����
	unsigned int	sect_mask;					// ����Ҫ��
	unsigned int	sect_mask_1;				// ����Ҫ��
	unsigned int	sect_mask_2;				// ����Ҫ��
	unsigned int	sect_mask_3;				// ����Ҫ��
	int				require_race;				// �������ƣ�0-���壬1-���壬 2-������
	int				require_level2;				// ����ȼ�����
	int				renascence_count;			// ת������Ҫ��
	unsigned int	god_devil_mask;				// ��ħ
	int				title_require;				// �ƺ�

	int				max_level;					// ���ɳ��ȼ�
	int				max_level2;					// ���ɳ��ȼ�2

	float			energy_recover_speed;		// �������������ٶ�
	float			energy_recover_factor;		// ��ս��״̬�������������ٶ�˥��ϵ��
	float			energy_drop_speed;			// ���������½��ٶ�

	int				fee_levelup;				// ����������
	int				fee_expfood;				// Ѫ��������
	int				fee_reset;					// ����������

	unsigned int	is_aircraft;
	float			fly_speed;					// �����ٶ�
	int				fly_mode;					// ����ģʽ
	float			fly_energy_drop_speed;
	int				fly_exp_added;				// ÿ30�����ӷɽ�����ֵ

	int auto_bloodsmelt_required_amount;
	int bloodsmelt_item;
	int auxilary_item;
	int auxilary_item_amount;

	// ��Ǯ
	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// ����Ѫ������
/////////////////////////////////////////////////////////////////////////////////////////////////
struct TALISMAN_EXPFOOD_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				exp_added;					// ���ӵľ���ֵ
	int				food_type;					// ���õķ�������

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

// ��������ֵ��������
struct TALISMAN_ENERGYFOOD_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·����

	int				energy_total;				// �ܵľ���ֵ
	int				energy_used_minimum;		// ����ʹ����Сֵ
	
	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// ��ҩƷ�Ķѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// ����������
/////////////////////////////////////////////////////////////////////////////////////////////////
struct TALISMAN_MERGEKATALYST_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// ��װ�����ͻ�����ʾʱ��ѯ������
/////////////////////////////////////////////////////////////////////////////////////////////////

// ��װ����
struct SUITE_ESSENCE
{
	// ������Ϣ
	unsigned int	id;							// ��װ(����)ID
	namechar		name[32];					// ��װ����, ���15������

	int				max_equips;					// ��װ����

	struct 
	{
		unsigned int	id;						// ��װ���1~14
	} equipments[14];

	struct 
	{
		unsigned int	id;						// 2����13���ܼ���ĸ�������
	} addons[13];

	char			file_gfx[128];				// ���׺��Ч��
	int				hh_type;					// �ҵ�
	int				equip_soul_suite;			// �Ƿ�������װ��0ΪĬ��ֵ������
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ������Ʒ��ģ��
//
//		ע�⣺������,��Ʒ�Ǹ��������Ʒ����,������ͨ��Ʒ,������Ʒ�Լ�������Ʒ������
//
//
///////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////
// �������ʵ�������
///////////////////////////////////////////////////////////////////////////////////////
struct REFINE_TICKET_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·����

	float			ext_reserved_prob;			// ��߱�������
	float			ext_succeed_prob;			// ��߳ɹ�����

	unsigned int	new_mode;					// �Ƿ���ģʽ
	int				refine_level;				// ���þ����ȼ�
	unsigned int	bind_equip;					// �������װ����Ĭ��Ϊ0
	float			new_succeed_prob[20];		// �����ɹ���

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

/*
	�����йܵ���
*/
struct OFFLINE_TRUSTEE_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·����

	int				max_trustee_time;			// ����й�ʱ��

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

///////////////////////////////////////////////////////////////////////////////////////
// ��Ʊ����
///////////////////////////////////////////////////////////////////////////////////////
struct LOTTERY_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·����

	int				dice_count;					// ���Գ�ȡ�Ĵ���(1~6)
	int				type;						// ��Ʊ����

	struct
	{
		namechar		desc[16];				// ��������, ���16������
		int				icon;					// ͼ��·����

	} candidates[32];							// ��ѡͼ��
	
	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};


///////////////////////////////////////////////////////////////////////////////////////
// ��Ʊ����(�ɶѵ�)
///////////////////////////////////////////////////////////////////////////////////////
struct LOTTERY2_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·����

	int				dice_count;					// ���Գ�ȡ�Ĵ���(1~6)
	int				type;						// ��Ʊ����

	struct
	{
		namechar		desc[16];				// ��������, ���16������
		int				icon;					// ͼ��·����

	} candidates[32];							// ��ѡͼ��
	
	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

///////////////////////////////////////////////////////////////////////////////////////
// ��Ʊ���� (ʹ����Ʒid)
///////////////////////////////////////////////////////////////////////////////////////
//Added 2011-07-01
struct LOTTERY3_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·����

	int				award_item;					// ��������Ʒid��0��ʾû�У�2012-12-18

	int				dice_count;					// ���Գ�ȡ�Ĵ���(1~6)
	int				type;						// ��Ʊ����
	
	struct
	{
		int			item_id;					// ��Ʒid
		int			item_count;					// ����Ʒ������
	}candidate_items[32];

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID

};

///////////////////////////////////////////////////////////////////////////////////////
// ¼�����
///////////////////////////////////////////////////////////////////////////////////////
struct CAMRECORDER_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·����

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};


///////////////////////////////////////////////////////////////////////////////////////
//
// ҩƷģ�����ݽṹ����
//
//
///////////////////////////////////////////////////////////////////////////////////////
// ҩƷ�����
struct MEDICINE_MAJOR_TYPE
{
	unsigned int	id;							// ҩƷ�����ID
	namechar		name[32];					// ҩƷ���������
};

// ҩƷС���
struct MEDICINE_SUB_TYPE
{
	unsigned int	id;							// ҩƷС���ID
	namechar		name[32];					// ҩƷС�������
};

// ҩƷ��������
struct MEDICINE_ESSENCE
{
	unsigned int	id;							// ҩƷ(����)ID
	unsigned int	id_major_type;				// ҩƷ�����ID
	unsigned int	id_sub_type;				// ҩƷС���ID
	namechar		name[32];					// ҩƷ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·����

	int				require_level;				// ��ҵļ���Ҫ��
	int				renascence_count;			// ת������Ҫ��
	int				cool_time;					// ��ȴʱ�䣨���룩

	unsigned int	only_in_war;				// �Ƿ��սר��

	int				type;						// ҩ��������֣�0-˲��HP��1-˲��MP��2-˲��HP��MP��
												// 3-������HP��4-������MP��5-������HP��MP��6-˲��DP��7-������DP

	int				hp[4];						// HP��˲�صı�ʾ���ޣ���˲�صı�ʾ�����Ĵεķֱ𲹸���
	int				mp[4];						// MP��˲�صı�ʾ���ޣ���˲�صı�ʾ�����Ĵεķֱ𲹸���
	int				dp[4];						// DP, ˲�صı�ʾ���ޣ���˲�صı�ʾ�����Ĵεķֱ𲹸���

	int				hmp;						// HMP (HP+MP combined recovery)

	int				map_restrict[5];			// ���õ�ͼ
	
	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// ��ҩƷ�Ķѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ����ԭ��ģ�����ݽṹ����
//
//
///////////////////////////////////////////////////////////////////////////////////////
// ����ԭ�ϴ����
struct MATERIAL_MAJOR_TYPE
{
	unsigned int	id;							// ����ԭ�ϴ����ID
	namechar		name[32];					// ����ԭ�ϴ��������
};

// ����ԭ��С���
struct MATERIAL_SUB_TYPE
{
	unsigned int	id;							// ����ԭ��С���ID
	namechar		name[32];					// ����ԭ��С�������
};

// ����ԭ�ϱ�������
struct MATERIAL_ESSENCE
{
	unsigned int	id;							// ����ԭ��(����)ID
	unsigned int	id_major_type;				// ����ԭ�ϴ����ID
	unsigned int	id_sub_type;				// ����ԭ��С���ID
	namechar		name[32];					// ����ԭ������, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				price;						// �����
	int				shop_price;					// ������

	// �����Ϣ
	int				decompose_price;			// ��ּ۸�
	int				decompose_time;				// ���ʱ��
	unsigned int	element_id;					// ��ֳɵ�Ԫʯ������ID
	int				element_num;				// ��ֲ�����Ԫʯ��

	// �ѵ���Ϣ
	int				pile_num_max;				// ��ԭ�ϵĶѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

///////////////////////////////////////////////////////////////////////////////////////
// ���������е���Ʒ
///////////////////////////////////////////////////////////////////////////////////////
struct DESTROYING_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ������ģ�����ݽṹ����
//
//		������,���ǰ�"��"����Diabllo�е�ʹ�÷���,��Ϊ"tome",��Ϊ��, ��, ��, ����
//
//
///////////////////////////////////////////////////////////////////////////////////////
// ������С���
struct SKILLTOME_SUB_TYPE
{
	unsigned int	id;							// ������С���ID
	namechar		name[32];					// ������С�������
};
		   
// �����鱾������
struct SKILLTOME_ESSENCE
{
	unsigned int	id;							// (����)ID
	unsigned int	id_sub_type;				// ������С���ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·����
	int				file_icon;					// ͼ��·����

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

// ������Ʒ��������
struct SKILLMATTER_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·����
	int				file_icon;					// ͼ��·����

	int				level_required;				// �ȼ�����
	int				renascence_count;			// ת������Ҫ��

	int				id_skill;					// ����ID
	int				level_skill;				// ���ܵȼ�

	unsigned int	only_in_war;				// �Ƿ��սר��

	int				cool_type;					// ��ȴʱ������

	unsigned int	permenent;					// ʹ�ú���ʧ��false-��ʧ��true-����ʧ
	unsigned int	cast_skill;					// 0ֻ������ʹ�ã�1�ɶԷ�����Ŀ��ʹ��
	unsigned int	god_devil_mask;				// ��ħ �컯������/��/ħ/��ѡ1/2/3

	int unknown_1;
	int unknown_2;

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

// ��ͨ����
struct VEHICLE_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	int				file_model;					// ģ���ļ�

	int				file_matter;				// ���ڵ��ϵ�ģ��·����
	int				file_icon;					// ͼ��·����

	int				level_required;				// �ȼ�����
	int				renascence_count;			// ת������Ҫ��
	unsigned int	nation_position_mask;		// ְλҪ��
												// 0x0001 ����
												// 0x0002 ����
												// 0x0004 ����
												// 0x0008 ��
												// 0x0010 ����
	unsigned int	only_qianji;				// �Ƿ�ǣ��ְҵר�� 
	unsigned int	only_in_war;				// �Ƿ��սר��
	unsigned int    is_multi_ride;              // �Ƿ������� 0����1����
	unsigned int	multi_ride_mode;			// ������˵�ģʽ 0������ģʽ 1������ģʽ
	unsigned int	multi_ride_limit;			// Multi_Ride_Limit

	float			speed;						// �ƶ��ٶ�
	float			max_speed;					// Max_Speed
	float			height;						// �߶�

	unsigned int	init_upgrade;
	float			addon_prop[5];

	struct
	{
		int			id;
		float		prop;
	} addon[20];

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

// ������
struct AIRCRAFT_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·����
	int				file_icon;					// ͼ��·����

	int				file_model;					// ģ���ļ�
	int				price;						// �����
	int				shop_price;					// ������
	int				level_required;				// �ȼ�����
	UInt64			character_combo_id;			// ְҵ����
	UInt64			character_combo_id2;		// ְҵ���ƣ���չְҵID�����ְҵ���룬Added 2011-07-14.
	float			speed;						// �ƶ��ٶ�
	int				fly_mode;					// ����ģʽ

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

// ���о���ֵҩƷ
struct FLY_ENERGYFOOD_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·����

	int				energy_total;				// �ܵľ���ֵ
	int				energy_used_minimum;		// ����ʹ����Сֵ
	
	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// ��ҩƷ�Ķѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

///////////////////////////////////////////////////////////////////////////////////////
// ���;���
///////////////////////////////////////////////////////////////////////////////////////
struct TRANSMITROLL_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��
	
	int				renascence_count;			// ������������
	int				require_level;				// �ȼ�����

	int				tag;						// Ŀ������id
	float			x;							// Ŀ��X����
	float			y;							// Ŀ��Y����
	float			z;							// Ŀ��Z����

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

///////////////////////////////////////////////////////////////////////////////////////
// ��������
///////////////////////////////////////////////////////////////////////////////////////
struct LUCKYROLL_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				type;						// ���ͣ�0-��Ʒ����1-�����

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

///////////////////////////////////////////////////////////////////////////////////////
// ˫���������
///////////////////////////////////////////////////////////////////////////////////////
struct DOUBLE_EXP_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				exp_multiple;				// ���鱶��		
	int				double_exp_time;			// �౶����ʱ��(��)��(�Ѿ���ֻ��˫����2013.4.23)

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

///////////////////////////////////////////////////////////////////////////////////////
//
// �سǾ���ģ�����ݽṹ����
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
struct TOWNSCROLL_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	float			use_time;					// ʹ��ʱ�䣨�룩

	int				price;						// �����
	int				shop_price;					// ������

	int				blank;						// �Ƿ�Ϊ�հ�

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

///////////////////////////////////////////////////////////////////////////////////////
//
// �������ģ�����ݽṹ����
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
struct REVIVESCROLL_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	float			use_time;					// ʹ��ʱ�䣨�룩
	int				cool_time;					// ��ȴʱ�䣨���룩
	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ���ǳ��þ���
//
//
//
///////////////////////////////////////////////////////////////////////////////////////

struct SIEGE_ARTILLERY_SCROLL_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};


///////////////////////////////////////////////////////////////////////////////////////
//
// ������Ʒģ�����ݽṹ����
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
struct TASKMATTER_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	int				file_icon;					// ͼ��·��

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

struct TASKMATTER_ESSENCE_2 : public TASKMATTER_ESSENCE
{
	unsigned int    price;
};



///////////////////////////////////////////////////////////////////////////////////////
//	�����������ͨ��Ʒ
///////////////////////////////////////////////////////////////////////////////////////
struct TASKNORMALMATTER_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ����������������ݽṹ����
//
//
//
///////////////////////////////////////////////////////////////////////////////////////
struct TASKDICE_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	struct
	{
		unsigned int	id;						// ����id
		float			probability;			// ����������ĸ���
		
	} task_lists[8];

	int				type;
	
	int			map_trigger;		// ������ͼid
	float		pos_trigger[3];		// ������
	float		radius_trigger;		// �����뾶

	struct 
	{
		unsigned int	map_id;			// ��ͼ��
		float			pos_min[3];		// ���½�
		float			pos_max[3];		// ���Ͻ�
	} trigger_point[8];

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

struct TASKDICE_ESSENCE_2 : public TASKDICE_ESSENCE
{
	unsigned int    price;
};


////////////////////////////////////////////////////////////////////////////////////
// GM ��Ʒ������
////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// GM ��Ʒ���������Ͷ���
///////////////////////////////////////////////////////////////////////////////////////
struct GM_GENERATOR_TYPE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������
};

///////////////////////////////////////////////////////////////////////////////////////
// GM ��Ʒ���������ݽṹ����
///////////////////////////////////////////////////////////////////////////////////////
struct GM_GENERATOR_ESSENCE
{
	unsigned int	id;							// ID
	unsigned int	id_type;					// ���ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	unsigned int	id_object;					// �����ɵ���Ʒ�����ID

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// �̻�����
////////////////////////////////////////////////////////////////////////////////////////
struct FIREWORKS_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				file_fw;					// ���Ч���ļ�
	int				level;						// �ȼ�

	int				time_to_fire;				// ȼ��ʱ��(��)

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// ���̻��Ķѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// �����̻�����
////////////////////////////////////////////////////////////////////////////////////////
struct TEXT_FIREWORKS_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				file_fw;					// ���Ч���ļ�
	int				level;						// �ȼ�

	namechar		char_can_use[256];			// ����ʹ�õ��ַ�����Щ
	int				max_char;					// �������������ַ���

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// ���̻��Ķѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// ǿ��ʯ����
////////////////////////////////////////////////////////////////////////////////////////
struct ESTONE_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	struct  
	{
		unsigned int	equip_mask;				// ����Ƕ��װ�����
		unsigned int	effect_addon_type;		// Ч��������������

	} effects[4];

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// ����ʯ����
////////////////////////////////////////////////////////////////////////////////////////
struct PSTONE_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	unsigned int	type;						// ���ͣ�0-��ͨ��1-����װ���ȼ�Ҫ��1��

	unsigned int	equip_mask;					// ����Ƕ��װ�����
	unsigned int	effect_addon_id;			// Ч����������id
	
	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// ����ʯ����
////////////////////////////////////////////////////////////////////////////////////////
struct SSTONE_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	unsigned int	equip_mask;					// ����Ƕ��װ�����
	unsigned int	skill_id;					// ����id
	int				skill_level;				// ���ܵȼ�

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// Ǳ���鱾��
////////////////////////////////////////////////////////////////////////////////////////
struct POTENTIAL_TOME_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	unsigned int	sect_mask;					// ����Ҫ��
	int				require_race;				// �������ƣ�0-���壬1-���壬 2-������
	int				require_level;				// �ȼ�����
	int				potential_id;				// Ǳ��id

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// ����ͬ�Ľ�
////////////////////////////////////////////////////////////////////////////////////////
struct COUPLE_JUMPTO_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

// ����
struct SPEAKER_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				id_icon_set;				// ��������׺�

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// ��ҩƷ�Ķѵ�����
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

// �鼮
struct BOOK_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��
	
	int				book_type;					// �Ű淽ʽ��1���� 2���� 3-6����
	int				file_content[32];

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// ��ҩƷ�Ķѵ�����
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// ������
////////////////////////////////////////////////////////////////////////////////////////
struct PET_BEDGE_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��
	int				file_head_icon[7];			// ͷ��ͼ��
	int				file_to_shown[7];			// ��ͬ�Ǽ�����

	namechar		desc[16];					// ������������ʾ�ڳ�������֮�£����16������
	float			size;						// �����С
	int				pet_type;					// ��������
	unsigned int	name_color;
	int				show_level;					// ��ʾ�ȼ�
	unsigned int	immune_type;
	int				level;						// �����ʼ�ȼ�

	float			sight_range;				// ��Ұ
	float			dmp_pos;					// �˺���
	float			attack_range;				// ��������

	int				intimacy_inc_speed;			// ���ܶ������ٶ�
	int				hunger_speed1;				// ��ս���±�ʳ�Ƚ����ٶ�
	int				hunger_speed2;				// ս���±�ʳ�Ƚ����ٶ�
	int				hp_gen1;					// ��ս����Ѫ�ٶ�
	int				hp_gen2;					// ս����Ѫ�ٶ�
	int				mp_gen1;					// ��ս����ħ�ٶ�
	int				mp_gen2;					// ս����ħ�ٶ�
	
	unsigned int	stand_mode;					// վ����ʽ�����ܵ��У�����վ����˫��վ�������շ��е�

	float			walk_speed;					// �����ƶ��ٶ�(M/S)
	float			run_speed;					// �����ƶ��ٶ�(M/S)
	
	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// ����ʳƷ
////////////////////////////////////////////////////////////////////////////////////////
struct PET_FOOD_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				pet_level_min;				// ����ȼ�����
	int				pet_level_max;				// ����ȼ�����
	unsigned int	pet_type_mask;				// ������������

	int				food_usage;					// ʳƷ��;
	unsigned int	pet_food_type;				// һ��������ҩ�����Ƕ��������ҩ��
	int				food_value;					// ʳƷ���ӵ�ֵ

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// �����Զ�����ҩ
////////////////////////////////////////////////////////////////////////////////////////
struct PET_AUTO_FOOD_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				food_value;					// ҩƷ������
	float			hp_gen;						// ÿ��ɻظ���Ѫ
	float			mp_gen;						// ÿ��ɻظ�����
	float			hunger_gen;					// ÿ��ɻظ���ʳ��

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// ������������
////////////////////////////////////////////////////////////////////////////////////////
struct PET_REFINE_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				arg1;						// ����ֵ1
	int				arg2;						// ����ֵ2
	int				arg3;						// ����ֵ3
	int				arg4;						// ����ֵ4

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// ����������������
////////////////////////////////////////////////////////////////////////////////////////
struct PET_ASSIST_REFINE_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// ���＼�ܾ�
////////////////////////////////////////////////////////////////////////////////////////
struct PET_SKILL_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				level_required;				// �ȼ�����
	int				id_skill;					// ����id
	int				skill_level;				// ���ܵȼ�
	unsigned int	only_in_war;				// �Ƿ��սר��
	unsigned int	effective_on_pet;			// ʹ�ö���	����/����
	int				cool_type;					// ��ȴʱ������

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

////////////////////////////////////////////////////////////////////////////////////////
// ����װ��
////////////////////////////////////////////////////////////////////////////////////////
struct PET_ARMOR_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��
	namechar		desc[16];					// ����, ���16������

	unsigned int	pet_type_mask;				// �����������
	int				lev_required;
	unsigned int	astro_type;					// �������޶�
	int				astro_level;				// �ǵ��޶�;
	int				equip_location;
	int				fee_enhance;				// ����װ��ǿ������
	
	struct {
		int			maximum;					// ����
		int			minimum;					// ����
	} int_props[12];

	struct {
		float		maximum;					// ����
		float		minimum;					// ����
	} float_props[2];

	struct {
		int			id;
		int			level;
	} addon_skills[5];

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

///////////////////////////////////////////////////////////////////////////////////////
// �䷽����
///////////////////////////////////////////////////////////////////////////////////////
struct RECIPEROLL_MAJOR_TYPE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
};

struct RECIPEROLL_SUB_TYPE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
};

struct RECIPEROLL_ESSENCE
{
	unsigned int	id;							// ID
	unsigned int	id_major_type;				// �����ID
	unsigned int	id_sub_type;				// С���ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	unsigned int	id_recipe;					// ��Ӧ���䷽ID

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

///////////////////////////////////////////////////////////////////////////////////////
//
// �䷽ģ�����ݽṹ����
//
//
///////////////////////////////////////////////////////////////////////////////////////

// �䷽�����
struct RECIPE_MAJOR_TYPE
{
	unsigned int	id;							// �䷽�����ID
	namechar		name[32];					// �䷽���������
};

// �䷽С���
struct RECIPE_SUB_TYPE
{
	unsigned int	id;							// �䷽С���ID
	namechar		name[32];					// �䷽С�������
};

struct RECIPE_ESSENCE
{
	unsigned int	id;							// �䷽����ID
	unsigned int	id_major_type;				// �䷽�����ID
	unsigned int	id_sub_type;				// �䷽С���ID
	namechar		name[32];					// �䷽��������

	int				level;						// �䷽����������ʱ���ݵ�ǰ�������ܶ����ʹ���ֵ�ж��Ƿ��ܹ�����
	
	struct 
	{
		float			probability;				// ���ɸ��ʣ����Զ���һ����
		unsigned int	id_to_make;					// ���ɵĶ���ID
		int				min_num_make;				// ���ɵĶ�����Ŀ����
		int				max_num_make;				// ���ɵĶ�����Ŀ����

	} products[3];
	
	int				price;						// �����ķ���
	float			duration;					// ������ʱ��

	struct
	{
		unsigned int	id;						// ԭ��id
		int				num;					// ԭ��������Ŀ

	}  materials[6];

	int				recipe_class;				// �䷽����
	int				required[7];
	int				cool_time;					// ��ȴʱ�䣨���룩
	int				cool_type;
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ����ģ�����ݽṹ����
//
//		ע: ����ֻ�������������С����, ����Ĺ�����������Ϊһ�������ڸ����н���ѡ��
//
//
///////////////////////////////////////////////////////////////////////////////////////

struct DROPTABLE_TYPE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������
};

struct DROPTABLE_ESSENCE
{
	unsigned int	id;							// ����(����)ID
	unsigned int	id_type;					// ���ID
	namechar		name[32];					// ����, ���15������

	struct										// ����0~5����Ʒ�ĸ���
	{
		float		probability;

	} num_to_drop[5];

	struct										// ������Ʒ�б�
	{
		unsigned int	id_obj;					// ������Ʒid
		float			probability;			// �������

	} drops[64];

};

// �������
struct MONSTER_TYPE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

};

// ���ﱾ������
struct MONSTER_ESSENCE
{
	unsigned int	id;
	unsigned int	id_type;
	namechar		name[32];

	namechar		prop[16];
	namechar		desc[16];
	namechar		monster_desc[16];

	unsigned int	faction;
	unsigned int	enemy_faction;
	unsigned int	monster_faction;

	int				file_model;
	int				head_icon;
	int				big_icon;

	float			size;

	unsigned int	name_color;

	unsigned int	id_strategy;


	unsigned int	select_strategy;
	unsigned int    air_prop;
	unsigned int	show_damage;
	unsigned int	resist_valid;

	int				level;
	int				show_level;
	unsigned int	is_boss;
	unsigned int	killed_exp;
	unsigned int	killed_drop;
	int				is_drop_adjustby_skill;

	unsigned int	immune_type;

	int unknown_1;

	float			sight_range;
	float			attack_range;

	unsigned int	aggressive_mode;
	unsigned int	monster_faction_ask_help;
	unsigned int	monster_faction_can_help;

	float			aggro_range;
	float			aggro_time;
	float			dead_aggro_time;

	unsigned int	patroll_mode;
	unsigned int	stand_mode;

	float			walk_speed;
	float			run_speed;

	int unknown_2;
	int unknown_3;

	unsigned int	common_strategy;

	unsigned int	after_death;
	unsigned int	adjust_exp;
	unsigned int	can_catch;
	unsigned int	player_cannot_attack;
	int				exp;
	int 			money_average;
	int				money_var;
	int				hp;
	int				hp_disp_lv;
	int				mp;
	int				dmg;
	int				defense;
	int				extra_damage;
	int				extra_defence;
	int				attack;
	int				armor;
	int				crit_rate;
	int				crit_damage;
	int				anti_stunt;
	int				anti_weak;
	int				anti_slow;
	int				anti_silence;
	int				anti_sleep;
	int				anti_twist;

	float			lvlup_exp;
	float 			lvlup_money_average;
	float			lvlup_money_var;
	float			lvlup_hp;
	float			lvlup_mp;
	float			lvlup_dmg;
	float			lvlup_defense;
	float			lvlup_attack;
	float			lvlup_armor;
	float			lvlup_crit_rate;
	float			lvlup_crit_damage;
	float			lvlup_anti_stunt;
	float			lvlup_anti_weak;
	float			lvlup_anti_slow;
	float			lvlup_anti_silence;
	float			lvlup_anti_sleep;
	float			lvlup_anti_twist;

	int				hp_gen1;
	int				hp_gen2;
	int				mp_gen1;
	int				mp_gen2;

	unsigned int	role_in_war;


	int				drop_times;

	struct
	{
		int		id_drop_table;
		float	prop_drop_table;
	} drop_table[3];

	struct
	{
		unsigned int	id_skill;
		int				level;
	} skills[32];

	int unknown_4;
	int unknown_5;
	int unknown_6;
	int unknown_7;
	int unknown_8;
	int unknown_9;
	int unknown_10;
	int unknown_11;
	int unknown_12;
	int unknown_13;
	int unknown_14;
	int unknown_15;
	int unknown_16;
	int unknown_17;
	int unknown_18;
	int unknown_19;
	int unknown_20;
	int unknown_21;
	int unknown_22;
	int unknown_23;

	float			dec_crit_rate;
	float			dec_crit_dmg;
	float			armor_prop;
	float			attack_prop;

	unsigned int	tick_stratege;
	unsigned int	task_share;
	unsigned int	item_drop_share;
	unsigned int	buff_area_id;

	int unknown_24;
	int unknown_25;
	int unknown_26;
	int unknown_27;
	int unknown_28;
	int unknown_29;
	int unknown_30;
	int unknown_31;
	int unknown_32;
	int unknown_33;
	int unknown_34;
	int unknown_35;
	int unknown_36;
};

///////////////////////////////////////////////////////////////////////////////////////
//
// NPCģ�����ݽṹ����
//
//		ע: NPC���������ͬ�����ݽṹ����NPC�ܹ��ṩ�ķ�������Ϊһ�����Լ��뵽NPC�����У�
//			���������ص����ݣ��磺������ʱ����Ʒ�б��ȣ�����Ϊ��������Դ��ڷ���ģ����
//			
//		����Ŀǰ���˽⣬����һ�������¼��ࣺ
//			0��������̸���ܣ������ɻ�����̸�����������ķ�����Ҫ�����ݣ��Ի�����
//			1��������Ʒ����Ҫ�����ݣ��Ի����ݡ����ṩ����Ʒ�б�
//			2���չ���Ʒ����Ҫ�����ݣ��Ի�����
//			3��������Ʒ��װ������Ҫ�����ݣ��Ի�����
//			4�������۵�װ����Ƕ��Ʒ����Ҫ�����ݣ��Ի����ݡ������ṩ����Ʒ�б���
//			5���������Ƕ��װ���ϵ���Ʒ����Ҫ�����ݣ��Ի�����
//			6����������֤���񡢽��������������ͬ������صķ�����Ҫ�����ݣ��Ի����ݡ������ṩ�ʹ����������б�
//			7������һЩ���ܣ���Ҫ�����ݣ��Ի����ݡ����Խ��ڵļ����б�
//			8������ҽ������ƣ���Ҫ�����ݣ��Ի�����
//			9������ҽ��д��ͣ���Ҫ�����ݣ��Ի����ݣ�˲��Ŀ��㣬�����б�
//			10��Ϊ����ṩ��ͨ�������������Ҫ�����ݣ��Ի����ݣ���ͨ���߼�·�ߣ��۸�
//			11����Ʒ�Ĵ��ۣ���Ҫ�����ݣ��Ի�����
//			12��Ϊ��Ҵ洢��Ʒ����Ǯ����Ҫ�����ݣ��Ի����ݣ�������
//			13���������ֽ��ض���Ʒ����Ҫ�����ݣ��Ի����ݡ����������ֽ����Ʒ�б����������ֽ���Ʒ��ʱ�䡢��ǮӰ�����
//			14���򵼹��ܣ���Ҫ�����ݣ��ض��ȼ���Ӧ�ĶԻ������������Ӧ�ĶԻ�
//			15����ɱ����/ɱ�����
//			16��ɱ��ĳ�����������
//			17��������ɻ��أ����ɹ���NPC
//			18����·ͨ����֤�ˣ���ƱԱNPC
//
///////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
//
// NPC����ģ�����ݽṹ����
//
///////////////////////////////////////////////////////////////////////////////////////

// ��̸����
struct NPC_TALK_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������	

	unsigned int	id_dialog;					// �Ի�ID
};

// ������Ʒ�ķ���
struct NPC_SELL_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	struct
	{
		namechar		page_title[8];			// ҳ�����֣����7������
		float			discount;
		unsigned int	id_goods[48];			// �ɳ��۵���Ʒ�б�

	} pages[8];									// ��ҳ��ʽ�洢����Ʒ�б�

	unsigned int	id_dialog;					// �Ի�ID
};

// �չ���Ʒ�ķ���
struct NPC_BUY_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������	

	unsigned int	id_dialog;					// �Ի�ID
};

// �����������
struct NPC_TASK_OUT_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������	

	unsigned int	id_task_set;				// ������
	int				task_storage_type;			//����������, Added 2011-02-21
	unsigned int	id_tasks[256];				// ���Դ����������б�
};

// ��֤����������
struct NPC_TASK_IN_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������	

	unsigned int	id_tasks[256];				// ���Դ����������б�
};

// ����������Ʒ����
struct NPC_TASK_MATTER_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������	

	struct
	{
		unsigned int	id_task;				// ���Դ����������б�
		struct
		{
		unsigned int	id_matter;				// ���������Ʒid
		int				num_matter;				// ���������Ʒ��Ŀ
		} taks_matters[4];

	} tasks[16];
};

// ���Ʒ���
struct NPC_HEAL_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	unsigned int	id_dialog;					// �Ի�ID
};

// ���ͷ���
struct NPC_TRANSMIT_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	struct {
		int			id_world;					// Ŀ�������ID
		namechar	name[16];					// Ŀ�������1
		float		x;							// Ŀ���x����
		float		y;							// Ŀ���y����
		float		z;							// Ŀ���z����
		int			fee;						// ����
		int unknown_1;
		int unknown_2;
		int unknown_3;
		int unknown_4;
		int			required_level;				// �ȼ�Ҫ��
	} targets[16];

	unsigned int	id_dialog;					// �Ի�ID
};

// ���۷���
struct NPC_PROXY_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	unsigned int	id_dialog;					// �Ի�ID
};

// �洢��Ʒ����Ǯ
struct NPC_STORAGE_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������
};

// ��ս�����������
struct NPC_WAR_TOWERBUILD_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	struct 
	{
		int				id_in_build;				// �����еĶ���id
		int				id_buildup;					// ������ɺ�Ķ���id
		int				id_object_need;				// ����Ҫ����Ʒ
		int				time_use;					// ���������ʱ��
		int				fee;						// ��������ķ���

	} build_info[4];
};

// ϴ�����
struct NPC_RESETPROP_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	struct 
	{
		int				id_object_need;			// ����Ҫ����Ʒ
		int				resetprop_type;			// ϴ�㷽ʽ
	} prop_entry[6];
};

// װ���󶨷���
struct NPC_EQUIPBIND_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	int				id_object_need;				// ����Ҫ����Ʒ
	int				price;						// ����Ҫ��Ǯ
};

// װ�����ٷ���
struct NPC_EQUIPDESTROY_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	int				id_object_need;				// ����Ҫ����Ʒ
	int				price;						// ����Ҫ��Ǯ
};

// װ��������ٷ���
struct NPC_EQUIPUNDESTROY_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	int				id_object_need;				// ����Ҫ����Ʒ
	int				price;						// ����Ҫ��Ǯ
};

// ��ս���򹭼��ַ���
struct NPC_WAR_BUY_ARCHER_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	int				price;						// ���Ľ�Ǯ
	int				war_material_id;			// ������Դ����
	int				war_material_count;			// ������Դ����
	unsigned int	item_wanted;				// ��Ҫ��������ĳ��Ʒ
	unsigned int	remove_one_item;			// �Ƿ�۵�1������Ʒ
	unsigned int	controller_id[20];			// ������id
};

// ��Ʒ���۷���
struct NPC_CONSIGN_SERVICE
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������
	unsigned int	deposit;				// ��֤��
};

// �����̵����
struct NPC_REPUTATION_SHOP_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������
	
	struct
	{
		namechar		page_title[8];			// ҳ�����֣����7������

		struct  
		{
			unsigned int	id_goods;
			
			struct
			{
				unsigned int	repu_id_req;
				unsigned int	repu_req_value;
				unsigned int	repu_id_consume;
				unsigned int	repu_consume_value;
			} repu_required_pre[3];
			
		}goods[48];	// �ɳ��۵���Ʒ�б�
		
	} pages[4];									// ��ҳ��ʽ�洢����Ʒ�б�
};

// UI���ͷ���
struct NPC_UI_TRANSFER_SERVICE
{
	unsigned int	id;							// ����(����)ID
	namechar		name[32];					// ����, ���15������

	int				trans_id[256];				// ����ģ��id
};

// NPC���Ͷ���
struct NPC_TYPE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������	

};

///////////////////////////////////////////////////////////////////////////////////////
//
// NPC����ģ�����ݽṹ����
//
///////////////////////////////////////////////////////////////////////////////////////
struct NPC_ESSENCE
{
	unsigned int	id;							// NPC(类型)ID
	namechar		name[32];					// 名称, 最多15个汉字

	unsigned int	id_type;					// NPC 类型, 从NPC_TYPE中选取

	namechar		desc[16];					// NPC描述，显示在NPC名字之下，最多16个汉字

	float			refresh_time;				// 刷新时间

	unsigned int	attack_rule;				// 是否被攻击 0-不可被攻击，1-可被攻击攻击变粉名，2-可被攻击攻击变红名

	int				file_model;					// 模型路径名
	int				head_icon;					// 头像

	unsigned int	id_src_monster;				// 基本属性来自的怪物ID，非零时表示使用该怪物属性替换本NPC的属性

	namechar		hello_msg[256];				// 开场白，是一个多行文本

	unsigned int	id_to_discover;				// 发现和传送服务的驿站ID

	unsigned int	domain_related;				// 是否领土相关 0-否，1-是
	unsigned int	guard_npc;					// 是否卫兵NPC  0-否，1-是
	unsigned int	is_vehicle;					// 是否交通工具 0-否，1-是

	unsigned int	war_role_config;			// 城战角色属性表id

	// 服务列表
	unsigned int	id_talk_service;			// 交谈的服务ID
	unsigned int	id_sell_service;			// 出售商品的服务ID
	unsigned int	id_gshop_majortype;			// 百宝阁出售的大类号ID
	unsigned int	id_buy_service;				// 收购商品的服务ID
	unsigned int	id_task_out_service;		// 任务相关的服务ID: 发放任务服务
	unsigned int	id_task_in_service;			// 任务相关的服务ID: 验证完成任务服务
	unsigned int	id_task_matter_service;		// 任务相关的服务ID: 发放任务物品服务
	unsigned int	id_heal_service;			// 治疗的服务ID
	unsigned int	id_transmit_service;		// 传送的服务ID
	unsigned int	id_proxy_service;			// 代售的服务ID
	unsigned int	id_storage_service;			// 仓库的服务ID
	unsigned int	id_war_towerbuild_service;	// 城战炮塔建造的服务ID
	unsigned int	id_resetprop_service;		// 洗点服务ID
	unsigned int	id_equipbind_service;		// 装备绑定服务
	unsigned int	id_equipdestroy_service;	// 装备销毁服务
	unsigned int	id_equipundestroy_service;	// 装备解除销毁服务
	unsigned int	id_war_archer_service;		// 城战购买弓箭手服务
	unsigned int	id_item_trade_service[4];		// 物物交易服务
	unsigned int	id_equip_soul_service;		// 器魄融合服务ID：0表示不开启此服务
	unsigned int	id_consign_service;			// 商品寄售服务ID
	unsigned int	id_reputation_shop_service;	// 声望商店ID
	int				id_trans_dest_service;		// 界面传送服务
	int				id_open_trans;				// 打开某个传送区域
	unsigned int id_item_trade2_service_1;
	unsigned int id_item_trade2_service_2;
	unsigned int	combined_services;			// 简单服务组合：其中的每一位代表一个不需要参数的服务，可能的有：
	//		0：驯化服务；1：跨服pk隐仙阁报名；2：物品锁定；3：驿站发现服务；4：帮派服务；5：修复破损物品；6：邮寄服务；7：拍卖服务；8：双倍经验打卡服务；
	//		9：孵化宠物蛋服务；10：还原宠物蛋服务；11：城战管理服务；12：离开战场服务；13：点卡寄售；14：帮派仓库服务；
	//		15：法宝相关服务；16：城战报名服务；17：战场入场服务；18：城战日常建设服务；19：宠物驯养；20：宠物放生；
	//		21：宠物合成；22：宠物命名; 23: 滴血认主；24：器魄镶嵌；25：器魄拆除；26：恢复魄力；27：装备拆解； 28：飞升战场管理服务
	//		29：宋金战场报名服务 30：竞技场报名服务 31：线上推广服务

	unsigned int	combined_services2;			// 简单服务组合：其中的每一位代表一个不需要参数的服务：
	// 0：形象修改服务；1：宠物装备强化; 2:跨服pk逸龙轩报名 3：跨服pk观战者报名 4:剧情战场报名服务 5：变身道具相关服务
	// 6：变身卡灵力恢复服务；7：领土战宣战；8:进入领土战战场 9：领土战奖励（包括各种奖励）10:星盘充能服务 11:修复破碎的物品（特殊） 
	// 12: 装备升级服务; 13: 传送到跨服服务; 14: 从跨服传回服务; 15: 进入挑战空间服务; 16: 宝石插槽鉴定服务; 17: 宝石插槽重铸服务;
	// 18: 宝石插槽定制服务; 19: 宝石镶嵌服务; 20: 宝石拆除服务; 21: 宝石升品服务; 22: 宝石精炼服务; 23: 宝石萃取服务; 24: 宝石熔炼服务;
	// 25: 跨服小队pk红队报名; 26: 跨服小队pk蓝队报名; 27: 跨服小队pk记者报名; 28: 副本相关服务; 29: 爬塔副本属性显示; 30: 改名服务
	// 31: 家族改名

	unsigned int	combined_services3;			// 简单服务组合：其中的每一位代表一个不需要参数的服务：
	// 0：帮派改名；1：法宝飞升；2：法宝技能融合；3：法宝技能镶嵌；4：无用别勾；5：装备星座打孔；6：星座镶嵌；7：星座摘除；8：无用别勾
	// 9：无用别勾；10：跨服报名传回 11:跨服报名传送 12:国战报名 13:飞升进度查询 14:首饰升级 15:生产技能快速生产 16:金身法身快速制作
	// 17:流水席战场报名 18:帮派基地 19：宠物飞升 20: 灾变专用 21:帮派元宝商城

	unsigned int	combined_services4;

// 洗PK值服务
	unsigned int	has_pkvalue_service;		// 是否提供洗PK值服务
	int				fee_per_pkvalue;			// 每洗一点PK值所需的花费

	bool			service_install;			// 是否提供镶嵌服务
	bool			service_uninstall;			// 是否提供拆卸服务
	bool			service_temp1;				// 占位
	bool			service_temp2;				// 占位

	unsigned int	id_mine;					// 附带的矿
	unsigned int	id_interaction_object;		// 可交互物体
	unsigned int	collision_in_server;		// 是否参与碰撞
	float			vehicle_min_length;			// 交通工具的最小长度
	float			vehicle_min_width;			// 交通工具的最小宽度
	float			vehicle_min_height;			// 交通工具的最小高度
	float			vehicle_max_length;
	float			vehicle_max_width;
	float			vehicle_max_height;
	int unknown_1;
	int				id_territory;				// 所属区域id，领土战专用，默认为0表示非领土战区域npc
	int				id_transcription;			// 副本模板ID，默认情况下为：0表示没有副本发放服务 Added 2011-07-20.
	int				id_transcription2;
	int				id_transcription3;
	int				id_transcription4;
	int				id_transcription5;
};

///////////////////////////////////////////////////////////////////////////////////////
//
// ��ս��ɫ���Ա�
//
///////////////////////////////////////////////////////////////////////////////////////

struct WAR_ROLE_CONFIG
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	int				attack_max_lev;				// �������ȼ�����

	struct
	{
		int			attack_value;				// �������ȼ�1-20�Ĺ�����
		int			attack_extra;				// �������ȼ�1-20�ĸ����˺�
	} attack_level[20];

	int				attack_war_material;		// ���蹥�����ȼ����ĳǷ���������
	float			attack_co[3];				// ���蹥�����ȼ����ĳǷ�����ֵϵ��a,b,c
	
	int				defence_max_lev;			// �������ȼ�����

	struct
	{
		int			defence_value;				// �������ȼ�1-20�ķ�����
		int			defence_extra;				// �������ȼ�1-20�ļ��⸽���˺�
	} defence_level[20];

	int				defence_war_material;		// ����������ȼ����ĳǷ���������
	float			defence_co[3];				// ����������ȼ����ĳǷ�����ֵϵ��a,b,c

	int				range_max_lev;				// ��̵ȼ�����
	int				range_values[5];			// ��̵ȼ�1-5
	int				range_war_material;			// ������̵ȼ����ĳǷ���������
	float			range_co[3];				// ������̵ȼ����ĳǷ�����ֵϵ��a,b,c

	int				strategy_max_lev;			// ���Եȼ�����
	int				strategy_id[5];				// ���Եȼ�1-5
	int				strategy_war_material;		// ������Եȼ����ĳǷ���������
	float			strategy_co[3];				// ������Եȼ����ĳǷ�����ֵϵ��a,b,c

	int				init_hp;					// ��ʼHP
	int				hp_war_material;			// ����HP���ĳǷ���������
	int				lvlup_hp;					// HPÿ�ν�����ߵĵ���
	int				hp_material_num;			// ÿ�ν���HP����Ƿ�����ֵ
};

///////////////////////////////////////////////////////////////////////////////////////
//
// �����ģ�����ݽṹ����
//
///////////////////////////////////////////////////////////////////////////////////////
struct MINE_TYPE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������
};


///////////////////////////////////////////////////////////////////////////////////////
//
// ����ģ�����ݽṹ����
//
///////////////////////////////////////////////////////////////////////////////////////
struct MINE_ESSENCE
{
	unsigned int	id;							// ��(����)ID
	unsigned int	id_type;					// ���ID
	namechar		name[32];					// ����, ���15������
						  
	unsigned int	level;						// �ȼ�
	unsigned int	level_required;				// �ȼ�����
	unsigned int	id_equipment_required;		// ����Ҫ�Ĺ�������
	unsigned int	eliminate_tool;				// ���Ĳɾ��þ�(true or false)
	unsigned int	time_min;					// �ɿ�ʱ�����ޣ��룩
	unsigned int	time_max;					// �ɿ�ʱ�����ޣ��룩


	int				exp;						// ÿ�βɼ�����ľ���
	int				skillpoint;					// ÿ�βɼ�����ļ��ܵ�

	int				cursor_icon;				// �ɿ�ʱ��ʾ������ͼ�꣨���Ϊ0������Ĭ�ϵ�ͼ�꣩	
	int				file_model;					// ģ��·����

	struct 
	{
		unsigned int	id;						// ԭ��ID 
		float			probability;			// �ڳ����� (�ܺ�Ϊ1)
		
	} materials[16];							// �ھ����ԭ�ϱ�

	int				num1;						// ԭ����Ŀ1
	float			probability1;				// ����1
	int				num2;						// ԭ����Ŀ2
	float			probability2;				// ����2

	unsigned int	task_in;					// ��Ҫ������id
	unsigned int	task_out;					// �ɼ����������id

	unsigned int	uninterruptable;			// �ɼ����̲����ж�, false-�����ж�, true-�����ж�

	struct 
	{
		unsigned int	id_monster;				// ��������id
		int				num;					// ����������Ŀ
		float			radius;					// �����뾶���ף�
	} npcgen[4];

	struct 
	{
		unsigned int	monster_faction;		// ��޵Ĺ�����ϸ��ϵ
		float			radius;					// ��ް뾶���ף�
		int				num;					// �����ֵ
	} aggros[1];

	unsigned int	role_in_war;				// ��ս�������ݵĽ�ɫ
												// 0 �ޣ�1 ���Ľ�����2 ������3 ������4 Ͷʯ����5 ���͵㣻6 ����㣻7 ����NPC��8 ռ���־��

	unsigned int	permenent;					// �ɼ�����ﲻ��ʧ��false-��ʧ��true-����ʧ

	unsigned int	activate_controller_mode;	// ���������������ʽ
	unsigned int	activate_controller_id[4];	// �������������id

	unsigned int	deactivate_controller_mode;	// �رչ����������ʽ
	unsigned int	deactivate_controller_id[4];// �رչ��������id
	unsigned int	close_collision_detection;	// �Ƿ�ر���ײ���, 0-���ر�, 1-�ر�
};

// �Ի����̶���
struct talk_proc
{
	namechar			text[64];					// �Ի��ĵ�һ�����ڵ���ʾ���֣����63������

	struct option
	{
		unsigned int	id;							// ѡ�����ӵ��Ӵ��ڻ���ID, ������λΪ1��ʾ��һ��Ԥ����Ĺ���, Ϊ0��ʾһ���Ӵ���
		namechar		text[64];					// ѡ�����ӵ���ʾ���֣����19������
		unsigned int	param;						// ѡ����صĲ���
	};

	struct window
	{
		unsigned int	id;							// ����ID, ���λ����Ϊ1
		unsigned int	id_parent;					// ������ID, Ϊ-1��ʾ������

		int				talk_text_len;				// �Ի����ֵĳ���
		namechar * 		talk_text;					// �Ի�����

		int				num_option;					// ѡ����Ŀ
		option *		options;					// ѡ���б�

		window() { options = 0; }
		~window() { delete [] options; options = 0; delete [] talk_text; talk_text = 0; }

		int save(FILE * fp)
		{
			fwrite(&id, sizeof(id), 1, fp);
			fwrite(&id_parent, sizeof(id_parent), 1, fp);

			fwrite(&talk_text_len, sizeof(talk_text_len), 1, fp);
			fwrite(talk_text, sizeof(namechar), talk_text_len, fp);

			fwrite(&num_option, sizeof(num_option), 1, fp);
			fwrite(options, sizeof(option), num_option, fp);
			return 0;
		}

		int load(FILE * fp)
		{
			fread(&id, sizeof(id), 1, fp);
			fread(&id_parent, sizeof(id_parent), 1, fp);

			fread(&talk_text_len, sizeof(talk_text_len), 1, fp);
			talk_text = new namechar[talk_text_len];
			fread(talk_text, sizeof(namechar), talk_text_len, fp);

			fread(&num_option, sizeof(num_option), 1, fp);
			options = new option[num_option];
			fread(options, sizeof(option), num_option, fp);
			return 0;
		}

		//Added by Ford.W 2010-06-21.
		window& operator = ( const window& src )
		{
			//copy the basic variables.
			id = src.id;
			id_parent = src.id_parent;
			talk_text_len = src.talk_text_len;
			num_option = src.num_option;

			//copy the talk text.
			if( !talk_text_len )
			{
				talk_text = NULL;
			}
			else
			{
				talk_text = new namechar[talk_text_len+1];
//				wcscpy(talk_text, src.talk_text);
				memcpy(talk_text, src.talk_text, (talk_text_len+1)*sizeof(namechar));
			}

			//copy the option.
			if( !num_option )
			{
				options = NULL;
			}
			else
			{
				//if the options is not NULL, then delete it.
				if( options )
				{
					delete []options;
					options = 0;
				}

				options = new option[num_option];
				if( !options )
				{
					//write log: failed to allocate memory.
					return *this;
				}

				//copy the options.
				for(int i = 0; i < num_option; ++i )
				{
					options[i] = src.options[i];
				}

			}

			return *this;
		}
		//Added end.
	};

	unsigned int		id_talk;				// �Ի������ID

	int					num_window;				// ���Ի����ֵĴ��ڸ���
	window *			windows;				// ���Ի����ֵĴ���
	
	talk_proc()
	{
		id_talk		= 0;
		num_window	= 0;
		windows		= 0;
	}

	~talk_proc()
	{
		delete [] windows;
		windows = 0;
		num_window = 0;
	}

	// ���̶���
	int save(FILE * fp)
	{
		fwrite(&id_talk, sizeof(id_talk), 1, fp);
		fwrite(text, sizeof(text), 1, fp);

		fwrite(&num_window, sizeof(num_window), 1, fp);
		for(int i=0; i<num_window; i++)
			windows[i].save(fp);
		
		return 0;
	}

	int load(FILE * fp)
	{
		fread(&id_talk, sizeof(id_talk), 1, fp);
		fread(text, sizeof(text), 1, fp);

		fread(&num_window, sizeof(num_window), 1, fp);
		windows = new window[num_window]; 
		for(int i=0; i<num_window; i++)
			windows[i].load(fp);

		return 0;
	}

	//Added by Ford.W 2010-06-21.
	talk_proc& operator = (const talk_proc& src)
	{
		//copy the basic variables.
//		wcscpy(text, src.text);
		memcpy(text, src.text, sizeof(src.text));
		id_talk = src.id_talk;
		num_window = src.num_window;

		//create num_window window, and then set the value.
		if( !num_window )
		{
			windows = NULL;
		}
		else
		{
			//create the windows, and if the windows isnot NULL, then delete it.
			if( windows )
			{
				delete []windows;
				windows = NULL;
			}

			windows = new window[num_window];
			if( !windows )
			{
				//write log: failed to allocate memory!
				return *this;//?something unreasonable?
			}

			//copy the data from the src talk_proc.
			for(int i = 0; i < num_window; ++i )
			{
				windows[i] = src.windows[i];
			}
		}

		//return the talk_proc instance.
		return *this;
	}

	//Added end.
};

///////////////////////////////////////////////////////////////////////////////////////
// ϵͳ�����ļ���ģ��
///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////
// ��Ӫ�ж��б�
///////////////////////////////////////////////////////////////////////////////////////
struct ENEMY_FACTION_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������

	unsigned int	enemy_factions[32];		// 0 - ��ͨ��ҵĵж���Ӫ�б�
											// 1 - ����״̬�ĵж���Ӫ�б�
											// 2 - ����״̬�ĵж���Ӫ�б�
											// 3 - ����״̬�ĵж���Ӫ�б�
											// 4 - ��ս�����ĵж���Ӫ�б�
											// 5 - ��ս�ط��ĵж���Ӫ�б�
											// 6 - ��ս�����Ѿ��ĵж���Ӫ�б�
											// 7 - ��ս�ط��Ѿ��ĵж���Ӫ�б�
											// 8 - ��ͨ����ĵж���Ӫ�б�
											// 9 - ��ͨNPC(���ܡ���������)�ĵж���Ӫ�б�
											// 10 - ������ϵ1�ĵж���Ӫ�б�
											// 11 - ������ϵ2�ĵж���Ӫ�б�
											// 12 - 32 ��ȱ�ĵж���Ӫ�б�
};

///////////////////////////////////////////////////////////////////////////////////////
// ְҵ���������б�
///////////////////////////////////////////////////////////////////////////////////////
struct CHARACTER_CLASS_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������

	unsigned int	character_class_id;		// ְҵid, ���ܵ���:0-����, 1-��ʿ, 2-��ʿ, 3-սʿ, 4-��ʿ, 5-��ʦ, 6-��ʦ, 7-����, 8-����ʿ, 9-�̿�, 10-��ʥ, 11-����, 12-��˾, 13-�ط�ʦ, 14-�ٻ�ʹ
	
	unsigned int	faction;				// ��ְҵ��������Ӫ
	unsigned int	enemy_faction;			// ��ְҵ�ĵж���Ӫ

	int				level_required;			// ת����ְҵ����Ҫ����С�ȼ�
	unsigned int	sect_mask;				// ���ɹ���
	int				level2;					// ����ȼ�

	int				hp;						// ��ʼ������
	int				mp;						// ��ʼħ��
	int				dmg;					// ��ʼ������
	int				defense;				// ��ʼ������
	int				attack;					// ��ʼ����
	int				armor;					// ��ʼ����
	float			crit_rate;				// ��ʼ����һ����(%)
	float			crit_damage;			// ��ʼ����һ���˺�(%)
	int				anti_stunt;				// ��ʼѣ�ο���
	int				anti_weak;				// ��ʼ��������
	int				anti_slow;				// ��ʼ��������
	int				anti_silence;			// ��ʼ��Ĭ����
	int				anti_sleep;				// ��ʼ˯�߿���
	int				anti_twist;				// ��ʼ���ƿ���

	float			lvlup_hp;				// ÿ��һ����������������
	float			lvlup_mp;				// ÿ��һ����������ħ��
	float			lvlup_dmg;				// ÿ��һ���������Ĺ�����
	float			lvlup_defense;			// ÿ��һ���������ķ�����
	float			lvlup_attack;			// ÿ��һ��������������
	float			lvlup_armor;			// ÿ��һ��������������
	float			lvlup_crit_rate;		// ÿ��һ��������������һ����(%)
	float			lvlup_crit_damage;		// ÿ��һ��������������һ���˺�(%)
	float			lvlup_anti_stunt;		// ÿ��һ����������ѣ�ο���
	float			lvlup_anti_weak;		// ÿ��һ������������������
	float			lvlup_anti_slow;		// ÿ��һ���������Ļ�������
	float			lvlup_anti_silence;		// ÿ��һ���������ĳ�Ĭ����
	float			lvlup_anti_sleep;		// ÿ��һ����������˯�߿���
	float			lvlup_anti_twist;		// ÿ��һ���������Ĳ��ƿ���

	float			walk_speed;				// �����ٶ�(M/S)
	float			run_speed;				// �����ٶ�(M/S)

	int				hp_gen1;				// ��ս��״̬��HP�ָ��ٶ�
	int				hp_gen2;				// ��ս��״̬�д���HP�ָ��ٶ�
	int				hp_gen3;				// ս��״̬��HP�ָ��ٶ�
	int				hp_gen4;				// ս��״̬�д���HP�ָ��ٶ�
	int				mp_gen1;				// ��ս��״̬��MP�ָ��ٶ�
	int				mp_gen2;				// ��ս��״̬�д���MP�ָ��ٶ�
	int				mp_gen3;				// ս��״̬��MP�ָ��ٶ�
	int				mp_gen4;				// ս��״̬�д���MP�ָ��ٶ�

	UInt64			character_combo_id;		// ����ת����ְҵ���, �ӵ�λ����λ�ĺ���Ϊ��0-����, 1-��ʿ, 2-��ʿ, 3-սʿ, 4-��ʿ, 5-��ʦ, 6-��ʦ, 7-����, 8-����ʿ, 9-�̿�, 10-��ʥ, 11-����, 12-��˾, 13-�ط�ʦ, 14-�ٻ�ʹ
	UInt64			character_combo_id2;	// ����ת����ְҵ���, ��չְҵID�����ְҵ���룬 Added 2011-07-14.
	
	int				xp_skill;				// ��ɫȺ�ּ���
};

///////////////////////////////////////////////////////////////////////////////////////
// �����������Ա�
///////////////////////////////////////////////////////////////////////////////////////
struct DEITY_CHARACTER_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������

	struct  
	{
		int				deity_point;			// ����ȼ��ﵽxʱ������ֵ
		int				hp;						// ����ȼ��ﵽxʱ���ӵ�������
		int				mp;						// ����ȼ��ﵽxʱ���ӵ�ħ��
		int				dmg;					// ����ȼ��ﵽxʱ���ӵĹ�����
		int				defense;				// ����ȼ��ﵽxʱ���ӵķ�����
		int				deity_power;			// ����ȼ��ﵽxʱ���ӵ���ʥ����������Ϊ��ʼΪ0��
		int				anti_stunt;				// ����ȼ��ﵽxʱ���ӵ�ѣ�ο���
		int				anti_weak;				// ����ȼ��ﵽxʱ���ӵ���������
		int				anti_slow;				// ����ȼ��ﵽxʱ���ӵĻ�������
		int				anti_silence;			// ����ȼ��ﵽxʱ���ӵĳ�Ĭ����
		int				anti_sleep;				// ����ȼ��ﵽxʱ���ӵ�˯�߿���
		int				anti_twist;				// ����ȼ��ﵽxʱ���ӵĲ��ƿ���

		int				dp_gen1;				// ��ս��״̬��DP�ָ��ٶ�
		int				dp_gen2;				// ��ս��״̬�д���DP�ָ��ٶ�
		int				dp_gen3;				// ս��״̬��DP�ָ��ٶ�
		int				dp_gen4;				// ս��״̬�д���DP�ָ��ٶ�
		int				talent_point;			// ����ȼ��ﵽx��õ��������
		int				skill_point;			// ����ȼ��ﵽx��õļ��ܵ���
	}deity_config[100];
};

///////////////////////////////////////////////////////////////////////////////////////
// ����������������Ϸ�м��㹫ʽ��ĳЩ����������
///////////////////////////////////////////////////////////////////////////////////////
struct PARAM_ADJUST_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������

	unsigned int    adjust_config_id;		//������id��Ϊ��ʹ����ǰ�ͷ���������ʹ�ò�ͬ��������
	struct
	{
		int			level_diff;				// Player�͹���ļ����(Player-����)(�������Ϊ0ʱ����ʾ����ʧЧ)
		float		adjust_exp;				// ��ɱ�ֻ�þ��������ϵ��(0.0~1.0)
		float		adjust_money;			// ��ɱ�ֻ�ý�Ǯ������Ŀ������ϵ��(0.0~1.0)
		float		adjust_matter;			// ��ɱ�ֵ�����Ʒ�ĸ��ʵ�����(0.0~1.0)
		float		adjust_attack;			// �Թ������ĵȼ��ͷ�ϵ��(0.0~1.0)
	} level_diff_adjust[16];				// ������ɱ�ּ�������			

	struct
	{
		float		adjust_exp;				// ��ɱ�ֻ�þ��������ϵ��(0.0~1.0)
	} team_adjust[11];						// ���ɱ��ʱ�ļ�������
	
	struct
	{
		float		adjust_exp;				// ��ɱ�ֻ�þ��������ϵ��(0.0~1.0)
	} team_profession_adjust[9];			// ������ְҵ������������

	struct			
	{
		int			level_diff;				// �����ȼ�������װ���ļ���
		float		adjust_exp;				// ������������������ϵ��������0.0��1.0��
	} level_diff_produce[9];
};

///////////////////////////////////////////////////////////////////////////////////////
// ����ƽ̨���ñ�
///////////////////////////////////////////////////////////////////////////////////////
struct PIE_LOVE_CONFIG
{
	unsigned int	id;								// id
	namechar		name[32];						// ���ƣ����15������

	struct  
	{
		int				fee_press_msg;				//������Ϣ���Ľ�Ǯ
		int             id_press_msg_item;			//������Ϣ������Ʒid
		int				id2_press_msg_item;			//������Ϣ������Ʒid2
		int				id_press_msg_task;			//������Ϣ��������id
		int				id_support_msg_item;		//ͶƱ֧����Ϣ������Ʒid
		int				id_support_msg_task;		//ͶƱ֧����Ϣ��������id
		int				id_against_msg_item;		//ͶƱ������Ϣ������ƷID
		int				id_against_msg_task;		//ͶƱ������Ϣ��������ID
		int				fee_apply_msg;				//������Ϣ���Ľ�Ǯ
		int				id_apply_msg_item;			//������Ϣ������ƷID
		int				id_apply_msg_task;			//������Ϣ��������ID
		int				fee_response_msg;			//���԰�ظ���Ϣ���Ľ�Ǯ
		int				id_response_msg_item;		//���԰�ظ���Ϣ������ƷID
		int				id_accept_apply_task1;		//�������봥������ID1
		int				id_accept_apply_task2;		//�������봥������ID2
		int				id_accept_apply_task3;		//�������봥������ID3
	} sns_config[5];
	// 4���ֱ��� ���顢���ѡ���ͽ���������ˡ�֪������	
};

///////////////////////////////////////////////////////////////////////////////////////
// ����������߱�
///////////////////////////////////////////////////////////////////////////////////////
struct PLAYER_LEVELEXP_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������

	double			exp[200];				// ��������1~200��ÿ������Ҫ�ľ���ֵ
	double			exp_lev_1[200];			// 1ת��������1~200��ÿ������Ҫ�ľ���ֵ
	int unknown_1;
	int unknown_2;
	int unknown_3;
	int unknown_4;
	int unknown_5;
	int unknown_6;
	int unknown_7;
	int unknown_8;
	int unknown_9;
	double			exp_lev_2[200];			// 2ת��������1~200��ÿ������Ҫ�ľ���ֵ
	double			exp_lev_3[200];			// 3ת��������1~200��ÿ������Ҫ�ľ���ֵ
	int				talisman_exp[100];		// ������������1~100��ÿ������Ҫ�ľ���ֵ

	int				prod_exp_need[10];			// �����ȼ�����1~10����Ҫ����
	int				prod_exp_gained[10][10];	// ����XX��װ����õľ���, ��ͨ�� prod_exp_gained[��ǰ����][�䷽����]��ȡ����������ֵ
	double			pet_exp[200];				// ������������1~200��ÿ������Ҫ�ľ���ֵ
	
	struct  
	{
		double	deity_exp;						// Ԫ��������Ҫ�ķ�����Ϊ
		float	levelup_prob;					// Ԫ�������ĸ���
		double	failure_exp;					// Ԫ������ʧ�ܿ۳��ķ�����Ϊ
	}deity[100];	
};

struct PLAYER_SECONDLEVEL_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������

	float			exp_lost[256];			// ������ʧ��
};

struct PLAYER_TALENT_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������

	int				talent_point[200];
	int				talent_point_1[200];
	int				talent_point_2[200];
	int				talent_point_3[200];
};

struct PLAYER_SKILL_POINT_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������

	int				skill_point[200];
	int				skill_point_1[200];
	int				skill_point_2[200];
	int				skill_point_3[200];
};

struct TITLE_PROP_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������

	unsigned int	id_title;				// ��νid
	unsigned int	id_addons[3];			// ��ν��������
};


/////////////////////////////////////////////////////////////////////////////////////////
// ����ID�����
/////////////////////////////////////////////////////////////////////////////////////////
struct SPECIAL_ID_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������
	
	unsigned int	version;					// elementdata version used by game designers	
	int			id_tmpl_jinfashen_config;	// ������������һ����ñ�id
	float		monster_drop_prob;			// ��������Ǯ����
	int			id_money_matter;			// ��Ǯ��Ʒ��id
	int			id_speaker;					// ǧ�ﴫ����id
	int			id_destroying_matter;		// �ݻ���Ʒ��id
	int			id_townscroll;				// ���Ͷݵط���id
	int			id_attacker_droptable;		// ����������������
	int			id_defender_droptable;		// �ط�������������
	int			id_talisman_reset_matter;	// ����ϴ����
	int			fee_talisman_merge;			// ��������������
	int			fee_talisman_enchant;		// ������ħ������
	int			id_war_material1;			// ��ս����1��ҪID
	int			id_war_material2;			// ��ս����2��ҪID
	int			fee_pet_gained;				// ����ѱ��������
	int			fee_pet_free;				// �������������
	int			fee_pet_refine;				// ��������������
	int			fee_pet_rename;				// �������������
	int			id_male_war_statue;			// �г�ս����id
	int			id_female_war_statue;		// Ů��ս����id
	int			id_vehicle_upgrade_item;	// ѱ����Ʒ
	int			fee_lock;					// ��������
	int			fee_unlock;					// ��������
	int			unlock_item_id;				// ��������id
	int			unlocking_item_id;
	int			id_damaged_item;			// �������Ʒ
	int			id_repair_damaged_item;		// �޸��������Ʒ
	int			id_bleed_identity_host_item;// ��Ѫ��������id
	int			fee_restore_soul;			// �ָ�ÿ��������������
	int			id_lowgrade_soul_stone;		// �ͼ�Ԫ����
	int			id_highgrade_soul_stone;	// �߼�Ԫ����
	int			id_enter_arena_item;		// ��ͨ������������Ʒid
	int			id_enter_arena_reborn_item;	// ����������������Ʒid
	int			id_change_face_item;		// �����������ĵ��ߵ�id
	int			id_speaker2;				// ǧ�ﴫ��id2 
	int			id_unique_bid_item;			// Ψһ��ͼ������ĳ��۷�

	//Added 2011-03-10.
	int			fee_gem_refine;				// ��ʯ����������
	int			fee_gem_extract;			// ��ʯ��ȡ������
	int			fee_gem_tessellation;		// ��ʯ��Ƕ������
	int			fee_gem_single_dismantle;	// ��ʯ���β��������
	int			fee_gem_smelt;				// ��ʯ����������
	int			fee_gem_slot_identify;		// ��ʯ��ۼ���������
	int			fee_gem_slot_customize;		// ��ʯ��۶���������
	int			fee_gem_slot_rebuild;		// ��ʯ�������������
	int			gem_upgrade_upper_limit;	// ��ʯ��Ʒ����
	int			id_gem_smelt_article;		// ��ʯ��������ID
	int			id_gem_smelt_article_1;		// ��ʯ��������ID����Ϊ�ǰ󶨵��ߵ� Added 2011-04-01
	int			id_gem_smelt_article_2;		// ��ʯ��������ID������Ϊ���������ô� Added 2011-04-01
	int			id_gem_refine_article;		// ��ʯ��������ID
	int			id_gem_refine_article_1;	// ��ʯ��������ID, ����Ϊ�ǰ󶨵��ߵ� Added 2011-04-01
	int			id_gem_refine_article_2;	// ��ʯ��������ID, ����Ϊ���������ô����� Added 2011-04-01
	int			id_gem_slot_lock_article;	// ��ʯ�������ʱ��������ID
	int			id_gem_slot_lock_article_1;	// ��ʯ�������ʱ��������ID, ����Ϊ�ǰ󶨵��ߵ� Added 2011-04-01
	int			id_gem_slot_lock_article_2;	// ��ʯ�������ʱ��������ID, ���������ô����� Added 2011-04-01
	int			id_gem_slot_rebuild_article[3];//��ʯ��������������ID,�����������ͨ���ǰ󶨣�������;�� Added 2011-05-03
	int			id_consign_role_item;		//��ɫ�����������ĵ���ƷID	Added 2012-03-23.
	int			consign_role_item_count;	//��ɫ��������������Ʒ������	Added 2012-03-23.
	int			consign_role_type;			//��ɫ��������Ҫʹ����� Added 2012-03-23.
	int			id_treasure_region_upgrade_item;	//���ɹž��ڱ�������������Ҫ���ĵ���Ʒ��ID Added 2012-03-23.
	
	int			id_speaker_special[3];		// ������id
	int			id_speaker_special_anony[3];	// ����������id
	int			id_cross_speaker_special[3];	// ID_Cross_Speaker_Special_1/2/3
	int			id_cross_speaker_special_anony[3];	// ID_Cross_Speaker_Special_Anony_1/2/3
	int			id_change_name[3];			// ��������id
	int			id_change_name_family[3];	// �����������id
	int			id_change_name_guild[3];	// ���ɸ�������id

	// �������
	int			id_equip_hole[3];			// װ����׵��ߣ���Ƕ������
	int			id_xingzuo_levelup[3];		// ������������
	int			fee_xingzuo_add;			// ������Ƕ������
	int			fee_xingzuo_remove;			// ����ժ��������
	int			fee_xingzuo_identify;		// ��������������

	int			id_fix_prop_lose;			// �޸���к�(δ��150����)�������id
	int			id_rose_free;				// ���õ�廨id
	int			id_rose_money;				// Ԫ��õ�廨id
	
	int			id_rename_equip_Props1;		// +12װ������������1id
	int			id_rename_equip_Props2;		// +12װ������������2id
	
	int			id_rune2013_fragment[2];	// ������Ƭid
	int			id_rune2013_erase[2];		// �Ƴ�����id
	int			id_rune2013_merge_extra_num[2];	// ��������ںϴ���id
	int			id_matrix_card_break;		// ID_Matrix_Card_Break
	int			fee_vehicle_enhance;		// Fee_Vehicle_Enhance

	int id_change_face_item_2;
	int id_change_face_item_3;
	int unknown_3;
};

/////////////////////////////////////////////////////////////////////////////////////////
// ת������ģ��
/////////////////////////////////////////////////////////////////////////////////////////
struct RENASCENCE_SKILL_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������

	UInt64			occup_lev_0;			// shit���Ҽ���ע�Ͳ����Ժ����Ҫ�޸Ĵ��붼�Ҳ���������
	UInt64			occup_lev_0_2;			// 0 תְҵ�����Ƶ�ְҵ���룬��չְҵID�����ְҵ���룬Added 2011-07-14.
	UInt64			occup_lev_1;
	UInt64			occup_lev_1_2;			// 1 תְҵ�����Ƶ�ְҵID���룬��չְҵID�����ְҵ���룬Added 2011-07-14.
	UInt64			occup_lev_2;
	UInt64			occup_lev_2_2;			// 2 תְҵ�����Ƶ�ְҵID���룬��չְҵID�����ְҵ���룬Added 2011-07-14.
	UInt64			occup_lev_3;
	UInt64			occup_lev_3_2;			// 3 תְҵ�����Ƶ�ְҵID���룬��չְҵID�����ְҵ���룬Added 2011-07-14.

	struct
	{
		int id;
		int lev;
	} skills[12];
};

struct RENASCENCE_PROP_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������

	UInt64			occup_lev_0;			// 1: �����ı��������������⣬��֪���⣬ 2������ע�ͺܲ��á�����
	UInt64			occup_lev_0_2;			// 0 תְҵ�����Ƶ�ְҵ���룬��չְҵID�����ְҵ���룬Added 2011-07-14
	UInt64			occup_lev_1;
	UInt64			occup_lev_1_2;			// 1 תְҵ�����Ƶ�ְҵ���룬��չְҵID�����ְҵ���룬Added 2011-07-14
	UInt64			occup_lev_2;
	UInt64			occup_lev_2_2;			// 2 תְҵ�����Ƶ�ְҵ���룬��չְҵID�����ְҵ���룬Added 2011-07-14
	UInt64			occup_lev_3;
	UInt64			occup_lev_3_2;			// 3 תְҵ�����Ƶ�ְҵ���룬��չְҵID�����ְҵ���룬Added 2011-07-14

	float			hp[15];
	float			mp[15];
	float			dmg[15];
	float			def[15];
	float			attack[15];
	float			armor[15];
	float			crit_rate[15];
	float			crit_damage[15];
	float			anti[6][15];
};

/*
	���ｻ��
*/

struct ITEM_TRADE_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������

	struct
	{
		namechar		page_title[8];			// ҳ�����֣����7������

		struct
		{
			unsigned int	id_goods;
			unsigned int	goods_num;	// ��Ӧ��Ʒ�Ĵ��������һ�����n����

			struct
			{
				unsigned int	id;
				int				count;
			} item_required[2];

			struct
			{
				unsigned int	id;
				int				count;
			} repu_required[2];

			struct 
			{
				unsigned int  type;  // �ķѣ��ɻ�ֵ or ...
				int			  count;
			} special_required[1];

			int unknown_1;
			int unknown_2;

		} goods[48];
	} pages[4];

	unsigned int	id_dialog;					// �Ի�ID
};

//�����ں��䷽
struct EQUIP_SOUL_MELD_SERVICE
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������
	unsigned int	prescription[500];		// �䷽
};

//����
struct EQUIP_SOUL_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	unsigned int	equip_mask;					// ����Ƕ��λ	
	int				level_required;				// �ȼ�����
	int				soul_type;					// �������࣬��0-9��
	int				soul_level;					// ���Ǳ����ļ��𣬣�0-16����Ĭ��Ϊ0����ʾ���ǵĵ���
	UInt64			character_combo_id;			// ְҵ����, �ӵ�λ����λ�ĺ���Ϊ��0-����, 1-��ʿ, 2-��ʿ, 3-սʿ, 4-��ʿ, 5-��ʦ, 6-��ʦ, 7-����, 8-����ʿ, 9-�̿�, 10-��ʥ, 11-����, 12-��˾, 13-�ط�ʦ, 14-�ٻ�ʹ
	UInt64			character_combo_id2;		// ְҵ����, ��չְҵID�����ְҵ���룬Added 2011-07-14.
	unsigned int	sect_mask;					// ����Ƕ������ �ͻ�����ʾ
	int				require_race;				// �������ƣ�0-���壬1-���壬 2-������
	int				hole_type;					// ��λ		0:ȫ����1����  2��ľ 3��ˮ 4���� 5����
	int				id_addon1;					// Ч����������
	int				cost_soul;					// ռ�õĻ���

	int				price;						// �����
	int				shop_price;					// ������

	float			fee_install;				// ����������
	float			fee_uninstall;				// ϴ��������
	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

// ������������Ʒ
struct SPECIAL_NAME_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·����

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

// �ν�ս��������Ʒ���ñ�
struct RECYCLE_ITEM_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				map_id;						// ��Ӧ�ĵ�ͼ��

	struct
	{
		int id_obj;
	}recycle_item_list[200];
};


// �ν�ս����������εĶ�Ӧ��
struct SCORE_TO_RANK_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				map_id;						// ��Ӧ�ĵ�ͼ��
	
	int				controller;					// ս����������Ŀ�����
	struct
	{
		int score;								//  �þ����������
		int task_id;							//  ս�������󣬸þ��δ���������id
		namechar rank_name[16];					//  �������ƣ����15������
	}score2rank[10];
};

// �ν�ս�������������ñ�
struct BATTLE_DROP_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				map_id;						// ��Ӧ�ĵ�ͼ��

	struct										// ������Ʒ�б�
	{
		unsigned int	id_obj;					// ������Ʒid
		float			probability;			// �������

	} drops[10][10];                            // ǰ������Ǿ�����ţ�����1-����10�������ǵ�����Ʒid���id1-id10
};

// �ν�ս�������Ӷ����ñ�
struct BATTLE_DEPRIVE_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				map_id;						// ��Ӧ�ĵ�ͼ��
	
	struct 
	{
		unsigned int id_obj;					//  �Ӷ���Ʒ��id
		unsigned int max_num;					//	�Ӷ����Ʒ����Ŀ����
	}deprive[10];

};

// �ν�ս�����ֻ�ȡ���ñ�
struct BATTLE_SCORE_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				map_id;						// ��Ӧ�ĵ�ͼ��

	struct 
	{
		unsigned int score;						// ��ȡ�Ļ���
	}scores[10];								// ɱ������1-10����ȡ�Ļ���

	struct 
	{
		unsigned int ext_score;
	}ext_score[8];								// ����ɱ��3-10�˻�ȡ�Ķ������						

	struct 
	{
		unsigned int monster_id;
		unsigned int monster_score;				
	}monster_scores[50];						// ɱ���ض�id�Ĺ����ȡ�Ļ���
	
	unsigned int   victory_score;				// �������ʤ����ȡ�Ļ���
};

// ���
struct GIFT_BAG_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				level_everLimit;			// �����ĵȼ������ޣ��ʺϷ���֮����жϣ�
	int				level_required;				// ��ǰ�ȼ�����
	int				level_upperLimit;			// ��ǰ�ȼ�����
	unsigned int	require_gender;				// �Ա����ƣ�0-�У�1-Ů��2-��Ů����
	int				renascence_count;			// ת������Ҫ��
	UInt64			character_combo_id;			// ְҵ����
	UInt64			character_combo_id2;		// ְҵ����, ��չְҵID�����ְҵ���룬Added 2011-07-14.
	unsigned int	god_devil_mask;				// ��ħ �컯������/��/ħ/��ѡ1/2/3
	unsigned int    open_time;					// ������Ҫ��ʱ��
	float			probability;				// �����ɹ��ĸ���
	unsigned int	success_disappear;			// �ɹ���������ʧ(1Ϊ�ǣ�0Ϊ��)
	unsigned int	faliure_disappear;			// ����ʧ�ܺ���ʧ(1Ϊ�ǣ�0Ϊ��)
	unsigned int	id1_object_need;			// ������Ҫ��Ʒ1
	unsigned int	id1_object_num;				// ������Ҫ��Ʒ1������
	unsigned int	id2_object_need;			// ������Ҫ��Ʒ2
	unsigned int	id2_object_num;				// ������Ҫ��Ʒ2������
	int				money_need;					// ������Ҫ��Ǯ
	unsigned int	num_object;					// �ں���Ʒ������

	struct 
	{
		unsigned int	id_object;				// ��Ʒid
		float			probability;			// ��Ʒ��������
		unsigned int	num_min;				// ��Ʒ����������
		unsigned int	num_max;				// ��Ʒ����������
		unsigned int	is_bind;				// ��Ʒ�Ƿ��
		unsigned int	effect_time;			// ��Ʒ��ʱЧ����λΪ�룬0Ϊ���ã�
	}gifts[16];
	
	unsigned int	normalize_group[4];

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
	
};

// ������������Ʒ
struct VIP_CARD_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·����

	int				vip_type;					// vip �������ͣ���������1�����ƽ𿨣�2�����׽𿨣�3������
	int				expires;					// ��Ч��
	
	float			multiple_exp;				// ����ӳ�

	unsigned int	free_helper;				// ��ѹһ�
	unsigned int	no_exp_drop;				// �����󲻵�����

	struct 
	{
		int skill_id;							// ������Ʒid
		int skill_level;						// ������Ʒ�ȼ�
	}buff[8];

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

struct INSTANCING_BATTLE_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// ���֣����15������

	int				map_id;						// ս��id
	namechar		desc[16];					// ս��˵��
	int				file_map;					// ��ͼ�洢·��
	int				active_fee;					// ����ѽ�Ǯ
	int				active_item;				// ����ս���ĵ���
	int				apply_time;					// ����ʱ��
	int				run_time;					// ����ʱ��
	int				renascence_count;			// ת������Ҫ��
	int				level_lower_limit;			// �ȼ�����
	int				level_upper_limit;			// �ȼ�����
	int				max_player;					// ս����������
	int				open_player_num;			// ��ǰ������������
	int				open_player_num2;			// �Զ�������������
	int				victory_controller;			// ս��ʤ��������������������
	int				reopen_time;				// ���¿���ʱ����
	int				relation_limit;				// ����ս������
	int				sect_limit;					// ��������
	int				forbid_food;				// ��ʳ����
	int				valid_tollgate_num;			// ��Ч�ؿ�������

	struct
	{
		namechar	tollgate_name[16];			// �ؿ�����
		int			score;						// ���֣�����ʱ�����Ļ���
		int			time_limit;					// ʱ��	
		int			score_per_sec;				// ÿ���Ӷ�Ӧ����
		namechar	introduce[16];				// ���
		int			open_controller;			// ����������������������
		int			finish_controller;			// ��ɿ�����
		int			death_penlty;				// �����ͷ��ķ���
	}tollgate[20];

	struct 
	{
		int			monster_id;					// ����id
		int			score;						// �������
	}monster[20];

	struct 
	{
		int			victory_score_level;
		int			victory_task_id;
		int			failure_score_level;
		int			failure_task_id;
	}award_level[5];
};

///////////////////////////////////////////////////////////////////////////////
//�������͸������ñ�
//Added 2011-07-12.
///////////////////////////////////////////////////////////////////////////////
struct TRANSCRIPTION_CONFIG
{
	unsigned int		id;						// ID
	namechar			name[32];				// ����������

	int					max_finish_count;		// ����ÿ����ɴ�������
	int					map_id;					// ������ͼ���id
	int					open_room_item_id;		// ����������������Ҫ����Ʒid
	int					open_room_item_count;	// ����������������Ҫ����Ʒ����
	int					room_active_time;		// ��������ʱ�ޣ�ʱ�䵽��������������������
	int					player_min_level;		// ���븱����ҵĵȼ�����
	int					player_max_level;		// ���븱����ҵĵȼ�����
	UInt64				character_combo_id;		// ���븱����ҵ�ְҵ���ƣ�profession_id:0 -- 63
	UInt64				character_combo_id2;	// ���븱����ҵ�ְҵ���ƣ�profession_id:64 -- 127
	unsigned int		god_devil_mask;			// ���븱����ҵ��컯���ƣ���/��/ħ��1/2/3
	int					renascence_count;		// ���븱����ҵ�ת���������ƣ�0��ʾû������
	int					required_race;			// �������ƣ�0-���壬1-���壬 2-������
	int					required_money;			// ���븱������Ҫ��ȡ�Ľ�Ǯ
	
	struct
	{
		int		reputation_type;				// ����Ҫ����������id
		int		reputation_value;				// ����Ҫ�����͵�����ֵ
	}required_reputation[4];					// ���븱������Ҫ���������ͺ�����ֵ
	
	int					required_item_id;		// ���븱������Ҫ����Ʒid
	int					required_item_count;	// ���븱������Ҫ����Ʒ����
	int					is_item_need_consumed;	// ����Ҫ����Ʒ�Ƿ����ģ� 0�������ģ�1������
	int					min_player_num;			// ������������������
	int					max_player_num;			// ������������������
	int					invincible_time;		// ���븱�����޵еȴ�ʱ�䣬�Ӹ�����ʽ����ʱ��ʼ��ʱ
	int					wait_time_before_leave;	// �������������ĵȴ�ʱ��
	int					total_exist_time;		// �����ӿ������Զ��رյ���ʱ�䣬0����ʾû��ʱ������
	int					controller_id;			// ����ʤ������������id�������������������
	int					award_task_id;			// ����ʤ����������id
	int					forbiddon_items_id[10];	// ������Ʒid�б�
	int					forbiddon_skill_id[10];	// ���ü���id�б�

	int					map_variable_id[20];	// ������ͼȫ�ֱ���id�б� Added 2012-04-09.
	
	int					level_controller_id[10];// ��ͬ�Ѷȶ�Ӧ�Ŀ�����ID Added 2012-08-20.

	int					strategy[10];			// �����������ñ�

	int					difficulty;				// Difficulty
};


struct CHANGE_SHAPE_CARD_ESSENCE 
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·����
						
	int				require_level;				// �ȼ�����
	int				require_sect;				// ��������
	
	UInt64			character_combo_id;			// ְҵ����
	UInt64			character_combo_id2;		// ְҵ����, ��չְҵID�����ְҵ���룬Added 2011-07-14.
	int				renascence_count;			// ת������Ҫ��		
	int				require_gender;
	int				repu_index;					// �������������
	int				require_repu;				// ��������ֵ����
	int				ref_temp_id;				// ���õ�����ģ���id
	namechar		preface[16];				// ����������
	namechar		epilogue[16];				// ��������
	float			energy_decrease_speed;		// �����½��ٶ�
	int				fee_restore;				// ��Ԫ
	int				fee_refine;					// ǿ��
	int				fee_contract;				// ����
	int				fee_recover_wakan;			// �ָ�ÿ����������	

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};


struct CHANGE_SHAPE_STONE_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·����
	
	int				refine_type;				// ������ʽ
	float			ext_succeed_prob;			// ��߳ɹ�����
	float			ext_match_value;			// �������ֵ

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};


struct CHANGE_SHAPE_PROP_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				monster_id;					// ��Ӧ�ı�������id
	int				action_type;				// ������������Ӣ��or��ͨ�֣�
	int				is_replace_name;			// �Ƿ��滻��������
	int				move_type;					// �����ƶ��ķ�ʽ��1-��������Կ����ƶ���2-�޷������ƶ���3-����߶���
	float			recover_prop;				// �ܵ������ĸ�ԭ���� 
	int				change_faction;				// �������Ƿ�ı���Ӫ 0�����ı� 1���ı䣨�ı���ȡ���������ֵ��
	unsigned int	faction;					// �������������Ӫ
	unsigned int	enemy_faction;				// ������ĵж���Ӫ	
	int				shield_skill;				// ���������μ���
	int				can_use_medicine;			// �ɷ�Ӻ���ҩ
	int				can_run_helper;				// �ɷ����˺�һ	
	int				can_fight;					// �Ƿ��ս��
	int				is_fly;						// �Ƿ���������
	int				hp_cost;					// ������Ѫ
	int				mp_cost;					// ��������
	int				is_equip_invalid;			// װ���Ƿ�ʧЧ 0����ʧЧ 1��ʧЧ
	int				is_replace_prop;			// �Ƿ��滻���ﱾ������ 0�����滻��1���滻
	int				is_buff_positive;			// ����buff��Ч����0������Ч�� 1������Ч��
	int unknown_1;

};

struct ORIGINAL_SHAPE_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
	int				table_id;					// ���ι����id
	struct  
	{
		int monster_id;
		struct  
		{
			int		original_type;				// ԭ�ε�����(0:npc, 1:monster)
			int		original_id;				// ����ԭ�ε�id
			float	probability;				// �������εĸ���
		}original[3];

	}shape_map[20];
};

// ��������ñ�
struct LIFE_SKILL_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	struct  
	{
		UInt64	character_combo_id;				// ְҵ����
		UInt64	character_combo_id2;			// ְҵ����, ��չְҵID�����ְҵ���룬Added 2011-07-14.
		int		require_gender;					// �Ա�����
		int		renascence_count;				// ������������
		int		require_level;					// �ȼ�����
		int		remove_renascence;				// ʧȥ���ܷ�������Ҫ��
		int		remove_level;					// ʧȥ��������ȼ�
		int		skill_id;						// ����id
		int		skill_level;					// ���ܵȼ�
	}skill_config[32];
};

struct ARENA_DROPTABLE_CONFIG 
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
	
	int				mapid;						// ��ͼ��

	struct  
	{
		int		itemid;							// ������Ʒid
	}droptable[10];
};

struct MERCENARY_CREDENTIAL_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·����
	
	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

// ���̣�������
struct TELEPORTATION_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				max_tele_point;				// ���ܼ�¼�Ĵ��͵����������
	int				default_day;				// ��ʼĬ�ϵ���Ч����

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

// ���̳���ʯ
struct TELEPORTATION_STONE_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				day;						// ����ʯ������������			

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

// ��ϼ��ܱ༭��ȴ������
struct COMBINE_SKILL_EDIT_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
	
	struct  
	{
		int		skill_id;
		int		cool_index;
		int		cool_time;
	}edit_config[30];
};

// �ض�װ��ͨ������������������ñ�
struct UPGRADE_EQUIP_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
	
	struct  
	{
		int		equip_id;						// װ��id
		int		stone_id;						// ģ��id
		int		num;							// ����ģ������
		int		out_equip_id;					// ����������id
	}upgrade_config[200];
};

// �ض�װ��ͨ������������������ñ����£�
// ���ϵ������µ�����������������12���ⶥ���ϵĲ��ⶥ
struct UPGRADE_EQUIP_CONFIG_1
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
	
	struct  
	{
		int		equip_id;						// װ��id
		int		stone_id;						// ģ��id
		int		out_equip_id;					// ����������id
	}upgrade_config[200];
};

// �ض�װ��������ʹ�õľ�������
struct UPGRADE_EQUIP_STONE_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				type;						// ���ͣ�0��ͨ1����			
	int				level;						// �ȼ���ֻ���������Ч��+X�س�
	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID

};

///////////////////////////////////////////////////////////////////////////////
//��ʯƷ��Ʒ�����ñ�
//���ã��Ա�ʯ����һ��/��һ��Ʒ����Ʒ�ʵı�ʯid�����ñ�
///////////////////////////////////////////////////////////////////////////////
struct GEM_CONFIG
{
	unsigned int			id;				//ģ��ID
	namechar				name[32];		//���ƣ����15������
	struct
	{
		unsigned int	ordinary_gem_id;	//��ͨƷ�ʱ�ʯid
		unsigned int	perfect_gem_id;		//����Ʒ�ʱ�ʯid
	}gem_id[10];							//�ֱ��ʾ��Ʒ����ʯ����Ӧ��id

};

///////////////////////////////////////////////////////////////////////////////
//��ʯģ��
///////////////////////////////////////////////////////////////////////////////
struct GEM_ESSENCE
{
	unsigned int			id;				//ģ��ID
	namechar				name[32];		//���ƣ����15������

	int						file_matter;	//���ڵ��ϵ�ģ��·��
	int						file_icon;		//ͼ��·��
	int						file_shape_icon;//��ʯ��״ͼ��·��

	int						shape_type;		//��ʯ����״���ͣ�ĿǰΪ0 - 11����12��
	int						grade;			//��ʯƷ������Ʒ����������IDģ����
	int						quality;		//��ʯƷ�ʣ�ͬһƷ���в�ͬƷ�ʣ���ǰ0����ͨƷ�ʣ�1������Ʒ��
	unsigned int			gem_config_id;	//��ʯ��/��Ʒ����Ʒ�ʵ�����ģ��id

	//��ʯ�������ԣ������������Ժ���Ƕ����,��16��
	struct
	{
		int		addon_type;		//�������Ե����ͣ�0:�������ԣ�1:��Ƕ����
		int		addon_id;		//��������ģ��id
	}addon_property[6];			//Ŀǰ�ݶ����6*3=18����������,��Ҫע�����:Ҫ�Ͳ߻�Լ���ø������Ե���д˳��

	//��Ʒ������
	int						fee_upgrade;	//ָ�����б�ʯ��Ʒ����ʱ�۳�����Ϸ������
	unsigned int			gem_extract_config_id;	//��ʯ��ȡ��ֽ�������ģ��ID

	int						price;			// �����
	int						shop_price;		// ������

	// �ѵ���Ϣ
	int						pile_num_max;	// �ѵ�����

	// ������ʽ
	unsigned int			proc_type;		// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID

};

///////////////////////////////////////////////////////////////////////////////
//��ʯħӡ
///////////////////////////////////////////////////////////////////////////////
struct GEM_SEAL_ESSENCE
{
	unsigned int			id;				//ģ��ID
	namechar				name[32];		//���ƣ����15������

	int						file_matter;	//���ڵ��ϵ�ģ��·��
	int						file_icon;		//ͼ��·��	

	int						shape_type;		//ħӡ����״����,�ͱ�ʯ����������Ӧ��Ŀǰ��13������

	int						price;			// �����
	int						shop_price;		// ������

	// �ѵ���Ϣ
	int						pile_num_max;	// �ѵ�����

	// ������ʽ
	unsigned int			proc_type;		// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID

};

///////////////////////////////////////////////////////////////////////////////
//��ʯ�۳�
///////////////////////////////////////////////////////////////////////////////
struct GEM_DUST_ESSENCE
{
	unsigned int			id;				//ģ��ID
	namechar				name[32];		//���ƣ����15������

	int						file_matter;	//���ڵ��ϵ�ģ��·��
	int						file_icon;		//ͼ��·��
	
	int						grade;			//��ǰ��ʯ�۳���Ʒ��

	//��ǰ��ʯ�۳��Ը�Ʒ����ʯ����Ʒ����
	struct
	{
		float	ordinary_upgrade_prob;		//����ͨ��ʯ����Ʒ����
		float	perfect_upgrade_prob;		//��������ʯ����Ʒ����
	}upgrade_prob[9];						//�ֱ��Ӧ10������Ʒ���ʣ���1-2�� 2-3�� 3-4������

	int						price;			// �����
	int						shop_price;		// ������

	// �ѵ���Ϣ
	int						pile_num_max;	// �ѵ�����

	// ������ʽ
	unsigned int			proc_type;		// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID

};

///////////////////////////////////////////////////////////////////////////////
//��ʯ��ȡ���������ģ��
///////////////////////////////////////////////////////////////////////////////
struct GEM_EXTRACT_CONFIG
{
	unsigned int			id;				//ģ��ID
	namechar				name[32];		//���ƣ����15������
	
	struct
	{
		unsigned int		gem_dust_id;	//���ɵı�ʯ�۳�ID
		int					gem_dust_num;	//���ɵı�ʯ�۳�������
		float				gem_dust_prob;	//���ɸñ�ʯ�۳��ĸ���
	}gem_dust_config[3];

};

///////////////////////////////////////////////////////////////////////////////
//ͨ�õ���ģ��
//��Ҫ�ṩ������������������Ʒ�������������ֶΣ���ͨ�õ���ʹ��
///////////////////////////////////////////////////////////////////////////////
struct GENERAL_ARTICLE_ESSENCE
{
	unsigned int			id;				//ģ��ID
	namechar				name[32];		//���ƣ����15������

	int						file_matter;	//���ڵ��ϵ�ģ��·��
	int						file_icon;		//ͼ��·��

	int						price;			// �����
	int						shop_price;		// ������

	// �ѵ���Ϣ
	int						pile_num_max;	// �ѵ�����

	// ������ʽ
	unsigned int			proc_type;		// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID

};

//=============================================================================
//Added 2011-12-05.
///////////////////////////////////////////////////////////////////////////////
//���߽������������ģ��
//��Ҫ���ô�����ķ��������Լ�ÿһ��С����ķ���ʱ��Ρ�
///////////////////////////////////////////////////////////////////////////////
struct ONLINE_GIFT_BAG_CONFIG
{
	unsigned int			id;							//ģ��ID
	namechar				name[32];					//���ƣ����15������

	int						file_matter;				//�����ڵ��ϵ�ģ��·��
	int						file_icon;					//ͼ��·��

	int						award_type;					//���������ͣ�0���������1��ÿ�ս���
	int						start_time_per_day;			//ÿ�ս���ʱ�Ŀ���ʱ��
	int						end_time_per_day;			//ÿ�ս���ʱ�Ľ���ʱ��
	int						start_time_per_interval;	//�����ʱ�Ŀ���ʱ��
	int						end_time_per_interval;		//�����ʱ�Ľ���ʱ��
	int						expire_time;				//������ȡ�Ľ�ֹʱ�䣬��Ҫ���ڻ�������ͣ�ÿ�ս���ÿ��0�����

	int						renascence_count;			//ת������Ҫ��
	int						require_min_level;			//�ȼ�����
	int						require_max_level;			//�ȼ�����
	unsigned int			require_gender;				//�Ա����ƣ�0-�У�1-Ů��2-��Ů����
	UInt64					character_combo_id;			//ְҵ���ƣ�ͨ������ʵ��
	UInt64					character_combo_id2;		//ְҵ���ƣ���չ����
	int						require_race;				//�������ƣ�0-���壬1-���壬 2-������
	
	struct
	{
		int		gift_bag_id;							//С���id
		int		deliver_interval;						//С������ż������������һ��С������ź�ʼ���㣬ʱ�䵥λ����
	}small_gift_bags[16];								//���16��С���
};

///////////////////////////////////////////////////////////////////////////////
//���߽���С�������ģ��
//��Ҫ������ط�����Ʒ��id�������������Ժ�ʱ�����Ƶ�����.
///////////////////////////////////////////////////////////////////////////////
struct SMALL_ONLINE_GIFT_BAG_ESSENCE
{
	unsigned int			id;							//ģ��ID
	namechar				name[32];					//���ƣ����15������

	int						file_matter;				//�����ڵ��ϵ�ģ��·��
	int						file_icon;					//ͼ��·��

	unsigned int			open_time;					//���������Ҫ��ʱ��
	unsigned int			item_num;					//����ڵ���Ʒ����

	struct
	{
		int				item_id;						//��ƷID
		int				item_count;						//��Ʒ����
		unsigned int	is_bind;						//��Ʒ�Ƿ��
		unsigned int	effect_time;					//��Ʒ��ʱЧ����λΪ�룬0Ϊ���ã�
	}item_configs[16];									//��Ʒ���ԣ����16����Ʒ
	
	int						price;						// �����
	int						shop_price;					// ������

	// �ѵ���Ϣ
	int						pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int			proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

//Added end.
//=============================================================================
//Added 2012-03-20.
///////////////////////////////////////////////////////////////////////////////
//���ɹž��ڱ���������ģ��
//��Ҫ�����������������������ʣ��¼��б�������
///////////////////////////////////////////////////////////////////////////////
struct SCROLL_REGION_CONFIG
{
	unsigned int			id;							//ģ��ID
	namechar				name[32];					//���ƣ����15������

	int						file_matter;				//����ͼ��
	int						file_icon;					//����ͼƬ

	namechar				region_desc[200];			//�������� ���200����
	float					region_level_prob[6];		//����ȼ���ʼ���ָ��ʣ��ֱ��Ӧ0-5�ȼ�ʱ�ĸ��ʣ�ע�⣺�����ܺ�Ϊ1
	float					region_level_up_prob[5];	//����ȼ��������ʣ��ֱ��Ӧ1-5�ȼ�����������

	struct
	{
		unsigned int	item_id;						//������ƷID
		unsigned int	item_count;						//������ID����Ʒ����
		float			deliver_prob;					//���Ÿ���Ʒ�ĸ���
		int				event_id;						//��Ӧ�����¼�ID��0-��ͨ�����¼���1-9�����⽱���¼�
	}region_award_list[6][10];							//��Ӧ0-5(6��)�ȼ��Ľ����б���ÿ���ȼ����10�ֽ���

	int					region_pos;					//����λ�ã�һ��12����1-9Ϊ��ͨ����λ�ã�10-12Ϊ��������λ��
	struct
	{
		int		active_start_time;						//�����ʱ�䣬������ʱ���룬--->��������Ϊ��λ��ע���ʱ��������
		int		active_stop_time;						//�����ʱ�䣬������ʱ���룬--->��������Ϊ��λ��ע���ʱ��������
	}hide_region_active_time[3];						//�����������򣬷ֱ��ʾ������Ļʱ��Σ�[�����ʱ�䣬�����ʱ��]��������д��Σ����Ϊ3��

	namechar				hide_region_active_desc[100];	//����������������
	unsigned int			hide_region_unlock_item_id;		//���������������ID

	struct
	{
		int			event_id;							//�¼�ID��0-��ͨ�� 1-9�����⣬�߻����룬������ظ�
		int			event_icon;							//ʱ��ͼƬ
		namechar	event_desc[100];					//�¼�����
	}event_list[10];									//�¼��б������10���¼���Ϣ
};

///////////////////////////////////////////////////////////////////////////////
//���ɹž��ڱ���������ģ��
//��Ҫ������ؽ��������ID�� ��������λ�õ���Ϣ
///////////////////////////////////////////////////////////////////////////////
struct SCROLL_UNLOCK_ESSENCE
{
	unsigned int			id;							//ģ��ID
	namechar				name[32];					//���ƣ����15������

	int						file_matter;				//�����ڵ��ϵ�ģ��·��
	int						file_icon;					//ͼ��·��

	unsigned int			region_id;					//��Ӧ��Ҫ����������ID
	int						region_pos;					//��Ӧ��Ҫ����������POS��λ�ã�1-9����ͨ 10-12����������

	int						price;						// �����
	int						shop_price;					// ������

	// �ѵ���Ϣ
	int						pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int			proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

///////////////////////////////////////////////////////////////////////////////
//���ɹž��ڱ�����̽����������ģ��
//��Ҫ������ؿ�������̽����������Ϣ
///////////////////////////////////////////////////////////////////////////////
struct SCROLL_DIG_COUNT_INC_ESSENCE
{
	unsigned int			id;							//ģ��ID
	namechar				name[32];					//���ƣ����15������

	int						file_matter;				//�����ڵ��ϵ�ģ��·��
	int						file_icon;					//ͼ��·��

	int						dig_count;					//�˵��߿������ڱ�����

	int						price;						// �����
	int						shop_price;					// ������

	// �ѵ���Ϣ
	int						pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int			proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

///////////////////////////////////////////////////////////////////////////////
//ſ���������ñ�
//��Ҫ����һ�㸱���Ļ�����Ϣ�Լ�ſ�������е����� Added 2012-03-29.
///////////////////////////////////////////////////////////////////////////////
struct TOWER_TRANSCRIPTION_CONFIG
{
	unsigned int		id;						// ID
	namechar			name[32];				// ����������

	int					max_finish_count;		// ����ÿ����ɴ�������
	int					map_id;					// ������ͼ���id
	int					open_room_item_id;		// ����������������Ҫ����Ʒid
	int					open_room_item_count;	// ����������������Ҫ����Ʒ����
	int					room_active_time;		// ��������ʱ�ޣ�ʱ�䵽��������������������
	int					player_min_level;		// ���븱����ҵĵȼ�����
	int					player_max_level;		// ���븱����ҵĵȼ�����
	UInt64				character_combo_id;		// ���븱����ҵ�ְҵ���ƣ�profession_id:0 -- 63
	UInt64				character_combo_id2;	// ���븱����ҵ�ְҵ���ƣ�profession_id:64 -- 127
	unsigned int		god_devil_mask;			// ���븱����ҵ��컯���ƣ���/��/ħ��1/2/3
	int					renascence_count;		// ���븱����ҵ�ת���������ƣ�0��ʾû������
	int					required_race;			// �������ƣ�0-���壬1-���壬 2-������
	int					required_money;			// ���븱������Ҫ��ȡ�Ľ�Ǯ
	
	struct REPUTATION_ITEM
	{
		int		reputation_type;				// ����Ҫ����������id
		int		reputation_value;				// ����Ҫ�����͵�����ֵ
	}required_reputation[4];					// ���븱������Ҫ���������ͺ�����ֵ
	
	int					required_item_id;		// ���븱������Ҫ����Ʒid
	int					required_item_count;	// ���븱������Ҫ����Ʒ����
	int					is_item_need_consumed;	// ����Ҫ����Ʒ�Ƿ����ģ� 0�������ģ�1������
	int					min_player_num;			// ������������������
	int					max_player_num;			// ������������������
	int					invincible_time;		// ���븱�����޵еȴ�ʱ�䣬�Ӹ�����ʽ����ʱ��ʼ��ʱ
	int					wait_time_before_leave;	// �������������ĵȴ�ʱ��
	int					total_exist_time;		// �����ӿ������Զ��رյ���ʱ�䣬0����ʾû��ʱ������
	int					controller_id;			// ����ʤ������������id�������������������
	int					award_task_id;			// ����ʤ����������id
	int					forbiddon_items_id[10];	// ������Ʒid�б�
	int					forbiddon_skill_id[10];	// ���ü���id�б�

	int					tower_layer_count;		// ſ�������ܲ���
	int					opened_tower_layer_num;	// ſ��������������
	int					tower_pass_count;		// ſ�������ܿ���
	int					enable_skill_id[20];	// ſ�������ڶ�����ü���ID�б� ���20��
	int					property_config_id[100];// ſ������ÿ���������ñ�ID ���100��
};


///////////////////////////////////////////////////////////////////////////////
//ſ������ÿ���������ñ�
//��Ҫ����ſ�������иò��������õĻ������� Added 2012-03-29.
///////////////////////////////////////////////////////////////////////////////
struct TOWER_TRANSCRIPTION_PROPERTY_CONFIG
{
	unsigned int		id;						// ID
	namechar			name[32];				// ����������

	int					tower_layer;			// �����ñ���Ч�Ĳ�������Ӧ�ڼ���
	float				monster_gen_prob[4];	// �ϻ���ѡ���������ʱ���Գ��ֵĸ��ʣ����Σ�1��2��3��4��
	float				monster_range_center[3];// ˢ�ַ�ΧΪԲ�ģ�Բ������λ��
	int					monster_range_radius;	// ˢ�ַ�ΧΪԲ�ģ�Բ�뾶
	float				renascence_pos[3];		// ſ�������ò�ĸ����
	int					success_controller_id;	// �ﵽʤ�������������Ŀ�����ID

	struct MONSTER_ADDON_PROP
	{
		int		hp;								// Ѫ
		int		mp;								// ħ
		int		attack;							// ����
		int		defence;						// ����
		int		addon_damage;					// �����˺�
		int		damage_resistance;				// �˺�����
		int		hit;							// ��ʼ����
		int		evade;							// ��ʼ����
		int		critical_rate;					// ��ʼ����һ���� ����ǧ����
		float	critical_damage;				// ��ʼ����һ���˺�
		int		resist[6];						// ��ʼ���ԣ��ֱ��Ӧ��ѣ�Σ���������������Ĭ��˯�ߣ����ƿ���
		int		anti_critical_rate;				// ��������һ���� ����ǧ����?
		float	anti_critical_damage;			// ��������һ���˺�
		int		skill_armor_rate;				// ���ܶ���		����ǧ����?
		int		skill_attack_rate;				// ��������		����ǧ����?
	}monster_addon_property[3];					// �ϻ���ѡ�������������󣬹������ӵĸ��ӻ������ԣ��ֱ��Ӧ��������1��2��3.ע�⣺������Ϊ4ʱ��û����������
	
	int					monster_id_list[32];	// �����ID�б�
	
	struct LIFE_TIME_AWARD_ITEM
	{
		int		item_id;						// ������ƷID
		int		item_count;						// ������Ʒ����
	}life_time_award[5];						// ����������Ʒ�б�

	struct SINGLE_TIME_AWARD_ITEM
	{
		int		item_id;						// ������ƷID
		int		item_count;						// ������Ʒ����
	}single_time_award[5];						// ���ν�����Ʒ�б�
};


///////////////////////////////////////////////////////////////////////////////////////
// Ԫ��ϵͳ�����ֵ����
///////////////////////////////////////////////////////////////////////////////////////
struct RUNE_DATA_CONFIG
{
	unsigned int	id;						// id
	namechar		name[32];				// ���ƣ����15������
	
	unsigned int	id_rune_reserve1;		// 
	unsigned int	id_rune_reserve2;		// 
	unsigned int	id_rune_reserve3;		// 
	unsigned int	id_rune_reserve4;		// 
	unsigned int	exp_identify;			// �����������ӵľ���
	unsigned int	exp_return;				// ��Ԫ�������ӵľ���
	unsigned int	exp_refresh;			// ϴ���������ӵľ���
	unsigned int	exp[100];				// Ԫ��1-100����������һ������Ҫ�ľ���ֵ
	
	struct SINGLE_TIME_AWARD_ITEM
	{
		unsigned int	lv_req;			// Ԫ��1-100���������ȼ�
		unsigned int	rebount_count_req;	// Ԫ��1-100����������������
	}equip_req[100]; // Ԫ��������2-101����������������
	unsigned int	decomposeo_exp[100];	// �ֽ�ƽ����λ1-100����õľ���
	
	struct FIXED_PROPERTY
	{
		unsigned int	level;		// �ȼ�
		unsigned int	attack;		// ��������
		unsigned int	hp;			// ��Ѫ����
		unsigned int	mp;			// ��������
		unsigned int	dp;			// Ԫ������
	}fixed_property[60];			// Ԫ��ϵͳ�̶�����
	
	
	unsigned int	id_rune_identify[5];		// Ԫ�����������Ʒid
	unsigned int	id_rune_refine[5];			// Ԫ��ϴ��������Ʒid
	unsigned int	id_rune_reset[5];			// Ԫ���Ԫ������Ʒid
	unsigned int	id_rune_change_slot[5];		// Ԫ��ı��λ������Ʒid
};

///////////////////////////////////////////////////////////////////////////////////////
// Ԫ��ϵͳ����֮��ģ��
///////////////////////////////////////////////////////////////////////////////////////
struct RUNE_COMB_PROPERTY
{
	// ������Ϣ
	unsigned int	id;							// id
	namechar		name[32];					// ���ƣ����15������
	
	int				max_ids;					// ������
	unsigned int	ids[10];					// ����id1~10
	unsigned int	addons[9];					// ����װ���Բ�ͬ��С��max_ids��id�������κ�����
};


///////////////////////////////////////////////////////////////////////////////////////
// Ԫ��װ��ģ��
///////////////////////////////////////////////////////////////////////////////////////
struct RUNE_EQUIP_ESSENCE
{
	unsigned int	id;							// id
	namechar		name[32];					// ���ƣ����15������

	int				file_model;					// Ԫ���ڽ�ɫ��Χ��ʾ��ģ��·��
	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				quality;					// ����Ʒ�� 0-100
	int				hole_max;					// ����λ�� 2-5
	int				require_level;				// �ȼ�����
	int				renascence_count;			// ת������Ҫ��
	
	// ��Ǯ
	int				price;						// �����
	int				shop_price;					// ������
	
	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

///////////////////////////////////////////////////////////////////////////////
//��С�ɹ������ñ�
//��Ҫ������С������������������ Added 2012-04-19.
///////////////////////////////////////////////////////////////////////////////
struct LITTLE_PET_UPGRADE_CONFIG
{
	unsigned int			id;							//ģ��ID
	namechar				name[32];					//���ƣ����15������

	struct FEED_PET
	{
		int		feed_pet_item_id;						//������С���������
		int		gain_exp;								//����һ����С�����þ���
	}feed_pet[2];										//���������������ID��һ���������þ���ֵ

	int						default_file_model;			//��С��Ĭ������ģ��·��
	
	struct PET_UPGRADE_INFO
	{
		int		file_model;								//�õȼ�����Ӧ������·��
		int		required_exp;							//����һ���������ü�����Ҫ�ľ��飺��pet_upgrade_info_list[0].required_exp -->��ʾ��0������1�����辭��ֵ
		int		award_item;								//�ﵽ�õȼ�������ȡ�Ľ�����ƷID
	}pet_upgrade_info_list[5];							//��С������������Ϣ�б������5���ȼ�
};

///////////////////////////////////////////////////////////////////////////////
// Ԫ�������Ʒģ��
///////////////////////////////////////////////////////////////////////////////
struct RUNE_SLOT_ESSENCE
{
	unsigned int			id;				//ģ��ID
	namechar				name[32];		//���ƣ����15������
	
	int						file_matter;	//���ڵ��ϵ�ģ��·��
	int						file_icon;		//ͼ��·��	
	
	int						price;			// �����
	int						shop_price;		// ������
	
	// �ѵ���Ϣ
	int						pile_num_max;	// �ѵ�����
	
	// ������ʽ
	unsigned int			proc_type;		// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
	
};

///////////////////////////////////////////////////////////////////////////////
//ȫ�������������ñ� Added 2012-06-19.
///////////////////////////////////////////////////////////////////////////////
struct DROP_INTERNAL_CONFIG
{
	unsigned int			id;							//ģ��ID
	namechar				name[32];					//���ƣ����15������
	
	int						time_of_duration;			// ����ʱ��
	int						start_time;					// ��ʼʱ��
	int						item_id;					// ������Ʒid
	int						item_num;					// ��������
	namechar				speak_words[100];			// ��������
	namechar				remain_words[100];			// ��������
	int						remain_speak_interval;		// �����������(��)
};

///////////////////////////////////////////////////////////////////////////////
//���PK�������ñ� Added 2012-06-25.
///////////////////////////////////////////////////////////////////////////////
struct PK2012_GUESS_CONFIG
{
	unsigned int			id;						//ģ��ID
	namechar				name[32];				//���ƣ����15������
	
	int						first_id;				// �ھ�ID
	int						second_id;				// �Ǿ�ID
	int						third_id;				// ����ID
	int						guess_start_time;		// ���¿�ʼʱ��
	int						guess_end_time;			// ���½�ֹʱ��
	int						accept_award_start_time;// �콱��ʼʱ��
	int						accept_award_end_time;	// �콱��ֹʱ��
	int						champion_guess_item;	// �ھ�Ͷע����
	int						champion_guess_itemNum;	// ÿͶ1ע���ĵ�������
	int						champion_guess_award;	// �ھ��������
	float					award_back_ratio;		// ������
	int						guess_item;				// ���µ���
	int						guess_item_num;			// ���µ�����������
	int						guess_award_item;		// ��ǿ�������
	int						guess_award_item3;		// ��ǿȫ�����
	int						guess_award_item2;		// ������ǿ���
	int						guess_award_item1;		// ����һǿ���
};


///////////////////////////////////////////////////////////////////////////////
//��ײ�����������ñ� Added 2012-08-02.
///////////////////////////////////////////////////////////////////////////////
struct COLLISION_RAID_TRANSFIGURE_CONFIG
{
	unsigned int			id;						//ģ��ID
	namechar				name[32];				//���ƣ����15������

	struct TRANSFIGURE_ITEM
	{
		unsigned int		transfigure_id;			//����ID��index
		int					file_matter;			//����ģ��·��
		int					file_icon;				//ѡ����ͼ��·��
		unsigned int		mass;					//�������ɫ����
		namechar			description[100];		//�����͵�������Ϣ���100������
	}transfigure_info_list[20];						//������Ϣ�б�
};

//////////////////////////////////////////////////////////////////////////
//��̯������Ʒ����ʾ��ͬ������ 2012-8-13
//////////////////////////////////////////////////////////////////////////
struct BOOTH_FIGURE_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
	
	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·����
	int				file_model;					// ��̯����
	
	int				price;						// �����
	int				shop_price;					// ������
	
	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

//////////////////////////////////////////////////////////////////////////
// ս��buff���� 2012.8.20
//////////////////////////////////////////////////////////////////////////
struct FLAG_BUFF_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
	
	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·����

	int				npc_id;						// �ٻ�����npcid
	int				require_level;				// �ȼ�����
	int				exit_time;					// ����ʱ��(s)
	int				cool_time;					// ��ȴʱ��(s)

	int				dmg;						// ����
	int				defence;					// ����
	int				hp;							// ��Ѫ
	int				mp;							// ����
	int				extra_defence;				// �����˺�
	int				crit_rate;					// ����һ����(ǧ����)
	int				crit_damage;				// ����һ���˺�(ǧ����)
	int				anti_stunt;					// ѣ�ο���
	int				anti_weak;					// ��������
	int				anti_slow;					// ��������
	int				anti_silence;				// ��Ĭ����
	int				anti_sleep;					// ˯�߿���
	int				anti_twist;					// ���ƿ���
	int				skill_attack_rate;			// ��������(ǧ����)
	int				skill_armor_rate;			// ���ܶ���(ǧ����)
	int				cult_defense[3];			// ����ħ��(ǧ����)
	int				cult_attack[3];				// ����ħ��(ǧ����)
	
	int				price;						// �����
	int				shop_price;					// ������
	
	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

///////////////////////////////////////////////////////////////////////////////
//�Ϲ����紫�ͷ������� Added 2012-08-15.
///////////////////////////////////////////////////////////////////////////////
struct UI_TRANSFER_CONFIG
{
	unsigned int			id;						//ģ��ID
	namechar				name[32];				//���ƣ����15������
	
	struct TRANS_POINT
	{
		unsigned int		trans_id;			// ID��index
		namechar			name[16];			// ����
		unsigned int		map_id;				// Ŀ���ͼ��
		float				pos[3];				// Ŀ������
		int					fee;				// ����
	}trans_point_list[256];						// ����Ŀ����б�
};

///////////////////////////////////////////////////////////////////////////////////////
// ������Ʒģ�����ݽṹ����
// ��Ƕ��װ����
///////////////////////////////////////////////////////////////////////////////////////
struct XINGZUO_ITEM_ESSENCE
{
	unsigned int	id;							// (����)ID
	namechar		name[32];					// ����, ���15������
	
	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��
	int				file_model;					// Ԥ���ģ���ʱ���ã��������֣������񷨱�����Ʈ��������Χ��ecmģ��
	
	unsigned int	equip_mask;					// ����Ƕ��λ	
	int				reborn_count;				// ��������

	int				price;						// �����
	int				shop_price;					// ������

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

// ��������ʯ
struct XINGZUO_ENERGY_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
	
	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·����
	
	int				energy;						// ����ֵ
	
	int				price;						// �����
	int				shop_price;					// ������
	
	// �ѵ���Ϣ
	int				pile_num_max;				// ��ҩƷ�Ķѵ�����
	
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

// ��ҩ�ϲ�����
struct CASH_MEDIC_MERGE_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
	
	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·����

	int				require_level;				// �ȼ�����
	int				renascence_count;			// ת������Ҫ��
	int				type;						// ��ע����
	int				cool_time;					// ��ȴʱ�䣨���룩

	int				item_ids[20];				// ��ЧҩƷid��
	
	int				price;						// �����
	int				shop_price;					// ������
	
	// �ѵ���Ϣ
	int				pile_num_max;				// ��ҩƷ�Ķѵ�����
	
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

///////////////////////////////////////////////////////////////////////////////
//��ײ������������ Added 2012-08-25.
///////////////////////////////////////////////////////////////////////////////
struct COLLISION_RAID_AWARD_CONFIG
{
	unsigned int			id;						//ģ��ID
	namechar				name[32];				//���ƣ����15������
	
	int				daily_award_item;			// ÿ�ս�����Ʒ
	struct TRANS_POINT
	{
		unsigned int		item_id;			// ������Ʒid
		unsigned int		item_num;			// ������Ʒ����
		unsigned int		score1_num;			// ����A����
		unsigned int		score2_num;			// ����B����
		unsigned int		win_num;			// �����ʤ����
	}award_list[20];							// ����
};

///////////////////////////////////////////////////////////////////////////////
//buff�������� Added 2012-10-29.
///////////////////////////////////////////////////////////////////////////////
struct BUFF_AREA_CONFIG
{
	unsigned int			id;						//ģ��ID
	namechar				name[32];				//���ƣ����15������
	
	int						skill_ids[10];			// �������򱻻��м���id��Ӧ����һЩ��buff����
	int						transfigure_id;			// ��������id
};

///////////////////////////////////////////////////////////////////////////////
//��Ծ������ Added 2012-11-05.
///////////////////////////////////////////////////////////////////////////////
struct LIVENESS_CONFIG
{
	unsigned int			id;						//ģ��ID
	namechar				name[32];				//���ƣ����31������
	
	int						scores[15];				// ÿ�����ֵ
	int						gifts[4];				// �ĸ���� 0-25��26-50��51-75��76-100

	struct ELEMENT 
	{
		namechar			name[16];				// ���ƣ����15�����֣�ר���ڲ߻�����
		int					type_activity;			// ����	
		int					require_level;			// ����ȼ�����
		int					renascence_count;		// ת������Ҫ��
		int					type_judge;				// �ж����ͣ�����require_id���ͷ�ʽ��0-����id��1-�����ͼid
		int unknown_1;
		int					require_id;				// �ж���ɵ�����id/�����ͼid
		int unknown_2;
		int unknown_3;
	} element_list[120];
};

///////////////////////////////////////////////////////////////////////////////
// �Ϲ��¸������ñ�(��ս����)
// 2012.11.12
///////////////////////////////////////////////////////////////////////////////
struct CHALLENGE_2012_CONFIG
{
	unsigned int	id;						// ID
	namechar		name[32];				// ����
	
	struct CONFIG_TMPL
	{
		int				controler_id;			// ������������������
		int				monster_id;				// ������սĿ���ɱ����ID	
		int				monster_count_min;		// ������սĿ�������������	
		int				monster_count_max;		// ������սĿ�������������	
		int				mine_id;				// ������սĿ�꿪��ID		
		int				mine_count_min;			// ������սĿ�꿪����������	
		int				mine_count_max;			// ������սĿ�꿪����������	
		int				dead_count;				// ������սĿ������������	
		int				time_limit;				// ������սĿ��ʱ��ʱ��		
		int				medic_limit_hp;			// ������ս���ҩƷʹ����	
		int				medic_limit_mp;			// ������ս����ҩƷʹ����	
		int				controller_id_challenge;// ������սĿ����ر���ID
		int				controller_id_min;		// ������սĿ����ر�������	
		int				controller_id_max;		// ������սĿ����ر�������	
		int				controller_id_win;		// ������սĿ���Ӧ�Ŀ�����ID
		int				award_item_id;			// ������սĿ����ɽ�����ƷID
		int				win_control_id;			// ������սĿ����ɿ���������ID
	};

	CONFIG_TMPL main_config;					// ����������
	CONFIG_TMPL lv_config[10];					// ÿһ�ؿ�����
};

// ̨������ 2012-12-14
struct SALE_PROMOTION_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
	
	namechar		ui_name[16];				// ��������, 2013-04-17
	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·����

	struct ELEMENT
	{
		int		valid_time_start;			// ��Ч����ʼʱ��
		int		valid_time_end;				// ��Ч�ڽ���ʱ��
		int		get_count;					// ���ظ���ȡ����
		int unknown_1;
		int unknown_2;
		int		condition_type;				// ǰ��������������ͣ��Ժ�߻��������ʾ
		int		condition_arg1;				// ǰ���������������1
		int		condition_arg2;				// ǰ���������������2
		int		condition_arg3;
		int		condition_arg4;
		int		condition_arg5;
		int		award_item1;				// ������Ʒ1
		int		award_item1_count;			// ������Ʒ����1
		int		award_item1_valid_time;		// ������Ʒ��Ч��1
		int		award_item1_is_bind;		// ������Ʒ�Ƿ��1
		int		award_item2;				// ������Ʒ2
		int		award_item2_count;			// ������Ʒ����2
		int		award_item2_valid_time;		// ������Ʒ��Ч��2
		int		award_item2_is_bind;		// ������Ʒ�Ƿ��2
		namechar desc[100];					// �߻��������������ܰ���ͼƬ�����ӵ���Ϣ
	} element_list[20];

	int				extra_award_need_count;	// ��ȡ���⽱��������ȡ����
	int				extra_award_id;			// ���⽱��
	int				extra_award_num;		// ���⽱��
	int				extra_award_valid_time;	// ���⽱��
	int				extra_award_is_bind;	// ���⽱��

	int				price;						// �����
	int				shop_price;					// ������
	
	// �ѵ���Ϣ
	int				pile_num_max;				// ��ҩƷ�Ķѵ�����
	
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

// �������⽱������������gfx��С��Ϸ�� 2012-12-14
struct TASK_SPECIAL_AWARD_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	// gfx
	int				gfx_path[20];				// ·��
	
	// С��Ϸ
	namechar		mini_game[10][8];			// ���֣�ֻ���߻���
};

// ��Ʊ���͵��������������Ҫһ���µ�ģ�壬�ṹ��GIFT_BAG_ESSENCE��ȫ��ͬ
struct GIFT_BAG_LOTTERY_DELIVER_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
	
	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��	
	
	int				level_everLimit;			// �����ĵȼ������ޣ��ʺϷ���֮����жϣ�
	int				level_required;				// ��ǰ�ȼ�����
	int				level_upperLimit;			// ��ǰ�ȼ�����
	unsigned int	require_gender;				// �Ա����ƣ�0-�У�1-Ů��2-��Ů����
	int				renascence_count;			// ת������Ҫ��
	UInt64			character_combo_id;			// ְҵ����
	UInt64			character_combo_id2;		// ְҵ����, ��չְҵID�����ְҵ���룬Added 2011-07-14.
	unsigned int	god_devil_mask;				// ��ħ �컯������/��/ħ/��ѡ1/2/3
	unsigned int    open_time;					// ������Ҫ��ʱ��
	float			probability;				// �����ɹ��ĸ���
	unsigned int	success_disappear;			// �ɹ���������ʧ(1Ϊ�ǣ�0Ϊ��)
	unsigned int	faliure_disappear;			// ����ʧ�ܺ���ʧ(1Ϊ�ǣ�0Ϊ��)
	unsigned int	id1_object_need;			// ������Ҫ��Ʒ1
	unsigned int	id1_object_num;				// ������Ҫ��Ʒ1������
	unsigned int	id2_object_need;			// ������Ҫ��Ʒ2
	unsigned int	id2_object_num;				// ������Ҫ��Ʒ2������
	int				money_need;					// ������Ҫ��Ǯ
	unsigned int	num_object;					// �ں���Ʒ������
	
	struct 
	{
		unsigned int	id_object;				// ��Ʒid
		float			probability;			// ��Ʒ��������
		unsigned int	num_min;				// ��Ʒ����������
		unsigned int	num_max;				// ��Ʒ����������
		unsigned int	is_bind;				// ��Ʒ�Ƿ��
		unsigned int	effect_time;			// ��Ʒ��ʱЧ����λΪ�룬0Ϊ���ã�
	}gifts[16];
	
	unsigned int	normalize_group[4];
	
	int				price;						// �����
	int				shop_price;					// ������
	
	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
	
};

// ��Բ���͵Ĳ�Ʊ
struct LOTTERY_TANGYUAN_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
	
	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��
	int unknown_1;

	int				open_item;					// ����������Ʒid
	int				open_item_num;				// ����������Ʒ����
	
	int				exp_get;					// ����һ�λ�þ���
	int				exp_level[7];				// ������1-7�����辭��

	struct 
	{
		struct 
		{
			unsigned int id;
			unsigned int num;
		}gifts[4];
	}group_gifts[8];
	
	int				price;						// �����
	int				shop_price;					// ������
	
	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
	
};

// ���Ѵ����
struct GIFT_PACK_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
	
	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��
	
	unsigned int    open_time;					// ������Ҫ��ʱ��

	struct 
	{
		unsigned int id;						// ���id
		unsigned int score;						// ������Ļ�������
	}item_gifts[4];
	
	int				price;						// �����
	int				shop_price;					// ������
	
	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
	
};

enum
{
	PROP_ADD_DAMAGE,
	PROP_ADD_DEF,
	PROP_ADD_HP,
	PROP_ADD_MP,
	PROP_ADD_ATTACK,		// ����
	PROP_ADD_ARMOR,			// ����
	PROP_ADD_CRIT_RATE,		// ���� 
	PROP_ADD_CRIT_ANTI,		// ������
	PROP_ADD_CRIT_DAMAGE,	// ����
	PROP_ADD_CRIT_DAMAGE_ANTI,	// ������
	PROP_ADD_SKILL_ATTACK_RATE,	// ��������
	PROP_ADD_SKILL_ARMOR_RATE,	// ���ܶ���
	PROP_ADD_RESISTANCE_1,
	PROP_ADD_RESISTANCE_2,
	PROP_ADD_RESISTANCE_3,
	PROP_ADD_RESISTANCE_4,
	PROP_ADD_RESISTANCE_5,
	PROP_ADD_CULT_DEFANCE_1,
	PROP_ADD_CULT_DEFANCE_2,	
	PROP_ADD_CULT_DEFANCE_3,
	PROP_ADD_CULT_ATTACK_1,
	PROP_ADD_CULT_ATTACK_2,
	PROP_ADD_CULT_ATTACK_3,
	PROP_ADD_NUM = 30,
};

// ������������ԭ���ϵ���
struct PROP_ADD_MATERIAL_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
	
	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				require_num;				// ��������
	int				forge_item[3];				// ��������

	struct 
	{
		unsigned int id;						// �ϳɵ���
		float		prop;						// ����
	}forge_result[10];	

	int				price;						// �����
	int				shop_price;					// ������
	
	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

// ���������������
struct PROP_ADD_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
	
	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				material_id;				// ԭ����id����������	
	int				require_level;				// �ȼ�����
	int				renascence_count;			// ת������Ҫ��
	
	int				prop_add[PROP_ADD_NUM];		// ����
	
	int				price;						// �����
	int				shop_price;					// ������
	
	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

// �������������������ñ�
struct PROP_ADD_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				prop_add_limit[PROP_ADD_NUM];	// ��������
};

// ��ս���ñ�
struct KING_WAR_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				king_continue_item;			// �������ν���
	int				king_continue_item_num;		// ��������

	struct  
	{
		int				monser_id;				// ��ս������id
		int				hp_delta;				// �����ı���
	} monster[2];
	
	struct
	{
		int id;
		int num;
		int score;
	} king_shop[20];							// �������ֹ���ģ��

	struct
	{
		int id;
		int num;
		int score;
	} normal_shop[20];							// ���˻��ֹ���ģ��

};

struct JINFASHEN_TO_MONEY_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	struct Config
	{
		int id;					//
		int money;				//
		int reputation;			// ��ħ���컯֮ͨ�ð汾����Ҫ����������
	}jinshen[25], fashen[25];
};

struct BATTLE_201304_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	unsigned int	map_id;						// ��ͼ��
	bool			add_time_if_full;			// �����Ƿ���ʱ
	int				minite_add;					// ��ʱ����
	int				require_level;				// �ȼ�����
	int				renascence_count;			// ת������Ҫ��

	struct Monster 
	{
		int id;
		int score;
	} monster_info[5];

	int			score;				// ��ͷ������ֵ
	int			score_target;		// Ŀ�����
	int			taskid_success_human;		// ������ɷ�������
	int			taskid_success_wild;		// ������ɷ�������
	int			taskid_everyone;	// ���뽱��������
};

// ����150-160����ģ�壬�벻������������
struct RUNE2013_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
	
	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int unknown_1;
	int unknown_2;
	int unknown_3;

	int				quality;					// Ʒ��0:���� 1:�е� 2:�ߵ� 3:��Ʒ
	int				prop_type;					// �������ͣ�enum PlayerPropType
	float			add_value;					// ÿ��������ֵ
	int				self_exp;					// ���徭��

	int				price;						// �����
	int				shop_price;					// ������
	
	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����
	
	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

// 150-160�����������ñ�
struct RUNE2013_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				lv_limit;					// �ȼ�����
	int				lvup_exp[10][10];			// �����������辭�飬���ȡ��еȡ��ߵȡ���Ʒ 1-10
};

// ���轱������
struct BASHE_AWARD_CONFIG
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������
	
	int				period;						// ���ʱ��(s)
	
	struct Award
	{
		int id;
		int count;
		float prob;
	} award_info[10];
};

//����������ʱ������
struct ARENA_SEASON_TIME_CONFIG
{
	unsigned int	id;							//ID
	namechar		name[32];					//���ƣ����15������

	struct Seanson_Time
	{
		int start_time;
		int end_time;
	}season_info[4];
};

//ÿ�ܸ��˷ֵ���������
struct PERSONAL_LEVEL_AWARD_CONFIG
{
	unsigned int	id;							//ID
	namechar		name[32];					//���ƣ����15������
		
	struct personal_level_award
	{
		int				level;						//��λ
		int				inception_points;			//��ʼ��
		int				cutoff_points;				//��ֹ��
		int				week_receive_num;			//ÿ����ȡ����

		struct award_item
		{
			int item_id;							//������ƷID
			int item_num;							//������Ʒ����
			int time_limit;							//ʱ������
		}item_info[5];
	}personal_level_award_info[15];
};

//ս��������������
struct TEAM_SEASON_AWARD_CONFIG
{
	unsigned int	id;							//ID
	namechar		name[32];					//���ƣ����15������

	struct team_season_award
	{
		int				inception_rank;				//������������ʼ
		int				cutoff_rank;				//������������ֹ

		struct award_item
		{
			int item_id;							//������ƷID
			int item_num;							//������Ʒ����
			int time_limit;							//ʱ������
		}item_info[6];
	}team_season_awrd_info[15];

};

//ÿ�ܶһ��ҽ�������
struct WEEK_CURRNCY_AWARD_CONFIG
{
	unsigned int	id;							//ID
	namechar		name[32];					//���ƣ����15������
	
	struct week_currncy_award
	{
		int				inception_rank;				//������������ʼ
		int				cutoff_rank;				//������������ֹ
		int				receive_currency_id;		//����ȡ�Ķһ���ID
		int				receive_currnecy_num;		//����ȡ�Ķһ�������
		int				week_receive_num;			//ÿ�ܿ���ȡ����
		int				time_limit;					//ʱ������
	}week_currncy_awrd_info[15];

};


//Ⱦɫ��
struct COLORANT_ITEM_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��

	int				color;						// ɫϵ 0:�� 1:�� 2:�� 3:�� 4:�� 5:�� 6:�� 7:�� 8:�� 9:�� 10:�� 11:��
	int				quality;					// Ʒ��

	int				index;

	int				price;						// �����
	int				shop_price;					// ������

	int				pile_num_max;				// �ѵ�����
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
};

// List 178 COLORANT2_ITEM_ESSENCE added in between older lists which leads to shifting of the other tables upfront
struct COLORANT2_ITEM_ESSENCE {
	unsigned int id;
	namechar name[32];
	int file_matter;
	int file_icon;
	int price;
	int shop_price;
	int pile_num_max;
	int proc_type;
	int color_index_unlock;
};

//�ɻ����������ñ�
struct INTERACTION_OBJECT_ESSENCE
{
	unsigned int	id;							// ID
	namechar		name[32];					// ����, ���15������

	int				file_matter;				// ģ��·��
	int				file_icon;					// ͼ��·��

	unsigned int	main_body;					//��˭Ϊ�������
	unsigned int	client_main_body;			//�ͻ��˹ҵ�����
	unsigned int	hook[6];					//�ҵ�����Ĺҵ�ģ��
	int				max_interaction_num;		//���ɻ�������
	unsigned int	disappear;                  //���ﻥ�����޴����Ƿ���ʧ
	int				require_item_id;			//������Ҫ����Ʒ
	int				require_task_id;			//������Ҫ������
	int				require_level;				//������Ҫ������ȼ�
	int				require_gender;				//�����Ա����ƣ�0-�У�1-Ů��2-��Ů����
	UInt64			character_combo_id;			//����ְҵ����
	UInt64			character_combo_id2;		//����ְҵ����
	int				consum_item_id;				//�������ĵ���Ʒ
	int				open_time;					//����������׼��ʱ��
	int				interaction_time;			//����ʱ��
	int				finish_time;				//��������׼��ʱ��
	unsigned int	monster_id;					//���õĹ���ģ��
	int				path_id;					//����·��
	unsigned int    move_enable;				//�ƶ�����
	int				velocity;					//�ƶ��ٶ�
	unsigned int    auto_end;					//����ص��Ƿ��Զ�����
	int				new_item_id;				//��ɻ�����ת��Ϊ�»�������
};

//�ɻ�������ҵ�����
struct INTERACTION_OBJECT_HOOK_CONFIG
{
	unsigned int	id;								//ID
	namechar		name[32];						//���ƣ����15������
	
	namechar		object_hook_name[100];			//��Ʒ�ҵ�����
	namechar		character_hook_name[100];		//����ҵ�����
	int				start_task_id;					//������ʼ����������
	int				start_open_controller_id;		//������ʼ�����Ŀ�����
	int				start_close_controller_id;		//��������ʼ�رյĿ�����

	int				prepare_time;					//׼��ʱ��
	namechar		start_object_action[100];		//������ʼʱ��Ʒ���ŵĶ���
	namechar		start_character_action[100];	//������ʼʱ���ﲥ�ŵĶ���
	namechar		static_object_action[100];		//������ֹʱ��Ʒ���ŵĶ���
	namechar		static_character_action[100];	//������ֹʱ���ﲥ�ŵĶ���
	namechar		move_object_action[100];		//�����ƶ�ʱ��Ʒ���ŵĶ���
	namechar		move_character_action[100];		//��������ʱ���ﲥ�ŵĶ���
	namechar		end_object_action[100];			//����������Ʒ���ŵĶ���
	namechar		end_character_action[100];		//�����������ﲥ�ŵĶ���

	int				end_task_id;					//������������������
	int				end_open_controller_id;			//�������������Ŀ�����
	int				end_close_controller_id;		//���������رյĿ�����
	
	struct award_item
	{
		int		item_id;							//����������õ���Ʒ
		float	prop;								//�����Ʒ�ĸ���
	}item_info[5];	

	unsigned int	accident;							//������ֹ��Ϊ
														// 0x0001 �ܹ�������
														// 0x0002 �����ƶ�����
	unsigned int	showbutton;							//���������Ƿ���ʾ��ť
	namechar		buttontext[100];						//������ť������

	int				absx;								//����ʱ���ȥ���ľ�������x
	int				absy;								//����ʱ���ȥ���ľ�������y	
	int				absz;								//����ʱ���ȥ���ľ�������z
	int				relativex;							//����ʱ���ȥ�����������x
	int				relativey;							//����ʱ���ȥ�����������y 
	int				relativez;							//����ʱ���ȥ�����������z
};

struct COLORANT_CONFIG
{
	unsigned int	id;								//ID
	namechar		name[32];						//���ƣ����15������

	struct colorant_prop
	{
		int				colorant_item_id;				//Ⱦɫ��ID
		float			probability;					//����
	}colorant_info[100];
};

//���ɴ������ñ�
struct FACTION_TRANSFER_CONFIG
{
	unsigned int	id;								//ID
	namechar		name[32];						//���ƣ����15������
	
	struct location
	{
		int			region_index;					//��������
		namechar	name[16];						//��������
		int			x;								//X����
		int			y;								//Y����
		int			z;								//Z����
	}transfer_location[30];
};

//�ɽ����������ñ�
struct BUILDING_REGION_CONFIG
{
	unsigned int	id;								//ID
	namechar		name[32];						//���ƣ����15������

	struct region
	{
		namechar		name[16];					//��������
		int				centerx;					//�������ĵ�X
		int				centery;					//�������ĵ�Y
		int				length;						//���򳤶�
		int				width;						//�������
		int				region_index;				//��������
		unsigned int	region_id;					//����id
	}build_region[30];
};

struct BUILDING_ESSENCE
{
	unsigned int	id;								//ID
	namechar		name[32];						//���ƣ����15������

	int				constructing_model;				//���ڽ����еĵȼ�ģ��
	namechar		building_desc[256];				//��������

	struct update_building
	{
		int				ordinary;						//��ͨ����ͼ��
		int				hightlight;						//��������ͼ��	
		int				grass_num;						//�����ȼ���Ҫ���������
		int				mine_num;						//�����ȼ���Ҫ�Ŀ�ʯ����
		int				monster_food_num;				//�����ȼ���Ҫ����������
		int				monster_core_num;				//�����ȼ���Ҫ�Ĺֺ�����
		int				money_num;						//�����ȼ���Ҫ��Ԫ������

		struct require_building 
		{
			unsigned int building_id;					//�����ȼ���Ҫ�Ľ���
			int			 level;							//�����ȼ���Ҫ�Ľ����ȼ�
		}req_building[10];

		int				building_model;					//����ģ��
		int				task_id;						//����ʱ�ɽ�����
		int				finish_num;						//����������ɴ���

		struct add_item
		{
			int item_id;								//������ƷID
			int item_num;								//������Ʒ����
			int time;									//������Ʒʱ��
		}item[5];	
		
		int				added_grass_num;				//�����������
		int				added_grass_time;				//�������ʱ��
		int				add_mine_num;					//���ӿ�ʯ����
		int				add_mine_time;					//���ӿ�ʯʱ��
		int				add_monster_core_num;			//�����޺�����
		int				add_mosnter_core_tiem;			//�����޺�ʱ��
		int				add_mosnter_food_num;			//������������
		int				add_monster_food_time;			//��������ʱ��
		int				add_money_num;					//����Ԫ������
		int				add_money_time;					//����Ԫ��ʱ��

		int controller_open[15];								//������������ID
		int controller_close[15];								//�رտ�������ID
	}up_building[5];
};

//�ؿ�
struct REGION_BLOCK_ESSENCE
{
	unsigned int	id;								//ID
	namechar		name[32];						//���ƣ����15������

	unsigned int   building[10];					//����ģ��ID
};

//��λ���ñ�
struct PHASE_CONFIG
{
	unsigned int	id;								//ID
	namechar		name[32];						//���ƣ����15������

	unsigned int	phaselist[550];					//Ĭ����Ϣ
};

//���6v6��Ʒ�һ���
struct CROSS6V6_ITEM_EXCHANGE_CONFIG
{
	unsigned int	id;								//ID
	namechar		name[32];						//���ƣ����15������	

	int unknown_1;
	int unknown_2;
	int unknown_3;
	int unknown_4;
	int unknown_5;
	int unknown_6;
	int unknown_7;
	int unknown_8;
	
	struct cross6v6_item_exchange
	{
		int				item_id;					//��ƷID
		int				item_num;					//��Ʒ����
		int				require_currency_id;		//����һ���ID
		int				require_currnecy_num;		//����һ�������
		int				time_limit;					//ʱ������
		int exchange_count;
	} cross6v6_item_exchange_info[64];
};

//�����������ñ�
struct TRANSCRIPT_STRATEGY_CONFIG
{
	unsigned int	id;								//ID
	namechar		name[32];						//���ƣ����15������

	struct level_info
	{
		int level_type;						
		int monster[10];
	} level_list[10];
};

//�����̳����ñ�
struct FACTION_SHOP_CONFIG
{
	unsigned int	id;								//ID
	namechar		name[32];						//���ƣ����15������

	struct faction_item
	{
		int item_index;			//��Ʒ���
		int item_id;			//��Ʒid
		int build_id;			//����id
		int build_level;		//�����ȼ�
		int contribution;		//���׶ȼ۸�
	} faction_item_list[100];
};


//���ɾ���Ʒ���ñ�
struct FACTION_AUCTION_CONFIG
{
	unsigned int	id;								//ID
	namechar		name[32];						//���ƣ����15������

	struct faction_auction_item
	{
		int		item_id;				//��Ʒid
		int		baseprice;				//�׼�
		int		file_icon;				//��Ʒ��ͼ��
		int		aucitontime;			//����ʱ��
		namechar words[100];			//��������
	} faction_auction_list[100];
};

//����Ԫ���̳����ñ�
struct FACTION_MONEY_SHOP_CONFIG
{
	unsigned int	id;								//ID
	namechar		name[32];						//���ƣ����15������

	struct faction_money_item
	{
		int		 item_index;			//��Ʒ����
		int		 build_id;				//����id
		int		 build_leve;			//�����ȼ�
		namechar item_name[16];			//��Ʒ����
		int		 price;					//�۸�
		int      file_icon;				//ͼ��
		namechar itemdes[100];			//����
		int	     cooltime;				//��ȴʱ��
		namechar words[100];			//����
		int		 type;					//����
		int		 auition_item_id;		//��������Ʒid
		int      pen_controller_id;		//����������id
		int		 exp_multi;				// ���鱶��		
		int		 multi_exp_time;		// �౶����ʱ��	
	}faction_money_item_list[140];
};

//���Ͽ�
struct COMPOUND_MINE_ESSENCE
{
	unsigned int	id;								//ID
	namechar		name[32];						//���ƣ����15������

	int				file_mine[3];					//��·��
	int				count;							//�ɲɼ�����
	int				file_mine_spe;					//��4·��
};

//��ʼȾɫ�����ñ�
struct COLORANT_DEFAULT_CONFIG
{
	unsigned int	id;								//ID
	namechar		name[32];						//���ƣ����15������

	int				colorant[10];					//Ⱦɫ��ID			
};


struct SIGN_IN_CONFIG
{
	unsigned int	id;
	namechar		name[32];
	int				sign_start_time;
	int				sign_end_time;
	int				exchange_end_time;
	int				sign_score;
	int				sign_package;

	struct package_list
	{
		int			package_id;
		int			score_consume;
		int			exchange_count;
	} package_list_info[4];
};


struct BABY_ESSENCE
{
	unsigned int	id;
	namechar		name[32];
	int				file_model;
	int file_model_reserved_1;
	int				initweapon_model;
	int				initbody_model;
	int				init_foot_model;
	int				init_hat_model;
	int				file_model_normal;
	int				file_model_tetra;
	int				file_model_left;
	int				file_model_back;
	int				file_model_changdao;
	int				file_model_judao;
	int				file_model_gong;
	int				file_model_sha_back;
	int				file_model_sha_01;
	int				file_model_sha_02;
	int file_model_reserved_2;
	int file_model_reserved_3;
	int file_model_reserved_4;
	int file_model_reserved_5;
	int				gender;
	int				sect_mask;
	float			attack_range;
	int				skill_id_1;
	int				skill_id_2;
	int				skill_id_3;
	int				skill_id_4;
	int				skill_id_5;
	int				skill_id_6;
	int				skill_id_7;
	int				skill_id_8;
	int				skill_id_9;
	int				skill_id_10;
	int				skill_id_11;
	int				skill_id_12;
	int				skill_id_13;
	int				skill_id_14;
	int				skill_id_15;
	namechar		description[100];
};


struct CLASS_BABY_RELATION_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	struct class_baby_relation
	{
		int			character_class_id;
		int			malerole_baby_id;
		int			femalerole_baby_id;
	} class_baby_relation_list[100];
};


struct BABY_CONFIG
{
	unsigned int	id;
	namechar		name[32];
	int				pick_range;
	int				name_color;
	float			sight_range;
	float			aggro_range;
	int				hunger_speed;
	int				stand_mode;
	float			move_speed;
	int				percopperhp;
	int				persilverhp;
	int				hp_limit;
	int				percoppermp;
	int				persilvermp;
	int				mp_limit;

	struct skill_deity_reg
	{
		int			skill_id[10];
	} skill_deity_reg_info[15];
};


struct BABY_PROPERTY_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	int				perception_ratio[100];
	int				charm_ratio[100];

	struct deity_ratio
	{
		int			attack_ratio;
		int			defence_ratio;
		int			hit_ratio;
		int			evade_ratio;
		int			crit_damage_ratio;
		int			crit_ratio;
	} deity_ratio_info[100];
};


struct BABY_UPGRADE_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	int				upqrade_exp_req[10];

	struct baby_upqrade
	{
		int			exp_req;
		int			success_prob;
	} baby_upqrade_list[100];
};

struct BABY_FASHION_ESSENCE
{
	unsigned int	id;
	namechar		name[32];

	int				model_path_id;
	int				icon_path_id;
	int				equip_mask;
	int unknown_1;
	int				equip_location;
	int				action_type;

	int				file_model_normal;
	int				file_model_tetra;
	int				file_model_left;
	int				file_model_back;
	int				file_model_changdao;
	int				file_model_judao;
	int				file_model_gong;
	int				file_model_sha_back;
	int				file_model_sha_01;
	int				file_model_sha_02;
	int file_model_reserved_1;

	int				require_gender;
	int				require_level;
	int				sect_mask;
	int				title_require;

	int				hp;
	int				mp;
	int				min_dmg;
	int				max_dmg;
	int				defence;
	int				attack;
	int				armor;
	float			attack_range;

	int				id_addon_1;
	int				id_addon_2;
	int				id_addon_3;
	int				id_addon_4;
	int				id_addon_5;

	int				fee_install_sstone;
	int				fee_uninstall_sstone;

	int				sell_price;
	int				buy_price;
	int				pile_num_max;
	int				proc_type;
};


struct BABY_TOY_ESSENCE
{
	unsigned int	id;
	namechar		name[32];

	int				model_path_id;
	int				icon_path_id;
	int				equip_mask;
	int				init_time_points;
	int				toy_level;

	int				added_attack_ratio;
	int				added_defense_ratio;
	int				added_hit_ratio;
	int				added_evade_ratio;
	int				added_crit_damage_ratio;
	int				added_crit_ratio;

	int				sell_price;
	int				buy_price;
	int				pile_num_max;
	int				proc_type;
};


struct BABY_BOOK_ESSENCE
{
	unsigned int	id;
	namechar		name[32];

	int				model_path_id;
	int				icon_path_id;
	int				equip_mask;
	int				init_time_points;
	int				toy_level;

	int				added_prop_ratio;

	int				sell_price;
	int				buy_price;
	int				pile_num_max;
	int				proc_type;
};


struct MATRIX_CARD_ESSENCE
{
	unsigned int	id;
	namechar		name[32];

	int				model_path_id;
	int				icon_path_id;
	int				normal_icon;
	int				big_icon;
	int				quality;

	struct base_addon
	{
		int			addon_id;
		float		coefficient;
	} base_addon[3];

	struct addon
	{
		int			id;
		int			level;
	} addons[6];

	struct intimate
	{
		namechar	name[16];   // wstring:32
		int			id;
		int			active_id[3];
	} intimates[4];

	int				sell_price;
	int				buy_price;
	int				pile_num_max;
	int				proc_type;
};


struct MATRIX_EQUIP_ESSENCE
{
	unsigned int	id;
	namechar		name[32];

	int				model_path_id;
	int				icon_path_id;
	int				normal_icon;
	int				big_icon;
	int				quality;

	struct base_addon
	{
		int			addon_id;
		float		coefficient;
	} base_addon[3];

	struct addon
	{
		int			id;
		int			level;
	} addons[6];

	int				sell_price;
	int				buy_price;
	int				pile_num_max;
	int				proc_type;
};


struct MATRIX_EXP_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	struct config
	{
		namechar	name[16];   // wstring:32
		int			num;
		int			exp;
	} config[7];
};


struct MATRIX_COLLECT_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	struct matrix_set
	{
		namechar	name[16];   // wstring:32
		int			addon;
		int			cards[30];
	} matrixsets[4];
};


struct MATRIX_CARD_UPGREAD_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	int				exp[59];
	float			val[4];
};


struct MATRIX_COMBINE_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	struct material
	{
		int			id;
		int			num;
	};

	struct recipe
	{
		int			card;
		int			decompose_id;
		int			decompose_num;
		material	materials[2];
	} recipes[120];
};


struct BABY_FOOD_ESSENCE
{
	unsigned int	id;
	namechar		name[32];

	int				model_path_id;
	int				icon_path_id;
	int				equip_mask;
	int				init_fatique_value;

	int				sell_price;
	int				buy_price;
	int				pile_num_max;
	int				proc_type;
};


struct BABY_TITLE_ESSENCE
{
	unsigned int	id;
	namechar		name[32];

	int				model_path_id;
	int				icon_path_id;
	int				type;
	int				title_id;

	int				sell_price;
	int				buy_price;
	int				pile_num_max;
	int				proc_type;
};


struct BABY_TITLE_LIST
{
	unsigned int	id;
	namechar		name[32];

	int				title_list[100];
};


struct VEHICLE_ENHANCE_ESSENCE
{
	unsigned int	id;
	namechar		name[32];

	int				model_path_id;
	int				icon_path_id;

	float			increased_speed;
	int				moment;

	int				sell_price;
	int				buy_price;
	int				pile_num_max;
	int				proc_type;
};


struct TALENT_SCROLL_ESSENCE
{
	unsigned int	id;
	namechar		name[32];

	int				model_path_id;
	int				icon_path_id;
	int				big_icon;

	int unknown_1;
	int unknown_2;

	int				talent_scroll_config;
	int				position;

	int				decomposition_id;
	int				decomposition_num;

	int				refine_id;
	int				refine_fee;

	int unknown_3;
	int unknown_4;

	int				refine_num[10];

	int				sell_price;
	int				buy_price;
	int				pile_num_max;
	int				proc_type;
};


struct TALENT_SCROLL_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	float			prop_prob[4];      // Prop_2_Prob ... Prop_5_Prob
	float			spe_prob;

	struct property_info
	{
		int			prop_id;
		float		max_value;
		float		min_value;
		float		add_value;
		float		prob;
	} property[32];

	float			level_prob[5];

	int				profession_addon_skill[18];
};


struct TALENT_SCROLL_SKILL_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	struct skill_info
	{
		int			id;
		int			level;
		float		prob;
		namechar	desc[256];
	} skill[16];
};


struct SKILL_REPLACE_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	struct skill_replace_info
	{
		int			original_skill_id;
		int			addon_skill_id;
	} skill[168];
};


struct HILL_TRANSCRIPTION_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	int				max_finish_count;
	int				map_id;

	int				open_room_item_id;
	int				open_room_item_count;

	int				room_active_time;

	int				player_min_level;
	int				player_max_level;

	long long		character_combo_id;
	long long		character_combo_id_2;

	int				god_devil_mask;
	int				renascence_count;
	int				required_race;

	int				required_money;

	struct reputation
	{
		int			type;
		int			value;
	} required_reputation[4];

	int				required_item_id;
	int				required_item_count;
	int				is_item_need_consumed;

	int				min_player_num;
	int				max_player_num;

	int				invincible_time;
	int				wait_time_before_leave;
	int				total_exist_time;

	int				controller_id;
	int				award_task_id;

	int				forbidden_items_id[10];
	int				forbidden_skill_id[10];

	int				map_variable_id[20];
	int				level_controller_id[10];

	int				strategy[10];

	int				difficulty;
};


struct CONSUME_AWARD_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	struct award_item_info
	{
		int			item_id;
		int			item_num;
	};

	struct consume_award_info
	{
		int				amount;
		award_item_info	items[8];
	} consume_award_info[7];
};


struct MULTI_TOWER_PROPERTY_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	int				tower_layer;

	float			monster_gen_prob[4];
	float			monster_range_center[3];
	int				monster_range_radius;

	float			renascence_pos[3];

	int				success_controller_id;

	struct monster_addon_property
	{
		int			hp;
		int			mp;
		int			attack;
		int			defence;
		int			addon_damage;
		int			damage_resistance;
		int			hit;
		int			evade;
		int			critical_rate;
		float		critical_damage;
		int			resist[6];
		int			anti_critical_rate;
		float		anti_critical_damage;
		int			skill_armor_rate;
		int			skill_attack_rate;
	} monster_addon_property[3];

	int				monster_id_list[32];

	struct award_item
	{
		int			item_id;
		int			item_count;
	};

	award_item		life_time_award[5];
	award_item		single_time_award[5];
};


struct RAID18V18_AWARD_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	int				first_win_award;

	struct award_info
	{
		int			item_id;
		int			item_num;
		int			score_1_num;
		int			score_2_num;
		int			win_num;
		int			time_limit;
		int			num_limit;
	} award_list[20];
};


struct EGGS_GIFT_BAG_ESSENCE
{
	unsigned int	id;
	namechar		name[32];

	int				model_path_id;
	int				icon_path_id;

	int				level_everlimit;
	int				level_required;
	int				level_upperlimit;
	int				require_gender;
	int				renascence_count;

	long long		character_combo_id;
	long long		character_combo_id_2;

	int				god_devil_mask;
	int				open_time;
	float			probability;
	int				success_disappear;
	int				faliure_disappear;

	struct object_need
	{
		int			object_need;
		int			object_num;
	} object_need[2];

	int				money_need;
	int				num_object;

	struct gift_info
	{
		int			id_object;
		float		probability;
		int			num_min;
		int			num_max;
		int			is_bind;
		int			effect_time;
	} gifts[16];

	int				normalize_group[4];

	int				world_id;

	struct position
	{
		float		x;
		float		y;
		float		z;
	} pos[2];

	int				logic;
	int				type;

	struct time_info
	{
		int			year;
		int			month;
		int			day;
		int			hour;
		int			min;
		int			wday;
	} time_info[2];

	int				spe_award_1;
	float			factor;
	namechar		spe_words_1[100];
	int				spe_award_2;
	namechar		spe_words_2[100];

	namechar		award_words[6][100];

	int				sell_price;
	int				buy_price;
	int				pile_num_max;
	int				proc_type;
};


struct ITEM_TRADE2_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	struct required_item_info
	{
		int			required_item;
		int			required_item_quantity;
	};

	struct trade_item_info
	{
		int					item_id;
		int					item_quantity;
		required_item_info	items[3];
	};

	struct trade_page
	{
		namechar			title[8];	// wstring:16
		trade_item_info		item[30];
	} trade_page[2];

	int				dialog_id;
};


struct GUESS_ESSENCE
{
	unsigned int	id;
	namechar		name[32];

	int				model_path_id;
	int				icon_path_id;

	int				sell_price;
	int				buy_price;
	int				pile_num_max;
	int				proc_type;
};


struct ELITE_GAME_RESULT_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	int				result;
};


struct ELITE_GAME_AWARD_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	int				deliver_award_time;
	int				guess_end_time;

	struct award_item
	{
		int			item_id;
		int			item_num;
	};

	struct award_info
	{
		int			start;
		int			end;
		award_item	item[20];
	} award_info[6];
};


struct TITLE_ITEM_ESSENCE
{
	unsigned int	id;
	namechar		name[32];

	int				model_path_id;
	int				icon_path_id;

	int				title;
	namechar		speak[100];
	int unknown_1;

	int				sell_price;
	int				buy_price;
	int				pile_num_max;
	int				proc_type;
};


struct SOUL_ENHANCE_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	int				per_point_soul_req;

	float			props_list[24];

	int				level_limit;

	struct level_up_info
	{
		int			points_req;
		float		success_prob;
		float		failed_return_ratio;
	} level_up_list[50];
};


struct SOUL_EXCHANGE_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	struct exchange_info
	{
		int			item_id;
		int			soul_req;
	} exchange_list[10];
};


struct SOUL_DROP_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	int				monster_id;

	struct soul_drop_info
	{
		int			min_count;
		int			max_count;
		float		probability;
		int			value;
	} soul_drop_list[4];
};


struct DAILY_SIGN_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	int				start_time;

	struct award_info
	{
		int			item_id;
		int			count;
	} awards[120];

	int				cost_list[30];
};

struct RED_PACKETS_ESSENCE
{
	unsigned int	id;
	namechar		name[32];
	unsigned int	file_matter;
	unsigned int	file_icon;
	unsigned int	cooltime;
	unsigned int	min_money;
	unsigned int	max_money;
	unsigned int	min_gold_coin;
	unsigned int	max_gold_coin;
	unsigned int	min_player;
	unsigned int	max_player;
	unsigned int	summon_npc_id;
	unsigned int	buff_id;
	unsigned int	buff_level;
	unsigned int	file_gfx_1;
	unsigned int	file_gfx_2;
	unsigned int	price;
	unsigned int	shop_price;
	unsigned int	pile_num_max;
	unsigned int	proc_type;
};

struct FORBID_COMMON_SKILL_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct forbid_skill_list
	{
		unsigned int skill_id[9];
	} skill_list[18];
};

struct FORBID_GODEVIL_SKILL_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct forbid_skill_list
	{
		unsigned int skill_id[15];
	} skill_list[18];
};

struct FORCE_CD_SKILL_CONFIG 
{
	unsigned int id;
	namechar name[32];

	struct force_cd_skill_list
	{
		unsigned int skill_id[5];
	} skill_list[18];
};

struct TALENT_SCROLL_REFINE_ESSENCE
{
	unsigned int id;
	namechar name[32];
	int file_matter;
	int file_icon;
	int refine_type;
	int refine_type_mask;
	int price;
	int shop_price;
	int pile_num_max;
	int proc_type;
};

struct FACTION_BASE_ACTIVITY_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct activity_list
	{
		namechar title[8];
		unsigned int build_id;
		unsigned int build_level;
		int res_type;
		int res_count;
		unsigned int controller_id;
		unsigned int region_index;
	} activity_list[4];
};

struct AMULET_ESSENCE
{
	unsigned int id;
	namechar name[32];
	unsigned int file_matter;
	unsigned int file_icon;
	namechar title[16];
	
	UInt64 character_combo_id;
	UInt64 character_combo_id_2;

	int require_gender;
	unsigned int require_level;
	
	unsigned int sect_1;
	unsigned int sect_2;
	unsigned int sect_3;
	unsigned int sect_4;

	unsigned int require_race;
	unsigned int require_level_2;
	unsigned int require_ascenstion_count;

	unsigned int fee_estone;
	unsigned int estone_id;
	unsigned int config_id;

	unsigned int price;
	unsigned int shop_price;
	unsigned pile_num_max;
	unsigned proc_type;

};

struct AMULET_CONFIG
{
	unsigned int id;
	namechar name[32];

	float prop_probability[4];

	struct base_prop_list
	{
		unsigned int prop_id;
		float max_value;
		float min_value;
		float add_value;
		float probability;
	} base_prop_list[12];

	struct hide_prop_list
	{
		unsigned int level;
		struct props
		{
			unsigned int prop_id;
			float probability;
			float value;
		} props[10];
		

	} hide_prop_list[5];

	float level_probability[5];
	int hole_level[8];
};

struct AMULET_STONE_ESSENCE
{
	unsigned int id;
	namechar name[32];
	unsigned int file_matter;
	unsigned int file_icon;
	unsigned int price;
	unsigned int shop_price;
	unsigned pile_num_max;
	unsigned int proc_type;
};

struct SPECIAL_VIP_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct card_list
	{
		unsigned int money;
		unsigned int open_day;
		unsigned int gift_bag_id;
		unsigned int exp_per_day;
		unsigned int exp_added;
		unsigned int free_helper;
		unsigned int no_exp_loss;

		struct buff_list
		{
			unsigned int skill_id;
			unsigned int skill_level;
		} buff_list[3];
	} card_list[3];

	unsigned int remote_bag_money;
	unsigned int remote_bag_day;
	unsigned int remote_exp_per_day;
	unsigned int free_fly_money;
	unsigned int free_fly_day;
	unsigned int fly_exp_per_day;
	
	struct item_list
	{
		unsigned int item_id;
		unsigned int money;
		unsigned int exp_required;
	} item_list[3];

	unsigned int max_exp;
};

struct CHARGE_AWARD_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct feedback_list
	{
		struct item_list
		{
			unsigned int score_min;
			unsigned int item_id;
			unsigned int item_num;
			unsigned int indate;
		} item_list[24];

	} feedback_list[6];

	struct exchange_list
	{
		struct item_exchange_list
		{
			unsigned int score_required;
			unsigned int item_id;
			unsigned int item_num;
			unsigned int indate;
			unsigned int unknown;
		} item_exchange_list[24];
	} exchange_list[6];

	unsigned int version;
	unsigned int activity_open;
	unsigned int unknown[6];

	unsigned int start_time;
	unsigned int end_time;
	
	float score_multiplier;

	struct extra_item_list
	{
		unsigned int score_min;
		unsigned int score_max;
		unsigned int item_id;
		unsigned int item_num;
		unsigned int indate;
	} extra_item_list[4];

	unsigned int extra_award_open;
	unsigned int extra_award_start_time;
	unsigned int extra_award_end_time;
};

struct NEW_TOWNSCROLL_ESSENCE
{
	unsigned int id;
	namechar name[32];
	unsigned int file_matter;
	unsigned int file_icon;
	unsigned int use_time;
	unsigned int price;
	unsigned int shop_price;
	unsigned int pile_num_max;
	unsigned int proc_type;
};

struct EQUIPMENT_RANDOMEXT_CONFIG
{
	unsigned int id;
	namechar name[32];

	float prop_probability[6];

	struct base_prop_list
	{
		unsigned int prop_id;
		float max_value;
		float min_value;
		float add_value;
		float probability;
	} base_prop_list[12];

	float level_probability[5];
};

struct INDISCRIMINATION_BATTLE_CONFIG
{
	unsigned int id;
	namechar name[32];
	
	unsigned int max_repeats;
	unsigned int need_win_num[48];
	unsigned int char_add_fee[48];
	unsigned int char_mask_fee[8];

	namechar player_name[16][80];

	unsigned int first_win_award_id;

	struct award_list
	{	
		unsigned int item_id;
		unsigned int item_num;
		unsigned int score;
		unsigned int time_limit;
	} award_list[12];

};

struct GROUPRAID_EVALAWARD_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct eval
	{
		unsigned int max;
		unsigned int min;
	} eval[4];

	unsigned int award_id[6];

	struct raid_level
	{
		unsigned int boss_id;
		unsigned int evalid;
		unsigned int timeid;
		unsigned int controller_id;
	} raid_level[10];
};

struct EQUIPMENT_IDENTIFY_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct identify_config
	{
		unsigned int equipment_id;
		UInt64 character_combo_id;
		UInt64 character_combo_id_2;
		unsigned int require_gender;
		unsigned int equip_prod_id;
	} identify_config[150];
};

struct HEADFRAME_ESSENCE
{
	unsigned int id;
	namechar name[32];

	unsigned int file_matter;
	unsigned int icon_matter;
	
	unsigned int time;
	unsigned int price;
	unsigned int shop_price;
	unsigned int pile_num_max;
	unsigned int proc_type;
	
	struct file
	{
		unsigned int head_frame;
		unsigned int head_frame_gfx;

	} file[4];
};

struct CONVERT_EQUIP_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct item_list
	{
		unsigned int equip_require_id;
		unsigned int stone_id;
		unsigned int stone_amount;
		unsigned int output_equip_id;
	} item_list[200];
};

struct AUSPICIOUS_HALL_CONFIG
{
	unsigned int id;
	namechar name[32];

	unsigned int unknown[324];
};

struct DIVINE_ACCESSORY_IMBUE_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct item_list
	{
		unsigned int pstone_id;
		unsigned int equipment_id;
	} item_list[50];
};

struct LEVEL_UP_REWARD_CONFIG
{
	unsigned int id;
	namechar name[32];
	
	struct list_levels
	{
		unsigned int level_required;
		unsigned int unknown;

		struct awards
		{	
			unsigned int award_id;
			unsigned int award_amount;
			unsigned int time_limit;
		} awards[8];
	} list_levels[31];
};

struct NEW_BOX_ESSENCE
{
	unsigned int id;
	namechar name[32];
	unsigned int file_matter;
	unsigned int file_icon;

	unsigned int required_level;
	unsigned int unknown_2;
	unsigned int unknown_3;
	unsigned int required_gender;
	unsigned int unknown_5;
	UInt64 character_combo_id_1;
	UInt64 character_combo_id_2;
	unsigned int god_devil_mask;
	unsigned int unknown_9;

	struct gifts
	{
		unsigned int gift_id;
		unsigned int gift_amount;
		unsigned int gift_bind;
		unsigned int time_limit;
	} gift[60];

	unsigned int unknowns[102];
	unsigned int price;
	unsigned int shop_price;
	unsigned int pile_num_max;
	unsigned int proc_type;
};

struct NEW_FASHION_PACK_ESSENCE
{
	unsigned int id;
	namechar name[32];

	unsigned int file_matter;
	unsigned int file_icon;
	unsigned int item_duration;
	unsigned int bind_items;

	struct options
	{
		UInt64 character_combo_id_1;
		UInt64 character_combo_id_2;
		unsigned int require_gender;
		unsigned int require_ascension;
		unsigned int require_min_level;
		unsigned int require_max_level;

		struct awards
		{
			unsigned int award_id;
			unsigned int award_amount;
		} award[10];
		
	} option[36];

	unsigned int price;
	unsigned int shop_price;
	unsigned int pile_num_max;
	unsigned int proc_type;
};

struct NEW_CRAFTING_CONFIG
{
	unsigned int id;
	namechar name[32];
	unsigned int recipe_id[62];
};

struct FAIRYHOUSE_GENERAL_CONFIG
{
	unsigned int id;
	namechar name[32];
	
	unsigned int unknown[11];

	struct props
	{
		namechar prop_name[16];
		unsigned int prop_id;
		unsigned int prop_level;
	} props[64];

	struct more_props
	{
		namechar more_prop_name[16];
		unsigned int unknown_1;
		unsigned int unknown_2;
		unsigned int unknown_3;
		unsigned int unknown_4;
	} more_props[3];
};

struct FAIRYHOUSE_PROBABILITY_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct props
	{
		unsigned prop_id;
		float probability;
	} props[128];

	float unknown_1;

	struct props_2
	{
		unsigned int prop_id;
		float probability;
	} props_2[16];
};


struct FAIRYHOUSE_BABY_TRAIT_CONFIG
{
	unsigned int id;
	namechar name[32];
	unsigned int item_id_1;
	float unknown_2;
	float unknown_3;
	unsigned int unknown_4;
	unsigned int item_id_2;
	float unknown_6;
	unsigned int unknown_7;
};

struct FAIRYHOUSE_PET_TRAIT_CONFIG
{
	unsigned int id;
	namechar name[32];

	unsigned int spell_id;
	unsigned int spell_level;
	float probability_1;
	unsigned int spell_level_2;
	float probability_2;
	unsigned int unknown_1;
};

struct FAIRYHOUSE_CHALLENGE_CONFIG
{
	unsigned int id;
	namechar name[32];

	unsigned int unknown[10];

	unsigned int transcription_id;

	unsigned int unknown_11;

	unsigned int controller_id; // not kind sure is this
	unsigned int controller_exist_time; // not sure is this.
	unsigned int unknown_12;
	unsigned int unknown_13;
	unsigned int unknown_14;
	unsigned int unknown_15;
	unsigned int unknown_16;
	unsigned int unknown_17;

	struct awards
	{
		unsigned int award_id;
		unsigned int award_amount;
	} awards[8];

	unsigned int unknown_18;
	unsigned int unknown_19;
	unsigned int unknown_20;
	float unknown_probability_1;
	unsigned int unknown_21;
	float unknown_probability_2;
	unsigned int unknown_22;
	float unknown_probability_3;
	unsigned int unknown_23;
	float unknown_probability_4;
	unsigned int unknown_24;
};

struct FAIRYHOUSE_BABY_GEAR_ESSENCE
{
	unsigned int id;
	namechar name[32];

	unsigned int file_matter;
	unsigned int file_icon;
	unsigned int shorten_challenge_time;
	unsigned int unknown_1;
	unsigned int pet_type; // 10: Random, 2: Luna, 1: Sola, 7: Terro, 9: Logi, 8: Nestun, 6: Magmas, 5: Flowen, 3: Auris, 4: Arborik
	float probability;
	unsigned int price;
	unsigned int shop_price;
	unsigned int pile_num_max;
	unsigned int proc_type;
};

struct FAIRYHOUSE_PET_GEAR_ESSENCE
{
	unsigned int id;
	namechar name[32];

	unsigned int file_matter;
	unsigned int file_icon;

	unsigned int pet_trait_config_id;
	float pet_trait_probability;

	unsigned int price;
	unsigned int shop_price;
	unsigned int pile_num_max;
	unsigned int proc_type;
};

struct FAIRYHOUSE_TRAIT_LEARN_ESSENCE
{
	unsigned int id;
	namechar name[32];
	unsigned int file_matter;
	unsigned int file_icon;

	unsigned int unknown[9];
	unsigned int pet_trait_config_id;
	unsigned int price;
	unsigned int shop_price;
	unsigned int pile_num_max;
	unsigned int proc_type;
};

struct PREPAID_BOOK_ESSENCE
{
	unsigned int id;
	namechar name[32];
	namechar window_title[16];
	unsigned int file_matter;
	unsigned int file_icon;
	unsigned int unknown_1;
	unsigned int unknown_2;
	unsigned int unknown_3;
	UInt64 combo_character_id_1;
	UInt64 combo_character_id_2;
	namechar description_1[100];
	unsigned int unknown_4;
	unsigned int unknown_5;
	namechar description_2[250];
	namechar description_3[250];
	namechar description_4[250];
	namechar description_5[250];
	namechar description_6[100];

	unsigned int unknown_6;
	unsigned int price;
	unsigned int shop_price;
	unsigned int pile_num_max;
	unsigned int proc_type;
};

struct TOKEN_BAG_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct items
	{
		namechar item_title[16];
		unsigned int unknown;
		unsigned int item_1;
		unsigned int item_2;
		unsigned int unknown_1;
		unsigned int unknown_2;
		unsigned int unknown_3;
	} items[100];
};

struct TIMELIMIT_BEAD_ESSENCE
{
	unsigned int id;
	namechar name[32];
	unsigned int file_matter;
	unsigned int file_icon;
	float add_value;
	unsigned int start_time;
	unsigned int end_time;
	unsigned int unknown_1;
	unsigned int min_level;
	unsigned int max_level;
	unsigned int unknown_2;
	unsigned int unknown_3;
	unsigned int unknown_4;
	unsigned int price;
	unsigned int shop_price;
	unsigned int pile_num_max;
	unsigned int proc_type;
};

struct EXTRA_TIMELIMIT_BEAD_ESSENCE
{
	unsigned int id;
	namechar name[32];
	unsigned int file_matter;
	unsigned int file_icon;
	unsigned int start_time;
	unsigned int end_time;
	unsigned int min_level;
	unsigned int max_level;
	unsigned int unknown_1;
	unsigned int level_up;
	unsigned int unknown_2;
	unsigned int price;
	unsigned int shop_price;
	unsigned int pile_num_max;
	unsigned int proc_type;
};

struct CONSTELLATIONEXP_CONFIG
{
	unsigned int id;
	namechar name[32];
	float unknown_prob_1;
	float unknown_prob_2;
	float unknown_prob_3;
	float unknown_prob_4;
	float unknown_prob_5;
	float unknown_prob_6;
	float unknown_prob_7;
	float unknown_prob_8;
	float unknown_prob_9;
	float unknown_prob_10;
	float unknown_prob_11;
	float unknown_prob_12;

	struct level
	{
		unsigned int level;
		float prob_1;
		float prob_2;
		float prob_3;
		float prob_4;
		float prob_5;
		float prob_6;
		float prob_7;
		float prob_8;
	} level[49];

	float unknown_2_prob[10];
};

struct CONSTELLATION_SKILL_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct list
	{
		float list_probability;
		struct skills
		{
			unsigned int skill_id;
			float probability;
		} skill[3];
		
	} list[10];
};

struct CONSTELLATION_CONFIG
{
	unsigned int id;
	namechar name[32];
	float unknown[131];
	unsigned int pre_star_config[12];
	unsigned int star_id[24];
};

struct CONSTELLATION_STARS_ESSENCE
{
	unsigned int id;
	namechar name[32];
	unsigned int file_matter;
	unsigned int file_icon;
	unsigned int file_orb_icon;
	unsigned int constellationexp_config_id;
	unsigned int star_type;
	unsigned int slot_type;
	unsigned int price;
	unsigned int shop_price;
	unsigned int pile_num_max;
	unsigned int proc_type;
	unsigned int constellation_skill_config[18]; // I assume this will grow with newer classes.
};

struct CONSTELLATION_SKILL_RELATION_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct skill_relations
	{
		unsigned int normal_skill_id;
		unsigned int upgraded_skill_id;
	} relations[306];
};

struct TALENT_SCROLL_TRIGGER_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct tomes
	{
		unsigned int tome_book_id;
		unsigned int useable_on_star;
		unsigned int increase_reforge_limit;
	} tomes[4];
};

struct EVENT_REWARD_SET_CONFIG
{
	unsigned int id;
	namechar name[32];

	unsigned int unknown[5];

	struct rewards
	{
		unsigned int award_id;
		unsigned int award_amount;
		unsigned int points_required;
	} reward[20];
};

struct EXTRA_LOTTERY_ESSENCE {
	unsigned int id;
	namechar name[32];
	unsigned int file_matter;
	unsigned int file_icon;
	
	unsigned int unknown[14];

	struct awards
	{
		unsigned int award_id;
		unsigned int award_amount;
	} award[30];

	unsigned int price;
	unsigned int shop_price;
	unsigned int pile_num_max;
	unsigned int proc_type;
};

struct SKILL_SET_CONFIG
{
	unsigned int id;
	namechar name[32];

	unsigned int skill_id[100];
};

struct GEAR_SCRIPTURE_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct scripture
	{
		unsigned int scripture_id;
		struct requirements
		{
			unsigned int required_item_id;
			unsigned int required_item_amount;
		} requirement[3];

		unsigned int unknown[7];

	} scripture[50];
};

struct FIREWORK_ARRAY_ESSENCE {
	unsigned int id;
	namechar name[32];
	unsigned int file_matter;
	unsigned int file_icon;
	unsigned int rewards_amount;
	unsigned int file_gfx;
	unsigned int unknown[3];
	namechar text[100];

	unsigned int price;
	unsigned int shop_price;
	unsigned int pile_num_max;
	unsigned int proc_type;
};

struct RESET_ITEM_ESSENCE
{
	unsigned int id;
	namechar name[32];
	unsigned int file_matter;
	unsigned int file_icon;
	unsigned int unknown;
	unsigned int reset_type;
	unsigned int price;
	unsigned int shop_price;
	unsigned int pile_num_max;
	unsigned int proc_type;
};

struct MARRIAGE_REWARD_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct rewards
	{
		unsigned int unknown;
		unsigned int reward_id;
	} reward[8];
};

struct MYSTIC_ENHANCE_MATERIAL_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct refinery
	{
		unsigned int talisman_id;
		unsigned int talisman_amount;
	} refinery[20];
};

struct LUCK_VALUE_CONFIG
{
	unsigned int id;
	namechar name[32];

	unsigned int unknown_1;

	struct luck
	{
		unsigned int unknown;
		float unknown_float;
	} luck[10];

	unsigned int unknowns[7];
};

struct CLASS_ITEM_CONFIG
{
	unsigned int id;
	namechar name[32];
	unsigned int mold_id;

	struct refinery
	{
		unsigned int equipment_id;
		unsigned int refinery_min;
		unsigned int refinery_max;
	} refinery[10];
};

struct NEW_GEAR_IDENTIFY_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct gear_list
	{
		unsigned int spirit_stone_id;
		unsigned int equipment_id[30];
	} gear_list[5];
};

struct NEW_GEAR_LIST_CONFIG
{
	unsigned int id;
	namechar name[32];
	unsigned int equipment_id[150];
};

struct INSTANCE_INTERCHANGE_CONFIG
{
	unsigned int id;
	namechar name[32];
	unsigned int unknown_id[200];
};

struct SPIRIT_STONE_ESSENCE
{
	unsigned int id;
	namechar name[32];
	unsigned int file_matter;
	unsigned int file_icon;
	unsigned int unknown;
	namechar text[16];
	unsigned int addon_id;
	unsigned int price;
	unsigned int shop_price;
	unsigned int pile_num_max;
	unsigned int proc_type;
};

struct VALUE_COST_CONFIG
{
	unsigned int id;
	namechar name[32];

	unsigned int unknown[140];
};

// UNDER DEVELOPMENT
struct ANNIVERSARY_ITEM_ESSENCE
{
	unsigned int id;              // 0x000
	namechar name[32];            // 0x004, 64 bytes
	unsigned int file_matter;     // 0x044
	unsigned int file_icon;       // 0x048
	unsigned int mode;            // 0x04C

	struct award_with_text
	{
		float probability;
		unsigned int active_score;
		namechar text[100];       // 200 bytes
		unsigned int award_id;
		unsigned int award_amount;
		unsigned int award_bind;
		unsigned int award_expire;
	} award_text[3];              // 3 * 224 = 672 bytes

	struct award_no_text
	{
		float probability;
		unsigned int award_id;
		unsigned int award_amount;
		unsigned int award_bind;
		unsigned int award_expire;
	} award_4;                    // 20 bytes

	unsigned int price;
	unsigned int shop_price;
	unsigned int pile_num_max;
	unsigned int proc_type;
};

struct SPIRIT_EXRACTION_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct equip_config
	{
		unsigned int level;
		unsigned int amount;
		unsigned int unknown;
	} config[10];
};

struct NEW_DOTA_SKILL_CONFIG
{
	unsigned int id;
	namechar name[32];
	unsigned int type;

	struct skills
	{
		unsigned int skill_id;
	} skill[22];
};

struct ANOTHER_DOTA_SKILL_CONFIG
{
	unsigned int id;
	namechar name[32];
	unsigned int type;

	struct skills
	{
		unsigned int skill_id;
	} skill[184];
};

struct EXTRA_DOTA_SKILL_CONFIG
{
	unsigned int id;
	namechar name[32];
	unsigned int gender; // 0 - male, 1 - female
		
	struct skills
	{
		unsigned int skill_id;
	} skill[24];
};

struct WARDOBE_FASHION_CONFIG
{
	unsigned int id;
	namechar name[32];
	unsigned int unknown_1;
	unsigned int fashion_id[149];
	unsigned int unknown_2;
	unsigned int unknown_3;
};

struct NEW_DYE_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct config
	{
		namechar text[256];
		unsigned int colorant_id;
		unsigned int unknown_1;
		unsigned int unknown_2;
		unsigned int unknown_3;
	} config[14];
};

struct EMBROIDERY_CONFIG
{
	unsigned int id;
	namechar name[32];
	unsigned int unknown[3];
};

struct UNIDENTIFIED_TABLE1736
{
	unsigned int id;
	namechar name[32];
	unsigned char unknown[1400];
};

struct MERGED_STAR_SOUL_LIMIT_CONFIG
{
	unsigned int id;
	namechar name[32];
	unsigned int type;
	unsigned int min_level;
	unsigned int max_level;
	unsigned int unknown[36];
};

struct MERGED_STAR_SOUL_GROWTH_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	int				per_point_soul_req;

	float			props_list[36];

	struct level_up_info
	{
		int			points_req;
		float		success_prob;
		float		failed_return_ratio;
	} level_up_list[100];
};

struct MERGED_STAR_SOUL_SLOT_CONFIG
{
	unsigned int id;
	namechar name[32];
	unsigned int required_points[36];
};

struct RUNE2013_MATERIAL_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct consumeables
	{
		unsigned int item_id;
		unsigned int receive_soul_points;
		unsigned int unknown;
	} consumeable[100];
};

struct NEW_VIP_CONFIG
{
	unsigned int id;
	namechar name[32];

	unsigned int unknown[31];
};

struct RUNE2013_CONSUMPTION_CONFIG
{
	unsigned int id;
	namechar name[32];

	unsigned int unknown[101];
};

struct FORBID_DIVINE_SKILL_CONFIG
{
	unsigned int id;
	namechar name[32];

	unsigned int skill_id[216];
};

struct GIFT_LETTER_ESSENCE
{
	unsigned int id;
	namechar name[32];
	unsigned int pre_unknown[8];
	unsigned int file_matter;
	unsigned int file_icon;
	unsigned int unknown[3];
	UInt64 character_combo_id_1;
	UInt64 character_combo_id_2;
	unsigned int unknown_4;
	namechar title[100];
	unsigned int unknown_5;
	unsigned int unknown_6;
	namechar text[1000];

	struct awards
	{
		unsigned int unknown;
		unsigned int award_id;
		unsigned int bind;
		unsigned int award_amount;
	} award[10];

	unsigned int unknown_7;

	unsigned int price;
	unsigned int shop_price;
	unsigned int pile_num_max;
	unsigned int proc_type;
};

struct WARDOBE_WEAPON_CONFIG
{
	unsigned int id;
	namechar name[32];
	unsigned int order;
	unsigned int file_surfaces;
	unsigned int equipment_id;
	unsigned int unknown_1;
	unsigned int unknown_2;
	namechar text[250];
	unsigned int unknown_3;
	unsigned int unknown_4;
	unsigned int unknown_5;
	unsigned int unknown_6;
};

struct WARDOBE_FOOTPRINT_CONFIG
{
	unsigned int id;
	namechar name[32];
	unsigned int order;
	unsigned int file_surfaces;
	unsigned int footprint_id;
	unsigned int unknown_1;
	unsigned int unknown_2;
	namechar text[250];
	unsigned int unknown_3;
	unsigned int unknown_4;
	unsigned int unknown_5;
	unsigned int unknown_6;
};

// Not yet implemented in this expansion.
struct WARDOBE_PET_CONFIG
{
	unsigned int id;
	namechar name[32];
	unsigned int order;
	unsigned int file_surfaces;
	unsigned int petconfig_id;
	unsigned int unknown_1;
	unsigned int unknown_2;
	namechar text[250];
	unsigned int unknown_3;
	unsigned int unknown_4;
	unsigned int unknown_5;
	unsigned int unknown_6;
};

struct FOOTPRINT_ESSENCE
{
	unsigned int id;
	namechar name[32];
	unsigned int file_matter;
	unsigned int file_icon;
	unsigned int file_gfx[3];
	unsigned int unknown_1;
	unsigned int unknown_2;
	unsigned int price;
	unsigned int shop_price;
	unsigned int unknown_3;
	unsigned int pile_num_max;
	unsigned int proc_type;
};

struct FORTUNE_GIFT_PACK_ESSENCE
{
	unsigned int id;
	namechar name[32];
	namechar ui_name[16];
	unsigned int file_matter;
	unsigned int file_icon;
	unsigned int timeframe_start;
	unsigned int timeframe_end;
	unsigned int timeleft_days;
	unsigned int unknown_2;

	struct awards
	{
		unsigned int require_ascension_count;
		unsigned int require_level_min;
		unsigned int unknown[6];
		
		unsigned int award_id;
		unsigned int award_count;
		unsigned int award_expire;
		unsigned int award_bind;

		unsigned int gift_award_id;
		unsigned int gift_award_amount;
		unsigned int gift_award_expire;
		unsigned int gift_award_bind;
		namechar description[100];

	} awards[20];
	unsigned int unknown[5];
	unsigned int price;
	unsigned int shop_price;
	unsigned int pile_num_max;
	unsigned int proc_type;
};

struct CANTEEN_EVENT_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct awards_list
	{
		struct awards {
			unsigned int award_id;
			unsigned int award_amount;
			unsigned int award_expire;
		} award[3];

		unsigned int unknown[4];
	} award_list[100];
};

struct RETURN_AWARD_CONFIG
{
	unsigned int id;
	namechar name[32];
	unsigned int unknown[250];
};

struct MOBILE_PHONE_INFO_CONFIG
{
	unsigned int id;
	namechar name[32];
	namechar text[1000];

	struct awards
	{
		unsigned int award_id;
		unsigned int award_amount;
		unsigned int award_bind;
		unsigned int award_expire;
	} award[4];
};

struct BEGINNER_ONLINE_REWARD_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct online_rewards
	{
		unsigned int gift_bad_id;
		unsigned int wait_time_seconds;
	} online_reward[10];
};

struct MONTHLY_RETURN_AWARD_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct award_lists
	{
		unsigned int item_order;
		unsigned int list_index;
		unsigned int award_id;
	} award_list[15];
};

struct INSTANT_TIER_CONFIG
{
	unsigned int id;
	namechar name[32];

	struct configs
	{
		unsigned int occupation_id;
		unsigned int required_level;
	} config[4];

	unsigned int unknown[13];
};

struct UNIDENTIFIED_TABLE1792A
{
	unsigned int id;
	namechar name[32];
	unsigned int unknown[115];
};

struct UNIDENTIFIED_TABLE1792B
{
	unsigned int id;
	namechar name[32];
	unsigned int unknown[40563];
};

struct PLOT_REWARD_CONFIG
{
	unsigned int id;
	namechar name[32];
	unsigned int unknown[1230];
};

//=============================================================================
// ID space and data type enumerations (must come after all struct definitions)
//=============================================================================

enum ID_SPACE
{
	ID_SPACE_ESSENCE	= 0,
	ID_SPACE_ADDON		= 1,
	ID_SPACE_TALK		= 2,
	ID_SPACE_RECIPE		= 3,
	ID_SPACE_CONFIG		= 4,
};

//数据类型标志，以下枚举项目顺序不能更改
enum DATA_TYPE
{
	DT_INVALID = 0,
	DT_EQUIPMENT_ADDON,
	DT_EQUIPMENT_MAJOR_TYPE,
	DT_EQUIPMENT_SUB_TYPE,
	DT_EQUIPMENT_ESSENCE,
	DT_REFINE_TICKET_ESSENCE,
	DT_MEDICINE_MAJOR_TYPE,
	DT_MEDICINE_SUB_TYPE,
	DT_MEDICINE_ESSENCE,
	DT_MATERIAL_MAJOR_TYPE,
	DT_MATERIAL_SUB_TYPE,
	DT_MATERIAL_ESSENCE,
	DT_SKILLTOME_SUB_TYPE,
	DT_SKILLTOME_ESSENCE,
	DT_TRANSMITROLL_ESSENCE,
	DT_LUCKYROLL_ESSENCE,
	DT_TOWNSCROLL_ESSENCE,
	DT_REVIVESCROLL_ESSENCE,
	DT_TASKMATTER_ESSENCE,
	DT_DROPTABLE_TYPE,
	DT_DROPTABLE_ESSENCE,
	DT_MONSTER_TYPE,
	DT_MONSTER_ESSENCE,
	DT_NPC_TALK_SERVICE,
	DT_NPC_SELL_SERVICE,
	DT_NPC_BUY_SERVICE,
	DT_NPC_TASK_IN_SERVICE,
	DT_NPC_TASK_OUT_SERVICE,
	DT_NPC_TASK_MATTER_SERVICE,
	DT_NPC_HEAL_SERVICE,
	DT_NPC_TRANSMIT_SERVICE,
	DT_NPC_PROXY_SERVICE,
	DT_NPC_STORAGE_SERVICE,
	DT_NPC_TYPE,
	DT_NPC_ESSENCE,
	DT_TALK_PROC,
	DT_RECIPE_MAJOR_TYPE,
	DT_RECIPE_SUB_TYPE,
	DT_RECIPE_ESSENCE,
	DT_ENEMY_FACTION_CONFIG,
	DT_CHARACTER_CLASS_CONFIG,
	DT_PARAM_ADJUST_CONFIG,
	DT_PIE_LOVE_CONFIG,
	DT_TASKDICE_ESSENCE,
	DT_TASKNORMALMATTER_ESSENCE,
	DT_PLAYER_LEVELEXP_CONFIG,
	DT_MINE_TYPE,
	DT_MINE_ESSENCE,
	DT_GM_GENERATOR_TYPE,
	DT_GM_GENERATOR_ESSENCE,
	DT_FIREWORKS_ESSENCE,
	DT_NPC_WAR_TOWERBUILD_SERVICE,
	DT_PLAYER_SECONDLEVEL_CONFIG,
	DT_NPC_RESETPROP_SERVICE,
	DT_ESTONE_ESSENCE,
	DT_PSTONE_ESSENCE,
	DT_SSTONE_ESSENCE,
	DT_RECIPEROLL_MAJOR_TYPE,
	DT_RECIPEROLL_SUB_TYPE,
	DT_RECIPEROLL_ESSENCE,
	DT_SUITE_ESSENCE,
	DT_DOUBLE_EXP_ESSENCE,
	DT_DESTROYING_ESSENCE,
	DT_NPC_EQUIPBIND_SERVICE,
	DT_NPC_EQUIPDESTROY_SERVICE,
	DT_NPC_EQUIPUNDESTROY_SERVICE,
	DT_SKILLMATTER_ESSENCE,
	DT_VEHICLE_ESSENCE,
	DT_COUPLE_JUMPTO_ESSENCE,
	DT_LOTTERY_ESSENCE,
	DT_CAMRECORDER_ESSENCE,
	DT_TITLE_PROP_CONFIG,
	DT_SPECIAL_ID_CONFIG,
	DT_TEXT_FIREWORKS_ESSENCE,
	DT_TALISMAN_MAINPART_ESSENCE,
	DT_TALISMAN_EXPFOOD_ESSENCE,
	DT_TALISMAN_MERGEKATALYST_ESSENCE,
	DT_TALISMAN_ENERGYFOOD_ESSENCE,
	DT_SPEAKER_ESSENCE,
	DT_PLAYER_TALENT_CONFIG,
	DT_POTENTIAL_TOME_ESSENCE,
	DT_WAR_ROLE_CONFIG,
	DT_NPC_WAR_BUY_ARCHER_SERVICE,
	DT_SIEGE_ARTILLERY_SCROLL_ESSENCE,
	DT_PET_BEDGE_ESSENCE,
	DT_PET_FOOD_ESSENCE,
	DT_PET_SKILL_ESSENCE,
	DT_PET_ARMOR_ESSENCE,
	DT_PET_AUTO_FOOD_ESSENCE,
	DT_PET_REFINE_ESSENCE,
	DT_PET_ASSIST_REFINE_ESSENCE,
	DT_RENASCENCE_SKILL_CONFIG,
	DT_RENASCENCE_PROP_CONFIG,
	DT_AIRCRAFT_ESSENCE,
	DT_FLY_ENERGYFOOD_ESSENCE,
	DT_ITEM_TRADE_CONFIG,
	DT_BOOK_ESSENCE,
	DT_PLAYER_SKILL_POINT_CONFIG,
	DT_OFFLINE_TRUSTEE_ESSENCE,
	DT_EQUIP_SOUL_ESSENCE,
	DT_EQUIP_SOUL_MELD_SERVICE,
	DT_SPECIAL_NAME_ITEM_ESSENCE,
	DT_RECYCLE_ITEM_CONFIG,
	DT_SCORE_TO_RANK_CONFIG,
	DT_BATTLE_DROP_CONFIG,
	DT_BATTLE_DEPRIVE_CONFIG,
	DT_BATTLE_SCORE_CONFIG,
	DT_GIFT_BAG_ESSENCE,
	DT_VIP_CARD_ESSENCE,
	DT_INSTANCING_BATTLE_CONFIG,
	DT_CHANGE_SHAPE_CARD_ESSENCE,
	DT_CHANGE_SHAPE_STONE_ESSENCE,
	DT_CHANGE_SHAPE_PROP_CONFIG,
	DT_ORIGINAL_SHAPE_CONFIG,
	DT_LIFE_SKILL_CONFIG,
	DT_ARENA_DROPTABLE_CONFIG,
	DT_MERCENARY_CREDENTIAL_ESSENCE,
	DT_TELEPORTATION_ESSENCE,
	DT_TELEPORTATION_STONE_ESSENCE,
	DT_COMBINE_SKILL_EDIT_CONFIG,
	DT_UPGRADE_EQUIP_CONFIG,
	DT_UPGRADE_EQUIP_CONFIG_1,
	DT_UPGRADE_EQUIP_STONE_ESSENCE,
	DT_NPC_CONSIGN_SERVICE,
	DT_DEITY_CHARACTER_CONFIG,
	DT_LOTTERY2_ESSENCE,
	DT_GEM_CONFIG,
	DT_GEM_ESSENCE,
	DT_GEM_SEAL_ESSENCE,
	DT_GEM_DUST_ESSENCE,
	DT_GEM_EXTRACT_CONFIG,
	DT_GENERAL_ARTICLE_ESSENCE,
	DT_LOTTERY3_ESSENCE,		//Added 2011-07-01.
	DT_TRANSCRIPTION_CONFIG,	//Added 2011-07-13.
	DT_ONLINE_GIFT_BAG_CONFIG,	//Added 2011-12-06.
	DT_SMALL_ONLINE_GIFT_BAG_ESSENCE, //Added 2011-12-06.
	DT_SCROLL_REGION_CONFIG,	//Added 2012-03-20.
	DT_SCROLL_UNLOCK_ESSENCE,	//Added 2012-03-20.
	DT_SCROLL_DIG_COUNT_INC_ESSENCE,	//Added 2012-03-22.
	DT_TOWER_TRANSCRIPTION_CONFIG,		//Added 2012-03-31.
	DT_TOWER_TRANSCRIPTION_PROPERTY_CONFIG,	//Added 2012-03-31.
	DT_RUNE_DATA_CONFIG,
	DT_RUNE_COMB_PROPERTY,
	DT_RUNE_EQUIP_ESSENCE,
	DT_LITTLE_PET_UPGRADE_CONFIG,		//Added 2012-04-19.
	DT_RUNE_SLOT_ESSENCE,
	DT_DROP_INTERNAL_CONFIG,
	DT_PK2012_GUESS_CONFIG,
	DT_COLLISION_RAID_TRANSFIGURE_CONFIG,//Added 2012-08-02.
	DT_BOOTH_FIGURE_ITEM_ESSENCE,
	DT_FLAG_BUFF_ITEM_ESSENCE,
	DT_NPC_REPUTATION_SHOP_SERVICE,
	DT_NPC_UI_TRANSFER_SERVICE,
	DT_UI_TRANSFER_CONFIG,
	DT_XINGZUO_ITEM_ESSENCE,
	DT_XINGZUO_ENERGY_ITEM_ESSENCE,
	DT_COLLISION_RAID_AWARD_CONFIG,
	DT_CASH_MEDIC_MERGE_ITEM_ESSENCE,
	DT_BUFF_AREA_CONFIG,
	DT_LIVENESS_CONFIG,
	DT_CHALLENGE_2012_CONFIG,
	DT_SALE_PROMOTION_ITEM_ESSENCE,
	DT_GIFT_BAG_LOTTERY_DELIVER_ESSENCE,
	DT_LOTTERY_TANGYUAN_ITEM_ESSENCE,
	DT_TASK_SPECIAL_AWARD_CONFIG,
	DT_GIFT_PACK_ITEM_ESSENCE,
	DT_PROP_ADD_MATERIAL_ITEM_ESSENCE,
	DT_PROP_ADD_ITEM_ESSENCE,
	DT_PROP_ADD_CONFIG,
	DT_KING_WAR_CONFIG,
	DT_JINFASHEN_TO_MONEY_CONFIG,
	DT_BATTLE_201304_CONFIG,
	DT_RUNE2013_ITEM_ESSENCE,
	DT_RUNE2013_CONFIG,
	DT_BASHE_AWARD_CONFIG,
	DT_ARENA_SEASON_TIME_CONFIG,
	DT_PERSONAL_LEVEL_AWARD_CONFIG,
	DT_TEAM_SEASON_AWARD_CONFIG,
	DT_WEEK_CURRNCY_AWARD_CONFIG,
	DT_COLORANT_ITEM_ESSENCE,
	DT_COLORANT2_ITEM_ESSENCE,
	DT_INTERACTION_OBJECT_ESSENCE,
	DT_INTERACTION_OBJECT_HOOK_CONFIG,
	DT_COLORANT_CONFIG,
	DT_FACTION_TRANSFER_CONFIG,
	DT_BUILDING_REGION_CONFIG,
	DT_BUILDING_ESSENCE,
	DT_REGION_BLOCK_ESSENCE,
	DT_PHASE_CONFIG,
	DT_CROSS6V6_ITEM_EXCHANGE_CONFIG,
	DT_TRANSCRIPT_STRATEGY_CONFIG,
	DT_FACTION_SHOP_CONFIG,
	DT_FACTION_AUCTION_CONFIG,
	DT_FACTION_MONEY_SHOP_CONFIG,
	DT_COMPOUND_MINE_ESSENCE,
	DT_COLORANT_DEFAULT_CONFIG,
	// v158 new data tables (loaded after talk_proc)
	DT_SIGN_IN_CONFIG,
	DT_BABY_ESSENCE,
	DT_CLASS_BABY_RELATION_CONFIG,
	DT_BABY_CONFIG,
	DT_BABY_PROPERTY_CONFIG,
	DT_BABY_UPGRADE_CONFIG,
	DT_BABY_FASHION_ESSENCE,
	DT_BABY_TOY_ESSENCE,
	DT_BABY_BOOK_ESSENCE,
	DT_MATRIX_CARD_ESSENCE,
	DT_MATRIX_EQUIP_ESSENCE,
	DT_MATRIX_EXP_CONFIG,
	DT_MATRIX_COLLECT_CONFIG,
	DT_MATRIX_CARD_UPGREAD_CONFIG,
	DT_MATRIX_COMBINE_CONFIG,
	DT_BABY_FOOD_ESSENCE,
	DT_BABY_TITLE_ESSENCE,
	DT_BABY_TITLE_LIST,
	DT_VEHICLE_ENHANCE_ESSENCE,
	DT_TALENT_SCROLL_ESSENCE,
	DT_TALENT_SCROLL_CONFIG,
	DT_TALENT_SCROLL_SKILL_CONFIG,
	DT_SKILL_REPLACE_CONFIG,
	DT_HILL_TRANSCRIPTION_CONFIG,
	DT_CONSUME_AWARD_CONFIG,
	DT_MULTI_TOWER_PROPERTY_CONFIG,
	DT_RAID18V18_AWARD_CONFIG,
	DT_EGGS_GIFT_BAG_ESSENCE,
	DT_ITEM_TRADE2_CONFIG,
	DT_GUESS_ESSENCE,
	DT_ELITE_GAME_RESULT_CONFIG,
	DT_ELITE_GAME_AWARD_CONFIG,
	DT_TITLE_ITEM_ESSENCE,
	DT_SOUL_ENHANCE_CONFIG,
	DT_SOUL_EXCHANGE_CONFIG,
	DT_SOUL_DROP_CONFIG,
	DT_DAILY_SIGN_CONFIG,
	DT_RED_PACKETS_ESSENCE,
	DT_FORBID_COMMON_SKILL_CONFIG,
	DT_FORBID_GODEVIL_SKILL_CONFIG,
	DT_FORCE_CD_SKILL_CONFIG,
	DT_TALENT_SCROLL_REFINE_ESSENCE,
	DT_FACTION_BASE_ACTIVITY_CONFIG,
	DT_AMULET_ESSENCE,
	DT_AMULET_CONFIG,
	DT_AMULET_STONE_ESSENCE,
	DT_SPECIAL_VIP_CONFIG,
	DT_CHARGE_AWARD_CONFIG,
	DT_NEW_TOWNSCROLL_ESSENCE,
	DT_EQUIPMENT_RANDOMEXT_CONFIG,
	DT_INDISCRIMINATION_BATTLE_CONFIG,
	DT_GROUPRAID_EVALAWARD_CONFIG,
	DT_EQUIPMENT_IDENTIFY_CONFIG,
	DT_HEADFRAME_ESSENCE,
	DT_CONVERT_EQUIP_CONFIG,
	DT_AUSPICIOUS_HALL_CONFIG,
	DT_DIVINE_ACCESSORY_IMBUE_CONFIG,
	DT_LEVEL_UP_REWARD_CONFIG,
	DT_NEW_BOX_ESSENCE,
	DT_NEW_FASHION_PACK_ESSENCE,
	DT_NEW_CRAFTING_CONFIG,
	DT_FAIRYHOUSE_GENERAL_CONFIG,
	DT_FAIRYHOUSE_PROBABILITY_CONFIG,
	DT_FAIRYHOUSE_BABY_TRAIT_CONFIG,
	DT_FAIRYHOUSE_PET_TRAIT_CONFIG,
	DT_FAIRYHOUSE_CHALLENGE_CONFIG,
	DT_FAIRYHOUSE_BABY_GEAR_ESSENCE,
	DT_FAIRYHOUSE_PET_GEAR_ESSENCE,
	DT_FAIRYHOUSE_TRAIT_LEARN_ESSENCE,
	DT_PREPAID_BOOK_ESSENCE,
	DT_TOKEN_BAG_CONFIG,
	DT_TIMELIMIT_BEAD_ESSENCE,
	DT_EXTRA_TIMELIMIT_BEAD_ESSENCE,
	DT_CONSTELLATIONEXP_CONFIG,
	DT_CONSTELLATION_SKILL_CONFIG,
	DT_CONSTELLATION_CONFIG,
	DT_CONSTELLATION_STARS_ESSENCE,
	DT_CONSTELLATION_SKILL_RELATION_CONFIG,
	DT_TALENT_SCROLL_TRIGGER_CONFIG,
	DT_EVENT_REWARD_SET_CONFIG,
	DT_EXTRA_LOTTERY_ESSENCE,
	DT_SKILL_SET_CONFIG,
	DT_GEAR_SCRIPTURE_CONFIG,
	DT_FIREWORK_ARRAY_ESSENCE,
	DT_RESET_ITEM_ESSENCE,
	DT_MARRIAGE_REWARD_CONFIG,
	DT_MYSTIC_ENHANCE_MATERIAL_CONFIG,
	DT_LUCK_VALUE_CONFIG,
	DT_CLASS_ITEM_CONFIG,
	DT_NEW_GEAR_IDENTIFY_CONFIG,
	DT_NEW_GEAR_LIST_CONFIG,
	DT_INSTANCE_INTERCHANGE_CONFIG,
	DT_SPIRIT_STONE_ESSENCE,
	DT_VALUE_COST_CONFIG,
	DT_ANNIVERSARY_ITEM_ESSENCE,
	DT_SPIRIT_EXRACTION_CONFIG,
	DT_NEW_DOTA_SKILL_CONFIG,
	DT_ANOTHER_DOTA_SKILL_CONFIG,
	DT_EXTRA_DOTA_SKILL_CONFIG,
	DT_WARDOBE_FASHION_CONFIG,
	DT_NEW_DYE_CONFIG,
	DT_EMBROIDERY_CONFIG,
	DT_UNIDENTIFIED_TABLE1736,
	DT_MERGED_STAR_SOUL_LIMIT_CONFIG,
	DT_MERGED_STAR_SOUL_GROWTH_CONFIG,
	DT_MERGED_STAR_SOUL_SLOT_CONFIG,
	DT_RUNE2013_MATERIAL_CONFIG,
	DT_NEW_VIP_CONFIG,
	DT_RUNE2013_CONSUMPTION_CONFIG,
	DT_FORBID_DIVINE_SKILL_CONFIG,
	DT_GIFT_LETTER_ESSENCE,
	DT_WARDOBE_WEAPON_CONFIG,
	DT_WARDOBE_FOOTPRINT_CONFIG,
	DT_WARDOBE_PET_CONFIG,
	DT_FOOTPRINT_ESSENCE,
	DT_FORTUNE_GIFT_PACK_ESSENCE,
	DT_CANTEEN_EVENT_CONFIG,
	DT_RETURN_AWARD_CONFIG,
	DT_MOBILE_PHONE_INFO_CONFIG,
	DT_BEGINNER_ONLINE_REWARD_CONFIG,
	DT_MONTHLY_RETURN_AWARD_CONFIG,
	DT_INSTANT_TIER_CONFIG,
	DT_UNIDENTIFIED_TABLE1792A,
	DT_UNIDENTIFIED_TABLE1792B,
	DT_PLOT_REWARD_CONFIG,
	DT_MAX,              //数据类型标志，以下枚举项目顺序不能更改
};

#pragma pack(pop, EXP_TYPES_INC)
#endif//_EXP_TYPES_H_
