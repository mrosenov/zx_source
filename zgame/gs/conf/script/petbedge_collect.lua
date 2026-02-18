---宠物采集脚本说明
---主要函数petbedge调用其它函数
---
---采集类型声明---------------------------------------------------------------------------

function ZLuaPetCollectTypes()
	local CollectTypes={}
	CollectTypes["zhongzhi"]		=0
	CollectTypes["famu"]				=1
	CollectTypes["shoulie"]			=2
	CollectTypes["diaoyu"]			=3
	CollectTypes["caikuang"]		=4
	CollectTypes["kaogu"]				=5
	return CollectTypes
end 

function ZLuaPetCollectMsg()
	local Msg={}
	Msg["NotEnoughLevel"]        = 101
	Msg["succzhongzhi"]          = 102
	Msg["succfamu"]              = 106
	Msg["succshoulie"]           = 107
	Msg["succdiaoyu"]            = 108
	Msg["succcaikuang"]          = 109
	Msg["succkaogu"]             = 110
	Msg["badluckzhongzhi"]       = 103
	Msg["badluckfamu"]           = 111
	Msg["badluckshoulie"]        = 112
	Msg["badluckdiaoyu"]         = 113
	Msg["badluckcaikuang"]       = 114
	Msg["badluckkaogu"]          = 115
	Msg["Wrong"]                 = 104
	Msg["Hungery"]               = 105
	Msg["cannotzhongzhi"]				 = 116
	Msg["cannotfamu"]				     = 117
	Msg["cannotshoulie"]				 = 118
	Msg["cannotdiaoyu"]				   = 119
	Msg["cannotcaikuang"]				 = 120
	Msg["cannotkaogu"]			   	 = 121
	return Msg
end

function PetBedgeCollect(pet_tid,pet,collecttype,mode)
	---DropItemList,具体物品
  local ItemList={{},{},{},{},{},{}}
	

	ItemList[1][1] = {
										---种植材料ID、手足配方ID、躯体配方ID
										16813,16814,16816,16817,16818,16819,16813,16814,16816,16817,16818,16819,																														
										16855,16856,16857,16860,16861,16862,16855,16856,16857,16860,16861,16862,																													
										16899,16900,16901,16903,16904,16905,16899,16900,16901,16903,16904,25401,																													
									  16942,16944,16945,16946,16947,16948,16942,16944,16945,16946,16947,25401,																													
										16986,16987,16988,16989,16990,16991,16986,16987,16988,16989,25402,25401,																													
										17032,17033,17034,17036,17037,17038,17032,17033,17034,17036,25402,25401,}
										
	ItemList[1][2] = {16812,16812,16812,16812,16812,25401,
										16858,16858,16858,16858,25401,25401,
										16898,16898,16898,16898,16898,16898,
										16943,16943,16943,16943,25402,25402,
										16985,16985,16985,16985,25401,25401,
										17035,17035,17035,17035,25402,25402,}
	
	ItemList[1][3] = {17913,17919,18178,18184,18190,18196,18202,18208,18214,18221,
										17914,17920,18179,18185,18191,18197,18203,18209,18216,18222,
										17915,17921,18180,18186,18192,18198,18204,18210,18217,18223,
										17916,17922,18181,18187,18193,18199,18205,18211,18218,18224,
										17917,17923,18182,18188,18194,18200,18206,18212,18219,18225,
										17918,17924,18183,18189,18195,18201,18207,18213,18220,18226}
	
	ItemList[1][4] = {17925,17926,17927,17928,17929,17930}
	
	ItemList[1][5] = {17931,17932,17933,17934,17935,17936}
	
	ItemList[1][6] = {16815,16859,16902,16941,16984,17011}
	
	ItemList[2][1] = {
										---伐木材料ID、内甲配方ID、外甲配方ID
										16820,16821,16824,16825,16826,16820,16821,16824,16825,16826,									
										16864,16865,16867,16868,16869,16864,16865,16867,16868,16869,
										16907,16908,16910,16911,16912,16907,16908,16910,16911,25401,
										16949,16950,16953,16954,16955,16949,16950,16953,16954,25401,
										16992,16993,16996,16997,16998,16992,16993,16996,25402,25401,
										17039,17040,17043,17044,17045,17039,17040,17043,25402,25401,}
									
	ItemList[2][2] = {16822,16822,16822,16822,16822,25401,
										16863,16863,16863,16863,25401,25401,
										16906,16906,16906,16906,16906,16906,
										16951,16951,16951,16951,25402,25402,
										16994,16994,16994,16994,25401,25401,
										17041,17041,17041,17041,25402,25402,}
	
	ItemList[2][3] = {17937,17943,17961,17967,
										17938,17944,17962,17968,
										17939,17945,17963,17969,
										17940,17946,17964,17970,
										17941,17947,17965,17971,
										17942,17948,17966,17972}
	
	ItemList[2][4] = {17949,17973,
										17950,17974,
										17951,17975,
										17952,17976,
										17953,17978,
										17954,17979}
	
	ItemList[2][5] = {17955,17979,
										17956,17980,
										17957,17981,
										17958,17982,
										17959,17983,
										17960,17984}
	
	ItemList[2][6] = {16823,16866,16909,16952,16995,17042}
	
	ItemList[3][1] = {
										---狩猎材料ID、金身配方ID、信念配方ID
										16827,16828,16832,16833,17003,16827,16828,16832,16833,17003,																														
										16871,16872,16874,16875,16876,16871,16872,16874,16875,16876,																														
										16914,16915,16917,16918,16919,16914,16915,16917,16918,25401,																														
										16956,16957,16960,16961,16962,16956,16957,16960,16961,25401,																														
										17001,17002,17004,17005,17010,17001,17002,17004,25402,25401,																													
										17046,17048,17050,17051,17052,17046,17048,17050,25402,25401,																		
										}
										
  ItemList[3][2] = {16829,16829,16829,16829,16829,25401,
  									16870,16870,16870,16870,25401,25401,
  									16913,16913,16913,16913,16913,16913,
  									16958,16958,16958,16958,25402,25402,
  									17000,17000,17000,17000,25401,25401,
  									17047,17047,17047,17047,25402,25402,}
  
  ItemList[3][3] = {17985,17991,18081,18087,18093,18099,18105,18111,18117,18123,18009,
  									17986,17992,18082,18088,18094,18100,18106,18112,18118,18124,18010,
  									17987,17993,18083,18089,18095,18101,18107,18113,18119,18125,18011,
  									17988,17994,18084,18090,18096,18102,18108,18114,18120,18126,18012,
  									17989,17995,18085,18091,18097,18103,18109,18115,18121,18127,18013,
  									17990,17996,18086,18092,18098,18104,18110,18116,18122,18128,18014}
  
  ItemList[3][4] = {17997,18015,17998,18016,17999,18017,18000,18018,18001,18019,18002,18020}
  
  ItemList[3][5] = {18003,18021,18004,18022,18005,18023,18006,18024,18007,18025,18008,18026}
  
  ItemList[3][6] = {16830,16873,16916,16959,16999,17049}
	
	ItemList[4][1] = {
										---钓鱼材料ID、心志配方ID、血统配方ID
										16834,16835,16836,16838,16839,16834,16835,16836,16838,16839,
										16878,16879,16880,16882,16877,16878,16879,16880,16882,16877,
										16920,16921,16922,16924,16925,16920,16921,16922,16924,25401,
										16963,16964,16965,16966,16967,16963,16964,16965,16966,25401,
										17009,17012,17013,17014,17015,17009,17012,17013,25402,25401,
										17053,17054,17055,17056,17058,17053,17054,17055,25402,25401,}
										
	ItemList[4][2] = {16837,16837,16837,16837,16837,25401,
										16881,16881,16881,16881,25401,25401,
										16923,16923,16923,16923,16923,16923,
										16968,16968,16968,16968,25402,25402,
										17016,17016,17016,17016,25401,25401,
										17059,17059,17059,17059,25402,25402,}
	
	ItemList[4][3] = {18027,18033,18129,18135,18141,18147,18153,18159,18165,18171,
										18028,18034,18130,18136,18142,18148,18154,18160,18166,18172,
										18029,18035,18131,18137,18143,18149,18155,18161,18167,18173,
										18030,18036,18132,18138,18144,18150,18156,18162,18168,18174,
										18031,18037,18133,18139,18145,18151,18157,18163,18169,18175,
										18032,18038,18134,18140,18146,18152,18158,18164,18170,18176}
	
	ItemList[4][4] = {18039,18040,18041,18042,18043,18044}
	
	ItemList[4][5] = {18045,18046,18047,18048,18049,18050}
	
	ItemList[4][6] = {16840,16883,16926,16969,17017,17057}
	
	ItemList[5][1] = {
										---采矿材料ID、运道配方ID、面目配方ID、
										16841,16842,16844,16846,16841,16842,16844,16846,16846,16844,																									
										16885,16886,16887,16888,16885,16886,16887,16888,16888,16887,																														
										16927,16928,16929,16930,16927,16928,16929,16930,25401,16930,																														
										16970,16972,16974,16975,16970,16972,16974,16975,25401,16975,																														
										17018,17019,17020,17022,17018,17019,17020,17022,25401,25402,																														
										17060,17061,17062,17063,17060,17061,17062,17063,25401,25402,}
										
	ItemList[5][2] = {16843,16843,16843,16843,16843,25401,
										16884,16884,16884,16884,25401,25401,
										16931,16931,16931,16931,16931,16931,
										16971,16971,16971,16971,25402,25402,
										17023,17023,17023,17023,25401,25401,
										17064,17064,17064,17064,25402,25402,}
	
	ItemList[5][3] = {17889,17895,18075,
										17890,17896,18076,
										17891,17897,18077,
										17892,17898,18078,
										17893,17899,18079,
										17894,17900,18080}
	
	ItemList[5][4] = {17901,17902,17903,17904,17905,17906}
	
	ItemList[5][5] = {17907,17908,17909,17910,17911,17912}
	
	ItemList[5][6] = {16845,16889,16932,16973,17021,17065}
	
	ItemList[6][1] = {
										---考古材料ID、气脉配方ID、神秘配方ID
										16847,16848,16853,16854,16847,16848,16853,16854,16853,16854,								
										16891,16893,16896,16897,16891,16893,16896,16897,16896,16897,
										16933,16935,16937,16939,16933,16935,16937,16939,16937,25401,
										16976,16977,16980,16982,16976,16977,16980,16982,16980,25401,
										17024,17027,17028,17029,17024,17027,17028,17029,25402,25401,
										17066,17067,17070,17071,17066,17067,17070,17071,25402,25401,}
										
	ItemList[6][2] = {16849,16851,16849,16851,16849,25401,
										16892,16894,16892,16894,25401,25401,
										16934,16940,16934,16940,16934,16940,
										16978,16983,16978,16983,25402,25402,
										17026,17030,17026,17030,25401,25401,
										17068,17072,17068,17072,25402,25402,}
	
	ItemList[6][3] = {18051,18057,18286,18227,18233,18239,18245,18251,18257,18263,18269,
										18052,18058,18287,18228,18234,18240,18246,18252,18258,18264,18270,
										18053,18059,18288,18229,18235,18241,18247,18253,18259,18265,18271,
										18054,18060,18289,18230,18236,18242,18248,18254,18260,18266,18272,
										18055,18061,18290,18231,18237,18243,18249,18255,18261,18267,18273,
										18056,18062,18291,18232,18238,18244,18250,18256,18262,18268,18274}
	
	ItemList[6][4] = {18063,18292,18064,18293,18065,18294,18066,18295,18067,18296,18068,18297}
	
	ItemList[6][5] = {18069,18298,18070,18299,18071,18300,18072,18301,18073,18302,18074,18303}
										
	ItemList[6][6] = {16850,16852,16890,16895,16936,16938,
										16979,16981,17025,17031,17069,17073}
	local Collecttype = collecttype
	local Mode= mode
	---分别为宠物id,宠物对象,采集类型(共六种)
	local DataIndex=ZLuaPetBedgeDataIndex()
	local CollectTypes=ZLuaPetCollectTypes()
	local Msg =ZLuaPetCollectMsg()
	local Acuity=pet:QueryData(DataIndex["acuity"]) + pet:QueryData(DataIndex["fatalityaddacuity"])or 0
	local Stamina=pet:QueryData(DataIndex["stamina"])	+ pet:QueryData(DataIndex["fatalityaddstamina"]) or 0
	local Sawy=pet:QueryData(DataIndex["sawy"]) + pet:QueryData(DataIndex["fatalityaddsawy"]) or 0
	local Dowry=pet:QueryData(DataIndex["dowry"]) +	pet:QueryData(DataIndex["fatalityadddowry"])or 0
	local Wisdom=pet:QueryData(DataIndex["wisdom"])  + pet:QueryData(DataIndex["fatalityaddwisdom"])or 0
	local CNR=pet:QueryData(DataIndex["cnr"]) + pet:QueryData(DataIndex["fatalityaddcnr"])or 0

	local Level= pet : QueryLevel() or 1
	local Star = pet : QueryStar() or 1
	--- 获得当前饱食度值，对其进行操作
	local msgid =	Msg["Wrong"]
	local origin=pet:QueryData(DataIndex["orign"])
	local Curhunger = pet:QueryCurHungerPoint() or 0
	---local Curhunger = pet:QueryMaxHungerPoint(DataIndex["maxhunger"])
	local maxhunger = pet:QueryMaxHungerPoint() or 1
	---每次减少100饱食度
	local newHungerValue = Curhunger - 100

	local channel = 0
	local count = 0
	local returnedthing = 0
  ---调用ZLuaPetCollectValueKey(),ZLuaPetCollectDropCount(),ZLuaPetCollectDropQuality()获得掉落数量和质量
	local ValueOfMode,ValueOfStar,ValueOfLevel,ValueOfAttribute,RateOfHunger=ZLuaPetCollectValueKey(pet,Mode,Star,Level,Curhunger,maxhunger,Collecttype)

	local DropQualityKey = ZLuaPetCollectDropQuality(ValueOfStar,ValueOfLevel,ValueOfMode,ValueOfAttribute)
	local DropCountKey   = ZLuaPetCollectDropCount(ValueOfMode,RateOfHunger)
	local LevelStep      = ZLuaPetCollectLevelSelect(Level)
	--[[if DropCountKey ~=0 then 
		counttt =counttt+1
	end
	if DropQualityKey ==2 then 
	  nice = nice +1
	 elseif DropQualityKey==3 then
	  elite = elite+1
	 end--]]
	---
	if Curhunger < 100  and  Curhunger>=0  then 
				msgid =Msg["Hungery"]
		
	elseif Curhunger < 0 then
				pet:SetCurHungerPoint(0)	 
   		  ZLuaPetBedgeRebuild(pet_tid,pet,origin)
				msgid=Msg["Wrong"]
	
	elseif Curhunger >= 100 then
  	    pet:SetCurHungerPoint(newHungerValue)		 
  	    ZLuaPetBedgeRebuild(pet_tid,pet,origin)
  	    count = DropCountKey
  	    if 	(((0<= Acuity) and (Acuity<=100)) 
		    and (( 0<= Stamina) and (Stamina <= 100)) 
		    and ((0<= Dowry) and (Dowry <=100)) 
		    and ((0<= Wisdom) and (Wisdom<=100))
		    and ((0<=CNR) and (CNR<=100)) 
		    and ((0<=Sawy) and (Sawy<=100))) 
		    and ((0<Level) and (Level<=150)) 
		    and ((0<Star) and (Star<=20)) 	
		    and (( collecttype<=6) and (collecttype>=0))  then
					local maxmetatable   = table.maxn(ItemList[collecttype+1][DropQualityKey])	
					---print("maxmetatable : "..maxmetatable)		
					if Level< 30 then 
								msgid = Msg["NotEnoughLevel"]
					elseif ValueOfAttribute ==0 then
		    			 	if     collecttype == 0 then 
		    			 		msgid = Msg["cannotzhongzhi"]
		    			 	elseif collecttype == 1 then 
		    			 		msgid = Msg["cannotfamu"]
		    				elseif collecttype == 2 then
		    				 	msgid = Msg["cannotshoulie"]
		    				elseif collecttype == 3 then
		    			 		msgid  = Msg["cannotdiaoyu"]
		    			 	elseif collecttype == 4 then
		    					msgid  = Msg["cannotcaikuang"]
		    				 elseif collecttype == 5 then
		    			 		msgid  = Msg["cannotkaogu"]
		    			 	 end
					elseif ValueOfAttribute ~=0 then 
						if DropCountKey ~= 0 then
							---print("levelstep : "..LevelStep)
		    			 	returnedthing = ItemList[collecttype+1][DropQualityKey][math.random(maxmetatable/6*LevelStep+1,maxmetatable/6*(LevelStep+1))]
		    			 	if     collecttype == 0 then 
		    			 		msgid = Msg["succzhongzhi"]
		    			 	elseif collecttype == 1 then 
		    			 		msgid = Msg["succfamu"]
		    				elseif collecttype == 2 then
		    				 	msgid = Msg["succshoulie"]
		    				elseif collecttype == 3 then
		    			 		msgid  = Msg["succdiaoyu"]
		    			 	elseif collecttype == 4 then
		    					msgid  = Msg["succcaikuang"]
		    				 elseif collecttype == 5 then
		    			 		msgid  = Msg["succkaogu"]
		    			 	 end
		    		elseif DropCountKey == 0 then
		    				if     collecttype == 0 then 
		    			 		msgid = Msg["badluckzhongzhi"]	
		    				elseif collecttype == 1 then	
		    					msgid = Msg["badluckfamu"]
		    				elseif collecttype == 2 then
		    				  msgid = Msg["badluckshoulie"]
		    				elseif collecttype == 3 then
		    					msgid = Msg["badluckdiaoyu"]
		    				elseif collecttype == 4 then
		    					msgid = Msg["badluckcaikuang"]
		    				elseif collecttype == 5 then
		    					msgid = Msg["badluckkaogu"]
		    				end
		    		end
		    	end  
		
		end 
	end
	return pet_tid,channel,msgid,returnedthing,count
end

function ZLuaPetCollectValueKey(pet,mode,Star,Level,Curhunger,maxhunger,collecttype)
	local ValueOfMode =0
	local ValueOfStar =0
	local ValueOfLevel=0
	local DataIndex=ZLuaPetBedgeDataIndex()
	local CollectTypes=ZLuaPetCollectTypes()
	local Msg =ZLuaPetCollectMsg()
	local Acuity=pet:QueryData(DataIndex["acuity"]) + pet:QueryData(DataIndex["fatalityaddacuity"])or 0
	local Stamina=pet:QueryData(DataIndex["stamina"])	+ pet:QueryData(DataIndex["fatalityaddstamina"]) or 0
	local Sawy=pet:QueryData(DataIndex["sawy"]) + pet:QueryData(DataIndex["fatalityaddsawy"]) or 0
	local Dowry=pet:QueryData(DataIndex["dowry"]) +	pet:QueryData(DataIndex["fatalityadddowry"])or 0
	local Wisdom=pet:QueryData(DataIndex["wisdom"])  + pet:QueryData(DataIndex["fatalityaddwisdom"])or 0
	local CNR=pet:QueryData(DataIndex["cnr"]) + pet:QueryData(DataIndex["fatalityaddcnr"])or 0
	local Level= pet : QueryLevel() or 1
	local Star = pet : QueryStar() or 1
	local RateOfHunger = Curhunger/maxhunger or 0
	local ValueOfAttribute=0
	local DataIndex=ZLuaPetBedgeDataIndex()

	if     mode == 0 then
    ValueOfMode = 1
	elseif mode == 1 then
		ValueOfMode = 0
	elseif mode == 2 then
		ValueOfMode = 0
	else
		ValueOfMode = 0
	end
	---星等所对应的权值
	if Star >= 10 then
		ValueOfStar = Star
	else
		ValueOfStar = 0
	end
	---等级所对应的权值
	if Level >=70 then
		ValueOfLevel = 10
	else
		ValueOfLevel = 0
	end
	---饱食度所对应的权值

	---各个属性对采集的权值
	if     collecttype ==0  then
		ValueOfAttribute = Acuity
	elseif collecttype ==1 then
		ValueOfAttribute = Stamina
  elseif collecttype ==2 then
		ValueOfAttribute = Sawy
	elseif collecttype ==3 then
		ValueOfAttribute = Dowry
	elseif collecttype ==4 then
		ValueOfAttribute = Wisdom
	elseif collecttype ==5 then
		ValueOfAttribute = CNR
	else
		ValueOfAttribute = 0
	end
	if ValueOfAttribute >25 then 
		ValueOfAttribute = 25
	end	
	return ValueOfMode,ValueOfStar,ValueOfLevel,ValueOfAttribute,RateOfHunger
end   

function ZLuaPetCollectDropCount(ValueOfMode,RateOfHunger)
---影响掉落概率的最终权值
---DropKey = ( Mode * 0.8 + Random(0.2-2.2) ) * (1 + HungerRate)/2
	local DropCountKey = 0
	local a = (ValueOfMode*8000 +math.random(2000,22000))*(1 + RateOfHunger)/2
	if a < 10000 then 
		DropCountKey = 0
	elseif a>=10000 and a < 20000 then 
		DropCountKey = 1
	elseif a>=20000 and a < 30000 then
		DropCountKey = 2
  elseif a>=30000  then
		DropCountKey = 3
	end
	return DropCountKey
end

function ZLuaPetCollectLevelSelect(level)
	local levelstep 
	if level <= 70 then 
		levelstep =0
	elseif level>70 and level<=100 then
		levelstep =1
	elseif level>100 and level<=120 then
		levelstep =2
	elseif level>120 and level<=135 then
		levelstep =3
	elseif level>135 and level<=145 then
		levelstep =4
	elseif level>145 then
		levelstep =5
	end
	return levelstep
end
		
function ZLuaPetCollectDropQuality(ValueOfStar,ValueOfLevel,ValueOfMode,ValueOfAttribute)
---影响掉落物品品质的最终权值   QualityKey =  Random((Star+Level),99) + Attribute + Mode
	local DropQualityKey = 1
	local b =  (math.random(0+ValueOfStar+ValueOfLevel,999)) + ValueOfAttribute+ ValueOfMode*15
	if b < 850 then 
		DropQualityKey = 1
	elseif b>=850 and b < 950 then 
		DropQualityKey = 2
  elseif b>=950 and b < 975 then 
		DropQualityKey = 3
  elseif b>=975 and b < 985 then 
		DropQualityKey = 4
  elseif b>=985 and b < 990 then 
		DropQualityKey = 5		
	elseif b>=990 then
		DropQualityKey = 6
	end
	return DropQualityKey	
end

function PetBedgeAskCollectRequirement(pet_tid,pet,collecttype)
	local RequireList = {}
  ---物品ID
  local RequireList = {18305}
	if pet_tid ~= nil then
		---此表具体物品未定	
		local hunger = pet:QueryCurHungerPoint() or 1
		if hunger >= 100 then 
				if collecttype == 0 then 
					return RequireList[1],1
				elseif collecttype ==1 then
		  		return RequireList[1],1
				elseif collecttype ==2 then
		 		 return RequireList[1],1
				elseif collecttype ==3 then
				  return RequireList[1],1
				elseif collecttype ==4 then
		 		 return RequireList[1],1
				elseif collecttype ==5 then
				  return RequireList[1],1
			  else 
		 			return 0,0
				end
		else  return 0,0
		end
	else 
			return 0,0
	end
end