---//AddSkillProficiency 
---//怪物捕捉信息静态数据
function ZStaticAvailableMonsterList()
	local staticmonster={}
		staticmonster[1]={monster_id=30280,pet_id=29996}	---//小鹿
		staticmonster[2]={monster_id=31555,pet_id=30991}	---//岩魔
		staticmonster[3]={monster_id=31556,pet_id=30988}	---//大头娃娃
		staticmonster[4]={monster_id=31557,pet_id=30989}	---//判官
		staticmonster[5]={monster_id=31558,pet_id=30990}	---//猫头鹰
	return staticmonster
end 
---//配方信息静态数据---------------------------------------------------------------------
function ZStaticRecipeList()
	local staticrecipe={}
		staticrecipe[1]={recipe_id=2417,recipe_group=1,recipe_level=1,recipe_type=11}---金曜之太极罡气
		staticrecipe[2]={recipe_id=2423,recipe_group=1,recipe_level=1,recipe_type=11}---水曜之太极罡气
		staticrecipe[3]={recipe_id=2411,recipe_group=1,recipe_level=1,recipe_type=11}---太极罡气
		staticrecipe[4]={recipe_id=2418,recipe_group=1,recipe_level=1,recipe_type=11}---金曜之先天罡气
		staticrecipe[5]={recipe_id=2424,recipe_group=1,recipe_level=1,recipe_type=11}---水曜之先天罡气
		staticrecipe[6]={recipe_id=2412,recipe_group=1,recipe_level=1,recipe_type=11}---先天罡气
		staticrecipe[7]={recipe_id=2519,recipe_group=1,recipe_level=2,recipe_type=11}---★★金曜之太极罡气
		staticrecipe[8]={recipe_id=2525,recipe_group=1,recipe_level=2,recipe_type=11}---★★水曜之太极罡气
		staticrecipe[9]={recipe_id=2520,recipe_group=1,recipe_level=2,recipe_type=11}---★★金曜之先天罡气
		staticrecipe[10]={recipe_id=2526,recipe_group=1,recipe_level=2,recipe_type=11}---★★水曜之先天罡气
		staticrecipe[11]={recipe_id=2507,recipe_group=1,recipe_level=2,recipe_type=11}---★金曜之太极罡气
		staticrecipe[12]={recipe_id=2513,recipe_group=1,recipe_level=2,recipe_type=11}---★水曜之太极罡气
		staticrecipe[13]={recipe_id=2508,recipe_group=1,recipe_level=2,recipe_type=11}---★金曜之先天罡气
		staticrecipe[14]={recipe_id=2514,recipe_group=1,recipe_level=2,recipe_type=11}---★水曜之先天罡气
		staticrecipe[15]={recipe_id=2369,recipe_group=1,recipe_level=1,recipe_type=11}---日曜之霓金头饰
		staticrecipe[16]={recipe_id=2363,recipe_group=1,recipe_level=1,recipe_type=11}---霓金头饰
		staticrecipe[17]={recipe_id=2370,recipe_group=1,recipe_level=1,recipe_type=11}---日曜之龙魂头饰
		staticrecipe[18]={recipe_id=2364,recipe_group=1,recipe_level=1,recipe_type=11}---龙魂头饰
		staticrecipe[19]={recipe_id=2465,recipe_group=1,recipe_level=2,recipe_type=11}---★★日曜之霓金头饰
		staticrecipe[20]={recipe_id=2466,recipe_group=1,recipe_level=2,recipe_type=11}---★★日曜之龙魂头饰
		staticrecipe[21]={recipe_id=2459,recipe_group=1,recipe_level=2,recipe_type=11}---★日曜之霓金头饰
		staticrecipe[22]={recipe_id=2460,recipe_group=1,recipe_level=2,recipe_type=11}---★日曜之龙魂头饰
		staticrecipe[23]={recipe_id=2405,recipe_group=1,recipe_level=1,recipe_type=11}---木曜之麒麟软甲
		staticrecipe[24]={recipe_id=2399,recipe_group=1,recipe_level=1,recipe_type=11}---麒麟软甲
		staticrecipe[25]={recipe_id=2406,recipe_group=1,recipe_level=1,recipe_type=11}---木曜之龙筋软甲
		staticrecipe[26]={recipe_id=2400,recipe_group=1,recipe_level=1,recipe_type=11}---龙筋软甲
		staticrecipe[27]={recipe_id=2501,recipe_group=1,recipe_level=2,recipe_type=11}---★★木曜之麒麟软甲
		staticrecipe[28]={recipe_id=2502,recipe_group=1,recipe_level=2,recipe_type=11}---★★木曜之龙筋软甲
		staticrecipe[29]={recipe_id=2495,recipe_group=1,recipe_level=2,recipe_type=11}---★木曜之麒麟软甲
		staticrecipe[30]={recipe_id=2496,recipe_group=1,recipe_level=2,recipe_type=11}---★木曜之龙筋软甲
		staticrecipe[31]={recipe_id=2747,recipe_group=1,recipe_level=1,recipe_type=11}---火曜之太乙心经
		staticrecipe[32]={recipe_id=2447,recipe_group=1,recipe_level=1,recipe_type=11}---罗候之太乙心经
		staticrecipe[33]={recipe_id=2441,recipe_group=1,recipe_level=1,recipe_type=11}---太乙心经
		staticrecipe[34]={recipe_id=2748,recipe_group=1,recipe_level=1,recipe_type=11}---火曜之黄帝心经
		staticrecipe[35]={recipe_id=2448,recipe_group=1,recipe_level=1,recipe_type=11}---罗候之黄帝心经
		staticrecipe[36]={recipe_id=2442,recipe_group=1,recipe_level=1,recipe_type=11}---黄帝心经
		staticrecipe[37]={recipe_id=2759,recipe_group=1,recipe_level=2,recipe_type=11}---★★火曜之太乙心经
		staticrecipe[38]={recipe_id=2537,recipe_group=1,recipe_level=2,recipe_type=11}---★★罗候之太乙心经
		staticrecipe[39]={recipe_id=2760,recipe_group=1,recipe_level=2,recipe_type=11}---★★火曜之黄帝心经
		staticrecipe[40]={recipe_id=2538,recipe_group=1,recipe_level=2,recipe_type=11}---★★罗候之黄帝心经
		staticrecipe[41]={recipe_id=2753,recipe_group=1,recipe_level=2,recipe_type=11}---★火曜之太乙心经
		staticrecipe[42]={recipe_id=2531,recipe_group=1,recipe_level=2,recipe_type=11}---★罗候之太乙心经
		staticrecipe[43]={recipe_id=2754,recipe_group=1,recipe_level=2,recipe_type=11}---★火曜之黄帝心经
		staticrecipe[44]={recipe_id=2532,recipe_group=1,recipe_level=2,recipe_type=11}---★罗候之黄帝心经
		staticrecipe[45]={recipe_id=2681,recipe_group=1,recipe_level=1,recipe_type=11}---万毒华裘
		staticrecipe[46]={recipe_id=2657,recipe_group=1,recipe_level=1,recipe_type=11}---百鬼画皮
		staticrecipe[47]={recipe_id=2693,recipe_group=1,recipe_level=1,recipe_type=11}---神秘石刻
		staticrecipe[48]={recipe_id=2663,recipe_group=1,recipe_level=1,recipe_type=11}---仙将环甲
		staticrecipe[49]={recipe_id=2651,recipe_group=1,recipe_level=1,recipe_type=11}---虎精锦裘
		staticrecipe[50]={recipe_id=2669,recipe_group=1,recipe_level=1,recipe_type=11}---巨蟒鳞刺
		staticrecipe[51]={recipe_id=2675,recipe_group=1,recipe_level=1,recipe_type=11}---囚牛骨刺
		staticrecipe[52]={recipe_id=2687,recipe_group=1,recipe_level=1,recipe_type=11}---冰晶霜盾
		staticrecipe[53]={recipe_id=2682,recipe_group=1,recipe_level=1,recipe_type=11}---毒王血咒
		staticrecipe[54]={recipe_id=2658,recipe_group=1,recipe_level=1,recipe_type=11}---鬼王魔骨
		staticrecipe[55]={recipe_id=2694,recipe_group=1,recipe_level=1,recipe_type=11}---古老印记
		staticrecipe[56]={recipe_id=2664,recipe_group=1,recipe_level=1,recipe_type=11}---元帅圣铠
		staticrecipe[57]={recipe_id=2652,recipe_group=1,recipe_level=1,recipe_type=11}---九尾狐裘
		staticrecipe[58]={recipe_id=2670,recipe_group=1,recipe_level=1,recipe_type=11}---兽王天威
		staticrecipe[59]={recipe_id=2676,recipe_group=1,recipe_level=1,recipe_type=11}---孽龙逆鳞
		staticrecipe[60]={recipe_id=2688,recipe_group=1,recipe_level=1,recipe_type=11}---大地圣护
		staticrecipe[61]={recipe_id=2729,recipe_group=1,recipe_level=1,recipe_type=11}---万毒金鼎
		staticrecipe[62]={recipe_id=2711,recipe_group=1,recipe_level=1,recipe_type=11}---琼林玉树
		staticrecipe[63]={recipe_id=2705,recipe_group=1,recipe_level=1,recipe_type=11}---倾国雪肤
		staticrecipe[64]={recipe_id=2723,recipe_group=1,recipe_level=1,recipe_type=11}---神龟灵骨
		staticrecipe[65]={recipe_id=2741,recipe_group=1,recipe_level=1,recipe_type=11}---神秘图腾
		staticrecipe[66]={recipe_id=2699,recipe_group=1,recipe_level=1,recipe_type=11}---虎精怒目
		staticrecipe[67]={recipe_id=2717,recipe_group=1,recipe_level=1,recipe_type=11}---巨蟒毒信
		staticrecipe[68]={recipe_id=2735,recipe_group=1,recipe_level=1,recipe_type=11}---冰晶碎刃
		staticrecipe[69]={recipe_id=2730,recipe_group=1,recipe_level=1,recipe_type=11}---毒王手札
		staticrecipe[70]={recipe_id=2712,recipe_group=1,recipe_level=1,recipe_type=11}---神木天宝
		staticrecipe[71]={recipe_id=2706,recipe_group=1,recipe_level=1,recipe_type=11}---无双艳色
		staticrecipe[72]={recipe_id=2724,recipe_group=1,recipe_level=1,recipe_type=11}---龙子华冠
		staticrecipe[73]={recipe_id=2742,recipe_group=1,recipe_level=1,recipe_type=11}---古老圣像
		staticrecipe[74]={recipe_id=2700,recipe_group=1,recipe_level=1,recipe_type=11}---九尾狐血
		staticrecipe[75]={recipe_id=2718,recipe_group=1,recipe_level=1,recipe_type=11}---兽王印记
		staticrecipe[76]={recipe_id=2736,recipe_group=1,recipe_level=1,recipe_type=11}---大地芳华
		staticrecipe[77]={recipe_id=2375,recipe_group=1,recipe_level=1,recipe_type=11}---夺命铁手
		staticrecipe[78]={recipe_id=2381,recipe_group=1,recipe_level=1,recipe_type=11}---土曜之夺命铁手
		staticrecipe[79]={recipe_id=2376,recipe_group=1,recipe_level=1,recipe_type=11}---勾魂铁手
		staticrecipe[80]={recipe_id=2382,recipe_group=1,recipe_level=1,recipe_type=11}---土曜之勾魂铁手
		staticrecipe[81]={recipe_id=2477,recipe_group=1,recipe_level=2,recipe_type=11}---★★土曜之夺命铁手
		staticrecipe[82]={recipe_id=2478,recipe_group=1,recipe_level=2,recipe_type=11}---★★土曜之勾魂铁手
		staticrecipe[83]={recipe_id=2471,recipe_group=1,recipe_level=2,recipe_type=11}---★土曜之夺命铁手
		staticrecipe[84]={recipe_id=2472,recipe_group=1,recipe_level=2,recipe_type=11}---★土曜之勾魂铁手
		staticrecipe[85]={recipe_id=2387,recipe_group=1,recipe_level=1,recipe_type=11}---麒麟战衣
		staticrecipe[86]={recipe_id=2393,recipe_group=1,recipe_level=1,recipe_type=11}---月曜之麒麟战衣
		staticrecipe[87]={recipe_id=2388,recipe_group=1,recipe_level=1,recipe_type=11}---龙筋战衣
		staticrecipe[88]={recipe_id=2394,recipe_group=1,recipe_level=1,recipe_type=11}---月曜之龙筋战衣
		staticrecipe[89]={recipe_id=2489,recipe_group=1,recipe_level=2,recipe_type=11}---★★月曜之麒麟战衣
		staticrecipe[90]={recipe_id=2490,recipe_group=1,recipe_level=2,recipe_type=11}---★★月曜之龙筋战衣
		staticrecipe[91]={recipe_id=2483,recipe_group=1,recipe_level=2,recipe_type=11}---★月曜之麒麟战衣
		staticrecipe[92]={recipe_id=2484,recipe_group=1,recipe_level=2,recipe_type=11}---★月曜之龙筋战衣
		staticrecipe[93]={recipe_id=2435,recipe_group=1,recipe_level=1,recipe_type=11}---计都之通神武念
		staticrecipe[94]={recipe_id=2429,recipe_group=1,recipe_level=1,recipe_type=11}---通神武念
		staticrecipe[95]={recipe_id=2436,recipe_group=1,recipe_level=1,recipe_type=11}---计都之无上武念
		staticrecipe[96]={recipe_id=2430,recipe_group=1,recipe_level=1,recipe_type=11}---无上武念
		staticrecipe[97]={recipe_id=2549,recipe_group=1,recipe_level=2,recipe_type=11}---★★计都之通神武念
		staticrecipe[98]={recipe_id=2550,recipe_group=1,recipe_level=2,recipe_type=11}---★★计都之无上武念
		staticrecipe[99]={recipe_id=2543,recipe_group=1,recipe_level=2,recipe_type=11}---★计都之通神武念
		staticrecipe[100]={recipe_id=2544,recipe_group=1,recipe_level=2,recipe_type=11}---★计都之无上武念
		staticrecipe[101]={recipe_id=2579,recipe_group=1,recipe_level=1,recipe_type=11}---琼林之恕
		staticrecipe[102]={recipe_id=2591,recipe_group=1,recipe_level=1,recipe_type=11}---幻生之妙
		staticrecipe[103]={recipe_id=2567,recipe_group=1,recipe_level=1,recipe_type=11}---倾国之幻
		staticrecipe[104]={recipe_id=2555,recipe_group=1,recipe_level=1,recipe_type=11}---三清宝相
		staticrecipe[105]={recipe_id=2585,recipe_group=1,recipe_level=1,recipe_type=11}---神龟之宁
		staticrecipe[106]={recipe_id=2597,recipe_group=1,recipe_level=1,recipe_type=11}---无常之苦
		staticrecipe[107]={recipe_id=2573,recipe_group=1,recipe_level=1,recipe_type=11}---瑶池之芳
		staticrecipe[108]={recipe_id=2561,recipe_group=1,recipe_level=1,recipe_type=11}---鲲鹏之志
		staticrecipe[109]={recipe_id=2580,recipe_group=1,recipe_level=1,recipe_type=11}---神木之露
		staticrecipe[110]={recipe_id=2592,recipe_group=1,recipe_level=1,recipe_type=11}---浩然之气
		staticrecipe[111]={recipe_id=2568,recipe_group=1,recipe_level=1,recipe_type=11}---无双之叹
		staticrecipe[112]={recipe_id=2556,recipe_group=1,recipe_level=1,recipe_type=11}---关圣金尊
		staticrecipe[113]={recipe_id=2586,recipe_group=1,recipe_level=1,recipe_type=11}---龙子之念
		staticrecipe[114]={recipe_id=2598,recipe_group=1,recipe_level=1,recipe_type=11}---冥君之秘
		staticrecipe[115]={recipe_id=2574,recipe_group=1,recipe_level=1,recipe_type=11}---天宫之谕
		staticrecipe[116]={recipe_id=2562,recipe_group=1,recipe_level=1,recipe_type=11}---鸣凤天心
		staticrecipe[117]={recipe_id=2609,recipe_group=1,recipe_level=1,recipe_type=11}---百鬼赤面
		staticrecipe[118]={recipe_id=2639,recipe_group=1,recipe_level=1,recipe_type=11}---幻生翡翠
		staticrecipe[119]={recipe_id=2603,recipe_group=1,recipe_level=1,recipe_type=11}---皇族玺绶
		staticrecipe[120]={recipe_id=2645,recipe_group=1,recipe_level=1,recipe_type=11}---无常鬼令
		staticrecipe[121]={recipe_id=2621,recipe_group=1,recipe_level=1,recipe_type=11}---仙将令符
		staticrecipe[122]={recipe_id=2627,recipe_group=1,recipe_level=1,recipe_type=11}---瑶池仙谱
		staticrecipe[123]={recipe_id=2633,recipe_group=1,recipe_level=1,recipe_type=11}---囚牛绘文
		staticrecipe[124]={recipe_id=2615,recipe_group=1,recipe_level=1,recipe_type=11}---鲲鹏巨翼
		staticrecipe[125]={recipe_id=2610,recipe_group=1,recipe_level=1,recipe_type=11}---鬼王魔血
		staticrecipe[126]={recipe_id=2640,recipe_group=1,recipe_level=1,recipe_type=11}---浩然古玉
		staticrecipe[127]={recipe_id=2604,recipe_group=1,recipe_level=1,recipe_type=11}---传国金镶玉
		staticrecipe[128]={recipe_id=2646,recipe_group=1,recipe_level=1,recipe_type=11}---冥君浮绘
		staticrecipe[129]={recipe_id=2622,recipe_group=1,recipe_level=1,recipe_type=11}---元帅天印
		staticrecipe[130]={recipe_id=2628,recipe_group=1,recipe_level=1,recipe_type=11}---天宫帝敕
		staticrecipe[131]={recipe_id=2634,recipe_group=1,recipe_level=1,recipe_type=11}---孽龙血印
		staticrecipe[132]={recipe_id=2616,recipe_group=1,recipe_level=1,recipe_type=11}---鸣凤天羽
		staticrecipe[133]={recipe_id=2453,recipe_group=1,recipe_level=1,recipe_type=11}---栖凤玉坠
		staticrecipe[134]={recipe_id=2454,recipe_group=1,recipe_level=1,recipe_type=11}---龙纹玉坠
		staticrecipe[135]={recipe_id=2919,recipe_group=0,recipe_level=2,recipe_type=5}---道逾护符<冰心玉阙><气贯长虹>配方
		staticrecipe[136]={recipe_id=2920,recipe_group=0,recipe_level=2,recipe_type=5}---道逾护符<护体罡气><返朴归真>配方
		staticrecipe[137]={recipe_id=2923,recipe_group=0,recipe_level=2,recipe_type=5}---道逾护符<狂雷天怒><五雷交轰>配方
		staticrecipe[138]={recipe_id=2922,recipe_group=0,recipe_level=2,recipe_type=5}---道逾护符<炼神还虚><盛冰之寒>配方
		staticrecipe[139]={recipe_id=2921,recipe_group=0,recipe_level=2,recipe_type=5}---道逾护符<破魔秘法><破魔怒意>配方
		staticrecipe[140]={recipe_id=2925,recipe_group=0,recipe_level=2,recipe_type=5}---道逾护符<天眷有道><玄天星芒>配方
		staticrecipe[141]={recipe_id=2926,recipe_group=0,recipe_level=2,recipe_type=5}---道逾护符<天雷狂啸><群雷裂空>配方
		staticrecipe[142]={recipe_id=2927,recipe_group=0,recipe_level=2,recipe_type=5}---道逾护符<天诛真诀><斩龙奥义>配方
		staticrecipe[143]={recipe_id=2924,recipe_group=0,recipe_level=2,recipe_type=5}---道逾护符<玄冰圣护><冰域天机>配方
		staticrecipe[144]={recipe_id=1743,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）冰心玉阙配方
		staticrecipe[145]={recipe_id=1761,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）冰域天机配方
		staticrecipe[146]={recipe_id=1759,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）返朴归真配方
		staticrecipe[147]={recipe_id=1776,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）焚心厉雷配方
		staticrecipe[148]={recipe_id=1747,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）风剑之芒配方
		staticrecipe[149]={recipe_id=1771,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）风雷之怨配方
		staticrecipe[150]={recipe_id=1748,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）归元真解配方
		staticrecipe[151]={recipe_id=1736,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）寒冰烈炎配方
		staticrecipe[152]={recipe_id=1741,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）护体罡气配方
		staticrecipe[153]={recipe_id=1770,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）华闪之歌配方
		staticrecipe[154]={recipe_id=1772,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）剑魔宿命配方
		staticrecipe[155]={recipe_id=1775,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）剑心彻悟配方
		staticrecipe[156]={recipe_id=1755,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）剑元之聚配方
		staticrecipe[157]={recipe_id=1773,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）绝圣弃智配方
		staticrecipe[158]={recipe_id=1760,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）狂雷天怒配方
		staticrecipe[159]={recipe_id=1746,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）雷神降世配方
		staticrecipe[160]={recipe_id=1764,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）雷云奇变配方
		staticrecipe[161]={recipe_id=1742,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）炼神还虚配方
		staticrecipe[162]={recipe_id=1750,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）怒剑心劫配方
		staticrecipe[163]={recipe_id=1738,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）破魔秘法配方
		staticrecipe[164]={recipe_id=1749,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）破魔怒意配方
		staticrecipe[165]={recipe_id=1756,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）气贯长虹配方
		staticrecipe[166]={recipe_id=1740,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）驱物法诀配方
		staticrecipe[167]={recipe_id=1767,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）群雷裂空配方
		staticrecipe[168]={recipe_id=1737,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）少阳初现配方
		staticrecipe[169]={recipe_id=1762,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）盛冰之寒配方
		staticrecipe[170]={recipe_id=1763,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）霜寒化境配方
		staticrecipe[171]={recipe_id=1768,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）天地无用配方
		staticrecipe[172]={recipe_id=1769,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）天剑随心配方
		staticrecipe[173]={recipe_id=1739,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）天眷有道配方
		staticrecipe[174]={recipe_id=1766,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）天雷狂啸配方
		staticrecipe[175]={recipe_id=1752,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）天诛真诀配方
		staticrecipe[176]={recipe_id=1765,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）五雷交轰配方
		staticrecipe[177]={recipe_id=1744,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）仙元护体配方
		staticrecipe[178]={recipe_id=1745,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）先天罡气配方
		staticrecipe[179]={recipe_id=1751,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）邪剑星羽配方
		staticrecipe[180]={recipe_id=1758,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）玄冰劲气配方
		staticrecipe[181]={recipe_id=1757,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）玄冰圣护配方
		staticrecipe[182]={recipe_id=1774,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）玄刹天威配方
		staticrecipe[183]={recipe_id=1754,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）玄天星芒配方
		staticrecipe[184]={recipe_id=1753,recipe_group=0,recipe_level=1,recipe_type=5}---（道逾）斩龙奥义配方
		staticrecipe[185]={recipe_id=2940,recipe_group=0,recipe_level=2,recipe_type=5}---佛偈护符<慈悲为怀><佛恩广被>配方
		staticrecipe[186]={recipe_id=2938,recipe_group=0,recipe_level=2,recipe_type=5}---佛偈护符<度尽劫波><无尽苦海>配方
		staticrecipe[187]={recipe_id=2944,recipe_group=0,recipe_level=2,recipe_type=5}---佛偈护符<法螺常念><驭物神通>配方
		staticrecipe[188]={recipe_id=2943,recipe_group=0,recipe_level=2,recipe_type=5}---佛偈护符<光明之颂><净世佛心>配方
		staticrecipe[189]={recipe_id=2942,recipe_group=0,recipe_level=2,recipe_type=5}---佛偈护符<袈裟蔽天><伏魔真诀>配方
		staticrecipe[190]={recipe_id=2945,recipe_group=0,recipe_level=2,recipe_type=5}---佛偈护符<净世佛心><万物通达>配方
		staticrecipe[191]={recipe_id=2941,recipe_group=0,recipe_level=2,recipe_type=5}---佛偈护符<九转之易><金刚不坏>配方
		staticrecipe[192]={recipe_id=2939,recipe_group=0,recipe_level=2,recipe_type=5}---佛偈护符<空即是色><色即是空>配方
		staticrecipe[193]={recipe_id=2937,recipe_group=0,recipe_level=2,recipe_type=5}---佛偈护符<天花乱坠><束心法咒>配方
		staticrecipe[194]={recipe_id=1808,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）慈悲为怀配方
		staticrecipe[195]={recipe_id=1789,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）度尽劫波配方
		staticrecipe[196]={recipe_id=1792,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）顿悟之偈配方
		staticrecipe[197]={recipe_id=1786,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）法螺常念配方
		staticrecipe[198]={recipe_id=1782,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）梵经心语配方
		staticrecipe[199]={recipe_id=1817,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）佛恩广被配方
		staticrecipe[200]={recipe_id=1816,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）佛尊法印配方
		staticrecipe[201]={recipe_id=1804,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）伏魔真诀配方
		staticrecipe[202]={recipe_id=1783,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）固本培元配方
		staticrecipe[203]={recipe_id=1787,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）光明之颂配方
		staticrecipe[204]={recipe_id=1805,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）极乐禅言配方
		staticrecipe[205]={recipe_id=1797,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）极乐颂泽配方
		staticrecipe[206]={recipe_id=1779,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）袈裟蔽天配方
		staticrecipe[207]={recipe_id=1780,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）降魔真言配方
		staticrecipe[208]={recipe_id=1814,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）金刚不坏配方
		staticrecipe[209]={recipe_id=1803,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）金刚破魔配方
		staticrecipe[210]={recipe_id=1802,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）净世佛心配方
		staticrecipe[211]={recipe_id=1807,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）九转之易配方
		staticrecipe[212]={recipe_id=1778,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）空即是色配方
		staticrecipe[213]={recipe_id=1791,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）空冥禅念配方
		staticrecipe[214]={recipe_id=1796,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）了世尘缘配方
		staticrecipe[215]={recipe_id=1798,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）灵魂绽放配方
		staticrecipe[216]={recipe_id=1801,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）乾坤正气配方
		staticrecipe[217]={recipe_id=1790,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）色即是空配方
		staticrecipe[218]={recipe_id=1809,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）神魔妄念配方
		staticrecipe[219]={recipe_id=1781,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）束心法咒配方
		staticrecipe[220]={recipe_id=1811,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）醍醐灌顶配方
		staticrecipe[221]={recipe_id=1793,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）天道循环配方
		staticrecipe[222]={recipe_id=1815,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）天鼓雷音配方
		staticrecipe[223]={recipe_id=1777,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）天花乱坠配方
		staticrecipe[224]={recipe_id=1810,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）天舞宝轮配方
		staticrecipe[225]={recipe_id=1813,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）万物通达配方
		staticrecipe[226]={recipe_id=1795,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）往生夙愿配方
		staticrecipe[227]={recipe_id=1806,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）无常轮渡配方
		staticrecipe[228]={recipe_id=1799,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）无尽苦海配方
		staticrecipe[229]={recipe_id=1800,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）相续不空配方
		staticrecipe[230]={recipe_id=1812,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）邪魔退散配方
		staticrecipe[231]={recipe_id=1785,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）悬壶济世配方
		staticrecipe[232]={recipe_id=1788,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）一语道破配方
		staticrecipe[233]={recipe_id=1794,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）驭物神通配方
		staticrecipe[234]={recipe_id=1784,recipe_group=0,recipe_level=1,recipe_type=5}---（佛偈）再造祷言配方
		staticrecipe[235]={recipe_id=2768,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）黯然咒配方
		staticrecipe[236]={recipe_id=2777,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）百毒炼配方
		staticrecipe[237]={recipe_id=2787,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）百鬼抄配方
		staticrecipe[238]={recipe_id=2788,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）蚩尤怒配方
		staticrecipe[239]={recipe_id=2781,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）挫骨印配方
		staticrecipe[240]={recipe_id=2775,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）地藏心配方
		staticrecipe[241]={recipe_id=2770,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）地灵咒配方
		staticrecipe[242]={recipe_id=2776,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）奠魂曲配方
		staticrecipe[243]={recipe_id=2774,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）定魂诀配方
		staticrecipe[244]={recipe_id=2797,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）毒鳞刺配方
		staticrecipe[245]={recipe_id=2762,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）恶鬼牙配方
		staticrecipe[246]={recipe_id=2798,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）附蛊蛭配方
		staticrecipe[247]={recipe_id=2765,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）攻心咒配方
		staticrecipe[248]={recipe_id=2801,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）蛊乾坤配方
		staticrecipe[249]={recipe_id=2792,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）鬼翼通配方
		staticrecipe[250]={recipe_id=2780,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）鬼阴风配方
		staticrecipe[251]={recipe_id=2764,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）化骨毒配方
		staticrecipe[252]={recipe_id=2783,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）回魂祭配方
		staticrecipe[253]={recipe_id=2789,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）截宗秘配方
		staticrecipe[254]={recipe_id=2763,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）沥血心配方
		staticrecipe[255]={recipe_id=2796,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）龙筋蛊配方
		staticrecipe[256]={recipe_id=2766,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）乱心诀配方
		staticrecipe[257]={recipe_id=2785,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）罗生门配方
		staticrecipe[258]={recipe_id=2773,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）落魂咒配方
		staticrecipe[259]={recipe_id=2784,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）猛鬼啸配方
		staticrecipe[260]={recipe_id=2769,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）幕天毒配方
		staticrecipe[261]={recipe_id=2772,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）孽龙毒配方
		staticrecipe[262]={recipe_id=2794,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）凝血咒配方
		staticrecipe[263]={recipe_id=2778,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）怒灵噬配方
		staticrecipe[264]={recipe_id=2771,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）生死道配方
		staticrecipe[265]={recipe_id=2795,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）噬魂蛊配方
		staticrecipe[266]={recipe_id=2793,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）饲五毒配方
		staticrecipe[267]={recipe_id=2800,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）碎魂蛊配方
		staticrecipe[268]={recipe_id=2802,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）天劫蛊配方
		staticrecipe[269]={recipe_id=2779,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）天劫火配方
		staticrecipe[270]={recipe_id=2767,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）无影毒配方
		staticrecipe[271]={recipe_id=2790,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）邪灵蛊配方
		staticrecipe[272]={recipe_id=2786,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）邪血神配方
		staticrecipe[273]={recipe_id=2799,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）血海经配方
		staticrecipe[274]={recipe_id=2791,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）应声蛊配方
		staticrecipe[275]={recipe_id=2782,recipe_group=0,recipe_level=1,recipe_type=5}---（凝蛊）真火咒配方
		staticrecipe[276]={recipe_id=2933,recipe_group=0,recipe_level=2,recipe_type=5}---凝蛊护符<挫骨印><猛鬼啸>配方
		staticrecipe[277]={recipe_id=2934,recipe_group=0,recipe_level=2,recipe_type=5}---凝蛊护符<毒鳞刺><碎魂蛊>配方
		staticrecipe[278]={recipe_id=2929,recipe_group=0,recipe_level=2,recipe_type=5}---凝蛊护符<沥血心><奠魂曲>配方
		staticrecipe[279]={recipe_id=2930,recipe_group=0,recipe_level=2,recipe_type=5}---凝蛊护符<沥血心><龙筋蛊>配方
		staticrecipe[280]={recipe_id=2936,recipe_group=0,recipe_level=2,recipe_type=5}---凝蛊护符<幕天毒><地灵咒>配方
		staticrecipe[281]={recipe_id=2935,recipe_group=0,recipe_level=2,recipe_type=5}---凝蛊护符<噬魂蛊><附蛊蛭>配方
		staticrecipe[282]={recipe_id=2931,recipe_group=0,recipe_level=2,recipe_type=5}---凝蛊护符<饲五毒><凝血咒>配方
		staticrecipe[283]={recipe_id=2932,recipe_group=0,recipe_level=2,recipe_type=5}---凝蛊护符<天劫火><真火咒>配方
		staticrecipe[284]={recipe_id=2928,recipe_group=0,recipe_level=2,recipe_type=5}---凝蛊护符<邪灵蛊><应声蛊>配方
		staticrecipe[285]={recipe_id=1847,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）黯然配方
		staticrecipe[286]={recipe_id=1853,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）不悔配方
		staticrecipe[287]={recipe_id=1827,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）不逾配方
		staticrecipe[288]={recipe_id=1829,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）残香配方
		staticrecipe[289]={recipe_id=1821,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）荡漾配方
		staticrecipe[290]={recipe_id=1844,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）悱恻配方
		staticrecipe[291]={recipe_id=1843,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）风物配方
		staticrecipe[292]={recipe_id=1826,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）风月配方
		staticrecipe[293]={recipe_id=1841,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）花间配方
		staticrecipe[294]={recipe_id=1845,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）化羽配方
		staticrecipe[295]={recipe_id=1824,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）幻境配方
		staticrecipe[296]={recipe_id=1854,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）幻情配方
		staticrecipe[297]={recipe_id=1822,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）击天配方
		staticrecipe[298]={recipe_id=1842,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）惊鸿配方
		staticrecipe[299]={recipe_id=1828,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）绝尘配方
		staticrecipe[300]={recipe_id=1849,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）绝情配方
		staticrecipe[301]={recipe_id=1852,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）冷血配方
		staticrecipe[302]={recipe_id=1820,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）魅心配方
		staticrecipe[303]={recipe_id=1825,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）梦乡配方
		staticrecipe[304]={recipe_id=1834,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）迷梦配方
		staticrecipe[305]={recipe_id=1840,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）迷失配方
		staticrecipe[306]={recipe_id=1830,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）翩飞配方
		staticrecipe[307]={recipe_id=1851,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）情癫配方
		staticrecipe[308]={recipe_id=1823,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）情动配方
		staticrecipe[309]={recipe_id=1836,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）情惑配方
		staticrecipe[310]={recipe_id=1856,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）情切配方
		staticrecipe[311]={recipe_id=1857,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）情贞配方
		staticrecipe[312]={recipe_id=1858,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）情致配方
		staticrecipe[313]={recipe_id=1819,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）识破配方
		staticrecipe[314]={recipe_id=1839,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）思恋配方
		staticrecipe[315]={recipe_id=1850,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）天籁配方
		staticrecipe[316]={recipe_id=1846,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）天伤配方
		staticrecipe[317]={recipe_id=1818,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）相济配方
		staticrecipe[318]={recipe_id=1837,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）心门配方
		staticrecipe[319]={recipe_id=1835,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）心愿配方
		staticrecipe[320]={recipe_id=1832,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）星月配方
		staticrecipe[321]={recipe_id=1838,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）亦尘配方
		staticrecipe[322]={recipe_id=1833,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）幽兰配方
		staticrecipe[323]={recipe_id=1848,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）驭宝配方
		staticrecipe[324]={recipe_id=1831,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）约定配方
		staticrecipe[325]={recipe_id=1855,recipe_group=0,recipe_level=1,recipe_type=5}---（天韵）绽放配方
		staticrecipe[326]={recipe_id=2949,recipe_group=0,recipe_level=2,recipe_type=5}---天韵护符<荡漾><迷梦>配方
		staticrecipe[327]={recipe_id=2952,recipe_group=0,recipe_level=2,recipe_type=5}---天韵护符<幻境><残香>配方
		staticrecipe[328]={recipe_id=2951,recipe_group=0,recipe_level=2,recipe_type=5}---天韵护符<击天><冷血>配方
		staticrecipe[329]={recipe_id=2953,recipe_group=0,recipe_level=2,recipe_type=5}---天韵护符<绝尘><星月>配方
		staticrecipe[330]={recipe_id=2948,recipe_group=0,recipe_level=2,recipe_type=5}---天韵护符<魅心><风月>配方
		staticrecipe[331]={recipe_id=2954,recipe_group=0,recipe_level=2,recipe_type=5}---天韵护符<翩飞><迷失>配方
		staticrecipe[332]={recipe_id=2950,recipe_group=0,recipe_level=2,recipe_type=5}---天韵护符<情动><黯然>配方
		staticrecipe[333]={recipe_id=2946,recipe_group=0,recipe_level=2,recipe_type=5}---天韵护符<心门><亦尘>配方
		staticrecipe[334]={recipe_id=2947,recipe_group=0,recipe_level=2,recipe_type=5}---天韵护符<心愿><绽放>配方
		staticrecipe[335]={recipe_id=1886,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）冰锋配方
		staticrecipe[336]={recipe_id=1881,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）冰寒配方
		staticrecipe[337]={recipe_id=1887,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）沉舟配方
		staticrecipe[338]={recipe_id=1870,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）炽热配方
		staticrecipe[339]={recipe_id=1884,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）孤注配方
		staticrecipe[340]={recipe_id=1860,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）火舞配方
		staticrecipe[341]={recipe_id=1869,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）亢龙配方
		staticrecipe[342]={recipe_id=1859,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）狂魔配方
		staticrecipe[343]={recipe_id=1866,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）狂怒配方
		staticrecipe[344]={recipe_id=1895,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）狂性配方
		staticrecipe[345]={recipe_id=1865,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）雷鸣配方
		staticrecipe[346]={recipe_id=1896,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）雷牙配方
		staticrecipe[347]={recipe_id=1861,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）戾气配方
		staticrecipe[348]={recipe_id=1899,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）灵咒配方
		staticrecipe[349]={recipe_id=1892,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）乱舞配方
		staticrecipe[350]={recipe_id=1883,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）梦魇配方
		staticrecipe[351]={recipe_id=1867,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）灭神配方
		staticrecipe[352]={recipe_id=1893,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）魔道配方
		staticrecipe[353]={recipe_id=1898,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）魔甲配方
		staticrecipe[354]={recipe_id=1897,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）魔刃配方
		staticrecipe[355]={recipe_id=1888,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）魔煞配方
		staticrecipe[356]={recipe_id=1875,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）魔心配方
		staticrecipe[357]={recipe_id=1885,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）末日配方
		staticrecipe[358]={recipe_id=1891,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）怒龙配方
		staticrecipe[359]={recipe_id=1863,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）破魂配方
		staticrecipe[360]={recipe_id=1889,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）破空配方
		staticrecipe[361]={recipe_id=1879,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）驱物配方
		staticrecipe[362]={recipe_id=1882,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）噬血配方
		staticrecipe[363]={recipe_id=1868,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）天谴配方
		staticrecipe[364]={recipe_id=1874,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）天煞配方
		staticrecipe[365]={recipe_id=1877,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）天佑配方
		staticrecipe[366]={recipe_id=1880,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）妄念配方
		staticrecipe[367]={recipe_id=1894,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）惟我配方
		staticrecipe[368]={recipe_id=1864,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）无间配方
		staticrecipe[369]={recipe_id=1872,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）信念配方
		staticrecipe[370]={recipe_id=1890,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）虚无配方
		staticrecipe[371]={recipe_id=1878,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）血绛配方
		staticrecipe[372]={recipe_id=1871,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）业火配方
		staticrecipe[373]={recipe_id=1873,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）战意配方
		staticrecipe[374]={recipe_id=1862,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）震慑配方
		staticrecipe[375]={recipe_id=1876,recipe_group=0,recipe_level=1,recipe_type=5}---（血咒）咒怨配方
		staticrecipe[376]={recipe_id=2957,recipe_group=0,recipe_level=2,recipe_type=5}---血咒护符<冰寒><冰锋>配方
		staticrecipe[377]={recipe_id=2955,recipe_group=0,recipe_level=2,recipe_type=5}---血咒护符<火舞><炽热>配方
		staticrecipe[378]={recipe_id=2960,recipe_group=0,recipe_level=2,recipe_type=5}---血咒护符<亢龙><怒龙>配方
		staticrecipe[379]={recipe_id=2956,recipe_group=0,recipe_level=2,recipe_type=5}---血咒护符<戾气><震慑>配方
		staticrecipe[380]={recipe_id=2961,recipe_group=0,recipe_level=2,recipe_type=5}---血咒护符<魔心><妄念>配方
		staticrecipe[381]={recipe_id=2959,recipe_group=0,recipe_level=2,recipe_type=5}---血咒护符<噬血><梦魇>配方
		staticrecipe[382]={recipe_id=2958,recipe_group=0,recipe_level=2,recipe_type=5}---血咒护符<天谴><天煞>配方
		staticrecipe[383]={recipe_id=2962,recipe_group=0,recipe_level=2,recipe_type=5}---血咒护符<无间><末日>配方
		staticrecipe[384]={recipe_id=2963,recipe_group=0,recipe_level=2,recipe_type=5}---血咒护符<信念><天佑>配方
		staticrecipe[385]={recipe_id=3040,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·二计配方
		staticrecipe[386]={recipe_id=3041,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·三计配方
		staticrecipe[387]={recipe_id=3042,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·四计配方
		staticrecipe[388]={recipe_id=3043,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·五计配方
		staticrecipe[389]={recipe_id=3044,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·六计配方
		staticrecipe[390]={recipe_id=3045,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·七计配方
		staticrecipe[391]={recipe_id=3046,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·八计配方
		staticrecipe[392]={recipe_id=3047,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·九计配方
		staticrecipe[393]={recipe_id=3048,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·十计配方
		staticrecipe[394]={recipe_id=3049,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·十一计配方
		staticrecipe[395]={recipe_id=3050,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·十二计配方
		staticrecipe[396]={recipe_id=3051,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·十三计配方
		staticrecipe[397]={recipe_id=3052,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·十四计配方
		staticrecipe[398]={recipe_id=3053,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·十五计配方
		staticrecipe[399]={recipe_id=3054,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·十六计配方
		staticrecipe[400]={recipe_id=3055,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·十七计配方
		staticrecipe[401]={recipe_id=3056,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·十八计配方
		staticrecipe[402]={recipe_id=3057,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·十九计配方
		staticrecipe[403]={recipe_id=3058,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·二十计配方
		staticrecipe[404]={recipe_id=3059,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·二十一计配方
		staticrecipe[405]={recipe_id=3060,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·二十二计配方
		staticrecipe[406]={recipe_id=3061,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·二十三计配方
		staticrecipe[407]={recipe_id=3062,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·二十四计配方
		staticrecipe[408]={recipe_id=3063,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·二十五计配方
		staticrecipe[409]={recipe_id=3064,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·二十六计配方
		staticrecipe[410]={recipe_id=3065,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·二十七计配方
		staticrecipe[411]={recipe_id=3066,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·二十八计配方
		staticrecipe[412]={recipe_id=3067,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·二十九计配方
		staticrecipe[413]={recipe_id=3068,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·三十计配方
		staticrecipe[414]={recipe_id=3069,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·三十一计配方
		staticrecipe[415]={recipe_id=3070,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·三十二计配方
		staticrecipe[416]={recipe_id=3071,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·三十三计配方
		staticrecipe[417]={recipe_id=3072,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·三十四计配方
		staticrecipe[418]={recipe_id=3073,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·三十五计配方
		staticrecipe[419]={recipe_id=3074,recipe_group=0,recipe_level=1,recipe_type=5}---神行锦囊·三十六计配方
		staticrecipe[420]={recipe_id=3075,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·二计配方
		staticrecipe[421]={recipe_id=3076,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·三计配方
		staticrecipe[422]={recipe_id=3077,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·四计配方
		staticrecipe[423]={recipe_id=3078,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·五计配方
		staticrecipe[424]={recipe_id=3079,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·六计配方
		staticrecipe[425]={recipe_id=3080,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·七计配方
		staticrecipe[426]={recipe_id=3081,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·八计配方
		staticrecipe[427]={recipe_id=3082,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·九计配方
		staticrecipe[428]={recipe_id=3083,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·十计配方
		staticrecipe[429]={recipe_id=3084,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·十一计配方
		staticrecipe[430]={recipe_id=3085,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·十二计配方
		staticrecipe[431]={recipe_id=3086,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·十三计配方
		staticrecipe[432]={recipe_id=3087,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·十四计配方
		staticrecipe[433]={recipe_id=3088,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·十五计配方
		staticrecipe[434]={recipe_id=3089,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·十六计配方
		staticrecipe[435]={recipe_id=3090,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·十七计配方
		staticrecipe[436]={recipe_id=3091,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·十八计配方
		staticrecipe[437]={recipe_id=3092,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·十九计配方
		staticrecipe[438]={recipe_id=3093,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·二十计配方
		staticrecipe[439]={recipe_id=3094,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·二十一计配方
		staticrecipe[440]={recipe_id=3095,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·二十二计配方
		staticrecipe[441]={recipe_id=3096,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·二十三计配方
		staticrecipe[442]={recipe_id=3097,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·二十四计配方
		staticrecipe[443]={recipe_id=3098,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·二十五计配方
		staticrecipe[444]={recipe_id=3099,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·二十六计配方
		staticrecipe[445]={recipe_id=3100,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·二十七计配方
		staticrecipe[446]={recipe_id=3101,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·二十八计配方
		staticrecipe[447]={recipe_id=3102,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·二十九计配方
		staticrecipe[448]={recipe_id=3103,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·三十计配方
		staticrecipe[449]={recipe_id=3104,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·三十一计配方
		staticrecipe[450]={recipe_id=3105,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·三十二计配方
		staticrecipe[451]={recipe_id=3106,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·三十三计配方
		staticrecipe[452]={recipe_id=3107,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·三十四计配方
		staticrecipe[453]={recipe_id=3108,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·三十五计配方
		staticrecipe[454]={recipe_id=3109,recipe_group=0,recipe_level=1,recipe_type=5}---太虚锦囊·三十六计配方
		staticrecipe[455]={recipe_id=3005,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·二计配方
		staticrecipe[456]={recipe_id=3006,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·三计配方
		staticrecipe[457]={recipe_id=3007,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·四计配方
		staticrecipe[458]={recipe_id=3008,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·五计配方
		staticrecipe[459]={recipe_id=3009,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·六计配方
		staticrecipe[460]={recipe_id=3010,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·七计配方
		staticrecipe[461]={recipe_id=3011,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·八计配方
		staticrecipe[462]={recipe_id=3012,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·九计配方
		staticrecipe[463]={recipe_id=3013,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·十计配方
		staticrecipe[464]={recipe_id=3014,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·十一计配方
		staticrecipe[465]={recipe_id=3015,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·十二计配方
		staticrecipe[466]={recipe_id=3016,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·十三计配方
		staticrecipe[467]={recipe_id=3017,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·十四计配方
		staticrecipe[468]={recipe_id=3018,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·十五计配方
		staticrecipe[469]={recipe_id=3019,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·十六计配方
		staticrecipe[470]={recipe_id=3020,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·十七计配方
		staticrecipe[471]={recipe_id=3021,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·十八计配方
		staticrecipe[472]={recipe_id=3022,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·十九计配方
		staticrecipe[473]={recipe_id=3023,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·二十计配方
		staticrecipe[474]={recipe_id=3024,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·二十一计配方
		staticrecipe[475]={recipe_id=3025,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·二十二计配方
		staticrecipe[476]={recipe_id=3026,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·二十三计配方
		staticrecipe[477]={recipe_id=3027,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·二十四计配方
		staticrecipe[478]={recipe_id=3028,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·二十五计配方
		staticrecipe[479]={recipe_id=3029,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·二十六计配方
		staticrecipe[480]={recipe_id=3030,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·二十七计配方
		staticrecipe[481]={recipe_id=3031,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·二十八计配方
		staticrecipe[482]={recipe_id=3032,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·二十九计配方
		staticrecipe[483]={recipe_id=3033,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·三十计配方
		staticrecipe[484]={recipe_id=3034,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·三十一计配方
		staticrecipe[485]={recipe_id=3035,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·三十二计配方
		staticrecipe[486]={recipe_id=3036,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·三十三计配方
		staticrecipe[487]={recipe_id=3037,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·三十四计配方
		staticrecipe[488]={recipe_id=3038,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·三十五计配方
		staticrecipe[489]={recipe_id=3039,recipe_group=0,recipe_level=1,recipe_type=5}---逍遥锦囊·三十六计配方
		staticrecipe[490]={recipe_id=1939,recipe_group=0,recipe_level=10,recipe_type=3}---[142]地藏盔[男]
		staticrecipe[491]={recipe_id=2005,recipe_group=0,recipe_level=10,recipe_type=3}---[142]地藏盔[女]
		staticrecipe[492]={recipe_id=1937,recipe_group=0,recipe_level=8,recipe_type=3}---[120]冥宁盔[男]
		staticrecipe[493]={recipe_id=2003,recipe_group=0,recipe_level=8,recipe_type=3}---[120]冥宁盔[女]
		staticrecipe[494]={recipe_id=1938,recipe_group=0,recipe_level=9,recipe_type=3}---[135]龙骨盔[男]
		staticrecipe[495]={recipe_id=2004,recipe_group=0,recipe_level=9,recipe_type=3}---[135]龙骨盔[女]
		staticrecipe[496]={recipe_id=1946,recipe_group=0,recipe_level=10,recipe_type=3}---[142]九地冥君明光盔[男]
		staticrecipe[497]={recipe_id=2012,recipe_group=0,recipe_level=10,recipe_type=3}---[142]九地冥君明光盔[女]
		staticrecipe[498]={recipe_id=1944,recipe_group=0,recipe_level=8,recipe_type=3}---[120]九曜定心盔[男]
		staticrecipe[499]={recipe_id=2010,recipe_group=0,recipe_level=8,recipe_type=3}---[120]九曜定心盔[女]
		staticrecipe[500]={recipe_id=1945,recipe_group=0,recipe_level=9,recipe_type=3}---[135]战天狂灵魔神盔[男]
		staticrecipe[501]={recipe_id=2011,recipe_group=0,recipe_level=9,recipe_type=3}---[135]战天狂灵魔神盔[女]
		staticrecipe[502]={recipe_id=1960,recipe_group=0,recipe_level=10,recipe_type=3}---[142]三昧真火金精盔[男]
		staticrecipe[503]={recipe_id=2026,recipe_group=0,recipe_level=10,recipe_type=3}---[142]三昧真火金精盔[女]
		staticrecipe[504]={recipe_id=1958,recipe_group=0,recipe_level=8,recipe_type=3}---[120]三才鬼星盔[男]
		staticrecipe[505]={recipe_id=2024,recipe_group=0,recipe_level=8,recipe_type=3}---[120]三才鬼星盔[女]
		staticrecipe[506]={recipe_id=1959,recipe_group=0,recipe_level=9,recipe_type=3}---[135]荒坟鬼冢磷焰盔[男]
		staticrecipe[507]={recipe_id=2025,recipe_group=0,recipe_level=9,recipe_type=3}---[135]荒坟鬼冢磷焰盔[女]
		staticrecipe[508]={recipe_id=1981,recipe_group=0,recipe_level=10,recipe_type=3}---[142]六道轮回魔心盔[男]
		staticrecipe[509]={recipe_id=2047,recipe_group=0,recipe_level=10,recipe_type=3}---[142]六道轮回魔心盔[女]
		staticrecipe[510]={recipe_id=1979,recipe_group=0,recipe_level=8,recipe_type=3}---[120]六驭鬼烈盔[男]
		staticrecipe[511]={recipe_id=2045,recipe_group=0,recipe_level=8,recipe_type=3}---[120]六驭鬼烈盔[女]
		staticrecipe[512]={recipe_id=1980,recipe_group=0,recipe_level=9,recipe_type=3}---[135]冥河毒蛟洗髓盔[男]
		staticrecipe[513]={recipe_id=2046,recipe_group=0,recipe_level=9,recipe_type=3}---[135]冥河毒蛟洗髓盔[女]
		staticrecipe[514]={recipe_id=1988,recipe_group=0,recipe_level=10,recipe_type=3}---[142]七劫天罪威灵盔[男]
		staticrecipe[515]={recipe_id=2054,recipe_group=0,recipe_level=10,recipe_type=3}---[142]七劫天罪威灵盔[女]
		staticrecipe[516]={recipe_id=1986,recipe_group=0,recipe_level=8,recipe_type=3}---[120]七孔聚魂盔[男]
		staticrecipe[517]={recipe_id=2052,recipe_group=0,recipe_level=8,recipe_type=3}---[120]七孔聚魂盔[女]
		staticrecipe[518]={recipe_id=1987,recipe_group=0,recipe_level=9,recipe_type=3}---[135]血海逐波淀魂盔[男]
		staticrecipe[519]={recipe_id=2053,recipe_group=0,recipe_level=9,recipe_type=3}---[135]血海逐波淀魂盔[女]
		staticrecipe[520]={recipe_id=1967,recipe_group=0,recipe_level=10,recipe_type=3}---[142]天弈元尊冥盔[男]
		staticrecipe[521]={recipe_id=2033,recipe_group=0,recipe_level=10,recipe_type=3}---[142]天弈元尊冥盔[女]
		staticrecipe[522]={recipe_id=1965,recipe_group=0,recipe_level=8,recipe_type=3}---[120]武围神将冥盔[男]
		staticrecipe[523]={recipe_id=2031,recipe_group=0,recipe_level=8,recipe_type=3}---[120]武围神将冥盔[女]
		staticrecipe[524]={recipe_id=1966,recipe_group=0,recipe_level=9,recipe_type=3}---[135]界河真君冥盔[男]
		staticrecipe[525]={recipe_id=2032,recipe_group=0,recipe_level=9,recipe_type=3}---[135]界河真君冥盔[女]
		staticrecipe[526]={recipe_id=1995,recipe_group=0,recipe_level=10,recipe_type=3}---[142]八凶玄火炎魔盔[男]
		staticrecipe[527]={recipe_id=2061,recipe_group=0,recipe_level=10,recipe_type=3}---[142]八凶玄火炎魔盔[女]
		staticrecipe[528]={recipe_id=1993,recipe_group=0,recipe_level=8,recipe_type=3}---[120]八门金锁盔[男]
		staticrecipe[529]={recipe_id=2059,recipe_group=0,recipe_level=8,recipe_type=3}---[120]八门金锁盔[女]
		staticrecipe[530]={recipe_id=1994,recipe_group=0,recipe_level=9,recipe_type=3}---[135]无影蛊毒瘟蝗盔[男]
		staticrecipe[531]={recipe_id=2060,recipe_group=0,recipe_level=9,recipe_type=3}---[135]无影蛊毒瘟蝗盔[女]
		staticrecipe[532]={recipe_id=1974,recipe_group=0,recipe_level=10,recipe_type=3}---[142]五气升腾朝日盔[男]
		staticrecipe[533]={recipe_id=2040,recipe_group=0,recipe_level=10,recipe_type=3}---[142]五气升腾朝日盔[女]
		staticrecipe[534]={recipe_id=1972,recipe_group=0,recipe_level=8,recipe_type=3}---[120]五行绝命盔[男]
		staticrecipe[535]={recipe_id=2038,recipe_group=0,recipe_level=8,recipe_type=3}---[120]五行绝命盔[女]
		staticrecipe[536]={recipe_id=1973,recipe_group=0,recipe_level=9,recipe_type=3}---[135]毒尊避尘金麟盔[男]
		staticrecipe[537]={recipe_id=2039,recipe_group=0,recipe_level=9,recipe_type=3}---[135]毒尊避尘金麟盔[女]
		staticrecipe[538]={recipe_id=1953,recipe_group=0,recipe_level=10,recipe_type=3}---[142]九幽尊主傲神盔[男]
		staticrecipe[539]={recipe_id=2019,recipe_group=0,recipe_level=10,recipe_type=3}---[142]九幽尊主傲神盔[女]
		staticrecipe[540]={recipe_id=1951,recipe_group=0,recipe_level=8,recipe_type=3}---[120]九蛊逆刃盔[男]
		staticrecipe[541]={recipe_id=2017,recipe_group=0,recipe_level=8,recipe_type=3}---[120]九蛊逆刃盔[女]
		staticrecipe[542]={recipe_id=1952,recipe_group=0,recipe_level=9,recipe_type=3}---[135]幽冥转魂天祭盔[男]
		staticrecipe[543]={recipe_id=2018,recipe_group=0,recipe_level=9,recipe_type=3}---[135]幽冥转魂天祭盔[女]
		staticrecipe[544]={recipe_id=251,recipe_group=0,recipe_level=10,recipe_type=3}---[142]皇极盔[男]
		staticrecipe[545]={recipe_id=299,recipe_group=0,recipe_level=10,recipe_type=3}---[142]皇极盔[女]
		staticrecipe[546]={recipe_id=249,recipe_group=0,recipe_level=8,recipe_type=3}---[120]狂神盔[男]
		staticrecipe[547]={recipe_id=297,recipe_group=0,recipe_level=8,recipe_type=3}---[120]狂神盔[女]
		staticrecipe[548]={recipe_id=250,recipe_group=0,recipe_level=9,recipe_type=3}---[135]龙魂盔[男]
		staticrecipe[549]={recipe_id=298,recipe_group=0,recipe_level=9,recipe_type=3}---[135]龙魂盔[女]
		staticrecipe[550]={recipe_id=958,recipe_group=0,recipe_level=10,recipe_type=3}---[142]三界恶灵邪神盔[男]
		staticrecipe[551]={recipe_id=959,recipe_group=0,recipe_level=10,recipe_type=3}---[142]三界恶灵邪神盔[女]
		staticrecipe[552]={recipe_id=962,recipe_group=0,recipe_level=8,recipe_type=3}---[120]三雄沥血盔[男]
		staticrecipe[553]={recipe_id=963,recipe_group=0,recipe_level=8,recipe_type=3}---[120]三雄沥血盔[女]
		staticrecipe[554]={recipe_id=964,recipe_group=0,recipe_level=9,recipe_type=3}---[135]樊龙碎鳞乌金盔[男]
		staticrecipe[555]={recipe_id=965,recipe_group=0,recipe_level=9,recipe_type=3}---[135]樊龙碎鳞乌金盔[女]
		staticrecipe[556]={recipe_id=966,recipe_group=0,recipe_level=10,recipe_type=3}---[142]六合纵横麒麟盔[男]
		staticrecipe[557]={recipe_id=967,recipe_group=0,recipe_level=10,recipe_type=3}---[142]六合纵横麒麟盔[女]
		staticrecipe[558]={recipe_id=970,recipe_group=0,recipe_level=8,recipe_type=3}---[120]六丁破军盔[男]
		staticrecipe[559]={recipe_id=971,recipe_group=0,recipe_level=8,recipe_type=3}---[120]六丁破军盔[女]
		staticrecipe[560]={recipe_id=972,recipe_group=0,recipe_level=9,recipe_type=3}---[135]天狱炼魂凝血盔[男]
		staticrecipe[561]={recipe_id=973,recipe_group=0,recipe_level=9,recipe_type=3}---[135]天狱炼魂凝血盔[女]
		staticrecipe[562]={recipe_id=974,recipe_group=0,recipe_level=10,recipe_type=3}---[142]七捷荡寇明光盔[男]
		staticrecipe[563]={recipe_id=975,recipe_group=0,recipe_level=10,recipe_type=3}---[142]七捷荡寇明光盔[女]
		staticrecipe[564]={recipe_id=978,recipe_group=0,recipe_level=8,recipe_type=3}---[120]七咒乱魂盔[男]
		staticrecipe[565]={recipe_id=979,recipe_group=0,recipe_level=8,recipe_type=3}---[120]七咒乱魂盔[女]
		staticrecipe[566]={recipe_id=980,recipe_group=0,recipe_level=9,recipe_type=3}---[135]狂岚啸天狻猊盔[男]
		staticrecipe[567]={recipe_id=981,recipe_group=0,recipe_level=9,recipe_type=3}---[135]狂岚啸天狻猊盔[女]
		staticrecipe[568]={recipe_id=982,recipe_group=0,recipe_level=10,recipe_type=3}---[142]四海啸傲龙筋盔[男]
		staticrecipe[569]={recipe_id=983,recipe_group=0,recipe_level=10,recipe_type=3}---[142]四海啸傲龙筋盔[女]
		staticrecipe[570]={recipe_id=986,recipe_group=0,recipe_level=8,recipe_type=3}---[120]四夷伏威盔[男]
		staticrecipe[571]={recipe_id=987,recipe_group=0,recipe_level=8,recipe_type=3}---[120]四夷伏威盔[女]
		staticrecipe[572]={recipe_id=988,recipe_group=0,recipe_level=9,recipe_type=3}---[135]兽面吞头连环盔[男]
		staticrecipe[573]={recipe_id=989,recipe_group=0,recipe_level=9,recipe_type=3}---[135]兽面吞头连环盔[女]
		staticrecipe[574]={recipe_id=990,recipe_group=0,recipe_level=10,recipe_type=3}---[142]八义继英掩心盔[男]
		staticrecipe[575]={recipe_id=991,recipe_group=0,recipe_level=10,recipe_type=3}---[142]八义继英掩心盔[女]
		staticrecipe[576]={recipe_id=994,recipe_group=0,recipe_level=8,recipe_type=3}---[120]八凶怒焰盔[男]
		staticrecipe[577]={recipe_id=995,recipe_group=0,recipe_level=8,recipe_type=3}---[120]八凶怒焰盔[女]
		staticrecipe[578]={recipe_id=996,recipe_group=0,recipe_level=9,recipe_type=3}---[135]金玉太岁绽魂盔[男]
		staticrecipe[579]={recipe_id=997,recipe_group=0,recipe_level=9,recipe_type=3}---[135]金玉太岁绽魂盔[女]
		staticrecipe[580]={recipe_id=998,recipe_group=0,recipe_level=10,recipe_type=3}---[142]五煞命延返魂盔[男]
		staticrecipe[581]={recipe_id=999,recipe_group=0,recipe_level=10,recipe_type=3}---[142]五煞命延返魂盔[女]
		staticrecipe[582]={recipe_id=1002,recipe_group=0,recipe_level=8,recipe_type=3}---[120]五狱狂灵盔[男]
		staticrecipe[583]={recipe_id=1003,recipe_group=0,recipe_level=8,recipe_type=3}---[120]五狱狂灵盔[女]
		staticrecipe[584]={recipe_id=1004,recipe_group=0,recipe_level=9,recipe_type=3}---[135]蛮城锦带黄金盔[男]
		staticrecipe[585]={recipe_id=1005,recipe_group=0,recipe_level=9,recipe_type=3}---[135]蛮城锦带黄金盔[女]
		staticrecipe[586]={recipe_id=1006,recipe_group=0,recipe_level=10,recipe_type=3}---[142]九幽酆都鬼灵盔[男]
		staticrecipe[587]={recipe_id=1007,recipe_group=0,recipe_level=10,recipe_type=3}---[142]九幽酆都鬼灵盔[女]
		staticrecipe[588]={recipe_id=1010,recipe_group=0,recipe_level=8,recipe_type=3}---[120]九兽赤鳞盔[男]
		staticrecipe[589]={recipe_id=1011,recipe_group=0,recipe_level=8,recipe_type=3}---[120]九兽赤鳞盔[女]
		staticrecipe[590]={recipe_id=1012,recipe_group=0,recipe_level=9,recipe_type=3}---[135]明尊天佑邪灵盔[男]
		staticrecipe[591]={recipe_id=1013,recipe_group=0,recipe_level=9,recipe_type=3}---[135]明尊天佑邪灵盔[女]
		staticrecipe[592]={recipe_id=950,recipe_group=0,recipe_level=10,recipe_type=3}---[142]九魔枭天冥魂盔[男]
		staticrecipe[593]={recipe_id=951,recipe_group=0,recipe_level=10,recipe_type=3}---[142]九魔枭天冥魂盔[女]
		staticrecipe[594]={recipe_id=954,recipe_group=0,recipe_level=8,recipe_type=3}---[120]九转焚心盔[男]
		staticrecipe[595]={recipe_id=955,recipe_group=0,recipe_level=8,recipe_type=3}---[120]九转焚心盔[女]
		staticrecipe[596]={recipe_id=956,recipe_group=0,recipe_level=9,recipe_type=3}---[135]离魂鬼契克劫盔[男]
		staticrecipe[597]={recipe_id=957,recipe_group=0,recipe_level=9,recipe_type=3}---[135]离魂鬼契克劫盔[女]
		staticrecipe[598]={recipe_id=255,recipe_group=0,recipe_level=10,recipe_type=3}---[142]绝艳盔[男]
		staticrecipe[599]={recipe_id=303,recipe_group=0,recipe_level=10,recipe_type=3}---[142]绝艳盔[女]
		staticrecipe[600]={recipe_id=253,recipe_group=0,recipe_level=8,recipe_type=3}---[120]销魂盔[男]
		staticrecipe[601]={recipe_id=301,recipe_group=0,recipe_level=8,recipe_type=3}---[120]销魂盔[女]
		staticrecipe[602]={recipe_id=254,recipe_group=0,recipe_level=9,recipe_type=3}---[135]惊才盔[男]
		staticrecipe[603]={recipe_id=302,recipe_group=0,recipe_level=9,recipe_type=3}---[135]惊才盔[女]
		staticrecipe[604]={recipe_id=1022,recipe_group=0,recipe_level=10,recipe_type=3}---[142]三弄梅花妙叹盔[男]
		staticrecipe[605]={recipe_id=1023,recipe_group=0,recipe_level=10,recipe_type=3}---[142]三弄梅花妙叹盔[女]
		staticrecipe[606]={recipe_id=1026,recipe_group=0,recipe_level=8,recipe_type=3}---[120]三尾玄狐盔[男]
		staticrecipe[607]={recipe_id=1027,recipe_group=0,recipe_level=8,recipe_type=3}---[120]三尾玄狐盔[女]
		staticrecipe[608]={recipe_id=1028,recipe_group=0,recipe_level=9,recipe_type=3}---[135]西川红锦百花盔[男]
		staticrecipe[609]={recipe_id=1029,recipe_group=0,recipe_level=9,recipe_type=3}---[135]西川红锦百花盔[女]
		staticrecipe[610]={recipe_id=1030,recipe_group=0,recipe_level=10,recipe_type=3}---[142]六叶妖娆金莲盔[男]
		staticrecipe[611]={recipe_id=1031,recipe_group=0,recipe_level=10,recipe_type=3}---[142]六叶妖娆金莲盔[女]
		staticrecipe[612]={recipe_id=1034,recipe_group=0,recipe_level=8,recipe_type=3}---[120]六道荡魂盔[男]
		staticrecipe[613]={recipe_id=1035,recipe_group=0,recipe_level=8,recipe_type=3}---[120]六道荡魂盔[女]
		staticrecipe[614]={recipe_id=1036,recipe_group=0,recipe_level=9,recipe_type=3}---[135]幻彩华光金缕盔[男]
		staticrecipe[615]={recipe_id=1037,recipe_group=0,recipe_level=9,recipe_type=3}---[135]幻彩华光金缕盔[女]
		staticrecipe[616]={recipe_id=1038,recipe_group=0,recipe_level=10,recipe_type=3}---[142]七彩莹华冰心盔[男]
		staticrecipe[617]={recipe_id=1039,recipe_group=0,recipe_level=10,recipe_type=3}---[142]七彩莹华冰心盔[女]
		staticrecipe[618]={recipe_id=1042,recipe_group=0,recipe_level=8,recipe_type=3}---[120]七宝玲珑盔[男]
		staticrecipe[619]={recipe_id=1043,recipe_group=0,recipe_level=8,recipe_type=3}---[120]七宝玲珑盔[女]
		staticrecipe[620]={recipe_id=1044,recipe_group=0,recipe_level=9,recipe_type=3}---[135]落花逐水困情盔[男]
		staticrecipe[621]={recipe_id=1045,recipe_group=0,recipe_level=9,recipe_type=3}---[135]落花逐水困情盔[女]
		staticrecipe[622]={recipe_id=1046,recipe_group=0,recipe_level=10,recipe_type=3}---[142]四野漫歌铭情盔[男]
		staticrecipe[623]={recipe_id=1047,recipe_group=0,recipe_level=10,recipe_type=3}---[142]四野漫歌铭情盔[女]
		staticrecipe[624]={recipe_id=1050,recipe_group=0,recipe_level=8,recipe_type=3}---[120]四圣步空盔[男]
		staticrecipe[625]={recipe_id=1051,recipe_group=0,recipe_level=8,recipe_type=3}---[120]四圣步空盔[女]
		staticrecipe[626]={recipe_id=1052,recipe_group=0,recipe_level=9,recipe_type=3}---[135]落霞弥空霓彩盔[男]
		staticrecipe[627]={recipe_id=1053,recipe_group=0,recipe_level=9,recipe_type=3}---[135]落霞弥空霓彩盔[女]
		staticrecipe[628]={recipe_id=1054,recipe_group=0,recipe_level=10,recipe_type=3}---[142]八宝夜明珍珠盔[男]
		staticrecipe[629]={recipe_id=1055,recipe_group=0,recipe_level=10,recipe_type=3}---[142]八宝夜明珍珠盔[女]
		staticrecipe[630]={recipe_id=1058,recipe_group=0,recipe_level=8,recipe_type=3}---[120]八荒冀云盔[男]
		staticrecipe[631]={recipe_id=1059,recipe_group=0,recipe_level=8,recipe_type=3}---[120]八荒冀云盔[女]
		staticrecipe[632]={recipe_id=1060,recipe_group=0,recipe_level=9,recipe_type=3}---[135]临凡谪仙逍遥盔[男]
		staticrecipe[633]={recipe_id=1061,recipe_group=0,recipe_level=9,recipe_type=3}---[135]临凡谪仙逍遥盔[女]
		staticrecipe[634]={recipe_id=1062,recipe_group=0,recipe_level=10,recipe_type=3}---[142]五岳云岚傲天盔[男]
		staticrecipe[635]={recipe_id=1063,recipe_group=0,recipe_level=10,recipe_type=3}---[142]五岳云岚傲天盔[女]
		staticrecipe[636]={recipe_id=1066,recipe_group=0,recipe_level=8,recipe_type=3}---[120]五色华光盔[男]
		staticrecipe[637]={recipe_id=1067,recipe_group=0,recipe_level=8,recipe_type=3}---[120]五色华光盔[女]
		staticrecipe[638]={recipe_id=1068,recipe_group=0,recipe_level=9,recipe_type=3}---[135]太上忘情炼心盔[男]
		staticrecipe[639]={recipe_id=1069,recipe_group=0,recipe_level=9,recipe_type=3}---[135]太上忘情炼心盔[女]
		staticrecipe[640]={recipe_id=1070,recipe_group=0,recipe_level=10,recipe_type=3}---[142]九幻迷天绛绡盔[男]
		staticrecipe[641]={recipe_id=1071,recipe_group=0,recipe_level=10,recipe_type=3}---[142]九幻迷天绛绡盔[女]
		staticrecipe[642]={recipe_id=1074,recipe_group=0,recipe_level=8,recipe_type=3}---[120]九焕金纱盔[男]
		staticrecipe[643]={recipe_id=1075,recipe_group=0,recipe_level=8,recipe_type=3}---[120]九焕金纱盔[女]
		staticrecipe[644]={recipe_id=1076,recipe_group=0,recipe_level=9,recipe_type=3}---[135]琼台天籁承欢盔[男]
		staticrecipe[645]={recipe_id=1077,recipe_group=0,recipe_level=9,recipe_type=3}---[135]琼台天籁承欢盔[女]
		staticrecipe[646]={recipe_id=1014,recipe_group=0,recipe_level=10,recipe_type=3}---[142]九宵琼台连玉盔[男]
		staticrecipe[647]={recipe_id=1015,recipe_group=0,recipe_level=10,recipe_type=3}---[142]九宵琼台连玉盔[女]
		staticrecipe[648]={recipe_id=1018,recipe_group=0,recipe_level=8,recipe_type=3}---[120]九华惑心盔[男]
		staticrecipe[649]={recipe_id=1019,recipe_group=0,recipe_level=8,recipe_type=3}---[120]九华惑心盔[女]
		staticrecipe[650]={recipe_id=1020,recipe_group=0,recipe_level=9,recipe_type=3}---[135]远峦接天离愁盔[男]
		staticrecipe[651]={recipe_id=1021,recipe_group=0,recipe_level=9,recipe_type=3}---[135]远峦接天离愁盔[女]
		staticrecipe[652]={recipe_id=3610,recipe_group=0,recipe_level=10,recipe_type=3}---100级九黎帽男配方
		staticrecipe[653]={recipe_id=3676,recipe_group=0,recipe_level=10,recipe_type=3}---100级九黎帽女配方
		staticrecipe[654]={recipe_id=3608,recipe_group=0,recipe_level=8,recipe_type=3}---80级九黎帽男配方
		staticrecipe[655]={recipe_id=3674,recipe_group=0,recipe_level=8,recipe_type=3}---80级九黎帽女配方
		staticrecipe[656]={recipe_id=3609,recipe_group=0,recipe_level=9,recipe_type=3}---90级九黎帽男配方
		staticrecipe[657]={recipe_id=3675,recipe_group=0,recipe_level=9,recipe_type=3}---90级九黎帽女配方
		staticrecipe[658]={recipe_id=3617,recipe_group=0,recipe_level=10,recipe_type=3}---逆天100级九黎帽男配方
		staticrecipe[659]={recipe_id=3683,recipe_group=0,recipe_level=10,recipe_type=3}---逆天100级九黎帽女配方
		staticrecipe[660]={recipe_id=3615,recipe_group=0,recipe_level=8,recipe_type=3}---逆天80级九黎帽男配方
		staticrecipe[661]={recipe_id=3681,recipe_group=0,recipe_level=8,recipe_type=3}---逆天80级九黎帽女配方
		staticrecipe[662]={recipe_id=3616,recipe_group=0,recipe_level=9,recipe_type=3}---逆天90级九黎帽男配方
		staticrecipe[663]={recipe_id=3682,recipe_group=0,recipe_level=9,recipe_type=3}---逆天90级九黎帽女配方
		staticrecipe[664]={recipe_id=3631,recipe_group=0,recipe_level=10,recipe_type=3}---逆天防御装100级九黎帽男配方
		staticrecipe[665]={recipe_id=3697,recipe_group=0,recipe_level=10,recipe_type=3}---逆天防御装100级九黎帽女配方
		staticrecipe[666]={recipe_id=3629,recipe_group=0,recipe_level=8,recipe_type=3}---逆天防御装80级九黎帽男配方
		staticrecipe[667]={recipe_id=3695,recipe_group=0,recipe_level=8,recipe_type=3}---逆天防御装80级九黎帽女配方
		staticrecipe[668]={recipe_id=3630,recipe_group=0,recipe_level=9,recipe_type=3}---逆天防御装90级九黎帽男配方
		staticrecipe[669]={recipe_id=3696,recipe_group=0,recipe_level=9,recipe_type=3}---逆天防御装90级九黎帽女配方
		staticrecipe[670]={recipe_id=3652,recipe_group=0,recipe_level=10,recipe_type=3}---逆天抗性装丙100级九黎帽男配方
		staticrecipe[671]={recipe_id=3718,recipe_group=0,recipe_level=10,recipe_type=3}---逆天抗性装丙100级九黎帽女配方
		staticrecipe[672]={recipe_id=3650,recipe_group=0,recipe_level=8,recipe_type=3}---逆天抗性装丙80级九黎帽男配方
		staticrecipe[673]={recipe_id=3716,recipe_group=0,recipe_level=8,recipe_type=3}---逆天抗性装丙80级九黎帽女配方
		staticrecipe[674]={recipe_id=3651,recipe_group=0,recipe_level=9,recipe_type=3}---逆天抗性装丙90级九黎帽男配方
		staticrecipe[675]={recipe_id=3717,recipe_group=0,recipe_level=9,recipe_type=3}---逆天抗性装丙90级九黎帽女配方
		staticrecipe[676]={recipe_id=3659,recipe_group=0,recipe_level=10,recipe_type=3}---逆天抗性装丁100级九黎帽男配方
		staticrecipe[677]={recipe_id=3725,recipe_group=0,recipe_level=10,recipe_type=3}---逆天抗性装丁100级九黎帽女配方
		staticrecipe[678]={recipe_id=3657,recipe_group=0,recipe_level=8,recipe_type=3}---逆天抗性装丁80级九黎帽男配方
		staticrecipe[679]={recipe_id=3723,recipe_group=0,recipe_level=8,recipe_type=3}---逆天抗性装丁80级九黎帽女配方
		staticrecipe[680]={recipe_id=3658,recipe_group=0,recipe_level=9,recipe_type=3}---逆天抗性装丁90级九黎帽男配方
		staticrecipe[681]={recipe_id=3724,recipe_group=0,recipe_level=9,recipe_type=3}---逆天抗性装丁90级九黎帽女配方
		staticrecipe[682]={recipe_id=3638,recipe_group=0,recipe_level=10,recipe_type=3}---逆天抗性装甲100级九黎帽男配方
		staticrecipe[683]={recipe_id=3704,recipe_group=0,recipe_level=10,recipe_type=3}---逆天抗性装甲100级九黎帽女配方
		staticrecipe[684]={recipe_id=3636,recipe_group=0,recipe_level=8,recipe_type=3}---逆天抗性装甲80级九黎帽男配方
		staticrecipe[685]={recipe_id=3702,recipe_group=0,recipe_level=8,recipe_type=3}---逆天抗性装甲80级九黎帽女配方
		staticrecipe[686]={recipe_id=3637,recipe_group=0,recipe_level=9,recipe_type=3}---逆天抗性装甲90级九黎帽男配方
		staticrecipe[687]={recipe_id=3703,recipe_group=0,recipe_level=9,recipe_type=3}---逆天抗性装甲90级九黎帽女配方
		staticrecipe[688]={recipe_id=3666,recipe_group=0,recipe_level=10,recipe_type=3}---逆天抗性装戊100级九黎帽男配方
		staticrecipe[689]={recipe_id=3732,recipe_group=0,recipe_level=10,recipe_type=3}---逆天抗性装戊100级九黎帽女配方
		staticrecipe[690]={recipe_id=3664,recipe_group=0,recipe_level=8,recipe_type=3}---逆天抗性装戊80级九黎帽男配方
		staticrecipe[691]={recipe_id=3730,recipe_group=0,recipe_level=8,recipe_type=3}---逆天抗性装戊80级九黎帽女配方
		staticrecipe[692]={recipe_id=3665,recipe_group=0,recipe_level=9,recipe_type=3}---逆天抗性装戊90级九黎帽男配方
		staticrecipe[693]={recipe_id=3731,recipe_group=0,recipe_level=9,recipe_type=3}---逆天抗性装戊90级九黎帽女配方
		staticrecipe[694]={recipe_id=3645,recipe_group=0,recipe_level=10,recipe_type=3}---逆天抗性装乙100级九黎帽男配方
		staticrecipe[695]={recipe_id=3711,recipe_group=0,recipe_level=10,recipe_type=3}---逆天抗性装乙100级九黎帽女配方
		staticrecipe[696]={recipe_id=3643,recipe_group=0,recipe_level=8,recipe_type=3}---逆天抗性装乙80级九黎帽男配方
		staticrecipe[697]={recipe_id=3709,recipe_group=0,recipe_level=8,recipe_type=3}---逆天抗性装乙80级九黎帽女配方
		staticrecipe[698]={recipe_id=3644,recipe_group=0,recipe_level=9,recipe_type=3}---逆天抗性装乙90级九黎帽男配方
		staticrecipe[699]={recipe_id=3710,recipe_group=0,recipe_level=9,recipe_type=3}---逆天抗性装乙90级九黎帽女配方
		staticrecipe[700]={recipe_id=3624,recipe_group=0,recipe_level=10,recipe_type=3}---逆天魔法装100级九黎帽男配方
		staticrecipe[701]={recipe_id=3690,recipe_group=0,recipe_level=10,recipe_type=3}---逆天魔法装100级九黎帽女配方
		staticrecipe[702]={recipe_id=3622,recipe_group=0,recipe_level=8,recipe_type=3}---逆天魔法装80级九黎帽男配方
		staticrecipe[703]={recipe_id=3688,recipe_group=0,recipe_level=8,recipe_type=3}---逆天魔法装80级九黎帽女配方
		staticrecipe[704]={recipe_id=3623,recipe_group=0,recipe_level=9,recipe_type=3}---逆天魔法装90级九黎帽男配方
		staticrecipe[705]={recipe_id=3689,recipe_group=0,recipe_level=9,recipe_type=3}---逆天魔法装90级九黎帽女配方
		staticrecipe[706]={recipe_id=4020,recipe_group=0,recipe_level=10,recipe_type=3}---100级烈山帽男配方
		staticrecipe[707]={recipe_id=4086,recipe_group=0,recipe_level=10,recipe_type=3}---100级烈山帽女配方
		staticrecipe[708]={recipe_id=4018,recipe_group=0,recipe_level=8,recipe_type=3}---80级烈山帽男配方
		staticrecipe[709]={recipe_id=4084,recipe_group=0,recipe_level=8,recipe_type=3}---80级烈山帽女配方
		staticrecipe[710]={recipe_id=4019,recipe_group=0,recipe_level=9,recipe_type=3}---90级烈山帽男配方
		staticrecipe[711]={recipe_id=4085,recipe_group=0,recipe_level=9,recipe_type=3}---90级烈山帽女配方
		staticrecipe[712]={recipe_id=4027,recipe_group=0,recipe_level=10,recipe_type=3}---逆天100级烈山帽男配方
		staticrecipe[713]={recipe_id=4093,recipe_group=0,recipe_level=10,recipe_type=3}---逆天100级烈山帽女配方
		staticrecipe[714]={recipe_id=4025,recipe_group=0,recipe_level=8,recipe_type=3}---逆天80级烈山帽男配方
		staticrecipe[715]={recipe_id=4091,recipe_group=0,recipe_level=8,recipe_type=3}---逆天80级烈山帽女配方
		staticrecipe[716]={recipe_id=4026,recipe_group=0,recipe_level=9,recipe_type=3}---逆天90级烈山帽男配方
		staticrecipe[717]={recipe_id=4092,recipe_group=0,recipe_level=9,recipe_type=3}---逆天90级烈山帽女配方
		staticrecipe[718]={recipe_id=4041,recipe_group=0,recipe_level=10,recipe_type=3}---逆天防御装100级烈山帽男配方
		staticrecipe[719]={recipe_id=4107,recipe_group=0,recipe_level=10,recipe_type=3}---逆天防御装100级烈山帽女配方
		staticrecipe[720]={recipe_id=4039,recipe_group=0,recipe_level=8,recipe_type=3}---逆天防御装80级烈山帽男配方
		staticrecipe[721]={recipe_id=4105,recipe_group=0,recipe_level=8,recipe_type=3}---逆天防御装80级烈山帽女配方
		staticrecipe[722]={recipe_id=4040,recipe_group=0,recipe_level=9,recipe_type=3}---逆天防御装90级烈山帽男配方
		staticrecipe[723]={recipe_id=4106,recipe_group=0,recipe_level=9,recipe_type=3}---逆天防御装90级烈山帽女配方
		staticrecipe[724]={recipe_id=4062,recipe_group=0,recipe_level=10,recipe_type=3}---逆天抗性装丙100级烈山帽男配方
		staticrecipe[725]={recipe_id=4128,recipe_group=0,recipe_level=10,recipe_type=3}---逆天抗性装丙100级烈山帽女配方
		staticrecipe[726]={recipe_id=4060,recipe_group=0,recipe_level=8,recipe_type=3}---逆天抗性装丙80级烈山帽男配方
		staticrecipe[727]={recipe_id=4126,recipe_group=0,recipe_level=8,recipe_type=3}---逆天抗性装丙80级烈山帽女配方
		staticrecipe[728]={recipe_id=4061,recipe_group=0,recipe_level=9,recipe_type=3}---逆天抗性装丙90级烈山帽男配方
		staticrecipe[729]={recipe_id=4127,recipe_group=0,recipe_level=9,recipe_type=3}---逆天抗性装丙90级烈山帽女配方
		staticrecipe[730]={recipe_id=4069,recipe_group=0,recipe_level=10,recipe_type=3}---逆天抗性装丁100级烈山帽男配方
		staticrecipe[731]={recipe_id=4135,recipe_group=0,recipe_level=10,recipe_type=3}---逆天抗性装丁100级烈山帽女配方
		staticrecipe[732]={recipe_id=4067,recipe_group=0,recipe_level=8,recipe_type=3}---逆天抗性装丁80级烈山帽男配方
		staticrecipe[733]={recipe_id=4133,recipe_group=0,recipe_level=8,recipe_type=3}---逆天抗性装丁80级烈山帽女配方
		staticrecipe[734]={recipe_id=4068,recipe_group=0,recipe_level=9,recipe_type=3}---逆天抗性装丁90级烈山帽男配方
		staticrecipe[735]={recipe_id=4134,recipe_group=0,recipe_level=9,recipe_type=3}---逆天抗性装丁90级烈山帽女配方
		staticrecipe[736]={recipe_id=4048,recipe_group=0,recipe_level=10,recipe_type=3}---逆天抗性装甲100级烈山帽男配方
		staticrecipe[737]={recipe_id=4114,recipe_group=0,recipe_level=10,recipe_type=3}---逆天抗性装甲100级烈山帽女配方
		staticrecipe[738]={recipe_id=4046,recipe_group=0,recipe_level=8,recipe_type=3}---逆天抗性装甲80级烈山帽男配方
		staticrecipe[739]={recipe_id=4112,recipe_group=0,recipe_level=8,recipe_type=3}---逆天抗性装甲80级烈山帽女配方
		staticrecipe[740]={recipe_id=4047,recipe_group=0,recipe_level=9,recipe_type=3}---逆天抗性装甲90级烈山帽男配方
		staticrecipe[741]={recipe_id=4113,recipe_group=0,recipe_level=9,recipe_type=3}---逆天抗性装甲90级烈山帽女配方
		staticrecipe[742]={recipe_id=4076,recipe_group=0,recipe_level=10,recipe_type=3}---逆天抗性装戊100级烈山帽男配方
		staticrecipe[743]={recipe_id=4142,recipe_group=0,recipe_level=10,recipe_type=3}---逆天抗性装戊100级烈山帽女配方
		staticrecipe[744]={recipe_id=4074,recipe_group=0,recipe_level=8,recipe_type=3}---逆天抗性装戊80级烈山帽男配方
		staticrecipe[745]={recipe_id=4140,recipe_group=0,recipe_level=8,recipe_type=3}---逆天抗性装戊80级烈山帽女配方
		staticrecipe[746]={recipe_id=4075,recipe_group=0,recipe_level=9,recipe_type=3}---逆天抗性装戊90级烈山帽男配方
		staticrecipe[747]={recipe_id=4141,recipe_group=0,recipe_level=9,recipe_type=3}---逆天抗性装戊90级烈山帽女配方
		staticrecipe[748]={recipe_id=4055,recipe_group=0,recipe_level=10,recipe_type=3}---逆天抗性装乙100级烈山帽男配方
		staticrecipe[749]={recipe_id=4121,recipe_group=0,recipe_level=10,recipe_type=3}---逆天抗性装乙100级烈山帽女配方
		staticrecipe[750]={recipe_id=4053,recipe_group=0,recipe_level=8,recipe_type=3}---逆天抗性装乙80级烈山帽男配方
		staticrecipe[751]={recipe_id=4119,recipe_group=0,recipe_level=8,recipe_type=3}---逆天抗性装乙80级烈山帽女配方
		staticrecipe[752]={recipe_id=4054,recipe_group=0,recipe_level=9,recipe_type=3}---逆天抗性装乙90级烈山帽男配方
		staticrecipe[753]={recipe_id=4120,recipe_group=0,recipe_level=9,recipe_type=3}---逆天抗性装乙90级烈山帽女配方
		staticrecipe[754]={recipe_id=4034,recipe_group=0,recipe_level=10,recipe_type=3}---逆天魔法装100级烈山帽男配方
		staticrecipe[755]={recipe_id=4100,recipe_group=0,recipe_level=10,recipe_type=3}---逆天魔法装100级烈山帽女配方
		staticrecipe[756]={recipe_id=4032,recipe_group=0,recipe_level=8,recipe_type=3}---逆天魔法装80级烈山帽男配方
		staticrecipe[757]={recipe_id=4098,recipe_group=0,recipe_level=8,recipe_type=3}---逆天魔法装80级烈山帽女配方
		staticrecipe[758]={recipe_id=4033,recipe_group=0,recipe_level=9,recipe_type=3}---逆天魔法装90级烈山帽男配方
		staticrecipe[759]={recipe_id=4099,recipe_group=0,recipe_level=9,recipe_type=3}---逆天魔法装90级烈山帽女配方
		staticrecipe[760]={recipe_id=259,recipe_group=0,recipe_level=10,recipe_type=3}---[142]玉清冠[男]
		staticrecipe[761]={recipe_id=307,recipe_group=0,recipe_level=10,recipe_type=3}---[142]玉清冠[女]
		staticrecipe[762]={recipe_id=257,recipe_group=0,recipe_level=8,recipe_type=3}---[120]太清冠[男]
		staticrecipe[763]={recipe_id=305,recipe_group=0,recipe_level=8,recipe_type=3}---[120]太清冠[女]
		staticrecipe[764]={recipe_id=258,recipe_group=0,recipe_level=9,recipe_type=3}---[135]上清冠[男]
		staticrecipe[765]={recipe_id=306,recipe_group=0,recipe_level=9,recipe_type=3}---[135]上清冠[女]
		staticrecipe[766]={recipe_id=1086,recipe_group=0,recipe_level=10,recipe_type=3}---[142]三皇敕令道元冠[男]
		staticrecipe[767]={recipe_id=1087,recipe_group=0,recipe_level=10,recipe_type=3}---[142]三皇敕令道元冠[女]
		staticrecipe[768]={recipe_id=1090,recipe_group=0,recipe_level=8,recipe_type=3}---[120]三仙证道冠[男]
		staticrecipe[769]={recipe_id=1091,recipe_group=0,recipe_level=8,recipe_type=3}---[120]三仙证道冠[女]
		staticrecipe[770]={recipe_id=1092,recipe_group=0,recipe_level=9,recipe_type=3}---[135]众妙参玄道尊冠[男]
		staticrecipe[771]={recipe_id=1093,recipe_group=0,recipe_level=9,recipe_type=3}---[135]众妙参玄道尊冠[女]
		staticrecipe[772]={recipe_id=1094,recipe_group=0,recipe_level=10,recipe_type=3}---[142]六气遁法天机冠[男]
		staticrecipe[773]={recipe_id=1095,recipe_group=0,recipe_level=10,recipe_type=3}---[142]六气遁法天机冠[女]
		staticrecipe[774]={recipe_id=1098,recipe_group=0,recipe_level=8,recipe_type=3}---[120]六爻占命冠[男]
		staticrecipe[775]={recipe_id=1099,recipe_group=0,recipe_level=8,recipe_type=3}---[120]六爻占命冠[女]
		staticrecipe[776]={recipe_id=1100,recipe_group=0,recipe_level=9,recipe_type=3}---[135]灵机玄刹玉真冠[男]
		staticrecipe[777]={recipe_id=1101,recipe_group=0,recipe_level=9,recipe_type=3}---[135]灵机玄刹玉真冠[女]
		staticrecipe[778]={recipe_id=1102,recipe_group=0,recipe_level=10,recipe_type=3}---[142]七法悟玄神藏冠[男]
		staticrecipe[779]={recipe_id=1103,recipe_group=0,recipe_level=10,recipe_type=3}---[142]七法悟玄神藏冠[女]
		staticrecipe[780]={recipe_id=1106,recipe_group=0,recipe_level=8,recipe_type=3}---[120]七宿驻灵冠[男]
		staticrecipe[781]={recipe_id=1107,recipe_group=0,recipe_level=8,recipe_type=3}---[120]七宿驻灵冠[女]
		staticrecipe[782]={recipe_id=1108,recipe_group=0,recipe_level=9,recipe_type=3}---[135]经天星君命元冠[男]
		staticrecipe[783]={recipe_id=1109,recipe_group=0,recipe_level=9,recipe_type=3}---[135]经天星君命元冠[女]
		staticrecipe[784]={recipe_id=1110,recipe_group=0,recipe_level=10,recipe_type=3}---[142]四圣法谍固灵冠[男]
		staticrecipe[785]={recipe_id=1111,recipe_group=0,recipe_level=10,recipe_type=3}---[142]四圣法谍固灵冠[女]
		staticrecipe[786]={recipe_id=1114,recipe_group=0,recipe_level=8,recipe_type=3}---[120]四海游龙冠[男]
		staticrecipe[787]={recipe_id=1115,recipe_group=0,recipe_level=8,recipe_type=3}---[120]四海游龙冠[女]
		staticrecipe[788]={recipe_id=1116,recipe_group=0,recipe_level=9,recipe_type=3}---[135]玉鼎结丹化圣冠[男]
		staticrecipe[789]={recipe_id=1117,recipe_group=0,recipe_level=9,recipe_type=3}---[135]玉鼎结丹化圣冠[女]
		staticrecipe[790]={recipe_id=1118,recipe_group=0,recipe_level=10,recipe_type=3}---[142]八门金锁定心冠[男]
		staticrecipe[791]={recipe_id=1119,recipe_group=0,recipe_level=10,recipe_type=3}---[142]八门金锁定心冠[女]
		staticrecipe[792]={recipe_id=1122,recipe_group=0,recipe_level=8,recipe_type=3}---[120]八极玉阳冠[男]
		staticrecipe[793]={recipe_id=1123,recipe_group=0,recipe_level=8,recipe_type=3}---[120]八极玉阳冠[女]
		staticrecipe[794]={recipe_id=1124,recipe_group=0,recipe_level=9,recipe_type=3}---[135]秘法通玄洞冥冠[男]
		staticrecipe[795]={recipe_id=1125,recipe_group=0,recipe_level=9,recipe_type=3}---[135]秘法通玄洞冥冠[女]
		staticrecipe[796]={recipe_id=1126,recipe_group=0,recipe_level=10,recipe_type=3}---[142]五帝诏谕玄极冠[男]
		staticrecipe[797]={recipe_id=1127,recipe_group=0,recipe_level=10,recipe_type=3}---[142]五帝诏谕玄极冠[女]
		staticrecipe[798]={recipe_id=1130,recipe_group=0,recipe_level=8,recipe_type=3}---[120]五元固神冠[男]
		staticrecipe[799]={recipe_id=1131,recipe_group=0,recipe_level=8,recipe_type=3}---[120]五元固神冠[女]
		staticrecipe[800]={recipe_id=1132,recipe_group=0,recipe_level=9,recipe_type=3}---[135]紫阳初照掸心冠[男]
		staticrecipe[801]={recipe_id=1133,recipe_group=0,recipe_level=9,recipe_type=3}---[135]紫阳初照掸心冠[女]
		staticrecipe[802]={recipe_id=1134,recipe_group=0,recipe_level=10,recipe_type=3}---[142]九刹疾雷浩光冠[男]
		staticrecipe[803]={recipe_id=1135,recipe_group=0,recipe_level=10,recipe_type=3}---[142]九刹疾雷浩光冠[女]
		staticrecipe[804]={recipe_id=1138,recipe_group=0,recipe_level=8,recipe_type=3}---[120]九英聚元冠[男]
		staticrecipe[805]={recipe_id=1139,recipe_group=0,recipe_level=8,recipe_type=3}---[120]九英聚元冠[女]
		staticrecipe[806]={recipe_id=1140,recipe_group=0,recipe_level=9,recipe_type=3}---[135]积元贮本弥金冠[男]
		staticrecipe[807]={recipe_id=1141,recipe_group=0,recipe_level=9,recipe_type=3}---[135]积元贮本弥金冠[女]
		staticrecipe[808]={recipe_id=1078,recipe_group=0,recipe_level=10,recipe_type=3}---[142]九晶聚瑞天怒冠[男]
		staticrecipe[809]={recipe_id=1079,recipe_group=0,recipe_level=10,recipe_type=3}---[142]九晶聚瑞天怒冠[女]
		staticrecipe[810]={recipe_id=1082,recipe_group=0,recipe_level=8,recipe_type=3}---[120]九阳洞仙冠[男]
		staticrecipe[811]={recipe_id=1083,recipe_group=0,recipe_level=8,recipe_type=3}---[120]九阳洞仙冠[女]
		staticrecipe[812]={recipe_id=1084,recipe_group=0,recipe_level=9,recipe_type=3}---[135]云门天官锦罗冠[男]
		staticrecipe[813]={recipe_id=1085,recipe_group=0,recipe_level=9,recipe_type=3}---[135]云门天官锦罗冠[女]
		staticrecipe[814]={recipe_id=263,recipe_group=0,recipe_level=10,recipe_type=3}---[142]菩提冠[男]
		staticrecipe[815]={recipe_id=311,recipe_group=0,recipe_level=10,recipe_type=3}---[142]菩提冠[女]
		staticrecipe[816]={recipe_id=261,recipe_group=0,recipe_level=8,recipe_type=3}---[120]无相冠[男]
		staticrecipe[817]={recipe_id=309,recipe_group=0,recipe_level=8,recipe_type=3}---[120]无相冠[女]
		staticrecipe[818]={recipe_id=262,recipe_group=0,recipe_level=9,recipe_type=3}---[135]龙象冠[男]
		staticrecipe[819]={recipe_id=310,recipe_group=0,recipe_level=9,recipe_type=3}---[135]龙象冠[女]
		staticrecipe[820]={recipe_id=1150,recipe_group=0,recipe_level=10,recipe_type=3}---[142]三世娑婆往生冠[男]
		staticrecipe[821]={recipe_id=1151,recipe_group=0,recipe_level=10,recipe_type=3}---[142]三世娑婆往生冠[女]
		staticrecipe[822]={recipe_id=1154,recipe_group=0,recipe_level=8,recipe_type=3}---[120]三皈妙悟冠[男]
		staticrecipe[823]={recipe_id=1155,recipe_group=0,recipe_level=8,recipe_type=3}---[120]三皈妙悟冠[女]
		staticrecipe[824]={recipe_id=1156,recipe_group=0,recipe_level=9,recipe_type=3}---[135]毗卢沙门尊者冠[男]
		staticrecipe[825]={recipe_id=1157,recipe_group=0,recipe_level=9,recipe_type=3}---[135]毗卢沙门尊者冠[女]
		staticrecipe[826]={recipe_id=1158,recipe_group=0,recipe_level=10,recipe_type=3}---[142]六觉无量功德冠[男]
		staticrecipe[827]={recipe_id=1159,recipe_group=0,recipe_level=10,recipe_type=3}---[142]六觉无量功德冠[女]
		staticrecipe[828]={recipe_id=1162,recipe_group=0,recipe_level=8,recipe_type=3}---[120]六言真诀冠[男]
		staticrecipe[829]={recipe_id=1163,recipe_group=0,recipe_level=8,recipe_type=3}---[120]六言真诀冠[女]
		staticrecipe[830]={recipe_id=1164,recipe_group=0,recipe_level=9,recipe_type=3}---[135]无业宿尽伽蓝冠[男]
		staticrecipe[831]={recipe_id=1165,recipe_group=0,recipe_level=9,recipe_type=3}---[135]无业宿尽伽蓝冠[女]
		staticrecipe[832]={recipe_id=1166,recipe_group=0,recipe_level=10,recipe_type=3}---[142]七劫轮回寂灭冠[男]
		staticrecipe[833]={recipe_id=1167,recipe_group=0,recipe_level=10,recipe_type=3}---[142]七劫轮回寂灭冠[女]
		staticrecipe[834]={recipe_id=1170,recipe_group=0,recipe_level=8,recipe_type=3}---[120]七法梵音冠[男]
		staticrecipe[835]={recipe_id=1171,recipe_group=0,recipe_level=8,recipe_type=3}---[120]七法梵音冠[女]
		staticrecipe[836]={recipe_id=1172,recipe_group=0,recipe_level=9,recipe_type=3}---[135]无量本行大士冠[男]
		staticrecipe[837]={recipe_id=1173,recipe_group=0,recipe_level=9,recipe_type=3}---[135]无量本行大士冠[女]
		staticrecipe[838]={recipe_id=1174,recipe_group=0,recipe_level=10,recipe_type=3}---[142]四相无畏光明冠[男]
		staticrecipe[839]={recipe_id=1175,recipe_group=0,recipe_level=10,recipe_type=3}---[142]四相无畏光明冠[女]
		staticrecipe[840]={recipe_id=1178,recipe_group=0,recipe_level=8,recipe_type=3}---[120]四承通达冠[男]
		staticrecipe[841]={recipe_id=1179,recipe_group=0,recipe_level=8,recipe_type=3}---[120]四承通达冠[女]
		staticrecipe[842]={recipe_id=1180,recipe_group=0,recipe_level=9,recipe_type=3}---[135]不坏尊王罗汉冠[男]
		staticrecipe[843]={recipe_id=1181,recipe_group=0,recipe_level=9,recipe_type=3}---[135]不坏尊王罗汉冠[女]
		staticrecipe[844]={recipe_id=1182,recipe_group=0,recipe_level=10,recipe_type=3}---[142]八音元清净体冠[男]
		staticrecipe[845]={recipe_id=1183,recipe_group=0,recipe_level=10,recipe_type=3}---[142]八音元清净体冠[女]
		staticrecipe[846]={recipe_id=1186,recipe_group=0,recipe_level=8,recipe_type=3}---[120]八德滤心冠[男]
		staticrecipe[847]={recipe_id=1187,recipe_group=0,recipe_level=8,recipe_type=3}---[120]八德滤心冠[女]
		staticrecipe[848]={recipe_id=1188,recipe_group=0,recipe_level=9,recipe_type=3}---[135]观身无常上师冠[男]
		staticrecipe[849]={recipe_id=1189,recipe_group=0,recipe_level=9,recipe_type=3}---[135]观身无常上师冠[女]
		staticrecipe[850]={recipe_id=1190,recipe_group=0,recipe_level=10,recipe_type=3}---[142]五浊恶世舍利冠[男]
		staticrecipe[851]={recipe_id=1191,recipe_group=0,recipe_level=10,recipe_type=3}---[142]五浊恶世舍利冠[女]
		staticrecipe[852]={recipe_id=1194,recipe_group=0,recipe_level=8,recipe_type=3}---[120]五轮宿业冠[男]
		staticrecipe[853]={recipe_id=1195,recipe_group=0,recipe_level=8,recipe_type=3}---[120]五轮宿业冠[女]
		staticrecipe[854]={recipe_id=1196,recipe_group=0,recipe_level=9,recipe_type=3}---[135]神通永驻金刚冠[男]
		staticrecipe[855]={recipe_id=1197,recipe_group=0,recipe_level=9,recipe_type=3}---[135]神通永驻金刚冠[女]
		staticrecipe[856]={recipe_id=1198,recipe_group=0,recipe_level=10,recipe_type=3}---[142]十方世界生佛冠[男]
		staticrecipe[857]={recipe_id=1199,recipe_group=0,recipe_level=10,recipe_type=3}---[142]十方世界生佛冠[女]
		staticrecipe[858]={recipe_id=1202,recipe_group=0,recipe_level=8,recipe_type=3}---[120]九妙慈航冠[男]
		staticrecipe[859]={recipe_id=1203,recipe_group=0,recipe_level=8,recipe_type=3}---[120]九妙慈航冠[女]
		staticrecipe[860]={recipe_id=1204,recipe_group=0,recipe_level=9,recipe_type=3}---[135]摩诃刹利诸天冠[男]
		staticrecipe[861]={recipe_id=1205,recipe_group=0,recipe_level=9,recipe_type=3}---[135]摩诃刹利诸天冠[女]
		staticrecipe[862]={recipe_id=1142,recipe_group=0,recipe_level=10,recipe_type=3}---[142]九殿阎罗地藏冠[男]
		staticrecipe[863]={recipe_id=1143,recipe_group=0,recipe_level=10,recipe_type=3}---[142]九殿阎罗地藏冠[女]
		staticrecipe[864]={recipe_id=1146,recipe_group=0,recipe_level=8,recipe_type=3}---[120]九尊华严冠[男]
		staticrecipe[865]={recipe_id=1147,recipe_group=0,recipe_level=8,recipe_type=3}---[120]九尊华严冠[女]
		staticrecipe[866]={recipe_id=1148,recipe_group=0,recipe_level=9,recipe_type=3}---[135]金山觉意佛尊冠[男]
		staticrecipe[867]={recipe_id=1149,recipe_group=0,recipe_level=9,recipe_type=3}---[135]金山觉意佛尊冠[女]
		staticrecipe[868]={recipe_id=3113,recipe_group=0,recipe_level=5,recipe_type=5}---造化之天循
		staticrecipe[869]={recipe_id=3114,recipe_group=0,recipe_level=5,recipe_type=5}---造化之天戍
		staticrecipe[870]={recipe_id=3116,recipe_group=0,recipe_level=5,recipe_type=5}---造化之天眷
		staticrecipe[871]={recipe_id=3115,recipe_group=0,recipe_level=5,recipe_type=5}---造化之天弈
		staticrecipe[872]={recipe_id=3117,recipe_group=0,recipe_level=5,recipe_type=5}---造化之玉虚
		staticrecipe[873]={recipe_id=3118,recipe_group=0,recipe_level=5,recipe_type=5}---造化之天道
		staticrecipe[874]={recipe_id=1929,recipe_group=0,recipe_level=10,recipe_type=1}---[148]魔神指
		staticrecipe[875]={recipe_id=1927,recipe_group=0,recipe_level=8,recipe_type=1}---[120]魔音刺
		staticrecipe[876]={recipe_id=1928,recipe_group=0,recipe_level=9,recipe_type=1}---[135]孽龙刺
		staticrecipe[877]={recipe_id=235,recipe_group=0,recipe_level=10,recipe_type=1}---[148]天刑刃
		staticrecipe[878]={recipe_id=233,recipe_group=0,recipe_level=8,recipe_type=1}---[120]屠龙刃
		staticrecipe[879]={recipe_id=234,recipe_group=0,recipe_level=9,recipe_type=1}---[135]戮魂刃
		staticrecipe[880]={recipe_id=239,recipe_group=0,recipe_level=10,recipe_type=1}---[148]魅天环
		staticrecipe[881]={recipe_id=237,recipe_group=0,recipe_level=8,recipe_type=1}---[120]摄魂环
		staticrecipe[882]={recipe_id=238,recipe_group=0,recipe_level=9,recipe_type=1}---[135]仙籁环
		staticrecipe[883]={recipe_id=3599,recipe_group=0,recipe_level=10,recipe_type=1}---100级九黎武器配方
		staticrecipe[884]={recipe_id=3597,recipe_group=0,recipe_level=8,recipe_type=1}---80级九黎武器配方
		staticrecipe[885]={recipe_id=3598,recipe_group=0,recipe_level=9,recipe_type=1}---90级九黎武器配方
		staticrecipe[886]={recipe_id=4010,recipe_group=0,recipe_level=10,recipe_type=1}---100级烈山武器配方
		staticrecipe[887]={recipe_id=4008,recipe_group=0,recipe_level=8,recipe_type=1}---80级烈山武器配方
		staticrecipe[888]={recipe_id=4009,recipe_group=0,recipe_level=9,recipe_type=1}---90级烈山武器配方
		staticrecipe[889]={recipe_id=243,recipe_group=0,recipe_level=10,recipe_type=1}---[148]天尊剑
		staticrecipe[890]={recipe_id=241,recipe_group=0,recipe_level=8,recipe_type=1}---[120]乾坤剑
		staticrecipe[891]={recipe_id=242,recipe_group=0,recipe_level=9,recipe_type=1}---[135]归元剑
		staticrecipe[892]={recipe_id=247,recipe_group=0,recipe_level=10,recipe_type=1}---[148]寂灭杖
		staticrecipe[893]={recipe_id=245,recipe_group=0,recipe_level=8,recipe_type=1}---[120]空明杖
		staticrecipe[894]={recipe_id=246,recipe_group=0,recipe_level=9,recipe_type=1}---[135]达摩杖
		staticrecipe[895]={recipe_id=2071,recipe_group=0,recipe_level=10,recipe_type=4}---[144]地藏靴[男]
		staticrecipe[896]={recipe_id=2137,recipe_group=0,recipe_level=10,recipe_type=4}---[144]地藏靴[女]
		staticrecipe[897]={recipe_id=2069,recipe_group=0,recipe_level=8,recipe_type=4}---[120]冥宁靴[男]
		staticrecipe[898]={recipe_id=2135,recipe_group=0,recipe_level=8,recipe_type=4}---[120]冥宁靴[女]
		staticrecipe[899]={recipe_id=2070,recipe_group=0,recipe_level=9,recipe_type=4}---[135]龙骨靴[男]
		staticrecipe[900]={recipe_id=2136,recipe_group=0,recipe_level=9,recipe_type=4}---[135]龙骨靴[女]
		staticrecipe[901]={recipe_id=2078,recipe_group=0,recipe_level=10,recipe_type=4}---[144]九地冥君明光靴[男]
		staticrecipe[902]={recipe_id=2144,recipe_group=0,recipe_level=10,recipe_type=4}---[144]九地冥君明光靴[女]
		staticrecipe[903]={recipe_id=2076,recipe_group=0,recipe_level=8,recipe_type=4}---[120]九曜定心靴[男]
		staticrecipe[904]={recipe_id=2142,recipe_group=0,recipe_level=8,recipe_type=4}---[120]九曜定心靴[女]
		staticrecipe[905]={recipe_id=2077,recipe_group=0,recipe_level=9,recipe_type=4}---[135]战天狂灵魔神靴[男]
		staticrecipe[906]={recipe_id=2143,recipe_group=0,recipe_level=9,recipe_type=4}---[135]战天狂灵魔神靴[女]
		staticrecipe[907]={recipe_id=2092,recipe_group=0,recipe_level=10,recipe_type=4}---[144]三昧真火金精靴[男]
		staticrecipe[908]={recipe_id=2158,recipe_group=0,recipe_level=10,recipe_type=4}---[144]三昧真火金精靴[女]
		staticrecipe[909]={recipe_id=2090,recipe_group=0,recipe_level=8,recipe_type=4}---[120]三才鬼星靴[男]
		staticrecipe[910]={recipe_id=2156,recipe_group=0,recipe_level=8,recipe_type=4}---[120]三才鬼星靴[女]
		staticrecipe[911]={recipe_id=2091,recipe_group=0,recipe_level=9,recipe_type=4}---[135]荒坟鬼冢磷焰靴[男]
		staticrecipe[912]={recipe_id=2157,recipe_group=0,recipe_level=9,recipe_type=4}---[135]荒坟鬼冢磷焰靴[女]
		staticrecipe[913]={recipe_id=2113,recipe_group=0,recipe_level=10,recipe_type=4}---[144]六道轮回魔心靴[男]
		staticrecipe[914]={recipe_id=2179,recipe_group=0,recipe_level=10,recipe_type=4}---[144]六道轮回魔心靴[女]
		staticrecipe[915]={recipe_id=2111,recipe_group=0,recipe_level=8,recipe_type=4}---[120]六驭鬼烈靴[男]
		staticrecipe[916]={recipe_id=2177,recipe_group=0,recipe_level=8,recipe_type=4}---[120]六驭鬼烈靴[女]
		staticrecipe[917]={recipe_id=2112,recipe_group=0,recipe_level=9,recipe_type=4}---[135]冥河毒蛟洗髓靴[男]
		staticrecipe[918]={recipe_id=2178,recipe_group=0,recipe_level=9,recipe_type=4}---[135]冥河毒蛟洗髓靴[女]
		staticrecipe[919]={recipe_id=2120,recipe_group=0,recipe_level=10,recipe_type=4}---[144]七劫天罪威灵靴[男]
		staticrecipe[920]={recipe_id=2186,recipe_group=0,recipe_level=10,recipe_type=4}---[144]七劫天罪威灵靴[女]
		staticrecipe[921]={recipe_id=2118,recipe_group=0,recipe_level=8,recipe_type=4}---[120]七孔聚魂靴[男]
		staticrecipe[922]={recipe_id=2184,recipe_group=0,recipe_level=8,recipe_type=4}---[120]七孔聚魂靴[女]
		staticrecipe[923]={recipe_id=2119,recipe_group=0,recipe_level=9,recipe_type=4}---[135]血海逐波淀魂靴[男]
		staticrecipe[924]={recipe_id=2185,recipe_group=0,recipe_level=9,recipe_type=4}---[135]血海逐波淀魂靴[女]
		staticrecipe[925]={recipe_id=2099,recipe_group=0,recipe_level=10,recipe_type=4}---[144]天弈元尊冥靴[男]
		staticrecipe[926]={recipe_id=2165,recipe_group=0,recipe_level=10,recipe_type=4}---[144]天弈元尊冥靴[女]
		staticrecipe[927]={recipe_id=2097,recipe_group=0,recipe_level=8,recipe_type=4}---[120]武围神将冥靴[男]
		staticrecipe[928]={recipe_id=2163,recipe_group=0,recipe_level=8,recipe_type=4}---[120]武围神将冥靴[女]
		staticrecipe[929]={recipe_id=2098,recipe_group=0,recipe_level=9,recipe_type=4}---[135]界河真君冥靴[男]
		staticrecipe[930]={recipe_id=2164,recipe_group=0,recipe_level=9,recipe_type=4}---[135]界河真君冥靴[女]
		staticrecipe[931]={recipe_id=2127,recipe_group=0,recipe_level=10,recipe_type=4}---[144]八凶玄火炎魔靴[男]
		staticrecipe[932]={recipe_id=2193,recipe_group=0,recipe_level=10,recipe_type=4}---[144]八凶玄火炎魔靴[女]
		staticrecipe[933]={recipe_id=2125,recipe_group=0,recipe_level=8,recipe_type=4}---[120]八门金锁靴[男]
		staticrecipe[934]={recipe_id=2191,recipe_group=0,recipe_level=8,recipe_type=4}---[120]八门金锁靴[女]
		staticrecipe[935]={recipe_id=2126,recipe_group=0,recipe_level=9,recipe_type=4}---[135]无影蛊毒瘟蝗靴[男]
		staticrecipe[936]={recipe_id=2192,recipe_group=0,recipe_level=9,recipe_type=4}---[135]无影蛊毒瘟蝗靴[女]
		staticrecipe[937]={recipe_id=2106,recipe_group=0,recipe_level=10,recipe_type=4}---[144]五气升腾朝日靴[男]
		staticrecipe[938]={recipe_id=2172,recipe_group=0,recipe_level=10,recipe_type=4}---[144]五气升腾朝日靴[女]
		staticrecipe[939]={recipe_id=2104,recipe_group=0,recipe_level=8,recipe_type=4}---[120]五行绝命靴[男]
		staticrecipe[940]={recipe_id=2170,recipe_group=0,recipe_level=8,recipe_type=4}---[120]五行绝命靴[女]
		staticrecipe[941]={recipe_id=2105,recipe_group=0,recipe_level=9,recipe_type=4}---[135]毒尊避尘金麟靴[男]
		staticrecipe[942]={recipe_id=2171,recipe_group=0,recipe_level=9,recipe_type=4}---[135]毒尊避尘金麟靴[女]
		staticrecipe[943]={recipe_id=2085,recipe_group=0,recipe_level=10,recipe_type=4}---[144]九幽尊主傲神靴[男]
		staticrecipe[944]={recipe_id=2151,recipe_group=0,recipe_level=10,recipe_type=4}---[144]九幽尊主傲神靴[女]
		staticrecipe[945]={recipe_id=2083,recipe_group=0,recipe_level=8,recipe_type=4}---[120]九蛊逆刃靴[男]
		staticrecipe[946]={recipe_id=2149,recipe_group=0,recipe_level=8,recipe_type=4}---[120]九蛊逆刃靴[女]
		staticrecipe[947]={recipe_id=2084,recipe_group=0,recipe_level=9,recipe_type=4}---[135]幽冥转魂天祭靴[男]
		staticrecipe[948]={recipe_id=2150,recipe_group=0,recipe_level=9,recipe_type=4}---[135]幽冥转魂天祭靴[女]
		staticrecipe[949]={recipe_id=283,recipe_group=0,recipe_level=10,recipe_type=4}---[144]皇极靴[男]
		staticrecipe[950]={recipe_id=331,recipe_group=0,recipe_level=10,recipe_type=4}---[144]皇极靴[女]
		staticrecipe[951]={recipe_id=281,recipe_group=0,recipe_level=8,recipe_type=4}---[120]狂神靴[男]
		staticrecipe[952]={recipe_id=329,recipe_group=0,recipe_level=8,recipe_type=4}---[120]狂神靴[女]
		staticrecipe[953]={recipe_id=282,recipe_group=0,recipe_level=9,recipe_type=4}---[135]龙魂靴[男]
		staticrecipe[954]={recipe_id=330,recipe_group=0,recipe_level=9,recipe_type=4}---[135]龙魂靴[女]
		staticrecipe[955]={recipe_id=1214,recipe_group=0,recipe_level=10,recipe_type=4}---[144]三界恶灵邪神靴[男]
		staticrecipe[956]={recipe_id=1215,recipe_group=0,recipe_level=10,recipe_type=4}---[144]三界恶灵邪神靴[女]
		staticrecipe[957]={recipe_id=1218,recipe_group=0,recipe_level=8,recipe_type=4}---[120]三雄沥血靴[男]
		staticrecipe[958]={recipe_id=1219,recipe_group=0,recipe_level=8,recipe_type=4}---[120]三雄沥血靴[女]
		staticrecipe[959]={recipe_id=1220,recipe_group=0,recipe_level=9,recipe_type=4}---[135]樊龙碎鳞乌金靴[男]
		staticrecipe[960]={recipe_id=1221,recipe_group=0,recipe_level=9,recipe_type=4}---[135]樊龙碎鳞乌金靴[女]
		staticrecipe[961]={recipe_id=1222,recipe_group=0,recipe_level=10,recipe_type=4}---[144]六合纵横麒麟靴[男]
		staticrecipe[962]={recipe_id=1223,recipe_group=0,recipe_level=10,recipe_type=4}---[144]六合纵横麒麟靴[女]
		staticrecipe[963]={recipe_id=1226,recipe_group=0,recipe_level=8,recipe_type=4}---[120]六丁破军靴[男]
		staticrecipe[964]={recipe_id=1227,recipe_group=0,recipe_level=8,recipe_type=4}---[120]六丁破军靴[女]
		staticrecipe[965]={recipe_id=1228,recipe_group=0,recipe_level=9,recipe_type=4}---[135]天狱炼魂凝血靴[男]
		staticrecipe[966]={recipe_id=1229,recipe_group=0,recipe_level=9,recipe_type=4}---[135]天狱炼魂凝血靴[女]
		staticrecipe[967]={recipe_id=1230,recipe_group=0,recipe_level=10,recipe_type=4}---[144]七捷荡寇明光靴[男]
		staticrecipe[968]={recipe_id=1231,recipe_group=0,recipe_level=10,recipe_type=4}---[144]七捷荡寇明光靴[女]
		staticrecipe[969]={recipe_id=1234,recipe_group=0,recipe_level=8,recipe_type=4}---[120]七咒乱魂靴[男]
		staticrecipe[970]={recipe_id=1235,recipe_group=0,recipe_level=8,recipe_type=4}---[120]七咒乱魂靴[女]
		staticrecipe[971]={recipe_id=1236,recipe_group=0,recipe_level=9,recipe_type=4}---[135]狂岚啸天狻猊靴[男]
		staticrecipe[972]={recipe_id=1237,recipe_group=0,recipe_level=9,recipe_type=4}---[135]狂岚啸天狻猊靴[女]
		staticrecipe[973]={recipe_id=1238,recipe_group=0,recipe_level=10,recipe_type=4}---[144]四海啸傲龙筋靴[男]
		staticrecipe[974]={recipe_id=1239,recipe_group=0,recipe_level=10,recipe_type=4}---[144]四海啸傲龙筋靴[女]
		staticrecipe[975]={recipe_id=1242,recipe_group=0,recipe_level=8,recipe_type=4}---[120]四夷伏威靴[男]
		staticrecipe[976]={recipe_id=1243,recipe_group=0,recipe_level=8,recipe_type=4}---[120]四夷伏威靴[女]
		staticrecipe[977]={recipe_id=1244,recipe_group=0,recipe_level=9,recipe_type=4}---[135]兽面吞头连环靴[男]
		staticrecipe[978]={recipe_id=1245,recipe_group=0,recipe_level=9,recipe_type=4}---[135]兽面吞头连环靴[女]
		staticrecipe[979]={recipe_id=1246,recipe_group=0,recipe_level=10,recipe_type=4}---[144]八义继英掩心靴[男]
		staticrecipe[980]={recipe_id=1247,recipe_group=0,recipe_level=10,recipe_type=4}---[144]八义继英掩心靴[女]
		staticrecipe[981]={recipe_id=1250,recipe_group=0,recipe_level=8,recipe_type=4}---[120]八凶怒焰靴[男]
		staticrecipe[982]={recipe_id=1251,recipe_group=0,recipe_level=8,recipe_type=4}---[120]八凶怒焰靴[女]
		staticrecipe[983]={recipe_id=1252,recipe_group=0,recipe_level=9,recipe_type=4}---[135]金玉太岁绽魂靴[男]
		staticrecipe[984]={recipe_id=1253,recipe_group=0,recipe_level=9,recipe_type=4}---[135]金玉太岁绽魂靴[女]
		staticrecipe[985]={recipe_id=1254,recipe_group=0,recipe_level=10,recipe_type=4}---[144]五煞命延返魂靴[男]
		staticrecipe[986]={recipe_id=1255,recipe_group=0,recipe_level=10,recipe_type=4}---[144]五煞命延返魂靴[女]
		staticrecipe[987]={recipe_id=1258,recipe_group=0,recipe_level=8,recipe_type=4}---[120]五狱狂灵靴[男]
		staticrecipe[988]={recipe_id=1259,recipe_group=0,recipe_level=8,recipe_type=4}---[120]五狱狂灵靴[女]
		staticrecipe[989]={recipe_id=1260,recipe_group=0,recipe_level=9,recipe_type=4}---[135]蛮城锦带黄金靴[男]
		staticrecipe[990]={recipe_id=1261,recipe_group=0,recipe_level=9,recipe_type=4}---[135]蛮城锦带黄金靴[女]
		staticrecipe[991]={recipe_id=1262,recipe_group=0,recipe_level=10,recipe_type=4}---[144]九幽酆都鬼灵靴[男]
		staticrecipe[992]={recipe_id=1263,recipe_group=0,recipe_level=10,recipe_type=4}---[144]九幽酆都鬼灵靴[女]
		staticrecipe[993]={recipe_id=1266,recipe_group=0,recipe_level=8,recipe_type=4}---[120]九兽赤鳞靴[男]
		staticrecipe[994]={recipe_id=1267,recipe_group=0,recipe_level=8,recipe_type=4}---[120]九兽赤鳞靴[女]
		staticrecipe[995]={recipe_id=1268,recipe_group=0,recipe_level=9,recipe_type=4}---[135]明尊天佑邪灵靴[男]
		staticrecipe[996]={recipe_id=1269,recipe_group=0,recipe_level=9,recipe_type=4}---[135]明尊天佑邪灵靴[女]
		staticrecipe[997]={recipe_id=1206,recipe_group=0,recipe_level=10,recipe_type=4}---[144]九魔枭天冥魂靴[男]
		staticrecipe[998]={recipe_id=1207,recipe_group=0,recipe_level=10,recipe_type=4}---[144]九魔枭天冥魂靴[女]
		staticrecipe[999]={recipe_id=1210,recipe_group=0,recipe_level=8,recipe_type=4}---[120]九转焚心靴[男]
		staticrecipe[1000]={recipe_id=1211,recipe_group=0,recipe_level=8,recipe_type=4}---[120]九转焚心靴[女]
		staticrecipe[1001]={recipe_id=1212,recipe_group=0,recipe_level=9,recipe_type=4}---[135]离魂鬼契克劫靴[男]
		staticrecipe[1002]={recipe_id=1213,recipe_group=0,recipe_level=9,recipe_type=4}---[135]离魂鬼契克劫靴[女]
		staticrecipe[1003]={recipe_id=287,recipe_group=0,recipe_level=10,recipe_type=4}---[144]绝艳靴[男]
		staticrecipe[1004]={recipe_id=335,recipe_group=0,recipe_level=10,recipe_type=4}---[144]绝艳靴[女]
		staticrecipe[1005]={recipe_id=285,recipe_group=0,recipe_level=8,recipe_type=4}---[120]销魂靴[男]
		staticrecipe[1006]={recipe_id=333,recipe_group=0,recipe_level=8,recipe_type=4}---[120]销魂靴[女]
		staticrecipe[1007]={recipe_id=286,recipe_group=0,recipe_level=9,recipe_type=4}---[135]惊才靴[男]
		staticrecipe[1008]={recipe_id=334,recipe_group=0,recipe_level=9,recipe_type=4}---[135]惊才靴[女]
		staticrecipe[1009]={recipe_id=1278,recipe_group=0,recipe_level=10,recipe_type=4}---[144]三弄梅花妙叹靴[男]
		staticrecipe[1010]={recipe_id=1279,recipe_group=0,recipe_level=10,recipe_type=4}---[144]三弄梅花妙叹靴[女]
		staticrecipe[1011]={recipe_id=1282,recipe_group=0,recipe_level=8,recipe_type=4}---[120]三尾玄狐靴[男]
		staticrecipe[1012]={recipe_id=1283,recipe_group=0,recipe_level=8,recipe_type=4}---[120]三尾玄狐靴[女]
		staticrecipe[1013]={recipe_id=1284,recipe_group=0,recipe_level=9,recipe_type=4}---[135]西川红锦百花靴[男]
		staticrecipe[1014]={recipe_id=1285,recipe_group=0,recipe_level=9,recipe_type=4}---[135]西川红锦百花靴[女]
		staticrecipe[1015]={recipe_id=1286,recipe_group=0,recipe_level=10,recipe_type=4}---[144]六叶妖娆金莲靴[男]
		staticrecipe[1016]={recipe_id=1287,recipe_group=0,recipe_level=10,recipe_type=4}---[144]六叶妖娆金莲靴[女]
		staticrecipe[1017]={recipe_id=1290,recipe_group=0,recipe_level=8,recipe_type=4}---[120]六道荡魂靴[男]
		staticrecipe[1018]={recipe_id=1291,recipe_group=0,recipe_level=8,recipe_type=4}---[120]六道荡魂靴[女]
		staticrecipe[1019]={recipe_id=1292,recipe_group=0,recipe_level=9,recipe_type=4}---[135]幻彩华光金缕靴[男]
		staticrecipe[1020]={recipe_id=1293,recipe_group=0,recipe_level=9,recipe_type=4}---[135]幻彩华光金缕靴[女]
		staticrecipe[1021]={recipe_id=1294,recipe_group=0,recipe_level=10,recipe_type=4}---[144]七彩莹华冰心靴[男]
		staticrecipe[1022]={recipe_id=1295,recipe_group=0,recipe_level=10,recipe_type=4}---[144]七彩莹华冰心靴[女]
		staticrecipe[1023]={recipe_id=1298,recipe_group=0,recipe_level=8,recipe_type=4}---[120]七宝玲珑靴[男]
		staticrecipe[1024]={recipe_id=1299,recipe_group=0,recipe_level=8,recipe_type=4}---[120]七宝玲珑靴[女]
		staticrecipe[1025]={recipe_id=1300,recipe_group=0,recipe_level=9,recipe_type=4}---[135]落花逐水困情靴[男]
		staticrecipe[1026]={recipe_id=1301,recipe_group=0,recipe_level=9,recipe_type=4}---[135]落花逐水困情靴[女]
		staticrecipe[1027]={recipe_id=1302,recipe_group=0,recipe_level=10,recipe_type=4}---[144]四野漫歌铭情靴[男]
		staticrecipe[1028]={recipe_id=1303,recipe_group=0,recipe_level=10,recipe_type=4}---[144]四野漫歌铭情靴[女]
		staticrecipe[1029]={recipe_id=1306,recipe_group=0,recipe_level=8,recipe_type=4}---[120]四圣步空靴[男]
		staticrecipe[1030]={recipe_id=1307,recipe_group=0,recipe_level=8,recipe_type=4}---[120]四圣步空靴[女]
		staticrecipe[1031]={recipe_id=1308,recipe_group=0,recipe_level=9,recipe_type=4}---[135]落霞弥空霓彩靴[男]
		staticrecipe[1032]={recipe_id=1309,recipe_group=0,recipe_level=9,recipe_type=4}---[135]落霞弥空霓彩靴[女]
		staticrecipe[1033]={recipe_id=1310,recipe_group=0,recipe_level=10,recipe_type=4}---[144]八宝夜明珍珠靴[男]
		staticrecipe[1034]={recipe_id=1311,recipe_group=0,recipe_level=10,recipe_type=4}---[144]八宝夜明珍珠靴[女]
		staticrecipe[1035]={recipe_id=1314,recipe_group=0,recipe_level=8,recipe_type=4}---[120]八荒冀云靴[男]
		staticrecipe[1036]={recipe_id=1315,recipe_group=0,recipe_level=8,recipe_type=4}---[120]八荒冀云靴[女]
		staticrecipe[1037]={recipe_id=1316,recipe_group=0,recipe_level=9,recipe_type=4}---[135]临凡谪仙逍遥靴[男]
		staticrecipe[1038]={recipe_id=1317,recipe_group=0,recipe_level=9,recipe_type=4}---[135]临凡谪仙逍遥靴[女]
		staticrecipe[1039]={recipe_id=1318,recipe_group=0,recipe_level=10,recipe_type=4}---[144]五岳云岚傲天靴[男]
		staticrecipe[1040]={recipe_id=1319,recipe_group=0,recipe_level=10,recipe_type=4}---[144]五岳云岚傲天靴[女]
		staticrecipe[1041]={recipe_id=1322,recipe_group=0,recipe_level=8,recipe_type=4}---[120]五色华光靴[男]
		staticrecipe[1042]={recipe_id=1323,recipe_group=0,recipe_level=8,recipe_type=4}---[120]五色华光靴[女]
		staticrecipe[1043]={recipe_id=1324,recipe_group=0,recipe_level=9,recipe_type=4}---[135]太上忘情炼心靴[男]
		staticrecipe[1044]={recipe_id=1325,recipe_group=0,recipe_level=9,recipe_type=4}---[135]太上忘情炼心靴[女]
		staticrecipe[1045]={recipe_id=1326,recipe_group=0,recipe_level=10,recipe_type=4}---[144]九幻迷天绛绡靴[男]
		staticrecipe[1046]={recipe_id=1327,recipe_group=0,recipe_level=10,recipe_type=4}---[144]九幻迷天绛绡靴[女]
		staticrecipe[1047]={recipe_id=1330,recipe_group=0,recipe_level=8,recipe_type=4}---[120]九焕金纱靴[男]
		staticrecipe[1048]={recipe_id=1331,recipe_group=0,recipe_level=8,recipe_type=4}---[120]九焕金纱靴[女]
		staticrecipe[1049]={recipe_id=1332,recipe_group=0,recipe_level=9,recipe_type=4}---[135]琼台天籁承欢靴[男]
		staticrecipe[1050]={recipe_id=1333,recipe_group=0,recipe_level=9,recipe_type=4}---[135]琼台天籁承欢靴[女]
		staticrecipe[1051]={recipe_id=1270,recipe_group=0,recipe_level=10,recipe_type=4}---[144]九宵琼台连玉靴[男]
		staticrecipe[1052]={recipe_id=1271,recipe_group=0,recipe_level=10,recipe_type=4}---[144]九宵琼台连玉靴[女]
		staticrecipe[1053]={recipe_id=1274,recipe_group=0,recipe_level=8,recipe_type=4}---[120]九华惑心靴[男]
		staticrecipe[1054]={recipe_id=1275,recipe_group=0,recipe_level=8,recipe_type=4}---[120]九华惑心靴[女]
		staticrecipe[1055]={recipe_id=1276,recipe_group=0,recipe_level=9,recipe_type=4}---[135]远峦接天离愁靴[男]
		staticrecipe[1056]={recipe_id=1277,recipe_group=0,recipe_level=9,recipe_type=4}---[135]远峦接天离愁靴[女]
		staticrecipe[1057]={recipe_id=3744,recipe_group=0,recipe_level=10,recipe_type=4}---100级九黎鞋男配方
		staticrecipe[1058]={recipe_id=3810,recipe_group=0,recipe_level=10,recipe_type=4}---100级九黎鞋女配方
		staticrecipe[1059]={recipe_id=3742,recipe_group=0,recipe_level=8,recipe_type=4}---80级九黎鞋男配方
		staticrecipe[1060]={recipe_id=3808,recipe_group=0,recipe_level=8,recipe_type=4}---80级九黎鞋女配方
		staticrecipe[1061]={recipe_id=3743,recipe_group=0,recipe_level=9,recipe_type=4}---90级九黎鞋男配方
		staticrecipe[1062]={recipe_id=3809,recipe_group=0,recipe_level=9,recipe_type=4}---90级九黎鞋女配方
		staticrecipe[1063]={recipe_id=3751,recipe_group=0,recipe_level=10,recipe_type=4}---逆天100级九黎鞋男配方
		staticrecipe[1064]={recipe_id=3817,recipe_group=0,recipe_level=10,recipe_type=4}---逆天100级九黎鞋女配方
		staticrecipe[1065]={recipe_id=3749,recipe_group=0,recipe_level=8,recipe_type=4}---逆天80级九黎鞋男配方
		staticrecipe[1066]={recipe_id=3815,recipe_group=0,recipe_level=8,recipe_type=4}---逆天80级九黎鞋女配方
		staticrecipe[1067]={recipe_id=3750,recipe_group=0,recipe_level=9,recipe_type=4}---逆天90级九黎鞋男配方
		staticrecipe[1068]={recipe_id=3816,recipe_group=0,recipe_level=9,recipe_type=4}---逆天90级九黎鞋女配方
		staticrecipe[1069]={recipe_id=3765,recipe_group=0,recipe_level=10,recipe_type=4}---逆天防御装100级九黎鞋男配方
		staticrecipe[1070]={recipe_id=3831,recipe_group=0,recipe_level=10,recipe_type=4}---逆天防御装100级九黎鞋女配方
		staticrecipe[1071]={recipe_id=3763,recipe_group=0,recipe_level=8,recipe_type=4}---逆天防御装80级九黎鞋男配方
		staticrecipe[1072]={recipe_id=3829,recipe_group=0,recipe_level=8,recipe_type=4}---逆天防御装80级九黎鞋女配方
		staticrecipe[1073]={recipe_id=3764,recipe_group=0,recipe_level=9,recipe_type=4}---逆天防御装90级九黎鞋男配方
		staticrecipe[1074]={recipe_id=3830,recipe_group=0,recipe_level=9,recipe_type=4}---逆天防御装90级九黎鞋女配方
		staticrecipe[1075]={recipe_id=3786,recipe_group=0,recipe_level=10,recipe_type=4}---逆天抗性装丙100级九黎鞋男配方
		staticrecipe[1076]={recipe_id=3852,recipe_group=0,recipe_level=10,recipe_type=4}---逆天抗性装丙100级九黎鞋女配方
		staticrecipe[1077]={recipe_id=3784,recipe_group=0,recipe_level=8,recipe_type=4}---逆天抗性装丙80级九黎鞋男配方
		staticrecipe[1078]={recipe_id=3850,recipe_group=0,recipe_level=8,recipe_type=4}---逆天抗性装丙80级九黎鞋女配方
		staticrecipe[1079]={recipe_id=3785,recipe_group=0,recipe_level=9,recipe_type=4}---逆天抗性装丙90级九黎鞋男配方
		staticrecipe[1080]={recipe_id=3851,recipe_group=0,recipe_level=9,recipe_type=4}---逆天抗性装丙90级九黎鞋女配方
		staticrecipe[1081]={recipe_id=3793,recipe_group=0,recipe_level=10,recipe_type=4}---逆天抗性装丁100级九黎鞋男配方
		staticrecipe[1082]={recipe_id=3859,recipe_group=0,recipe_level=10,recipe_type=4}---逆天抗性装丁100级九黎鞋女配方
		staticrecipe[1083]={recipe_id=3791,recipe_group=0,recipe_level=8,recipe_type=4}---逆天抗性装丁80级九黎鞋男配方
		staticrecipe[1084]={recipe_id=3857,recipe_group=0,recipe_level=8,recipe_type=4}---逆天抗性装丁80级九黎鞋女配方
		staticrecipe[1085]={recipe_id=3792,recipe_group=0,recipe_level=9,recipe_type=4}---逆天抗性装丁90级九黎鞋男配方
		staticrecipe[1086]={recipe_id=3858,recipe_group=0,recipe_level=9,recipe_type=4}---逆天抗性装丁90级九黎鞋女配方
		staticrecipe[1087]={recipe_id=3772,recipe_group=0,recipe_level=10,recipe_type=4}---逆天抗性装甲100级九黎鞋男配方
		staticrecipe[1088]={recipe_id=3838,recipe_group=0,recipe_level=10,recipe_type=4}---逆天抗性装甲100级九黎鞋女配方
		staticrecipe[1089]={recipe_id=3770,recipe_group=0,recipe_level=8,recipe_type=4}---逆天抗性装甲80级九黎鞋男配方
		staticrecipe[1090]={recipe_id=3836,recipe_group=0,recipe_level=8,recipe_type=4}---逆天抗性装甲80级九黎鞋女配方
		staticrecipe[1091]={recipe_id=3771,recipe_group=0,recipe_level=9,recipe_type=4}---逆天抗性装甲90级九黎鞋男配方
		staticrecipe[1092]={recipe_id=3837,recipe_group=0,recipe_level=9,recipe_type=4}---逆天抗性装甲90级九黎鞋女配方
		staticrecipe[1093]={recipe_id=3800,recipe_group=0,recipe_level=10,recipe_type=4}---逆天抗性装戊100级九黎鞋男配方
		staticrecipe[1094]={recipe_id=3866,recipe_group=0,recipe_level=10,recipe_type=4}---逆天抗性装戊100级九黎鞋女配方
		staticrecipe[1095]={recipe_id=3798,recipe_group=0,recipe_level=8,recipe_type=4}---逆天抗性装戊80级九黎鞋男配方
		staticrecipe[1096]={recipe_id=3864,recipe_group=0,recipe_level=8,recipe_type=4}---逆天抗性装戊80级九黎鞋女配方
		staticrecipe[1097]={recipe_id=3799,recipe_group=0,recipe_level=9,recipe_type=4}---逆天抗性装戊90级九黎鞋男配方
		staticrecipe[1098]={recipe_id=3865,recipe_group=0,recipe_level=9,recipe_type=4}---逆天抗性装戊90级九黎鞋女配方
		staticrecipe[1099]={recipe_id=3779,recipe_group=0,recipe_level=10,recipe_type=4}---逆天抗性装乙100级九黎鞋男配方
		staticrecipe[1100]={recipe_id=3845,recipe_group=0,recipe_level=10,recipe_type=4}---逆天抗性装乙100级九黎鞋女配方
		staticrecipe[1101]={recipe_id=3777,recipe_group=0,recipe_level=8,recipe_type=4}---逆天抗性装乙80级九黎鞋男配方
		staticrecipe[1102]={recipe_id=3843,recipe_group=0,recipe_level=8,recipe_type=4}---逆天抗性装乙80级九黎鞋女配方
		staticrecipe[1103]={recipe_id=3778,recipe_group=0,recipe_level=9,recipe_type=4}---逆天抗性装乙90级九黎鞋男配方
		staticrecipe[1104]={recipe_id=3844,recipe_group=0,recipe_level=9,recipe_type=4}---逆天抗性装乙90级九黎鞋女配方
		staticrecipe[1105]={recipe_id=3758,recipe_group=0,recipe_level=10,recipe_type=4}---逆天魔法装100级九黎鞋男配方
		staticrecipe[1106]={recipe_id=3824,recipe_group=0,recipe_level=10,recipe_type=4}---逆天魔法装100级九黎鞋女配方
		staticrecipe[1107]={recipe_id=3756,recipe_group=0,recipe_level=8,recipe_type=4}---逆天魔法装80级九黎鞋男配方
		staticrecipe[1108]={recipe_id=3822,recipe_group=0,recipe_level=8,recipe_type=4}---逆天魔法装80级九黎鞋女配方
		staticrecipe[1109]={recipe_id=3757,recipe_group=0,recipe_level=9,recipe_type=4}---逆天魔法装90级九黎鞋男配方
		staticrecipe[1110]={recipe_id=3823,recipe_group=0,recipe_level=9,recipe_type=4}---逆天魔法装90级九黎鞋女配方
		staticrecipe[1111]={recipe_id=4152,recipe_group=0,recipe_level=10,recipe_type=4}---100级烈山鞋男配方
		staticrecipe[1112]={recipe_id=4218,recipe_group=0,recipe_level=10,recipe_type=4}---100级烈山鞋女配方
		staticrecipe[1113]={recipe_id=4150,recipe_group=0,recipe_level=8,recipe_type=4}---80级烈山鞋男配方
		staticrecipe[1114]={recipe_id=4216,recipe_group=0,recipe_level=8,recipe_type=4}---80级烈山鞋女配方
		staticrecipe[1115]={recipe_id=4151,recipe_group=0,recipe_level=9,recipe_type=4}---90级烈山鞋男配方
		staticrecipe[1116]={recipe_id=4217,recipe_group=0,recipe_level=9,recipe_type=4}---90级烈山鞋女配方
		staticrecipe[1117]={recipe_id=4159,recipe_group=0,recipe_level=10,recipe_type=4}---逆天100级烈山鞋男配方
		staticrecipe[1118]={recipe_id=4225,recipe_group=0,recipe_level=10,recipe_type=4}---逆天100级烈山鞋女配方
		staticrecipe[1119]={recipe_id=4157,recipe_group=0,recipe_level=8,recipe_type=4}---逆天80级烈山鞋男配方
		staticrecipe[1120]={recipe_id=4223,recipe_group=0,recipe_level=8,recipe_type=4}---逆天80级烈山鞋女配方
		staticrecipe[1121]={recipe_id=4158,recipe_group=0,recipe_level=9,recipe_type=4}---逆天90级烈山鞋男配方
		staticrecipe[1122]={recipe_id=4224,recipe_group=0,recipe_level=9,recipe_type=4}---逆天90级烈山鞋女配方
		staticrecipe[1123]={recipe_id=4173,recipe_group=0,recipe_level=10,recipe_type=4}---逆天防御装100级烈山鞋男配方
		staticrecipe[1124]={recipe_id=4239,recipe_group=0,recipe_level=10,recipe_type=4}---逆天防御装100级烈山鞋女配方
		staticrecipe[1125]={recipe_id=4171,recipe_group=0,recipe_level=8,recipe_type=4}---逆天防御装80级烈山鞋男配方
		staticrecipe[1126]={recipe_id=4237,recipe_group=0,recipe_level=8,recipe_type=4}---逆天防御装80级烈山鞋女配方
		staticrecipe[1127]={recipe_id=4172,recipe_group=0,recipe_level=9,recipe_type=4}---逆天防御装90级烈山鞋男配方
		staticrecipe[1128]={recipe_id=4238,recipe_group=0,recipe_level=9,recipe_type=4}---逆天防御装90级烈山鞋女配方
		staticrecipe[1129]={recipe_id=4194,recipe_group=0,recipe_level=10,recipe_type=4}---逆天抗性装丙100级烈山鞋男配方
		staticrecipe[1130]={recipe_id=4260,recipe_group=0,recipe_level=10,recipe_type=4}---逆天抗性装丙100级烈山鞋女配方
		staticrecipe[1131]={recipe_id=4192,recipe_group=0,recipe_level=8,recipe_type=4}---逆天抗性装丙80级烈山鞋男配方
		staticrecipe[1132]={recipe_id=4258,recipe_group=0,recipe_level=8,recipe_type=4}---逆天抗性装丙80级烈山鞋女配方
		staticrecipe[1133]={recipe_id=4193,recipe_group=0,recipe_level=9,recipe_type=4}---逆天抗性装丙90级烈山鞋男配方
		staticrecipe[1134]={recipe_id=4259,recipe_group=0,recipe_level=9,recipe_type=4}---逆天抗性装丙90级烈山鞋女配方
		staticrecipe[1135]={recipe_id=4201,recipe_group=0,recipe_level=10,recipe_type=4}---逆天抗性装丁100级烈山鞋男配方
		staticrecipe[1136]={recipe_id=4267,recipe_group=0,recipe_level=10,recipe_type=4}---逆天抗性装丁100级烈山鞋女配方
		staticrecipe[1137]={recipe_id=4199,recipe_group=0,recipe_level=8,recipe_type=4}---逆天抗性装丁80级烈山鞋男配方
		staticrecipe[1138]={recipe_id=4265,recipe_group=0,recipe_level=8,recipe_type=4}---逆天抗性装丁80级烈山鞋女配方
		staticrecipe[1139]={recipe_id=4200,recipe_group=0,recipe_level=9,recipe_type=4}---逆天抗性装丁90级烈山鞋男配方
		staticrecipe[1140]={recipe_id=4266,recipe_group=0,recipe_level=9,recipe_type=4}---逆天抗性装丁90级烈山鞋女配方
		staticrecipe[1141]={recipe_id=4180,recipe_group=0,recipe_level=10,recipe_type=4}---逆天抗性装甲100级烈山鞋男配方
		staticrecipe[1142]={recipe_id=4246,recipe_group=0,recipe_level=10,recipe_type=4}---逆天抗性装甲100级烈山鞋女配方
		staticrecipe[1143]={recipe_id=4178,recipe_group=0,recipe_level=8,recipe_type=4}---逆天抗性装甲80级烈山鞋男配方
		staticrecipe[1144]={recipe_id=4244,recipe_group=0,recipe_level=8,recipe_type=4}---逆天抗性装甲80级烈山鞋女配方
		staticrecipe[1145]={recipe_id=4179,recipe_group=0,recipe_level=9,recipe_type=4}---逆天抗性装甲90级烈山鞋男配方
		staticrecipe[1146]={recipe_id=4245,recipe_group=0,recipe_level=9,recipe_type=4}---逆天抗性装甲90级烈山鞋女配方
		staticrecipe[1147]={recipe_id=4208,recipe_group=0,recipe_level=10,recipe_type=4}---逆天抗性装戊100级烈山鞋男配方
		staticrecipe[1148]={recipe_id=4274,recipe_group=0,recipe_level=10,recipe_type=4}---逆天抗性装戊100级烈山鞋女配方
		staticrecipe[1149]={recipe_id=4206,recipe_group=0,recipe_level=8,recipe_type=4}---逆天抗性装戊80级烈山鞋男配方
		staticrecipe[1150]={recipe_id=4272,recipe_group=0,recipe_level=8,recipe_type=4}---逆天抗性装戊80级烈山鞋女配方
		staticrecipe[1151]={recipe_id=4207,recipe_group=0,recipe_level=9,recipe_type=4}---逆天抗性装戊90级烈山鞋男配方
		staticrecipe[1152]={recipe_id=4273,recipe_group=0,recipe_level=9,recipe_type=4}---逆天抗性装戊90级烈山鞋女配方
		staticrecipe[1153]={recipe_id=4187,recipe_group=0,recipe_level=10,recipe_type=4}---逆天抗性装乙100级烈山鞋男配方
		staticrecipe[1154]={recipe_id=4253,recipe_group=0,recipe_level=10,recipe_type=4}---逆天抗性装乙100级烈山鞋女配方
		staticrecipe[1155]={recipe_id=4185,recipe_group=0,recipe_level=8,recipe_type=4}---逆天抗性装乙80级烈山鞋男配方
		staticrecipe[1156]={recipe_id=4251,recipe_group=0,recipe_level=8,recipe_type=4}---逆天抗性装乙80级烈山鞋女配方
		staticrecipe[1157]={recipe_id=4186,recipe_group=0,recipe_level=9,recipe_type=4}---逆天抗性装乙90级烈山鞋男配方
		staticrecipe[1158]={recipe_id=4252,recipe_group=0,recipe_level=9,recipe_type=4}---逆天抗性装乙90级烈山鞋女配方
		staticrecipe[1159]={recipe_id=4166,recipe_group=0,recipe_level=10,recipe_type=4}---逆天魔法装100级烈山鞋男配方
		staticrecipe[1160]={recipe_id=4232,recipe_group=0,recipe_level=10,recipe_type=4}---逆天魔法装100级烈山鞋女配方
		staticrecipe[1161]={recipe_id=4164,recipe_group=0,recipe_level=8,recipe_type=4}---逆天魔法装80级烈山鞋男配方
		staticrecipe[1162]={recipe_id=4230,recipe_group=0,recipe_level=8,recipe_type=4}---逆天魔法装80级烈山鞋女配方
		staticrecipe[1163]={recipe_id=4165,recipe_group=0,recipe_level=9,recipe_type=4}---逆天魔法装90级烈山鞋男配方
		staticrecipe[1164]={recipe_id=4231,recipe_group=0,recipe_level=9,recipe_type=4}---逆天魔法装90级烈山鞋女配方
		staticrecipe[1165]={recipe_id=291,recipe_group=0,recipe_level=10,recipe_type=4}---[144]玉清履[男]
		staticrecipe[1166]={recipe_id=339,recipe_group=0,recipe_level=10,recipe_type=4}---[144]玉清履[女]
		staticrecipe[1167]={recipe_id=289,recipe_group=0,recipe_level=8,recipe_type=4}---[120]太清履[男]
		staticrecipe[1168]={recipe_id=337,recipe_group=0,recipe_level=8,recipe_type=4}---[120]太清履[女]
		staticrecipe[1169]={recipe_id=290,recipe_group=0,recipe_level=9,recipe_type=4}---[135]上清履[男]
		staticrecipe[1170]={recipe_id=338,recipe_group=0,recipe_level=9,recipe_type=4}---[135]上清履[女]
		staticrecipe[1171]={recipe_id=1342,recipe_group=0,recipe_level=10,recipe_type=4}---[144]三皇敕令道元履[男]
		staticrecipe[1172]={recipe_id=1343,recipe_group=0,recipe_level=10,recipe_type=4}---[144]三皇敕令道元履[女]
		staticrecipe[1173]={recipe_id=1346,recipe_group=0,recipe_level=8,recipe_type=4}---[120]三仙证道履[男]
		staticrecipe[1174]={recipe_id=1347,recipe_group=0,recipe_level=8,recipe_type=4}---[120]三仙证道履[女]
		staticrecipe[1175]={recipe_id=1348,recipe_group=0,recipe_level=9,recipe_type=4}---[135]众妙参玄道尊履[男]
		staticrecipe[1176]={recipe_id=1349,recipe_group=0,recipe_level=9,recipe_type=4}---[135]众妙参玄道尊履[女]
		staticrecipe[1177]={recipe_id=1350,recipe_group=0,recipe_level=10,recipe_type=4}---[144]六气遁法天机履[男]
		staticrecipe[1178]={recipe_id=1351,recipe_group=0,recipe_level=10,recipe_type=4}---[144]六气遁法天机履[女]
		staticrecipe[1179]={recipe_id=1354,recipe_group=0,recipe_level=8,recipe_type=4}---[120]六爻占命履[男]
		staticrecipe[1180]={recipe_id=1355,recipe_group=0,recipe_level=8,recipe_type=4}---[120]六爻占命履[女]
		staticrecipe[1181]={recipe_id=1356,recipe_group=0,recipe_level=9,recipe_type=4}---[135]灵机玄刹玉真履[男]
		staticrecipe[1182]={recipe_id=1357,recipe_group=0,recipe_level=9,recipe_type=4}---[135]灵机玄刹玉真履[女]
		staticrecipe[1183]={recipe_id=1358,recipe_group=0,recipe_level=10,recipe_type=4}---[144]七法悟玄神藏履[男]
		staticrecipe[1184]={recipe_id=1359,recipe_group=0,recipe_level=10,recipe_type=4}---[144]七法悟玄神藏履[女]
		staticrecipe[1185]={recipe_id=1362,recipe_group=0,recipe_level=8,recipe_type=4}---[120]七宿驻灵履[男]
		staticrecipe[1186]={recipe_id=1363,recipe_group=0,recipe_level=8,recipe_type=4}---[120]七宿驻灵履[女]
		staticrecipe[1187]={recipe_id=1364,recipe_group=0,recipe_level=9,recipe_type=4}---[135]经天星君命元履[男]
		staticrecipe[1188]={recipe_id=1365,recipe_group=0,recipe_level=9,recipe_type=4}---[135]经天星君命元履[女]
		staticrecipe[1189]={recipe_id=1366,recipe_group=0,recipe_level=10,recipe_type=4}---[144]四圣法谍固灵履[男]
		staticrecipe[1190]={recipe_id=1367,recipe_group=0,recipe_level=10,recipe_type=4}---[144]四圣法谍固灵履[女]
		staticrecipe[1191]={recipe_id=1370,recipe_group=0,recipe_level=8,recipe_type=4}---[120]四海游龙履[男]
		staticrecipe[1192]={recipe_id=1371,recipe_group=0,recipe_level=8,recipe_type=4}---[120]四海游龙履[女]
		staticrecipe[1193]={recipe_id=1372,recipe_group=0,recipe_level=9,recipe_type=4}---[135]玉鼎结丹化圣履[男]
		staticrecipe[1194]={recipe_id=1373,recipe_group=0,recipe_level=9,recipe_type=4}---[135]玉鼎结丹化圣履[女]
		staticrecipe[1195]={recipe_id=1374,recipe_group=0,recipe_level=10,recipe_type=4}---[144]八门金锁定心履[男]
		staticrecipe[1196]={recipe_id=1375,recipe_group=0,recipe_level=10,recipe_type=4}---[144]八门金锁定心履[女]
		staticrecipe[1197]={recipe_id=1378,recipe_group=0,recipe_level=8,recipe_type=4}---[120]八极玉阳履[男]
		staticrecipe[1198]={recipe_id=1379,recipe_group=0,recipe_level=8,recipe_type=4}---[120]八极玉阳履[女]
		staticrecipe[1199]={recipe_id=1380,recipe_group=0,recipe_level=9,recipe_type=4}---[135]秘法通玄洞冥履[男]
		staticrecipe[1200]={recipe_id=1381,recipe_group=0,recipe_level=9,recipe_type=4}---[135]秘法通玄洞冥履[女]
		staticrecipe[1201]={recipe_id=1382,recipe_group=0,recipe_level=10,recipe_type=4}---[144]五帝诏谕玄极履[男]
		staticrecipe[1202]={recipe_id=1383,recipe_group=0,recipe_level=10,recipe_type=4}---[144]五帝诏谕玄极履[女]
		staticrecipe[1203]={recipe_id=1386,recipe_group=0,recipe_level=8,recipe_type=4}---[120]五元固神履[男]
		staticrecipe[1204]={recipe_id=1387,recipe_group=0,recipe_level=8,recipe_type=4}---[120]五元固神履[女]
		staticrecipe[1205]={recipe_id=1388,recipe_group=0,recipe_level=9,recipe_type=4}---[135]紫阳初照掸心履[男]
		staticrecipe[1206]={recipe_id=1389,recipe_group=0,recipe_level=9,recipe_type=4}---[135]紫阳初照掸心履[女]
		staticrecipe[1207]={recipe_id=1390,recipe_group=0,recipe_level=10,recipe_type=4}---[144]九刹疾雷浩光履[男]
		staticrecipe[1208]={recipe_id=1391,recipe_group=0,recipe_level=10,recipe_type=4}---[144]九刹疾雷浩光履[女]
		staticrecipe[1209]={recipe_id=1394,recipe_group=0,recipe_level=8,recipe_type=4}---[120]九英聚元履[男]
		staticrecipe[1210]={recipe_id=1395,recipe_group=0,recipe_level=8,recipe_type=4}---[120]九英聚元履[女]
		staticrecipe[1211]={recipe_id=1396,recipe_group=0,recipe_level=9,recipe_type=4}---[135]积元贮本弥金履[男]
		staticrecipe[1212]={recipe_id=1397,recipe_group=0,recipe_level=9,recipe_type=4}---[135]积元贮本弥金履[女]
		staticrecipe[1213]={recipe_id=1334,recipe_group=0,recipe_level=10,recipe_type=4}---[144]九晶聚瑞天怒履[男]
		staticrecipe[1214]={recipe_id=1335,recipe_group=0,recipe_level=10,recipe_type=4}---[144]九晶聚瑞天怒履[女]
		staticrecipe[1215]={recipe_id=1338,recipe_group=0,recipe_level=8,recipe_type=4}---[120]九阳洞仙履[男]
		staticrecipe[1216]={recipe_id=1339,recipe_group=0,recipe_level=8,recipe_type=4}---[120]九阳洞仙履[女]
		staticrecipe[1217]={recipe_id=1340,recipe_group=0,recipe_level=9,recipe_type=4}---[135]云门天官锦罗履[男]
		staticrecipe[1218]={recipe_id=1341,recipe_group=0,recipe_level=9,recipe_type=4}---[135]云门天官锦罗履[女]
		staticrecipe[1219]={recipe_id=295,recipe_group=0,recipe_level=10,recipe_type=4}---[144]菩提履[男]
		staticrecipe[1220]={recipe_id=343,recipe_group=0,recipe_level=10,recipe_type=4}---[144]菩提履[女]
		staticrecipe[1221]={recipe_id=293,recipe_group=0,recipe_level=8,recipe_type=4}---[120]无相履[男]
		staticrecipe[1222]={recipe_id=341,recipe_group=0,recipe_level=8,recipe_type=4}---[120]无相履[女]
		staticrecipe[1223]={recipe_id=294,recipe_group=0,recipe_level=9,recipe_type=4}---[135]龙象履[男]
		staticrecipe[1224]={recipe_id=342,recipe_group=0,recipe_level=9,recipe_type=4}---[135]龙象履[女]
		staticrecipe[1225]={recipe_id=1406,recipe_group=0,recipe_level=10,recipe_type=4}---[144]三世娑婆往生履[男]
		staticrecipe[1226]={recipe_id=1407,recipe_group=0,recipe_level=10,recipe_type=4}---[144]三世娑婆往生履[女]
		staticrecipe[1227]={recipe_id=1410,recipe_group=0,recipe_level=8,recipe_type=4}---[120]三皈妙悟履[男]
		staticrecipe[1228]={recipe_id=1411,recipe_group=0,recipe_level=8,recipe_type=4}---[120]三皈妙悟履[女]
		staticrecipe[1229]={recipe_id=1412,recipe_group=0,recipe_level=9,recipe_type=4}---[135]毗卢沙门尊者履[男]
		staticrecipe[1230]={recipe_id=1413,recipe_group=0,recipe_level=9,recipe_type=4}---[135]毗卢沙门尊者履[女]
		staticrecipe[1231]={recipe_id=1414,recipe_group=0,recipe_level=10,recipe_type=4}---[144]六觉无量功德履[男]
		staticrecipe[1232]={recipe_id=1415,recipe_group=0,recipe_level=10,recipe_type=4}---[144]六觉无量功德履[女]
		staticrecipe[1233]={recipe_id=1418,recipe_group=0,recipe_level=8,recipe_type=4}---[120]六言真诀履[男]
		staticrecipe[1234]={recipe_id=1419,recipe_group=0,recipe_level=8,recipe_type=4}---[120]六言真诀履[女]
		staticrecipe[1235]={recipe_id=1420,recipe_group=0,recipe_level=9,recipe_type=4}---[135]无业宿尽伽蓝履[男]
		staticrecipe[1236]={recipe_id=1421,recipe_group=0,recipe_level=9,recipe_type=4}---[135]无业宿尽伽蓝履[女]
		staticrecipe[1237]={recipe_id=1422,recipe_group=0,recipe_level=10,recipe_type=4}---[144]七劫轮回寂灭履[男]
		staticrecipe[1238]={recipe_id=1423,recipe_group=0,recipe_level=10,recipe_type=4}---[144]七劫轮回寂灭履[女]
		staticrecipe[1239]={recipe_id=1426,recipe_group=0,recipe_level=8,recipe_type=4}---[120]七法梵音履[男]
		staticrecipe[1240]={recipe_id=1427,recipe_group=0,recipe_level=8,recipe_type=4}---[120]七法梵音履[女]
		staticrecipe[1241]={recipe_id=1428,recipe_group=0,recipe_level=9,recipe_type=4}---[135]无量本行大士履[男]
		staticrecipe[1242]={recipe_id=1429,recipe_group=0,recipe_level=9,recipe_type=4}---[135]无量本行大士履[女]
		staticrecipe[1243]={recipe_id=1430,recipe_group=0,recipe_level=10,recipe_type=4}---[144]四相无畏光明履[男]
		staticrecipe[1244]={recipe_id=1431,recipe_group=0,recipe_level=10,recipe_type=4}---[144]四相无畏光明履[女]
		staticrecipe[1245]={recipe_id=1434,recipe_group=0,recipe_level=8,recipe_type=4}---[120]四承通达履[男]
		staticrecipe[1246]={recipe_id=1435,recipe_group=0,recipe_level=8,recipe_type=4}---[120]四承通达履[女]
		staticrecipe[1247]={recipe_id=1436,recipe_group=0,recipe_level=9,recipe_type=4}---[135]不坏尊王罗汉履[男]
		staticrecipe[1248]={recipe_id=1437,recipe_group=0,recipe_level=9,recipe_type=4}---[135]不坏尊王罗汉履[女]
		staticrecipe[1249]={recipe_id=1438,recipe_group=0,recipe_level=10,recipe_type=4}---[144]八音元清净体履[男]
		staticrecipe[1250]={recipe_id=1439,recipe_group=0,recipe_level=10,recipe_type=4}---[144]八音元清净体履[女]
		staticrecipe[1251]={recipe_id=1442,recipe_group=0,recipe_level=8,recipe_type=4}---[120]八德滤心履[男]
		staticrecipe[1252]={recipe_id=1443,recipe_group=0,recipe_level=8,recipe_type=4}---[120]八德滤心履[女]
		staticrecipe[1253]={recipe_id=1444,recipe_group=0,recipe_level=9,recipe_type=4}---[135]观身无常上师履[男]
		staticrecipe[1254]={recipe_id=1445,recipe_group=0,recipe_level=9,recipe_type=4}---[135]观身无常上师履[女]
		staticrecipe[1255]={recipe_id=1446,recipe_group=0,recipe_level=10,recipe_type=4}---[144]五浊恶世舍利履[男]
		staticrecipe[1256]={recipe_id=1447,recipe_group=0,recipe_level=10,recipe_type=4}---[144]五浊恶世舍利履[女]
		staticrecipe[1257]={recipe_id=1450,recipe_group=0,recipe_level=8,recipe_type=4}---[120]五轮宿业履[男]
		staticrecipe[1258]={recipe_id=1451,recipe_group=0,recipe_level=8,recipe_type=4}---[120]五轮宿业履[女]
		staticrecipe[1259]={recipe_id=1452,recipe_group=0,recipe_level=9,recipe_type=4}---[135]神通永驻金刚履[男]
		staticrecipe[1260]={recipe_id=1453,recipe_group=0,recipe_level=9,recipe_type=4}---[135]神通永驻金刚履[女]
		staticrecipe[1261]={recipe_id=1454,recipe_group=0,recipe_level=10,recipe_type=4}---[144]十方世界生佛履[男]
		staticrecipe[1262]={recipe_id=1455,recipe_group=0,recipe_level=10,recipe_type=4}---[144]十方世界生佛履[女]
		staticrecipe[1263]={recipe_id=1458,recipe_group=0,recipe_level=8,recipe_type=4}---[120]九妙慈航履[男]
		staticrecipe[1264]={recipe_id=1459,recipe_group=0,recipe_level=8,recipe_type=4}---[120]九妙慈航履[女]
		staticrecipe[1265]={recipe_id=1460,recipe_group=0,recipe_level=9,recipe_type=4}---[135]摩诃刹利诸天履[男]
		staticrecipe[1266]={recipe_id=1461,recipe_group=0,recipe_level=9,recipe_type=4}---[135]摩诃刹利诸天履[女]
		staticrecipe[1267]={recipe_id=1398,recipe_group=0,recipe_level=10,recipe_type=4}---[144]九殿阎罗地藏履[男]
		staticrecipe[1268]={recipe_id=1399,recipe_group=0,recipe_level=10,recipe_type=4}---[144]九殿阎罗地藏履[女]
		staticrecipe[1269]={recipe_id=1402,recipe_group=0,recipe_level=8,recipe_type=4}---[120]九尊华严履[男]
		staticrecipe[1270]={recipe_id=1403,recipe_group=0,recipe_level=8,recipe_type=4}---[120]九尊华严履[女]
		staticrecipe[1271]={recipe_id=1404,recipe_group=0,recipe_level=9,recipe_type=4}---[135]金山觉意佛尊履[男]
		staticrecipe[1272]={recipe_id=1405,recipe_group=0,recipe_level=9,recipe_type=4}---[135]金山觉意佛尊履[女]
		staticrecipe[1273]={recipe_id=2203,recipe_group=0,recipe_level=10,recipe_type=2}---[146]地藏甲[男]
		staticrecipe[1274]={recipe_id=2269,recipe_group=0,recipe_level=10,recipe_type=2}---[146]地藏甲[女]
		staticrecipe[1275]={recipe_id=2201,recipe_group=0,recipe_level=8,recipe_type=2}---[120]冥宁甲[男]
		staticrecipe[1276]={recipe_id=2267,recipe_group=0,recipe_level=8,recipe_type=2}---[120]冥宁甲[女]
		staticrecipe[1277]={recipe_id=2202,recipe_group=0,recipe_level=9,recipe_type=2}---[135]龙骨甲[男]
		staticrecipe[1278]={recipe_id=2268,recipe_group=0,recipe_level=9,recipe_type=2}---[135]龙骨甲[女]
		staticrecipe[1279]={recipe_id=2210,recipe_group=0,recipe_level=10,recipe_type=2}---[146]九地冥君明光甲[男]
		staticrecipe[1280]={recipe_id=2276,recipe_group=0,recipe_level=10,recipe_type=2}---[146]九地冥君明光甲[女]
		staticrecipe[1281]={recipe_id=2208,recipe_group=0,recipe_level=8,recipe_type=2}---[120]九曜定心甲[男]
		staticrecipe[1282]={recipe_id=2274,recipe_group=0,recipe_level=8,recipe_type=2}---[120]九曜定心甲[女]
		staticrecipe[1283]={recipe_id=2209,recipe_group=0,recipe_level=9,recipe_type=2}---[135]战天狂灵魔神甲[男]
		staticrecipe[1284]={recipe_id=2275,recipe_group=0,recipe_level=9,recipe_type=2}---[135]战天狂灵魔神甲[女]
		staticrecipe[1285]={recipe_id=2224,recipe_group=0,recipe_level=10,recipe_type=2}---[146]三昧真火金精甲[男]
		staticrecipe[1286]={recipe_id=2290,recipe_group=0,recipe_level=10,recipe_type=2}---[146]三昧真火金精甲[女]
		staticrecipe[1287]={recipe_id=2222,recipe_group=0,recipe_level=8,recipe_type=2}---[120]三才鬼星甲[男]
		staticrecipe[1288]={recipe_id=2288,recipe_group=0,recipe_level=8,recipe_type=2}---[120]三才鬼星甲[女]
		staticrecipe[1289]={recipe_id=2223,recipe_group=0,recipe_level=9,recipe_type=2}---[135]荒坟鬼冢磷焰甲[男]
		staticrecipe[1290]={recipe_id=2289,recipe_group=0,recipe_level=9,recipe_type=2}---[135]荒坟鬼冢磷焰甲[女]
		staticrecipe[1291]={recipe_id=2245,recipe_group=0,recipe_level=10,recipe_type=2}---[146]六道轮回魔心甲[男]
		staticrecipe[1292]={recipe_id=2311,recipe_group=0,recipe_level=10,recipe_type=2}---[146]六道轮回魔心甲[女]
		staticrecipe[1293]={recipe_id=2243,recipe_group=0,recipe_level=8,recipe_type=2}---[120]六驭鬼烈甲[男]
		staticrecipe[1294]={recipe_id=2309,recipe_group=0,recipe_level=8,recipe_type=2}---[120]六驭鬼烈甲[女]
		staticrecipe[1295]={recipe_id=2244,recipe_group=0,recipe_level=9,recipe_type=2}---[135]冥河毒蛟洗髓甲[男]
		staticrecipe[1296]={recipe_id=2310,recipe_group=0,recipe_level=9,recipe_type=2}---[135]冥河毒蛟洗髓甲[女]
		staticrecipe[1297]={recipe_id=2252,recipe_group=0,recipe_level=10,recipe_type=2}---[146]七劫天罪威灵甲[男]
		staticrecipe[1298]={recipe_id=2318,recipe_group=0,recipe_level=10,recipe_type=2}---[146]七劫天罪威灵甲[女]
		staticrecipe[1299]={recipe_id=2250,recipe_group=0,recipe_level=8,recipe_type=2}---[120]七孔聚魂甲[男]
		staticrecipe[1300]={recipe_id=2316,recipe_group=0,recipe_level=8,recipe_type=2}---[120]七孔聚魂甲[女]
		staticrecipe[1301]={recipe_id=2251,recipe_group=0,recipe_level=9,recipe_type=2}---[135]血海逐波淀魂甲[男]
		staticrecipe[1302]={recipe_id=2317,recipe_group=0,recipe_level=9,recipe_type=2}---[135]血海逐波淀魂甲[女]
		staticrecipe[1303]={recipe_id=2231,recipe_group=0,recipe_level=10,recipe_type=2}---[146]天弈元尊冥甲[男]
		staticrecipe[1304]={recipe_id=2297,recipe_group=0,recipe_level=10,recipe_type=2}---[146]天弈元尊冥甲[女]
		staticrecipe[1305]={recipe_id=2229,recipe_group=0,recipe_level=8,recipe_type=2}---[120]武围神将冥甲[男]
		staticrecipe[1306]={recipe_id=2295,recipe_group=0,recipe_level=8,recipe_type=2}---[120]武围神将冥甲[女]
		staticrecipe[1307]={recipe_id=2230,recipe_group=0,recipe_level=9,recipe_type=2}---[135]界河真君冥甲[男]
		staticrecipe[1308]={recipe_id=2296,recipe_group=0,recipe_level=9,recipe_type=2}---[135]界河真君冥甲[女]
		staticrecipe[1309]={recipe_id=2259,recipe_group=0,recipe_level=10,recipe_type=2}---[146]八凶玄火炎魔甲[男]
		staticrecipe[1310]={recipe_id=2325,recipe_group=0,recipe_level=10,recipe_type=2}---[146]八凶玄火炎魔甲[女]
		staticrecipe[1311]={recipe_id=2257,recipe_group=0,recipe_level=8,recipe_type=2}---[120]八门金锁甲[男]
		staticrecipe[1312]={recipe_id=2323,recipe_group=0,recipe_level=8,recipe_type=2}---[120]八门金锁甲[女]
		staticrecipe[1313]={recipe_id=2258,recipe_group=0,recipe_level=9,recipe_type=2}---[135]无影蛊毒瘟蝗甲[男]
		staticrecipe[1314]={recipe_id=2324,recipe_group=0,recipe_level=9,recipe_type=2}---[135]无影蛊毒瘟蝗甲[女]
		staticrecipe[1315]={recipe_id=2238,recipe_group=0,recipe_level=10,recipe_type=2}---[146]五气升腾朝日甲[男]
		staticrecipe[1316]={recipe_id=2304,recipe_group=0,recipe_level=10,recipe_type=2}---[146]五气升腾朝日甲[女]
		staticrecipe[1317]={recipe_id=2236,recipe_group=0,recipe_level=8,recipe_type=2}---[120]五行绝命甲[男]
		staticrecipe[1318]={recipe_id=2302,recipe_group=0,recipe_level=8,recipe_type=2}---[120]五行绝命甲[女]
		staticrecipe[1319]={recipe_id=2237,recipe_group=0,recipe_level=9,recipe_type=2}---[135]毒尊避尘金麟甲[男]
		staticrecipe[1320]={recipe_id=2303,recipe_group=0,recipe_level=9,recipe_type=2}---[135]毒尊避尘金麟甲[女]
		staticrecipe[1321]={recipe_id=2217,recipe_group=0,recipe_level=10,recipe_type=2}---[146]九幽尊主傲神甲[男]
		staticrecipe[1322]={recipe_id=2283,recipe_group=0,recipe_level=10,recipe_type=2}---[146]九幽尊主傲神甲[女]
		staticrecipe[1323]={recipe_id=2215,recipe_group=0,recipe_level=8,recipe_type=2}---[120]九蛊逆刃甲[男]
		staticrecipe[1324]={recipe_id=2281,recipe_group=0,recipe_level=8,recipe_type=2}---[120]九蛊逆刃甲[女]
		staticrecipe[1325]={recipe_id=2216,recipe_group=0,recipe_level=9,recipe_type=2}---[135]幽冥转魂天祭甲[男]
		staticrecipe[1326]={recipe_id=2282,recipe_group=0,recipe_level=9,recipe_type=2}---[135]幽冥转魂天祭甲[女]
		staticrecipe[1327]={recipe_id=267,recipe_group=0,recipe_level=10,recipe_type=2}---[146]皇极甲[男]
		staticrecipe[1328]={recipe_id=315,recipe_group=0,recipe_level=10,recipe_type=2}---[146]皇极甲[女]
		staticrecipe[1329]={recipe_id=265,recipe_group=0,recipe_level=8,recipe_type=2}---[120]狂神甲[男]
		staticrecipe[1330]={recipe_id=313,recipe_group=0,recipe_level=8,recipe_type=2}---[120]狂神甲[女]
		staticrecipe[1331]={recipe_id=266,recipe_group=0,recipe_level=9,recipe_type=2}---[135]龙魂甲[男]
		staticrecipe[1332]={recipe_id=314,recipe_group=0,recipe_level=9,recipe_type=2}---[135]龙魂甲[女]
		staticrecipe[1333]={recipe_id=1470,recipe_group=0,recipe_level=10,recipe_type=2}---[146]三界恶灵邪神铠[男]
		staticrecipe[1334]={recipe_id=1471,recipe_group=0,recipe_level=10,recipe_type=2}---[146]三界恶灵邪神铠[女]
		staticrecipe[1335]={recipe_id=1474,recipe_group=0,recipe_level=8,recipe_type=2}---[120]三雄沥血铠[男]
		staticrecipe[1336]={recipe_id=1475,recipe_group=0,recipe_level=8,recipe_type=2}---[120]三雄沥血铠[女]
		staticrecipe[1337]={recipe_id=1476,recipe_group=0,recipe_level=9,recipe_type=2}---[135]樊龙碎鳞乌金甲[男]
		staticrecipe[1338]={recipe_id=1477,recipe_group=0,recipe_level=9,recipe_type=2}---[135]樊龙碎鳞乌金甲[女]
		staticrecipe[1339]={recipe_id=1478,recipe_group=0,recipe_level=10,recipe_type=2}---[146]六合纵横麒麟铠[男]
		staticrecipe[1340]={recipe_id=1479,recipe_group=0,recipe_level=10,recipe_type=2}---[146]六合纵横麒麟铠[女]
		staticrecipe[1341]={recipe_id=1482,recipe_group=0,recipe_level=8,recipe_type=2}---[120]六丁破军铠[男]
		staticrecipe[1342]={recipe_id=1483,recipe_group=0,recipe_level=8,recipe_type=2}---[120]六丁破军铠[女]
		staticrecipe[1343]={recipe_id=1484,recipe_group=0,recipe_level=9,recipe_type=2}---[135]天狱炼魂凝血铠[男]
		staticrecipe[1344]={recipe_id=1485,recipe_group=0,recipe_level=9,recipe_type=2}---[135]天狱炼魂凝血铠[女]
		staticrecipe[1345]={recipe_id=1486,recipe_group=0,recipe_level=10,recipe_type=2}---[146]七捷荡寇明光铠[男]
		staticrecipe[1346]={recipe_id=1487,recipe_group=0,recipe_level=10,recipe_type=2}---[146]七捷荡寇明光铠[女]
		staticrecipe[1347]={recipe_id=1490,recipe_group=0,recipe_level=8,recipe_type=2}---[120]七咒乱魂铠[男]
		staticrecipe[1348]={recipe_id=1491,recipe_group=0,recipe_level=8,recipe_type=2}---[120]七咒乱魂铠[女]
		staticrecipe[1349]={recipe_id=1492,recipe_group=0,recipe_level=9,recipe_type=2}---[135]狂岚啸天狻猊铠[男]
		staticrecipe[1350]={recipe_id=1493,recipe_group=0,recipe_level=9,recipe_type=2}---[135]狂岚啸天狻猊铠[女]
		staticrecipe[1351]={recipe_id=1494,recipe_group=0,recipe_level=10,recipe_type=2}---[146]四海啸傲龙筋铠[男]
		staticrecipe[1352]={recipe_id=1495,recipe_group=0,recipe_level=10,recipe_type=2}---[146]四海啸傲龙筋铠[女]
		staticrecipe[1353]={recipe_id=1498,recipe_group=0,recipe_level=8,recipe_type=2}---[120]四夷伏威铠[男]
		staticrecipe[1354]={recipe_id=1499,recipe_group=0,recipe_level=8,recipe_type=2}---[120]四夷伏威铠[女]
		staticrecipe[1355]={recipe_id=1500,recipe_group=0,recipe_level=9,recipe_type=2}---[135]兽面吞头连环铠[男]
		staticrecipe[1356]={recipe_id=1501,recipe_group=0,recipe_level=9,recipe_type=2}---[135]兽面吞头连环铠[女]
		staticrecipe[1357]={recipe_id=1502,recipe_group=0,recipe_level=10,recipe_type=2}---[146]八义继英掩心铠[男]
		staticrecipe[1358]={recipe_id=1503,recipe_group=0,recipe_level=10,recipe_type=2}---[146]八义继英掩心铠[女]
		staticrecipe[1359]={recipe_id=1506,recipe_group=0,recipe_level=8,recipe_type=2}---[120]八凶怒焰铠[男]
		staticrecipe[1360]={recipe_id=1507,recipe_group=0,recipe_level=8,recipe_type=2}---[120]八凶怒焰铠[女]
		staticrecipe[1361]={recipe_id=1508,recipe_group=0,recipe_level=9,recipe_type=2}---[135]金玉太岁绽魂铠[男]
		staticrecipe[1362]={recipe_id=1509,recipe_group=0,recipe_level=9,recipe_type=2}---[135]金玉太岁绽魂铠[女]
		staticrecipe[1363]={recipe_id=1510,recipe_group=0,recipe_level=10,recipe_type=2}---[146]五煞命延返魂铠[男]
		staticrecipe[1364]={recipe_id=1511,recipe_group=0,recipe_level=10,recipe_type=2}---[146]五煞命延返魂铠[女]
		staticrecipe[1365]={recipe_id=1514,recipe_group=0,recipe_level=8,recipe_type=2}---[120]五狱狂灵铠[男]
		staticrecipe[1366]={recipe_id=1515,recipe_group=0,recipe_level=8,recipe_type=2}---[120]五狱狂灵铠[女]
		staticrecipe[1367]={recipe_id=1516,recipe_group=0,recipe_level=9,recipe_type=2}---[135]蛮城锦带黄金甲[男]
		staticrecipe[1368]={recipe_id=1517,recipe_group=0,recipe_level=9,recipe_type=2}---[135]蛮城锦带黄金甲[女]
		staticrecipe[1369]={recipe_id=1518,recipe_group=0,recipe_level=10,recipe_type=2}---[146]九幽酆都鬼灵铠[男]
		staticrecipe[1370]={recipe_id=1519,recipe_group=0,recipe_level=10,recipe_type=2}---[146]九幽酆都鬼灵铠[女]
		staticrecipe[1371]={recipe_id=1522,recipe_group=0,recipe_level=8,recipe_type=2}---[120]九兽赤鳞铠[男]
		staticrecipe[1372]={recipe_id=1523,recipe_group=0,recipe_level=8,recipe_type=2}---[120]九兽赤鳞铠[女]
		staticrecipe[1373]={recipe_id=1524,recipe_group=0,recipe_level=9,recipe_type=2}---[135]明尊天佑邪灵铠[男]
		staticrecipe[1374]={recipe_id=1525,recipe_group=0,recipe_level=9,recipe_type=2}---[135]明尊天佑邪灵铠[女]
		staticrecipe[1375]={recipe_id=1462,recipe_group=0,recipe_level=10,recipe_type=2}---[146]九魔枭天冥魂铠[男]
		staticrecipe[1376]={recipe_id=1463,recipe_group=0,recipe_level=10,recipe_type=2}---[146]九魔枭天冥魂铠[女]
		staticrecipe[1377]={recipe_id=1466,recipe_group=0,recipe_level=8,recipe_type=2}---[120]九转焚心铠[男]
		staticrecipe[1378]={recipe_id=1467,recipe_group=0,recipe_level=8,recipe_type=2}---[120]九转焚心铠[女]
		staticrecipe[1379]={recipe_id=1468,recipe_group=0,recipe_level=9,recipe_type=2}---[135]离魂鬼契克劫铠[男]
		staticrecipe[1380]={recipe_id=1469,recipe_group=0,recipe_level=9,recipe_type=2}---[135]离魂鬼契克劫铠[女]
		staticrecipe[1381]={recipe_id=271,recipe_group=0,recipe_level=10,recipe_type=2}---[146]绝艳衫[男]
		staticrecipe[1382]={recipe_id=319,recipe_group=0,recipe_level=10,recipe_type=2}---[146]绝艳衫[女]
		staticrecipe[1383]={recipe_id=269,recipe_group=0,recipe_level=8,recipe_type=2}---[120]销魂衫[男]
		staticrecipe[1384]={recipe_id=317,recipe_group=0,recipe_level=8,recipe_type=2}---[120]销魂衫[女]
		staticrecipe[1385]={recipe_id=270,recipe_group=0,recipe_level=9,recipe_type=2}---[135]惊才衫[男]
		staticrecipe[1386]={recipe_id=318,recipe_group=0,recipe_level=9,recipe_type=2}---[135]惊才衫[女]
		staticrecipe[1387]={recipe_id=1534,recipe_group=0,recipe_level=10,recipe_type=2}---[146]三弄梅花妙叹衫[男]
		staticrecipe[1388]={recipe_id=1535,recipe_group=0,recipe_level=10,recipe_type=2}---[146]三弄梅花妙叹衫[女]
		staticrecipe[1389]={recipe_id=1538,recipe_group=0,recipe_level=8,recipe_type=2}---[120]三尾玄狐衫[男]
		staticrecipe[1390]={recipe_id=1539,recipe_group=0,recipe_level=8,recipe_type=2}---[120]三尾玄狐衫[女]
		staticrecipe[1391]={recipe_id=1540,recipe_group=0,recipe_level=9,recipe_type=2}---[135]西川红锦百花衫[男]
		staticrecipe[1392]={recipe_id=1541,recipe_group=0,recipe_level=9,recipe_type=2}---[135]西川红锦百花衫[女]
		staticrecipe[1393]={recipe_id=1542,recipe_group=0,recipe_level=10,recipe_type=2}---[146]六叶妖娆金莲衫[男]
		staticrecipe[1394]={recipe_id=1543,recipe_group=0,recipe_level=10,recipe_type=2}---[146]六叶妖娆金莲衫[女]
		staticrecipe[1395]={recipe_id=1546,recipe_group=0,recipe_level=8,recipe_type=2}---[120]六道荡魂衫[男]
		staticrecipe[1396]={recipe_id=1547,recipe_group=0,recipe_level=8,recipe_type=2}---[120]六道荡魂衫[女]
		staticrecipe[1397]={recipe_id=1548,recipe_group=0,recipe_level=9,recipe_type=2}---[135]幻彩华光金缕衫[男]
		staticrecipe[1398]={recipe_id=1549,recipe_group=0,recipe_level=9,recipe_type=2}---[135]幻彩华光金缕衫[女]
		staticrecipe[1399]={recipe_id=1550,recipe_group=0,recipe_level=10,recipe_type=2}---[146]七彩莹华冰心衫[男]
		staticrecipe[1400]={recipe_id=1551,recipe_group=0,recipe_level=10,recipe_type=2}---[146]七彩莹华冰心衫[女]
		staticrecipe[1401]={recipe_id=1554,recipe_group=0,recipe_level=8,recipe_type=2}---[120]七宝玲珑衫[男]
		staticrecipe[1402]={recipe_id=1555,recipe_group=0,recipe_level=8,recipe_type=2}---[120]七宝玲珑衫[女]
		staticrecipe[1403]={recipe_id=1556,recipe_group=0,recipe_level=9,recipe_type=2}---[135]落花逐水困情衫[男]
		staticrecipe[1404]={recipe_id=1557,recipe_group=0,recipe_level=9,recipe_type=2}---[135]落花逐水困情衫[女]
		staticrecipe[1405]={recipe_id=1558,recipe_group=0,recipe_level=10,recipe_type=2}---[146]四野漫歌铭情衫[男]
		staticrecipe[1406]={recipe_id=1559,recipe_group=0,recipe_level=10,recipe_type=2}---[146]四野漫歌铭情衫[女]
		staticrecipe[1407]={recipe_id=1562,recipe_group=0,recipe_level=8,recipe_type=2}---[120]四圣步空衫[男]
		staticrecipe[1408]={recipe_id=1563,recipe_group=0,recipe_level=8,recipe_type=2}---[120]四圣步空衫[女]
		staticrecipe[1409]={recipe_id=1564,recipe_group=0,recipe_level=9,recipe_type=2}---[135]落霞弥空霓彩衫[男]
		staticrecipe[1410]={recipe_id=1565,recipe_group=0,recipe_level=9,recipe_type=2}---[135]落霞弥空霓彩衫[女]
		staticrecipe[1411]={recipe_id=1566,recipe_group=0,recipe_level=10,recipe_type=2}---[146]八宝夜明珍珠衫[男]
		staticrecipe[1412]={recipe_id=1567,recipe_group=0,recipe_level=10,recipe_type=2}---[146]八宝夜明珍珠衫[女]
		staticrecipe[1413]={recipe_id=1570,recipe_group=0,recipe_level=8,recipe_type=2}---[120]八荒冀云衫[男]
		staticrecipe[1414]={recipe_id=1571,recipe_group=0,recipe_level=8,recipe_type=2}---[120]八荒冀云衫[女]
		staticrecipe[1415]={recipe_id=1572,recipe_group=0,recipe_level=9,recipe_type=2}---[135]临凡谪仙逍遥衫[男]
		staticrecipe[1416]={recipe_id=1573,recipe_group=0,recipe_level=9,recipe_type=2}---[135]临凡谪仙逍遥衫[女]
		staticrecipe[1417]={recipe_id=1574,recipe_group=0,recipe_level=10,recipe_type=2}---[146]五岳云岚傲天衫[男]
		staticrecipe[1418]={recipe_id=1575,recipe_group=0,recipe_level=10,recipe_type=2}---[146]五岳云岚傲天衫[女]
		staticrecipe[1419]={recipe_id=1578,recipe_group=0,recipe_level=8,recipe_type=2}---[120]五色华光衫[男]
		staticrecipe[1420]={recipe_id=1579,recipe_group=0,recipe_level=8,recipe_type=2}---[120]五色华光衫[女]
		staticrecipe[1421]={recipe_id=1580,recipe_group=0,recipe_level=9,recipe_type=2}---[135]太上忘情炼心铠[男]
		staticrecipe[1422]={recipe_id=1581,recipe_group=0,recipe_level=9,recipe_type=2}---[135]太上忘情炼心铠[女]
		staticrecipe[1423]={recipe_id=1582,recipe_group=0,recipe_level=10,recipe_type=2}---[146]九幻迷天绛绡衫[男]
		staticrecipe[1424]={recipe_id=1583,recipe_group=0,recipe_level=10,recipe_type=2}---[146]九幻迷天绛绡衫[女]
		staticrecipe[1425]={recipe_id=1586,recipe_group=0,recipe_level=8,recipe_type=2}---[120]九焕金纱铠[男]
		staticrecipe[1426]={recipe_id=1587,recipe_group=0,recipe_level=8,recipe_type=2}---[120]九焕金纱铠[女]
		staticrecipe[1427]={recipe_id=1588,recipe_group=0,recipe_level=9,recipe_type=2}---[135]琼台天籁承欢衫[男]
		staticrecipe[1428]={recipe_id=1589,recipe_group=0,recipe_level=9,recipe_type=2}---[135]琼台天籁承欢衫[女]
		staticrecipe[1429]={recipe_id=1526,recipe_group=0,recipe_level=10,recipe_type=2}---[146]九宵琼台连玉衫[男]
		staticrecipe[1430]={recipe_id=1527,recipe_group=0,recipe_level=10,recipe_type=2}---[146]九宵琼台连玉衫[女]
		staticrecipe[1431]={recipe_id=1530,recipe_group=0,recipe_level=8,recipe_type=2}---[120]九华惑心铠[男]
		staticrecipe[1432]={recipe_id=1531,recipe_group=0,recipe_level=8,recipe_type=2}---[120]九华惑心铠[女]
		staticrecipe[1433]={recipe_id=1532,recipe_group=0,recipe_level=9,recipe_type=2}---[135]远峦接天离愁衫[男]
		staticrecipe[1434]={recipe_id=1533,recipe_group=0,recipe_level=9,recipe_type=2}---[135]远峦接天离愁衫[女]
		staticrecipe[1435]={recipe_id=3878,recipe_group=0,recipe_level=10,recipe_type=2}---100级九黎甲男配方
		staticrecipe[1436]={recipe_id=3944,recipe_group=0,recipe_level=10,recipe_type=2}---100级九黎甲女配方
		staticrecipe[1437]={recipe_id=3876,recipe_group=0,recipe_level=8,recipe_type=2}---80级九黎甲男配方
		staticrecipe[1438]={recipe_id=3942,recipe_group=0,recipe_level=8,recipe_type=2}---80级九黎甲女配方
		staticrecipe[1439]={recipe_id=3877,recipe_group=0,recipe_level=9,recipe_type=2}---90级九黎甲男配方
		staticrecipe[1440]={recipe_id=3943,recipe_group=0,recipe_level=9,recipe_type=2}---90级九黎甲女配方
		staticrecipe[1441]={recipe_id=3885,recipe_group=0,recipe_level=10,recipe_type=2}---逆天100级九黎甲男配方
		staticrecipe[1442]={recipe_id=3951,recipe_group=0,recipe_level=10,recipe_type=2}---逆天100级九黎甲女配方
		staticrecipe[1443]={recipe_id=3883,recipe_group=0,recipe_level=8,recipe_type=2}---逆天80级九黎甲男配方
		staticrecipe[1444]={recipe_id=3949,recipe_group=0,recipe_level=8,recipe_type=2}---逆天80级九黎甲女配方
		staticrecipe[1445]={recipe_id=3884,recipe_group=0,recipe_level=9,recipe_type=2}---逆天90级九黎甲男配方
		staticrecipe[1446]={recipe_id=3950,recipe_group=0,recipe_level=9,recipe_type=2}---逆天90级九黎甲女配方
		staticrecipe[1447]={recipe_id=3899,recipe_group=0,recipe_level=10,recipe_type=2}---逆天防御装100级九黎甲男配方
		staticrecipe[1448]={recipe_id=3965,recipe_group=0,recipe_level=10,recipe_type=2}---逆天防御装100级九黎甲女配方
		staticrecipe[1449]={recipe_id=3897,recipe_group=0,recipe_level=8,recipe_type=2}---逆天防御装80级九黎甲男配方
		staticrecipe[1450]={recipe_id=3963,recipe_group=0,recipe_level=8,recipe_type=2}---逆天防御装80级九黎甲女配方
		staticrecipe[1451]={recipe_id=3898,recipe_group=0,recipe_level=9,recipe_type=2}---逆天防御装90级九黎甲男配方
		staticrecipe[1452]={recipe_id=3964,recipe_group=0,recipe_level=9,recipe_type=2}---逆天防御装90级九黎甲女配方
		staticrecipe[1453]={recipe_id=3920,recipe_group=0,recipe_level=10,recipe_type=2}---逆天抗性装丙100级九黎甲男配方
		staticrecipe[1454]={recipe_id=3986,recipe_group=0,recipe_level=10,recipe_type=2}---逆天抗性装丙100级九黎甲女配方
		staticrecipe[1455]={recipe_id=3918,recipe_group=0,recipe_level=8,recipe_type=2}---逆天抗性装丙80级九黎甲男配方
		staticrecipe[1456]={recipe_id=3984,recipe_group=0,recipe_level=8,recipe_type=2}---逆天抗性装丙80级九黎甲女配方
		staticrecipe[1457]={recipe_id=3919,recipe_group=0,recipe_level=9,recipe_type=2}---逆天抗性装丙90级九黎甲男配方
		staticrecipe[1458]={recipe_id=3985,recipe_group=0,recipe_level=9,recipe_type=2}---逆天抗性装丙90级九黎甲女配方
		staticrecipe[1459]={recipe_id=3927,recipe_group=0,recipe_level=10,recipe_type=2}---逆天抗性装丁100级九黎甲男配方
		staticrecipe[1460]={recipe_id=3993,recipe_group=0,recipe_level=10,recipe_type=2}---逆天抗性装丁100级九黎甲女配方
		staticrecipe[1461]={recipe_id=3925,recipe_group=0,recipe_level=8,recipe_type=2}---逆天抗性装丁80级九黎甲男配方
		staticrecipe[1462]={recipe_id=3991,recipe_group=0,recipe_level=8,recipe_type=2}---逆天抗性装丁80级九黎甲女配方
		staticrecipe[1463]={recipe_id=3926,recipe_group=0,recipe_level=9,recipe_type=2}---逆天抗性装丁90级九黎甲男配方
		staticrecipe[1464]={recipe_id=3992,recipe_group=0,recipe_level=9,recipe_type=2}---逆天抗性装丁90级九黎甲女配方
		staticrecipe[1465]={recipe_id=3906,recipe_group=0,recipe_level=10,recipe_type=2}---逆天抗性装甲100级九黎甲男配方
		staticrecipe[1466]={recipe_id=3972,recipe_group=0,recipe_level=10,recipe_type=2}---逆天抗性装甲100级九黎甲女配方
		staticrecipe[1467]={recipe_id=3904,recipe_group=0,recipe_level=8,recipe_type=2}---逆天抗性装甲80级九黎甲男配方
		staticrecipe[1468]={recipe_id=3970,recipe_group=0,recipe_level=8,recipe_type=2}---逆天抗性装甲80级九黎甲女配方
		staticrecipe[1469]={recipe_id=3905,recipe_group=0,recipe_level=9,recipe_type=2}---逆天抗性装甲90级九黎甲男配方
		staticrecipe[1470]={recipe_id=3971,recipe_group=0,recipe_level=9,recipe_type=2}---逆天抗性装甲90级九黎甲女配方
		staticrecipe[1471]={recipe_id=3934,recipe_group=0,recipe_level=10,recipe_type=2}---逆天抗性装戊100级九黎甲男配方
		staticrecipe[1472]={recipe_id=4000,recipe_group=0,recipe_level=10,recipe_type=2}---逆天抗性装戊100级九黎甲女配方
		staticrecipe[1473]={recipe_id=3932,recipe_group=0,recipe_level=8,recipe_type=2}---逆天抗性装戊80级九黎甲男配方
		staticrecipe[1474]={recipe_id=3998,recipe_group=0,recipe_level=8,recipe_type=2}---逆天抗性装戊80级九黎甲女配方
		staticrecipe[1475]={recipe_id=3933,recipe_group=0,recipe_level=9,recipe_type=2}---逆天抗性装戊90级九黎甲男配方
		staticrecipe[1476]={recipe_id=3999,recipe_group=0,recipe_level=9,recipe_type=2}---逆天抗性装戊90级九黎甲女配方
		staticrecipe[1477]={recipe_id=3913,recipe_group=0,recipe_level=10,recipe_type=2}---逆天抗性装乙100级九黎甲男配方
		staticrecipe[1478]={recipe_id=3979,recipe_group=0,recipe_level=10,recipe_type=2}---逆天抗性装乙100级九黎甲女配方
		staticrecipe[1479]={recipe_id=3911,recipe_group=0,recipe_level=8,recipe_type=2}---逆天抗性装乙80级九黎甲男配方
		staticrecipe[1480]={recipe_id=3977,recipe_group=0,recipe_level=8,recipe_type=2}---逆天抗性装乙80级九黎甲女配方
		staticrecipe[1481]={recipe_id=3912,recipe_group=0,recipe_level=9,recipe_type=2}---逆天抗性装乙90级九黎甲男配方
		staticrecipe[1482]={recipe_id=3978,recipe_group=0,recipe_level=9,recipe_type=2}---逆天抗性装乙90级九黎甲女配方
		staticrecipe[1483]={recipe_id=3892,recipe_group=0,recipe_level=10,recipe_type=2}---逆天魔法装100级九黎甲男配方
		staticrecipe[1484]={recipe_id=3958,recipe_group=0,recipe_level=10,recipe_type=2}---逆天魔法装100级九黎甲女配方
		staticrecipe[1485]={recipe_id=3890,recipe_group=0,recipe_level=8,recipe_type=2}---逆天魔法装80级九黎甲男配方
		staticrecipe[1486]={recipe_id=3956,recipe_group=0,recipe_level=8,recipe_type=2}---逆天魔法装80级九黎甲女配方
		staticrecipe[1487]={recipe_id=3891,recipe_group=0,recipe_level=9,recipe_type=2}---逆天魔法装90级九黎甲男配方
		staticrecipe[1488]={recipe_id=3957,recipe_group=0,recipe_level=9,recipe_type=2}---逆天魔法装90级九黎甲女配方
		staticrecipe[1489]={recipe_id=4284,recipe_group=0,recipe_level=10,recipe_type=2}---100级烈山甲男配方
		staticrecipe[1490]={recipe_id=4350,recipe_group=0,recipe_level=10,recipe_type=2}---100级烈山甲女配方
		staticrecipe[1491]={recipe_id=4282,recipe_group=0,recipe_level=8,recipe_type=2}---80级烈山甲男配方
		staticrecipe[1492]={recipe_id=4348,recipe_group=0,recipe_level=8,recipe_type=2}---80级烈山甲女配方
		staticrecipe[1493]={recipe_id=4283,recipe_group=0,recipe_level=9,recipe_type=2}---90级烈山甲男配方
		staticrecipe[1494]={recipe_id=4349,recipe_group=0,recipe_level=9,recipe_type=2}---90级烈山甲女配方
		staticrecipe[1495]={recipe_id=4291,recipe_group=0,recipe_level=10,recipe_type=2}---逆天100级烈山甲男配方
		staticrecipe[1496]={recipe_id=4357,recipe_group=0,recipe_level=10,recipe_type=2}---逆天100级烈山甲女配方
		staticrecipe[1497]={recipe_id=4289,recipe_group=0,recipe_level=8,recipe_type=2}---逆天80级烈山甲男配方
		staticrecipe[1498]={recipe_id=4355,recipe_group=0,recipe_level=8,recipe_type=2}---逆天80级烈山甲女配方
		staticrecipe[1499]={recipe_id=4290,recipe_group=0,recipe_level=9,recipe_type=2}---逆天90级烈山甲男配方
		staticrecipe[1500]={recipe_id=4356,recipe_group=0,recipe_level=9,recipe_type=2}---逆天90级烈山甲女配方
		staticrecipe[1501]={recipe_id=4305,recipe_group=0,recipe_level=10,recipe_type=2}---逆天防御装100级烈山甲男配方
		staticrecipe[1502]={recipe_id=4371,recipe_group=0,recipe_level=10,recipe_type=2}---逆天防御装100级烈山甲女配方
		staticrecipe[1503]={recipe_id=4303,recipe_group=0,recipe_level=8,recipe_type=2}---逆天防御装80级烈山甲男配方
		staticrecipe[1504]={recipe_id=4369,recipe_group=0,recipe_level=8,recipe_type=2}---逆天防御装80级烈山甲女配方
		staticrecipe[1505]={recipe_id=4304,recipe_group=0,recipe_level=9,recipe_type=2}---逆天防御装90级烈山甲男配方
		staticrecipe[1506]={recipe_id=4370,recipe_group=0,recipe_level=9,recipe_type=2}---逆天防御装90级烈山甲女配方
		staticrecipe[1507]={recipe_id=4326,recipe_group=0,recipe_level=10,recipe_type=2}---逆天抗性装丙100级烈山甲男配方
		staticrecipe[1508]={recipe_id=4392,recipe_group=0,recipe_level=10,recipe_type=2}---逆天抗性装丙100级烈山甲女配方
		staticrecipe[1509]={recipe_id=4324,recipe_group=0,recipe_level=8,recipe_type=2}---逆天抗性装丙80级烈山甲男配方
		staticrecipe[1510]={recipe_id=4390,recipe_group=0,recipe_level=8,recipe_type=2}---逆天抗性装丙80级烈山甲女配方
		staticrecipe[1511]={recipe_id=4325,recipe_group=0,recipe_level=9,recipe_type=2}---逆天抗性装丙90级烈山甲男配方
		staticrecipe[1512]={recipe_id=4391,recipe_group=0,recipe_level=9,recipe_type=2}---逆天抗性装丙90级烈山甲女配方
		staticrecipe[1513]={recipe_id=4333,recipe_group=0,recipe_level=10,recipe_type=2}---逆天抗性装丁100级烈山甲男配方
		staticrecipe[1514]={recipe_id=4399,recipe_group=0,recipe_level=10,recipe_type=2}---逆天抗性装丁100级烈山甲女配方
		staticrecipe[1515]={recipe_id=4331,recipe_group=0,recipe_level=8,recipe_type=2}---逆天抗性装丁80级烈山甲男配方
		staticrecipe[1516]={recipe_id=4397,recipe_group=0,recipe_level=8,recipe_type=2}---逆天抗性装丁80级烈山甲女配方
		staticrecipe[1517]={recipe_id=4332,recipe_group=0,recipe_level=9,recipe_type=2}---逆天抗性装丁90级烈山甲男配方
		staticrecipe[1518]={recipe_id=4398,recipe_group=0,recipe_level=9,recipe_type=2}---逆天抗性装丁90级烈山甲女配方
		staticrecipe[1519]={recipe_id=4312,recipe_group=0,recipe_level=10,recipe_type=2}---逆天抗性装甲100级烈山甲男配方
		staticrecipe[1520]={recipe_id=4378,recipe_group=0,recipe_level=10,recipe_type=2}---逆天抗性装甲100级烈山甲女配方
		staticrecipe[1521]={recipe_id=4310,recipe_group=0,recipe_level=8,recipe_type=2}---逆天抗性装甲80级烈山甲男配方
		staticrecipe[1522]={recipe_id=4376,recipe_group=0,recipe_level=8,recipe_type=2}---逆天抗性装甲80级烈山甲女配方
		staticrecipe[1523]={recipe_id=4311,recipe_group=0,recipe_level=9,recipe_type=2}---逆天抗性装甲90级烈山甲男配方
		staticrecipe[1524]={recipe_id=4377,recipe_group=0,recipe_level=9,recipe_type=2}---逆天抗性装甲90级烈山甲女配方
		staticrecipe[1525]={recipe_id=4340,recipe_group=0,recipe_level=10,recipe_type=2}---逆天抗性装戊100级烈山甲男配方
		staticrecipe[1526]={recipe_id=4405,recipe_group=0,recipe_level=10,recipe_type=2}---逆天抗性装戊100级烈山甲女配方
		staticrecipe[1527]={recipe_id=4338,recipe_group=0,recipe_level=8,recipe_type=2}---逆天抗性装戊80级烈山甲男配方
		staticrecipe[1528]={recipe_id=4406,recipe_group=0,recipe_level=8,recipe_type=2}---逆天抗性装戊80级烈山甲女配方
		staticrecipe[1529]={recipe_id=4339,recipe_group=0,recipe_level=9,recipe_type=2}---逆天抗性装戊90级烈山甲男配方
		staticrecipe[1530]={recipe_id=4404,recipe_group=0,recipe_level=9,recipe_type=2}---逆天抗性装戊90级烈山甲女配方
		staticrecipe[1531]={recipe_id=4319,recipe_group=0,recipe_level=10,recipe_type=2}---逆天抗性装乙100级烈山甲男配方
		staticrecipe[1532]={recipe_id=4385,recipe_group=0,recipe_level=10,recipe_type=2}---逆天抗性装乙100级烈山甲女配方
		staticrecipe[1533]={recipe_id=4317,recipe_group=0,recipe_level=8,recipe_type=2}---逆天抗性装乙80级烈山甲男配方
		staticrecipe[1534]={recipe_id=4383,recipe_group=0,recipe_level=8,recipe_type=2}---逆天抗性装乙80级烈山甲女配方
		staticrecipe[1535]={recipe_id=4318,recipe_group=0,recipe_level=9,recipe_type=2}---逆天抗性装乙90级烈山甲男配方
		staticrecipe[1536]={recipe_id=4384,recipe_group=0,recipe_level=9,recipe_type=2}---逆天抗性装乙90级烈山甲女配方
		staticrecipe[1537]={recipe_id=4298,recipe_group=0,recipe_level=10,recipe_type=2}---逆天魔法装100级烈山甲男配方
		staticrecipe[1538]={recipe_id=4364,recipe_group=0,recipe_level=10,recipe_type=2}---逆天魔法装100级烈山甲女配方
		staticrecipe[1539]={recipe_id=4296,recipe_group=0,recipe_level=8,recipe_type=2}---逆天魔法装80级烈山甲男配方
		staticrecipe[1540]={recipe_id=4362,recipe_group=0,recipe_level=8,recipe_type=2}---逆天魔法装80级烈山甲女配方
		staticrecipe[1541]={recipe_id=4297,recipe_group=0,recipe_level=9,recipe_type=2}---逆天魔法装90级烈山甲男配方
		staticrecipe[1542]={recipe_id=4363,recipe_group=0,recipe_level=9,recipe_type=2}---逆天魔法装90级烈山甲女配方
		staticrecipe[1543]={recipe_id=275,recipe_group=0,recipe_level=10,recipe_type=2}---[146]玉清袍[男]
		staticrecipe[1544]={recipe_id=323,recipe_group=0,recipe_level=10,recipe_type=2}---[146]玉清袍[女]
		staticrecipe[1545]={recipe_id=273,recipe_group=0,recipe_level=8,recipe_type=2}---[120]太清袍[男]
		staticrecipe[1546]={recipe_id=321,recipe_group=0,recipe_level=8,recipe_type=2}---[120]太清袍[女]
		staticrecipe[1547]={recipe_id=274,recipe_group=0,recipe_level=9,recipe_type=2}---[135]上清袍[男]
		staticrecipe[1548]={recipe_id=322,recipe_group=0,recipe_level=9,recipe_type=2}---[135]上清袍[女]
		staticrecipe[1549]={recipe_id=1598,recipe_group=0,recipe_level=10,recipe_type=2}---[146]三皇敕令道元袍[男]
		staticrecipe[1550]={recipe_id=1599,recipe_group=0,recipe_level=10,recipe_type=2}---[146]三皇敕令道元袍[女]
		staticrecipe[1551]={recipe_id=1602,recipe_group=0,recipe_level=8,recipe_type=2}---[120]三仙证道袍[男]
		staticrecipe[1552]={recipe_id=1603,recipe_group=0,recipe_level=8,recipe_type=2}---[120]三仙证道袍[女]
		staticrecipe[1553]={recipe_id=1604,recipe_group=0,recipe_level=9,recipe_type=2}---[135]众妙参玄道尊袍[男]
		staticrecipe[1554]={recipe_id=1605,recipe_group=0,recipe_level=9,recipe_type=2}---[135]众妙参玄道尊袍[女]
		staticrecipe[1555]={recipe_id=1606,recipe_group=0,recipe_level=10,recipe_type=2}---[146]六气遁法天机袍[男]
		staticrecipe[1556]={recipe_id=1607,recipe_group=0,recipe_level=10,recipe_type=2}---[146]六气遁法天机袍[女]
		staticrecipe[1557]={recipe_id=1610,recipe_group=0,recipe_level=8,recipe_type=2}---[120]六爻占命袍[男]
		staticrecipe[1558]={recipe_id=1611,recipe_group=0,recipe_level=8,recipe_type=2}---[120]六爻占命袍[女]
		staticrecipe[1559]={recipe_id=1612,recipe_group=0,recipe_level=9,recipe_type=2}---[135]灵机玄刹玉真袍[男]
		staticrecipe[1560]={recipe_id=1613,recipe_group=0,recipe_level=9,recipe_type=2}---[135]灵机玄刹玉真袍[女]
		staticrecipe[1561]={recipe_id=1614,recipe_group=0,recipe_level=10,recipe_type=2}---[146]七法悟玄神藏袍[男]
		staticrecipe[1562]={recipe_id=1615,recipe_group=0,recipe_level=10,recipe_type=2}---[146]七法悟玄神藏袍[女]
		staticrecipe[1563]={recipe_id=1618,recipe_group=0,recipe_level=8,recipe_type=2}---[120]七宿驻灵袍[男]
		staticrecipe[1564]={recipe_id=1619,recipe_group=0,recipe_level=8,recipe_type=2}---[120]七宿驻灵袍[女]
		staticrecipe[1565]={recipe_id=1620,recipe_group=0,recipe_level=9,recipe_type=2}---[135]经天星君命元袍[男]
		staticrecipe[1566]={recipe_id=1621,recipe_group=0,recipe_level=9,recipe_type=2}---[135]经天星君命元袍[女]
		staticrecipe[1567]={recipe_id=1622,recipe_group=0,recipe_level=10,recipe_type=2}---[146]四圣法谍固灵袍[男]
		staticrecipe[1568]={recipe_id=1623,recipe_group=0,recipe_level=10,recipe_type=2}---[146]四圣法谍固灵袍[女]
		staticrecipe[1569]={recipe_id=1626,recipe_group=0,recipe_level=8,recipe_type=2}---[120]四海游龙袍[男]
		staticrecipe[1570]={recipe_id=1627,recipe_group=0,recipe_level=8,recipe_type=2}---[120]四海游龙袍[女]
		staticrecipe[1571]={recipe_id=1628,recipe_group=0,recipe_level=9,recipe_type=2}---[135]玉鼎结丹化圣袍[男]
		staticrecipe[1572]={recipe_id=1629,recipe_group=0,recipe_level=9,recipe_type=2}---[135]玉鼎结丹化圣袍[女]
		staticrecipe[1573]={recipe_id=1630,recipe_group=0,recipe_level=10,recipe_type=2}---[146]八门金锁定心袍[男]
		staticrecipe[1574]={recipe_id=1631,recipe_group=0,recipe_level=10,recipe_type=2}---[146]八门金锁定心袍[女]
		staticrecipe[1575]={recipe_id=1634,recipe_group=0,recipe_level=8,recipe_type=2}---[120]八极玉阳袍[男]
		staticrecipe[1576]={recipe_id=1635,recipe_group=0,recipe_level=8,recipe_type=2}---[120]八极玉阳袍[女]
		staticrecipe[1577]={recipe_id=1636,recipe_group=0,recipe_level=9,recipe_type=2}---[135]秘法通玄洞冥袍[男]
		staticrecipe[1578]={recipe_id=1637,recipe_group=0,recipe_level=9,recipe_type=2}---[135]秘法通玄洞冥袍[女]
		staticrecipe[1579]={recipe_id=1638,recipe_group=0,recipe_level=10,recipe_type=2}---[146]五帝诏谕玄极袍[男]
		staticrecipe[1580]={recipe_id=1639,recipe_group=0,recipe_level=10,recipe_type=2}---[146]五帝诏谕玄极袍[女]
		staticrecipe[1581]={recipe_id=1642,recipe_group=0,recipe_level=8,recipe_type=2}---[120]五元固神袍[男]
		staticrecipe[1582]={recipe_id=1643,recipe_group=0,recipe_level=8,recipe_type=2}---[120]五元固神袍[女]
		staticrecipe[1583]={recipe_id=1644,recipe_group=0,recipe_level=9,recipe_type=2}---[135]紫阳初照掸心袍[男]
		staticrecipe[1584]={recipe_id=1645,recipe_group=0,recipe_level=9,recipe_type=2}---[135]紫阳初照掸心袍[女]
		staticrecipe[1585]={recipe_id=1646,recipe_group=0,recipe_level=10,recipe_type=2}---[146]九刹疾雷浩光袍[男]
		staticrecipe[1586]={recipe_id=1647,recipe_group=0,recipe_level=10,recipe_type=2}---[146]九刹疾雷浩光袍[女]
		staticrecipe[1587]={recipe_id=1650,recipe_group=0,recipe_level=8,recipe_type=2}---[120]九英聚元袍[男]
		staticrecipe[1588]={recipe_id=1651,recipe_group=0,recipe_level=8,recipe_type=2}---[120]九英聚元袍[女]
		staticrecipe[1589]={recipe_id=1652,recipe_group=0,recipe_level=9,recipe_type=2}---[135]积元贮本弥金袍[男]
		staticrecipe[1590]={recipe_id=1653,recipe_group=0,recipe_level=9,recipe_type=2}---[135]积元贮本弥金袍[女]
		staticrecipe[1591]={recipe_id=1590,recipe_group=0,recipe_level=10,recipe_type=2}---[146]九晶聚瑞天怒袍[男]
		staticrecipe[1592]={recipe_id=1591,recipe_group=0,recipe_level=10,recipe_type=2}---[146]九晶聚瑞天怒袍[女]
		staticrecipe[1593]={recipe_id=1594,recipe_group=0,recipe_level=8,recipe_type=2}---[120]九阳洞仙袍[男]
		staticrecipe[1594]={recipe_id=1595,recipe_group=0,recipe_level=8,recipe_type=2}---[120]九阳洞仙袍[女]
		staticrecipe[1595]={recipe_id=1596,recipe_group=0,recipe_level=9,recipe_type=2}---[135]云门天官锦罗袍[男]
		staticrecipe[1596]={recipe_id=1597,recipe_group=0,recipe_level=9,recipe_type=2}---[135]云门天官锦罗袍[女]
		staticrecipe[1597]={recipe_id=279,recipe_group=0,recipe_level=10,recipe_type=2}---[146]菩提袍[男]
		staticrecipe[1598]={recipe_id=327,recipe_group=0,recipe_level=10,recipe_type=2}---[146]菩提袍[女]
		staticrecipe[1599]={recipe_id=277,recipe_group=0,recipe_level=8,recipe_type=2}---[120]无相袍[男]
		staticrecipe[1600]={recipe_id=325,recipe_group=0,recipe_level=8,recipe_type=2}---[120]无相袍[女]
		staticrecipe[1601]={recipe_id=278,recipe_group=0,recipe_level=9,recipe_type=2}---[135]龙象袍[男]
		staticrecipe[1602]={recipe_id=326,recipe_group=0,recipe_level=9,recipe_type=2}---[135]龙象袍[女]
		staticrecipe[1603]={recipe_id=1662,recipe_group=0,recipe_level=10,recipe_type=2}---[146]三世娑婆往生袍[男]
		staticrecipe[1604]={recipe_id=1663,recipe_group=0,recipe_level=10,recipe_type=2}---[146]三世娑婆往生袍[女]
		staticrecipe[1605]={recipe_id=1666,recipe_group=0,recipe_level=8,recipe_type=2}---[120]三皈妙悟袍[男]
		staticrecipe[1606]={recipe_id=1667,recipe_group=0,recipe_level=8,recipe_type=2}---[120]三皈妙悟袍[女]
		staticrecipe[1607]={recipe_id=1668,recipe_group=0,recipe_level=9,recipe_type=2}---[135]毗卢沙门尊者袍[男]
		staticrecipe[1608]={recipe_id=1669,recipe_group=0,recipe_level=9,recipe_type=2}---[135]毗卢沙门尊者袍[女]
		staticrecipe[1609]={recipe_id=1670,recipe_group=0,recipe_level=10,recipe_type=2}---[146]六觉无量功德袍[男]
		staticrecipe[1610]={recipe_id=1671,recipe_group=0,recipe_level=10,recipe_type=2}---[146]六觉无量功德袍[女]
		staticrecipe[1611]={recipe_id=1674,recipe_group=0,recipe_level=8,recipe_type=2}---[120]六言真诀袍[男]
		staticrecipe[1612]={recipe_id=1675,recipe_group=0,recipe_level=8,recipe_type=2}---[120]六言真诀袍[女]
		staticrecipe[1613]={recipe_id=1676,recipe_group=0,recipe_level=9,recipe_type=2}---[135]无业宿尽伽蓝袍[男]
		staticrecipe[1614]={recipe_id=1677,recipe_group=0,recipe_level=9,recipe_type=2}---[135]无业宿尽伽蓝袍[女]
		staticrecipe[1615]={recipe_id=1678,recipe_group=0,recipe_level=10,recipe_type=2}---[146]七劫轮回寂灭袍[男]
		staticrecipe[1616]={recipe_id=1679,recipe_group=0,recipe_level=10,recipe_type=2}---[146]七劫轮回寂灭袍[女]
		staticrecipe[1617]={recipe_id=1682,recipe_group=0,recipe_level=8,recipe_type=2}---[120]七法梵音袍[男]
		staticrecipe[1618]={recipe_id=1683,recipe_group=0,recipe_level=8,recipe_type=2}---[120]七法梵音袍[女]
		staticrecipe[1619]={recipe_id=1684,recipe_group=0,recipe_level=9,recipe_type=2}---[135]无量本行大士袍[男]
		staticrecipe[1620]={recipe_id=1685,recipe_group=0,recipe_level=9,recipe_type=2}---[135]无量本行大士袍[女]
		staticrecipe[1621]={recipe_id=1686,recipe_group=0,recipe_level=10,recipe_type=2}---[146]四相无畏光明袍[男]
		staticrecipe[1622]={recipe_id=1687,recipe_group=0,recipe_level=10,recipe_type=2}---[146]四相无畏光明袍[女]
		staticrecipe[1623]={recipe_id=1690,recipe_group=0,recipe_level=8,recipe_type=2}---[120]四承通达袍[男]
		staticrecipe[1624]={recipe_id=1691,recipe_group=0,recipe_level=8,recipe_type=2}---[120]四承通达袍[女]
		staticrecipe[1625]={recipe_id=1692,recipe_group=0,recipe_level=9,recipe_type=2}---[135]不坏尊王罗汉袍[男]
		staticrecipe[1626]={recipe_id=1693,recipe_group=0,recipe_level=9,recipe_type=2}---[135]不坏尊王罗汉袍[女]
		staticrecipe[1627]={recipe_id=1694,recipe_group=0,recipe_level=10,recipe_type=2}---[146]八音元清净体袍[男]
		staticrecipe[1628]={recipe_id=1695,recipe_group=0,recipe_level=10,recipe_type=2}---[146]八音元清净体袍[女]
		staticrecipe[1629]={recipe_id=1698,recipe_group=0,recipe_level=8,recipe_type=2}---[120]八德滤心袍[男]
		staticrecipe[1630]={recipe_id=1699,recipe_group=0,recipe_level=8,recipe_type=2}---[120]八德滤心袍[女]
		staticrecipe[1631]={recipe_id=1700,recipe_group=0,recipe_level=9,recipe_type=2}---[135]观身无常上师袍[男]
		staticrecipe[1632]={recipe_id=1701,recipe_group=0,recipe_level=9,recipe_type=2}---[135]观身无常上师袍[女]
		staticrecipe[1633]={recipe_id=1702,recipe_group=0,recipe_level=10,recipe_type=2}---[146]五浊恶世舍利袍[男]
		staticrecipe[1634]={recipe_id=1703,recipe_group=0,recipe_level=10,recipe_type=2}---[146]五浊恶世舍利袍[女]
		staticrecipe[1635]={recipe_id=1706,recipe_group=0,recipe_level=8,recipe_type=2}---[120]五轮宿业袍[男]
		staticrecipe[1636]={recipe_id=1707,recipe_group=0,recipe_level=8,recipe_type=2}---[120]五轮宿业袍[女]
		staticrecipe[1637]={recipe_id=1708,recipe_group=0,recipe_level=9,recipe_type=2}---[135]神通永驻金刚袍[男]
		staticrecipe[1638]={recipe_id=1709,recipe_group=0,recipe_level=9,recipe_type=2}---[135]神通永驻金刚袍[女]
		staticrecipe[1639]={recipe_id=1710,recipe_group=0,recipe_level=10,recipe_type=2}---[146]十方世界生佛袍[男]
		staticrecipe[1640]={recipe_id=1711,recipe_group=0,recipe_level=10,recipe_type=2}---[146]十方世界生佛袍[女]
		staticrecipe[1641]={recipe_id=1714,recipe_group=0,recipe_level=8,recipe_type=2}---[120]九妙慈航袍[男]
		staticrecipe[1642]={recipe_id=1715,recipe_group=0,recipe_level=8,recipe_type=2}---[120]九妙慈航袍[女]
		staticrecipe[1643]={recipe_id=1716,recipe_group=0,recipe_level=9,recipe_type=2}---[135]摩诃刹利诸天袍[男]
		staticrecipe[1644]={recipe_id=1717,recipe_group=0,recipe_level=9,recipe_type=2}---[135]摩诃刹利诸天袍[女]
		staticrecipe[1645]={recipe_id=1654,recipe_group=0,recipe_level=10,recipe_type=2}---[146]九殿阎罗地藏袍[男]
		staticrecipe[1646]={recipe_id=1655,recipe_group=0,recipe_level=10,recipe_type=2}---[146]九殿阎罗地藏袍[女]
		staticrecipe[1647]={recipe_id=1658,recipe_group=0,recipe_level=8,recipe_type=2}---[120]九尊华严袍[男]
		staticrecipe[1648]={recipe_id=1659,recipe_group=0,recipe_level=8,recipe_type=2}---[120]九尊华严袍[女]
		staticrecipe[1649]={recipe_id=1660,recipe_group=0,recipe_level=9,recipe_type=2}---[135]金山觉意佛尊袍[男]
		staticrecipe[1650]={recipe_id=1661,recipe_group=0,recipe_level=9,recipe_type=2}---[135]金山觉意佛尊袍[女]
		staticrecipe[1651]={recipe_id=2967,recipe_group=0,recipe_level=1,recipe_type=5}---法身·两仪化生配方
		staticrecipe[1652]={recipe_id=2968,recipe_group=0,recipe_level=1,recipe_type=5}---法身·三花聚顶配方
		staticrecipe[1653]={recipe_id=2969,recipe_group=0,recipe_level=1,recipe_type=5}---法身·四象纵横配方
		staticrecipe[1654]={recipe_id=2970,recipe_group=0,recipe_level=1,recipe_type=5}---法身·五气朝元配方
		staticrecipe[1655]={recipe_id=2971,recipe_group=0,recipe_level=1,recipe_type=5}---法身·六爻离合配方
		staticrecipe[1656]={recipe_id=2972,recipe_group=0,recipe_level=1,recipe_type=5}---法身·七星转魂配方
		staticrecipe[1657]={recipe_id=2973,recipe_group=0,recipe_level=1,recipe_type=5}---法身·八法运通配方
		staticrecipe[1658]={recipe_id=2974,recipe_group=0,recipe_level=1,recipe_type=5}---法身·九转阴阳配方
		staticrecipe[1659]={recipe_id=2975,recipe_group=0,recipe_level=1,recipe_type=5}---法身·十方乾坤配方
		staticrecipe[1660]={recipe_id=2976,recipe_group=0,recipe_level=1,recipe_type=5}---法身·百劫洞冥配方
		staticrecipe[1661]={recipe_id=2977,recipe_group=0,recipe_level=1,recipe_type=5}---法身·千界婆娑配方
		staticrecipe[1662]={recipe_id=2978,recipe_group=0,recipe_level=1,recipe_type=5}---法身·万流至尊配方
		staticrecipe[1663]={recipe_id=2979,recipe_group=0,recipe_level=1,recipe_type=5}---金身·两仪化生配方
		staticrecipe[1664]={recipe_id=2980,recipe_group=0,recipe_level=1,recipe_type=5}---金身·三花聚顶配方
		staticrecipe[1665]={recipe_id=2981,recipe_group=0,recipe_level=1,recipe_type=5}---金身·四象纵横配方
		staticrecipe[1666]={recipe_id=2982,recipe_group=0,recipe_level=1,recipe_type=5}---金身·五气朝元配方
		staticrecipe[1667]={recipe_id=2983,recipe_group=0,recipe_level=1,recipe_type=5}---金身·六爻离合配方
		staticrecipe[1668]={recipe_id=2984,recipe_group=0,recipe_level=1,recipe_type=5}---金身·七星转魂配方
		staticrecipe[1669]={recipe_id=2985,recipe_group=0,recipe_level=1,recipe_type=5}---金身·八法运通配方
		staticrecipe[1670]={recipe_id=2986,recipe_group=0,recipe_level=1,recipe_type=5}---金身·九转阴阳配方
		staticrecipe[1671]={recipe_id=2987,recipe_group=0,recipe_level=1,recipe_type=5}---金身·十方乾坤配方
		staticrecipe[1672]={recipe_id=2988,recipe_group=0,recipe_level=1,recipe_type=5}---金身·百劫洞冥配方
		staticrecipe[1673]={recipe_id=2989,recipe_group=0,recipe_level=1,recipe_type=5}---金身·千界婆娑配方
		staticrecipe[1674]={recipe_id=2990,recipe_group=0,recipe_level=1,recipe_type=5}---金身·万流至尊配方
		staticrecipe[1675]={recipe_id=2362,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1676]={recipe_id=2368,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1677]={recipe_id=2374,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1678]={recipe_id=2380,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1679]={recipe_id=2386,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1680]={recipe_id=2392,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1681]={recipe_id=2398,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1682]={recipe_id=2404,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1683]={recipe_id=2410,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1684]={recipe_id=2416,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1685]={recipe_id=2422,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1686]={recipe_id=2428,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1687]={recipe_id=2434,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1688]={recipe_id=2440,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1689]={recipe_id=2446,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1690]={recipe_id=2452,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1691]={recipe_id=2458,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1692]={recipe_id=2464,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1693]={recipe_id=2470,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1694]={recipe_id=2476,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1695]={recipe_id=2482,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1696]={recipe_id=2488,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1697]={recipe_id=2494,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1698]={recipe_id=2500,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1699]={recipe_id=2506,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1700]={recipe_id=2512,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1701]={recipe_id=2518,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1702]={recipe_id=2524,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1703]={recipe_id=2530,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1704]={recipe_id=2536,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1705]={recipe_id=2542,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1706]={recipe_id=2548,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1707]={recipe_id=2554,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1708]={recipe_id=2560,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1709]={recipe_id=2566,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1710]={recipe_id=2572,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1711]={recipe_id=2578,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1712]={recipe_id=2584,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1713]={recipe_id=2590,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1714]={recipe_id=2596,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1715]={recipe_id=2602,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1716]={recipe_id=2608,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1717]={recipe_id=2614,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1718]={recipe_id=2620,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1719]={recipe_id=2626,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1720]={recipe_id=2632,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1721]={recipe_id=2638,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1722]={recipe_id=2644,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1723]={recipe_id=2650,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1724]={recipe_id=2656,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1725]={recipe_id=2662,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1726]={recipe_id=2668,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1727]={recipe_id=2674,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1728]={recipe_id=2680,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1729]={recipe_id=2686,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1730]={recipe_id=2692,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1731]={recipe_id=2698,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1732]={recipe_id=2704,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1733]={recipe_id=2710,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1734]={recipe_id=2716,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1735]={recipe_id=2722,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1736]={recipe_id=2728,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1737]={recipe_id=2734,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1738]={recipe_id=2740,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1739]={recipe_id=2746,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1740]={recipe_id=2752,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1741]={recipe_id=2758,recipe_group=1,recipe_level=3,recipe_type=11}
		staticrecipe[1742]={recipe_id=4410,recipe_group=0,recipe_level=1,recipe_type=5}	---辣手
		staticrecipe[1743]={recipe_id=4411,recipe_group=0,recipe_level=1,recipe_type=5}	---狱火
		staticrecipe[1744]={recipe_id=4412,recipe_group=0,recipe_level=1,recipe_type=5}	---铁腕
		staticrecipe[1745]={recipe_id=4413,recipe_group=0,recipe_level=1,recipe_type=5}	---龙腾
		staticrecipe[1746]={recipe_id=4414,recipe_group=0,recipe_level=1,recipe_type=5}	---豪烈
		staticrecipe[1747]={recipe_id=4415,recipe_group=0,recipe_level=1,recipe_type=5}	---金戈
		staticrecipe[1748]={recipe_id=4416,recipe_group=0,recipe_level=1,recipe_type=5}	---怒奔
		staticrecipe[1749]={recipe_id=4417,recipe_group=0,recipe_level=1,recipe_type=5}	---命脉
		staticrecipe[1750]={recipe_id=4418,recipe_group=0,recipe_level=1,recipe_type=5}	---龙影
		staticrecipe[1751]={recipe_id=4419,recipe_group=0,recipe_level=1,recipe_type=5}	---苍穹
		staticrecipe[1752]={recipe_id=4420,recipe_group=0,recipe_level=1,recipe_type=5}	---望族
		staticrecipe[1753]={recipe_id=4421,recipe_group=0,recipe_level=1,recipe_type=5}	---血刃
		staticrecipe[1754]={recipe_id=4422,recipe_group=0,recipe_level=1,recipe_type=5}	---绝杀
		staticrecipe[1755]={recipe_id=4423,recipe_group=0,recipe_level=1,recipe_type=5}	---血河
		staticrecipe[1756]={recipe_id=4424,recipe_group=0,recipe_level=1,recipe_type=5}	---凶兽
		staticrecipe[1757]={recipe_id=4425,recipe_group=0,recipe_level=1,recipe_type=5}	---龙威
		staticrecipe[1758]={recipe_id=4426,recipe_group=0,recipe_level=1,recipe_type=5}	---不休
		staticrecipe[1759]={recipe_id=4427,recipe_group=0,recipe_level=1,recipe_type=5}	---连舞
		staticrecipe[1760]={recipe_id=4428,recipe_group=0,recipe_level=1,recipe_type=5}	---罡风
		staticrecipe[1761]={recipe_id=4429,recipe_group=0,recipe_level=1,recipe_type=5}	---云岭
		staticrecipe[1762]={recipe_id=4430,recipe_group=0,recipe_level=1,recipe_type=5}	---燎原
		staticrecipe[1763]={recipe_id=4431,recipe_group=0,recipe_level=1,recipe_type=5}	---绝壁
		staticrecipe[1764]={recipe_id=4432,recipe_group=0,recipe_level=1,recipe_type=5}	---双刃
		staticrecipe[1765]={recipe_id=4433,recipe_group=0,recipe_level=1,recipe_type=5}	---安步
		staticrecipe[1766]={recipe_id=4434,recipe_group=0,recipe_level=1,recipe_type=5}	---登云
		staticrecipe[1767]={recipe_id=4435,recipe_group=0,recipe_level=1,recipe_type=5}	---剪径
		staticrecipe[1768]={recipe_id=4436,recipe_group=0,recipe_level=1,recipe_type=5}	---洪声
		staticrecipe[1769]={recipe_id=4437,recipe_group=0,recipe_level=1,recipe_type=5}	---天宗
		staticrecipe[1770]={recipe_id=4438,recipe_group=0,recipe_level=1,recipe_type=5}	---轰然
		staticrecipe[1771]={recipe_id=4439,recipe_group=0,recipe_level=1,recipe_type=5}	---杀将
		staticrecipe[1772]={recipe_id=4440,recipe_group=0,recipe_level=1,recipe_type=5}	---笑谈
		staticrecipe[1773]={recipe_id=4441,recipe_group=0,recipe_level=1,recipe_type=5}	---雄雌
		staticrecipe[1774]={recipe_id=4442,recipe_group=0,recipe_level=1,recipe_type=5}	---唇舌
		staticrecipe[1775]={recipe_id=4443,recipe_group=0,recipe_level=1,recipe_type=5}	---铁骨
		staticrecipe[1776]={recipe_id=4444,recipe_group=0,recipe_level=1,recipe_type=5}	---百辟
		staticrecipe[1777]={recipe_id=4445,recipe_group=0,recipe_level=1,recipe_type=5}	---精武
		staticrecipe[1778]={recipe_id=4446,recipe_group=0,recipe_level=1,recipe_type=5}	---吊睛
		staticrecipe[1779]={recipe_id=4447,recipe_group=0,recipe_level=1,recipe_type=5}	---不屈
		staticrecipe[1780]={recipe_id=4448,recipe_group=0,recipe_level=1,recipe_type=5}	---军魂
		staticrecipe[1781]={recipe_id=4449,recipe_group=0,recipe_level=1,recipe_type=5}	---通臂
		staticrecipe[1782]={recipe_id=4450,recipe_group=0,recipe_level=1,recipe_type=5}	---碎金
		staticrecipe[1783]={recipe_id=4451,recipe_group=0,recipe_level=1,recipe_type=5}	---裂魂
		staticrecipe[1784]={recipe_id=4452,recipe_group=0,recipe_level=1,recipe_type=5}	---千钧
		staticrecipe[1785]={recipe_id=4453,recipe_group=0,recipe_level=1,recipe_type=5}	---睚眦
		staticrecipe[1786]={recipe_id=4454,recipe_group=0,recipe_level=1,recipe_type=5}	---骁勇
		staticrecipe[1787]={recipe_id=4455,recipe_group=0,recipe_level=1,recipe_type=5}	---意气
		staticrecipe[1788]={recipe_id=4456,recipe_group=0,recipe_level=1,recipe_type=5}	---异禀
		staticrecipe[1789]={recipe_id=4457,recipe_group=0,recipe_level=1,recipe_type=5}	---狂飙
		staticrecipe[1790]={recipe_id=4458,recipe_group=0,recipe_level=1,recipe_type=5}	---本命
		staticrecipe[1791]={recipe_id=4459,recipe_group=0,recipe_level=1,recipe_type=5}	---祇护
		staticrecipe[1792]={recipe_id=4460,recipe_group=0,recipe_level=1,recipe_type=5}	---凶神
		staticrecipe[1793]={recipe_id=4461,recipe_group=0,recipe_level=1,recipe_type=5}	---春华
		staticrecipe[1794]={recipe_id=4462,recipe_group=0,recipe_level=1,recipe_type=5}	---虎须
		staticrecipe[1795]={recipe_id=4463,recipe_group=0,recipe_level=1,recipe_type=5}	---剑齿
		staticrecipe[1796]={recipe_id=4464,recipe_group=0,recipe_level=1,recipe_type=5}	---余馨
		staticrecipe[1797]={recipe_id=4465,recipe_group=0,recipe_level=1,recipe_type=5}	---琼浆
		staticrecipe[1798]={recipe_id=4466,recipe_group=0,recipe_level=1,recipe_type=5}	---健木
		staticrecipe[1799]={recipe_id=4467,recipe_group=0,recipe_level=1,recipe_type=5}	---神行
		staticrecipe[1800]={recipe_id=4468,recipe_group=0,recipe_level=1,recipe_type=5}	---乘云
		staticrecipe[1801]={recipe_id=4469,recipe_group=0,recipe_level=1,recipe_type=5}	---灵言
		staticrecipe[1802]={recipe_id=4470,recipe_group=0,recipe_level=1,recipe_type=5}	---魁星
		staticrecipe[1803]={recipe_id=4471,recipe_group=0,recipe_level=1,recipe_type=5}	---荣光
		staticrecipe[1804]={recipe_id=4472,recipe_group=0,recipe_level=1,recipe_type=5}	---奇葩
		staticrecipe[1805]={recipe_id=4473,recipe_group=0,recipe_level=1,recipe_type=5}	---毒芽
		staticrecipe[1806]={recipe_id=4474,recipe_group=0,recipe_level=1,recipe_type=5}	---虬枝
		staticrecipe[1807]={recipe_id=4475,recipe_group=0,recipe_level=1,recipe_type=5}	---千年
		staticrecipe[1808]={recipe_id=4476,recipe_group=0,recipe_level=1,recipe_type=5}	---绕梁
		staticrecipe[1809]={recipe_id=4477,recipe_group=0,recipe_level=1,recipe_type=5}	---咫尺
		staticrecipe[1810]={recipe_id=4478,recipe_group=0,recipe_level=1,recipe_type=5}	---浮波
		staticrecipe[1811]={recipe_id=4479,recipe_group=0,recipe_level=1,recipe_type=5}	---逐风
		staticrecipe[1812]={recipe_id=4480,recipe_group=0,recipe_level=1,recipe_type=5}	---逆足
		staticrecipe[1813]={recipe_id=4481,recipe_group=0,recipe_level=1,recipe_type=5}	---暮色
		staticrecipe[1814]={recipe_id=4482,recipe_group=0,recipe_level=1,recipe_type=5}	---火灵
		staticrecipe[1815]={recipe_id=4483,recipe_group=0,recipe_level=1,recipe_type=5}	---龙皇
		staticrecipe[1816]={recipe_id=4484,recipe_group=0,recipe_level=1,recipe_type=5}	---星雨
		staticrecipe[1817]={recipe_id=4485,recipe_group=0,recipe_level=1,recipe_type=5}	---烨空
		staticrecipe[1818]={recipe_id=4486,recipe_group=0,recipe_level=1,recipe_type=5}	---天河
		staticrecipe[1819]={recipe_id=4487,recipe_group=0,recipe_level=1,recipe_type=5}	---神陨
		staticrecipe[1820]={recipe_id=4488,recipe_group=0,recipe_level=1,recipe_type=5}	---浩瀚
		staticrecipe[1821]={recipe_id=4489,recipe_group=0,recipe_level=1,recipe_type=5}	---弦歌
		staticrecipe[1822]={recipe_id=4490,recipe_group=0,recipe_level=1,recipe_type=5}	---血锋
		staticrecipe[1823]={recipe_id=4491,recipe_group=0,recipe_level=1,recipe_type=5}	---灼目
		staticrecipe[1824]={recipe_id=4492,recipe_group=0,recipe_level=1,recipe_type=5}	---猎魔
		staticrecipe[1825]={recipe_id=4493,recipe_group=0,recipe_level=1,recipe_type=5}	---技穷
		staticrecipe[1826]={recipe_id=4494,recipe_group=0,recipe_level=1,recipe_type=5}	---极光
		staticrecipe[1827]={recipe_id=4495,recipe_group=0,recipe_level=1,recipe_type=5}	---锐金
		staticrecipe[1828]={recipe_id=4496,recipe_group=0,recipe_level=1,recipe_type=5}	---气旋
		staticrecipe[1829]={recipe_id=4497,recipe_group=0,recipe_level=1,recipe_type=5}	---神目
		staticrecipe[1830]={recipe_id=4498,recipe_group=0,recipe_level=1,recipe_type=5}	---钩齿
		staticrecipe[1831]={recipe_id=4499,recipe_group=0,recipe_level=1,recipe_type=5}	---角弦
		staticrecipe[1832]={recipe_id=4500,recipe_group=0,recipe_level=1,recipe_type=5}	---挥洒
		staticrecipe[1833]={recipe_id=4501,recipe_group=0,recipe_level=1,recipe_type=5}	---凶狼
		staticrecipe[1834]={recipe_id=4502,recipe_group=0,recipe_level=1,recipe_type=5}	---奎牯
		staticrecipe[1835]={recipe_id=4503,recipe_group=0,recipe_level=1,recipe_type=5}	---威熊
		staticrecipe[1836]={recipe_id=4504,recipe_group=0,recipe_level=1,recipe_type=5}	---玄龟
		staticrecipe[1837]={recipe_id=4505,recipe_group=0,recipe_level=1,recipe_type=5}	---怒兽
		staticrecipe[1838]={recipe_id=4506,recipe_group=0,recipe_level=1,recipe_type=5}	---灵体
		staticrecipe[1839]={recipe_id=4507,recipe_group=0,recipe_level=1,recipe_type=5}	---风萧
		staticrecipe[1840]={recipe_id=4508,recipe_group=0,recipe_level=1,recipe_type=5}	---林谧
		staticrecipe[1841]={recipe_id=4509,recipe_group=0,recipe_level=1,recipe_type=5}	---火烈
		staticrecipe[1842]={recipe_id=4510,recipe_group=0,recipe_level=1,recipe_type=5}	---山隆
		staticrecipe[1843]={recipe_id=4511,recipe_group=0,recipe_level=1,recipe_type=5}	---百变
		staticrecipe[1844]={recipe_id=4512,recipe_group=0,recipe_level=1,recipe_type=5}	---龙腾_豪烈配方
		staticrecipe[1845]={recipe_id=4513,recipe_group=0,recipe_level=1,recipe_type=5}	---血刃_绝杀配方
		staticrecipe[1846]={recipe_id=4514,recipe_group=0,recipe_level=1,recipe_type=5}	---龙影_苍穹配方
		staticrecipe[1847]={recipe_id=4515,recipe_group=0,recipe_level=1,recipe_type=5}	---罡风_云岭配方
		staticrecipe[1848]={recipe_id=4516,recipe_group=0,recipe_level=1,recipe_type=5}	---燎原_绝壁配方
		staticrecipe[1849]={recipe_id=4517,recipe_group=0,recipe_level=1,recipe_type=5}	---登云_剪径配方
		staticrecipe[1850]={recipe_id=4518,recipe_group=0,recipe_level=1,recipe_type=5}	---轰然_杀将配方
		staticrecipe[1851]={recipe_id=4519,recipe_group=0,recipe_level=1,recipe_type=5}	---吊睛_不屈配方
		staticrecipe[1852]={recipe_id=4520,recipe_group=0,recipe_level=1,recipe_type=5}	---碎金_裂魂配方
		staticrecipe[1853]={recipe_id=4521,recipe_group=0,recipe_level=1,recipe_type=5}	---余馨_琼浆配方
		staticrecipe[1854]={recipe_id=4522,recipe_group=0,recipe_level=1,recipe_type=5}	---神行_乘云配方
		staticrecipe[1855]={recipe_id=4523,recipe_group=0,recipe_level=1,recipe_type=5}	---威熊_玄龟配方
		staticrecipe[1856]={recipe_id=4524,recipe_group=0,recipe_level=1,recipe_type=5}	---火烈_山隆配方
		staticrecipe[1857]={recipe_id=4525,recipe_group=0,recipe_level=1,recipe_type=5}	---暮色_火灵配方
		staticrecipe[1858]={recipe_id=4526,recipe_group=0,recipe_level=1,recipe_type=5}	---天河_神陨配方
		staticrecipe[1859]={recipe_id=4527,recipe_group=0,recipe_level=1,recipe_type=5}	---猎魔_技穷配方
		staticrecipe[1860]={recipe_id=4528,recipe_group=0,recipe_level=1,recipe_type=5}	---虎须_剑齿配方
		staticrecipe[1861]={recipe_id=4529,recipe_group=0,recipe_level=1,recipe_type=5}	---凶狼_奎牯配方
		staticrecipe[1862]={recipe_id=5355,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_肘锤配方
		staticrecipe[1863]={recipe_id=5356,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_切脉配方
		staticrecipe[1864]={recipe_id=5357,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_足扫配方
		staticrecipe[1865]={recipe_id=5358,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_手刀配方
		staticrecipe[1866]={recipe_id=5359,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_龙拳配方
		staticrecipe[1867]={recipe_id=5360,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_瞬步配方
		staticrecipe[1868]={recipe_id=5361,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_风旋配方
		staticrecipe[1869]={recipe_id=5362,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_鬼足配方
		staticrecipe[1870]={recipe_id=5363,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_雷炎配方
		staticrecipe[1871]={recipe_id=5364,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_龙辉配方
		staticrecipe[1872]={recipe_id=5365,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_斩破配方
		staticrecipe[1873]={recipe_id=5366,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_摄魂配方
		staticrecipe[1874]={recipe_id=5367,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_诛心配方
		staticrecipe[1875]={recipe_id=5368,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_神弃配方
		staticrecipe[1876]={recipe_id=5369,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_死斗配方
		staticrecipe[1877]={recipe_id=5370,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_血噬配方
		staticrecipe[1878]={recipe_id=5371,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_杀劫配方
		staticrecipe[1879]={recipe_id=5372,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_鬼翼配方
		staticrecipe[1880]={recipe_id=5373,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_浮舟配方
		staticrecipe[1881]={recipe_id=5374,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_魂锁配方
		staticrecipe[1882]={recipe_id=5375,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_杀神配方
		staticrecipe[1883]={recipe_id=5376,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_通幽配方
		staticrecipe[1884]={recipe_id=5377,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_蚁聚配方
		staticrecipe[1885]={recipe_id=5378,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_鹰扬配方
		staticrecipe[1886]={recipe_id=5379,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_兔脱配方
		staticrecipe[1887]={recipe_id=5380,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_蛇蜒配方
		staticrecipe[1888]={recipe_id=5381,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_鬼武配方
		staticrecipe[1889]={recipe_id=5382,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_鹤行配方
		staticrecipe[1890]={recipe_id=5383,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_龟息配方
		staticrecipe[1891]={recipe_id=5384,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_鸩羽配方
		staticrecipe[1892]={recipe_id=5385,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_蚊须配方
		staticrecipe[1893]={recipe_id=5386,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_蛛涎配方
		staticrecipe[1894]={recipe_id=5387,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_蝎针配方
		staticrecipe[1895]={recipe_id=5388,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_冥蛊配方
		staticrecipe[1896]={recipe_id=5389,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_傲骨配方
		staticrecipe[1897]={recipe_id=5390,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_凝眸配方
		staticrecipe[1898]={recipe_id=5391,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_铁肩配方
		staticrecipe[1899]={recipe_id=5392,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_绝指配方
		staticrecipe[1900]={recipe_id=5393,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_杀心配方
		staticrecipe[1901]={recipe_id=5394,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_风影配方
		staticrecipe[1902]={recipe_id=5395,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_仲寿配方
		staticrecipe[1903]={recipe_id=5396,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_玄影配方
		staticrecipe[1904]={recipe_id=5397,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_影灵配方
		staticrecipe[1905]={recipe_id=5398,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_影心配方
		staticrecipe[1906]={recipe_id=5399,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_佐影配方
		staticrecipe[1907]={recipe_id=5400,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_弼影配方
		staticrecipe[1908]={recipe_id=5401,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_弋星配方
		staticrecipe[1909]={recipe_id=5402,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_孤云配方
		staticrecipe[1910]={recipe_id=5403,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_危崖配方
		staticrecipe[1911]={recipe_id=5404,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_缺月配方
		staticrecipe[1912]={recipe_id=5405,recipe_group=0,recipe_level=1,recipe_type=5}	---怀光_天心配方
		staticrecipe[1913]={recipe_id=5406,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_听魂配方
		staticrecipe[1914]={recipe_id=5407,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_幽声配方
		staticrecipe[1915]={recipe_id=5408,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_沧澜配方
		staticrecipe[1916]={recipe_id=5409,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_知秋配方
		staticrecipe[1917]={recipe_id=5410,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_入海配方
		staticrecipe[1918]={recipe_id=5411,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_凌空配方
		staticrecipe[1919]={recipe_id=5412,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_映月配方
		staticrecipe[1920]={recipe_id=5413,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_巍巍配方
		staticrecipe[1921]={recipe_id=5414,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_率然配方
		staticrecipe[1922]={recipe_id=5415,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_争鸣配方
		staticrecipe[1923]={recipe_id=5416,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_鸿儒配方
		staticrecipe[1924]={recipe_id=5417,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_骁将配方
		staticrecipe[1925]={recipe_id=5418,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_逸仙配方
		staticrecipe[1926]={recipe_id=5419,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_地灵配方
		staticrecipe[1927]={recipe_id=5420,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_物华配方
		staticrecipe[1928]={recipe_id=5421,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_天宝配方
		staticrecipe[1929]={recipe_id=5422,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_人杰配方
		staticrecipe[1930]={recipe_id=5423,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_鸣鸾配方
		staticrecipe[1931]={recipe_id=5424,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_君威配方
		staticrecipe[1932]={recipe_id=5425,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_遐迩配方
		staticrecipe[1933]={recipe_id=5426,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_余韵配方
		staticrecipe[1934]={recipe_id=5427,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_风语配方
		staticrecipe[1935]={recipe_id=5428,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_狂岚配方
		staticrecipe[1936]={recipe_id=5429,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_升阳配方
		staticrecipe[1937]={recipe_id=5430,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_初罡配方
		staticrecipe[1938]={recipe_id=5431,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_地延配方
		staticrecipe[1939]={recipe_id=5432,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_中冲配方
		staticrecipe[1940]={recipe_id=5433,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_嘈切配方
		staticrecipe[1941]={recipe_id=5434,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_朔盈配方
		staticrecipe[1942]={recipe_id=5435,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_温润配方
		staticrecipe[1943]={recipe_id=5436,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_寒梅配方
		staticrecipe[1944]={recipe_id=5437,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_庆吉配方
		staticrecipe[1945]={recipe_id=5438,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_轻歌配方
		staticrecipe[1946]={recipe_id=5439,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_曼舞配方
		staticrecipe[1947]={recipe_id=5440,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_远声配方
		staticrecipe[1948]={recipe_id=5441,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_云梦配方
		staticrecipe[1949]={recipe_id=5442,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_奇韵配方
		staticrecipe[1950]={recipe_id=5443,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_绝响配方
		staticrecipe[1951]={recipe_id=5444,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_鸿鹄配方
		staticrecipe[1952]={recipe_id=5445,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_运筹配方
		staticrecipe[1953]={recipe_id=5446,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_解语配方
		staticrecipe[1954]={recipe_id=5447,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_傲竹配方
		staticrecipe[1955]={recipe_id=5448,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_江渚配方
		staticrecipe[1956]={recipe_id=5449,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_塞曲配方
		staticrecipe[1957]={recipe_id=5450,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_辉煌配方
		staticrecipe[1958]={recipe_id=5451,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_锦屏配方
		staticrecipe[1959]={recipe_id=5452,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_冲霄配方
		staticrecipe[1960]={recipe_id=5453,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_上善配方
		staticrecipe[1961]={recipe_id=5454,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_连绵配方
		staticrecipe[1962]={recipe_id=5455,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_琴殇配方
		staticrecipe[1963]={recipe_id=5456,recipe_group=0,recipe_level=1,recipe_type=5}	---天华_天外配方
		staticrecipe[1964]={recipe_id=5457,recipe_group=0,recipe_level=1,recipe_type=5}	---足扫_手刀配方
		staticrecipe[1965]={recipe_id=5458,recipe_group=0,recipe_level=1,recipe_type=5}	---雷炎_龙辉配方
		staticrecipe[1966]={recipe_id=5459,recipe_group=0,recipe_level=1,recipe_type=5}	---神弃_死斗配方
		staticrecipe[1967]={recipe_id=5460,recipe_group=0,recipe_level=1,recipe_type=5}	---鬼翼_浮舟配方
		staticrecipe[1968]={recipe_id=5461,recipe_group=0,recipe_level=1,recipe_type=5}	---兔脱_蛇蜒配方
		staticrecipe[1969]={recipe_id=5462,recipe_group=0,recipe_level=1,recipe_type=5}	---鸩羽_蚊须配方
		staticrecipe[1970]={recipe_id=5463,recipe_group=0,recipe_level=1,recipe_type=5}	---蛛涎_蝎针配方
		staticrecipe[1971]={recipe_id=5464,recipe_group=0,recipe_level=1,recipe_type=5}	---仲寿_影灵配方
		staticrecipe[1972]={recipe_id=5465,recipe_group=0,recipe_level=1,recipe_type=5}	---孤云_缺月配方
		staticrecipe[1973]={recipe_id=5466,recipe_group=0,recipe_level=1,recipe_type=5}	---幽声_骁将配方
		staticrecipe[1974]={recipe_id=5467,recipe_group=0,recipe_level=1,recipe_type=5}	---巍巍_率然配方
		staticrecipe[1975]={recipe_id=5468,recipe_group=0,recipe_level=1,recipe_type=5}	---知秋_凌空配方
		staticrecipe[1976]={recipe_id=5469,recipe_group=0,recipe_level=1,recipe_type=5}	---风语_鸣鸾配方
		staticrecipe[1977]={recipe_id=5470,recipe_group=0,recipe_level=1,recipe_type=5}	---嘈切_朔盈配方
		staticrecipe[1978]={recipe_id=5471,recipe_group=0,recipe_level=1,recipe_type=5}	---中冲_庆吉配方
		staticrecipe[1979]={recipe_id=5472,recipe_group=0,recipe_level=1,recipe_type=5}	---云梦_奇韵配方
		staticrecipe[1980]={recipe_id=5473,recipe_group=0,recipe_level=1,recipe_type=5}	---辉煌_锦屏配方
		staticrecipe[1981]={recipe_id=5474,recipe_group=0,recipe_level=1,recipe_type=5}	---鸿鹄_运筹配方
		staticrecipe[1982]={recipe_id=5507,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_嚣魔配方
		staticrecipe[1983]={recipe_id=5508,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_汹渊配方
		staticrecipe[1984]={recipe_id=5509,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_忤逆配方
		staticrecipe[1985]={recipe_id=5510,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_神魂配方
		staticrecipe[1986]={recipe_id=5511,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_激扬配方
		staticrecipe[1987]={recipe_id=5512,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_狂情配方
		staticrecipe[1988]={recipe_id=5513,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_暴戾配方
		staticrecipe[1989]={recipe_id=5514,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_逆天配方
		staticrecipe[1990]={recipe_id=5515,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_紫宸配方
		staticrecipe[1991]={recipe_id=5516,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_惊神配方
		staticrecipe[1992]={recipe_id=5517,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_雄枭配方
		staticrecipe[1993]={recipe_id=5518,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_苍炎配方
		staticrecipe[1994]={recipe_id=5519,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_盈亏配方
		staticrecipe[1995]={recipe_id=5520,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_傲视配方
		staticrecipe[1996]={recipe_id=5521,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_妄劫配方
		staticrecipe[1997]={recipe_id=5522,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_战威配方
		staticrecipe[1998]={recipe_id=5523,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_情恨配方
		staticrecipe[1999]={recipe_id=5524,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_战栗配方
		staticrecipe[2000]={recipe_id=5525,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_心死配方
		staticrecipe[2001]={recipe_id=5526,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_蛊惑配方
		staticrecipe[2002]={recipe_id=5527,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_桎梏配方
		staticrecipe[2003]={recipe_id=5528,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_魇魔配方
		staticrecipe[2004]={recipe_id=5529,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_魂殁配方
		staticrecipe[2005]={recipe_id=5530,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_流云配方
		staticrecipe[2006]={recipe_id=5531,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_枯冢配方
		staticrecipe[2007]={recipe_id=5532,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_轮回配方
		staticrecipe[2008]={recipe_id=5533,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_扑朔配方
		staticrecipe[2009]={recipe_id=5534,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_陌路配方
		staticrecipe[2010]={recipe_id=5535,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_徒劳配方
		staticrecipe[2011]={recipe_id=5536,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_磷火配方
		staticrecipe[2012]={recipe_id=5537,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_亡魂配方
		staticrecipe[2013]={recipe_id=5538,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_踌躇配方
		staticrecipe[2014]={recipe_id=5539,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_天兆配方
		staticrecipe[2015]={recipe_id=5540,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_天骄配方
		staticrecipe[2016]={recipe_id=5541,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_天策配方
		staticrecipe[2017]={recipe_id=5542,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_真武配方
		staticrecipe[2018]={recipe_id=5543,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_干戈配方
		staticrecipe[2019]={recipe_id=5544,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_摧朽配方
		staticrecipe[2020]={recipe_id=5545,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_奎星配方
		staticrecipe[2021]={recipe_id=5546,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_百炼配方
		staticrecipe[2022]={recipe_id=5547,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_洪涛配方
		staticrecipe[2023]={recipe_id=5548,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_刺棘配方
		staticrecipe[2024]={recipe_id=5549,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_死劫配方
		staticrecipe[2025]={recipe_id=5550,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_金锋配方
		staticrecipe[2026]={recipe_id=5551,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_睥睨配方
		staticrecipe[2027]={recipe_id=5552,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_神咒配方
		staticrecipe[2028]={recipe_id=5553,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_魁斗配方
		staticrecipe[2029]={recipe_id=5554,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_绝命配方
		staticrecipe[2030]={recipe_id=5555,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_缠羁配方
		staticrecipe[2031]={recipe_id=5556,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_错脉配方
		staticrecipe[2032]={recipe_id=5557,recipe_group=0,recipe_level=1,recipe_type=5}	---太昊_伏魔配方
		staticrecipe[2033]={recipe_id=5558,recipe_group=0,recipe_level=1,recipe_type=5}	---神魂_逆天配方
		staticrecipe[2034]={recipe_id=5559,recipe_group=0,recipe_level=1,recipe_type=5}	---紫宸_心死配方
		staticrecipe[2035]={recipe_id=5560,recipe_group=0,recipe_level=1,recipe_type=5}	---情恨_蛊惑配方
		staticrecipe[2036]={recipe_id=5561,recipe_group=0,recipe_level=1,recipe_type=5}	---流云_天骄配方
		staticrecipe[2037]={recipe_id=5562,recipe_group=0,recipe_level=1,recipe_type=5}	---枯冢_天策配方
		staticrecipe[2038]={recipe_id=5563,recipe_group=0,recipe_level=1,recipe_type=5}	---轮回_真武配方
		staticrecipe[2039]={recipe_id=5564,recipe_group=0,recipe_level=1,recipe_type=5}	---干戈_神咒配方
		staticrecipe[2040]={recipe_id=5565,recipe_group=0,recipe_level=1,recipe_type=5}	---百炼_魁斗配方
		staticrecipe[2041]={recipe_id=5566,recipe_group=0,recipe_level=1,recipe_type=5}	---洪涛_刺棘配方
		staticrecipe[2042]={recipe_id=5567,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_丹心配方
		staticrecipe[2043]={recipe_id=5568,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_点绛配方
		staticrecipe[2044]={recipe_id=5569,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_荧惑配方
		staticrecipe[2045]={recipe_id=5570,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_陨星配方
		staticrecipe[2046]={recipe_id=5571,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_垂辉配方
		staticrecipe[2047]={recipe_id=5572,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_殷雷配方
		staticrecipe[2048]={recipe_id=5573,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_烁金配方
		staticrecipe[2049]={recipe_id=5574,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_荼火配方
		staticrecipe[2050]={recipe_id=5575,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_燃釜配方
		staticrecipe[2051]={recipe_id=5576,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_彰灼配方
		staticrecipe[2052]={recipe_id=5577,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_赴汤配方
		staticrecipe[2053]={recipe_id=5578,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_销焰配方
		staticrecipe[2054]={recipe_id=5579,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_烛烬配方
		staticrecipe[2055]={recipe_id=5580,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_灭度配方
		staticrecipe[2056]={recipe_id=5581,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_巨擘配方
		staticrecipe[2057]={recipe_id=5582,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_万炬配方
		staticrecipe[2058]={recipe_id=5583,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_炎山配方
		staticrecipe[2059]={recipe_id=5584,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_卓尔配方
		staticrecipe[2060]={recipe_id=5585,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_风声配方
		staticrecipe[2061]={recipe_id=5586,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_无执配方
		staticrecipe[2062]={recipe_id=5587,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_寒笙配方
		staticrecipe[2063]={recipe_id=5588,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_枯荣配方
		staticrecipe[2064]={recipe_id=5589,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_冷绡配方
		staticrecipe[2065]={recipe_id=5590,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_燕斜配方
		staticrecipe[2066]={recipe_id=5591,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_绝命配方
		staticrecipe[2067]={recipe_id=5592,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_火道配方
		staticrecipe[2068]={recipe_id=5593,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_元尘配方
		staticrecipe[2069]={recipe_id=5594,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_匹练配方
		staticrecipe[2070]={recipe_id=5595,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_无邪配方
		staticrecipe[2071]={recipe_id=5596,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_霜冷配方
		staticrecipe[2072]={recipe_id=5597,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_萍踪配方
		staticrecipe[2073]={recipe_id=5598,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_贯日配方
		staticrecipe[2074]={recipe_id=5599,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_余光配方
		staticrecipe[2075]={recipe_id=5600,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_长空配方
		staticrecipe[2076]={recipe_id=5601,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_真炎配方
		staticrecipe[2077]={recipe_id=5602,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_无痕配方
		staticrecipe[2078]={recipe_id=5603,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_先声配方
		staticrecipe[2079]={recipe_id=5604,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_玄山配方
		staticrecipe[2080]={recipe_id=5605,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_诡谲配方
		staticrecipe[2081]={recipe_id=5606,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_斗破配方
		staticrecipe[2082]={recipe_id=5607,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_鸣镝配方
		staticrecipe[2083]={recipe_id=5608,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_逆流配方
		staticrecipe[2084]={recipe_id=5609,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_劫海配方
		staticrecipe[2085]={recipe_id=5610,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_驰骋配方
		staticrecipe[2086]={recipe_id=5611,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_苦咒配方
		staticrecipe[2087]={recipe_id=5612,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_离殇配方
		staticrecipe[2088]={recipe_id=5613,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_伐魂配方
		staticrecipe[2089]={recipe_id=5614,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_罔顾配方
		staticrecipe[2090]={recipe_id=5615,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_恸地配方
		staticrecipe[2091]={recipe_id=5616,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_戮情配方
		staticrecipe[2092]={recipe_id=5617,recipe_group=0,recipe_level=1,recipe_type=5}	---焚香_无道配方
		staticrecipe[2093]={recipe_id=5618,recipe_group=0,recipe_level=1,recipe_type=5}	---燃釜_销焰配方
		staticrecipe[2094]={recipe_id=5619,recipe_group=0,recipe_level=1,recipe_type=5}	---彰灼_赴汤配方
		staticrecipe[2095]={recipe_id=5620,recipe_group=0,recipe_level=1,recipe_type=5}	---烛烬_灭度配方
		staticrecipe[2096]={recipe_id=5621,recipe_group=0,recipe_level=1,recipe_type=5}	---冷绡_元尘配方
		staticrecipe[2097]={recipe_id=5622,recipe_group=0,recipe_level=1,recipe_type=5}	---卓尔_枯荣配方
		staticrecipe[2098]={recipe_id=5623,recipe_group=0,recipe_level=1,recipe_type=5}	---匹练_霜冷配方
		staticrecipe[2099]={recipe_id=5624,recipe_group=0,recipe_level=1,recipe_type=5}	---斗破_鸣镝配方
		staticrecipe[2100]={recipe_id=5625,recipe_group=0,recipe_level=1,recipe_type=5}	---劫海_伐魂配方
		staticrecipe[2101]={recipe_id=5626,recipe_group=0,recipe_level=1,recipe_type=5}	---离殇_罔顾配方
		staticrecipe[2102]={recipe_id=4550,recipe_group=0,recipe_level=8,recipe_type=1}
		staticrecipe[2103]={recipe_id=4551,recipe_group=0,recipe_level=9,recipe_type=1}
		staticrecipe[2104]={recipe_id=4552,recipe_group=0,recipe_level=10,recipe_type=1}
		staticrecipe[2105]={recipe_id=4560,recipe_group=0,recipe_level=8,recipe_type=3}
		staticrecipe[2106]={recipe_id=4561,recipe_group=0,recipe_level=9,recipe_type=3}
		staticrecipe[2107]={recipe_id=4562,recipe_group=0,recipe_level=10,recipe_type=3}
		staticrecipe[2108]={recipe_id=4626,recipe_group=0,recipe_level=8,recipe_type=3}
		staticrecipe[2109]={recipe_id=4627,recipe_group=0,recipe_level=9,recipe_type=3}
		staticrecipe[2110]={recipe_id=4628,recipe_group=0,recipe_level=10,recipe_type=3}
		staticrecipe[2111]={recipe_id=4692,recipe_group=0,recipe_level=8,recipe_type=4}
		staticrecipe[2112]={recipe_id=4693,recipe_group=0,recipe_level=9,recipe_type=4}
		staticrecipe[2113]={recipe_id=4694,recipe_group=0,recipe_level=10,recipe_type=4}
		staticrecipe[2114]={recipe_id=4758,recipe_group=0,recipe_level=8,recipe_type=4}
		staticrecipe[2115]={recipe_id=4759,recipe_group=0,recipe_level=9,recipe_type=4}
		staticrecipe[2116]={recipe_id=4760,recipe_group=0,recipe_level=10,recipe_type=4}
		staticrecipe[2117]={recipe_id=4824,recipe_group=0,recipe_level=8,recipe_type=2}
		staticrecipe[2118]={recipe_id=4825,recipe_group=0,recipe_level=9,recipe_type=2}
		staticrecipe[2119]={recipe_id=4826,recipe_group=0,recipe_level=10,recipe_type=2}
		staticrecipe[2120]={recipe_id=4890,recipe_group=0,recipe_level=8,recipe_type=2}
		staticrecipe[2121]={recipe_id=4891,recipe_group=0,recipe_level=9,recipe_type=2}
		staticrecipe[2122]={recipe_id=4892,recipe_group=0,recipe_level=10,recipe_type=2}
		staticrecipe[2123]={recipe_id=4956,recipe_group=0,recipe_level=8,recipe_type=1}
		staticrecipe[2124]={recipe_id=4957,recipe_group=0,recipe_level=9,recipe_type=1}
		staticrecipe[2125]={recipe_id=4958,recipe_group=0,recipe_level=10,recipe_type=1}
		staticrecipe[2126]={recipe_id=4966,recipe_group=0,recipe_level=8,recipe_type=3}
		staticrecipe[2127]={recipe_id=4967,recipe_group=0,recipe_level=9,recipe_type=3}
		staticrecipe[2128]={recipe_id=4968,recipe_group=0,recipe_level=10,recipe_type=3}
		staticrecipe[2129]={recipe_id=5032,recipe_group=0,recipe_level=8,recipe_type=3}
		staticrecipe[2130]={recipe_id=5033,recipe_group=0,recipe_level=9,recipe_type=3}
		staticrecipe[2131]={recipe_id=5034,recipe_group=0,recipe_level=10,recipe_type=3}
		staticrecipe[2132]={recipe_id=5098,recipe_group=0,recipe_level=8,recipe_type=4}
		staticrecipe[2133]={recipe_id=5099,recipe_group=0,recipe_level=9,recipe_type=4}
		staticrecipe[2134]={recipe_id=5100,recipe_group=0,recipe_level=10,recipe_type=4}
		staticrecipe[2135]={recipe_id=5164,recipe_group=0,recipe_level=8,recipe_type=4}
		staticrecipe[2136]={recipe_id=5165,recipe_group=0,recipe_level=9,recipe_type=4}
		staticrecipe[2137]={recipe_id=5166,recipe_group=0,recipe_level=10,recipe_type=4}
		staticrecipe[2138]={recipe_id=5230,recipe_group=0,recipe_level=8,recipe_type=2}
		staticrecipe[2139]={recipe_id=5231,recipe_group=0,recipe_level=9,recipe_type=2}
		staticrecipe[2140]={recipe_id=5232,recipe_group=0,recipe_level=10,recipe_type=2}
		staticrecipe[2141]={recipe_id=5296,recipe_group=0,recipe_level=8,recipe_type=2}
		staticrecipe[2142]={recipe_id=5297,recipe_group=0,recipe_level=9,recipe_type=2}
		staticrecipe[2143]={recipe_id=5298,recipe_group=0,recipe_level=10,recipe_type=2}
		staticrecipe[2144]={recipe_id=5642,recipe_group=0,recipe_level=8,recipe_type=1}
		staticrecipe[2145]={recipe_id=5643,recipe_group=0,recipe_level=9,recipe_type=1}
		staticrecipe[2146]={recipe_id=5644,recipe_group=0,recipe_level=10,recipe_type=1}
		staticrecipe[2147]={recipe_id=5659,recipe_group=0,recipe_level=8,recipe_type=3}
		staticrecipe[2148]={recipe_id=5660,recipe_group=0,recipe_level=8,recipe_type=3}
		staticrecipe[2149]={recipe_id=5661,recipe_group=0,recipe_level=9,recipe_type=3}
		staticrecipe[2150]={recipe_id=5662,recipe_group=0,recipe_level=9,recipe_type=3}
		staticrecipe[2151]={recipe_id=5663,recipe_group=0,recipe_level=10,recipe_type=3}
		staticrecipe[2152]={recipe_id=5664,recipe_group=0,recipe_level=10,recipe_type=3}
		staticrecipe[2153]={recipe_id=5679,recipe_group=0,recipe_level=8,recipe_type=2}
		staticrecipe[2154]={recipe_id=5680,recipe_group=0,recipe_level=8,recipe_type=2}
		staticrecipe[2155]={recipe_id=5681,recipe_group=0,recipe_level=9,recipe_type=2}
		staticrecipe[2156]={recipe_id=5682,recipe_group=0,recipe_level=9,recipe_type=2}
		staticrecipe[2157]={recipe_id=5683,recipe_group=0,recipe_level=10,recipe_type=2}
		staticrecipe[2158]={recipe_id=5684,recipe_group=0,recipe_level=10,recipe_type=2}
		staticrecipe[2159]={recipe_id=5699,recipe_group=0,recipe_level=8,recipe_type=4}
		staticrecipe[2160]={recipe_id=5700,recipe_group=0,recipe_level=8,recipe_type=4}
		staticrecipe[2161]={recipe_id=5701,recipe_group=0,recipe_level=9,recipe_type=4}
		staticrecipe[2162]={recipe_id=5702,recipe_group=0,recipe_level=9,recipe_type=4}
		staticrecipe[2163]={recipe_id=5703,recipe_group=0,recipe_level=10,recipe_type=4}
		staticrecipe[2164]={recipe_id=5704,recipe_group=0,recipe_level=10,recipe_type=4}
		staticrecipe[2165]={recipe_id=5712,recipe_group=0,recipe_level=8,recipe_type=1}
		staticrecipe[2166]={recipe_id=5713,recipe_group=0,recipe_level=9,recipe_type=1}
		staticrecipe[2167]={recipe_id=5714,recipe_group=0,recipe_level=10,recipe_type=1}
		staticrecipe[2168]={recipe_id=5729,recipe_group=0,recipe_level=8,recipe_type=3}
		staticrecipe[2169]={recipe_id=5730,recipe_group=0,recipe_level=8,recipe_type=3}
		staticrecipe[2170]={recipe_id=5731,recipe_group=0,recipe_level=9,recipe_type=3}
		staticrecipe[2171]={recipe_id=5732,recipe_group=0,recipe_level=9,recipe_type=3}
		staticrecipe[2172]={recipe_id=5733,recipe_group=0,recipe_level=10,recipe_type=3}
		staticrecipe[2173]={recipe_id=5734,recipe_group=0,recipe_level=10,recipe_type=3}
		staticrecipe[2174]={recipe_id=5749,recipe_group=0,recipe_level=8,recipe_type=2}
		staticrecipe[2175]={recipe_id=5750,recipe_group=0,recipe_level=8,recipe_type=2}
		staticrecipe[2176]={recipe_id=5751,recipe_group=0,recipe_level=9,recipe_type=2}
		staticrecipe[2177]={recipe_id=5752,recipe_group=0,recipe_level=9,recipe_type=2}
		staticrecipe[2178]={recipe_id=5753,recipe_group=0,recipe_level=10,recipe_type=2}
		staticrecipe[2179]={recipe_id=5754,recipe_group=0,recipe_level=10,recipe_type=2}
		staticrecipe[2180]={recipe_id=5769,recipe_group=0,recipe_level=8,recipe_type=4}
		staticrecipe[2181]={recipe_id=5770,recipe_group=0,recipe_level=8,recipe_type=4}
		staticrecipe[2182]={recipe_id=5771,recipe_group=0,recipe_level=9,recipe_type=4}
		staticrecipe[2183]={recipe_id=5772,recipe_group=0,recipe_level=9,recipe_type=4}
		staticrecipe[2184]={recipe_id=5773,recipe_group=0,recipe_level=10,recipe_type=4}
		staticrecipe[2185]={recipe_id=5774,recipe_group=0,recipe_level=10,recipe_type=4}
		staticrecipe[2186]={recipe_id=5819,recipe_group=0,recipe_level=8,recipe_type=2}
		staticrecipe[2187]={recipe_id=5820,recipe_group=0,recipe_level=8,recipe_type=2}
		staticrecipe[2188]={recipe_id=5821,recipe_group=0,recipe_level=9,recipe_type=2}
		staticrecipe[2189]={recipe_id=5822,recipe_group=0,recipe_level=9,recipe_type=2}
		staticrecipe[2190]={recipe_id=5823,recipe_group=0,recipe_level=10,recipe_type=2}
		staticrecipe[2191]={recipe_id=5824,recipe_group=0,recipe_level=10,recipe_type=2}
		staticrecipe[2192]={recipe_id=5839,recipe_group=0,recipe_level=8,recipe_type=4}
		staticrecipe[2193]={recipe_id=5840,recipe_group=0,recipe_level=8,recipe_type=4}
		staticrecipe[2194]={recipe_id=5841,recipe_group=0,recipe_level=9,recipe_type=4}
		staticrecipe[2195]={recipe_id=5842,recipe_group=0,recipe_level=9,recipe_type=4}
		staticrecipe[2196]={recipe_id=5843,recipe_group=0,recipe_level=10,recipe_type=4}
		staticrecipe[2197]={recipe_id=5844,recipe_group=0,recipe_level=10,recipe_type=4}
		staticrecipe[2198]={recipe_id=5799,recipe_group=0,recipe_level=8,recipe_type=3}
		staticrecipe[2199]={recipe_id=5800,recipe_group=0,recipe_level=8,recipe_type=3}
		staticrecipe[2200]={recipe_id=5801,recipe_group=0,recipe_level=9,recipe_type=3}
		staticrecipe[2201]={recipe_id=5802,recipe_group=0,recipe_level=9,recipe_type=3}
		staticrecipe[2202]={recipe_id=5803,recipe_group=0,recipe_level=10,recipe_type=3}
		staticrecipe[2203]={recipe_id=5804,recipe_group=0,recipe_level=10,recipe_type=3}
		staticrecipe[2204]={recipe_id=5782,recipe_group=0,recipe_level=8,recipe_type=1}
		staticrecipe[2205]={recipe_id=5783,recipe_group=0,recipe_level=9,recipe_type=1}
		staticrecipe[2206]={recipe_id=5784,recipe_group=0,recipe_level=10,recipe_type=1}
		staticrecipe[2207]={recipe_id=5850,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2208]={recipe_id=5851,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2209]={recipe_id=5852,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2210]={recipe_id=5853,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2211]={recipe_id=5854,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2212]={recipe_id=5855,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2213]={recipe_id=5856,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2214]={recipe_id=5857,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2215]={recipe_id=5858,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2216]={recipe_id=5859,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2217]={recipe_id=5860,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2218]={recipe_id=5861,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2219]={recipe_id=5862,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2220]={recipe_id=5863,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2221]={recipe_id=5864,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2222]={recipe_id=5865,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2223]={recipe_id=5866,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2224]={recipe_id=5867,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2225]={recipe_id=5868,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2226]={recipe_id=5869,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2227]={recipe_id=5870,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2228]={recipe_id=5871,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2229]={recipe_id=5872,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2230]={recipe_id=5873,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2231]={recipe_id=5874,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2232]={recipe_id=5875,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2233]={recipe_id=5876,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2234]={recipe_id=5877,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2235]={recipe_id=5878,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2236]={recipe_id=5879,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2237]={recipe_id=5880,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2238]={recipe_id=5881,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2239]={recipe_id=5882,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2240]={recipe_id=5883,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2241]={recipe_id=5884,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2242]={recipe_id=5885,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2243]={recipe_id=5886,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2244]={recipe_id=5887,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2245]={recipe_id=5888,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2246]={recipe_id=5889,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2247]={recipe_id=5890,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2248]={recipe_id=5891,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2249]={recipe_id=5892,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2250]={recipe_id=5893,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2251]={recipe_id=5894,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2252]={recipe_id=5895,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2253]={recipe_id=5896,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2254]={recipe_id=5897,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2255]={recipe_id=5898,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2256]={recipe_id=5899,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2257]={recipe_id=5900,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2258]={recipe_id=5901,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2259]={recipe_id=5902,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2260]={recipe_id=5903,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2261]={recipe_id=5904,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2262]={recipe_id=5905,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2263]={recipe_id=5906,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2264]={recipe_id=5907,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2265]={recipe_id=5908,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2266]={recipe_id=5909,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2267]={recipe_id=5917,recipe_group=0,recipe_level=8,recipe_type=1}
		staticrecipe[2268]={recipe_id=5918,recipe_group=0,recipe_level=9,recipe_type=1}
		staticrecipe[2269]={recipe_id=5919,recipe_group=0,recipe_level=10,recipe_type=1}
		staticrecipe[2270]={recipe_id=5934,recipe_group=0,recipe_level=8,recipe_type=3}
		staticrecipe[2271]={recipe_id=5935,recipe_group=0,recipe_level=8,recipe_type=3}
		staticrecipe[2272]={recipe_id=5936,recipe_group=0,recipe_level=9,recipe_type=3}
		staticrecipe[2273]={recipe_id=5937,recipe_group=0,recipe_level=9,recipe_type=3}
		staticrecipe[2274]={recipe_id=5938,recipe_group=0,recipe_level=10,recipe_type=3}
		staticrecipe[2275]={recipe_id=5939,recipe_group=0,recipe_level=10,recipe_type=3}
		staticrecipe[2276]={recipe_id=5954,recipe_group=0,recipe_level=8,recipe_type=2}
		staticrecipe[2277]={recipe_id=5955,recipe_group=0,recipe_level=8,recipe_type=2}
		staticrecipe[2278]={recipe_id=5956,recipe_group=0,recipe_level=9,recipe_type=2}
		staticrecipe[2279]={recipe_id=5957,recipe_group=0,recipe_level=9,recipe_type=2}
		staticrecipe[2280]={recipe_id=5958,recipe_group=0,recipe_level=10,recipe_type=2}
		staticrecipe[2281]={recipe_id=5959,recipe_group=0,recipe_level=10,recipe_type=2}
		staticrecipe[2282]={recipe_id=5974,recipe_group=0,recipe_level=8,recipe_type=4}
		staticrecipe[2283]={recipe_id=5975,recipe_group=0,recipe_level=8,recipe_type=4}
		staticrecipe[2284]={recipe_id=5976,recipe_group=0,recipe_level=9,recipe_type=4}
		staticrecipe[2285]={recipe_id=5977,recipe_group=0,recipe_level=9,recipe_type=4}
		staticrecipe[2286]={recipe_id=5978,recipe_group=0,recipe_level=10,recipe_type=4}
		staticrecipe[2287]={recipe_id=5979,recipe_group=0,recipe_level=10,recipe_type=4}
		staticrecipe[2288]={recipe_id=5987,recipe_group=0,recipe_level=8,recipe_type=1}
		staticrecipe[2289]={recipe_id=5988,recipe_group=0,recipe_level=9,recipe_type=1}
		staticrecipe[2290]={recipe_id=5989,recipe_group=0,recipe_level=10,recipe_type=1}
		staticrecipe[2291]={recipe_id=5997,recipe_group=0,recipe_level=8,recipe_type=3}
		staticrecipe[2292]={recipe_id=5998,recipe_group=0,recipe_level=9,recipe_type=3}
		staticrecipe[2293]={recipe_id=5999,recipe_group=0,recipe_level=10,recipe_type=3}
		staticrecipe[2294]={recipe_id=6007,recipe_group=0,recipe_level=8,recipe_type=2}
		staticrecipe[2295]={recipe_id=6008,recipe_group=0,recipe_level=9,recipe_type=2}
		staticrecipe[2296]={recipe_id=6009,recipe_group=0,recipe_level=10,recipe_type=2}
		staticrecipe[2297]={recipe_id=6017,recipe_group=0,recipe_level=8,recipe_type=4}
		staticrecipe[2298]={recipe_id=6018,recipe_group=0,recipe_level=9,recipe_type=4}
		staticrecipe[2299]={recipe_id=6019,recipe_group=0,recipe_level=10,recipe_type=4}
		staticrecipe[2300]={recipe_id=6030,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2301]={recipe_id=6031,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2302]={recipe_id=6032,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2303]={recipe_id=6033,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2304]={recipe_id=6034,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2305]={recipe_id=6035,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2306]={recipe_id=6036,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2307]={recipe_id=6037,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2308]={recipe_id=6038,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2309]={recipe_id=6039,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2310]={recipe_id=6040,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2311]={recipe_id=6041,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2312]={recipe_id=6042,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2313]={recipe_id=6043,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2314]={recipe_id=6044,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2315]={recipe_id=6045,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2316]={recipe_id=6046,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2317]={recipe_id=6047,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2318]={recipe_id=6048,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2319]={recipe_id=6049,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2320]={recipe_id=6050,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2321]={recipe_id=6051,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2322]={recipe_id=6052,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2323]={recipe_id=6053,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2324]={recipe_id=6054,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2325]={recipe_id=6055,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2326]={recipe_id=6056,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2327]={recipe_id=6057,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2328]={recipe_id=6058,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2329]={recipe_id=6059,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2330]={recipe_id=6060,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2331]={recipe_id=6061,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2332]={recipe_id=6062,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2333]={recipe_id=6063,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2334]={recipe_id=6064,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2335]={recipe_id=6065,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2336]={recipe_id=6066,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2337]={recipe_id=6067,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2338]={recipe_id=6068,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2339]={recipe_id=6069,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2340]={recipe_id=6070,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2341]={recipe_id=6071,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2342]={recipe_id=6072,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2343]={recipe_id=6073,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2344]={recipe_id=6074,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2345]={recipe_id=6075,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2346]={recipe_id=6076,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2347]={recipe_id=6077,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2348]={recipe_id=6078,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2349]={recipe_id=6079,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2350]={recipe_id=6080,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2351]={recipe_id=6081,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2352]={recipe_id=6082,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2353]={recipe_id=6083,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2354]={recipe_id=6084,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2355]={recipe_id=6085,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2356]={recipe_id=6086,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2357]={recipe_id=6087,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2358]={recipe_id=6088,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2359]={recipe_id=6089,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2360]={recipe_id=6090,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2361]={recipe_id=6091,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2362]={recipe_id=6092,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2363]={recipe_id=6093,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2364]={recipe_id=6094,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2365]={recipe_id=6095,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2366]={recipe_id=6096,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2367]={recipe_id=6097,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2368]={recipe_id=6098,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2369]={recipe_id=6099,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2370]={recipe_id=6100,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2371]={recipe_id=6101,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2372]={recipe_id=6102,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2373]={recipe_id=6103,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2374]={recipe_id=6104,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2375]={recipe_id=6105,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2376]={recipe_id=6106,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2377]={recipe_id=6107,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2378]={recipe_id=6108,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2379]={recipe_id=6109,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2380]={recipe_id=6110,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2381]={recipe_id=6111,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2382]={recipe_id=6112,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2383]={recipe_id=6113,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2384]={recipe_id=6114,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2385]={recipe_id=6115,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2386]={recipe_id=6116,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2387]={recipe_id=6117,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2388]={recipe_id=6118,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2389]={recipe_id=6119,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2390]={recipe_id=6120,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2391]={recipe_id=6121,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2392]={recipe_id=6122,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2393]={recipe_id=6123,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2394]={recipe_id=6124,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2395]={recipe_id=6125,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2396]={recipe_id=6126,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2397]={recipe_id=6127,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2398]={recipe_id=6128,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2399]={recipe_id=6129,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2400]={recipe_id=6130,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2401]={recipe_id=6131,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2402]={recipe_id=6132,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2403]={recipe_id=6133,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2404]={recipe_id=6134,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2405]={recipe_id=6135,recipe_group=0,recipe_level=1,recipe_type=5}
		staticrecipe[2406]={recipe_id=6136,recipe_group=0,recipe_level=1,recipe_type=5}



	return staticrecipe
end 
---//************************************************************************************************- 
---//************************************************************************************************-  
---//************************************************************************************************- 
---//************************************************************************************************- 
---//宠物捕捉部分开始--------------------------------------------------------------------------------
---//怪物种族合理性判断------------------------------------------------------------------------------
function ZCatchIsAvailableMonsRace(monster_raceinfo)
	if monster_raceinfo>=1 and monster_raceinfo<65536 then 
		return 1
	else
		return 0
	end 	
end 
---//怪物种族对应的捕捉技能id
function ZSkillsOfPetCatch(monster_raceinfo)
	local returnedskillid=0
	local msg_id_whencatch=0		---//临时测试
	local catchskilllist={}
		catchskilllist[1]={raceinfo=1,skillid=1971,msg_id=311}				---//人族
		catchskilllist[2]={raceinfo=2,skillid=1973,msg_id=312}				---//妖族
		catchskilllist[3]={raceinfo=4,skillid=1973,msg_id=313}				---//鬼族
		catchskilllist[4]={raceinfo=8,skillid=1971,msg_id=314}				---//羽族
		catchskilllist[5]={raceinfo=16,skillid=1973,msg_id=315}				---//魅族
		catchskilllist[6]={raceinfo=32,skillid=1971,msg_id=316}				---//天将
		catchskilllist[7]={raceinfo=64,skillid=1972,msg_id=317}				---//仙灵
		catchskilllist[8]={raceinfo=128,skillid=1972,msg_id=318}			---//山精
		catchskilllist[9]={raceinfo=256,skillid=1974,msg_id=319}			---//野兽
		catchskilllist[10]={raceinfo=512,skillid=1972,msg_id=320}			---//异兽
		catchskilllist[11]={raceinfo=1024,skillid=1974,msg_id=321}		---//水族
		catchskilllist[12]={raceinfo=2048,skillid=1974,msg_id=322}		---//虫蛊
		catchskilllist[13]={raceinfo=4096,skillid=1973,msg_id=323}		---//幻灵
		catchskilllist[14]={raceinfo=8192,skillid=1971,msg_id=324}		---//死灵
		catchskilllist[15]={raceinfo=16384,skillid=1974,msg_id=325}		---//元素
		catchskilllist[16]={raceinfo=32768,skillid=1972,msg_id=326}		---//特殊

	for i,v in pairs(catchskilllist) do
		if monster_raceinfo == v.raceinfo then 
			returnedskillid=v.skillid
			msg_id_whencatch=v.msg_id
		end
	end
	return returnedskillid,msg_id_whencatch
end

---//怪物捕捉难度对于捕捉成功率的影响----------------------------------------------------------------
function ZCatchChanceFromDifficulty(monster_catchdifficulty)
	if type(monster_catchdifficulty)~="number" or monster_catchdifficulty<=0 or monster_catchdifficulty>8 then 
		monster_catchdifficulty=9
	end 
	local difficultyratio	={0.95,0.85,0.80,0.75,0.65,0.55,0.45,0.25,0.01}			---//捕捉难度导致的难度系数
	local difficultyaddexp={1,2,4,8,12,16,24,32,0}														---//捕捉难度导致的技能熟练度增加
	
	return difficultyratio[monster_catchdifficulty],difficultyaddexp[monster_catchdifficulty]
end 

---//怪物气血对于捕捉成功率的影响--------------------------------------------------------------------
function ZCatchChanceFromHP(monster_hp)
	if type(monster_hp)~="number" or monster_hp<=0 or monster_hp>1 then 
		monster_hp=1
	end 
	local hpratio=math.min(math.max(1-0.5*monster_hp*monster_hp,0),1)
	return hpratio
end 

---//人物怪物级别差对于捕捉成功率的影响-----------------------------------------------------------
function ZCatchChanceFromLevels(monster_level,player_level)
	if type(monster_level)~="number" or monster_level<1 or monster_level>150 then 
		monster_level=150
	end 
	if type(player_level)~="number" or player_level<1 or player_level>160 then 
		player_level=1
	end 
	local levelplus=1
	if player_level>monster_level*2 then 
		levelplus=10
	elseif player_level<monster_level*0.5 then 
		levelplus=-10
	else
		levelplus=0
	end 	
	return levelplus
end 

---//宠物捕捉主函数---------------------------------------------------------------
function PetBedgeCatch(monster_id, monster_level, monster_raceinfo, monster_catchdifficulty,monster_hp,player_level, player)
	local pet_id,msg_channel,msg_id=0,0,301
	local maincatchskillid=1970				---//捕捉技能id
	local passive_catch_skillid=0			---//race对应的被动捕捉精通技能id
	local finnalcatchchance=0					---//捕捉的总成功率
	local iscatch=0										---//是否捕捉成功
	local skill_alvl,skill_blvl=0,0		---//被动捕捉精通技能的加点等级、总等级
	local maxraceinfo=65536						---//最大种族允许的编号(2^16)
	local addskillexp=0								---//捕捉增加的技能熟练度
	local availablepetlist=ZStaticAvailableMonsterList()
	
	---//2010年特殊捕捉需求，潘珊
	if monster_id==33370 then
		return 33528,0,327
	end
	
	---//怪物id不正确
	if type(monster_id)~="number" or type(monster_raceinfo)~="number" then
	 return pet_id,msg_channel,302
	end 
	---//怪物级别不正确
	if type(monster_level)~="number" or monster_level<1 or monster_level>160 then
	 return pet_id,msg_channel,303
	end 
	---//怪物种族不正确
	if type(monster_raceinfo)~="number" or monster_raceinfo<=0 or monster_raceinfo>maxraceinfo or ZCatchIsAvailableMonsRace(monster_raceinfo)==0 then
	 return pet_id,msg_channel,304
	end 	
	---//模板中捕捉系数不正确或者不可以捕捉
	if type(monster_catchdifficulty)~="number" or monster_catchdifficulty<=0 or monster_catchdifficulty>8 then
	 return pet_id,msg_channel,305
	end 	
	---//气血不满足条件,高于50%
	if type(monster_hp)~="number" or monster_hp<=0 or monster_hp>0.5 then
	 return pet_id,msg_channel,306
	end 	
	---//玩家级别传递错误
	if type(player_level)~="number" or player_level<=0 or player_level>160 then
	 return pet_id,msg_channel,307
	end 	

	passive_catch_skillid,msg_id=ZSkillsOfPetCatch(monster_raceinfo)
	skill_alvl,skill_blvl=player:PlayerQuerySkilllvl(passive_catch_skillid)
	
	---//难度导致的捕捉因子，难度导致的增加技能熟练
	local difficultyratio,addexpifcatch=ZCatchChanceFromDifficulty(monster_catchdifficulty)
	---//怪物HP比例导致的捕捉因子
	local HPratio=ZCatchChanceFromHP(monster_hp)
	---//人和怪物级别差导致的捕捉加成/衰减
	local leveldiffplus=ZCatchChanceFromLevels(monster_level,player_level)
	finnalcatchchance=(0.6*skill_alvl*skill_alvl+5*skill_alvl+10)*difficultyratio*HPratio+leveldiffplus
	iscatch=ZMiscThrowDice(finnalcatchchance)
	if iscatch==1 then 
		for i,v in pairs(availablepetlist) do
			if v.monster_id==monster_id then 
				pet_id=v.pet_id
			end 
		end 
		player:AddSkillProficiency(passive_catch_skillid,addexpifcatch) 
		player:AddSkillProficiency(maincatchskillid,10) 
	else
		pet_id=0
		msg_id=309
		addskillexp=0
		player:AddSkillProficiency(maincatchskillid,1) 
	end 
		return pet_id,msg_channel,msg_id
end
---//宠物捕捉部分结束-------------------------------------------------------------------------------- 
---//************************************************************************************************- 
---//************************************************************************************************- 
---//************************************************************************************************- 
---//************************************************************************************************- 
---//生产专精部分开始-------------------------------------------------------------------------------- 
---//获得配方的基本属性信息--------------------------------------------------------------------------
function ZRecipeGetProps(recipe_id)
	if type(recipe_id)~="number" then
	 return 0,0,401
	end 	
	local recipe_group=99				---//0,人物生产，1,宠物生产 ,99-error
	local recipe_level=99				---//配方等级1-10,99-error
	local recipe_type=99				---//1,人物武器，2人物衣服，3人物帽子，4人物鞋子，5，人物其它
															---//11 宠物相关,99-error
	local recipelist=ZStaticRecipeList()
	for i,v in pairs(recipelist) do
		if v.recipe_id == recipe_id then 
			recipe_group=v.recipe_group 
			recipe_level=v.recipe_level
			recipe_type=v.recipe_type
		end
	end
	if recipe_group~=1 and recipe_group~=0 then recipe_group=99 end   	
	if recipe_level<1 or recipe_level>10 then recipe_level=99 end  
	if recipe_type~=1 and recipe_type~=2 and recipe_type~=3 and recipe_type~=4 and recipe_type~=5 and recipe_type~=11 then recipe_type=99 end 												
	return recipe_group,recipe_level,recipe_type
end 
---//获得配方对应的技能等级信息--------------------------------------------------------------------------
function ZSkillsOfRecipes(recipe_type,recipe_id,player_level,iscreteinuniqueplace)
	if recipe_type==99 or type(recipe_type)~="number" then return 0 end 
	local re_skill_1,re_skill_2,re_skill_3,re_skill_4=0,0,0,0
	local recipeskilllist={}
		if iscreteinuniqueplace==1 then 
			recipeskilllist[1]={recipe_type=1,skillid_1=1979,skillid_2=389,skillid_3=389,skillid_4=389}
			recipeskilllist[2]={recipe_type=2,skillid_1=1980,skillid_2=389,skillid_3=389,skillid_4=389}
			recipeskilllist[3]={recipe_type=3,skillid_1=1981,skillid_2=389,skillid_3=389,skillid_4=389}
			recipeskilllist[4]={recipe_type=4,skillid_1=1982,skillid_2=389,skillid_3=389,skillid_4=389}
			recipeskilllist[5]={recipe_type=5,skillid_1=1984,skillid_2=389,skillid_3=389,skillid_4=389}
			recipeskilllist[6]={recipe_type=11,skillid_1=1983,skillid_2=389,skillid_3=389,skillid_4=389}
		else
			recipeskilllist[1]={recipe_type=1,skillid_1=1979,skillid_2=389,skillid_3=389,skillid_4=389}
			recipeskilllist[2]={recipe_type=2,skillid_1=1980,skillid_2=389,skillid_3=389,skillid_4=389}
			recipeskilllist[3]={recipe_type=3,skillid_1=1981,skillid_2=389,skillid_3=389,skillid_4=389}
			recipeskilllist[4]={recipe_type=4,skillid_1=1982,skillid_2=389,skillid_3=389,skillid_4=389}
			recipeskilllist[5]={recipe_type=5,skillid_1=1984,skillid_2=389,skillid_3=389,skillid_4=389}
			recipeskilllist[6]={recipe_type=11,skillid_1=1983,skillid_2=389,skillid_3=389,skillid_4=389}
		end 	
	for i,v in pairs(recipeskilllist) do
		if recipe_type == v.recipe_type then 
			re_skill_1=v.skillid_1
			re_skill_2=v.skillid_2
			re_skill_3=v.skillid_3
			re_skill_4=v.skillid_4
		end
	end
	return re_skill_1,re_skill_2,re_skill_3,re_skill_4	

end 
---//获得配方额外产出的产出信息-------------------------------------------------------------------------
function ZStaticRecipeExtItems(recipe_group,recipe_level,recipe_type)
	local recipeextlist={}
	local recipeextchance={}
	if recipe_group==0 and recipe_level<8 and recipe_type~=5 then 
		return 0,0
	end
	
	if recipe_group==0 then					---//人物生产
			recipeextlist={30185,30186,30187,30188,30189}
			if recipe_level==8 then 
				recipeextchance={27000,18000,9000,1000,100}		
			elseif recipe_level==9 then
				recipeextchance={27000,18000,9000,2000,200}			
			elseif recipe_level==10 then
				recipeextchance={27000,18000,9000,3000,300}					
			else 
				recipeextchance={27000,18000,9000,100,100}				
			end 
	elseif recipe_group==1 then 		---//宠物生产
			recipeextlist={30190,30191,30192,30193,30194}
			if recipe_level==1 then
				recipeextchance={27000,18000,9000,1000,100}
			elseif recipe_level==2 then
				recipeextchance={27000,18000,9000,2000,200}
			elseif recipe_level==3 then
				recipeextchance={27000,18000,9000,200,50}
			else
				recipeextchance={27000,18000,9000,100,100}
			end 							
	else
			recipeextlist={0,0,0,0,0}
			recipeextchance={100,100,100,100,100}
	end 	

		return recipeextlist[ZMiscGetRanPoint(recipeextchance)],1
end

---//生产获得额外产出主函数--------------------------------------------------------------------------
function ExtGainOfCreate(recipe_id, player_level, player_gender, player_prof, player)
	local item_id=0											---//产出物品id
	local item_count=0									---//产出物品数量
	local msg_id=401										---//提示信息	
	local discover_type,create_type=0,1	---//在生产时，discover_type默认为0，所有create_type暂默认为1

	if type(recipe_id)~="number" or type(player_level)~="number" or player_level<0 or player_level>160 then
	 return item_id,item_count,msg_id
	end 	
	
	local withskill_1,withskill_2,withskill_3,withskill_4=0,0,0,0			---//对应技能,最多四个
	local finnalrecipechance=0																				---//产出物品概率
	local iscreteinuniqueplace=0																			---//是否存在特定区域			
	local uniqueitem=0						
	local uniquecount=0				
	local skill_alvl1,skill_blvl1=0,0
	local skill_alvl2,skill_blvl2=0,0
	local skill_alvl3,skill_blvl3=0,0
	local skill_alvl4,skill_blvl4=0,0
	
	local recipe_group,recipe_level,recipe_type=0,1,1
	---//0,人物生产，1,宠物生产 ,99-error
	---//配方等级1-10,99-error
	---//1,人物武器，2人物衣服，3人物帽子，4人物鞋子，5，其它
	---//11 宠物相关,99-error
	local maps_id,maps_x,maps_y,maps_z=0,0,0,0
	---//地图信息
	
	maps_id,maps_x,maps_y,maps_z=player:PlayerQueryPosition()
	uniqueitem,uniquecount=ZMiscFindInAreas(discover_type,create_type,maps_id,maps_x,maps_y,maps_z,recipe_id)

	if uniqueitem>0 and uniquecount>0 then 	iscreteinuniqueplace=1 end 
	
	recipe_group,recipe_level,recipe_type=ZRecipeGetProps(recipe_id)
	withskill_1,withskill_2,withskill_3,withskill_4=ZSkillsOfRecipes(recipe_type,recipe_id,player_level,iscreteinuniqueplace)
	skill_alvl1,skill_blvl1=player:PlayerQuerySkilllvl(withskill_1)
	skill_alvl2,skill_blvl2=player:PlayerQuerySkilllvl(withskill_2)
	skill_alvl3,skill_blvl3=player:PlayerQuerySkilllvl(withskill_3)
	skill_alvl4,skill_blvl4=player:PlayerQuerySkilllvl(withskill_4)
	
	local finnalrecipechance=0				---//是否有额外产出
	local passive_create_skillid=0		---//增加熟练度的技能
	local addskillexp=0								---//增加的熟练度
	---//鉴于现在只有一个技能在使用
	finnalrecipechance=6*skill_alvl1+15
	if recipe_group==99 or recipe_level==99 or recipe_type==99 then finnalrecipechance=0 end
	
	if ZMiscThrowDice(finnalrecipechance)==1 then 
			if iscreteinuniqueplace==1 then 
					item_id,item_count=ZMiscFindInAreas(discover_type,create_type,maps_id,maps_x,maps_y,maps_z,recipe_id)
			else
					item_id,item_count=ZStaticRecipeExtItems(recipe_group,recipe_level,recipe_type)
					player:AddSkillProficiency(withskill_1,1) 
			end 	
			msg_id=402
	else

			msg_id=403
	end 
		return item_id,item_count,msg_id

end 
---//生产专精部分结束-------------------------------------------------------------------------------- 
---//************************************************************************************************- 
---//************************************************************************************************- 
---//************************************************************************************************- 
---//************************************************************************************************- 
---//探寻系统部分开始-------------------------------------------------------------------------------- 
---//地图区域掉落数据
function ZStaticAreaDrops(discover_type,create_type)
	local areamiscitems={}
	---//discover and create 公用掉落数据(if create need sometimes)
	---//地图id,maps_id,-1代表不限制地图
	---//是否独占产出(即此点是独占点，独占点会屏蔽其它非独占点)
	---//坐标起始点，坐标终止点(左下--->右上)
	---//产生物品的id，产生物品的数量下限，产生物品的数量上限
	---//几率(符合条件的区域会归一化，不超过32(maxitemslist)种
	---//配方id,create单独用数据项
	if discover_type==1 and create_type==0 then 
		areamiscitems[1]={maps_id=1,is_unique=0,maps_x1=-400,maps_y1=-400,maps_z1=-400,maps_x2=400,maps_y2=400,maps_z2=400,item_id=16812,item_count1=1,item_count2=3,dicovery_chance=100,recipe_id=-1}
		areamiscitems[2]={maps_id=1,is_unique=0,maps_x1=-400,maps_y1=-400,maps_z1=-400,maps_x2=400,maps_y2=400,maps_z2=400,item_id=16813,item_count1=1,item_count2=3,dicovery_chance=100,recipe_id=-1}
		areamiscitems[3]={maps_id=1,is_unique=0,maps_x1=-400,maps_y1=-400,maps_z1=-400,maps_x2=400,maps_y2=400,maps_z2=400,item_id=16814,item_count1=1,item_count2=3,dicovery_chance=100,recipe_id=-1}
		areamiscitems[4]={maps_id=1,is_unique=0,maps_x1=0,maps_y1=-400,maps_z1=-400,maps_x2=400,maps_y2=400,maps_z2=400,item_id=16815,item_count1=1,item_count2=3,dicovery_chance=100,recipe_id=-1}
		areamiscitems[5]={maps_id=1,is_unique=0,maps_x1=0,maps_y1=-400,maps_z1=-400,maps_x2=400,maps_y2=400,maps_z2=400,item_id=16816,item_count1=1,item_count2=3,dicovery_chance=100,recipe_id=-1}
		areamiscitems[6]={maps_id=1,is_unique=0,maps_x1=0,maps_y1=-400,maps_z1=-400,maps_x2=400,maps_y2=400,maps_z2=400,item_id=16817,item_count1=1,item_count2=3,dicovery_chance=100,recipe_id=-1}
		areamiscitems[7]={maps_id=-1,is_unique=0,maps_x1=-400,maps_y1=-400,maps_z1=-400,maps_x2=400,maps_y2=400,maps_z2=400,item_id=16830,item_count1=1,item_count2=3,dicovery_chance=100,recipe_id=-1}
	elseif discover_type==2 and create_type==0 then
		areamiscitems[1]={maps_id=2,is_unique=0,maps_x1=-400,maps_y1=-400,maps_z1=-400,maps_x2=400,maps_y2=400,maps_z2=400,item_id=16823,item_count1=1,item_count2=3,dicovery_chance=100,recipe_id=-1}
		areamiscitems[2]={maps_id=3,is_unique=0,maps_x1=-400,maps_y1=-400,maps_z1=-400,maps_x2=400,maps_y2=400,maps_z2=400,item_id=16824,item_count1=1,item_count2=3,dicovery_chance=100,recipe_id=-1}
		areamiscitems[3]={maps_id=4,is_unique=0,maps_x1=-400,maps_y1=-400,maps_z1=-400,maps_x2=400,maps_y2=400,maps_z2=400,item_id=16825,item_count1=1,item_count2=3,dicovery_chance=100,recipe_id=-1}
	elseif create_type==0 and discover_type>2 then 
		areamiscitems[1]={maps_id=2,is_unique=0,maps_x1=-400,maps_y1=-400,maps_z1=-400,maps_x2=400,maps_y2=400,maps_z2=400,item_id=0,item_count1=0,item_count2=0,dicovery_chance=100,recipe_id=-1}
		areamiscitems[2]={maps_id=3,is_unique=0,maps_x1=-400,maps_y1=-400,maps_z1=-400,maps_x2=400,maps_y2=400,maps_z2=400,item_id=0,item_count1=0,item_count2=0,dicovery_chance=100,recipe_id=-1}
		areamiscitems[3]={maps_id=4,is_unique=0,maps_x1=-400,maps_y1=-400,maps_z1=-400,maps_x2=400,maps_y2=400,maps_z2=400,item_id=0,item_count1=0,item_count2=0,dicovery_chance=100,recipe_id=-1}
		areamiscitems[4]={maps_id=4,is_unique=1,maps_x1=200,maps_y1=-400,maps_z1=200,maps_x2=400,maps_y2=400,maps_z2=400,item_id=0,item_count1=0,item_count2=0,dicovery_chance=100,recipe_id=-1}
	end 
	
	if discover_type==0 and create_type==1 then
		areamiscitems[1]={maps_id=1,is_unique=0,maps_x1=-400,maps_y1=-400,maps_z1=-400,maps_x2=400,maps_y2=400,maps_z2=400,item_id=685,item_count1=1,item_count2=3,dicovery_chance=100,recipe_id=-1}
		areamiscitems[2]={maps_id=1,is_unique=0,maps_x1=-400,maps_y1=-400,maps_z1=-400,maps_x2=400,maps_y2=400,maps_z2=400,item_id=685,item_count1=1,item_count2=3,dicovery_chance=100,recipe_id=-1}
		areamiscitems[3]={maps_id=1,is_unique=0,maps_x1=-400,maps_y1=-400,maps_z1=-400,maps_x2=400,maps_y2=400,maps_z2=400,item_id=685,item_count1=1,item_count2=3,dicovery_chance=100,recipe_id=-1}
		areamiscitems[4]={maps_id=1,is_unique=0,maps_x1=-400,maps_y1=-400,maps_z1=-400,maps_x2=400,maps_y2=400,maps_z2=400,item_id=685,item_count1=1,item_count2=3,dicovery_chance=100,recipe_id=-1}
	end 	
	
	return areamiscitems
end 
---//不同探索类型对应的被动技能id
function ZSkillsOfDiscovery(discover_type)
	local re_skill_1,re_skill_2,re_skill_3,re_skill_4=0,0,0,0
	---//一种探索技能最多可与四个技能相关
	local discoveryskilllist={}
		discoveryskilllist[1]={discover_type=1,skill_id1=218,skill_id2=219,skill_id3=220,skill_id4=221}
		discoveryskilllist[2]={discover_type=2,skill_id1=218,skill_id2=219,skill_id3=220,skill_id4=221}
	for i,v in pairs(discoveryskilllist) do
		if discover_type == v.discover_type then 
			re_skill_1=v.skill_id1
			re_skill_2=v.skill_id2
			re_skill_3=v.skill_id3
			re_skill_4=v.skill_id4
		end
	end
	return re_skill_1,re_skill_2,re_skill_3,re_skill_4
end
---//探寻获得额外产出主函数--------------------------------------------------------------------------
function DiscoveryGain(player_level, player_gender, player_prof,discover_type, player)
	local create_type=0					---discover的时候，create_type默认为0
	local gain_id,item_count,msg_id=0,0,501
	local maps_id,maps_x,maps_y,maps_z=1,100,0,100
	local skill_alvl1,skill_blvl1=0,0
	local skill_alvl2,skill_blvl2=0,0
	local skill_alvl3,skill_blvl3=0,0
	local skill_alvl4,skill_blvl4=0,0
	local isgain=0							---//是否成功的获得了物品
	maps_id,maps_x,maps_y,maps_z=player:PlayerQueryPosition()
	
	
	local withskill_1,withskill_2,withskill_3,withskill_4=0,0,0,0
	withskill_1,withskill_2,withskill_3,withskill_4= ZSkillsOfDiscovery(discover_type)
	skill_alvl1,skill_blvl1=player:PlayerQuerySkilllvl(withskill_1)
	skill_alvl2,skill_blvl2=player:PlayerQuerySkilllvl(withskill_2)
	skill_alvl3,skill_blvl3=player:PlayerQuerySkilllvl(withskill_3)
	skill_alvl4,skill_blvl4=player:PlayerQuerySkilllvl(withskill_4)
	
	local finnaldiscoverychance=10*(skill_alvl1+skill_alvl2+skill_alvl3+skill_alvl4)
	
	if ZMiscThrowDice(finnaldiscoverychance)==1 then 
		---//discover_type,create_type,maps_id,maps_x,maps_y,maps_z,recipe_id
		gain_id,item_count=ZMiscFindInAreas(discover_type,create_type,maps_id,maps_x,maps_y,maps_z,0)
		
		msg_id=502
	
	else
		msg_id=503
	end 	
	return gain_id, item_count, msg_id
end 


---//探寻系统部分结束-------------------------------------------------------------------------------- 

-----//与程序对接的接口
-----
---PetBedgeCatch(monster_id, monster_level, monster_raceinfo, monster_catchdifficulty, monster_hp, player_level);
---ExtGainOfCreate(recipe_id, player_level,player_gender, player_prof);
---DiscoveryGain(player_level, player_gender, player_prof,discover_type);
---player_gender: 0 - 女  1-男
---PlayerQuerySkilllvl(skill_id) 
---PlayerQueryPosition()

---//共用函数部分开始------------------------------------------------------------------------------
---//成功率roll---------------------------------------------------------
function ZMiscThrowDice(chance)
	if type(chance)~="number" or chance<=0 then 
		chance=0
	end 
	if chance>100 then chance=100 end 
	local zrollresult=0
	local zrolldice= math.random()*100   
	if zrolldice<chance and zrolldice~=0 then 
		zrollresult=1
	else 
		zrollresult=0
	end 
	return zrollresult
end 
---//随机一个num1-num2之间的数字----------------------------------
function ZMiscRandomNumber(num1,num2)
	if num2<0 or num1<0 then return 0 end 
	if num1==num2 then return num1 end 
	return math.min(num1,num2)+math.floor(math.random()*math.abs(num1-num2)) 
end 	

---//判断是否在规定区域内----------------------------------
function ZMiscIsInAreas(x0,y0,z0,x1,y1,z1,x2,y2,z2)
	if x0>=x1 and x0<=x2 and y0>=y1 and y0<=y2 and z0>=z1 and z0<=z2 then 
		return 1
	elseif x0<=x1 and x0>=x2 and y0<=y1 and y0>=y2 and z0<=z1 and z0>=z2 then 
		return 1
	else 
		return 0
	end 
end 

---//在划定区域内GetMisc----------------------------------
function ZMiscFindInAreas(discover_type,create_type,maps_id,maps_x,maps_y,maps_z,recipe_id)
	local maxitemslist=32		---//最多一个点有32种掉落
	local existuniquearea=0	---//存在unique掉落区域
	local itemdroplist={}

	for i=1,maxitemslist do
		itemdroplist[i]={is_unique=0,item_id=0,item_count=0,dicovery_chance=0}
	end 
	local extdropitems=ZStaticAreaDrops(discover_type,create_type)
	local nowitemlist=0		---//已有的掉落数量
		
	
	---//收集符合条件的地图掉落信息
	---//discover type 
	if discover_type~=0 and create_type==0 then 
		for i,v in pairs(extdropitems) do
			if (maps_id == v.maps_id or v.maps_id==-1) and ZMiscIsInAreas(maps_x,maps_y,maps_z,v.maps_x1,v.maps_y1,v.maps_z1,v.maps_x2,v.maps_y2,v.maps_z2)==1 
				and nowitemlist<=32 then 
				
				nowitemlist=nowitemlist+1
				itemdroplist[nowitemlist].is_unique=v.is_unique
				if itemdroplist[nowitemlist].is_unique>0 then existuniquearea=1 end 
				itemdroplist[nowitemlist].item_id=v.item_id
				itemdroplist[nowitemlist].item_count=ZMiscRandomNumber(v.item_count1,v.item_count2)
				itemdroplist[nowitemlist].dicovery_chance=math.max(v.dicovery_chance,0)
			else
			end
		end
	---//create type
	elseif discover_type==0 and create_type~=0 then
		
			for i,v in pairs(extdropitems) do
			if (maps_id == v.maps_id or v.maps_id==-1) and ZMiscIsInAreas(maps_x,maps_y,maps_z,v.maps_x1,v.maps_y1,v.maps_z1,v.maps_x2,v.maps_y2,v.maps_z2)==1 
				and nowitemlist<=32  and recipe_id==v.recipe_id then 
				nowitemlist=nowitemlist+1
				itemdroplist[nowitemlist].is_unique=v.is_unique
				if itemdroplist[nowitemlist].is_unique>0 then existuniquearea=1 end 
				itemdroplist[nowitemlist].item_id=v.item_id
				itemdroplist[nowitemlist].item_count=ZMiscRandomNumber(v.item_count1,v.item_count2)
				itemdroplist[nowitemlist].dicovery_chance=math.max(v.dicovery_chance,0)
			else
			end
		end	
	else 
		return 0,0
	end 	 	

	---//处理存在unique_area的情况
	if existuniquearea>0 then 
		for i,v in pairs(itemdroplist) do
			if v.is_unique==0 then 
				v.dicovery_chance=0
			end 
		end
	end 
	
	---//概率叠加并选取一个掉落结果
	local finnalposition=-1				---//最终选取的掉落id的listid
	local sumchances={}				
	for m=0,maxitemslist-1 do
		sumchances[0]=0
		sumchances[m+1]=sumchances[m]+itemdroplist[m+1].dicovery_chance
	end 
	local randomchance=math.max(math.min(math.random()*sumchances[maxitemslist],sumchances[maxitemslist]),1)
	for k=0,maxitemslist-1 do 
			if randomchance>sumchances[k] and randomchance<=sumchances[k+1] then 
				finnalposition=k+1
			end 
	end 
	
	---//返回结果
	if finnalposition<=0 or finnalposition>maxitemslist then
		return 0,0
	else
		return itemdroplist[finnalposition].item_id,itemdroplist[finnalposition].item_count
	end
	
end 
----出现几率正态化--------------------------------------------------------------
function ZMiscNormChance(n1)
	local n1_size=table.getn(n1)
	local Total_Chance=0
	local i,m,k
	---先求和
	for i=1,n1_size do
   		Total_Chance=n1[i]+Total_Chance          
	end
 	---生成概率组序列
	local nx={}
	for k=1,n1_size do
		if k==1 then 
			nx[1]=n1[k]
		else
	   	nx[k]=n1[k]+nx[k-1]              
	  end
	end

	---再生产标准化的概率组序列
	local n2={}
     for m=1,n1_size do
     		if Total_Chance==0 then                 ---对于全0序列的处理，返回{1,0,0...}
			 		n2[m]=0
					n2[1]=1
				else
					n2[m]=nx[m]/ Total_Chance
				end
	 	 end
	---返回一个概率序列 
    ---ZLuaTal_Special_Print(n2)   				---$$$$测试用$$$$-----
	return n2
end
-------------------------------------------------------------------------------- 

----按照出现概率随机应该选取的位置----------------------------------------------
function ZMiscGetRanPoint(s1)
	local js,ron1
	ron1= math.random()                
  local p1=ZMiscNormChance(s1)
	for js=1,table.getn(p1) do
          if ron1<=p1[js] then 
    	  		return js
		  		end
	end
	return 1
end