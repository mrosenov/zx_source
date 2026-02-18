--[[
//添加新宠物需要设置的数据区

1.设定合法宠物
ZLuaAvailablePets()

--]]

---当前合法宠物ID----//此处待补充------------------------------------------------------------
function ZLuaAvailablePets()
	local AvailablePets={}
	local Race=ZLuaPetBedgeRaceDesc()
	local Element=ZLuaPetBedgeElementDesc()
	local Horoscope=ZLuaPetBedgeHoroscopeDesc()
	
	AvailablePets["TestPet"]=
	{pet_tid=15782,	race=Race["renzu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}---//测试1
	---//是否可以能力修炼，是否可以年龄/寿命，是否可以放生，是否有形象成长，初始的假档位
	 
	AvailablePets["TestPet2"]=
	{pet_tid=15843,race=Race["dijing"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}---//测试2

	AvailablePets["conglinshanzhu"]=
	{pet_tid=16503,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=22992,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//丛林山猪(野兽)
	
	AvailablePets["beimingshuyao"]=
	{pet_tid=16504,race=Race["shanjing"],
	crit=0.01,critdmg=1.1,spirit_id=22993,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//北冥树妖(山精)
	
	AvailablePets["bingyuanxuexiong"]=
	{pet_tid=16505,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=22994,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//冰原雪熊(野兽)
	
	AvailablePets["meixindiejing"]=
	{pet_tid=16506,race=Race["meizu"],
	crit=0.01,critdmg=1.1,spirit_id=22995,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//魅心蝶精(魅族)
	
	AvailablePets["songcaijinshu"]=
	{pet_tid=16507,race=Race["yishou"],
	crit=0.01,critdmg=1.1,spirit_id=22996,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//送财金鼠(异兽)

	AvailablePets["shouhushiling"]=
	{pet_tid=16508,race=Race["yuansu"],
	crit=0.01,critdmg=1.1,spirit_id=22997,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//守护石灵(元素)

	AvailablePets["yiyujingling"]=
	{pet_tid=18306,race=Race["huanling"],
	crit=0.01,critdmg=1.1,spirit_id=22998,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//异域精灵(幻灵)
	
	AvailablePets["Q_conglinshanzhu"]=
	{pet_tid=18385,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=22992,
	combine_able=1,fixage_able=1,free_able=0,
	changefigure_able=1,default_potential=1,born_special=0}	---//丛林山猪(野兽)(任务)
	
	AvailablePets["Q_beimingshuyao"]=
	{pet_tid=18386,race=Race["shanjing"],
	crit=0.01,critdmg=1.1,spirit_id=22993,
	combine_able=1,fixage_able=1,free_able=0,
	changefigure_able=1,default_potential=1,born_special=0}	---//北冥树妖(山精)(任务)

	AvailablePets["Q_shouhushiling"]=
	{pet_tid=18798,race=Race["yuansu"],
	crit=0.01,critdmg=1.1,spirit_id=22997,
	combine_able=1,fixage_able=1,free_able=0,
	changefigure_able=1,default_potential=1,born_special=0}	---//守护石灵(元素)(任务)	

	AvailablePets["S_luxueqi"]=
	{pet_tid=18416,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=30,born_special=15}	---//陆雪琪(特殊)(特定)	
	
	AvailablePets["S_luxueqi2"]=
	{pet_tid=21773,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=30,born_special=15}	---//陆雪琪(特殊2)(特定)	

	AvailablePets["Q_heyangqingwa"]=
	{pet_tid=18818,race=Race["shuizu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=1,free_able=1,
	changefigure_able=0,default_potential=1,born_special=0}	---//河阳青蛙(水族)(任务/食物宠)	
	
	AvailablePets["Q_heyangyelang"]=
	{pet_tid=18819,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=1,free_able=1,
	changefigure_able=0,default_potential=1,born_special=0}	---//河阳野狼(野兽)(任务/食物宠)	

	AvailablePets["sancaiyaozhu"]=
	{pet_tid=18929,race=Race["yaozu"],
	crit=0.01,critdmg=1.1,spirit_id=22999,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//三彩妖猪(妖族)	

	AvailablePets["poliudijing"]=
	{pet_tid=19263,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//破流地精(仙灵)	
	
	AvailablePets["miaochihuangniao"]=
	{pet_tid=19510,race=Race["yuzu"],
	crit=0.01,critdmg=1.1,spirit_id=23000,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//妙翅黄鸟(羽族)	

	AvailablePets["caiyihuangniao"]=
	{pet_tid=20108,race=Race["yuzu"],
	crit=0.01,critdmg=1.1,spirit_id=23001,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//彩翼黄鸟(羽族)	

	AvailablePets["huanmoshuling"]=
	{pet_tid=19110,race=Race["huanling"],
	crit=0.01,critdmg=1.1,spirit_id=23002,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//幻魔树灵(幻灵)		
	
	AvailablePets["xianjielinghou"]=
	{pet_tid=20488,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=23003,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//仙界灵猴(仙灵)
	
	AvailablePets["shashouzhixie"]=
	{pet_tid=20489,race=Race["chonggu"],
	crit=0.01,critdmg=1.1,spirit_id=23004,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//杀手之蝎(虫蛊)			
	
	AvailablePets["zhanshenyanmo"]=
	{pet_tid=20561,race=Race["yuansu"],
	crit=0.01,critdmg=1.1,spirit_id=23005,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=0,default_potential=1,born_special=0}	---//战神炎魔(元素)		
	
	AvailablePets["shengdanxueren"]=
	{pet_tid=22105,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=23006,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//圣诞雪人(仙灵)		
	
	AvailablePets["yuegongyutu"]=
	{pet_tid=22484,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=23007,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//月宫玉兔(仙灵)		
	
	AvailablePets["menghuanmaonv"]=
	{pet_tid=21247,race=Race["yaozu"],
	crit=0.01,critdmg=1.1,spirit_id=23008,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//梦幻猫女(妖族)		

	AvailablePets["S_biyao"]=
	{pet_tid=19124,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=30,born_special=15}	---//碧瑶(特殊)(特定)	
	
	AvailablePets["S_kuilei01"]=
	{pet_tid=22431,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=30,born_special=15}	---//机关巨人(特殊)(特定)	
	
	AvailablePets["S_kuilei02"]=
	{pet_tid=22432,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=30,born_special=15}	---//涅磐天尊(特殊)(特定)	
	
	AvailablePets["S_kuilei03"]=
	{pet_tid=22433,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=30,born_special=15}	---//道玄(特殊)(特定)	
	
	AvailablePets["S_kuilei04"]=
	{pet_tid=22434,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=30,born_special=15}	---//黑心老人(特殊)(特定)	
	
	AvailablePets["S_kuilei05"]=
	{pet_tid=22435,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=30,born_special=15}	---//金铃夫人(特殊)(特定)	
	
	AvailablePets["S_kuilei06"]=
	{pet_tid=22436,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=30,born_special=15}	---//普智(特殊)(特定)	

	AvailablePets["S_kuilei07"]=
	{pet_tid=22437,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=30,born_special=15}	---//鬼先生(特殊)(特定)	
	
	AvailablePets["baihu"]=
	{pet_tid=23337,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=23338,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//白虎(野兽)
	
	AvailablePets["N_shouhushiling"]=
	{pet_tid=24650,race=Race["yuansu"],
	crit=0.01,critdmg=1.1,spirit_id=22997,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//扭蛋守护石灵(野兽)
	
	AvailablePets["G_conglinshanzhu"]=
	{pet_tid=24781,race=Race["shanjing"],
	crit=0.01,critdmg=1.1,spirit_id=25283,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//丛林山猪(礼包)
	
	AvailablePets["H_shouhushiling"]=
	{pet_tid=25117,race=Race["yuansu"],
	crit=0.01,critdmg=1.1,spirit_id=22997,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//鸿利守护石灵(野兽)
	
	AvailablePets["H_songcaijinshu"]=
	{pet_tid=25118,race=Race["yishou"],
	crit=0.01,critdmg=1.1,spirit_id=22996,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//鸿利送财金鼠(异兽)
	
	AvailablePets["sanyanlinghou"]=
	{pet_tid=25124,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=25125,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//三眼灵猴(仙灵)
	
	AvailablePets["Z_baihu"]=
	{pet_tid=25234,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=23338,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//战场白虎(野兽)
	
	AvailablePets["maozei"]=
	{pet_tid=25311,race=Race["yaozu"],
	crit=0.01,critdmg=1.1,spirit_id=25391,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//猫贼(妖族)	
	
	AvailablePets["cairen_1"]=
	{pet_tid=28557,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=0,default_potential=1,born_special=0}	---//才人1(特殊)(特定)	
	
	AvailablePets["cairen_2"]=
	{pet_tid=28558,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=0,default_potential=1,born_special=0}	---//才人2(特殊)(特定)	
	
	AvailablePets["cairen_3"]=
	{pet_tid=28559,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=0,default_potential=1,born_special=0}	---//才人3(特殊)(特定)	
	
	AvailablePets["cairen_4"]=
	{pet_tid=28560,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=0,default_potential=1,born_special=0}	---//才人4(特殊)(特定)	
	
	AvailablePets["cairen_5"]=
	{pet_tid=28561,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=0,default_potential=1,born_special=0}	---//才人5(特殊)(特定)
	
	AvailablePets["fuwang"]=
	{pet_tid=33448,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=0,default_potential=1,born_special=0}	---//斧王
	
	AvailablePets["dahuilang"]=
	{pet_tid=25422,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//大灰狼(野兽)
	
	AvailablePets["dahuilang2"]=
	{pet_tid=34054,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//大灰狼2(野兽)	
	
	
	AvailablePets["xiaomianyang"]=
	{pet_tid=25421,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//小绵羊(野兽)
	
	AvailablePets["zhanshenxingxing"]=
	{pet_tid=25385,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//09跨服PK奖励宠物猩猩(野兽)

	AvailablePets["huli"]=
	{pet_tid=29992,race=Race["yishou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=25,born_special=15}	---//狐狸(异兽赠送高星等)

--[[
	AvailablePets["huli"]=
	{pet_tid=29992,race=Race["yishou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=1,born_special=0}	---//狐狸(异兽赠送高星等)
--]]
	
	AvailablePets["huli2"]=
	{pet_tid=33146,race=Race["yishou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//狐狸(异兽)

	AvailablePets["dahuang"]=
	{pet_tid=29993,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//大黄(仙灵)
	
	AvailablePets["songshu"]=
	{pet_tid=29994,race=Race["shanjing"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//松鼠(山精)
	
	AvailablePets["xiongmao"]=
	{pet_tid=29995,race=Race["yishou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//熊猫(异兽)
	
	AvailablePets["lu"]=
	{pet_tid=29996,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//鹿(野兽)
	
	AvailablePets["nainiu"]=
	{pet_tid=29997,race=Race["yishou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//奶牛(异兽)		
	
	AvailablePets["datouwawa"]=
	{pet_tid=30988,race=Race["yaozu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//大头娃娃(妖族)	
	
	AvailablePets["panguan"]=
	{pet_tid=30989,race=Race["siling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//判官(死灵)	
	
	AvailablePets["maotouying"]=
	{pet_tid=30990,race=Race["shanjing"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//猫头鹰(山精)	
	
	AvailablePets["yanmo"]=
	{pet_tid=30991,race=Race["yuansu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//岩魔(元素)	
	
	AvailablePets["zx2libaochong"]=
	{pet_tid=31463,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=20,born_special=15}	---//诛仙2礼包宠(特殊)
	
	AvailablePets["qingtianzhanhun"]=
	{pet_tid=32118,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=40,born_special=15}	---//国庆草庙村特殊宠(特殊)	
	
	AvailablePets["xinshoukabaihu"]=
	{pet_tid=33280,race=Race["yishou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=25,born_special=15}	---//白虎(虎年红包卡赠送)
	
	AvailablePets["疫鬼"]=
	{pet_tid=33522,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=20,born_special=15}	---//疫鬼(特殊)
	
	AvailablePets["qingrenjielaohu"]=
	{pet_tid=34069,race=Race["yishou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=25,born_special=15}	---//情人节老虎(白色情人节新手卡赠送)
	
	AvailablePets["qiubite"]=
	{pet_tid=34126,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//丘比特（仙灵）
	
	AvailablePets["yecha"]=
	{pet_tid=34214,race=Race["shuizu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//天帝宝库宠物-夜叉(水族)
	
	AvailablePets["shuangmianguai"]=
	{pet_tid=34215,race=Race["yaozu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//死亡竞技场宠物-双面怪(妖族)
	
	AvailablePets["pangtouyu"]=
	{pet_tid=34289,race=Race["shuizu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//胖头鱼(水族)
	
	AvailablePets["guangminghuo"]=
	{pet_tid=40541,race=Race["yuansu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//光明火(元素)
	
	AvailablePets["meiqijingling"]=
	{pet_tid=40631,race=Race["yuansu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//美奇精灵(元素)
	
	AvailablePets["leishe"]=
	{pet_tid=40665,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//雷蛇(野兽)
	
		AvailablePets["beimeixiniu"]=
	{pet_tid=40863,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//北美犀牛(野兽)
	
		AvailablePets["beimeikuiniu"]=
	{pet_tid=40864,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//北美夔牛(野兽)
	
		AvailablePets["beimeizhizhu"]=
	{pet_tid=40865,race=Race["yaozu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//北美蜘蛛(妖族)
	
		AvailablePets["beimeisantouquan"]=
	{pet_tid=40866,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//北美三头犬(特殊)
	
		AvailablePets["beimeirenshenwawa"]=
	{pet_tid=40867,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//北美人参娃娃(仙灵)
	
		AvailablePets["beimeiqie"]=
	{pet_tid=40868,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//北美企鹅(野兽)
	
		AvailablePets["beimeihuolong"]=
	{pet_tid=40869,race=Race["yishou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//北美火龙(异兽)
	
		AvailablePets["beimeidaxiang"]=
	{pet_tid=40870,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//北美大象(野兽)
	
	AvailablePets["yudigege"]=
	{pet_tid=41166,race=Race["renzu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=24,born_special=15}	---//御帝哥哥(人族)
	
	AvailablePets["aifei"]=
	{pet_tid=41167,race=Race["renzu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=24,born_special=15}	---//爱妃(人族)
	
	AvailablePets["zhenyumoxie"]=
	{pet_tid=41261,race=Race["chonggu"],
	crit=0.01,critdmg=1.1,spirit_id=23004,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//镇狱魔蝎(虫蛊)	
	
	AvailablePets["guanghanyutu"]=
	{pet_tid=41262,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=23007,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//广寒玉兔(仙灵)	

	AvailablePets["2010taotie"]=
	{pet_tid=41928,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//2010跨服pk赛奖励宠物_饕餮（仙灵）
	
	AvailablePets["1123linghu"]=
	{pet_tid=41782,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=25,born_special=15}	---//灵狐(仙灵)	
	
	AvailablePets["1123yuty"]=
	{pet_tid=41783,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=25,born_special=15}---//玉兔(仙灵)

	AvailablePets["1123feitianbaihu"]=
	{pet_tid=41784,race=Race["yishou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=25,born_special=15}---//飞天白虎(异兽)
	
	AvailablePets["1123zhandimenghu"]=
	{pet_tid=41785,race=Race["yishou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=25,born_special=15}---//战地猛虎(异兽)
	
	AvailablePets["chongwuzhangyu"]=
	{pet_tid=42683,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}---//20100118宠物章鱼（野兽）
	
	AvailablePets["2011kuiniu"]=
	{pet_tid=43404,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//2011祈愿夔牛(野兽)
	
	AvailablePets["vipdaxiang"]=
	{pet_tid=43405,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//vip大象(野兽)
	
	AvailablePets["20110524renshenwawa"]=
	{pet_tid=43254,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//海外人参娃娃(仙灵)
	
	AvailablePets["20110524feitianbaihu"]=
	{pet_tid=44162,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=25,born_special=15}	---//20110524飞天白虎(仙灵)
	
	AvailablePets["20110524guanghanyutu"]=
	{pet_tid=44163,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=25,born_special=15}	---//20110524广寒玉兔(仙灵)
	
	AvailablePets["20110524bingyuanxuexiong"]=
	{pet_tid=44164,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=25,born_special=15}	---//20110524冰原雪熊(野兽)
	
	AvailablePets["20110524kuiniu"]=
	{pet_tid=44246,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=25,born_special=15}	---//20110524VIP夔牛(野兽)
	
	AvailablePets["20100621qie"]=
	{pet_tid=44644,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}		---//20110621企鹅（野兽）
	
	AvailablePets["2011kuafuxiaohuolong"]=
	{pet_tid=49297,race=Race["yishou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//2011跨服PK小火龙(异兽)
	
	AvailablePets["20110922xiaolangren"]=
	{pet_tid=49336,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}		---//20110922小浪人（野兽）
	
	AvailablePets["20110922waiwaixiongnan"]=
	{pet_tid=49337,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}		---//20110922歪歪熊男（野兽）
	
	AvailablePets["20110922waiwaixiongnv"]=
	{pet_tid=49338,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}		---//20110922歪歪熊女（野兽）
	
	AvailablePets["20110922mafei"]=
	{pet_tid=43443,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}		---//20110922马匪（仙灵）
	
	AvailablePets["2011092218xingmafei"]=
	{pet_tid=50624,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=1,default_potential=25,born_special=15}		---//2011092218星马匪（仙灵）
	
	AvailablePets["lingxiaocheng1"]=
	{pet_tid=50764,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=30,born_special=15}	---//凌霄城神裔1号宠物特殊)(特定)	
	
	AvailablePets["lingxiaocheng2"]=
	{pet_tid=50776,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=30,born_special=15}	---//凌霄城神裔2号宠物特殊)(特定)	
	
	AvailablePets["lingxiaocheng3"]=
	{pet_tid=50777,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=30,born_special=15}	---//凌霄城神裔3号宠物特殊)(特定)	
	
	AvailablePets["lingxiaocheng4"]=
	{pet_tid=50778,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=30,born_special=15}	---//凌霄城神裔4号宠物特殊)(特定)	
	
	AvailablePets["lingxiaocheng5"]=
	{pet_tid=50779,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=30,born_special=15}	---//凌霄城神裔5号宠物特殊)(特定)	
	
	AvailablePets["lingxiaocheng6"]=
	{pet_tid=50780,race=Race["teshu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=0,default_potential=30,born_special=15}	---//凌霄城神裔6号宠物特殊)(特定)	
	
	AvailablePets["biyao"]=
	{pet_tid=50846,race=Race["renzu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//碧瑶(人族)
	
	AvailablePets["2012012218xinglong"]=
	{pet_tid=51081,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=1,default_potential=25,born_special=15}		---//2012012218星龙（仙灵）
	
	AvailablePets["20120122long"]=
	{pet_tid=51112,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//20120122龙(仙灵)
	
	AvailablePets["2012042718xingchongwu1"]=
	{pet_tid=52354,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=1,default_potential=25,born_special=15}		---//2012042718星任务宠物1（仙灵）
	
	AvailablePets["2012042718xingchongwu2"]=
	{pet_tid=52355,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=1,default_potential=25,born_special=15}		---//2012042718星任务宠物2（仙灵）
	
	AvailablePets["2012042718xingchongwu3"]=
	{pet_tid=52356,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=1,default_potential=25,born_special=15}		---//2012042718星任务宠物3（仙灵）
	
	AvailablePets["2012042718xinglinghou"]=
	{pet_tid=51507,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=1,default_potential=25,born_special=15}		---//2012042718星三眼灵猴（仙灵）
	
	AvailablePets["2012042718xingxueren"]=
	{pet_tid=51508,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=1,default_potential=25,born_special=15}		---//2012042718星圣诞雪人（仙灵）
	
	AvailablePets["20120503huangniao"]=
	{pet_tid=51755,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}					---//20120503腾讯黄鸟（仙灵）
	
	AvailablePets["20120503zhuxiaoxian"]=
	{pet_tid=52380,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}					---//20120503诛小仙（仙灵）
	
	
	AvailablePets["20120505zhuxiaoxian1"]=
	{pet_tid=51566,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}					---//20120505宠物1（仙灵）
	
	AvailablePets["20120505zhuxiaoxian2"]=
	{pet_tid=51567,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}					---//20120505宠物2（仙灵）
	
	AvailablePets["20120505zhuxiaoxian3"]=
	{pet_tid=52387,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}					---//20120505宠物3（仙灵）
	
	AvailablePets["20120510zhuxiaoxian"]=
	{pet_tid=52408,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}					---//20120510鬼厉诛小仙（仙灵）
	
	AvailablePets["20130704zhuxiaoxian2"]=
	{pet_tid=58404,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}					---//20130704雪琪诛小仙（仙灵）

	AvailablePets["20120629biyao"]=
	{pet_tid=52498,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}					---//20120629碧瑶（仙灵）
	
	AvailablePets["20120808haozhui"]=
	{pet_tid=52702,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}					---//20120808豪锥（仙灵）
	
	AvailablePets["2012082118xingguoqinghuolong"]=
	{pet_tid=53401,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=1,default_potential=25,born_special=15}		---//2012042718星国庆火龙（仙灵）
	
	AvailablePets["2012082118xingzhuxiaoxian"]=
	{pet_tid=53403,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=1,default_potential=25,born_special=15}		---//2012042718星诛小仙（仙灵）	
	
	AvailablePets["20120905zhuxiaoxianpk"]=
	{pet_tid=54653,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//20120905跨服pk赛奖励宠物_诛小仙（仙灵）
	
	AvailablePets["20120905bingyuanxuexiong"]=
	{pet_tid=54661,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//20120905礼包冰原雪熊（野兽）
	
	AvailablePets["20120905zhenyumoxie1"]=
	{pet_tid=54662,race=Race["chonggu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//20120905镇狱魔蝎1(虫蛊)
	
		AvailablePets["20120905zhenyumoxie2"]=
	{pet_tid=54663,race=Race["chonggu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//20120905镇狱魔蝎2(虫蛊)
	
		AvailablePets["20120905zhenyumoxie3"]=
	{pet_tid=54664,race=Race["chonggu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//20120905镇狱魔蝎3(虫蛊)
	
		AvailablePets["20120905zhenyumoxie4"]=
	{pet_tid=54665,race=Race["chonggu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//20120905镇狱魔蝎4(虫蛊)

  AvailablePets["20120905shouhushiling1"]=
	{pet_tid=54666,race=Race["yuansu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//20120905守护石灵1(元素)
	
	  AvailablePets["20120905shouhushiling2"]=
	{pet_tid=54667,race=Race["yuansu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//20120905守护石灵2(元素)
	
	  AvailablePets["20120905shouhushiling3"]=
	{pet_tid=54668,race=Race["yuansu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//20120905守护石灵3(元素)
	
	  AvailablePets["20120905shouhushiling4"]=
	{pet_tid=54669,race=Race["yuansu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//20120905守护石灵4(元素)
	
	AvailablePets["20121009shengdanxueren"]=
	{pet_tid=54845,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=23006,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//20121009礼包圣诞雪人(仙灵)
	
	AvailablePets["leishegaise2012-11-20"]=
	{pet_tid=55193,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//雷蛇改色20121120(野兽)
	
	AvailablePets["2012122518xingfeixueqi"]=
	{pet_tid=55445,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=1,default_potential=25,born_special=15}		---//2012122518星肥雪琪（仙灵）
	
	AvailablePets["2012122618xingchunjiechongwu"]=
	{pet_tid=55468,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=1,default_potential=25,born_special=15}		---//2012122618星春节宠物（仙灵）
	
	AvailablePets["20121227chunjiexiangweishe"]=
	{pet_tid=55494,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//20121227三星春节响尾蛇(野兽)
	
	AvailablePets["20130307yingwu"]=
	{pet_tid=55488,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//20130307三星鹦鹉(野兽)
	
	AvailablePets["20130307yuanyang"]=
	{pet_tid=55780,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}	---//20130307三星鸳鸯(野兽)
	
	AvailablePets["20130307baoxiyingwu"]=
	{pet_tid=55486,race=Race["yeshou"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=1,default_potential=25,born_special=15}		---//2013030718星报喜鹦鹉（野兽）
	
	AvailablePets["20130425guili"]=
	{pet_tid=58041,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=1,default_potential=25,born_special=15}		---//2013042518星诛小仙鬼厉（仙灵）
	
	AvailablePets["20130507yingwu"]=
	{pet_tid=58234,race=Race["yeshou"],
	crit=0.03,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=1,default_potential=25,born_special=15}		---//2013050718星台历鹦鹉（仙灵）

	AvailablePets["20130711maoxiaoxian"]=
	{pet_tid=58412,race=Race["yaozu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}					---//20130711猫小仙（妖族）

	AvailablePets["20130730beimeichongwu01"]=
	{pet_tid=58438,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}					---//20130730北美宠物01（仙灵）

	AvailablePets["20130730beimeichongwu02"]=
	{pet_tid=58439,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}					---//20130730北美宠物02（仙灵）

	AvailablePets["20130711maoxiaoxianreal"]=
	{pet_tid=60299,race=Race["yaozu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}					---//20130826猫小仙修复版（妖族）

	AvailablePets["20130826zhuxianxian"]=
	{pet_tid=60273,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}					---//20130826永久诛小仙（仙灵）

	AvailablePets["20130910taobaobao"]=
	{pet_tid=60277,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}					---//20130910taobaobao（仙灵）

	AvailablePets["20130910tengxunshizhe"]=
	{pet_tid=60848,race=Race["yuzu"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}					---//20130910taobaobao（仙灵）

	AvailablePets["20130915kuafu01"]=
	{pet_tid=60451,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=1,default_potential=25,born_special=15,potential_reborncnt=1}		---//2013091518星跨服飞升战宠（仙灵）

	AvailablePets["20130915kuafu02"]=
	{pet_tid=60452,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=1,default_potential=25,born_special=15,potential_reborncnt=1}		---//2013091518星跨服飞升战宠（仙灵）

	AvailablePets["20130922baihu"]=
	{pet_tid=62191,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}					---//20130922baihu（仙灵）

	AvailablePets["20130929momo"]=
	{pet_tid=62257,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}					---//20130929momo猴（仙灵）

	AvailablePets["20130929momo18"]=
	{pet_tid=62258,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=1,default_potential=25,born_special=15}		---//20130929momo猴18星（仙灵）

	AvailablePets["20131120xiaoxiao"]=
	{pet_tid=62725,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=0,fixage_able=0,free_able=0,
	changefigure_able=1,default_potential=25,born_special=15}					---//20131120小小天公18星（仙灵）

	AvailablePets["20131129shengdan"]=
	{pet_tid=62772,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}					---//20131129圣诞宠物（仙灵）

	AvailablePets["20131212GTnan"]=
	{pet_tid=62920,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}					---//20131212GTnan

	AvailablePets["20131212GTnv"]=
	{pet_tid=62921,race=Race["xianling"],
	crit=0.01,critdmg=1.1,spirit_id=0,
	combine_able=1,fixage_able=1,free_able=1,
	changefigure_able=1,default_potential=1,born_special=0}					---//20131212GTnv

	return AvailablePets
end 
---分割线什么的最讨厌了----------------------------------------------------------------------------
---宠物data索引数组----------------------------------------------------------------------------
function ZLuaPetBedgeDataIndex()
	local DataIndex={}
		DataIndex["blank"]												= 0
		DataIndex["id"]														= 1
		DataIndex["star"]													= 2
		DataIndex["maintype"]											= 3
		DataIndex["element"]											= 4
		DataIndex["identify"]											= 5
		DataIndex["race"]													= 6
		DataIndex["horoscope"]										= 7
		DataIndex["extradamage"]									= 8
		DataIndex["extradamagereduce"]						= 9
		DataIndex["maxhunger"]										= 10
		DataIndex["curhunger"]										= 11	
		DataIndex["maxhonor"]											= 12
		DataIndex["curhonor"]											= 13		
		DataIndex["critrate"]											= 14
		DataIndex["critdamage"]										= 15
		DataIndex["nativecritrate"]								= 16
		DataIndex["nativecritdamage"]							= 17			
		DataIndex["age"]													= 18
		DataIndex["life"]													= 19
		DataIndex["acuity"]												= 20
		DataIndex["stamina"]											= 21
		DataIndex["sawy"]													= 22
		DataIndex["dowry"]												= 23
		DataIndex["wisdom"]												= 24
		DataIndex["cnr"]													= 25
		DataIndex["face"]													= 26
		DataIndex["shape"]												= 27
		DataIndex["isgrowup"]											= 28
		DataIndex["isaberrant"]										= 29
		DataIndex["orign"]												= 30
		DataIndex["nativeorigin"]									= 31
		DataIndex["default_hp"]										= 32		---0
		DataIndex["default_mp"]										= 33		---1
		DataIndex["default_minatk"]								= 34		---2
		DataIndex["default_maxatk"]								= 35		---3
		DataIndex["default_def"]									= 36		---4
		DataIndex["default_attackrate"]						= 37		---5
		DataIndex["default_dodge"]								= 38		---6
		DataIndex["default_resist1"]							= 39		---7
		DataIndex["default_resist2"]							= 40		---8
		DataIndex["default_resist3"]							= 41		---9
		DataIndex["default_resist4"]							= 42		---10
		DataIndex["default_resist5"]							= 43		---11
		DataIndex["default_resist6"]							= 44		---12
		DataIndex["potential_hp"]									= 45		---0
		DataIndex["potential_mp"]									= 46		---1
		DataIndex["potential_minatk"]							= 47		---2
		DataIndex["potential_maxatk"]							= 48		---3
		DataIndex["potential_def"]								= 49		---4
		DataIndex["potential_attackrate"]					= 50		---5
		DataIndex["potential_dodge"]							= 51		---6
		DataIndex["potential_resist1"]						= 52		---7
		DataIndex["potential_resist2"]						= 53		---8
		DataIndex["potential_resist3"]						= 54		---9
		DataIndex["potential_resist4"]						= 55		---10
		DataIndex["potential_resist5"]						= 56		---11
		DataIndex["potential_resist6"]						= 57		---12
		DataIndex["skillid_0"]										= 58		
		DataIndex["skillid_1"]										= 59	
		DataIndex["skillid_2"]										= 60	
		DataIndex["skillid_3"]										= 61	
		DataIndex["skillid_4"]										= 62	
		DataIndex["skillid_5"]										= 63	
		DataIndex["skillid_6"]										= 64	
		DataIndex["skillid_7"]										= 65	
		DataIndex["skillid_8"]										= 66	
		DataIndex["skillid_9"]										= 67	
		DataIndex["skillid_10"]										= 68	
		DataIndex["skillid_11"]										= 69	
		DataIndex["skilllvl_0"]										= 70		
		DataIndex["skilllvl_1"]										= 71	
		DataIndex["skilllvl_2"]										= 72	
		DataIndex["skilllvl_3"]										= 73	
		DataIndex["skilllvl_4"]										= 74	
		DataIndex["skilllvl_5"]										= 75	
		DataIndex["skilllvl_6"]										= 76	
		DataIndex["skilllvl_7"]										= 77	
		DataIndex["skilllvl_8"]										= 78	
		DataIndex["skilllvl_9"]										= 79	
		DataIndex["skilllvl_10"]									= 80	
		DataIndex["skilllvl_11"]									= 81
		DataIndex["talent_1"]											= 82		
		DataIndex["talent_2"]											= 83	
		DataIndex["talent_3"]											= 84	
		DataIndex["talent_4"]											= 85	
		DataIndex["talent_5"]											= 86	
		DataIndex["talent_6"]											= 87	
		DataIndex["talent_7"]											= 88	
		DataIndex["talent_8"]											= 89	
		DataIndex["talent_9"]											= 90	
		DataIndex["talent_10"]										= 91	
		DataIndex["talent_11"]										= 92	
		DataIndex["talent_12"]										= 93
		DataIndex["talent_13"]										= 94
		DataIndex["isspecial"]										= 95
		DataIndex["feedtimes"]										= 96
		DataIndex["feedaids"]											= 97		
		DataIndex["fatalityinchp"]								= 98
		DataIndex["fatalityincmp"]								= 99
		DataIndex["fatalityincminatk"]						= 100
		DataIndex["fatalityincmaxatk"]						= 101
		DataIndex["fatalityincdef"]								= 102
		DataIndex["fatalityaddatkrate"]						= 103
		DataIndex["fatalityadddodge"]							= 104
		DataIndex["fatalityincres1"]							= 105
		DataIndex["fatalityincres2"]							= 106
		DataIndex["fatalityincres3"]							= 107	
		DataIndex["fatalityincres4"]							= 108
		DataIndex["fatalityincres5"]							= 109
		DataIndex["fatalityincres6"]							= 110
		DataIndex["fatalityaddcrit"]							= 111
		DataIndex["fatalityaddcritdmg"]						= 112	
		DataIndex["fatalityaddacuity"]						= 113	
		DataIndex["fatalityaddstamina"]						= 114
		DataIndex["fatalityaddsawy"]							= 115
		DataIndex["fatalityadddowry"]							= 116
		DataIndex["fatalityaddwisdom"]						= 117
		DataIndex["fatalityaddcnr"]								= 118							
		DataIndex["smallgrow"]										= 119
		DataIndex["reserve1"]											= 120
		DataIndex["reserve2"]											= 121
		DataIndex["reserve3"]											= 122
		DataIndex["reserve4"]											= 123
		DataIndex["reserve5"]											= 124
		DataIndex["reserve6"]											= 125
		DataIndex["reserve7"]											= 126
		DataIndex["reserve8"]											= 127
		DataIndex["reserve9"]											= 128	
--[[
		DataIndex["skillid_12"]										= 129	
		DataIndex["skillid_13"]										= 130	
		DataIndex["skillid_14"]										= 131	
		DataIndex["skillid_15"]										= 132	
		DataIndex["skilllvl_12"]									= 133	
		DataIndex["skilllvl_13"]									= 134	
		DataIndex["skilllvl_14"]									= 135	
		DataIndex["skilllvl_15"]									= 136
--]]
		return DataIndex
end 

---宠物来源约定数组----//此处尚未同程序约定-------------------------------
function ZLuaPetBedgeOrigins()
	local ServerOrigins = {}			
	ServerOrigins["task"]  						= 0
	ServerOrigins["gshop"] 						= 1
	ServerOrigins["npc"]  						= 2
	ServerOrigins["lottery"] 	 				= 3
	ServerOrigins["drop"]			 	 			= 4
	ServerOrigins["combine"] 					= 5
	ServerOrigins["catch"] 						= 6
	return ServerOrigins
end 

---宠物五行约定数组-------------------------------------------------------
function ZLuaPetBedgeElementDesc()
	local Element = {}			
	Element["danqie"]  							= 0			---//胆怯
	Element["lumang"]								= 1			---//鲁莽
	Element["kuangye"]  						= 2			---//狂野
	Element["baozao"] 	 						= 3			---//暴躁
	Element["gaoao"]			 	 				= 4			---//高傲	
	Element["youya"]								= 5			---//优雅
	Element["yaomei"]  							= 6			---//妖媚
	Element["wenrou"]								= 7			---//温柔
	Element["yinxian"]  						= 8			---//阴险
	Element["xiee"] 	 							= 9			---//邪恶
	Element["wanpi"]			 	 				= 10		---//顽皮	
	Element["weiyan"]								= 11		---//威严
	Element["shensheng"]  					= 12		---//神圣
	Element["ruizhi"]								= 13		---//睿智
	Element["miantian"]  						= 14		---//腼腆
	Element["boxue"] 	 							= 15		---//博学
	Element["gupi"]			 	 					= 16		---//孤僻	
	Element["yiyu"]									= 17		---//异域

	return Element
end 

---宠物种族约定数组-------------------------------------------------------
function ZLuaPetBedgeRaceDesc()
	local Race = {}			
	Race["renzu"]  								= 0
	Race["yaozu"] 								= 1
	Race["guizu"]  								=	2
	Race["yuzu"]  								=	3
	Race["meizu"] 	 							= 4
	Race["tianjiang"]			 	 			= 5	
	Race["xianling"] 							= 6
	Race["shanjing"]  						= 7
	Race["yeshou"] 								= 8
	Race["yishou"]  							=	9
	Race["shuizu"] 	 							= 10
	Race["chonggu"]			 	 				= 11	
	Race["huanling"] 							= 12
	Race["siling"]  							= 13
	Race["yuansu"] 								= 14
	Race["teshu"]  								=	15
	return Race
	--[[鬼怪/精怪/走兽/飞禽/灵物/仙宠/神怪/虫蛊/
			草木/水族/异兽/化生/死灵/妖魔/上古/神秘--(old)
			人族/妖族/鬼族/羽族/魅族/天将/仙灵/山精/
			野兽/异兽/水族/虫蛊/幻灵/死灵/元素/特殊--]]		
end 

---宠物星辰约定数组-------------------------------------------------------
function ZLuaPetBedgeHoroscopeDesc()
	local Horoscope= {}			
	Horoscope["riyao"]  						= 0		---日曜（圣骑）
	Horoscope["yueyao"] 						= 1		---月曜（萨满）
	Horoscope["jinyao"]  						= 2		---金曜（战士）
	Horoscope["muyao"] 	 						= 3		---木曜（小德）
	Horoscope["shuiyao"]			 			= 4		---水曜（牧师）
	Horoscope["huoyao"] 						= 5		---火曜（法师）
	Horoscope["tuyao"] 							= 6		---土曜（猎人）
	Horoscope["luohou"] 						= 7		---罗候（术士）
	Horoscope["jidu"] 							= 8		---计都（盗贼）
	return Horoscope
end 
---约定的特殊index编码,处理战斗相关的值(初始/成长/当前值)
function ZLuaPetBedgeSpecialIndex()
	local SpecialIndex={}
		SpecialIndex["hp"]=0
		SpecialIndex["mp"]=1
		SpecialIndex["minatk"]=2
		SpecialIndex["maxatk"]=3
		SpecialIndex["defence"]=4
		SpecialIndex["atkrate"]=5
		SpecialIndex["dodge"]=6
		SpecialIndex["res1"]=7		---//眩晕
		SpecialIndex["res2"]=8		---//虚弱
		SpecialIndex["res3"]=9		---//定身
		SpecialIndex["res4"]=10		---//魅惑
		SpecialIndex["res5"]=11		---//昏睡
		SpecialIndex["res6"]=12		---//冰冻
	return SpecialIndex
end 

---//定义飞升属性数组
function ZLuaPetBedgeRebornAttrDataIndex()
	local RebornAttrDataIndex={}
	RebornAttrDataIndex["HP"]	= 0
	RebornAttrDataIndex["MP"]	= 1
	RebornAttrDataIndex["minatk"]	= 2
	RebornAttrDataIndex["maxatk"]	= 3
	RebornAttrDataIndex["def"]	= 4
	RebornAttrDataIndex["attackrate"]	= 5
	RebornAttrDataIndex["dodge"]	= 6
	RebornAttrDataIndex["crit"]	= 7
	RebornAttrDataIndex["critdmg"]	= 8
	RebornAttrDataIndex["res1"]	= 9
	RebornAttrDataIndex["res2"]	= 10
	RebornAttrDataIndex["res3"]	= 11
	RebornAttrDataIndex["res4"]	= 12
	RebornAttrDataIndex["res5"]	= 13
	return 	RebornAttrDataIndex
end


---检查宠物是否为合法宠物ID------------------------------------------------
function ZLuaPetBedgeExistInTmplIDs(pet_tid)
	if type(pet_tid) ~= "number" then return 0 end
	local availablePets = ZLuaAvailablePets()
	local pet
	for _,pet in pairs(availablePets) do
		if pet.pet_tid == pet_tid then return pet.pet_tid end
	end
	return 0
end

---获取宠物种族/五行/致命一击率/致命一击伤害信息---------------------------
function ZLuaPetBedgeStaticInfo(pet_tid)
	if type(pet_tid) ~= "number" then return 0 end
	local availablePets = ZLuaAvailablePets()
	local pet
	for _,pet in pairs(availablePets) do
		if pet.pet_tid == pet_tid then 
			return pet.race,pet.crit,pet.critdmg
		end
	end
	return 0,0,0
end
---宠物初始化--------------------------------------------------------------
function PetBedgeInit(pet_tid,pet,origin,master_level)
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return 0 end
	if pet == nil then return 0 end
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local availablePets = ZLuaAvailablePets()
		
	---取得宠物来源（服务器约定）
	local serverorigin = origin or ServerOrigins["drop"]
	local race,crit,critdmg=ZLuaPetBedgeStaticInfo(pet_tid)
	
	---//定义星相(petbedge_grades中定义)
	local horoscope=ZLuaPetBedgeHoroscopeInfo(pet_tid,origin)

	---//定义性格与说话有关(petbedge_free中定义)
	local element=ZLuaPetBedgeElementInfo(pet_tid,origin)
	
	
	local GradeDirection=ZLuaPetBedgeGradeDirection()
	
	---获取宠物限界/相关档位信息(maintype/Atk/Def/Hp/Mp/Res1-Res6)(file:petbedge_grades.lua)
	local MainType,StarLevel
	local GradesList={}
	MainType=ZLuaPetBedgeSetMaintype(pet_tid,horoscope,origin)
	GradesList,StarLevel=ZLuaPetBedgeInitGrades(pet_tid,horoscope,MainType,serverorigin)
	

	---获取宠物技能信息(Skills/SkillsLevel)(file:petbedge_skills.lua)
	local SkillsList={}
	local SkillsLevel={}
	SkillsList,SkillsLevel=ZLuaPetBedgeSkills(pet_tid,pet,horoscope,serverorigin)
	
	---获取宠物天赋信息(Talents)(file:petbedge_skills.lua)
	local TalenetsList={}
	TalenetsList=ZLuaPetBedgeTalents(pet_tid,serverorigin)
	
	---获取宠物生活能力信息(Live)(file:petbedge_skills.lua)
	local LivesList={}
	LivesList=ZLuaPetBedgeLives(pet_tid,serverorigin)
--[[
	---生成宠物模板ID
	if pet_tid==29992 then
		pet:UpdateData(DataIndex["id"], pet_tid)
	else
		pet:UpdateData(DataIndex["id"], 33146)
	end
--]]

  pet:UpdateData(DataIndex["id"], pet_tid)
  	
	---设定宠物级别和宠物经验
	pet:SetLevel(1)
	pet:SetExp(0)

	---生成宠物鉴定情况
	pet:UpdateData(DataIndex["identify"], 0)
		
	---生成宠物来源、(记录来源)、种族、性格和星相
	pet:UpdateData(DataIndex["orign"], serverorigin)	
	pet:UpdateData(DataIndex["nativeorigin"],serverorigin)	
	pet:UpdateData(DataIndex["race"], race)
	pet:UpdateData(DataIndex["element"], element)
	pet:UpdateData(DataIndex["horoscope"], horoscope)
			
	---处理宠物被动技能/种族技能/星辰技能------------------------------------
	local Race=ZLuaPetBedgeRaceDesc()
	local Horoscope= ZLuaPetBedgeHoroscopeDesc()		
	
	local PseudoPassiveAddDef=0
	local PseudoPassiveAddRes1=0
	local PseudoPassiveAddRes2=0
	local PseudoPassiveAddRes3=0
	local PseudoPassiveAddRes4=0
	local PseudoPassiveAddRes5=0
	local PseudoPassiveAddRes6=0
	local PseudoPassiveAddCrit=0
	local PseudoPassiveAddCritDmg=0
	local PseudoPassiveAddAtkRate=0
	local PseudoPassiveAddDodge=0
	local PseudoPassiveAddExtraDamage=0
	local PseudoPassiveAddExtraDamageReduce=0
	
	local FatalitySkillID,HoroscopeAddSkillID,RaceAddSkillID,DescriptionID=0,0,0,0
	---//轮回技能/星相技能/种族技能/职业描述
	
	FatalitySkillID=ZLuaPetBedgeFatalitySkills(pet_tid,origin)

	if race==Race["renzu"] then 					---人族
		RaceAddSkillID=1223
	elseif race==Race["yaozu"] then 			---妖族
		PseudoPassiveAddRes5=15
		RaceAddSkillID=1224
	elseif race==Race["guizu"] then 			---鬼族
		RaceAddSkillID=1225
	elseif race==Race["yuzu"] then 				---羽族
		PseudoPassiveAddAtkRate=15
		RaceAddSkillID=1226
	elseif race==Race["meizu"] then 			---魅族
		PseudoPassiveAddRes4=15
		RaceAddSkillID=1240
	elseif race==Race["tianjiang"] then		---天将	 
		PseudoPassiveAddRes3=15
		RaceAddSkillID=1241
	elseif race==Race["xianling"] then 		---仙灵
		RaceAddSkillID=1242
	elseif race==Race["shanjing"] then 		---山精
		RaceAddSkillID=1243
	elseif race==Race["yeshou"] then 			---野兽
		PseudoPassiveAddCritDmg=0.1
		RaceAddSkillID=1244
	elseif race==Race["yishou"] then 			---异兽
		PseudoPassiveAddExtraDamageReduce=10
		RaceAddSkillID=1245
	elseif race==Race["shuizu"] then 			---水族
		PseudoPassiveAddRes6=15
		RaceAddSkillID=1246
	elseif race==Race["chonggu"] then 		---虫蛊
		PseudoPassiveAddExtraDamage=20
		RaceAddSkillID=1247
	elseif race==Race["huanling"] then 		---幻灵
		RaceAddSkillID=1248
	elseif race==Race["siling"] then 			---死灵
		PseudoPassiveAddRes1=15
		RaceAddSkillID=1249
	elseif race==Race["yuansu"] then 			---元素
		PseudoPassiveAddDef=10
		PseudoPassiveAddRes2=10
		RaceAddSkillID=1250
	elseif race==Race["teshu"] then 			---特殊
		PseudoPassiveAddDodge=15
		RaceAddSkillID=1251
	else
	end 
	
	if horoscope==Horoscope["riyao"] then				---日曜
		HoroscopeAddSkillID=1252
		DescriptionID=1359
	elseif horoscope==Horoscope["yueyao"] then	---月曜
		HoroscopeAddSkillID=1253
		DescriptionID=1360
	elseif horoscope==Horoscope["jinyao"] then	---金曜
		HoroscopeAddSkillID=1254
		DescriptionID=1361
	elseif horoscope==Horoscope["muyao"] then		---木曜
		HoroscopeAddSkillID=1255
		DescriptionID=1362
	elseif horoscope==Horoscope["shuiyao"] then	---水曜
		HoroscopeAddSkillID=1256
		DescriptionID=1363
	elseif horoscope==Horoscope["huoyao"] then	---火曜
		HoroscopeAddSkillID=1257
		DescriptionID=1364
	elseif horoscope==Horoscope["tuyao"] then		---土曜
		HoroscopeAddSkillID=1258
		DescriptionID=1365
	elseif horoscope==Horoscope["luohou"] then	---罗候
		HoroscopeAddSkillID=1259
		DescriptionID=1366
	elseif horoscope==Horoscope["jidu"] then		---计都
		HoroscopeAddSkillID=1260
		DescriptionID=1367
	else 
	end 
	--[[	
	---生成宠物种族收益影响(PseudoPassiveSkills)
	---人族	语言交流	修炼的成功率增加3％。								1
	---妖族	妖类直觉	昏睡抗性增加15点。									1
	---鬼族	不灭生命	从生命资质获得的成长增加5％					1
	---羽族	鹰眼洞察	命中增加15点。											1
	---魅族	魅者之心	魅惑抗性增加15点。									1
	---天将	自由之戈	定身抗性增加15点。									1
	---仙灵	巧夺天工	所有生活能力值增加5点。							1
	---山精	光芒力量	气血回复速度加倍。									1
	---野兽	野性力量	致命一击伤害增加10％。							1
	---异兽	壁垒贯注	受到伤害减少10点。									1
	---水族	浩瀚乃大	被冰冻的概率下降。									1
	---虫蛊	蝼蚁之力	对普通目标至少造成20点伤害。				1
	---幻灵	虚空汲取	精力回复速度加倍。									1
	---死灵	死亡意志	眩晕抗性增加15点。									1
	---元素	命运藐视	虚弱抗性增加15点。									1
	---特殊	天生幸运	躲闪增加15点。											1
	---人族/妖族/鬼族/羽族/魅族/天将/仙灵/山精/
			野兽/异兽/水族/虫蛊/幻灵/死灵/元素/特殊
	--]]				

	pet:UpdateData(DataIndex["critrate"], crit+PseudoPassiveAddCrit)
	pet:UpdateData(DataIndex["critdamage"], critdmg+PseudoPassiveAddCritDmg)
	pet:UpdateData(DataIndex["nativecritrate"], crit+PseudoPassiveAddCrit)	 	
 	pet:UpdateData(DataIndex["nativecritdamage"], critdmg+PseudoPassiveAddCritDmg)
	---//nativecrit/nativecritdamage为处理亲密度对crit/critdmg影响而设置的属性项
	
	
	---生成宠物战斗相关信息
	pet:UpdateData(DataIndex["extradamage"], 1+PseudoPassiveAddExtraDamage)
	pet:UpdateData(DataIndex["extradamagereduce"], 1+PseudoPassiveAddExtraDamageReduce)
	pet:UpdateData(DataIndex["star"], StarLevel)
	pet:UpdateData(DataIndex["maintype"], MainType)

	---//初始值为实际值
	pet:UpdateData(DataIndex["default_hp"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["default_hp"]],GradeDirection["default_hp"]))	
	pet:UpdateData(DataIndex["default_mp"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["default_mp"]],GradeDirection["default_mp"]))
	pet:UpdateData(DataIndex["default_minatk"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["default_minatk"]],GradeDirection["default_minatk"]))
	pet:UpdateData(DataIndex["default_maxatk"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["default_maxatk"]],GradeDirection["default_maxatk"]))
	pet:UpdateData(DataIndex["default_def"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["default_def"]],GradeDirection["default_def"])+PseudoPassiveAddDef)
	pet:UpdateData(DataIndex["default_attackrate"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["default_attackrate"]],GradeDirection["default_attackrate"])+PseudoPassiveAddAtkRate)
	pet:UpdateData(DataIndex["default_dodge"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["default_dodge"]],GradeDirection["default_dodge"])+PseudoPassiveAddDodge)
	
	pet:UpdateData(DataIndex["default_resist1"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["default_resist1"]],GradeDirection["default_resist1"])+PseudoPassiveAddRes1)
	pet:UpdateData(DataIndex["default_resist2"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["default_resist2"]],GradeDirection["default_resist2"])+PseudoPassiveAddRes2)
	pet:UpdateData(DataIndex["default_resist3"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["default_resist3"]],GradeDirection["default_resist3"])+PseudoPassiveAddRes3)
	pet:UpdateData(DataIndex["default_resist4"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["default_resist4"]],GradeDirection["default_resist4"])+PseudoPassiveAddRes4)
	pet:UpdateData(DataIndex["default_resist5"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["default_resist5"]],GradeDirection["default_resist5"])+PseudoPassiveAddRes5)
	pet:UpdateData(DataIndex["default_resist6"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["default_resist6"]],GradeDirection["default_resist6"])+PseudoPassiveAddRes6)

	---//档位值为标么值(客户端显示值)
	pet:UpdateData(DataIndex["potential_hp"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["potential_hp"]],GradeDirection["potential_hp"]))
	pet:UpdateData(DataIndex["potential_mp"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["potential_mp"]],GradeDirection["potential_mp"]))
	pet:UpdateData(DataIndex["potential_minatk"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["potential_minatk"]],GradeDirection["potential_minatk"]))
	pet:UpdateData(DataIndex["potential_maxatk"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["potential_maxatk"]],GradeDirection["potential_maxatk"]))
	pet:UpdateData(DataIndex["potential_def"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["potential_def"]],GradeDirection["potential_def"]))
	pet:UpdateData(DataIndex["potential_attackrate"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["potential_attackrate"]],GradeDirection["potential_attackrate"]))
	pet:UpdateData(DataIndex["potential_dodge"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["potential_dodge"]],GradeDirection["potential_dodge"]))
	pet:UpdateData(DataIndex["potential_resist1"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["potential_resist1"]],GradeDirection["potential_resist1"]))
	pet:UpdateData(DataIndex["potential_resist2"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["potential_resist2"]],GradeDirection["potential_resist2"]))
	pet:UpdateData(DataIndex["potential_resist3"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["potential_resist3"]],GradeDirection["potential_resist3"]))
	pet:UpdateData(DataIndex["potential_resist4"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["potential_resist4"]],GradeDirection["potential_resist4"]))
	pet:UpdateData(DataIndex["potential_resist5"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["potential_resist5"]],GradeDirection["potential_resist5"]))
	pet:UpdateData(DataIndex["potential_resist6"], ZLuaPetBedgeGradeData(GradesList[GradeDirection["potential_resist6"]],GradeDirection["potential_resist6"]))
		
			
	---生成宠物技能相关信息
	---//1为宠物星辰技能(因为为主动)
	pet:UpdateData(DataIndex["skillid_0"], HoroscopeAddSkillID or 0)	
	pet:UpdateData(DataIndex["skillid_1"], SkillsList[1] or 0)	
	pet:UpdateData(DataIndex["skillid_2"], SkillsList[2] or 0)	
	pet:UpdateData(DataIndex["skillid_3"], SkillsList[3] or 0)	
	pet:UpdateData(DataIndex["skillid_4"], SkillsList[4] or 0)	
	pet:UpdateData(DataIndex["skillid_5"], SkillsList[5] or 0)	
	pet:UpdateData(DataIndex["skillid_6"], SkillsList[6] or 0)	
	pet:UpdateData(DataIndex["skillid_7"], SkillsList[7] or 0)	
	
	
	---第9个预留为宠物轮回技能指向
	pet:UpdateData(DataIndex["skillid_8"], FatalitySkillID or 0)	
	
	---第10个预留为宠物种族技能指向
	pet:UpdateData(DataIndex["skillid_9"], RaceAddSkillID or 0)	
	
	---第11个职业特性描述
	pet:UpdateData(DataIndex["skillid_10"], DescriptionID)	
	
	---第12个空白(用于测试)
	pet:UpdateData(DataIndex["skillid_11"], 0)	

	--[[	
	---宠物飞升扩充4个技能位	
	pet:UpdateData(DataIndex["skillid_12"], 0)	
	pet:UpdateData(DataIndex["skillid_13"], 0)	
	pet:UpdateData(DataIndex["skillid_14"], 0)	
	pet:UpdateData(DataIndex["skillid_15"], 0)	
	--]]
	pet:UpdateData(DataIndex["skilllvl_0"], 1)	
	pet:UpdateData(DataIndex["skilllvl_1"], SkillsLevel[1] or 0)	
	pet:UpdateData(DataIndex["skilllvl_2"], SkillsLevel[2] or 0)	
	pet:UpdateData(DataIndex["skilllvl_3"], SkillsLevel[3] or 0)	
	pet:UpdateData(DataIndex["skilllvl_4"], SkillsLevel[4] or 0)	
	pet:UpdateData(DataIndex["skilllvl_5"], SkillsLevel[5] or 0)	
	pet:UpdateData(DataIndex["skilllvl_6"], SkillsLevel[6] or 0)	
	pet:UpdateData(DataIndex["skilllvl_7"], SkillsLevel[7] or 0)	
	pet:UpdateData(DataIndex["skilllvl_8"], 1)	
	pet:UpdateData(DataIndex["skilllvl_9"], 1)	
	pet:UpdateData(DataIndex["skilllvl_10"], 1)	
	pet:UpdateData(DataIndex["skilllvl_11"], 0)	

	--[[
	pet:UpdateData(DataIndex["skilllvl_12"], 0)	
	pet:UpdateData(DataIndex["skilllvl_13"], 0)	
	pet:UpdateData(DataIndex["skilllvl_14"], 0)	
	pet:UpdateData(DataIndex["skilllvl_15"], 0)	
	--]]
	---生成宠物天赋相关信息
	pet:UpdateData(DataIndex["talent_1"], TalenetsList[1] or 0)	
	pet:UpdateData(DataIndex["talent_2"], TalenetsList[2] or 0)	
	pet:UpdateData(DataIndex["talent_3"], TalenetsList[3] or 0)	
	pet:UpdateData(DataIndex["talent_4"], TalenetsList[4] or 0)	
	pet:UpdateData(DataIndex["talent_5"], TalenetsList[5] or 0)	
	pet:UpdateData(DataIndex["talent_6"], TalenetsList[6] or 0)	
	pet:UpdateData(DataIndex["talent_7"], TalenetsList[7] or 0)	
	pet:UpdateData(DataIndex["talent_8"], TalenetsList[8] or 0)	
	pet:UpdateData(DataIndex["talent_9"], TalenetsList[9] or 0)	
	pet:UpdateData(DataIndex["talent_10"], TalenetsList[10] or 0)	
	pet:UpdateData(DataIndex["talent_11"], TalenetsList[11] or 0)	
	pet:UpdateData(DataIndex["talent_12"], TalenetsList[12] or 0)	
	pet:UpdateData(DataIndex["talent_13"], TalenetsList[13] or 0)
	pet:UpdateData(DataIndex["isspecial"], 0)
	

	---生成宠物生活相关信息
	pet:UpdateData(DataIndex["age"], LivesList[1] or 0)	
	pet:UpdateData(DataIndex["life"], LivesList[2] or 0)	
	pet:UpdateData(DataIndex["maxhunger"],LivesList[3] or 0)	
	pet:UpdateData(DataIndex["curhunger"],LivesList[4] or 0)	
	pet:UpdateData(DataIndex["maxhonor"],LivesList[5] or 0)	
	pet:UpdateData(DataIndex["curhonor"],LivesList[6] or 0)
	pet:UpdateData(DataIndex["acuity"], LivesList[7] or 0)	
	pet:UpdateData(DataIndex["stamina"], LivesList[8] or 0)	
	pet:UpdateData(DataIndex["sawy"], LivesList[9] or 0)	
	pet:UpdateData(DataIndex["dowry"], LivesList[10] or 0)	
	pet:UpdateData(DataIndex["wisdom"], LivesList[11] or 0)	
	pet:UpdateData(DataIndex["cnr"], LivesList[12] or 0)	
	
	
	pet:SetAge(LivesList[1] or 0)
	pet:SetLife(LivesList[2] or 0)
	pet:SetMaxHungerPoint(LivesList[3] or 0)
	pet:SetCurHungerPoint(LivesList[4] or 0)
	pet:SetMaxHonorPoint(LivesList[5] or 0)
	pet:SetCurHonorPoint(LivesList[6] or 0)
	
	--[[
	pet:SetAcuity(LivesList[6] or 0)
	pet:SetStamina(LivesList[7] or 0)
	pet:SetSawy(LivesList[8] or 0)
	pet:SetAcuity(LivesList[9] or 0)
	pet:SetDowry(LivesList[10] or 0)
	pet:SetWisdom(LivesList[11] or 0)
	pet:SetCNR(LivesList[12] or 0)
 	--]]
 	
 	---其它值
 	pet:UpdateData(DataIndex["face"], 0)	
 	pet:UpdateData(DataIndex["shape"],1)	
 	pet:UpdateData(DataIndex["isgrowup"],0)					---//宠物升级会改变形象(美工专用每10级会改变一次,0为不会)	
 	pet:UpdateData(DataIndex["isaberrant"],0)				---//宠物是否为变异宠物
	
	pet:SetShape(1)
	pet:SetFace(0)
	
	pet:UpdateData(DataIndex["feedtimes"], 0)	
 	pet:UpdateData(DataIndex["feedaids"], 0)
 	pet:UpdateData(DataIndex["fatalityinchp"], 1)
  pet:UpdateData(DataIndex["fatalityincmp"], 1)
  pet:UpdateData(DataIndex["fatalityincminatk"], 1)
  pet:UpdateData(DataIndex["fatalityincmaxatk"], 1)
  pet:UpdateData(DataIndex["fatalityincdef"], 1)  
  pet:UpdateData(DataIndex["fatalityaddatkrate"], 0)
  pet:UpdateData(DataIndex["fatalityadddodge"], 0)
  pet:UpdateData(DataIndex["fatalityincres1"], 1)
  pet:UpdateData(DataIndex["fatalityincres2"], 1)
  pet:UpdateData(DataIndex["fatalityincres3"], 1)
  pet:UpdateData(DataIndex["fatalityincres4"], 1)
  pet:UpdateData(DataIndex["fatalityincres5"], 1)
  pet:UpdateData(DataIndex["fatalityincres6"], 1)
  pet:UpdateData(DataIndex["fatalityaddcrit"], 1)
  pet:UpdateData(DataIndex["fatalityaddcritdmg"], 0)
  pet:UpdateData(DataIndex["fatalityaddacuity"], 0)
  pet:UpdateData(DataIndex["fatalityaddstamina"], 0)
  pet:UpdateData(DataIndex["fatalityaddsawy"], 0)
  pet:UpdateData(DataIndex["fatalityadddowry"], 0)
  pet:UpdateData(DataIndex["fatalityaddwisdom"], 0)
  pet:UpdateData(DataIndex["fatalityaddcnr"], 0)
  
  ---其他备用值
  pet:UpdateData(DataIndex["blank"], 0)
  pet:UpdateData(DataIndex["smallgrow"], 0)
  pet:UpdateData(DataIndex["reserve1"], 0)	---20090303用于限制宠物修炼技能格次数
  pet:UpdateData(DataIndex["reserve2"], 0)
  pet:UpdateData(DataIndex["reserve3"], 0)
  pet:UpdateData(DataIndex["reserve4"], 0)
  pet:UpdateData(DataIndex["reserve5"], 0)
  pet:UpdateData(DataIndex["reserve6"], 0)
  pet:UpdateData(DataIndex["reserve7"], 0)
  pet:UpdateData(DataIndex["reserve8"], 0)
  pet:UpdateData(DataIndex["reserve9"], 0)

	---设置飞升属性
	local p_reborncnt = 0
 	for i,v in pairs(availablePets) do
		if v.pet_tid==pet_tid and v.potential_reborncnt == 1 then p_reborncnt = 1 end
	end	
	
	pet:SetRebornCnt(math.max(pet:QueryRebornCnt(),p_reborncnt))
 	for i=0,13 do
 		pet:SetRebornPropAdd(i,pet:QueryRebornPropAdd(i))
 	end

	
 	---//特殊宠物 主角系列和傀儡系列等级预处理和驯养预处理
 	local templevel=1
 	for i,v in pairs(availablePets) do
		if v.pet_tid==pet_tid and v.born_special ~= 0 and type(v.born_special)=="number" then 
 			if type(master_level)=="number" and master_level>=1 and master_level<=160 then 
 				templevel=math.min(master_level+v.born_special,160)
			end
		pet:SetLevel(templevel)
 		PetBedgeAdopt(pet_tid,pet,origin) 
		end
	end

		ZLuaPetBedgeRebuild(pet_tid,pet,origin,master_level,_)
 	---HuangXin要求的return项(固定)
 	return pet_tid
 end
-------------------------------------------------------------------------------------------------------------
---//程序接口
function PetBedgeRebuild(pet_tid,pet,origin,master_level,cult)
	ZLuaPetBedgeRebuild(pet_tid,pet,origin,master_level,cult)
	---//cult为玩家造化，1，仙；2，魔；4，佛，用来消除佛技能中舍利对于非佛宠物的影响
	return pet_tid;
end 
-------------------------------------------------------------------------------------------------------------
--------宠物属性重新设置和计算--------------------------------------------------------------------------------
function ZLuaPetBedgeRebuild(pet_tid,pet,origin,master_level,cult)
	
	
	local DataIndex=ZLuaPetBedgeDataIndex()
	local SpecialIndex=ZLuaPetBedgeSpecialIndex()
	local availablePets = ZLuaAvailablePets()
	local Race=ZLuaPetBedgeRaceDesc()
	
	local level=pet:QueryLevel()
	
	---//特殊宠物 主角系列和傀儡系列等级预处理	
	for i,v in pairs(availablePets) do
		if v.pet_tid==pet_tid and v.born_special ~= 0 and type(v.born_special)=="number" then 
 			if type(master_level)=="number" and master_level>=1 and master_level<=160 then 
 				level=math.max(math.min(master_level+v.born_special,160),1)
			end
			pet:SetLevel(level)
		end
	end

		---纠正2009年台湾版狐狸宠物星等异常的Bug---起始---------------------------------	
		if pet_tid==29992 and pet:QueryData(DataIndex["id"])==29992 then
			---检查是否异常宠物
				pet:UpdateData(DataIndex["id"], 33146)
			---修正id后重新初始化	
		 		PetBedgeInit(33146,pet,origin,master_level)
				pet:UpdateData(DataIndex["identify"], 1)
		end
	
		pet:SetStar(pet:QueryData(DataIndex["star"]))
		pet:SetMainType(pet:QueryData(DataIndex["maintype"]))
		
		pet:SetElement(pet:QueryData(DataIndex["element"]))
		pet:SetIdentify(pet:QueryData(DataIndex["identify"]))
		pet:SetRace(pet:QueryData(DataIndex["race"]))
		pet:SetHoroscope(pet:QueryData(DataIndex["horoscope"]))
		
		pet:SetCombatAttrInit(SpecialIndex["hp"],pet:QueryData(DataIndex["default_hp"]))
		pet:SetCombatAttrInit(SpecialIndex["mp"],pet:QueryData(DataIndex["default_mp"]))
		pet:SetCombatAttrInit(SpecialIndex["minatk"],pet:QueryData(DataIndex["default_minatk"]))
		pet:SetCombatAttrInit(SpecialIndex["maxatk"],pet:QueryData(DataIndex["default_maxatk"]))
		pet:SetCombatAttrInit(SpecialIndex["defence"],pet:QueryData(DataIndex["default_def"]))
		pet:SetCombatAttrInit(SpecialIndex["atkrate"],pet:QueryData(DataIndex["default_attackrate"]))
		pet:SetCombatAttrInit(SpecialIndex["dodge"],pet:QueryData(DataIndex["default_dodge"]))
		pet:SetCombatAttrInit(SpecialIndex["res1"],pet:QueryData(DataIndex["default_resist1"]))
		pet:SetCombatAttrInit(SpecialIndex["res2"],pet:QueryData(DataIndex["default_resist2"]))
		pet:SetCombatAttrInit(SpecialIndex["res3"],pet:QueryData(DataIndex["default_resist3"]))
		pet:SetCombatAttrInit(SpecialIndex["res4"],pet:QueryData(DataIndex["default_resist4"]))
		pet:SetCombatAttrInit(SpecialIndex["res5"],pet:QueryData(DataIndex["default_resist5"]))
		pet:SetCombatAttrInit(SpecialIndex["res6"],pet:QueryData(DataIndex["default_resist6"]))
		
		pet:SetCombatAttrPotential(SpecialIndex["hp"],pet:QueryData(DataIndex["potential_hp"]))
		pet:SetCombatAttrPotential(SpecialIndex["mp"],pet:QueryData(DataIndex["potential_mp"]))
		pet:SetCombatAttrPotential(SpecialIndex["minatk"],pet:QueryData(DataIndex["potential_minatk"]))
		pet:SetCombatAttrPotential(SpecialIndex["maxatk"],pet:QueryData(DataIndex["potential_maxatk"]))
		pet:SetCombatAttrPotential(SpecialIndex["defence"],pet:QueryData(DataIndex["potential_def"]))
		pet:SetCombatAttrPotential(SpecialIndex["atkrate"],pet:QueryData(DataIndex["potential_attackrate"]))
		pet:SetCombatAttrPotential(SpecialIndex["dodge"],pet:QueryData(DataIndex["potential_dodge"]))
		pet:SetCombatAttrPotential(SpecialIndex["res1"],pet:QueryData(DataIndex["potential_resist1"]))
		pet:SetCombatAttrPotential(SpecialIndex["res2"],pet:QueryData(DataIndex["potential_resist2"]))
		pet:SetCombatAttrPotential(SpecialIndex["res3"],pet:QueryData(DataIndex["potential_resist3"]))
		pet:SetCombatAttrPotential(SpecialIndex["res4"],pet:QueryData(DataIndex["potential_resist4"]))
		pet:SetCombatAttrPotential(SpecialIndex["res5"],pet:QueryData(DataIndex["potential_resist5"]))
		pet:SetCombatAttrPotential(SpecialIndex["res6"],pet:QueryData(DataIndex["potential_resist6"]))

	
		pet:SetMaxHungerPoint(500+(level-1)*90)
		pet:SetMaxHonorPoint(pet:QueryMaxHonorPoint())
		pet:SetCurHungerPoint(pet:QueryCurHungerPoint())
		pet:SetCurHonorPoint(pet:QueryCurHonorPoint())
		pet:SetAge(pet:QueryAge())
		pet:SetLife(pet:QueryLife())
		
		pet:UpdateData(DataIndex["curhunger"],pet:QueryCurHungerPoint())
		pet:UpdateData(DataIndex["maxhunger"],pet:QueryMaxHungerPoint())
		pet:UpdateData(DataIndex["curhonor"],pet:QueryCurHonorPoint())
		pet:UpdateData(DataIndex["maxhunger"],500+(level-1)*30)
		pet:UpdateData(DataIndex["age"],pet:QueryAge())
		pet:UpdateData(DataIndex["life"],pet:QueryLife())
	
		---//处理非佛舍利消除
		if cult~=4 and type(cult)=="number" then 
			ZLuaPetBedgeRollBack(pet_tid,pet)
		end  
		
		---//处理宠物天命
	  local pea_atk,pea_def,pea_crithurt=0,0,0
	  pea_atk,pea_def,pea_crithurt=ZLuaPetBedgeFatalityEffect(pet_tid,pet,origin)
	  
	 		---//处理宠物多阶种族技能
	  local spirit_crit=0
	  spirit_crit=ZLuaPetBedgeSpiritEffect(pet_tid,pet,origin)
		
		pet:SetSkillID(0,pet:QueryData(DataIndex["skillid_0"]))
		pet:SetSkillID(1,pet:QueryData(DataIndex["skillid_1"]))
		pet:SetSkillID(2,pet:QueryData(DataIndex["skillid_2"]))
		pet:SetSkillID(3,pet:QueryData(DataIndex["skillid_3"]))
		pet:SetSkillID(4,pet:QueryData(DataIndex["skillid_4"]))
		pet:SetSkillID(5,pet:QueryData(DataIndex["skillid_5"]))
		pet:SetSkillID(6,pet:QueryData(DataIndex["skillid_6"]))
		pet:SetSkillID(7,pet:QueryData(DataIndex["skillid_7"]))
		pet:SetSkillID(8,pet:QueryData(DataIndex["skillid_8"]))
		pet:SetSkillID(9,pet:QueryData(DataIndex["skillid_9"]))
		pet:SetSkillID(10,pet:QueryData(DataIndex["skillid_10"]))
		pet:SetSkillID(11,pet:QueryData(DataIndex["skillid_11"]))
	--[[
		pet:SetSkillID(12,pet:QueryData(DataIndex["skillid_12"]))
		pet:SetSkillID(13,pet:QueryData(DataIndex["skillid_13"]))
		pet:SetSkillID(14,pet:QueryData(DataIndex["skillid_14"]))
		pet:SetSkillID(15,pet:QueryData(DataIndex["skillid_15"]))
	--]]
		
		pet:SetTalent(0,pet:QueryData(DataIndex["talent_1"]))
		pet:SetTalent(1,pet:QueryData(DataIndex["talent_2"]))
		pet:SetTalent(2,pet:QueryData(DataIndex["talent_3"]))
		pet:SetTalent(3,pet:QueryData(DataIndex["talent_4"]))
		pet:SetTalent(4,pet:QueryData(DataIndex["talent_5"]))
		pet:SetTalent(5,pet:QueryData(DataIndex["talent_6"]))
		pet:SetTalent(6,pet:QueryData(DataIndex["talent_7"]))
		pet:SetTalent(7,pet:QueryData(DataIndex["talent_8"]))
		pet:SetTalent(8,pet:QueryData(DataIndex["talent_9"]))
		pet:SetTalent(9,pet:QueryData(DataIndex["talent_10"]))
		pet:SetTalent(10,pet:QueryData(DataIndex["talent_11"]))
		pet:SetTalent(11,pet:QueryData(DataIndex["talent_12"]))
		pet:SetTalent(12,pet:QueryData(DataIndex["talent_13"]))

		local talentslist={
		pet:QueryData(DataIndex["talent_1"]),pet:QueryData(DataIndex["talent_2"]),
		pet:QueryData(DataIndex["talent_3"]),pet:QueryData(DataIndex["talent_4"]),
		pet:QueryData(DataIndex["talent_5"]),pet:QueryData(DataIndex["talent_6"]),
		pet:QueryData(DataIndex["talent_7"]),pet:QueryData(DataIndex["talent_8"]),
		pet:QueryData(DataIndex["talent_9"]),pet:QueryData(DataIndex["talent_10"]),
		pet:QueryData(DataIndex["talent_11"]),pet:QueryData(DataIndex["talent_12"]),
		pet:QueryData(DataIndex["talent_13"])}
		
		local skillslistex={
		pet:QueryData(DataIndex["skillid_1"]),pet:QueryData(DataIndex["skillid_2"]),
		pet:QueryData(DataIndex["skillid_3"]),pet:QueryData(DataIndex["skillid_4"]),
		pet:QueryData(DataIndex["skillid_5"]),pet:QueryData(DataIndex["skillid_6"]),
		pet:QueryData(DataIndex["skillid_7"])}
		---//计算特质的技能不包含第1个技能和最后4个技能
		
		pet:SetSkillLevel(0,1+math.floor(level/75)+math.floor(level/160))
		

		---//技能等级受到特质影响
		pet:SetSkillLevel(1,ZLuaPetBedgeSkillBenefitByTalenet(skillslistex[1],talentslist,level)+pet:QueryData(DataIndex["skilllvl_1"]))
		pet:SetSkillLevel(2,ZLuaPetBedgeSkillBenefitByTalenet(skillslistex[2],talentslist,level)+pet:QueryData(DataIndex["skilllvl_2"]))
		pet:SetSkillLevel(3,ZLuaPetBedgeSkillBenefitByTalenet(skillslistex[3],talentslist,level)+pet:QueryData(DataIndex["skilllvl_3"]))
		pet:SetSkillLevel(4,ZLuaPetBedgeSkillBenefitByTalenet(skillslistex[4],talentslist,level)+pet:QueryData(DataIndex["skilllvl_4"]))
		pet:SetSkillLevel(5,ZLuaPetBedgeSkillBenefitByTalenet(skillslistex[5],talentslist,level)+pet:QueryData(DataIndex["skilllvl_5"]))
		pet:SetSkillLevel(6,ZLuaPetBedgeSkillBenefitByTalenet(skillslistex[6],talentslist,level)+pet:QueryData(DataIndex["skilllvl_6"]))
		pet:SetSkillLevel(7,ZLuaPetBedgeSkillBenefitByTalenet(skillslistex[7],talentslist,level)+pet:QueryData(DataIndex["skilllvl_7"]))
	
		pet:SetSkillLevel(8,pet:QueryData(DataIndex["skilllvl_8"]))
		pet:SetSkillLevel(9,pet:QueryData(DataIndex["skilllvl_9"]))			
		pet:SetSkillLevel(10,pet:QueryData(DataIndex["skilllvl_10"]))
		pet:SetSkillLevel(11,pet:QueryData(DataIndex["skilllvl_11"]))
			
		local Star=pet:QueryData(DataIndex["star"])
		local IsGrowUp=pet:QueryData(DataIndex["isgrowup"])
		local IsAberrant=pet:QueryData(DataIndex["isaberrant"])
		
		---//星等改变/变异导致的形象变化
		local FaceFromShape={}
		
			FaceFromShape[1]=0
			FaceFromShape[2]=1
			FaceFromShape[3]=2
			FaceFromShape[4]=3
			FaceFromShape[5]=4
			FaceFromShape[6]=5
			FaceFromShape[7]=6
			
		---//处理宠物虽星等成长改变形象和头像的情况
		if IsGrowUp==1 then 	
			pet:SetShape(math.max(math.min(math.floor(level/10)+1,7),1))
			pet:SetFace(FaceFromShape[math.max(math.min(math.floor(level/10)+1,7),1)])
		else
			pet:SetShape(math.max(pet:QueryData(DataIndex["shape"]),1))
			pet:SetFace(math.max(FaceFromShape[pet:QueryData(DataIndex["shape"])],0))
		end 
		
		---处理宠物被动技能
		local Race=ZLuaPetBedgeRaceDesc()
		local race=pet:QueryData(DataIndex["race"])
		
		local PseudoPassiveIncHpGrow=1
	
		if race==Race["renzu"] then 
		elseif race==Race["yaozu"] then 
		elseif race==Race["guizu"] then 
			PseudoPassiveIncHpGrow=1.05
		elseif race==Race["yuzu"] then 
		elseif race==Race["meizu"] then 
		elseif race==Race["tianjiang"] then 
		elseif race==Race["xianling"] then 
		elseif race==Race["dijing"] then 
		elseif race==Race["yeshou"] then 
		elseif race==Race["yishou"] then 
		elseif race==Race["shuizu"] then 
		elseif race==Race["chonggu"] then 
		elseif race==Race["huanling"] then 
		elseif race==Race["siling"] then 
		elseif race==Race["yuansu"] then 
		elseif race==Race["teshu"] then 
		else
		end 
		--[[ 种族特性描述
		---生成宠物种族收益影响(PseudoPassiveSkills)
		---人族	语言交流	修炼的成功率增加3％。								1
		---妖族	妖类直觉	昏睡抗性增加15点。									1
		---鬼族	不灭生命	从生命资质获得的成长增加5％					1
		---羽族	鹰眼洞察	命中增加15点。											1
		---魅族	魅者之心	魅惑抗性增加15点。									1
		---天将	自由之戈	定身抗性增加15点。									1
		---仙灵	巧夺天工	所有生活能力值增加5点。							1
		---山精	光合作用	气血回复速度加倍。									1
		---野兽	野性力量	致命一击伤害增加10％。							1
		---异兽	壁垒贯注	受到伤害减少10。										1
		---水族	浩瀚乃大	被冰冻的概率下降。									1
		---虫蛊	蝼蚁之力	对普通目标至少造成20点伤害。				1
		---幻灵	虚空汲取	精力回复速度加倍。									1
		---死灵	死亡意志	眩晕抗性增加15点。									1
		---元素	命运藐视	虚弱抗性增加15点。									1
		---特殊	天生幸运	躲闪增加15点。											1
		---人族/妖族/鬼族/羽族/魅族/天将/仙灵/地精/
			野兽/异兽/水族/虫蛊/幻灵/死灵/元素/特殊
		--]]
		
		
		---纠正2009年国庆期间抓到的判官种族不正确的Bug---起始---------------------------------	
		if pet_tid==30989 and race~=Race["siling"] then
			---纠正种族
				pet:UpdateData(DataIndex["race"], Race["siling"])
			---死灵初始眩晕抗性16点	
				pet:UpdateData(DataIndex["default_resist1"], 16)
			---死灵种族技能1249
				pet:UpdateData(DataIndex["skillid_9"], 1249)
				pet:UpdateData(DataIndex["skilllvl_9"], 1)
				local LivesList={}
				LivesList=ZLuaPetBedgeLives(pet_tid,serverorigin)
				pet:UpdateData(DataIndex["acuity"], LivesList[7] or 0)	
				pet:UpdateData(DataIndex["stamina"], LivesList[8] or 0)	
				pet:UpdateData(DataIndex["sawy"], LivesList[9] or 0)	
				pet:UpdateData(DataIndex["dowry"], LivesList[10] or 0)	
				pet:UpdateData(DataIndex["wisdom"], LivesList[11] or 0)	
				pet:UpdateData(DataIndex["cnr"], LivesList[12] or 0)
		end
		---纠正2009年国庆期间抓到的判官种族不正确的Bug---完结---------------------------------		
		
					
	
		---//属性设置
		pet:SetCombatAttrCur(SpecialIndex["hp"],
		math.min(math.floor(pet:QueryData(DataIndex["default_hp"])+pet:QueryRebornPropAdd(0)+(-0.21*(level-1)*(level-1)+102.17*(level-1))
		*PseudoPassiveIncHpGrow
		*pet:QueryData(DataIndex["fatalityinchp"])
		*ZLuaPetTrueGrowthFromGrades(pet:QueryData(DataIndex["potential_hp"]),SpecialIndex["hp"])),640000)
		)
			
		pet:SetCombatAttrCur(SpecialIndex["mp"],
		math.min(math.floor(pet:QueryData(DataIndex["default_mp"])+pet:QueryRebornPropAdd(1)+(-0.168*(level-1)*(level-1)+81.74*(level-1))
		*pet:QueryData(DataIndex["fatalityincmp"])
		*ZLuaPetTrueGrowthFromGrades(pet:QueryData(DataIndex["potential_mp"]),SpecialIndex["mp"])),640000)
		)
		
		pet:SetCombatAttrCur(SpecialIndex["minatk"],
		math.min(math.floor(pet:QueryData(DataIndex["default_minatk"])+pet:QueryRebornPropAdd(2)+(0.1023*(level-1)*(level-1)+1.326*(level-1))
		*pet:QueryData(DataIndex["fatalityincminatk"])
		*ZLuaPetTrueGrowthFromGrades(pet:QueryData(DataIndex["potential_minatk"]),SpecialIndex["minatk"])),64000)
		)
		
		pet:SetCombatAttrCur(SpecialIndex["maxatk"],
		math.min(math.floor(pet:QueryData(DataIndex["default_maxatk"])+pet:QueryRebornPropAdd(3)+(0.1023*(level-1)*(level-1)+1.326*(level-1))
		*pet:QueryData(DataIndex["fatalityincmaxatk"])
		*ZLuaPetTrueGrowthFromGrades(pet:QueryData(DataIndex["potential_maxatk"]),SpecialIndex["maxatk"]))+pea_atk,64000)
		)
		
		---如果飞升属性小攻超过大攻，暂时置小攻为大攻
		if pet:QueryCombatAttrCur(SpecialIndex["minatk"])>pet:QueryCombatAttrCur(SpecialIndex["maxatk"]) then
			pet:SetCombatAttrCur(SpecialIndex["minatk"],pet:QueryCombatAttrCur(SpecialIndex["maxatk"]))
		end
		
		pet:SetCombatAttrCur(SpecialIndex["defence"],
		math.min(math.floor(pet:QueryData(DataIndex["default_def"])+pet:QueryRebornPropAdd(4)+(0.04121*(level-1)*(level-1)+0.2*(level-1))
		*pet:QueryData(DataIndex["fatalityincdef"])
		*ZLuaPetTrueGrowthFromGrades(pet:QueryData(DataIndex["potential_def"]),SpecialIndex["defence"]))+pea_def,64000)
		)
		
		pet:SetCombatAttrCur(SpecialIndex["atkrate"],
		math.min(math.floor(pet:QueryData(DataIndex["default_attackrate"])+pet:QueryRebornPropAdd(5)+pet:QueryData(DataIndex["fatalityaddatkrate"])
		+(level-1)*ZLuaPetTrueGrowthFromGrades(pet:QueryData(DataIndex["potential_attackrate"]),SpecialIndex["atkrate"])),64000)
		)
		
		pet:SetCombatAttrCur(SpecialIndex["dodge"],
		math.min(math.floor(pet:QueryData(DataIndex["default_dodge"])+pet:QueryRebornPropAdd(6)+pet:QueryData(DataIndex["fatalityadddodge"])
		+(level-1)*ZLuaPetTrueGrowthFromGrades(pet:QueryData(DataIndex["potential_dodge"]),SpecialIndex["dodge"])),64000)
		)
		
		pet:SetCombatAttrCur(SpecialIndex["res1"],math.floor(pet:QueryData(DataIndex["default_resist1"])+pet:QueryRebornPropAdd(9)+
		(level-1)*pet:QueryData(DataIndex["fatalityincres1"])*
		ZLuaPetTrueGrowthFromGrades(pet:QueryData(DataIndex["potential_resist1"]),SpecialIndex["res1"])))
		
		pet:SetCombatAttrCur(SpecialIndex["res2"],math.floor(pet:QueryData(DataIndex["default_resist2"])+pet:QueryRebornPropAdd(10)+
		(level-1)*pet:QueryData(DataIndex["fatalityincres2"])*
		ZLuaPetTrueGrowthFromGrades(pet:QueryData(DataIndex["potential_resist2"]),SpecialIndex["res2"])))
		
		pet:SetCombatAttrCur(SpecialIndex["res3"],math.floor(pet:QueryData(DataIndex["default_resist3"])+pet:QueryRebornPropAdd(11)+
		(level-1)*pet:QueryData(DataIndex["fatalityincres3"])*
		ZLuaPetTrueGrowthFromGrades(pet:QueryData(DataIndex["potential_resist3"]),SpecialIndex["res3"])))
		
		pet:SetCombatAttrCur(SpecialIndex["res4"],math.floor(pet:QueryData(DataIndex["default_resist4"])+pet:QueryRebornPropAdd(12)+
		(level-1)*pet:QueryData(DataIndex["fatalityincres4"])*
		ZLuaPetTrueGrowthFromGrades(pet:QueryData(DataIndex["potential_resist4"]),SpecialIndex["res4"])))
		
		pet:SetCombatAttrCur(SpecialIndex["res5"],math.floor(pet:QueryData(DataIndex["default_resist5"])+pet:QueryRebornPropAdd(13)+
		(level-1)*pet:QueryData(DataIndex["fatalityincres5"])*
		ZLuaPetTrueGrowthFromGrades(pet:QueryData(DataIndex["potential_resist5"]),SpecialIndex["res5"])))
		
		pet:SetCombatAttrCur(SpecialIndex["res6"],math.floor(pet:QueryData(DataIndex["default_resist6"])+
		(level-1)*pet:QueryData(DataIndex["fatalityincres6"])*
		ZLuaPetTrueGrowthFromGrades(pet:QueryData(DataIndex["potential_resist6"]),SpecialIndex["res6"])))

		local critfromhonor,critdmgfromhonor=0,0
		critfromhonor,critdmgfromhonor=ZLuaPetBedgeCritBenefitByHonor(pet:QueryData(DataIndex["curhonor"]))
		
		pet:SetExtraDamage(pet:QueryData(DataIndex["extradamage"]))
		pet:SetExtraDamageReduce(pet:QueryData(DataIndex["extradamagereduce"]))
		pet:SetCritRate(pet:QueryData(DataIndex["nativecritrate"])+critfromhonor+pet:QueryData(DataIndex["fatalityaddcrit"])+0.01*spirit_crit+0.001*(pet:QueryRebornPropAdd(7)))---宠物飞升增加暴击率，千分制
		pet:SetCritDamage(pet:QueryData(DataIndex["nativecritdamage"])+critdmgfromhonor+pet:QueryData(DataIndex["fatalityaddcritdmg"])+0.08*pea_crithurt+0.001*(pet:QueryRebornPropAdd(8)))
		
		pet:SetAcuity(pet:QueryData(DataIndex["acuity"])+pet:QueryData(DataIndex["fatalityaddacuity"]))
		pet:SetStamina(pet:QueryData(DataIndex["stamina"])+pet:QueryData(DataIndex["fatalityaddstamina"]))
		pet:SetSawy(pet:QueryData(DataIndex["sawy"])+pet:QueryData(DataIndex["fatalityaddsawy"]))
		pet:SetDowry(pet:QueryData(DataIndex["dowry"])+pet:QueryData(DataIndex["fatalityadddowry"]))
		pet:SetWisdom(pet:QueryData(DataIndex["wisdom"])+pet:QueryData(DataIndex["fatalityaddwisdom"]))
		pet:SetCNR(pet:QueryData(DataIndex["cnr"])+pet:QueryData(DataIndex["fatalityaddcnr"]))
		
		if level==1 then
			pet:SetCurHP(pet:QueryCombatAttrCur(SpecialIndex["hp"]))
  		pet:SetCurVigor(pet:QueryCombatAttrCur(SpecialIndex["mp"]))
  	end
 		
 		---飞升相关属性设置
 		local RebornAttrDataIndex=ZLuaPetBedgeRebornAttrDataIndex()
   	local rebornAttrLevel={}
  	rebornAttrLevel[0]					=	math.floor(pet:QueryRebornPropAdd(0)/6000)
  	rebornAttrLevel[1]					=	math.floor(pet:QueryRebornPropAdd(1)/6000)
  	rebornAttrLevel[2]					=	math.floor(pet:QueryRebornPropAdd(2)/500) 
  	rebornAttrLevel[3]					=	math.floor(pet:QueryRebornPropAdd(3)/600) 
  	rebornAttrLevel[4]					=	math.floor(pet:QueryRebornPropAdd(4)/200)
  	rebornAttrLevel[5]					=	math.floor(pet:QueryRebornPropAdd(5)/50) 
  	rebornAttrLevel[6]					=	math.floor(pet:QueryRebornPropAdd(6)/50) 
  	rebornAttrLevel[7]					=	math.floor(pet:QueryRebornPropAdd(7)/60) 
  	rebornAttrLevel[8]					=	math.floor(pet:QueryRebornPropAdd(8)/180) 
  	rebornAttrLevel[9]					=	math.floor(pet:QueryRebornPropAdd(9)/50) 
  	rebornAttrLevel[10]					=	math.floor(pet:QueryRebornPropAdd(10)/50) 
  	rebornAttrLevel[11]					=	math.floor(pet:QueryRebornPropAdd(11)/50) 
  	rebornAttrLevel[12]					=	math.floor(pet:QueryRebornPropAdd(12)/50) 
  	rebornAttrLevel[13]					=	math.floor(pet:QueryRebornPropAdd(13)/50) 

		local rebornlevel = 0
		for i = 1,14 do
			rebornlevel = rebornlevel + rebornAttrLevel[i-1]
		end
		pet:SetRebornStar(math.floor(rebornlevel/14))
			
		---飞升后每次rebuild都要根据星等/附加星等重新设置宠物隐藏被动属性增益
		---技能命中-技能闪躲-减暴击-减暴击伤害-减免伤害-减免伤害百分比
		--[[
		rebornAttrLevel[15]					=math.floor(pet:QuerySkillAttackRate()/17)
		rebornAttrLevel[16]					=math.floor(pet:QuerySkillArmorRate()/6)
		rebornAttrLevel[17]					=math.floor(pet:QueryAntiCritRate()/6)
		rebornAttrLevel[18]					=math.floor(pet:QueryAntiCritDamage()/28)
		rebornAttrLevel[19]					=math.floor(pet:QueryDamageReduce()/133)
		rebornAttrLevel[20]					=math.floor(pet:QueryDamageReducePercent()/2)
		--]]
		pet:SetSkillAttackRate(0.085*pet:QueryStar())
		pet:SetSkillArmorRate(0.03*pet:QueryStar())
		pet:SetAntiCritRate(0.03*pet:QueryStar())
		pet:SetAntiCritDamage(0.14*pet:QueryStar())
		pet:SetDamageReduce(66.5*pet:QueryStar())
		pet:SetDamageReducePercent(0.01*pet:QueryStar())
		pet:SetRebornCnt(pet:QueryRebornCnt())
	 	for i=0,13 do
	 		pet:SetRebornPropAdd(i,pet:QueryRebornPropAdd(i))
	 	end
		
end 
---宠物升级-------------------------------------------------------------------------------
function PetBedgeLevelUp(pet_tid,pet,origin)
		ZLuaPetBedgeRebuild(pet_tid,pet,origin,_,_)
		local SpecialIndex=ZLuaPetBedgeSpecialIndex()
		pet:SetCurHP(pet:QueryCombatAttrCur(SpecialIndex["hp"]))
  	---pet:SetCurVigor(pet:QueryCombatAttrCur(SpecialIndex["mp"]))
		return pet_tid;
end
---宠物驯养-------------------------------------------------------------------------------
function PetBedgeAdopt(pet_tid,pet,origin)
		local DataIndex=ZLuaPetBedgeDataIndex()
		local availablePets = ZLuaAvailablePets()
			pet:SetLevel(1)
			pet:SetExp(0)
		 	pet:UpdateData(DataIndex["identify"], 1)
		 if pet:QueryRebornCnt()==0 then
		 	pet:SetCurHonorPoint(math.max(pet:QueryCurHonorPoint()/2,1))
	 		pet:SetMaxHonorPoint(math.max(pet:QueryMaxHonorPoint()/2,80))
	 		pet:SetCurHungerPoint(450)
	 		pet:SetMaxHungerPoint(500)
	 		ZLuaPetBedgeRollBack(pet_tid,pet)
	 	 else
	 	 end
	 		pet:UpdateData(DataIndex["feedaids"],0)
	 		ZLuaPetBedgeRebuild(pet_tid,pet,origin,_,_)
			return pet_tid;
end 
---宠物死亡-------------------------------------------------------------------------------
function PetBedgeDead(pet_tid,pet,dead_type)
  ---//dead_type  0:被怪杀死	1:被人或者被人的宠物杀死  
  if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then 
  	return 0 
  end
	
	if dead_type==0 then 
		local PetLife=pet:QueryLife()
		local	PetStar=pet:QueryStar()
		local PetCurHonor=pet:QueryCurHonorPoint()
		local PetMaxHonor=pet:QueryMaxHonorPoint()
		
		local NewPetLife,NewPetCurHonor,NewPetMaxHonor
		local DataIndex=ZLuaPetBedgeDataIndex()
		local petlevel=pet:QueryLevel()
		local fatalityskillid=pet:QueryData(DataIndex["skillid_8"])
		
		local lifereduce=1
		if petlevel>=60 then  
			lifereduce=math.floor(petlevel/3)+1
		end 
		
		if petlevel>=60 and fatalityskillid==1398 and PetStar>=8 then 
			lifereduce=math.floor(0.75*petlevel/3)+1
		end 

		if fatalityskillid==1397 and PetStar>=8 then 
			NewPetLife=math.max(PetLife-lifereduce,pet:QueryAge())
			NewPetCurHonor=math.max(math.floor(PetCurHonor*0.925),1)
			NewPetMaxHonor=math.min(math.max(math.ceil(PetMaxHonor*1),NewPetCurHonor,80),57600)				
		else 
			NewPetLife=math.max(PetLife-lifereduce,pet:QueryAge())
			NewPetCurHonor=math.max(math.floor(PetCurHonor*0.9),1)
			NewPetMaxHonor=math.min(math.max(math.ceil(PetMaxHonor*1),NewPetCurHonor,80),57600)
		end 
		
		pet:SetLife(NewPetLife)
		pet:SetCurHonorPoint(NewPetCurHonor)
	  pet:SetMaxHonorPoint(NewPetMaxHonor)

   ---//处理舍利属性死亡后数据rollback的问题	
	  ZLuaPetBedgeRollBack(pet_tid,pet)

    local origin=pet:QueryData(DataIndex["nativeorigin"])
	  ZLuaPetBedgeRebuild(pet_tid,pet,origin,_,_)
 
 	else 	
		---//被人和人的宠物击杀无损失
	end 
  pet:SetCurHP(2*pet:QueryLevel())
  pet:SetCurVigor(5)
  return pet_tid;
end

---//处理宠物轮回/豆神修炼/舍利修炼的影响
function ZLuaPetBedgeFatalityEffect(pet_tid,pet,origin)
		
		local DataIndex=ZLuaPetBedgeDataIndex()
		local level=pet:QueryLevel()	 	
		local star=pet:QueryStar()
		---//轮回影响
		local fatalityskill=pet:QueryData(DataIndex["skillid_8"])
	
		---//舍利修炼
		local buddhaskillid=pet:QueryData(DataIndex["skillid_10"])
		local buddhaskilllvl=pet:QueryData(DataIndex["skilllvl_10"])
	


		---//豆神修炼
		local peaskillid=pet:QueryData(DataIndex["skillid_11"])
		local peaskilllvl=pet:QueryData(DataIndex["skilllvl_11"])
				 	
	 	if level>160 or level<1 or type(level)~="number" then level=1 end 
	 	if star>20 or star<1 or type(star)~="number" then star=1 end 
	 	
	 	pet:UpdateData(DataIndex["fatalityinchp"], 1)
		pet:UpdateData(DataIndex["fatalityincmp"], 1)
		pet:UpdateData(DataIndex["fatalityincminatk"], 1)
		pet:UpdateData(DataIndex["fatalityincmaxatk"], 1)
		pet:UpdateData(DataIndex["fatalityincdef"], 1)  
		pet:UpdateData(DataIndex["fatalityaddatkrate"], 0)
		pet:UpdateData(DataIndex["fatalityadddodge"], 0)
		pet:UpdateData(DataIndex["fatalityincres1"], 1)
		pet:UpdateData(DataIndex["fatalityincres2"], 1)
		pet:UpdateData(DataIndex["fatalityincres3"], 1)
		pet:UpdateData(DataIndex["fatalityincres4"], 1)
		pet:UpdateData(DataIndex["fatalityincres5"], 1)
		pet:UpdateData(DataIndex["fatalityincres6"], 1)
		pet:UpdateData(DataIndex["fatalityaddcrit"], 0)
		pet:UpdateData(DataIndex["fatalityaddcritdmg"], 0)
		pet:UpdateData(DataIndex["fatalityaddacuity"], 0)		---//种植
		pet:UpdateData(DataIndex["fatalityaddstamina"], 0)	---//采伐
		pet:UpdateData(DataIndex["fatalityaddsawy"], 0)			---//捕兽
		pet:UpdateData(DataIndex["fatalityadddowry"], 0)		---//渔猎
		pet:UpdateData(DataIndex["fatalityaddwisdom"], 0)		---//矿产
		pet:UpdateData(DataIndex["fatalityaddcnr"], 0)			---//宝物
	 	
 		 		
	 	if fatalityskill==1368 then 	---//海龙王
			pet:UpdateData(DataIndex["fatalityadddowry"], math.floor(level/90)*5)
		elseif fatalityskill==1369 then  ---//土灵神
			pet:UpdateData(DataIndex["fatalityaddstamina"], math.floor(level/90)*5)
		elseif fatalityskill==1370 then 			---//地理仙
			pet:UpdateData(DataIndex["fatalityaddwisdom"], math.floor(level/90)*5)
		elseif fatalityskill==1371 then 			---//王牌猎人
			pet:UpdateData(DataIndex["fatalityaddsawy"], math.floor(level/90)*5)
		elseif fatalityskill==1372 then 			---//百晓生
			pet:UpdateData(DataIndex["fatalityaddcnr"], math.floor(level/90)*5)
		elseif fatalityskill==1373 then 			---//智多星
			pet:UpdateData(DataIndex["fatalityaddacuity"], math.floor(level/90)*2)
			pet:UpdateData(DataIndex["fatalityaddstamina"], math.floor(level/90)*2)
			pet:UpdateData(DataIndex["fatalityaddsawy"], math.floor(level/90)*2)
			pet:UpdateData(DataIndex["fatalityadddowry"], math.floor(level/90)*2)
			pet:UpdateData(DataIndex["fatalityaddwisdom"], math.floor(level/90)*2)
			pet:UpdateData(DataIndex["fatalityaddcnr"], math.floor(level/90)*2)
		elseif fatalityskill==1374 then 			---//蛮牛战士
			pet:UpdateData(DataIndex["fatalityincres1"], 1+0.01*math.floor(level/15))
		elseif fatalityskill==1375 then			---//霍去病
			local weakness=0.01*math.floor(pet:QueryData(DataIndex["potential_resist2"])/1500)
			pet:UpdateData(DataIndex["fatalityincres1"], 1+weakness)
			pet:UpdateData(DataIndex["fatalityincres3"], 1+weakness)
		  pet:UpdateData(DataIndex["fatalityincres4"], 1+weakness)
		  pet:UpdateData(DataIndex["fatalityincres5"], 1+weakness)
		  pet:UpdateData(DataIndex["fatalityincres6"], 1)
		elseif fatalityskill==1376 then 		---//风元素
			pet:UpdateData(DataIndex["fatalityincres3"], 1+0.01*star)
		elseif fatalityskill==1377 then 		---//不可不戒
			local charm=0.01*math.floor(pet:QueryData(DataIndex["potential_resist4"])/666)
			pet:UpdateData(DataIndex["fatalityincres4"], 1+charm)
		elseif fatalityskill==1378 then 		---//资深玩家
			if pet:QueryData(DataIndex["potential_resist5"])>=3000 then 
				pet:UpdateData(DataIndex["fatalityincmp"], 1.02)
				pet:UpdateData(DataIndex["fatalityincdef"], 1.01)
			end 		
		elseif fatalityskill==1379 then 		---//超人
			if pet:QueryData(DataIndex["potential_hp"])>=2000 and pet:QueryData(DataIndex["potential_mp"])>=2000
			and pet:QueryData(DataIndex["potential_minatk"])>=2000 and pet:QueryData(DataIndex["potential_maxatk"])>=2000
			and pet:QueryData(DataIndex["potential_def"])>=2000 and pet:QueryData(DataIndex["potential_attackrate"])>=2000
			and pet:QueryData(DataIndex["potential_dodge"])>=2000 and pet:QueryData(DataIndex["potential_resist1"])>=2000
			and pet:QueryData(DataIndex["potential_resist2"])>=2000 and pet:QueryData(DataIndex["potential_resist3"])>=2000
			and pet:QueryData(DataIndex["potential_resist4"])>=2000 and pet:QueryData(DataIndex["potential_resist5"])>=2000
			then 
				pet:UpdateData(DataIndex["fatalityincmaxatk"], 1.02)		
			end 		
		elseif fatalityskill==1380 then 		---//人才
			if star>=(level/10) then 
			pet:UpdateData(DataIndex["fatalityaddcrit"], 0.01)
			pet:UpdateData(DataIndex["fatalityadddodge"], 5)
			end 		
		elseif fatalityskill==1381 then 		---//哪吒
			if pet:QueryData(DataIndex["age"])*10<=pet:QueryData(DataIndex["life"]) then 
			pet:UpdateData(DataIndex["fatalityaddcritdmg"], 0.15)	
			end 
		elseif fatalityskill==1382 then 		---//白蛇
			if pet:QueryMaxHonorPoint()>=20000 then 
				pet:UpdateData(DataIndex["fatalityincminatk"], 1.01)	
				pet:UpdateData(DataIndex["fatalityincmaxatk"], 1.01)	
			end 	
		elseif fatalityskill==1383 then 		---//角斗士
			if pet:QueryData(DataIndex["potential_maxatk"])>=3500 then 
				pet:UpdateData(DataIndex["fatalityincmaxatk"], 1.01)
				pet:UpdateData(DataIndex["fatalityaddcritdmg"], 0.05)		
			end 
		elseif fatalityskill==1384 then			---//狙击手
			if pet:QueryData(DataIndex["potential_attackrate"])>=3500 then 
				pet:UpdateData(DataIndex["fatalityinchp"], 1.02)
			end  
		elseif fatalityskill==1385 then 		---//魔法师
			if pet:QueryData(DataIndex["potential_mp"])>=3500 then 
				pet:UpdateData(DataIndex["fatalityincmaxatk"], 1.02)
			end 
		elseif fatalityskill==1386 then 		---//吸血鬼
			if pet:QueryData(DataIndex["potential_hp"])>=3500 then 
				pet:UpdateData(DataIndex["fatalityincdef"], 1.01)
			end 		
		elseif fatalityskill==1387 then			---//十字军
			if pet:QueryData(DataIndex["potential_def"])>=3500 then 
				pet:UpdateData(DataIndex["fatalityincres1"], 1.02)
				pet:UpdateData(DataIndex["fatalityincres2"], 1.02)
				pet:UpdateData(DataIndex["fatalityincres3"], 1.02)
				pet:UpdateData(DataIndex["fatalityincres4"], 1.02)
				pet:UpdateData(DataIndex["fatalityincres5"], 1.02)
			end  
		elseif fatalityskill==1388 then 		---//干罗
			if star>=12 then 
				pet:UpdateData(DataIndex["fatalityincmaxatk"], 1.01)
				pet:UpdateData(DataIndex["fatalityinchp"], 1.01)
			end 
		elseif fatalityskill==1389 then 		---//阴阳师
			if math.ceil(star/2)-math.floor(star/2)<1 then 
				pet:UpdateData(DataIndex["fatalityincmaxatk"], 1.02)
			else 
				pet:UpdateData(DataIndex["fatalityinchp"], 1.02)
			end 
		elseif fatalityskill==1390 then			---//透明人
			if pet:QueryData(DataIndex["potential_attackrate"])>=3500 and pet:QueryData(DataIndex["potential_dodge"])>=3500 then 
				pet:UpdateData(DataIndex["fatalityincmaxatk"], 1.01)
				pet:UpdateData(DataIndex["fatalityincmp"], 1.02)
			end 
		elseif fatalityskill==1391 then 		---//太极宗师
			if star<10 then 
				pet:UpdateData(DataIndex["fatalityinchp"], 0.97)
				pet:UpdateData(DataIndex["fatalityincmp"], 0.97)												
			else 
				pet:UpdateData(DataIndex["fatalityinchp"], 1)
				pet:UpdateData(DataIndex["fatalityincmp"], 1)
				pet:UpdateData(DataIndex["fatalityaddcrit"], 0.01)	
				pet:UpdateData(DataIndex["fatalityaddcritdmg"], 0.05)
			end 
	 	elseif fatalityskill==1392 then			---//蜘蛛侠
	 		if pet:QueryData(DataIndex["potential_dodge"])>=3500 and pet:QueryData(DataIndex["potential_resist3"])>=3500 then 
	 			pet:UpdateData(DataIndex["fatalityinchp"], 1.01)
				pet:UpdateData(DataIndex["fatalityincmp"], 1.01)	
	 		end 
	 	elseif fatalityskill==1393 then			---//五朵金花
	 		local res1=pet:QueryData(DataIndex["potential_resist1"])
	 		local res2=pet:QueryData(DataIndex["potential_resist2"])
	 		local res3=pet:QueryData(DataIndex["potential_resist3"])
	 		local res4=pet:QueryData(DataIndex["potential_resist4"])
	 		local res5=pet:QueryData(DataIndex["potential_resist5"])
	 		if res1+res2+res3+res4+res5>=15000 then 
	 			pet:UpdateData(DataIndex["fatalityincres1"], 1.02)
		  	pet:UpdateData(DataIndex["fatalityincres2"], 1.02)
		  	pet:UpdateData(DataIndex["fatalityincres3"], 1.02)
		  	pet:UpdateData(DataIndex["fatalityincres4"], 1.02)
		  	pet:UpdateData(DataIndex["fatalityincres5"], 1.02)	
	 		end 	 		
	 	elseif fatalityskill==1394 then 			---//神农后裔
	 		pet:UpdateData(DataIndex["fatalityaddacuity"], math.floor(level/90)*5)
	 		
	 	else
		 	---//乐天派1397
		 	---//不死鸟1398
		 	---//others
		 	pet:UpdateData(DataIndex["fatalityinchp"], 1)
		  pet:UpdateData(DataIndex["fatalityincmp"], 1)
		  pet:UpdateData(DataIndex["fatalityincminatk"], 1)
		  pet:UpdateData(DataIndex["fatalityincmaxatk"], 1)
		  pet:UpdateData(DataIndex["fatalityincdef"], 1)  
		  pet:UpdateData(DataIndex["fatalityaddatkrate"], 0)
		  pet:UpdateData(DataIndex["fatalityadddodge"], 0)
		  pet:UpdateData(DataIndex["fatalityincres1"], 1)
		  pet:UpdateData(DataIndex["fatalityincres2"], 1)
		  pet:UpdateData(DataIndex["fatalityincres3"], 1)
		  pet:UpdateData(DataIndex["fatalityincres4"], 1)
		  pet:UpdateData(DataIndex["fatalityincres5"], 1)
		  pet:UpdateData(DataIndex["fatalityincres6"], 1)
		  pet:UpdateData(DataIndex["fatalityaddcrit"], 0)
		  pet:UpdateData(DataIndex["fatalityaddcritdmg"], 0)
		  pet:UpdateData(DataIndex["fatalityaddacuity"], 0)
		  pet:UpdateData(DataIndex["fatalityaddstamina"], 0)
		  pet:UpdateData(DataIndex["fatalityaddsawy"], 0)
		  pet:UpdateData(DataIndex["fatalityadddowry"], 0)
		  pet:UpdateData(DataIndex["fatalityaddwisdom"], 0)
		  pet:UpdateData(DataIndex["fatalityaddcnr"], 0)
	 	end   
		---//处理舍利相关问题-------
		if buddhaskilllvl<0 or buddhaskilllvl>20 then buddhaskilllvl=0 end 
		if  buddhaskillid==1356 then 
			pet:UpdateData(DataIndex["fatalityincminatk"], pet:QueryData(DataIndex["fatalityincminatk"])+0.01*buddhaskilllvl+0.0025*star*buddhaskilllvl)
		  pet:UpdateData(DataIndex["fatalityincmaxatk"], pet:QueryData(DataIndex["fatalityincmaxatk"])+0.01*buddhaskilllvl+0.0025*star*buddhaskilllvl)
		elseif  buddhaskillid==1357 then
		  pet:UpdateData(DataIndex["fatalityincdef"], pet:QueryData(DataIndex["fatalityincdef"])+0.01*buddhaskilllvl+0.0015*star*buddhaskilllvl) 
		elseif  buddhaskillid==1358 then
			pet:UpdateData(DataIndex["fatalityincminatk"], pet:QueryData(DataIndex["fatalityincminatk"])+0.01*buddhaskilllvl+0.0025*star*buddhaskilllvl)
		  pet:UpdateData(DataIndex["fatalityincmaxatk"], pet:QueryData(DataIndex["fatalityincmaxatk"])+0.01*buddhaskilllvl+0.0025*star*buddhaskilllvl)
		  pet:UpdateData(DataIndex["fatalityincdef"], pet:QueryData(DataIndex["fatalityincdef"])+0.01*buddhaskilllvl+0.0015*star*buddhaskilllvl) 
		else
		
		end 
		
		---//处理豆神相关问题-------
		if peaskillid>=1262 and  peaskillid<=1270 and peaskilllvl>=0 and peaskilllvl<=99 then 
		
			local para1=math.max(peaskillid-1262,0)
			local para2=peaskilllvl
			local sumpara=para1*99+para2
			if peaskilllvl==99 then 
				return 2*sumpara,sumpara,para1+1
			else
				return 2*sumpara,sumpara,para1
			end 
		else 
			return 0,0,0
		end 

end 
---//专用来处理宠物数据需要rollback的情况-----------------------------------------------------------
function ZLuaPetBedgeRollBack(pet_tid,pet)
		
		if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return 0 end
		if pet == nil then return 0 end
		local DataIndex=ZLuaPetBedgeDataIndex()
		local ServerOrigins=ZLuaPetBedgeOrigins()
		local availablePets = ZLuaAvailablePets()
		
		local PetHoroscope=pet:QueryHoroscope()
		local Horoscope= ZLuaPetBedgeHoroscopeDesc()	
	
	  local DescriptionID=1359
	  if PetHoroscope==Horoscope["riyao"] then				---日曜
			DescriptionID=1359
		elseif PetHoroscope==Horoscope["yueyao"] then		---月曜
			DescriptionID=1360
		elseif PetHoroscope==Horoscope["jinyao"] then		---金曜
			DescriptionID=1361
		elseif PetHoroscope==Horoscope["muyao"] then		---木曜
			DescriptionID=1362
		elseif PetHoroscope==Horoscope["shuiyao"] then	---水曜
			DescriptionID=1363
		elseif PetHoroscope==Horoscope["huoyao"] then		---火曜
			DescriptionID=1364
		elseif PetHoroscope==Horoscope["tuyao"] then		---土曜
			DescriptionID=1365
		elseif PetHoroscope==Horoscope["luohou"] then		---罗候
			DescriptionID=1366
		elseif PetHoroscope==Horoscope["jidu"] then			---计都
			DescriptionID=1367
		else
		 	DescriptionID=1359
		end 
	  pet:UpdateData(DataIndex["skillid_10"], DescriptionID or 0)
		pet:UpdateData(DataIndex["skilllvl_10"],1)

end 		

---//处理宠物多阶种族技能
function ZLuaPetBedgeSpiritEffect(pet_tid,pet,origin)
	local DataIndex=ZLuaPetBedgeDataIndex()
	local SpecialIndex=ZLuaPetBedgeSpecialIndex()
	local availablePets = ZLuaAvailablePets()
	
	local raceskillid=pet:QueryData(DataIndex["skillid_9"])	
	local raceskilllvl=0
	
	if raceskillid==1355 then
	 raceskilllvl=pet:QueryData(DataIndex["skilllvl_9"])	
	else
	 raceskilllvl=0
	end  

	return raceskilllvl
end 