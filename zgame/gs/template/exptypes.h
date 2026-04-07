#ifndef _EXP_TYPES_H_
#define _EXP_TYPES_H_

#include <stdio.h>
#include <stdlib.h>

#ifdef _WINDOWS
#include <memory.h>
#endif

#define ELEMENTDATA_VERSION			0x1000009e

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

// װ����������
struct EQUIPMENT_ESSENCE
{
	// ������Ϣ
	unsigned int	id;							// װ��(����)ID
	unsigned int	id_major_type;				// װ�������ID
	unsigned int	id_sub_type;				// װ��С���ID
	namechar		name[32];					// װ������, ���15������

	unsigned int	equip_type;					// װ����𣬿��ܵ��� 0-������1-���ߣ�2-��Ʒ��3-ʱװ��4-���5-��������
	unsigned int	equip_mask;					// �߼��Ͽ���װ���Ĳ�λ���ɲ�ͬ��λMask��ɣ�ʹ�õĲ�λ����Ϊ��
												// EQUIP_MASK_WEAPON         = 0x0001,
												// EQUIP_MASK_HEAD           = 0x0002,
												// EQUIP_MASK_BODY           = 0x0004,
												// EQUIP_MASK_FOOT           = 0x0008,
												// EQUIP_MASK_NECK           = 0x0010,
												// EQUIP_MASK_FINGER1        = 0x0020,
												// EQUIP_MASK_FINGER2        = 0x0040,
												// EQUIP_MASK_FASHION_EYE    = 0x0080,
												// EQUIP_MASK_FASHION_NOSE   = 0x0100,
												// EQUIP_MASK_FASHION_LIP    = 0x0200,
												// EQUIP_MASK_FASHION_HEAD   = 0x0400,
												// EQUIP_MASK_FASHION_BODY   = 0x0800,
												// EQUIP_MASK_FASHION_LEG    = 0x1000,
												// EQUIP_MASK_FASHION_FOOT   = 0x2000,
												// EQUIP_MASK_FASHION_BACK   = 0x4000,
												// EQUIP_MASK_WING			 = 0x8000,
												// EQUIP_MASK_MAIN_TALISMAN	 = 0x10000,
												// EQUIP_MASK_SUB_TALISMAN1	 = 0x20000,
												// EQUIP_MASK_SUB_TALISMAN2	 = 0x40000,
												
	
	int				file_model_male;			// ��ģ��·��
	int				file_model_female;			// Ůģ��·��
	
	int				file_model_male_armor_xuanyuan;	//��ԯ��ñ��ģ��·������ϵ�� pathid ռλ�ã�Added 2012-07-24.
	int				file_model_female_armor_xuanyuan;//��ԯ Ůñ��ģ��·������ϵ��pathid ռλ�ã�Added 2012-07-24.
	int				file_model_big_male;		// ��������ģ��·��
	int				file_model_big_female;		// ������Ůģ��·��
	int				file_model_small_male;		// С������ģ��·��
	int				file_model_small_female;	// С����Ůģ��·��
	int				file_model_tiny_male;		// ΢������ģ��·��
	int				file_model_tiny_female;		// ΢����Ůģ��·��
	int				file_model_tetrapod_male;	// ����������ģ��·��
	int				file_model_big_male_left;	// �����������ֱ�ģ��·�� ��Ҫ����̫�˫�ֲ�ͬ����Added 2011-08-09.
	int				file_model_big_female_left;	// ������Ů���ֱ�ģ��·�� ��Ҫ����̫�˫�ֲ�ͬ����Added 2011-08-09.
	int				file_model_back_male;		// ������ģ��·���� ��Ҫ����̫�/���㱳��ģ��Ч��Added 2011-08-09.
	int				file_model_back_female;		// ����Ůģ��·���� ��Ҫ����̫�/���㱳��ģ��Ч��Added 2011-08-09.
	int				file_model_female_taihao;	// ̫�Ůʱװ·���� ר������̫�Ůʱװ��ģ��·�� Added 2011-08-09.

	// Added version 158
	int				file_model_back_female_fash;		// Back_Female fashion model path
	int				file_model_female_taihao_fash;		// Forta_Female_Fash model path
	int				file_model_big_male_sword;			// Hydran_Male sword weapon model
	int				file_model_big_female_sword;		// Hydran_Female sword weapon model
	int				file_model_big_male_scythe;			// Hydran_Male scythe weapon model
	int				file_model_big_female_scythe;		// Hydran_Female scythe weapon model
	int				file_model_big_male_bow;			// Hydran_Male bow weapon model
	int				file_model_big_female_bow;			// Hydran_Female bow weapon model
	int				file_model_reserved1;				// reserved
	int				file_model_reserved2;				// reserved
	int				file_model_reserved3;				// reserved
	int				file_model_reserved4;				// reserved
	int				file_model_reserved5;				// reserved
	int				file_model_reserved6;				// reserved

	int				file_matter;				// ���ڵ��ϵ�ģ��·��
	int				file_icon;					// ͼ��·��



	unsigned int	equip_location;				// ģ�͸��ǲ�λ�����ܵ��У�0-����ʾ��1-ͷ����2-���£�3-���£�4-���´����£�
												// 5-Ь�ӣ�6-�۾���7-���ӣ�8-���ӣ�9-���Σ�10-���
	unsigned int	action_type;				// �����Ķ����������ͣ����ܵ���: 0-����, 1-�̱�, 2-�̷�, 3-ذ��, 4-����, 5-����

	namechar		show_level[16];				// װ��Ʒ������
	int				level;						// װ���ȼ�

	UInt64			character_combo_id;			// ְҵ����, �ӵ�λ����λ�ĺ���Ϊ��0-����, 1-��ʿ, 2-��ʿ, 3-սʿ, 4-��ʿ, 5-��ʦ, 6-��ʦ, 7-����, 8-����ʿ, 9-�̿�, 10-��ʥ, 11-����, 12-��˾, 13-�ط�ʦ, 14-�ٻ�ʹ
	UInt64			character_combo_id2;		// ְҵ���ƣ���չְҵID�������ӵ�ְҵ��Added 2011-07-14
	UInt64			char_lev_1;
	UInt64			char_lev_1_2;				// תְҵʱ��ְҵ���ƣ���չְҵID�����ְҵ���ƣ�Added 2011-07-15. 
	UInt64			char_lev_2;
	UInt64			char_lev_2_2;				// תְҵʱ��ְҵ���ƣ���չְҵID�����ְҵ���ƣ�Added 2011-07-15. 
	UInt64			char_lev_3;
	UInt64			char_lev_3_2;				// תְҵʱ��ְҵ���ƣ���չְҵID�����ְҵ���ƣ�Added 2011-07-15.
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
	unsigned int	nation_position_mask;		// ְλҪ��
												// 0x0001 ����
												// 0x0002 ����
												// 0x0004 ����
												// 0x0008 ��
												// 0x0010 ����
												// 0x0020 ����
	int				title_require;				// �ƺ�

	struct
	{
		int index;
		int num;
	} require_repu[2];

	
	/*
	0   Ĭ��ֵ����ʾû���������
	10	�������
	20	˫ȫ����
	40	��ɽ��ʿ
	70	�ĺ�����
	100	�������
	130	���Ϸ���
	160	��������
	190	�˻�����
	230	����Ӣ��
	270	ʮ������
	310	��ʤ�罫
	350	ǧ������
	390	�����˻�
	760	��������
	*/
		
	int				require_territory;			// �������
	// ��������
	int				hp;							// ����
	int				mp;							// ħ��
	int				dp;							// ����
	int				deity_power;				// ��ʥ��
	int				min_dmg;					// ��С������
	int				max_dmg;					// ��󹥻���
	int				defence;					// ������
	int				attack;						// ����
	int				armor;						// ����
	
	float			attack_range;				// ��������

	// ��Ǯ
	int				price;						// �����
	int				shop_price;					// ������

	int				id_addon1;					// ��1�ָ������Ե�����ID
	int				id_addon2;					// ��2�ָ������Ե�����ID
	int				id_addon3;					// ��3�ָ������Ե�����ID
	int				id_addon4;					// ��4�ָ������Ե�����ID
	int				id_addon5;					// ��5�ָ������Ե�����ID

	int				fee_estone;					// ����ǿ��������
	int				fee_install_pstone;			// ����׷��������
	int				fee_uninstall_pstone;		// ���β�ж׷��ʯ������
	int				fee_install_sstone;			// ���Ӽ���������
	int				fee_uninstall_sstone;		// ���β�ж����ʯ������

	unsigned int	id_estone;					// ���õ�ǿ��ʯ
	int				ehanced_value[20];			// 1~20��ǿ�������ֵ

	struct 
	{
		int			level;
		
		struct
		{
			int		id;
			float	probability;
		} prop[5];

	} hidden_prop[2];

	int			blood_sacrifice;			// �Ƿ�����Ѫ�� 0��ʾ������
	int			can_punching;				// �Ƿ�������� 0��ʾ������
	int			color;						// �Ƿ�����Ⱦɫ 0��ʾ������

	// Added version 158
	int			fixed_color;				// Dyed_Fixed_Color
	int			dyed;						// Dyed

	int				soul_level;					// ���Եȼ� 
	//  ������������
	struct  
	{
		int level;		// �����ȼ�
		int addon;		// ��������
	}hidden_addon[3];

	int				gem_slot_identify;		//����Ʒ�Ƿ��ܽ��б�ʯ��ۼ����� 0��ʾ���������༭����Ե�ʶ�ʹ��int

	// �ѵ���Ϣ
	int				pile_num_max;				// �ѵ�����

	// ������ʽ
	unsigned int	proc_type;					// �����¼��ַ�ʽ�����: ����ʱ�Ƿ���䣬�Ƿ�������ڵ��ϣ��Ƿ��������NPC�����������Ʒ���Ƿ������Ҽ佻�ף��Ƿ����������Ʒ���Ƿ�װ����󶨣�������ѡ�У�����GUID
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
	unsigned int	id;							// ����(����)ID
	unsigned int	id_type;					// ���ID
	namechar		name[32];					// ����, ���15������

	namechar		prop[16];					// ��������, ���16������
	namechar		desc[16];					// ��������, ���16������
	namechar		monster_desc[16];			// ������������ʾ�ڹ�������֮�£����16������

	unsigned int	faction;					// �Լ�������Ӫ��ϴ���
	unsigned int	enemy_faction;				// �ж���Ӫ��ϴ���
	unsigned int	monster_faction;			// �������ϸ��ϵ����

	int				file_model;					// ģ��·����
	int				head_icon;					// ͷ��
	int				big_icon;					// ����ͼ������ͼ�꣬������ſ��������Added 2012-04-23.
	
	float			size;						// ��С

	unsigned int	name_color;					// ������ɫ
	
	unsigned int	id_strategy;				// ���﹥������ID
												// 0 ��ʾ�����ⲫ�����
												// 1 ��ʾ�������������
												// 2 ��ʾħ�������
												// 3 ��ʾ�ⲫ��Զ�������
												// 4 ������
												// 5 ����������
												// 6 ��׮��
												// 7 ����ħ����

	unsigned int	select_strategy;			// ���ﱻѡ�в���
												// 0 ��������ѡ��
												// Tab����ѡ��
												// �κη�ʽ������ѡ��
	unsigned int    air_prop;					// �Ƿ��ǿ��й� 0���� 1����
	unsigned int	show_damage;				// �ͻ����Ƿ���ʾ�˺� 0���� 1: ��
	unsigned int	resist_valid;				// Resist_Valid

	int				level;						// ����ȼ�
	int				show_level;					// ��ʾ�ȼ�
	unsigned int	is_boss;					// �Ƿ�BOSS
	unsigned int	killed_exp;					// ����ɱ���Ƿ���ʧ����
	unsigned int	killed_drop;				// ����ɱ���Ƿ������Ʒ
	int				is_drop_adjustby_skill;		// �����Ƿ��ܼ���Ӱ�� Ĭ��ֵΪ0����ʾ���䲻�ܼ��ܵ�Ӱ�죻1����ʾ������ܵ�����Ӱ��
	
	unsigned int	immune_type;				// �������ͣ�ΪMask��Ϸ�ʽ����λ�ĺ���ӵ͵�������Ϊ��
												// ���߿־塢������ä�����߻��ˡ����߱����mp�����߼��ܷ����������߼��̶�ֵ����
												// ����ѣ�Ρ��������������߻��������߳�Ĭ������˯�ߡ����߲��ơ����߽�ʳ�����߿���
												// ���߳����˺������߳���������Ԫ���ɢ��������ħ�����߱������������ա�����Ѫ��
												// ���߽��ͷ���%

	float			sight_range;				// ��Ұ
	float			attack_range;				// ��������
	
	unsigned int	aggressive_mode;			// ������ѡ��0 - ������1 - ����
	unsigned int	monster_faction_ask_help;	// ����Щ��Ӫ�������ӹ�����ϸ��ϵ��ѡ��
	unsigned int	monster_faction_can_help;	// ������Щ��Ӫ�������ӹ�����ϸ��ϵ��ѡ��

	float			aggro_range;				// ��޾���
	float			aggro_time;					// ���ʱ��
	float			dead_aggro_time;			// ����������ʱ��

	unsigned int	patroll_mode;				// Ѳ�߷�ʽ�����ܵ��У�����߶���...
	unsigned int	stand_mode;					// վ����ʽ�����ܵ��У�����վ����˫��վ�������շ��е�

	float			walk_speed;					// �����ƶ��ٶ�(M/S)
	float			run_speed;					// �����ƶ��ٶ�(M/S)
	
	unsigned int	common_strategy;			// �������

	unsigned int	after_death;				// ������������-0���Ա�-0x1������-0x2
	unsigned int	adjust_exp;					// �����Ƿ��ܳͷ�ϵ��Ӱ��, Ĭ��Ϊ0������Ӱ�죬1Ϊ��Ӱ��
	unsigned int	can_catch;					// �Ƿ���Բ�׽��Ĭ��0��ʾ���ɲ�׽��1-8��ʾ��׽���Ѷ�:
												// �ӷǳ����ס����ס��Ƚ����ס��еȡ��ѡ��Ƚ��ѡ��ǳ��ѡ����ϼ���
	unsigned int	player_cannot_attack;		// ��ҹ����Ƿ���Ч��:��Ч-0�� ��Ч-1								
	int				exp;						// ��ʼ����
	int 			money_average;				// ��ʼǮ����׼ֵ
	int				money_var;					// ��ʼǮ������ֵ
	int				hp;							// ��ʼ������
	int				hp_disp_lv;					// Ѫ������
	int				mp;							// ��ʼħ��
	int				dmg;						// ��ʼ������
	int				defense;					// ��ʼ������
	int				extra_damage;				// �����˺�
	int				extra_defence;				// �����˺�
	int				attack;						// ��ʼ����
	int				armor;						// ��ʼ����
	int				crit_rate;					// ��ʼ����һ����(%)
	int				crit_damage;				// ��ʼ����һ���˺�(%)
	int				anti_stunt;					// ��ʼѣ�ο���
	int				anti_weak;					// ��ʼ��������
	int				anti_slow;					// ��ʼ��������
	int				anti_silence;				// ��ʼ��Ĭ����
	int				anti_sleep;					// ��ʼ˯�߿���
	int				anti_twist;					// ��ʼ���ƿ���

	float			lvlup_exp;					// ÿ��һ���������ľ���
	float 			lvlup_money_average;		// ÿ��һ����������Ǯ����׼ֵ
	float			lvlup_money_var;			// ÿ��һ����������Ǯ������ֵ
	float			lvlup_hp;					// ÿ��һ����������������
	float			lvlup_mp;					// ÿ��һ����������ħ��
	float			lvlup_dmg;					// ÿ��һ���������Ĺ�����
	float			lvlup_defense;				// ÿ��һ���������ķ�����
	float			lvlup_attack;				// ÿ��һ��������������
	float			lvlup_armor;				// ÿ��һ��������������
	float			lvlup_crit_rate;			// ÿ��һ��������������һ����(%)
	float			lvlup_crit_damage;			// ÿ��һ��������������һ���˺�(%)
	float			lvlup_anti_stunt;			// ÿ��һ����������ѣ�ο���
	float			lvlup_anti_weak;			// ÿ��һ������������������
	float			lvlup_anti_slow;			// ÿ��һ���������Ļ�������
	float			lvlup_anti_silence;			// ÿ��һ���������ĳ�Ĭ����
	float			lvlup_anti_sleep;			// ÿ��һ����������˯�߿���
	float			lvlup_anti_twist;			// ÿ��һ���������Ĳ��ƿ���

	int				hp_gen1;					// ���ٻ�Ѫ�ٶ�
	int				hp_gen2;					// ���ٻ�Ѫ�ٶ�
	int				mp_gen1;					// ���ٻ�ħ�ٶ�
	int				mp_gen2;					// ���ٻ�ħ�ٶ�

	unsigned int	role_in_war;				// ��ս�������ݵĽ�ɫ
												// 0 �ޣ�1 ���Ľ�����2 ������3 ������4 Ͷʯ����5 ���͵㣻6 ����㣻7 ����NPC��8 ռ���־��

	int				drop_times;					// �������(1-10)

	struct  
	{
		int		id_drop_table;						// ����� id
		float	prop_drop_table;					// ������ĸ���
	}drop_table[3];

	// �����б�
	struct
	{
		unsigned int	id_skill;				// ����ļ�������ID
		int				level;					// ���ܵļ���
	} skills[32];

	float			dec_crit_rate;
	float			dec_crit_dmg;
	float			armor_prop;
	float			attack_prop;

	unsigned int	tick_stratege;				// ��������Ƿ�ʼ��������ִ��
	unsigned int	task_share;					// �ù����Ƿ����Ϊ���������ɱ�֣������󣬷��������¿ɹ���ɱ�� Added 2012-04-09.
	unsigned int	item_drop_share;			// �ù��ﱻɱ�����Ƿ���������� Added 2012-04-09.
	unsigned int	buff_area_id;				// ������ص�buff��������id
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
	unsigned int	id;							// NPC(����)ID
	namechar		name[32];					// ����, ���15������

	unsigned int	id_type;					// NPC ����, ��NPC_TYPE��ѡȡ
	
	namechar		desc[16];					// NPC��������ʾ��NPC����֮�£����16������

	float			refresh_time;				// ˢ��ʱ��

	unsigned int	attack_rule;				// �Ƿ񱻹��� 0-���ɱ�������1-�ɱ����������������2-�ɱ��������������
	
	int				file_model;					// ģ��·����
	int				head_icon;					// ͷ��

	unsigned int	id_src_monster;				// �����������ԵĹ���ID������ʱ��ʾʹ�øù��������滻��NPC������

	namechar		hello_msg[256];				// �����ף���һ�������ı�

	unsigned int	id_to_discover;				// ���ֺʹ��ͷ������վID

	unsigned int	domain_related;				// �Ƿ�������� 0-��1-��
	unsigned int	guard_npc;					// �Ƿ�����NPC  0-��1-��
	unsigned int	is_vehicle;					// �Ƿ�ͨ���� 0-��1-��

	unsigned int	war_role_config;			// ��ս��ɫ���Ա�id

	// �����б�
	unsigned int	id_talk_service;			// ��̸�ķ���ID
	unsigned int	id_sell_service;			// ������Ʒ�ķ���ID
	unsigned int	id_gshop_majortype;			// �ٱ�����۵Ĵ����ID
	unsigned int	id_buy_service;				// �չ���Ʒ�ķ���ID
	unsigned int	id_task_out_service;		// ������صķ���ID: �����������
	unsigned int	id_task_in_service;			// ������صķ���ID: ��֤����������
	unsigned int	id_task_matter_service;		// ������صķ���ID: ����������Ʒ����
	unsigned int	id_heal_service;			// ���Ƶķ���ID
	unsigned int	id_transmit_service;		// ���͵ķ���ID
	unsigned int	id_proxy_service;			// ���۵ķ���ID
	unsigned int	id_storage_service;			// �ֿ�ķ���ID
	unsigned int	id_war_towerbuild_service;	// ��ս��������ķ���ID
	unsigned int	id_resetprop_service;		// ϴ�����ID
	unsigned int	id_equipbind_service;		// װ���󶨷���
	unsigned int	id_equipdestroy_service;	// װ�����ٷ���
	unsigned int	id_equipundestroy_service;	// װ��������ٷ���
	unsigned int	id_war_archer_service;		// ��ս���򹭼��ַ���
	unsigned int	id_item_trade_service[4];		// ���ｻ�׷���
	unsigned int	id_equip_soul_service;		// �����ںϷ���ID��0��ʾ�������˷���
	unsigned int	id_consign_service;			// ��Ʒ���۷���ID
	unsigned int	id_reputation_shop_service;	// �����̵�ID
	int				id_trans_dest_service;		// ���洫�ͷ���
	int				id_open_trans;				// ��ĳ����������
	unsigned int	combined_services;			// �򵥷�����ϣ����е�ÿһλ����һ������Ҫ�����ķ��񣬿��ܵ��У�
												//		0��ѱ������1�����pk���ɸ�����2����Ʒ������3����վ���ַ���4�����ɷ���5���޸�������Ʒ��6���ʼķ���7����������8��˫������򿨷���
												//		9���������ﵰ����10����ԭ���ﵰ����11����ս��������12���뿪ս������13���㿨���ۣ�14�����ɲֿ����
												//		15��������ط���16����ս��������17��ս���볡����18����ս�ճ��������19������ѱ����20�����������
												//		21������ϳɣ�22����������; 23: ��Ѫ������24��������Ƕ��25�����ǲ����26���ָ�������27��װ����⣻ 28������ս����������
												//		29���ν�ս���������� 30���������������� 31�������ƹ����

	unsigned int	combined_services2;			// �򵥷�����ϣ����е�ÿһλ����һ������Ҫ�����ķ���
												// 0�������޸ķ���1������װ��ǿ��; 2:���pk���������� 3�����pk��ս�߱��� 4:����ս���������� 5������������ط���
												// 6�������������ָ�����7������ս��ս��8:��������սս�� 9������ս�������������ֽ�����10:���̳��ܷ��� 11:�޸��������Ʒ�����⣩ 
												// 12: װ����������; 13: ���͵��������; 14: �ӿ�����ط���; 15: ������ս�ռ����; 16: ��ʯ��ۼ�������; 17: ��ʯ�����������;
												// 18: ��ʯ��۶��Ʒ���; 19: ��ʯ��Ƕ����; 20: ��ʯ�������; 21: ��ʯ��Ʒ����; 22: ��ʯ��������; 23: ��ʯ��ȡ����; 24: ��ʯ��������;
												// 25: ���С��pk��ӱ���; 26: ���С��pk���ӱ���; 27: ���С��pk���߱���; 28: ������ط���; 29: ��������������ʾ; 30: ��������
												// 31: �������

	unsigned int	combined_services3;			// �򵥷�����ϣ����е�ÿһλ����һ������Ҫ�����ķ���
												// 0�����ɸ�����1������������2�����������ںϣ�3������������Ƕ��4�����ñ𹴣�5��װ��������ף�6��������Ƕ��7������ժ����8�����ñ�
												// 9�����ñ𹴣�10������������� 11:����������� 12:��ս���� 13:�������Ȳ�ѯ 14:�������� 15:�������ܿ������� 16:����������������
												// 17:��ˮϯս������ 18:���ɻ��� 19��������� 20: �ֱ�ר�� 21:����Ԫ���̳�

	unsigned int	combined_services4;			// Special_Mode_2
	unsigned int	combined_services5;			// Special_Mode_3
	unsigned int	combined_services6;			// Special_Mode_4

	// ϴPKֵ����
	unsigned int	has_pkvalue_service;		// �Ƿ��ṩϴPKֵ����
	int				fee_per_pkvalue;			// ÿϴһ��PKֵ����Ļ���

	bool			service_install;			// �Ƿ��ṩ��Ƕ����
	bool			service_uninstall;			// �Ƿ��ṩ��ж����
	bool			service_temp1;				// ռλ
	bool			service_temp2;				// ռλ

	unsigned int	id_mine;					// �����Ŀ�
	unsigned int	id_interaction_object;		// �ɽ�������
	unsigned int	collision_in_server;		// �Ƿ������ײ
	float			vehicle_min_length;			// ��ͨ���ߵ���С����
	float			vehicle_min_width;			// ��ͨ���ߵ���С����
	float			vehicle_min_height;			// ��ͨ���ߵ���С�߶�
	float			vehicle_max_length;
	float			vehicle_max_width;
	float			vehicle_max_height;
	int				id_territory;				// ��������id������սר�ã�Ĭ��Ϊ0��ʾ������ս����npc
	int				id_transcription;			// ����ģ��ID(Instance_1)��Ĭ�������Ϊ��0��ʾû�и������ŷ��� Added 2011-07-20.
	int				id_transcription2;			// Instance_2
	int				id_transcription3;			// Instance_3
	int				id_transcription4;			// Instance_4
	int				id_transcription5;			// Instance_5
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
			}special_required[1];

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
		int					require_id;				// �ж���ɵ�����id/�����ͼid
	}element_list[120];
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
		int		condition_type;				// ǰ��������������ͣ��Ժ�߻��������ʾ
		int		condition_arg1;				// ǰ���������������1
		int		condition_arg2;				// ǰ���������������2
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
	int				lvup_exp[4][10];			// �����������辭�飬���ȡ��еȡ��ߵȡ���Ʒ 1-10
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

	unsigned int	phaselist[200];					//Ĭ����Ϣ
};

//���6v6��Ʒ�һ���
struct CROSS6V6_ITEM_EXCHANGE_CONFIG
{
	unsigned int	id;								//ID
	namechar		name[32];						//���ƣ����15������	
	
	struct cross6v6_item_exchange
	{
		int				item_id;					//��ƷID
		int				item_num;					//��Ʒ����
		int				require_currency_id;		//����һ���ID
		int				require_currnecy_num;		//����һ�������
		int				time_limit;					//ʱ������
		int exchange_count;
	}cross6v6_item_exchange_info[15];
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
	}level_list[10];
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
	}faction_item_list[100];
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
	}faction_auction_list[100];
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

//=============================================================================

// Mitkos Changes elements.data v158

// List[193]
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

// List[194]
struct BABY_ESSENCE
{
	unsigned int	id;
	namechar		name[32];
	int				file_model;
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

// List[195]
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

// List[196]
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

// List[197]
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

// List[198]
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

// List[199]
struct BABY_FASHION_ESSENCE
{
	unsigned int	id;
	namechar		name[32];

	int				model_path_id;
	int				icon_path_id;
	int				equip_mask;
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
	int				stack_amt;
	int				trade_behavior;
};

// List[200]
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
	int				stack_amt;
	int				trade_behavior;
};

// List[201]
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
	int				stack_amt;
	int				trade_behavior;
};

// List[202]
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
	int				stack_amt;
	int				trade_behavior;
};

// List[203]
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
	int				stack_amt;
	int				trade_behavior;
};

// List[204]
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

// List[205]
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

// List[206]
struct MATRIX_CARD_UPGREAD_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	int				exp[59];
	float			val[4];
};

// List[207]
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
	} recipes[220];
};

// List[208]
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
	int				stack_amt;
	int				trade_behavior;
};

// List[209]
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
	int				stack_amt;
	int				trade_behavior;
};

// List[210]
struct BABY_TITLE_LIST
{
	unsigned int	id;
	namechar		name[32];

	int				title_list[100];
};

// List[211]
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
	int				stack_amt;
	int				trade_behavior;
};

// List[212]
struct TALENT_SCROLL_ESSENCE
{
	unsigned int	id;
	namechar		name[32];

	int				model_path_id;
	int				icon_path_id;
	int				big_icon;

	int				talent_scroll_config;
	int				position;

	int				decomposition_id;
	int				decomposition_num;

	int				refine_id;
	int				refine_fee;

	int				refine_num[10];

	int				sell_price;
	int				buy_price;
	int				stack_amt;
	int				trade_behavior;
};

// List[213]
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

	int				profession_addon_skill[15];
};

// List[214]
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

// List[215]
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

// List[216]
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

// List[217]
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
		award_item_info	award_item_info[8];
	} consume_award_info[7];
};

// List[218]
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

// List[219]
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

// List[220]
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
	int				stack_amt;
	int				trade_behavior;
};

// List[221]
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
		required_item_info	required_item_info[3];
	};

	struct trade_page
	{
		namechar			title[8];	// wstring:16
		trade_item_info		item[30];
	} trade_page[2];

	int				dialog_id;
};

// List[222]
struct GUESS_ESSENCE
{
	unsigned int	id;
	namechar		name[32];

	int				model_path_id;
	int				icon_path_id;

	int				sell_price;
	int				buy_price;
	int				stack_amt;
	int				trade_behavior;
};

// List[223]
struct ELITE_GAME_RESULT_CONFIG
{
	unsigned int	id;
	namechar		name[32];

	int				result;
};

// List[224]
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

// List[225]
struct TITLE_ITEM_ESSENCE
{
	unsigned int	id;
	namechar		name[32];

	int				model_path_id;
	int				icon_path_id;

	int				title;
	namechar		speak[100];

	int				sell_price;
	int				buy_price;
	int				stack_amt;
	int				trade_behavior;
};

// List[226]
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

// List[227]
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

// List[228]
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

// List[229]
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

// Mitkos Changes elements.data v158 END


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
	DT_MAX,              //数据类型标志，以下枚举项目顺序不能更改
};

#pragma pack(pop, EXP_TYPES_INC)
#endif//_EXP_TYPES_H_