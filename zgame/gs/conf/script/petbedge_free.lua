--[[
//设定宠物初始性格和说话

1.设置宠物的初始性格
ZLuaPetBedgeElementInfo(pet_tid)

2.设定宠物的说话
PetBedgeTalk(pet_tid,pet)


--]]
---宠物放生----------------------------------------------------------------------------------------
function PetBedgeFree(pet_tid,pet,origin)
  if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then 
  	return 0,1,75,0,0 
  end
  
  if pet == nil then return end
	---//星等/级别/限界
	local PetStar,PetLevel,PetMainType,PetIsIdentify,PetAge,PetLife
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
			--[[
		["potential_hp"]				=0		["potential_mp"]				=1		["potential_minatk"]		=2		
		["potential_maxatk"]		=3		["potential_def"]				=4		["potential_attackrate"]=5
		["potential_dodge"]			=6		["potential_resist1"]		=7		["potential_resist2"]		=8		
		["potential_resist3"]		=9		["potential_resist4"]		=10		["potential_resist5"]		=11
		["potential_resist6"]		=12
		--]]
	local ItemList={}
	
	---//hp
	ItemList[0]={16029,16030,16031,16032,16033,16034,16035,16036,16037,16038,
							 16039,16040,16041,16042,16043,16044,16045,16046,16047,16048}
	---//mp						
	ItemList[1]={16049,16050,16051,16052,16053,16054,16055,16056,16057,16058,
							 16059,16060,16061,16062,16063,16064,16065,16066,16067,16068}
	---//minatk
	ItemList[2]={16069,16070,16071,16072,16073,16074,16075,16076,16077,16078,
							 16079,16080,16081,16082,16083,16084,16085,16086,16087,16088}
	---//maxatk
	ItemList[3]={16089,16090,16091,16092,16093,16094,16095,16096,16097,16098,
							 16099,16100,16101,16102,16103,16104,16105,16106,16107,16108}
	---//def
	ItemList[4]={16109,16110,16111,16112,16113,16114,16115,16116,16117,16118,
							 16119,16120,16121,16122,16123,16124,16125,16126,16127,16128}
	---//atkrate
	ItemList[5]={16129,16130,16131,16132,16133,16134,16135,16136,16137,16138,
							 16139,16140,16141,16142,16143,16144,16145,16146,16147,16148}
	---//dodge
	ItemList[6]={16149,16150,16151,16152,16153,16154,16155,16156,16157,16158,
							 16159,16160,16161,16162,16164,16163,16165,16166,16167,16168}
	---//res1
	ItemList[7]={16169,16170,16171,16172,16173,16174,16175,16176,16177,16178,
							 16179,16180,16181,16182,16183,16184,16185,16186,16187,16188}
	---//res2
	ItemList[8]={16189,16190,16191,16192,16193,16194,16195,16196,16197,16198,
							 16199,16200,16201,16202,16203,16204,16205,16206,16207,16208}
	---//res3
	ItemList[9]={16249,16250,16251,16252,16253,16254,16255,16256,16257,16258,
								16259,16260,16261,16262,16263,16264,16265,16266,16267,16268}	
	---//res4
	ItemList[10]={16209,16210,16211,16212,16213,16214,16215,16216,16217,16218,
							 16219,16220,16221,16222,16223,16224,16225,16226,16227,16228}
	---//res5
	ItemList[11]={16229,16230,16231,16232,16233,16234,16235,16236,16237,16238,
								16239,16240,16241,16242,16243,16244,16245,16246,16247,16248}
	---//res6
	ItemList[12]={16269,16270,16271,16272,16273,16274,16275,16276,16277,16278,
								16279,16280,16281,16282,16283,16284,16285,16286,16287,16288}
				
	---ItemList[X][Y],X代表MainType,Y代表在X-MainType下20等星分别对应的物品id

local ItemChanceFromStar={}
	ItemChanceFromStar[1]={99,1,0,0,0,0,0}
	ItemChanceFromStar[2]={80,20,0,0,0,0,0}
	ItemChanceFromStar[3]={60,40,0,0,0,0,0}
	ItemChanceFromStar[4]={35,60,5,0,0,0,0}
	ItemChanceFromStar[5]={0,80,20,0,0,0,0}
	ItemChanceFromStar[6]={0,65,30,5,0,0,0}
	ItemChanceFromStar[7]={0,50,40,10,0,0,0}
	ItemChanceFromStar[8]={0,40,45,15,0,0,0}
	ItemChanceFromStar[9]={0,20,50,25,5,0,0}
	ItemChanceFromStar[10]={0,0,50,40,10,0,0}
	ItemChanceFromStar[11]={0,0,40,40,20,0,0}
	ItemChanceFromStar[12]={0,0,30,30,35,5,0}
	ItemChanceFromStar[13]={0,0,20,20,50,10,0}
	ItemChanceFromStar[14]={0,0,10,15,60,15,0}
	ItemChanceFromStar[15]={0,0,0,10,50,40,0}
	ItemChanceFromStar[16]={0,0,0,5,40,50,5}
	ItemChanceFromStar[17]={0,0,0,0,25,40,35}
	ItemChanceFromStar[18]={0,0,0,0,10,30,60}
	ItemChanceFromStar[19]={0,0,0,0,0,15,85}
	ItemChanceFromStar[20]={0,0,0,0,0,0,100}

	local AvailablePets=ZLuaAvailablePets()
	local ItemReturned,NumReturned
	PetStar=pet:QueryStar()
	PetLevel=pet:QueryLevel()
	PetMainType=pet:QueryMainType()	
	PetAge=pet:QueryAge()
	PetLife=pet:QueryLife()
		
	PetIsIdentify=pet:QueryData(DataIndex["identify"])
	
	---//特定宠物无法放生
	for i,v in pairs(AvailablePets) do
		if v.pet_tid == pet_tid and v.free_able==0 then   
			return 0,1,86,0,0	
		end 
	end
		
	---//任务无偿给予的宠物无法放生
	if pet_tid==AvailablePets["Q_conglinshanzhu"].pet_tid or pet_tid==AvailablePets["Q_beimingshuyao"].pet_tid
	or pet_tid==AvailablePets["Q_shouhushiling"].pet_tid then 
		return 0,1,86,0,0	
	end

	
	---//特定宠物无法放生
	if pet_tid==AvailablePets["S_luxueqi"].pet_tid or pet_tid==AvailablePets["S_luxueqi2"].pet_tid or pet_tid==AvailablePets["S_biyao"].pet_tid or pet_tid==AvailablePets["zx2libaochong"].pet_tid or pet_tid==AvailablePets["qingtianzhanhun"].pet_tid then 
		return 0,1,87,0,0	
	end 		

	---//Boss傀儡系列特殊处理
	if pet_tid==AvailablePets["S_kuilei01"].pet_tid or  pet_tid==AvailablePets["S_kuilei02"].pet_tid 
			or  pet_tid==AvailablePets["S_kuilei03"].pet_tid  or  pet_tid==AvailablePets["S_kuilei04"].pet_tid 
			or  pet_tid==AvailablePets["S_kuilei05"].pet_tid  or  pet_tid==AvailablePets["S_kuilei06"].pet_tid 
			or  pet_tid==AvailablePets["S_kuilei07"].pet_tid  then
		return pet_tid,1,100,19257,1
  end 
	
	if PetIsIdentify==0 then 		----//未经鉴定(驯养)不能放生
		return 0,1,78,0,0	
	end 
	
	if PetLevel<30 then 				----//设置为X级别以下不能放生
		return 0,1,76,0,0
	end 
	if PetStar<1 then 					----//设置为X星等以下不能放生
		return 0,1,77,0,0
	end 	

	if PetAge>=PetLife then 		----//宠物年龄>=寿命不能放生
		return 0,1,79,0,0	
	end
	
	---//特定宠物星等、级别无效
	if pet_tid==AvailablePets["Q_heyangqingwa"].pet_tid or pet_tid==AvailablePets["Q_heyangyelang"].pet_tid then 
		PetStar=1	
		PetLevel=60
	end 
			
	local NumFromStar={1,1,1,1,1,1,1,1,1,1,
										 1,1,1,1,1,1,1,1,1,1}	
	---宠物星等对物品数量的影响
	local NumFromLevel=1
	---宠物级别对物品数量的影响		
	if PetStar<=12 and PetStar>=1 then
		NumFromLevel=math.floor(PetLevel/135)+math.floor(PetLevel/150)
	elseif PetStar>12 and PetStar<=17 then  
		NumFromLevel=math.floor(PetLevel/105)+math.floor(PetLevel/135)+math.floor(PetLevel/150)
	elseif PetStar>17 and PetStar<=20 then 
		NumFromLevel=math.floor(PetLevel/75)+math.floor(PetLevel/105)+math.floor(PetLevel/150)
	else
		NumFromLevel=1
	end 
	
	NumReturned=NumFromStar[PetStar]+NumFromLevel
	ItemReturned=ZPetGrade_Get_RanPoint(ItemChanceFromStar[PetStar])
	
	return pet_tid,1,100,ItemList[PetMainType][ItemReturned],NumReturned
end

---宠物性格设定(影响宠物说话)
function ZLuaPetBedgeElementInfo(pet_tid,origin)
  if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then 
  	return 0 
  end
	local AvailablePets=ZLuaAvailablePets()
	local Element=ZLuaPetBedgeElementDesc()
	--[[目前设定的性格
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
	--]]
	local elementlist={}
	local elementchance={}
		
	if pet_tid==AvailablePets["conglinshanzhu"].pet_tid or pet_tid==AvailablePets["Q_conglinshanzhu"].pet_tid 
	or pet_tid==AvailablePets["sancaiyaozhu"].pet_tid then 
		elementlist={1,2,3,0,4,9}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["beimingshuyao"].pet_tid or pet_tid==AvailablePets["G_conglinshanzhu"].pet_tid or pet_tid==AvailablePets["Q_beimingshuyao"].pet_tid or pet_tid==AvailablePets["huanmoshuling"].pet_tid then 
		elementlist={0,4,15,11,12,13}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["bingyuanxuexiong"].pet_tid then 
		elementlist={1,4,10,11,12,17}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["meixindiejing"].pet_tid then 
		elementlist={0,5,6,7,10,14}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["songcaijinshu"].pet_tid or pet_tid==AvailablePets["H_songcaijinshu"].pet_tid then 
		elementlist={0,8,10,13,15,17}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["shouhushiling"].pet_tid or pet_tid==AvailablePets["Q_shouhushiling"].pet_tid or pet_tid==AvailablePets["N_shouhushiling"].pet_tid or pet_tid==AvailablePets["H_shouhushiling"].pet_tid or pet_tid==AvailablePets["guangminghuo"].pet_tid then 
		elementlist={4,17,11,12,13,16}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["yiyujingling"].pet_tid then 
		elementlist={8,9,16,17,3,2}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["poliudijing"].pet_tid then 			---//待修改
		elementlist={1,2,3,8,9,10}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["miaochihuangniao"].pet_tid then 
		elementlist={2,3,4,10,12,15}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["caiyihuangniao"].pet_tid then 
		elementlist={1,5,6,8,9,14}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["xianjielinghou"].pet_tid then 
		elementlist={1,5,6,8,9,14}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["shashouzhixie"].pet_tid then 
		elementlist={1,2,3,8,9,16}
		elementchance={100,100,100,100,100,100}	
	elseif pet_tid==AvailablePets["zhanshenyanmo"].pet_tid then 
		elementlist={3,4,12,2,17,16}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["menghuanmaonv"].pet_tid then 
		elementlist={5,6,7,9,10,17}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["shengdanxueren"].pet_tid then 
		elementlist={5,7,10,13,14,15}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["yuegongyutu"].pet_tid then 
		elementlist={5,7,10,13,14,15}
		elementchance={100,100,100,100,100,100}	
	elseif pet_tid==AvailablePets["S_luxueqi"].pet_tid or pet_tid==AvailablePets["S_luxueqi2"].pet_tid then 
		elementlist={12,12,12,12,12,12}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["S_biyao"].pet_tid then 
		elementlist={2,2,2,2,2,2}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["baihu"].pet_tid or pet_tid==AvailablePets["Z_baihu"].pet_tid then 
		elementlist={2,4,10,11,13,15}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["sanyanlinghou"].pet_tid then 
		elementlist={1,5,6,8,9,14}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["maozei"].pet_tid then 
		elementlist={2,5,8,9,10,15}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["dahuilang"].pet_tid or pet_tid==AvailablePets["dahuilang2"].pet_tid then 
		elementlist={0,1,7,13,14,15}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["xiaomianyang"].pet_tid then 
		elementlist={4,6,8,9,10,2}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["zhanshenxingxing"].pet_tid then 
		elementlist={1,2,3,11,12,16}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["huli"].pet_tid or pet_tid==AvailablePets["huli2"].pet_tid then 
		elementlist={5,6,8,10,13,15}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["dahuang"].pet_tid then 
		elementlist={2,3,4,11,12,15}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["songshu"].pet_tid then 
		elementlist={0,5,7,10,13,14}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["xiongmao"].pet_tid then 
		elementlist={1,2,3,10,13,14}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["lu"].pet_tid then 
		elementlist={0,4,5,7,10,14}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["nainiu"].pet_tid then 
		elementlist={1,2,3,9,10,14}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["datouwawa"].pet_tid then 
		elementlist={0,5,6,10,14,15}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["panguan"].pet_tid then 
		elementlist={3,8,9,11,13,16}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["maotouying"].pet_tid then 
		elementlist={4,5,11,12,13,15}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["yanmo"].pet_tid then 
		elementlist={2,3,11,12,15,16}
		elementchance={100,100,100,100,100,100}	
	elseif pet_tid==AvailablePets["qiubite"].pet_tid then 
		elementlist={0,5,10,13,14,17}
		elementchance={100,100,100,100,100,100}
	elseif pet_tid==AvailablePets["zx2libaochong"].pet_tid or pet_tid==AvailablePets["qingtianzhanhun"].pet_tid or pet_tid==AvailablePets["xinshoukabaihu"].pet_tid then 
		elementlist={4,5,7,10,13,15}
		elementchance={100,100,100,100,100,100}
	else 
		elementlist={0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17}
		elementchance={100,100,100,100,100,100,100,100,100,
									 100,100,100,100,100,100,100,100,100}
	end 
	return elementlist[ZPetGrade_Get_RanPoint(elementchance)]

end 
---宠物日常说话----------------------------------------------------------------------------------------
function PetBedgeTalk(pet_tid,pet)
	---//pet_tid:宠物id
	---//pet 宠物对象
  if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then 
  	return 0,1,1 
  end
	local AvailablePets=ZLuaAvailablePets()
	local Element=ZLuaPetBedgeElementDesc()
	local DataIndex=ZLuaPetBedgeDataIndex()
	
	local msgid,channelid=1000,0
	
	---//特定宠物不会说话
	if pet_tid==AvailablePets["Q_heyangqingwa"].pet_tid or pet_tid==AvailablePets["Q_heyangyelang"].pet_tid then 
		return pet_tid,channelid,msgid
	end 
		
	local talk={}
	talk[1]={}						---//各个性格宠物说话
	talk[2]={}						---//具体宠物_常规说话
	talk[3]={}						---//具体宠物_高亲密度说话
	talk[4]={}						---//具体宠物_高级别说话
	talk[5]={}						---//具体宠物_饥饿说话
	talk[6]={}						---//功能说话
	
	
	
	local petelement=pet:QueryData(DataIndex["element"])
	local petlevel=pet:QueryLevel()
	local pethungercase=pet:QueryCurHungerPoint()/(pet:QueryMaxHungerPoint()+1)
	local pethonor=pet:QueryCurHonorPoint()
		
	if pet_tid==AvailablePets["conglinshanzhu"].pet_tid or pet_tid==AvailablePets["Q_conglinshanzhu"].pet_tid 
	or  pet_tid==AvailablePets["sancaiyaozhu"].pet_tid then 
		talk[2]={1001,1002,1003,1004,1005,1006,1007,1008,1009,1010,1011,1012,1013,1014,1015,1016,1017,1018,1019,1020}
		talk[3]={1021,1022,1023,1024,1025,1026,1027,1028,1029,1030}
		talk[4]={1031,1032,1033,1034,1035,1036,1037,1038,1039,1040}
		talk[5]={1041,1042,1043,1044}
	elseif pet_tid==AvailablePets["beimingshuyao"].pet_tid or pet_tid==AvailablePets["Q_beimingshuyao"].pet_tid 
	or pet_tid==AvailablePets["huanmoshuling"].pet_tid or pet_tid==AvailablePets["G_conglinshanzhu"].pet_tid then 
		talk[2]={1101,1102,1103,1104,1105,1106,1107,1108,1109,1110,1111,1112,1113,1114,1115,1116,1117,1118,1119,1120}
		talk[3]={1121,1122,1123,1124,1125,1126,1127,1128,1129,1130}
		talk[4]={1131,1132,1133,1134,1135,1136,1137,1138,1139,1140}
		talk[5]={1141,1142,1143,1144}
	elseif pet_tid==AvailablePets["bingyuanxuexiong"].pet_tid then 
		talk[2]={1201,1202,1203,1204,1205,1206,1207,1208,1209,1210,1211,1212,1213,1214,1215,1216,1217,1218,1219,1220}
		talk[3]={1221,1222,1223,1224,1225,1226,1227,1228,1229,1230}
		talk[4]={1231,1232,1233,1234,1235,1236,1237,1238,1239,1240}
		talk[5]={1241,1242,1243,1244}
	elseif pet_tid==AvailablePets["meixindiejing"].pet_tid then 
		talk[2]={1301,1302,1303,1304,1305,1306,1307,1308,1309,1310,1311,1312,1313,1314,1315,1316,1317,1318,1319,1320}
		talk[3]={1321,1322,1323,1324,1325,1326,1327,1328,1329,1330}
		talk[4]={1331,1332,1333,1334,1335,1336,1337,1338,1339,1340}
		talk[5]={1341,1342,1343,1344}
	elseif pet_tid==AvailablePets["songcaijinshu"].pet_tid or pet_tid==AvailablePets["H_songcaijinshu"].pet_tid then 
		talk[2]={1401,1402,1403,1404,1405,1406,1407,1408,1409,1410,1411,1412,1413,1414,1415,1416,1417,1418,1419,1420}
		talk[3]={1421,1422,1423,1424,1425,1426,1427,1428,1429,1430}
		talk[4]={1431,1432,1433,1434,1435,1436,1437,1438,1439,1440}
		talk[5]={1441,1442,1443,1444}
	elseif pet_tid==AvailablePets["shouhushiling"].pet_tid or pet_tid==AvailablePets["Q_shouhushiling"].pet_tid or pet_tid==AvailablePets["N_shouhushiling"].pet_tid or pet_tid==AvailablePets["H_shouhushiling"].pet_tid then 
		talk[2]={1501,1502,1503,1504,1505,1506,1507,1508,1509,1510,1511,1512,1513,1514,1515,1516,1517,1518,1519,1520}
		talk[3]={1521,1522,1523,1524,1525,1526,1527,1528,1529,1530}
		talk[4]={1531,1532,1533,1534,1535,1536,1537,1538,1539,1540}
		talk[5]={1541,1542,1543,1544}
	elseif pet_tid==AvailablePets["yiyujingling"].pet_tid then 
		talk[2]={1601,1602,1603,1604,1605,1606,1607,1608,1609,1610,1611,1612,1613,1614,1615,1616,1617,1618,1619,1620}
		talk[3]={1621,1622,1623,1624,1625,1626,1627,1628,1629,1630}
		talk[4]={1631,1632,1633,1634,1635,1636,1637,1638,1639,1640}
		talk[5]={1641,1642,1643,1644}
	elseif pet_tid==AvailablePets["poliudijing"].pet_tid then ---//待修改
		talk[2]={1601,1602,1603,1604,1605,1606,1607,1608,1609,1610,1611,1612,1613,1614,1615,1616,1617,1618,1619,1620}
		talk[3]={1621,1622,1623,1624,1625,1626,1627,1628,1629,1630}
		talk[4]={1631,1632,1633,1634,1635,1636,1637,1638,1639,1640}
		talk[5]={1641,1642,1643,1644}
	elseif pet_tid==AvailablePets["miaochihuangniao"].pet_tid or pet_tid==AvailablePets["caiyihuangniao"].pet_tid then 
		talk[2]={1701,1702,1703,1704,1705,1706,1707,1708,1709,1710,1711,1712,1713,1714,1715,1716,1717,1718,1719,1720}
		talk[3]={1721,1722,1723,1724,1725,1726,1727,1728,1729,1730}
		talk[4]={1731,1732,1733,1734,1735,1736,1737,1738,1739,1740}
		talk[5]={1741,1742,1743,1744}
	elseif pet_tid==AvailablePets["shashouzhixie"].pet_tid then 
		talk[2]={1801,1802,1803,1804,1805,1806,1807,1808,1809,1810,1811,1812,1813,1814,1815,1816,1817,1818,1819,1820}
		talk[3]={1821,1822,1823,1824,1825,1826,1827,1828,1829,1830}
		talk[4]={1831,1832,1833,1834,1835,1836,1837,1838,1839,1840}
		talk[5]={1841,1842,1843,1844}
	elseif pet_tid==AvailablePets["menghuanmaonv"].pet_tid then 
		talk[2]={1901,1902,1903,1904,1905,1906,1907,1908,1909,1910,1911,1912,1913,1914,1915,1916,1917,1918,1919,1920}
		talk[3]={1921,1922,1923,1924,1925,1926,1927,1928,1929,1930}
		talk[4]={1931,1932,1933,1934,1935,1936,1937,1938,1939,1940}
		talk[5]={1941,1942,1943,1944}
	elseif pet_tid==AvailablePets["zhanshenyanmo"].pet_tid then 
		talk[2]={2001,2002,2003,2004,2005,2006,2007,2008,2009,2010,2011,2012,2013,2014,2015,2016,2017,2018,2019,2020}
		talk[3]={2021,2022,2023,2024,2025,2026,2027,2028,2029,2030}
		talk[4]={2031,2032,2033,2034,2035,2036,2037,2038,2039,2040}
		talk[5]={2041,2042,2043,2044}
	elseif pet_tid==AvailablePets["shengdanxueren"].pet_tid then 
		talk[2]={2101,2102,2103,2104,2105,2106,2107,2108,2109,2110,2111,2112,2113,2114,2115,2116,2117,2118,2119,2120}
		talk[3]={2121,2122,2123,2124,2125,2126,2127,2128,2129,2130}
		talk[4]={2131,2132,2133,2134,2135,2136,2137,2138,2139,2140}
		talk[5]={2141,2142,2143,2144}
	elseif pet_tid==AvailablePets["yuegongyutu"].pet_tid then 
		talk[2]={2201,2202,2203,2204,2205,2206,2207,2208,2209,2210,2211,2212,2213,2214,2215,2216,2217,2218,2219,2220}
		talk[3]={2221,2222,2223,2224,2225,2226,2227,2228,2229,2230}
		talk[4]={2231,2232,2233,2234,2235,2236,2237,2238,2239,2240}
		talk[5]={2241,2242,2243,2244}	
	elseif pet_tid==AvailablePets["S_luxueqi"].pet_tid then 
		talk[2]={601,602,603,604,605,606,607,608,609,610,611,612,613,
						 614,615,616,617,618,619,620,621,622,623,624,625,626,
						 627,628,629,630,631,632,633,634,635,636,637,638,639,
						 640,641,642}
		talk[3]={}
		talk[4]={}
		talk[5]={}
	elseif pet_tid==AvailablePets["S_biyao"].pet_tid then ---//待修改
		talk[2]={701,702,703,704,705,706,707,708,709,710,711,712,713,
						 714,715,716,717,718,719,720,721,722,723,724,725,726,
						 727,728,729,730,731,732,733,734,735,736,737,738,739,
						 740,741,742}
		talk[3]={}
		talk[4]={}
		talk[5]={}
	elseif pet_tid==AvailablePets["baihu"].pet_tid or pet_tid==AvailablePets["Z_baihu"].pet_tid then 
		talk[2]={2301,2302,2303,2304,2305,2306,2307,2308,2309,2310,2311,2312,2313,2314,2315,2316,2317,2318,2319,2320}
		talk[3]={2321,2322,2323,2324,2325,2326,2327,2328,2329,2330}
		talk[4]={2331,2332,2333,2334,2335,2336,2337,2338,2339,2340}
		talk[5]={2341,2342,2343,2344}	
	elseif pet_tid==AvailablePets["maozei"].pet_tid then 
		talk[2]={2401,2402,2403,2404,2405,2406,2407,2408,2409,2410,2411,2412,2413,2414,2415,2416,2417,2418,2419,2420}
		talk[3]={2421,2422,2423,2424,2425,2426,2427,2428,2429,2430}
		talk[4]={2431,2432,2433,2434,2435,2436,2437,2438,2439,2440}
		talk[5]={2441,2442,2443,2444}	
	elseif pet_tid==AvailablePets["dahuilang"].pet_tid or pet_tid==AvailablePets["dahuilang2"].pet_tid then 
		talk[2]={3101,3102,3103,3104,3105,3106,3107,3108,3109,3110,3111,3112,3113,3114,3115,3116,3117,3118,3119,3120}
		talk[3]={3121,3122,3123,3124,3125,3126,3127,3128,3129,3130}
		talk[4]={3131,3132,3133,3134,3135,3136,3137,3138,3139,3140}
		talk[5]={3141,3142,3143,3144}
	elseif pet_tid==AvailablePets["xiaomianyang"].pet_tid then 
		talk[2]={3001,3002,3003,3004,3005,3006,3007,3008,3009,3010,3011,3012,3013,3014,3015,3016,3017,3018,3019,3020}
		talk[3]={3021,3022,3023,3024,3025,3026,3027,3028,3029,3030}
		talk[4]={3031,3032,3033,3034,3035,3036,3037,3038,3039,3040}
		talk[5]={3041,3042,3043,3044}	
	elseif pet_tid==AvailablePets["huli"].pet_tid or pet_tid==AvailablePets["huli2"].pet_tid then ---//待修改
		talk[2]={}
		talk[3]={}
		talk[4]={}
		talk[5]={}
	elseif pet_tid==AvailablePets["dahuang"].pet_tid then ---//待修改
		talk[2]={}
		talk[3]={}
		talk[4]={}
		talk[5]={}	
	elseif pet_tid==AvailablePets["songshu"].pet_tid then ---//待修改
		talk[2]={}
		talk[3]={}
		talk[4]={}
		talk[5]={}
	elseif pet_tid==AvailablePets["xiongmao"].pet_tid then ---//待修改
		talk[2]={}
		talk[3]={}
		talk[4]={}
		talk[5]={}
	elseif pet_tid==AvailablePets["lu"].pet_tid then ---//待修改
		talk[2]={2701,2702,2703,2704,2705,2706,2707,2708,2709,2710,2711,2712,2713,2714,2715,2716,2717,2718,2719,2720}
		talk[3]={2721,2722,2723,2724,2725,2726,2727,2728,2729,2730}
		talk[4]={2731,2732,2733,2734,2735,2736,2737,2738,2739,2740}
		talk[5]={2741,2742,2743,2744}	
	elseif pet_tid==AvailablePets["nainiu"].pet_tid then ---//待修改
		talk[2]={}
		talk[3]={}
		talk[4]={}
		talk[5]={}
	elseif pet_tid==AvailablePets["datouwawa"].pet_tid then ---//待修改
		talk[2]={2801,2802,2803,2804,2805,2806,2807,2808,2809,2810,2811,2812,2813,2814,2815,2816,2817,2818,2819,2820}
		talk[3]={2821,2822,2823,2824,2825,2826,2827,2828,2829,2830}
		talk[4]={2831,2832,2833,2834,2835,2836,2837,2838,2839,2840}
		talk[5]={2841,2842,2843,2844}	
	elseif pet_tid==AvailablePets["panguan"].pet_tid then ---//待修改
		talk[2]={2901,2902,2903,2904,2905,2906,2907,2908,2909,2910,2911,2912,2913,2914,2915,2916,2917,2918,2919,2920}
		talk[3]={2921,2922,2923,2924,2925,2926,2927,2928,2929,2930}
		talk[4]={2931,2932,2933,2934,2935,2936,2937,2938,2939,2940}
		talk[5]={2941,2942,2943,2944}	
	elseif pet_tid==AvailablePets["maotouying"].pet_tid then ---//待修改
		talk[2]={2601,2602,2603,2604,2605,2606,2607,2608,2609,2610,2611,2612,2613,2614,2615,2616,2617,2618,2619,2620}
		talk[3]={2621,2622,2623,2624,2625,2626,2627,2628,2629,2630}
		talk[4]={2631,2632,2633,2634,2635,2636,2637,2638,2639,2640}
		talk[5]={2641,2642,2643,2644}	
	elseif pet_tid==AvailablePets["yanmo"].pet_tid then ---//待修改
		talk[2]={2501,2502,2503,2504,2505,2506,2507,2508,2509,2510,2511,2512,2513,2514,2515,2516,2517,2518,2519,2520}
		talk[3]={2521,2522,2523,2524,2525,2526,2527,2528,2529,2530}
		talk[4]={2531,2532,2533,2534,2535,2536,2537,2538,2539,2540}
		talk[5]={2541,2542,2543,2544}	
	elseif pet_tid==AvailablePets["zx2libaochong"].pet_tid or pet_tid==AvailablePets["qingtianzhanhun"].pet_tid or pet_tid==AvailablePets["xinshoukabaihu"].pet_tid then ---//待修改
		talk[2]={}
		talk[3]={}
		talk[4]={}
		talk[5]={}
	elseif pet_tid==AvailablePets["qiubite"].pet_tid then 
		talk[2]={3201,3202,3203,3204,3205,3206,3207,3208,3209,3210,3211,3212,3213,3214,3215,3216,3217,3218,3219,3220}
		talk[3]={3221,3222,3223,3224,3225,3226,3227,3228,3229,3230}
		talk[4]={3231,3232,3233,3234,3235,3236,3237,3238,3239,3240}
		talk[5]={3241,3242,3243,3244}	
	else 
	end 
	if petelement==Element["danqie"] then ---//胆怯
		talk[1]={5101,5102,5103,5104,5105,5106,5107,5108,5109,5110}
	elseif petelement==Element["lumang"] then---//鲁莽
		talk[1]={5111,5112,5113,5114,5115,5116,5117,5118,5119,5120}
	elseif petelement==Element["kuangye"] then---//狂野
		talk[1]={5121,5122,5123,5132,5132,5132,5127,5128,5129,5130}
	elseif petelement==Element["baozao"] then---//暴躁
		talk[1]={5131,5132,5133,5134,5135,5136,5137,5138,5139,5140}
	elseif petelement==Element["gaoao"] then---//高傲	
		talk[1]={5141,5142,5143,5144,5145,5146,5147,5148,5149,5150}
	elseif petelement==Element["youya"] then	---//优雅
		talk[1]={5151,5152,5153,5154,5155,5156,5157,5158,5159,5160}
	elseif petelement==Element["yaomei"] then	---//妖媚
		talk[1]={5161,5162,5163,5164,5165,5166,5167,5168,5169,5170}
	elseif petelement==Element["wenrou"] then		---//温柔
		talk[1]={5171,5172,5173,5174,5175,5176,5177,5178,5179,5180}
	elseif petelement==Element["yinxian"] then	---//阴险
		talk[1]={5181,5182,5183,5184,5185,5186,5187,5188,5189,5190}
	elseif petelement==Element["xiee"] then	---//邪恶
		talk[1]={5191,5192,5193,5194,5195,5196,5197,5198,5199,5200}
	elseif petelement==Element["wanpi"] then---//顽皮
		talk[1]={5201,5202,5203,5204,5205,5206,5207,5208,5209,5210}
	elseif petelement==Element["weiyan"] then---//威严
		talk[1]={5211,5212,5213,5214,5215,5216,5217,5218,5219,5220}
	elseif petelement==Element["shensheng"] then---//神圣
		talk[1]={5221,5222,5223,5232,5232,5232,5227,5228,5229,5230}
	elseif petelement==Element["ruizhi"] then---//睿智
		talk[1]={5231,5232,5233,5234,5235,5236,5237,5238,5239,5320}
	elseif petelement==Element["miantian"] then---//腼腆
		talk[1]={5321,5322,5323,5324,5325,5326,5327,5328,5329,5320}
	elseif petelement==Element["boxue"] then---//博学
		talk[1]={5321,5322,5323,5324,5325,5326,5327,5328,5329,5320}
	elseif petelement==Element["gupi"] then---//孤僻	
		talk[1]={5321,5322,5323,5324,5325,5326,5327,5328,5329,5270}
	elseif petelement==Element["yiyu"] then---//异域
		talk[1]={5271,5272,5273,5274,5275,5276,5277,5278,5279,5280}
	else
	end 
	
		talk[6]={601,602,603,604,605,606,607,608,609,610,
						 611,612,613}
	
	local talkchance={100,100,0,0,0,0}
	if pethungercase<0.32 then 
		talkchance[5]=500
	else
		talkchance[5]=0
	end 	
	
	if petlevel>=120 then 
		talkchance[4]=100
		talkchance[6]=0
	elseif petlevel<30 then 
		talkchance[4]=0
		talkchance[6]=50	
	else
		talkchance[4]=0
		talkchance[6]=0
	end 
	
	if pethonor>=10000 then 
		talkchance[3]=100
	else
		talkchance[3]=0
	end 	
	
	if pet_tid==AvailablePets["S_biyao"].pet_tid or  pet_tid==AvailablePets["S_luxueqi"].pet_tid then
		talkchance={0,100,0,0,0,0}
	end 
	local talklistchoose=ZPetGrade_Get_RanPoint(talkchance)
	local msgid=talk[talklistchoose][ZPetGrade_Get_RanPoint(talk[talklistchoose])]
	if type(msgid)~="number" then msgid=1000 end 

	return pet_tid,channelid,msgid

end 
