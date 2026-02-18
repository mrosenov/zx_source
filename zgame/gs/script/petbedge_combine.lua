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
		---//					effect4(基础成功概率>100,实际成功率为effect4-本体级别)
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
		
		---//成长观察药剂(美工专用，临时屏蔽)
		---CidMatrix[18725]={type=35,effect1=0,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}

		---//轮回改变
		CidMatrix[19183]={type=36,effect1=0,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}		

		---//神秘豆汁(固定值攻击和防御成长)
		CidMatrix[20481]={type=37,effect1=1,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}		
		CidMatrix[20483]={type=37,effect1=10,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
		CidMatrix[20484]={type=37,effect1=50,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}	
				
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
		CidMatrix[18725]={type=39,effect1=0,effect2=0,effect3=0,effect4=0,effect5=0,effect6=0,effect7=0}
		
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
	
	for i=1,7 do
		if petskill_id[i]==CidMatrix[cid].effect2 then 
				IsExistSameSkill=1
		---//存在相同技能
		---//使用了技能修炼辅助道具：必须使用宠物不会的技能卷且会增加技能栏数
				if UsingAidType==1 and aid~=0 then
					return 0,0,94
				end  
				if ZPetBedge_ChanceCalc(CidMatrix[cid].effect4-petskill_lvl[i])==1 and petskill_lvl[i]<=30 then 
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
					if petskill_lvl[i]>30 then 
						return 0,0,11
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
		else 
			if 	TotalSkills==1 then 
				pet_out:UpdateData(DataIndex["skillid_2"],CidMatrix[cid].effect2)	
				pet_out:UpdateData(DataIndex["skilllvl_2"],CidMatrix[cid].effect3)
			elseif TotalSkills==2 then
				pet_out:UpdateData(DataIndex["skillid_3"],CidMatrix[cid].effect2)	
				pet_out:UpdateData(DataIndex["skilllvl_3"],CidMatrix[cid].effect3)
			elseif TotalSkills==3 then
				pet_out:UpdateData(DataIndex["skillid_4"],CidMatrix[cid].effect2)	
				pet_out:UpdateData(DataIndex["skilllvl_4"],CidMatrix[cid].effect3)
			elseif TotalSkills==4 then
				pet_out:UpdateData(DataIndex["skillid_5"],CidMatrix[cid].effect2)	
				pet_out:UpdateData(DataIndex["skilllvl_5"],CidMatrix[cid].effect3)
			elseif TotalSkills==5 then
				pet_out:UpdateData(DataIndex["skillid_6"],CidMatrix[cid].effect2)	
				pet_out:UpdateData(DataIndex["skilllvl_6"],CidMatrix[cid].effect3)
			elseif TotalSkills==6 then
				pet_out:UpdateData(DataIndex["skillid_7"],CidMatrix[cid].effect2)	
				pet_out:UpdateData(DataIndex["skilllvl_7"],CidMatrix[cid].effect3)
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
	if level<0 or level>150 then return end 
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
	local PoisonFailedSmallGrow=1
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
		
	local MainType,Element,Origin
	---宠物的主类型/五行/来源
	MainType=pet_in:QueryMainType()
	Element=pet_in:QueryElement()
	Horoscope=pet_in:QueryHoroscope()
	Origin=pet_in:QueryData(DataIndex["nativeorigin"])
	
	local MainTypeGrowEffectAdd=0
	---//宠物主类型导致的成长增益	
	MainTypeGrowEffectInc=ZLuaPetBedgeMaintypeGrowEffect(MainType,PoisonGrowType)
	
	---宠物的主要属性成长上限
	local MaxGrowGrades={}
	MaxGrowGrades=ZLuaPetBedgeMaxGrowGrades(pet_tid,MainType,Horoscope,Origin)
	
	---宠物星等超过物品的最大可使用星等
	if PoisonUseMaxStar<=NativeStar then 
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
	local MaintypeChance={100,200,200,200,100,400,400,600,600,600,600,600,0}
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
function ZLuaPetBedgeBuddhaGrow(pet_tid,pet_in,pet_out,cid,aid,origin)  
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local NewPeaSkillID=pet_in:QueryData(DataIndex["skillid_10"]) or 0
	local NewPeaSkillLevel=pet_in:QueryData(DataIndex["skilllvl_10"]) or 0
	local CidMatrix=ZLuaPetBedgeCidMatrix()
	local cideffect1=CidMatrix[cid].effect1	---type
	local cideffect2=CidMatrix[cid].effect2	---effect lvl
	
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
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local CidMatrix=ZLuaPetBedgeCidMatrix()

	return 685,0,93
end 
-----宠物魂化设置-------------------------------------------------------------------------
function ZLuaPetBedgeSetSpirit(pet_tid,pet_in,pet_out,cid,aid,origin)  
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local CidMatrix=ZLuaPetBedgeCidMatrix()

	return pet_tid,0,93
end 
---改变宠物形象-------------------------------------------------------------------------------
function ZLuaPetBedgeChangeState(pet_tid,pet_in,pet_out,cid,aid,origin) 
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local CidMatrix=ZLuaPetBedgeCidMatrix()
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
---变异宠物---------------------------------------------------------------------------
function ZLuaPetBedgeAberrantState(pet_tid,pet_in,pet_out,cid,aid,origin) 
	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	if pet_in == nil then return end
	local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local IsAberrant=pet_in:QueryData(DataIndex["isaberrant"])
	local msg_id=82
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
function PetBedgeCombine(pet_tid,pet_in,pet_out,cid,aid)
  if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return 0,1,99 end
  if pet_in == nil then return 0,1,99 end
  local AvailablePets=ZLuaAvailablePets()
  local CidMatrix=ZLuaPetBedgeCidMatrix()
  local AidMatrix=ZLuaPetBedgeAidMatrix()
  local DataIndex=ZLuaPetBedgeDataIndex()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	
	if pet_tid==AvailablePets["S_luxueqi"].pet_tid or pet_tid==AvailablePets["S_biyao"].pet_tid 
		or pet_tid==AvailablePets["S_luxueqi2"].pet_tid or pet_tid==AvailablePets["S_kuilei01"].pet_tid
		or pet_tid==AvailablePets["S_kuilei02"].pet_tid or pet_tid==AvailablePets["S_kuilei03"].pet_tid
		or pet_tid==AvailablePets["S_kuilei04"].pet_tid or pet_tid==AvailablePets["S_kuilei05"].pet_tid 
		or pet_tid==AvailablePets["S_kuilei06"].pet_tid or pet_tid==AvailablePets["S_kuilei07"].pet_tid then
		return 0,1,98
	end 
	if pet_tid==AvailablePets["Q_heyangqingwa"].pet_tid and (CidMatrix[cid].type<3 or CidMatrix[cid].type>4) then 
		return 0,1,98
	end 
	if pet_tid==AvailablePets["Q_heyangyelang"].pet_tid and (CidMatrix[cid].type<3 or CidMatrix[cid].type>4) then 
		return 0,1,98
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
 	
 	local origin=pet_in:QueryData(DataIndex["nativeorigin"])
 	local isidentify=pet_in:QueryData(DataIndex["identify"])
 	local newpet_id,msg_channel,msg_id
	local DoSpecialSpirit=0			---//专用于宠物交换spirit
	
  ---//宠物逆天重生	
  if CidMatrix[cid].type==1 then 
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeHolyReborn(pet_tid,pet_in,pet_out,cid,aid,origin)  
  elseif CidMatrix[cid].type==2 then 
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeIdentify(pet_tid,pet_in,pet_out,cid,aid,origin)
  ---//宠物改变年龄
  elseif CidMatrix[cid].type==3 then
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeChangeAge(pet_tid,pet_in,pet_out,cid,aid,origin)
  ---//宠物改变寿命
  elseif CidMatrix[cid].type==4 then
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
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeBuddhaGrow(pet_tid,pet_in,pet_out,cid,aid,origin)  
  ---//宠物魂化抽提(切换宠物类别之第一步)
  elseif CidMatrix[cid].type==39 and isidentify==1 then  
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeGetSpirit(pet_tid,pet_in,pet_out,cid,aid,origin)
  	DoSpecialSpirit=1  
  ---//宠物魂化设定(切换宠物类别之第二步)
  elseif CidMatrix[cid].type==40 and isidentify==1 then  
  	newpet_id,msg_channel,msg_id=ZLuaPetBedgeSetSpirit(pet_tid,pet_in,pet_out,cid,aid,origin) 
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
   	ZLuaPetBedgeRebuild(newpet_id,pet_out,Origin,_)
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