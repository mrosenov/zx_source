---定义所有的修炼辅助道具功能----------------------------------------------------------------------
function ZLuaPetBedgeAidMatrix()
	local AidMatrix={}
		---//type=0:主要属性修炼概率和效果调整
		---//effect1:成功概率增加,effect2:使用效果增加,是否溅射增长
		AidMatrix[16322]={effect1=25,effect2=0,type=0}
		AidMatrix[16323]={effect1=0,effect2=0.30,type=0}
		
		---//type=1:技能修炼调整
		---//effect1:最多可容纳的技能数量
		AidMatrix[22393]={effect1=7,effect2=0.30,type=1}
		
		AidMatrix[0]={effect1=0,effect2=0,effect3=99}
	return AidMatrix
end 
-----定义所有的修炼用道具功能----------------------------------------------------------------------
function ZLuaPetBedgeCidMatrix()
	local Horoscope=ZLuaPetBedgeHoroscopeDesc()
	local Race=ZLuaPetBedgeRaceDesc()
	local CidMatrix={}
		---//type=1	 	逆天重生
		---//type=2	 	鉴定道具,effect1
		---//type=3		改变年龄,effect1(减少年龄量)
		---//type=4		改变寿命,effect1(增加寿命量)
		---//type=5	 	宠物更改五行(性格),effect1(1-6分别代表六种性格,7代表随机)
		---//type=6	 	宠物改变天赋,effect1(1-15分别代表具体点、随机和变异)
		---//type=7	 	宠物学习技能,effect1(位置(1-10固定位置,13随机位置),effect2技能,effect3,技能级别,
		---//	effect4 需求种族,effect5,需求职业,effect6,需求形态,effect7,具体id)
		---//type=7	 	宠物学习技能,effect1(适合职业0-8,99通用),effect2(技能指向id),effect3(追加技能级别)
		---//					effect4(基础成功概率>100,实际成功率为effect4-本体级别),effect5,技能书星等)
		---//	effect4 需求种族,effect5,需求职业,effect6,需求形态,effect7,具体id)	
		---//type=8	 	宠物遗忘技能,effect1(位置)
		---//type=9	 	宠物增加亲密度当前值,effect1(增加数值)、effect2(增加到上限的比例<=1)
		---//type=10	宠物增加亲密度上限,effect1(数值)
		---//type=11	宠物属性成长(气血),effect1(概率1-100),effect2(增加值),effect3(MaxstarCanUse)
		---//type=12	宠物属性成长(真气),effect1(概率1-100),effect2(增加值),effect3(MaxstarCanUse)
		---//type=13	宠物属性成长(最小攻击),effect1(概率1-100),effect2(增加值),effect3(MaxstarCanUse)
		---//type=14	宠物属性成长(最大攻击),effect1(概率1-100),effect2(增加值),effect3(MaxstarCanUse)
		---//type=15	宠物属性成长(通用攻击),effect1(概率1-100),effect2(增加值),effect3(MaxstarCanUse)
		---//type=16	宠物属性成长(防御),effect1(概率1-100),effect2(增加值),effect3(MaxstarCanUse)
		---//type=17	宠物属性成长(命中),effect1(概率1-100),effect2(增加值),effect3(MaxstarCanUse)
		---//type=18	宠物属性成长(躲闪),effect1(概率1-100),effect2(增加值),effect3(MaxstarCanUse)
		---//type=19	宠物属性成长(眩晕),effect1(概率1-100),effect2(增加值),effect3(MaxstarCanUse)
		---//type=20	宠物属性成长(虚弱),effect1(概率1-100),effect2(增加值),effect3(MaxstarCanUse)
		---//type=21	宠物属性成长(定身),effect1(概率1-100),effect2(增加值),effect3(MaxstarCanUse)
		---//type=22	宠物属性成长(魅惑),effect1(概率1-100),effect2(增加值),effect3(MaxstarCanUse)
		---//type=23	宠物属性成长(昏睡),effect1(概率1-100),effect2(增加值),effect3(MaxstarCanUse)
		---//type=24	宠物属性成长(冰冻),effect1(概率1-100),effect2(增加值),effect3(MaxstarCanUse)
		---//type=25	随机战斗属性成长(每日任务)
		---//type11-25 effect4/effect5/effect6为第二套数值(使用aid的被动成长),effect7为失败被动成长
		---//type=26	宠物属性成长生活1,effect1(概率1-100),effect2(增加值),effect3(MaxstarCanUse)
		---//type=27	宠物属性成长生活2,effect1(概率1-100),effect2(增加值),effect3(MaxstarCanUse)
		---//type=28	宠物属性成长生活3,effect1(概率1-100),effect2(增加值),effect3(MaxstarCanUse)
		---//type=29	宠物属性成长生活4,effect1(概率1-100),effect2(增加值),effect3(MaxstarCanUse)
		---//type=30	宠物属性成长生活5,effect1(概率1-100),effect2(增加值),effect3(MaxstarCanUse)
		---//type=31	宠物属性成长生活6,effect1(概率1-100),effect2(增加值),effect3(MaxstarCanUse)
		---//type=32	改变主类型(限界)
		---//type=33	形象改变药剂
		---//type=34	形象变异药剂
		---//type=35  形象观察药剂(美工用)
		---//type=36 	改变轮回
		---//type=37	宠物斗神修炼
		---//type=38	宠物舍利强化(佛缘专用effect1(类型1-攻击2-防御3-混合)effect2(等级))
			
		---//预留道具
		CidMatrix[16302]={type=1,effect1=0,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}		
		CidMatrix[31246]={type=1,effect1=0,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		---//鉴定道具
		CidMatrix[16303]={type=2,effect1=0,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		---//改变年龄
		CidMatrix[16304]={type=3,effect1=100,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}			
		CidMatrix[16305]={type=3,effect1=500,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[16306]={type=3,effect1=2000,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		---//改变寿命
		CidMatrix[16315]={type=4,effect1=100,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}			
		CidMatrix[16316]={type=4,effect1=500,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[16317]={type=4,effect1=2000,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		---//性格改变
		CidMatrix[18371]={type=5,effect1=99,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[18370]={type=5,effect1=0,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16320]={type=5,effect1=1,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[16326]={type=5,effect1=2,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[16327]={type=5,effect1=3,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[16328]={type=5,effect1=4,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[16329]={type=5,effect1=5,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[16330]={type=5,effect1=6,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[16331]={type=5,effect1=7,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		---//天赋改变
		CidMatrix[16311]={type=6,effect1=1,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}			
		CidMatrix[16312]={type=6,effect1=2,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16332]={type=6,effect1=3,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[16333]={type=6,effect1=4,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16334]={type=6,effect1=5,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16335]={type=6,effect1=6,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16336]={type=6,effect1=7,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16337]={type=6,effect1=8,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16338]={type=6,effect1=9,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16339]={type=6,effect1=10,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16340]={type=6,effect1=11,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16341]={type=6,effect1=12,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16342]={type=6,effect1=13,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16343]={type=6,effect1=14,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16344]={type=6,effect1=15,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		
		---//技能修炼
		CidMatrix[19356]={type=7,effect1=0,effect2=1399,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//守护<日曜>
		CidMatrix[19357]={type=7,effect1=0,effect2=1400,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//热忱<日曜>
		CidMatrix[19358]={type=7,effect1=0,effect2=1401,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//闪耀<日曜>
		CidMatrix[19359]={type=7,effect1=0,effect2=1402,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//光辉<日曜>
		CidMatrix[19360]={type=7,effect1=0,effect2=1403,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//神聚<日曜>
		CidMatrix[19361]={type=7,effect1=0,effect2=1404,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//中天<日曜>
		CidMatrix[19362]={type=7,effect1=1,effect2=1405,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//新月<月曜>
		CidMatrix[19363]={type=7,effect1=1,effect2=1406,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//会神<月曜>
		CidMatrix[19364]={type=7,effect1=1,effect2=1407,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//贯注<月曜>
		CidMatrix[19365]={type=7,effect1=1,effect2=1408,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//流星<月曜>
		CidMatrix[19366]={type=7,effect1=1,effect2=1409,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//月影<月曜>
		CidMatrix[19367]={type=7,effect1=1,effect2=1410,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//水月<月曜>
		CidMatrix[19368]={type=7,effect1=2,effect2=1411,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//芒刺<金曜>
		CidMatrix[19369]={type=7,effect1=2,effect2=1412,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//怒嚎<金曜>
		CidMatrix[19370]={type=7,effect1=2,effect2=1413,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//全力<金曜>
		CidMatrix[19371]={type=7,effect1=2,effect2=1414,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//撕裂<金曜>
		CidMatrix[19372]={type=7,effect1=2,effect2=1415,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//刺耳<金曜>
		CidMatrix[19373]={type=7,effect1=2,effect2=1416,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//挑衅<金曜>
		CidMatrix[19374]={type=7,effect1=3,effect2=1417,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//曜灵<木曜>
		CidMatrix[19375]={type=7,effect1=3,effect2=1418,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//林荫<木曜>
		CidMatrix[19376]={type=7,effect1=3,effect2=1419,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//猛击<木曜>
		CidMatrix[19377]={type=7,effect1=3,effect2=1420,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//明灭<木曜>
		CidMatrix[19378]={type=7,effect1=3,effect2=1421,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//羁绊<木曜>
		CidMatrix[19379]={type=7,effect1=3,effect2=1422,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//疲惫<木曜>
		CidMatrix[19380]={type=7,effect1=4,effect2=1423,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//甘泉<水曜>
		CidMatrix[19381]={type=7,effect1=4,effect2=1424,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//祷告<水曜>
		CidMatrix[19382]={type=7,effect1=4,effect2=1425,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//预言<水曜>
		CidMatrix[19383]={type=7,effect1=4,effect2=1426,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//波纹<水曜>
		CidMatrix[19384]={type=7,effect1=4,effect2=1427,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//涟漪<水曜>
		CidMatrix[19385]={type=7,effect1=4,effect2=1428,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//思虑<水曜>
		CidMatrix[19386]={type=7,effect1=5,effect2=1429,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//火甲<火曜>
		CidMatrix[19387]={type=7,effect1=5,effect2=1430,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//贯通<火曜>
		CidMatrix[19388]={type=7,effect1=5,effect2=1431,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//酝酿<火曜>
		CidMatrix[19389]={type=7,effect1=5,effect2=1432,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//灼热<火曜>
		CidMatrix[19390]={type=7,effect1=5,effect2=1433,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//真火<火曜>
		CidMatrix[19391]={type=7,effect1=5,effect2=1434,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//燃尽<火曜>
		CidMatrix[19392]={type=7,effect1=6,effect2=1435,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//地祝<土曜>
		CidMatrix[19393]={type=7,effect1=6,effect2=1436,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//洞察<土曜>
		CidMatrix[19394]={type=7,effect1=6,effect2=1437,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//伤痕<土曜>
		CidMatrix[19395]={type=7,effect1=6,effect2=1438,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//追迹<土曜>
		CidMatrix[19396]={type=7,effect1=6,effect2=1439,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//沉沦<土曜>
		CidMatrix[19397]={type=7,effect1=6,effect2=1440,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//陷地<土曜>
		CidMatrix[19398]={type=7,effect1=7,effect2=1441,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//妄言<罗候>
		CidMatrix[19399]={type=7,effect1=7,effect2=1442,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//黯淡<罗候>
		CidMatrix[19400]={type=7,effect1=7,effect2=1443,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//宿命<罗候>
		CidMatrix[19401]={type=7,effect1=7,effect2=1444,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//禁锢<罗候>
		CidMatrix[19402]={type=7,effect1=7,effect2=1445,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//喝言<罗候>
		CidMatrix[19403]={type=7,effect1=7,effect2=1446,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//苦痛<罗候>
		CidMatrix[19404]={type=7,effect1=8,effect2=1447,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//闪避<计都>
		CidMatrix[19405]={type=7,effect1=8,effect2=1448,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//藐视<计都>
		CidMatrix[19406]={type=7,effect1=8,effect2=1449,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//锋利<计都>
		CidMatrix[19407]={type=7,effect1=8,effect2=1450,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//厄运<计都>
		CidMatrix[19408]={type=7,effect1=8,effect2=1451,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//忏悔<计都>
		CidMatrix[19409]={type=7,effect1=8,effect2=1452,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//迷茫<计都>
		CidMatrix[19410]={type=7,effect1=99,effect2=1453,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//沸腾<通用>
		CidMatrix[19411]={type=7,effect1=99,effect2=1454,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//凝华<通用>
		CidMatrix[19412]={type=7,effect1=99,effect2=1455,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//挥洒<通用>
		CidMatrix[19413]={type=7,effect1=99,effect2=1456,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//蛮力<通用>
		CidMatrix[19414]={type=7,effect1=99,effect2=1457,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//修整<通用>
		CidMatrix[19415]={type=7,effect1=99,effect2=1458,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//汲取<通用>
		
		CidMatrix[23229]={type=7,effect1=0,effect2=1399,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·守护<日曜>★
		CidMatrix[23230]={type=7,effect1=0,effect2=1400,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·热忱<日曜>★
		CidMatrix[23231]={type=7,effect1=0,effect2=1401,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·闪耀<日曜>★
		CidMatrix[23232]={type=7,effect1=0,effect2=1402,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·光辉<日曜>★
		CidMatrix[23233]={type=7,effect1=0,effect2=1403,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·神聚<日曜>★
		CidMatrix[23234]={type=7,effect1=0,effect2=1404,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·中天<日曜>★
		CidMatrix[23235]={type=7,effect1=1,effect2=1405,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·新月<月曜>★
		CidMatrix[23236]={type=7,effect1=1,effect2=1406,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·会神<月曜>★
		CidMatrix[23237]={type=7,effect1=1,effect2=1407,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·贯注<月曜>★
		CidMatrix[23238]={type=7,effect1=1,effect2=1408,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·流星<月曜>★
		CidMatrix[23239]={type=7,effect1=1,effect2=1409,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·月影<月曜>★
		CidMatrix[23240]={type=7,effect1=1,effect2=1410,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·水月<月曜>★
		CidMatrix[23241]={type=7,effect1=2,effect2=1411,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·芒刺<金曜>★
		CidMatrix[23242]={type=7,effect1=2,effect2=1412,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·怒嚎<金曜>★
		CidMatrix[23243]={type=7,effect1=2,effect2=1413,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·全力<金曜>★
		CidMatrix[23244]={type=7,effect1=2,effect2=1414,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·撕裂<金曜>★
		CidMatrix[23245]={type=7,effect1=2,effect2=1415,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·刺耳<金曜>★
		CidMatrix[23246]={type=7,effect1=2,effect2=1416,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·挑衅<金曜>★
		CidMatrix[23247]={type=7,effect1=3,effect2=1417,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·曜灵<木曜>★
		CidMatrix[23248]={type=7,effect1=3,effect2=1418,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·林荫<木曜>★
		CidMatrix[23249]={type=7,effect1=3,effect2=1419,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·猛击<木曜>★
		CidMatrix[23250]={type=7,effect1=3,effect2=1420,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·明灭<木曜>★
		CidMatrix[23251]={type=7,effect1=3,effect2=1421,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·羁绊<木曜>★
		CidMatrix[23252]={type=7,effect1=3,effect2=1422,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·疲惫<木曜>★
		CidMatrix[23253]={type=7,effect1=4,effect2=1423,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·甘泉<水曜>★
		CidMatrix[23254]={type=7,effect1=4,effect2=1424,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·祷告<水曜>★
		CidMatrix[23255]={type=7,effect1=4,effect2=1425,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·预言<水曜>★
		CidMatrix[23256]={type=7,effect1=4,effect2=1426,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·波纹<水曜>★
		CidMatrix[23257]={type=7,effect1=4,effect2=1427,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·涟漪<水曜>★
		CidMatrix[23258]={type=7,effect1=4,effect2=1428,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·思虑<水曜>★
		CidMatrix[23259]={type=7,effect1=5,effect2=1429,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·火甲<火曜>★
		CidMatrix[23260]={type=7,effect1=5,effect2=1430,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·贯通<火曜>★
		CidMatrix[23261]={type=7,effect1=5,effect2=1431,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·酝酿<火曜>★
		CidMatrix[23263]={type=7,effect1=5,effect2=1432,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·灼热<火曜>★
		CidMatrix[23264]={type=7,effect1=5,effect2=1433,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·真火<火曜>★
		CidMatrix[23266]={type=7,effect1=5,effect2=1434,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·燃尽<火曜>★
		CidMatrix[23268]={type=7,effect1=6,effect2=1435,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·地祝<土曜>★
		CidMatrix[23269]={type=7,effect1=6,effect2=1436,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·洞察<土曜>★
		CidMatrix[23270]={type=7,effect1=6,effect2=1437,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·伤痕<土曜>★
		CidMatrix[23271]={type=7,effect1=6,effect2=1438,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·追迹<土曜>★
		CidMatrix[23272]={type=7,effect1=6,effect2=1439,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·沉沦<土曜>★
		CidMatrix[23273]={type=7,effect1=6,effect2=1440,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·陷地<土曜>★
		CidMatrix[23274]={type=7,effect1=7,effect2=1441,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·妄言<罗候>★
		CidMatrix[23275]={type=7,effect1=7,effect2=1442,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·黯淡<罗候>★
		CidMatrix[23276]={type=7,effect1=7,effect2=1443,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·宿命<罗候>★
		CidMatrix[23277]={type=7,effect1=7,effect2=1444,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·禁锢<罗候>★
		CidMatrix[23278]={type=7,effect1=7,effect2=1445,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·喝言<罗候>★
		CidMatrix[23279]={type=7,effect1=7,effect2=1446,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·苦痛<罗候>★
		CidMatrix[23280]={type=7,effect1=8,effect2=1447,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·闪避<计都>★
		CidMatrix[23281]={type=7,effect1=8,effect2=1448,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·藐视<计都>★
		CidMatrix[23282]={type=7,effect1=8,effect2=1449,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·锋利<计都>★
		CidMatrix[23283]={type=7,effect1=8,effect2=1450,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·厄运<计都>★
		CidMatrix[23284]={type=7,effect1=8,effect2=1451,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·忏悔<计都>★
		CidMatrix[23285]={type=7,effect1=8,effect2=1452,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·迷茫<计都>★
		CidMatrix[23286]={type=7,effect1=99,effect2=1453,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·沸腾<通用>★
		CidMatrix[23287]={type=7,effect1=99,effect2=1454,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·凝华<通用>★
		CidMatrix[23288]={type=7,effect1=99,effect2=1455,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·挥洒<通用>★
		CidMatrix[23289]={type=7,effect1=99,effect2=1456,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·蛮力<通用>★
		CidMatrix[23290]={type=7,effect1=99,effect2=1457,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·修整<通用>★
		CidMatrix[23291]={type=7,effect1=99,effect2=1458,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//宠物修炼宝玉·汲取<通用>★

		CidMatrix[60541]={type=7,effect1=0,effect2=5018,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//<日曜>
		CidMatrix[60543]={type=7,effect1=1,effect2=5019,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//<月曜>
		CidMatrix[60545]={type=7,effect1=2,effect2=5020,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//<金曜>
		CidMatrix[60547]={type=7,effect1=3,effect2=5021,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//<木曜>
		CidMatrix[60549]={type=7,effect1=4,effect2=5022,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//<水曜>
		CidMatrix[60551]={type=7,effect1=5,effect2=5023,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//<火曜>
		CidMatrix[60553]={type=7,effect1=6,effect2=5024,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//<土曜>
		CidMatrix[60555]={type=7,effect1=8,effect2=5025,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//<计都>
		CidMatrix[60557]={type=7,effect1=7,effect2=5026,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//<罗侯>

		CidMatrix[60542]={type=7,effect1=0,effect2=5018,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//<日曜>2
		CidMatrix[60544]={type=7,effect1=1,effect2=5019,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//<月曜>2
		CidMatrix[60546]={type=7,effect1=2,effect2=5020,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//<金曜>2
		CidMatrix[60548]={type=7,effect1=3,effect2=5021,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//<木曜>2
		CidMatrix[60550]={type=7,effect1=4,effect2=5022,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//<水曜>2
		CidMatrix[60552]={type=7,effect1=5,effect2=5023,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//<火曜>2
		CidMatrix[60554]={type=7,effect1=6,effect2=5024,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//<土曜>2
		CidMatrix[60556]={type=7,effect1=8,effect2=5025,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//<计都>2
		CidMatrix[60558]={type=7,effect1=7,effect2=5026,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//<罗侯>2

		CidMatrix[61424]={type=7,effect1=0,effect2=5018,effect3=1,effect4=300,effect5=2,effect6=0,effect7=0}---//<日曜>3
		CidMatrix[61425]={type=7,effect1=1,effect2=5019,effect3=1,effect4=300,effect5=2,effect6=0,effect7=0}---//<月曜>3
		CidMatrix[61426]={type=7,effect1=2,effect2=5020,effect3=1,effect4=300,effect5=2,effect6=0,effect7=0}---//<金曜>3
		CidMatrix[61427]={type=7,effect1=3,effect2=5021,effect3=1,effect4=300,effect5=2,effect6=0,effect7=0}---//<木曜>3
		CidMatrix[61428]={type=7,effect1=4,effect2=5022,effect3=1,effect4=300,effect5=2,effect6=0,effect7=0}---//<水曜>3
		CidMatrix[61429]={type=7,effect1=5,effect2=5023,effect3=1,effect4=300,effect5=2,effect6=0,effect7=0}---//<火曜>3
		CidMatrix[61431]={type=7,effect1=6,effect2=5024,effect3=1,effect4=300,effect5=2,effect6=0,effect7=0}---//<土曜>3
		CidMatrix[61432]={type=7,effect1=8,effect2=5025,effect3=1,effect4=300,effect5=2,effect6=0,effect7=0}---//<计都>3
		CidMatrix[61433]={type=7,effect1=7,effect2=5026,effect3=1,effect4=300,effect5=2,effect6=0,effect7=0}---//<罗侯>3

		CidMatrix[60559]={type=7,effect1=99,effect2=5027,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//<通用>
		CidMatrix[60561]={type=7,effect1=99,effect2=5028,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//<通用>
		CidMatrix[60563]={type=7,effect1=99,effect2=5029,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//<通用>
		CidMatrix[60565]={type=7,effect1=99,effect2=5030,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//<通用>
		CidMatrix[60567]={type=7,effect1=99,effect2=5031,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//<通用>
		CidMatrix[60569]={type=7,effect1=99,effect2=5032,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//<通用>
		CidMatrix[60571]={type=7,effect1=99,effect2=5033,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//<通用>
		CidMatrix[60573]={type=7,effect1=99,effect2=5034,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//<通用>
		CidMatrix[60575]={type=7,effect1=99,effect2=5035,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//<通用>
		CidMatrix[60577]={type=7,effect1=99,effect2=5036,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//<通用>
		CidMatrix[60579]={type=7,effect1=99,effect2=5037,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//<通用>
		CidMatrix[60581]={type=7,effect1=99,effect2=5038,effect3=1,effect4=300,effect5=0,effect6=0,effect7=0}---//<通用>

		CidMatrix[60560]={type=7,effect1=99,effect2=5027,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//<通用>2
		CidMatrix[60562]={type=7,effect1=99,effect2=5028,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//<通用>2
		CidMatrix[60564]={type=7,effect1=99,effect2=5029,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//<通用>2
		CidMatrix[60566]={type=7,effect1=99,effect2=5030,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//<通用>2
		CidMatrix[60568]={type=7,effect1=99,effect2=5031,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//<通用>2
		CidMatrix[60570]={type=7,effect1=99,effect2=5032,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//<通用>2
		CidMatrix[60572]={type=7,effect1=99,effect2=5033,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//<通用>2
		CidMatrix[60574]={type=7,effect1=99,effect2=5034,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//<通用>2
		CidMatrix[60576]={type=7,effect1=99,effect2=5035,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//<通用>2
		CidMatrix[60578]={type=7,effect1=99,effect2=5036,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//<通用>2
		CidMatrix[60580]={type=7,effect1=99,effect2=5037,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//<通用>2
		CidMatrix[60582]={type=7,effect1=99,effect2=5038,effect3=1,effect4=300,effect5=1,effect6=0,effect7=0}---//<通用>2

		CidMatrix[61434]={type=7,effect1=99,effect2=5027,effect3=1,effect4=300,effect5=2,effect6=0,effect7=0}---//<通用>3
		CidMatrix[61435]={type=7,effect1=99,effect2=5028,effect3=1,effect4=300,effect5=2,effect6=0,effect7=0}---//<通用>3
		CidMatrix[61436]={type=7,effect1=99,effect2=5029,effect3=1,effect4=300,effect5=2,effect6=0,effect7=0}---//<通用>3
		CidMatrix[61437]={type=7,effect1=99,effect2=5030,effect3=1,effect4=300,effect5=2,effect6=0,effect7=0}---//<通用>3
		CidMatrix[61438]={type=7,effect1=99,effect2=5031,effect3=1,effect4=300,effect5=2,effect6=0,effect7=0}---//<通用>3
		CidMatrix[61439]={type=7,effect1=99,effect2=5032,effect3=1,effect4=300,effect5=2,effect6=0,effect7=0}---//<通用>3
		CidMatrix[61440]={type=7,effect1=99,effect2=5033,effect3=1,effect4=300,effect5=2,effect6=0,effect7=0}---//<通用>3
		CidMatrix[61441]={type=7,effect1=99,effect2=5034,effect3=1,effect4=300,effect5=2,effect6=0,effect7=0}---//<通用>3
		CidMatrix[61442]={type=7,effect1=99,effect2=5035,effect3=1,effect4=300,effect5=2,effect6=0,effect7=0}---//<通用>3
		CidMatrix[61443]={type=7,effect1=99,effect2=5036,effect3=1,effect4=300,effect5=2,effect6=0,effect7=0}---//<通用>3
		CidMatrix[61444]={type=7,effect1=99,effect2=5037,effect3=1,effect4=300,effect5=2,effect6=0,effect7=0}---//<通用>3
		CidMatrix[61445]={type=7,effect1=99,effect2=5038,effect3=1,effect4=300,effect5=2,effect6=0,effect7=0}---//<通用>3


		---//改变亲密度当前值
		CidMatrix[16307]={type=9,effect1=50,effect2=1,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}			
		CidMatrix[16308]={type=9,effect1=300,effect2=1,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[16309]={type=9,effect1=1600,effect2=1,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}			
		CidMatrix[16310]={type=9,effect1=3600,effect2=1,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}			
		---//改变亲密度最大值
		CidMatrix[16311]={type=10,effect1=50,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}			
		CidMatrix[16312]={type=10,effect1=300,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[16313]={type=10,effect1=1600,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}			
		CidMatrix[16314]={type=10,effect1=3600,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
	
		---//气血改变
		---//估计价格 ￥0.5/￥1.0/￥2.0/￥5.0/￥15.0/￥30.0/￥60.0
		CidMatrix[16029]={type=11,effect1=0,effect2=10,effect3=8,effect4=100,effect5=1,effect6=6,effect7=1}
		CidMatrix[16030]={type=11,effect1=0,effect2=20,effect3=12,effect4=100,effect5=2,effect6=15,effect7=2}
		CidMatrix[16031]={type=11,effect1=0,effect2=40,effect3=18,effect4=100,effect5=4,effect6=20,effect7=4}
		CidMatrix[16032]={type=11,effect1=0,effect2=100,effect3=20,effect4=100,effect5=10,effect6=20,effect7=8}
		CidMatrix[16033]={type=11,effect1=0,effect2=300,effect3=20,effect4=100,effect5=30,effect6=20,effect7=16}
		CidMatrix[16034]={type=11,effect1=0,effect2=600,effect3=20,effect4=100,effect5=60,effect6=20,effect7=32}
		CidMatrix[16035]={type=11,effect1=0,effect2=1200,effect3=20,effect4=100,effect5=120,effect6=20,effect7=64}
		CidMatrix[16036]={type=11,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16037]={type=11,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16038]={type=11,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16039]={type=11,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16040]={type=11,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16041]={type=11,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16042]={type=11,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16043]={type=11,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16044]={type=11,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16045]={type=11,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16046]={type=11,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16047]={type=11,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16048]={type=11,effect1=0,effect2=9999,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[23881]={type=11,effect1=0,effect2=40,effect3=12,effect4=100,effect5=4,effect6=20,effect7=4}

		---//真气改变
		CidMatrix[16049]={type=12,effect1=0,effect2=10,effect3=8,effect4=100,effect5=1,effect6=6,effect7=1}
		CidMatrix[16050]={type=12,effect1=0,effect2=20,effect3=12,effect4=100,effect5=2,effect6=15,effect7=2}
		CidMatrix[16051]={type=12,effect1=0,effect2=40,effect3=18,effect4=100,effect5=4,effect6=20,effect7=4}
		CidMatrix[16052]={type=12,effect1=0,effect2=100,effect3=20,effect4=100,effect5=10,effect6=20,effect7=8}
		CidMatrix[16053]={type=12,effect1=0,effect2=300,effect3=20,effect4=100,effect5=30,effect6=20,effect7=16}
		CidMatrix[16054]={type=12,effect1=0,effect2=600,effect3=20,effect4=100,effect5=60,effect6=20,effect7=32}
		CidMatrix[16055]={type=12,effect1=0,effect2=1200,effect3=20,effect4=100,effect5=120,effect6=20,effect7=64}
		CidMatrix[16056]={type=12,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16057]={type=12,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16058]={type=12,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16059]={type=12,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16060]={type=12,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16061]={type=12,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16062]={type=12,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16063]={type=12,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16064]={type=12,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16065]={type=12,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16066]={type=12,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16067]={type=12,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16068]={type=12,effect1=0,effect2=9999,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}

			---//最小攻击改变
		CidMatrix[16069]={type=13,effect1=0,effect2=10,effect3=8,effect4=100,effect5=1,effect6=6,effect7=1}
		CidMatrix[16070]={type=13,effect1=0,effect2=20,effect3=12,effect4=100,effect5=2,effect6=15,effect7=2}
		CidMatrix[16071]={type=13,effect1=0,effect2=40,effect3=18,effect4=100,effect5=4,effect6=20,effect7=4}
		CidMatrix[16072]={type=13,effect1=0,effect2=100,effect3=20,effect4=100,effect5=10,effect6=20,effect7=8}
		CidMatrix[16073]={type=13,effect1=0,effect2=300,effect3=20,effect4=100,effect5=30,effect6=20,effect7=16}
		CidMatrix[16074]={type=13,effect1=0,effect2=600,effect3=20,effect4=100,effect5=60,effect6=20,effect7=32}
		CidMatrix[16075]={type=13,effect1=0,effect2=1200,effect3=20,effect4=100,effect5=120,effect6=20,effect7=64}
		CidMatrix[16076]={type=13,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16077]={type=13,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16078]={type=13,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16079]={type=13,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16080]={type=13,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16081]={type=13,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16082]={type=13,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16083]={type=13,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16084]={type=13,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16085]={type=13,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16086]={type=13,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16087]={type=13,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16088]={type=13,effect1=0,effect2=9999,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}

		---//最大攻击改变
		CidMatrix[16089]={type=14,effect1=0,effect2=10,effect3=8,effect4=100,effect5=1,effect6=6,effect7=1}
		CidMatrix[16090]={type=14,effect1=0,effect2=20,effect3=12,effect4=100,effect5=2,effect6=15,effect7=2}
		CidMatrix[16091]={type=14,effect1=0,effect2=40,effect3=18,effect4=100,effect5=4,effect6=20,effect7=4}
		CidMatrix[16092]={type=14,effect1=0,effect2=100,effect3=20,effect4=100,effect5=10,effect6=20,effect7=8}
		CidMatrix[16093]={type=14,effect1=0,effect2=300,effect3=20,effect4=100,effect5=30,effect6=20,effect7=16}
		CidMatrix[16094]={type=14,effect1=0,effect2=600,effect3=20,effect4=100,effect5=60,effect6=20,effect7=32}
		CidMatrix[16095]={type=14,effect1=0,effect2=1200,effect3=20,effect4=100,effect5=120,effect6=20,effect7=64}
		CidMatrix[16096]={type=14,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16097]={type=14,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16098]={type=14,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16099]={type=14,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16100]={type=14,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16101]={type=14,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16102]={type=14,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16103]={type=14,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16104]={type=14,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16105]={type=14,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16106]={type=14,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16107]={type=14,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16108]={type=14,effect1=0,effect2=9999,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}

		---//防御改变
		CidMatrix[16109]={type=15,effect1=0,effect2=10,effect3=8,effect4=100,effect5=1,effect6=6,effect7=1}
		CidMatrix[16110]={type=15,effect1=0,effect2=20,effect3=12,effect4=100,effect5=2,effect6=15,effect7=2}
		CidMatrix[16111]={type=15,effect1=0,effect2=40,effect3=18,effect4=100,effect5=4,effect6=20,effect7=4}
		CidMatrix[16112]={type=15,effect1=0,effect2=100,effect3=20,effect4=100,effect5=10,effect6=20,effect7=8}
		CidMatrix[16113]={type=15,effect1=0,effect2=300,effect3=20,effect4=100,effect5=30,effect6=20,effect7=16}
		CidMatrix[16114]={type=15,effect1=0,effect2=600,effect3=20,effect4=100,effect5=60,effect6=20,effect7=32}
		CidMatrix[16115]={type=15,effect1=0,effect2=1200,effect3=20,effect4=100,effect5=120,effect6=20,effect7=64}
		CidMatrix[16116]={type=15,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16117]={type=15,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16118]={type=15,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16119]={type=15,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16120]={type=15,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16121]={type=15,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16122]={type=15,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16123]={type=15,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16124]={type=15,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16125]={type=15,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16126]={type=15,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16127]={type=15,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16128]={type=15,effect1=0,effect2=9999,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}

		---//命中改变
		CidMatrix[16129]={type=16,effect1=0,effect2=10,effect3=8,effect4=100,effect5=1,effect6=6,effect7=1}
		CidMatrix[16130]={type=16,effect1=0,effect2=20,effect3=12,effect4=100,effect5=2,effect6=15,effect7=2}
		CidMatrix[16131]={type=16,effect1=0,effect2=40,effect3=18,effect4=100,effect5=4,effect6=20,effect7=4}
		CidMatrix[16132]={type=16,effect1=0,effect2=100,effect3=20,effect4=100,effect5=10,effect6=20,effect7=8}
		CidMatrix[16133]={type=16,effect1=0,effect2=300,effect3=20,effect4=100,effect5=30,effect6=20,effect7=16}
		CidMatrix[16134]={type=16,effect1=0,effect2=600,effect3=20,effect4=100,effect5=60,effect6=20,effect7=32}
		CidMatrix[16135]={type=16,effect1=0,effect2=1200,effect3=20,effect4=100,effect5=120,effect6=20,effect7=64}
		CidMatrix[16136]={type=16,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16137]={type=16,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16138]={type=16,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16139]={type=16,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16140]={type=16,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16141]={type=16,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16142]={type=16,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16143]={type=16,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16144]={type=16,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16145]={type=16,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16146]={type=16,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16147]={type=16,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16148]={type=16,effect1=0,effect2=9999,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
	
		---//躲闪改变
		CidMatrix[16149]={type=17,effect1=0,effect2=10,effect3=8,effect4=100,effect5=1,effect6=6,effect7=1}
		CidMatrix[16150]={type=17,effect1=0,effect2=20,effect3=12,effect4=100,effect5=2,effect6=15,effect7=2}
		CidMatrix[16151]={type=17,effect1=0,effect2=40,effect3=18,effect4=100,effect5=4,effect6=20,effect7=4}
		CidMatrix[16152]={type=17,effect1=0,effect2=100,effect3=20,effect4=100,effect5=10,effect6=20,effect7=8}
		CidMatrix[16153]={type=17,effect1=0,effect2=300,effect3=20,effect4=100,effect5=30,effect6=20,effect7=16}
		CidMatrix[16154]={type=17,effect1=0,effect2=600,effect3=20,effect4=100,effect5=60,effect6=20,effect7=32}
		CidMatrix[16155]={type=17,effect1=0,effect2=1200,effect3=20,effect4=100,effect5=120,effect6=20,effect7=64}
		CidMatrix[16156]={type=17,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16157]={type=17,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16158]={type=17,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16159]={type=17,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16160]={type=17,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16161]={type=17,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16162]={type=17,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16164]={type=17,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16163]={type=17,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16165]={type=17,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16166]={type=17,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16167]={type=17,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16168]={type=17,effect1=0,effect2=9999,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}

		---//眩晕改变
		CidMatrix[16169]={type=18,effect1=0,effect2=10,effect3=8,effect4=100,effect5=1,effect6=6,effect7=1}
		CidMatrix[16170]={type=18,effect1=0,effect2=20,effect3=12,effect4=100,effect5=2,effect6=15,effect7=2}
		CidMatrix[16171]={type=18,effect1=0,effect2=40,effect3=18,effect4=100,effect5=4,effect6=20,effect7=4}
		CidMatrix[16172]={type=18,effect1=0,effect2=100,effect3=20,effect4=100,effect5=10,effect6=20,effect7=8}
		CidMatrix[16173]={type=18,effect1=0,effect2=300,effect3=20,effect4=100,effect5=30,effect6=20,effect7=16}
		CidMatrix[16174]={type=18,effect1=0,effect2=600,effect3=20,effect4=100,effect5=60,effect6=20,effect7=32}
		CidMatrix[16175]={type=18,effect1=0,effect2=1200,effect3=20,effect4=100,effect5=120,effect6=20,effect7=64}
		CidMatrix[16176]={type=18,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16177]={type=18,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16178]={type=18,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16179]={type=18,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16180]={type=18,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16181]={type=18,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16182]={type=18,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16183]={type=18,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16184]={type=18,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16185]={type=18,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16186]={type=18,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16187]={type=18,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16188]={type=18,effect1=0,effect2=9999,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}

		---//虚弱改变
		CidMatrix[16189]={type=19,effect1=0,effect2=10,effect3=8,effect4=100,effect5=1,effect6=6,effect7=1}
		CidMatrix[16190]={type=19,effect1=0,effect2=20,effect3=12,effect4=100,effect5=2,effect6=15,effect7=2}
		CidMatrix[16191]={type=19,effect1=0,effect2=40,effect3=18,effect4=100,effect5=4,effect6=20,effect7=4}
		CidMatrix[16192]={type=19,effect1=0,effect2=100,effect3=20,effect4=100,effect5=10,effect6=20,effect7=8}
		CidMatrix[16193]={type=19,effect1=0,effect2=300,effect3=20,effect4=100,effect5=30,effect6=20,effect7=16}
		CidMatrix[16194]={type=19,effect1=0,effect2=600,effect3=20,effect4=100,effect5=60,effect6=20,effect7=32}
		CidMatrix[16195]={type=19,effect1=0,effect2=1200,effect3=20,effect4=100,effect5=120,effect6=20,effect7=64}
		CidMatrix[16196]={type=19,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16197]={type=19,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16198]={type=19,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16199]={type=19,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16200]={type=19,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16201]={type=19,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16202]={type=19,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16203]={type=19,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16204]={type=19,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16205]={type=19,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16206]={type=19,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16207]={type=19,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16208]={type=19,effect1=0,effect2=9999,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}

		---//定身改变
		CidMatrix[16249]={type=20,effect1=0,effect2=10,effect3=8,effect4=100,effect5=1,effect6=6,effect7=1}
		CidMatrix[16250]={type=20,effect1=0,effect2=20,effect3=12,effect4=100,effect5=2,effect6=15,effect7=2}
		CidMatrix[16251]={type=20,effect1=0,effect2=40,effect3=18,effect4=100,effect5=4,effect6=20,effect7=4}
		CidMatrix[16252]={type=20,effect1=0,effect2=100,effect3=20,effect4=100,effect5=10,effect6=20,effect7=8}
		CidMatrix[16253]={type=20,effect1=0,effect2=300,effect3=20,effect4=100,effect5=30,effect6=20,effect7=16}
		CidMatrix[16254]={type=20,effect1=0,effect2=600,effect3=20,effect4=100,effect5=60,effect6=20,effect7=32}
		CidMatrix[16255]={type=20,effect1=0,effect2=1200,effect3=20,effect4=100,effect5=120,effect6=20,effect7=64}
		CidMatrix[16256]={type=20,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16257]={type=20,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16258]={type=20,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16259]={type=20,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16260]={type=20,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16261]={type=20,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16262]={type=20,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16263]={type=20,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16264]={type=20,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16265]={type=20,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16266]={type=20,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16267]={type=20,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16268]={type=20,effect1=0,effect2=9999,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}

		---//魅惑改变
		CidMatrix[16209]={type=21,effect1=0,effect2=10,effect3=8,effect4=100,effect5=1,effect6=6,effect7=1}
		CidMatrix[16210]={type=21,effect1=0,effect2=20,effect3=12,effect4=100,effect5=2,effect6=15,effect7=2}
		CidMatrix[16211]={type=21,effect1=0,effect2=40,effect3=18,effect4=100,effect5=4,effect6=20,effect7=4}
		CidMatrix[16212]={type=21,effect1=0,effect2=100,effect3=20,effect4=100,effect5=10,effect6=20,effect7=8}
		CidMatrix[16213]={type=21,effect1=0,effect2=300,effect3=20,effect4=100,effect5=30,effect6=20,effect7=16}
		CidMatrix[16214]={type=21,effect1=0,effect2=600,effect3=20,effect4=100,effect5=60,effect6=20,effect7=32}
		CidMatrix[16215]={type=21,effect1=0,effect2=1200,effect3=20,effect4=100,effect5=120,effect6=20,effect7=64}
		CidMatrix[16216]={type=21,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16217]={type=21,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16218]={type=21,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16219]={type=21,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16220]={type=21,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16221]={type=21,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16222]={type=21,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16223]={type=21,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16224]={type=21,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16225]={type=21,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16226]={type=21,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16227]={type=21,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16228]={type=21,effect1=0,effect2=9999,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}

		---//昏睡改变
		CidMatrix[16229]={type=22,effect1=0,effect2=10,effect3=8,effect4=100,effect5=1,effect6=6,effect7=1}
		CidMatrix[16230]={type=22,effect1=0,effect2=20,effect3=12,effect4=100,effect5=2,effect6=15,effect7=2}
		CidMatrix[16231]={type=22,effect1=0,effect2=40,effect3=18,effect4=100,effect5=4,effect6=20,effect7=4}
		CidMatrix[16232]={type=22,effect1=0,effect2=100,effect3=20,effect4=100,effect5=10,effect6=20,effect7=8}
		CidMatrix[16233]={type=22,effect1=0,effect2=300,effect3=20,effect4=100,effect5=30,effect6=20,effect7=16}
		CidMatrix[16234]={type=22,effect1=0,effect2=600,effect3=20,effect4=100,effect5=60,effect6=20,effect7=32}
		CidMatrix[16235]={type=22,effect1=0,effect2=1200,effect3=20,effect4=100,effect5=120,effect6=20,effect7=64}
		CidMatrix[16236]={type=22,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16237]={type=22,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16238]={type=22,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16239]={type=22,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16240]={type=22,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16241]={type=22,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16242]={type=22,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16243]={type=22,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16244]={type=22,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16245]={type=22,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16246]={type=22,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16247]={type=22,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16248]={type=22,effect1=0,effect2=9999,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}


		---//冰冻改变
		CidMatrix[16269]={type=23,effect1=0,effect2=10,effect3=8,effect4=100,effect5=1,effect6=6,effect7=1}
		CidMatrix[16270]={type=23,effect1=0,effect2=20,effect3=12,effect4=100,effect5=2,effect6=15,effect7=2}
		CidMatrix[16271]={type=23,effect1=0,effect2=40,effect3=18,effect4=100,effect5=4,effect6=20,effect7=4}
		CidMatrix[16272]={type=23,effect1=0,effect2=100,effect3=20,effect4=100,effect5=10,effect6=20,effect7=8}
		CidMatrix[16273]={type=23,effect1=0,effect2=300,effect3=20,effect4=100,effect5=30,effect6=20,effect7=16}
		CidMatrix[16274]={type=23,effect1=0,effect2=600,effect3=20,effect4=100,effect5=60,effect6=20,effect7=32}
		CidMatrix[16275]={type=23,effect1=0,effect2=1200,effect3=20,effect4=100,effect5=120,effect6=20,effect7=64}
		CidMatrix[16276]={type=23,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16277]={type=23,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16278]={type=23,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16279]={type=23,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16280]={type=23,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16281]={type=23,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16282]={type=23,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16283]={type=23,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16284]={type=23,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16285]={type=23,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16286]={type=23,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16287]={type=23,effect1=0,effect2=1,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[16288]={type=23,effect1=0,effect2=9999,effect3=20,effect4=0,effect5=0,effect6=0,effect7=0}

		---//随机战斗属性改变
		---//分别为15-60、61-120、121-150人物获得(3个转生前,3个转生后)
		CidMatrix[17588]={type=25,effect1=0,effect2=10,effect3=10,effect4=100,effect5=1,effect6=10,effect7=0}
		CidMatrix[17589]={type=25,effect1=0,effect2=20,effect3=10,effect4=100,effect5=2,effect6=10,effect7=0}
		CidMatrix[17590]={type=25,effect1=0,effect2=40,effect3=15,effect4=100,effect5=4,effect6=15,effect7=0}
		CidMatrix[17591]={type=25,effect1=0,effect2=15,effect3=15,effect4=100,effect5=2,effect6=15,effect7=0}
		CidMatrix[17592]={type=25,effect1=0,effect2=25,effect3=15,effect4=100,effect5=4,effect6=15,effect7=0}
		CidMatrix[17593]={type=25,effect1=0,effect2=50,effect3=20,effect4=100,effect5=8,effect6=20,effect7=0}
		
		---//主类型改变(限界)改变
		CidMatrix[16320]={type=32,effect1=0,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}		

		---//形象改变药剂(因星等改变的药剂)
		---//effect1(最低使用星等)/effect2(指向的shape)/effect3(指向的shape(变异情况下)
		CidMatrix[17782]={type=33,effect1=1,effect2=1,effect3=2,effect4=0,effect5=0,effect6=0,effect7=0}		
		CidMatrix[17819]={type=33,effect1=12,effect2=3,effect3=4,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[17820]={type=33,effect1=18,effect2=5,effect3=6,effect4=0,effect5=0,effect6=0,effect7=0}
	
		---//变异药剂(宠物发生变异的药剂)
		CidMatrix[18726]={type=34,effect1=0,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		
		---//成长观察药剂(美工专用)
		CidMatrix[18725]={type=35,effect1=0,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}

		---//轮回改变
		CidMatrix[19183]={type=36,effect1=0,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}		

		---//神秘豆汁(固定值攻击和防御成长)
		CidMatrix[20481]={type=37,effect1=1,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}		
		CidMatrix[20483]={type=37,effect1=10,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20484]={type=37,effect1=50,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[23880]={type=37,effect1=1,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
				
		---//舍利强化(人物修炼佛产生特殊物品)
		CidMatrix[20048]={type=38,effect1=1,effect2=1,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}		
		CidMatrix[20049]={type=38,effect1=1,effect2=2,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20050]={type=38,effect1=1,effect2=3,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}			
		CidMatrix[20051]={type=38,effect1=1,effect2=4,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20052]={type=38,effect1=1,effect2=5,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20053]={type=38,effect1=1,effect2=6,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20054]={type=38,effect1=1,effect2=7,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20055]={type=38,effect1=1,effect2=8,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20056]={type=38,effect1=1,effect2=9,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20057]={type=38,effect1=1,effect2=10,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20058]={type=38,effect1=1,effect2=11,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20059]={type=38,effect1=1,effect2=12,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20060]={type=38,effect1=1,effect2=13,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20061]={type=38,effect1=1,effect2=14,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20062]={type=38,effect1=1,effect2=15,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20063]={type=38,effect1=1,effect2=16,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20064]={type=38,effect1=1,effect2=17,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20065]={type=38,effect1=1,effect2=18,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20066]={type=38,effect1=1,effect2=19,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20067]={type=38,effect1=1,effect2=20,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		
		CidMatrix[20068]={type=38,effect1=2,effect2=1,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20069]={type=38,effect1=2,effect2=2,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20070]={type=38,effect1=2,effect2=3,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20071]={type=38,effect1=2,effect2=4,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20072]={type=38,effect1=2,effect2=5,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20073]={type=38,effect1=2,effect2=6,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20074]={type=38,effect1=2,effect2=7,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20075]={type=38,effect1=2,effect2=8,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20076]={type=38,effect1=2,effect2=9,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20077]={type=38,effect1=2,effect2=10,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20078]={type=38,effect1=2,effect2=11,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[20079]={type=38,effect1=2,effect2=12,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20080]={type=38,effect1=2,effect2=13,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20081]={type=38,effect1=2,effect2=14,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20082]={type=38,effect1=2,effect2=15,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[20083]={type=38,effect1=2,effect2=16,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[20084]={type=38,effect1=2,effect2=17,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20085]={type=38,effect1=2,effect2=18,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20086]={type=38,effect1=2,effect2=19,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20087]={type=38,effect1=2,effect2=20,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}											
		
		---//宠物魂化Get药剂(临时id，占用了成长观察药剂)
		CidMatrix[22990]={type=39,effect1=0,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}

		---//宠物魂化Set药剂(临时id，占用了成长观察药剂)
		CidMatrix[22992]={type=40,effect1=16503,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[22993]={type=40,effect1=16504,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[22994]={type=40,effect1=16505,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[22995]={type=40,effect1=16506,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[22996]={type=40,effect1=16507,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[22997]={type=40,effect1=16508,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[22998]={type=40,effect1=18306,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[22999]={type=40,effect1=18929,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[23000]={type=40,effect1=19510,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[23001]={type=40,effect1=20108,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[23002]={type=40,effect1=19110,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[23003]={type=40,effect1=20488,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[23004]={type=40,effect1=20489,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[23005]={type=40,effect1=20561,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[23006]={type=40,effect1=22105,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[23007]={type=40,effect1=22484,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[23008]={type=40,effect1=21247,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		CidMatrix[23338]={type=40,effect1=23337,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}		

		---//宠物测试药剂（随需求改变用途--当期生成150级20星）		
		CidMatrix[15846]={type=41,effect1=0,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}		

		---//宠物洗技能药剂		
		CidMatrix[62254]={type=42,effect1=0,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}		

		---//宠物洗技能药剂		
		CidMatrix[62262]={type=43,effect1=0,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}		
---		CidMatrix[15846]={type=43,effect1=0,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}		
		return CidMatrix
end 

---宠物逆天重生-----------------------------------------------------------------------------------
function ZLuaPetBedgeHolyReborn(pet_tid,pet_in,pet_out,cid,aid,origin)
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local petstar=pet_in:QueryStar()
	local petlevel=pet_in:QueryLevel()
	if petstar>5 then 
		return 0,0,88
	end 
	local DataIndex=ZLuaPetBedgeDataIndex()
	PetBedgeInit(pet_tid,pet_out,origin)
	pet_out:UpdateData(DataIndex["identify"], 1)
	pet_out:SetLevel(petlevel)
	return pet_tid,0,1
end 
---宠物鉴定----------------------------------------------------------------------------------------
function ZLuaPetBedgeIdentify(pet_tid,pet_in,pet_out,cid,aid,origin)
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local isidentify=pet_in:QueryIdentify()
	if isidentify==0 then 
		pet_out:UpdateData(DataIndex["identify"], 1)
		return pet_tid,0,1
	else 
		---//宠物已经鉴定
		return 0,0,2	
	end 		
end 
---宠物改变年龄------------------------------------------------------------------------------------
function ZLuaPetBedgeChangeAge(pet_tid,pet_in,pet_out,cid,aid,origin)
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local CidMatrix=ZLuaPetBedgeCidMatrix()
	
	local howmanyage=pet_in:QueryAge()
	howmanyage=math.min(math.max(howmanyage-CidMatrix[cid].effect1,1),pet_in:QueryLife())
	pet_out:SetAge(howmanyage)

	return pet_tid,0,3
end 
---宠物改变寿命------------------------------------------------------------------------------------
function ZLuaPetBedgeChangeLife(pet_tid,pet_in,pet_out,cid,aid,origin)
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local CidMatrix=ZLuaPetBedgeCidMatrix()
	
	local howmanylife=pet_in:QueryLife()
	if howmanylife<25000 then 
		howmanylife=math.min(howmanylife+CidMatrix[cid].effect1,25000)
		pet_out:SetLife(howmanylife)
		return pet_tid,0,4
	else 
		return 0,0,5	 
	end 
	
end 
---宠物改变五行(性格)------------------------------------------------------------------------------
function ZLuaPetBedgeChangeEmement(pet_tid,pet_in,pet_out,cid,aid,origin)
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local CidMatrix=ZLuaPetBedgeCidMatrix()
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local Emement=ZLuaPetBedgeElementDesc()
	local cidelement=CidMatrix[cid].effect1
	local newelement=pet_in:QueryElement()
	
	---与当前性格相同则失败
	if pet_in:QueryElement()==cidelement and cidelement~=99 then 
		return 0,0,6
	end 
	if cidelement==99 then 
		newelement=ZLuaPetBedgeElementInfo(pet_tid) 
	else
		newelement=cidelement
	end 
 	
 	pet_out:UpdateData(DataIndex["element"], newelement)
	return pet_tid,0,7
end 

---宠物改变天赋------------------------------------------------------------------------------------
function ZLuaPetBedgeChangeTalent(pet_tid,pet_in,pet_out,cid,aid,origin)
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local CidMatrix=ZLuaPetBedgeCidMatrix()
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local TalentsChoosed
	local TalentsList={}	
		TalentsList[1]={10,5,0,0,0,0,0,0,0,0,0,5,0}
		TalentsList[2]={5,10,5,0,0,0,0,0,0,0,0,0,5}
		TalentsList[3]={0,5,10,5,0,0,0,0,0,0,0,0,0}
		TalentsList[4]={0,0,5,10,5,0,0,0,0,0,0,0,0}
		TalentsList[5]={0,0,0,5,10,5,0,0,0,0,0,0,0}
		TalentsList[6]={0,0,0,0,5,10,5,0,0,0,0,0,5}
		TalentsList[7]={0,0,0,0,0,5,10,5,0,0,0,0,0}
		TalentsList[8]={0,0,0,0,0,0,5,10,5,0,0,0,0}
		TalentsList[9]={0,0,0,0,0,0,0,5,10,5,0,0,0}
		TalentsList[10]={0,0,0,0,0,0,0,0,5,10,5,0,5}
		TalentsList[11]={0,0,0,0,0,0,0,0,0,5,10,5,0}
		TalentsList[12]={5,0,0,0,0,0,0,0,0,0,5,10,0}
		TalentsList[13]={0,5,0,0,0,5,0,0,0,5,0,0,10}

		---//13以下为变异宠物特质
		TalentsList[14]={10,0,5,0,0,0,0,0,0,0,5,0,0}
		TalentsList[15]={0,10,0,5,0,0,0,0,0,0,0,5,0}
		TalentsList[16]={5,0,10,0,5,0,0,0,0,0,0,0,0}
		TalentsList[17]={0,5,0,10,0,5,0,0,0,0,0,0,0}
		TalentsList[18]={0,0,5,0,10,0,5,0,0,0,0,0,0}
		TalentsList[19]={0,0,0,5,0,10,0,5,0,0,0,0,0}
		TalentsList[20]={0,0,0,0,5,0,10,0,5,0,0,0,0}
		TalentsList[21]={0,0,0,0,0,5,0,10,0,5,0,0,0}
		TalentsList[22]={0,0,0,0,0,0,5,0,10,0,5,0,0}
		TalentsList[23]={0,0,0,0,0,0,0,5,0,10,0,5,0}
		TalentsList[24]={0,0,0,0,0,0,0,0,5,0,10,0,5}
		TalentsList[25]={0,5,0,0,0,0,0,0,0,5,0,10,0}
		TalentsList[26]={0,0,0,5,0,0,0,5,0,0,0,5,10}
		
	local c=CidMatrix[cid].effect1
	if c>1 and c<=13 then TalentsChoosed=c end 
	if c==14 then TalentsChoosed=math.floor(math.random()*13+1) end 
	if c~=15 and (TalentsChoosed>13 or TalentsChoosed<1) then TalentsChoosed=1 end 
	if c==15 then TalentsChoosed=math.floor(math.random()*table.getn(TalentsList)+1) end 
	if c==15 and 	TalentsChoosed>table.getn(TalentsList) then TalentsChoosed=1 end 
	
	pet_out:UpdateData(DataIndex["talent_1"], TalentsList[TalentsChoosed][1] or 0)	
	pet_out:UpdateData(DataIndex["talent_2"], TalentsList[TalentsChoosed][2] or 0)	
	pet_out:UpdateData(DataIndex["talent_3"], TalentsList[TalentsChoosed][3] or 0)	
	pet_out:UpdateData(DataIndex["talent_4"], TalentsList[TalentsChoosed][4] or 0)	
	pet_out:UpdateData(DataIndex["talent_5"], TalentsList[TalentsChoosed][5] or 0)	
	pet_out:UpdateData(DataIndex["talent_6"], TalentsList[TalentsChoosed][6] or 0)	
	pet_out:UpdateData(DataIndex["talent_7"], TalentsList[TalentsChoosed][7] or 0)	
	pet_out:UpdateData(DataIndex["talent_8"], TalentsList[TalentsChoosed][8] or 0)	
	pet_out:UpdateData(DataIndex["talent_9"], TalentsList[TalentsChoosed][9] or 0)	
	pet_out:UpdateData(DataIndex["talent_10"], TalentsList[TalentsChoosed][10] or 0)	
	pet_out:UpdateData(DataIndex["talent_11"], TalentsList[TalentsChoosed][11] or 0)	
	pet_out:UpdateData(DataIndex["talent_12"], TalentsList[TalentsChoosed][12] or 0)	
	pet_out:UpdateData(DataIndex["talent_13"], TalentsList[TalentsChoosed][13] or 0)
	pet_out:UpdateData(DataIndex["isspecial"], 0)

	return pet_tid,0,8
	
end 
---宠物学习技能-------------------------------------------------------------------------------------
function ZLuaPetBedgeLearnSkill(pet_tid,pet_in,pet_out,cid,aid,origin)
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local CidMatrix=ZLuaPetBedgeCidMatrix()
	local AidMatrix=ZLuaPetBedgeAidMatrix()
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local Horoscope=ZLuaPetBedgeHoroscopeDesc()
	local Race=ZLuaPetBedgeRaceDesc()
	---//type=7	 	宠物学习技能,effect1(适合职业0-8,99通用),effect2(技能指向id),effect3(追加技能级别)
	---//					effect4(基础成功概率>100,实际成功率为100-本体级别)
	
	---可能的技能学习条件限制
	local pet_level,pet_horoscope,pet_race,pet_shape
		pet_level=pet_in:QueryLevel()
		pet_race=pet_in:QueryData(DataIndex["race"])	
		pet_horoscope=pet_in:QueryData(DataIndex["horoscope"])	
		pet_shape=pet_in:QueryData(DataIndex["shape"])	
		
	---//星相不满足学习条件
	if pet_horoscope~=CidMatrix[cid].effect1 and CidMatrix[cid].effect1~=99 then 
		return 0,0,9
	end 
	
	local petskill_id,petskill_lvl={},{}
	---//只对第2-8个技能才可能使用技能卷轴
	petskill_id[1]=pet_in:QueryData(DataIndex["skillid_1"]) or 0	
	petskill_id[2]=pet_in:QueryData(DataIndex["skillid_2"])	or 0
	petskill_id[3]=pet_in:QueryData(DataIndex["skillid_3"])	or 0
	petskill_id[4]=pet_in:QueryData(DataIndex["skillid_4"])	or 0
	petskill_id[5]=pet_in:QueryData(DataIndex["skillid_5"])	or 0
	petskill_id[6]=pet_in:QueryData(DataIndex["skillid_6"])	or 0
	petskill_id[7]=pet_in:QueryData(DataIndex["skillid_7"])	or 0
	
	petskill_lvl[1]=pet_in:QueryData(DataIndex["skilllvl_1"])	or 0
	petskill_lvl[2]=pet_in:QueryData(DataIndex["skilllvl_2"])	or 0
	petskill_lvl[3]=pet_in:QueryData(DataIndex["skilllvl_3"])	or 0
	petskill_lvl[4]=pet_in:QueryData(DataIndex["skilllvl_4"])	or 0
	petskill_lvl[5]=pet_in:QueryData(DataIndex["skilllvl_5"])	or 0
	petskill_lvl[6]=pet_in:QueryData(DataIndex["skilllvl_6"])	or 0
	petskill_lvl[7]=pet_in:QueryData(DataIndex["skilllvl_7"])	or 0
	
	local TotalSkills=0					---//已有技能总数
	local IsExistSameSkill=0		---//是否存在相同技能
	local IsExistPosition=0			---//是否有空白位置
	local BlankPostion=0				---//空白位置所在
	local UsingAidType=99				---//使用的aid的种类
	
	UsingAidType=AidMatrix[aid].type or 99
	local CidItemLevel=0					---//技能书的档次
	if CidMatrix[cid].effect5 == 0 then CidItemLevel = 0
		elseif CidMatrix[cid].effect5 == 1 then CidItemLevel = 1
		elseif CidMatrix[cid].effect5 == 2 then CidItemLevel = 1
	end
---	CidItemLevel=CidMatrix[cid].effect5
	local CidItemLevel2=0					---//技能书的档次2
	if CidMatrix[cid].effect5 == 0 then CidItemLevel2 = 30
		elseif CidMatrix[cid].effect5 == 1 then CidItemLevel2 = 60
		elseif CidMatrix[cid].effect5 == 2 then CidItemLevel2 = 69
	end
		
	for i=1,7 do
		if petskill_id[i]==CidMatrix[cid].effect2 then 
				IsExistSameSkill=1
		---//存在相同技能
		---//使用了技能修炼辅助道具：必须使用宠物不会的技能卷且会增加技能栏数
				if UsingAidType==1 and aid~=0 then
					return 0,0,94
				end  
				if ZPetBedge_ChanceCalc(CidMatrix[cid].effect4-petskill_lvl[i])==1 and petskill_lvl[i]<=CidItemLevel2 then 
					petskill_lvl[i]=petskill_lvl[i]+1
					pet_out:UpdateData(DataIndex["skilllvl_1"],petskill_lvl[1])
					pet_out:UpdateData(DataIndex["skilllvl_2"],petskill_lvl[2])
					pet_out:UpdateData(DataIndex["skilllvl_3"],petskill_lvl[3])
					pet_out:UpdateData(DataIndex["skilllvl_4"],petskill_lvl[4])
					pet_out:UpdateData(DataIndex["skilllvl_5"],petskill_lvl[5])
					pet_out:UpdateData(DataIndex["skilllvl_6"],petskill_lvl[6])
					pet_out:UpdateData(DataIndex["skilllvl_7"],petskill_lvl[7])
					return pet_tid,0,10	
				else
					if petskill_lvl[i]>30 and CidItemLevel==0 then 
						return 0,0,11
					elseif petskill_lvl[i]>60 and CidItemLevel==1 then 
						return 0,0,97
					else
						return 0,0,12
					end 
				end 
		end
		if petskill_id[i]~=0 and petskill_id[i]~=nil then
			TotalSkills=TotalSkills+1
		end 
	end 	
	---//不存在相同技能
	local msg_no_exsit_case=13
	---//简单替代逻辑
	if cid > 60000 then if pet_in:QueryRebornCnt()==0 then return 0,0,215 end end	---简单处理飞升技能书仅用于飞升宠物,爱谁谁来弄
	if IsExistSameSkill==0 and UsingAidType~=1 then  
			local RandomPostion=math.max(math.min(math.floor(math.random()*TotalSkills+1),TotalSkills),1)
			petskill_id[RandomPostion]=CidMatrix[cid].effect2
			petskill_lvl[RandomPostion]=CidMatrix[cid].effect3			
			msg_no_exsit_case=14
		pet_out:UpdateData(DataIndex["skillid_1"],petskill_id[1])
		pet_out:UpdateData(DataIndex["skillid_2"],petskill_id[2])
		pet_out:UpdateData(DataIndex["skillid_3"],petskill_id[3])
		pet_out:UpdateData(DataIndex["skillid_4"],petskill_id[4])
		pet_out:UpdateData(DataIndex["skillid_5"],petskill_id[5])
		pet_out:UpdateData(DataIndex["skillid_6"],petskill_id[6])
		pet_out:UpdateData(DataIndex["skillid_7"],petskill_id[7])	
				
		pet_out:UpdateData(DataIndex["skilllvl_1"],petskill_lvl[1])
		pet_out:UpdateData(DataIndex["skilllvl_2"],petskill_lvl[2])
		pet_out:UpdateData(DataIndex["skilllvl_3"],petskill_lvl[3])
		pet_out:UpdateData(DataIndex["skilllvl_4"],petskill_lvl[4])
		pet_out:UpdateData(DataIndex["skilllvl_5"],petskill_lvl[5])
		pet_out:UpdateData(DataIndex["skilllvl_6"],petskill_lvl[6])
		pet_out:UpdateData(DataIndex["skilllvl_7"],petskill_lvl[7])
		
		return pet_tid,0,msg_no_exsit_case
	---//简单增加逻辑
	elseif IsExistSameSkill==0 and UsingAidType==1 and aid~=0 then 
		if TotalSkills>=7 then 
			return 0,0,96	
		elseif pet_in:QueryData(DataIndex["reserve1"])==1 then 
			return 0,0,207		
		else 
			if 	TotalSkills==1 then 
				pet_out:UpdateData(DataIndex["skillid_2"],CidMatrix[cid].effect2)	
				pet_out:UpdateData(DataIndex["skilllvl_2"],CidMatrix[cid].effect3)
			 	pet_out:UpdateData(DataIndex["reserve1"], 1)
			elseif TotalSkills==2 then
				pet_out:UpdateData(DataIndex["skillid_3"],CidMatrix[cid].effect2)	
				pet_out:UpdateData(DataIndex["skilllvl_3"],CidMatrix[cid].effect3)
				pet_out:UpdateData(DataIndex["reserve1"], 1)
			elseif TotalSkills==3 then
				pet_out:UpdateData(DataIndex["skillid_4"],CidMatrix[cid].effect2)	
				pet_out:UpdateData(DataIndex["skilllvl_4"],CidMatrix[cid].effect3)
				pet_out:UpdateData(DataIndex["reserve1"], 1)
			elseif TotalSkills==4 then
				pet_out:UpdateData(DataIndex["skillid_5"],CidMatrix[cid].effect2)	
				pet_out:UpdateData(DataIndex["skilllvl_5"],CidMatrix[cid].effect3)
				pet_out:UpdateData(DataIndex["reserve1"], 1)
			elseif TotalSkills==5 then
				pet_out:UpdateData(DataIndex["skillid_6"],CidMatrix[cid].effect2)	
				pet_out:UpdateData(DataIndex["skilllvl_6"],CidMatrix[cid].effect3)
				pet_out:UpdateData(DataIndex["reserve1"], 1)
			elseif TotalSkills==6 then
				pet_out:UpdateData(DataIndex["skillid_7"],CidMatrix[cid].effect2)	
				pet_out:UpdateData(DataIndex["skilllvl_7"],CidMatrix[cid].effect3)
				pet_out:UpdateData(DataIndex["reserve1"], 1)
			else
				return 0,0,96	
			end 
			return pet_tid,0,95
		end 
	else 
		return 0,0,96	
	end 
	
end 
---宠物遗忘技能------------------------------------------------------------------------------------
---//此功能尚未开放
function ZLuaPetBedgeForgetSkill(pet_tid,pet_in,pet_out,cid,aid,origin)
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local CidMatrix=ZLuaPetBedgeCidMatrix()
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()	
	local positonchoosed
	return pet_tid,0,99
end 
---宠物更改亲密度当前值----------------------------------------------------------------------------
function ZLuaPetBedgeChangeCurHonor(pet_tid,pet_in,pet_out,cid,aid,origin)
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local CidMatrix=ZLuaPetBedgeCidMatrix()
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	if pet_in:QueryCurHonorPoint()>=pet_in:QueryMaxHonorPoint()*CidMatrix[cid].effect2 then 
		return 0,0,15
	else 
		pet_out:SetCurHonorPoint(math.min(pet_in:QueryCurHonorPoint()+CidMatrix[cid].effect1,pet_in:QueryMaxHonorPoint()))
		return pet_tid,0,16
	end 
end 
---宠物更改亲密度上限------------------------------------------------------------------------------
function ZLuaPetBedgeChangeMaxHonor(pet_tid,pet_in,pet_out,cid,aid,origin)
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local CidMatrix=ZLuaPetBedgeCidMatrix()
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local preMaxHornorPoint
	if pet_in:QueryMaxHonorPoint()>=57600 then 
		return 0,0,17
	else 
		preMaxHonorPoint=pet_in:QueryMaxHonorPoint()+CidMatrix[cid].effect1
		pet_out:SetMaxHonorPoint(math.min(preMaxHonorPoint,57600))	
		return pet_tid,0,18
	end 	

end 
---宠物更改战斗属性的成功概率基础值序列------------------------------------------------------------
function ZLuaPetBedgeCombatGrowChance(pet,poisongrowtype,level)
	if level<0 or level>160 then return end 
	local BaseCombineChance={}
	local pseudogrades=ZLuaPetBedgeSuperPseudoGrades()
	local nowgrades,nowposition=pseudogrades[1],1
	---当前成长度,成长度所在的档位
	local DataIndex=ZLuaPetBedgeDataIndex() 
	---//此处类型号为生写,注意修改
	if poisongrowtype==11 then 
		nowgrades=pet:QueryData(DataIndex["potential_hp"])
	elseif poisongrowtype==12 then 
		nowgrades=pet:QueryData(DataIndex["potential_mp"])
	elseif poisongrowtype==13 then 
		nowgrades=pet:QueryData(DataIndex["potential_minatk"])
	elseif poisongrowtype==14 then 
		nowgrades=pet:QueryData(DataIndex["potential_maxatk"])
	elseif poisongrowtype==15 then 
		nowgrades=pet:QueryData(DataIndex["potential_def"])
	elseif poisongrowtype==16 then 	
		nowgrades=pet:QueryData(DataIndex["potential_attackrate"])
	elseif poisongrowtype==17 then 
		nowgrades=pet:QueryData(DataIndex["potential_dodge"])
	elseif poisongrowtype==18 then 
		nowgrades=pet:QueryData(DataIndex["potential_resist1"])
	elseif poisongrowtype==19 then 
		nowgrades=pet:QueryData(DataIndex["potential_resist2"])
	elseif poisongrowtype==20 then
		nowgrades=pet:QueryData(DataIndex["potential_resist3"])
	elseif poisongrowtype==21 then 
		nowgrades=pet:QueryData(DataIndex["potential_resist4"])
	elseif poisongrowtype==22 then	
		nowgrades=pet:QueryData(DataIndex["potential_resist5"])
	elseif poisongrowtype==23 then
		nowgrades=pet:QueryData(DataIndex["potential_resist6"])
	else
		nowgrades=pseudogrades[1]
	end 
	for i=1,table.getn(pseudogrades)-1 do
		if nowgrades>=pseudogrades[i] and nowgrades<pseudogrades[i+1] then 
			nowposition=i	
		elseif nowgrades>=pseudogrades[table.getn(pseudogrades)] then 
			nowposition=table.getn(pseudogrades)
		end 
	end 

	if nowposition<1 or nowposition>table.getn(pseudogrades) then
		nowposition=table.getn(pseudogrades)
	end 
	BaseCombineChance={99,95,90,85,80,75,75,70,70,65,
										 60,55,50,45,40,35,30,25,25,25,
										 20,20,20,20,15,15,15,10,10,10,
										 5,5,5,5,5,5,5,5,5,5,
										 5,5,5,5,5,5,5,5,5,5,
										 5,5,5,5,5,5,5,5,5,5}	
	return BaseCombineChance[nowposition]
end 

---宠物更改战斗相关属性值--------------------------------------------------------------------------
function ZLuaPetBedgeCombatPropDeal(pet_tid,pet_in,pet_out,cid,aid,origin,usetype)
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if type(cid)~="number" or type(aid)~="number" then return end 
	if pet_in == nil then return end

	local CidMatrix=ZLuaPetBedgeCidMatrix()
	local AidMatrix=ZLuaPetBedgeAidMatrix()
	local DataIndex=ZLuaPetBedgeDataIndex()
	local GradeDirection=ZLuaPetBedgeGradeDirection()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	
	local NativeStar,NativeLevel,NativeGrowChance
	local PoisonGrowType,PoisonBaseGrowChance,PoisonBaseGrowEffect
	local PoisonUseMaxStar
		---//宠物星等/宠物级别/宠物因对应grades和级别的基础合成成功率
		---//药提供的成长类/药提供的额外成功率/药提供的基础效果
		---//宠物道具可使用的最大星等
	NativeStar=pet_in:QueryData(DataIndex["star"])
	NativeLevel=pet_in:QueryLevel()
	
	if usetype==1 then
		---初次成长使用数据
		PoisonGrowType=CidMatrix[cid].type
		PoisonBaseGrowChance=CidMatrix[cid].effect1
		PoisonBaseGrowEffect=CidMatrix[cid].effect2
		PoisonUseMaxStar=CidMatrix[cid].effect3
	elseif usetype>1 and type(usetype)=="number" then 
		---二次成长使用数据
		PoisonGrowType=usetype
		PoisonBaseGrowChance=CidMatrix[cid].effect4
		PoisonBaseGrowEffect=CidMatrix[cid].effect5
		PoisonUseMaxStar=CidMatrix[cid].effect6
	else
		---二次成长使用数据
		PoisonGrowType=CidMatrix[cid].type
		PoisonBaseGrowChance=0
		PoisonBaseGrowEffect=0
		PoisonUseMaxStar=20
	end 

	---//修炼失败获得的微量共用类增益
	local PosionFailedSmallGrow=1
	PosionFailedSmallGrow=CidMatrix[cid].effect7
	---//当前累计的微量共用类增益系数
	local NowSmallGrow=math.floor(pet_in:QueryData(DataIndex["smallgrow"])/10)
	
	
	local ItemAddGrowChance,ItemGrowEffectInc
	---//道具提供的额外成功率,道具提供的效果加成
	ItemAddGrowChance=AidMatrix[aid].effect1
	ItemGrowEffectInc=AidMatrix[aid].effect2
	
	
	local RaceAddGrowChance=0
	---//种族提供的额外成功率
	local Race=ZLuaPetBedgeRaceDesc()
	local race=pet_in:QueryData(DataIndex["race"])
	if race==Race["renzu"] then 
		RaceAddGrowChance=3 
	end 
		
	local MainType,Element,Origin,Horoscope
	---宠物的主类型/五行/来源
	MainType=pet_in:QueryMainType()
	Element=pet_in:QueryElement()
	Horoscope=pet_in:QueryHoroscope()
	Origin=pet_in:QueryData(DataIndex["nativeorigin"])
	
	local MainTypeGrowEffectInc=0
	---//宠物主类型导致的成长增益	
	MainTypeGrowEffectInc=ZLuaPetBedgeMaintypeGrowEffect(MainType,PoisonGrowType)
	
	---宠物的主要属性成长上限
	local MaxGrowGrades={}
	MaxGrowGrades=ZLuaPetBedgeMaxGrowGrades(pet_tid,MainType,Horoscope,Origin)
	
	---使用辅助道具不正确
	if aid~=0 and AidMatrix[aid].type~=0 then 
		return 0,0,208
	end 	
	
	---宠物星等超过物品的最大可使用星等
	if PoisonUseMaxStar<=NativeStar and PoisonUseMaxStar~=20 then 
		return 0,0,19		
	end 	
	
	---随机成长则随机选取成长类型
	if PoisonGrowType==25 then 
		PoisonGrowType=ZLuaChooseNormalNums(11,24,1) 
	end 
	
	local TotalGrowChance,TotalGrowEffect,TotalGrowCalc
	---//总成功率(转化后),道具提供的总效果,总成功率计算出的判断因子
	NativeGrowChance=ZLuaPetBedgeCombatGrowChance(pet_in,PoisonGrowType,NativeLevel)	
	TotalGrowChance=NativeGrowChance+PoisonBaseGrowChance+ItemAddGrowChance+RaceAddGrowChance
	TotalGrowCalc=ZPetBedge_ChanceCalc(TotalGrowChance)
	TotalGrowEffect=PoisonBaseGrowEffect*(1+ItemGrowEffectInc+MainTypeGrowEffectInc)
	
	if PoisonGrowType==11 then 
	---//type=11	宠物属性成长(气血),effect1(概率1-100),effect2(增加值)
		if pet_in:QueryData(DataIndex["potential_hp"])<MaxGrowGrades[GradeDirection["potential_hp"]] then 
			if TotalGrowCalc==1 then 
				pet_out:UpdateData(DataIndex["potential_hp"],math.min(pet_in:QueryData(DataIndex["potential_hp"])+TotalGrowEffect,
				MaxGrowGrades[GradeDirection["potential_hp"]]))
				pet_out:UpdateData(DataIndex["feedtimes"], pet_in:QueryData(DataIndex["feedtimes"])+1)	
				return pet_tid,0,20
			else
				if pet_in:QueryData(DataIndex["smallgrow"])>=10 then 
					pet_out:UpdateData(DataIndex["potential_hp"],math.min(pet_in:QueryData(DataIndex["potential_hp"])+NowSmallGrow,
					MaxGrowGrades[GradeDirection["potential_hp"]]))
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])-10*NowSmallGrow)
				else
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])+PosionFailedSmallGrow)
				end 
				return pet_tid,0,21
			end 		
		else
				return 0,0,22			
		end 
	elseif PoisonGrowType==12 then 
		---//type=12	宠物属性成长(真气),effect1(概率1-100),effect2(增加值)
		if pet_in:QueryData(DataIndex["potential_mp"])<MaxGrowGrades[GradeDirection["potential_mp"]] then 
			if TotalGrowCalc==1 then 
				pet_out:UpdateData(DataIndex["potential_mp"],math.min(pet_in:QueryData(DataIndex["potential_mp"])+TotalGrowEffect,
				MaxGrowGrades[GradeDirection["potential_mp"]]))
				pet_out:UpdateData(DataIndex["feedtimes"], pet_in:QueryData(DataIndex["feedtimes"])+1)	
				return pet_tid,0,23
			else
				if pet_in:QueryData(DataIndex["smallgrow"])>=10 then 
					pet_out:UpdateData(DataIndex["potential_mp"],math.min(pet_in:QueryData(DataIndex["potential_mp"])+NowSmallGrow,
					MaxGrowGrades[GradeDirection["potential_mp"]]))
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])-10*NowSmallGrow)
				else
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])+PosionFailedSmallGrow)
				end
				return pet_tid,0,24
			end 		
		else
				return 0,0,25			
		end 
	elseif PoisonGrowType==13 then
	---//type=13	宠物属性成长(最小攻击),effect1(概率1-100),effect2(增加值)
		if pet_in:QueryData(DataIndex["potential_minatk"])<MaxGrowGrades[GradeDirection["potential_minatk"]] and
		 pet_in:QueryData(DataIndex["potential_minatk"])<pet_in:QueryData(DataIndex["potential_maxatk"]) then 
			if TotalGrowCalc==1 then 
				if pet_in:QueryData(DataIndex["potential_minatk"])>=pet_in:QueryData(DataIndex["potential_maxatk"]) then
					return 0,0,26
				else
				pet_out:UpdateData(DataIndex["potential_minatk"],math.min(pet_in:QueryData(DataIndex["potential_minatk"])+TotalGrowEffect,
				MaxGrowGrades[GradeDirection["potential_minatk"]],pet_in:QueryData(DataIndex["potential_maxatk"])))
				pet_out:UpdateData(DataIndex["feedtimes"], pet_in:QueryData(DataIndex["feedtimes"])+1)	
					return pet_tid,0,27
				end 
			else
				if pet_in:QueryData(DataIndex["smallgrow"])>=10 then 
					pet_out:UpdateData(DataIndex["potential_minatk"],math.min(pet_in:QueryData(DataIndex["potential_minatk"])+NowSmallGrow,
					MaxGrowGrades[GradeDirection["potential_minatk"]],pet_in:QueryData(DataIndex["potential_maxatk"])))
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])-10*NowSmallGrow)
				else
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])+PosionFailedSmallGrow)
				end
				return pet_tid,0,28
			end 		
		else
				return 0,0,29			
		end 
	elseif PoisonGrowType==14 then
	---//type=14	宠物属性成长(最大攻击),effect1(概率1-100),effect2(增加值)
		if pet_in:QueryData(DataIndex["potential_maxatk"])<MaxGrowGrades[GradeDirection["potential_maxatk"]] then 
			if TotalGrowCalc==1 then 
				pet_out:UpdateData(DataIndex["potential_maxatk"],math.min(pet_in:QueryData(DataIndex["potential_maxatk"])+TotalGrowEffect,
				MaxGrowGrades[GradeDirection["potential_maxatk"]]))
				pet_out:UpdateData(DataIndex["feedtimes"], pet_in:QueryData(DataIndex["feedtimes"])+1)	
				return pet_tid,0,30
			else
				if pet_in:QueryData(DataIndex["smallgrow"])>=10 then 
					pet_out:UpdateData(DataIndex["potential_maxatk"],math.min(pet_in:QueryData(DataIndex["potential_maxatk"])+NowSmallGrow,
					MaxGrowGrades[GradeDirection["potential_maxatk"]]))
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])-10*NowSmallGrow)
				else
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])+PosionFailedSmallGrow)
				end 
				return pet_tid,0,31
			end 		
		else
				return 0,0,32			
		end 
	elseif PoisonGrowType==24  then
	---//type=15	宠物属性成长(通用攻击),effect1(概率1-100),effect2(增加值)
		if pet_in:QueryData(DataIndex["potential_minatk"])<MaxGrowGrades[GradeDirection["potential_minatk"]] and
				 pet_in:QueryData(DataIndex["potential_minatk"])<=pet_in:QueryData(DataIndex["potential_maxatk"]) and
				 pet_in:QueryData(DataIndex["potential_maxatk"])<=MaxGrowGrades[GradeDirection["potential_maxatk"]] then 
			if TotalGrowCalc==1 then 
				pet_out:UpdateData(DataIndex["potential_minatk"],math.min(pet_in:QueryData(DataIndex["potential_minatk"])+TotalGrowEffect,
				MaxGrowGrades[GradeDirection["potential_minatk"]],pet_in:QueryData(DataIndex["potential_maxatk"])))
				pet_out:UpdateData(DataIndex["potential_maxatk"],math.min(pet_in:QueryData(DataIndex["potential_maxatk"])+TotalGrowEffect,
				MaxGrowGrades[GradeDirection["potential_maxatk"]]))
				pet_out:UpdateData(DataIndex["feedtimes"], pet_in:QueryData(DataIndex["feedtimes"])+2)	
				return pet_tid,0,33
			else
				return pet_tid,0,34
			end 		
		else
				return 0,0,35			
		end 
	elseif PoisonGrowType==15 then
	---//type=16	宠物属性成长(防御),effect1(概率1-100),effect2(增加值)
		if pet_in:QueryData(DataIndex["potential_def"])<MaxGrowGrades[GradeDirection["potential_def"]] then 
			if TotalGrowCalc==1 then 
				pet_out:UpdateData(DataIndex["potential_def"],math.min(pet_in:QueryData(DataIndex["potential_def"])+TotalGrowEffect,
				MaxGrowGrades[GradeDirection["potential_def"]]))
				pet_out:UpdateData(DataIndex["feedtimes"], pet_in:QueryData(DataIndex["feedtimes"])+1)	
				return pet_tid,0,36
			else
				if pet_in:QueryData(DataIndex["smallgrow"])>=10 then
					pet_out:UpdateData(DataIndex["potential_def"],math.min(pet_in:QueryData(DataIndex["potential_def"])+NowSmallGrow,
					MaxGrowGrades[GradeDirection["potential_def"]]))
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])-10*NowSmallGrow)
				else
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])+PosionFailedSmallGrow)
				end 
				return pet_tid,0,37
			end 		
		else
				return 0,0,38			
		end
	elseif PoisonGrowType==16 then
	---//type=17	宠物属性成长(命中),effect1(概率1-100),effect2(增加值)
		if pet_in:QueryData(DataIndex["potential_attackrate"])<MaxGrowGrades[GradeDirection["potential_attackrate"]] then 
			if TotalGrowCalc==1 then 
				pet_out:UpdateData(DataIndex["potential_attackrate"],math.min(pet_in:QueryData(DataIndex["potential_attackrate"])+TotalGrowEffect,
				MaxGrowGrades[GradeDirection["potential_attackrate"]]))
				pet_out:UpdateData(DataIndex["feedtimes"], pet_in:QueryData(DataIndex["feedtimes"])+1)	
				return pet_tid,0,39
			else
				if pet_in:QueryData(DataIndex["smallgrow"])>=10 then
					pet_out:UpdateData(DataIndex["potential_attackrate"],math.min(pet_in:QueryData(DataIndex["potential_attackrate"])+NowSmallGrow,
					MaxGrowGrades[GradeDirection["potential_attackrate"]]))
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])-10*NowSmallGrow)
				else
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])+PosionFailedSmallGrow)
				end 			
				return pet_tid,0,40
			end 		
		else
				return 0,0,41			
		end
	elseif PoisonGrowType==17 then
	---//type=18	宠物属性成长(躲闪),effect1(概率1-100),effect2(增加值)
		if pet_in:QueryData(DataIndex["potential_dodge"])<MaxGrowGrades[GradeDirection["potential_dodge"]] then 
			if TotalGrowCalc==1 then 
				pet_out:UpdateData(DataIndex["potential_dodge"],math.min(pet_in:QueryData(DataIndex["potential_dodge"])+TotalGrowEffect,
				MaxGrowGrades[GradeDirection["potential_dodge"]]))
				pet_out:UpdateData(DataIndex["feedtimes"], pet_in:QueryData(DataIndex["feedtimes"])+1)	
				return pet_tid,0,42
			else
				if pet_in:QueryData(DataIndex["smallgrow"])>=10 then
					pet_out:UpdateData(DataIndex["potential_dodge"],math.min(pet_in:QueryData(DataIndex["potential_dodge"])+NowSmallGrow,
					MaxGrowGrades[GradeDirection["potential_dodge"]]))
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])-10*NowSmallGrow)
				else
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])+PosionFailedSmallGrow)
				end  
				return pet_tid,0,43
			end 		
		else
				return 0,0,44			
		end
	elseif PoisonGrowType==18 then
	---//type=19	宠物属性成长(抗性1),effect1(概率1-100),effect2(增加值)
		if pet_in:QueryData(DataIndex["potential_resist1"])<MaxGrowGrades[GradeDirection["potential_resist1"]] then 
			if TotalGrowCalc==1 then 
				pet_out:UpdateData(DataIndex["potential_resist1"],math.min(pet_in:QueryData(DataIndex["potential_resist1"])+TotalGrowEffect,
				MaxGrowGrades[GradeDirection["potential_resist1"]]))
				pet_out:UpdateData(DataIndex["feedtimes"], pet_in:QueryData(DataIndex["feedtimes"])+1)	
				return pet_tid,0,45
			else
				if pet_in:QueryData(DataIndex["smallgrow"])>=10 then
					pet_out:UpdateData(DataIndex["potential_resist1"],math.min(pet_in:QueryData(DataIndex["potential_resist1"])+NowSmallGrow,
					MaxGrowGrades[GradeDirection["potential_resist1"]]))
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])-10*NowSmallGrow)
				else
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])+PosionFailedSmallGrow)
				end 
				return pet_tid,0,46
			end 		
		else
				return 0,0,47
		end
	elseif PoisonGrowType==19 then
	---//type=20	宠物属性成长(抗性2),effect1(概率1-100),effect2(增加值)
		if pet_in:QueryData(DataIndex["potential_resist2"])<MaxGrowGrades[GradeDirection["potential_resist2"]] then 
			if TotalGrowCalc==1 then 
				pet_out:UpdateData(DataIndex["potential_resist2"],math.min(pet_in:QueryData(DataIndex["potential_resist2"])+TotalGrowEffect,
				MaxGrowGrades[GradeDirection["potential_resist2"]]))
				pet_out:UpdateData(DataIndex["feedtimes"], pet_in:QueryData(DataIndex["feedtimes"])+1)	
				return pet_tid,0,48
			else
				if pet_in:QueryData(DataIndex["smallgrow"])>=10 then
					pet_out:UpdateData(DataIndex["potential_resist2"],math.min(pet_in:QueryData(DataIndex["potential_resist2"])+NowSmallGrow,
					MaxGrowGrades[GradeDirection["potential_resist2"]]))
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])-10*NowSmallGrow)
				else
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])+PosionFailedSmallGrow)
				end 
				return pet_tid,0,49
			end 		
		else
				return 0,0,50
		end
	elseif PoisonGrowType==20 then
	---//type=21	宠物属性成长(抗性3),effect1(概率1-100),effect2(增加值)
		if pet_in:QueryData(DataIndex["potential_resist3"])<MaxGrowGrades[GradeDirection["potential_resist3"]] then 
			if TotalGrowCalc==1 then 
				pet_out:UpdateData(DataIndex["potential_resist3"],math.min(pet_in:QueryData(DataIndex["potential_resist3"])+TotalGrowEffect,
				MaxGrowGrades[GradeDirection["potential_resist3"]]))
				pet_out:UpdateData(DataIndex["feedtimes"], pet_in:QueryData(DataIndex["feedtimes"])+1)	
				return pet_tid,0,51
			else
				if pet_in:QueryData(DataIndex["smallgrow"])>=10 then
					pet_out:UpdateData(DataIndex["potential_resist3"],math.min(pet_in:QueryData(DataIndex["potential_resist3"])+NowSmallGrow,
					MaxGrowGrades[GradeDirection["potential_resist3"]]))
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])-10*NowSmallGrow)
				else
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])+PosionFailedSmallGrow)
				end 
				return pet_tid,0,52
			end 		
		else
				return 0,0,53
		end
	elseif PoisonGrowType==21 then
	---//type=22	宠物属性成长(抗性4),effect1(概率1-100),effect2(增加值)
		if pet_in:QueryData(DataIndex["potential_resist4"])<MaxGrowGrades[GradeDirection["potential_resist4"]] then 
			if TotalGrowCalc==1 then 
				pet_out:UpdateData(DataIndex["potential_resist4"],math.min(pet_in:QueryData(DataIndex["potential_resist4"])+TotalGrowEffect,
				MaxGrowGrades[GradeDirection["potential_resist4"]]))
				pet_out:UpdateData(DataIndex["feedtimes"], pet_in:QueryData(DataIndex["feedtimes"])+1)	
				return pet_tid,0,54
			else
				if pet_in:QueryData(DataIndex["smallgrow"])>=10 then
					pet_out:UpdateData(DataIndex["potential_resist4"],math.min(pet_in:QueryData(DataIndex["potential_resist4"])+NowSmallGrow,
					MaxGrowGrades[GradeDirection["potential_resist4"]]))
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])-10*NowSmallGrow)
				else
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])+PosionFailedSmallGrow)
				end 
				return pet_tid,0,55
			end 		
		else
				return 0,0,56
		end
	elseif PoisonGrowType==22 then
	---//type=23	宠物属性成长(抗性5),effect1(概率1-100),effect2(增加值)
		if pet_in:QueryData(DataIndex["potential_resist5"])<MaxGrowGrades[GradeDirection["potential_resist5"]] then 
			if TotalGrowCalc==1 then 
				pet_out:UpdateData(DataIndex["potential_resist5"],math.min(pet_in:QueryData(DataIndex["potential_resist5"])+TotalGrowEffect,
				MaxGrowGrades[GradeDirection["potential_resist5"]]))
				pet_out:UpdateData(DataIndex["feedtimes"], pet_in:QueryData(DataIndex["feedtimes"])+1)	
				return pet_tid,0,57
			else
				if pet_in:QueryData(DataIndex["smallgrow"])>=10 then
					pet_out:UpdateData(DataIndex["potential_resist5"],math.min(pet_in:QueryData(DataIndex["potential_resist5"])+NowSmallGrow,
					MaxGrowGrades[GradeDirection["potential_resist5"]]))
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])-10*NowSmallGrow)
				else
					pet_out:UpdateData(DataIndex["smallgrow"],pet_in:QueryData(DataIndex["smallgrow"])+PosionFailedSmallGrow)
				end 	
				return pet_tid,0,58
			end 		
		else
				return 0,0,59
		end
	elseif PoisonGrowType==23 then
	---//type=24	宠物属性成长(抗性6),effect1(概率1-100),effect2(增加值)
		if pet_in:QueryData(DataIndex["potential_resist6"])<MaxGrowGrades[GradeDirection["potential_resist6"]] then 
			if TotalGrowCalc==1 then 
				pet_out:UpdateData(DataIndex["potential_resist6"],math.min(pet_in:QueryData(DataIndex["potential_resist6"])+TotalGrowEffect,
				MaxGrowGrades[GradeDirection["potential_resist6"]]))
				pet_out:UpdateData(DataIndex["feedtimes"], pet_in:QueryData(DataIndex["feedtimes"])+1)	
				return pet_tid,0,60
			else
				return pet_tid,0,61
			end 		
		else
				return 0,0,62
		end
	else
				return 0,0,63
	end 

end 
---宠物更改生活相关属性值--------------------------------------------------------------------------
function ZLuaPetBedgeLifePropDeal(pet_tid,pet_in,pet_out,cid,aid,origin)
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if type(cid)~="number" or type(aid)~="number" then return end 
	if pet_in == nil then return end
	local CidMatrix=ZLuaPetBedgeCidMatrix()
	local AidMatrix=ZLuaPetBedgeAidMatrix()
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()	
	
	local PoisonGrowType,PoisonBaseGrowChance,PoisonBaseGrowEffect
	local PoisonUseMaxStar
		---//宠物星等/宠物级别
		---//宠物本身成长概率/药提供的成长类/药提供的额外成功率/药提供的基础效果
		---//宠物道具可使用的最大星等
	PoisonGrowType=CidMatrix[cid].type
	PoisonBaseGrowChance=CidMatrix[cid].effect1
	PoisonBaseGrowEffect=CidMatrix[cid].effect2
	PoisonUseMaxStar=CidMatrix[cid].effect3
	
	local ItemAddGrowChance,ItemGrowEffectInc
	---//道具提供的额外成功率,道具提供的效果加成
	ItemAddGrowChance=AidMatrix[aid].effect1
	ItemGrowEffectInc=AidMatrix[aid].effect2

	
	local NativeStar=pet_in:QueryStar()
		---宠物星等超过物品的最大可使用星等
	if PoisonUseMaxStar<NativeStar then 
		return 0,0,64		
	end 	
		
	if ZPetBedge_ChanceCalc(PoisonBaseGrowChance+ItemAddGrowChance)~=1 then 
		return 0,0,65
	else
		if PoisonGrowType==26 then 
			pet_out:UpdateData(DataIndex["acuity"],pet_in:QueryData(DataIndex["acuity"])+
			PoisonBaseGrowEffect*(1+ItemGrowEffectInc))
			return pet_tid,0,66
		elseif PoisonGrowType==27 then
			pet_out:UpdateData(DataIndex["stamina"],pet_in:QueryData(DataIndex["stamina"])+
			PoisonBaseGrowEffect*(1+ItemGrowEffectInc))
			return pet_tid,0,67
		elseif PoisonGrowType==28 then
			pet_out:UpdateData(DataIndex["sawy"],pet_in:QueryData(DataIndex["sawy"])+
			PoisonBaseGrowEffect*(1+ItemGrowEffectInc))
			return pet_tid,0,68
		elseif PoisonGrowType==29 then
			pet_out:UpdateData(DataIndex["dowry"],pet_in:QueryData(DataIndex["dowry"])+
			PoisonBaseGrowEffect*(1+ItemGrowEffectInc))
			return pet_tid,0,69
		elseif PoisonGrowType==30 then
			pet_out:UpdateData(DataIndex["wisdom"],pet_in:QueryData(DataIndex["wisdom"])+
			PoisonBaseGrowEffect*(1+ItemGrowEffectInc))			
			return pet_tid,0,70
		elseif PoisonGrowType==31 then
			pet_out:UpdateData(DataIndex["cnr"],pet_in:QueryData(DataIndex["cnr"])+
			PoisonBaseGrowEffect*(1+ItemGrowEffectInc))
			return pet_tid,0,71
		else
			return pet_tid,0,72
		end 
	end 	
end 

---改变宠物主类型(限界)----------------------------------------------------------------------------
function ZLuaPetBedgeChangeMainType(pet_tid,pet_in,pet_out,cid,aid,origin)
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local nativemaintype=pet_in:QueryMainType()
	---//各种限界类型出现的概率
	local MaintypeList={0,1,2,3,4,5,6,7,8,9,10,11,12}
	local MaintypeChance={100,600,100,100,100,700,300,600,600,600,600,600,0}
	MaintypeChance[nativemaintype+1]=0
	local newmaintype=MaintypeList[ZPetGrade_Get_RanPoint(MaintypeChance)]
	if newmaintype<0 then newmaintype=0 end 
	if newmaintype>=12 then newmaintype=11 end
	---//冰冻限界隐藏anywhere
	pet_out:UpdateData(DataIndex["maintype"], newmaintype)
	return pet_tid,0,73
end 

---改变宠物轮回----------------------------------------------------------------------------
function ZLuaPetBedgeChangeFatality(pet_tid,pet_in,pet_out,cid,aid,origin)
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local NewFatality=ZLuaPetBedgeFatalitySkills(pet_tid,origin)
	pet_out:UpdateData(DataIndex["skillid_8"], NewFatality or 0)	
	return pet_tid,0,89
end
---宠物豆神修炼-------------------------------------------------------------------------------
function ZLuaPetBedgeStaticGrowUp(pet_tid,pet_in,pet_out,cid,aid,origin)  
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local NewPeaSkillID=pet_in:QueryData(DataIndex["skillid_11"]) or 0
	local NewPeaSkillLevel=pet_in:QueryData(DataIndex["skilllvl_11"]) or 0
	local CidMatrix=ZLuaPetBedgeCidMatrix()
	local cideffect=CidMatrix[cid].effect1
	
	local para1=math.max(NewPeaSkillID-1262,0)
	local para2=NewPeaSkillLevel+cideffect
	local sumpara=para1*99+para2
	
	local newpara1=math.ceil(sumpara/99)+1261
	local newpara2=sumpara-math.floor(sumpara/99)*99
	if newpara2==0 and newpara1~=0 then 
		newpara2=99
	end 

	if sumpara>=0 and sumpara<=9*99 then 
		pet_out:UpdateData(DataIndex["skillid_11"], newpara1 or 0)
		pet_out:UpdateData(DataIndex["skilllvl_11"], newpara2 or 0)
		return pet_tid,0,90
	else
		return 0,0,91
	end 

end 
----------------------------------------------------------------------------------------------
-----宠物佛缘舍利修炼-------------------------------------------------------------------------
function ZLuaPetBedgeBuddhaGrow(pet_tid,pet_in,pet_out,cid,aid,origin,cult)  
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local NewPeaSkillID=pet_in:QueryData(DataIndex["skillid_10"]) or 0
	local NewPeaSkillLevel=pet_in:QueryData(DataIndex["skilllvl_10"]) or 0
	local CidMatrix=ZLuaPetBedgeCidMatrix()
	local cideffect1=CidMatrix[cid].effect1	---type
	local cideffect2=CidMatrix[cid].effect2	---effect lvl
	
	---//非佛不可修炼舍利
	if cult~=4 and type(cult)=="number" then
		return 0,0,92
	end
	
	if cideffect1==1 then 
		pet_out:UpdateData(DataIndex["skillid_10"], 1356 or 0)
		pet_out:UpdateData(DataIndex["skilllvl_10"], cideffect2 or 0)
	elseif cideffect1==2 then 
		pet_out:UpdateData(DataIndex["skillid_10"], 1357 or 0)
		pet_out:UpdateData(DataIndex["skilllvl_10"], cideffect2 or 0)
	elseif cideffect1==3 then 
		pet_out:UpdateData(DataIndex["skillid_10"], 1358 or 0)
		pet_out:UpdateData(DataIndex["skilllvl_10"], cideffect2 or 0)
	else
		return 0,0,92
	end 
	return pet_tid,0,93
end 
-----宠物魂化抽提-------------------------------------------------------------------------
function ZLuaPetBedgeGetSpirit(pet_tid,pet_in,pet_out,cid,aid,origin)  
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local CidMatrix=ZLuaPetBedgeCidMatrix()
	local availablePets=ZLuaAvailablePets()
	
	local pet
	local newspirit_id=0
	for _,pet in pairs(availablePets) do
		if pet.pet_tid == pet_tid then 
			newspirit_id=pet.spirit_id 
		end
	end
	if newspirit_id~=0 and pet_in:QueryLevel()>75 then 
		return newspirit_id,0,201
	elseif newspirit_id~=0 and pet_in:QueryLevel()<75 then 
		return 0,0,202
	else
		return 0,0,203
	end 
end 
-----宠物魂化设置-------------------------------------------------------------------------
function ZLuaPetBedgeSetSpirit(pet_tid,pet_in,pet_out,cid,aid,origin)  
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local CidMatrix=ZLuaPetBedgeCidMatrix()
	local newpet_tid=CidMatrix[cid].effect1
	
  local availableChangeModelPets={16503,16504,16505,16506,16507,16508,18306,18929,19510,20108,19110,20488,
  																20489,20561,22105,22484,21247,23337,24650,24781,25117,25118,25124,25234,25311,25422,34054,
  																25421,25385,33146,29993,29994,29995,29996,29997,30988,30989,30990,30991,34126,34214,34215,
  																34289,40541,40631,40665,40863,40864,40865,40866,40867,40868,40869,40870,41261,41262,41928,42683,62772}
	
	for _,petid in pairs(availableChangeModelPets) do
		if petid==pet_tid then 
			local nativerace,_,_=ZLuaPetBedgeStaticInfo(newpet_tid)
			pet_out:UpdateData(DataIndex["race"], nativerace)
			pet_out:UpdateData(DataIndex["skillid_9"],1355)	
			pet_out:UpdateData(DataIndex["skilllvl_9"],pet_in:QueryData(DataIndex["skilllvl_9"]))
			return newpet_tid,0,206	
			
		end
	end
	return 0,0,203
	
end 
---改变宠物形象-------------------------------------------------------------------------------
function ZLuaPetBedgeChangeState(pet_tid,pet_in,pet_out,cid,aid,origin) 
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local CidMatrix=ZLuaPetBedgeCidMatrix()
	local AvailablePets=ZLuaAvailablePets()
		
	---//特定宠物无法改变形象
	for i,v in pairs(AvailablePets) do
		if v.pet_tid == pet_tid and v.changefigure_able==0 then   
			return 0,1,87	
		end 
	end
	---飞升后不在这里改形象
	if pet_in:QueryRebornCnt() == 1 then
		return 0,1,213
	end
	
	local Star=pet_in:QueryData(DataIndex["star"]) or 1
	---//effect1(最低使用星等)/effect2(指向的shape)/effect3(指向的shape(变异情况下)
	if Star<CidMatrix[cid].effect1 then 
		return 0,0,80
	else
		if pet_in:QueryData(DataIndex["isaberrant"])==1 then 
			pet_out:UpdateData(DataIndex["shape"],CidMatrix[cid].effect3)
		else
			pet_out:UpdateData(DataIndex["shape"],CidMatrix[cid].effect2)
		end 	
		return pet_tid,0,81
	end 	
end 

---测试用：改变宠物星等为12星
function ZLuaPetBedgeSetTest(pet_tid,pet_in,pet_out,cid,aid,origin)
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local CidMatrix=ZLuaPetBedgeCidMatrix()
	local AvailablePets=ZLuaAvailablePets()

	for _,index in pairs(DataIndex) do
		pet_out:UpdateData(index,pet_in:QueryData(index))
	end

	
	pet_out:UpdateData(DataIndex["potential_hp"],6000)	
	pet_out:UpdateData(DataIndex["potential_mp"],6000)	
	pet_out:UpdateData(DataIndex["potential_minatk"],6000)	
	pet_out:UpdateData(DataIndex["potential_maxatk"],6000)	
	pet_out:UpdateData(DataIndex["potential_def"],6000)	
	pet_out:UpdateData(DataIndex["potential_attackrate"],6000)	
	pet_out:UpdateData(DataIndex["potential_dodge"],6000)	
	pet_out:UpdateData(DataIndex["potential_resist1"],6000)	
	pet_out:UpdateData(DataIndex["potential_resist2"],6000)	
	pet_out:UpdateData(DataIndex["potential_resist3"],6000)	
	pet_out:UpdateData(DataIndex["potential_resist4"],6000)	
	pet_out:UpdateData(DataIndex["potential_resist5"],6000)	
	pet_out:UpdateData(DataIndex["star"],ZLuaPetBedgeReCalcStar(pet_tid,pet_out,origin))	
	pet_out:SetLevel(150)

 	ZLuaPetBedgeRebuild(pet_tid,pet_out,ServerOrigins["combine"],_,_)
		
	return pet_tid,0,212
end
---重新洗技能
function ZLuaPetBedgeRandomSkill(pet_tid,pet_in,pet_out,cid,aid,origin)
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	if pet_in:QueryRebornCnt() ~= 1 then return 0,0,221 end
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local serverorigin = origin or ServerOrigins["drop"]
	local AvailablePets=ZLuaAvailablePets()
	local horoscope	= pet_in:QueryHoroscope()	
	local SkillsList={}
	local SkillsLevel={}
	SkillsList,SkillsLevel=ZLuaPetBedgeSkills(pet_tid,pet_in,horoscope,serverorigin)
	pet_out:UpdateData(DataIndex["skillid_1"], SkillsList[1] or 0)	
	pet_out:UpdateData(DataIndex["skillid_2"], SkillsList[2] or 0)	
	pet_out:UpdateData(DataIndex["skillid_3"], SkillsList[3] or 0)	
	pet_out:UpdateData(DataIndex["skillid_4"], SkillsList[4] or 0)	
	pet_out:UpdateData(DataIndex["skillid_5"], SkillsList[5] or 0)	
	pet_out:UpdateData(DataIndex["skillid_6"], SkillsList[6] or 0)	
	pet_out:UpdateData(DataIndex["skillid_7"], SkillsList[7] or 0)	
	
	pet_out:UpdateData(DataIndex["skilllvl_1"], SkillsLevel[1] or 0)	
	pet_out:UpdateData(DataIndex["skilllvl_2"], SkillsLevel[2] or 0)	
	pet_out:UpdateData(DataIndex["skilllvl_3"], SkillsLevel[3] or 0)	
	pet_out:UpdateData(DataIndex["skilllvl_4"], SkillsLevel[4] or 0)	
	pet_out:UpdateData(DataIndex["skilllvl_5"], SkillsLevel[5] or 0)	
	pet_out:UpdateData(DataIndex["skilllvl_6"], SkillsLevel[6] or 0)	
	pet_out:UpdateData(DataIndex["skilllvl_7"], SkillsLevel[7] or 0)	
	return pet_tid,0,219


	
end
---改变星曜
function ZLuaPetBedgeRandomHoroscope(pet_tid,pet_in,pet_out,cid,aid,origin)
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	if pet_in:QueryRebornCnt() ~= 1 then return 0,0,221 end
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local serverorigin = origin or ServerOrigins["drop"]
	local AvailablePets=ZLuaAvailablePets()
	local horoscope=ZLuaPetBedgeHoroscopeInfo(pet_tid,origin)
	local Horoscope= ZLuaPetBedgeHoroscopeDesc()		
	local HoroscopeAddSkillID

	pet_out:UpdateData(DataIndex["horoscope"], horoscope)
	if horoscope==Horoscope["riyao"] then				---日曜
		HoroscopeAddSkillID=5009
	elseif horoscope==Horoscope["yueyao"] then	---月曜
		HoroscopeAddSkillID=5010
	elseif horoscope==Horoscope["jinyao"] then	---金曜
		HoroscopeAddSkillID=5011
	elseif horoscope==Horoscope["muyao"] then		---木曜
		HoroscopeAddSkillID=5012
	elseif horoscope==Horoscope["shuiyao"] then	---水曜
		HoroscopeAddSkillID=5013
	elseif horoscope==Horoscope["huoyao"] then	---火曜
		HoroscopeAddSkillID=5014
	elseif horoscope==Horoscope["tuyao"] then		---土曜
		HoroscopeAddSkillID=5015
	elseif horoscope==Horoscope["luohou"] then	---罗候
		HoroscopeAddSkillID=5016
	elseif horoscope==Horoscope["jidu"] then		---计都
		HoroscopeAddSkillID=5017
	else 
	end 
	pet_out:UpdateData(DataIndex["skillid_0"], HoroscopeAddSkillID)

	if horoscope==Horoscope["riyao"]	then 
		skillslist=		{1399,1400,1401,1402,1403,1404,1453,1454,1455,1456,1457,1458,5018}
		skillschance=	{1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,200}
	elseif horoscope==Horoscope["yueyao"]	then 
		skillslist=		{1405,1406,1407,1408,1409,1410,1453,1454,1455,1456,1457,1458,5019}
		skillschance=	{1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,200}
	elseif horoscope==Horoscope["jinyao"]	then 
		skillslist=		{1411,1412,1413,1414,1415,1416,1453,1454,1455,1456,1457,1458,5020}
		skillschance=	{1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,200}
	elseif horoscope==Horoscope["muyao"] then
		skillslist=		{1417,1418,1419,1420,1421,1422,1453,1454,1455,1456,1457,1458,5021}
		skillschance=	{1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,200}	
	elseif horoscope==Horoscope["shuiyao"] then
		skillslist=		{1423,1424,1425,1426,1427,1428,1453,1454,1455,1456,1457,1458,5022}
		skillschance=	{1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,200}	
	elseif horoscope==Horoscope["huoyao"]	then 
		skillslist=		{1429,1430,1431,1432,1433,1434,1453,1454,1455,1456,1457,1458,5023}
		skillschance=	{1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,200}
	elseif horoscope==Horoscope["tuyao"] then	
		skillslist=		{1435,1436,1437,1438,1439,1440,1453,1454,1455,1456,1457,1458,5024}
		skillschance=	{1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,200}
	elseif horoscope==Horoscope["luohou"]	then 
		skillslist=		{1441,1442,1443,1444,1445,1446,1453,1454,1455,1456,1457,1458,5026}
		skillschance=	{1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,200}
	elseif horoscope==Horoscope["jidu"]	then 
		skillslist=		{1447,1448,1449,1450,1451,1452,1453,1454,1455,1456,1457,1458,5025}
		skillschance=	{1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,200}
	else
		skillslist=		{1453,1454,1455,1456,1457,1458,5027,5028}
		skillschance=	{1000,1000,1000,1000,1000,1000,200,200}
	end	

	local choosenum	= 0			---//选取的技能数量	

	if pet_out:QueryData(DataIndex["skillid_1"])~=0  then choosenum=choosenum+1 end
	if pet_out:QueryData(DataIndex["skillid_2"])~=0  then choosenum=choosenum+1 end
	if pet_out:QueryData(DataIndex["skillid_3"])~=0  then choosenum=choosenum+1 end
	if pet_out:QueryData(DataIndex["skillid_4"])~=0  then choosenum=choosenum+1 end
	if pet_out:QueryData(DataIndex["skillid_5"])~=0  then choosenum=choosenum+1 end
	if pet_out:QueryData(DataIndex["skillid_6"])~=0  then choosenum=choosenum+1 end 
	if pet_out:QueryData(DataIndex["skillid_7"])~=0  then choosenum=choosenum+1 end

	local maxsamecol=1			---//每个技能最多出现的次数
	local maxskillslevel=1	---//每个技能最多出现的级别
	local choosedskills={}
	
	choosedskills=ZLuaCheckOutNums(skillslist,skillschance,choosenum,maxsamecol)
	
	local SkillsList={}
	local SkillsLevel={}
	SkillsList,SkillsLevel=ZLuaPetBedgeMakeSkillsTable(choosedskills,choosenum,maxskillslevel)
	pet_out:UpdateData(DataIndex["skillid_1"], SkillsList[1] or 0)	
	pet_out:UpdateData(DataIndex["skillid_2"], SkillsList[2] or 0)	
	pet_out:UpdateData(DataIndex["skillid_3"], SkillsList[3] or 0)	
	pet_out:UpdateData(DataIndex["skillid_4"], SkillsList[4] or 0)	
	pet_out:UpdateData(DataIndex["skillid_5"], SkillsList[5] or 0)	
	pet_out:UpdateData(DataIndex["skillid_6"], SkillsList[6] or 0)	
	pet_out:UpdateData(DataIndex["skillid_7"], SkillsList[7] or 0)	
	
	pet_out:UpdateData(DataIndex["skilllvl_1"], SkillsLevel[1] or 0)	
	pet_out:UpdateData(DataIndex["skilllvl_2"], SkillsLevel[2] or 0)	
	pet_out:UpdateData(DataIndex["skilllvl_3"], SkillsLevel[3] or 0)	
	pet_out:UpdateData(DataIndex["skilllvl_4"], SkillsLevel[4] or 0)	
	pet_out:UpdateData(DataIndex["skilllvl_5"], SkillsLevel[5] or 0)	
	pet_out:UpdateData(DataIndex["skilllvl_6"], SkillsLevel[6] or 0)	
	pet_out:UpdateData(DataIndex["skilllvl_7"], SkillsLevel[7] or 0)	
	
	return pet_tid,0,220
end



---变异宠物---------------------------------------------------------------------------
function ZLuaPetBedgeAberrantState(pet_tid,pet_in,pet_out,cid,aid,origin) 
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local AvailablePets=ZLuaAvailablePets()
	local IsAberrant=pet_in:QueryData(DataIndex["isaberrant"])
	local msg_id=82
	
	---//特定宠物无法改变形象
	for i,v in pairs(AvailablePets) do
		if v.pet_tid == pet_tid and v.changefigure_able==0 then   
			return 0,1,87	
		end 
	end
	--飞升后不在这里改形象
	if pet_in:QueryRebornCnt() == 1 then
		return 0,1,213
	end
		
	if IsAberrant==0 then 
		IsAberrant=1
			pet_out:UpdateData(DataIndex["shape"],pet_in:QueryData(DataIndex["shape"])+1)
		msg_id=82
	else 
		IsAberrant=0
			pet_out:UpdateData(DataIndex["shape"],pet_in:QueryData(DataIndex["shape"])-1)
		msg_id=83
	end 
	pet_out:UpdateData(DataIndex["isaberrant"],IsAberrant)
	return pet_tid,0,msg_id
end 
---宠物形象开始随级别成长而成长(美工用)------------------------------------------------------
function ZLuaPetBedgeArtGrowState(pet_tid,pet_in,pet_out,cid,aid,origin) 
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local IsGrow=pet_in:QueryData(DataIndex["isgrowup"])
	local msg_id=84
	if IsGrow==0 then 
		IsGrow=1
		msg_id=84
	else 
		IsGrow=0
		msg_id=85
	end 
	pet_out:UpdateData(DataIndex["isgrowup"],IsGrow)
	return pet_tid,0,msg_id
end 
---宠物修炼主函数----------------------------------------------------------------------------------
function PetBedgeCombine(pet_tid,pet_in,pet_out,cid,aid,cult)
  if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return 0,1,99 end
  if pet_in == nil then return 0,1,99 end
  local AvailablePets=ZLuaAvailablePets()
  local CidMatrix=ZLuaPetBedgeCidMatrix()
  local AidMatrix=ZLuaPetBedgeAidMatrix()
  local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()

	for i,v in pairs(AvailablePets) do
		if v.pet_tid == pet_tid and v.combine_able==0 and v.fixage_able==0 then   
			return 0,1,98
		end 
	end
	
	for i,v in pairs(AvailablePets) do
		if v.pet_tid == pet_tid and v.combine_able==0 and v.fixage_able==1 and (CidMatrix[cid].type<3 or CidMatrix[cid].type>4) then   
			return 0,1,98
		end 
	end
	
  ---//拷贝数据到新宠物
  for i,v in pairs(DataIndex) do
		pet_out:UpdateData(v, pet_in:QueryData(v) )
	end
 	pet_out:SetLevel(pet_in:QueryLevel())
 	pet_out:SetExp(pet_in:QueryExp())
 	pet_out:SetCurHP(pet_in:QueryCurHP())
 	pet_out:SetCurVigor(pet_in:QueryCurVigor())
 	pet_out:SetAge(pet_in:QueryAge())
 	pet_out:SetLife(pet_in:QueryLife())
  pet_out:SetCurHonorPoint(pet_in:QueryCurHonorPoint())
 	pet_out:SetMaxHonorPoint(pet_in:QueryMaxHonorPoint())
  pet_out:SetCurHungerPoint(pet_in:QueryCurHungerPoint())
 	pet_out:SetMaxHungerPoint(pet_in:QueryMaxHungerPoint())
 	pet_out:SetRebornCnt(pet_in:QueryRebornCnt()) ---飞升
 	for i=0,13 do
 		pet_out:SetRebornPropAdd(i,pet_in:QueryRebornPropAdd(i))---飞升属性
 	end
 	pet_out:SetRebornStar(pet_in:QueryRebornStar()) ---飞升星等
	
 	pet_out:SetSkillAttackRate(pet_in:QuerySkillAttackRate()) ---飞升隐藏属性
 	pet_out:SetSkillArmorRate(pet_in:QuerySkillArmorRate()) ---飞升隐藏属性
 	pet_out:SetAntiCritRate(pet_in:QueryAntiCritRate()) ---飞升隐藏属性
 	pet_out:SetAntiCritDamage(pet_in:QueryAntiCritDamage()) ---飞升隐藏属性
 	pet_out:SetDamageReduce(pet_in:QueryDamageReduce()) ---飞升隐藏属性
 	pet_out:SetDamageReducePercent(pet_in:QueryDamageReducePercent()) ---飞升隐藏属性
 	
 	local origin=pet_in:QueryData(DataIndex["nativeorigin"])
 	local isidentify=pet_in:QueryData(DataIndex["identify"])
 	local newpet_id,msg_channel,msg_id
	local DoSpecialSpirit=0			---//专用于宠物交换spirit
		
  ---//屏蔽不需要辅助道具的修炼
	if aid~=0 and CidMatrix[cid].type<=6 then 
		return 0,0,208
	end 
	if aid~=0 and CidMatrix[cid].type>7 and CidMatrix[cid].type<11 then 
		return 0,0,208
	end 	
	if aid~=0 and CidMatrix[cid].type>25 then 
		return 0,0,208
	end 	
  if CidMatrix[cid].type==nil or type(CidMatrix[cid].type)~="number" then CidMatrix[cid].type=0 end 
  
  ---//宠物逆天重生	
  if CidMatrix[cid].type==1 and isidentify==1 then 
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeHolyReborn(pet_tid,pet_in,pet_out,cid,aid,origin)  
  elseif CidMatrix[cid].type==2 and isidentify==1 then 
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeIdentify(pet_tid,pet_in,pet_out,cid,aid,origin)
  ---//宠物改变年龄
  elseif CidMatrix[cid].type==3 and isidentify==1 then
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeChangeAge(pet_tid,pet_in,pet_out,cid,aid,origin)
  ---//宠物改变寿命
  elseif CidMatrix[cid].type==4 and isidentify==1 then
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeChangeLife(pet_tid,pet_in,pet_out,cid,aid,origin)
  ---//宠物改变性格
  elseif CidMatrix[cid].type==5 and isidentify==1 then 
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeChangeEmement(pet_tid,pet_in,pet_out,cid,aid,origin)
  ---//宠物改变天赋
  elseif CidMatrix[cid].type==6 and isidentify==1 then
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeChangeTalent(pet_tid,pet_in,pet_out,cid,aid,origin)
  ---//宠物学习技能
  elseif CidMatrix[cid].type==7 and isidentify==1 then 
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeLearnSkill(pet_tid,pet_in,pet_out,cid,aid,origin)
  ---//宠物遗忘技能
  elseif CidMatrix[cid].type==8 and isidentify==1 then
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeForgetSkill(pet_tid,pet_in,pet_out,cid,aid,origin)
  ---//宠物改变当前亲密度
  elseif CidMatrix[cid].type==9 and isidentify==1 then 
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeChangeCurHonor(pet_tid,pet_in,pet_out,cid,aid,origin)
  ---//宠物改变最大亲密度
  elseif CidMatrix[cid].type==10 and isidentify==1 then  
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeChangeMaxHonor(pet_tid,pet_in,pet_out,cid,aid,origin)
 	---//宠物改变战斗相关属性
  elseif CidMatrix[cid].type>=11 and CidMatrix[cid].type<=25 and isidentify==1 then  
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeCombatPropDeal(pet_tid,pet_in,pet_out,cid,aid,origin,1)
  	---//usetype=1代表主动成长(因cid导致的一次成长)
  	---//usetype>1代表被动成长(因aid导致的二次成长,且usetype代表成长类型)

   	if newpet_id~=0 and ZLuaPetBedgeExistInTmplIDs(newpet_id)~=0 then 
  		---//重新计算星等
  		local NewStar=ZLuaPetBedgeReCalcStar(newpet_id,pet_out,origin)
  		pet_out:UpdateData(DataIndex["star"],NewStar)
    	---所有战斗属性被动成长(是否溅射成长)
    	if AidMatrix[aid].effect3==1 and aid~=0 then 
    		ZLuaPetBedgePassiveAddCombatProp(pet_tid,pet_in,pet_out,cid,aid,origin)
    	end 
    	---增加亲密度上限
  		ZLuaPetBedgePassiveAddMaxHonor(pet_tid,pet_in,pet_out,cid,aid,origin)
  		---统计使用的aid
  		if aid~=0 and aid~=nil then 
  			pet_out:UpdateData(DataIndex["feedaids"],pet_in:QueryData(DataIndex["feedaids"])+1)
  		end 
  	end 		

  ---//宠物改变生活相关属性
  elseif CidMatrix[cid].type>=26 and CidMatrix[cid].type<=31 and isidentify==1 then
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeLifePropDeal(pet_tid,pet_in,pet_out,cid,aid,origin)
  ---//宠物改变主类型
  elseif CidMatrix[cid].type==32 and isidentify==1 then  
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeChangeMainType(pet_tid,pet_in,pet_out,cid,aid,origin) 
   ---//宠物改变形象
  elseif CidMatrix[cid].type==33 and isidentify==1 then  
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeChangeState(pet_tid,pet_in,pet_out,cid,aid,origin) 
   ---//宠物变异(形象)
  elseif CidMatrix[cid].type==34 and isidentify==1 then  
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeAberrantState(pet_tid,pet_in,pet_out,cid,aid,origin)   
   ---//宠物开始成长(美工用)
  elseif CidMatrix[cid].type==35 and isidentify==1 then  
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeArtGrowState(pet_tid,pet_in,pet_out,cid,aid,origin)   
  ---//宠物改变轮回
  elseif CidMatrix[cid].type==36 and isidentify==1 then  
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeChangeFatality(pet_tid,pet_in,pet_out,cid,aid,origin)   
 	---//宠物通过豆神修炼提升攻击和防御
  elseif CidMatrix[cid].type==37 and isidentify==1 then  
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeStaticGrowUp(pet_tid,pet_in,pet_out,cid,aid,origin)  
 	---//宠物通过舍利强化宠物属性
  elseif CidMatrix[cid].type==38 and isidentify==1 then  
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeBuddhaGrow(pet_tid,pet_in,pet_out,cid,aid,origin,cult)  
  ---//宠物魂化抽提(切换宠物类别之第一步)
  elseif CidMatrix[cid].type==39 and isidentify==1 then  
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeGetSpirit(pet_tid,pet_in,pet_out,cid,aid,origin)
  	DoSpecialSpirit=1  
  ---//宠物魂化设定(切换宠物类别之第二步)
  elseif CidMatrix[cid].type==40 and isidentify==1 then  
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeSetSpirit(pet_tid,pet_in,pet_out,cid,aid,origin) 
  elseif CidMatrix[cid].type==41 and isidentify==1 then  
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeSetTest(pet_tid,pet_in,pet_out,cid,aid,origin) 
  elseif CidMatrix[cid].type==42 and isidentify==1 then  
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeRandomSkill(pet_tid,pet_in,pet_out,cid,aid,origin) 
  elseif CidMatrix[cid].type==43 and isidentify==1 then  
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeRandomHoroscope(pet_tid,pet_in,pet_out,cid,aid,origin) 
  else 
  	newpet_id=0
  	msg_channel=0
  	msg_id=74				---宠物未经鉴定或非法道具
  end 
  
  if (ZLuaPetBedgeExistInTmplIDs(newpet_id)==0 or newpet_id==0) and DoSpecialSpirit==0 then 
  	newpet_id=0
  elseif DoSpecialSpirit==1 then 
  	---//宠物魂化Get不判断产出id
  else
  	---//pet_out:UpdateData(DataIndex["origin"],ServerOrigins["combine"])
   	pet_out:UpdateData(DataIndex["id"],newpet_id)
   	ZLuaPetBedgeRebuild(newpet_id,pet_out,ServerOrigins["combine"],_,cult)
  end 
  return newpet_id,0,msg_id
end
---重新计算宠物星等-------------------------------------------------------------------------------
function ZLuaPetBedgeReCalcStar(pet_id,pet,origin)
		if ZLuaPetBedgeExistInTmplIDs(pet_id)==0 then return 1 end 
		local GradeDirection=ZLuaPetBedgeGradeDirection()
  	local DataIndex=ZLuaPetBedgeDataIndex()
  	local Grades={}
  		Grades[GradeDirection["potential_hp"]]				=pet:QueryData(DataIndex["potential_hp"])
			Grades[GradeDirection["potential_mp"]]				=pet:QueryData(DataIndex["potential_mp"])
			Grades[GradeDirection["potential_minatk"]]		=pet:QueryData(DataIndex["potential_minatk"])
			Grades[GradeDirection["potential_maxatk"]]		=pet:QueryData(DataIndex["potential_maxatk"])
			Grades[GradeDirection["potential_def"]]				=pet:QueryData(DataIndex["potential_def"])
			Grades[GradeDirection["potential_attackrate"]]=pet:QueryData(DataIndex["potential_attackrate"])
			Grades[GradeDirection["potential_dodge"]]			=pet:QueryData(DataIndex["potential_dodge"])
			Grades[GradeDirection["potential_resist1"]]		=pet:QueryData(DataIndex["potential_resist1"])
			Grades[GradeDirection["potential_resist2"]]		=pet:QueryData(DataIndex["potential_resist2"])
			Grades[GradeDirection["potential_resist3"]]		=pet:QueryData(DataIndex["potential_resist3"])
			Grades[GradeDirection["potential_resist4"]]		=pet:QueryData(DataIndex["potential_resist4"])
			Grades[GradeDirection["potential_resist5"]]		=pet:QueryData(DataIndex["potential_resist5"])
			Grades[GradeDirection["potential_resist6"]]		=pet:QueryData(DataIndex["potential_resist6"])
  	local Horoscope=pet:QueryHoroscope()
  	local NewStar=ZLuaPetStarFromGrades(pet_id,origin,Horoscope,Grades,2)	---//参数2代表合成后重新计算星等
   	if NewStar>20 or NewStar<0 then NewStar=1 end
  	return NewStar
end 
---宠物在炼化过程中增加亲密度上限-------------------------------------------------------------------
function ZLuaPetBedgePassiveAddMaxHonor(pet_tid,pet_in,pet_out,cid,aid,origin)
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local MaxHonor=57600
	local CidMatrix=ZLuaPetBedgeCidMatrix()
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	
	local AddChance=15
	local AddHonor=100
	if aid~=0 and aid~=nil then 
		AddChance=85 
		AddHonor=150
	end
	if CidMatrix[cid].effect3>=12 then 
		AddChance=85  
		AddHonor=200
	end 
	
	if ZPetBedge_ChanceCalc(AddChance)==1 then  
		pet_out:SetMaxHonorPoint(math.min(pet_in:QueryMaxHonorPoint()+AddHonor,MaxHonor))
	end

end 
---宠物在炼化过程中增加所有战斗属性-----------------------------------------------------------------
function ZLuaPetBedgePassiveAddCombatProp(pet_tid,pet_in,pet_out,cid,aid,origin)
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if type(cid)~="number" or type(aid)~="number" then return end 
	if pet_in == nil then return end

	local CidMatrix=ZLuaPetBedgeCidMatrix()
	local AidMatrix=ZLuaPetBedgeAidMatrix()
	local DataIndex=ZLuaPetBedgeDataIndex()
	local GradeDirection=ZLuaPetBedgeGradeDirection()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local usetype
	
	if aid~=0 and aid~=nil then 
  	for usetype=11,24 do
  	ZLuaPetBedgeCombatPropDeal(pet_tid,pet_in,pet_out,cid,aid,origin,usetype)
  	end 
  end 
	
end 

---//宠物主要类型导致的成长增益(当修炼成功时)
function ZLuaPetBedgeMaintypeGrowEffect(maintype,poisongrowtype)
	---//maintype from 0-12	
	---//poisongrowtype=11-23
	---//此部分较为破坏逻辑(生写)	
	local maintypegrowchanceadd=0
	
	if type(maintype)~="number" or type(poisongrowtype)~="number" then 
		maintypegrowchanceadd=0
	end 	
	if maintype==poisongrowtype-11 then 
		maintypegrowchanceadd=0.05
	end 
	
	return maintypegrowchanceadd
end 


---//宠物飞升处理函数
function PetBedgeReborn(pet_tid,pet)
	local DataIndex=ZLuaPetBedgeDataIndex()
	local SpecialIndex=ZLuaPetBedgeSpecialIndex()
	local availablePets = ZLuaAvailablePets()
	local Race=ZLuaPetBedgeRaceDesc()
	local horoscope	= pet:QueryHoroscope()	
	local Horoscope= ZLuaPetBedgeHoroscopeDesc()		
	local HoroscopeAddSkillID

	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return 0 end
	if pet == nil then return 0 end
	if pet:QueryRebornCnt() ~= 0 then return 0,0,209 end
	if pet:QueryLevel() < 150 or pet:QueryStar() < 12 then return 0,0,210 end

			
	for i,v in pairs(availablePets) do
		if v.pet_tid == pet_tid then   
			if v.combine_able==0 or v.fixage_able==0 or v.changefigure_able ==0 then
			return 0,0,214
			end
		end 
	end

	pet:SetAge(0)
	pet:SetLife(65535)
	pet:SetMaxHungerPoint(65535)
	pet:SetMaxHonorPoint(65535)
	pet:SetCurHungerPoint(65535)
	pet:SetCurHonorPoint(65535)

	if horoscope==Horoscope["riyao"] then				---日曜
		HoroscopeAddSkillID=5009
	elseif horoscope==Horoscope["yueyao"] then	---月曜
		HoroscopeAddSkillID=5010
	elseif horoscope==Horoscope["jinyao"] then	---金曜
		HoroscopeAddSkillID=5011
	elseif horoscope==Horoscope["muyao"] then		---木曜
		HoroscopeAddSkillID=5012
	elseif horoscope==Horoscope["shuiyao"] then	---水曜
		HoroscopeAddSkillID=5013
	elseif horoscope==Horoscope["huoyao"] then	---火曜
		HoroscopeAddSkillID=5014
	elseif horoscope==Horoscope["tuyao"] then		---土曜
		HoroscopeAddSkillID=5015
	elseif horoscope==Horoscope["luohou"] then	---罗候
		HoroscopeAddSkillID=5016
	elseif horoscope==Horoscope["jidu"] then		---计都
		HoroscopeAddSkillID=5017
	else 
	end 

	pet:UpdateData(DataIndex["skillid_0"], HoroscopeAddSkillID)
	
	local Skills={}
	Skills[1] = "skillid_1"
	Skills[2] = "skillid_2"
	Skills[3] = "skillid_3"
	Skills[4] = "skillid_4"
	Skills[5] = "skillid_5"
	Skills[6] = "skillid_6"
	local Skilllvls={}
	Skilllvls[1] = "skilllvl_1"
	Skilllvls[2] = "skilllvl_2"
	Skilllvls[3] = "skilllvl_3"
	Skilllvls[4] = "skilllvl_4"
	Skilllvls[5] = "skilllvl_5"
	Skilllvls[6] = "skilllvl_6"
	for i = 1,6 do
		if pet:QueryData(DataIndex[Skills[i]]) == 0 then 
			pet:UpdateData(DataIndex[Skills[i]], 5027+math.ceil(math.random(0,1)))
			pet:UpdateData(DataIndex[Skilllvls[i]], math.ceil(math.random(0,30)))
			break
		end
	end	
---	pet:UpdateData(DataIndex["skillid_6"], 5027+math.ceil(math.random(0,1)))
---	pet:UpdateData(DataIndex["skilllvl_6"], math.ceil(math.random(0,40)))

	pet:UpdateData(DataIndex["shape"],7)

	ZLuaPetBedgeRebuild(pet_tid,pet,_,_,_)
	return pet_tid,0,0
end

---//宠物改变形象处理函数
function PetBedgeChangeShape(pet_tid,pet,shape_id)
	local DataIndex=ZLuaPetBedgeDataIndex()
	local SpecialIndex=ZLuaPetBedgeSpecialIndex()
	local availablePets = ZLuaAvailablePets()
	local Race=ZLuaPetBedgeRaceDesc()

	if pet:QueryRebornCnt() ~= 1 then return 0,0,9 end
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return 0 end
	if pet == nil then return 0 end

	for i,v in pairs(availablePets) do
		if v.pet_tid == pet_tid then
			if v.combine_able==0 or v.changefigure_able==0 then   
			return 0,0,216
			end
		end 
	end

	if shape_id == 1 then
		pet:UpdateData(DataIndex["shape"],1)
	elseif shape_id == 2 and pet:QueryStar()>=12 then
		pet:UpdateData(DataIndex["shape"],3+pet:QueryData(DataIndex["isaberrant"]))
	elseif shape_id == 3 and pet:QueryStar()>=18  then
		pet:UpdateData(DataIndex["shape"],5+pet:QueryData(DataIndex["isaberrant"]))
	elseif shape_id == 4 then	
		pet:UpdateData(DataIndex["shape"],7)
	else return
	end

	ZLuaPetBedgeRebuild(pet_tid,pet,_,_,_)
	return pet_tid,0,0
end

---//宠物飞升属性修炼处理
function PetBedgeRefineAttr(pet_tid,pet,origin,master_level,attr_type,assist_id,assist_num)
	local DataIndex=ZLuaPetBedgeDataIndex()
	local SpecialIndex=ZLuaPetBedgeSpecialIndex()
	local availablePets = ZLuaAvailablePets()
	local Race=ZLuaPetBedgeRaceDesc()

	local rise = 0
	local riseRat = 0

	for i,v in pairs(availablePets) do
		if v.pet_tid == pet_tid and v.combine_able==0 and v.fixage_able==0 then   
			return 0,0,98
		end 
	end

 	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return 0 end
	if pet == nil then return 0 end
	if pet:QueryRebornCnt() ~= 1 then return 0 end
	if assist_id == nil then return 0 end
	if attr_type >14 or attr_type <1 then return 0 end

	local rebornAttrLim={}
	rebornAttrLim[0]	= 60000 
	rebornAttrLim[1]	= 60000 
	rebornAttrLim[2]	= 5000 
	rebornAttrLim[3]	= 6000 
	rebornAttrLim[4]	= 2000 
	rebornAttrLim[5]	= 500 
	rebornAttrLim[6]	= 500 
	rebornAttrLim[7]	= 600 
	rebornAttrLim[8]	= 1800 
	rebornAttrLim[9]	= 500 
	rebornAttrLim[10]	= 500 
	rebornAttrLim[11]	= 500 
	rebornAttrLim[12]	= 500 
	rebornAttrLim[13]	= 500 
	
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
	
	if assist_id ~= 18794 or assist_id == nil then return 0,0,217 end
	
	if assist_id == 18794 then
		if assist_num == 1 then
 			rise = 10
 			riseRat = 0.1*(1.0-0.1*rebornAttrLevel[attr_type-1])
 		elseif assist_num == 10 then
 			rise = 11
 			riseRat = 1.0*(1.2-0.1*rebornAttrLevel[attr_type-1])
 		elseif assist_num == 100 then
 			rise = 120
 			riseRat = 1.0*(1.4-0.1*rebornAttrLevel[attr_type-1])
 		else
 			return 0,0,122
 		end
 	end

	local k = 0
	if math.random()<riseRat then
		k = 1
	end
	
	local t = {}
	for i=0,13 do
		t[i] = rebornAttrLim[i]*0.0001
	end

 ---	if rebornAttrLevel[attr_type-1] >= 10 then return 0,0,211 end
	if pet:QueryRebornPropAdd(attr_type-1)>=rebornAttrLim[attr_type-1] then return 0,0,211 end
	
	for j =1,14 do
		if attr_type == j then
				pet:SetRebornPropAdd(j-1,pet:QueryRebornPropAdd(j-1) + k*t[j-1]*rise)
				if pet:QueryRebornPropAdd(j-1)>=rebornAttrLim[j-1] then
					pet:SetRebornPropAdd(j-1,rebornAttrLim[j-1])
				end
		end
	end
--[[
	for i=0,13 do
		if rebornAttrLevel[i] >= 10 then 
			pet:SetRebornPropAdd(i,rebornAttrLim[i])
		end 
	end
	--]]
	ZLuaPetBedgeRebuild(pet_tid,pet,_,_,_)
	return pet_tid,0,0
end

----概率处理函数-----------------------------------------------------------------------------------
---成功概率为chance--------------------------------------------------------------------------------
function ZPetBedge_ChanceCalc(chance)
	if type(chance)~="number" or chance<0 then return 0 end 
	local c
	local cx={}
	if chance>=100 then 
		c=1
	elseif chance<=0 then 
		c=0
	else 
		cx={100-chance,chance}
		c=ZPetGrade_Get_RanPoint(cx)-1
	end 
	return c	
end 
---------------------------------------------------------------------------------------------------