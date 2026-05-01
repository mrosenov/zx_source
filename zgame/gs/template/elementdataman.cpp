#include "elementdataman.h"
using namespace abase;

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdarg.h>
#include <unistd.h>

#ifdef _WINDOWS

#include "EC_MD5Hash.h"

#endif

// ---- gs debug logger (writes to /root/gs_debug.log) ----
static FILE* g_gs_log = NULL;
static void gs_log(const char* fmt, ...)
{
    if (!g_gs_log)
    {
        g_gs_log = fopen("/root/gs_debug.log", "a");
        if (!g_gs_log)
            return;
    }
    time_t t = time(NULL);
    struct tm* tm_info = localtime(&t);
    fprintf(g_gs_log, "[%02d:%02d:%02d][pid=%d] ",
        tm_info->tm_hour, tm_info->tm_min, tm_info->tm_sec,
        (int)getpid());
    va_list args;
    va_start(args, fmt);
    vfprintf(g_gs_log, fmt, args);
    va_end(args);
    fprintf(g_gs_log, "\n");
    fflush(g_gs_log);
}
// ---- end logger ----

const char * DataTypeName[DT_MAX+1] =
{
	"DT_INVALID",		
	"DT_EQUIPMENT_ADDON",
	"DT_EQUIPMENT_MAJOR_TYPE",
	"DT_EQUIPMENT_SUB_TYPE",
	"DT_EQUIPMENT_ESSENCE",
	"DT_REFINE_TICKET_ESSENCE",
	"DT_MEDICINE_MAJOR_TYPE",
	"DT_MEDICINE_SUB_TYPE",
	"DT_MEDICINE_ESSENCE",
	"DT_MATERIAL_MAJOR_TYPE",
	"DT_MATERIAL_SUB_TYPE",
	"DT_MATERIAL_ESSENCE",
	"DT_SKILLTOME_SUB_TYPE",
	"DT_SKILLTOME_ESSENCE",	
	"DT_TRANSMITROLL_ESSENCE",
	"DT_LUCKYROLL_ESSENCE",
	"DT_TOWNSCROLL_ESSENCE",
	"DT_REVIVESCROLL_ESSENCE",
	"DT_TASKMATTER_ESSENCE",
	"DT_DROPTABLE_TYPE",
	"DT_DROPTABLE_ESSENCE",
	"DT_MONSTER_TYPE",
	"DT_MONSTER_ESSENCE",	
	"DT_NPC_TALK_SERVICE",
	"DT_NPC_SELL_SERVICE",
	"DT_NPC_BUY_SERVICE",
	"DT_NPC_TASK_IN_SERVICE",
	"DT_NPC_TASK_OUT_SERVICE",
	"DT_NPC_TASK_MATTER_SERVICE",
	"DT_NPC_HEAL_SERVICE",
	"DT_NPC_TRANSMIT_SERVICE",	
	"DT_NPC_PROXY_SERVICE",
	"DT_NPC_STORAGE_SERVICE",
	"DT_NPC_TYPE",
	"DT_NPC_ESSENCE",
	"DT_TALK_PROC",
	"DT_RECIPE_MAJOR_TYPE",
	"DT_RECIPE_SUB_TYPE",
	"DT_RECIPE_ESSENCE",
	"DT_ENEMY_FACTION_CONFIG",	
	"DT_CHARACTER_CLASS_CONFIG",
	"DT_PARAM_ADJUST_CONFIG",
	"DT_PIE_LOVE_CONFIG",
	"DT_TASKDICE_ESSENCE",
	"DT_TASKNORMALMATTER_ESSENCE",
	"DT_PLAYER_LEVELEXP_CONFIG",
	"DT_MINE_TYPE",
	"DT_MINE_ESSENCE",
	"DT_GM_GENERATOR_TYPE",
	"DT_GM_GENERATOR_ESSENCE",
	"DT_FIREWORKS_ESSENCE",
	"DT_NPC_WAR_TOWERBUILD_SERVICE",
	"DT_PLAYER_SECONDLEVEL_CONFIG",
	"DT_NPC_RESETPROP_SERVICE",
	"DT_ESTONE_ESSENCE",
	"DT_PSTONE_ESSENCE",
	"DT_SSTONE_ESSENCE",
	"DT_RECIPEROLL_MAJOR_TYPE",
	"DT_RECIPEROLL_SUB_TYPE",
	"DT_RECIPEROLL_ESSENCE",
	"DT_SUITE_ESSENCE",
	"DT_DOUBLE_EXP_ESSENCE",
	"DT_DESTROYING_ESSENCE",
	"DT_NPC_EQUIPBIND_SERVICE",
	"DT_NPC_EQUIPDESTROY_SERVICE",
	"DT_NPC_EQUIPUNDESTROY_SERVICE",
	"DT_SKILLMATTER_ESSENCE",
	"DT_VEHICLE_ESSENCE",
	"DT_COUPLE_JUMPTO_ESSENCE",
	"DT_LOTTERY_ESSENCE",
	"DT_CAMRECORDER_ESSENCE",
	"DT_TITLE_PROP_CONFIG",
	"DT_SPECIAL_ID_CONFIG",
	"DT_TEXT_FIREWORKS_ESSENCE",
	"DT_TALISMAN_MAINPART_ESSENCE",
	"DT_TALISMAN_EXPFOOD_ESSENCE",
	"DT_TALISMAN_MERGEKATALYST_ESSENCE",
	"DT_TALISMAN_ENERGYFOOD_ESSENCE",
	"DT_SPEAKER_ESSENCE",
	"DT_PLAYER_TALENT_CONFIG",
	"DT_POTENTIAL_TOME_ESSENCE",
	"DT_WAR_ROLE_CONFIG",
	"DT_NPC_WAR_BUY_ARCHER_SERVICE",
	"DT_SIEGE_ARTILLERY_SCROLL_ESSENCE",
	"DT_PET_BEDGE_ESSENCE",
	"DT_PET_FOOD_ESSENCE",
	"DT_PET_SKILL_ESSENCE",
	"DT_PET_ARMOR_ESSENCE",
	"DT_AUTO_FOOD_ESSENCE",
	"DT_PET_REFINE_ESSENCE",
	"DT_PET_ASSIST_REFINE_ESSENCE",
	"DT_RENASCENCE_SKILL_CONFIG",
	"DT_RENASCENCE_PROP_CONFIG",
	"DT_AIRCRAFT_ESSENCE",
	"DT_FLY_ENERGYFOOD_ESSENCE",
	"DT_ITEM_TRADE_CONFIG",
	"DT_BOOK_ESSENCE",
	"DT_PLAYER_SKILL_POINT_CONFIG",
	"DT_OFFLINE_TRUSTEE_ESSENCE",
	"DT_EQUIP_SOUL_ESSENCE",
	"DT_EQUIP_SOUL_MELD_SERVICE",
	"DT_SPECIAL_NAME_ITEM_ESSENCE",
	"DT_RECYCLE_ITEM_CONFIG",
	"DT_SCORE_TO_RANK_CONFIG",
	"DT_BATTLE_DROP_CONFIG",
	"DT_BATTLE_DEPRIVE_CONFIG",
	"DT_BATTLE_SCORE_CONFIG",
	"DT_GIFT_BAG_ESSENCE",
	"DT_VIP_CARD_ESSENCE",
	"DT_INSTANCING_BATTLE_CONFIG",
	"DT_CHANGE_SHAPE_CARD_ESSENCE",
	"DT_CHANGE_SHAPE_STONE_ESSENCE",
	"DT_CHANGE_SHAPE_PROP_CONFIG",
	"DT_ORIGINAL_SHAPE_CONFIG",
	"DT_LIFE_SKILL_CONFIG",
	"DT_ARENA_DROPTABLE_CONFIG",
	"DT_MERCENARY_CREDENTIAL_ESSENCE",
	"DT_TELEPORTATION_ESSENCE",
	"DT_TELEPORTATION_STONE_ESSENCE",
	"DT_COMBINE_SKILL_EDIT_CONFIG",
	"DT_UPGRADE_EQUIP_CONFIG",
	"DT_UPGRADE_EQUIP_CONFIG_1",
	"DT_UPGRADE_EQUIP_STONE_ESSENCE",
	"DT_NPC_CONSIGN_SERVICE",
	"DT_DEITY_CHARACTER_CONFIG",
	"DT_LOTTERY2_ESSENCE",
	"DT_GEM_CONFIG",
	"DT_GEM_ESSENCE",
	"DT_GEM_SEAL_ESSENCE",
	"DT_GEM_DUST_ESSENCE",
	"DT_GEM_EXTRACT_CONFIG",
	"DT_GENERAL_ARTICLE_ESSENCE",
	"DT_LOTTERY3_ESSENCE",		//Added 2011-07-01.
	"DT_TRANSCRIPTION_CONFIG",	//Added 2011-07-13. 
	"DT_ONLINE_GIFT_BAG_CONFIG",//Added 2011-12-06.
	"DT_SMALL_ONLINE_GIFT_BAG_ESSENCE", //Added 2011-12-06.
	"DT_SCROLL_REGION_CONFIG", //Added 2012-03-20.
	"DT_SCROLL_UNLOCK_ESSENCE",//Added 2012-03-20.
	"DT_SCROLL_DIG_COUNT_INC_ESSENCE",	//Added 2012-03-22.
	"DT_TOWER_TRANSCRIPTION_CONFIG",	//Added 2012-03-31.
	"DT_TOWER_TRANSCRIPTION_PROPERTY_CONFIG",//Added 2012-03-31.
	"DT_RUNE_DATA_CONFIG",//Added 2012.4.17.
	"DT_RUNE_COMB_PROPERTY",//Added 2012.4.17.
	"DT_RUNE_EQUIP_ESSENCE",//Added 2012.4.17.
	"DT_LITTLE_PET_UPGRADE_CONFIG", //Added 2012-04-19.
	"DT_RUNE_SLOT_ESSENCE", //Added 2012-04-21.
	"DT_DROP_INTERNAL_CONFIG",
	"DT_PK2012_GUESS_CONFIG",
	"DT_COLLISION_RAID_TRANSFIGURE_CONFIG",	//Added 2012-08-02.
	"DT_BOOTH_FIGURE_ITEM_ESSENCE",
	"DT_FLAG_BUFF_ITEM_ESSENCE",
	"DT_NPC_REPUTATION_SHOP_SERVICE",
	"DT_NPC_UI_TRANSFER_SERVICE",
	"DT_UI_TRANSFER_CONFIG",
	"DT_XINGZUO_ITEM_ESSENCE",
	"DT_XINGZUO_ENERGY_ITEM_ESSENCE",
	"DT_COLLISION_RAID_AWARD_CONFIG",
	"DT_CASH_MEDIC_MERGE_ITEM_ESSENCE",
	"DT_BUFF_AREA_CONFIG",
	"DT_LIVENESS_CONFIG",
	"DT_CHALLENGE_2012_CONFIG",
	"DT_SALE_PROMOTION_ITEM_ESSENCE",
	"DT_GIFT_BAG_LOTTERY_DELIVER_ESSENCE",
	"DT_LOTTERY_TANGYUAN_ITEM_ESSENCE",
	"DT_TASK_SPECIAL_AWARD_CONFIG",
	"DT_GIFT_PACK_ITEM_ESSENCE",	
	"DT_PROP_ADD_CONFIG",
	"DT_PROP_ADD_MATERIAL_ITEM_ESSENCE",
	"DT_PROP_ADD_ITEM_ESSENCE",
	"DT_KING_WAR_CONFIG",
	"DT_JINFASHEN_TO_MONEY_CONFIG",
	"DT_BATTLE_201304_CONFIG",
	"DT_RUNE2013_ITEM_ESSENCE",
	"DT_RUNE2013_CONFIG",
	"DT_BASHE_AWARD_CONFIG",
	"DT_ARENA_SEASON_TIME_CONFIG",
	"DT_PERSONAL_LEVEL_AWARD_CONFIG",
	"DT_TEAM_SEASON_AWARD_CONFIG",
	"DT_WEEK_CURRNCY_AWARD_CONFIG",
	"DT_COLORANT_ITEM_ESSENCE"
	"DT_COLORANT2_ITEM_ESSENCE"
	"DT_INTERACTION_OBJECT_ESSENCE",
	"DT_INTERACTION_OBJECT_HOOK_CONFIG",
	"DT_COLORANT_CONFIG",
	"DT_FACTION_TRANSFER_CONFIG",
	"DT_BUILDING_REGION_CONFIG",
	"DT_BUILDING_ESSENCE",
	"DT_REGION_BLOCK_ESSENCE",
	"DT_PHASE_CONFIG",
	"DT_CROSS6V6_ITEM_EXCHANGE_CONFIG",
	"DT_TRANSCRIPT_STRATEGY_CONFIG",
	"DT_FACTION_SHOP_CONFIG",
	"DT_FACTION_AUCTION_CONFIG",
	"DT_FACTION_MONEY_SHOP_CONFIG",
	"DT_COMPOUND_MINE_ESSENCE",
	"DT_COLORANT_DEFAULT_CONFIG",
	// v158 new data tables (loaded after talk_proc)
	"DT_SIGN_IN_CONFIG",
	"DT_BABY_ESSENCE",
	"DT_CLASS_BABY_RELATION_CONFIG",
	"DT_BABY_CONFIG",
	"DT_BABY_PROPERTY_CONFIG",
	"DT_BABY_UPGRADE_CONFIG",
	"DT_BABY_FASHION_ESSENCE",
	"DT_BABY_TOY_ESSENCE",
	"DT_BABY_BOOK_ESSENCE",
	"DT_MATRIX_CARD_ESSENCE",
	"DT_MATRIX_EQUIP_ESSENCE",
	"DT_MATRIX_EXP_CONFIG",
	"DT_MATRIX_COLLECT_CONFIG",
	"DT_MATRIX_CARD_UPGREAD_CONFIG",
	"DT_MATRIX_COMBINE_CONFIG",
	"DT_BABY_FOOD_ESSENCE",
	"DT_BABY_TITLE_ESSENCE",
	"DT_BABY_TITLE_LIST",
	"DT_VEHICLE_ENHANCE_ESSENCE",
	"DT_TALENT_SCROLL_ESSENCE",
	"DT_TALENT_SCROLL_CONFIG",
	"DT_TALENT_SCROLL_SKILL_CONFIG",
	"DT_SKILL_REPLACE_CONFIG",
	"DT_HILL_TRANSCRIPTION_CONFIG",
	"DT_CONSUME_AWARD_CONFIG",
	"DT_MULTI_TOWER_PROPERTY_CONFIG",
	"DT_RAID18V18_AWARD_CONFIG",
	"DT_EGGS_GIFT_BAG_ESSENCE",
	"DT_ITEM_TRADE2_CONFIG",
	"DT_GUESS_ESSENCE",
	"DT_ELITE_GAME_RESULT_CONFIG",
	"DT_ELITE_GAME_AWARD_CONFIG",
	"DT_TITLE_ITEM_ESSENCE",
	"DT_SOUL_ENHANCE_CONFIG",
	"DT_SOUL_EXCHANGE_CONFIG",
	"DT_SOUL_DROP_CONFIG",
	"DT_DAILY_SIGN_CONFIG",
	"DT_RED_PACKETS_ESSENCE",
	"DT_FORBID_COMMON_SKILL_CONFIG",
	"DT_FORBID_GODEVIL_SKILL_CONFIG",
	"DT_FORCE_CD_SKILL_CONFIG",
	"DT_TALENT_SCROLL_REFINE_ESSENCE",
	"DT_FACTION_BASE_ACTIVITY_CONFIG",
	"DT_AMULET_ESSENCE",
	"DT_AMULET_CONFIG",
	"DT_AMULET_STONE_ESSENCE",
	"DT_SPECIAL_VIP_CONFIG",
	"DT_CHARGE_AWARD_CONFIG",
	"DT_NEW_TOWNSCROLL_ESSENCE",
	"DT_EQUIPMENT_RANDOMEXT_CONFIG",
	"DT_INDISCRIMINATION_BATTLE_CONFIG",
	"DT_GROUPRAID_EVALAWARD_CONFIG",
	"DT_EQUIPMENT_IDENTIFY_CONFIG",
	"DT_HEADFRAME_ESSENCE",
	"DT_CONVERT_EQUIP_CONFIG",
	"DT_AUSPICIOUS_HALL_CONFIG",
	"DT_DIVINE_ACCESSORY_IMBUE_CONFIG",
	"DT_LEVEL_UP_REWARD_CONFIG",
	"DT_NEW_BOX_ESSENCE",
	"DT_NEW_FASHION_PACK_ESSENCE",
	"DT_NEW_CRAFTING_CONFIG",
	"DT_FAIRYHOUSE_GENERAL_CONFIG",
	"DT_FAIRYHOUSE_PROBABILITY_CONFIG",
	"DT_FAIRYHOUSE_BABY_TRAIT_CONFIG",
	"DT_FAIRYHOUSE_PET_TRAIT_CONFIG",
	"DT_FAIRYHOUSE_CHALLENGE_CONFIG",
	"DT_FAIRYHOUSE_BABY_GEAR_ESSENCE",
	"DT_FAIRYHOUSE_PET_GEAR_ESSENCE",
	"DT_FAIRYHOUSE_TRAIT_LEARN_ESSENCE",
	"DT_PREPAID_BOOK_ESSENCE",
	"DT_TOKEN_BAG_CONFIG",
	"DT_TIMELIMIT_BEAD_ESSENCE",
	"DT_EXTRA_TIMELIMIT_BEAD_ESSENCE",
	"DT_CONSTELLATIONEXP_CONFIG",
	"DT_CONSTELLATION_SKILL_CONFIG",
	"DT_CONSTELLATION_CONFIG",
	"DT_CONSTELLATION_STARS_ESSENCE",
	"DT_CONSTELLATION_SKILL_RELATION_CONFIG",
	"DT_TALENT_SCROLL_TRIGGER_CONFIG",
	"DT_EVENT_REWARD_SET_CONFIG",
	"DT_EXTRA_LOTTERY_ESSENCE",
	"DT_SKILL_SET_CONFIG",
	"DT_GEAR_SCRIPTURE_CONFIG",
	"DT_FIREWORK_ARRAY_ESSENCE",
	"DT_RESET_ITEM_ESSENCE",
	"DT_MARRIAGE_REWARD_CONFIG",
	"DT_MYSTIC_ENHANCE_MATERIAL_CONFIG",
	"DT_LUCK_VALUE_CONFIG",
	"DT_CLASS_ITEM_CONFIG",
	"DT_NEW_GEAR_IDENTIFY_CONFIG",
	"DT_NEW_GEAR_LIST_CONFIG",
	"DT_INSTANCE_INTERCHANGE_CONFIG",
	"DT_SPIRIT_STONE_ESSENCE",
	"DT_VALUE_COST_CONFIG",
	"DT_ANNIVERSARY_ITEM_ESSENCE",
	"DT_SPIRIT_EXRACTION_CONFIG",
	"DT_NEW_DOTA_SKILL_CONFIG",
	"DT_ANOTHER_DOTA_SKILL_CONFIG",
	"DT_EXTRA_DOTA_SKILL_CONFIG",
	"DT_WARDOBE_FASHION_CONFIG",
	"DT_NEW_DYE_CONFIG",
	"DT_EMBROIDERY_CONFIG",
	"DT_MERGED_STAR_SOUL_LIMIT_CONFIG",
	"DT_MERGED_STAR_SOUL_GROWTH_CONFIG",
	"DT_MERGED_STAR_SOUL_SLOT_CONFIG",
	"DT_RUNE2013_MATERIAL_CONFIG",
	"DT_NEW_VIP_CONFIG",
	"DT_RUNE2013_CONSUMPTION_CONFIG",
	"DT_FORBID_DIVINE_SKILL_CONFIG",
	"DT_GIFT_LETTER_ESSENCE",
	"DT_WARDOBE_WEAPON_CONFIG",
	"DT_WARDOBE_FOOTPRINT_CONFIG",
	//"DT_WARDOBE_PET_CONFIG",
	"DT_FOOTPRINT_ESSENCE",
	"DT_FORTUNE_GIFT_PACK_ESSENCE",
	"DT_CANTEEN_EVENT_CONFIG",
	"DT_RETURN_AWARD_CONFIG",
	"DT_MOBILE_PHONE_INFO_CONFIG",
	"DT_BEGINNER_ONLINE_REWARD_CONFIG",
	"DT_MONTHLY_RETURN_AWARD_CONFIG",
	"DT_INSTANT_TIER_CONFIG",
	"DT_PLOT_REWARD_CONFIG",
	"DT_MAX",
};

void eliminate_zero_item(unsigned char * data, size_t stride, size_t count)
{
	unsigned char * d = data;
	unsigned char * p = data;

	for(unsigned int i=0; i<count; i++)
	{
		int flag = *(int *)d;
		if( flag == 0 )
		{
			d += stride;
			continue;
		}

		if( p != d )
			memcpy(p, d, stride);

		d += stride;
		p += stride;
	}

	if( data + stride * count - p )
		memset(p, 0, data + stride * count - p);
}

void elementdataman::NormalizeRandom(float* r, int n)
{
	double t=0;
	float * slider = r;
	for(int i=0; i<n; i++)
	{
		t += *slider;
		slider++;
	}
	if(t>0.00001)
	{
		slider = r;
		for(int i=0; i<n; i++)
		{
			*slider /= t;
			slider++;
		}
	}
	else
	{
		memset(r, 0, sizeof(float) * n);
		*r = 1.f;
	}
}

elementdataman::elementdataman()
{
	unsigned int i;
	for(i=0; i<DT_MAX; i++)
	{
		start_ptr_array.push_back(NULL);
	}

	type_size_array.push_back(0);
	type_size_array.push_back(sizeof(EQUIPMENT_ADDON));
	type_size_array.push_back(sizeof(EQUIPMENT_MAJOR_TYPE));
	type_size_array.push_back(sizeof(EQUIPMENT_SUB_TYPE));
	type_size_array.push_back(sizeof(EQUIPMENT_ESSENCE));
	type_size_array.push_back(sizeof(REFINE_TICKET_ESSENCE));
	type_size_array.push_back(sizeof(MEDICINE_MAJOR_TYPE));
	type_size_array.push_back(sizeof(MEDICINE_SUB_TYPE));
	type_size_array.push_back(sizeof(MEDICINE_ESSENCE));
	type_size_array.push_back(sizeof(MATERIAL_MAJOR_TYPE));
	type_size_array.push_back(sizeof(MATERIAL_SUB_TYPE));
	type_size_array.push_back(sizeof(MATERIAL_ESSENCE));
	type_size_array.push_back(sizeof(SKILLTOME_SUB_TYPE));
	type_size_array.push_back(sizeof(SKILLTOME_ESSENCE));		
	type_size_array.push_back(sizeof(TRANSMITROLL_ESSENCE));
	type_size_array.push_back(sizeof(LUCKYROLL_ESSENCE));
	type_size_array.push_back(sizeof(TOWNSCROLL_ESSENCE));
	type_size_array.push_back(sizeof(REVIVESCROLL_ESSENCE));
	type_size_array.push_back(sizeof(TASKMATTER_ESSENCE));
	type_size_array.push_back(sizeof(DROPTABLE_TYPE));
	type_size_array.push_back(sizeof(DROPTABLE_ESSENCE));
	type_size_array.push_back(sizeof(MONSTER_TYPE));
	type_size_array.push_back(sizeof(MONSTER_ESSENCE));	
	type_size_array.push_back(sizeof(NPC_TALK_SERVICE));
	type_size_array.push_back(sizeof(NPC_SELL_SERVICE));
	type_size_array.push_back(sizeof(NPC_BUY_SERVICE));
	type_size_array.push_back(sizeof(NPC_TASK_IN_SERVICE));
	type_size_array.push_back(sizeof(NPC_TASK_OUT_SERVICE));
	type_size_array.push_back(sizeof(NPC_TASK_MATTER_SERVICE));
	type_size_array.push_back(sizeof(NPC_HEAL_SERVICE));
	type_size_array.push_back(sizeof(NPC_TRANSMIT_SERVICE));
	type_size_array.push_back(sizeof(NPC_PROXY_SERVICE));
	type_size_array.push_back(sizeof(NPC_STORAGE_SERVICE));
	type_size_array.push_back(sizeof(NPC_TYPE));	
	type_size_array.push_back(sizeof(NPC_ESSENCE));
	type_size_array.push_back(sizeof(talk_proc*));
	type_size_array.push_back(sizeof(RECIPE_MAJOR_TYPE));
	type_size_array.push_back(sizeof(RECIPE_SUB_TYPE));
	type_size_array.push_back(sizeof(RECIPE_ESSENCE));
	type_size_array.push_back(sizeof(ENEMY_FACTION_CONFIG));
	type_size_array.push_back(sizeof(CHARACTER_CLASS_CONFIG));
	type_size_array.push_back(sizeof(PARAM_ADJUST_CONFIG));
	type_size_array.push_back(sizeof(PIE_LOVE_CONFIG));
	type_size_array.push_back(sizeof(TASKDICE_ESSENCE));
	type_size_array.push_back(sizeof(TASKNORMALMATTER_ESSENCE));
	type_size_array.push_back(sizeof(PLAYER_LEVELEXP_CONFIG));
	type_size_array.push_back(sizeof(MINE_TYPE));
	type_size_array.push_back(sizeof(MINE_ESSENCE));
	type_size_array.push_back(sizeof(GM_GENERATOR_TYPE));
	type_size_array.push_back(sizeof(GM_GENERATOR_ESSENCE));
	type_size_array.push_back(sizeof(FIREWORKS_ESSENCE));
	type_size_array.push_back(sizeof(NPC_WAR_TOWERBUILD_SERVICE));
	type_size_array.push_back(sizeof(PLAYER_SECONDLEVEL_CONFIG));
	type_size_array.push_back(sizeof(NPC_RESETPROP_SERVICE));
	type_size_array.push_back(sizeof(ESTONE_ESSENCE));
	type_size_array.push_back(sizeof(PSTONE_ESSENCE));
	type_size_array.push_back(sizeof(SSTONE_ESSENCE));
	type_size_array.push_back(sizeof(RECIPEROLL_MAJOR_TYPE));
	type_size_array.push_back(sizeof(RECIPEROLL_SUB_TYPE));
	type_size_array.push_back(sizeof(RECIPEROLL_ESSENCE));
	type_size_array.push_back(sizeof(SUITE_ESSENCE));
	type_size_array.push_back(sizeof(DOUBLE_EXP_ESSENCE));
	type_size_array.push_back(sizeof(DESTROYING_ESSENCE));
	type_size_array.push_back(sizeof(NPC_EQUIPBIND_SERVICE));
	type_size_array.push_back(sizeof(NPC_EQUIPDESTROY_SERVICE));
	type_size_array.push_back(sizeof(NPC_EQUIPUNDESTROY_SERVICE));
	type_size_array.push_back(sizeof(SKILLMATTER_ESSENCE));
	type_size_array.push_back(sizeof(VEHICLE_ESSENCE));
	type_size_array.push_back(sizeof(COUPLE_JUMPTO_ESSENCE));
	type_size_array.push_back(sizeof(LOTTERY_ESSENCE));
	type_size_array.push_back(sizeof(CAMRECORDER_ESSENCE));
	type_size_array.push_back(sizeof(TITLE_PROP_CONFIG));
	type_size_array.push_back(sizeof(SPECIAL_ID_CONFIG));
	type_size_array.push_back(sizeof(TEXT_FIREWORKS_ESSENCE));
	type_size_array.push_back(sizeof(TALISMAN_MAINPART_ESSENCE));
	type_size_array.push_back(sizeof(TALISMAN_EXPFOOD_ESSENCE));
	type_size_array.push_back(sizeof(TALISMAN_MERGEKATALYST_ESSENCE));
	type_size_array.push_back(sizeof(TALISMAN_ENERGYFOOD_ESSENCE));
	type_size_array.push_back(sizeof(SPEAKER_ESSENCE));
	type_size_array.push_back(sizeof(PLAYER_TALENT_CONFIG));
	type_size_array.push_back(sizeof(POTENTIAL_TOME_ESSENCE));
	type_size_array.push_back(sizeof(WAR_ROLE_CONFIG));
	type_size_array.push_back(sizeof(NPC_WAR_BUY_ARCHER_SERVICE));
	type_size_array.push_back(sizeof(SIEGE_ARTILLERY_SCROLL_ESSENCE));
	type_size_array.push_back(sizeof(PET_BEDGE_ESSENCE));
	type_size_array.push_back(sizeof(PET_FOOD_ESSENCE));
	type_size_array.push_back(sizeof(PET_SKILL_ESSENCE));
	type_size_array.push_back(sizeof(PET_ARMOR_ESSENCE));
	type_size_array.push_back(sizeof(PET_AUTO_FOOD_ESSENCE));
	type_size_array.push_back(sizeof(PET_REFINE_ESSENCE));
	type_size_array.push_back(sizeof(PET_ASSIST_REFINE_ESSENCE));
	type_size_array.push_back(sizeof(RENASCENCE_SKILL_CONFIG));
	type_size_array.push_back(sizeof(RENASCENCE_PROP_CONFIG));
	type_size_array.push_back(sizeof(AIRCRAFT_ESSENCE));
	type_size_array.push_back(sizeof(FLY_ENERGYFOOD_ESSENCE));
	type_size_array.push_back(sizeof(ITEM_TRADE_CONFIG));
	type_size_array.push_back(sizeof(BOOK_ESSENCE));
	type_size_array.push_back(sizeof(PLAYER_SKILL_POINT_CONFIG));
	type_size_array.push_back(sizeof(OFFLINE_TRUSTEE_ESSENCE));
	type_size_array.push_back(sizeof(EQUIP_SOUL_ESSENCE));
	type_size_array.push_back(sizeof(EQUIP_SOUL_MELD_SERVICE));
	type_size_array.push_back(sizeof(SPECIAL_NAME_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(RECYCLE_ITEM_CONFIG));
	type_size_array.push_back(sizeof(SCORE_TO_RANK_CONFIG));
	type_size_array.push_back(sizeof(BATTLE_DROP_CONFIG));
	type_size_array.push_back(sizeof(BATTLE_DEPRIVE_CONFIG));
	type_size_array.push_back(sizeof(BATTLE_SCORE_CONFIG));
	type_size_array.push_back(sizeof(GIFT_BAG_ESSENCE));
	type_size_array.push_back(sizeof(VIP_CARD_ESSENCE));
	type_size_array.push_back(sizeof(INSTANCING_BATTLE_CONFIG));
	type_size_array.push_back(sizeof(CHANGE_SHAPE_CARD_ESSENCE));
	type_size_array.push_back(sizeof(CHANGE_SHAPE_STONE_ESSENCE));
	type_size_array.push_back(sizeof(CHANGE_SHAPE_PROP_CONFIG));
	type_size_array.push_back(sizeof(ORIGINAL_SHAPE_CONFIG));
	type_size_array.push_back(sizeof(LIFE_SKILL_CONFIG));
	type_size_array.push_back(sizeof(ARENA_DROPTABLE_CONFIG));
	type_size_array.push_back(sizeof(MERCENARY_CREDENTIAL_ESSENCE));
	type_size_array.push_back(sizeof(TELEPORTATION_ESSENCE));
	type_size_array.push_back(sizeof(TELEPORTATION_STONE_ESSENCE));
	type_size_array.push_back(sizeof(COMBINE_SKILL_EDIT_CONFIG));
	type_size_array.push_back(sizeof(UPGRADE_EQUIP_CONFIG));
	type_size_array.push_back(sizeof(UPGRADE_EQUIP_CONFIG_1));
	type_size_array.push_back(sizeof(UPGRADE_EQUIP_STONE_ESSENCE));
	type_size_array.push_back(sizeof(NPC_CONSIGN_SERVICE));
	type_size_array.push_back(sizeof(DEITY_CHARACTER_CONFIG));
	type_size_array.push_back(sizeof(LOTTERY2_ESSENCE));
	type_size_array.push_back(sizeof(GEM_CONFIG));//Added 2011-03-14.
	type_size_array.push_back(sizeof(GEM_ESSENCE));
	type_size_array.push_back(sizeof(GEM_SEAL_ESSENCE));
	type_size_array.push_back(sizeof(GEM_DUST_ESSENCE));
	type_size_array.push_back(sizeof(GEM_EXTRACT_CONFIG));
	type_size_array.push_back(sizeof(GENERAL_ARTICLE_ESSENCE));
	type_size_array.push_back(sizeof(LOTTERY3_ESSENCE)); //Added 2011-07-01.
	type_size_array.push_back(sizeof(TRANSCRIPTION_CONFIG)); //Added 2011-07-13.
	type_size_array.push_back(sizeof(ONLINE_GIFT_BAG_CONFIG)); //Added 2011-12-06.
	type_size_array.push_back(sizeof(SMALL_ONLINE_GIFT_BAG_ESSENCE)); //Added 2011-12-06.
	type_size_array.push_back(sizeof(SCROLL_REGION_CONFIG));	//Added 2012-03-20.
	type_size_array.push_back(sizeof(SCROLL_UNLOCK_ESSENCE));	//Added 2012-03-20.
	type_size_array.push_back(sizeof(SCROLL_DIG_COUNT_INC_ESSENCE));	//Added 2012-03-22.
	type_size_array.push_back(sizeof(TOWER_TRANSCRIPTION_CONFIG));		//Added 2012-03-31.
	type_size_array.push_back(sizeof(TOWER_TRANSCRIPTION_PROPERTY_CONFIG));//Added 2012-03-31.
	type_size_array.push_back(sizeof(RUNE_DATA_CONFIG));//Added 2012.04.17.
	type_size_array.push_back(sizeof(RUNE_COMB_PROPERTY));//Added 2012.04.17.
	type_size_array.push_back(sizeof(RUNE_EQUIP_ESSENCE));//Added 2012.04.17.
	type_size_array.push_back(sizeof(LITTLE_PET_UPGRADE_CONFIG));	//Added 2012-04-19.
	type_size_array.push_back(sizeof(RUNE_SLOT_ESSENCE));
	type_size_array.push_back(sizeof(DROP_INTERNAL_CONFIG));
	type_size_array.push_back(sizeof(PK2012_GUESS_CONFIG));
	type_size_array.push_back(sizeof(COLLISION_RAID_TRANSFIGURE_CONFIG));	//Added 2012-08-02.
	type_size_array.push_back(sizeof(BOOTH_FIGURE_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(FLAG_BUFF_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(NPC_REPUTATION_SHOP_SERVICE));
	type_size_array.push_back(sizeof(NPC_UI_TRANSFER_SERVICE));
	type_size_array.push_back(sizeof(UI_TRANSFER_CONFIG));
	type_size_array.push_back(sizeof(XINGZUO_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(XINGZUO_ENERGY_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(COLLISION_RAID_AWARD_CONFIG));
	type_size_array.push_back(sizeof(CASH_MEDIC_MERGE_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(BUFF_AREA_CONFIG));
	type_size_array.push_back(sizeof(LIVENESS_CONFIG));
	type_size_array.push_back(sizeof(CHALLENGE_2012_CONFIG));
	type_size_array.push_back(sizeof(SALE_PROMOTION_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(GIFT_BAG_LOTTERY_DELIVER_ESSENCE));
	type_size_array.push_back(sizeof(LOTTERY_TANGYUAN_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(TASK_SPECIAL_AWARD_CONFIG));
	type_size_array.push_back(sizeof(GIFT_PACK_ITEM_ESSENCE));	
	type_size_array.push_back(sizeof(PROP_ADD_MATERIAL_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(PROP_ADD_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(PROP_ADD_CONFIG));
	type_size_array.push_back(sizeof(KING_WAR_CONFIG));
	type_size_array.push_back(sizeof(JINFASHEN_TO_MONEY_CONFIG));
	type_size_array.push_back(sizeof(BATTLE_201304_CONFIG));
	type_size_array.push_back(sizeof(RUNE2013_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(RUNE2013_CONFIG));
	type_size_array.push_back(sizeof(BASHE_AWARD_CONFIG));
	type_size_array.push_back(sizeof(ARENA_SEASON_TIME_CONFIG));
	type_size_array.push_back(sizeof(PERSONAL_LEVEL_AWARD_CONFIG));
	type_size_array.push_back(sizeof(TEAM_SEASON_AWARD_CONFIG));
	type_size_array.push_back(sizeof(WEEK_CURRNCY_AWARD_CONFIG));
	type_size_array.push_back(sizeof(COLORANT_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(COLORANT2_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(INTERACTION_OBJECT_ESSENCE));
	type_size_array.push_back(sizeof(INTERACTION_OBJECT_HOOK_CONFIG));
	type_size_array.push_back(sizeof(COLORANT_CONFIG));
	type_size_array.push_back(sizeof(FACTION_TRANSFER_CONFIG));
	type_size_array.push_back(sizeof(BUILDING_REGION_CONFIG));
	type_size_array.push_back(sizeof(BUILDING_ESSENCE));
	type_size_array.push_back(sizeof(REGION_BLOCK_ESSENCE));
	type_size_array.push_back(sizeof(PHASE_CONFIG));
	type_size_array.push_back(sizeof(CROSS6V6_ITEM_EXCHANGE_CONFIG));
	type_size_array.push_back(sizeof(TRANSCRIPT_STRATEGY_CONFIG));
	type_size_array.push_back(sizeof(FACTION_SHOP_CONFIG));
	type_size_array.push_back(sizeof(FACTION_AUCTION_CONFIG));
	type_size_array.push_back(sizeof(FACTION_MONEY_SHOP_CONFIG));
	type_size_array.push_back(sizeof(COMPOUND_MINE_ESSENCE));
	type_size_array.push_back(sizeof(COLORANT_DEFAULT_CONFIG));
	// v158 new data tables (loaded after talk_proc)
	type_size_array.push_back(sizeof(SIGN_IN_CONFIG));
	type_size_array.push_back(sizeof(BABY_ESSENCE));
	type_size_array.push_back(sizeof(CLASS_BABY_RELATION_CONFIG));
	type_size_array.push_back(sizeof(BABY_CONFIG));
	type_size_array.push_back(sizeof(BABY_PROPERTY_CONFIG));
	type_size_array.push_back(sizeof(BABY_UPGRADE_CONFIG));
	type_size_array.push_back(sizeof(BABY_FASHION_ESSENCE));
	type_size_array.push_back(sizeof(BABY_TOY_ESSENCE));
	type_size_array.push_back(sizeof(BABY_BOOK_ESSENCE));
	type_size_array.push_back(sizeof(MATRIX_CARD_ESSENCE));
	type_size_array.push_back(sizeof(MATRIX_EQUIP_ESSENCE));
	type_size_array.push_back(sizeof(MATRIX_EXP_CONFIG));
	type_size_array.push_back(sizeof(MATRIX_COLLECT_CONFIG));
	type_size_array.push_back(sizeof(MATRIX_CARD_UPGREAD_CONFIG));
	type_size_array.push_back(sizeof(MATRIX_COMBINE_CONFIG));
	type_size_array.push_back(sizeof(BABY_FOOD_ESSENCE));
	type_size_array.push_back(sizeof(BABY_TITLE_ESSENCE));
	type_size_array.push_back(sizeof(BABY_TITLE_LIST));
	type_size_array.push_back(sizeof(VEHICLE_ENHANCE_ESSENCE));
	type_size_array.push_back(sizeof(TALENT_SCROLL_ESSENCE));
	type_size_array.push_back(sizeof(TALENT_SCROLL_CONFIG));
	type_size_array.push_back(sizeof(TALENT_SCROLL_SKILL_CONFIG));
	type_size_array.push_back(sizeof(SKILL_REPLACE_CONFIG));
	type_size_array.push_back(sizeof(HILL_TRANSCRIPTION_CONFIG));
	type_size_array.push_back(sizeof(CONSUME_AWARD_CONFIG));
	type_size_array.push_back(sizeof(MULTI_TOWER_PROPERTY_CONFIG));
	type_size_array.push_back(sizeof(RAID18V18_AWARD_CONFIG));
	type_size_array.push_back(sizeof(EGGS_GIFT_BAG_ESSENCE));
	type_size_array.push_back(sizeof(ITEM_TRADE2_CONFIG));
	type_size_array.push_back(sizeof(GUESS_ESSENCE));
	type_size_array.push_back(sizeof(ELITE_GAME_RESULT_CONFIG));
	type_size_array.push_back(sizeof(ELITE_GAME_AWARD_CONFIG));
	type_size_array.push_back(sizeof(TITLE_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(SOUL_ENHANCE_CONFIG));
	type_size_array.push_back(sizeof(SOUL_EXCHANGE_CONFIG));
	type_size_array.push_back(sizeof(SOUL_DROP_CONFIG));
	type_size_array.push_back(sizeof(DAILY_SIGN_CONFIG));
	type_size_array.push_back(sizeof(RED_PACKETS_ESSENCE));
	type_size_array.push_back(sizeof(FORBID_COMMON_SKILL_CONFIG));
	type_size_array.push_back(sizeof(FORBID_GODEVIL_SKILL_CONFIG));
	type_size_array.push_back(sizeof(FORCE_CD_SKILL_CONFIG));
	type_size_array.push_back(sizeof(TALENT_SCROLL_REFINE_ESSENCE));
	type_size_array.push_back(sizeof(FACTION_BASE_ACTIVITY_CONFIG));
	type_size_array.push_back(sizeof(AMULET_ESSENCE));
	type_size_array.push_back(sizeof(AMULET_CONFIG));
	type_size_array.push_back(sizeof(AMULET_STONE_ESSENCE));
	type_size_array.push_back(sizeof(SPECIAL_VIP_CONFIG));
	type_size_array.push_back(sizeof(CHARGE_AWARD_CONFIG));
	type_size_array.push_back(sizeof(NEW_TOWNSCROLL_ESSENCE));
	type_size_array.push_back(sizeof(EQUIPMENT_RANDOMEXT_CONFIG));
	type_size_array.push_back(sizeof(INDISCRIMINATION_BATTLE_CONFIG));
	type_size_array.push_back(sizeof(GROUPRAID_EVALAWARD_CONFIG));
	type_size_array.push_back(sizeof(EQUIPMENT_IDENTIFY_CONFIG));
	type_size_array.push_back(sizeof(HEADFRAME_ESSENCE));
	type_size_array.push_back(sizeof(CONVERT_EQUIP_CONFIG));
	type_size_array.push_back(sizeof(AUSPICIOUS_HALL_CONFIG));
	type_size_array.push_back(sizeof(DIVINE_ACCESSORY_IMBUE_CONFIG));
	type_size_array.push_back(sizeof(LEVEL_UP_REWARD_CONFIG));
	type_size_array.push_back(sizeof(NEW_BOX_ESSENCE));
	type_size_array.push_back(sizeof(NEW_FASHION_PACK_ESSENCE));
	type_size_array.push_back(sizeof(NEW_CRAFTING_CONFIG));
	type_size_array.push_back(sizeof(FAIRYHOUSE_GENERAL_CONFIG));
	type_size_array.push_back(sizeof(FAIRYHOUSE_PROBABILITY_CONFIG));
	type_size_array.push_back(sizeof(FAIRYHOUSE_BABY_TRAIT_CONFIG));
	type_size_array.push_back(sizeof(FAIRYHOUSE_PET_TRAIT_CONFIG));
	type_size_array.push_back(sizeof(FAIRYHOUSE_CHALLENGE_CONFIG));
	type_size_array.push_back(sizeof(FAIRYHOUSE_BABY_GEAR_ESSENCE));
	type_size_array.push_back(sizeof(FAIRYHOUSE_PET_GEAR_ESSENCE));
	type_size_array.push_back(sizeof(FAIRYHOUSE_TRAIT_LEARN_ESSENCE));
	type_size_array.push_back(sizeof(PREPAID_BOOK_ESSENCE));
	type_size_array.push_back(sizeof(TOKEN_BAG_CONFIG));
	type_size_array.push_back(sizeof(TIMELIMIT_BEAD_ESSENCE));
	type_size_array.push_back(sizeof(EXTRA_TIMELIMIT_BEAD_ESSENCE));
	type_size_array.push_back(sizeof(CONSTELLATIONEXP_CONFIG));
	type_size_array.push_back(sizeof(CONSTELLATION_SKILL_CONFIG));
	type_size_array.push_back(sizeof(CONSTELLATION_CONFIG));
	type_size_array.push_back(sizeof(CONSTELLATION_STARS_ESSENCE));
	type_size_array.push_back(sizeof(CONSTELLATION_SKILL_RELATION_CONFIG));
	type_size_array.push_back(sizeof(TALENT_SCROLL_TRIGGER_CONFIG));
	type_size_array.push_back(sizeof(EVENT_REWARD_SET_CONFIG));
	type_size_array.push_back(sizeof(EXTRA_LOTTERY_ESSENCE));
	type_size_array.push_back(sizeof(SKILL_SET_CONFIG));
	type_size_array.push_back(sizeof(GEAR_SCRIPTURE_CONFIG));
	type_size_array.push_back(sizeof(FIREWORK_ARRAY_ESSENCE));
	type_size_array.push_back(sizeof(RESET_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(MARRIAGE_REWARD_CONFIG));
	type_size_array.push_back(sizeof(MYSTIC_ENHANCE_MATERIAL_CONFIG));
	type_size_array.push_back(sizeof(LUCK_VALUE_CONFIG));
	type_size_array.push_back(sizeof(CLASS_ITEM_CONFIG));
	type_size_array.push_back(sizeof(NEW_GEAR_IDENTIFY_CONFIG));
	type_size_array.push_back(sizeof(NEW_GEAR_LIST_CONFIG));
	type_size_array.push_back(sizeof(INSTANCE_INTERCHANGE_CONFIG));
	type_size_array.push_back(sizeof(SPIRIT_STONE_ESSENCE));
	type_size_array.push_back(sizeof(VALUE_COST_CONFIG));
	type_size_array.push_back(sizeof(ANNIVERSARY_ITEM_ESSENCE));
	type_size_array.push_back(sizeof(SPIRIT_EXRACTION_CONFIG));
	type_size_array.push_back(sizeof(NEW_DOTA_SKILL_CONFIG));
	type_size_array.push_back(sizeof(ANOTHER_DOTA_SKILL_CONFIG));
	type_size_array.push_back(sizeof(EXTRA_DOTA_SKILL_CONFIG));
	type_size_array.push_back(sizeof(WARDOBE_FASHION_CONFIG));
	type_size_array.push_back(sizeof(NEW_DYE_CONFIG));
	type_size_array.push_back(sizeof(EMBROIDERY_CONFIG));
	type_size_array.push_back(sizeof(MERGED_STAR_SOUL_LIMIT_CONFIG));
	type_size_array.push_back(sizeof(MERGED_STAR_SOUL_GROWTH_CONFIG));
	type_size_array.push_back(sizeof(MERGED_STAR_SOUL_SLOT_CONFIG));
	type_size_array.push_back(sizeof(RUNE2013_MATERIAL_CONFIG));
	type_size_array.push_back(sizeof(NEW_VIP_CONFIG));
	type_size_array.push_back(sizeof(RUNE2013_CONSUMPTION_CONFIG));
	type_size_array.push_back(sizeof(FORBID_DIVINE_SKILL_CONFIG));
	type_size_array.push_back(sizeof(GIFT_LETTER_ESSENCE));
	type_size_array.push_back(sizeof(WARDOBE_WEAPON_CONFIG));
	type_size_array.push_back(sizeof(WARDOBE_FOOTPRINT_CONFIG));
	type_size_array.push_back(sizeof(FOOTPRINT_ESSENCE));
	type_size_array.push_back(sizeof(FORTUNE_GIFT_PACK_ESSENCE));
	type_size_array.push_back(sizeof(CANTEEN_EVENT_CONFIG));
	type_size_array.push_back(sizeof(RETURN_AWARD_CONFIG));
	type_size_array.push_back(sizeof(MOBILE_PHONE_INFO_CONFIG));
	type_size_array.push_back(sizeof(BEGINNER_ONLINE_REWARD_CONFIG));
	type_size_array.push_back(sizeof(MONTHLY_RETURN_AWARD_CONFIG));
	type_size_array.push_back(sizeof(INSTANT_TIER_CONFIG));
	type_size_array.push_back(sizeof(PLOT_REWARD_CONFIG));
	type_size_array.push_back(sizeof(0));	//DT_MAX
}

elementdataman::~elementdataman()
{
	for(unsigned int i=0; i<talk_proc_array.size(); i++)
	{
		delete talk_proc_array[i];
	}
}

void elementdataman::add_structure(unsigned int id, EQUIPMENT_ADDON & data)
{
	equipment_addon_array.push_back(data);
	unsigned int pos = equipment_addon_array.size()-1;
	add_id_index(ID_SPACE_ADDON, id, DT_EQUIPMENT_ADDON, pos, &(equipment_addon_array[0]));
}

void elementdataman::add_structure(unsigned int id, EQUIPMENT_MAJOR_TYPE & data)
{
	equipment_major_type_array.push_back(data);
	unsigned int pos = equipment_major_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_EQUIPMENT_MAJOR_TYPE, pos, &(equipment_major_type_array[0]));
}

void elementdataman::add_structure(unsigned int id, EQUIPMENT_SUB_TYPE & data)
{
	equipment_sub_type_array.push_back(data);
	unsigned int pos = equipment_sub_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_EQUIPMENT_SUB_TYPE, pos, &(equipment_sub_type_array[0]));
}

void elementdataman::add_structure(unsigned int id, EQUIPMENT_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	equipment_essence_array.push_back(data);
	unsigned int pos = equipment_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_EQUIPMENT_ESSENCE, pos, &(equipment_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, REFINE_TICKET_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	refine_ticket_essence_array.push_back(data);
	unsigned int pos = refine_ticket_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_REFINE_TICKET_ESSENCE, pos, &(refine_ticket_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, OFFLINE_TRUSTEE_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	offline_trustee_essence_array.push_back(data);
	unsigned int pos = offline_trustee_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_OFFLINE_TRUSTEE_ESSENCE, pos, &(offline_trustee_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, LOTTERY_ESSENCE & data)
{
	data.pile_num_max = 1;
	if( data.dice_count == 0 ) data.dice_count = 1;

	data.proc_type &= ~(1<<7);

	lottery_essence_array.push_back(data);
	unsigned int pos = lottery_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_LOTTERY_ESSENCE, pos, &(lottery_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, LOTTERY2_ESSENCE & data)
{
//	data.pile_num_max = 1;
	if( data.dice_count != 1 ) data.dice_count = 1;

	data.proc_type &= ~(1<<7);

	lottery2_essence_array.push_back(data);
	unsigned int pos = lottery2_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_LOTTERY2_ESSENCE, pos, &(lottery2_essence_array[0]));
}

//Added 2011-07-01.
void elementdataman::add_structure(unsigned int id, LOTTERY3_ESSENCE & data)
{
//	data.pile_num_max = 1;
	if ( data.dice_count != 1 )
	{
		data.dice_count = 1;
	}

	data.proc_type &= ~(1<<7);

	lottery3_essence_array.push_back(data);
	unsigned int pos = lottery3_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_LOTTERY3_ESSENCE, pos, &(lottery3_essence_array[0]));
}

//Added 2011-03-14.
void elementdataman::add_structure(unsigned int id,  GEM_CONFIG & data)
{
	//���Ƚ�ģ��data���뵽��Ӧ������gem_config_array��
	gem_config_array.push_back(data);
	unsigned int pos = gem_config_array.size() - 1;

	//Ȼ��������������¼��configģ��data��id����Ӧ��pos
	add_id_index(ID_SPACE_CONFIG, id, DT_GEM_CONFIG, pos, &(gem_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,  GEM_ESSENCE & data)
{
	//���ȴ���һЩ�����������߼�
	if( data.pile_num_max == 0 )
	{
		data.pile_num_max = 1;
	}
	data.proc_type &= ~(1<<7);

	//Ȼ��ģ��data���뵽��Ӧ������gem_essence_array��
	gem_essence_array.push_back(data);
	unsigned int pos = gem_essence_array.size() - 1;

	//���������������¼��essenceģ��data��id����Ӧ��pos
	add_id_index(ID_SPACE_ESSENCE, id, DT_GEM_ESSENCE, pos, &(gem_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id,  GEM_SEAL_ESSENCE & data)
{
	//���ȴ���һЩ�����������߼�
	if( data.pile_num_max == 0 )
	{
		data.pile_num_max = 1;
	}
	data.proc_type &= ~(1<<7);

	//Ȼ��ģ��data���뵽��Ӧ������gem_seal_essence_array��
	gem_seal_essence_array.push_back(data);
	unsigned int pos = gem_seal_essence_array.size() - 1;

	//�����������������¼��essenceģ��data��id����Ӧ��pos
	add_id_index(ID_SPACE_ESSENCE, id, DT_GEM_SEAL_ESSENCE, pos, &(gem_seal_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id,  GEM_DUST_ESSENCE & data)
{
	//���ȴ���һЩ�����������߼�
	if( data.pile_num_max == 0 )
	{
		data.pile_num_max = 1;
	}
	data.proc_type &= ~(1<<7);

	//Ȼ��ģ��data���뵽��Ӧ������gem_dust_essence_array��
	gem_dust_essence_array.push_back(data);
	unsigned int pos = gem_dust_essence_array.size() - 1;

	//���������������¼��essenceģ��data��id����Ӧ��pos
	add_id_index(ID_SPACE_ESSENCE, id, DT_GEM_DUST_ESSENCE, pos, &(gem_dust_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id,  GEM_EXTRACT_CONFIG & data)
{
	//���ȣ���ģ��data���뵽��Ӧ������gem_extract_config_array��
	gem_extract_config_array.push_back(data);
	unsigned int pos = gem_extract_config_array.size() - 1;

	//���������������¼��configģ��data��id����Ӧ��pos
	add_id_index(ID_SPACE_CONFIG, id, DT_GEM_EXTRACT_CONFIG, pos, &(gem_extract_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,  GENERAL_ARTICLE_ESSENCE & data)
{
	//���ȣ���ģ��data���뵽��Ӧ������general_article_essence_array��
	general_article_essence_array.push_back(data);
	unsigned int pos = general_article_essence_array.size() - 1;

	//���������������¼��essenceģ��data��id����Ӧ��pos
	add_id_index(ID_SPACE_ESSENCE, id, DT_GENERAL_ARTICLE_ESSENCE, pos, &(general_article_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, SMALL_ONLINE_GIFT_BAG_ESSENCE & data)
{
	//���ȴ���һЩ�����������߼�
	if( data.pile_num_max == 0 )
	{
		data.pile_num_max = 1;
	}
	data.proc_type &= ~(1<<7);

	//��Σ���ģ��data���뵽��Ӧ������small_online_gift_bag_essence_array��
	small_online_gift_bag_essence_array.push_back(data);
	unsigned int pos = small_online_gift_bag_essence_array.size() - 1;

	//���������������¼��essenceģ��data��id����Ӧ��λ��pos
	add_id_index(ID_SPACE_ESSENCE, id, DT_SMALL_ONLINE_GIFT_BAG_ESSENCE, pos, &(small_online_gift_bag_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, SCROLL_UNLOCK_ESSENCE & data)
{
	//���ȴ���һЩ�����������߼�
	if( data.pile_num_max == 0 )
	{
		data.pile_num_max = 1;
	}
	data.proc_type &= ~(1<<7);

	//��Σ���ģ��data���뵽��Ӧ������scroll_unlock_essence_array��
	scroll_unlock_essence_array.push_back(data);
	unsigned int pos = scroll_unlock_essence_array.size() - 1;

	//���������������¼��essenceģ��data��id����Ӧ��λ��pos
	add_id_index(ID_SPACE_ESSENCE, id, DT_SCROLL_UNLOCK_ESSENCE, pos, &(scroll_unlock_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, SCROLL_DIG_COUNT_INC_ESSENCE & data)
{
	//���ȴ���һЩ�����������߼�
	if( data.pile_num_max == 0 )
	{
		data.pile_num_max = 1;
	}
	data.proc_type &= ~(1<<7);

	//��Σ���ģ��data���뵽��Ӧ������scroll_dig_count_inc_essence_array��
	scroll_dig_count_inc_essence_array.push_back(data);
	unsigned int pos = scroll_dig_count_inc_essence_array.size() - 1;

	//���������������¼��essenceģ��data��id����Ӧ��λ��pos
	add_id_index(ID_SPACE_ESSENCE, id, DT_SCROLL_DIG_COUNT_INC_ESSENCE, pos, &(scroll_dig_count_inc_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, ONLINE_GIFT_BAG_CONFIG & data)
{
	//���ȣ���ģ��data���뵽��Ӧ������online_gift_bag_config_array��
	online_gift_bag_config_array.push_back(data);
	unsigned int pos = online_gift_bag_config_array.size() - 1;

	//���������������¼��configģ��data��id����Ӧ��λ��pos
	add_id_index(ID_SPACE_CONFIG, id, DT_ONLINE_GIFT_BAG_CONFIG, pos, &(online_gift_bag_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, SCROLL_REGION_CONFIG & data)
{
	//���ȣ���ģ��data���뵽��Ӧ������scroll_region_config_array��
	scroll_region_config_array.push_back(data);
	unsigned int pos = scroll_region_config_array.size() - 1;

	//���������������¼��configģ��data��id����Ӧ��λ��pos
	add_id_index(ID_SPACE_CONFIG, id, DT_SCROLL_REGION_CONFIG, pos, &(scroll_region_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, CAMRECORDER_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	camrecorder_essence_array.push_back(data);
	unsigned int pos = camrecorder_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_CAMRECORDER_ESSENCE, pos, &(camrecorder_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, MEDICINE_MAJOR_TYPE & data)
{
	medicine_major_type_array.push_back(data);

	unsigned int pos = medicine_major_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MEDICINE_MAJOR_TYPE, pos, &(medicine_major_type_array[0]));
}

void elementdataman::add_structure(unsigned int id, MEDICINE_SUB_TYPE & data)
{
	medicine_sub_type_array.push_back(data);

	unsigned int pos = medicine_sub_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MEDICINE_SUB_TYPE, pos, &(medicine_sub_type_array[0]));
}

void elementdataman::add_structure(unsigned int id, MEDICINE_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	medicine_essence_array.push_back(data);

	unsigned int pos = medicine_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MEDICINE_ESSENCE, pos, &(medicine_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, MATERIAL_MAJOR_TYPE & data)
{
	material_major_type_array.push_back(data);

	unsigned int pos = material_major_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MATERIAL_MAJOR_TYPE, pos, &(material_major_type_array[0]));
}

void elementdataman::add_structure(unsigned int id, MATERIAL_SUB_TYPE & data)
{
	material_sub_type_array.push_back(data);

	unsigned int pos = material_sub_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MATERIAL_SUB_TYPE, pos, &(material_sub_type_array[0]));
}

void elementdataman::add_structure(unsigned int id, MATERIAL_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	material_essence_array.push_back(data);

	unsigned int pos = material_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MATERIAL_ESSENCE, pos, &(material_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, DESTROYING_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	destroying_essence_array.push_back(data);
	
	unsigned int pos = destroying_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_DESTROYING_ESSENCE, pos, &(destroying_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, SKILLTOME_SUB_TYPE & data)
{
	skilltome_sub_type_array.push_back(data);
	
	unsigned int pos = skilltome_sub_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_SKILLTOME_SUB_TYPE, pos, &(skilltome_sub_type_array[0]));
}

void elementdataman::add_structure(unsigned int id, SKILLTOME_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	skilltome_essence_array.push_back(data);
	
	unsigned int pos = skilltome_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_SKILLTOME_ESSENCE, pos, &(skilltome_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, TRANSMITROLL_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	transmitroll_essence_array.push_back(data);

	unsigned int pos = transmitroll_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TRANSMITROLL_ESSENCE, pos, &(transmitroll_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, LUCKYROLL_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	luckyroll_essence_array.push_back(data);

	unsigned int pos = luckyroll_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_LUCKYROLL_ESSENCE, pos, &(luckyroll_essence_array[0]));
}

	
void elementdataman::add_structure(unsigned int id, TOWNSCROLL_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	townscroll_essence_array.push_back(data);

	unsigned int pos = townscroll_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TOWNSCROLL_ESSENCE, pos, &(townscroll_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, REVIVESCROLL_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	revivescroll_essence_array.push_back(data);

	unsigned int pos = revivescroll_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_REVIVESCROLL_ESSENCE, pos, &(revivescroll_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, SIEGE_ARTILLERY_SCROLL_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	siege_artillery_scroll_essence_array.push_back(data);

	unsigned int pos = siege_artillery_scroll_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_SIEGE_ARTILLERY_SCROLL_ESSENCE, pos, &(siege_artillery_scroll_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, TASKMATTER_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	taskmatter_essence_array.push_back(data);

	unsigned int pos = taskmatter_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TASKMATTER_ESSENCE, pos, &(taskmatter_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, DROPTABLE_TYPE & data)
{
	droptable_type_array.push_back(data);

	unsigned int pos = droptable_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_DROPTABLE_TYPE, pos, &(droptable_type_array[0]));
}

void elementdataman::add_structure(unsigned int id, DROPTABLE_ESSENCE & data)
{
	int i=0;
	float r[64];
	for(i=0; i<5; i++)	r[i] = data.num_to_drop[i].probability;
	NormalizeRandom(r, 5);
	for(i=0; i<5; i++)	data.num_to_drop[i].probability = r[i];

	//eliminate_zero_item((unsigned char*) data.drops, sizeof(unsigned int)+sizeof(float), 64);

	for(i=0; i<64; i++)	r[i] = data.drops[i].probability;
	NormalizeRandom(r, 64);
	for(i=0; i<64; i++)	data.drops[i].probability = r[i];

	droptable_essence_array.push_back(data);

	unsigned int pos = droptable_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_DROPTABLE_ESSENCE, pos, &(droptable_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, MONSTER_TYPE & data)
{
	monster_type_array.push_back(data);

	unsigned int pos = monster_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MONSTER_TYPE, pos, &(monster_type_array[0]));
}

void elementdataman::add_structure(unsigned int id, MONSTER_ESSENCE & data)
{
	eliminate_zero_item((unsigned char*) data.skills, sizeof(unsigned int)+sizeof(int), 32);

	if( data.drop_times < 0 ) data.drop_times = 0;
	if( data.drop_times > 100 ) data.drop_times = 100;

	monster_essence_array.push_back(data);

	unsigned int pos = monster_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MONSTER_ESSENCE, pos, &(monster_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_TALK_SERVICE & data)
{
	npc_talk_service_array.push_back(data);

	unsigned int pos = npc_talk_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_TALK_SERVICE, pos, &(npc_talk_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_SELL_SERVICE & data)
{
	npc_sell_service_array.push_back(data);
	unsigned int pos = npc_sell_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_SELL_SERVICE, pos, &(npc_sell_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_BUY_SERVICE & data)
{
	npc_buy_service_array.push_back(data);

	unsigned int pos = npc_buy_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_BUY_SERVICE, pos, &(npc_buy_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_TASK_IN_SERVICE & data)
{
	eliminate_zero_item((unsigned char*) data.id_tasks, sizeof(int), 32);

	npc_task_in_service_array.push_back(data);

	unsigned int pos = npc_task_in_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_TASK_IN_SERVICE, pos, &(npc_task_in_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_TASK_OUT_SERVICE & data)
{
	eliminate_zero_item((unsigned char*) data.id_tasks, sizeof(int), 32);
	
	npc_task_out_service_array.push_back(data);

	unsigned int pos = npc_task_out_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_TASK_OUT_SERVICE, pos, &(npc_task_out_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_TASK_MATTER_SERVICE & data)
{
	eliminate_zero_item((unsigned char*) data.tasks, 4*(sizeof(unsigned int)+sizeof(int))+sizeof(int), 16);
	npc_task_matter_service_array.push_back(data);

	unsigned int pos = npc_task_matter_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_TASK_MATTER_SERVICE, pos, &(npc_task_matter_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_HEAL_SERVICE & data)
{
	npc_heal_service_array.push_back(data);

	unsigned int pos = npc_heal_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_HEAL_SERVICE, pos, &(npc_heal_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_TRANSMIT_SERVICE & data)
{
	eliminate_zero_item((unsigned char*) data.targets, sizeof(namechar) * 16 + sizeof(int) * 3 + sizeof(float) * 3, 16);
	npc_transmit_service_array.push_back(data);

	unsigned int pos = npc_transmit_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_TRANSMIT_SERVICE, pos, &(npc_transmit_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_PROXY_SERVICE & data)
{
	npc_proxy_service_array.push_back(data);

	unsigned int pos = npc_proxy_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_PROXY_SERVICE, pos, &(npc_proxy_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_STORAGE_SERVICE & data)
{
	npc_storage_service_array.push_back(data);

	unsigned int pos = npc_storage_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_STORAGE_SERVICE, pos, &(npc_storage_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_EQUIPBIND_SERVICE & data)
{
	npc_equipbind_service_array.push_back(data);
	
	unsigned int pos = npc_equipbind_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_EQUIPBIND_SERVICE, pos, &(npc_equipbind_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_EQUIPDESTROY_SERVICE & data)
{
	npc_equipdestroy_service_array.push_back(data);
	
	unsigned int pos = npc_equipdestroy_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_EQUIPDESTROY_SERVICE, pos, &(npc_equipdestroy_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_WAR_BUY_ARCHER_SERVICE & data)
{
	npc_war_buy_archer_service_array.push_back(data);
	
	unsigned int pos = npc_war_buy_archer_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_WAR_BUY_ARCHER_SERVICE, pos, &(npc_war_buy_archer_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_EQUIPUNDESTROY_SERVICE & data)
{
	npc_equipundestroy_service_array.push_back(data);
	
	unsigned int pos = npc_equipundestroy_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_EQUIPUNDESTROY_SERVICE, pos, &(npc_equipundestroy_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_CONSIGN_SERVICE & data)
{
	npc_consign_service_array.push_back(data);
	
	unsigned int pos = npc_consign_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_CONSIGN_SERVICE, pos, &(npc_consign_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_REPUTATION_SHOP_SERVICE & data)
{
	npc_reputation_shop_service_array.push_back(data);
	
	unsigned int pos = npc_reputation_shop_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_REPUTATION_SHOP_SERVICE, pos, &(npc_reputation_shop_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_UI_TRANSFER_SERVICE & data)
{
	npc_ui_transfer_service_array.push_back(data);
	
	unsigned int pos = npc_ui_transfer_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_UI_TRANSFER_SERVICE, pos, &(npc_ui_transfer_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_TYPE & data)
{
	npc_type_array.push_back(data);

	unsigned int pos = npc_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_TYPE, pos, &(npc_type_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_ESSENCE & data)
{
	npc_essence_array.push_back(data);

	unsigned int pos = npc_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_ESSENCE, pos, &(npc_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, EQUIP_SOUL_ESSENCE & data)
{
	if(data.pile_num_max == 0) data.pile_num_max = 1;

	data.proc_type &= ~(1<<7);

	equip_soul_essence_array.push_back(data);
	unsigned int pos = equip_soul_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_EQUIP_SOUL_ESSENCE, pos, &(equip_soul_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, EQUIP_SOUL_MELD_SERVICE & data)
{
	//if(data.pile_num_max == 0) data.pile_num_max = 1;

	//data.proc_type &= ~(1<<7);

	equip_soul_meld_service_array.push_back(data);
	unsigned int pos = equip_soul_meld_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_EQUIP_SOUL_MELD_SERVICE, pos, &(equip_soul_meld_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, SPECIAL_NAME_ITEM_ESSENCE & data)
{
	if(data.pile_num_max == 0) data.pile_num_max = 1;

	data.proc_type &= ~(1<<7);

	special_name_item_essence_array.push_back(data);
	unsigned int pos = special_name_item_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_SPECIAL_NAME_ITEM_ESSENCE, pos, &(special_name_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, GIFT_BAG_ESSENCE & data)
{
	if(data.pile_num_max == 0) data.pile_num_max = 1;

	data.proc_type &= ~(1<<7);

	gift_bag_essence_array.push_back(data);
	unsigned int pos = gift_bag_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_GIFT_BAG_ESSENCE, pos, &(gift_bag_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, VIP_CARD_ESSENCE & data)
{
	if(data.pile_num_max == 0) data.pile_num_max = 1;

	data.proc_type &= ~(1<<7);

	vip_card_essence_array.push_back(data);
	unsigned int pos = vip_card_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_VIP_CARD_ESSENCE, pos, &(vip_card_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, COLLISION_RAID_AWARD_CONFIG & data)
{
	collision_raid_award_config_array.push_back(data);
	
	unsigned int pos = collision_raid_award_config_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_COLLISION_RAID_AWARD_CONFIG, pos, &(collision_raid_award_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, BUFF_AREA_CONFIG & data)
{
	buff_area_config_array.push_back(data);
	
	unsigned int pos = buff_area_config_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_BUFF_AREA_CONFIG, pos, &(buff_area_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, LIVENESS_CONFIG & data)
{
	liveness_config_array.push_back(data);
	
	unsigned int pos = liveness_config_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_LIVENESS_CONFIG, pos, &(liveness_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, CHALLENGE_2012_CONFIG & data)
{
	challenge_2012_config_array.push_back(data);
	
	unsigned int pos = challenge_2012_config_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_CHALLENGE_2012_CONFIG, pos, &(challenge_2012_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, SALE_PROMOTION_ITEM_ESSENCE & data)
{
	sale_promotion_item_essence_array.push_back(data);
	
	unsigned int pos = sale_promotion_item_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_SALE_PROMOTION_ITEM_ESSENCE, pos, &(sale_promotion_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, GIFT_BAG_LOTTERY_DELIVER_ESSENCE & data)
{
	gift_bag_lottery_deliver_essence_array.push_back(data);
	
	unsigned int pos = gift_bag_lottery_deliver_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_GIFT_BAG_LOTTERY_DELIVER_ESSENCE, pos, &(gift_bag_lottery_deliver_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, LOTTERY_TANGYUAN_ITEM_ESSENCE & data)
{
	lottery_tangyuan_item_essence_array.push_back(data);
	
	unsigned int pos = lottery_tangyuan_item_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_LOTTERY_TANGYUAN_ITEM_ESSENCE, pos, &(lottery_tangyuan_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, GIFT_PACK_ITEM_ESSENCE & data)
{
	gift_pack_item_essence_array.push_back(data);
	
	unsigned int pos = gift_pack_item_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_GIFT_PACK_ITEM_ESSENCE, pos, &(gift_pack_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, PROP_ADD_MATERIAL_ITEM_ESSENCE & data)
{
	prop_add_material_item_essence_array.push_back(data);
	
	unsigned int pos = prop_add_material_item_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_PROP_ADD_MATERIAL_ITEM_ESSENCE, pos, &(prop_add_material_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, PROP_ADD_ITEM_ESSENCE & data)
{
	prop_add_item_essence_array.push_back(data);
	
	unsigned int pos = prop_add_item_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_PROP_ADD_ITEM_ESSENCE, pos, &(prop_add_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, TASK_SPECIAL_AWARD_CONFIG & data)
{
	task_special_award_config_array.push_back(data);
	
	unsigned int pos = task_special_award_config_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TASK_SPECIAL_AWARD_CONFIG, pos, &(task_special_award_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, PROP_ADD_CONFIG & data)
{
	prop_add_config_array.push_back(data);
	
	unsigned int pos = prop_add_config_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_PROP_ADD_CONFIG, pos, &(prop_add_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, KING_WAR_CONFIG & data)
{
	king_war_config_array.push_back(data);
	
	unsigned int pos = king_war_config_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_KING_WAR_CONFIG, pos, &(king_war_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, JINFASHEN_TO_MONEY_CONFIG & data)
{
	jinfashen_to_money_config_array.push_back(data);
	
	unsigned int pos = jinfashen_to_money_config_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_JINFASHEN_TO_MONEY_CONFIG, pos, &(jinfashen_to_money_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, BATTLE_201304_CONFIG & data)
{
	battle_201304_config_array.push_back(data);
	
	unsigned int pos = battle_201304_config_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_BATTLE_201304_CONFIG, pos, &(battle_201304_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, RUNE2013_ITEM_ESSENCE & data)
{
	rune2013_item_essence_array.push_back(data);
	
	unsigned int pos = rune2013_item_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_RUNE2013_ITEM_ESSENCE, pos, &(rune2013_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, COLORANT_ITEM_ESSENCE & data)
{
	colorant_item_essence_array.push_back(data);

	unsigned int pos = colorant_item_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_COLORANT_ITEM_ESSENCE, pos, &(colorant_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, COLORANT2_ITEM_ESSENCE& data)
{
	colorant2_item_essence_array.push_back(data);

	unsigned int pos = colorant2_item_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_COLORANT2_ITEM_ESSENCE, pos, &(colorant2_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, INTERACTION_OBJECT_ESSENCE& data)
{
	interaction_object_essence_array.push_back(data);
	unsigned int pos = interaction_object_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_INTERACTION_OBJECT_ESSENCE, pos, &(interaction_object_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, BUILDING_ESSENCE & data)
{
	building_essence_array.push_back(data);
	unsigned int pos = building_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_BUILDING_ESSENCE, pos, &(building_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, REGION_BLOCK_ESSENCE & data)
{
	region_block_essence_array.push_back(data);
	unsigned int pos = region_block_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_REGION_BLOCK_ESSENCE, pos, &(region_block_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, RUNE2013_CONFIG & data)
{
	rune2013_config_array.push_back(data);
	
	unsigned int pos = rune2013_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_RUNE2013_CONFIG, pos, &(rune2013_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, BASHE_AWARD_CONFIG & data)
{
	bashe_award_config_array.push_back(data);
	
	unsigned int pos = bashe_award_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_BASHE_AWARD_CONFIG, pos, &(bashe_award_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, ARENA_SEASON_TIME_CONFIG & data)
{
	arena_season_time_config_array.push_back(data);
	unsigned int pos = arena_season_time_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_ARENA_SEASON_TIME_CONFIG, pos, &(arena_season_time_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, PERSONAL_LEVEL_AWARD_CONFIG & data)
{
	personal_level_award_config_array.push_back(data);
	unsigned int pos = personal_level_award_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_PERSONAL_LEVEL_AWARD_CONFIG, pos, &(personal_level_award_config_array[0]));		
}

void elementdataman::add_structure(unsigned int id, TEAM_SEASON_AWARD_CONFIG & data)
{
	team_season_award_config_array.push_back(data);
	unsigned int pos = team_season_award_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_TEAM_SEASON_AWARD_CONFIG, pos, &(team_season_award_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, WEEK_CURRNCY_AWARD_CONFIG & data)
{
	week_currency_award_config_array.push_back(data);
	unsigned int pos = week_currency_award_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_WEEK_CURRNCY_AWARD_CONFIG, pos, &(week_currency_award_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, INTERACTION_OBJECT_HOOK_CONFIG & data)
{
	interaction_object_hook_config_array.push_back(data);
	unsigned int pos = interaction_object_hook_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_INTERACTION_OBJECT_HOOK_CONFIG, pos, &(interaction_object_hook_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, COLORANT_CONFIG & data)
{
	colorant_config_array.push_back(data);
	unsigned int pos = colorant_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_COLORANT_CONFIG, pos, &(colorant_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, FACTION_TRANSFER_CONFIG & data)
{
	faction_transfer_config_array.push_back(data);
	unsigned int pos = faction_transfer_config_array.size() -1; 
	add_id_index(ID_SPACE_CONFIG, id, DT_FACTION_TRANSFER_CONFIG, pos, &(faction_transfer_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, BUILDING_REGION_CONFIG & data)
{
	building_region_config_array.push_back(data);
	unsigned int pos = building_region_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_BUILDING_REGION_CONFIG, pos, &(building_region_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, PHASE_CONFIG & data)
{
	phase_config_array.push_back(data);
	unsigned int pos = phase_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_PHASE_CONFIG, pos, &(phase_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, TRANSCRIPT_STRATEGY_CONFIG & data)
{
	transcript_strategy_config_array.push_back(data);
	unsigned int pos = transcript_strategy_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_TRANSCRIPT_STRATEGY_CONFIG, pos, &(transcript_strategy_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, FACTION_SHOP_CONFIG & data)
{
	faction_shop_config_array.push_back(data);
	unsigned int pos = faction_shop_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_FACTION_SHOP_CONFIG, pos, &(faction_shop_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, FACTION_AUCTION_CONFIG & data)
{
	faction_auction_config_array.push_back(data);
	unsigned int pos = faction_auction_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_FACTION_AUCTION_CONFIG, pos, &(faction_auction_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, FACTION_MONEY_SHOP_CONFIG & data)
{
	faction_money_shop_config_array.push_back(data);
	unsigned int pos = faction_money_shop_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_FACTION_MONEY_SHOP_CONFIG, pos, &(faction_money_shop_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, COLORANT_DEFAULT_CONFIG & data)
{
	colorant_default_config_array.push_back(data);
	unsigned int pos = colorant_default_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_COLORANT_DEFAULT_CONFIG, pos, &(colorant_default_config_array[0]));
}

// v158 new data tables
void elementdataman::add_structure(unsigned int id, SIGN_IN_CONFIG & data)
{
	sign_in_config_array.push_back(data);
	unsigned int pos = sign_in_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_SIGN_IN_CONFIG, pos, &(sign_in_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, BABY_ESSENCE & data)
{
	baby_essence_array.push_back(data);
	unsigned int pos = baby_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_BABY_ESSENCE, pos, &(baby_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, CLASS_BABY_RELATION_CONFIG & data)
{
	class_baby_relation_config_array.push_back(data);
	unsigned int pos = class_baby_relation_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_CLASS_BABY_RELATION_CONFIG, pos, &(class_baby_relation_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, BABY_CONFIG & data)
{
	baby_config_array.push_back(data);
	unsigned int pos = baby_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_BABY_CONFIG, pos, &(baby_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, BABY_PROPERTY_CONFIG & data)
{
	baby_property_config_array.push_back(data);
	unsigned int pos = baby_property_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_BABY_PROPERTY_CONFIG, pos, &(baby_property_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, BABY_UPGRADE_CONFIG & data)
{
	baby_upgrade_config_array.push_back(data);
	unsigned int pos = baby_upgrade_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_BABY_UPGRADE_CONFIG, pos, &(baby_upgrade_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, BABY_FASHION_ESSENCE & data)
{
	baby_fashion_essence_array.push_back(data);
	unsigned int pos = baby_fashion_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_BABY_FASHION_ESSENCE, pos, &(baby_fashion_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, BABY_TOY_ESSENCE & data)
{
	baby_toy_essence_array.push_back(data);
	unsigned int pos = baby_toy_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_BABY_TOY_ESSENCE, pos, &(baby_toy_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, BABY_BOOK_ESSENCE & data)
{
	baby_book_essence_array.push_back(data);
	unsigned int pos = baby_book_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_BABY_BOOK_ESSENCE, pos, &(baby_book_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, MATRIX_CARD_ESSENCE & data)
{
	matrix_card_essence_array.push_back(data);
	unsigned int pos = matrix_card_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MATRIX_CARD_ESSENCE, pos, &(matrix_card_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, MATRIX_EQUIP_ESSENCE & data)
{
	matrix_equip_essence_array.push_back(data);
	unsigned int pos = matrix_equip_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MATRIX_EQUIP_ESSENCE, pos, &(matrix_equip_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, MATRIX_EXP_CONFIG & data)
{
	matrix_exp_config_array.push_back(data);
	unsigned int pos = matrix_exp_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_MATRIX_EXP_CONFIG, pos, &(matrix_exp_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, MATRIX_COLLECT_CONFIG & data)
{
	matrix_collect_config_array.push_back(data);
	unsigned int pos = matrix_collect_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_MATRIX_COLLECT_CONFIG, pos, &(matrix_collect_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, MATRIX_CARD_UPGREAD_CONFIG & data)
{
	matrix_card_upgread_config_array.push_back(data);
	unsigned int pos = matrix_card_upgread_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_MATRIX_CARD_UPGREAD_CONFIG, pos, &(matrix_card_upgread_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, MATRIX_COMBINE_CONFIG & data)
{
	matrix_combine_config_array.push_back(data);
	unsigned int pos = matrix_combine_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_MATRIX_COMBINE_CONFIG, pos, &(matrix_combine_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, BABY_FOOD_ESSENCE & data)
{
	baby_food_essence_array.push_back(data);
	unsigned int pos = baby_food_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_BABY_FOOD_ESSENCE, pos, &(baby_food_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, BABY_TITLE_ESSENCE& data)
{
	baby_title_essence_array.push_back(data);
	unsigned int pos = baby_title_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_BABY_TITLE_ESSENCE, pos, &(baby_title_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, BABY_TITLE_LIST& data)
{
	baby_title_list_array.push_back(data);
	unsigned int pos = baby_title_list_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_BABY_TITLE_LIST, pos, &(baby_title_list_array[0]));
}

void elementdataman::add_structure(unsigned int id, VEHICLE_ENHANCE_ESSENCE& data)
{
	vehicle_enhance_essence_array.push_back(data);
	unsigned int pos = vehicle_enhance_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_VEHICLE_ENHANCE_ESSENCE, pos, &(vehicle_enhance_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, TALENT_SCROLL_ESSENCE& data)
{
	talent_scroll_essence_array.push_back(data);
	unsigned int pos = talent_scroll_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TALENT_SCROLL_ESSENCE, pos, &(talent_scroll_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, TALENT_SCROLL_CONFIG& data)
{
	talent_scroll_config_array.push_back(data);
	unsigned int pos = talent_scroll_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_TALENT_SCROLL_CONFIG, pos, &(talent_scroll_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, TALENT_SCROLL_SKILL_CONFIG& data)
{
	talent_scroll_skill_config_array.push_back(data);
	unsigned int pos = talent_scroll_skill_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_TALENT_SCROLL_SKILL_CONFIG, pos, &(talent_scroll_skill_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, SKILL_REPLACE_CONFIG& data)
{
	skill_replace_config_array.push_back(data);
	unsigned int pos = skill_replace_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_SKILL_REPLACE_CONFIG, pos, &(skill_replace_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, HILL_TRANSCRIPTION_CONFIG& data)
{
	hill_transcription_config_array.push_back(data);
	unsigned int pos = hill_transcription_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_HILL_TRANSCRIPTION_CONFIG, pos, &(hill_transcription_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, CONSUME_AWARD_CONFIG& data)
{
	consume_award_config_array.push_back(data);
	unsigned int pos = consume_award_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_CONSUME_AWARD_CONFIG, pos, &(consume_award_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, MULTI_TOWER_PROPERTY_CONFIG& data)
{
	multi_tower_property_config_array.push_back(data);
	unsigned int pos = multi_tower_property_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_MULTI_TOWER_PROPERTY_CONFIG, pos, &(multi_tower_property_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, RAID18V18_AWARD_CONFIG& data)
{
	raid18v18_award_config_array.push_back(data);
	unsigned int pos = raid18v18_award_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_RAID18V18_AWARD_CONFIG, pos, &(raid18v18_award_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, EGGS_GIFT_BAG_ESSENCE& data)
{
	eggs_gift_bag_essence_array.push_back(data);
	unsigned int pos = eggs_gift_bag_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_EGGS_GIFT_BAG_ESSENCE, pos, &(eggs_gift_bag_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, ITEM_TRADE2_CONFIG& data)
{
	item_trade2_config_array.push_back(data);
	unsigned int pos = item_trade2_config_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_ITEM_TRADE2_CONFIG, pos, &(item_trade2_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, GUESS_ESSENCE& data)
{
	guess_essence_array.push_back(data);
	unsigned int pos = guess_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_GUESS_ESSENCE, pos, &(guess_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, ELITE_GAME_RESULT_CONFIG& data)
{
	elite_game_result_config_array.push_back(data);
	unsigned int pos = elite_game_result_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_ELITE_GAME_RESULT_CONFIG, pos, &(elite_game_result_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, ELITE_GAME_AWARD_CONFIG& data)
{
	elite_game_award_config_array.push_back(data);
	unsigned int pos = elite_game_award_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_ELITE_GAME_AWARD_CONFIG, pos, &(elite_game_award_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, TITLE_ITEM_ESSENCE& data)
{
	title_item_essence_array.push_back(data);
	unsigned int pos = title_item_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TITLE_ITEM_ESSENCE, pos, &(title_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, SOUL_ENHANCE_CONFIG& data)
{
	soul_enhance_config_array.push_back(data);
	unsigned int pos = soul_enhance_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_SOUL_ENHANCE_CONFIG, pos, &(soul_enhance_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, SOUL_EXCHANGE_CONFIG& data)
{
	soul_exchange_config_array.push_back(data);
	unsigned int pos = soul_exchange_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_SOUL_EXCHANGE_CONFIG, pos, &(soul_exchange_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, SOUL_DROP_CONFIG& data)
{
	soul_drop_config_array.push_back(data);
	unsigned int pos = soul_drop_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_SOUL_DROP_CONFIG, pos, &(soul_drop_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, DAILY_SIGN_CONFIG& data)
{
	daily_sign_config_array.push_back(data);
	unsigned int pos = daily_sign_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_DAILY_SIGN_CONFIG, pos, &(daily_sign_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, RED_PACKETS_ESSENCE& data)
{
	red_packets_essence_array.push_back(data);
	unsigned int pos = red_packets_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_RED_PACKETS_ESSENCE, pos, &(red_packets_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, FORBID_COMMON_SKILL_CONFIG& data)
{
	forbid_common_skill_config_array.push_back(data);
	unsigned int pos = forbid_common_skill_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_FORBID_COMMON_SKILL_CONFIG, pos, &(forbid_common_skill_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, FORBID_GODEVIL_SKILL_CONFIG& data)
{
	forbid_godevil_skill_config_array.push_back(data);
	unsigned int pos = forbid_godevil_skill_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_FORBID_GODEVIL_SKILL_CONFIG, pos, &(forbid_godevil_skill_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, COMPOUND_MINE_ESSENCE & data)
{
	compound_mine_essence_array.push_back(data);
	unsigned int pos = compound_mine_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_COMPOUND_MINE_ESSENCE, pos, &(compound_mine_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, CROSS6V6_ITEM_EXCHANGE_CONFIG & data)
{
	cross6v6_item_exchange_config_array.push_back(data);
	unsigned int pos = cross6v6_item_exchange_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_CROSS6V6_ITEM_EXCHANGE_CONFIG, pos, &(cross6v6_item_exchange_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, CHANGE_SHAPE_CARD_ESSENCE & data)
{
	if(data.pile_num_max == 0) data.pile_num_max = 1;

	data.proc_type &= ~(1<<7);

	change_shape_card_essence_array.push_back(data);
	unsigned int pos = change_shape_card_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_CHANGE_SHAPE_CARD_ESSENCE, pos, &(change_shape_card_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, CHANGE_SHAPE_STONE_ESSENCE & data)
{
	if(data.pile_num_max == 0) data.pile_num_max = 1;

	data.proc_type &= ~(1<<7);

	change_shape_stone_essence_array.push_back(data);
	unsigned int pos = change_shape_stone_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_CHANGE_SHAPE_STONE_ESSENCE, pos, &(change_shape_stone_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, MERCENARY_CREDENTIAL_ESSENCE & data)
{
	if(data.pile_num_max == 0) data.pile_num_max = 1;

	data.proc_type &= ~(1<<7);

	mercenary_credential_essence_array.push_back(data);
	unsigned int pos = mercenary_credential_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MERCENARY_CREDENTIAL_ESSENCE, pos, &(mercenary_credential_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, TELEPORTATION_ESSENCE & data)
{
	if(data.pile_num_max == 0) data.pile_num_max = 1;

	data.proc_type &= ~(1<<7);

	teleportation_essence_array.push_back(data);
	unsigned int pos = teleportation_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TELEPORTATION_ESSENCE, pos, &(teleportation_essence_array[0]));
}


void elementdataman::add_structure(unsigned int id, TELEPORTATION_STONE_ESSENCE & data)
{
	if(data.pile_num_max == 0) data.pile_num_max = 1;

	data.proc_type &= ~(1<<7);

	teleportation_stone_essence_array.push_back(data);
	unsigned int pos = teleportation_stone_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TELEPORTATION_STONE_ESSENCE, pos, &(teleportation_stone_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, UPGRADE_EQUIP_STONE_ESSENCE & data)
{
	if(data.pile_num_max == 0) data.pile_num_max = 1;

	data.proc_type &= ~(1<<7);

	upgrade_equip_stone_essence_array.push_back(data);
	unsigned int pos = upgrade_equip_stone_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_UPGRADE_EQUIP_STONE_ESSENCE, pos, &(upgrade_equip_stone_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, talk_proc * data)
{
	talk_proc_array.push_back(data);

	unsigned int pos = talk_proc_array.size()-1;
	add_id_index(ID_SPACE_TALK, id, DT_TALK_PROC, pos, &(talk_proc_array[0]));
}

void elementdataman::add_structure(unsigned int id,  RECIPE_MAJOR_TYPE & data)
{
	recipe_major_type_array.push_back(data);

	unsigned int pos = recipe_major_type_array.size()-1;
	add_id_index(ID_SPACE_RECIPE, id, DT_RECIPE_MAJOR_TYPE, pos, &(recipe_major_type_array[0]));
}

void elementdataman::add_structure(unsigned int id,  RECIPE_SUB_TYPE & data)
{
	recipe_sub_type_array.push_back(data);

	unsigned int pos = recipe_sub_type_array.size()-1;
	add_id_index(ID_SPACE_RECIPE, id, DT_RECIPE_SUB_TYPE, pos, &(recipe_sub_type_array[0]));
}

void elementdataman::add_structure(unsigned int id,  RECIPE_ESSENCE & data)
{
	recipe_essence_array.push_back(data);

	unsigned int pos = recipe_essence_array.size()-1;
	add_id_index(ID_SPACE_RECIPE, id, DT_RECIPE_ESSENCE, pos, &(recipe_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, ENEMY_FACTION_CONFIG & data)
{
	enemy_faction_config_array.push_back(data);

	unsigned int pos = enemy_faction_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_ENEMY_FACTION_CONFIG, pos, &(enemy_faction_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,  CHARACTER_CLASS_CONFIG & data)
{
	character_class_config_array.push_back(data);

	unsigned int pos = character_class_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_CHARACTER_CLASS_CONFIG, pos, &(character_class_config_array[0]));

}

void elementdataman::add_structure(unsigned int id,  PARAM_ADJUST_CONFIG & data)
{
	eliminate_zero_item((unsigned char*) data.level_diff_adjust, sizeof(int)+sizeof(float)*5, 16);
	eliminate_zero_item((unsigned char*) data.level_diff_produce, sizeof(int)+sizeof(float), 9);
	param_adjust_config_array.push_back(data);

	unsigned int pos = param_adjust_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_PARAM_ADJUST_CONFIG, pos, &(param_adjust_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,  PIE_LOVE_CONFIG & data)
{
	pie_love_config_array.push_back(data);

	unsigned int pos = pie_love_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_PIE_LOVE_CONFIG, pos, &(pie_love_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,  TASKDICE_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	int i;
	float r[16];
	for(i=0; i<8; i++)	r[i] = data.task_lists[i].probability;
	NormalizeRandom(r, 8);
	for(i=0; i<8; i++)	data.task_lists[i].probability = r[i];

	taskdice_essence_array.push_back(data);

	unsigned int pos = taskdice_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TASKDICE_ESSENCE, pos, &(taskdice_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id,  TASKNORMALMATTER_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	tasknormalmatter_essence_array.push_back(data);

	unsigned int pos = tasknormalmatter_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TASKNORMALMATTER_ESSENCE, pos, &(tasknormalmatter_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 PLAYER_LEVELEXP_CONFIG & data)
{
	player_levelexp_config_array.push_back(data);

	unsigned int pos = player_levelexp_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_PLAYER_LEVELEXP_CONFIG, pos, &(player_levelexp_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 PLAYER_TALENT_CONFIG & data)
{
	player_talent_config_array.push_back(data);

	unsigned int pos = player_talent_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_PLAYER_TALENT_CONFIG, pos, &(player_talent_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 PLAYER_SKILL_POINT_CONFIG & data)
{
	player_skill_point_config_array.push_back(data);

	unsigned int pos = player_skill_point_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_PLAYER_SKILL_POINT_CONFIG, pos, &(player_skill_point_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 MINE_TYPE & data)
{
	mine_type_array.push_back(data);

	unsigned int pos = mine_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MINE_TYPE, pos, &(mine_type_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 MINE_ESSENCE & data)
{
	int		i;

	float r[16];
	for(i=0; i<16; i++)	r[i] = data.materials[i].probability;
	NormalizeRandom(r, 16);
	for(i=0; i<16; i++)	data.materials[i].probability = r[i];
	
	r[0] = data.probability1;
	r[1] = data.probability2;
	NormalizeRandom(r, 2);
	data.probability1 = r[0];
	data.probability2 = r[1];

	eliminate_zero_item((unsigned char*) data.npcgen, sizeof(unsigned int) + sizeof(int) + sizeof(float), 4);
	eliminate_zero_item((unsigned char*) data.aggros, sizeof(unsigned int) + sizeof(float) + sizeof(int), 1);

	mine_essence_array.push_back(data);

	unsigned int pos = mine_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_MINE_ESSENCE, pos, &(mine_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, WAR_ROLE_CONFIG & data)
{
	war_role_config_array.push_back(data);

	unsigned int pos = war_role_config_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_WAR_ROLE_CONFIG, pos, &(war_role_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, GM_GENERATOR_TYPE & data)
{
	gm_generator_type_array.push_back(data);

	unsigned int pos = gm_generator_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_GM_GENERATOR_TYPE, pos, &(gm_generator_type_array[0]));
}

void elementdataman::add_structure(unsigned int id, GM_GENERATOR_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	gm_generator_essence_array.push_back(data);

	unsigned int pos = gm_generator_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_GM_GENERATOR_ESSENCE, pos, &(gm_generator_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, FIREWORKS_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	fireworks_essence_array.push_back(data);

	unsigned int pos = fireworks_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_FIREWORKS_ESSENCE, pos, &(fireworks_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, TEXT_FIREWORKS_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	text_fireworks_essence_array.push_back(data);

	unsigned int pos = text_fireworks_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TEXT_FIREWORKS_ESSENCE, pos, &(text_fireworks_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, TALISMAN_MAINPART_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	talisman_mainpart_essence_array.push_back(data);

	unsigned int pos = talisman_mainpart_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TALISMAN_MAINPART_ESSENCE, pos, &(talisman_mainpart_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, TALISMAN_EXPFOOD_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	talisman_expfood_essence_array.push_back(data);

	unsigned int pos = talisman_expfood_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TALISMAN_EXPFOOD_ESSENCE, pos, &(talisman_expfood_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, TALISMAN_ENERGYFOOD_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	talisman_energyfood_essence_array.push_back(data);

	unsigned int pos = talisman_energyfood_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TALISMAN_ENERGYFOOD_ESSENCE, pos, &(talisman_energyfood_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, SPEAKER_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	speaker_essence_array.push_back(data);

	unsigned int pos = speaker_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_SPEAKER_ESSENCE, pos, &(speaker_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, BOOK_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	book_essence_array.push_back(data);

	unsigned int pos = book_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_BOOK_ESSENCE, pos, &(book_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, TALISMAN_MERGEKATALYST_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	talisman_mergekatalyst_essence_array.push_back(data);

	unsigned int pos = talisman_mergekatalyst_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TALISMAN_MERGEKATALYST_ESSENCE, pos, &(talisman_mergekatalyst_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_WAR_TOWERBUILD_SERVICE & data)
{
	eliminate_zero_item((unsigned char*)data.build_info, sizeof(int)+sizeof(int)*4, 4);

	npc_war_towerbuild_service_array.push_back(data);

	unsigned int pos = npc_war_towerbuild_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_WAR_TOWERBUILD_SERVICE, pos, &(npc_war_towerbuild_service_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 PLAYER_SECONDLEVEL_CONFIG & data)
{
	player_secondlevel_config_array.push_back(data);

	unsigned int pos = player_secondlevel_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_PLAYER_SECONDLEVEL_CONFIG, pos, &(player_secondlevel_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 TITLE_PROP_CONFIG & data)
{
	title_prop_config_array.push_back(data);

	eliminate_zero_item((unsigned char*)data.id_addons, sizeof(int), 3);

	unsigned int pos = title_prop_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_TITLE_PROP_CONFIG, pos, &(title_prop_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 SPECIAL_ID_CONFIG & data)
{
	special_id_config_array.push_back(data);

	unsigned int pos = special_id_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_SPECIAL_ID_CONFIG, pos, &(special_id_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 RENASCENCE_SKILL_CONFIG & data)
{
	renasence_skill_config_array.push_back(data);

	unsigned int pos = renasence_skill_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_RENASCENCE_SKILL_CONFIG, pos, &(renasence_skill_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 RENASCENCE_PROP_CONFIG & data)
{
	renasence_prop_config_array.push_back(data);

	unsigned int pos = renasence_prop_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_RENASCENCE_PROP_CONFIG, pos, &(renasence_prop_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 RECYCLE_ITEM_CONFIG & data)
{
	recycle_item_config_array.push_back(data);

	unsigned int pos = recycle_item_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_RECYCLE_ITEM_CONFIG, pos, &(recycle_item_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 SCORE_TO_RANK_CONFIG & data)
{
	score_to_rank_config_array.push_back(data);

	unsigned int pos = score_to_rank_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_SCORE_TO_RANK_CONFIG, pos, &(score_to_rank_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 BATTLE_DROP_CONFIG & data)
{
	battle_drop_config_array.push_back(data);

	unsigned int pos = battle_drop_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_BATTLE_DROP_CONFIG, pos, &(battle_drop_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 BATTLE_DEPRIVE_CONFIG & data)
{
	battle_deprive_config_array.push_back(data);

	unsigned int pos = battle_deprive_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_BATTLE_DEPRIVE_CONFIG, pos, &(battle_deprive_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 BATTLE_SCORE_CONFIG & data)
{
	battle_score_config_array.push_back(data);

	unsigned int pos = battle_score_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_BATTLE_SCORE_CONFIG, pos, &(battle_score_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 INSTANCING_BATTLE_CONFIG & data)
{
	instancing_battle_config_array.push_back(data);

	unsigned int pos = instancing_battle_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_INSTANCING_BATTLE_CONFIG, pos, &(instancing_battle_config_array[0]));
}

///////////////////////////////////////////////////////////////////////////////
//���Ӹ���ģ�����ݵ�ģ���б��У���������Ӧ��������
//Added 2011-07-13.
///////////////////////////////////////////////////////////////////////////////
void elementdataman::add_structure(unsigned int id,	 TRANSCRIPTION_CONFIG & data)
{
	//������ģ�����ݴ���ģ���б���
	transcription_config_array.push_back(data);

	//������Ӧ��������Ϣ
	unsigned int pos = transcription_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_TRANSCRIPTION_CONFIG, pos, &(transcription_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 TOWER_TRANSCRIPTION_CONFIG & data)
{
	//������ģ�����ݴ���ģ���б���
	tower_transcription_config_array.push_back(data);

	//������Ӧ��������Ϣ
	unsigned int pos = tower_transcription_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_TOWER_TRANSCRIPTION_CONFIG, pos, &(tower_transcription_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 TOWER_TRANSCRIPTION_PROPERTY_CONFIG & data)
{
	//������ģ�����ݴ���ģ���б���
	tower_transcription_property_config_array.push_back(data);

	//������Ӧ��������Ϣ
	unsigned int pos = tower_transcription_property_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_TOWER_TRANSCRIPTION_PROPERTY_CONFIG, pos, &(tower_transcription_property_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 LITTLE_PET_UPGRADE_CONFIG & data)
{
	//����С��ģ�����ݴ���ģ���б���
	little_pet_upgrade_config_array.push_back(data);

	//������Ӧ��������Ϣ
	unsigned int pos = little_pet_upgrade_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_LITTLE_PET_UPGRADE_CONFIG, pos, &(little_pet_upgrade_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 COLLISION_RAID_TRANSFIGURE_CONFIG & data)
{
	//����ײ������������ģ�����ݴ���ģ���б���
	collision_raid_transfigure_config_array.push_back(data);

	//������Ӧ��������Ϣ
	unsigned int pos = collision_raid_transfigure_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_COLLISION_RAID_TRANSFIGURE_CONFIG, pos, &(collision_raid_transfigure_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 UI_TRANSFER_CONFIG & data)
{
	ui_transfer_config_array.push_back(data);
	
	//������Ӧ��������Ϣ
	unsigned int pos = ui_transfer_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_UI_TRANSFER_CONFIG, pos, &(ui_transfer_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 DROP_INTERNAL_CONFIG & data)
{
	//����С��ģ�����ݴ���ģ���б���
	drop_interval_config_array.push_back(data);
	
	//������Ӧ��������Ϣ
	unsigned int pos = drop_interval_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_DROP_INTERNAL_CONFIG, pos, &(drop_interval_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 PK2012_GUESS_CONFIG & data)
{
	//����С��ģ�����ݴ���ģ���б���
	pk2012_guess_config_array.push_back(data);
	
	//������Ӧ��������Ϣ
	unsigned int pos = pk2012_guess_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_PK2012_GUESS_CONFIG, pos, &(pk2012_guess_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 RUNE_DATA_CONFIG & data)
{
	rune_data_config_array.push_back(data);
	
	unsigned int pos = rune_data_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_RUNE_DATA_CONFIG, pos, &(rune_data_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 RUNE_COMB_PROPERTY & data)
{
	rune_comb_property_array.push_back(data);
	
	unsigned int pos = rune_comb_property_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_RUNE_COMB_PROPERTY, pos, &(rune_comb_property_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 RUNE_EQUIP_ESSENCE & data)
{
	rune_equip_array.push_back(data);
	
	unsigned int pos = rune_equip_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_RUNE_EQUIP_ESSENCE, pos, &(rune_equip_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 RUNE_SLOT_ESSENCE & data)
{
	rune_slot_array.push_back(data);
	
	unsigned int pos = rune_slot_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_RUNE_SLOT_ESSENCE, pos, &(rune_slot_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 BOOTH_FIGURE_ITEM_ESSENCE & data)
{
	booth_figure_item_array.push_back(data);
	
	unsigned int pos = booth_figure_item_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_BOOTH_FIGURE_ITEM_ESSENCE, pos, &(booth_figure_item_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 FLAG_BUFF_ITEM_ESSENCE & data)
{
	flag_buff_item_array.push_back(data);
	
	unsigned int pos = flag_buff_item_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_FLAG_BUFF_ITEM_ESSENCE, pos, &(flag_buff_item_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 XINGZUO_ITEM_ESSENCE & data)
{
	xingzuo_item_essence_array.push_back(data);
	
	unsigned int pos = xingzuo_item_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_XINGZUO_ITEM_ESSENCE, pos, &(xingzuo_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 XINGZUO_ENERGY_ITEM_ESSENCE & data)
{
	xingzuo_energy_item_essence_array.push_back(data);
	
	unsigned int pos = xingzuo_energy_item_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_XINGZUO_ENERGY_ITEM_ESSENCE, pos, &(xingzuo_energy_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 CASH_MEDIC_MERGE_ITEM_ESSENCE & data)
{
	cash_medic_merge_item_essence_array.push_back(data);
	
	unsigned int pos = cash_medic_merge_item_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_CASH_MEDIC_MERGE_ITEM_ESSENCE, pos, &(cash_medic_merge_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 CHANGE_SHAPE_PROP_CONFIG & data)
{
	change_shape_prop_config_array.push_back(data);

	unsigned int pos = change_shape_prop_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_CHANGE_SHAPE_PROP_CONFIG, pos, &(change_shape_prop_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 ORIGINAL_SHAPE_CONFIG & data)
{
	original_shape_config_array.push_back(data);

	unsigned int pos = original_shape_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_ORIGINAL_SHAPE_CONFIG, pos, &(original_shape_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 LIFE_SKILL_CONFIG & data)
{
	life_skill_config_array.push_back(data);

	unsigned int pos = life_skill_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_LIFE_SKILL_CONFIG, pos, &(life_skill_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 ARENA_DROPTABLE_CONFIG & data)
{
	arena_droptable_config_array.push_back(data);

	unsigned int pos = arena_droptable_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_ARENA_DROPTABLE_CONFIG, pos, &(arena_droptable_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 COMBINE_SKILL_EDIT_CONFIG & data)
{
	combine_skill_edit_config_array.push_back(data);

	unsigned int pos = combine_skill_edit_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_COMBINE_SKILL_EDIT_CONFIG, pos, &(combine_skill_edit_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 UPGRADE_EQUIP_CONFIG & data)
{
	upgrade_equip_config_array.push_back(data);

	unsigned int pos = upgrade_equip_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_UPGRADE_EQUIP_CONFIG, pos, &(upgrade_equip_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 UPGRADE_EQUIP_CONFIG_1 & data)
{
	upgrade_equip_config_1_array.push_back(data);
	
	unsigned int pos = upgrade_equip_config_1_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_UPGRADE_EQUIP_CONFIG, pos, &(upgrade_equip_config_1_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 DEITY_CHARACTER_CONFIG & data)
{
	deity_character_config_array.push_back(data);

	unsigned int pos = deity_character_config_array.size()-1;
	add_id_index(ID_SPACE_CONFIG, id, DT_DEITY_CHARACTER_CONFIG, pos, &(deity_character_config_array[0]));
}

void elementdataman::add_structure(unsigned int id,	 ITEM_TRADE_CONFIG & data)
{
	item_trade_config_array.push_back(data);

	unsigned int pos = item_trade_config_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_ITEM_TRADE_CONFIG, pos, &(item_trade_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, NPC_RESETPROP_SERVICE & data)
{
	eliminate_zero_item((unsigned char*)data.prop_entry, sizeof(int)+sizeof(int)*4, 15);

	npc_resetprop_service_array.push_back(data);

	unsigned int pos = npc_resetprop_service_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NPC_RESETPROP_SERVICE, pos, &(npc_resetprop_service_array[0]));
}

void elementdataman::add_structure(unsigned int id, ESTONE_ESSENCE & data)
{
	if(data.pile_num_max <= 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	estone_essence_array.push_back(data);

	unsigned int pos = estone_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_ESTONE_ESSENCE, pos, &(estone_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, PSTONE_ESSENCE & data)
{
	if(data.pile_num_max <= 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	pstone_essence_array.push_back(data);

	unsigned int pos = pstone_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_PSTONE_ESSENCE, pos, &(pstone_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, SSTONE_ESSENCE & data)
{
	if(data.pile_num_max <= 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	sstone_essence_array.push_back(data);

	unsigned int pos = sstone_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_SSTONE_ESSENCE, pos, &(sstone_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, POTENTIAL_TOME_ESSENCE & data)
{
	if(data.pile_num_max <= 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	potential_tome_essence_array.push_back(data);

	unsigned int pos = potential_tome_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_POTENTIAL_TOME_ESSENCE, pos, &(potential_tome_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id,  RECIPEROLL_MAJOR_TYPE & data)
{
	reciperoll_major_type_array.push_back(data);

	unsigned int pos = reciperoll_major_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_RECIPEROLL_MAJOR_TYPE, pos, &(reciperoll_major_type_array[0]));
}

void elementdataman::add_structure(unsigned int id,  RECIPEROLL_SUB_TYPE & data)
{
	reciperoll_sub_type_array.push_back(data);

	unsigned int pos = reciperoll_sub_type_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_RECIPEROLL_SUB_TYPE, pos, &(reciperoll_sub_type_array[0]));
}

void elementdataman::add_structure(unsigned int id,  RECIPEROLL_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	reciperoll_essence_array.push_back(data);
	
	unsigned int pos = reciperoll_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_RECIPEROLL_ESSENCE, pos, &(reciperoll_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id,  SUITE_ESSENCE & data)
{
	eliminate_zero_item((unsigned char*) data.equipments, sizeof(unsigned int), 14);
	suite_essence_array.push_back(data);
	
	unsigned int pos = suite_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_SUITE_ESSENCE, pos, &(suite_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, DOUBLE_EXP_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	double_exp_essence_array.push_back(data);

	unsigned int pos = double_exp_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_DOUBLE_EXP_ESSENCE, pos, &(double_exp_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, SKILLMATTER_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	skillmatter_essence_array.push_back(data);
	
	unsigned int pos = skillmatter_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_SKILLMATTER_ESSENCE, pos, &(skillmatter_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, VEHICLE_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	vehicle_essence_array.push_back(data);
	
	unsigned int pos = vehicle_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_VEHICLE_ESSENCE, pos, &(vehicle_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, FLY_ENERGYFOOD_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	fly_energyfood_essence_array.push_back(data);

	unsigned int pos = fly_energyfood_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_FLY_ENERGYFOOD_ESSENCE, pos, &(fly_energyfood_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, AIRCRAFT_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	aircraft_essence_array.push_back(data);
	
	unsigned int pos = aircraft_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_AIRCRAFT_ESSENCE, pos, &(aircraft_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, COUPLE_JUMPTO_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	couple_jumpto_essence_array.push_back(data);
	
	unsigned int pos = couple_jumpto_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_COUPLE_JUMPTO_ESSENCE, pos, &(couple_jumpto_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, PET_BEDGE_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	pet_bedge_essence_array.push_back(data);

	unsigned int pos = pet_bedge_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_PET_BEDGE_ESSENCE, pos, &(pet_bedge_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, PET_FOOD_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	pet_food_essence_array.push_back(data);

	unsigned int pos = pet_food_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_PET_FOOD_ESSENCE, pos, &(pet_food_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, PET_AUTO_FOOD_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	pet_auto_food_essence_array.push_back(data);
	unsigned int pos = pet_auto_food_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_PET_AUTO_FOOD_ESSENCE, pos, &(pet_auto_food_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, PET_REFINE_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	pet_refine_essence_array.push_back(data);
	unsigned int pos = pet_refine_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_PET_REFINE_ESSENCE, pos, &(pet_refine_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, PET_ASSIST_REFINE_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	pet_assist_refine_essence_array.push_back(data);
	unsigned int pos = pet_assist_refine_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_PET_ASSIST_REFINE_ESSENCE, pos, &(pet_assist_refine_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, PET_SKILL_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	pet_skill_essence_array.push_back(data);

	unsigned int pos = pet_skill_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_PET_SKILL_ESSENCE, pos, &(pet_skill_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, PET_ARMOR_ESSENCE & data)
{
	if(data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1<<7);

	pet_armor_essence_array.push_back(data);

	unsigned int pos = pet_armor_essence_array.size()-1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_PET_ARMOR_ESSENCE, pos, &(pet_armor_essence_array[0]));
}

///  NEW
void elementdataman::add_structure(unsigned int id, FORCE_CD_SKILL_CONFIG& data)
{
	force_cd_skill_config_array.push_back(data);

	unsigned int pos = force_cd_skill_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_FORCE_CD_SKILL_CONFIG, pos, &(force_cd_skill_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, TALENT_SCROLL_REFINE_ESSENCE& data)
{
	if (data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1 << 7);

	talent_scroll_refine_essence_array.push_back(data);

	unsigned int pos = talent_scroll_refine_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TALENT_SCROLL_REFINE_ESSENCE, pos, &(talent_scroll_refine_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, FACTION_BASE_ACTIVITY_CONFIG& data)
{
	faction_base_activity_config_array.push_back(data);

	unsigned int pos = faction_base_activity_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_FACTION_BASE_ACTIVITY_CONFIG, pos, &(faction_base_activity_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, AMULET_ESSENCE& data)
{
	if (data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1 << 7);

	amulet_essence_array.push_back(data);

	unsigned int pos = amulet_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_AMULET_ESSENCE, pos, &(amulet_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, AMULET_CONFIG& data)
{
	amulet_config_array.push_back(data);

	unsigned int pos = amulet_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_AMULET_CONFIG, pos, &(amulet_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, AMULET_STONE_ESSENCE& data)
{
	if (data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1 << 7);

	amulet_stone_essence_array.push_back(data);

	unsigned int pos = amulet_stone_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_AMULET_STONE_ESSENCE, pos, &(amulet_stone_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, SPECIAL_VIP_CONFIG& data)
{
	special_vip_config_array.push_back(data);

	unsigned int pos = special_vip_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_SPECIAL_VIP_CONFIG, pos, &(special_vip_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, CHARGE_AWARD_CONFIG& data)
{
	charge_award_config_array.push_back(data);

	unsigned int pos = charge_award_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_CHARGE_AWARD_CONFIG, pos, &(charge_award_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, NEW_TOWNSCROLL_ESSENCE& data)
{
	if (data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1 << 7);

	new_townscroll_essence_array.push_back(data);

	unsigned int pos = new_townscroll_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NEW_TOWNSCROLL_ESSENCE, pos, &(new_townscroll_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, EQUIPMENT_RANDOMEXT_CONFIG& data)
{
	equipment_randomext_config_array.push_back(data);

	unsigned int pos = equipment_randomext_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_EQUIPMENT_RANDOMEXT_CONFIG, pos, &(equipment_randomext_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, INDISCRIMINATION_BATTLE_CONFIG& data)
{
	indiscrimination_battle_config_array.push_back(data);

	unsigned int pos = indiscrimination_battle_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_INDISCRIMINATION_BATTLE_CONFIG, pos, &(indiscrimination_battle_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, GROUPRAID_EVALAWARD_CONFIG& data)
{
	groupraid_evalaward_config_array.push_back(data);

	unsigned int pos = groupraid_evalaward_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_GROUPRAID_EVALAWARD_CONFIG, pos, &(groupraid_evalaward_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, EQUIPMENT_IDENTIFY_CONFIG& data)
{
	equipment_identify_config_array.push_back(data);

	unsigned int pos = equipment_identify_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_EQUIPMENT_IDENTIFY_CONFIG, pos, &(equipment_identify_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, HEADFRAME_ESSENCE& data)
{
	if (data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1 << 7);

	headframe_essence_array.push_back(data);

	unsigned int pos = headframe_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_HEADFRAME_ESSENCE, pos, &(headframe_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, CONVERT_EQUIP_CONFIG& data)
{
	convert_equip_config_array.push_back(data);

	unsigned int pos = convert_equip_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_CONVERT_EQUIP_CONFIG, pos, &(convert_equip_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, AUSPICIOUS_HALL_CONFIG& data)
{
	auspicious_hall_config_array.push_back(data);

	unsigned int pos = auspicious_hall_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_AUSPICIOUS_HALL_CONFIG, pos, &(auspicious_hall_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, DIVINE_ACCESSORY_IMBUE_CONFIG& data)
{
	divine_accessory_imbue_config_array.push_back(data);

	unsigned int pos = divine_accessory_imbue_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_DIVINE_ACCESSORY_IMBUE_CONFIG, pos, &(divine_accessory_imbue_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, LEVEL_UP_REWARD_CONFIG& data)
{
	level_up_reward_config_array.push_back(data);

	unsigned int pos = level_up_reward_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_LEVEL_UP_REWARD_CONFIG, pos, &(level_up_reward_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, NEW_BOX_ESSENCE& data)
{
	if (data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1 << 7);

	new_box_essence_array.push_back(data);

	unsigned int pos = new_box_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NEW_BOX_ESSENCE, pos, &(new_box_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, NEW_FASHION_PACK_ESSENCE& data)
{
	if (data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1 << 7);

	new_fashion_pack_essence_array.push_back(data);

	unsigned int pos = new_fashion_pack_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_NEW_FASHION_PACK_ESSENCE, pos, &(new_fashion_pack_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, NEW_CRAFTING_CONFIG& data)
{
	new_crafting_config_array.push_back(data);

	unsigned int pos = new_crafting_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_NEW_CRAFTING_CONFIG, pos, &(new_crafting_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, FAIRYHOUSE_GENERAL_CONFIG& data)
{
	fairyhouse_general_config_array.push_back(data);

	unsigned int pos = fairyhouse_general_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_FAIRYHOUSE_GENERAL_CONFIG, pos, &(fairyhouse_general_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, FAIRYHOUSE_PROBABILITY_CONFIG& data)
{
	fairyhouse_probability_config_array.push_back(data);

	unsigned int pos = fairyhouse_probability_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_FAIRYHOUSE_PROBABILITY_CONFIG, pos, &(fairyhouse_probability_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, FAIRYHOUSE_BABY_TRAIT_CONFIG& data)
{
	fairyhouse_baby_trait_config_array.push_back(data);

	unsigned int pos = fairyhouse_baby_trait_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_FAIRYHOUSE_BABY_TRAIT_CONFIG, pos, &(fairyhouse_baby_trait_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, FAIRYHOUSE_PET_TRAIT_CONFIG& data)
{
	fairyhouse_pet_trait_config_array.push_back(data);

	unsigned int pos = fairyhouse_pet_trait_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_FAIRYHOUSE_PET_TRAIT_CONFIG, pos, &(fairyhouse_pet_trait_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, FAIRYHOUSE_CHALLENGE_CONFIG& data)
{
	fairyhouse_challenge_config_array.push_back(data);

	unsigned int pos = fairyhouse_challenge_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_FAIRYHOUSE_CHALLENGE_CONFIG, pos, &(fairyhouse_challenge_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, FAIRYHOUSE_BABY_GEAR_ESSENCE& data)
{
	if (data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1 << 7);

	fairyhouse_baby_gear_essence_array.push_back(data);

	unsigned int pos = fairyhouse_baby_gear_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_FAIRYHOUSE_BABY_GEAR_ESSENCE, pos, &(fairyhouse_baby_gear_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, FAIRYHOUSE_PET_GEAR_ESSENCE& data)
{
	if (data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1 << 7);

	fairyhouse_pet_gear_essence_array.push_back(data);

	unsigned int pos = fairyhouse_pet_gear_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_FAIRYHOUSE_PET_GEAR_ESSENCE, pos, &(fairyhouse_pet_gear_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, FAIRYHOUSE_TRAIT_LEARN_ESSENCE& data)
{
	if (data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1 << 7);

	fairyhouse_trait_learn_essence_array.push_back(data);

	unsigned int pos = fairyhouse_trait_learn_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_FAIRYHOUSE_TRAIT_LEARN_ESSENCE, pos, &(fairyhouse_trait_learn_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, PREPAID_BOOK_ESSENCE& data)
{
	if (data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1 << 7);

	prepaid_book_essence_array.push_back(data);

	unsigned int pos = prepaid_book_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_PREPAID_BOOK_ESSENCE, pos, &(prepaid_book_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, TOKEN_BAG_CONFIG& data)
{
	token_bag_config_array.push_back(data);

	unsigned int pos = token_bag_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_TOKEN_BAG_CONFIG, pos, &(token_bag_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, TIMELIMIT_BEAD_ESSENCE& data)
{
	if (data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1 << 7);

	timelimit_bead_essence_array.push_back(data);

	unsigned int pos = timelimit_bead_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_TIMELIMIT_BEAD_ESSENCE, pos, &(timelimit_bead_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, EXTRA_TIMELIMIT_BEAD_ESSENCE& data)
{
	if (data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1 << 7);

	extra_timelimit_bead_essence_array.push_back(data);

	unsigned int pos = extra_timelimit_bead_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_EXTRA_TIMELIMIT_BEAD_ESSENCE, pos, &(extra_timelimit_bead_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, CONSTELLATIONEXP_CONFIG& data)
{
	constellationexp_config_array.push_back(data);

	unsigned int pos = constellationexp_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_CONSTELLATIONEXP_CONFIG, pos, &(constellationexp_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, CONSTELLATION_SKILL_CONFIG& data)
{
	constellation_skill_config_array.push_back(data);

	unsigned int pos = constellation_skill_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_CONSTELLATION_SKILL_CONFIG, pos, &(constellation_skill_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, CONSTELLATION_CONFIG& data)
{
	constellation_config_array.push_back(data);

	unsigned int pos = constellation_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_CONSTELLATION_CONFIG, pos, &(constellation_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, CONSTELLATION_STARS_ESSENCE& data)
{
	if (data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1 << 7);

	constellation_stars_essence_array.push_back(data);

	unsigned int pos = constellation_stars_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_CONSTELLATION_STARS_ESSENCE, pos, &(constellation_stars_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, CONSTELLATION_SKILL_RELATION_CONFIG& data)
{
	constellation_skill_relation_config_array.push_back(data);

	unsigned int pos = constellation_skill_relation_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_CONSTELLATION_SKILL_RELATION_CONFIG, pos, &(constellation_skill_relation_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, TALENT_SCROLL_TRIGGER_CONFIG& data)
{
	talent_scroll_trigger_config_array.push_back(data);

	unsigned int pos = talent_scroll_trigger_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_TALENT_SCROLL_TRIGGER_CONFIG, pos, &(talent_scroll_trigger_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, EVENT_REWARD_SET_CONFIG& data)
{
	event_reward_set_config_array.push_back(data);

	unsigned int pos = event_reward_set_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_EVENT_REWARD_SET_CONFIG, pos, &(event_reward_set_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, EXTRA_LOTTERY_ESSENCE& data)
{
	if (data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1 << 7);

	extra_lottery_essence_array.push_back(data);

	unsigned int pos = extra_lottery_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_EXTRA_LOTTERY_ESSENCE, pos, &(extra_lottery_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, SKILL_SET_CONFIG& data)
{
	skill_set_config_array.push_back(data);

	unsigned int pos = skill_set_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_SKILL_SET_CONFIG, pos, &(skill_set_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, GEAR_SCRIPTURE_CONFIG& data)
{
	gear_scripture_config_array.push_back(data);

	unsigned int pos = gear_scripture_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_GEAR_SCRIPTURE_CONFIG, pos, &(gear_scripture_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, FIREWORK_ARRAY_ESSENCE& data)
{
	if (data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1 << 7);

	firework_array_essence_array.push_back(data);

	unsigned int pos = firework_array_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_FIREWORK_ARRAY_ESSENCE, pos, &(firework_array_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, RESET_ITEM_ESSENCE& data)
{
	if (data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1 << 7);

	reset_item_essence_array.push_back(data);

	unsigned int pos = reset_item_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_RESET_ITEM_ESSENCE, pos, &(reset_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, MARRIAGE_REWARD_CONFIG& data)
{
	marriage_reward_config_array.push_back(data);

	unsigned int pos = marriage_reward_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_MARRIAGE_REWARD_CONFIG, pos, &(marriage_reward_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, MYSTIC_ENHANCE_MATERIAL_CONFIG& data)
{
	mystic_enhance_material_config_array.push_back(data);

	unsigned int pos = mystic_enhance_material_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_MYSTIC_ENHANCE_MATERIAL_CONFIG, pos, &(mystic_enhance_material_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, LUCK_VALUE_CONFIG& data)
{
	luck_value_config_array.push_back(data);

	unsigned int pos = luck_value_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_LUCK_VALUE_CONFIG, pos, &(luck_value_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, CLASS_ITEM_CONFIG& data)
{
	class_item_config_array.push_back(data);

	unsigned int pos = class_item_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_CLASS_ITEM_CONFIG, pos, &(class_item_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, NEW_GEAR_IDENTIFY_CONFIG& data)
{
	new_gear_identify_config_array.push_back(data);

	unsigned int pos = new_gear_identify_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_NEW_GEAR_IDENTIFY_CONFIG, pos, &(new_gear_identify_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, NEW_GEAR_LIST_CONFIG& data)
{
	new_gear_list_config_array.push_back(data);

	unsigned int pos = new_gear_list_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_NEW_GEAR_LIST_CONFIG, pos, &(new_gear_list_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, INSTANCE_INTERCHANGE_CONFIG& data)
{
	instance_interchange_config_array.push_back(data);

	unsigned int pos = instance_interchange_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_INSTANCE_INTERCHANGE_CONFIG, pos, &(instance_interchange_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, SPIRIT_STONE_ESSENCE& data)
{
	if (data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1 << 7);

	spirit_stone_essence_array.push_back(data);

	unsigned int pos = spirit_stone_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_SPIRIT_STONE_ESSENCE, pos, &(spirit_stone_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, VALUE_COST_CONFIG& data)
{
	value_cost_config_array.push_back(data);

	unsigned int pos = value_cost_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_VALUE_COST_CONFIG, pos, &(value_cost_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, ANNIVERSARY_ITEM_ESSENCE& data)
{
	if (data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1 << 7);

	anniversary_item_essence_array.push_back(data);

	unsigned int pos = anniversary_item_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_ANNIVERSARY_ITEM_ESSENCE, pos, &(anniversary_item_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, SPIRIT_EXRACTION_CONFIG& data)
{
	spirit_exraction_config_array.push_back(data);

	unsigned int pos = spirit_exraction_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_SPIRIT_EXRACTION_CONFIG, pos, &(spirit_exraction_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, NEW_DOTA_SKILL_CONFIG& data)
{
	new_dota_skill_config_array.push_back(data);

	unsigned int pos = new_dota_skill_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_NEW_DOTA_SKILL_CONFIG, pos, &(new_dota_skill_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, ANOTHER_DOTA_SKILL_CONFIG& data)
{
	another_dota_skill_config_array.push_back(data);

	unsigned int pos = another_dota_skill_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_ANOTHER_DOTA_SKILL_CONFIG, pos, &(another_dota_skill_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, EXTRA_DOTA_SKILL_CONFIG& data)
{
	extra_dota_skill_config_array.push_back(data);

	unsigned int pos = extra_dota_skill_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_EXTRA_DOTA_SKILL_CONFIG, pos, &(extra_dota_skill_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, WARDOBE_FASHION_CONFIG& data)
{
	wardobe_fashion_config_array.push_back(data);

	unsigned int pos = wardobe_fashion_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_WARDOBE_FASHION_CONFIG, pos, &(wardobe_fashion_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, NEW_DYE_CONFIG& data)
{
	new_dye_config_array.push_back(data);

	unsigned int pos = new_dye_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_NEW_DYE_CONFIG, pos, &(new_dye_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, EMBROIDERY_CONFIG& data)
{
	embroidery_config_array.push_back(data);

	unsigned int pos = embroidery_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_EMBROIDERY_CONFIG, pos, &(embroidery_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, MERGED_STAR_SOUL_LIMIT_CONFIG& data)
{
	merged_star_soul_limit_config_array.push_back(data);

	unsigned int pos = merged_star_soul_limit_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_MERGED_STAR_SOUL_LIMIT_CONFIG, pos, &(merged_star_soul_limit_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, MERGED_STAR_SOUL_GROWTH_CONFIG& data)
{
	merged_star_soul_growth_config_array.push_back(data);

	unsigned int pos = merged_star_soul_growth_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_MERGED_STAR_SOUL_GROWTH_CONFIG, pos, &(merged_star_soul_growth_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, MERGED_STAR_SOUL_SLOT_CONFIG& data)
{
	merged_star_soul_slot_config_array.push_back(data);

	unsigned int pos = merged_star_soul_slot_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_MERGED_STAR_SOUL_SLOT_CONFIG, pos, &(merged_star_soul_slot_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, RUNE2013_MATERIAL_CONFIG& data)
{
	rune2013_material_config_array.push_back(data);

	unsigned int pos = rune2013_material_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_RUNE2013_MATERIAL_CONFIG, pos, &(rune2013_material_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, NEW_VIP_CONFIG& data)
{
	new_vip_config_array.push_back(data);

	unsigned int pos = new_vip_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_NEW_VIP_CONFIG, pos, &(new_vip_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, RUNE2013_CONSUMPTION_CONFIG& data)
{
	rune2013_consumption_config_array.push_back(data);

	unsigned int pos = rune2013_consumption_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_RUNE2013_CONSUMPTION_CONFIG, pos, &(rune2013_consumption_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, FORBID_DIVINE_SKILL_CONFIG& data)
{
	forbid_divine_skill_config_array.push_back(data);

	unsigned int pos = forbid_divine_skill_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_FORBID_DIVINE_SKILL_CONFIG, pos, &(forbid_divine_skill_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, GIFT_LETTER_ESSENCE& data)
{
	if (data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1 << 7);

	gift_letter_essence_array.push_back(data);

	unsigned int pos = gift_letter_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_GIFT_LETTER_ESSENCE, pos, &(gift_letter_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, WARDOBE_WEAPON_CONFIG& data)
{
	wardobe_weapon_config_array.push_back(data);

	unsigned int pos = wardobe_weapon_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_WARDOBE_WEAPON_CONFIG, pos, &(wardobe_weapon_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, WARDOBE_FOOTPRINT_CONFIG& data)
{
	wardobe_footprint_config_array.push_back(data);

	unsigned int pos = wardobe_footprint_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_WARDOBE_FOOTPRINT_CONFIG, pos, &(wardobe_footprint_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, FOOTPRINT_ESSENCE& data)
{
	if (data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1 << 7);

	footprint_essence_array.push_back(data);

	unsigned int pos = footprint_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_FOOTPRINT_ESSENCE, pos, &(footprint_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, FORTUNE_GIFT_PACK_ESSENCE& data)
{
	if (data.pile_num_max == 0)		data.pile_num_max = 1;
	data.proc_type &= ~(1 << 7);

	fortune_gift_pack_essence_array.push_back(data);

	unsigned int pos = fortune_gift_pack_essence_array.size() - 1;
	add_id_index(ID_SPACE_ESSENCE, id, DT_FORTUNE_GIFT_PACK_ESSENCE, pos, &(fortune_gift_pack_essence_array[0]));
}

void elementdataman::add_structure(unsigned int id, CANTEEN_EVENT_CONFIG& data)
{
	canteen_event_config_array.push_back(data);

	unsigned int pos = canteen_event_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_CANTEEN_EVENT_CONFIG, pos, &(canteen_event_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, RETURN_AWARD_CONFIG& data)
{
	return_award_config_array.push_back(data);

	unsigned int pos = return_award_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_RETURN_AWARD_CONFIG, pos, &(return_award_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, MOBILE_PHONE_INFO_CONFIG& data)
{
	mobile_phone_info_config_array.push_back(data);

	unsigned int pos = mobile_phone_info_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_MOBILE_PHONE_INFO_CONFIG, pos, &(mobile_phone_info_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, BEGINNER_ONLINE_REWARD_CONFIG& data)
{
	beginner_online_reward_config_array.push_back(data);

	unsigned int pos = beginner_online_reward_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_BEGINNER_ONLINE_REWARD_CONFIG, pos, &(beginner_online_reward_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, MONTHLY_RETURN_AWARD_CONFIG& data)
{
	monthly_return_award_config_array.push_back(data);

	unsigned int pos = monthly_return_award_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_MONTHLY_RETURN_AWARD_CONFIG, pos, &(monthly_return_award_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, INSTANT_TIER_CONFIG& data)
{
	instant_tier_config_array.push_back(data);

	unsigned int pos = instant_tier_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_INSTANT_TIER_CONFIG, pos, &(instant_tier_config_array[0]));
}

void elementdataman::add_structure(unsigned int id, PLOT_REWARD_CONFIG& data)
{
	plot_reward_config_array.push_back(data);

	unsigned int pos = plot_reward_config_array.size() - 1;
	add_id_index(ID_SPACE_CONFIG, id, DT_PLOT_REWARD_CONFIG, pos, &(plot_reward_config_array[0]));
}

// END NEW
void elementdataman::add_id_index_essence(unsigned int id, DATA_TYPE type, unsigned int pos, void * start)
{
	LOCATION p;
	p.type = type;
	p.pos = pos;
	essence_id_index_map[id] = p;
	start_ptr_array[type] = start;
}


void elementdataman::add_id_index_addon(unsigned int id, DATA_TYPE type, unsigned int pos, void * start)
{
	LOCATION p;
	p.type = type;
	p.pos = pos;
	addon_id_index_map[id] = p;
	start_ptr_array[type] = start;
}

void elementdataman::add_id_index_talk(unsigned int id, DATA_TYPE type, unsigned int pos, void * start)
{
	LOCATION p;
	p.type = type;
	p.pos = pos;
	talk_id_index_map[id] = p;
	start_ptr_array[type] = start;
}

void elementdataman::add_id_index_recipe(unsigned int id, DATA_TYPE type, unsigned int pos, void * start)
{
	LOCATION p;
	p.type = type;
	p.pos = pos;
	recipe_id_index_map[id] = p;
	start_ptr_array[type] = start;
}

void elementdataman::add_id_index_config(unsigned int id, DATA_TYPE type, unsigned int pos, void * start)
{
	LOCATION p;
	p.type = type;
	p.pos = pos;
	config_id_index_map[id] = p;
	start_ptr_array[type] = start;
}

void elementdataman::add_id_index(ID_SPACE idspace, unsigned int id, DATA_TYPE type, unsigned int pos, void * start)
{
	switch(idspace)
	{
	case ID_SPACE_ESSENCE:
		add_id_index_essence(id, type, pos, start);	
		break;
	case ID_SPACE_ADDON:
		add_id_index_addon(id, type, pos, start);
		break;
	case ID_SPACE_TALK:
		add_id_index_talk(id, type, pos, start);
		break;
	case ID_SPACE_RECIPE:
		add_id_index_recipe(id, type, pos, start);
	case ID_SPACE_CONFIG:
		add_id_index_config(id, type, pos, start);			
	default:
		break;
	}
}

unsigned int elementdataman::get_data_id(ID_SPACE idspace, unsigned int index, DATA_TYPE& datatype)
{
	unsigned int id = 0;
	unsigned int i;
	IDToLOCATIONMap::iterator itr;
	switch(idspace)
	{
	case ID_SPACE_ESSENCE:
		itr = essence_id_index_map.begin();
 		for(i=0; i<index; i++)	++itr;
		id = itr->first;
		datatype = (itr->second).type;
		break;
		
	case ID_SPACE_ADDON:
		itr = addon_id_index_map.begin();
		for(i=0; i<index; i++)	++itr;
		id = itr->first;
		datatype = (itr->second).type;
		break;
		
	case ID_SPACE_TALK:
		itr = talk_id_index_map.begin();
		for(i=0; i<index; i++)	++itr;
		id = itr->first;
		datatype = (itr->second).type;
		break;

	case ID_SPACE_RECIPE:
		itr = recipe_id_index_map.begin();
		for(i=0; i<index; i++)	++itr;
		id = itr->first;
		datatype = (itr->second).type;
		break;

	case ID_SPACE_CONFIG:
		itr = config_id_index_map.begin();
		for(i=0; i<index; i++)	++itr;
		id = itr->first;
		datatype = (itr->second).type;
		break;

	default:
		id = 0;
		datatype = DT_INVALID;
		break;
	}
	return id;
}

unsigned int elementdataman::get_first_data_id(ID_SPACE idspace, DATA_TYPE& datatype)
{
	unsigned int id = 0;
	switch(idspace)
	{
	case ID_SPACE_ESSENCE:
		esssence_id_index_itr = essence_id_index_map.begin();
		if(esssence_id_index_itr == essence_id_index_map.end())
		{
			id = 0;
			datatype = DT_INVALID;
		}
		else
		{
			id = esssence_id_index_itr->first;
			datatype = (esssence_id_index_itr->second).type;
		}
		break;
		
	case ID_SPACE_ADDON:
		addon_id_index_itr = addon_id_index_map.begin();
		if(addon_id_index_itr == addon_id_index_map.end())
		{
			id = 0;
			datatype = DT_INVALID;
		}
		else
		{
			id = addon_id_index_itr->first;
			datatype = (addon_id_index_itr->second).type;
		}
		break;
		
	case ID_SPACE_TALK:
		talk_id_index_itr = talk_id_index_map.begin();
		if(talk_id_index_itr == talk_id_index_map.end())
		{
			id = 0;
			datatype = DT_INVALID;
		}
		else
		{
			id = talk_id_index_itr->first;
			datatype = (talk_id_index_itr->second).type;
		}
		break;

	case ID_SPACE_RECIPE:
		recipe_id_index_itr = recipe_id_index_map.begin();
		if(recipe_id_index_itr == recipe_id_index_map.end())
		{
			id = 0;
			datatype = DT_INVALID;
		}
		else
		{
			id = recipe_id_index_itr->first;
			datatype = (recipe_id_index_itr->second).type;
		}
		break;

	case ID_SPACE_CONFIG:
		config_id_index_itr = config_id_index_map.begin();
		if(config_id_index_itr == config_id_index_map.end())
		{
			id = 0;
			datatype = DT_INVALID;
		}
		else
		{
			id = config_id_index_itr->first;
			datatype = (config_id_index_itr->second).type;
		}
		break;

	default:
		id = 0;
		datatype = DT_INVALID;
		break;
	}
	return id;
}

unsigned int elementdataman::get_next_data_id(ID_SPACE idspace, DATA_TYPE& datatype)
{
	unsigned int id;
	switch(idspace)
	{
	case ID_SPACE_ESSENCE:
		++esssence_id_index_itr;
		if(esssence_id_index_itr == essence_id_index_map.end())
		{
			id = 0;
			datatype = DT_INVALID;
		}
		else
		{
			id = esssence_id_index_itr->first;
			datatype = (esssence_id_index_itr->second).type;
		}
		break;
		
	case ID_SPACE_ADDON:
		++addon_id_index_itr;
		if(addon_id_index_itr == addon_id_index_map.end())
		{
			id = 0;
			datatype = DT_INVALID;
		}
		else
		{
			id = addon_id_index_itr->first;
			datatype = (addon_id_index_itr->second).type;
		}
		break;
		
	case ID_SPACE_TALK:
		++talk_id_index_itr;
		if(talk_id_index_itr == talk_id_index_map.end())
		{
			id = 0;
			datatype = DT_INVALID;
		}
		else
		{
			id = talk_id_index_itr->first;
			datatype = (talk_id_index_itr->second).type;
		}
		break;

	case ID_SPACE_RECIPE:
		++recipe_id_index_itr;
		if(recipe_id_index_itr == recipe_id_index_map.end())
		{
			id = 0;
			datatype = DT_INVALID;
		}
		else
		{
			id = recipe_id_index_itr->first;
			datatype = (recipe_id_index_itr->second).type;
		}
		break;

	case ID_SPACE_CONFIG:
		++config_id_index_itr;
		if(config_id_index_itr == config_id_index_map.end())
		{
			id = 0;
			datatype = DT_INVALID;
		}
		else
		{
			id = config_id_index_itr->first;
			datatype = (config_id_index_itr->second).type;
		}
		break;

	default:
		datatype = DT_INVALID;
		break;
	}
	return id;	
}

unsigned int elementdataman::get_data_num(ID_SPACE idspace)
{
	switch(idspace)
	{
	case ID_SPACE_ESSENCE:
		return essence_id_index_map.size();

	case ID_SPACE_ADDON:
		return addon_id_index_map.size();

	case ID_SPACE_TALK:
		return talk_id_index_map.size();

	case ID_SPACE_RECIPE:
		return recipe_id_index_map.size();

	case ID_SPACE_CONFIG:
		return config_id_index_map.size();

	default:
		return 0;
	}
	return 0;
}

DATA_TYPE elementdataman::get_data_type(unsigned int id, ID_SPACE idspace)
{
	IDToLOCATIONMap::iterator itr;
	switch(idspace)
	{
	case ID_SPACE_ESSENCE:
		itr = essence_id_index_map.find(id);
		if(itr != essence_id_index_map.end())
		{
			return (itr->second).type;
		}
		else
			return DT_INVALID;			
		break;

	case ID_SPACE_ADDON:
		itr = addon_id_index_map.find(id);
		if(itr != addon_id_index_map.end())
		{
			return (itr->second).type;
		}
		else
			return DT_INVALID;
		break;		

	case ID_SPACE_TALK:
		itr = talk_id_index_map.find(id);
		if(itr != talk_id_index_map.end())
		{
			return (itr->second).type;
		}
		else
			return DT_INVALID;
		break;

	case ID_SPACE_RECIPE:
		itr = recipe_id_index_map.find(id);
		if(itr != recipe_id_index_map.end())
		{
			return (itr->second).type;
		}
		else
			return DT_INVALID;
		break;

	case ID_SPACE_CONFIG:
		itr = config_id_index_map.find(id);
		if(itr != config_id_index_map.end())
		{
			return (itr->second).type;
		}
		else
			return DT_INVALID;
		break;

	default:
		return DT_INVALID;
		break;
	}
}

const void * elementdataman::get_data_ptr(unsigned int id, ID_SPACE idspace, DATA_TYPE& datatype)
{
	datatype = DT_INVALID;
	int pos;
	IDToLOCATIONMap::iterator itr;
	switch(idspace)
	{
	case ID_SPACE_ESSENCE:
		itr = essence_id_index_map.find(id);
		if(itr != essence_id_index_map.end())
		{
			datatype = (itr->second).type;
			pos = (itr->second).pos;
		}
		else
			datatype = DT_INVALID;			
		break;

	case ID_SPACE_ADDON:
		itr = addon_id_index_map.find(id);
		if(itr != addon_id_index_map.end())
		{
			datatype = (itr->second).type;
			pos = (itr->second).pos;
		}
		else
			datatype = DT_INVALID;
		break;
		
	case ID_SPACE_TALK:
		itr = talk_id_index_map.find(id);
		if(itr != talk_id_index_map.end())
		{
			datatype = (itr->second).type;
			pos = (itr->second).pos;
		}
		else
			datatype = DT_INVALID;
		break;

	case ID_SPACE_RECIPE:
		itr = recipe_id_index_map.find(id);
		if(itr != recipe_id_index_map.end())
		{
			datatype = (itr->second).type;
			pos = (itr->second).pos;
		}
		else
			datatype = DT_INVALID;
		break;

	case ID_SPACE_CONFIG:
		itr = config_id_index_map.find(id);
		if(itr != config_id_index_map.end())
		{
			datatype = (itr->second).type;
			pos = (itr->second).pos;
		}
		else
			datatype = DT_INVALID;
		break;

	default:
		datatype = DT_INVALID;
		break;
	}

	if(datatype != DT_INVALID)
	{
		char * start = (char *) start_ptr_array[datatype];
		if(datatype == DT_TALK_PROC)
			return (const void*) *(int *)(start+pos*type_size_array[datatype]);
		else
			return start+pos*type_size_array[datatype];
	}
	else
		return NULL;
}

void elementdataman::setup_hash_map()
{
#define ADD_HASH_MAP(ids, typ, arr) \
p.type = typ;\
for(i=0; i<arr.size(); i++)\
{\
	start_ptr_array[typ] = &(arr[0]);\
	p.pos = i;\
	ids##_id_index_map[arr[i].id] = p;\
}

	unsigned int i;
	LOCATION p;

	ADD_HASH_MAP(addon, DT_EQUIPMENT_ADDON, equipment_addon_array)
	ADD_HASH_MAP(essence, DT_EQUIPMENT_MAJOR_TYPE, equipment_major_type_array)
	ADD_HASH_MAP(essence, DT_EQUIPMENT_SUB_TYPE, equipment_sub_type_array)
	ADD_HASH_MAP(essence, DT_EQUIPMENT_ESSENCE, equipment_essence_array)
	ADD_HASH_MAP(essence, DT_REFINE_TICKET_ESSENCE, refine_ticket_essence_array)
	ADD_HASH_MAP(essence, DT_MEDICINE_MAJOR_TYPE, medicine_major_type_array)
	ADD_HASH_MAP(essence, DT_MEDICINE_SUB_TYPE, medicine_sub_type_array)
	ADD_HASH_MAP(essence, DT_MEDICINE_ESSENCE, medicine_essence_array)
	ADD_HASH_MAP(essence, DT_MATERIAL_MAJOR_TYPE, material_major_type_array)
	ADD_HASH_MAP(essence, DT_MATERIAL_SUB_TYPE, material_sub_type_array)
	ADD_HASH_MAP(essence, DT_MATERIAL_ESSENCE, material_essence_array)
	ADD_HASH_MAP(essence, DT_SKILLTOME_SUB_TYPE, skilltome_sub_type_array)
	ADD_HASH_MAP(essence, DT_SKILLTOME_ESSENCE, skilltome_essence_array)
	ADD_HASH_MAP(essence, DT_TRANSMITROLL_ESSENCE, transmitroll_essence_array)
	ADD_HASH_MAP(essence, DT_LUCKYROLL_ESSENCE, luckyroll_essence_array)
	ADD_HASH_MAP(essence, DT_TOWNSCROLL_ESSENCE, townscroll_essence_array)
	ADD_HASH_MAP(essence, DT_REVIVESCROLL_ESSENCE, revivescroll_essence_array)
	ADD_HASH_MAP(essence, DT_TASKMATTER_ESSENCE, taskmatter_essence_array)
	ADD_HASH_MAP(essence, DT_DROPTABLE_TYPE, droptable_type_array)
	ADD_HASH_MAP(essence, DT_DROPTABLE_ESSENCE, droptable_essence_array)
	ADD_HASH_MAP(essence, DT_MONSTER_TYPE, monster_type_array)
	ADD_HASH_MAP(essence, DT_MONSTER_ESSENCE, monster_essence_array)
	ADD_HASH_MAP(essence, DT_NPC_TALK_SERVICE, npc_talk_service_array)
	ADD_HASH_MAP(essence, DT_NPC_SELL_SERVICE, npc_sell_service_array)
	ADD_HASH_MAP(essence, DT_NPC_BUY_SERVICE, npc_buy_service_array)
	ADD_HASH_MAP(essence, DT_NPC_TASK_IN_SERVICE, npc_task_in_service_array)
	ADD_HASH_MAP(essence, DT_NPC_TASK_OUT_SERVICE, npc_task_out_service_array)
	ADD_HASH_MAP(essence, DT_NPC_TASK_MATTER_SERVICE, npc_task_matter_service_array)
	ADD_HASH_MAP(essence, DT_NPC_HEAL_SERVICE, npc_heal_service_array)
	ADD_HASH_MAP(essence, DT_NPC_TRANSMIT_SERVICE, npc_transmit_service_array)
	ADD_HASH_MAP(essence, DT_NPC_PROXY_SERVICE, npc_proxy_service_array)
	ADD_HASH_MAP(essence, DT_NPC_STORAGE_SERVICE, npc_storage_service_array)
	ADD_HASH_MAP(essence, DT_NPC_TYPE, npc_type_array)
	ADD_HASH_MAP(essence, DT_NPC_ESSENCE, npc_essence_array)
	ADD_HASH_MAP(recipe, DT_RECIPE_MAJOR_TYPE, recipe_major_type_array)
	ADD_HASH_MAP(recipe, DT_RECIPE_SUB_TYPE, recipe_sub_type_array)
	ADD_HASH_MAP(recipe, DT_RECIPE_ESSENCE, recipe_essence_array)
	ADD_HASH_MAP(config, DT_ENEMY_FACTION_CONFIG, enemy_faction_config_array)
	ADD_HASH_MAP(config, DT_CHARACTER_CLASS_CONFIG, character_class_config_array)
	ADD_HASH_MAP(config, DT_PARAM_ADJUST_CONFIG, param_adjust_config_array)
	ADD_HASH_MAP(config, DT_PIE_LOVE_CONFIG, pie_love_config_array)
	ADD_HASH_MAP(essence, DT_TASKDICE_ESSENCE, taskdice_essence_array)
	ADD_HASH_MAP(essence, DT_TASKNORMALMATTER_ESSENCE, tasknormalmatter_essence_array);
	ADD_HASH_MAP(config, DT_PLAYER_LEVELEXP_CONFIG, player_levelexp_config_array)
	ADD_HASH_MAP(essence, DT_MINE_TYPE, mine_type_array)
	ADD_HASH_MAP(essence, DT_MINE_ESSENCE, mine_essence_array)
	ADD_HASH_MAP(essence, DT_GM_GENERATOR_TYPE, gm_generator_type_array);
	ADD_HASH_MAP(essence, DT_GM_GENERATOR_ESSENCE, gm_generator_essence_array);
	ADD_HASH_MAP(essence, DT_FIREWORKS_ESSENCE, fireworks_essence_array);
	ADD_HASH_MAP(essence, DT_NPC_WAR_TOWERBUILD_SERVICE, npc_war_towerbuild_service_array);
	ADD_HASH_MAP(config, DT_PLAYER_SECONDLEVEL_CONFIG, player_secondlevel_config_array);
	ADD_HASH_MAP(essence, DT_NPC_RESETPROP_SERVICE, npc_resetprop_service_array);
	ADD_HASH_MAP(essence, DT_ESTONE_ESSENCE, estone_essence_array);
	ADD_HASH_MAP(essence, DT_PSTONE_ESSENCE, pstone_essence_array);
	ADD_HASH_MAP(essence, DT_SSTONE_ESSENCE, sstone_essence_array);
	ADD_HASH_MAP(essence, DT_RECIPEROLL_MAJOR_TYPE, reciperoll_major_type_array);
	ADD_HASH_MAP(essence, DT_RECIPEROLL_SUB_TYPE, reciperoll_sub_type_array);
	ADD_HASH_MAP(essence, DT_RECIPEROLL_ESSENCE, reciperoll_essence_array);
	ADD_HASH_MAP(essence, DT_SUITE_ESSENCE, suite_essence_array);
	ADD_HASH_MAP(essence, DT_DOUBLE_EXP_ESSENCE, double_exp_essence_array);
	ADD_HASH_MAP(essence, DT_DESTROYING_ESSENCE, destroying_essence_array);
	ADD_HASH_MAP(essence, DT_NPC_EQUIPBIND_SERVICE, npc_equipbind_service_array);
	ADD_HASH_MAP(essence, DT_NPC_EQUIPDESTROY_SERVICE, npc_equipdestroy_service_array);
	ADD_HASH_MAP(essence, DT_NPC_EQUIPUNDESTROY_SERVICE, npc_equipundestroy_service_array);
	ADD_HASH_MAP(essence, DT_SKILLMATTER_ESSENCE, skillmatter_essence_array);
	ADD_HASH_MAP(essence, DT_VEHICLE_ESSENCE, vehicle_essence_array);
	ADD_HASH_MAP(essence, DT_COUPLE_JUMPTO_ESSENCE, couple_jumpto_essence_array);
	ADD_HASH_MAP(essence, DT_LOTTERY_ESSENCE, lottery_essence_array);
	ADD_HASH_MAP(essence, DT_CAMRECORDER_ESSENCE, camrecorder_essence_array);
	ADD_HASH_MAP(config, DT_TITLE_PROP_CONFIG, title_prop_config_array);
	ADD_HASH_MAP(config, DT_SPECIAL_ID_CONFIG, special_id_config_array);
	ADD_HASH_MAP(essence, DT_TEXT_FIREWORKS_ESSENCE, text_fireworks_essence_array);
	ADD_HASH_MAP(essence, DT_TALISMAN_MAINPART_ESSENCE, talisman_mainpart_essence_array);
	ADD_HASH_MAP(essence, DT_TALISMAN_EXPFOOD_ESSENCE, talisman_expfood_essence_array);
	ADD_HASH_MAP(essence, DT_TALISMAN_MERGEKATALYST_ESSENCE, talisman_mergekatalyst_essence_array);
	ADD_HASH_MAP(essence, DT_TALISMAN_ENERGYFOOD_ESSENCE, talisman_energyfood_essence_array);
	ADD_HASH_MAP(essence, DT_SPEAKER_ESSENCE, speaker_essence_array);
	ADD_HASH_MAP(config, DT_PLAYER_TALENT_CONFIG, player_talent_config_array); 
	ADD_HASH_MAP(essence, DT_POTENTIAL_TOME_ESSENCE, potential_tome_essence_array);
	ADD_HASH_MAP(essence, DT_WAR_ROLE_CONFIG, war_role_config_array);
	ADD_HASH_MAP(essence, DT_NPC_WAR_BUY_ARCHER_SERVICE, npc_war_buy_archer_service_array);
	ADD_HASH_MAP(essence, DT_SIEGE_ARTILLERY_SCROLL_ESSENCE, siege_artillery_scroll_essence_array);
	ADD_HASH_MAP(essence, DT_PET_BEDGE_ESSENCE, pet_bedge_essence_array);
	ADD_HASH_MAP(essence, DT_PET_FOOD_ESSENCE, pet_food_essence_array);
	ADD_HASH_MAP(essence, DT_PET_SKILL_ESSENCE, pet_skill_essence_array);
	ADD_HASH_MAP(essence, DT_PET_ARMOR_ESSENCE, pet_armor_essence_array);
	ADD_HASH_MAP(essence, DT_PET_AUTO_FOOD_ESSENCE, pet_auto_food_essence_array);
	ADD_HASH_MAP(essence, DT_PET_REFINE_ESSENCE, pet_refine_essence_array);
	ADD_HASH_MAP(essence, DT_PET_ASSIST_REFINE_ESSENCE, pet_assist_refine_essence_array);
	ADD_HASH_MAP(config, DT_RENASCENCE_SKILL_CONFIG, renasence_skill_config_array);
	ADD_HASH_MAP(config, DT_RENASCENCE_PROP_CONFIG, renasence_prop_config_array);
	ADD_HASH_MAP(essence, DT_AIRCRAFT_ESSENCE, aircraft_essence_array);
	ADD_HASH_MAP(essence, DT_FLY_ENERGYFOOD_ESSENCE, fly_energyfood_essence_array);
	ADD_HASH_MAP(essence, DT_ITEM_TRADE_CONFIG, item_trade_config_array);
	ADD_HASH_MAP(essence, DT_BOOK_ESSENCE, book_essence_array);
	ADD_HASH_MAP(config, DT_PLAYER_SKILL_POINT_CONFIG, player_skill_point_config_array); 
	ADD_HASH_MAP(essence, DT_OFFLINE_TRUSTEE_ESSENCE, offline_trustee_essence_array);
	ADD_HASH_MAP(essence, DT_EQUIP_SOUL_ESSENCE, equip_soul_essence_array);
	ADD_HASH_MAP(essence, DT_EQUIP_SOUL_MELD_SERVICE, equip_soul_meld_service_array);
	ADD_HASH_MAP(essence, DT_SPECIAL_NAME_ITEM_ESSENCE, special_name_item_essence_array);
	ADD_HASH_MAP(config, DT_RECYCLE_ITEM_CONFIG, recycle_item_config_array);
	ADD_HASH_MAP(config, DT_SCORE_TO_RANK_CONFIG, score_to_rank_config_array);
	ADD_HASH_MAP(config, DT_BATTLE_DROP_CONFIG, battle_drop_config_array);
	ADD_HASH_MAP(config, DT_BATTLE_DEPRIVE_CONFIG, battle_deprive_config_array);
	ADD_HASH_MAP(config, DT_BATTLE_SCORE_CONFIG, battle_score_config_array);
	ADD_HASH_MAP(essence, DT_GIFT_BAG_ESSENCE, gift_bag_essence_array);
	ADD_HASH_MAP(essence, DT_VIP_CARD_ESSENCE, vip_card_essence_array);
	ADD_HASH_MAP(config, DT_INSTANCING_BATTLE_CONFIG, instancing_battle_config_array);
	ADD_HASH_MAP(essence, DT_CHANGE_SHAPE_CARD_ESSENCE, change_shape_card_essence_array);
	ADD_HASH_MAP(essence, DT_CHANGE_SHAPE_STONE_ESSENCE, change_shape_stone_essence_array);
	ADD_HASH_MAP(config, DT_CHANGE_SHAPE_PROP_CONFIG, change_shape_prop_config_array);
	ADD_HASH_MAP(config, DT_ORIGINAL_SHAPE_CONFIG, original_shape_config_array);
	ADD_HASH_MAP(config, DT_LIFE_SKILL_CONFIG, life_skill_config_array);
	ADD_HASH_MAP(config, DT_ARENA_DROPTABLE_CONFIG, arena_droptable_config_array);
	ADD_HASH_MAP(essence, DT_MERCENARY_CREDENTIAL_ESSENCE, mercenary_credential_essence_array);
	ADD_HASH_MAP(essence, DT_TELEPORTATION_ESSENCE, teleportation_essence_array);
	ADD_HASH_MAP(essence, DT_TELEPORTATION_STONE_ESSENCE, teleportation_stone_essence_array);
	ADD_HASH_MAP(config, DT_COMBINE_SKILL_EDIT_CONFIG, combine_skill_edit_config_array);
	ADD_HASH_MAP(config, DT_UPGRADE_EQUIP_CONFIG, upgrade_equip_config_array);
	ADD_HASH_MAP(config, DT_UPGRADE_EQUIP_CONFIG_1, upgrade_equip_config_1_array);
	ADD_HASH_MAP(essence, DT_UPGRADE_EQUIP_STONE_ESSENCE, upgrade_equip_stone_essence_array);
	ADD_HASH_MAP(essence, DT_NPC_CONSIGN_SERVICE, npc_consign_service_array);
	ADD_HASH_MAP(config, DT_DEITY_CHARACTER_CONFIG, deity_character_config_array);
	ADD_HASH_MAP(essence, DT_LOTTERY2_ESSENCE, lottery2_essence_array);	
	ADD_HASH_MAP(config, DT_GEM_CONFIG, gem_config_array);//Added 2011-03-14.
	ADD_HASH_MAP(essence, DT_GEM_ESSENCE, gem_essence_array);
	ADD_HASH_MAP(essence, DT_GEM_SEAL_ESSENCE, gem_seal_essence_array);
	ADD_HASH_MAP(essence, DT_GEM_DUST_ESSENCE, gem_dust_essence_array);
	ADD_HASH_MAP(config, DT_GEM_EXTRACT_CONFIG, gem_extract_config_array);
	ADD_HASH_MAP(essence, DT_GENERAL_ARTICLE_ESSENCE, general_article_essence_array);
	ADD_HASH_MAP(essence, DT_LOTTERY3_ESSENCE, lottery3_essence_array); //Added 2011-07-01.
	ADD_HASH_MAP(config, DT_TRANSCRIPTION_CONFIG, transcription_config_array); //Added 2011-07-13.
	ADD_HASH_MAP(config, DT_ONLINE_GIFT_BAG_CONFIG, online_gift_bag_config_array); //Added 2011-12-06.
	ADD_HASH_MAP(essence, DT_SMALL_ONLINE_GIFT_BAG_ESSENCE, small_online_gift_bag_essence_array); //Added 2011-12-06.
	ADD_HASH_MAP(config, DT_SCROLL_REGION_CONFIG, scroll_region_config_array); //Added 2012-03-20.
	ADD_HASH_MAP(essence, DT_SCROLL_UNLOCK_ESSENCE, scroll_unlock_essence_array); //Added 2012-03-20.
	ADD_HASH_MAP(essence, DT_SCROLL_DIG_COUNT_INC_ESSENCE, scroll_dig_count_inc_essence_array); //Added 2012-03-22.
	ADD_HASH_MAP(config, DT_TOWER_TRANSCRIPTION_CONFIG, tower_transcription_config_array);		//Added 2012-03-31.
	ADD_HASH_MAP(config, DT_TOWER_TRANSCRIPTION_PROPERTY_CONFIG, tower_transcription_property_config_array);//Added 2012-03-31.
	ADD_HASH_MAP(config, DT_RUNE_DATA_CONFIG, rune_data_config_array);//Added 2012.04.17.
	ADD_HASH_MAP(config, DT_RUNE_COMB_PROPERTY, rune_comb_property_array);//Added 2012.04.17.
	ADD_HASH_MAP(essence, DT_RUNE_EQUIP_ESSENCE, rune_equip_array);//Added 2012.04.17.
	ADD_HASH_MAP(config, DT_LITTLE_PET_UPGRADE_CONFIG, little_pet_upgrade_config_array);	//Added 2012-04-19.
	ADD_HASH_MAP(essence, DT_RUNE_SLOT_ESSENCE, rune_slot_array);
	ADD_HASH_MAP(config, DT_DROP_INTERNAL_CONFIG, drop_interval_config_array);
	ADD_HASH_MAP(config, DT_PK2012_GUESS_CONFIG, pk2012_guess_config_array);
	ADD_HASH_MAP(config, DT_COLLISION_RAID_TRANSFIGURE_CONFIG, collision_raid_transfigure_config_array);	//Added 2012-08-02.
	ADD_HASH_MAP(essence, DT_BOOTH_FIGURE_ITEM_ESSENCE, booth_figure_item_array);
	ADD_HASH_MAP(essence, DT_FLAG_BUFF_ITEM_ESSENCE, flag_buff_item_array);
	ADD_HASH_MAP(essence, DT_NPC_REPUTATION_SHOP_SERVICE, npc_reputation_shop_service_array);
	ADD_HASH_MAP(essence, DT_NPC_UI_TRANSFER_SERVICE, npc_ui_transfer_service_array);
	ADD_HASH_MAP(config, DT_UI_TRANSFER_CONFIG, ui_transfer_config_array);
	ADD_HASH_MAP(essence, DT_XINGZUO_ITEM_ESSENCE, xingzuo_item_essence_array);
	ADD_HASH_MAP(essence, DT_XINGZUO_ENERGY_ITEM_ESSENCE, xingzuo_energy_item_essence_array);
	ADD_HASH_MAP(config, DT_COLLISION_RAID_AWARD_CONFIG, collision_raid_award_config_array);
	ADD_HASH_MAP(essence, DT_CASH_MEDIC_MERGE_ITEM_ESSENCE, cash_medic_merge_item_essence_array);
	ADD_HASH_MAP(config, DT_BUFF_AREA_CONFIG, buff_area_config_array);
	ADD_HASH_MAP(config, DT_LIVENESS_CONFIG, liveness_config_array);
	ADD_HASH_MAP(config, DT_CHALLENGE_2012_CONFIG, challenge_2012_config_array);
	ADD_HASH_MAP(essence, DT_SALE_PROMOTION_ITEM_ESSENCE, sale_promotion_item_essence_array);
	ADD_HASH_MAP(essence, DT_GIFT_BAG_LOTTERY_DELIVER_ESSENCE, gift_bag_lottery_deliver_essence_array);
	ADD_HASH_MAP(essence, DT_LOTTERY_TANGYUAN_ITEM_ESSENCE, lottery_tangyuan_item_essence_array);
	ADD_HASH_MAP(config, DT_TASK_SPECIAL_AWARD_CONFIG, task_special_award_config_array);
	ADD_HASH_MAP(essence, DT_GIFT_PACK_ITEM_ESSENCE, gift_pack_item_essence_array);
	ADD_HASH_MAP(config, DT_PROP_ADD_CONFIG, prop_add_config_array);
	ADD_HASH_MAP(essence, DT_PROP_ADD_MATERIAL_ITEM_ESSENCE, prop_add_material_item_essence_array);
	ADD_HASH_MAP(essence, DT_PROP_ADD_ITEM_ESSENCE, prop_add_item_essence_array);
	ADD_HASH_MAP(config, DT_KING_WAR_CONFIG, king_war_config_array);
	ADD_HASH_MAP(config, DT_JINFASHEN_TO_MONEY_CONFIG, jinfashen_to_money_config_array);
	ADD_HASH_MAP(config, DT_BATTLE_201304_CONFIG, battle_201304_config_array);
	ADD_HASH_MAP(essence, DT_RUNE2013_ITEM_ESSENCE, rune2013_item_essence_array);
	ADD_HASH_MAP(config, DT_RUNE2013_CONFIG, rune2013_config_array);
	ADD_HASH_MAP(config, DT_BASHE_AWARD_CONFIG, bashe_award_config_array);
	ADD_HASH_MAP(config, DT_ARENA_SEASON_TIME_CONFIG, arena_season_time_config_array);
	ADD_HASH_MAP(config, DT_PERSONAL_LEVEL_AWARD_CONFIG, personal_level_award_config_array);
	ADD_HASH_MAP(config, DT_TEAM_SEASON_AWARD_CONFIG, team_season_award_config_array);
	ADD_HASH_MAP(config, DT_WEEK_CURRNCY_AWARD_CONFIG, week_currency_award_config_array);
	ADD_HASH_MAP(essence, DT_COLORANT_ITEM_ESSENCE, colorant_item_essence_array);
	ADD_HASH_MAP(essence, DT_COLORANT2_ITEM_ESSENCE, colorant2_item_essence_array);
	ADD_HASH_MAP(essence, DT_INTERACTION_OBJECT_ESSENCE,interaction_object_essence_array);
	ADD_HASH_MAP(config,  DT_INTERACTION_OBJECT_HOOK_CONFIG, interaction_object_hook_config_array);
	ADD_HASH_MAP(config,  DT_COLORANT_CONFIG, colorant_config_array);
	ADD_HASH_MAP(config,  DT_FACTION_TRANSFER_CONFIG, faction_transfer_config_array);
	ADD_HASH_MAP(config,  DT_BUILDING_REGION_CONFIG, building_region_config_array);
	ADD_HASH_MAP(essence, DT_BUILDING_ESSENCE, building_essence_array);
	ADD_HASH_MAP(essence, DT_REGION_BLOCK_ESSENCE, region_block_essence_array);
	ADD_HASH_MAP(config, DT_PHASE_CONFIG, phase_config_array);
	ADD_HASH_MAP(config, DT_CROSS6V6_ITEM_EXCHANGE_CONFIG, cross6v6_item_exchange_config_array);
	ADD_HASH_MAP(config, DT_TRANSCRIPT_STRATEGY_CONFIG, transcript_strategy_config_array);
	ADD_HASH_MAP(config, DT_FACTION_SHOP_CONFIG, faction_shop_config_array);
	ADD_HASH_MAP(config, DT_FACTION_AUCTION_CONFIG, faction_auction_config_array);
	ADD_HASH_MAP(config, DT_FACTION_MONEY_SHOP_CONFIG, faction_money_shop_config_array);
	ADD_HASH_MAP(essence, DT_COMPOUND_MINE_ESSENCE, compound_mine_essence_array);
	ADD_HASH_MAP(config, DT_COLORANT_DEFAULT_CONFIG, colorant_default_config_array);
	// v158 new data tables (loaded after talk_proc)
	ADD_HASH_MAP(config, DT_SIGN_IN_CONFIG, sign_in_config_array);
	ADD_HASH_MAP(essence, DT_BABY_ESSENCE, baby_essence_array);
	ADD_HASH_MAP(config, DT_CLASS_BABY_RELATION_CONFIG, class_baby_relation_config_array);
	ADD_HASH_MAP(config, DT_BABY_CONFIG, baby_config_array);
	ADD_HASH_MAP(config, DT_BABY_PROPERTY_CONFIG, baby_property_config_array);
	ADD_HASH_MAP(config, DT_BABY_UPGRADE_CONFIG, baby_upgrade_config_array);
	ADD_HASH_MAP(essence, DT_BABY_FASHION_ESSENCE, baby_fashion_essence_array);
	ADD_HASH_MAP(essence, DT_BABY_TOY_ESSENCE, baby_toy_essence_array);
	ADD_HASH_MAP(essence, DT_BABY_BOOK_ESSENCE, baby_book_essence_array);
	ADD_HASH_MAP(essence, DT_MATRIX_CARD_ESSENCE, matrix_card_essence_array);
	ADD_HASH_MAP(essence, DT_MATRIX_EQUIP_ESSENCE, matrix_equip_essence_array);
	ADD_HASH_MAP(config, DT_MATRIX_EXP_CONFIG, matrix_exp_config_array);
	ADD_HASH_MAP(config, DT_MATRIX_COLLECT_CONFIG, matrix_collect_config_array);
	ADD_HASH_MAP(config, DT_MATRIX_CARD_UPGREAD_CONFIG, matrix_card_upgread_config_array);
	ADD_HASH_MAP(config, DT_MATRIX_COMBINE_CONFIG, matrix_combine_config_array);
	ADD_HASH_MAP(essence, DT_BABY_FOOD_ESSENCE, baby_food_essence_array);
	ADD_HASH_MAP(essence, DT_BABY_TITLE_ESSENCE, baby_title_essence_array);
	ADD_HASH_MAP(config, DT_BABY_TITLE_LIST, baby_title_list_array);
	ADD_HASH_MAP(essence, DT_VEHICLE_ENHANCE_ESSENCE, vehicle_enhance_essence_array);
	ADD_HASH_MAP(essence, DT_TALENT_SCROLL_ESSENCE, talent_scroll_essence_array);
	ADD_HASH_MAP(config, DT_TALENT_SCROLL_CONFIG, talent_scroll_config_array);
	ADD_HASH_MAP(config, DT_TALENT_SCROLL_SKILL_CONFIG, talent_scroll_skill_config_array);
	ADD_HASH_MAP(config, DT_SKILL_REPLACE_CONFIG, skill_replace_config_array);
	ADD_HASH_MAP(config, DT_HILL_TRANSCRIPTION_CONFIG, hill_transcription_config_array);
	ADD_HASH_MAP(config, DT_CONSUME_AWARD_CONFIG, consume_award_config_array);
	ADD_HASH_MAP(config, DT_MULTI_TOWER_PROPERTY_CONFIG, multi_tower_property_config_array);
	ADD_HASH_MAP(config, DT_RAID18V18_AWARD_CONFIG, raid18v18_award_config_array);
	ADD_HASH_MAP(essence, DT_EGGS_GIFT_BAG_ESSENCE, eggs_gift_bag_essence_array);
	ADD_HASH_MAP(essence, DT_ITEM_TRADE2_CONFIG, item_trade2_config_array);
	ADD_HASH_MAP(essence, DT_GUESS_ESSENCE, guess_essence_array);
	ADD_HASH_MAP(config, DT_ELITE_GAME_RESULT_CONFIG, elite_game_result_config_array);
	ADD_HASH_MAP(config, DT_ELITE_GAME_AWARD_CONFIG, elite_game_award_config_array);
	ADD_HASH_MAP(essence, DT_TITLE_ITEM_ESSENCE, title_item_essence_array);
	ADD_HASH_MAP(config, DT_SOUL_ENHANCE_CONFIG, soul_enhance_config_array);
	ADD_HASH_MAP(config, DT_SOUL_EXCHANGE_CONFIG, soul_exchange_config_array);
	ADD_HASH_MAP(config, DT_SOUL_DROP_CONFIG, soul_drop_config_array);
	ADD_HASH_MAP(config, DT_DAILY_SIGN_CONFIG, daily_sign_config_array);
	ADD_HASH_MAP(essence, DT_RED_PACKETS_ESSENCE, red_packets_essence_array);
	ADD_HASH_MAP(essence, DT_FORBID_COMMON_SKILL_CONFIG, forbid_common_skill_config_array);
	ADD_HASH_MAP(essence, DT_FORBID_GODEVIL_SKILL_CONFIG, forbid_godevil_skill_config_array);
	ADD_HASH_MAP(essence, DT_FORCE_CD_SKILL_CONFIG, force_cd_skill_config_array);
	ADD_HASH_MAP(essence, DT_TALENT_SCROLL_REFINE_ESSENCE, talent_scroll_refine_essence_array);
	ADD_HASH_MAP(config, DT_FACTION_BASE_ACTIVITY_CONFIG, faction_base_activity_config_array);
	ADD_HASH_MAP(essence, DT_AMULET_ESSENCE, amulet_essence_array);
	ADD_HASH_MAP(config, DT_AMULET_CONFIG, amulet_config_array);
	ADD_HASH_MAP(essence, DT_AMULET_STONE_ESSENCE, amulet_stone_essence_array);
	ADD_HASH_MAP(config, DT_SPECIAL_VIP_CONFIG, special_vip_config_array);
	ADD_HASH_MAP(config, DT_CHARGE_AWARD_CONFIG, charge_award_config_array);
	ADD_HASH_MAP(essence, DT_NEW_TOWNSCROLL_ESSENCE, new_townscroll_essence_array);
	ADD_HASH_MAP(config, DT_EQUIPMENT_RANDOMEXT_CONFIG, equipment_randomext_config_array);
	ADD_HASH_MAP(config, DT_INDISCRIMINATION_BATTLE_CONFIG, indiscrimination_battle_config_array);
	ADD_HASH_MAP(config, DT_GROUPRAID_EVALAWARD_CONFIG, groupraid_evalaward_config_array);
	ADD_HASH_MAP(config, DT_EQUIPMENT_IDENTIFY_CONFIG, equipment_identify_config_array);
	ADD_HASH_MAP(essence, DT_HEADFRAME_ESSENCE, headframe_essence_array);
	ADD_HASH_MAP(config, DT_CONVERT_EQUIP_CONFIG, convert_equip_config_array);
	ADD_HASH_MAP(config, DT_AUSPICIOUS_HALL_CONFIG, auspicious_hall_config_array);
	ADD_HASH_MAP(config, DT_DIVINE_ACCESSORY_IMBUE_CONFIG, divine_accessory_imbue_config_array);
	ADD_HASH_MAP(config, DT_LEVEL_UP_REWARD_CONFIG, level_up_reward_config_array);
	ADD_HASH_MAP(essence, DT_NEW_BOX_ESSENCE, new_box_essence_array);
	ADD_HASH_MAP(essence, DT_NEW_FASHION_PACK_ESSENCE, new_fashion_pack_essence_array);
	ADD_HASH_MAP(config, DT_NEW_CRAFTING_CONFIG, new_crafting_config_array);
	ADD_HASH_MAP(config, DT_FAIRYHOUSE_GENERAL_CONFIG, fairyhouse_general_config_array);
	ADD_HASH_MAP(config, DT_FAIRYHOUSE_PROBABILITY_CONFIG, fairyhouse_probability_config_array);
	ADD_HASH_MAP(config, DT_FAIRYHOUSE_BABY_TRAIT_CONFIG, fairyhouse_baby_trait_config_array);
	ADD_HASH_MAP(config, DT_FAIRYHOUSE_PET_TRAIT_CONFIG, fairyhouse_pet_trait_config_array);
	ADD_HASH_MAP(config, DT_FAIRYHOUSE_CHALLENGE_CONFIG, fairyhouse_challenge_config_array);
	ADD_HASH_MAP(essence, DT_FAIRYHOUSE_BABY_GEAR_ESSENCE, fairyhouse_baby_gear_essence_array);
	ADD_HASH_MAP(essence, DT_FAIRYHOUSE_PET_GEAR_ESSENCE, fairyhouse_pet_gear_essence_array);
	ADD_HASH_MAP(essence, DT_FAIRYHOUSE_TRAIT_LEARN_ESSENCE, fairyhouse_trait_learn_essence_array);
	ADD_HASH_MAP(essence, DT_PREPAID_BOOK_ESSENCE, prepaid_book_essence_array);
	ADD_HASH_MAP(config, DT_TOKEN_BAG_CONFIG, token_bag_config_array);
	ADD_HASH_MAP(essence, DT_TIMELIMIT_BEAD_ESSENCE, timelimit_bead_essence_array);
	ADD_HASH_MAP(essence, DT_EXTRA_TIMELIMIT_BEAD_ESSENCE, extra_timelimit_bead_essence_array);
	ADD_HASH_MAP(config, DT_CONSTELLATIONEXP_CONFIG, constellationexp_config_array);
	ADD_HASH_MAP(config, DT_CONSTELLATION_SKILL_CONFIG, constellation_skill_config_array);
	ADD_HASH_MAP(config, DT_CONSTELLATION_CONFIG, constellation_config_array);
	ADD_HASH_MAP(essence, DT_CONSTELLATION_STARS_ESSENCE, constellation_stars_essence_array);
	ADD_HASH_MAP(config, DT_CONSTELLATION_SKILL_RELATION_CONFIG, constellation_skill_relation_config_array);
	ADD_HASH_MAP(config, DT_TALENT_SCROLL_TRIGGER_CONFIG, talent_scroll_trigger_config_array);
	ADD_HASH_MAP(config, DT_EVENT_REWARD_SET_CONFIG, event_reward_set_config_array);
	ADD_HASH_MAP(essence, DT_EXTRA_LOTTERY_ESSENCE, extra_lottery_essence_array);
	ADD_HASH_MAP(config, DT_SKILL_SET_CONFIG, skill_set_config_array);
	ADD_HASH_MAP(config, DT_GEAR_SCRIPTURE_CONFIG, gear_scripture_config_array);
	ADD_HASH_MAP(essence, DT_FIREWORK_ARRAY_ESSENCE, firework_array_essence_array);
	ADD_HASH_MAP(essence, DT_RESET_ITEM_ESSENCE, reset_item_essence_array);
	ADD_HASH_MAP(config, DT_MARRIAGE_REWARD_CONFIG, marriage_reward_config_array);
	ADD_HASH_MAP(config, DT_MYSTIC_ENHANCE_MATERIAL_CONFIG, mystic_enhance_material_config_array);
	ADD_HASH_MAP(config, DT_LUCK_VALUE_CONFIG, luck_value_config_array);
	ADD_HASH_MAP(config, DT_CLASS_ITEM_CONFIG, class_item_config_array);
	ADD_HASH_MAP(config, DT_NEW_GEAR_IDENTIFY_CONFIG, new_gear_identify_config_array);
	ADD_HASH_MAP(config, DT_NEW_GEAR_LIST_CONFIG, new_gear_list_config_array);
	ADD_HASH_MAP(config, DT_INSTANCE_INTERCHANGE_CONFIG, instance_interchange_config_array);
	ADD_HASH_MAP(essence, DT_SPIRIT_STONE_ESSENCE, spirit_stone_essence_array);
	ADD_HASH_MAP(config, DT_VALUE_COST_CONFIG, value_cost_config_array);
	ADD_HASH_MAP(essence, DT_ANNIVERSARY_ITEM_ESSENCE, anniversary_item_essence_array);
	ADD_HASH_MAP(config, DT_SPIRIT_EXRACTION_CONFIG, spirit_exraction_config_array);
	ADD_HASH_MAP(config, DT_NEW_DOTA_SKILL_CONFIG, new_dota_skill_config_array);
	ADD_HASH_MAP(config, DT_ANOTHER_DOTA_SKILL_CONFIG, another_dota_skill_config_array);
	ADD_HASH_MAP(config, DT_EXTRA_DOTA_SKILL_CONFIG, extra_dota_skill_config_array);
	ADD_HASH_MAP(config, DT_WARDOBE_FASHION_CONFIG, wardobe_fashion_config_array);
	ADD_HASH_MAP(config, DT_NEW_DYE_CONFIG, new_dye_config_array);
	ADD_HASH_MAP(config, DT_EMBROIDERY_CONFIG, embroidery_config_array);
	ADD_HASH_MAP(config, DT_MERGED_STAR_SOUL_LIMIT_CONFIG, merged_star_soul_limit_config_array);
	ADD_HASH_MAP(config, DT_MERGED_STAR_SOUL_GROWTH_CONFIG, merged_star_soul_growth_config_array);
	ADD_HASH_MAP(config, DT_MERGED_STAR_SOUL_SLOT_CONFIG, merged_star_soul_slot_config_array);
	ADD_HASH_MAP(config, DT_RUNE2013_MATERIAL_CONFIG, rune2013_material_config_array);
	ADD_HASH_MAP(config, DT_NEW_VIP_CONFIG, new_vip_config_array);
	ADD_HASH_MAP(config, DT_RUNE2013_CONSUMPTION_CONFIG, rune2013_consumption_config_array);
	ADD_HASH_MAP(config, DT_FORBID_DIVINE_SKILL_CONFIG, forbid_divine_skill_config_array);
	ADD_HASH_MAP(essence, DT_GIFT_LETTER_ESSENCE, gift_letter_essence_array);
	ADD_HASH_MAP(config, DT_WARDOBE_WEAPON_CONFIG, wardobe_weapon_config_array);
	ADD_HASH_MAP(config, DT_WARDOBE_FOOTPRINT_CONFIG, wardobe_footprint_config_array);
	ADD_HASH_MAP(essence, DT_FOOTPRINT_ESSENCE, footprint_essence_array);
	ADD_HASH_MAP(essence, DT_FORTUNE_GIFT_PACK_ESSENCE, fortune_gift_pack_essence_array);
	ADD_HASH_MAP(config, DT_CANTEEN_EVENT_CONFIG, canteen_event_config_array);
	ADD_HASH_MAP(config, DT_RETURN_AWARD_CONFIG, return_award_config_array);
	ADD_HASH_MAP(config, DT_MOBILE_PHONE_INFO_CONFIG, mobile_phone_info_config_array);
	ADD_HASH_MAP(config, DT_BEGINNER_ONLINE_REWARD_CONFIG, beginner_online_reward_config_array);
	ADD_HASH_MAP(config, DT_MONTHLY_RETURN_AWARD_CONFIG, monthly_return_award_config_array);
	ADD_HASH_MAP(config, DT_INSTANT_TIER_CONFIG, instant_tier_config_array);
	ADD_HASH_MAP(config, DT_PLOT_REWARD_CONFIG, plot_reward_config_array);
#undef ADD_HASH_MAP
}

int elementdataman::save_data(const char * pathname)
{
	long md5pos[5];

	FILE * file;
	file = fopen(pathname, "wb+");
	if(file == NULL)		return -1;

	unsigned int version = ELEMENTDATA_VERSION;
	fwrite(&version, sizeof(unsigned int), 1, file);

	time_t t = time(NULL);
	fwrite(&t, sizeof(time_t), 1, file);

	equipment_addon_array.save(file);
	equipment_major_type_array.save(file);
	equipment_sub_type_array.save(file);
	equipment_essence_array.save(file);
	medicine_major_type_array.save(file);
	medicine_sub_type_array.save(file);
	medicine_essence_array.save(file);
	material_major_type_array.save(file);
	material_sub_type_array.save(file);
	material_essence_array.save(file);
	refine_ticket_essence_array.save(file);
	skilltome_sub_type_array.save(file);
	skilltome_essence_array.save(file);

	md5pos[0] = ftell(file);
	fseek(file, 8, SEEK_CUR);

	transmitroll_essence_array.save(file);
	luckyroll_essence_array.save(file);
	townscroll_essence_array.save(file);
	revivescroll_essence_array.save(file);
	taskmatter_essence_array.save(file);
	droptable_type_array.save(file);
	droptable_essence_array.save(file);
	monster_type_array.save(file);
	monster_essence_array.save(file);
	offline_trustee_essence_array.save(file);

#ifdef _WINDOWS 
#ifdef _MD5_CHECK
	extern void MyGetComputerName(char * szBuffer, unsigned long * pLen);
	extern void MySleep();
	// Get the computer's name of the exporter
	unsigned int tag = 0x19e75edf;
	fwrite(&tag, sizeof(unsigned int), 1, file);
	char cname[256];
	unsigned long len = 256;
	MyGetComputerName(cname, &len);
	fwrite(&len, sizeof(unsigned int), 1, file);
	char c[4] = {(char)0x5f, (char)0x6d, (char)0xe8, (char)0xc9};
	for(unsigned int l=0; l<len; l++)
		cname[l] = cname[l] ^ c[l % 4];
	fwrite(cname, len, 1, file);
	MySleep();
	time_t ttt = time(NULL);
	fwrite(&ttt, sizeof(time_t), 1, file);
#endif
#endif

	npc_talk_service_array.save(file);
	npc_sell_service_array.save(file);
	npc_buy_service_array.save(file);
	npc_task_in_service_array.save(file);
	npc_task_out_service_array.save(file);
	npc_task_matter_service_array.save(file);
	npc_heal_service_array.save(file);
	npc_transmit_service_array.save(file);
	npc_proxy_service_array.save(file);
	npc_storage_service_array.save(file);
	npc_type_array.save(file);
	npc_essence_array.save(file);
	recipe_major_type_array.save(file);

	md5pos[1] = ftell(file);
	fseek(file, 8, SEEK_CUR);

	recipe_sub_type_array.save(file);
	recipe_essence_array.save(file);
	enemy_faction_config_array.save(file);
	character_class_config_array.save(file);
	param_adjust_config_array.save(file);
	pie_love_config_array.save(file);
	taskdice_essence_array.save(file);
	tasknormalmatter_essence_array.save(file);
	mine_type_array.save(file);
	mine_essence_array.save(file);
	gm_generator_type_array.save(file);
	gm_generator_essence_array.save(file);
	fireworks_essence_array.save(file);
	player_levelexp_config_array.save(file);
	npc_war_towerbuild_service_array.save(file);
	player_secondlevel_config_array.save(file);
	npc_resetprop_service_array.save(file);
	estone_essence_array.save(file);
	pstone_essence_array.save(file);
	sstone_essence_array.save(file);

#ifdef _WINDOWS 
#ifdef _MD5_CHECK
	extern void MyGetHardInfo(unsigned char * buffer, unsigned long * len);
	unsigned int tag2 = 0xee35679f;
	fwrite(&tag2, sizeof(unsigned int), 1, file);
	unsigned char buffer[1024];
	unsigned long buffer_len = 1024;
	MyGetHardInfo(buffer, &buffer_len);
	fwrite(&buffer_len, sizeof(unsigned int), 1, file);
	fwrite(buffer, buffer_len, 1, file);
#endif
#endif

	reciperoll_major_type_array.save(file);
	reciperoll_sub_type_array.save(file);
	reciperoll_essence_array.save(file);
	suite_essence_array.save(file);
	double_exp_essence_array.save(file);
	destroying_essence_array.save(file);

	md5pos[2] = ftell(file);
	fseek(file, 8, SEEK_CUR);

	npc_equipbind_service_array.save(file);
	npc_equipdestroy_service_array.save(file);
	npc_equipundestroy_service_array.save(file);
	skillmatter_essence_array.save(file);
	vehicle_essence_array.save(file);
	couple_jumpto_essence_array.save(file);
	lottery_essence_array.save(file);
	camrecorder_essence_array.save(file);
	title_prop_config_array.save(file);
	special_id_config_array.save(file);
	text_fireworks_essence_array.save(file);
	talisman_mainpart_essence_array.save(file);
	talisman_expfood_essence_array.save(file);
	talisman_mergekatalyst_essence_array.save(file);
	talisman_energyfood_essence_array.save(file);
	speaker_essence_array.save(file);
	player_talent_config_array.save(file);
	player_skill_point_config_array.save(file);
	potential_tome_essence_array.save(file);
	war_role_config_array.save(file);
	npc_war_buy_archer_service_array.save(file);
	siege_artillery_scroll_essence_array.save(file);
	pet_bedge_essence_array.save(file);
	pet_food_essence_array.save(file);
	pet_skill_essence_array.save(file);
	pet_armor_essence_array.save(file);
	pet_auto_food_essence_array.save(file);
	pet_refine_essence_array.save(file);
	pet_assist_refine_essence_array.save(file);
	renasence_skill_config_array.save(file);
	renasence_prop_config_array.save(file);
	aircraft_essence_array.save(file);
	fly_energyfood_essence_array.save(file);
	item_trade_config_array.save(file);
	book_essence_array.save(file);
	equip_soul_essence_array.save(file);
	equip_soul_meld_service_array.save(file);
	special_name_item_essence_array.save(file);
	recycle_item_config_array.save(file);
	score_to_rank_config_array.save(file);
	battle_drop_config_array.save(file);

	md5pos[3] = ftell(file);
	fseek(file, 8, SEEK_CUR);

	battle_deprive_config_array.save(file);
	battle_score_config_array.save(file);
	gift_bag_essence_array.save(file);
	vip_card_essence_array.save(file);
	instancing_battle_config_array.save(file);
	change_shape_card_essence_array.save(file);
	change_shape_stone_essence_array.save(file);
	change_shape_prop_config_array.save(file);
	original_shape_config_array.save(file);
	life_skill_config_array.save(file);
	arena_droptable_config_array.save(file);
	mercenary_credential_essence_array.save(file);
	teleportation_essence_array.save(file);
	teleportation_stone_essence_array.save(file);
	combine_skill_edit_config_array.save(file);
	upgrade_equip_config_array.save(file);
	upgrade_equip_config_1_array.save(file);
	upgrade_equip_stone_essence_array.save(file);
	npc_consign_service_array.save(file);
	deity_character_config_array.save(file);
	lottery2_essence_array.save(file);
	gem_config_array.save(file);//Added 2011-03-14.
	gem_essence_array.save(file);
	gem_seal_essence_array.save(file);
	gem_dust_essence_array.save(file);
	gem_extract_config_array.save(file);
	general_article_essence_array.save(file);
	lottery3_essence_array.save(file); //Added 2011-07-01.
	transcription_config_array.save(file); //Added 2011-07-13.
	online_gift_bag_config_array.save(file); //Added 2011-12-06.
	small_online_gift_bag_essence_array.save(file); //Added 2011-12-06.
	scroll_region_config_array.save(file); //Added 2012-03-20.
	scroll_unlock_essence_array.save(file); //Added 2012-03-20.
	scroll_dig_count_inc_essence_array.save(file);	//Added 2012-03-22.
	tower_transcription_config_array.save(file);	//Added 2012-03-31.
	tower_transcription_property_config_array.save(file);//Added 2012-03-31.
	rune_data_config_array.save(file);//Added 2012.04.17
	rune_comb_property_array.save(file);//Added 2012.04.17
	rune_equip_array.save(file);//Added 2012.04.17
	little_pet_upgrade_config_array.save(file);	//Added 2012-04-19.
	rune_slot_array.save(file);//Added 2012.04.21
	drop_interval_config_array.save(file);
	pk2012_guess_config_array.save(file);
	collision_raid_transfigure_config_array.save(file);	//Added 2012-08-02.
	booth_figure_item_array.save(file);
	flag_buff_item_array.save(file);
	npc_reputation_shop_service_array.save(file);
	npc_ui_transfer_service_array.save(file);
	ui_transfer_config_array.save(file);
	xingzuo_item_essence_array.save(file);
	xingzuo_energy_item_essence_array.save(file);
	collision_raid_award_config_array.save(file);
	cash_medic_merge_item_essence_array.save(file);
	buff_area_config_array.save(file);
	liveness_config_array.save(file);
	challenge_2012_config_array.save(file);
	sale_promotion_item_essence_array.save(file);
	gift_bag_lottery_deliver_essence_array.save(file);
	lottery_tangyuan_item_essence_array.save(file);
	task_special_award_config_array.save(file);
	gift_pack_item_essence_array.save(file);	
	prop_add_config_array.save(file);
	prop_add_material_item_essence_array.save(file);
	prop_add_item_essence_array.save(file);
	king_war_config_array.save(file);
	jinfashen_to_money_config_array.save(file);
	battle_201304_config_array.save(file);
	rune2013_item_essence_array.save(file);
	rune2013_config_array.save(file);
	bashe_award_config_array.save(file);
	arena_season_time_config_array.save(file);
	personal_level_award_config_array.save(file);
	team_season_award_config_array.save(file);
	week_currency_award_config_array.save(file);
	colorant_item_essence_array.save(file);
	colorant2_item_essence_array.save(file);
	interaction_object_essence_array.save(file);
	interaction_object_hook_config_array.save(file);
	colorant_config_array.save(file);
	faction_transfer_config_array.save(file);
	building_region_config_array.save(file);
	building_essence_array.save(file);
	region_block_essence_array.save(file);
	phase_config_array.save(file);
	cross6v6_item_exchange_config_array.save(file);
	transcript_strategy_config_array.save(file);
	faction_shop_config_array.save(file);
	faction_auction_config_array.save(file);
	faction_money_shop_config_array.save(file);
	compound_mine_essence_array.save(file);
	colorant_default_config_array.save(file);

	size_t sz = talk_proc_array.size();

	fwrite(&sz, sizeof(size_t), 1, file);
	size_t i;
	for(i=0; i<sz; i++)
		talk_proc_array[i]->save(file);

	md5pos[4] = ftell(file);

#ifdef _WINDOWS
	long fstart = 0;
	char buf[8192];
	for(i = 0; i < 5; i++)
	{
		fseek(file, fstart, SEEK_SET);
		while(fstart < md5pos[i])
		{
			int readsize = 8192;
			if( md5pos[i] - fstart < readsize )
				readsize = md5pos[i] - fstart;
			fread(buf, 1, readsize, file);
			g_MD5Hash.Update(buf, readsize);
			fstart += readsize;
		}
		fstart += 8;
	}

	g_MD5Hash.Final();
	unsigned int size = 50;
	g_MD5Hash.GetString(buf, size);
	for(i = 0; i < 4; i++)
	{
		fseek(file, md5pos[i], SEEK_SET);
		fwrite(buf + i * 8, 1, 8, file);
	}
#endif

	fclose(file);
	return 0;
}

int elementdataman::load_data(const char * pathname)
{
	long md5pos[5];

	gs_log("load_data: opening '%s'", pathname);
	FILE * file;
	file = fopen(pathname, "rb");
	if(file == NULL)
	{
		gs_log("load_data: fopen failed");
		return -1;
	}

	unsigned int version = 0;
	fread(&version, sizeof(unsigned int), 1, file);
	gs_log("load_data: file version=0x%x expected=0x%x", version, (unsigned int)ELEMENTDATA_VERSION);
	if( version != ELEMENTDATA_VERSION )
	{
		gs_log("load_data: version mismatch - aborting");
		fclose(file);
		return -1;
	}

	time_t t;		// #define _USE_32BIT_TIME_T sizeof(time_t)=4
	fread(&t, sizeof(time_t), 1, file);
	gs_log("load_data: version OK, beginning array loads");

#define LOAD_ARRAY(arr) \
	{ \
		unsigned int _stored = (arr).peek_stored_sizeof(file); \
		unsigned int _expected = (arr).get_element_sizeof(); \
		gs_log("load_data: loading " #arr " at offset %ld  stored_sizeof=%u  expected_sizeof=%u", ftell(file), _stored, _expected); \
		if((arr).load(file) != 0) { gs_log("load_data: FAILED " #arr "  stored_sizeof=%u  expected_sizeof=%u  diff=%d", _stored, _expected, (int)_stored - (int)_expected); fclose(file); return -1; } \
	}

	LOAD_ARRAY(equipment_addon_array)
	LOAD_ARRAY(equipment_major_type_array)
	LOAD_ARRAY(equipment_sub_type_array)
	LOAD_ARRAY(equipment_essence_array)
	LOAD_ARRAY(medicine_major_type_array)
	LOAD_ARRAY(medicine_sub_type_array)
	LOAD_ARRAY(medicine_essence_array)
	LOAD_ARRAY(material_major_type_array)
	LOAD_ARRAY(material_sub_type_array)
	LOAD_ARRAY(material_essence_array)
	LOAD_ARRAY(refine_ticket_essence_array)
	LOAD_ARRAY(skilltome_sub_type_array)
	LOAD_ARRAY(skilltome_essence_array)

	md5pos[0] = ftell(file);
	fseek(file, 8, SEEK_CUR);

	LOAD_ARRAY(transmitroll_essence_array)
	LOAD_ARRAY(luckyroll_essence_array)
	LOAD_ARRAY(townscroll_essence_array)
	LOAD_ARRAY(revivescroll_essence_array)
	LOAD_ARRAY(taskmatter_essence_array)
	LOAD_ARRAY(droptable_type_array)
	LOAD_ARRAY(droptable_essence_array)
	LOAD_ARRAY(monster_type_array)
	LOAD_ARRAY(monster_essence_array)
	LOAD_ARRAY(offline_trustee_essence_array)

	// skip the computer's name of the exporter
	unsigned int tag;
	fread(&tag, sizeof(unsigned int), 1, file);
	char buffer[1024];
	unsigned int len;
	fread(&len, sizeof(unsigned int), 1, file);
	fread(buffer, len, 1, file);
	fread(&t, sizeof(int), 1, file);
	
	LOAD_ARRAY(npc_talk_service_array)
	LOAD_ARRAY(npc_sell_service_array)
	LOAD_ARRAY(npc_buy_service_array)
	LOAD_ARRAY(npc_task_in_service_array)
	LOAD_ARRAY(npc_task_out_service_array)
	LOAD_ARRAY(npc_task_matter_service_array)
	LOAD_ARRAY(npc_heal_service_array)
	LOAD_ARRAY(npc_transmit_service_array)
	LOAD_ARRAY(npc_proxy_service_array)
	LOAD_ARRAY(npc_storage_service_array)
	LOAD_ARRAY(npc_type_array)
	LOAD_ARRAY(npc_essence_array)
	LOAD_ARRAY(recipe_major_type_array)

	md5pos[1] = ftell(file);
	fseek(file, 8, SEEK_CUR);

	LOAD_ARRAY(recipe_sub_type_array)
	LOAD_ARRAY(recipe_essence_array)
	LOAD_ARRAY(enemy_faction_config_array)
	LOAD_ARRAY(character_class_config_array)
	LOAD_ARRAY(param_adjust_config_array)
	LOAD_ARRAY(pie_love_config_array)
	LOAD_ARRAY(taskdice_essence_array)
	LOAD_ARRAY(tasknormalmatter_essence_array)
	LOAD_ARRAY(mine_type_array)
	LOAD_ARRAY(mine_essence_array)
	LOAD_ARRAY(gm_generator_type_array)
	LOAD_ARRAY(gm_generator_essence_array)
	LOAD_ARRAY(fireworks_essence_array)
	LOAD_ARRAY(player_levelexp_config_array)
	LOAD_ARRAY(npc_war_towerbuild_service_array)
	LOAD_ARRAY(player_secondlevel_config_array)
	LOAD_ARRAY(npc_resetprop_service_array)
	LOAD_ARRAY(estone_essence_array)
	LOAD_ARRAY(pstone_essence_array)
	LOAD_ARRAY(sstone_essence_array)

	fread(&tag, sizeof(unsigned int), 1, file);
	fread(&len, sizeof(unsigned int), 1, file);
	fread(buffer, len, 1, file);

	LOAD_ARRAY(reciperoll_major_type_array)
	LOAD_ARRAY(reciperoll_sub_type_array)
	LOAD_ARRAY(reciperoll_essence_array)
	LOAD_ARRAY(suite_essence_array)
	LOAD_ARRAY(double_exp_essence_array)
	LOAD_ARRAY(destroying_essence_array)

	md5pos[2] = ftell(file);
	fseek(file, 8, SEEK_CUR);

	LOAD_ARRAY(npc_equipbind_service_array)
	LOAD_ARRAY(npc_equipdestroy_service_array)
	LOAD_ARRAY(npc_equipundestroy_service_array)
	LOAD_ARRAY(skillmatter_essence_array)
	LOAD_ARRAY(vehicle_essence_array)
	LOAD_ARRAY(couple_jumpto_essence_array)
	LOAD_ARRAY(lottery_essence_array)
	LOAD_ARRAY(camrecorder_essence_array)
	LOAD_ARRAY(title_prop_config_array)
	LOAD_ARRAY(special_id_config_array)
	LOAD_ARRAY(text_fireworks_essence_array)
	LOAD_ARRAY(talisman_mainpart_essence_array)
	LOAD_ARRAY(talisman_expfood_essence_array)
	LOAD_ARRAY(talisman_mergekatalyst_essence_array)
	LOAD_ARRAY(talisman_energyfood_essence_array)
	LOAD_ARRAY(speaker_essence_array)
	LOAD_ARRAY(player_talent_config_array)
	LOAD_ARRAY(player_skill_point_config_array)
	LOAD_ARRAY(potential_tome_essence_array)
	LOAD_ARRAY(war_role_config_array)
	LOAD_ARRAY(npc_war_buy_archer_service_array)
	LOAD_ARRAY(siege_artillery_scroll_essence_array)
	LOAD_ARRAY(pet_bedge_essence_array)
	LOAD_ARRAY(pet_food_essence_array)
	LOAD_ARRAY(pet_skill_essence_array)
	LOAD_ARRAY(pet_armor_essence_array)
	LOAD_ARRAY(pet_auto_food_essence_array)
	LOAD_ARRAY(pet_refine_essence_array)
	LOAD_ARRAY(pet_assist_refine_essence_array)
	LOAD_ARRAY(renasence_skill_config_array)
	LOAD_ARRAY(renasence_prop_config_array)
	LOAD_ARRAY(aircraft_essence_array)
	LOAD_ARRAY(fly_energyfood_essence_array)
	LOAD_ARRAY(item_trade_config_array)
	LOAD_ARRAY(book_essence_array)
	LOAD_ARRAY(equip_soul_essence_array)
	LOAD_ARRAY(equip_soul_meld_service_array)
	LOAD_ARRAY(special_name_item_essence_array)
	LOAD_ARRAY(recycle_item_config_array)
	LOAD_ARRAY(score_to_rank_config_array)
	LOAD_ARRAY(battle_drop_config_array)

	md5pos[3] = ftell(file);
	fseek(file, 8, SEEK_CUR);

	LOAD_ARRAY(battle_deprive_config_array)
	LOAD_ARRAY(battle_score_config_array)
	LOAD_ARRAY(gift_bag_essence_array)
	LOAD_ARRAY(vip_card_essence_array)
	LOAD_ARRAY(instancing_battle_config_array)
	LOAD_ARRAY(change_shape_card_essence_array)
	LOAD_ARRAY(change_shape_stone_essence_array)
	LOAD_ARRAY(change_shape_prop_config_array)
	LOAD_ARRAY(original_shape_config_array)
	LOAD_ARRAY(life_skill_config_array)
	LOAD_ARRAY(arena_droptable_config_array)
	LOAD_ARRAY(mercenary_credential_essence_array)
	LOAD_ARRAY(teleportation_essence_array)
	LOAD_ARRAY(teleportation_stone_essence_array)
	LOAD_ARRAY(combine_skill_edit_config_array)
	LOAD_ARRAY(upgrade_equip_config_array)
	LOAD_ARRAY(upgrade_equip_config_1_array)
	LOAD_ARRAY(upgrade_equip_stone_essence_array)
	LOAD_ARRAY(npc_consign_service_array)
	LOAD_ARRAY(deity_character_config_array)
	LOAD_ARRAY(lottery2_essence_array)
	LOAD_ARRAY(gem_config_array)
	LOAD_ARRAY(gem_essence_array)
	LOAD_ARRAY(gem_seal_essence_array)
	LOAD_ARRAY(gem_dust_essence_array)
	LOAD_ARRAY(gem_extract_config_array)
	LOAD_ARRAY(general_article_essence_array)
	LOAD_ARRAY(lottery3_essence_array)
	LOAD_ARRAY(transcription_config_array)
	LOAD_ARRAY(online_gift_bag_config_array)
	LOAD_ARRAY(small_online_gift_bag_essence_array)
	LOAD_ARRAY(scroll_region_config_array)
	LOAD_ARRAY(scroll_unlock_essence_array)
	LOAD_ARRAY(scroll_dig_count_inc_essence_array)
	LOAD_ARRAY(tower_transcription_config_array)
	LOAD_ARRAY(tower_transcription_property_config_array)
	LOAD_ARRAY(rune_data_config_array)
	LOAD_ARRAY(rune_comb_property_array)
	LOAD_ARRAY(rune_equip_array)
	LOAD_ARRAY(little_pet_upgrade_config_array)
	LOAD_ARRAY(rune_slot_array)
	LOAD_ARRAY(drop_interval_config_array)
	LOAD_ARRAY(pk2012_guess_config_array)
	LOAD_ARRAY(collision_raid_transfigure_config_array)
	LOAD_ARRAY(booth_figure_item_array)
	LOAD_ARRAY(flag_buff_item_array)
	LOAD_ARRAY(npc_reputation_shop_service_array)
	LOAD_ARRAY(npc_ui_transfer_service_array)
	LOAD_ARRAY(ui_transfer_config_array)
	LOAD_ARRAY(xingzuo_item_essence_array)
	LOAD_ARRAY(xingzuo_energy_item_essence_array)
	LOAD_ARRAY(collision_raid_award_config_array)
	LOAD_ARRAY(cash_medic_merge_item_essence_array)
	LOAD_ARRAY(buff_area_config_array)
	LOAD_ARRAY(liveness_config_array)
	LOAD_ARRAY(challenge_2012_config_array)
	LOAD_ARRAY(sale_promotion_item_essence_array)
	LOAD_ARRAY(gift_bag_lottery_deliver_essence_array)
	LOAD_ARRAY(lottery_tangyuan_item_essence_array)
	LOAD_ARRAY(task_special_award_config_array)
	LOAD_ARRAY(gift_pack_item_essence_array)
	LOAD_ARRAY(prop_add_config_array)
	LOAD_ARRAY(prop_add_material_item_essence_array)
	LOAD_ARRAY(prop_add_item_essence_array)
	LOAD_ARRAY(king_war_config_array)
	LOAD_ARRAY(jinfashen_to_money_config_array)
	LOAD_ARRAY(battle_201304_config_array)
	LOAD_ARRAY(rune2013_item_essence_array)
	LOAD_ARRAY(rune2013_config_array)
	LOAD_ARRAY(bashe_award_config_array)
	LOAD_ARRAY(arena_season_time_config_array)
	LOAD_ARRAY(personal_level_award_config_array)
	LOAD_ARRAY(team_season_award_config_array)
	LOAD_ARRAY(week_currency_award_config_array)
	LOAD_ARRAY(colorant_item_essence_array)
	LOAD_ARRAY(colorant2_item_essence_array)
	LOAD_ARRAY(interaction_object_essence_array)
	LOAD_ARRAY(interaction_object_hook_config_array)
	LOAD_ARRAY(colorant_config_array)
	LOAD_ARRAY(faction_transfer_config_array)
	LOAD_ARRAY(building_region_config_array)
	LOAD_ARRAY(building_essence_array)
	LOAD_ARRAY(region_block_essence_array)
	LOAD_ARRAY(phase_config_array)
	LOAD_ARRAY(cross6v6_item_exchange_config_array)
	LOAD_ARRAY(transcript_strategy_config_array)
	LOAD_ARRAY(faction_shop_config_array)
	LOAD_ARRAY(faction_auction_config_array)
	LOAD_ARRAY(faction_money_shop_config_array)
	LOAD_ARRAY(compound_mine_essence_array)
	LOAD_ARRAY(colorant_default_config_array)

	// v158 new data tables (before talk_proc, matching editor file layout)
	LOAD_ARRAY(sign_in_config_array)
	LOAD_ARRAY(baby_essence_array)
	LOAD_ARRAY(class_baby_relation_config_array)
	LOAD_ARRAY(baby_config_array)
	LOAD_ARRAY(baby_property_config_array)
	LOAD_ARRAY(baby_upgrade_config_array)
	LOAD_ARRAY(baby_fashion_essence_array)
	LOAD_ARRAY(baby_toy_essence_array)
	LOAD_ARRAY(baby_book_essence_array)
	LOAD_ARRAY(matrix_card_essence_array)
	LOAD_ARRAY(matrix_equip_essence_array)
	LOAD_ARRAY(matrix_exp_config_array)
	LOAD_ARRAY(matrix_collect_config_array)
	LOAD_ARRAY(matrix_card_upgread_config_array)
	LOAD_ARRAY(matrix_combine_config_array)
	LOAD_ARRAY(baby_food_essence_array)
	LOAD_ARRAY(baby_title_essence_array)
	LOAD_ARRAY(baby_title_list_array)
	LOAD_ARRAY(vehicle_enhance_essence_array)
	LOAD_ARRAY(talent_scroll_essence_array)
	LOAD_ARRAY(talent_scroll_config_array)
	LOAD_ARRAY(talent_scroll_skill_config_array)
	LOAD_ARRAY(skill_replace_config_array)
	LOAD_ARRAY(hill_transcription_config_array)
	LOAD_ARRAY(consume_award_config_array)
	LOAD_ARRAY(multi_tower_property_config_array)
	LOAD_ARRAY(raid18v18_award_config_array)
	LOAD_ARRAY(eggs_gift_bag_essence_array)
	LOAD_ARRAY(item_trade2_config_array)
	LOAD_ARRAY(guess_essence_array)
	LOAD_ARRAY(elite_game_result_config_array)
	LOAD_ARRAY(elite_game_award_config_array)
	LOAD_ARRAY(title_item_essence_array)
	LOAD_ARRAY(soul_enhance_config_array)
	LOAD_ARRAY(soul_exchange_config_array)
	LOAD_ARRAY(soul_drop_config_array)
	LOAD_ARRAY(daily_sign_config_array)
	LOAD_ARRAY(red_packets_essence_array)
	LOAD_ARRAY(forbid_common_skill_config_array)
	LOAD_ARRAY(forbid_godevil_skill_config_array)
	LOAD_ARRAY(force_cd_skill_config_array)
	LOAD_ARRAY(talent_scroll_refine_essence_array);
	LOAD_ARRAY(faction_base_activity_config_array);
	LOAD_ARRAY(amulet_essence_array);
	LOAD_ARRAY(amulet_config_array);
	LOAD_ARRAY(amulet_stone_essence_array);
	LOAD_ARRAY(special_vip_config_array);
	LOAD_ARRAY(charge_award_config_array);
	LOAD_ARRAY(new_townscroll_essence_array);
	LOAD_ARRAY(equipment_randomext_config_array);
	LOAD_ARRAY(indiscrimination_battle_config_array);
	LOAD_ARRAY(groupraid_evalaward_config_array);
	LOAD_ARRAY(equipment_identify_config_array);
	LOAD_ARRAY(headframe_essence_array);
	LOAD_ARRAY(convert_equip_config_array);
	LOAD_ARRAY(auspicious_hall_config_array);
	LOAD_ARRAY(divine_accessory_imbue_config_array);
	LOAD_ARRAY(level_up_reward_config_array);
	LOAD_ARRAY(new_box_essence_array);
	LOAD_ARRAY(new_fashion_pack_essence_array);
	LOAD_ARRAY(new_crafting_config_array);
	LOAD_ARRAY(fairyhouse_general_config_array);
	LOAD_ARRAY(fairyhouse_probability_config_array);
	LOAD_ARRAY(fairyhouse_baby_trait_config_array);
	LOAD_ARRAY(fairyhouse_pet_trait_config_array);
	LOAD_ARRAY(fairyhouse_challenge_config_array);
	LOAD_ARRAY(fairyhouse_baby_gear_essence_array);
	LOAD_ARRAY(fairyhouse_pet_gear_essence_array);
	LOAD_ARRAY(fairyhouse_trait_learn_essence_array);
	LOAD_ARRAY(prepaid_book_essence_array);
	LOAD_ARRAY(token_bag_config_array);
	LOAD_ARRAY(timelimit_bead_essence_array);
	LOAD_ARRAY(extra_timelimit_bead_essence_array);
	LOAD_ARRAY(constellationexp_config_array);
	LOAD_ARRAY(constellation_skill_config_array);
	LOAD_ARRAY(constellation_config_array);
	LOAD_ARRAY(constellation_stars_essence_array);
	LOAD_ARRAY(constellation_skill_relation_config_array);
	LOAD_ARRAY(talent_scroll_trigger_config_array);
	LOAD_ARRAY(event_reward_set_config_array);
	LOAD_ARRAY(extra_lottery_essence_array);
	LOAD_ARRAY(skill_set_config_array);
	LOAD_ARRAY(gear_scripture_config_array);
	LOAD_ARRAY(firework_array_essence_array);
	LOAD_ARRAY(reset_item_essence_array);
	LOAD_ARRAY(marriage_reward_config_array);
	LOAD_ARRAY(mystic_enhance_material_config_array);
	LOAD_ARRAY(luck_value_config_array);
	LOAD_ARRAY(class_item_config_array);
	LOAD_ARRAY(new_gear_identify_config_array);
	LOAD_ARRAY(new_gear_list_config_array);
	LOAD_ARRAY(instance_interchange_config_array);
	LOAD_ARRAY(spirit_stone_essence_array);
	LOAD_ARRAY(value_cost_config_array);
	LOAD_ARRAY(anniversary_item_essence_array);
	LOAD_ARRAY(spirit_exraction_config_array);
	LOAD_ARRAY(new_dota_skill_config_array);
	LOAD_ARRAY(another_dota_skill_config_array);
	LOAD_ARRAY(extra_dota_skill_config_array);
	LOAD_ARRAY(wardobe_fashion_config_array);
	LOAD_ARRAY(new_dye_config_array);
	LOAD_ARRAY(embroidery_config_array);
	LOAD_ARRAY(merged_star_soul_limit_config_array);
	LOAD_ARRAY(merged_star_soul_growth_config_array);
	LOAD_ARRAY(merged_star_soul_slot_config_array);

	md5pos[4] = ftell(file);
	fseek(file, 8, SEEK_CUR);

	LOAD_ARRAY(rune2013_material_config_array);
	LOAD_ARRAY(new_vip_config_array);
	LOAD_ARRAY(rune2013_consumption_config_array);
	LOAD_ARRAY(forbid_divine_skill_config_array);
	LOAD_ARRAY(gift_letter_essence_array);
	LOAD_ARRAY(wardobe_weapon_config_array);
	LOAD_ARRAY(wardobe_footprint_config_array);
	LOAD_ARRAY(footprint_essence_array);
	LOAD_ARRAY(fortune_gift_pack_essence_array);
	LOAD_ARRAY(canteen_event_config_array);
	LOAD_ARRAY(return_award_config_array);
	LOAD_ARRAY(mobile_phone_info_config_array);
	LOAD_ARRAY(beginner_online_reward_config_array);
	LOAD_ARRAY(monthly_return_award_config_array);
	LOAD_ARRAY(instant_tier_config_array);
	LOAD_ARRAY(plot_reward_config_array);

	gs_log("load_data: loading talk_proc entries");
	size_t sz = 0;
	if(fread(&sz, sizeof(size_t), 1, file) != 1)
	{
		gs_log("load_data: FAILED reading talk_proc count");
		fclose(file);
		return -1;
	}
	gs_log("load_data: talk_proc count=%lu", (unsigned long)sz);
	size_t i;
	for(i=0; i<sz; i++)
	{
		talk_proc * tp = new talk_proc;
		tp->load(file);
		add_structure(tp->id_talk, tp);
	}

	setup_hash_map();

	fclose(file);
	return 0;
}