----------------------------------------------------------
---变身脚本
---
---transform_type 0物品 1技能 2任务
---


function ZLuaAvailableTransforms()
   local transforms
   transforms = {}

   transforms["TEST1"] 						= {template_id = 124,type = "物品",transform_type = 0}
   transforms["TEST2"] 						= {template_id = 127,type = "物品",transform_type = 0}
   transforms["TEST3"] 						= {template_id = 131,type = "物品",transform_type = 0}
   transforms["TEST4"] 						= {template_id = 681,type = "物品",transform_type = 0}
   transforms["TEST5"] 						= {template_id = 682,type = "物品",transform_type = 0}
   transforms["TEST6"] 						= {template_id = 683,type = "物品",transform_type = 0}
   transforms["TEST7"] 						= {template_id = 684,type = "物品",transform_type = 0}

   transforms["wupindaoxuan"] 		= {template_id = 150,type = "物品",transform_type = 0}   ---充值赠送永久熊猫  
   transforms["wupinhuluwa"] 			= {template_id = 425,type = "物品",transform_type = 0}   ---葫芦娃幻灵石  

   transforms["wupinlongshen"] 		= {template_id = 519,type = "物品",transform_type = 0}   ---龙神幻灵石  

   transforms["wupinguoqing01"] 	= {template_id = 151,type = "物品",transform_type = 0}   ---国庆中秋活动01  
   transforms["wupinguoqing02"] 	= {template_id = 152,type = "物品",transform_type = 0}   ---国庆中秋活动02  
   transforms["wupinguoqing03"] 	= {template_id = 153,type = "物品",transform_type = 0}   ---国庆中秋活动03  

   transforms["wupinchongyang"] 	= {template_id = 155,type = "物品",transform_type = 0}   ---重阳活动用菊花精  

   transforms["wupinxueren01"] 		= {template_id = 156,type = "物品",transform_type = 0}   ---雪人活动用雪人01  
   transforms["wupinxueren02"] 		= {template_id = 159,type = "物品",transform_type = 0}   ---雪人活动用雪人02  
   transforms["wupinxueren03"] 		= {template_id = 160,type = "物品",transform_type = 0}   ---雪人活动用雪人03
   transforms["wupinxueren04"] 		= {template_id = 161,type = "物品",transform_type = 0}   ---雪人活动用雪人04

   transforms["wupinxueguoteshi"] = {template_id = 743,type = "物品",transform_type = 0}   ---台历雪国特使

   transforms["wupinsanjie01"]	 	= {template_id = 165,type = "物品",transform_type = 0}   ---三界乱斗BOSS形象01  
   transforms["wupinshennong"]	 	= {template_id = 730,type = "物品",transform_type = 0}   ---有声任务神农幻灵石  
   transforms["wupinyueshen"]	 		= {template_id = 339,type = "物品",transform_type = 0}   ---天界月神变身  
   transforms["wupinshouhuzhe"]		= {template_id = 680,type = "物品",transform_type = 0}   ---异界守护者变身  

   transforms["jinengsanjie01"]	 	= {template_id = 168,type = "技能",transform_type = 1}   ---三界BOSS幻灵石变羊技能  
   transforms["jinengfenxiang01"]	= {template_id = 485,type = "技能",transform_type = 1}   ---焚香谷三重火兽混乱技能  


   transforms["wupinpaisong1"] 		= {template_id = 145,type = "物品",transform_type = 0}   ---限时派送      
   transforms["wupinpaisong2"] 		= {template_id = 148,type = "物品",transform_type = 0}   ---限时派送  
   transforms["wupinpaisong3"] 		= {template_id = 149,type = "物品",transform_type = 0}   ---限时派送  
   transforms["wupinpengzhuang1"] = {template_id = 694,type = "物品",transform_type = 0}   ---碰撞副本奖励  
   transforms["wupinpengzhuang2"]	= {template_id = 695,type = "物品",transform_type = 0}   ---碰撞副本奖励
   transforms["wupinpengzhuang3"]	= {template_id = 696,type = "物品",transform_type = 0}   ---碰撞副本奖励  
   transforms["wupinxiaolangren"]	= {template_id = 702,type = "物品",transform_type = 0}   ---小浪人  
   transforms["wupinguilifei"]		= {template_id = 739,type = "物品",transform_type = 0}   ---幻灵石鬼厉肥版  

   transforms["wupinhuoyan"]			= {template_id = 731,type = "物品",transform_type = 0}   ---良缘火岩  
   transforms["wupinhuowa"]				= {template_id = 732,type = "物品",transform_type = 0}   ---良缘火娃  
   transforms["wupinkuishe"]			= {template_id = 733,type = "物品",transform_type = 0}   ---蝰蛇  

   transforms["wupinhuamei"] 			= {template_id = 133,type = "物品",transform_type = 0}   ---花魅
   transforms["wupinhanfei"] 			= {template_id = 134,type = "物品",transform_type = 0}   ---悍匪
   transforms["wupinlinghou"] 		= {template_id = 135,type = "物品",transform_type = 0}   ---灵猴
   transforms["wupinyingchifeng"] = {template_id = 144,type = "物品",transform_type = 0}   ---硬翅蜂
   transforms["wupinhuayao"] 			= {template_id = 146,type = "物品",transform_type = 0}   ---花妖
   transforms["wupinguidaoshi"] 	= {template_id = 147,type = "物品",transform_type = 0}   ---鬼道士

   transforms["wupinshoushen"] 		= {template_id = 297,type = "物品",transform_type = 0}   ---跨服PK奖励兽神幻灵石
   transforms["wupinqingxin"] 		= {template_id = 338,type = "物品",transform_type = 0}   ---情人节情心幻灵石
   transforms["wupinchiyou"] 			= {template_id = 483,type = "物品",transform_type = 0}   ---2011跨服PK幻灵石蚩尤真身
   transforms["wupintianjienan"] 	= {template_id = 650,type = "物品",transform_type = 0}   ---2012跨服PK幻灵石天界男圣使
   transforms["wupintianjienv"] 	= {template_id = 651,type = "物品",transform_type = 0}   ---2012跨服PK幻灵石天界女圣使
   transforms["wupinvip"] 				= {template_id = 514,type = "物品",transform_type = 0}   ---2012充值钻石会员奖励限时

   transforms["wupinhuigui01"] 			= {template_id = 565,type = "物品",transform_type = 0}   ---2012老玩家回归01
   transforms["wupinhuigui02"] 			= {template_id = 566,type = "物品",transform_type = 0}   ---2012老玩家回归02
   transforms["wupinhuigui03"] 			= {template_id = 567,type = "物品",transform_type = 0}   ---2012老玩家回归03
   transforms["wupinhuigui04"] 			= {template_id = 568,type = "物品",transform_type = 0}   ---2012老玩家回归04
   transforms["wupinhuigui05"] 			= {template_id = 569,type = "物品",transform_type = 0}   ---2012老玩家回归05
   transforms["wupinhuigui06"] 			= {template_id = 570,type = "物品",transform_type = 0}   ---2012老玩家回归06

   transforms["jinenghuigui01"]		 	= {template_id = 571,type = "技能",transform_type = 1}   ---2012老玩家回归11
   transforms["jinenghuigui02"]		 	= {template_id = 572,type = "技能",transform_type = 1}   ---2012老玩家回归12
   transforms["jinenghuigui03"]		 	= {template_id = 573,type = "技能",transform_type = 1}   ---2012老玩家回归13
   transforms["jinenghuigui04"]		 	= {template_id = 574,type = "技能",transform_type = 1}   ---2012老玩家回归14
   transforms["jinenghuigui05"]		 	= {template_id = 575,type = "技能",transform_type = 1}   ---2012老玩家回归15
   transforms["jinenghuigui06"]		 	= {template_id = 576,type = "技能",transform_type = 1}   ---2012老玩家回归16

   transforms["jinengceshi01"] 		= {template_id = 157,type = "技能",transform_type = 1}   ---测试技能变身兔子
   transforms["jinengceshi02"] 		= {template_id = 158,type = "技能",transform_type = 1}   ---测试技能变身兔子替换属性
   transforms["jinengceshi03"] 		= {template_id = 166,type = "技能",transform_type = 1}   ---测试技能变身

   transforms["TEST_DOTA01"] 			= {template_id = 276,type = "技能",transform_type = 1}   ---测试DOTA骷髅弓箭手
   transforms["TEST_DOTA02"] 			= {template_id = 277,type = "技能",transform_type = 1}   ---测试DOTA狼人人形态
   transforms["TEST_DOTA03"] 			= {template_id = 278,type = "技能",transform_type = 1}   ---测试DOTA狼人狼形态
 
   transforms["renwuceshi01"] 		= {template_id = 163,type = "任务",transform_type = 2}   ---测试任务变身
   transforms["renwuceshi02"] 		= {template_id = 164,type = "任务",transform_type = 2}   ---测试任务变身替换属性
   
   transforms["renwugaoqiao"] 		= {template_id = 169,type = "任务",transform_type = 2}   ---任务用：高跷变身
   transforms["renwuzhuahaozi"]		= {template_id = 170,type = "任务",transform_type = 2}   ---任务用：抓耗子
   transforms["jinengbianshitou"]	= {template_id = 172,type = "技能",transform_type = 1}   ---技能用：变石头  
   transforms["renwudatouwawa"]		= {template_id = 173,type = "任务",transform_type = 2}   ---任务用：大头娃娃
   transforms["renwuyutu"] 				= {template_id = 174,type = "任务",transform_type = 2}   ---任务用：玉兔
   transforms["renwujinshizi"] 		= {template_id = 175,type = "任务",transform_type = 2}   ---任务用：金狮子
   transforms["renwudingshenxueren"] = {template_id = 176,type = "任务",transform_type = 2}   ---任务用：定身雪人

   transforms["jinengbaozha"]		 	= {template_id = 177,type = "技能",transform_type = 1}   ---技能用：爆炸效果
   transforms["jinengshuyao"]	 		= {template_id = 190,type = "技能",transform_type = 1}   ---技能用：变成树妖
   transforms["jinengleiji"]	 		= {template_id = 304,type = "技能",transform_type = 1}   ---技能用：天界雷神雷击
   transforms["jinengbiyan"]	 		= {template_id = 305,type = "技能",transform_type = 1}   ---技能用：天界泽神闭眼
   transforms["jinenghunluan"]	 	= {template_id = 314,type = "技能",transform_type = 1}   ---技能用：天界月神混乱

   transforms["jinengzhuxiangushe"]	 	= {template_id = 350,type = "技能",transform_type = 1}   ---技能用：主线怪物用变身骨蛇技能

   transforms["jinengjiazuqingyun"]	 	= {template_id = 410,type = "技能",transform_type = 1}   ---技能用：家族副本青云
   transforms["jinengjiazuguiwang"]	 	= {template_id = 412,type = "技能",transform_type = 1}   ---技能用：家族副本鬼王
   transforms["jinengjiazuhehuan"]	 	= {template_id = 411,type = "技能",transform_type = 1}   ---技能用：家族副本合欢
   transforms["jinengjiazutianyin"]	 	= {template_id = 413,type = "技能",transform_type = 1}   ---技能用：家族副本天音
   transforms["jinengjiazuguidao"]	 	= {template_id = 414,type = "技能",transform_type = 1}   ---技能用：家族副本鬼道
   transforms["jinengjiazujiuli"]	 		= {template_id = 416,type = "技能",transform_type = 1}   ---技能用：家族副本九黎
   transforms["jinengjiazulieshan"]	 	= {template_id = 415,type = "技能",transform_type = 1}   ---技能用：家族副本烈山
   transforms["jinengjiazuhuaiguang"]	= {template_id = 417,type = "技能",transform_type = 1}   ---技能用：家族副本怀光
   transforms["jinengjiazutianhua"]	 	= {template_id = 418,type = "技能",transform_type = 1}   ---技能用：家族副本天华
   transforms["jinengjiazufenxiang"]	= {template_id = 486,type = "技能",transform_type = 1}   ---技能用：家族副本焚香
   transforms["jinengjiazutaihao"]	 	= {template_id = 487,type = "技能",transform_type = 1}   ---技能用：家族副本太昊   
   transforms["jinengjiazuchenhuang"]	= {template_id = 708,type = "技能",transform_type = 1}   ---技能用：家族副本辰皇   
   transforms["jinengjiazuqianji"]		= {template_id = 841,type = "技能",transform_type = 1}   ---技能用：家族副本牵机   
   transforms["jinengjiazuyingzhao"]	= {template_id = 842,type = "技能",transform_type = 1}   ---技能用：家族副本英招   
   
   transforms["jinengjiazubianshen1"]	 	= {template_id = 419,type = "技能",transform_type = 1}   ---技能用：家族副本趣味变身
   transforms["jinengjiazubianshen2"]	 	= {template_id = 420,type = "技能",transform_type = 1}   ---技能用：家族副本趣味变身
   transforms["jinengjiazubianshen3"]	 	= {template_id = 421,type = "技能",transform_type = 1}   ---技能用：家族副本趣味变身
   transforms["jinengjiazubianshen4"]	 	= {template_id = 422,type = "技能",transform_type = 1}   ---技能用：家族副本趣味变身

   transforms["jinengyanwuzhanchang"]	 	= {template_id = 756,type = "技能",transform_type = 1}   ---技能用：天界演武战场变身

   transforms["jinengCHfeisheng01"]	= {template_id = 709,type = "技能",transform_type = 1}   ---技能用：辰皇飞升技能01   
   transforms["jinengCHfeisheng02"]	= {template_id = 710,type = "技能",transform_type = 1}   ---技能用：辰皇飞升技能02   
   transforms["jinengCHfeisheng03"]	= {template_id = 711,type = "技能",transform_type = 1}   ---技能用：辰皇飞升技能03   
   transforms["jinengCHfeisheng04"]	= {template_id = 712,type = "技能",transform_type = 1}   ---技能用：辰皇飞升技能04   
   transforms["jinengCHfeisheng05"]	= {template_id = 713,type = "技能",transform_type = 1}   ---技能用：辰皇飞升技能05   
   transforms["jinengCHfeisheng06"]	= {template_id = 714,type = "技能",transform_type = 1}   ---技能用：辰皇飞升技能06   

   transforms["jinengCHfeisheng11"]	= {template_id = 715,type = "技能",transform_type = 1}   ---技能用：辰皇飞升技能011   
   transforms["jinengCHfeisheng12"]	= {template_id = 716,type = "技能",transform_type = 1}   ---技能用：辰皇飞升技能021   
   transforms["jinengCHfeisheng13"]	= {template_id = 717,type = "技能",transform_type = 1}   ---技能用：辰皇飞升技能031   
   transforms["jinengCHfeisheng14"]	= {template_id = 718,type = "技能",transform_type = 1}   ---技能用：辰皇飞升技能041  
   transforms["jinengCHfeisheng15"]	= {template_id = 719,type = "技能",transform_type = 1}   ---技能用：辰皇飞升技能051   
   transforms["jinengCHfeisheng16"]	= {template_id = 720,type = "技能",transform_type = 1}   ---技能用：辰皇飞升技能061  

   transforms["jinengqixibainiu"]	= {template_id = 769,type = "技能",transform_type = 1}   ---技能用：2013七夕白牛  
   transforms["renwuqixibainiu"]	= {template_id = 772,type = "技能",transform_type = 2}   ---任务用：2013七夕白牛  

   transforms["wupinwufuhu01"]	 	= {template_id = 191,type = "物品",transform_type = 0}   ---金虎
   transforms["wupinwufuhu02"]	 	= {template_id = 192,type = "物品",transform_type = 0}   ---木虎
   transforms["wupinwufuhu03"]	 	= {template_id = 193,type = "物品",transform_type = 0}   ---水虎
   transforms["wupinwufuhu04"]	 	= {template_id = 194,type = "物品",transform_type = 0}   ---火虎
   transforms["wupinwufuhu05"]	 	= {template_id = 195,type = "物品",transform_type = 0}   ---土虎

   transforms["wupinyyxiongx"]	 	= {template_id = 539,type = "物品",transform_type = 0}   ---YY熊
   transforms["wupinyyxiongy"]	 	= {template_id = 540,type = "物品",transform_type = 0}   ---YY熊
   transforms["wupinzhouyixian"]	= {template_id = 541,type = "物品",transform_type = 0}   ---周一仙
   transforms["wupinfeitianxianzi"]	 	= {template_id = 542,type = "物品",transform_type = 0}   ---飞天仙子
   transforms["wupingongyuanyang"]	 	= {template_id = 746,type = "物品",transform_type = 0}   ---公鸳鸯
   transforms["wupinmuyuanyang"]		 	= {template_id = 747,type = "物品",transform_type = 0}   ---母鸳鸯
   transforms["wupinkushuangtianshi"]	= {template_id = 751,type = "物品",transform_type = 0}   ---2013酷爽天师
   transforms["wupinkushuangtianshi2"]	= {template_id = 752,type = "物品",transform_type = 0}   ---2013酷爽天师2
   transforms["wupinzhangyu"]	= {template_id = 869,type = "物品",transform_type = 0}   ---2013章鱼

   transforms["wupincaishen"]	= {template_id = 785,type = "物品",transform_type = 0}   ---财神
   transforms["wupinnantong"]	= {template_id = 786,type = "物品",transform_type = 0}   ---男童
   transforms["wupinnvtong"]	= {template_id = 787,type = "物品",transform_type = 0}   ---女童

   transforms["wupinxiaobai2013"]	= {template_id = 883,type = "物品",transform_type = 0}   ---小白
   transforms["wupinguili2013"]	= {template_id = 884,type = "物品",transform_type = 0}   ---鬼厉
   transforms["wupinxueqi2013"]	= {template_id = 885,type = "物品",transform_type = 0}   ---雪琪
   transforms["wupinbiyao2013"]	= {template_id = 886,type = "物品",transform_type = 0}   ---碧瑶

   transforms["wupin2013dijun"]	= {template_id = 839,type = "物品",transform_type = 0}   ---帝俊

   transforms["renwuchunjietu01"]	= {template_id = 315,type = "任务",transform_type = 0}   ---2011春节兔子A
   transforms["renwuchunjietu02"]	= {template_id = 316,type = "任务",transform_type = 0}   ---2011春节兔子B
   transforms["renwuchunjietu03"]	= {template_id = 317,type = "任务",transform_type = 0}   ---2011春节兔子C
   transforms["renwuchunjietu04"]	= {template_id = 318,type = "任务",transform_type = 0}   ---2011春节兔子D

   transforms["renwuhaiwai01"]	= {template_id = 325,type = "任务",transform_type = 0}   ---2011海外活动用1
   transforms["renwuhaiwai02"]	= {template_id = 326,type = "任务",transform_type = 0}   ---2011海外活动用2
   transforms["renwuhaiwai03"]	= {template_id = 327,type = "任务",transform_type = 0}   ---2011海外活动用3
   transforms["renwuhaiwai04"]	= {template_id = 328,type = "任务",transform_type = 0}   ---2011海外活动用4

   transforms["renwuzhuogui01"]	 	= {template_id = 196,type = "任务",transform_type = 2}   ---火岩魔
   transforms["renwuzhuogui02"]	 	= {template_id = 197,type = "任务",transform_type = 2}   ---老鼠
   transforms["renwuzhuogui03"]	 	= {template_id = 198,type = "任务",transform_type = 2}   ---石头
   transforms["renwuzhuogui04"]	 	= {template_id = 199,type = "任务",transform_type = 2}   ---雪人
   transforms["renwuzhuogui05"]	 	= {template_id = 200,type = "任务",transform_type = 2}   ---树妖

   transforms["jinengtiandibaoku01"]	 	= {template_id = 201,type = "技能",transform_type = 1}   ---骷髅弓箭手
   transforms["jinengtiandibaoku02"]	 	= {template_id = 202,type = "技能",transform_type = 1}   ---骷髅将军

   transforms["renwuyurenjie01"]	 	= {template_id = 227,type = "任务",transform_type = 2}   ---任务愚人节活动01
   transforms["renwuyurenjie02"]	 	= {template_id = 228,type = "任务",transform_type = 2}   ---任务愚人节活动02
   transforms["renwuyurenjie03"]	 	= {template_id = 229,type = "任务",transform_type = 2}   ---任务愚人节活动03
   transforms["renwuyurenjie04"]	 	= {template_id = 230,type = "任务",transform_type = 2}   ---任务愚人节活动04
   transforms["renwuyurenjie05"]	 	= {template_id = 231,type = "任务",transform_type = 2}   ---任务愚人节活动05
   transforms["renwuyurenjie06"]	 	= {template_id = 232,type = "任务",transform_type = 2}   ---任务愚人节活动06
   transforms["renwuyurenjie07"]	 	= {template_id = 233,type = "任务",transform_type = 2}   ---任务愚人节活动07
   transforms["renwuyurenjie08"]	 	= {template_id = 234,type = "任务",transform_type = 2}   ---任务愚人节活动08
   transforms["renwuyurenjie09"]	 	= {template_id = 235,type = "任务",transform_type = 2}   ---任务愚人节活动09
   transforms["renwuyurenjie10"]	 	= {template_id = 236,type = "任务",transform_type = 2}   ---任务愚人节活动10

   transforms["renwuyurenjie010"]	 	= {template_id = 237,type = "任务",transform_type = 2}   ---任务愚人节活动010
   transforms["renwuyurenjie020"]	 	= {template_id = 238,type = "任务",transform_type = 2}   ---任务愚人节活动020
   transforms["renwuyurenjie030"]	 	= {template_id = 239,type = "任务",transform_type = 2}   ---任务愚人节活动030
   transforms["renwuyurenjie040"]	 	= {template_id = 240,type = "任务",transform_type = 2}   ---任务愚人节活动040
   transforms["renwuyurenjie050"]	 	= {template_id = 241,type = "任务",transform_type = 2}   ---任务愚人节活动050
   transforms["renwuyurenjie060"]	 	= {template_id = 242,type = "任务",transform_type = 2}   ---任务愚人节活动060
   transforms["renwuyurenjie070"]	 	= {template_id = 243,type = "任务",transform_type = 2}   ---任务愚人节活动070
   transforms["renwuyurenjie080"]	 	= {template_id = 244,type = "任务",transform_type = 2}   ---任务愚人节活动080
   transforms["renwuyurenjie090"]	 	= {template_id = 245,type = "任务",transform_type = 2}   ---任务愚人节活动090
   transforms["renwuyurenjie100"]	 	= {template_id = 246,type = "任务",transform_type = 2}   ---任务愚人节活动100

   transforms["renwuqingmingegui"]	= {template_id = 247,type = "任务",transform_type = 2}   ---任务清明节恶鬼变身
   transforms["renwujingcha"]	 			= {template_id = 249,type = "任务",transform_type = 2}   ---任务警察变身
   transforms["renwupingmin"]	 			= {template_id = 250,type = "任务",transform_type = 2}   ---任务平民变身

   transforms["renwudiushoujuan01"]	= {template_id = 253,type = "任务",transform_type = 2}   ---任务丢手绢变身-鬼
   transforms["renwudiushoujuan02"]	= {template_id = 254,type = "任务",transform_type = 2}   ---任务丢手绢变身-小朋友
   transforms["renwuzhishu"]	 			= {template_id = 255,type = "任务",transform_type = 2}   ---任务植树变身
   transforms["renwudiushoujuan03"]	= {template_id = 256,type = "任务",transform_type = 2}   ---任务丢手绢变身-小朋友2
   transforms["renwudiushoujuan04"]	= {template_id = 257,type = "任务",transform_type = 2}   ---任务丢手绢变身-老男人
   transforms["renwudiushoujuan05"]	= {template_id = 258,type = "任务",transform_type = 2}   ---任务丢手绢变身-老女人

   transforms["renwuchiyanyaotong"]	= {template_id = 259,type = "任务",transform_type = 2}   ---任务猪妖变身
   transforms["renwucangsong"]			= {template_id = 260,type = "任务",transform_type = 2}   ---任务杀人游戏苍松变身

   transforms["renwuLTdiji"]				= {template_id = 262,type = "任务",transform_type = 2}   ---任务领土战低级攻城车
   transforms["renwuLTzhongji"]			= {template_id = 263,type = "任务",transform_type = 2}   ---任务领土战中级攻城车
   transforms["renwuLTgaoji"]				= {template_id = 264,type = "任务",transform_type = 2}   ---任务领土战高级攻城车
   transforms["renwuLTdiji01"]			= {template_id = 271,type = "任务",transform_type = 2}   ---任务领土战低级额外01
   transforms["renwuLTzhongji01"]		= {template_id = 272,type = "任务",transform_type = 2}   ---任务领土战中级额外01
   transforms["renwuLTzhongji02"]		= {template_id = 273,type = "任务",transform_type = 2}   ---任务领土战中级额外02
   transforms["renwuLTdiji02"]			= {template_id = 274,type = "任务",transform_type = 2}   ---任务领土战低级守方
   transforms["renwuLTzhongji03"]		= {template_id = 275,type = "任务",transform_type = 2}   ---任务领土战中级守方
   transforms["renwuLTgongjianshou"]= {template_id = 283,type = "任务",transform_type = 2}   ---任务领土战弓箭手变身
   transforms["renwuLTgaoji2"]			= {template_id = 279,type = "任务",transform_type = 2}   ---任务领土战高级守方变身
   transforms["renwuLTgaoji01"]			= {template_id = 280,type = "任务",transform_type = 2}   ---任务领土战高级额外01变身
   transforms["renwuLTgaoji02"]			= {template_id = 281,type = "任务",transform_type = 2}   ---任务领土战高级额外02变身
   transforms["renwuLTgaoji03"]			= {template_id = 282,type = "任务",transform_type = 2}   ---任务领土战高级额外03变身

   transforms["renwucaiquanhuodong"]= {template_id = 265,type = "任务",transform_type = 2}   ---任务猜拳活动变身
   transforms["renwuzhadanren"]			= {template_id = 266,type = "任务",transform_type = 2}   ---任务炸弹人活动变身
   transforms["renwuLTbangzhu"]			= {template_id = 267,type = "任务",transform_type = 2}   ---任务领土战帮主变身
   transforms["renwuyayunhui01"]		= {template_id = 288,type = "任务",transform_type = 2}   ---任务亚运会马术
   transforms["renwupaopaokading"]	= {template_id = 289,type = "任务",transform_type = 2}   ---任务跑跑活动
   transforms["renwuquanji"]				= {template_id = 291,type = "任务",transform_type = 2}   ---任务亚运拳击
   transforms["renwusheji"]					= {template_id = 292,type = "任务",transform_type = 2}   ---任务亚运射击

   transforms["renwuzhuanzhi"]	   	= {template_id = 290,type = "任务",transform_type = 2}   ---任务封神转职用
   transforms["renwuleijie"]	 	  	= {template_id = 294,type = "任务",transform_type = 2}   ---任务抗雷劫回血

   transforms["renwuduobao"]	 	  	= {template_id = 333,type = "任务",transform_type = 2}   ---任务夺宝活动
   transforms["renwuhuoyuansu"]	 	 	= {template_id = 482,type = "任务",transform_type = 2}   ---任务2011国庆火元素

   transforms["renwuqiaokeli01"]	 	= {template_id = 334,type = "任务",transform_type = 2}   ---任务白色情人节巧克力男A
   transforms["renwuqiaokeli02"]	 	= {template_id = 335,type = "任务",transform_type = 2}   ---任务白色情人节巧克力男B
   transforms["renwuqiaokeli03"]	 	= {template_id = 336,type = "任务",transform_type = 2}   ---任务白色情人节巧克力女A
   transforms["renwuqiaokeli04"]	 	= {template_id = 337,type = "任务",transform_type = 2}   ---任务白色情人节巧克力女B

   transforms["renwuzhuxianlizu"]	 	= {template_id = 349,type = "任务",transform_type = 2}   ---任务新主线用黎族变身
   transforms["renwuzhuxianguidao"]	= {template_id = 409,type = "任务",transform_type = 2}   ---任务新主线用鬼道士变身
   transforms["renwumiaohuilingshou"]	= {template_id = 429,type = "技能",transform_type = 1}   ---任务/技能庙会驱赶灵兽

   transforms["renwushengdan01"]	 	= {template_id = 506,type = "任务",transform_type = 2}   ---任务海外圣诞节雪人01
   transforms["renwushengdan02"]	 	= {template_id = 507,type = "任务",transform_type = 2}   ---任务海外圣诞节雪人02
   transforms["renwushengdan03"]	 	= {template_id = 508,type = "任务",transform_type = 2}   ---任务海外圣诞节雪人03
   transforms["renwushengdan04"]	 	= {template_id = 509,type = "任务",transform_type = 2}   ---任务海外圣诞节雪人04
   transforms["renwushengdan05"]	 	= {template_id = 510,type = "任务",transform_type = 2}   ---任务海外圣诞节雪人05
   transforms["renwushengdan06"]	 	= {template_id = 511,type = "任务",transform_type = 2}   ---任务海外圣诞节雪人06

   transforms["renwuyuandan2012"]	 	= {template_id = 512,type = "任务",transform_type = 2}   ---任务2012年元旦活动
   transforms["renwuchunjie2012"]	 	= {template_id = 513,type = "任务",transform_type = 2}   ---任务2012年春节活动

   transforms["renwuguaishushu"]	 	= {template_id = 582,type = "任务",transform_type = 2}   ---任务2012年5周年活动用
   transforms["renwuguaishushu2"]	 	= {template_id = 580,type = "任务",transform_type = 2}   ---任务2012年5周年活动用
   transforms["renwuguaishushu3"]	 	= {template_id = 581,type = "任务",transform_type = 2}   ---任务2012年5周年活动用
   transforms["renwuguaishushu4"]	 	= {template_id = 630,type = "任务",transform_type = 2}   ---任务2012年5周年活动用
   transforms["renwuguaishushu5"]	 	= {template_id = 631,type = "任务",transform_type = 2}   ---任务2012年5周年活动用
   transforms["renwuguaishushu6"]	 	= {template_id = 632,type = "任务",transform_type = 2}   ---任务2012年5周年活动用
   transforms["renwuguaishushu7"]	 	= {template_id = 634,type = "任务",transform_type = 2}   ---任务2012年5周年活动用
   transforms["renwuguaishushu8"]	 	= {template_id = 635,type = "任务",transform_type = 2}   ---任务2012年5周年活动用
   transforms["renwu641"]	 					= {template_id = 641,type = "任务",transform_type = 2}   ---
   transforms["renwuxuanyuan"]	 		= {template_id = 686,type = "任务",transform_type = 2}   ---任务轩辕职业任务
   transforms["renwuxiaojingling"]	= {template_id = 687,type = "任务",transform_type = 2}   ---任务9变身
   transforms["renwunanyidu"]				= {template_id = 693,type = "任务",transform_type = 2}   ---任务难易度副本变身

   transforms["renwumaozei"]				= {template_id = 755,type = "任务",transform_type = 2}   ---任务6周年活动打猫贼

   transforms["renwufuben001"]				= {template_id = 806,type = "任务",transform_type = 2}   ---任务河阳大灾变鬼厉
   transforms["renwufuben002"]				= {template_id = 807,type = "任务",transform_type = 2}   ---任务河阳大灾变雪琪
   transforms["renwufuben003"]				= {template_id = 808,type = "任务",transform_type = 2}   ---任务河阳大灾变碧瑶

   transforms["renwufuben101"]				= {template_id = 809,type = "任务",transform_type = 2}   ---任务河阳大灾变青云
   transforms["renwufuben102"]				= {template_id = 810,type = "任务",transform_type = 2}   ---任务河阳大灾变鬼王
   transforms["renwufuben103"]				= {template_id = 811,type = "任务",transform_type = 2}   ---任务河阳大灾变合欢
   transforms["renwufuben104"]				= {template_id = 812,type = "任务",transform_type = 2}   ---任务河阳大灾变天音
   transforms["renwufuben105"]				= {template_id = 813,type = "任务",transform_type = 2}   ---任务河阳大灾变鬼道
   transforms["renwufuben106"]				= {template_id = 814,type = "任务",transform_type = 2}   ---任务河阳大灾变焚香
   transforms["renwufuben107"]				= {template_id = 815,type = "任务",transform_type = 2}   ---任务河阳大灾变九黎
   transforms["renwufuben108"]				= {template_id = 816,type = "任务",transform_type = 2}   ---任务河阳大灾变烈山
   transforms["renwufuben109"]				= {template_id = 817,type = "任务",transform_type = 2}   ---任务河阳大灾变怀光
   transforms["renwufuben110"]				= {template_id = 818,type = "任务",transform_type = 2}   ---任务河阳大灾变天华
   transforms["renwufuben111"]				= {template_id = 819,type = "任务",transform_type = 2}   ---任务河阳大灾变太昊
   transforms["renwufuben112"]				= {template_id = 820,type = "任务",transform_type = 2}   ---任务河阳大灾变辰皇
   transforms["renwufuben113"]				= {template_id = 822,type = "任务",transform_type = 2}   ---任务河阳大灾变牵机
   transforms["renwufuben114"]				= {template_id = 823,type = "任务",transform_type = 2}   ---任务河阳大灾变英招

   transforms["renwu091701"]				= {template_id = 890,type = "任务",transform_type = 2}   ---
   transforms["renwu091702"]				= {template_id = 891,type = "任务",transform_type = 2}   ---
   transforms["renwu091703"]				= {template_id = 892,type = "任务",transform_type = 2}   ---
   transforms["renwu091704"]				= {template_id = 893,type = "任务",transform_type = 2}   ---

   return transforms   
end


function ZLuaTransExistInCtmplIDs(template_id)
	if type(template_id) ~= "number" then return 0 end
	local availableTransforms = ZLuaAvailableTransforms()
	local tansforms
	for _,tansforms in pairs(availableTransforms) do
		if tansforms.template_id == template_id then return 1 end
	end
	return 0
end


function ZLuaSkillsConfig(template_id)

  if ZLuaTransExistInCtmplIDs(template_id) == 0 then return 0 end

  local transforms = ZLuaAvailableTransforms()

  local addskilllearnlevel
  local addskilllearnexplevel
  local addskillid
  local addskilllevel


  if template_id == transforms["TEST1"].template_id then 
   addskilllearnlevel                  = { 1,1,1,1,1,1,1,1,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,1,1,1,1,0,0,0,0 } 
   addskillid                          = { 4020,4028,4029,4014,4016,4032,4034,4039,0,0,0,0 }	---测试技能
   addskilllevel                       = { 1,1,1,1,1,1,1,1,0,0,0,0 }
   
  elseif template_id == transforms["TEST2"].template_id then
   addskilllearnlevel                  = { 1,1,1,1,1,1,1,1,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,1,1,1,1,0,0,0,0 } 
   addskillid                          = { 4012,4013,4030,4031,4037,3998,4010,4011,0,0,0,0 }	---测试技能
   addskilllevel                       = { 1,1,1,1,1,1,1,1,0,0,0,0 }

  elseif template_id == transforms["TEST3"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 4535,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 }    
  elseif template_id == transforms["TEST4"].template_id then
   addskilllearnlevel                  = { 1,1,1,1,1,1,1,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,1,1,1,0,0,0,0,0 } 
   addskillid                          = { 4009,4012,4013,4014,4015,4016,4017,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,1,1,1,0,0,0,0,0 }
  elseif template_id == transforms["TEST5"].template_id then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 4018,4020,4023,4024,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 }
  elseif template_id == transforms["TEST6"].template_id then
   addskilllearnlevel                  = { 1,1,1,1,1,1,1,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,1,1,1,0,0,0,0,0 } 
   addskillid                          = { 4027,4030,4031,4032,4033,4034,4035,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,1,1,1,0,0,0,0,0 }
  elseif template_id == transforms["TEST7"].template_id then
   addskilllearnlevel                  = { 1,1,1,1,1,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,1,0,0,0,0,0,0,0 } 
   addskillid                          = { 4036,4037,4038,4039,4041,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,1,0,0,0,0,0,0,0 }

	---测试用DOTA骷髅弓箭手
  elseif template_id == transforms["TEST_DOTA01"].template_id then
   addskilllearnlevel                  = { 1,1,1,1,1,1,1,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,1,1,1,0,0,0,0,0 } 
   addskillid                          = { 3754,2977,2934,2935,2936,2964,2988,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,1,1,1,0,0,0,0,0 }    

  elseif template_id == transforms["TEST_DOTA02"].template_id then
   addskilllearnlevel                  = { 1,1,1,1,1,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,1,0,0,0,0,0,0,0 } 
   addskillid                          = { 3224,3232,3233,3234,2940,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,1,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["TEST_DOTA03"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2763,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 }    

  ---充值奖励熊猫
  elseif template_id == transforms["wupindaoxuan"].template_id 
  or template_id == transforms["wupinlongshen"].template_id 
  or template_id == transforms["wupinhuluwa"].template_id then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 }    

  ---国庆中秋活动奖励三种
  elseif template_id == transforms["wupinguoqing01"].template_id then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["wupinguoqing02"].template_id then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["wupinguoqing03"].template_id then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 }    

  ---重阳活动用限时
  elseif template_id == transforms["wupinchongyang"].template_id then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 }    
	
	---有声任务神农幻灵石
  elseif template_id == transforms["wupinshennong"].template_id then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 }    

  ---雪人活动用限时
  elseif template_id == transforms["wupinxueren01"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2362,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["wupinxueren02"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2362,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 2,0,0,0,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["wupinxueren03"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2376,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["wupinxueren04"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2376,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 2,0,0,0,0,0,0,0,0,0,0,0 }    

  ---三界乱斗活动BOSS形象
  elseif template_id == transforms["wupinsanjie01"].template_id 
  or template_id == transforms["wupinshouhuzhe"].template_id 
  then
   addskilllearnlevel                  = { 16,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 5,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2380,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 }    

  ---三界幻化形象变羊技能
  elseif template_id == transforms["jinengsanjie01"].template_id or 
  			 template_id == transforms["jinengleiji"].template_id or
  			 template_id == transforms["jinengzhuxiangushe"].template_id or
  			 template_id == transforms["jinengfenxiang01"].template_id or
  			 template_id == transforms["wupinxueguoteshi"].template_id or
  			 template_id == transforms["jinenghunluan"].template_id then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 }    

	---天界月神变身
  elseif template_id == transforms["wupinyueshen"].template_id then
   addskilllearnlevel                  = { 16,16,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 5,5,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3249,3250,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,0,0,0,0,0,0,0,0,0,0 }    

	---天界泽神防止黑暗攻击
  elseif template_id == transforms["jinengbiyan"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3205,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 }    

	---家族副本变身
  elseif template_id == transforms["jinengjiazuqingyun"].template_id then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3292,3293,3294,3295,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["jinengjiazuguiwang"].template_id then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3296,3297,3298,3299,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["jinengjiazuhehuan"].template_id then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3300,3301,3302,3303,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["jinengjiazutianyin"].template_id then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3304,3305,3306,3307,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["jinengjiazuguidao"].template_id then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3308,3309,3310,3311,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["jinengjiazujiuli"].template_id then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3312,3313,3314,3315,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["jinengjiazulieshan"].template_id then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3316,3317,3318,3319,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["jinengjiazuhuaiguang"].template_id then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3320,3321,3322,3323,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["jinengjiazutianhua"].template_id then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3324,3325,3326,3327,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["jinengjiazufenxiang"].template_id then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3731,3732,3733,3734,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["jinengjiazutaihao"].template_id then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3723,3724,3725,3730,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["jinengjiazuchenhuang"].template_id then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 4532,4533,4534,4535,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["jinengjiazuqianji"].template_id then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 5107,5108,5109,5110,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["jinengjiazuyingzhao"].template_id then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 5111,5112,5113,5114,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 }    

  ---派送用限时，只用了第一个
  elseif template_id == transforms["wupinpaisong1"].template_id then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["wupinpaisong2"].template_id then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["wupinpaisong3"].template_id then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["wupinxiaolangren"].template_id 
  or template_id == transforms["wupinhuoyan"].template_id 
  or template_id == transforms["wupinhuowa"].template_id 
  or template_id == transforms["wupinkuishe"].template_id 
  or template_id == transforms["wupinguilifei"].template_id 
  then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 }    

  ---商店卖6种
  elseif template_id == transforms["wupinhuamei"].template_id then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["wupinhanfei"].template_id then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["wupinlinghou"].template_id then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["wupinyingchifeng"].template_id then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["wupinhuayao"].template_id then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 }    

  elseif template_id == transforms["wupinguidaoshi"].template_id then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 } 

  ---测试用技能变身01
  elseif template_id == transforms["jinengceshi01"].template_id then
   addskilllearnlevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 1746,1747,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,0,0,0,0,0,0,0,0,0,0 } 

  ---测试用技能变身02
  elseif template_id == transforms["jinengceshi02"].template_id then
   addskilllearnlevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 1924,2763,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,0,0,0,0,0,0,0,0,0,0 } 

  ---测试用技能变身03
  elseif template_id == transforms["jinengceshi03"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 1924,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

  ---测试用任务变身01
  elseif template_id == transforms["renwuceshi01"].template_id then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 } 

  ---测试用任务变身02
  elseif template_id == transforms["renwuceshi02"].template_id then
   addskilllearnlevel                  = { 1,1,1,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 1924,2579,2763,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,0,0,0,0,0,0,0,0,0 } 

  ---任务用：高跷变身
  elseif template_id == transforms["renwugaoqiao"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2381,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

  ---任务用：抓耗子
  elseif template_id == transforms["renwuzhuahaozi"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2384,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

  ---技能用：变石头
  elseif template_id == transforms["jinengbianshitou"].template_id 
  		or template_id == transforms["renwudatouwawa"].template_id 
  		or template_id == transforms["renwuyutu"].template_id
  		or template_id == transforms["renwujinshizi"].template_id
  		or template_id == transforms["renwudingshenxueren"].template_id
  		or template_id == transforms["jinengbaozha"].template_id
  		or template_id == transforms["jinengshuyao"].template_id
  		or template_id == transforms["wupinpengzhuang1"].template_id
  		or template_id == transforms["wupinpengzhuang2"].template_id
  		or template_id == transforms["wupinpengzhuang3"].template_id
  then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 } 

  ---春节五福虎幻灵石
  elseif template_id == transforms["wupinwufuhu01"].template_id 
  		or template_id == transforms["wupinwufuhu02"].template_id
  		or template_id == transforms["wupinwufuhu03"].template_id
  		or template_id == transforms["wupinwufuhu04"].template_id
  		or template_id == transforms["wupinwufuhu05"].template_id
  		or template_id == transforms["wupinyyxiongx"].template_id
  		or template_id == transforms["wupinyyxiongy"].template_id
  		or template_id == transforms["wupinzhouyixian"].template_id
  		or template_id == transforms["wupinfeitianxianzi"].template_id
  		or template_id == transforms["wupingongyuanyang"].template_id
  		or template_id == transforms["wupinmuyuanyang"].template_id
  		or template_id == transforms["wupinkushuangtianshi"].template_id
  		or template_id == transforms["wupinkushuangtianshi2"].template_id
  		or template_id == transforms["wupinzhangyu"].template_id
  		or template_id == transforms["wupincaishen"].template_id
  		or template_id == transforms["wupinnantong"].template_id
  		or template_id == transforms["wupinnvtong"].template_id
  		or template_id == transforms["wupinxiaobai2013"].template_id
  		or template_id == transforms["wupinguili2013"].template_id
  		or template_id == transforms["wupinxueqi2013"].template_id
  		or template_id == transforms["wupinbiyao2013"].template_id
  then 
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 } 

  ---2011春节兔子变身
  elseif template_id == transforms["renwuchunjietu01"].template_id 
  		or template_id == transforms["renwuchunjietu02"].template_id
  		or template_id == transforms["renwuchunjietu03"].template_id
  		or template_id == transforms["renwuchunjietu04"].template_id
  		or template_id == transforms["renwuhaiwai01"].template_id
  		or template_id == transforms["renwuhaiwai02"].template_id
  		or template_id == transforms["renwuhaiwai03"].template_id
  		or template_id == transforms["renwuhaiwai04"].template_id
  then 
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2763,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

  ---任务用：捉鬼变身
  elseif template_id == transforms["renwuzhuogui01"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2577,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

  elseif template_id == transforms["renwuzhuogui02"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2578,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

  elseif template_id == transforms["renwuzhuogui03"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2579,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

  elseif template_id == transforms["renwuzhuogui04"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2580,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

  elseif template_id == transforms["renwuzhuogui05"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2581,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

  ---技能用：天地宝库变身
  elseif template_id == transforms["jinengtiandibaoku01"].template_id then
   addskilllearnlevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2607,2621,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
  
  elseif template_id == transforms["jinengtiandibaoku02"].template_id then
   addskilllearnlevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2601,2602,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,0,0,0,0,0,0,0,0,0,0 } 

	---任务愚人节活动1-10号变身
	elseif template_id == transforms["renwuyurenjie01"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2721,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuyurenjie02"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2721,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 2,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuyurenjie03"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2721,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 3,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuyurenjie04"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2721,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 4,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuyurenjie05"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2721,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 5,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuyurenjie06"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2721,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 6,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuyurenjie07"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2721,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 7,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuyurenjie08"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2721,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 8,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuyurenjie09"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2721,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 9,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuyurenjie10"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2721,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 10,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuyurenjie010"].template_id
			or template_id == transforms["renwuyurenjie020"].template_id 
			or template_id == transforms["renwuyurenjie030"].template_id
			or template_id == transforms["renwuyurenjie040"].template_id
			or template_id == transforms["renwuyurenjie050"].template_id
			or template_id == transforms["renwuyurenjie060"].template_id
			or template_id == transforms["renwuyurenjie070"].template_id
			or template_id == transforms["renwuyurenjie080"].template_id
			or template_id == transforms["renwuyurenjie090"].template_id
			or template_id == transforms["renwuyurenjie100"].template_id	then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuqingmingegui"].template_id then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 } 

	---杀人游戏变身
	elseif template_id == transforms["renwujingcha"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2743,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwupingmin"].template_id then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 } 

	---丢手绢游戏变身
	elseif template_id == transforms["renwudiushoujuan01"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2749,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwudiushoujuan02"].template_id or
				 template_id == transforms["renwudiushoujuan03"].template_id or
				 template_id == transforms["renwudiushoujuan04"].template_id or
				 template_id == transforms["renwudiushoujuan05"].template_id or
				 template_id == transforms["renwuqiaokeli01"].template_id or
				 template_id == transforms["renwuqiaokeli02"].template_id or
				 template_id == transforms["renwuqiaokeli03"].template_id or
				 template_id == transforms["renwuzhuxianlizu"].template_id or
				 template_id == transforms["renwuzhuxianguidao"].template_id or
				 template_id == transforms["renwuhuoyuansu"].template_id or
				 template_id == transforms["renwuqiaokeli04"].template_id or
				 template_id == transforms["renwushengdan01"].template_id or
				 template_id == transforms["renwushengdan02"].template_id or
				 template_id == transforms["renwushengdan03"].template_id or
				 template_id == transforms["renwushengdan04"].template_id or
				 template_id == transforms["renwushengdan05"].template_id or
				 template_id == transforms["renwushengdan06"].template_id or
				 template_id == transforms["renwu641"].template_id then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuchiyanyaotong"].template_id then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuyuandan2012"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3870,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuchunjie2012"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3873,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuguaishushu"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3901,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuguaishushu4"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3889,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuguaishushu5"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3890,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuguaishushu2"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3885,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuguaishushu3"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3886,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuguaishushu8"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3886,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuguaishushu7"].template_id 
	then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuguaishushu6"].template_id 
	then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3888,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["jinengyanwuzhanchang"].template_id 
	then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 4761,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["jinengqixibainiu"].template_id 
	then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 4768,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuqixibainiu"].template_id 
	then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwucangsong"].template_id or 
				 template_id == transforms["jinengjiazubianshen1"].template_id or
				 template_id == transforms["jinengjiazubianshen2"].template_id or
				 template_id == transforms["jinengjiazubianshen3"].template_id or
				 template_id == transforms["jinengjiazubianshen4"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2763,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

	--辰皇飞升技能01
	elseif template_id == transforms["jinengCHfeisheng01"].template_id 
	or template_id == transforms["jinengCHfeisheng11"].template_id
	then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 4636,4637,4638,4654,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["jinengCHfeisheng02"].template_id
	or template_id == transforms["jinengCHfeisheng12"].template_id
 	then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 4639,4640,4641,4654,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["jinengCHfeisheng03"].template_id 
	or template_id == transforms["jinengCHfeisheng13"].template_id
	then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 4642,4643,4644,4654,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["jinengCHfeisheng04"].template_id 
	or template_id == transforms["jinengCHfeisheng14"].template_id
	then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 4645,4646,4647,4654,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["jinengCHfeisheng05"].template_id 
	or template_id == transforms["jinengCHfeisheng15"].template_id
	then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 4648,4649,4650,4654,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["jinengCHfeisheng06"].template_id 
	or template_id == transforms["jinengCHfeisheng16"].template_id
	then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 4651,4652,4653,4654,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 

  ---庙会活动驱赶灵兽
	elseif template_id == transforms["renwumiaohuilingshou"].template_id then
   addskilllearnlevel                  = { 3,2,1,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3340,3340,3340,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 3,2,1,0,0,0,0,0,0,0,0,0 } 

  ---领土战攻城车变身
	elseif template_id == transforms["renwuLTdiji"].template_id then
   addskilllearnlevel                  = { 1,1,1,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2922,2923,2930,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuLTzhongji"].template_id then
   addskilllearnlevel                  = { 1,1,1,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2924,2925,2931,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuLTgaoji"].template_id 
	or template_id == transforms["renwuLTgaoji2"].template_id then
   addskilllearnlevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2962,2963,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuLTgaoji03"].template_id then
   addskilllearnlevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2965,2966,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuLTdiji01"].template_id 
	or template_id == transforms["renwuLTgaoji01"].template_id then
   addskilllearnlevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2926,2927,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuLTzhongji01"].template_id then
   addskilllearnlevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2926,2927,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuLTzhongji02"].template_id 
	or template_id == transforms["renwuLTgaoji02"].template_id then
   addskilllearnlevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2928,2929,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuLTdiji02"].template_id then
   addskilllearnlevel                  = { 1,1,1,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2922,2923,2930,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuLTzhongji03"].template_id then
   addskilllearnlevel                  = { 1,1,1,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2924,2925,2931,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuLTgongjianshou"].template_id then
   addskilllearnlevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2968,2969,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,0,0,0,0,0,0,0,0,0,0 } 

	---猜拳
	elseif template_id == transforms["renwucaiquanhuodong"].template_id then
   addskilllearnlevel                  = { 1,1,1,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2799,2800,2801,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,0,0,0,0,0,0,0,0,0 } 

	---炸弹人活动
	elseif template_id == transforms["renwuzhadanren"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 2817,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

	---LT御驾亲征
	elseif template_id == transforms["renwuLTbangzhu"].template_id then
		local i,j
		i = math.random()
		if i >= 0 and i <= 0.30 then j = 2891
		elseif i > 0.30 and i <= 0.45 then j = 2892
		elseif i > 0.45 and i <= 0.55 then j = 2893
		elseif i > 0.55 and i <= 0.65 then j = 2894
		elseif i > 0.65 and i <= 0.80 then j = 2895
		elseif i > 0.80 and i <= 0.90 then j = 2896
		elseif i > 0.90 and i <= 1.00 then j = 2897	 end
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { j,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

	---亚运会马术
	elseif template_id == transforms["renwuyayunhui01"].template_id then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 } 

	---亚运会拳击射击
	elseif template_id == transforms["renwuquanji"].template_id then
   addskilllearnlevel                  = { 1,1,1,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3159,3160,3161,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwusheji"].template_id then
   addskilllearnlevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3162,3163,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,0,0,0,0,0,0,0,0,0,0 } 

	---跑跑卡丁车
	elseif template_id == transforms["renwupaopaokading"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 327,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 500,0,0,0,0,0,0,0,0,0,0,0 } 

	---封神转职用
	elseif template_id == transforms["renwuzhuanzhi"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3158,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

	---封神抗雷劫变身
	elseif template_id == transforms["renwuleijie"].template_id then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3164,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 3,0,0,0,0,0,0,0,0,0,0,0 } 

	---跨服PK奖励兽神幻灵石
	elseif template_id == transforms["wupinshoushen"].template_id
	or template_id == transforms["wupinqingxin"].template_id
	or template_id == transforms["wupinvip"].template_id
	or template_id == transforms["wupinchiyou"].template_id
	or template_id == transforms["wupintianjienan"].template_id
	or template_id == transforms["wupintianjienv"].template_id
	or template_id == transforms["renwuxiaojingling"].template_id
	then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 } 

	---2013任务6周年活动
	elseif template_id == transforms["renwumaozei"].template_id	then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 4759,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

	---20130917任务变身1
	elseif template_id == transforms["renwu091701"].template_id	then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 5234,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

	---20130917任务变身2
	elseif template_id == transforms["renwu091702"].template_id	then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 5235,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

	---20130917任务变身3
	elseif template_id == transforms["renwu091703"].template_id	then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 5236,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

	---20130917任务变身4
	elseif template_id == transforms["renwu091704"].template_id	then
   addskilllearnlevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 5237,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,0,0,0,0,0,0,0,0,0,0,0 } 

	---2012难易度副本嘲讽
	elseif template_id == transforms["renwunanyidu"].template_id	then
   addskilllearnlevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 4404,2763,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,0,0,0,0,0,0,0,0,0,0 } 

	---2012老玩家回归
	elseif template_id == transforms["wupinhuigui01"].template_id	then
   addskilllearnlevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3893,3900,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["wupinhuigui02"].template_id	then
   addskilllearnlevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3894,3900,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["wupinhuigui03"].template_id	then
   addskilllearnlevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3895,3900,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["wupinhuigui04"].template_id	then
   addskilllearnlevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3896,3900,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["wupinhuigui05"].template_id	then
   addskilllearnlevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3897,3900,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["wupinhuigui06"].template_id	then
   addskilllearnlevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3898,3900,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,0,0,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["jinenghuigui01"].template_id	or
	template_id == transforms["jinenghuigui02"].template_id	or
	template_id == transforms["jinenghuigui03"].template_id	or
	template_id == transforms["jinenghuigui04"].template_id	or
	template_id == transforms["jinenghuigui05"].template_id	or
	template_id == transforms["jinenghuigui06"].template_id	then
   addskilllearnlevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3899,3900,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,0,0,0,0,0,0,0,0,0,0 } 

	---2011任务用夺宝活动
	elseif template_id == transforms["renwuduobao"].template_id then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3231,3232,3233,3234,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 

	elseif template_id == transforms["renwuxuanyuan"].template_id	then
   addskilllearnlevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 4113,4114,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,0,0,0,0,0,0,0,0,0,0 } 

	---河阳城灾变副本变身技能
	---鬼厉
	elseif template_id == transforms["renwufuben001"].template_id	then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 5039,5040,5041,5042,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 

	---碧瑶
	elseif template_id == transforms["renwufuben002"].template_id	then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 5043,5044,5045,5046,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 

	---雪琪
	elseif template_id == transforms["renwufuben003"].template_id	then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 5047,5048,5049,5050,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 

	---青云
	elseif template_id == transforms["renwufuben101"].template_id	then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 5051,5052,5053,5054,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 

	---鬼王
	elseif template_id == transforms["renwufuben102"].template_id	then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 5055,5056,5057,5058,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 

	---合欢
	elseif template_id == transforms["renwufuben103"].template_id	then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 5059,5060,5061,5062,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
	
	---天音
	elseif template_id == transforms["renwufuben104"].template_id	then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 5063,5064,5065,5066,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 

	---鬼道
	elseif template_id == transforms["renwufuben105"].template_id	then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 5067,5068,5069,5070,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 

	---焚香
	elseif template_id == transforms["renwufuben106"].template_id	then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 5071,5072,5073,5074,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 

	---九黎
	elseif template_id == transforms["renwufuben107"].template_id	then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 5075,5076,5077,5078,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 

	---烈山
	elseif template_id == transforms["renwufuben108"].template_id	then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 5079,5080,5081,5082,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 

	---怀光
	elseif template_id == transforms["renwufuben109"].template_id	then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 5083,5084,5085,3320,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 

	---天华
	elseif template_id == transforms["renwufuben110"].template_id	then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 5087,5088,5089,5090,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 

	---太昊
	elseif template_id == transforms["renwufuben111"].template_id	then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 3723,5092,5093,3725,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 

	---辰皇
	elseif template_id == transforms["renwufuben112"].template_id	then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 5095,5096,5097,5098,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 

	---牵机
	elseif template_id == transforms["renwufuben113"].template_id	then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 5099,5100,5109,5102,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 

	---英招
	elseif template_id == transforms["renwufuben114"].template_id	then
   addskilllearnlevel                  = { 1,1,1,1,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 1,1,1,1,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 5103,5104,5105,5106,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 1,1,1,1,0,0,0,0,0,0,0,0 } 

	---6v6dijun
	elseif template_id == transforms["wupin2013dijun"].template_id	then
   addskilllearnlevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllearnexplevel               = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addskillid                          = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addskilllevel                       = { 0,0,0,0,0,0,0,0,0,0,0,0 } 

  else return 0 
  end
  
  return addskilllearnlevel,addskilllearnexplevel,addskillid,addskilllevel  
  
end


function InitTransformSkill(template_id, transform_type, level, exp_level, player)
  if ZLuaTransExistInCtmplIDs(template_id) == 0 then return 0 end
  if level == nil or level <= 0 or level > 20 or exp_level == nil or exp_level <= 0 or exp_level > 5 then return 0 end
  if transform_type ~= 0 and transform_type ~= 1 and transform_type ~= 2 then return 0 end
    
  local addskillLearnLevel = {}
  local addskillLearnExpLevel = {}
  local addskillID = {}
  local addskillLevel = {}

  local magicAddskillLearnLevel, magicAddskillLearnExpLevel,magicAddskillID,magicAddskillLevel = ZLuaSkillsConfig(template_id)  

  for l=1,12 do
  addskillLearnLevel[l] = magicAddskillLearnLevel[l]
  addskillLearnExpLevel[l] = magicAddskillLearnExpLevel[l]
  addskillID[l] = magicAddskillID[l]
  addskillLevel[l] = magicAddskillLevel[l]
  end
	
  for i=1,12 do
	if addskillLearnLevel[i] ~= nil and addskillLearnLevel[i] > 0  and addskillLearnExpLevel[i] ~=nil and addskillLearnExpLevel[i] >0 and addskillID[i] ~= nil and addskillLevel[i] ~= 0 and addskillLevel[i] > 0 then
		local unknown = true
		for j=1,level do
		  if addskillLearnLevel[i] == j then
			for k=1,exp_level do
 			   if addskillLearnExpLevel[i] == k then
 				  player:AddTransformSkill( addskillID[i],addskillLevel[i] )
				  unknown = false
			   end
			end
		  end
		end
	end
  end
  
  return 1  
end


---属性改变模块
function InitNormalTransformProp(template_id, transform_type, level, exp_level, player)
  if ZLuaTransExistInCtmplIDs(template_id) == 0 then return 0 end
  if level == nil or level <= 0 or level > 20 or exp_level == nil or exp_level <= 0 or exp_level > 5 then return 0 end
  if transform_type ~= 0 and transform_type ~= 1 and transform_type ~= 2 then return 0 end

  local transforms = ZLuaAvailableTransforms()

  local curHP = player:GetHP()
  local curMP = player:GetMP()
  local curDamageHigh,curDamageLow = player:GetDamage()
  local curDefense = player:GetDefense()
  local curAttack = player:GetAttack()
  local curArmor = player:GetArmor()
  local curCritRate,curCritDamage = player:GetCrit()
  local curSkillAttackRate,curSkillArmorRate = player:GetSkillRate()
  local curAntiCritRate,curAntiCritDamage = player:GetAntiCrit()
  local curResistance0,curResistance1,curResistance2,curResistance3,curResistance4,curResistance5 = player:GetResistance()
  local curSpeed = player:GetSpeed()
  local curAttackRange = player:GetAttackRange()


	---transform_type由服务器定义，幻灵石--0；技能--1；任务--2  
  if transform_type == 0 then return 0
  end

  if transform_type == 1 then
  	if template_id == transforms["jinengceshi01"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP(0.5*curHP)	
  		else return 0 end	

  	elseif template_id == transforms["jinengsanjie01"].template_id then
  		if level == 1 and exp_level == 1 then
  		else return 0 end

  	elseif template_id == transforms["jinengtiandibaoku01"].template_id then
  		if level == 1 and exp_level == 1 then
  		  player:AddAttackRange(-curAttackRange+20)
  		  player:AddDamage(5000)
  		else return 0 end

  	elseif template_id == transforms["jinengtiandibaoku02"].template_id then
  		if level == 1 and exp_level == 1 then
  		  player:AddDefense(5000)
  		else return 0 end

  	elseif template_id == transforms["renwuceshi02"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( -curHP + 99999)
  			player:AddMP( -curMP + 99999 )
  			player:AddSpeed(4.5)
  			player:AddCrit( -curCritRate + 777 , -curCritDamage +7.77)
  	  	player:AddDamage( -curDamageHigh + 99999)
  			player:AddAttack( -curAttack + 9999)
  			player:AddDefense( -curDefense + 9999)
  			player:AddArmor( -curArmor + 9999)
  			player:AddResistance( -curResistance0 + 777 , -curResistance1 + 777 , -curResistance2 + 777,- curResistance3 + 777,- curResistance4 + 777,- curResistance5 + 777)
  			player:AddAntiCrit( -curAntiCritRate + 777 , -curAntiCritDamage + 7.77)
  			player:AddSkillRate(-curSkillAttackRate + 999 , -curSkillArmorRate + 999)
  		  player:AddAttackRange(-curAttackRange+20)
  		else return 0  	end

  	elseif template_id == transforms["renwudiushoujuan01"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddSpeed( -curSpeed + 2 )
  		else return 0  end	

  	elseif template_id == transforms["renwudiushoujuan02"].template_id 
  			or template_id == transforms["renwudiushoujuan03"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddSpeed( -curSpeed + 3 )
  		else return 0  end	

		---御驾亲征测试
  	elseif template_id == transforms["renwuLTbangzhu"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( curHP*0.25 )
  		else return 0  end	

  	elseif template_id == transforms["TEST_DOTA01"].template_id then
  		if level == 1 and exp_level == 1 then
  		  player:AddAttackRange(-curAttackRange+20)
   			player:AddHP( curHP*0.25 )
  		else return 0  end	

  	elseif template_id == transforms["TEST_DOTA02"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( 30000 )
  			player:AddDefense( 2000 )
  			player:AddSpeed( -curSpeed + 2 )
  		  player:AddAttackRange(-curAttackRange+2)
  		else return 0  end	

  	elseif template_id == transforms["jinengleiji"].template_id then
  		if level == 1 and exp_level == 1 then
  		  player:AddSpeed(-curSpeed+4)
  		else return 0  end	

		---家族副本职业变身属性
  	elseif template_id == transforms["jinengjiazuqingyun"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( 125125 )
  			player:AddMP( 116326 )
  			player:AddCrit( 151 , 0.55 )
  	  	player:AddDamage( 6296 )
  			player:AddAttack( 723 )
  			player:AddDefense( 3243 )
  			player:AddArmor( 540 )
  			player:AddResistance( 394 , 418 , 405, 501, 269, 0)
  			player:AddAntiCrit( 471 , 4.19)
  			player:AddSkillRate( 158 , 59)
  		  player:AddSpeed(2)
  		else return 0  end	

  	elseif template_id == transforms["jinengjiazuguiwang"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( 148580 )
  			player:AddMP( 133964 )
  			player:AddCrit( 181 , 0.56 )
  	  	player:AddDamage( 6230 )
  			player:AddAttack( 723)
  			player:AddDefense( 3596)
  			player:AddArmor( 540)
  			player:AddResistance( 276 , 380 , 291, 276, 532,0)
  			player:AddAntiCrit( 491 , 3.98)
  			player:AddSkillRate( 167 , 59)
  		  player:AddSpeed(2)
  		else return 0  end	

  	elseif template_id == transforms["jinengjiazuhehuan"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( 131789)
  			player:AddMP( 103912 )
  			player:AddCrit( 271 , 0.53)
  	  	player:AddDamage( 6515)
  			player:AddAttack( 761)
  			player:AddDefense( 3528)
  			player:AddArmor( 569)
  			player:AddResistance( 328 , 419 , 420, 274, 291,0)
  			player:AddAntiCrit( 441 , 2.84)
  			player:AddSkillRate( 149 , 83)
  		  player:AddSpeed(2)
  		else return 0  end	

  	elseif template_id == transforms["jinengjiazutianyin"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( 147944)
  			player:AddMP( 89215 )
  			player:AddCrit( 181 , 0.46)
  	  	player:AddDamage( 6048)
  			player:AddAttack( 606)
  			player:AddDefense( 3405)
  			player:AddArmor( 635)
  			player:AddResistance( 404 , 333 , 485, 339, 307,0)
  			player:AddAntiCrit( 581 , 3.24)
  			player:AddSkillRate(160 , 86)
  		  player:AddSpeed(2)
  		else return 0  end	

  	elseif template_id == transforms["jinengjiazuguidao"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( 106983 )
  			player:AddMP(  62851 )
  			player:AddCrit( 161 , 0.46 )
  	  	player:AddDamage(  6150 )
  			player:AddAttack( 664 )
  			player:AddDefense( 3553 )
  			player:AddArmor( 593 )
  			player:AddResistance( 361 , 465 , 379, 456, 239,0)
  			player:AddAntiCrit( 441 , 3.05)
  			player:AddSkillRate( 152 , 56)
  		  player:AddSpeed(2)
  		else return 0  end	

  	elseif template_id == transforms["jinengjiazujiuli"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( 143456)
  			player:AddMP( 117896 )
  			player:AddCrit( 171 , 0.56)
  	  	player:AddDamage( 6194)
  			player:AddAttack( 755)
  			player:AddDefense( 3470)
  			player:AddArmor( 463)
  			player:AddResistance( 424 , 399 , 351, 323, 256,0)
  			player:AddAntiCrit( 431 , 3.17)
  			player:AddSkillRate( 164 , 60)
  		  player:AddSpeed(2)
  		else return 0  end	

  	elseif template_id == transforms["jinengjiazulieshan"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( 119286)
  			player:AddMP( 80699 )
  			player:AddCrit( 201 , 0.51)
  	  	player:AddDamage( 6684)
  			player:AddAttack( 734)
  			player:AddDefense( 3624)
  			player:AddArmor( 463)
  			player:AddResistance( 362 , 148 , 356, 379, 300,0)
  			player:AddAntiCrit( 421 , 2.89)
  			player:AddSkillRate(150 , 80)
  		  player:AddSpeed(2)
  		else return 0  end	

  	elseif template_id == transforms["jinengjiazuhuaiguang"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( 125619 )
  			player:AddMP( 99924 )
  			player:AddCrit( 207 , 0.67 )
  	  	player:AddDamage( 6777 )
  			player:AddAttack( 808 )
  			player:AddDefense( 3454 )
  			player:AddArmor( 577)
  			player:AddResistance( 385 , 405 , 386, 313, 264,0 )
  			player:AddAntiCrit( 421 , 2.37 )
  			player:AddSkillRate( 152 , 78 )
  		  player:AddSpeed(2)
  		else return 0  end	

  	elseif template_id == transforms["jinengjiazutianhua"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP(114215)
  			player:AddMP( 130992 )
  			player:AddCrit( 161 , 0.51)
  	  	player:AddDamage( 6118)
  			player:AddAttack( 700)
  			player:AddDefense( 3748)
  			player:AddArmor( 603)
  			player:AddResistance( 338 , 349 , 389, 344, 262,0)
  			player:AddAntiCrit( 521 , 2.53)
  			player:AddSkillRate( 168 , 63)
  		  player:AddSpeed(2)
  		else return 0  end	

  	elseif template_id == transforms["jinengjiazufenxiang"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( 106983 )
  			player:AddMP(  62851 )
  			player:AddCrit( 161 , 0.46 )
  	  	player:AddDamage(  6150 )
  			player:AddAttack( 664 )
  			player:AddDefense( 3553 )
  			player:AddArmor( 593 )
  			player:AddResistance( 361 , 465 , 379, 456, 239,0)
  			player:AddAntiCrit( 441 , 3.05)
  			player:AddSkillRate( 152 , 56)
  		  player:AddSpeed(2)
  		else return 0  end	

  	elseif template_id == transforms["jinengjiazutaihao"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( 106983 )
  			player:AddMP(  62851 )
  			player:AddCrit( 161 , 0.46 )
  	  	player:AddDamage(  6150 )
  			player:AddAttack( 664 )
  			player:AddDefense( 3553 )
  			player:AddArmor( 593 )
  			player:AddResistance( 361 , 465 , 379, 456, 239,0)
  			player:AddAntiCrit( 441 , 3.05)
  			player:AddSkillRate( 152 , 56)
  		  player:AddSpeed(2)
  		else return 0  end	

  	elseif template_id == transforms["jinengjiazuchenhuang"].template_id 
  	or template_id == transforms["jinengjiazuqianji"].template_id 
  	or template_id == transforms["jinengjiazuyingzhao"].template_id 
  	then
  		if level == 1 and exp_level == 1 then
  			player:AddHP(114215)
  			player:AddMP( 130992 )
  			player:AddCrit( 161 , 0.51)
  	  	player:AddDamage( 6118)
  			player:AddAttack( 700)
  			player:AddDefense( 3748)
  			player:AddArmor( 603)
  			player:AddResistance( 338 , 349 , 389, 344, 262,0)
  			player:AddAntiCrit( 521 , 2.53)
  			player:AddSkillRate( 168 , 63)
  		  player:AddSpeed(2)
  		else return 0  end	

  	elseif template_id == transforms["jinengqixibainiu"].template_id then
  		if level == 1 and exp_level == 1 then
  		  player:AddSpeed(-curSpeed + 1.5)
  		else return 0  end	

	--辰皇飞升技能气血增益
  	elseif template_id == transforms["jinengCHfeisheng01"].template_id 
  	or template_id == transforms["jinengCHfeisheng02"].template_id 
  	or template_id == transforms["jinengCHfeisheng03"].template_id 
  	or template_id == transforms["jinengCHfeisheng04"].template_id 
  	or template_id == transforms["jinengCHfeisheng05"].template_id 
  	or template_id == transforms["jinengCHfeisheng06"].template_id 
  	or template_id == transforms["jinengCHfeisheng11"].template_id 
  	or template_id == transforms["jinengCHfeisheng12"].template_id 
  	or template_id == transforms["jinengCHfeisheng13"].template_id 
  	or template_id == transforms["jinengCHfeisheng14"].template_id 
  	or template_id == transforms["jinengCHfeisheng15"].template_id 
  	or template_id == transforms["jinengCHfeisheng16"].template_id 
  	then
  		if level == 1 and exp_level == 1 then
  			player:AddHP(curMP)
  		else return 0  end	


  	end
  end
  
  if transform_type == 2 then
  	if template_id == transforms["renwuceshi01"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP(-curHP+7777)	
  		else return 0  end	

  	elseif template_id == transforms["wupinsanjie01"].template_id then
  		if level == 16 and exp_level == 1 then
  			player:AddHP(0.5*curHP)	
  			player:AddMP(0.5*curMP)	
  			player:AddDefense(500)	
  			player:AddDamage(500)	
  			player:AddCrit(0.10 , 0.50)	
  			player:AddSkillRate(0.2 , 0.1)	
  		else return 0  end 
 
  	elseif template_id == transforms["renwugaoqiao"].template_id or template_id == transforms["renwuhuoyuansu"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddSpeed(-curSpeed + 3.5)
  		else return 0  end	

  	elseif template_id == transforms["wupinwufuhu01"].template_id 
  	or template_id == transforms["renwuchunjietu01"].template_id 
  	or template_id == transforms["renwuhaiwai01"].template_id 
  	then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( 2000 )
  			player:AddCrit( 10 , 0 )
  		else return 0  end 

  	elseif template_id == transforms["wupinwufuhu02"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddAttack( 60 )
  			player:AddArmor( 30 )
  		else return 0  end 

  	elseif template_id == transforms["wupinwufuhu03"].template_id 
  	or template_id == transforms["renwuchunjietu02"].template_id 
  	or template_id == transforms["renwuhaiwai02"].template_id 
  	then
  		if level == 1 and exp_level == 1 then
  			player:AddMP( 2000 )
  			player:AddCrit( 0 , 0.1 )
  		else return 0  end 

  	elseif template_id == transforms["wupinwufuhu04"].template_id 
  	or template_id == transforms["renwuchunjietu03"].template_id 
  	or template_id == transforms["renwuhaiwai03"].template_id 
  	then
  		if level == 1 and exp_level == 1 then
  	  	player:AddDamage( curDamageHigh*0.02+60 )
  		else return 0  end 

  	elseif template_id == transforms["wupinwufuhu05"].template_id 
  	or template_id == transforms["renwuchunjietu04"].template_id 
  	or template_id == transforms["renwuhaiwai04"].template_id 
  	then
  		if level == 1 and exp_level == 1 then
  			player:AddDefense( curDefense*0.02+30 )
  		else return 0  end 

		---愚人节变身
  	elseif template_id == transforms["renwuyurenjie010"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddMP( curMP*0.05 )
  			player:AddDefense( -30 )
  		else return 0  end	

  	elseif template_id == transforms["renwuyurenjie020"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddMP( -curMP*0.1 )
  			player:AddCrit( 10 , 0)
  		else return 0  end	

  	elseif template_id == transforms["renwuyurenjie030"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddDefense( curDefense*0.03 )
  			player:AddCrit( -10 , 0)
  		else return 0  end	

  	elseif template_id == transforms["renwuyurenjie040"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( curHP*0.05 )
  			player:AddSkillRate(-10 , 0)
  		else return 0  end	

  	elseif template_id == transforms["renwuyurenjie050"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddDefense( curDefense*0.03 )
  			player:AddArmor( -30 )
  		else return 0  end	

  	elseif template_id == transforms["renwuyurenjie060"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( -curHP*0.1 )
  			player:AddSkillRate(0 , 20)
  		else return 0  end	

  	elseif template_id == transforms["renwuyurenjie070"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddCrit( 0 , 0.2)
  			player:AddSkillRate(-10 , 0)
  		else return 0  end	

  	elseif template_id == transforms["renwuyurenjie080"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddAntiCrit( 0 , 0.2)
  			player:AddAttack( -30 )
  		else return 0  end	

  	elseif template_id == transforms["renwuyurenjie090"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddMP( curMP*0.05 )
  			player:AddDamage(curDamageHigh*0.01)	
  		else return 0  end	

  	elseif template_id == transforms["renwuyurenjie100"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( curHP*0.05 )
  			player:AddDefense( curDefense*0.01 )
  		else return 0  end	

  	elseif template_id == transforms["renwujingcha"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddSpeed( -curSpeed + 5 )
  			player:AddSkillRate( 0,-curSkillArmorRate + 1999 )
  		else return 0  end	

  	elseif template_id == transforms["renwupingmin"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddSpeed( -curSpeed + 2 )
  		else return 0  end	

  	elseif template_id == transforms["renwudiushoujuan01"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddSpeed( -curSpeed + 2 )
  		else return 0  end	

  	elseif template_id == transforms["renwudiushoujuan02"].template_id 
  			or template_id == transforms["renwudiushoujuan03"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddSpeed( -curSpeed + 3 )
  		else return 0  end	

		---御驾亲征
  	elseif template_id == transforms["renwuLTbangzhu"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( curHP*0.25 )
  		else return 0  end	

		---领土战攻城车
  	elseif template_id == transforms["renwuLTdiji"].template_id 
  			or template_id == transforms["renwuLTdiji02"].template_id	then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( 30000 )
  			player:AddDefense( 2000 )
  			player:AddSpeed( -2.5 )
  		  player:AddAttackRange(-curAttackRange+2)
  		else return 0  end	

  	elseif template_id == transforms["renwuLTzhongji"].template_id  
  	or template_id == transforms["renwuLTzhongji03"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( 50000 )
  			player:AddDefense( 5000 )
  			player:AddSpeed( -3.3 )
  		  player:AddAttackRange(-curAttackRange+22)
  		else return 0  end	

  	elseif template_id == transforms["renwuLTgaoji03"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( 100000 )
  			player:AddDefense( 5000 )
  			player:AddSpeed( -1.5 )
  		  player:AddAttackRange(6)
  		else return 0  end	

  	elseif template_id == transforms["renwuLTdiji01"].template_id 
  	or template_id == transforms["renwuLTzhongji01"].template_id
  	or template_id == transforms["renwuLTgaoji01"].template_id  then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( 10000 )
  			player:AddSpeed( -1.5 )
  		  player:AddAttackRange(-curAttackRange+4)
  		else return 0  end	

  	elseif template_id == transforms["renwuLTzhongji02"].template_id  
  	or template_id == transforms["renwuLTgaoji02"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( 80000 )
  			player:AddDefense( 2000 )
  			player:AddSpeed( -2.5 )
  		  player:AddAttackRange(-curAttackRange+4)
  		else return 0  end	

  	elseif template_id == transforms["renwuLTgaoji"].template_id  
  	or template_id == transforms["renwuLTgaoji2"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( 100000 )
  			player:AddSpeed( -2.5 )
  		  player:AddAttackRange(20)
  		else return 0  end	

  	elseif template_id == transforms["renwuLTgongjianshou"].template_id then
  		if level == 1 and exp_level == 1 then
  		  player:AddAttackRange(20)
  		else return 0  end	

  	elseif template_id == transforms["renwuyayunhui01"].template_id then
  		if level == 1 and exp_level == 1 then
  		  player:AddSpeed( -curSpeed+8 )
  		else return 0  end	

  	elseif template_id == transforms["renwupaopaokading"].template_id then
  		if level == 1 and exp_level == 1 then
  		  player:AddSpeed( -curSpeed+3.5 )
  			player:AddSkillRate(-curSkillAttackRate + 999 , 0)
  		else return 0  end	

  	elseif template_id == transforms["renwuqiaokeli01"].template_id 
  	or template_id == transforms["renwuqiaokeli03"].template_id
  	then
  		if level == 1 and exp_level == 1 then
  			player:AddDamage(curDamageHigh*0.05)	
  		else return 0  end	

  	elseif template_id == transforms["renwuqiaokeli02"].template_id 
  	or template_id == transforms["renwuqiaokeli04"].template_id then
  		if level == 1 and exp_level == 1 then
  			player:AddCrit( 20 , 0)
  		else return 0  end	

  	elseif template_id == transforms["renwushengdan01"].template_id  then
  		if level == 1 and exp_level == 1 then
  			player:AddDamage(curDamageHigh*0.05)	
  		else return 0  end	

  	elseif template_id == transforms["renwushengdan02"].template_id  then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( curHP*0.1 )
  		else return 0  end	

  	elseif template_id == transforms["renwushengdan03"].template_id  then
  		if level == 1 and exp_level == 1 then
  			player:AddMP( curMP*0.1 )
  		else return 0  end	

  	elseif template_id == transforms["renwushengdan04"].template_id  then
  		if level == 1 and exp_level == 1 then
  			player:AddArmor( 100 )
  		else return 0  end	

  	elseif template_id == transforms["renwushengdan05"].template_id  then
  		if level == 1 and exp_level == 1 then
  			player:AddCrit( 25 , 0 )
  		else return 0  end	

  	elseif template_id == transforms["renwushengdan06"].template_id  then
  		if level == 1 and exp_level == 1 then
  			player:AddCrit( 0 , 0.03 )
  		else return 0  end	

	---任务怪叔叔
  	elseif template_id == transforms["renwuguaishushu3"].template_id  
  	or template_id == transforms["renwuguaishushu8"].template_id
  	then
  		if level == 1 and exp_level == 1 then
  			player:AddResistance( 0 , 0 , 9000, 0, 0,0)
  		else return 0  end	

  	elseif template_id == transforms["renwuguaishushu2"].template_id  then
  		if level == 1 and exp_level == 1 then
  		  player:AddSpeed( -1.5 )
  		else return 0  end	
  	

  	elseif template_id == transforms["renwu641"].template_id  then
  		if level == 1 and exp_level == 1 then
  			player:AddDamage(curDamageHigh*0.02+60)	
  		else return 0  end	

  	elseif template_id == transforms["renwuqixibainiu"].template_id then
  		if level == 1 and exp_level == 1 then
  		  player:AddSpeed(-curSpeed + 1.5)
  		else return 0  end	
	
	---河阳灾变副本
  	elseif template_id == transforms["renwufuben001"].template_id 
  			or template_id == transforms["renwufuben002"].template_id 
  			or template_id == transforms["renwufuben003"].template_id 
  			or template_id == transforms["renwufuben101"].template_id 
  			or template_id == transforms["renwufuben102"].template_id 
  			or template_id == transforms["renwufuben103"].template_id 
  			or template_id == transforms["renwufuben104"].template_id 
  			or template_id == transforms["renwufuben105"].template_id 
  			or template_id == transforms["renwufuben106"].template_id 
  			or template_id == transforms["renwufuben107"].template_id 
  			or template_id == transforms["renwufuben108"].template_id 
  			or template_id == transforms["renwufuben109"].template_id 
  			or template_id == transforms["renwufuben110"].template_id 
  			or template_id == transforms["renwufuben111"].template_id 
  			or template_id == transforms["renwufuben112"].template_id 
  			or template_id == transforms["renwufuben113"].template_id 
  			or template_id == transforms["renwufuben114"].template_id 
  	then
  		if level == 1 and exp_level == 1 then
  			player:AddHP( 300000-curHP )
  			player:AddMP( 280000-curMP )
  			player:AddCrit( 950-curCritRate , 8.00-curCritDamage )
  	  	player:AddDamage( 30000-curDamageHigh )
  			player:AddAttack( 1200-curAttack )
  			player:AddDefense( 6666-curDefense )
  			player:AddArmor( 800-curArmor )
  			player:AddResistance( 888-curResistance0 , 888-curResistance1 , 888-curResistance2, 888-curResistance3, 888-curResistance4, 0)
  			player:AddAntiCrit( 666-curAntiCritRate , 6.66-curAntiCritDamage)
  			player:AddSkillRate( 188-curSkillAttackRate , 88-curSkillArmorRate)
  		  player:AddSpeed(6.5-curSpeed)
  		else return 0  end	

  	end

  end

  return 1
end


