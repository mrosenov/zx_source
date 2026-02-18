local lottery_template = {};

function AddLotteryTemplate(id, s1, c1)
	lottery_template[tonumber(id)] = {};
	lottery_template[tonumber(id)].Select = s1;
	lottery_template[tonumber(id)].Cashing = c1;
end

----出现几率正态化-------------------------------------------------------------------------------------------
function ZLottery_Chance_Norm(n1)
  local n1_size=table.getn(n1)
	local Total_Chance=0
	local i,k,m
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
-------------------------------------------------------------------------------------------------------------
----按照出现概率随机应该选取的位置---------------------------------------------------------------------------
function ZLottery_Get_RanPoint(s1)
	local js,ron1
	ron1= math.random()                
	---print("---------------------------")     ---$$$$测试用$$$$-----
	---print("Now the random Num is:",ron1) 	---$$$$测试用$$$$-----
  local p1=ZLottery_Chance_Norm(s1)
	for js=1,table.getn(p1) do
          if ron1<=p1[js] then 
		    	---print("So We Choose:",js) 		---$$$$测试用$$$$-----
    	  		return js
		  		end
	end
	return 1
end
-------------------------------------------------------------------------------------------------------------
---找到S1中a数字的个数---------------------------------------------------------------------------------------
function ZLottery_FindSameNum(s1,a)
	local i,j
	j=0
	for i=1,table.getn(s1) do
		if s1[i]==a then 
		j=j+1
		end 
	end 
	return j	
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType6576SelectNumber(list, max_count) --- Lottery Used@20070618(神木宝盒)
	local NumList={1,2,3,4}
	local NumChance={360,360,360,180}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType7916SelectNumber(list, max_count) --- Lottery Used@20070706(玄龟金鼎)
	local NumList={1,2,3}
	local NumChance={120,50,830}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType10484SelectNumber(list, max_count) --- Lottery Used@20070723(仙工宝箱)
	local NumList={1,2}
	local NumChance={140,860}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType10722SelectNumber(list, max_count) --- Lottery Used@200708XX(灵狐元丹)
	local NumList={1,2,3,4}
	local NumChance={280,10,200,510}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType11537SelectNumber(list, max_count) --- Lottery Used@20070924(玄蛇之魂)
	local NumList={1,2,3,4}
	local NumChance={180,10,150,660}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType11742SelectNumber(list, max_count) --- Lottery Used@20071010(国庆山河扇大礼包)
	local NumList={1,2,3,4}
	local NumChance={100,0,0,0}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType11743SelectNumber(list, max_count) --- Lottery Used@20071010(国庆万毒鼎大礼包)
	local NumList={1,2,3,4}
	local NumChance={100,0,0,0}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType12006SelectNumber(list, max_count) --- Lottery Used@20071106(龙驹宝鉴)
	local NumList={1,2,3,4}
	local NumChance={100,40,100,760}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType13318SelectNumber(list, max_count) --- Lottery Used@20071123(绽魂仙羽)
	local NumList={1,2,3,4}
	local NumChance={40,240,83,637}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType13319SelectNumber(list, max_count) --- Lottery Used@20071123(乾坤遁宝)
	local NumList={1,2,3,4}
	local NumChance={159,50,105,686}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryTypeCharmSeason1SelectNumber(list, max_count,id) --- Lottery Used@20071123(第一批护符材料箱)
	local NumList={1,2,3,4}
	local NumChance={100,0,0,0}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType13377SelectNumber(list, max_count) --- Lottery Used@20071123(虚拟币箱子出产的绽魂仙羽)
	local NumList={1,2,3,4}
	local NumChance={40,240,83,637}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType13593SelectNumber(list, max_count) --- Lottery Used@20071218(月光宝盒)
	local NumList={1,2,3,4}
	local NumChance={136,216,400,248}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType13905SelectNumber(list, max_count) --- Lottery Used@20080115(狼图腾)
	local NumList={1,2,3,4}
	local NumChance={1443,4643,2000,1914}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType15677SelectNumber(list, max_count) --- Lottery Used@20080222(狮王之心)
	local NumList={1,2,3,4}
	local NumChance={1494,1710,1000,5796}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType15866SelectNumber(list, max_count) --- Lottery Used@20080315(雷光墨玉)
	local NumList={1,2,3,4}
	local NumChance={1260,2155,2934,3651}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType16550SelectNumber(list, max_count) --- Lottery Used@20080416(逐鹿御令)
	local NumList={1,2,3,4}
	local NumChance={1260,3420,3300,2020}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType18283SelectNumber(list, max_count) --- Lottery Used@20080509(麒麟金尊)
	local NumList={1,2,3,4}
	local NumChance={1260,3550,3300,1890}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType18799SelectNumber(list, max_count) --- Lottery Used@20080601(昆仑天晶)
	local NumList={1,2,3,4}
	local NumChance={2000,2160,3000,2840}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType18890SelectNumber(list, max_count) --- Lottery Used@20080619(行天圣宝)
	local NumList={1,2,3,4}
	local NumChance={1494,2160,3000,3346}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryTypeGoldSeason1SelectNumber(list, max_count) --- Lottery Used@20080704(金币回收宝箱)
	local NumList={1,2,3,4}
	local NumChance={100,0,0,0}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryTypeItemSeason1SelectNumber(list, max_count) --- Lottery Used@20080708(装备奖励宝箱)
	local NumList={1,2,3,4}
	local NumChance={100,0,0,0}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType19125SelectNumber(list, max_count) 			--- Lottery Used@20080710(神龙秘笈)
	local NumList={1,2,3,4}
	local NumChance={1450,3110,3000,2440}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType19297SelectNumber(list, max_count) 			--- Lottery Used@20080730(云光天鉴)
	local NumList={1,2,3,4}
	local NumChance={1450,2470,3000,3080}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType19434SelectNumber(list, max_count) 			--- Lottery Used@20080812(龙宫锦图)
	local NumList={1,2,3,4}
	local NumChance={1260,2470,3000,3270}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType19558SelectNumber(list, max_count) 			--- Lottery Used@20080902(如意之鼓)
	local NumList={1,2,3,4}
	local NumChance={1260,2470,3000,3270}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryTypeGoldSeason2SelectNumber(list, max_count) --- Lottery Used@20080812(金币回收宝箱/宠物技能书)
	local NumList={1,2,3,4}
	local NumChance={100,0,0,0}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType19713SelectNumber(list, max_count) 			--- Lottery Used@20080916(炎蹄之心)
	local NumList={1,2,3,4}
	local NumChance={1260,1920,3000,3820}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType20097SelectNumber(list, max_count) 			--- Lottery Used@20080930(08国庆虚拟币礼包)
	local NumList={1,2,3,4}
	local NumChance={1150,3280,3000,2570}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType20109SelectNumber(list, max_count) 			--- Lottery Used@20080930(鸣珏瑰玉)
	local NumList={1,2,3,4}
	local NumChance={1450,1000,3000,4550}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType20284SelectNumber(list, max_count) 			--- Lottery Used@20081009(苍穹光印)
	local NumList={1,2,3,4}
	local NumChance={1590,3420,3000,1990}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType20480SelectNumber(list, max_count)			--- Lottery Used@20081022A(孔雀令)
	local NumList={1,2,3,4}
	local NumChance={1260,1920,3000,3820}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType20482SelectNumber(list, max_count)			--- Lottery Used@20081022B(龙皇古卷)
	local NumList={1,2,3,4}
	local NumChance={1700,2500,3300,2500}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType20460SelectNumber(list, max_count)			--- Lottery Used@20081106(命运轮盘)
	local NumList={1,2,3,4}
	local NumChance={20,20,30,30}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType21194SelectNumber(list, max_count)			--- Lottery Used@20081113(兽骨玉匣)
	local NumList={1,2,3,4}
	local NumChance={126,247,300,327}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType21697SelectNumber(list, max_count)			--- Lottery Used@20081201(极东绽石)
	local NumList={1,2,3,4}
	local NumChance={171,311,300,218}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType21752SelectNumber(list, max_count)			--- Lottery Used@20081202(天眷神篇/雪琪月虚拟币彩票)
	local NumList={1,2,3,4}
	local NumChance={126,311,300,263}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType22126SelectNumber(list, max_count)			--- Lottery Used@20081221(云雪绘卷)
	local NumList={1,2,3,4}
	local NumChance={126,247,300,327}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType22127SelectNumber(list, max_count)			--- Lottery Used@20081230(炎光匣龙)
	local NumList={1,2,3,4}
	local NumChance={145,192,300,363}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryTypeItemSeason2SelectNumber(list, max_count) --- Lottery Used@20090113(09春节虚拟币彩票)
	local NumList={1,2,3,4}
--local NumChance={171,381,255,193}
	local NumChance={285,255,381,79}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType23144SelectNumber(list, max_count)			--- Lottery Used@20090203(轮盘:天鹅)
	local NumList={1,2,3,4}
	local NumChance={5,20,30,45}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType23162SelectNumber(list, max_count)			--- Lottery Used@20090211(轮盘:河阳驴)
	local PriceChance = {25,450,4000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {100}								-- 一等奖内部相对概率(1号)
	subChance[2] = {2,1,1,1,1,1,2}			-- 二等奖内部相对概率(3号，5号，7号，9号，11号，13号，15号)
	subChance[3] = {25,25,25,25}				-- 三等奖内部相对概率(4号，6号，12号，14号)
	subChance[4] = {2,5,5,8}						-- 四等奖内部相对概率(2号，8号，10号，16号)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1], 													PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[3]*subChance[3][1]/subSum[3],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][3]/subSum[2], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][4]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][5]/subSum[2], PriceChance[3]*subChance[3][3]/subSum[3],
		PriceChance[2]*subChance[2][6]/subSum[2], PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][7]/subSum[2], PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType23018SelectNumber(list, max_count) --- Lottery Used@20090213(09年3月虚拟币彩票：青铜宝箱)
	local NumList={1,2,3,4}
	local NumChance={21,42,30,7}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType23295SelectNumber(list, max_count) --- Lottery Used@20090304(09年3月宠物彩票：鸿蒙秘宝)
	local NumList={1,2,3,4}
	local NumChance={10,42,30,18}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType23212SelectNumber(list, max_count)			--- Lottery Used@20090211(轮盘:圣龙玉髓)
	local PriceChance = {10,250,4000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {10}												-- 一等奖内部相对概率(1号)
	subChance[2] = {100,50,100}								-- 二等奖内部相对概率(5号，9号，13号)
	subChance[3] = {1000,1000,1000,1000}			-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {25,10,10,25,25,40,40,25}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1], 													PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][3]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType23336SelectNumber(list, max_count)			--- Lottery Used@20090313(轮盘:凤栖金枝)
	local PriceChance = {20,100,3300}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {10,10}										-- 一等奖内部相对概率(1号，9号)
	subChance[2] = {40,60}										-- 二等奖内部相对概率(5号，13号)
	subChance[3] = {13,20,13,20}							-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {25,10,10,25,25,40,40,25}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[1]*subChance[1][2]/subSum[1], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType23363SelectNumber(list, max_count)			--- Lottery Used@20090318(轮盘:天韵神篇)
	local PriceChance = {20,100,4000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {10,10}										-- 一等奖内部相对概率(1号，9号)
	subChance[2] = {40,60}										-- 二等奖内部相对概率(5号，13号)
	subChance[3] = {1000,1000,1000,1000}			-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {25,10,10,25,25,40,40,25}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[1]*subChance[1][2]/subSum[1], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType23422SelectNumber(list, max_count)			--- Lottery Used@20090326(轮盘:玄文虎石)
	local PriceChance = {20,100,4000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {10}												-- 一等奖内部相对概率(1号)
	subChance[2] = {33,33,33}									-- 二等奖内部相对概率(5号，9号，13号)
	subChance[3] = {1000,1000,1000,1000}			-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {25,10,10,25,25,40,40,25}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][3]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType23423SelectNumber(list, max_count) --- Lottery Used@20090326(09年3月凌霄城副本彩票：凌霄秘宝)
	local NumList={1,2,3,4}
	local NumChance={3684,3684,1000,1632}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType24343SelectNumber(list, max_count)			--- Lottery Used@20090211(天衮靛石:獬豸)
	local PriceChance = {25,450,4000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {100}								-- 一等奖内部相对概率(1号)
	subChance[2] = {2,1,1,1,1,1,2}			-- 二等奖内部相对概率(3号，5号，7号，9号，11号，13号，15号)
	subChance[3] = {25,25,25,25}				-- 三等奖内部相对概率(4号，6号，12号，14号)
	subChance[4] = {2,5,5,8}						-- 四等奖内部相对概率(2号，8号，10号，16号)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1], 													PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[3]*subChance[3][1]/subSum[3],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][3]/subSum[2], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][4]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][5]/subSum[2], PriceChance[3]*subChance[3][3]/subSum[3],
		PriceChance[2]*subChance[2][6]/subSum[2], PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][7]/subSum[2], PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType24344SelectNumber(list, max_count) 			--- Lottery Used@20090506(09两周年虚拟币彩票)
	local NumList={1,2,3,4}
	local NumChance={1150,3280,3000,2570}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType24993SelectNumber(list, max_count)			--- Lottery Used@20090518(万兽金尊777)
	local NumList={1,2,3,4}
	local NumChance={126,216,300,358}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType24998SelectNumber(list, max_count)			--- Lottery Used@20090518(玄天宝录777)
	local NumList={1,2,3,4}
	local NumChance={126,216,330,328}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType24999SelectNumber(list, max_count)			--- Lottery Used@20090518(万兽金尊轮盘)
	local PriceChance = {200,1008,43625}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 100000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {100}												-- 一等奖内部相对概率(1号)
	subChance[2] = {33,33,33}									-- 二等奖内部相对概率(5号，9号，13号)
	subChance[3] = {25,25,25,25}			-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {25,10,10,25,25,40,40,25}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][3]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType25000SelectNumber(list, max_count)			--- Lottery Used@20090518(玄天宝录轮盘)
	local PriceChance = {200,1008,33878}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 100000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {100}												-- 一等奖内部相对概率(1号)
	subChance[2] = {33,33,33}									-- 二等奖内部相对概率(5号，9号，13号)
	subChance[3] = {25,25,25,25}			-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {25,10,10,25,25,40,40,25}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][3]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType25305SelectNumber(list, max_count)			--- Lottery Used@20090605(天竺牙雕)
	local PriceChance = {20,250,4000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20}												-- 一等奖内部相对概率(1号)
	subChance[2] = {100,50,100}								-- 二等奖内部相对概率(5号，9号，13号)
	subChance[3] = {1000,1000,1000,1000}			-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {25,10,10,25,25,40,40,25}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1], 													PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][3]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType25417SelectNumber(list, max_count)			--- Lottery Used@20090623(血月镇木)
	local PriceChance = {20,100,4000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {10,10}										-- 一等奖内部相对概率(1号，9号)
	subChance[2] = {40,60}										-- 二等奖内部相对概率(5号，13号)
	subChance[3] = {1000,1000,1000,1000}			-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {25,10,10,25,25,40,40,25}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[1]*subChance[1][2]/subSum[1], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType25418SelectNumber(list, max_count)			--- Lottery Used@20090623(大漠金沙)
	local PriceChance = {20,100,4000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20}												-- 一等奖内部相对概率(1号)
	subChance[2] = {33,33,33}									-- 二等奖内部相对概率(5号，9号，13号)
	subChance[3] = {1000,1000,1000,1000}			-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {25,10,10,25,25,40,40,25}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1], 													PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][3]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType25749SelectNumber(list, max_count)			--- Lottery Used@20090722(太苍空岩)
	local PriceChance = {20,100,4000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20}												-- 一等奖内部相对概率(1号)
	subChance[2] = {33,33,33}									-- 二等奖内部相对概率(5号，9号，13号)
	subChance[3] = {1000,1000,1000,1000}			-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {25,10,10,25,25,40,40,25}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1], 													PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][3]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType25750SelectNumber(list, max_count)			--- Lottery Used@20090723(九叶仙芝)
	local PriceChance = {32,250,5000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {10,2,20}										-- 一等奖内部相对概率(1,9,13)
	subChance[2] = {60,50,50,50,40}							-- 二等奖内部相对概率(3,5,7,11,15)
	subChance[3] = {1250,500,2000,1250}					-- 三等奖内部相对概率(4,6,12,14)
	subChance[4] = {25,10,40,25}								-- 四等奖内部相对概率(2,8,10,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[3]*subChance[3][1]/subSum[3],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[2]*subChance[2][3]/subSum[2], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[1]*subChance[1][2]/subSum[1], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[2]*subChance[2][4]/subSum[2], PriceChance[3]*subChance[3][3]/subSum[3],
		PriceChance[1]*subChance[1][3]/subSum[1], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[2]*subChance[2][5]/subSum[2], PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType28573SelectNumber(list, max_count)			--- Lottery Used@20090804(混沌秘宝)
	local PriceChance = {10,240,4575}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {10,50,20,20}							-- 一等奖内部相对概率
	subChance[2] = {40,60,50,50,40}						-- 二等奖内部相对概率
	subChance[3] = {175,2500,2000}						-- 三等奖内部相对概率
	subChance[4] = {1505,1000,1000,1670}			-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[2]*subChance[2][5]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType28703SelectNumber(list, max_count)			--- Lottery Used@20090810(龙皇魂骨)
	local PriceChance = {10,100,4000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {10,10}										-- 一等奖内部相对概率(1号，9号)
	subChance[2] = {40,60}										-- 二等奖内部相对概率(5号，13号)
	subChance[3] = {1000,1000,1000,1000}			-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {25,10,10,25,25,40,40,25}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[1]*subChance[1][2]/subSum[1], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType32605SelectNumber(list, max_count)			--- Lottery Used@20091105(狮蛮宝录)
	local PriceChance = {20,300,5000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20}												-- 一等奖内部相对概率(1号)
	subChance[2] = {150,100,50}								-- 二等奖内部相对概率(5号，9号，13号)
	subChance[3] = {1250,500,2000,1250}				-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {25,10,10,25,25,40,40,25}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1], 													PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][3]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType32692SelectNumber(list, max_count)			--- Lottery Used@20091119(多罗慧叶)
	local PriceChance = {20,100,4500}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {10,10}										-- 一等奖内部相对概率(1号，9号)
	subChance[2] = {80,20}										-- 二等奖内部相对概率(5号，13号)
	subChance[3] = {1250,1000,1000,1250}			-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {25,10,10,25,25,40,40,25}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[1]*subChance[1][2]/subSum[1], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType33055SelectNumber(list, max_count) --- Lottery Used@20091204(2009圣诞袜子)
	local NumList={1,2,3,4}
	local NumChance={100,0,0,0}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType33122SelectNumber(list, max_count)			--- Lottery Used@20091207(圣诞兔子)
	local PriceChance = {20,200,4500}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {10,10}										-- 一等奖内部相对概率(1号，9号)
	subChance[2] = {80,120}									-- 二等奖内部相对概率(5号，13号)
	subChance[3] = {1250,1000,1000,1250}			-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {25,10,10,25,25,40,40,25}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[1]*subChance[1][2]/subSum[1], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType33530SelectNumber(list, max_count)			--- Lottery Used@20100113(玄金虎玉)
	local PriceChance = {20,100,4000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {18,2}											-- 一等奖内部相对概率(1号，9号)
	subChance[2] = {40,60}										-- 二等奖内部相对概率(5号，13号)
	subChance[3] = {1000,1000,1000,1000}			-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {40,10,10,40,40,10,10,40}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[1]*subChance[1][2]/subSum[1], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType33822SelectNumber(list, max_count)			--- Lottery Used@20100202(贺岁桃符)
	local PriceChance = {22,200,3500}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {60,70,20,70}						-- 一等奖内部相对概率
	subChance[2] = {50,60,40,50}						-- 二等奖内部相对概率
	subChance[3] = {20,15,15,20}						-- 三等奖内部相对概率
	subChance[4] = {10,40,40,10}						-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType33823SelectNumber(list, max_count)			--- Lottery Used@20100202(迎新春联)
	local PriceChance = {24,200,2000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {100,30,10,100}					-- 一等奖内部相对概率
	subChance[2] = {50,60,40,50}						-- 二等奖内部相对概率
	subChance[3] = {5,15,15,5}							-- 三等奖内部相对概率
	subChance[4] = {10,40,40,10}						-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType33988SelectNumber(list, max_count) --- Lottery Used@20100204(2010春节红包)
	local NumList={1,2,3,4}
	local NumChance={2160,3110,2000,2730}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType34124SelectNumber(list, max_count)										--- Lottery Used@20100303(金宇帝书)
	local PriceChance = {10,100,3500}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {10,10}										-- 一等奖内部相对概率(1号，9号)
	subChance[2] = {40,60}										-- 二等奖内部相对概率(5号，13号)
	subChance[3] = {20,15,15,20}							-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {40,10,10,40,40,10,10,40}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[1]*subChance[1][2]/subSum[1], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType34125SelectNumber(list, max_count)										--- Lottery Used@20100303(御贡锦匣)
	local PriceChance = {25,250,2000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {10,15}											-- 一等奖内部相对概率(1号，9号)
	subChance[2] = {150,100}										-- 二等奖内部相对概率(5号，13号)
	subChance[3] = {5,15,15,5}									-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {50,40,10,50,50,10,40,50}		-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[1]*subChance[1][2]/subSum[1], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType34295SelectNumber(list, max_count)										--- Lottery Used@20100323(锦绣绘卷)
	local PriceChance = {20,100,3500}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {10,10}										-- 一等奖内部相对概率(1号，9号)
	subChance[2] = {80,20}										-- 二等奖内部相对概率(5号，13号)
	subChance[3] = {20,15,15,20}							-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {40,10,10,40,40,10,10,40}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[1]*subChance[1][2]/subSum[1], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType34296SelectNumber(list, max_count)			--- Lottery Used@20100323(天宫神石)
	local PriceChance = {10,150,2000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {10}												-- 一等奖内部相对概率(1号)
	subChance[2] = {30,50,70}									-- 二等奖内部相对概率(5号，9号，13号)
	subChance[3] = {50,150,150,50}						-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {50,40,10,50,50,10,40,50}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1], 													PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][3]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType34937SelectNumber(list, max_count)										--- Lottery Used@20100415(火焰纹章)
	local PriceChance = {12,100,3500}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {10,2}											-- 一等奖内部相对概率(1号，9号)
	subChance[2] = {40,60}										-- 二等奖内部相对概率(5号，13号)
	subChance[3] = {20,15,15,20}							-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {40,10,10,40,40,10,10,40}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[1]*subChance[1][2]/subSum[1], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType35165SelectNumber(list, max_count)			--- Lottery Used@20100422(寻梦玉蝶)
	local PriceChance = {10,150,2000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {10}												-- 一等奖内部相对概率(1号)
	subChance[2] = {50,50,50}									-- 二等奖内部相对概率(5号，9号，13号)
	subChance[3] = {50,150,150,50}						-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {50,40,10,50,50,10,40,50}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1], 													PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][3]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType35360SelectNumber(list, max_count)										--- Lottery Used@20100504(天池灵泉)
	local PriceChance = {10,100,3500}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {5,5}											-- 一等奖内部相对概率(1号，9号)
	subChance[2] = {40,60}										-- 二等奖内部相对概率(5号，13号)
	subChance[3] = {20,15,15,20}							-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {40,10,10,40,40,10,10,40}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[1]*subChance[1][2]/subSum[1], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType35361SelectNumber(list, max_count)			--- Lottery Used@20100504(五光锦匣)
	local PriceChance = {10,150,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {10}												-- 一等奖内部相对概率(1号)
	subChance[2] = {50,50,50}									-- 二等奖内部相对概率(5号，9号，13号)
	subChance[3] = {750,750,750,750}					-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {50,40,10,50,50,10,40,50}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1], 													PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][3]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType35362SelectNumber(list, max_count)			--- Lottery Used@20100504(天工开物)
	local PriceChance = {20,150,2000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20}												-- 一等奖内部相对概率(1号)
	subChance[2] = {40,50,60}									-- 二等奖内部相对概率(5号，9号，13号)
	subChance[3] = {50,150,150,50}					-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {50,40,10,50,50,10,40,50}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1], 													PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][3]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType35363SelectNumber(list, max_count)			--- Lottery Used@20100504(聚灵神龛)
	local PriceChance = {20,150,4000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20}												-- 一等奖内部相对概率(1号)
	subChance[2] = {40,50,60}									-- 二等奖内部相对概率(5号，9号，13号)
	subChance[3] = {1000,1000,1000,1000}			-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {50,40,10,50,50,10,40,50}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1], 													PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][3]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType40344SelectNumber(list, max_count)			--- Lottery Used@20100527(十全界书)
	local PriceChance = {18,150,4000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {100,40,20,20}						-- 一等奖内部相对概率
	subChance[2] = {25,60,40,25}						-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}						-- 三等奖内部相对概率
	subChance[4] = {80,20,20,80}						-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType40345SelectNumber(list, max_count)			--- Lottery Used@20100527(天关幻罡)
	local PriceChance = {20,100,2500}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20}												-- 一等奖内部相对概率(1号)
	subChance[2] = {33,33,33}									-- 二等奖内部相对概率(5号，9号，13号)
	subChance[3] = {500,1000,500,500}					-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {30,20,20,30,30,20,20,30}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1], 													PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][3]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType40346SelectNumber(list, max_count)			--- Lottery Used@20100527(伏灵神引)
	local PriceChance = {20,100,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20}												-- 一等奖内部相对概率(1号)
	subChance[2] = {33,33,33}									-- 二等奖内部相对概率(5号，9号，13号)
	subChance[3] = {500,1000,1000,500}				-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {30,20,20,30,30,20,20,30}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1], 													PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][3]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType40529SelectNumber(list, max_count)			--- Lottery Used@20100623(暗月真解)
	local PriceChance = {20,150,4000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20}												-- 一等奖内部相对概率(1号)
	subChance[2] = {40,50,60}									-- 二等奖内部相对概率(5号，9号，13号)
	subChance[3] = {1000,1000,1000,1000}			-- 三等奖内部相对概率(3号，7号，11号，15号)
	subChance[4] = {50,40,10,50,50,10,40,50}	-- 四等奖内部相对概率(2,4,6,8,10,12,14,16)

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1], 													PriceChance[4]*subChance[4][1]/subSum[4], PriceChance[3]*subChance[3][1]/subSum[3], PriceChance[4]*subChance[4][2]/subSum[4],
		PriceChance[2]*subChance[2][1]/subSum[2], PriceChance[4]*subChance[4][3]/subSum[4], PriceChance[3]*subChance[3][2]/subSum[3], PriceChance[4]*subChance[4][4]/subSum[4],
		PriceChance[2]*subChance[2][2]/subSum[2], PriceChance[4]*subChance[4][5]/subSum[4], PriceChance[3]*subChance[3][3]/subSum[3], PriceChance[4]*subChance[4][6]/subSum[4],
		PriceChance[2]*subChance[2][3]/subSum[2], PriceChance[4]*subChance[4][7]/subSum[4], PriceChance[3]*subChance[3][4]/subSum[3], PriceChance[4]*subChance[4][8]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType40670SelectNumber(list, max_count)			--- Lottery Used@20100706(八极穹炉)
	local PriceChance = {7,150,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {25,10,10,25}					-- 一等奖内部相对概率
	subChance[2] = {25,50,50,25}						-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}							-- 三等奖内部相对概率
	subChance[4] = {10,40,40,10}						-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType40928SelectNumber(list, max_count)			--- Lottery Used@20100804(龙蛇演义)
	local PriceChance = {20,150,4000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {10,10,10,10}					-- 一等奖内部相对概率
	subChance[2] = {25,50,50,25}						-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}							-- 三等奖内部相对概率
	subChance[4] = {10,40,40,10}						-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType41080SelectNumber(list, max_count)			--- Lottery Used@20100817(洛华天羽)
	local PriceChance = {20,200,4000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {70,50,70,10}					-- 一等奖内部相对概率
	subChance[2] = {25,25,25,25}					-- 二等奖内部相对概率
	subChance[3] = {30,10,30,10}					-- 三等奖内部相对概率
	subChance[4] = {10,40,40,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType41307SelectNumber(list, max_count)			--- Lottery Used@20100915(千华锦萃)
	local PriceChance = {30,300,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {25,50,25,50}					-- 一等奖内部相对概率
	subChance[2] = {25,50,50,25}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,20}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType41391SelectNumber(list, max_count)			--- Lottery Used@20100928(啸天金哨)
	local PriceChance = {20,200,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {60,25,5,10}					-- 一等奖内部相对概率
	subChance[2] = {50,50,50,50}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,15,15}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType42338SelectNumber(list, max_count)			--- Lottery Used@20101129(扶桑巨木)
	local PriceChance = {30,300,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {25,50,25,50}					-- 一等奖内部相对概率
	subChance[2] = {25,50,50,25}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,20}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType42353SelectNumber(list, max_count)			--- Lottery Used@20101202(琅嬛琼琚-仙御)
	local PriceChance = {40,200,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20,5,13,2}					-- 一等奖内部相对概率
	subChance[2] = {50,60,50,40}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType42659SelectNumber(list, max_count)			--- Lottery Used@20101202(测试可堆叠彩票)
	local PriceChance = {40,200,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20,5,13,2}					-- 一等奖内部相对概率
	subChance[2] = {50,60,50,40}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType42664SelectNumber(list, max_count)			--- Lottery Used@20101227(测试可堆叠彩票2)
	local PriceChance = {40,200,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20,5,13,2}					-- 一等奖内部相对概率
	subChance[2] = {50,60,50,40}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType42665SelectNumber(list, max_count)			--- Lottery Used@20101227(吉祥如意)
	local PriceChance = {40,200,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20,40,150,150,40}					-- 一等奖内部相对概率（1、5、7、11、13）
	subChance[2] = {50,50,100}									-- 二等奖内部相对概率（3、9、15）
	subChance[3] = {10,10,10,10}							-- 三等奖内部相对概率（4、6、12、14）
	subChance[4] = {10,10,10,10}							-- 四等奖内部相对概率（2、8、10、16）

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][5]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType42784SelectNumber(list, max_count)			--- Lottery Used@20110128（混元秘藏）
	local PriceChance = {20,200,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {30,50,40,30,50}					-- 一等奖内部相对概率（1、5、7、11、13）
	subChance[2] = {40,100,60}									-- 二等奖内部相对概率（3、9、15）
	subChance[3] = {10,10,10,10}							-- 三等奖内部相对概率（4、6、12、14）
	subChance[4] = {10,10,10,10}							-- 四等奖内部相对概率（2、8、10、16）

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][5]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType42868SelectNumber(list, max_count)			--- Lottery Used@20110217(金玉良橼)
	local PriceChance = {40,200,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20,5,13,2}					-- 一等奖内部相对概率
	subChance[2] = {50,60,40,50}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType43403SelectNumber(list, max_count)			--- Lottery Used@20110330(2011兔子彩票)
	local PriceChance = {40,200,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20,9,2,9}					-- 一等奖内部相对概率
	subChance[2] = {50,50,50,50}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType44403SelectNumber(list, max_count)			--- Lottery Used@20110429(天心雪华)
	local PriceChance = {20,200,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20,80,20,80}					-- 一等奖内部相对概率
	subChance[2] = {50,50,50,50}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType44638SelectNumber(list, max_count)			--- Lottery Used@20110531(金龙宝匣)
	local PriceChance = {20,200,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20,90,20,90}					-- 一等奖内部相对概率
	subChance[2] = {20,60,60,60}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType44764SelectNumber(list, max_count)			--- Lottery Used@20110629(幻缘仙尊)
	local PriceChance = {35,200,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {13,17,5}					-- 一等奖内部相对概率
	subChance[2] = {30,20,100,20,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do 
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][5]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType44766SelectNumber(list, max_count)			--- Lottery Used@20110629(惊鸿玉砚)
	local PriceChance = {28,180,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {10,8,2,8}					-- 一等奖内部相对概率
	subChance[2] = {70,30,30,50}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType44823SelectNumber(list, max_count)			--- Lottery Used@20120419(鸿蒙宝气)
	local PriceChance = {20,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {50,20,50,80}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType50621SelectNumber(list, max_count)			--- Lottery Used@20110818(太极初章)
	local PriceChance = {20,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {50,20,50,80}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType50824SelectNumber(list, max_count)			--- Lottery Used@20111010(桃源秘录)
	local PriceChance = {40,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20,3,2,15}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType50910SelectNumber(list, max_count)			--- Lottery Used@20111102(任务用金丹彩票)
	local PriceChance = {500,1500,6000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20,20,20,20}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType50911SelectNumber(list, max_count)			--- Lottery Used@20111102(任务用造化彩票)
	local PriceChance = {500,1500,6000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20,20,20,20}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType50912SelectNumber(list, max_count)			--- Lottery Used@20111102(任务用元神珠彩票)
	local PriceChance = {500,1500,6000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20,20,20,20}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType55493SelectNumber(list, max_count)			--- Lottery Used@20111102(任务用元神珠彩票)
	local PriceChance = {6,1500,6000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20,20,20,20}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType50913SelectNumber(list, max_count)			--- Lottery Used@20111102(任务用宠物装备彩票)
	local PriceChance = {2000,2000,4000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20,20,20,20}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType50914SelectNumber(list, max_count)			--- Lottery Used@20111102(任务用日月星沙包彩票)
	local PriceChance = {800,1000,6000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20,20,20,20}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType50984SelectNumber(list, max_count)			--- Lottery Used@20111115(九色圣丹)
	local PriceChance = {40,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {22,8,2,8}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType51097SelectNumber(list, max_count)			--- Lottery Used@20111215(龙宫仙芝)
	local PriceChance = {40,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {28,5,2,5}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType51203SelectNumber(list, max_count)			--- Lottery Used@201120131(不灭龙魂)
	local PriceChance = {40,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20,15,2,3}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType51267SelectNumber(list, max_count)			--- Lottery Used@20120220(三生灵石)
	local PriceChance = {40,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {22,8,2,8}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType51452SelectNumber(list, max_count)			--- Lottery Used@20120327(五色烟云)
	local PriceChance = {40,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {22,8,2,8}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType52346SelectNumber(list, max_count)			--- Lottery Used@20120426(时光宝鉴)
	local PriceChance = {30,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {18,5,2,5}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType52457SelectNumber(list, max_count)			--- Lottery Used@20120607(深海之心)
	local PriceChance = {40,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {28,5,2,5}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType52471SelectNumber(list, max_count)			--- Lottery Used@20120628(夸父)
	local PriceChance = {40,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {22,8,2,8}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType52680SelectNumber(list, max_count)			--- Lottery Used@20120802(蝴蝶飞剑)
	local PriceChance = {30,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {18,5,2,5}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType54031SelectNumber(list, max_count)			--- Lottery Used@20120831(任务用天晶彩票)
	local PriceChance = {200,2000,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {5,5,5,5}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType52751SelectNumber(list, max_count)			---20120810(天缘果)
	local PriceChance = {150,1000,1000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {15,35,15,35}							    -- 一等奖内部相对概率（1,5,9,13）
	subChance[2] = {25,25,25,25}					   	    -- 二等奖内部相对概率（3,7,11,15）
	subChance[3] = {25,25,25,25}						      -- 三等奖内部相对概率（4,6,12,14）
	subChance[4] = {3000,900,1000,3000}			      -- 四等奖内部相对概率（2,8,10,16）

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType54054SelectNumber(list, max_count)			--- Lottery Used@201220901(猴子坐骑)
	local PriceChance = {30,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {18,5,2,5}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType54858SelectNumber(list, max_count)			--- Lottery Used@20121010(夜阑听雪)
	local PriceChance = {30,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {18,5,2,5}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType54886SelectNumber(list, max_count)			--- Lottery Used@20121026(洞天真火)
	local PriceChance = {20,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {8,7,2,3}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType54909SelectNumber(list, max_count)			--- Lottery Used@20121107(国战用彩票)
	local PriceChance = {150,2000,3920}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {45,50,5,50}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType61160SelectNumber(list, max_count)			--- Lottery Used@20130912(任务用帮派彩票)
	local PriceChance = {1,9,1500}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {10,10,10,10}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType54929SelectNumber(list, max_count)			--- Lottery Used@20121113(碧色雕笼)
	local PriceChance = {40,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20,10,2,8}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType55541SelectNumber(list, max_count)			--- Lottery Used@20130107(金蛇狂舞)
	local PriceChance = {20,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {80,50,20,50}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType55590SelectNumber(list, max_count)			--- Lottery Used@20130205(神灯驼)
	local PriceChance = {30,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {50,20,50,180}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType55792SelectNumber(list, max_count)			--- Lottery Used@20130305(玉楼春)
	local PriceChance = {40,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20,10,2,8}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType55966SelectNumber(list, max_count)			--- Lottery Used@20120407(蓦山溪)
	local PriceChance = {40,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {225,80,15,80}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType57426SelectNumber(list, max_count)			--- Lottery Used@20130419(望仙门)
	local PriceChance = {30,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {20,4,2,4}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType58313SelectNumber(list, max_count)			--- Lottery Used@20130606(摸鱼儿)
	local PriceChance = {40,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {220,80,15,85}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType58365SelectNumber(list, max_count)			--- Lottery Used@20130626(瑞云浓)
	local PriceChance = {35,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {225,80,15,30}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType58555SelectNumber(list, max_count)			--- Lottery Used@20130801(谒金门)
	local PriceChance = {30,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {185,50,15,50}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType58866SelectNumber(list, max_count)			--- Lottery Used@20130815(洞天神石)
	local PriceChance = {5,100,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 100000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {10,10,10,10}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType60300SelectNumber(list, max_count)			--- Lottery Used@20130826(鹊桥仙)
	local PriceChance = {35,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {200,85,15,100}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType61139SelectNumber(list, max_count)			--- Lottery Used@20130912(少年游)
	local PriceChance = {30,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {50,15,50,185}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType62459SelectNumber(list, max_count)			--- Lottery Used@20131031(渡江云)
	local PriceChance = {35,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {200,85,15,50}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType62462SelectNumber(list, max_count)			--- Lottery Used@20131031(仙豆)
	local PriceChance = {5,100,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 100000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {10,10,10,10}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType62773SelectNumber(list, max_count)			--- Lottery Used@20131129(落梅风)
	local PriceChance = {40,120,3000}																						-- 一二三等奖概率(单位:万分之)
	PriceChance[4] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3]		--     四等奖概率(单位:万分之)
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {200,85,15,100}					-- 一等奖内部相对概率
	subChance[2] = {30,30,30,30}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,10,10}					-- 四等奖内部相对概率

	for j=1,4 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16}
	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[2]*subChance[2][1]/subSum[2], 
		PriceChance[3]*subChance[3][1]/subSum[3],
		
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[2]*subChance[2][2]/subSum[2], 
		PriceChance[4]*subChance[4][2]/subSum[4],

		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[2]*subChance[2][3]/subSum[2], 
		PriceChance[3]*subChance[3][3]/subSum[3],

		PriceChance[1]*subChance[1][4]/subSum[1], 
		PriceChance[3]*subChance[3][4]/subSum[3], 
		PriceChance[2]*subChance[2][4]/subSum[2], 
		PriceChance[4]*subChance[4][4]/subSum[4],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType58780SelectNumber(list, max_count)	--- Lottery Used@20130809(染色剂)
	local PriceChance = {1,2,4,8,16}																						-- 一二三等奖概率(单位:万分之)
	local subChance = {}
	local subSum = {}
	subChance[1] = {4,4,4,4,5,5,5,5,6,6,6,6}					-- 一等奖内部相对概率
	subChance[2] = {4,4,4,4,5,5,5,5,6,6,6,6}					-- 二等奖内部相对概率
	subChance[3] = {4,4,4,4,5,5,5,5,6,6,6,6}					-- 三等奖内部相对概率
	subChance[4] = {4,4,4,4,5,5,5,5,6,6,6,6}					-- 四等奖内部相对概率
	subChance[5] = {4,4,4,4,5,5,5,5,6,6,6,6}					-- 五等奖内部相对概率

	for j=1,5 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList={}
	local NumChance={}
	for k=1,60 do
		NumList[k]=k
		j=(k-1)%12
		i=(k-1-j)/12+1
		NumChance[k]=PriceChance[i]*subChance[i][j+1]/subSum[i]
	end	

	return NumList[ZLottery_Get_RanPoint(NumChance)]
end

-------------------------------------------------------------------------------------------------------------
-------------------------------煮啊煮啊煮元宵----------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType55442SelectNumber(list, max_count)			--- Lottery Used@20121113(测试彩票)
	local PriceChance	= {2,5,10,20,120,500,1000}
	PriceChance[8] = 10000 - PriceChance[1] - PriceChance[2] - PriceChance[3] - PriceChance[4] -PriceChance[5] -PriceChance[6] -PriceChance[7]		--     四等奖概率(单位:万分之)


	for i = 1,7 do
		if max_count >= i then
			PriceChance[8-i] = PriceChance[8-i] + PriceChance[9-i]
			PriceChance[9-i] = 0
		end
	end
	
	local subChance = {}
	local subSum = {}
	subChance[1] = {10,10,10,0}					-- 一等奖内部相对概率
	subChance[2] = {30,30,0,0}					-- 二等奖内部相对概率
	subChance[3] = {10,10,10,10}					-- 三等奖内部相对概率
	subChance[4] = {10,10,0,0}					-- 四等奖内部相对概率
	subChance[5] = {10,10,0,0}					-- 五等奖内部相对概率
	subChance[6] = {10,10,0,0}					-- 六等奖内部相对概率
	subChance[7] = {30,30,0,0}					-- 七等奖内部相对概率
	subChance[8] = {10,10,0,0}					-- 八等奖内部相对概率
	
	for j=1,8 do
		subSum[j] = 0
		for i,_ in pairs(subChance[j]) do
			subSum[j] = subSum[j] + subChance[j][i]
		end
	end
	
	local NumList = {29,30,31,32,25,26,27,28,21,22,23,24,17,18,19,20,13,14,15,16,9,10,11,12,5,6,7,8,1,2,3,4}

	local NumChance =
	{
		PriceChance[1]*subChance[1][1]/subSum[1], 
		PriceChance[1]*subChance[1][2]/subSum[1], 
		PriceChance[1]*subChance[1][3]/subSum[1], 
		PriceChance[1]*subChance[1][4]/subSum[1],
		
		PriceChance[2]*subChance[2][1]/subSum[2],
		PriceChance[2]*subChance[2][2]/subSum[2],
		PriceChance[2]*subChance[2][3]/subSum[2],
		PriceChance[2]*subChance[2][4]/subSum[2],
		
		PriceChance[3]*subChance[3][1]/subSum[3], 
		PriceChance[3]*subChance[3][2]/subSum[3], 
		PriceChance[3]*subChance[3][3]/subSum[3], 
		PriceChance[3]*subChance[3][4]/subSum[3],

		PriceChance[4]*subChance[4][1]/subSum[4], 
		PriceChance[4]*subChance[4][2]/subSum[4], 
		PriceChance[4]*subChance[4][3]/subSum[4], 
		PriceChance[4]*subChance[4][4]/subSum[4],

		PriceChance[5]*subChance[5][1]/subSum[5], 
		PriceChance[5]*subChance[5][2]/subSum[5], 
		PriceChance[5]*subChance[5][3]/subSum[5], 
		PriceChance[5]*subChance[5][4]/subSum[5],

		PriceChance[6]*subChance[6][1]/subSum[6], 
		PriceChance[6]*subChance[6][2]/subSum[6], 
		PriceChance[6]*subChance[6][3]/subSum[6], 
		PriceChance[6]*subChance[6][4]/subSum[6],

		PriceChance[7]*subChance[7][1]/subSum[7], 
		PriceChance[7]*subChance[7][2]/subSum[7], 
		PriceChance[7]*subChance[7][3]/subSum[7], 
		PriceChance[7]*subChance[7][4]/subSum[7],
	
		PriceChance[8]*subChance[8][1]/subSum[8], 
		PriceChance[8]*subChance[8][2]/subSum[8], 
		PriceChance[8]*subChance[8][3]/subSum[8], 
		PriceChance[8]*subChance[8][4]/subSum[8],
	}
	return NumList[ZLottery_Get_RanPoint(NumChance)]
end
-------------------------------------------------------------------------------------------------------------
----------------------------------------------------  完美的分割线  -----------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType6576Cashing(cur_list)		---(神木宝盒)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][4713]=100	--神木骰
	
	Out_Prize[2][6473]=100	--月华之灵

	Out_Prize[3][3988]=100	--回城符
	Out_Prize[3][3991]=100
	Out_Prize[3][6430]=100
	Out_Prize[3][3989]=100
	Out_Prize[3][3990]=100
	Out_Prize[3][3992]=100
	
	Out_Prize[4][1807]=100	--太一轮
		
	local i,v,k
	
	for k=1,4 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="444" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif ZLottery_FindSameNum(cur_list,4)==2 or nowcurlist[fmax_count]=="333" or nowcurlist[fmax_count]=="222" or nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif ZLottery_FindSameNum(cur_list,4)==1 then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,2,0,0
	else 
		return 4,0,1807,1,0,0
	end
	return 4,0,1807,1,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType7916Cashing(cur_list)	---(玄龟金鼎)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][7925]=100	--龙龟
	
	Out_Prize[2][7540]=100	--寒铁面具男
	Out_Prize[2][7541]=100	--寒铁面具女
	
	Out_Prize[3][6473]=100	--月华之灵
	Out_Prize[4][1807]=100	--太一轮
		
	local i,v,k
	
	for k=1,4 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222" then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="333" or (ZLottery_FindSameNum(cur_list,1)==1 and ZLottery_FindSameNum(cur_list,2)==1 and ZLottery_FindSameNum(cur_list,3)==1) then  
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,1,0,0
	else 
		return 4,0,1807,2,0,0
	end
	return 4,0,1807,2,0,0 
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType10484Cashing(cur_list)	---(仙工宝箱)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][4708]=100	---七星剑
	Out_Prize[1][4709]=100	---四灵幡
	Out_Prize[1][4710]=100	---金刚杵
	Out_Prize[1][4711]=100	---多情环
	
	Out_Prize[2][6047]=50	---八卦石秘笈
	
	Out_Prize[2][6050]=100	---六合镜秘笈
	Out_Prize[2][6048]=100	---朱雀印秘笈
	Out_Prize[2][6051]=100	---轮回珠秘笈
	Out_Prize[2][6049]=100	---慑心铃秘笈
	
	Out_Prize[2][6054]=50	---七星剑秘笈
	Out_Prize[2][6052]=50	---四灵幡秘笈
	Out_Prize[2][6055]=50	---金刚杵秘笈
	Out_Prize[2][6053]=50	---多情环秘笈
	
	Out_Prize[2][6056]=10	---山河扇秘笈
	Out_Prize[2][6057]=10	---神木骰秘笈
	Out_Prize[2][6059]=10	---玉净瓶秘笈

	Out_Prize[3][7928]=100  --八卦石向性熔炼剂
	Out_Prize[3][7932]=100  --15级法宝向性熔炼剂
	Out_Prize[3][7935]=100  --45级法宝向性熔炼剂
	Out_Prize[3][7939]=100  --75级法宝向性熔炼剂
	
	Out_Prize[4][4703]=100  --八卦石
		
	local i,v,k
	
	for k=1,4 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif ZLottery_FindSameNum(cur_list,1)==2 then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif ZLottery_FindSameNum(cur_list,1)==1 then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,1,0,0
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,1,0,0
	end
	return 4,0,4703,1,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType10722Cashing(cur_list)	---(灵狐元丹)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][10715]=100		--灵狐
	
	Out_Prize[2][7824]=100		--脸谱面具
	
	
	Out_Prize[3][6473]=100		--月华之灵
	Out_Prize[4][8079]=100		--飞天神符
		
	local i,v,k
	
	for k=1,4 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if ZLottery_FindSameNum(cur_list,1)==1 and ZLottery_FindSameNum(cur_list,2)==1 and ZLottery_FindSameNum(cur_list,3)==1 then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif ZLottery_FindSameNum(cur_list,1)>=1 and ZLottery_FindSameNum(cur_list,2)>=1 and ZLottery_FindSameNum(cur_list,3)==0 then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif ZLottery_FindSameNum(cur_list,1)>=1 and ZLottery_FindSameNum(cur_list,2)==0 then  
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,1,0,0
	else 
		return 4,0,8079,2,0,0
	end
	return 4,0,8079,2,0,0 
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType11537Cashing(cur_list)	---(玄蛇之魂)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][11538]=100		---玄蛇
	
	Out_Prize[2][11539]=100		---无字天书
	
	
	Out_Prize[3][6473]=100		---月华之灵
	Out_Prize[4][8079]=100		---飞天神符
		
	local i,v,k
	
	for k=1,4 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if ZLottery_FindSameNum(cur_list,1)==1 and ZLottery_FindSameNum(cur_list,2)==1 and ZLottery_FindSameNum(cur_list,3)==1 then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif ZLottery_FindSameNum(cur_list,1)>=1 and ZLottery_FindSameNum(cur_list,2)>=1 and ZLottery_FindSameNum(cur_list,3)==0 then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif ZLottery_FindSameNum(cur_list,1)>=1 and ZLottery_FindSameNum(cur_list,2)==0 then  
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,1,0,0
	else 
		return 4,0,8079,2,0,0
	end
	return 4,0,8079,2,0,0 
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType11742Cashing(cur_list)	---(国庆山河扇大礼包)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
		
	if nowcurlist[fmax_count]=="111" then 
			return 1,0,4712,1,0,0
		end
	return 1,0,4712,1,0,0 
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType11743Cashing(cur_list)	---(国庆万毒鼎大礼包)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
		
	if nowcurlist[fmax_count]=="111" then 
			return 1,0,4714,1,0,0
		end
	return 1,0,4714,1,0,0 
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType12006Cashing(cur_list)	---(龙驹宝鉴)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][12005]=100		---独角兽
	
	Out_Prize[2][10869]=100		---金葫芦
	
	
	Out_Prize[3][2579]=100		---暗月石
	Out_Prize[4][8079]=100		---飞天神符
		
	local i,v,k
	
	for k=1,4 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if ZLottery_FindSameNum(cur_list,1)==1 and ZLottery_FindSameNum(cur_list,2)==1 and ZLottery_FindSameNum(cur_list,3)==1 then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif ZLottery_FindSameNum(cur_list,1)>=1 and ZLottery_FindSameNum(cur_list,2)>=1 and ZLottery_FindSameNum(cur_list,3)==0 then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif ZLottery_FindSameNum(cur_list,1)>=1 and ZLottery_FindSameNum(cur_list,2)==0 then  
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,8,0,0
	else 
		return 4,0,8079,2,0,0
	end
	return 4,0,8079,2,0,0 
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType13318Cashing(cur_list)	---(绽魂仙羽)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	Out_Prize[5]={}
	
	Out_Prize[1][13342]=100		---孤云白鹤（兑换券）	
	
	Out_Prize[2][13312]=20		---碧瑶装衣服
	Out_Prize[2][13313]=40		---碧瑶装头饰	
	Out_Prize[2][13314]=40		---碧瑶装鞋子

	Out_Prize[3][13344]=100		---//箱子兑换券(玩家可见为3等奖)  天宫秘宝		

	Out_Prize[4][11678]=25		---玄真秘文石(玩家可见为4等奖)
	Out_Prize[4][11676]=25		---魔魂秘文石
	Out_Prize[4][11679]=25		---佛尊秘文石			
	Out_Prize[4][11677]=25		---奇艳秘文石

	Out_Prize[5][8079]=100		---飞天神符	
		
	local i,v,k
	
	for k=1,5 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,5 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if ZLottery_FindSameNum(cur_list,1)==1 and ZLottery_FindSameNum(cur_list,2)==1 and ZLottery_FindSameNum(cur_list,3)==1 then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif ZLottery_FindSameNum(cur_list,1)>=1 and ZLottery_FindSameNum(cur_list,2)>=1 and ZLottery_FindSameNum(cur_list,3)==0 then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif ZLottery_FindSameNum(cur_list,4)==3 then  
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,1,0,0
	elseif ZLottery_FindSameNum(cur_list,1)>=1 and ZLottery_FindSameNum(cur_list,2)==0 then  
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,2,0,0
	else 
		return 5,0,8079,4,0,0
	end
	return 5,0,8079,4,0,0 
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType13377Cashing(cur_list)	---(虚拟币出产箱子的绽魂仙羽)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	Out_Prize[5]={}
	
	Out_Prize[1][13342]=100		---鹤（兑换券）	
	
	Out_Prize[2][13312]=20		---衣服
	Out_Prize[2][13313]=40		---头饰	
	Out_Prize[2][13314]=40		---鞋子
		
	Out_Prize[3][13344]=100		---//箱子兑换券
	
	Out_Prize[4][11678]=25		---玄真秘文石
	Out_Prize[4][11676]=25		---魔魂秘文石
	Out_Prize[4][11679]=25		---佛尊秘文石			
	Out_Prize[4][11677]=25		---奇艳秘文石
	

	Out_Prize[5][8079]=100		---飞天神符	
		
	local i,v,k
	
	for k=1,5 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,5 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if ZLottery_FindSameNum(cur_list,1)==1 and ZLottery_FindSameNum(cur_list,2)==1 and ZLottery_FindSameNum(cur_list,3)==1 then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif ZLottery_FindSameNum(cur_list,1)>=1 and ZLottery_FindSameNum(cur_list,2)>=1 and ZLottery_FindSameNum(cur_list,3)==0 then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif ZLottery_FindSameNum(cur_list,4)==3 then  
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,1,0,0
	elseif ZLottery_FindSameNum(cur_list,1)>=1 and ZLottery_FindSameNum(cur_list,2)==0 then  
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,2,0,0
	else 
		return 5,0,8079,4,0,0
	end
	return 5,0,8079,4,0,0 
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType13319Cashing(cur_list)	---(乾坤遁宝)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][10867]=100		---阴阳镜
	Out_Prize[2][13350]=100		---鸿运符
	Out_Prize[3][13343]=100		---聚宝盆
	Out_Prize[4][1807]=100		---太一轮
		
	local i,v,k
	for k=1,4 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if ZLottery_FindSameNum(cur_list,1)==1 and ZLottery_FindSameNum(cur_list,2)==1 and ZLottery_FindSameNum(cur_list,3)==1 then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif ZLottery_FindSameNum(cur_list,1)>=1 and ZLottery_FindSameNum(cur_list,2)>=1 and ZLottery_FindSameNum(cur_list,3)==0 then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,15,0,0
	elseif ZLottery_FindSameNum(cur_list,1)>=1 and ZLottery_FindSameNum(cur_list,2)==0 then  
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,1,0,0
	else 
		return 4,0,1807,6,0,0
	end
	return 4,0,1807,6,0,0 
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryTypeCharmSeason1Cashing(cur_list,id)	---(第一批护符材料)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	
	if id==13320 then  ---道逾
		Out_Prize[1][12686]=200  ---寒冰烈炎_金
		Out_Prize[1][12691]=47  ---少阳初现_金
		Out_Prize[1][12696]=63  ---破魔秘法_金
		Out_Prize[1][12701]=27  ---天眷有道_金
		Out_Prize[1][12706]=26  ---驱物法诀_金
		Out_Prize[1][12711]=31  ---护体罡气_金
		Out_Prize[1][12716]=53  ---炼神还虚_金
		Out_Prize[1][12722]=42  ---冰心玉阙_金
		Out_Prize[1][12727]=28  ---仙元护体_金
		Out_Prize[1][12732]=31  ---先天罡气_金
		Out_Prize[1][12737]=47  ---雷神降世_金
		Out_Prize[1][12742]=200  ---风剑之芒_金
		Out_Prize[1][12747]=170  ---归元真解_金
		Out_Prize[1][12752]=63  ---破魔怒意_金
		Out_Prize[1][12757]=63  ---怒剑心劫_金
		Out_Prize[1][12762]=37  ---邪剑星羽_金
		Out_Prize[1][12767]=47  ---天诛真诀_金
		Out_Prize[1][12772]=32  ---斩龙奥义_金
		Out_Prize[1][12777]=27  ---玄天星芒_金
		Out_Prize[1][12782]=170  ---剑元之聚_金
		Out_Prize[1][12788]=29  ---气贯长虹_金
		Out_Prize[1][12793]=30  ---玄冰圣护_金
		Out_Prize[1][12798]=29  ---玄冰劲气_金
		Out_Prize[1][12803]=23  ---返朴归真_金
		Out_Prize[1][12808]=23  ---狂雷天怒_金
		Out_Prize[1][12813]=38  ---冰域天机_金
		Out_Prize[1][12818]=36  ---盛冰之寒_金
		Out_Prize[1][12823]=72  ---霜寒化境_金
		Out_Prize[1][12828]=63  ---雷云奇变_金
		Out_Prize[1][12833]=63  ---五雷交轰_金
		Out_Prize[1][12838]=25  ---天雷狂啸_金
		Out_Prize[1][12843]=29  ---群雷裂空_金
		Out_Prize[1][12848]=34  ---天地无用_金

	elseif id==13321 then 
		Out_Prize[1][12687]=200  ---寒冰烈炎_木
		Out_Prize[1][12692]=47  ---少阳初现_木
		Out_Prize[1][12697]=63  ---破魔秘法_木
		Out_Prize[1][12702]=27  ---天眷有道_木
		Out_Prize[1][12707]=26  ---驱物法诀_木
		Out_Prize[1][12712]=31  ---护体罡气_木
		Out_Prize[1][12718]=53  ---炼神还虚_木
		Out_Prize[1][12723]=42  ---冰心玉阙_木
		Out_Prize[1][12728]=28  ---仙元护体_木
		Out_Prize[1][12733]=31  ---先天罡气_木
		Out_Prize[1][12738]=47  ---雷神降世_木
		Out_Prize[1][12743]=200  ---风剑之芒_木
		Out_Prize[1][12748]=170  ---归元真解_木
		Out_Prize[1][12753]=63  ---破魔怒意_木
		Out_Prize[1][12758]=63  ---怒剑心劫_木
		Out_Prize[1][12763]=37  ---邪剑星羽_木
		Out_Prize[1][12768]=47  ---天诛真诀_木
		Out_Prize[1][12773]=32  ---斩龙奥义_木
		Out_Prize[1][12778]=27  ---玄天星芒_木
		Out_Prize[1][12783]=170  ---剑元之聚_木
		Out_Prize[1][12789]=29  ---气贯长虹_木
		Out_Prize[1][12794]=30  ---玄冰圣护_木
		Out_Prize[1][12799]=29  ---玄冰劲气_木
		Out_Prize[1][12804]=23  ---返朴归真_木
		Out_Prize[1][12809]=23  ---狂雷天怒_木
		Out_Prize[1][12814]=38  ---冰域天机_木
		Out_Prize[1][12819]=36  ---盛冰之寒_木
		Out_Prize[1][12824]=72  ---霜寒化境_木
		Out_Prize[1][12829]=63  ---雷云奇变_木
		Out_Prize[1][12834]=63  ---五雷交轰_木
		Out_Prize[1][12839]=25  ---天雷狂啸_木
		Out_Prize[1][12844]=29  ---群雷裂空_木
		Out_Prize[1][12849]=34  ---天地无用_木

	elseif id==13322 then 
		Out_Prize[1][12688]=200  ---寒冰烈炎_水
		Out_Prize[1][12693]=47  ---少阳初现_水
		Out_Prize[1][12698]=63  ---破魔秘法_水
		Out_Prize[1][12703]=27  ---天眷有道_水
		Out_Prize[1][12708]=26  ---驱物法诀_水
		Out_Prize[1][12713]=31  ---护体罡气_水
		Out_Prize[1][12719]=53  ---炼神还虚_水
		Out_Prize[1][12724]=42  ---冰心玉阙_水
		Out_Prize[1][12729]=28  ---仙元护体_水
		Out_Prize[1][12734]=31  ---先天罡气_水
		Out_Prize[1][12739]=47  ---雷神降世_水
		Out_Prize[1][12744]=200  ---风剑之芒_水
		Out_Prize[1][12749]=170  ---归元真解_水
		Out_Prize[1][12754]=63  ---破魔怒意_水
		Out_Prize[1][12759]=63  ---怒剑心劫_水
		Out_Prize[1][12764]=37  ---邪剑星羽_水
		Out_Prize[1][12769]=47  ---天诛真诀_水
		Out_Prize[1][12774]=32  ---斩龙奥义_水
		Out_Prize[1][12779]=27  ---玄天星芒_水
		Out_Prize[1][12784]=170  ---剑元之聚_水
		Out_Prize[1][12790]=29  ---气贯长虹_水
		Out_Prize[1][12795]=30  ---玄冰圣护_水
		Out_Prize[1][12800]=29  ---玄冰劲气_水
		Out_Prize[1][12805]=23  ---返朴归真_水
		Out_Prize[1][12810]=23  ---狂雷天怒_水
		Out_Prize[1][12815]=38  ---冰域天机_水
		Out_Prize[1][12820]=36  ---盛冰之寒_水
		Out_Prize[1][12825]=72  ---霜寒化境_水
		Out_Prize[1][12830]=63  ---雷云奇变_水
		Out_Prize[1][12835]=63  ---五雷交轰_水
		Out_Prize[1][12840]=25  ---天雷狂啸_水
		Out_Prize[1][12845]=29  ---群雷裂空_水
		Out_Prize[1][12850]=34  ---天地无用_水

	elseif id==13323 then 
		Out_Prize[1][12689]=200  ---寒冰烈炎_火
		Out_Prize[1][12694]=47  ---少阳初现_火
		Out_Prize[1][12699]=63  ---破魔秘法_火
		Out_Prize[1][12704]=27  ---天眷有道_火
		Out_Prize[1][12709]=26  ---驱物法诀_火
		Out_Prize[1][12714]=31  ---护体罡气_火
		Out_Prize[1][12720]=53  ---炼神还虚_火
		Out_Prize[1][12725]=42  ---冰心玉阙_火
		Out_Prize[1][12730]=28  ---仙元护体_火
		Out_Prize[1][12735]=31  ---先天罡气_火
		Out_Prize[1][12740]=47  ---雷神降世_火
		Out_Prize[1][12745]=200  ---风剑之芒_火
		Out_Prize[1][12750]=170  ---归元真解_火
		Out_Prize[1][12755]=63  ---破魔怒意_火
		Out_Prize[1][12760]=63  ---怒剑心劫_火
		Out_Prize[1][12765]=37  ---邪剑星羽_火
		Out_Prize[1][12770]=47  ---天诛真诀_火
		Out_Prize[1][12775]=32  ---斩龙奥义_火
		Out_Prize[1][12780]=27  ---玄天星芒_火
		Out_Prize[1][12785]=170  ---剑元之聚_火
		Out_Prize[1][12791]=29  ---气贯长虹_火
		Out_Prize[1][12796]=30  ---玄冰圣护_火
		Out_Prize[1][12801]=29  ---玄冰劲气_火
		Out_Prize[1][12806]=23  ---返朴归真_火
		Out_Prize[1][12811]=23  ---狂雷天怒_火
		Out_Prize[1][12816]=38  ---冰域天机_火
		Out_Prize[1][12821]=36  ---盛冰之寒_火
		Out_Prize[1][12826]=72  ---霜寒化境_火
		Out_Prize[1][12831]=63  ---雷云奇变_火
		Out_Prize[1][12836]=63  ---五雷交轰_火
		Out_Prize[1][12841]=25  ---天雷狂啸_火
		Out_Prize[1][12846]=29  ---群雷裂空_火
		Out_Prize[1][12851]=34  ---天地无用_火

	elseif id==13324 then 
		Out_Prize[1][12690]=200  ---寒冰烈炎_土
		Out_Prize[1][12695]=47  ---少阳初现_土
		Out_Prize[1][12700]=63  ---破魔秘法_土
		Out_Prize[1][12705]=27  ---天眷有道_土
		Out_Prize[1][12710]=26  ---驱物法诀_土
		Out_Prize[1][12715]=31  ---护体罡气_土
		Out_Prize[1][12721]=53  ---炼神还虚_土
		Out_Prize[1][12726]=42  ---冰心玉阙_土
		Out_Prize[1][12731]=28  ---仙元护体_土
		Out_Prize[1][12736]=31  ---先天罡气_土
		Out_Prize[1][12741]=47  ---雷神降世_土
		Out_Prize[1][12746]=200  ---风剑之芒_土
		Out_Prize[1][12751]=170  ---归元真解_土
		Out_Prize[1][12756]=63  ---破魔怒意_土
		Out_Prize[1][12761]=63  ---怒剑心劫_土
		Out_Prize[1][12766]=37  ---邪剑星羽_土
		Out_Prize[1][12771]=47  ---天诛真诀_土
		Out_Prize[1][12776]=32  ---斩龙奥义_土
		Out_Prize[1][12781]=27  ---玄天星芒_土
		Out_Prize[1][12786]=170  ---剑元之聚_土
		Out_Prize[1][12792]=29  ---气贯长虹_土
		Out_Prize[1][12797]=30  ---玄冰圣护_土
		Out_Prize[1][12802]=29  ---玄冰劲气_土
		Out_Prize[1][12807]=23  ---返朴归真_土
		Out_Prize[1][12812]=23  ---狂雷天怒_土
		Out_Prize[1][12817]=38  ---冰域天机_土
		Out_Prize[1][12822]=36  ---盛冰之寒_土
		Out_Prize[1][12827]=72  ---霜寒化境_土
		Out_Prize[1][12832]=63  ---雷云奇变_土
		Out_Prize[1][12837]=63  ---五雷交轰_土
		Out_Prize[1][12842]=25  ---天雷狂啸_土
		Out_Prize[1][12847]=29  ---群雷裂空_土
		Out_Prize[1][12852]=34  ---天地无用_土

	elseif id==13325 then 
		Out_Prize[1][12072]=200  ---狂魔_金
		Out_Prize[1][12073]=30  ---火舞_金
		Out_Prize[1][12074]=30  ---戾气_金
		Out_Prize[1][12075]=47  ---震慑_金
		Out_Prize[1][12076]=46  ---破魂_金
		Out_Prize[1][12077]=31  ---无间_金
		Out_Prize[1][12078]=38  ---雷鸣_金
		Out_Prize[1][12079]=40  ---狂怒_金
		Out_Prize[1][12080]=26  ---灭神_金
		Out_Prize[1][12081]=30  ---天谴_金
		Out_Prize[1][12082]=47  ---亢龙_金
		Out_Prize[1][12083]=33  ---炽热_金
		Out_Prize[1][12084]=42  ---业火_金
		Out_Prize[1][12085]=30  ---信念_金
		Out_Prize[1][12086]=33  ---战意_金
		Out_Prize[1][12087]=30  ---天煞_金
		Out_Prize[1][12088]=40  ---魔心_金
		Out_Prize[1][12089]=27  ---咒怨_金
		Out_Prize[1][12090]=30  ---天佑_金
		Out_Prize[1][12091]=26  ---血咒_金
		Out_Prize[1][12092]=26  ---驱物_金
		Out_Prize[1][12093]=26  ---妄念_金
		Out_Prize[1][12094]=53  ---冰寒_金
		Out_Prize[1][12095]=38  ---噬血_金
		Out_Prize[1][12096]=29  ---梦魇_金
		Out_Prize[1][12097]=31  ---孤注_金
		Out_Prize[1][12098]=27  ---末日_金
		Out_Prize[1][12099]=53  ---冰锋_金
		Out_Prize[1][12100]=92  ---沉舟_金
		Out_Prize[1][12101]=47  ---魔煞_金
		Out_Prize[1][12102]=30  ---破空_金
		Out_Prize[1][12103]=35  ---虚无_金
		Out_Prize[1][12104]=26  ---怒龙_金

	elseif id==13326 then 
		Out_Prize[1][12128]=200  ---狂魔_木
		Out_Prize[1][12130]=30  ---火舞_木
		Out_Prize[1][12131]=30  ---戾气_木
		Out_Prize[1][12133]=47  ---震慑_木
		Out_Prize[1][12135]=46  ---破魂_木
		Out_Prize[1][12136]=31  ---无间_木
		Out_Prize[1][12139]=38  ---雷鸣_木
		Out_Prize[1][12141]=40  ---狂怒_木
		Out_Prize[1][12144]=26  ---灭神_木
		Out_Prize[1][12147]=30  ---天谴_木
		Out_Prize[1][12148]=47  ---亢龙_木
		Out_Prize[1][12149]=33  ---炽热_木
		Out_Prize[1][12151]=42  ---业火_木
		Out_Prize[1][12153]=30  ---信念_木
		Out_Prize[1][12155]=33  ---战意_木
		Out_Prize[1][12157]=30  ---天煞_木
		Out_Prize[1][12159]=40  ---魔心_木
		Out_Prize[1][12161]=27  ---咒怨_木
		Out_Prize[1][12164]=30  ---天佑_木
		Out_Prize[1][12166]=26  ---血咒_木
		Out_Prize[1][12167]=26  ---驱物_木
		Out_Prize[1][12169]=26  ---妄念_木
		Out_Prize[1][12171]=53  ---冰寒_木
		Out_Prize[1][12173]=38  ---噬血_木
		Out_Prize[1][12175]=29  ---梦魇_木
		Out_Prize[1][12177]=31  ---孤注_木
		Out_Prize[1][12179]=27  ---末日_木
		Out_Prize[1][12181]=53  ---冰锋_木
		Out_Prize[1][12182]=92  ---沉舟_木
		Out_Prize[1][12184]=47  ---魔煞_木
		Out_Prize[1][12185]=30  ---破空_木
		Out_Prize[1][12187]=35  ---虚无_木
		Out_Prize[1][12188]=26  ---怒龙_木

	elseif id==13327 then 
		Out_Prize[1][12228]=200  ---狂魔_水
		Out_Prize[1][12230]=30  ---火舞_水
		Out_Prize[1][12232]=30  ---戾气_水
		Out_Prize[1][12234]=47  ---震慑_水
		Out_Prize[1][12236]=46  ---破魂_水
		Out_Prize[1][12238]=31  ---无间_水
		Out_Prize[1][12242]=38  ---雷鸣_水
		Out_Prize[1][12243]=40  ---狂怒_水
		Out_Prize[1][12244]=26  ---灭神_水
		Out_Prize[1][12245]=30  ---天谴_水
		Out_Prize[1][12247]=47  ---亢龙_水
		Out_Prize[1][12248]=33  ---炽热_水
		Out_Prize[1][12250]=42  ---业火_水
		Out_Prize[1][12252]=30  ---信念_水
		Out_Prize[1][12254]=33  ---战意_水
		Out_Prize[1][12257]=30  ---天煞_水
		Out_Prize[1][12259]=40  ---魔心_水
		Out_Prize[1][12260]=27  ---咒怨_水
		Out_Prize[1][12261]=30  ---天佑_水
		Out_Prize[1][12262]=26  ---血咒_水
		Out_Prize[1][12263]=26  ---驱物_水
		Out_Prize[1][12264]=26  ---妄念_水
		Out_Prize[1][12265]=53  ---冰寒_水
		Out_Prize[1][12266]=38  ---噬血_水
		Out_Prize[1][12267]=29  ---梦魇_水
		Out_Prize[1][12268]=31  ---孤注_水
		Out_Prize[1][12269]=27  ---末日_水
		Out_Prize[1][12270]=53  ---冰锋_水
		Out_Prize[1][12271]=92  ---沉舟_水
		Out_Prize[1][12272]=47  ---魔煞_水
		Out_Prize[1][12273]=30  ---破空_水
		Out_Prize[1][12274]=35  ---虚无_水
		Out_Prize[1][12275]=26  ---怒龙_水


	elseif id==13328 then 
		Out_Prize[1][12284]=200  ---狂魔_火
		Out_Prize[1][12285]=30  ---火舞_火
		Out_Prize[1][12286]=30  ---戾气_火
		Out_Prize[1][12287]=47  ---震慑_火
		Out_Prize[1][12288]=46  ---破魂_火
		Out_Prize[1][12289]=31  ---无间_火
		Out_Prize[1][12290]=38  ---雷鸣_火
		Out_Prize[1][12291]=40  ---狂怒_火
		Out_Prize[1][12292]=26  ---灭神_火
		Out_Prize[1][12293]=30  ---天谴_火
		Out_Prize[1][12294]=47  ---亢龙_火
		Out_Prize[1][12295]=33  ---炽热_火
		Out_Prize[1][12296]=42  ---业火_火
		Out_Prize[1][12298]=30  ---信念_火
		Out_Prize[1][12300]=33  ---战意_火
		Out_Prize[1][12302]=30  ---天煞_火
		Out_Prize[1][12304]=40  ---魔心_火
		Out_Prize[1][12306]=27  ---咒怨_火
		Out_Prize[1][12308]=30  ---天佑_火
		Out_Prize[1][12310]=26  ---血咒_火
		Out_Prize[1][12312]=26  ---驱物_火
		Out_Prize[1][12314]=26  ---妄念_火
		Out_Prize[1][12316]=53  ---冰寒_火
		Out_Prize[1][12318]=38  ---噬血_火
		Out_Prize[1][12322]=29  ---梦魇_火
		Out_Prize[1][12323]=31  ---孤注_火
		Out_Prize[1][12325]=27  ---末日_火
		Out_Prize[1][12327]=53  ---冰锋_火
		Out_Prize[1][12329]=92  ---沉舟_火
		Out_Prize[1][12330]=47  ---魔煞_火
		Out_Prize[1][12333]=30  ---破空_火
		Out_Prize[1][12335]=35  ---虚无_火
		Out_Prize[1][12337]=26  ---怒龙_火


	elseif id==13329 then 
		Out_Prize[1][12346]=200  ---狂魔_土
		Out_Prize[1][12347]=30  ---火舞_土
		Out_Prize[1][12348]=30  ---戾气_土
		Out_Prize[1][12349]=47  ---震慑_土
		Out_Prize[1][12350]=46  ---破魂_土
		Out_Prize[1][12351]=31  ---无间_土
		Out_Prize[1][12352]=38  ---雷鸣_土
		Out_Prize[1][12353]=40  ---狂怒_土
		Out_Prize[1][12354]=26  ---灭神_土
		Out_Prize[1][12355]=30  ---天谴_土
		Out_Prize[1][12356]=47  ---亢龙_土
		Out_Prize[1][12357]=33  ---炽热_土
		Out_Prize[1][12358]=42  ---业火_土
		Out_Prize[1][12359]=30  ---信念_土
		Out_Prize[1][12360]=33  ---战意_土
		Out_Prize[1][12361]=30  ---天煞_土
		Out_Prize[1][12362]=40  ---魔心_土
		Out_Prize[1][12363]=27  ---咒怨_土
		Out_Prize[1][12364]=30  ---天佑_土
		Out_Prize[1][12365]=26  ---血咒_土
		Out_Prize[1][12366]=26  ---驱物_土
		Out_Prize[1][12367]=26  ---妄念_土
		Out_Prize[1][12368]=53  ---冰寒_土
		Out_Prize[1][12369]=38  ---噬血_土
		Out_Prize[1][12370]=29  ---梦魇_土
		Out_Prize[1][12371]=31  ---孤注_土
		Out_Prize[1][12372]=27  ---末日_土
		Out_Prize[1][12373]=53  ---冰锋_土
		Out_Prize[1][12374]=92  ---沉舟_土
		Out_Prize[1][12375]=47  ---魔煞_土
		Out_Prize[1][12376]=30  ---破空_土
		Out_Prize[1][12377]=35  ---虚无_土
		Out_Prize[1][12378]=26  ---怒龙_土


	elseif id==13330 then 
		Out_Prize[1][12107]=92  ---天花乱坠_金
		Out_Prize[1][12109]=29  ---空即是色_金
		Out_Prize[1][12111]=31  ---袈裟蔽天_金
		Out_Prize[1][12115]=200  ---降魔真言_金
		Out_Prize[1][12117]=92  ---束心法咒_金
		Out_Prize[1][12118]=170  ---梵经心语_金
		Out_Prize[1][12119]=28  ---固本培元_金
		Out_Prize[1][12120]=47  ---再造祷言_金
		Out_Prize[1][12121]=43  ---悬壶济世_金
		Out_Prize[1][12122]=35  ---法螺常念_金
		Out_Prize[1][12123]=27  ---光明之颂_金
		Out_Prize[1][12124]=35  ---一语道破_金
		Out_Prize[1][12125]=34  ---度尽劫波_金
		Out_Prize[1][12126]=29  ---色即是空_金
		Out_Prize[1][12127]=29  ---空冥禅念_金
		Out_Prize[1][12129]=28  ---顿悟之偈_金
		Out_Prize[1][12132]=26  ---天道循环_金
		Out_Prize[1][12134]=26  ---驭物神通_金
		Out_Prize[1][12137]=92  ---往生夙愿_金
		Out_Prize[1][12138]=28  ---了世尘缘_金
		Out_Prize[1][12140]=53  ---极乐颂泽_金
		Out_Prize[1][12142]=34  ---灵魂绽放_金
		Out_Prize[1][12143]=27  ---无尽苦海_金
		Out_Prize[1][12145]=35  ---相续不空_金
		Out_Prize[1][12146]=35  ---乾坤正气_金
		Out_Prize[1][12150]=29  ---净世佛心_金
		Out_Prize[1][12152]=29  ---金刚破魔_金
		Out_Prize[1][12154]=31  ---伏魔真诀_金
		Out_Prize[1][12156]=32  ---极乐禅言_金
		Out_Prize[1][12158]=30  ---无常轮渡_金
		Out_Prize[1][12160]=55  ---九转之易_金
		Out_Prize[1][12162]=34  ---慈悲为怀_金
		Out_Prize[1][12163]=30  ---神魔妄念_金

	elseif id==13331 then 
		Out_Prize[1][12183]=92  ---天花乱坠_木
		Out_Prize[1][12186]=29  ---空即是色_木
		Out_Prize[1][12189]=31  ---袈裟蔽天_木
		Out_Prize[1][12192]=200  ---降魔真言_木
		Out_Prize[1][12194]=92  ---束心法咒_木
		Out_Prize[1][12197]=170  ---梵经心语_木
		Out_Prize[1][12199]=28  ---固本培元_木
		Out_Prize[1][12202]=47  ---再造祷言_木
		Out_Prize[1][12203]=43  ---悬壶济世_木
		Out_Prize[1][12204]=35  ---法螺常念_木
		Out_Prize[1][12205]=27  ---光明之颂_木
		Out_Prize[1][12206]=35  ---一语道破_木
		Out_Prize[1][12207]=34  ---度尽劫波_木
		Out_Prize[1][12208]=29  ---色即是空_木
		Out_Prize[1][12209]=29  ---空冥禅念_木
		Out_Prize[1][12210]=28  ---顿悟之偈_木
		Out_Prize[1][12211]=26  ---天道循环_木
		Out_Prize[1][12212]=26  ---驭物神通_木
		Out_Prize[1][12213]=92  ---往生夙愿_木
		Out_Prize[1][12214]=28  ---了世尘缘_木
		Out_Prize[1][12215]=53  ---极乐颂泽_木
		Out_Prize[1][12216]=34  ---灵魂绽放_木
		Out_Prize[1][12217]=27  ---无尽苦海_木
		Out_Prize[1][12218]=35  ---相续不空_木
		Out_Prize[1][12219]=35  ---乾坤正气_木
		Out_Prize[1][12220]=29  ---净世佛心_木
		Out_Prize[1][12221]=29  ---金刚破魔_木
		Out_Prize[1][12222]=31  ---伏魔真诀_木
		Out_Prize[1][12223]=32  ---极乐禅言_木
		Out_Prize[1][12224]=30  ---无常轮渡_木
		Out_Prize[1][12225]=55  ---九转之易_木
		Out_Prize[1][12226]=34  ---慈悲为怀_木
		Out_Prize[1][12227]=30  ---神魔妄念_木

	elseif id==13332 then
		Out_Prize[1][12246]=92  ---天花乱坠_水
		Out_Prize[1][12249]=29  ---空即是色_水
		Out_Prize[1][12251]=31  ---袈裟蔽天_水
		Out_Prize[1][12253]=200  ---降魔真言_水
		Out_Prize[1][12255]=92  ---束心法咒_水
		Out_Prize[1][12256]=170  ---梵经心语_水
		Out_Prize[1][12258]=28  ---固本培元_水
		Out_Prize[1][12297]=47  ---再造祷言_水
		Out_Prize[1][12299]=43  ---悬壶济世_水
		Out_Prize[1][12301]=35  ---法螺常念_水
		Out_Prize[1][12303]=27  ---光明之颂_水
		Out_Prize[1][12305]=35  ---一语道破_水
		Out_Prize[1][12307]=34  ---度尽劫波_水
		Out_Prize[1][12309]=29  ---色即是空_水
		Out_Prize[1][12311]=29  ---空冥禅念_水
		Out_Prize[1][12313]=28  ---顿悟之偈_水
		Out_Prize[1][12315]=26  ---天道循环_水
		Out_Prize[1][12317]=26  ---驭物神通_水
		Out_Prize[1][12319]=92  ---往生夙愿_水
		Out_Prize[1][12320]=28  ---了世尘缘_水
		Out_Prize[1][12321]=53  ---极乐颂泽_水
		Out_Prize[1][12324]=34  ---灵魂绽放_水
		Out_Prize[1][12326]=27  ---无尽苦海_水
		Out_Prize[1][12328]=35  ---相续不空_水
		Out_Prize[1][12331]=35  ---乾坤正气_水
		Out_Prize[1][12332]=29  ---净世佛心_水
		Out_Prize[1][12334]=29  ---金刚破魔_水
		Out_Prize[1][12336]=31  ---伏魔真诀_水
		Out_Prize[1][12387]=32  ---极乐禅言_水
		Out_Prize[1][12388]=30  ---无常轮渡_水
		Out_Prize[1][12389]=55  ---九转之易_水
		Out_Prize[1][12390]=34  ---慈悲为怀_水
		Out_Prize[1][12391]=30  ---神魔妄念_水

 
	elseif id==13333 then 
		Out_Prize[1][12400]=92  ---天花乱坠_火
		Out_Prize[1][12401]=29  ---空即是色_火
		Out_Prize[1][12402]=31  ---袈裟蔽天_火
		Out_Prize[1][12403]=200  ---降魔真言_火
		Out_Prize[1][12404]=92  ---束心法咒_火
		Out_Prize[1][12405]=170  ---梵经心语_火
		Out_Prize[1][12406]=28  ---固本培元_火
		Out_Prize[1][12407]=47  ---再造祷言_火
		Out_Prize[1][12408]=43  ---悬壶济世_火
		Out_Prize[1][12409]=35  ---法螺常念_火
		Out_Prize[1][12410]=27  ---光明之颂_火
		Out_Prize[1][12411]=35  ---一语道破_火
		Out_Prize[1][12412]=34  ---度尽劫波_火
		Out_Prize[1][12413]=29  ---色即是空_火
		Out_Prize[1][12414]=29  ---空冥禅念_火
		Out_Prize[1][12415]=28  ---顿悟之偈_火
		Out_Prize[1][12416]=26  ---天道循环_火
		Out_Prize[1][12417]=26  ---驭物神通_火
		Out_Prize[1][12418]=92  ---往生夙愿_火
		Out_Prize[1][12419]=28  ---了世尘缘_火
		Out_Prize[1][12420]=53  ---极乐颂泽_火
		Out_Prize[1][12422]=34  ---灵魂绽放_火
		Out_Prize[1][12450]=27  ---无尽苦海_火
		Out_Prize[1][12453]=35  ---相续不空_火
		Out_Prize[1][12455]=35  ---乾坤正气_火
		Out_Prize[1][12457]=29  ---净世佛心_火
		Out_Prize[1][12460]=29  ---金刚破魔_火
		Out_Prize[1][12463]=31  ---伏魔真诀_火
		Out_Prize[1][12465]=32  ---极乐禅言_火
		Out_Prize[1][12466]=30  ---无常轮渡_火
		Out_Prize[1][12468]=55  ---九转之易_火
		Out_Prize[1][12470]=34  ---慈悲为怀_火
		Out_Prize[1][12473]=30  ---神魔妄念_火


	elseif id==13334 then 
		Out_Prize[1][12503]=92  ---天花乱坠_土
		Out_Prize[1][13235]=29  ---空即是色_土
		Out_Prize[1][12505]=31  ---袈裟蔽天_土
		Out_Prize[1][12507]=200  ---降魔真言_土
		Out_Prize[1][12509]=92  ---束心法咒_土
		Out_Prize[1][12512]=170  ---梵经心语_土
		Out_Prize[1][12514]=28  ---固本培元_土
		Out_Prize[1][12516]=47  ---再造祷言_土
		Out_Prize[1][12518]=43  ---悬壶济世_土
		Out_Prize[1][12519]=35  ---法螺常念_土
		Out_Prize[1][12520]=27  ---光明之颂_土
		Out_Prize[1][12521]=35  ---一语道破_土
		Out_Prize[1][12522]=34  ---度尽劫波_土
		Out_Prize[1][12523]=29  ---色即是空_土
		Out_Prize[1][12524]=29  ---空冥禅念_土
		Out_Prize[1][12525]=28  ---顿悟之偈_土
		Out_Prize[1][12526]=26  ---天道循环_土
		Out_Prize[1][12527]=26  ---驭物神通_土
		Out_Prize[1][12528]=92  ---往生夙愿_土
		Out_Prize[1][12529]=28  ---了世尘缘_土
		Out_Prize[1][12530]=53  ---极乐颂泽_土
		Out_Prize[1][12531]=34  ---灵魂绽放_土
		Out_Prize[1][12532]=27  ---无尽苦海_土
		Out_Prize[1][12533]=35  ---相续不空_土
		Out_Prize[1][12534]=35  ---乾坤正气_土
		Out_Prize[1][12535]=29  ---净世佛心_土
		Out_Prize[1][12536]=29  ---金刚破魔_土
		Out_Prize[1][12537]=31  ---伏魔真诀_土
		Out_Prize[1][12538]=32  ---极乐禅言_土
		Out_Prize[1][12539]=30  ---无常轮渡_土
		Out_Prize[1][12540]=55  ---九转之易_土
		Out_Prize[1][12541]=34  ---慈悲为怀_土
		Out_Prize[1][12542]=30  ---神魔妄念_土


	elseif id==13335 then
		Out_Prize[1][12421]=200  ---相济_金
		Out_Prize[1][12423]=63  ---识破_金
		Out_Prize[1][12424]=35  ---魅心_金
		Out_Prize[1][12437]=27  ---荡漾_金
		Out_Prize[1][12442]=92  ---击天_金
		Out_Prize[1][12447]=26  ---情动_金
		Out_Prize[1][12454]=34  ---幻境_金
		Out_Prize[1][12462]=29  ---梦乡_金
		Out_Prize[1][12472]=29  ---风月_金
		Out_Prize[1][12478]=37  ---不逾_金
		Out_Prize[1][12485]=28  ---绝尘_金
		Out_Prize[1][12493]=34  ---残香_金
		Out_Prize[1][12500]=42  ---翩飞_金
		Out_Prize[1][12510]=38  ---约定_金
		Out_Prize[1][12551]=38  ---星月_金
		Out_Prize[1][12556]=92  ---幽兰_金
		Out_Prize[1][12561]=34  ---迷梦_金
		Out_Prize[1][12566]=34  ---心愿_金
		Out_Prize[1][12571]=22  ---情惑_金
		Out_Prize[1][12576]=72  ---心门_金
		Out_Prize[1][12581]=42  ---亦尘_金
		Out_Prize[1][12586]=34  ---思恋_金
		Out_Prize[1][12591]=31  ---迷失_金
		Out_Prize[1][12596]=47  ---花间_金
		Out_Prize[1][12601]=28  ---惊鸿_金
		Out_Prize[1][12606]=28  ---风物_金
		Out_Prize[1][12611]=47  ---悱恻_金
		Out_Prize[1][12616]=42  ---化羽_金
		Out_Prize[1][12621]=34  ---天伤_金
		Out_Prize[1][12626]=26  ---黯然_金
		Out_Prize[1][12631]=200  ---驭宝_金
		Out_Prize[1][12632]=27  ---绝情_金
		Out_Prize[1][12641]=37  ---天籁_金

 	elseif id==13336 then 
 		Out_Prize[1][12425]=200  ---相济_木
		Out_Prize[1][12429]=63  ---识破_木
		Out_Prize[1][12433]=35  ---魅心_木
		Out_Prize[1][12438]=27  ---荡漾_木
		Out_Prize[1][12443]=92  ---击天_木
		Out_Prize[1][12448]=26  ---情动_木
		Out_Prize[1][12456]=34  ---幻境_木
		Out_Prize[1][12464]=29  ---梦乡_木
		Out_Prize[1][12474]=29  ---风月_木
		Out_Prize[1][12479]=37  ---不逾_木
		Out_Prize[1][12488]=28  ---绝尘_木
		Out_Prize[1][12494]=34  ---残香_木
		Out_Prize[1][12502]=42  ---翩飞_木
		Out_Prize[1][12511]=38  ---约定_木
		Out_Prize[1][12552]=38  ---星月_木
		Out_Prize[1][12557]=92  ---幽兰_木
		Out_Prize[1][12562]=34  ---迷梦_木
		Out_Prize[1][12567]=34  ---心愿_木
		Out_Prize[1][12572]=22  ---情惑_木
		Out_Prize[1][12577]=72  ---心门_木
		Out_Prize[1][12582]=42  ---亦尘_木
		Out_Prize[1][12587]=34  ---思恋_木
		Out_Prize[1][12592]=31  ---迷失_木
		Out_Prize[1][12597]=47  ---花间_木
		Out_Prize[1][12602]=28  ---惊鸿_木
		Out_Prize[1][12607]=28  ---风物_木
		Out_Prize[1][12612]=47  ---悱恻_木
		Out_Prize[1][12617]=42  ---化羽_木
		Out_Prize[1][12622]=34  ---天伤_木
		Out_Prize[1][12627]=26  ---黯然_木
		Out_Prize[1][12633]=200  ---驭宝_木
		Out_Prize[1][12637]=27  ---绝情_木
		Out_Prize[1][12642]=37  ---天籁_木

	elseif id==13337 then
		Out_Prize[1][12426]=200  ---相济_水
		Out_Prize[1][12430]=63  ---识破_水
		Out_Prize[1][12434]=35  ---魅心_水
		Out_Prize[1][12439]=27  ---荡漾_水
		Out_Prize[1][12444]=92  ---击天_水
		Out_Prize[1][12449]=26  ---情动_水
		Out_Prize[1][12458]=34  ---幻境_水
		Out_Prize[1][12471]=29  ---梦乡_水
		Out_Prize[1][12475]=29  ---风月_水
		Out_Prize[1][12480]=37  ---不逾_水
		Out_Prize[1][12490]=28  ---绝尘_水
		Out_Prize[1][12495]=34  ---残香_水
		Out_Prize[1][12504]=42  ---翩飞_水
		Out_Prize[1][12513]=38  ---约定_水
		Out_Prize[1][12553]=38  ---星月_水
		Out_Prize[1][12558]=92  ---幽兰_水
		Out_Prize[1][12563]=34  ---迷梦_水
		Out_Prize[1][12568]=34  ---心愿_水
		Out_Prize[1][12573]=22  ---情惑_水
		Out_Prize[1][12578]=72  ---心门_水
		Out_Prize[1][12583]=42  ---亦尘_水
		Out_Prize[1][12588]=34  ---思恋_水
		Out_Prize[1][12593]=31  ---迷失_水
		Out_Prize[1][12598]=47  ---花间_水
		Out_Prize[1][12603]=28  ---惊鸿_水
		Out_Prize[1][12608]=28  ---风物_水
		Out_Prize[1][12613]=47  ---悱恻_水
		Out_Prize[1][12618]=42  ---化羽_水
		Out_Prize[1][12623]=34  ---天伤_水
		Out_Prize[1][12628]=26  ---黯然_水
		Out_Prize[1][12634]=200  ---驭宝_水
		Out_Prize[1][12638]=27  ---绝情_水
		Out_Prize[1][12643]=37  ---天籁_水

 	elseif id==13338 then 
 		Out_Prize[1][12427]=200  ---相济_火
		Out_Prize[1][12431]=63  ---识破_火
		Out_Prize[1][12435]=35  ---魅心_火
		Out_Prize[1][12440]=27  ---荡漾_火
		Out_Prize[1][12445]=92  ---击天_火
		Out_Prize[1][12451]=26  ---情动_火
		Out_Prize[1][12459]=34  ---幻境_火
		Out_Prize[1][12467]=29  ---梦乡_火
		Out_Prize[1][12476]=29  ---风月_火
		Out_Prize[1][12481]=37  ---不逾_火
		Out_Prize[1][12491]=28  ---绝尘_火
		Out_Prize[1][12496]=34  ---残香_火
		Out_Prize[1][12506]=42  ---翩飞_火
		Out_Prize[1][12515]=38  ---约定_火
		Out_Prize[1][12554]=38  ---星月_火
		Out_Prize[1][12559]=92  ---幽兰_火
		Out_Prize[1][12564]=34  ---迷梦_火
		Out_Prize[1][12569]=34  ---心愿_火
		Out_Prize[1][12574]=22  ---情惑_火
		Out_Prize[1][12579]=72  ---心门_火
		Out_Prize[1][12584]=42  ---亦尘_火
		Out_Prize[1][12589]=34  ---思恋_火
		Out_Prize[1][12594]=31  ---迷失_火
		Out_Prize[1][12599]=47  ---花间_火
		Out_Prize[1][12604]=28  ---惊鸿_火
		Out_Prize[1][12609]=28  ---风物_火
		Out_Prize[1][12614]=47  ---悱恻_火
		Out_Prize[1][12619]=42  ---化羽_火
		Out_Prize[1][12624]=34  ---天伤_火
		Out_Prize[1][12629]=26  ---黯然_火
		Out_Prize[1][12635]=200  ---驭宝_火
		Out_Prize[1][12639]=27  ---绝情_火
		Out_Prize[1][12644]=37  ---天籁_火
	
	elseif id==13339 then 
		Out_Prize[1][12428]=200  ---相济_土
		Out_Prize[1][12432]=63  ---识破_土
		Out_Prize[1][12436]=35  ---魅心_土
		Out_Prize[1][12441]=27  ---荡漾_土
		Out_Prize[1][12446]=92  ---击天_土
		Out_Prize[1][12452]=26  ---情动_土
		Out_Prize[1][12461]=34  ---幻境_土
		Out_Prize[1][12469]=29  ---梦乡_土
		Out_Prize[1][12477]=29  ---风月_土
		Out_Prize[1][12484]=37  ---不逾_土
		Out_Prize[1][12492]=28  ---绝尘_土
		Out_Prize[1][12497]=34  ---残香_土
		Out_Prize[1][12508]=42  ---翩飞_土
		Out_Prize[1][12517]=38  ---约定_土
		Out_Prize[1][12555]=38  ---星月_土
		Out_Prize[1][12560]=92  ---幽兰_土
		Out_Prize[1][12565]=34  ---迷梦_土
		Out_Prize[1][12570]=34  ---心愿_土
		Out_Prize[1][12575]=22  ---情惑_土
		Out_Prize[1][12580]=72  ---心门_土
		Out_Prize[1][12585]=42  ---亦尘_土
		Out_Prize[1][12590]=34  ---思恋_土
		Out_Prize[1][12595]=31  ---迷失_土
		Out_Prize[1][12600]=47  ---花间_土
		Out_Prize[1][12605]=28  ---惊鸿_土
		Out_Prize[1][12610]=28  ---风物_土
		Out_Prize[1][12615]=47  ---悱恻_土
		Out_Prize[1][12620]=42  ---化羽_土
		Out_Prize[1][12625]=34  ---天伤_土
		Out_Prize[1][12630]=26  ---黯然_土
		Out_Prize[1][12636]=200  ---驭宝_土
		Out_Prize[1][12640]=27  ---绝情_土
		Out_Prize[1][12645]=37  ---天籁_土
	
	elseif id==18752 then
		Out_Prize[1][18460]=120---恶鬼牙_金
		Out_Prize[1][18465]=60---沥血心_金
		Out_Prize[1][18470]=120---化骨毒_金
		Out_Prize[1][18475]=120---攻心咒_金
		Out_Prize[1][18480]=120---乱心诀_金
		Out_Prize[1][18485]=120---无影毒_金
		Out_Prize[1][18490]=120---黯然咒_金
		Out_Prize[1][18495]=120---幕天毒_金
		Out_Prize[1][18500]=120---地灵咒_金
		Out_Prize[1][18510]=60---孽龙毒_金
		Out_Prize[1][18520]=60---定魂诀_金
		Out_Prize[1][18535]=120---百毒炼_金
		Out_Prize[1][18540]=120---怒灵噬_金
		Out_Prize[1][18545]=120---天劫火_金
		Out_Prize[1][18550]=120---鬼阴风_金
		Out_Prize[1][18555]=60---挫骨印_金
		Out_Prize[1][18560]=60---真火咒_金
		Out_Prize[1][18565]=120---回魂祭_金
		Out_Prize[1][18570]=120---猛鬼啸_金
		Out_Prize[1][18575]=60---罗生门_金
		Out_Prize[1][18580]=15---邪血神_金
		Out_Prize[1][18585]=120---百鬼抄_金
		Out_Prize[1][18595]=120---截宗秘_金
		Out_Prize[1][18600]=120---邪灵蛊_金
		Out_Prize[1][18605]=120---应声蛊_金
		Out_Prize[1][18610]=120---鬼翼通_金
		Out_Prize[1][18615]=60---饲五毒_金
		Out_Prize[1][18620]=120---凝血咒_金
		Out_Prize[1][18625]=120---噬魂蛊_金
		Out_Prize[1][18635]=60---毒鳞刺_金
		Out_Prize[1][18640]=120---附蛊蛭_金
		Out_Prize[1][18650]=15---碎魂蛊_金
		Out_Prize[1][18655]=60---蛊乾坤_金
	elseif id==18753 then
		Out_Prize[1][18461]=120---恶鬼牙_木
		Out_Prize[1][18466]=60---沥血心_木
		Out_Prize[1][18471]=120---化骨毒_木
		Out_Prize[1][18476]=120---攻心咒_木
		Out_Prize[1][18481]=120---乱心诀_木
		Out_Prize[1][18486]=120---无影毒_木
		Out_Prize[1][18491]=120---黯然咒_木
		Out_Prize[1][18496]=120---幕天毒_木
		Out_Prize[1][18501]=120---地灵咒_木
		Out_Prize[1][18511]=60---孽龙毒_木
		Out_Prize[1][18521]=60---定魂诀_木
		Out_Prize[1][18536]=120---百毒炼_木
		Out_Prize[1][18541]=120---怒灵噬_木
		Out_Prize[1][18546]=120---天劫火_木
		Out_Prize[1][18551]=120---鬼阴风_木
		Out_Prize[1][18556]=60---挫骨印_木
		Out_Prize[1][18561]=60---真火咒_木
		Out_Prize[1][18566]=120---回魂祭_木
		Out_Prize[1][18571]=120---猛鬼啸_木
		Out_Prize[1][18576]=60---罗生门_木
		Out_Prize[1][18581]=15---邪血神_木
		Out_Prize[1][18586]=120---百鬼抄_木
		Out_Prize[1][18596]=120---截宗秘_木
		Out_Prize[1][18601]=120---邪灵蛊_木
		Out_Prize[1][18606]=120---应声蛊_木
		Out_Prize[1][18611]=120---鬼翼通_木
		Out_Prize[1][18616]=60---饲五毒_木
		Out_Prize[1][18621]=120---凝血咒_木
		Out_Prize[1][18626]=120---噬魂蛊_木
		Out_Prize[1][18636]=60---毒鳞刺_木
		Out_Prize[1][18641]=120---附蛊蛭_木
		Out_Prize[1][18651]=15---碎魂蛊_木
		Out_Prize[1][18656]=60---蛊乾坤_木
	elseif id==18754 then
		Out_Prize[1][18462]=120---恶鬼牙_水
		Out_Prize[1][18467]=60---沥血心_水
		Out_Prize[1][18472]=120---化骨毒_水
		Out_Prize[1][18477]=120---攻心咒_水
		Out_Prize[1][18482]=120---乱心诀_水
		Out_Prize[1][18487]=120---无影毒_水
		Out_Prize[1][18492]=120---黯然咒_水
		Out_Prize[1][18497]=120---幕天毒_水
		Out_Prize[1][18502]=120---地灵咒_水
		Out_Prize[1][18512]=60---孽龙毒_水
		Out_Prize[1][18522]=60---定魂诀_水
		Out_Prize[1][18537]=120---百毒炼_水
		Out_Prize[1][18542]=120---怒灵噬_水
		Out_Prize[1][18547]=120---天劫火_水
		Out_Prize[1][18552]=120---鬼阴风_水
		Out_Prize[1][18557]=60---挫骨印_水
		Out_Prize[1][18562]=60---真火咒_水
		Out_Prize[1][18567]=120---回魂祭_水
		Out_Prize[1][18572]=120---猛鬼啸_水
		Out_Prize[1][18577]=60---罗生门_水
		Out_Prize[1][18582]=15---邪血神_水
		Out_Prize[1][18587]=120---百鬼抄_水
		Out_Prize[1][18597]=120---截宗秘_水
		Out_Prize[1][18602]=120---邪灵蛊_水
		Out_Prize[1][18607]=120---应声蛊_水
		Out_Prize[1][18612]=120---鬼翼通_水
		Out_Prize[1][18617]=60---饲五毒_水
		Out_Prize[1][18622]=120---凝血咒_水
		Out_Prize[1][18627]=120---噬魂蛊_水
		Out_Prize[1][18637]=60---毒鳞刺_水
		Out_Prize[1][18642]=120---附蛊蛭_水
		Out_Prize[1][18652]=15---碎魂蛊_水
		Out_Prize[1][18657]=60---蛊乾坤_水
	elseif id==18755 then
		Out_Prize[1][18463]=120---恶鬼牙_火
		Out_Prize[1][18468]=60---沥血心_火
		Out_Prize[1][18473]=120---化骨毒_火
		Out_Prize[1][18478]=120---攻心咒_火
		Out_Prize[1][18483]=120---乱心诀_火
		Out_Prize[1][18488]=120---无影毒_火
		Out_Prize[1][18493]=120---黯然咒_火
		Out_Prize[1][18498]=120---幕天毒_火
		Out_Prize[1][18503]=120---地灵咒_火
		Out_Prize[1][18513]=60---孽龙毒_火
		Out_Prize[1][18523]=60---定魂诀_火
		Out_Prize[1][18538]=120---百毒炼_火
		Out_Prize[1][18543]=120---怒灵噬_火
		Out_Prize[1][18548]=120---天劫火_火
		Out_Prize[1][18553]=120---鬼阴风_火
		Out_Prize[1][18558]=60---挫骨印_火
		Out_Prize[1][18563]=60---真火咒_火
		Out_Prize[1][18568]=120---回魂祭_火
		Out_Prize[1][18573]=120---猛鬼啸_火
		Out_Prize[1][18578]=60---罗生门_火
		Out_Prize[1][18583]=15---邪血神_火
		Out_Prize[1][18588]=120---百鬼抄_火
		Out_Prize[1][18598]=120---截宗秘_火
		Out_Prize[1][18603]=120---邪灵蛊_火
		Out_Prize[1][18608]=120---应声蛊_火
		Out_Prize[1][18613]=120---鬼翼通_火
		Out_Prize[1][18618]=60---饲五毒_火
		Out_Prize[1][18623]=120---凝血咒_火
		Out_Prize[1][18628]=120---噬魂蛊_火
		Out_Prize[1][18638]=60---毒鳞刺_火
		Out_Prize[1][18643]=120---附蛊蛭_火
		Out_Prize[1][18653]=15---碎魂蛊_火
		Out_Prize[1][18658]=60---蛊乾坤_火

	elseif id==18756 then 
		Out_Prize[1][18464]=120---恶鬼牙_土
		Out_Prize[1][18469]=60---沥血心_土
		Out_Prize[1][18474]=120---化骨毒_土
		Out_Prize[1][18479]=120---攻心咒_土
		Out_Prize[1][18484]=120---乱心诀_土
		Out_Prize[1][18489]=120---无影毒_土
		Out_Prize[1][18494]=120---黯然咒_土
		Out_Prize[1][18499]=120---幕天毒_土
		Out_Prize[1][18504]=120---地灵咒_土
		Out_Prize[1][18514]=60---孽龙毒_土
		Out_Prize[1][18524]=60---定魂诀_土
		Out_Prize[1][18539]=120---百毒炼_土
		Out_Prize[1][18544]=120---怒灵噬_土
		Out_Prize[1][18549]=120---天劫火_土
		Out_Prize[1][18554]=120---鬼阴风_土
		Out_Prize[1][18559]=60---挫骨印_土
		Out_Prize[1][18564]=60---真火咒_土
		Out_Prize[1][18569]=120---回魂祭_土
		Out_Prize[1][18574]=120---猛鬼啸_土
		Out_Prize[1][18579]=60---罗生门_土
		Out_Prize[1][18584]=15---邪血神_土
		Out_Prize[1][18589]=120---百鬼抄_土
		Out_Prize[1][18599]=120---截宗秘_土
		Out_Prize[1][18604]=120---邪灵蛊_土
		Out_Prize[1][18609]=120---应声蛊_土
		Out_Prize[1][18614]=120---鬼翼通_土
		Out_Prize[1][18619]=60---饲五毒_土
		Out_Prize[1][18624]=120---凝血咒_土
		Out_Prize[1][18629]=120---噬魂蛊_土
		Out_Prize[1][18639]=60---毒鳞刺_土
		Out_Prize[1][18644]=120---附蛊蛭_土
		Out_Prize[1][18654]=15---碎魂蛊_土
		Out_Prize[1][18659]=60---蛊乾坤_土

	-- 九黎护符材料
	elseif id==31494 then	-- 九黎金
		Out_Prize[1][30477]=140
		Out_Prize[1][30478]=50
		Out_Prize[1][30479]=20
		Out_Prize[1][30480]=200
		Out_Prize[1][30481]=140
		Out_Prize[1][30482]=90
		Out_Prize[1][30483]=50
		Out_Prize[1][30484]=20
		Out_Prize[1][30485]=140
		Out_Prize[1][30486]=50
		Out_Prize[1][30487]=20
		Out_Prize[1][30491]=200
		Out_Prize[1][30492]=90
		Out_Prize[1][30493]=50
		Out_Prize[1][30494]=140
		Out_Prize[1][30495]=50
		Out_Prize[1][30496]=20
		Out_Prize[1][30497]=200
		Out_Prize[1][30498]=140
		Out_Prize[1][30499]=90
		Out_Prize[1][30500]=50
		Out_Prize[1][30501]=20
		Out_Prize[1][30502]=140
		Out_Prize[1][30503]=50
		Out_Prize[1][30504]=20
		Out_Prize[1][30511]=140
		Out_Prize[1][30512]=50
		Out_Prize[1][30513]=20
		Out_Prize[1][30514]=200
		Out_Prize[1][30515]=140
		Out_Prize[1][30516]=90
		Out_Prize[1][30517]=50
		Out_Prize[1][30518]=20

	elseif id==31495 then	-- 九黎木
		Out_Prize[1][30528]=140
		Out_Prize[1][30529]=50
		Out_Prize[1][30530]=20
		Out_Prize[1][30531]=200
		Out_Prize[1][30532]=140
		Out_Prize[1][30533]=90
		Out_Prize[1][30534]=50
		Out_Prize[1][30535]=20
		Out_Prize[1][30536]=140
		Out_Prize[1][30537]=50
		Out_Prize[1][30538]=20
		Out_Prize[1][30542]=200
		Out_Prize[1][30543]=90
		Out_Prize[1][30544]=50
		Out_Prize[1][30545]=140
		Out_Prize[1][30546]=50
		Out_Prize[1][30547]=20
		Out_Prize[1][30548]=200
		Out_Prize[1][30549]=140
		Out_Prize[1][30550]=90
		Out_Prize[1][30551]=50
		Out_Prize[1][30552]=20
		Out_Prize[1][30553]=140
		Out_Prize[1][30554]=50
		Out_Prize[1][30555]=20
		Out_Prize[1][30562]=140
		Out_Prize[1][30563]=50
		Out_Prize[1][30564]=20
		Out_Prize[1][30565]=200
		Out_Prize[1][30566]=140
		Out_Prize[1][30567]=90
		Out_Prize[1][30568]=50
		Out_Prize[1][30569]=20

	elseif id==31496 then	-- 九黎水
		Out_Prize[1][30579]=140
		Out_Prize[1][30580]=50
		Out_Prize[1][30581]=20
		Out_Prize[1][30582]=200
		Out_Prize[1][30583]=140
		Out_Prize[1][30584]=90
		Out_Prize[1][30585]=50
		Out_Prize[1][30586]=20
		Out_Prize[1][30587]=140
		Out_Prize[1][30588]=50
		Out_Prize[1][30589]=20
		Out_Prize[1][30593]=200
		Out_Prize[1][30594]=90
		Out_Prize[1][30595]=50
		Out_Prize[1][30596]=140
		Out_Prize[1][30597]=50
		Out_Prize[1][30598]=20
		Out_Prize[1][30599]=200
		Out_Prize[1][30600]=140
		Out_Prize[1][30601]=90
		Out_Prize[1][30602]=50
		Out_Prize[1][30603]=20
		Out_Prize[1][30604]=140
		Out_Prize[1][30605]=50
		Out_Prize[1][30606]=20
		Out_Prize[1][30613]=140
		Out_Prize[1][30614]=50
		Out_Prize[1][30615]=20
		Out_Prize[1][30616]=200
		Out_Prize[1][30617]=140
		Out_Prize[1][30618]=90
		Out_Prize[1][30619]=50
		Out_Prize[1][30620]=20

	elseif id==31493 then	-- 九黎火
		Out_Prize[1][30630]=140
		Out_Prize[1][30631]=50
		Out_Prize[1][30632]=20
		Out_Prize[1][30633]=200
		Out_Prize[1][30634]=140
		Out_Prize[1][30635]=90
		Out_Prize[1][30636]=50
		Out_Prize[1][30637]=20
		Out_Prize[1][30638]=140
		Out_Prize[1][30639]=50
		Out_Prize[1][30640]=20
		Out_Prize[1][30644]=200
		Out_Prize[1][30645]=90
		Out_Prize[1][30646]=50
		Out_Prize[1][30647]=140
		Out_Prize[1][30648]=50
		Out_Prize[1][30649]=20
		Out_Prize[1][30650]=200
		Out_Prize[1][30651]=140
		Out_Prize[1][30652]=90
		Out_Prize[1][30653]=50
		Out_Prize[1][30654]=20
		Out_Prize[1][30655]=140
		Out_Prize[1][30656]=50
		Out_Prize[1][30657]=20
		Out_Prize[1][30664]=140
		Out_Prize[1][30665]=50
		Out_Prize[1][30666]=20
		Out_Prize[1][30667]=200
		Out_Prize[1][30668]=140
		Out_Prize[1][30669]=90
		Out_Prize[1][30670]=50
		Out_Prize[1][30671]=20

	elseif id==31497 then	-- 九黎土
		Out_Prize[1][30681]=140
		Out_Prize[1][30682]=50
		Out_Prize[1][30683]=20
		Out_Prize[1][30684]=200
		Out_Prize[1][30685]=140
		Out_Prize[1][30686]=90
		Out_Prize[1][30687]=50
		Out_Prize[1][30688]=20
		Out_Prize[1][30689]=140
		Out_Prize[1][30690]=50
		Out_Prize[1][30691]=20
		Out_Prize[1][30695]=200
		Out_Prize[1][30696]=90
		Out_Prize[1][30697]=50
		Out_Prize[1][30698]=140
		Out_Prize[1][30699]=50
		Out_Prize[1][30700]=20
		Out_Prize[1][30701]=200
		Out_Prize[1][30702]=140
		Out_Prize[1][30703]=90
		Out_Prize[1][30704]=50
		Out_Prize[1][30705]=20
		Out_Prize[1][30706]=140
		Out_Prize[1][30707]=50
		Out_Prize[1][30708]=20
		Out_Prize[1][30715]=140
		Out_Prize[1][30716]=50
		Out_Prize[1][30717]=20
		Out_Prize[1][30718]=200
		Out_Prize[1][30719]=140
		Out_Prize[1][30720]=90
		Out_Prize[1][30721]=50
		Out_Prize[1][30722]=20

	-- 烈山护符材料
	elseif id==31499 then	-- 烈山金
		Out_Prize[1][30731]=200
		Out_Prize[1][30732]=140
		Out_Prize[1][30733]=90
		Out_Prize[1][30734]=50
		Out_Prize[1][30736]=200
		Out_Prize[1][30737]=140
		Out_Prize[1][30738]=90
		Out_Prize[1][30740]=20
		Out_Prize[1][30747]=140
		Out_Prize[1][30748]=200
		Out_Prize[1][30749]=140
		Out_Prize[1][30750]=90
		Out_Prize[1][30751]=50
		Out_Prize[1][30995]=20
		Out_Prize[1][30753]=200
		Out_Prize[1][30754]=140
		Out_Prize[1][30755]=90
		Out_Prize[1][30756]=50
		Out_Prize[1][30759]=140
		Out_Prize[1][30760]=200
		Out_Prize[1][30761]=140
		Out_Prize[1][30762]=90
		Out_Prize[1][30763]=50
		Out_Prize[1][30764]=20
		Out_Prize[1][30771]=200
		Out_Prize[1][30772]=140
		Out_Prize[1][30773]=90
		Out_Prize[1][30774]=50
		Out_Prize[1][30775]=20
		Out_Prize[1][30778]=200
		Out_Prize[1][30779]=140
		Out_Prize[1][30780]=90
		Out_Prize[1][30781]=50

	elseif id==31500 then	-- 烈山木
		Out_Prize[1][30784]=200
		Out_Prize[1][30785]=140
		Out_Prize[1][30786]=90
		Out_Prize[1][30787]=50
		Out_Prize[1][30789]=200
		Out_Prize[1][30790]=140
		Out_Prize[1][30791]=90
		Out_Prize[1][30793]=20
		Out_Prize[1][30800]=140
		Out_Prize[1][30801]=200
		Out_Prize[1][30802]=140
		Out_Prize[1][30803]=90
		Out_Prize[1][30804]=50
		Out_Prize[1][30805]=20
		Out_Prize[1][30806]=200
		Out_Prize[1][30807]=140
		Out_Prize[1][30808]=90
		Out_Prize[1][30809]=50
		Out_Prize[1][30811]=140
		Out_Prize[1][30812]=200
		Out_Prize[1][30813]=140
		Out_Prize[1][30814]=90
		Out_Prize[1][30815]=50
		Out_Prize[1][30816]=20
		Out_Prize[1][30823]=200
		Out_Prize[1][30824]=140
		Out_Prize[1][30825]=90
		Out_Prize[1][30826]=50
		Out_Prize[1][30827]=20
		Out_Prize[1][30829]=200
		Out_Prize[1][30830]=140
		Out_Prize[1][30831]=90
		Out_Prize[1][30832]=50

	elseif id==31501 then	-- 烈山水
		Out_Prize[1][30835]=200
		Out_Prize[1][30836]=140
		Out_Prize[1][30837]=90
		Out_Prize[1][30838]=50
		Out_Prize[1][30840]=200
		Out_Prize[1][30841]=140
		Out_Prize[1][30842]=90
		Out_Prize[1][30844]=20
		Out_Prize[1][30851]=140
		Out_Prize[1][30852]=200
		Out_Prize[1][30853]=140
		Out_Prize[1][30854]=90
		Out_Prize[1][30855]=50
		Out_Prize[1][30856]=20
		Out_Prize[1][30857]=200
		Out_Prize[1][30858]=140
		Out_Prize[1][30859]=90
		Out_Prize[1][30860]=50
		Out_Prize[1][30862]=140
		Out_Prize[1][30863]=200
		Out_Prize[1][30864]=140
		Out_Prize[1][30865]=90
		Out_Prize[1][30866]=50
		Out_Prize[1][30867]=20
		Out_Prize[1][30874]=200
		Out_Prize[1][30875]=140
		Out_Prize[1][30876]=90
		Out_Prize[1][30877]=50
		Out_Prize[1][30878]=20
		Out_Prize[1][30880]=200
		Out_Prize[1][30881]=140
		Out_Prize[1][30882]=90
		Out_Prize[1][30883]=50

	elseif id==31498 then	-- 烈山火
		Out_Prize[1][30886]=200
		Out_Prize[1][30887]=140
		Out_Prize[1][30888]=90
		Out_Prize[1][30889]=50
		Out_Prize[1][30891]=200
		Out_Prize[1][30892]=140
		Out_Prize[1][30893]=90
		Out_Prize[1][30895]=20
		Out_Prize[1][30902]=140
		Out_Prize[1][30903]=200
		Out_Prize[1][30904]=140
		Out_Prize[1][30905]=90
		Out_Prize[1][30906]=50
		Out_Prize[1][30907]=20
		Out_Prize[1][30908]=200
		Out_Prize[1][30909]=140
		Out_Prize[1][30910]=90
		Out_Prize[1][30911]=50
		Out_Prize[1][30913]=140
		Out_Prize[1][30914]=200
		Out_Prize[1][30915]=140
		Out_Prize[1][30916]=90
		Out_Prize[1][30917]=50
		Out_Prize[1][30918]=20
		Out_Prize[1][30925]=200
		Out_Prize[1][30926]=140
		Out_Prize[1][30927]=90
		Out_Prize[1][30928]=50
		Out_Prize[1][30929]=20
		Out_Prize[1][30931]=200
		Out_Prize[1][30932]=140
		Out_Prize[1][30933]=90
		Out_Prize[1][30934]=50

	elseif id==31502 then	-- 烈山土
		Out_Prize[1][30938]=200
		Out_Prize[1][30939]=140
		Out_Prize[1][30940]=90
		Out_Prize[1][30941]=50
		Out_Prize[1][30943]=200
		Out_Prize[1][30944]=140
		Out_Prize[1][30945]=90
		Out_Prize[1][30947]=20
		Out_Prize[1][30954]=140
		Out_Prize[1][30955]=200
		Out_Prize[1][30956]=140
		Out_Prize[1][30957]=90
		Out_Prize[1][30958]=50
		Out_Prize[1][30959]=20
		Out_Prize[1][30960]=200
		Out_Prize[1][30961]=140
		Out_Prize[1][30962]=90
		Out_Prize[1][30963]=50
		Out_Prize[1][30965]=140
		Out_Prize[1][30966]=200
		Out_Prize[1][30967]=140
		Out_Prize[1][30968]=90
		Out_Prize[1][30969]=50
		Out_Prize[1][30970]=20
		Out_Prize[1][30977]=200
		Out_Prize[1][30978]=140
		Out_Prize[1][30979]=90
		Out_Prize[1][30980]=50
		Out_Prize[1][30981]=20
		Out_Prize[1][30983]=200
		Out_Prize[1][30984]=140
		Out_Prize[1][30985]=90
		Out_Prize[1][30986]=50

	elseif id==40357 then	-- 怀光土
		Out_Prize[1][38737]=200
		Out_Prize[1][38754]=200
		Out_Prize[1][38743]=200
		Out_Prize[1][38766]=200
		Out_Prize[1][38748]=200
		Out_Prize[1][38759]=200
		Out_Prize[1][38783]=200
		Out_Prize[1][38738]=140
		Out_Prize[1][38755]=140
		Out_Prize[1][38777]=140
		Out_Prize[1][38744]=140
		Out_Prize[1][38767]=140
		Out_Prize[1][38749]=140
		Out_Prize[1][38760]=140
		Out_Prize[1][38784]=140
		Out_Prize[1][38739]=90
		Out_Prize[1][38756]=90
		Out_Prize[1][38745]=90
		Out_Prize[1][38768]=90
		Out_Prize[1][38750]=90
		Out_Prize[1][38761]=90
		Out_Prize[1][38785]=90
		Out_Prize[1][38740]=50
		Out_Prize[1][38757]=50
		Out_Prize[1][38779]=50
		Out_Prize[1][38746]=50
		Out_Prize[1][38769]=50
		Out_Prize[1][38751]=50
		Out_Prize[1][38762]=50
		Out_Prize[1][38786]=50
		Out_Prize[1][38741]=20
		Out_Prize[1][38763]=20
		Out_Prize[1][38752]=20

	elseif id==40358 then	-- 怀光木
		Out_Prize[1][38431]=200
		Out_Prize[1][38448]=200
		Out_Prize[1][38437]=200
		Out_Prize[1][38460]=200
		Out_Prize[1][38442]=200
		Out_Prize[1][38453]=200
		Out_Prize[1][38477]=200
		Out_Prize[1][38432]=140
		Out_Prize[1][38449]=140
		Out_Prize[1][38471]=140
		Out_Prize[1][38438]=140
		Out_Prize[1][38461]=140
		Out_Prize[1][38443]=140
		Out_Prize[1][38454]=140
		Out_Prize[1][38478]=140
		Out_Prize[1][38433]=90
		Out_Prize[1][38450]=90
		Out_Prize[1][38439]=90
		Out_Prize[1][38462]=90
		Out_Prize[1][38444]=90
		Out_Prize[1][38455]=90
		Out_Prize[1][38479]=90
		Out_Prize[1][38434]=50
		Out_Prize[1][38451]=50
		Out_Prize[1][38473]=50
		Out_Prize[1][38440]=50
		Out_Prize[1][38463]=50
		Out_Prize[1][38445]=50
		Out_Prize[1][38456]=50
		Out_Prize[1][38480]=50
		Out_Prize[1][38435]=20
		Out_Prize[1][38457]=20
		Out_Prize[1][38446]=20

	elseif id==40359 then	-- 怀光水
		Out_Prize[1][38533]=200
		Out_Prize[1][38550]=200
		Out_Prize[1][38539]=200
		Out_Prize[1][38562]=200
		Out_Prize[1][38544]=200
		Out_Prize[1][38555]=200
		Out_Prize[1][38579]=200
		Out_Prize[1][38534]=140
		Out_Prize[1][38551]=140
		Out_Prize[1][38573]=140
		Out_Prize[1][38540]=140
		Out_Prize[1][38563]=140
		Out_Prize[1][38545]=140
		Out_Prize[1][38556]=140
		Out_Prize[1][38580]=140
		Out_Prize[1][38535]=90
		Out_Prize[1][38552]=90
		Out_Prize[1][38541]=90
		Out_Prize[1][38564]=90
		Out_Prize[1][38546]=90
		Out_Prize[1][38557]=90
		Out_Prize[1][38581]=90
		Out_Prize[1][38536]=50
		Out_Prize[1][38553]=50
		Out_Prize[1][38575]=50
		Out_Prize[1][38542]=50
		Out_Prize[1][38565]=50
		Out_Prize[1][38547]=50
		Out_Prize[1][38558]=50
		Out_Prize[1][38582]=50
		Out_Prize[1][38537]=20
		Out_Prize[1][38559]=20
		Out_Prize[1][38548]=20

	elseif id==40360 then	-- 怀光火
		Out_Prize[1][38635]=200
		Out_Prize[1][38652]=200
		Out_Prize[1][38641]=200
		Out_Prize[1][38664]=200
		Out_Prize[1][38646]=200
		Out_Prize[1][38657]=200
		Out_Prize[1][38681]=200
		Out_Prize[1][38636]=140
		Out_Prize[1][38653]=140
		Out_Prize[1][38675]=140
		Out_Prize[1][38642]=140
		Out_Prize[1][38665]=140
		Out_Prize[1][38647]=140
		Out_Prize[1][38658]=140
		Out_Prize[1][38682]=140
		Out_Prize[1][38637]=90
		Out_Prize[1][38654]=90
		Out_Prize[1][38643]=90
		Out_Prize[1][38666]=90
		Out_Prize[1][38648]=90
		Out_Prize[1][38659]=90
		Out_Prize[1][38683]=90
		Out_Prize[1][38638]=50
		Out_Prize[1][38655]=50
		Out_Prize[1][38677]=50
		Out_Prize[1][38644]=50
		Out_Prize[1][38667]=50
		Out_Prize[1][38649]=50
		Out_Prize[1][38660]=50
		Out_Prize[1][38684]=50
		Out_Prize[1][38639]=20
		Out_Prize[1][38661]=20
		Out_Prize[1][38650]=20

	elseif id==40361 then	-- 怀光金
		Out_Prize[1][38839]=200
		Out_Prize[1][38346]=200
		Out_Prize[1][38335]=200
		Out_Prize[1][38358]=200
		Out_Prize[1][38340]=200
		Out_Prize[1][38351]=200
		Out_Prize[1][38375]=200
		Out_Prize[1][38840]=140
		Out_Prize[1][38347]=140
		Out_Prize[1][38369]=140
		Out_Prize[1][38336]=140
		Out_Prize[1][38359]=140
		Out_Prize[1][38341]=140
		Out_Prize[1][38352]=140
		Out_Prize[1][38376]=140
		Out_Prize[1][38331]=90
		Out_Prize[1][38348]=90
		Out_Prize[1][38337]=90
		Out_Prize[1][38360]=90
		Out_Prize[1][38342]=90
		Out_Prize[1][38353]=90
		Out_Prize[1][38377]=90
		Out_Prize[1][38332]=50
		Out_Prize[1][38349]=50
		Out_Prize[1][38371]=50
		Out_Prize[1][38338]=50
		Out_Prize[1][38361]=50
		Out_Prize[1][38343]=50
		Out_Prize[1][38354]=50
		Out_Prize[1][38378]=50
		Out_Prize[1][38333]=20
		Out_Prize[1][38355]=20
		Out_Prize[1][38344]=20

	elseif id==40362 then	-- 天华土
		Out_Prize[1][38812]=200
		Out_Prize[1][38809]=200
		Out_Prize[1][38822]=200
		Out_Prize[1][38792]=200
		Out_Prize[1][38788]=200
		Out_Prize[1][38789]=140
		Out_Prize[1][38798]=200
		Out_Prize[1][38832]=200
		Out_Prize[1][38833]=140
		Out_Prize[1][38813]=140
		Out_Prize[1][38793]=140
		Out_Prize[1][38805]=140
		Out_Prize[1][38823]=140
		Out_Prize[1][38815]=200
		Out_Prize[1][38829]=200
		Out_Prize[1][38795]=200
		Out_Prize[1][38826]=200
		Out_Prize[1][38810]=90
		Out_Prize[1][38814]=90
		Out_Prize[1][38824]=90
		Out_Prize[1][38799]=140
		Out_Prize[1][38830]=140
		Out_Prize[1][38816]=140
		Out_Prize[1][38796]=140
		Out_Prize[1][38819]=50
		Out_Prize[1][38827]=140
		Out_Prize[1][38791]=50
		Out_Prize[1][38806]=50
		Out_Prize[1][38825]=50
		Out_Prize[1][38803]=50
		Out_Prize[1][38835]=20
		Out_Prize[1][38804]=50
		Out_Prize[1][38808]=50

	elseif id==40363 then	-- 天华木
		Out_Prize[1][38506]=200
		Out_Prize[1][38503]=200
		Out_Prize[1][38516]=200
		Out_Prize[1][38486]=200
		Out_Prize[1][38482]=200
		Out_Prize[1][38483]=140
		Out_Prize[1][38492]=200
		Out_Prize[1][38526]=200
		Out_Prize[1][38527]=140
		Out_Prize[1][38507]=140
		Out_Prize[1][38487]=140
		Out_Prize[1][38499]=140
		Out_Prize[1][38517]=140
		Out_Prize[1][38509]=200
		Out_Prize[1][38523]=200
		Out_Prize[1][38489]=200
		Out_Prize[1][38520]=200
		Out_Prize[1][38504]=90
		Out_Prize[1][38508]=90
		Out_Prize[1][38518]=90
		Out_Prize[1][38493]=140
		Out_Prize[1][38524]=140
		Out_Prize[1][38510]=140
		Out_Prize[1][38490]=140
		Out_Prize[1][38513]=50
		Out_Prize[1][38521]=140
		Out_Prize[1][38485]=50
		Out_Prize[1][38500]=50
		Out_Prize[1][38519]=50
		Out_Prize[1][38497]=50
		Out_Prize[1][38529]=20
		Out_Prize[1][38498]=50
		Out_Prize[1][38502]=50

	elseif id==40364 then	-- 天华水
		Out_Prize[1][38608]=200
		Out_Prize[1][38605]=200
		Out_Prize[1][38618]=200
		Out_Prize[1][38588]=200
		Out_Prize[1][38584]=200
		Out_Prize[1][38585]=140
		Out_Prize[1][38594]=200
		Out_Prize[1][38628]=200
		Out_Prize[1][38629]=140
		Out_Prize[1][38609]=140
		Out_Prize[1][38589]=140
		Out_Prize[1][38601]=140
		Out_Prize[1][38619]=140
		Out_Prize[1][38611]=200
		Out_Prize[1][38625]=200
		Out_Prize[1][38591]=200
		Out_Prize[1][38622]=200
		Out_Prize[1][38606]=90
		Out_Prize[1][38610]=90
		Out_Prize[1][38620]=90
		Out_Prize[1][38595]=140
		Out_Prize[1][38626]=140
		Out_Prize[1][38612]=140
		Out_Prize[1][38592]=140
		Out_Prize[1][38615]=50
		Out_Prize[1][38623]=140
		Out_Prize[1][38587]=50
		Out_Prize[1][38602]=50
		Out_Prize[1][38621]=50
		Out_Prize[1][38599]=50
		Out_Prize[1][38631]=20
		Out_Prize[1][38600]=50
		Out_Prize[1][38604]=50

	elseif id==40365 then	-- 天华火
		Out_Prize[1][38710]=200
		Out_Prize[1][38707]=200
		Out_Prize[1][38720]=200
		Out_Prize[1][38690]=200
		Out_Prize[1][38686]=200
		Out_Prize[1][38687]=140
		Out_Prize[1][38696]=200
		Out_Prize[1][38730]=200
		Out_Prize[1][38731]=140
		Out_Prize[1][38711]=140
		Out_Prize[1][38691]=140
		Out_Prize[1][38703]=140
		Out_Prize[1][38721]=140
		Out_Prize[1][38713]=200
		Out_Prize[1][38727]=200
		Out_Prize[1][38693]=200
		Out_Prize[1][38724]=200
		Out_Prize[1][38708]=90
		Out_Prize[1][38712]=90
		Out_Prize[1][38722]=90
		Out_Prize[1][38697]=140
		Out_Prize[1][38728]=140
		Out_Prize[1][38714]=140
		Out_Prize[1][38694]=140
		Out_Prize[1][38717]=50
		Out_Prize[1][38725]=140
		Out_Prize[1][38689]=50
		Out_Prize[1][38704]=50
		Out_Prize[1][38723]=50
		Out_Prize[1][38701]=50
		Out_Prize[1][38733]=20
		Out_Prize[1][38702]=50
		Out_Prize[1][38706]=50

	elseif id==40366 then	-- 天华金
		Out_Prize[1][38404]=200
		Out_Prize[1][38401]=200
		Out_Prize[1][38414]=200
		Out_Prize[1][38384]=200
		Out_Prize[1][38380]=200
		Out_Prize[1][38381]=140
		Out_Prize[1][38390]=200
		Out_Prize[1][38424]=200
		Out_Prize[1][38425]=140
		Out_Prize[1][38405]=140
		Out_Prize[1][38385]=140
		Out_Prize[1][38397]=140
		Out_Prize[1][38415]=140
		Out_Prize[1][38407]=200
		Out_Prize[1][38421]=200
		Out_Prize[1][38387]=200
		Out_Prize[1][38418]=200
		Out_Prize[1][38402]=90
		Out_Prize[1][38406]=90
		Out_Prize[1][38416]=90
		Out_Prize[1][38391]=140
		Out_Prize[1][38422]=140
		Out_Prize[1][38408]=140
		Out_Prize[1][38388]=140
		Out_Prize[1][38411]=50
		Out_Prize[1][38419]=140
		Out_Prize[1][38383]=50
		Out_Prize[1][38398]=50
		Out_Prize[1][38417]=50
		Out_Prize[1][38395]=50
		Out_Prize[1][38427]=20
		Out_Prize[1][38396]=50
		Out_Prize[1][38400]=50
		
	elseif id==50615 then  --太昊金
		Out_Prize[1][49620]=90
		Out_Prize[1][49624]=200
		Out_Prize[1][49662]=150
		Out_Prize[1][49621]=200
		Out_Prize[1][49632]=150
		Out_Prize[1][49645]=200
		Out_Prize[1][49649]=90
		Out_Prize[1][49625]=150
		Out_Prize[1][49628]=150
		Out_Prize[1][49650]=120
		Out_Prize[1][49654]=120
		Out_Prize[1][49657]=120
		Out_Prize[1][49646]=150
		Out_Prize[1][49663]=120
		Out_Prize[1][49617]=200
		Out_Prize[1][49622]=150
		Out_Prize[1][49633]=50
		Out_Prize[1][49630]=120
		Out_Prize[1][49634]=50
		Out_Prize[1][49637]=120
		Out_Prize[1][49651]=90
		Out_Prize[1][49655]=90
		Out_Prize[1][49618]=20
		Out_Prize[1][49623]=50
		Out_Prize[1][49631]=50
		Out_Prize[1][49638]=120
		Out_Prize[1][49648]=20
		Out_Prize[1][49656]=90
		Out_Prize[1][49660]=50
		Out_Prize[1][49635]=90
		Out_Prize[1][49639]=90
		Out_Prize[1][49640]=50
		Out_Prize[1][49661]=90
		
	elseif id==50616 then  --太昊木
		Out_Prize[1][49671]=90
		Out_Prize[1][49675]=200
		Out_Prize[1][49713]=150
		Out_Prize[1][49672]=200
		Out_Prize[1][49683]=150
		Out_Prize[1][49696]=200
		Out_Prize[1][49700]=90
		Out_Prize[1][49676]=150
		Out_Prize[1][49679]=150
		Out_Prize[1][49701]=120
		Out_Prize[1][49705]=120
		Out_Prize[1][49708]=120
		Out_Prize[1][49697]=150
		Out_Prize[1][49714]=120
		Out_Prize[1][49668]=200
		Out_Prize[1][49673]=150
		Out_Prize[1][49684]=50
		Out_Prize[1][49681]=120
		Out_Prize[1][49685]=50
		Out_Prize[1][49688]=120
		Out_Prize[1][49702]=90
		Out_Prize[1][49706]=90
		Out_Prize[1][49669]=20
		Out_Prize[1][49674]=50
		Out_Prize[1][49682]=50
		Out_Prize[1][49689]=120
		Out_Prize[1][49699]=20
		Out_Prize[1][49707]=90
		Out_Prize[1][49711]=50
		Out_Prize[1][49686]=90
		Out_Prize[1][49690]=90
		Out_Prize[1][49691]=50
		Out_Prize[1][49712]=90
	
	elseif id==50617 then  --太昊水
		Out_Prize[1][49722]=90
		Out_Prize[1][49726]=200
		Out_Prize[1][49764]=150
		Out_Prize[1][49723]=200
		Out_Prize[1][49734]=150
		Out_Prize[1][49747]=200
		Out_Prize[1][49751]=90
		Out_Prize[1][49727]=150
		Out_Prize[1][49730]=150
		Out_Prize[1][49752]=120
		Out_Prize[1][49756]=120
		Out_Prize[1][49759]=120
		Out_Prize[1][49748]=150
		Out_Prize[1][49765]=120
		Out_Prize[1][49719]=200
		Out_Prize[1][49724]=150
		Out_Prize[1][49735]=50
		Out_Prize[1][49732]=120
		Out_Prize[1][49736]=50
		Out_Prize[1][49739]=120
		Out_Prize[1][49753]=90
		Out_Prize[1][49757]=90
		Out_Prize[1][49720]=20
		Out_Prize[1][49725]=50
		Out_Prize[1][49733]=50
		Out_Prize[1][49740]=120
		Out_Prize[1][49750]=20
		Out_Prize[1][49758]=90
		Out_Prize[1][49762]=50
		Out_Prize[1][49737]=90
		Out_Prize[1][49741]=90
		Out_Prize[1][49742]=50
		Out_Prize[1][49763]=90
		
	elseif id==50618 then  --太昊火
		Out_Prize[1][49773]=90
		Out_Prize[1][49777]=200
		Out_Prize[1][49815]=150
		Out_Prize[1][49774]=200
		Out_Prize[1][49785]=150
		Out_Prize[1][49798]=200
		Out_Prize[1][49802]=90
		Out_Prize[1][49778]=150
		Out_Prize[1][49781]=150
		Out_Prize[1][49803]=120
		Out_Prize[1][49807]=120
		Out_Prize[1][49810]=120
		Out_Prize[1][49799]=150
		Out_Prize[1][49816]=120
		Out_Prize[1][49770]=200
		Out_Prize[1][49775]=150
		Out_Prize[1][49786]=50
		Out_Prize[1][49783]=120
		Out_Prize[1][49787]=50
		Out_Prize[1][49790]=120
		Out_Prize[1][49804]=90
		Out_Prize[1][49808]=90
		Out_Prize[1][49771]=20
		Out_Prize[1][49776]=50
		Out_Prize[1][49784]=50
		Out_Prize[1][49791]=120
		Out_Prize[1][49801]=20
		Out_Prize[1][49809]=90
		Out_Prize[1][49813]=50
		Out_Prize[1][49788]=90
		Out_Prize[1][49792]=90
		Out_Prize[1][49793]=50
		Out_Prize[1][49814]=90
		
	elseif id==50619 then  --太昊土
		Out_Prize[1][49824]=90
		Out_Prize[1][49828]=200
		Out_Prize[1][49866]=150
		Out_Prize[1][49825]=200
		Out_Prize[1][49836]=150
		Out_Prize[1][49849]=200
		Out_Prize[1][49853]=90
		Out_Prize[1][49829]=150
		Out_Prize[1][49832]=150
		Out_Prize[1][49854]=120
		Out_Prize[1][49858]=120
		Out_Prize[1][49861]=120
		Out_Prize[1][49850]=150
		Out_Prize[1][49867]=120
		Out_Prize[1][49821]=200
		Out_Prize[1][49826]=150
		Out_Prize[1][49837]=50
		Out_Prize[1][49834]=120
		Out_Prize[1][49838]=50
		Out_Prize[1][49841]=120
		Out_Prize[1][49855]=90
		Out_Prize[1][49859]=90
		Out_Prize[1][49822]=20
		Out_Prize[1][49827]=50
		Out_Prize[1][49835]=50
		Out_Prize[1][49842]=120
		Out_Prize[1][49852]=20
		Out_Prize[1][49860]=90
		Out_Prize[1][49864]=50
		Out_Prize[1][49839]=90
		Out_Prize[1][49843]=90
		Out_Prize[1][49844]=50
		Out_Prize[1][49865]=90

	elseif id==50610 then  --焚香金
		Out_Prize[1][49873]=220
		Out_Prize[1][49876]=220
		Out_Prize[1][49874]=220
		Out_Prize[1][49890]=220
		Out_Prize[1][49895]=30
		Out_Prize[1][49888]=110
		Out_Prize[1][49906]=110
		Out_Prize[1][49905]=20
		Out_Prize[1][49904]=40
		Out_Prize[1][49871]=10
		Out_Prize[1][49886]=10
		Out_Prize[1][49900]=40
		Out_Prize[1][49903]=10
		Out_Prize[1][49907]=50
		Out_Prize[1][49920]=10
		Out_Prize[1][49878]=20
		Out_Prize[1][49881]=20
		Out_Prize[1][49879]=110
		Out_Prize[1][49880]=50
		Out_Prize[1][49882]=30
		Out_Prize[1][49883]=10
		Out_Prize[1][49892]=40
		Out_Prize[1][49896]=30
		Out_Prize[1][49887]=20
		Out_Prize[1][49891]=50
		Out_Prize[1][49897]=20
		Out_Prize[1][49899]=30
		Out_Prize[1][49909]=110
		Out_Prize[1][49910]=110
		Out_Prize[1][49912]=40
		Out_Prize[1][49916]=30
		Out_Prize[1][49915]=50
		Out_Prize[1][49917]=10
	
	elseif id==50611 then  --焚香木
		Out_Prize[1][49924]=220
		Out_Prize[1][49927]=220
		Out_Prize[1][49925]=220
		Out_Prize[1][49941]=220
		Out_Prize[1][49946]=30
		Out_Prize[1][49939]=110
		Out_Prize[1][49957]=110
		Out_Prize[1][49956]=20
		Out_Prize[1][49955]=40
		Out_Prize[1][49922]=10
		Out_Prize[1][49937]=10
		Out_Prize[1][49951]=40
		Out_Prize[1][49954]=10
		Out_Prize[1][49958]=50
		Out_Prize[1][49971]=10
		Out_Prize[1][49929]=20
		Out_Prize[1][49932]=20
		Out_Prize[1][49930]=110
		Out_Prize[1][49931]=50
		Out_Prize[1][49933]=30
		Out_Prize[1][49934]=10
		Out_Prize[1][49943]=40
		Out_Prize[1][49947]=30
		Out_Prize[1][49938]=20
		Out_Prize[1][49942]=50
		Out_Prize[1][49948]=20
		Out_Prize[1][49950]=30
		Out_Prize[1][49960]=110
		Out_Prize[1][49961]=110
		Out_Prize[1][49963]=40
		Out_Prize[1][49967]=30
		Out_Prize[1][49966]=50
		Out_Prize[1][49968]=10

	elseif id==50612 then  --焚香水
		Out_Prize[1][49975]=220
		Out_Prize[1][49978]=220
		Out_Prize[1][49976]=220
		Out_Prize[1][49992]=220
		Out_Prize[1][49997]=30
		Out_Prize[1][49990]=110
		Out_Prize[1][50008]=110
		Out_Prize[1][50007]=20
		Out_Prize[1][50006]=40
		Out_Prize[1][49973]=10
		Out_Prize[1][49988]=10
		Out_Prize[1][50002]=40
		Out_Prize[1][50005]=10
		Out_Prize[1][50009]=50
		Out_Prize[1][50022]=10
		Out_Prize[1][49980]=20
		Out_Prize[1][49983]=20
		Out_Prize[1][49981]=110
		Out_Prize[1][49982]=50
		Out_Prize[1][49984]=30
		Out_Prize[1][49985]=10
		Out_Prize[1][49994]=40
		Out_Prize[1][49998]=30
		Out_Prize[1][49989]=20
		Out_Prize[1][49993]=50
		Out_Prize[1][49999]=20
		Out_Prize[1][50001]=30
		Out_Prize[1][50011]=110
		Out_Prize[1][50012]=110
		Out_Prize[1][50014]=40
		Out_Prize[1][50018]=30
		Out_Prize[1][50017]=50
		Out_Prize[1][50019]=10	
	
	elseif id==50613 then  --焚香火
		Out_Prize[1][50026]=220
		Out_Prize[1][50029]=220
		Out_Prize[1][50027]=220
		Out_Prize[1][50043]=220
		Out_Prize[1][50048]=30
		Out_Prize[1][50041]=110
		Out_Prize[1][50059]=110
		Out_Prize[1][50058]=20
		Out_Prize[1][50057]=40
		Out_Prize[1][50024]=10
		Out_Prize[1][50039]=10
		Out_Prize[1][50053]=40
		Out_Prize[1][50056]=10
		Out_Prize[1][50060]=50
		Out_Prize[1][50073]=10
		Out_Prize[1][50031]=20
		Out_Prize[1][50034]=20
		Out_Prize[1][50032]=110
		Out_Prize[1][50033]=50
		Out_Prize[1][50035]=30
		Out_Prize[1][50036]=10
		Out_Prize[1][50045]=40
		Out_Prize[1][50049]=30
		Out_Prize[1][50040]=20
		Out_Prize[1][50044]=50
		Out_Prize[1][50050]=20
		Out_Prize[1][50052]=30
		Out_Prize[1][50062]=110
		Out_Prize[1][50063]=110
		Out_Prize[1][50065]=40
		Out_Prize[1][50069]=30
		Out_Prize[1][50068]=50
		Out_Prize[1][50070]=10

	elseif id==50614 then  --焚香土
		Out_Prize[1][50077]=220
		Out_Prize[1][50080]=220
		Out_Prize[1][50078]=220
		Out_Prize[1][50094]=220
		Out_Prize[1][50099]=30
		Out_Prize[1][50092]=110
		Out_Prize[1][50110]=110
		Out_Prize[1][50109]=20
		Out_Prize[1][50108]=40
		Out_Prize[1][50075]=10
		Out_Prize[1][50090]=10
		Out_Prize[1][50104]=40
		Out_Prize[1][50107]=10
		Out_Prize[1][50111]=50
		Out_Prize[1][50124]=10
		Out_Prize[1][50082]=20
		Out_Prize[1][50085]=20
		Out_Prize[1][50083]=110
		Out_Prize[1][50084]=50
		Out_Prize[1][50086]=30
		Out_Prize[1][50087]=10
		Out_Prize[1][50096]=40
		Out_Prize[1][50100]=30
		Out_Prize[1][50091]=20
		Out_Prize[1][50095]=50
		Out_Prize[1][50101]=20
		Out_Prize[1][50103]=30
		Out_Prize[1][50113]=110
		Out_Prize[1][50114]=110
		Out_Prize[1][50116]=40
		Out_Prize[1][50120]=30
		Out_Prize[1][50119]=50
		Out_Prize[1][50121]=10
		
	elseif id==54766 then  --辰皇金
		Out_Prize[1][54191]=220
		Out_Prize[1][54192]=220
		Out_Prize[1][54193]=220
		Out_Prize[1][54196]=220
		Out_Prize[1][54197]=220
		Out_Prize[1][54198]=110
		Out_Prize[1][54208]=110
		Out_Prize[1][54209]=110
		Out_Prize[1][54210]=110
		Out_Prize[1][54213]=110
		Out_Prize[1][54214]=50
		Out_Prize[1][54215]=50
		Out_Prize[1][54219]=50
		Out_Prize[1][54220]=50
		Out_Prize[1][54221]=50
		Out_Prize[1][54224]=30
		Out_Prize[1][54228]=30
		Out_Prize[1][54229]=30
		Out_Prize[1][54230]=30
		Out_Prize[1][54194]=20
		Out_Prize[1][54199]=20
		Out_Prize[1][54200]=20
		Out_Prize[1][54211]=20
		Out_Prize[1][54216]=20
		Out_Prize[1][54217]=20
		Out_Prize[1][54222]=20
		Out_Prize[1][54227]=20
		Out_Prize[1][54231]=20
		Out_Prize[1][54195]=10
		Out_Prize[1][54201]=10
		Out_Prize[1][54212]=10
		Out_Prize[1][54218]=10
		Out_Prize[1][54223]=10
		
	elseif id==54767 then  --辰皇木
		Out_Prize[1][54242]=220
		Out_Prize[1][54243]=220
		Out_Prize[1][54244]=220
		Out_Prize[1][54247]=220
		Out_Prize[1][54248]=220
		Out_Prize[1][54249]=110
		Out_Prize[1][54259]=110
		Out_Prize[1][54260]=110
		Out_Prize[1][54261]=110
		Out_Prize[1][54264]=110
		Out_Prize[1][54265]=50
		Out_Prize[1][54266]=50
		Out_Prize[1][54270]=50
		Out_Prize[1][54271]=50
		Out_Prize[1][54272]=50
		Out_Prize[1][54275]=30
		Out_Prize[1][54279]=30
		Out_Prize[1][54280]=30
		Out_Prize[1][54281]=30
		Out_Prize[1][54245]=20
		Out_Prize[1][54250]=20
		Out_Prize[1][54251]=20
		Out_Prize[1][54262]=20
		Out_Prize[1][54267]=20
		Out_Prize[1][54268]=20
		Out_Prize[1][54273]=20
		Out_Prize[1][54278]=20
		Out_Prize[1][54282]=20
		Out_Prize[1][54246]=10
		Out_Prize[1][54252]=10
		Out_Prize[1][54263]=10
		Out_Prize[1][54269]=10
		Out_Prize[1][54274]=10
		
	elseif id==54768 then  --辰皇水
		Out_Prize[1][54293]=220
		Out_Prize[1][54294]=220
		Out_Prize[1][54295]=220
		Out_Prize[1][54298]=220
		Out_Prize[1][54299]=220
		Out_Prize[1][54300]=110
		Out_Prize[1][54310]=110
		Out_Prize[1][54311]=110
		Out_Prize[1][54312]=110
		Out_Prize[1][54315]=110
		Out_Prize[1][54316]=50
		Out_Prize[1][54317]=50
		Out_Prize[1][54321]=50
		Out_Prize[1][54322]=50
		Out_Prize[1][54323]=50
		Out_Prize[1][54326]=30
		Out_Prize[1][54330]=30
		Out_Prize[1][54331]=30
		Out_Prize[1][54332]=30
		Out_Prize[1][54296]=20
		Out_Prize[1][54301]=20
		Out_Prize[1][54302]=20
		Out_Prize[1][54313]=20
		Out_Prize[1][54318]=20
		Out_Prize[1][54319]=20
		Out_Prize[1][54324]=20
		Out_Prize[1][54329]=20
		Out_Prize[1][54333]=20
		Out_Prize[1][54297]=10
		Out_Prize[1][54303]=10
		Out_Prize[1][54314]=10
		Out_Prize[1][54320]=10
		Out_Prize[1][54325]=10

	elseif id==54769 then  --辰皇火
		Out_Prize[1][54344]=220
		Out_Prize[1][54345]=220
		Out_Prize[1][54346]=220
		Out_Prize[1][54349]=220
		Out_Prize[1][54350]=220
		Out_Prize[1][54351]=110
		Out_Prize[1][54361]=110
		Out_Prize[1][54362]=110
		Out_Prize[1][54363]=110
		Out_Prize[1][54366]=110
		Out_Prize[1][54367]=50
		Out_Prize[1][54368]=50
		Out_Prize[1][54372]=50
		Out_Prize[1][54373]=50
		Out_Prize[1][54374]=50
		Out_Prize[1][54377]=30
		Out_Prize[1][54381]=30
		Out_Prize[1][54382]=30
		Out_Prize[1][54383]=30
		Out_Prize[1][54347]=20
		Out_Prize[1][54352]=20
		Out_Prize[1][54353]=20
		Out_Prize[1][54364]=20
		Out_Prize[1][54369]=20
		Out_Prize[1][54370]=20
		Out_Prize[1][54375]=20
		Out_Prize[1][54380]=20
		Out_Prize[1][54384]=20
		Out_Prize[1][54348]=10
		Out_Prize[1][54354]=10
		Out_Prize[1][54365]=10
		Out_Prize[1][54371]=10
		Out_Prize[1][54376]=10

	elseif id==54770 then  --辰皇土
		Out_Prize[1][54395]=220
		Out_Prize[1][54396]=220
		Out_Prize[1][54397]=220
		Out_Prize[1][54400]=220
		Out_Prize[1][54401]=220
		Out_Prize[1][54402]=110
		Out_Prize[1][54412]=110
		Out_Prize[1][54413]=110
		Out_Prize[1][54414]=110
		Out_Prize[1][54417]=110
		Out_Prize[1][54418]=50
		Out_Prize[1][54419]=50
		Out_Prize[1][54423]=50
		Out_Prize[1][54424]=50
		Out_Prize[1][54425]=50
		Out_Prize[1][54428]=30
		Out_Prize[1][54432]=30
		Out_Prize[1][54433]=30
		Out_Prize[1][54434]=30
		Out_Prize[1][54398]=20
		Out_Prize[1][54403]=20
		Out_Prize[1][54404]=20
		Out_Prize[1][54415]=20
		Out_Prize[1][54420]=20
		Out_Prize[1][54421]=20
		Out_Prize[1][54426]=20
		Out_Prize[1][54431]=20
		Out_Prize[1][54435]=20
		Out_Prize[1][54399]=10
		Out_Prize[1][54405]=10
		Out_Prize[1][54416]=10
		Out_Prize[1][54422]=10
		Out_Prize[1][54427]=10
		
	elseif id==62174 then  --牵机金
		Out_Prize[1][61502]=220
		Out_Prize[1][61503]=220
		Out_Prize[1][61504]=110
		Out_Prize[1][61505]=110
		Out_Prize[1][61506]=50
		Out_Prize[1][61507]=50
		Out_Prize[1][61508]=30
		Out_Prize[1][61509]=30
		Out_Prize[1][61510]=20
		Out_Prize[1][61511]=10
		Out_Prize[1][61512]=10
		Out_Prize[1][61513]=220
		Out_Prize[1][61514]=220
		Out_Prize[1][61515]=110
		Out_Prize[1][61516]=110
		Out_Prize[1][61517]=50
		Out_Prize[1][61518]=50
		Out_Prize[1][61519]=30
		Out_Prize[1][61520]=30
		Out_Prize[1][61521]=20
		Out_Prize[1][61522]=10
		Out_Prize[1][61523]=10
		Out_Prize[1][61524]=220
		Out_Prize[1][61525]=220
		Out_Prize[1][61526]=110
		Out_Prize[1][61527]=110
		Out_Prize[1][61528]=50
		Out_Prize[1][61529]=50
		Out_Prize[1][61530]=30
		Out_Prize[1][61531]=30
		Out_Prize[1][61532]=20
		Out_Prize[1][61533]=10
		Out_Prize[1][61534]=10

	elseif id==62175 then  --牵机木
		Out_Prize[1][61540]=220
		Out_Prize[1][61541]=220
		Out_Prize[1][61542]=110
		Out_Prize[1][61543]=110
		Out_Prize[1][61544]=50
		Out_Prize[1][61545]=50
		Out_Prize[1][61546]=30
		Out_Prize[1][61547]=30
		Out_Prize[1][61548]=20
		Out_Prize[1][61549]=10
		Out_Prize[1][61550]=10
		Out_Prize[1][61551]=220
		Out_Prize[1][61552]=220
		Out_Prize[1][61553]=110
		Out_Prize[1][61554]=110
		Out_Prize[1][61555]=50
		Out_Prize[1][61556]=50
		Out_Prize[1][61557]=30
		Out_Prize[1][61558]=30
		Out_Prize[1][61559]=20
		Out_Prize[1][61560]=10
		Out_Prize[1][61561]=10
		Out_Prize[1][61562]=220
		Out_Prize[1][61563]=220
		Out_Prize[1][61564]=110
		Out_Prize[1][61565]=110
		Out_Prize[1][61566]=50
		Out_Prize[1][61567]=50
		Out_Prize[1][61568]=30
		Out_Prize[1][61569]=30
		Out_Prize[1][61570]=20
		Out_Prize[1][61571]=10
		Out_Prize[1][61572]=10

	elseif id==62176 then  --牵机水
		Out_Prize[1][61578]=220
		Out_Prize[1][61579]=220
		Out_Prize[1][61580]=110
		Out_Prize[1][61581]=110
		Out_Prize[1][61582]=50
		Out_Prize[1][61583]=50
		Out_Prize[1][61584]=30
		Out_Prize[1][61585]=30
		Out_Prize[1][61586]=20
		Out_Prize[1][61587]=10
		Out_Prize[1][61588]=10
		Out_Prize[1][61589]=220
		Out_Prize[1][61590]=220
		Out_Prize[1][61591]=110
		Out_Prize[1][61592]=110
		Out_Prize[1][61593]=50
		Out_Prize[1][61594]=50
		Out_Prize[1][61595]=30
		Out_Prize[1][61596]=30
		Out_Prize[1][61597]=20
		Out_Prize[1][61598]=10
		Out_Prize[1][61599]=10
		Out_Prize[1][61600]=220
		Out_Prize[1][61601]=220
		Out_Prize[1][61602]=110
		Out_Prize[1][61603]=110
		Out_Prize[1][61604]=50
		Out_Prize[1][61605]=50
		Out_Prize[1][61606]=30
		Out_Prize[1][61607]=30
		Out_Prize[1][61608]=20
		Out_Prize[1][61609]=10
		Out_Prize[1][61610]=10
		
	elseif id==62177 then  --牵机火
		Out_Prize[1][61616]=220
		Out_Prize[1][61617]=220
		Out_Prize[1][61618]=110
		Out_Prize[1][61619]=110
		Out_Prize[1][61620]=50
		Out_Prize[1][61621]=50
		Out_Prize[1][61622]=30
		Out_Prize[1][61623]=30
		Out_Prize[1][61624]=20
		Out_Prize[1][61625]=10
		Out_Prize[1][61626]=10
		Out_Prize[1][61627]=220
		Out_Prize[1][61628]=220
		Out_Prize[1][61629]=110
		Out_Prize[1][61630]=110
		Out_Prize[1][61631]=50
		Out_Prize[1][61632]=50
		Out_Prize[1][61633]=30
		Out_Prize[1][61634]=30
		Out_Prize[1][61635]=20
		Out_Prize[1][61636]=10
		Out_Prize[1][61637]=10
		Out_Prize[1][61638]=220
		Out_Prize[1][61639]=220
		Out_Prize[1][61640]=110
		Out_Prize[1][61641]=110
		Out_Prize[1][61642]=50
		Out_Prize[1][61643]=50
		Out_Prize[1][61644]=30
		Out_Prize[1][61645]=30
		Out_Prize[1][61646]=20
		Out_Prize[1][61647]=10
		Out_Prize[1][61648]=10

	elseif id==62178 then  --牵机土
		Out_Prize[1][61654]=220
		Out_Prize[1][61655]=220
		Out_Prize[1][61656]=110
		Out_Prize[1][61657]=110
		Out_Prize[1][61658]=50
		Out_Prize[1][61659]=50
		Out_Prize[1][61660]=30
		Out_Prize[1][61661]=30
		Out_Prize[1][61662]=20
		Out_Prize[1][61663]=10
		Out_Prize[1][61664]=10
		Out_Prize[1][61665]=220
		Out_Prize[1][61666]=220
		Out_Prize[1][61667]=110
		Out_Prize[1][61668]=110
		Out_Prize[1][61669]=50
		Out_Prize[1][61670]=50
		Out_Prize[1][61671]=30
		Out_Prize[1][61672]=30
		Out_Prize[1][61673]=20
		Out_Prize[1][61674]=10
		Out_Prize[1][61675]=10
		Out_Prize[1][61676]=220
		Out_Prize[1][61677]=220
		Out_Prize[1][61678]=110
		Out_Prize[1][61679]=110
		Out_Prize[1][61680]=50
		Out_Prize[1][61681]=50
		Out_Prize[1][61682]=30
		Out_Prize[1][61683]=30
		Out_Prize[1][61684]=20
		Out_Prize[1][61685]=10
		Out_Prize[1][61686]=10
		
	elseif id==62179 then  --英招金
		Out_Prize[1][61695]=220
		Out_Prize[1][61698]=220
		Out_Prize[1][61709]=220
		Out_Prize[1][61715]=220
		Out_Prize[1][61720]=220
		Out_Prize[1][61729]=220
		Out_Prize[1][61732]=220
		Out_Prize[1][61692]=110
		Out_Prize[1][61699]=110
		Out_Prize[1][61710]=110
		Out_Prize[1][61716]=110
		Out_Prize[1][61721]=110
		Out_Prize[1][61726]=110
		Out_Prize[1][61733]=110
		Out_Prize[1][61696]=50
		Out_Prize[1][61700]=50
		Out_Prize[1][61711]=50
		Out_Prize[1][61717]=50
		Out_Prize[1][61730]=50
		Out_Prize[1][61734]=50
		Out_Prize[1][61693]=30
		Out_Prize[1][61697]=30
		Out_Prize[1][61701]=30
		Out_Prize[1][61718]=30
		Out_Prize[1][61722]=20
		Out_Prize[1][61727]=20
		Out_Prize[1][61731]=20
		Out_Prize[1][61735]=20
		Out_Prize[1][61694]=10
		Out_Prize[1][61702]=10
		Out_Prize[1][61725]=10
		Out_Prize[1][61728]=10
		Out_Prize[1][61736]=10
		
	elseif id==62180 then  --英招木
		Out_Prize[1][61746]=220
		Out_Prize[1][61749]=220
		Out_Prize[1][61760]=220
		Out_Prize[1][61766]=220
		Out_Prize[1][61771]=220
		Out_Prize[1][61780]=220
		Out_Prize[1][61783]=220
		Out_Prize[1][61743]=110
		Out_Prize[1][61750]=110
		Out_Prize[1][61761]=110
		Out_Prize[1][61767]=110
		Out_Prize[1][61772]=110
		Out_Prize[1][61777]=110
		Out_Prize[1][61784]=110
		Out_Prize[1][61747]=50
		Out_Prize[1][61751]=50
		Out_Prize[1][61762]=50
		Out_Prize[1][61768]=50
		Out_Prize[1][61781]=50
		Out_Prize[1][61785]=50
		Out_Prize[1][61744]=30
		Out_Prize[1][61748]=30
		Out_Prize[1][61752]=30
		Out_Prize[1][61769]=30
		Out_Prize[1][61773]=20
		Out_Prize[1][61778]=20
		Out_Prize[1][61782]=20
		Out_Prize[1][61786]=20
		Out_Prize[1][61745]=10
		Out_Prize[1][61753]=10
		Out_Prize[1][61776]=10
		Out_Prize[1][61779]=10
		Out_Prize[1][61787]=10
	
	elseif id==62181 then  --英招水
		Out_Prize[1][61797]=220
		Out_Prize[1][61800]=220
		Out_Prize[1][61811]=220
		Out_Prize[1][61817]=220
		Out_Prize[1][61822]=220
		Out_Prize[1][61831]=220
		Out_Prize[1][61834]=220
		Out_Prize[1][61794]=110
		Out_Prize[1][61801]=110
		Out_Prize[1][61812]=110
		Out_Prize[1][61818]=110
		Out_Prize[1][61823]=110
		Out_Prize[1][61828]=110
		Out_Prize[1][61835]=110
		Out_Prize[1][61798]=50
		Out_Prize[1][61802]=50
		Out_Prize[1][61813]=50
		Out_Prize[1][61819]=50
		Out_Prize[1][61832]=50
		Out_Prize[1][61836]=50
		Out_Prize[1][61795]=30
		Out_Prize[1][61799]=30
		Out_Prize[1][61803]=30
		Out_Prize[1][61820]=30
		Out_Prize[1][61824]=20
		Out_Prize[1][61829]=20
		Out_Prize[1][61833]=20
		Out_Prize[1][61837]=20
		Out_Prize[1][61796]=10
		Out_Prize[1][61804]=10
		Out_Prize[1][61827]=10
		Out_Prize[1][61830]=10
		Out_Prize[1][61838]=10

	elseif id==62182 then  --英招火
		Out_Prize[1][61848]=220
		Out_Prize[1][61851]=220
		Out_Prize[1][61862]=220
		Out_Prize[1][61868]=220
		Out_Prize[1][61873]=220
		Out_Prize[1][61882]=220
		Out_Prize[1][61885]=220
		Out_Prize[1][61845]=110
		Out_Prize[1][61852]=110
		Out_Prize[1][61863]=110
		Out_Prize[1][61869]=110
		Out_Prize[1][61874]=110
		Out_Prize[1][61879]=110
		Out_Prize[1][61886]=110
		Out_Prize[1][61849]=50
		Out_Prize[1][61853]=50
		Out_Prize[1][61864]=50
		Out_Prize[1][61870]=50
		Out_Prize[1][61883]=50
		Out_Prize[1][61887]=50
		Out_Prize[1][61846]=30
		Out_Prize[1][61850]=30
		Out_Prize[1][61854]=30
		Out_Prize[1][61871]=30
		Out_Prize[1][61875]=20
		Out_Prize[1][61880]=20
		Out_Prize[1][61884]=20
		Out_Prize[1][61888]=20
		Out_Prize[1][61847]=10
		Out_Prize[1][61855]=10
		Out_Prize[1][61878]=10
		Out_Prize[1][61881]=10
		Out_Prize[1][61889]=10

	elseif id==62183 then  --英招土
		Out_Prize[1][61899]=220
		Out_Prize[1][61902]=220
		Out_Prize[1][61913]=220
		Out_Prize[1][61919]=220
		Out_Prize[1][61924]=220
		Out_Prize[1][61933]=220
		Out_Prize[1][61936]=220
		Out_Prize[1][61896]=110
		Out_Prize[1][61903]=110
		Out_Prize[1][61914]=110
		Out_Prize[1][61920]=110
		Out_Prize[1][61925]=110
		Out_Prize[1][61930]=110
		Out_Prize[1][61937]=110
		Out_Prize[1][61900]=50
		Out_Prize[1][61904]=50
		Out_Prize[1][61915]=50
		Out_Prize[1][61921]=50
		Out_Prize[1][61934]=50
		Out_Prize[1][61938]=50
		Out_Prize[1][61897]=30
		Out_Prize[1][61901]=30
		Out_Prize[1][61905]=30
		Out_Prize[1][61922]=30
		Out_Prize[1][61926]=20
		Out_Prize[1][61931]=20
		Out_Prize[1][61935]=20
		Out_Prize[1][61939]=20
		Out_Prize[1][61898]=10
		Out_Prize[1][61906]=10
		Out_Prize[1][61929]=10
		Out_Prize[1][61932]=10
		Out_Prize[1][61940]=10

	elseif id == 62061 then
		Out_Prize[1][60541]=200
		Out_Prize[1][60543]=200
		Out_Prize[1][60545]=200
		Out_Prize[1][60547]=200
		Out_Prize[1][60549]=200
		Out_Prize[1][60551]=200
		Out_Prize[1][60553]=200
		Out_Prize[1][60555]=200
		Out_Prize[1][60557]=200
		Out_Prize[1][60559]=320
		Out_Prize[1][60561]=320
		Out_Prize[1][60563]=100
		Out_Prize[1][60565]=100
		Out_Prize[1][60567]=100
		Out_Prize[1][60569]=100
		Out_Prize[1][60571]=40
		Out_Prize[1][60573]=40
		Out_Prize[1][60575]=40
		Out_Prize[1][60577]=40
		Out_Prize[1][60579]=40
		Out_Prize[1][60581]=40
		
	elseif id == 62062 then
		Out_Prize[1][60542]=50
		Out_Prize[1][60544]=50
		Out_Prize[1][60546]=50
		Out_Prize[1][60548]=50
		Out_Prize[1][60550]=50
		Out_Prize[1][60552]=50
		Out_Prize[1][60554]=50
		Out_Prize[1][60556]=50
		Out_Prize[1][60558]=50
		Out_Prize[1][60560]=80
		Out_Prize[1][60562]=80
		Out_Prize[1][60564]=25
		Out_Prize[1][60566]=25
		Out_Prize[1][60568]=25
		Out_Prize[1][60570]=25
		Out_Prize[1][60572]=10
		Out_Prize[1][60574]=10
		Out_Prize[1][60576]=10
		Out_Prize[1][60578]=10
		Out_Prize[1][60580]=10
		Out_Prize[1][60582]=10
		
	elseif id == 62063 then
		Out_Prize[1][61424]=10
		Out_Prize[1][61425]=10
		Out_Prize[1][61426]=10
		Out_Prize[1][61427]=10
		Out_Prize[1][61428]=10
		Out_Prize[1][61429]=10
		Out_Prize[1][61431]=10
		Out_Prize[1][61432]=10
		Out_Prize[1][61433]=10
		Out_Prize[1][61434]=16
		Out_Prize[1][61435]=16
		Out_Prize[1][61436]=5
		Out_Prize[1][61437]=5
		Out_Prize[1][61438]=5
		Out_Prize[1][61439]=5
		Out_Prize[1][61440]=2
		Out_Prize[1][61441]=2
		Out_Prize[1][61442]=2
		Out_Prize[1][61443]=2
		Out_Prize[1][61444]=2
		Out_Prize[1][61445]=2		

	else
		Out_Prize[1][id]=100
	end 	
		
	local i,v,k
	
	for k=1,1 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,1 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	end
	return 1,0,id,1,0,0 
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType13593Cashing(cur_list)		---(月光宝盒)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][13592]=100		---玄武灵狐(坐骑兑换券)
	
	Out_Prize[2][10867]=100   ---阴阳镜

	Out_Prize[3][13343]=100   ---聚宝盆

	Out_Prize[4][8079]=100	---飞天神符
		
	local i,v,k
	
	for k=1,4 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,1)==0 and ZLottery_FindSameNum(cur_list,2)==0) or
	ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,1,0,0
	else 
		return 4,0,8079,2,0,0
	end
	return 4,0,8079,2,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType13905Cashing(cur_list)		---(狼图腾)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][13486]=100 ---霜狼
	
	Out_Prize[2][13904]=333 ---//高级天罗
	Out_Prize[2][13903]=667	---//高级建木
	
	Out_Prize[3][13901]=333	---//加Def药
	Out_Prize[3][13900]=667	---//加Atk药
	
	Out_Prize[4][8079]=100	---飞天神符
		
	local i,v,k
	
	for k=1,4 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,2,0,0
	else 
		return 4,0,8079,2,0,0
	end
	return 4,0,8079,2,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType15677Cashing(cur_list)		---(狮王之心)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][15672]=100		---仙狮
	
	Out_Prize[2][7931]=40			---结婚礼包男
	Out_Prize[2][7937]=60			---结婚礼包女	

	Out_Prize[3][15693]=140	---暗月兑换券
	Out_Prize[3][15694]=20	---赤日兑换券	
	
	Out_Prize[4][8079]=100	---飞天神符
		
	local i,v,k
	
	for k=1,4 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,1)==0 and ZLottery_FindSameNum(cur_list,2)==0) or
	ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		if prizeout==15694 then 
			return 3,0,prizeout,1,0,0
		else
			return 3,0,prizeout,2,0,0
		end 
	else 
		return 4,0,8079,2,0,0
	end
	return 4,0,8079,2,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType15866Cashing(cur_list)		---(雷光墨玉)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][15865]=100		---墨金骠骑
	
	Out_Prize[2][15868]=100		---落魂灯	

	Out_Prize[3][13343]=100		---聚宝盆
		
	Out_Prize[4][13901]=25		---加Def药
	Out_Prize[4][13900]=100		---加Atk药
		
	local i,v,k
	
	for k=1,4 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,1)==0 and ZLottery_FindSameNum(cur_list,2)==0) or
	(ZLottery_FindSameNum(cur_list,1)==1 and ZLottery_FindSameNum(cur_list,2)==1 and 
	ZLottery_FindSameNum(cur_list,3)==1) then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,10,0,0
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,1,0,0
	end
	return 4,0,13900,1,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType16550Cashing(cur_list)		---(逐鹿御令)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][16549]=100		---五色鹿
	
	Out_Prize[2][7931]=40			---结婚礼包男
	Out_Prize[2][7937]=60			---结婚礼包女

	Out_Prize[3][7939]=100		---天宝逆龙鳞
		
	Out_Prize[4][13901]=25		---加Def药
	Out_Prize[4][13900]=100		---加Atk药
		
	local i,v,k
	
	for k=1,4 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,1)==0 and ZLottery_FindSameNum(cur_list,2)==0) or
	(ZLottery_FindSameNum(cur_list,1)==1 and ZLottery_FindSameNum(cur_list,2)==1 and 
	ZLottery_FindSameNum(cur_list,3)==1) then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,3,0,0
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,1,0,0
	end
	return 4,0,13900,1,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType18283Cashing(cur_list)		---(麒麟金尊)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][18284]=100	---麒麟兑换券
	
	Out_Prize[2][13307]=80	---鬼面面具
	Out_Prize[2][13306]=20	---蝴蝶面饰	

	Out_Prize[3][15693]=50	---暗月兑换券
	Out_Prize[3][15694]=30	---赤日兑换券	
	
	Out_Prize[4][8079]=100	---飞天神符
		
	local i,v,k
	
	for k=1,4 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		if prizeout==15694 then 
			return 3,0,prizeout,1,0,0
		else
			return 3,0,prizeout,2,0,0
		end 
	else 
		return 4,0,8079,2,0,0
	end
	return 4,0,8079,2,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType18799Cashing(cur_list)		---(昆仑天晶)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][18801]=100		---冰原雪熊
	
	Out_Prize[2][18797]=100		---绝版法宝兑换券

	Out_Prize[3][15693]=140		---暗月兑换券
	Out_Prize[3][15694]=20		---赤日兑换券	
	
	Out_Prize[4][13901]=25		---加Def药
	Out_Prize[4][13900]=100		---加Atk药
		
	local i,v,k
	
	for k=1,4 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,1)==0 and ZLottery_FindSameNum(cur_list,2)==0) or
	ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		if prizeout==15694 then 
			return 3,0,prizeout,1,0,0
		else
			return 3,0,prizeout,2,0,0
		end 
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,1,0,0
	end
	return 4,0,13900,2,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType18890Cashing(cur_list)		---(行天圣宝)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][18891]=100		---龙骧兑换券
	
	Out_Prize[2][10872]=100		---五龙轮

	Out_Prize[3][18794]=100		---仙豆
	
	Out_Prize[4][8079]=100		---飞天神符
		
	local i,v,k
	
	for k=1,4 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,1)==0 and ZLottery_FindSameNum(cur_list,2)==0) or
	ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
			return 3,0,prizeout,3,0,0
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,2,0,0
	end
	return 4,0,8079,2,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryTypeItemSeason1Cashing(cur_list,id)		---(金币回收彩票)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	
	if id==19112 then
		Out_Prize[1][3449]={chance=100,num=1}---//魔噬
		Out_Prize[1][3459]={chance=100,num=1}---//圣护
		Out_Prize[1][3530]={chance=100,num=1}---//魔御
		Out_Prize[1][3540]={chance=100,num=1}---//魔吟
		Out_Prize[1][6151]={chance=100,num=1}---//裂云戒
		Out_Prize[1][6152]={chance=100,num=1}---//裂空戒
		Out_Prize[1][6153]={chance=100,num=1}---//裂魂戒
		Out_Prize[1][6154]={chance=100,num=1}---//荒雷戒
		Out_Prize[1][6155]={chance=100,num=1}---//凌云戒
		Out_Prize[1][6156]={chance=100,num=1}---//飞云戒
		Out_Prize[1][6157]={chance=100,num=1}---//雷云戒
		Out_Prize[1][6158]={chance=100,num=1}---//集英戒
		Out_Prize[1][6159]={chance=100,num=1}---//雷英戒
		Out_Prize[1][6160]={chance=100,num=1}---//空雷戒
		Out_Prize[1][6161]={chance=100,num=1}---//华光戒
		Out_Prize[1][6162]={chance=100,num=1}---//雷光戒
		Out_Prize[1][6163]={chance=100,num=1}---//极光戒
		Out_Prize[1][6164]={chance=100,num=1}---//断云戒
		Out_Prize[1][6165]={chance=100,num=1}---//涅云戒
		Out_Prize[1][6311]={chance=100,num=1}---//魔循
		Out_Prize[1][6331]={chance=100,num=1}---//魔锐
		Out_Prize[1][14377]={chance=100,num=1}---//陵延神盔（男）
		Out_Prize[1][14384]={chance=100,num=1}---//济灵冥盔（男）
		Out_Prize[1][14518]={chance=100,num=1}---//陵延神盔（女）
		Out_Prize[1][14525]={chance=100,num=1}---//济灵冥盔（女）
		Out_Prize[1][14750]={chance=100,num=1}---//陵延神甲（男）
		Out_Prize[1][14757]={chance=100,num=1}---//济灵冥甲（男）
		Out_Prize[1][14891]={chance=100,num=1}---//陵延神甲（女）
		Out_Prize[1][14898]={chance=100,num=1}---//济灵冥甲（女）
		Out_Prize[1][15032]={chance=100,num=1}---//陵延神靴（男）
		Out_Prize[1][15039]={chance=100,num=1}---//济灵冥靴（男）
		Out_Prize[1][15173]={chance=100,num=1}---//陵延神靴（女）
		Out_Prize[1][15180]={chance=100,num=1}---//济灵冥靴（女）
		Out_Prize[1][3008]={chance=100,num=1}---//荒冢战甲（男）
		Out_Prize[1][3015]={chance=100,num=1}---//闲歌灵衫（男）
		Out_Prize[1][3022]={chance=100,num=1}---//筑基道袍（男）
		Out_Prize[1][3029]={chance=100,num=1}---//宝树禅衣（男）
		Out_Prize[1][3036]={chance=100,num=1}---//荒冢战靴（男）
		Out_Prize[1][3043]={chance=100,num=1}---//闲歌灵靴（男）
		Out_Prize[1][3050]={chance=100,num=1}---//筑基道履（男）
		Out_Prize[1][3057]={chance=100,num=1}---//宝树禅履（男）
		Out_Prize[1][3064]={chance=100,num=1}---//荒冢战盔（男）
		Out_Prize[1][3071]={chance=100,num=1}---//闲歌灵盔（男）
		Out_Prize[1][3078]={chance=100,num=1}---//筑基道冠（男）
		Out_Prize[1][3085]={chance=100,num=1}---//宝树禅冠（男）
		Out_Prize[1][3092]={chance=100,num=1}---//荒冢战甲（女）
		Out_Prize[1][3099]={chance=100,num=1}---//闲歌灵衫（女）
		Out_Prize[1][3106]={chance=100,num=1}---//筑基道袍（女）
		Out_Prize[1][3113]={chance=100,num=1}---//宝树禅衣（女）
		Out_Prize[1][3120]={chance=100,num=1}---//荒冢战靴（女）
		Out_Prize[1][3127]={chance=100,num=1}---//闲歌灵靴（女）
		Out_Prize[1][3134]={chance=100,num=1}---//筑基道履（女）
		Out_Prize[1][3141]={chance=100,num=1}---//宝树禅履（女）
		Out_Prize[1][3148]={chance=100,num=1}---//荒冢战盔（女）
		Out_Prize[1][3155]={chance=100,num=1}---//闲歌灵盔（女）
		Out_Prize[1][3162]={chance=100,num=1}---//筑基道冠（女）
		Out_Prize[1][3169]={chance=100,num=1}---//宝树禅冠（女）
		Out_Prize[1][5180]={chance=100,num=1}---//杀欲魔盔（男）
		Out_Prize[1][5181]={chance=100,num=1}---//杀欲魔盔（女）
		Out_Prize[1][5208]={chance=100,num=1}---//雅意灵盔（男）
		Out_Prize[1][5209]={chance=100,num=1}---//雅意灵盔（女）
		Out_Prize[1][5236]={chance=100,num=1}---//修本道冠（男）
		Out_Prize[1][5237]={chance=100,num=1}---//修本道冠（女）
		Out_Prize[1][5264]={chance=100,num=1}---//明台禅冠（男）
		Out_Prize[1][5265]={chance=100,num=1}---//明台禅冠（女）
		Out_Prize[1][5316]={chance=100,num=1}---//杀欲魔靴（男）
		Out_Prize[1][5317]={chance=100,num=1}---//杀欲魔靴（女）
		Out_Prize[1][5344]={chance=100,num=1}---//雅意灵靴（男）
		Out_Prize[1][5345]={chance=100,num=1}---//雅意灵靴（女）
		Out_Prize[1][5372]={chance=100,num=1}---//修本道履（男）
		Out_Prize[1][5373]={chance=100,num=1}---//修本道履（女）
		Out_Prize[1][5400]={chance=100,num=1}---//明台禅履（男）
		Out_Prize[1][5401]={chance=100,num=1}---//明台禅履（女）
		Out_Prize[1][5428]={chance=100,num=1}---//杀欲魔甲（男）
		Out_Prize[1][5429]={chance=100,num=1}---//杀欲魔甲（女）
		Out_Prize[1][5456]={chance=100,num=1}---//雅意灵衫（男）
		Out_Prize[1][5457]={chance=100,num=1}---//雅意灵衫（女）
		Out_Prize[1][5484]={chance=100,num=1}---//修本道袍（男）
		Out_Prize[1][5485]={chance=100,num=1}---//修本道袍（女）
		Out_Prize[1][5512]={chance=100,num=1}---//明台禅衣（男）
		Out_Prize[1][5513]={chance=100,num=1}---//明台禅衣（女）
	elseif id==19113 then 
		Out_Prize[1][3450]={chance=100,num=1}---//龙威
		Out_Prize[1][3460]={chance=100,num=1}---//天佑
		Out_Prize[1][3531]={chance=100,num=1}---//龙胆
		Out_Prize[1][3541]={chance=100,num=1}---//龙心
		Out_Prize[1][6312]={chance=100,num=1}---//龙循
		Out_Prize[1][6332]={chance=100,num=1}---//龙锐
		Out_Prize[1][14378]={chance=100,num=1}---//碎魂天残盔（男）
		Out_Prize[1][14385]={chance=100,num=1}---//黑煞解尸盔（男）
		Out_Prize[1][14519]={chance=100,num=1}---//碎魂天残盔（女）
		Out_Prize[1][14526]={chance=100,num=1}---//黑煞解尸盔（女）
		Out_Prize[1][14751]={chance=100,num=1}---//碎魂天残甲（男）
		Out_Prize[1][14758]={chance=100,num=1}---//黑煞解尸甲（男）
		Out_Prize[1][14892]={chance=100,num=1}---//碎魂天残甲（女）
		Out_Prize[1][14899]={chance=100,num=1}---//黑煞解尸甲（女）
		Out_Prize[1][15033]={chance=100,num=1}---//碎魂天残靴（男）
		Out_Prize[1][15040]={chance=100,num=1}---//黑煞解尸靴（男）
		Out_Prize[1][15174]={chance=100,num=1}---//碎魂天残靴（女）
		Out_Prize[1][15181]={chance=100,num=1}---//黑煞解尸靴（女）
		Out_Prize[1][3009]={chance=100,num=1}---//魔尊戍魂铠（男）
		Out_Prize[1][3016]={chance=100,num=1}---//幻雾山岚衫（男）
		Out_Prize[1][3023]={chance=100,num=1}---//龙华矫天袍（男）
		Out_Prize[1][3030]={chance=100,num=1}---//寂灭功德袍（男）
		Out_Prize[1][3037]={chance=100,num=1}---//魔尊戍魂靴（男）
		Out_Prize[1][3044]={chance=100,num=1}---//幻雾山岚靴（男）
		Out_Prize[1][3051]={chance=100,num=1}---//龙华矫天履（男）
		Out_Prize[1][3058]={chance=100,num=1}---//寂灭功德履（男）
		Out_Prize[1][3065]={chance=100,num=1}---//魔尊戍魂盔（男）
		Out_Prize[1][3072]={chance=100,num=1}---//幻雾山岚盔（男）
		Out_Prize[1][3079]={chance=100,num=1}---//龙华矫天冠（男）
		Out_Prize[1][3086]={chance=100,num=1}---//寂灭功德冠（男）
		Out_Prize[1][3093]={chance=100,num=1}---//魔尊戍魂铠（女）
		Out_Prize[1][3100]={chance=100,num=1}---//幻雾山岚衫（女）
		Out_Prize[1][3107]={chance=100,num=1}---//龙华矫天袍（女）
		Out_Prize[1][3114]={chance=100,num=1}---//寂灭功德袍（女）
		Out_Prize[1][3121]={chance=100,num=1}---//魔尊戍魂靴（女）
		Out_Prize[1][3128]={chance=100,num=1}---//幻雾山岚靴（女）
		Out_Prize[1][3135]={chance=100,num=1}---//龙华矫天履（女）
		Out_Prize[1][3142]={chance=100,num=1}---//寂灭功德履（女）
		Out_Prize[1][3149]={chance=100,num=1}---//魔尊戍魂盔（女）
		Out_Prize[1][3156]={chance=100,num=1}---//幻雾山岚盔（女）
		Out_Prize[1][3163]={chance=100,num=1}---//龙华矫天冠（女）
		Out_Prize[1][3170]={chance=100,num=1}---//寂灭功德冠（女）
		Out_Prize[1][8278]={chance=100,num=1}---//浴血龙威盔（男）
		Out_Prize[1][8279]={chance=100,num=1}---//浴血龙威盔（女）
		Out_Prize[1][8390]={chance=100,num=1}---//离人挽歌盔（男）
		Out_Prize[1][8391]={chance=100,num=1}---//离人挽歌盔（女）
		Out_Prize[1][8510]={chance=100,num=1}---//定心云锦冠（男）
		Out_Prize[1][8512]={chance=100,num=1}---//定心云锦冠（女）
		Out_Prize[1][8648]={chance=100,num=1}---//宝树聚元冠（男）
		Out_Prize[1][8649]={chance=100,num=1}---//宝树聚元冠（女）
		Out_Prize[1][8856]={chance=100,num=1}---//浴血龙威靴（男）
		Out_Prize[1][8857]={chance=100,num=1}---//浴血龙威靴（女）
		Out_Prize[1][8968]={chance=100,num=1}---//离人挽歌靴（男）
		Out_Prize[1][8969]={chance=100,num=1}---//离人挽歌靴（女）
		Out_Prize[1][9080]={chance=100,num=1}---//定心云锦履（男）
		Out_Prize[1][9081]={chance=100,num=1}---//定心云锦履（女）
		Out_Prize[1][9192]={chance=100,num=1}---//宝树聚元履（男）
		Out_Prize[1][9193]={chance=100,num=1}---//宝树聚元履（女）
		Out_Prize[1][9304]={chance=100,num=1}---//浴血龙威甲（男）
		Out_Prize[1][9305]={chance=100,num=1}---//浴血龙威甲（女）
		Out_Prize[1][9416]={chance=100,num=1}---//离人挽歌衫（男）
		Out_Prize[1][9417]={chance=100,num=1}---//离人挽歌衫（女）
		Out_Prize[1][9528]={chance=100,num=1}---//定心云锦袍（男）
		Out_Prize[1][9529]={chance=100,num=1}---//定心云锦袍（女）
		Out_Prize[1][9640]={chance=100,num=1}---//宝树聚元袍（男）
		Out_Prize[1][9641]={chance=100,num=1}---//宝树聚元袍（女）
	elseif id==19114 then
		Out_Prize[1][3451]={chance=100,num=1}---//仙恕
		Out_Prize[1][3461]={chance=100,num=1}---//清虚
		Out_Prize[1][3532]={chance=100,num=1}---//仙助
		Out_Prize[1][3542]={chance=100,num=1}---//仙灵
		Out_Prize[1][6313]={chance=100,num=1}---//仙循
		Out_Prize[1][6333]={chance=100,num=1}---//仙锐
		Out_Prize[1][10926]={chance=100,num=1}---//破云戒
		Out_Prize[1][10927]={chance=100,num=1}---//破空戒
		Out_Prize[1][10928]={chance=100,num=1}---//破魂戒
		Out_Prize[1][10929]={chance=100,num=1}---//狂雷戒
		Out_Prize[1][10930]={chance=100,num=1}---//荡云戒
		Out_Prize[1][10931]={chance=100,num=1}---//流云戒
		Out_Prize[1][10932]={chance=100,num=1}---//定云戒
		Out_Prize[1][10933]={chance=100,num=1}---//聚英戒
		Out_Prize[1][10934]={chance=100,num=1}---//飞英戒
		Out_Prize[1][10935]={chance=100,num=1}---//幻雷戒
		Out_Prize[1][10936]={chance=100,num=1}---//烁光戒
		Out_Prize[1][10937]={chance=100,num=1}---//盈光戒
		Out_Prize[1][10938]={chance=100,num=1}---//绚光戒
		Out_Prize[1][10939]={chance=100,num=1}---//朔云戒
		Out_Prize[1][10940]={chance=100,num=1}---//奠云戒
		Out_Prize[1][14379]={chance=100,num=1}---//九曜定心盔（男）
		Out_Prize[1][14386]={chance=100,num=1}---//九蛊逆刃盔（男）
		Out_Prize[1][14520]={chance=100,num=1}---//九曜定心盔（女）
		Out_Prize[1][14527]={chance=100,num=1}---//九蛊逆刃盔（女）
		Out_Prize[1][14752]={chance=100,num=1}---//九曜定心甲（男）
		Out_Prize[1][14759]={chance=100,num=1}---//九蛊逆刃甲（男）
		Out_Prize[1][14893]={chance=100,num=1}---//九曜定心甲（女）
		Out_Prize[1][14900]={chance=100,num=1}---//九蛊逆刃甲（女）
		Out_Prize[1][15034]={chance=100,num=1}---//九曜定心靴（男）
		Out_Prize[1][15041]={chance=100,num=1}---//九蛊逆刃靴（男）
		Out_Prize[1][15175]={chance=100,num=1}---//九曜定心靴（女）
		Out_Prize[1][15182]={chance=100,num=1}---//九蛊逆刃靴（女）
		Out_Prize[1][3010]={chance=100,num=1}---//九转焚心铠（男）
		Out_Prize[1][3017]={chance=100,num=1}---//九华惑心铠（男）
		Out_Prize[1][3024]={chance=100,num=1}---//九阳洞仙袍（男）
		Out_Prize[1][3031]={chance=100,num=1}---//九尊华严袍（男）
		Out_Prize[1][3038]={chance=100,num=1}---//九转焚心靴（男）
		Out_Prize[1][3045]={chance=100,num=1}---//九华惑心靴（男）
		Out_Prize[1][3052]={chance=100,num=1}---//九阳洞仙履（男）
		Out_Prize[1][3059]={chance=100,num=1}---//九尊华严履（男）
		Out_Prize[1][3066]={chance=100,num=1}---//九转焚心盔（男）
		Out_Prize[1][3073]={chance=100,num=1}---//九华惑心盔（男）
		Out_Prize[1][3080]={chance=100,num=1}---//九阳洞仙冠（男）
		Out_Prize[1][3087]={chance=100,num=1}---//九尊华严冠（男）
		Out_Prize[1][3094]={chance=100,num=1}---//九转焚心铠（女）
		Out_Prize[1][3101]={chance=100,num=1}---//九华惑心铠（女）
		Out_Prize[1][3108]={chance=100,num=1}---//九阳洞仙袍（女）
		Out_Prize[1][3115]={chance=100,num=1}---//九尊华严袍（女）
		Out_Prize[1][3122]={chance=100,num=1}---//九转焚心靴（女）
		Out_Prize[1][3129]={chance=100,num=1}---//九华惑心靴（女）
		Out_Prize[1][3136]={chance=100,num=1}---//九阳洞仙履（女）
		Out_Prize[1][3143]={chance=100,num=1}---//九尊华严履（女）
		Out_Prize[1][3150]={chance=100,num=1}---//九转焚心盔（女）
		Out_Prize[1][3157]={chance=100,num=1}---//九华惑心盔（女）
		Out_Prize[1][3164]={chance=100,num=1}---//九阳洞仙冠（女）
		Out_Prize[1][3171]={chance=100,num=1}---//九尊华严冠（女）
		Out_Prize[1][8306]={chance=100,num=1}---//九兽赤鳞盔（男）
		Out_Prize[1][8307]={chance=100,num=1}---//九兽赤鳞盔（女）
		Out_Prize[1][8418]={chance=100,num=1}---//九焕金纱盔（男）
		Out_Prize[1][8419]={chance=100,num=1}---//九焕金纱盔（女）
		Out_Prize[1][8561]={chance=100,num=1}---//九英聚元冠（男）
		Out_Prize[1][8562]={chance=100,num=1}---//九英聚元冠（女）
		Out_Prize[1][8676]={chance=100,num=1}---//九妙慈航冠（男）
		Out_Prize[1][8677]={chance=100,num=1}---//九妙慈航冠（女）
		Out_Prize[1][8884]={chance=100,num=1}---//九兽赤鳞靴（男）
		Out_Prize[1][8885]={chance=100,num=1}---//九兽赤鳞靴（女）
		Out_Prize[1][8996]={chance=100,num=1}---//九焕金纱靴（男）
		Out_Prize[1][8997]={chance=100,num=1}---//九焕金纱靴（女）
		Out_Prize[1][9108]={chance=100,num=1}---//九英聚元履（男）
		Out_Prize[1][9109]={chance=100,num=1}---//九英聚元履（女）
		Out_Prize[1][9220]={chance=100,num=1}---//九妙慈航履（男）
		Out_Prize[1][9221]={chance=100,num=1}---//九妙慈航履（女）
		Out_Prize[1][9332]={chance=100,num=1}---//九兽赤鳞铠（男）
		Out_Prize[1][9333]={chance=100,num=1}---//九兽赤鳞铠（女）
		Out_Prize[1][9444]={chance=100,num=1}---//九焕金纱铠（男）
		Out_Prize[1][9445]={chance=100,num=1}---//九焕金纱铠（女）
		Out_Prize[1][9556]={chance=100,num=1}---//九英聚元袍（男）
		Out_Prize[1][9557]={chance=100,num=1}---//九英聚元袍（女）
		Out_Prize[1][9668]={chance=100,num=1}---//九妙慈航袍（男）
		Out_Prize[1][9669]={chance=100,num=1}---//九妙慈航袍（女）
	elseif id==19115 then 
		Out_Prize[1][3452]={chance=100,num=1}---//神威
		Out_Prize[1][3462]={chance=100,num=1}---//太虚
		Out_Prize[1][3533]={chance=100,num=1}---//神迹
		Out_Prize[1][3543]={chance=100,num=1}---//神喻
		Out_Prize[1][6314]={chance=100,num=1}---//神循
		Out_Prize[1][6334]={chance=100,num=1}---//神锐
		Out_Prize[1][14380]={chance=100,num=1}---//战天狂灵魔神盔（男）
		Out_Prize[1][14387]={chance=100,num=1}---//幽冥转魂天祭盔（男）
		Out_Prize[1][14521]={chance=100,num=1}---//战天狂灵魔神盔（女）
		Out_Prize[1][14528]={chance=100,num=1}---//幽冥转魂天祭盔（女）
		Out_Prize[1][14753]={chance=100,num=1}---//战天狂灵魔神甲（男）
		Out_Prize[1][14760]={chance=100,num=1}---//幽冥转魂天祭甲（男）
		Out_Prize[1][14894]={chance=100,num=1}---//战天狂灵魔神甲（女）
		Out_Prize[1][14901]={chance=100,num=1}---//幽冥转魂天祭甲（女）
		Out_Prize[1][15035]={chance=100,num=1}---//战天狂灵魔神靴（男）
		Out_Prize[1][15042]={chance=100,num=1}---//幽冥转魂天祭靴（男）
		Out_Prize[1][15176]={chance=100,num=1}---//战天狂灵魔神靴（女）
		Out_Prize[1][15183]={chance=100,num=1}---//幽冥转魂天祭靴（女）
		Out_Prize[1][3011]={chance=100,num=1}---//离魂鬼契克劫铠（男）
		Out_Prize[1][3018]={chance=100,num=1}---//远峦接天离愁衫（男）
		Out_Prize[1][3025]={chance=100,num=1}---//云门天官锦罗袍（男）
		Out_Prize[1][3032]={chance=100,num=1}---//金山觉意佛尊袍（男）
		Out_Prize[1][3039]={chance=100,num=1}---//离魂鬼契克劫靴（男）
		Out_Prize[1][3046]={chance=100,num=1}---//远峦接天离愁靴（男）
		Out_Prize[1][3053]={chance=100,num=1}---//云门天官锦罗履（男）
		Out_Prize[1][3060]={chance=100,num=1}---//金山觉意佛尊履（男）
		Out_Prize[1][3067]={chance=100,num=1}---//离魂鬼契克劫盔（男）
		Out_Prize[1][3074]={chance=100,num=1}---//远峦接天离愁盔（男）
		Out_Prize[1][3081]={chance=100,num=1}---//云门天官锦罗冠（男）
		Out_Prize[1][3088]={chance=100,num=1}---//金山觉意佛尊冠（男）
		Out_Prize[1][3095]={chance=100,num=1}---//离魂鬼契克劫铠（女）
		Out_Prize[1][3102]={chance=100,num=1}---//远峦接天离愁衫（女）
		Out_Prize[1][3109]={chance=100,num=1}---//云门天官锦罗袍（女）
		Out_Prize[1][3116]={chance=100,num=1}---//金山觉意佛尊袍（女）
		Out_Prize[1][3123]={chance=100,num=1}---//离魂鬼契克劫靴（女）
		Out_Prize[1][3130]={chance=100,num=1}---//远峦接天离愁靴（女）
		Out_Prize[1][3137]={chance=100,num=1}---//云门天官锦罗履（女）
		Out_Prize[1][3144]={chance=100,num=1}---//金山觉意佛尊履（女）
		Out_Prize[1][3151]={chance=100,num=1}---//离魂鬼契克劫盔（女）
		Out_Prize[1][3158]={chance=100,num=1}---//远峦接天离愁盔（女）
		Out_Prize[1][3165]={chance=100,num=1}---//云门天官锦罗冠（女）
		Out_Prize[1][3172]={chance=100,num=1}---//金山觉意佛尊冠（女）
		Out_Prize[1][8334]={chance=100,num=1}---//明尊天佑邪灵盔（男）
		Out_Prize[1][8335]={chance=100,num=1}---//明尊天佑邪灵盔（女）
		Out_Prize[1][8446]={chance=100,num=1}---//琼台天籁承欢盔（男）
		Out_Prize[1][8447]={chance=100,num=1}---//琼台天籁承欢盔（女）
		Out_Prize[1][8592]={chance=100,num=1}---//积元贮本弥金冠（男）
		Out_Prize[1][8593]={chance=100,num=1}---//积元贮本弥金冠（女）
		Out_Prize[1][8704]={chance=100,num=1}---//摩诃刹利诸天冠（男）
		Out_Prize[1][8705]={chance=100,num=1}---//摩诃刹利诸天冠（女）
		Out_Prize[1][8912]={chance=100,num=1}---//明尊天佑邪灵靴（男）
		Out_Prize[1][8913]={chance=100,num=1}---//明尊天佑邪灵靴（女）
		Out_Prize[1][9024]={chance=100,num=1}---//琼台天籁承欢靴（男）
		Out_Prize[1][9025]={chance=100,num=1}---//琼台天籁承欢靴（女）
		Out_Prize[1][9136]={chance=100,num=1}---//积元贮本弥金履（男）
		Out_Prize[1][9137]={chance=100,num=1}---//积元贮本弥金履（女）
		Out_Prize[1][9248]={chance=100,num=1}---//摩诃刹利诸天履（男）
		Out_Prize[1][9249]={chance=100,num=1}---//摩诃刹利诸天履（女）
		Out_Prize[1][9360]={chance=100,num=1}---//明尊天佑邪灵铠（男）
		Out_Prize[1][9361]={chance=100,num=1}---//明尊天佑邪灵铠（女）
		Out_Prize[1][9472]={chance=100,num=1}---//琼台天籁承欢衫（男）
		Out_Prize[1][9473]={chance=100,num=1}---//琼台天籁承欢衫（女）
		Out_Prize[1][9584]={chance=100,num=1}---//积元贮本弥金袍（男）
		Out_Prize[1][9585]={chance=100,num=1}---//积元贮本弥金袍（女）
		Out_Prize[1][9696]={chance=100,num=1}---//摩诃刹利诸天袍（男）
		Out_Prize[1][9697]={chance=100,num=1}---//摩诃刹利诸天袍（女）
		Out_Prize[1][6315]={chance=100,num=1}---//天循(可强化戒指1)
		Out_Prize[1][6305]={chance=100,num=1}---//天戍(可强化戒指2)
		Out_Prize[1][3463]={chance=100,num=1}---//玉虚(可强化戒指3)
		Out_Prize[1][10957]={chance=100,num=1}---//天道戒(可强化戒指4)
	end
 
	local i,v
	lot_prize[1]={}
	lot_prizechance[1]={}
	for i,v in pairs(Out_Prize[1]) do 
			table.insert(lot_prize[1],i)
			table.insert(lot_prizechance[1],v.chance)
	end 
--	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,Out_Prize[1][prizeout].num,0,0
--	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryTypeGoldSeason1Cashing(cur_list,id)	---(虚拟币兑换物品)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	local prizenum=1
	Out_Prize[1]={}
	
	if id==19079 then  
		Out_Prize[1][14062]=200 
		prizenum=5
	elseif id==19081 then 
		Out_Prize[1][14062]=200 
		prizenum=10
	elseif id==19082 then 
		Out_Prize[1][14062]=200 
		prizenum=20
	elseif id ==19087 then 
		Out_Prize[1][14062]=200 
		prizenum=40
	else
		Out_Prize[1][id]=100
	end 	
		
	local i,v,k
	
	for k=1,1 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,1 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,prizenum,0,0
	end
	return 1,0,id,1,0,0 
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType19125Cashing(cur_list)		---(神龙秘笈)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][19132]=100		---熊猫panda
	
	Out_Prize[2][19127]=100		---龙火面具(男)
	Out_Prize[2][19128]=200		---鸟羽面具(女)
	
	Out_Prize[3][13343]=140		---聚宝盆
	
	Out_Prize[4][8079]=25		---飞天神符
		
	local i,v,k
	
	for k=1,4 do
	lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,1)==0 and ZLottery_FindSameNum(cur_list,2)==0) or
	ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,10,0,0
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,2,0,0
	end
	return 4,0,8079,2,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType19297Cashing(cur_list)		---(云光天鉴)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][19298]=100		---云壤之华(云梭兑换券)
	
	Out_Prize[2][16506]=100		---宠物：魅心蝶精

	Out_Prize[3][18794]=100		---仙豆
	
	Out_Prize[4][8079]=100			---飞天神符
		
	local i,v,k
	
	for k=1,4 do
	lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,1)==0 and ZLottery_FindSameNum(cur_list,2)==0) or
	ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,3,0,0
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,2,0,0
	end
	return 4,0,8079,2,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType19434Cashing(cur_list)		---(票彩鱼金/龙宫锦图)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][19435]=100		---锦鳞(金鱼兑换券)
	
	Out_Prize[2][19129]=100		---毒蝎面具(男)
	Out_Prize[2][19130]=200		---花瓣面具(女)

	Out_Prize[3][18794]=100		---仙豆
	
	Out_Prize[4][13901]=25		---加Def药
	Out_Prize[4][13900]=100		---加Atk药
		
	local i,v,k
	for k=1,4 do
	lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,1)==0 and ZLottery_FindSameNum(cur_list,2)==0) or
	ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,3,0,0
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,1,0,0
	end
	return 4,0,13900,1,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType19558Cashing(cur_list)		---(如意金鼓/朔望彤云)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][19559]=100		---玉如意(飞剑兑换券)
	
	Out_Prize[2][18306]=100		---缥缈之影(宠物)

	Out_Prize[3][16551]=100		---集宝盆
	Out_Prize[3][19418]=900		---灵宝秘尘
	
	Out_Prize[4][8079]=100		---飞天神符

	local i,v,k
	for k=1,4 do
	lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,1)==0 and ZLottery_FindSameNum(cur_list,2)==0) or
	ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		if prizeout==16551 then 
			return 3,0,16551,1,0,0
		else
			return 3,0,19418,100,0,0
		end 
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,2,0,0
	end
	return 4,0,8079,2,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryTypeGoldSeason2Cashing(cur_list,id)		---(虚拟币回收彩票/宠物技能书)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	
	if id==19425 then
		Out_Prize[1][19356]={chance=100,num=1}---//守护<日曜>
		Out_Prize[1][19357]={chance=100,num=1}---//热忱<日曜>
		Out_Prize[1][19358]={chance=100,num=1}---//闪耀<日曜>
		Out_Prize[1][19359]={chance=100,num=1}---//光辉<日曜>
		Out_Prize[1][19360]={chance=100,num=1}---//神聚<日曜>
		Out_Prize[1][19361]={chance=100,num=1}---//中天<日曜>
	elseif id==19426 then 
		Out_Prize[1][19362]={chance=100,num=1}---//新月<月曜>
		Out_Prize[1][19363]={chance=100,num=1}---//会神<月曜>
		Out_Prize[1][19364]={chance=100,num=1}---//贯注<月曜>
		Out_Prize[1][19365]={chance=100,num=1}---//流星<月曜>
		Out_Prize[1][19366]={chance=100,num=1}---//月影<月曜>
		Out_Prize[1][19367]={chance=100,num=1}---//水月<月曜>
	elseif id==19427 then 
		Out_Prize[1][19368]={chance=100,num=1}---//芒刺<金曜>
		Out_Prize[1][19369]={chance=100,num=1}---//怒嚎<金曜>
		Out_Prize[1][19370]={chance=100,num=1}---//全力<金曜>
		Out_Prize[1][19371]={chance=100,num=1}---//撕裂<金曜>
		Out_Prize[1][19372]={chance=100,num=1}---//刺耳<金曜>
		Out_Prize[1][19373]={chance=100,num=1}---//挑衅<金曜>
	elseif id==19428 then 
		Out_Prize[1][19374]={chance=100,num=1}---//曜灵<木曜>
		Out_Prize[1][19375]={chance=100,num=1}---//林荫<木曜>
		Out_Prize[1][19376]={chance=100,num=1}---//猛击<木曜>
		Out_Prize[1][19377]={chance=100,num=1}---//明灭<木曜>
		Out_Prize[1][19378]={chance=100,num=1}---//羁绊<木曜>
		Out_Prize[1][19379]={chance=100,num=1}---//疲惫<木曜>
	elseif id==19429 then 
		Out_Prize[1][19380]={chance=100,num=1}---//甘泉<水曜>
		Out_Prize[1][19381]={chance=100,num=1}---//祷告<水曜>
		Out_Prize[1][19382]={chance=100,num=1}---//预言<水曜>
		Out_Prize[1][19383]={chance=100,num=1}---//波纹<水曜>
		Out_Prize[1][19384]={chance=100,num=1}---//涟漪<水曜>
		Out_Prize[1][19385]={chance=100,num=1}---//思虑<水曜>
	elseif id==19430 then 
		Out_Prize[1][19386]={chance=100,num=1}---//火甲<火曜>
		Out_Prize[1][19387]={chance=100,num=1}---//贯通<火曜>
		Out_Prize[1][19388]={chance=100,num=1}---//酝酿<火曜>
		Out_Prize[1][19389]={chance=100,num=1}---//灼热<火曜>
		Out_Prize[1][19390]={chance=100,num=1}---//真火<火曜>
		Out_Prize[1][19391]={chance=100,num=1}---//燃尽<火曜>
	elseif id==19431 then 
		Out_Prize[1][19392]={chance=100,num=1}---//地祝<土曜>
		Out_Prize[1][19393]={chance=100,num=1}---//洞察<土曜>
		Out_Prize[1][19394]={chance=100,num=1}---//伤痕<土曜>
		Out_Prize[1][19395]={chance=100,num=1}---//追迹<土曜>
		Out_Prize[1][19396]={chance=100,num=1}---//沉沦<土曜>
		Out_Prize[1][19397]={chance=100,num=1}---//陷地<土曜>
	elseif id==19432 then 
		Out_Prize[1][19398]={chance=100,num=1}---//妄言<罗候>
		Out_Prize[1][19399]={chance=100,num=1}---//黯淡<罗候>
		Out_Prize[1][19400]={chance=100,num=1}---//宿命<罗候>
		Out_Prize[1][19401]={chance=100,num=1}---//禁锢<罗候>
		Out_Prize[1][19402]={chance=100,num=1}---//喝言<罗候>
		Out_Prize[1][19403]={chance=100,num=1}---//苦痛<罗候>
	elseif id==19433 then 
		Out_Prize[1][19404]={chance=100,num=1}---//闪避<计都>
		Out_Prize[1][19405]={chance=100,num=1}---//藐视<计都>
		Out_Prize[1][19406]={chance=100,num=1}---//锋利<计都>
		Out_Prize[1][19407]={chance=100,num=1}---//厄运<计都>
		Out_Prize[1][19408]={chance=100,num=1}---//忏悔<计都>
		Out_Prize[1][19409]={chance=100,num=1}---//迷茫<计都>
	end
		Out_Prize[1][19410]={chance=100,num=1}---//沸腾<通用>
		Out_Prize[1][19411]={chance=100,num=1}---//凝华<通用>
		Out_Prize[1][19412]={chance=100,num=1}---//挥洒<通用>
		Out_Prize[1][19413]={chance=100,num=1}---//蛮力<通用>
		Out_Prize[1][19414]={chance=100,num=1}---//修整<通用>
		Out_Prize[1][19415]={chance=100,num=1}---//汲取<通用>
	
	local i,v
	lot_prize[1]={}
	lot_prizechance[1]={}
	for i,v in pairs(Out_Prize[1]) do 
			table.insert(lot_prize[1],i)
			table.insert(lot_prizechance[1],v.chance)
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,Out_Prize[1][prizeout].num,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType19713Cashing(cur_list)		---(炎蹄之心)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][19714]=100		---妖马兑换券
	
	Out_Prize[2][7931]=40			---结婚礼包男
	Out_Prize[2][7937]=60			---结婚礼包女

	Out_Prize[3][18794]=100		---仙豆
	
	Out_Prize[4][8079]=100		---飞天神符
	local i,v,k
	for k=1,4 do
	lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,1)==0 and ZLottery_FindSameNum(cur_list,2)==0) or
	ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,3,0,0
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,2,0,0
	end
	return 4,0,8079,2,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType20097Cashing(cur_list)		---(08国情虚拟币礼包)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][20096]=100		---新暴击项链
	
	Out_Prize[2][6315]=200			---100级躲闪戒指
	Out_Prize[2][6305]=200			---100级防御戒指
	Out_Prize[2][3463]=200			---100级减伤戒指
	Out_Prize[2][10957]=100			---100级全抗戒指

	Out_Prize[3][16551]=100			---集宝盆
	Out_Prize[3][13301]=60			---五行珠1
	Out_Prize[3][13302]=60			---五行珠2
	Out_Prize[3][13303]=60			---五行珠3
	Out_Prize[3][13304]=60			---五行珠4
	Out_Prize[3][13305]=60			---五行珠5
	
	Out_Prize[4][20098]=100			---功德香兑换券
	Out_Prize[4][20099]=100			---乾坤通宝兑换券
	
	local i,v,k
	for k=1,4 do
	lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,1)==0 and ZLottery_FindSameNum(cur_list,2)==0) or
	ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		if prizeout==16551 then 
			return 3,0,prizeout,1,0,0
		else
			return 3,0,prizeout,5,0,0
		end 	
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,1,0,0
	end
	return 4,0,20098,1,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType20109Cashing(cur_list)		---(鸣珏瑰玉)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][20110]=100		---飞剑：碧落兑换券
	
	Out_Prize[2][20090]=40		---法宝：混元金斗

	Out_Prize[3][18794]=100		---仙豆
	
	Out_Prize[4][8079]=100		---飞天神符
	local i,v,k
	for k=1,4 do
	lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,1)==0 and ZLottery_FindSameNum(cur_list,2)==0) or
	ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,3,0,0
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,2,0,0
	end
	return 4,0,8079,2,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType20284Cashing(cur_list)		---(苍穹光印)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][20283]=100		---飞剑：鸿飞兑换券
	
	Out_Prize[2][20108]=100		---法宝：彩翼黄鸟
	Out_Prize[2][19510]=100		---宠物：妙翅黄鸟
	
	Out_Prize[3][18794]=500		---仙豆
	Out_Prize[3][7939]=100		---天宝逆龙鳞
	Out_Prize[3][19257]=200		---麒麟内丹
	
	Out_Prize[4][8079]=100		---飞天神符
	
	local i,v,k
	for k=1,4 do
	lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,1)==0 and ZLottery_FindSameNum(cur_list,2)==0) or
	ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		if prizeout==19257 then 
			return 3,0,prizeout,1,0,0
		else
			return 3,0,prizeout,3,0,0
		end 
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,2,0,0
	end
	return 4,0,8079,2,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType20480Cashing(cur_list)		---(孔雀令)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][20486]=100		---孔雀兑换券

	Out_Prize[2][7931]=40			---结婚礼包男
	Out_Prize[2][7937]=60			---结婚礼包女

	Out_Prize[3][18794]=100		---仙豆

	Out_Prize[4][13901]=25		---加Def药
	Out_Prize[4][13900]=100		---加Atk药

	local i,v,k
	for k=1,4 do
	lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,1)==0 and ZLottery_FindSameNum(cur_list,2)==0)
			or ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,3,0,0
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,1,0,0
	end
	return 4,0,13900,1,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType20482Cashing(cur_list)		---(龙皇古卷)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][20487]=100 ---//飞剑紫芒兑换券
	
	Out_Prize[2][18797]=100 ---//绝版法宝兑换券
	
	Out_Prize[3][13343]=100	---//聚宝盆
	
	Out_Prize[4][8079]=100	---//飞天神符
		
	local i,v,k
	
	for k=1,4 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,10,0,0
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,2,0,0
	end
	return 4,0,8079,2,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType20460Cashing(cur_list)		---(命运轮盘)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 

	if nowcurlist[fmax_count]=="11" then 
		return 1,0,19277,1,0,0
	elseif nowcurlist[fmax_count]=="12" then
		return 4,0,19278,1,0,0
	elseif nowcurlist[fmax_count]=="13" then
		return 3,0,19279,1,0,0
	elseif nowcurlist[fmax_count]=="14" then
		return 4,0,19280,1,0,0
	elseif nowcurlist[fmax_count]=="21" then
		return 2,0,19281,1,0,0
	elseif nowcurlist[fmax_count]=="22" then
		return 4,0,19282,1,0,0
	elseif nowcurlist[fmax_count]=="23" then
		return 3,0,19283,1,0,0
	elseif nowcurlist[fmax_count]=="24" then
		return 4,0,19284,1,0,0
	elseif nowcurlist[fmax_count]=="31" then
		return 2,0,19285,1,0,0
	elseif nowcurlist[fmax_count]=="32" then
		return 4,0,19286,1,0,0
	elseif nowcurlist[fmax_count]=="33" then
		return 3,0,19277,1,0,0
	elseif nowcurlist[fmax_count]=="34" then
		return 4,0,19278,1,0,0
	elseif nowcurlist[fmax_count]=="41" then
		return 2,0,19279,1,0,0
	elseif nowcurlist[fmax_count]=="42" then
		return 4,0,19280,1,0,0
	elseif nowcurlist[fmax_count]=="43" then
		return 3,0,19281,1,0,0
	else
		return 4,0,19282,1,0,0
	end
	
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType21194Cashing(cur_list)		---(兽骨玉匣)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][21189]=100		---绝版坐骑兑换券

	Out_Prize[2][20491]=100		---法宝：长明灯

	Out_Prize[3][18794]=100		---仙豆三个

	Out_Prize[4][13901]=20		---龟灵冰晶
	Out_Prize[4][13900]=80		---虎魄冰晶

	local i,v,k
	for k=1,4 do
	lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,1)==0 and ZLottery_FindSameNum(cur_list,2)==0)
			or ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,3,0,0
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,1,0,0
	end
	return 4,0,13900,1,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType21697Cashing(cur_list)		---(极东绽石)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}

	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][21890]=100 ---//飞剑罡拓兑换券
	Out_Prize[2][19257]=100 ---//麒麟内丹
	Out_Prize[3][13343]=100	---//聚宝盆
	Out_Prize[4][8079]=100	---//飞天神符
		
	local i,v,k
	
	for k=1,4 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,10,0,0
	elseif ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,10,0,0
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,2,0,0
	end
	return 4,0,8079,2,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType21752Cashing(cur_list)		---(天眷神篇/雪琪月虚拟币彩票)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}

	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}

	Out_Prize[1][20096]=100			---新暴击项链:神藏
	Out_Prize[2][21742]=33			---神行锦囊·初计
	Out_Prize[2][21743]=33			---太虚锦囊·初计
	Out_Prize[2][21744]=33			---逍遥锦囊·初计
	Out_Prize[3][21200]=50			---龙魂符(仙魔精练符)
	Out_Prize[3][21345]=50			---造化珠×10
	Out_Prize[4][20098]=50			---成捆的功德香
	Out_Prize[4][19557]=50			---月宫祝福
	
	local i,v,k
	for k=1,4 do
	lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,1)==0 and ZLottery_FindSameNum(cur_list,2)==0)
			or ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		if prizeout==21345 then
			return 3,0,prizeout,10,0,0
		else
			return 3,0,prizeout,1,0,0
		end
	else
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,1,0,0
	end
	return 4,0,20098,1,0,0
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType22126Cashing(cur_list)		---(云雪绘卷)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][22321]=100		---白雕

	Out_Prize[2][22121]=100		---法宝：新月宝鉴

	Out_Prize[3][18794]=100		---仙豆三个

	Out_Prize[4][8079]=100		---飞天神符两个

	local i,v,k
	for k=1,4 do
	lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,1)==0 and ZLottery_FindSameNum(cur_list,2)==0)
			or ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,3,0,0
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,2,0,0
	end
	return 4,0,13900,1,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType22127Cashing(cur_list)		---(炎光匣龙)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][22438]=100		---火麒麟兑换券

	Out_Prize[2][7931]=40			---结婚礼包男
	Out_Prize[2][7937]=60			---结婚礼包女

	Out_Prize[3][18794]=100		---仙豆

	Out_Prize[4][13901]=20		---加Def药
	Out_Prize[4][13900]=80		---加Atk药

	local i,v,k
	for k=1,4 do
	lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,1)==0 and ZLottery_FindSameNum(cur_list,2)==0)
			or ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,3,0,0
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,1,0,0
	end
	return 4,0,13900,1,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryTypeItemSeason2Cashing(cur_list)		---(09春节虚拟币彩票)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][20503]=100		---嗜血珠兑换券
	
	Out_Prize[2][22097]=50		---造化奇石
	Out_Prize[2][6315]=10			---戒指1
	Out_Prize[2][6305]=10			---戒指2
	Out_Prize[2][22074]=10		---戒指3
	Out_Prize[2][22073]=10		---戒指4
	Out_Prize[2][3463]=10			---戒指5
	Out_Prize[2][10957]=10		---戒指6

	Out_Prize[3][19254]=100		---千年芝草
	Out_Prize[3][13343]=50		---聚宝盆
	Out_Prize[3][21153]=40		---胜石

	Out_Prize[4][20099]=10		---成串的乾坤通宝
	Out_Prize[4][20098]=35		---功德香兑换券
	Out_Prize[4][21345]=30		---造化珠
		
	local i,v,k
	
	for k=1,4 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="444" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="333"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,4)==0 and ZLottery_FindSameNum(cur_list,3)==0) or
	(ZLottery_FindSameNum(cur_list,4)==1 and ZLottery_FindSameNum(cur_list,3)==1 and 
	ZLottery_FindSameNum(cur_list,2)==1) then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		if prizeout == 19254 then				return 3,0,prizeout,15,0,0
		elseif prizeout == 13343 then		return 3,0,prizeout,10,0,0
		else 														return 3,0,prizeout,3,0,0
		end
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		if prizeout == 21345 then 			return 4,0,prizeout,80,0,0
		else 														return 4,0,prizeout,3,0,0
		end
	end
	return 4,0,21345,80,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType23144Cashing(cur_list)		---(轮盘:天鹅)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 

	if nowcurlist[fmax_count]=="11" then 			--天鹅
		return 1,0,23142,1,0,0
	elseif nowcurlist[fmax_count]=="12" then	--飞天
		return 4,0,8079,2,0,0
	elseif nowcurlist[fmax_count]=="13" then	--聚宝盆
		return 3,0,13343,10,0,0
	elseif nowcurlist[fmax_count]=="14" then	--龟灵
		return 4,0,13901,1,0,0
	elseif nowcurlist[fmax_count]=="21" then	--男包包
		return 2,0,7931,1,0,0
	elseif nowcurlist[fmax_count]=="22" then	--龟灵
		return 4,0,13901,1,0,0
	elseif nowcurlist[fmax_count]=="23" then	--仙豆
		return 3,0,18794,3,0,0
	elseif nowcurlist[fmax_count]=="24" then	--飞天
		return 4,0,8079,2,0,0
	elseif nowcurlist[fmax_count]=="31" then	--女包包
		return 2,0,7937,1,0,0
	elseif nowcurlist[fmax_count]=="32" then	--飞天
		return 4,0,8079,2,0,0
	elseif nowcurlist[fmax_count]=="33" then	--仙豆
		return 3,0,18794,3,0,0
	elseif nowcurlist[fmax_count]=="34" then	--虎魄
		return 4,0,13900,1,0,0
	elseif nowcurlist[fmax_count]=="41" then	--行天
		return 2,0,18911,1,0,0
	elseif nowcurlist[fmax_count]=="42" then	--虎魄
		return 4,0,13900,1,0,0
	elseif nowcurlist[fmax_count]=="43" then	--聚宝盆
		return 3,0,13343,10,0,0
	else																			--飞天
		return 4,0,8079,2,0,0
	end
	
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType23162Cashing(cur_list)		---(轮盘:河阳驴)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 

	if cur_list[1]==1 then 				--河阳驴兑换券
		return 1,0,23164,1,0,0

	elseif cur_list[1]==3 then		--蝴蝶面具
		return 2,0,13306,1,0,0
	elseif cur_list[1]==5 then		--鬼道秘文石
		return 2,0,17832,1,0,0
	elseif cur_list[1]==7 then		--天音秘文石
		return 2,0,11679,1,0,0
	elseif cur_list[1]==9 then		--青云秘文石
		return 2,0,11678,1,0,0
	elseif cur_list[1]==11 then		--合欢秘文石
		return 2,0,11677,1,0,0
	elseif cur_list[1]==13 then		--鬼王秘文石
		return 2,0,11676,1,0,0
	elseif cur_list[1]==15 then		--鬼面面具
		return 2,0,13307,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--龟灵
		return 4,0,13901,1,0,0
	elseif cur_list[1]==8 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天
		return 4,0,8079,2,0,0
	else													--虎魄
		return 4,0,13900,1,0,0
	end
	
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType23018Cashing(cur_list)		---(09年3月虚拟币彩票：青铜宝箱)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	local Prize1st = {
		20782,20783,20784,20785,	-- 120 & 135 鬼道帽
		20806,20807,20808,20809,	-- 120 & 135 鬼王帽
		20830,20831,20832,20833,	-- 120 & 135 合欢帽
		20854,20855,20856,20857,	-- 120 & 135 青云帽
		20878,20879,20880,20881,	-- 120 & 135 天音帽
		20902,20903,20904,20905,	-- 120 & 135 鬼道鞋
		20926,20927,20928,20929,	-- 120 & 135 鬼王鞋
		20950,20951,20952,20953,	-- 120 & 135 合欢鞋
		20974,20975,20976,20977,	-- 120 & 135 青云鞋
		20998,20999,21000,21001,	-- 120 & 135 天音鞋
	}
	for _,v in pairs(Prize1st) do
		Out_Prize[1][v]= 25
	end
	
	Out_Prize[2][21153]=100		---胜石

	Out_Prize[3][22124]=50		---寒月之辉
	Out_Prize[3][21200]=50		---龙魂符

	Out_Prize[4][22343]=50		---绑定的造化珠
	Out_Prize[4][19254]=50		---千年芝草
	
		
	local i,v,k
	
	for k=1,4 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 

	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,1,0,0
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		if prizeout == 22343 then
			return 4,0,prizeout,25,0,0
		else 
			return 4,0,prizeout,6,0,0
		end
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType23295Cashing(cur_list)		---(09年3月宠物彩票：鸿蒙秘宝)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}

	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][23327]=100		---鸿蒙雪参兑换券

	Out_Prize[2][16320]=50		---易筋锻骨丹
	Out_Prize[2][23296]=50		---神魔秘录

	Out_Prize[3][22312]=50		---宠物泡泡糖
	Out_Prize[3][21200]=50		---龙魂符

	Out_Prize[4][19254]=100		---千年芝草
	
		
	local i,v,k
	
	for k=1,4 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 

	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222" then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,1)==0 and ZLottery_FindSameNum(cur_list,2)==0)
			or ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,2,0,0
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,5,0,0
	end
	return 4,0,19254,5,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType23212Cashing(cur_list)		---(轮盘:圣龙玉髓)
	if cur_list[1]==1 then 				--敖弘兑换券
		return 1,0,23299,1,0,0

	elseif cur_list[1]==5 then		--龙魂符十五个
		return 2,0,21200,15,0,0
	elseif cur_list[1]==9 then		--天书洗点
		return 2,0,11539,1,0,0
	elseif cur_list[1]==13 then		--龙魂符十五个
		return 2,0,21200,15,0,0

	elseif cur_list[1]==3 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==7 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==11 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==15 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==4 then		--龟灵
		return 4,0,13901,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,13901,1,0,0
	elseif cur_list[1]==8 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==12 then		--虎魄
		return 4,0,13900,1,0,0
	elseif cur_list[1]==14 then		--虎魄
		return 4,0,13900,1,0,0
	else													--飞天
		return 4,0,8079,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType23336Cashing(cur_list)		---(轮盘:凤栖金枝)
	if cur_list[1]==1 then 				--凤凰兑换券
		return 1,0,23344,1,0,0
	elseif cur_list[1]==9 then		--凤凰兑换券
		return 1,0,23344,1,0,0

	elseif cur_list[1]==5 then		--男包包
		return 2,0,7931,1,0,0
	elseif cur_list[1]==13 then		--女包包
		return 2,0,7937,1,0,0

	elseif cur_list[1]==3 then		--盆子
		return 3,0,13343,10,0,0
	elseif cur_list[1]==7 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==11 then		--盆子
		return 3,0,13343,10,0,0
	elseif cur_list[1]==15 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==4 then		--龟灵
		return 4,0,13901,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,13901,1,0,0
	elseif cur_list[1]==8 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==12 then		--虎魄
		return 4,0,13900,1,0,0
	elseif cur_list[1]==14 then		--虎魄
		return 4,0,13900,1,0,0
	else													--飞天
		return 4,0,8079,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType23363Cashing(cur_list)		---(轮盘:天韵神篇)
	if cur_list[1]==1 then 				--空瑞兑换券
		return 1,0,23364,1,0,0
	elseif cur_list[1]==9 then		--空瑞兑换券
		return 1,0,23364,1,0,0

	elseif cur_list[1]==5 then		--男包包
		return 2,0,7931,1,0,0
	elseif cur_list[1]==13 then		--女包包
		return 2,0,7937,1,0,0

	elseif cur_list[1]==3 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==7 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==11 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==15 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==4 then		--龟灵
		return 4,0,13901,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,13901,1,0,0
	elseif cur_list[1]==8 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==12 then		--虎魄
		return 4,0,13900,1,0,0
	elseif cur_list[1]==14 then		--虎魄
		return 4,0,13900,1,0,0
	else													--飞天
		return 4,0,8079,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType23422Cashing(cur_list)		---(轮盘:玄文虎石)
	if cur_list[1]==1 then 				--怒虎兑换券
		return 1,0,23426,1,0,0

	elseif cur_list[1]==5 then		--浮屠
		return 2,0,23353,1,0,0
	elseif cur_list[1]==9 then		--浮屠
		return 2,0,23353,1,0,0
	elseif cur_list[1]==13 then		--浮屠
		return 2,0,23353,1,0,0

	elseif cur_list[1]==3 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==7 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==11 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==15 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==4 then		--龟灵
		return 4,0,13901,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,13901,1,0,0
	elseif cur_list[1]==8 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==12 then		--虎魄
		return 4,0,13900,1,0,0
	elseif cur_list[1]==14 then		--虎魄
		return 4,0,13900,1,0,0
	else													--飞天
		return 4,0,8079,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType23423Cashing(cur_list)		---(09年3月凌霄副本彩票：凌霄秘宝)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][20664]=40		--- 135坤衣
	Out_Prize[1][20665]=60		---
	Out_Prize[1][20688]=40		---
	Out_Prize[1][20689]=60		---
	Out_Prize[1][20712]=40		---
	Out_Prize[1][20713]=60		---
	Out_Prize[1][20736]=40		---
	Out_Prize[1][20737]=60		---
	Out_Prize[1][20760]=40		---
	Out_Prize[1][20761]=60		---
	
	Out_Prize[2][20784]=40		--- 135坤帽
	Out_Prize[2][20785]=60		---
	Out_Prize[2][20808]=40		---
	Out_Prize[2][20809]=60		---
	Out_Prize[2][20832]=40		---
	Out_Prize[2][20833]=60		---
	Out_Prize[2][20856]=40		---
	Out_Prize[2][20857]=60		---
	Out_Prize[2][20880]=40		---
	Out_Prize[2][20881]=60		---

	Out_Prize[3][22343]=100		--- 绑定造化珠

	Out_Prize[4][21154]=100		--- 连天斗玉
		
	local i,v,k
	
	for k=1,4 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,200,0,0
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,20,0,0
	end

	return 4,0,prizeout,20,0,0
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType24343Cashing(cur_list)		---(天衮靛石:獬豸)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 

	if cur_list[1]==1 then 				--獬豸兑换券
		return 1,0,24338,1,0,0

	elseif cur_list[1]==3 then		--鸟羽面具
		return 2,0,19128,1,0,0
	elseif cur_list[1]==5 then		--鬼道秘文石
		return 2,0,17832,1,0,0
	elseif cur_list[1]==7 then		--天音秘文石
		return 2,0,11679,1,0,0
	elseif cur_list[1]==9 then		--青云秘文石
		return 2,0,11678,1,0,0
	elseif cur_list[1]==11 then		--合欢秘文石
		return 2,0,11677,1,0,0
	elseif cur_list[1]==13 then		--鬼王秘文石
		return 2,0,11676,1,0,0
	elseif cur_list[1]==15 then		--龙火面具
		return 2,0,19127,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--龟灵
		return 4,0,13901,1,0,0
	elseif cur_list[1]==8 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天
		return 4,0,8079,2,0,0
	else													--虎魄
		return 4,0,13900,1,0,0
	end
	
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType24344Cashing(cur_list)		---(09两周年虚拟币彩票)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][10875]=50		---摄魂
	Out_Prize[1][20096]=50		---神藏
	
	Out_Prize[2][6315]=200			---100级躲闪戒指
	Out_Prize[2][6305]=200			---100级防御戒指
	Out_Prize[2][3463]=200			---100级减伤戒指
	Out_Prize[2][10957]=100			---100级全抗戒指

	Out_Prize[3][16551]=50			---集宝盆三个
	Out_Prize[3][19257]=50			---麒麟内丹
	
	Out_Prize[4][20098]=50			---功德香兑换券三个
	Out_Prize[4][20099]=50			---乾坤通宝兑换券三个
	
	local i,v,k
	for k=1,4 do
	lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,1)==0 and ZLottery_FindSameNum(cur_list,2)==0) or
	ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		if prizeout==16551 then 
			return 3,0,prizeout,3,0,0
		else
			return 3,0,prizeout,1,0,0
		end 	
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,3,0,0
	end
	return 4,0,20098,3,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType24993Cashing(cur_list)		---(万兽金尊777)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][21189]=100		---百兽淀魂珠
	Out_Prize[2][24904]=100		---幻灵精魄
	Out_Prize[3][18794]=100		---仙豆三个
	Out_Prize[4][13901]=100		---龟灵冰晶
	Out_Prize[4][13900]=100		---虎魄冰晶
	Out_Prize[4][8079]=100		---飞天神符两个

	local i,v,k
	for k=1,4 do
	lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,1)==0 and ZLottery_FindSameNum(cur_list,2)==0)
			or ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,3,0,0
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		if 		 prizeout == 13901 then	return 4,0,prizeout,1,0,0
		elseif prizeout == 13900 then	return 4,0,prizeout,1,0,0
		else 													return 4,0,prizeout,2,0,0
		end
	end
	return 4,0,8079,2,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType24998Cashing(cur_list)		---(玄天宝录777)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][24903]=100		---先天风云谱
	Out_Prize[2][18797]=100		---御宝神篇	
	Out_Prize[3][13343]=100		---聚宝盆
	Out_Prize[4][13901]=25		---龟灵冰晶
	Out_Prize[4][13900]=100		---虎魄冰晶
	Out_Prize[4][8079]=100		---飞天神符两个
		
	local i,v,k
	
	for k=1,4 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,1,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,1,0,0
	elseif (ZLottery_FindSameNum(cur_list,1)==0 and ZLottery_FindSameNum(cur_list,2)==0) or
	(ZLottery_FindSameNum(cur_list,1)==1 and ZLottery_FindSameNum(cur_list,2)==1 and 
	ZLottery_FindSameNum(cur_list,3)==1) then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,10,0,0
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		if 		 prizeout == 13901 then	return 4,0,prizeout,1,0,0
		elseif prizeout == 13900 then	return 4,0,prizeout,1,0,0
		else 													return 4,0,prizeout,2,0,0
		end
	end
	return 4,0,8079,2,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType24999Cashing(cur_list)		---(万兽金尊轮盘)
	if cur_list[1]==1 then 				--百兽淀魂珠
		return 1,0,21189,1,0,0

	elseif cur_list[1]==5 then		--幻灵精魄
		return 2,0,24904,1,0,0
	elseif cur_list[1]==9 then		--幻灵精魄
		return 2,0,24904,1,0,0
	elseif cur_list[1]==13 then		--幻灵精魄
		return 2,0,24904,1,0,0

	elseif cur_list[1]==3 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==7 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==11 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==15 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==4 then		--龟灵
		return 4,0,13901,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,13901,1,0,0
	elseif cur_list[1]==8 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==12 then		--虎魄
		return 4,0,13900,1,0,0
	elseif cur_list[1]==14 then		--虎魄
		return 4,0,13900,1,0,0
	else													--飞天
		return 4,0,8079,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType25000Cashing(cur_list)		---(玄天宝录轮盘)
	if cur_list[1]==1 then 				--先天风云谱
		return 1,0,24903,1,0,0

	elseif cur_list[1]==5 then		--御宝神篇
		return 2,0,18797,1,0,0
	elseif cur_list[1]==9 then		--御宝神篇
		return 2,0,18797,1,0,0
	elseif cur_list[1]==13 then		--御宝神篇
		return 2,0,18797,1,0,0

	elseif cur_list[1]==3 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==7 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==11 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==15 then		--聚宝盆
		return 3,0,13343,10,0,0

	elseif cur_list[1]==2 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==4 then		--龟灵
		return 4,0,13901,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,13901,1,0,0
	elseif cur_list[1]==8 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==12 then		--虎魄
		return 4,0,13900,1,0,0
	elseif cur_list[1]==14 then		--虎魄
		return 4,0,13900,1,0,0
	else													--飞天
		return 4,0,8079,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType25305Cashing(cur_list)		---(天竺牙雕)
	if cur_list[1]==1 then 				--象王兑换券
		return 1,0,25302,1,0,0

	elseif cur_list[1]==5 then		--花瓣面具（女）
		return 2,0,19130,1,0,0
	elseif cur_list[1]==9 then		--无字天书
		return 2,0,11539,1,0,0
	elseif cur_list[1]==13 then		--毒蝎面具（男）
		return 2,0,19129,1,0,0

	elseif cur_list[1]==3 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==7 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==11 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==15 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==4 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==12 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==14 then		--虎魄
		return 4,0,25307,1,0,0
	else													--飞天
		return 4,0,8079,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType25417Cashing(cur_list)		---(血月镇木)
	if cur_list[1]==1 then 				--夜枭兑换券
		return 1,0,25419,1,0,0
	elseif cur_list[1]==9 then		--夜枭兑换券
		return 1,0,25419,1,0,0

	elseif cur_list[1]==5 then		--男包包
		return 2,0,7931,1,0,0
	elseif cur_list[1]==13 then		--女包包
		return 2,0,7937,1,0,0

	elseif cur_list[1]==3 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==7 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==11 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==15 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==4 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==12 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==14 then		--虎魄
		return 4,0,25307,1,0,0
	else													--飞天
		return 4,0,8079,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType25418Cashing(cur_list)		---(大漠金沙)
	if cur_list[1]==1 then 				--沙王兑换券
		return 1,0,25420,1,0,0

	elseif cur_list[1]==5 then		--无字天书
		return 2,0,11539,1,0,0
	elseif cur_list[1]==9 then		--无字天书
		return 2,0,11539,1,0,0
	elseif cur_list[1]==13 then		--无字天书
		return 2,0,11539,1,0,0

	elseif cur_list[1]==3 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==7 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==11 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==15 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==4 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==12 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==14 then		--虎魄
		return 4,0,25307,1,0,0
	else													--飞天
		return 4,0,8079,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType25749Cashing(cur_list)		---(太苍空岩)
	if cur_list[1]==1 then 				--混沌葫芦兑换券
		return 1,0,25746,1,0,0

	elseif cur_list[1]==5 then		--引魂幡
		return 2,0,25732,1,0,0
	elseif cur_list[1]==9 then		--引魂幡
		return 2,0,25732,1,0,0
	elseif cur_list[1]==13 then		--引魂幡
		return 2,0,25732,1,0,0

	elseif cur_list[1]==3 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==7 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==11 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==15 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==4 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==12 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==14 then		--虎魄
		return 4,0,25307,1,0,0
	else													--飞天
		return 4,0,8079,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType25750Cashing(cur_list)		---(九叶仙芝)
	if cur_list[1]==1 then 				--玉仑仙辇兑换券
		return 1,0,25747,1,0,0
	elseif cur_list[1]==9 then		--10级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--蝠王封印
		return 1,0,32104,1,0,0

	elseif cur_list[1]==3 then		--女包包
		return 2,0,7937,1,0,0
	elseif cur_list[1]==5 then		--行天
		return 2,0,18911,1,0,0
	elseif cur_list[1]==7 then		--幻灵精魄
		return 2,0,24904,1,0,0
	elseif cur_list[1]==11 then		--御宝神篇
		return 2,0,18797,1,0,0
	elseif cur_list[1]==15 then		--男包包
		return 2,0,7931,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--集宝盆
		return 3,0,16551,1,0,0
	elseif cur_list[1]==12 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==8 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==10 then		--虎魄
		return 4,0,25307,1,0,0
	else													--飞天
		return 4,0,8079,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType28573Cashing(cur_list)		---(混沌秘宝)
	if cur_list[1]==1 then 				--天魔血晶
		return 1,0,27150,1,0,0
	elseif cur_list[1]==5 then		--摄魂
		return 1,0,10875,1,0,0
	elseif cur_list[1]==9 then		--精金铁玉（十级）
		return 1,0,28588,1,0,0
	elseif cur_list[1]==13 then		--嗜血珠
		return 1,0,10876,1,0,0

	elseif cur_list[1]==3 then		--太乙
		return 2,0,25729,1,0,0
	elseif cur_list[1]==7 then		--女包包
		return 2,0,7937,1,0,0
	elseif cur_list[1]==11 then		--男包包
		return 2,0,7931,1,0,0
	elseif cur_list[1]==14 then		--幻灵精魄
		return 2,0,24904,1,0,0
	elseif cur_list[1]==15 then		--太乙
		return 2,0,25729,1,0,0

	elseif cur_list[1]==4 then		--秘纹石晶
		return 3,0,28587,1,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--聚宝盆
		return 3,0,13343,10,0,0

	elseif cur_list[1]==8 then		--集宝盆
		return 4,0,16551,1,0,0
	elseif cur_list[1]==10 then		--胜石
		return 4,0,21153,2,0,0
	elseif cur_list[1]==16 then		--秋日思语
		return 4,0,25731,3,0,0
	else													--飞天
		return 4,0,8079,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType28703Cashing(cur_list)		---(龙皇魂骨)
	if cur_list[1]==1 then 				--敖蒙兑换券
		return 1,0,28707,1,0,0
	elseif cur_list[1]==9 then		--敖蒙兑换券
		return 1,0,28707,1,0,0

	elseif cur_list[1]==5 then		--男包包
		return 2,0,7931,1,0,0
	elseif cur_list[1]==13 then		--女包包
		return 2,0,7937,1,0,0

	elseif cur_list[1]==3 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==7 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==11 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==15 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==4 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==12 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==14 then		--虎魄
		return 4,0,25307,1,0,0
	else													--飞天
		return 4,0,8079,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType32605Cashing(cur_list)		---(狮蛮宝录)
	if cur_list[1]==1 then 				--金毛狮王兑换券
		return 1,0,32604,1,0,0

	elseif cur_list[1]==5 then		--秘文石晶
		return 2,0,28587,1,0,0
	elseif cur_list[1]==9 then		--无字天书
		return 2,0,11539,1,0,0
	elseif cur_list[1]==13 then		--幻灵精魄
		return 2,0,24904,1,0,0

	elseif cur_list[1]==3 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==7 then		--集宝盆
		return 3,0,16551,1,0,0
	elseif cur_list[1]==11 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==15 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==4 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==12 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==14 then		--虎魄
		return 4,0,25307,1,0,0
	else													--飞天
		return 4,0,8079,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType32692Cashing(cur_list)		---(多罗慧叶)
	if cur_list[1]==1 then 				--金鹏兑换券
		return 1,0,32710,1,0,0
	elseif cur_list[1]==9 then		--金鹏兑换券
		return 1,0,32710,1,0,0

	elseif cur_list[1]==5 then		--太乙
		return 2,0,25729,1,0,0
	elseif cur_list[1]==13 then		--太昊
		return 2,0,25728,1,0,0

	elseif cur_list[1]==3 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==7 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==11 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==15 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==4 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==12 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==14 then		--虎魄
		return 4,0,25307,1,0,0
	else													--飞天
		return 4,0,8079,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType33055Cashing(cur_list)	---(2009圣诞袜子)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local lot_prizenumber={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	
	Out_Prize[1][31819]={chance=05,number=1}  ---双倍珠子*1     0.05
	
	Out_Prize[1][18827]={chance=05,number=1}  ---神品坚盾*1     0.05
	Out_Prize[1][18826]={chance=05,number=1}  ---神品利矛*1     0.05
	Out_Prize[1][18820]={chance=10,number=1}  ---极品坚盾*1     0.1
	Out_Prize[1][18821]={chance=10,number=1}  ---极品利矛*1     0.1

	Out_Prize[1][19252]={chance=10,number=2}  ---轩辕通宝*2     0.1
	Out_Prize[1][16385]={chance=10,number=25} ---乾坤通宝*25    0.1
	Out_Prize[1][19254]={chance=10,number=5}  ---千年芝草*5     0.1
	Out_Prize[1][22312]={chance=10,number=3}  ---泡泡糖*3       0.1 
	Out_Prize[1][23880]={chance=10,number=2}  ---龙牙草*2       0.1
	
	Out_Prize[1][22343]={chance=05,number=500}---造化珠*500     0.05
	Out_Prize[1][24653]={chance=05,number=1}  ---绑定虎魄*1     0.05
	Out_Prize[1][24652]={chance=05,number=1}  ---绑定龟灵*1     0.05
		
	local i,v
	
	lot_prize[1]={}
	lot_prizechance[1]={}
	lot_prizenumber[1]={}
	
	for i,v in pairs(Out_Prize[1]) do 
		table.insert(lot_prize[1],i)
		table.insert(lot_prizechance[1],v.chance)
		table.insert(lot_prizenumber[1],v.number)
	end 

	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,Out_Prize[1][prizeout].number,0,0
	end
	return 1,0,24653,1,0,0 
end
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType33122Cashing(cur_list)		---(圣诞兔子)
	if cur_list[1]==1 then 				--兔子兑换
		return 1,0,33124,1,0,0
	elseif cur_list[1]==9 then		--兔子兑换
		return 1,0,33124,1,0,0

	elseif cur_list[1]==5 then		--圣诞女面具
		return 2,0,33208,1,0,0
	elseif cur_list[1]==13 then		--圣诞男面具
		return 2,0,33156,1,0,0

	elseif cur_list[1]==3 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==7 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==11 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==15 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==4 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天
		return 4,0,8079,2,0,0
	elseif cur_list[1]==12 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==14 then		--虎魄
		return 4,0,25307,1,0,0
	else													--飞天
		return 4,0,8079,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType33530Cashing(cur_list)		---(玄金虎玉)
	if cur_list[1]==1 then 				--冥虎兑换券
		return 1,0,33529,1,0,0
	elseif cur_list[1]==9 then		--10级铁玉兑换券
		return 1,0,32103,1,0,0

	elseif cur_list[1]==5 then		--男包包
		return 2,0,7931,1,0,0
	elseif cur_list[1]==13 then		--女包包
		return 2,0,7937,1,0,0

	elseif cur_list[1]==3 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==7 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==11 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==15 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==4 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==10 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==12 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==14 then		--龟灵
		return 4,0,25306,1,0,0
	else													--虎魄
		return 4,0,25307,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType33822Cashing(cur_list)		---(贺岁桃符)
	if cur_list[1]==1 then 				--百兽淀魂珠
		return 1,0,21189,1,0,0
	elseif cur_list[1]==5 then		--百兽淀魂珠
		return 1,0,21189,1,0,0
	elseif cur_list[1]==9 then		--10级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--百兽淀魂珠
		return 1,0,21189,1,0,0

	elseif cur_list[1]==3 then		--幻灵精魄
		return 2,0,24904,1,0,0
	elseif cur_list[1]==7 then		--女包包
		return 2,0,7937,1,0,0
	elseif cur_list[1]==11 then		--男包包
		return 2,0,7931,1,0,0
	elseif cur_list[1]==15 then		--幻灵精魄
		return 2,0,24904,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==12 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--龟灵冰晶
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--虎魄冰晶
		return 4,0,25307,1,0,0
	elseif cur_list[1]==10 then		--虎魄冰晶
		return 4,0,25307,1,0,0
	elseif cur_list[1]==16 then		--龟灵冰晶
		return 4,0,25306,1,0,0
	else													--虎魄冰晶
		return 4,0,25307,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType33823Cashing(cur_list)		---(迎新春联)
	if cur_list[1]==1 then 				--先天风云谱
		return 1,0,24903,1,0,0
	elseif cur_list[1]==5 then		--摄魂礼包
		return 1,0,33820,1,0,0
	elseif cur_list[1]==9 then		--天魔血晶礼包
		return 1,0,33821,1,0,0
	elseif cur_list[1]==13 then		--先天风云谱
		return 1,0,24903,1,0,0

	elseif cur_list[1]==3 then		--御宝神篇
		return 2,0,18797,1,0,0
	elseif cur_list[1]==7 then		--女包包
		return 2,0,7937,1,0,0
	elseif cur_list[1]==11 then		--男包包
		return 2,0,7931,1,0,0
	elseif cur_list[1]==15 then		--御宝神篇
		return 2,0,18797,1,0,0

	elseif cur_list[1]==4 then		--集宝盆
		return 3,0,16551,3,0,0
	elseif cur_list[1]==6 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==12 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==14 then		--集宝盆
		return 3,0,16551,3,0,0

	elseif cur_list[1]==2 then		--龟灵冰晶
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--虎魄冰晶
		return 4,0,25307,1,0,0
	elseif cur_list[1]==10 then		--虎魄冰晶
		return 4,0,25307,1,0,0
	elseif cur_list[1]==16 then		--龟灵冰晶
		return 4,0,25306,1,0,0
	else													--虎魄冰晶
		return 4,0,25307,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType33988Cashing(cur_list)		---(2010春节红包)
	---fmax_count为该彩票的有效位数
	local cp
	local nowcurlist={}
	local fmax_count=table.getn(cur_list)
	nowcurlist[0]=""
	for cp=1,fmax_count do
		nowcurlist[cp]=nowcurlist[cp-1]..cur_list[cp]
	end 
	
	local lot_prize={}
	local lot_prizechance={}
	local prizeout
	local Out_Prize={}
	Out_Prize[1]={}
	Out_Prize[2]={}
	Out_Prize[3]={}
	Out_Prize[4]={}
	
	Out_Prize[1][14062]=100 ---金叶子	
	Out_Prize[2][14062]=100 ---金叶子
	Out_Prize[3][14062]=100	---金叶子	
	Out_Prize[4][14062]=100	---金叶子
		
	local i,v,k
	
	for k=1,4 do
		lot_prize[k]={}
		lot_prizechance[k]={}
	end 
	
	for k=1,4 do
		for i,v in pairs(Out_Prize[k]) do 
			table.insert(lot_prize[k],i)
			table.insert(lot_prizechance[k],v)
		end 
	end 
	if nowcurlist[fmax_count]=="111" then 
		prizeout=lot_prize[1][ZLottery_Get_RanPoint(lot_prizechance[1])]
		return 1,0,prizeout,666,0,0
	elseif nowcurlist[fmax_count]=="222"  then 
		prizeout=lot_prize[2][ZLottery_Get_RanPoint(lot_prizechance[2])]
		return 2,0,prizeout,66,0,0
	elseif ZLottery_FindSameNum(cur_list,1)==2 or ZLottery_FindSameNum(cur_list,2)==2 then 
		prizeout=lot_prize[3][ZLottery_Get_RanPoint(lot_prizechance[3])]
		return 3,0,prizeout,6,0,0
	else 
		prizeout=lot_prize[4][ZLottery_Get_RanPoint(lot_prizechance[4])]
		return 4,0,prizeout,1,0,0
	end
	return 4,0,14062,1,0,0 
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType34124Cashing(cur_list)		---(金宇帝书)
	if cur_list[1]==1 then 				--羽皇兑换
		return 1,0,34042,1,0,0
	elseif cur_list[1]==9 then		--羽皇兑换
		return 1,0,34042,1,0,0

	elseif cur_list[1]==5 then		--男包包
		return 2,0,7931,1,0,0
	elseif cur_list[1]==13 then		--女包包
		return 2,0,7937,1,0,0

	elseif cur_list[1]==3 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==7 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==11 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==15 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==4 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==10 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==12 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==14 then		--龟灵
		return 4,0,25306,1,0,0
	else													--虎魄
		return 4,0,25307,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType34125Cashing(cur_list)		---(御贡锦匣)
	if cur_list[1]==1 then 				--礼服大礼包
		return 1,0,34043,1,0,0
	elseif cur_list[1]==9 then		--婚纱大礼包
		return 1,0,34044,1,0,0

	elseif cur_list[1]==5 then		--秘文石晶
		return 2,0,28587,1,0,0
	elseif cur_list[1]==13 then		--无字天书
		return 2,0,11539,1,0,0

	elseif cur_list[1]==3 then		--集宝盆
		return 3,0,16551,1,0,0
	elseif cur_list[1]==7 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==11 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==15 then		--集宝盆
		return 3,0,16551,1,0,0

	elseif cur_list[1]==2 then		--飞天神符
		return 4,0,8079,2,0,0
	elseif cur_list[1]==4 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--飞天神符
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天神符
		return 4,0,8079,2,0,0
	elseif cur_list[1]==12 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==14 then		--虎魄
		return 4,0,25307,1,0,0
	else													--飞天神符
		return 4,0,8079,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType34295Cashing(cur_list)		---(锦绣绘卷)
	if cur_list[1]==1 then 				--龙马兑换男
		return 1,0,34351,1,0,0
	elseif cur_list[1]==9 then		--龙马兑换女
		return 1,0,34352,1,0,0

	elseif cur_list[1]==5 then		--太乙
		return 2,0,25729,1,0,0
	elseif cur_list[1]==13 then		--太昊
		return 2,0,25728,1,0,0

	elseif cur_list[1]==3 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==7 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==11 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==15 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==4 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==10 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==12 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==14 then		--龟灵
		return 4,0,25306,1,0,0
	else													--虎魄
		return 4,0,25307,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType34296Cashing(cur_list)		---(天宫神石)
	if cur_list[1]==1 then 				--魅影兑换
		return 1,0,34353,1,0,0

	elseif cur_list[1]==5 then		--云纹天罗
		return 2,0,13904,1,0,0
	elseif cur_list[1]==9 then		--行天圣印
		return 2,0,18911,1,0,0
	elseif cur_list[1]==13 then		--栖凤建木
		return 2,0,13903,1,0,0

	elseif cur_list[1]==3 then		--集宝盆
		return 3,0,16551,1,0,0
	elseif cur_list[1]==7 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==11 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==15 then		--集宝盆
		return 3,0,16551,1,0,0

	elseif cur_list[1]==2 then		--飞天神符
		return 4,0,8079,2,0,0
	elseif cur_list[1]==4 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--飞天神符
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天神符
		return 4,0,8079,2,0,0
	elseif cur_list[1]==12 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==14 then		--虎魄
		return 4,0,25307,1,0,0
	else													--飞天神符
		return 4,0,8079,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType34937Cashing(cur_list)		---(火焰纹章)
	if cur_list[1]==1 then 				--赤霄兑换券
		return 1,0,33807,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0

	elseif cur_list[1]==5 then		--男包包
		return 2,0,7931,1,0,0
	elseif cur_list[1]==13 then		--女包包
		return 2,0,7937,1,0,0

	elseif cur_list[1]==3 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==7 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==11 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==15 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==4 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==10 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==12 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==14 then		--龟灵
		return 4,0,25306,1,0,0
	else													--虎魄
		return 4,0,25307,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType35165Cashing(cur_list)		---(寻梦玉蝶)
	if cur_list[1]==1 then 				--彩翼兑换券
		return 1,0,35179,1,0,0

	elseif cur_list[1]==5 then		--神行锦囊·初计
		return 2,0,21742,1,0,0
	elseif cur_list[1]==9 then		--太虚锦囊·初计
		return 2,0,21743,1,0,0
	elseif cur_list[1]==13 then		--逍遥锦囊·初计
		return 2,0,21744,1,0,0

	elseif cur_list[1]==3 then		--集宝盆
		return 3,0,16551,1,0,0
	elseif cur_list[1]==7 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==11 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==15 then		--集宝盆
		return 3,0,16551,1,0,0

	elseif cur_list[1]==2 then		--飞天神符
		return 4,0,8079,2,0,0
	elseif cur_list[1]==4 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--飞天神符
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天神符
		return 4,0,8079,2,0,0
	elseif cur_list[1]==12 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==14 then		--虎魄
		return 4,0,25307,1,0,0
	else													--飞天神符
		return 4,0,8079,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType35360Cashing(cur_list)		---(天池灵泉)
	if cur_list[1]==1 then 				--天华水精,瑶池金梭兑换券
		return 1,0,35358,1,0,0
	elseif cur_list[1]==9 then		--天华水精,瑶池金梭兑换券
		return 1,0,35358,1,0,0

	elseif cur_list[1]==5 then		--男包包
		return 2,0,7931,1,0,0
	elseif cur_list[1]==13 then		--女包包
		return 2,0,7937,1,0,0

	elseif cur_list[1]==3 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==7 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==11 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==15 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==4 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==10 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==12 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==14 then		--龟灵
		return 4,0,25306,1,0,0
	else													--虎魄
		return 4,0,25307,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType35361Cashing(cur_list)		---(五光锦匣)
	if cur_list[1]==1 then 				--金丝绣帕，飞绫兑换券
		return 1,0,35359,1,0,0

	elseif cur_list[1]==5 then		--秘文石晶
		return 2,0,28587,1,0,0
	elseif cur_list[1]==9 then		--秘文石晶
		return 2,0,28587,1,0,0
	elseif cur_list[1]==13 then		--秘文石晶
		return 2,0,28587,1,0,0

	elseif cur_list[1]==3 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==7 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==11 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==15 then		--聚宝盆
		return 3,0,13343,10,0,0

	elseif cur_list[1]==2 then		--飞天神符
		return 4,0,8079,2,0,0
	elseif cur_list[1]==4 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--飞天神符
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天神符
		return 4,0,8079,2,0,0
	elseif cur_list[1]==12 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==14 then		--虎魄
		return 4,0,25307,1,0,0
	else													--飞天神符
		return 4,0,8079,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType35362Cashing(cur_list)		---(天工开物)
	if cur_list[1]==1 then 				--先天风云谱
		return 1,0,24903,1,0,0

	elseif cur_list[1]==5 then		--男包包
		return 2,0,7931,1,0,0
	elseif cur_list[1]==9 then		--御宝神篇
		return 2,0,18797,1,0,0
	elseif cur_list[1]==13 then		--女包包
		return 2,0,7937,1,0,0

	elseif cur_list[1]==3 then		--集宝盆
		return 3,0,16551,1,0,0
	elseif cur_list[1]==7 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==11 then		--聚宝盆
		return 3,0,13343,10,0,0
	elseif cur_list[1]==15 then		--集宝盆
		return 3,0,16551,1,0,0

	elseif cur_list[1]==2 then		--飞天神符
		return 4,0,8079,2,0,0
	elseif cur_list[1]==4 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--飞天神符
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天神符
		return 4,0,8079,2,0,0
	elseif cur_list[1]==12 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==14 then		--虎魄
		return 4,0,25307,1,0,0
	else													--飞天神符
		return 4,0,8079,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType35363Cashing(cur_list)		---(聚灵神龛)
	if cur_list[1]==1 then 				--百兽淀魂珠
		return 1,0,21189,1,0,0

	elseif cur_list[1]==5 then		--男包包
		return 2,0,7931,1,0,0
	elseif cur_list[1]==9 then		--幻灵精魄
		return 2,0,24904,1,0,0
	elseif cur_list[1]==13 then		--女包包
		return 2,0,7937,1,0,0

	elseif cur_list[1]==3 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==7 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==11 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==15 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--飞天神符
		return 4,0,8079,2,0,0
	elseif cur_list[1]==4 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--飞天神符
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天神符
		return 4,0,8079,2,0,0
	elseif cur_list[1]==12 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==14 then		--虎魄
		return 4,0,25307,1,0,0
	else													--飞天神符
		return 4,0,8079,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType40344Cashing(cur_list)		---(十全界书)
	if cur_list[1]==1 then 				--百兽淀魂珠
		return 1,0,21189,1,0,0
	elseif cur_list[1]==5 then		--摄魂礼包
		return 1,0,33820,1,0,0
	elseif cur_list[1]==9 then		--10级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--嗜血珠礼包
		return 1,0,40338,1,0,0

	elseif cur_list[1]==3 then		--无字天书
		return 2,0,11539,1,0,0
	elseif cur_list[1]==7 then		--女包包
		return 2,0,7937,1,0,0
	elseif cur_list[1]==11 then		--男包包
		return 2,0,7931,1,0,0
	elseif cur_list[1]==15 then		--无字天书
		return 2,0,11539,1,0,0

	elseif cur_list[1]==4 then		--仙豆三个
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆三个
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆三个
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆三个
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--虎魄玄冰
		return 4,0,25307,1,0,0
	elseif cur_list[1]==8 then		--龟灵玄冰
		return 4,0,25306,1,0,0
	elseif cur_list[1]==10 then		--龟灵玄冰
		return 4,0,25306,1,0,0
	else													--虎魄玄冰
		return 4,0,25307,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType40345Cashing(cur_list)		---(天关幻罡)
	if cur_list[1]==1 then 				--先天风云谱
		return 1,0,24903,1,0,0

	elseif cur_list[1]==5 then		--御宝神篇
		return 2,0,18797,1,0,0
	elseif cur_list[1]==9 then		--御宝神篇
		return 2,0,18797,1,0,0
	elseif cur_list[1]==13 then		--御宝神篇
		return 2,0,18797,1,0,0

	elseif cur_list[1]==3 then		--豪血龙丹
		return 3,0,23860,1,0,0
	elseif cur_list[1]==7 then		--魔煞龙丹
		return 3,0,23861,1,0,0
	elseif cur_list[1]==11 then		--八凶龙丹
		return 3,0,23862,1,0,0
	elseif cur_list[1]==15 then		--豪血龙丹
		return 3,0,23860,1,0,0

	elseif cur_list[1]==2 then		--御空秘印
		return 4,0,18910,1,0,0
	elseif cur_list[1]==4 then		--太一轮五个
		return 4,0,1807,5,0,0
	elseif cur_list[1]==6 then		--太一轮五个
		return 4,0,1807,5,0,0
	elseif cur_list[1]==8 then		--御空秘印
		return 4,0,18910,1,0,0
	elseif cur_list[1]==10 then		--御空秘印
		return 4,0,18910,1,0,0
	elseif cur_list[1]==12 then		--太一轮五个
		return 4,0,1807,5,0,0
	elseif cur_list[1]==14 then		--太一轮五个
		return 4,0,1807,5,0,0
	else													--御空秘印
		return 4,0,18910,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType40346Cashing(cur_list)		---(伏灵神引)
	if cur_list[1]==1 then 				--冰封的鸿蒙雪参
		return 1,0,23327,1,0,0

	elseif cur_list[1]==5 then		--幻灵精魄
		return 2,0,24904,1,0,0
	elseif cur_list[1]==9 then		--幻灵精魄
		return 2,0,24904,1,0,0
	elseif cur_list[1]==13 then		--幻灵精魄
		return 2,0,24904,1,0,0

	elseif cur_list[1]==3 then		--九曜琉璃
		return 3,0,18414,1,0,0
	elseif cur_list[1]==7 then		--麒麟内丹
		return 3,0,19257,3,0,0
	elseif cur_list[1]==11 then		--麒麟内丹
		return 3,0,19257,3,0,0
	elseif cur_list[1]==15 then		--九曜琉璃
		return 3,0,18414,1,0,0

	elseif cur_list[1]==2 then		--宠技彩对券
		return 4,0,40339,1,0,0
	elseif cur_list[1]==4 then		--太一轮五个
		return 4,0,1807,5,0,0
	elseif cur_list[1]==6 then		--太一轮五个
		return 4,0,1807,5,0,0
	elseif cur_list[1]==8 then		--宠技彩对券
		return 4,0,40339,1,0,0
	elseif cur_list[1]==10 then		--宠技彩对券
		return 4,0,40339,1,0,0
	elseif cur_list[1]==12 then		--太一轮五个
		return 4,0,1807,5,0,0
	elseif cur_list[1]==14 then		--太一轮五个
		return 4,0,1807,5,0,0
	else													--宠技彩对券
		return 4,0,40339,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType40529Cashing(cur_list)		---(暗月真解)
	if cur_list[1]==1 then 				--毕戊兑换券
		return 1,0,40531,1,0,0

	elseif cur_list[1]==5 then		--太昊
		return 2,0,25728,1,0,0
	elseif cur_list[1]==9 then		--造化配方礼包
		return 2,0,40527,1,0,0
	elseif cur_list[1]==13 then		--太乙
		return 2,0,25729,1,0,0

	elseif cur_list[1]==3 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==7 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==11 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==15 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--回城卷
		return 4,0,8079,2,0,0
	elseif cur_list[1]==4 then		--虎魄
		return 4,0,25307,1,0,0
	elseif cur_list[1]==6 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--回城卷
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--回城卷
		return 4,0,8079,2,0,0
	elseif cur_list[1]==12 then		--龟灵
		return 4,0,25306,1,0,0
	elseif cur_list[1]==14 then		--虎魄
		return 4,0,25307,1,0,0
	else													--回城卷
		return 4,0,8079,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType40670Cashing(cur_list)		---(八极穹炉)
	if cur_list[1]==1 then 				--乾元灵胎
		return 1,0,40673,1,0,0
	elseif cur_list[1]==5 then		--混元天晶
		return 1,0,40668,1,0,0
	elseif cur_list[1]==9 then		--金精铁玉10
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--乾元灵胎
		return 1,0,40673,1,0,0

	elseif cur_list[1]==3 then		--神行锦囊·初计
		return 2,0,21742,1,0,0
	elseif cur_list[1]==7 then		--逍遥锦囊·初计
		return 2,0,21744,1,0,0
	elseif cur_list[1]==11 then		--太虚锦囊·初计
		return 2,0,21743,1,0,0
	elseif cur_list[1]==15 then		--神行锦囊·初计
		return 2,0,21742,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--龟灵冰晶
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--虎魄冰晶
		return 4,0,25307,1,0,0
	elseif cur_list[1]==10 then		--虎魄冰晶
		return 4,0,25307,1,0,0
	elseif cur_list[1]==16 then		--龟灵冰晶
		return 4,0,25306,1,0,0
	else													--虎魄冰晶
		return 4,0,25307,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType40928Cashing(cur_list)		---(龙蛇演义)
	if cur_list[1]==1 then 				--雷蛇坐骑兑换券
		return 1,0,40924,1,0,0
	elseif cur_list[1]==5 then		--敖弘兑换卷
		return 1,0,23299,1,0,0
	elseif cur_list[1]==9 then		--雷蛇坐骑兑换券
		return 1,0,40924,1,0,0
	elseif cur_list[1]==13 then		--敖蒙兑换券
		return 1,0,28707,1,0,0

	elseif cur_list[1]==3 then		--沙滩装衣服兑换券
		return 2,0,40905,1,0,0
	elseif cur_list[1]==7 then		--沙滩装鞋子兑换券
		return 2,0,40906,1,0,0
	elseif cur_list[1]==11 then		--沙滩装头饰兑换券
		return 2,0,40904,1,0,0
	elseif cur_list[1]==15 then		--沙滩装衣服兑换券
		return 2,0,40905,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--龟灵冰晶
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--虎魄冰晶
		return 4,0,25307,1,0,0
	elseif cur_list[1]==10 then		--虎魄冰晶
		return 4,0,25307,1,0,0
	elseif cur_list[1]==16 then		--龟灵冰晶
		return 4,0,25306,1,0,0
	else													--虎魄冰晶
		return 4,0,25307,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType41080Cashing(cur_list)		---(洛华天羽)
	if cur_list[1]==1 then 				--洛华天羽
		return 1,0,41079,1,0,0
	elseif cur_list[1]==5 then		--乾元灵胎
		return 1,0,40673,1,0,0
	elseif cur_list[1]==9 then		--洛华天羽
		return 1,0,41079,1,0,0
	elseif cur_list[1]==13 then		--混元天晶
		return 1,0,40668,1,0,0

	elseif cur_list[1]==3 then		--御宝神篇
		return 2,0,18797,1,0,0
	elseif cur_list[1]==7 then		--冰封的鸿蒙雪參
		return 2,0,23327,1,0,0
	elseif cur_list[1]==11 then		--无字天书
		return 2,0,11539,1,0,0
	elseif cur_list[1]==15 then		--御宝神篇
		return 2,0,18797,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--神魔秘录
		return 3,0,23296,1,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--神魔秘录
		return 3,0,23296,1,0,0

	elseif cur_list[1]==2 then		--龟灵冰晶
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--虎魄冰晶
		return 4,0,25307,1,0,0
	elseif cur_list[1]==10 then		--虎魄冰晶
		return 4,0,25307,1,0,0
	elseif cur_list[1]==16 then		--龟灵冰晶
		return 4,0,25306,1,0,0
	else													--虎魄冰晶
		return 4,0,25307,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType41307Cashing(cur_list)		---(千华锦萃)
	if cur_list[1]==1 then 				--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==5 then		--先天风云谱
		return 1,0,24903,1,0,0
	elseif cur_list[1]==9 then		--混元天晶九阶
		return 1,0,40668,1,0,0
	elseif cur_list[1]==13 then		--百兽淀魂珠
		return 1,0,21189,1,0,0

	elseif cur_list[1]==3 then		--御宝神篇
		return 2,0,18797,1,0,0
	elseif cur_list[1]==7 then		--秘文石晶
		return 2,0,28587,1,0,0
	elseif cur_list[1]==11 then		--无字天书
		return 2,0,11539,1,0,0
	elseif cur_list[1]==15 then		--幻灵精魄
		return 2,0,24904,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--龟灵冰晶
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--飞天神符
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天神符
		return 4,0,8079,2,0,0
	elseif cur_list[1]==16 then		--虎魄冰晶
		return 4,0,25307,1,0,0
	else													--飞天神符
		return 4,0,8079,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType41391Cashing(cur_list)		---(啸天金哨)
	if cur_list[1]==1 then 				--大黄兑换券
		return 1,0,41390,1,0,0
	elseif cur_list[1]==5 then		--摄魂
		return 1,0,33820,1,0,0
	elseif cur_list[1]==9 then		--天魔血晶
		return 1,0,33821,1,0,0
	elseif cur_list[1]==13 then		--噬血珠
		return 1,0,40338,1,0,0

	elseif cur_list[1]==3 then		--龙火面具男
		return 2,0,19127,1,0,0
	elseif cur_list[1]==7 then		--毒蝎面具男
		return 2,0,19129,1,0,0
	elseif cur_list[1]==11 then		--蝴蝶面饰女
		return 2,0,13306,1,0,0
	elseif cur_list[1]==15 then		--花瓣面饰女
		return 2,0,19130,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--龟灵冰晶
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--龟灵冰晶
		return 4,0,25306,1,0,0
	elseif cur_list[1]==10 then		--虎魄冰晶
		return 4,0,25307,1,0,0
	elseif cur_list[1]==16 then		--虎魄冰晶
		return 4,0,25307,1,0,0
	else													--虎魄冰晶
		return 4,0,25307,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType42338Cashing(cur_list)		---(扶桑巨木)
	if cur_list[1]==1 then 				--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==5 then		--涅槃
		return 1,0,42150,1,0,0
	elseif cur_list[1]==9 then		--混元天晶九阶
		return 1,0,40668,1,0,0
	elseif cur_list[1]==13 then		--涅槃
		return 1,0,42150,1,0,0

	elseif cur_list[1]==3 then		--御宝神篇
		return 2,0,18797,1,0,0
	elseif cur_list[1]==7 then		--秘文石晶
		return 2,0,28587,1,0,0
	elseif cur_list[1]==11 then		--无字天书
		return 2,0,11539,1,0,0
	elseif cur_list[1]==15 then		--幻灵精魄
		return 2,0,24904,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--龟灵冰晶
		return 4,0,25306,1,0,0
	elseif cur_list[1]==8 then		--飞天神符
		return 4,0,8079,2,0,0
	elseif cur_list[1]==10 then		--飞天神符
		return 4,0,8079,2,0,0
	elseif cur_list[1]==16 then		--虎魄冰晶
		return 4,0,25307,1,0,0
	else													--飞天神符
		return 4,0,8079,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType42353Cashing(cur_list)		---(琅嬛琼琚)
	if cur_list[1]==1 then 				--仙御
		return 1,0,42352,1,0,0
	elseif cur_list[1]==5 then		--乾元灵胎
		return 1,0,40673,1,0,0
	elseif cur_list[1]==9 then		--主角时装兑换券
		return 1,0,42399,1,0,0
	elseif cur_list[1]==13 then		--十级铁玉礼包
		return 1,0,32103,1,0,0

	elseif cur_list[1]==3 then		--太乙
		return 2,0,25729,1,0,0
	elseif cur_list[1]==7 then		--造化秘录
		return 2,0,40527,1,0,0
	elseif cur_list[1]==11 then		--阵营全洗兑换券
		return 2,0,42400,1,0,0
	elseif cur_list[1]==15 then		--太昊
		return 2,0,25728,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType42659Cashing(cur_list)		---(测试可堆叠彩票)
	if cur_list[1]==1 then 				--仙御
		return 1,0,42352,1,0,0
	elseif cur_list[1]==5 then		--乾元灵胎
		return 1,0,40673,1,0,0
	elseif cur_list[1]==9 then		--主角时装兑换券
		return 1,0,42399,1,0,0
	elseif cur_list[1]==13 then		--十级铁玉礼包
		return 1,0,32103,1,0,0

	elseif cur_list[1]==3 then		--太乙
		return 2,0,25729,1,0,0
	elseif cur_list[1]==7 then		--造化秘录
		return 2,0,40527,1,0,0
	elseif cur_list[1]==11 then		--阵营全洗兑换券
		return 2,0,42400,1,0,0
	elseif cur_list[1]==15 then		--太昊
		return 2,0,25728,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType42664Cashing(cur_list)		---(测试可堆叠彩票)
	if cur_list[1]==1 then 				--仙御
		return 1,0,42352,1,0,0
	elseif cur_list[1]==5 then		--乾元灵胎
		return 1,0,40673,1,0,0
	elseif cur_list[1]==9 then		--主角时装兑换券
		return 1,0,42399,1,0,0
	elseif cur_list[1]==13 then		--十级铁玉礼包
		return 1,0,32103,1,0,0

	elseif cur_list[1]==3 then		--太乙
		return 2,0,25729,1,0,0
	elseif cur_list[1]==7 then		--造化秘录
		return 2,0,40527,1,0,0
	elseif cur_list[1]==11 then		--阵营全洗兑换券
		return 2,0,42400,1,0,0
	elseif cur_list[1]==15 then		--太昊
		return 2,0,25728,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType42665Cashing(cur_list)		---(吉祥如意)
	if cur_list[1]==1 then 				--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==5 then		--百兽淀魂珠
		return 1,0,21189,1,0,0
	elseif cur_list[1]==7 then		--老母鸡飞剑
		return 1,0,42661,1,0,0
	elseif cur_list[1]==11 then		--老母鸡坐骑
		return 1,0,42662,1,0,0
	elseif cur_list[1]==13 then		--先天风云谱
		return 1,0,24903,1,0,0

	elseif cur_list[1]==3 then		--幻灵精魄
		return 2,0,24904,1,0,0
	elseif cur_list[1]==9 then		--御宝神篇
		return 2,0,18797,1,0,0
	elseif cur_list[1]==15 then		--秘文石晶
		return 2,0,28587,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType42784Cashing(cur_list)		---(混元秘藏)
if cur_list[1]==1 then 				--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==5 then		--乾元灵胎
		return 1,0,40673,1,0,0
	elseif cur_list[1]==7 then		--噬血珠
		return 1,0,10876,1,0,0
	elseif cur_list[1]==11 then		--天魔血晶
		return 1,0,33821,1,0,0
	elseif cur_list[1]==13 then		--混元天晶九阶
		return 1,0,40668,1,0,0

	elseif cur_list[1]==3 then		--结婚大礼包男
		return 2,0,7931,1,0,0
	elseif cur_list[1]==9 then		--无字天书
		return 2,0,11539,1,0,0
	elseif cur_list[1]==15 then		--结婚大礼包女
		return 2,0,7937,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType42868Cashing(cur_list)		---(金玉良橼)
	if cur_list[1]==1 then 				--碧水金晶兽
		return 1,0,42865,1,0,0
	elseif cur_list[1]==5 then		--乾元灵胎
		return 1,0,40673,1,0,0
	elseif cur_list[1]==9 then		--20高级元魂珠礼包
		return 1,0,42862,1,0,0
	elseif cur_list[1]==13 then		--十级铁玉礼包
		return 1,0,32103,1,0,0

	elseif cur_list[1]==3 then		--九筒面具
		return 2,0,42866,1,0,0
	elseif cur_list[1]==7 then		--栖凤建木
		return 2,0,13903,1,0,0
	elseif cur_list[1]==11 then		--云纹天罗
		return 2,0,13904,1,0,0
	elseif cur_list[1]==15 then		--发财面具
		return 2,0,42867,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType43403Cashing(cur_list)		---(2011兔子彩票)
	if cur_list[1]==1 then 				--兔子坐骑
		return 1,0,43466,1,0,0
	elseif cur_list[1]==5 then		--百兽淀魂珠
		return 1,0,21189,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--先天风云谱
		return 1,0,24903,1,0,0

	elseif cur_list[1]==3 then		--逍遥锦囊·初计
		return 2,0,21744,1,0,0
	elseif cur_list[1]==7 then		--神行锦囊·初计
		return 2,0,21742,1,0,0
	elseif cur_list[1]==11 then		--太虚锦囊·初计
		return 2,0,21743,1,0,0
	elseif cur_list[1]==15 then		--行天圣印
		return 2,0,18911,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType44403Cashing(cur_list)		---(天心雪华)
	if cur_list[1]==1 then 				--天琊礼包
		return 1,0,44495,1,0,0
	elseif cur_list[1]==5 then		--混元天晶九阶
		return 1,0,40668,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--乾元灵胎
		return 1,0,40673,1,0,0

	elseif cur_list[1]==3 then		--秘文石晶
		return 2,0,28587,1,0,0
	elseif cur_list[1]==7 then		--御宝神篇
		return 2,0,18797,1,0,0
	elseif cur_list[1]==11 then		--幻灵精魄
		return 2,0,24904,1,0,0
	elseif cur_list[1]==15 then		--三界洗魂令
		return 2,0,42400,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType44638Cashing(cur_list)		---(金龙宝匣)
	if cur_list[1]==1 then 				--120龙筋礼包
		return 1,0,40510,1,0,0
	elseif cur_list[1]==5 then		--百兽淀魂珠
		return 1,0,21189,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--先天风云谱
		return 1,0,24903,1,0,0

	elseif cur_list[1]==3 then		--衍月星云锁
		return 2,0,43570,1,0,0
	elseif cur_list[1]==7 then		--太乙
		return 2,0,25729,1,0,0
	elseif cur_list[1]==11 then		--太昊
		return 2,0,25728,1,0,0
	elseif cur_list[1]==15 then		--造化秘录
		return 2,0,40527,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType44823Cashing(cur_list)		---(鸿蒙宝气)
	if cur_list[1]==1 then 				--天琊
		return 1,0,44495,1,0,0
	elseif cur_list[1]==5 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==9 then		--噬魂
		return 1,0,50620,1,0,0
	elseif cur_list[1]==13 then		--混元天晶九阶
		return 1,0,40668,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType44764Cashing(cur_list)		---(幻缘仙尊)
	if cur_list[1]==1 then 				--主角时装兑换券
		return 1,0,42399,1,0,0
	elseif cur_list[1]==5 then		--20高级元魂珠礼包
		return 1,0,42862,1,0,0
	elseif cur_list[1]==13 then		--十级铁玉礼包
		return 1,0,32103,1,0,0

	elseif cur_list[1]==3 then		--结婚大礼包女
		return 2,0,7937,1,0,0
	elseif cur_list[1]==7 then		--结婚大礼包男
		return 2,0,7931,1,0,0
	elseif cur_list[1]==9 then		--无字天书
		return 2,0,11539,1,0,0
	elseif cur_list[1]==11 then		--结婚大礼包男
		return 2,0,7931,1,0,0
	elseif cur_list[1]==15 then		--结婚大礼包女
		return 2,0,7937,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType44766Cashing(cur_list)		---(惊鸿玉砚)
	if cur_list[1]==1 then 				--墨舞
		return 1,0,44765,1,0,0
	elseif cur_list[1]==5 then		--乾元灵胎
		return 1,0,40673,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--混元天晶九阶
		return 1,0,40668,1,0,0

	elseif cur_list[1]==3 then		--栖凤建木
		return 2,0,13903,1,0,0
	elseif cur_list[1]==7 then		--行天圣印
		return 2,0,18911,1,0,0
	elseif cur_list[1]==11 then		--行天圣印
		return 2,0,18911,1,0,0
	elseif cur_list[1]==15 then		--云纹天罗
		return 2,0,13904,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType50621Cashing(cur_list)		---(太极初章)
	if cur_list[1]==1 then 				--天琊
		return 1,0,44495,1,0,0
	elseif cur_list[1]==5 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==9 then		--噬魂
		return 1,0,50620,1,0,0
	elseif cur_list[1]==13 then		--混元天晶九阶
		return 1,0,40668,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType50824Cashing(cur_list)		---(桃源秘录)
	if cur_list[1]==1 then 				--桃花乌
		return 1,0,50845,1,0,0
	elseif cur_list[1]==5 then		--龙筋礼包
		return 1,0,40510,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--魂珠礼包
		return 1,0,42862,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType50984Cashing(cur_list)		---(九色圣丹)
	if cur_list[1]==1 then 				--圣诞坐骑
		return 1,0,50983,1,0,0
	elseif cur_list[1]==5 then		--混元天晶9阶
		return 1,0,40668,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--乾元灵胎
		return 1,0,40673,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType51097Cashing(cur_list)		---(龙宫仙芝)
	if cur_list[1]==1 then 				--春节坐骑
		return 1,0,51082,1,0,0
	elseif cur_list[1]==5 then		--天琊
		return 1,0,44495,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--噬魂
		return 1,0,50620,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType51203Cashing(cur_list)		---(不灭龙魂)
	if cur_list[1]==1 then 				--春节坐骑
		return 1,0,51202,1,0,0
	elseif cur_list[1]==5 then		--魂珠礼包
		return 1,0,42862,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--龙筋礼包
		return 1,0,40510,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType51267Cashing(cur_list)		---(三生灵石)
	if cur_list[1]==1 then 				--圣灵翅膀
		return 1,0,51290,1,0,0
	elseif cur_list[1]==5 then		--混元天晶9阶
		return 1,0,40668,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--乾元灵胎
		return 1,0,40673,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType51452Cashing(cur_list)		---(五色烟云)
	if cur_list[1]==1 then 				--翰青承云卷坐骑
		return 1,0,51451,1,0,0
	elseif cur_list[1]==5 then		--混元天晶9阶
		return 1,0,40668,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--主角时装兑换券
		return 1,0,42399,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType52346Cashing(cur_list)		---(时光宝鉴)
	if cur_list[1]==1 then 				--梭影飞剑
		return 1,0,52345,1,0,0
	elseif cur_list[1]==5 then		--百计法身兑换券
		return 1,0,52343,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--百计金身兑换券
		return 1,0,52342,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType52457Cashing(cur_list)		---(深海之心)
	if cur_list[1]==1 then 				--沧溟之鳞
		return 1,0,52456,1,0,0
	elseif cur_list[1]==5 then		--天琊
		return 1,0,44495,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--噬魂
		return 1,0,50620,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType52471Cashing(cur_list)		---(夸父)
	if cur_list[1]==1 then 				--夸父兑换券
		return 1,0,52470,1,0,0
	elseif cur_list[1]==5 then		--混元天晶9阶
		return 1,0,40668,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--主角时装兑换券
		return 1,0,42399,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType52680Cashing(cur_list)		---(蝴蝶飞剑)
	if cur_list[1]==1 then 				--蝴蝶飞剑
		return 1,0,52679,1,0,0
	elseif cur_list[1]==5 then		--百计法身兑换券
		return 1,0,52343,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--百计金身兑换券
		return 1,0,52342,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType54054Cashing(cur_list)		---(猴子坐骑)
	if cur_list[1]==1 then 				--猴子坐骑
		return 1,0,54043,1,0,0
	elseif cur_list[1]==5 then		--天琊
		return 1,0,44495,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--噬魂
		return 1,0,50620,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType54858Cashing(cur_list)		---(夜阑听雪)
	if cur_list[1]==1 then 				--幽夜沉香(飞骑）
		return 1,0,54844,1,0,0
	elseif cur_list[1]==5 then		--百计法身兑换券
		return 1,0,52343,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--百计金身兑换券
		return 1,0,52342,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType54886Cashing(cur_list)		---(洞天真火)
	if cur_list[1]==1 then 				--乾元灵胎
		return 1,0,40673,1,0,0
	elseif cur_list[1]==5 then		--主角时装兑换券
		return 1,0,42399,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--龙筋礼包
		return 1,0,40510,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType54929Cashing(cur_list)		---(碧色雕笼)
	if cur_list[1]==1 then 				--子撵
		return 1,0,54919,1,0,0
	elseif cur_list[1]==5 then		--魂珠礼包
		return 1,0,42862,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--魂珠礼包
		return 1,0,40668,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType55541Cashing(cur_list)		---(金蛇狂舞)
	if cur_list[1]==1 then 				--9阶混元天晶
		return 1,0,40668,1,0,0
	elseif cur_list[1]==5 then		--百计法身兑换券
		return 1,0,52343,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--百计金身兑换券
		return 1,0,52342,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType55590Cashing(cur_list)		---(神灯驼)
	if cur_list[1]==1 then 				--天琊
		return 1,0,44495,1,0,0
	elseif cur_list[1]==5 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==9 then		--噬魂
		return 1,0,50620,1,0,0
	elseif cur_list[1]==13 then		--神骆驼
		return 1,0,55596,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType55792Cashing(cur_list)		---(玉楼春)
	if cur_list[1]==1 then 				--末日翅膀兑换券
		return 1,0,55758,1,0,0
	elseif cur_list[1]==5 then		--魂珠礼包
		return 1,0,42862,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--主角时装兑换券
		return 1,0,42399,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==6 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==12 then		--仙豆
		return 3,0,18794,3,0,0
	elseif cur_list[1]==14 then		--仙豆
		return 3,0,18794,3,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType55966Cashing(cur_list)		---(蓦山溪)
	if cur_list[1]==1 then 				--布老虎坐骑
		return 1,0,55969,1,0,0
	elseif cur_list[1]==5 then		--混元天晶9阶
		return 1,0,40668,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--乾元灵胎
		return 1,0,40673,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--造化珠
		return 3,0,21345,500,0,0
	elseif cur_list[1]==6 then		--天龙魔血
		return 3,0,6350,1,0,0
	elseif cur_list[1]==12 then		--造化珠
		return 3,0,21345,500,0,0
	elseif cur_list[1]==14 then		--天龙魔血
		return 3,0,6350,1,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType57426Cashing(cur_list)		---(望仙门)
	if cur_list[1]==1 then 				--曙光翅膀兑换券
		return 1,0,57425,1,0,0
	elseif cur_list[1]==5 then		--百计法身兑换券
		return 1,0,52343,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--百计金身兑换券
		return 1,0,52342,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--造化珠
		return 3,0,21345,500,0,0
	elseif cur_list[1]==6 then		--天龙魔血
		return 3,0,6350,1,0,0
	elseif cur_list[1]==12 then		--造化珠
		return 3,0,21345,500,0,0
	elseif cur_list[1]==14 then		--天龙魔血
		return 3,0,6350,1,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType58313Cashing(cur_list)		---(摸鱼儿)
	if cur_list[1]==1 then 				--招肥兑换券
		return 1,0,58314,1,0,0
	elseif cur_list[1]==5 then		--混元天晶9阶
		return 1,0,40668,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--主角时装兑换券
		return 1,0,42399,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--造化珠
		return 3,0,21345,500,0,0
	elseif cur_list[1]==6 then		--天龙魔血
		return 3,0,6350,1,0,0
	elseif cur_list[1]==12 then		--造化珠
		return 3,0,21345,500,0,0
	elseif cur_list[1]==14 then		--天龙魔血
		return 3,0,6350,1,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType58365Cashing(cur_list)		---(瑞云浓)
	if cur_list[1]==1 then 				--飞剑彩鸢兑换券
		return 1,0,58363,1,0,0
	elseif cur_list[1]==5 then		--混元天晶9阶
		return 1,0,40668,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--龙筋礼包
		return 1,0,40510,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--造化珠
		return 3,0,21345,500,0,0
	elseif cur_list[1]==6 then		--天龙魔血
		return 3,0,6350,1,0,0
	elseif cur_list[1]==12 then		--造化珠
		return 3,0,21345,500,0,0
	elseif cur_list[1]==14 then		--天龙魔血
		return 3,0,6350,1,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType58555Cashing(cur_list)		---(谒金门)
	if cur_list[1]==1 then 				--幽夜沉香(飞骑）
		return 1,0,58554,1,0,0
	elseif cur_list[1]==5 then		--百计法身兑换券
		return 1,0,52343,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--百计金身兑换券
		return 1,0,52342,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--造化珠
		return 3,0,21345,500,0,0
	elseif cur_list[1]==6 then		--天龙魔血
		return 3,0,6350,1,0,0
	elseif cur_list[1]==12 then		--造化珠
		return 3,0,21345,500,0,0
	elseif cur_list[1]==14 then		--天龙魔血
		return 3,0,6350,1,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType58866Cashing(cur_list)		---(洞天神石)
	if cur_list[1]==1 then 				--洞天神石
		return 1,0,58868,1,0,0
	elseif cur_list[1]==5 then		--洞天神石
		return 1,0,58868,1,0,0
	elseif cur_list[1]==9 then		--洞天神石
		return 1,0,58868,1,0,0
	elseif cur_list[1]==13 then		--洞天神石
		return 1,0,58868,1,0,0

	elseif cur_list[1]==3 then		--洞天神石
		return 2,0,58867,1,0,0
	elseif cur_list[1]==7 then		--洞天神石
		return 2,0,58867,1,0,0
	elseif cur_list[1]==11 then		--洞天神石
		return 2,0,58867,1,0,0
	elseif cur_list[1]==15 then		--洞天神石
		return 2,0,58867,1,0,0

	elseif cur_list[1]==4 then		--洞天神石
		return 3,0,57440,100,0,0
	elseif cur_list[1]==6 then		--洞天神石
		return 3,0,57440,100,0,0
	elseif cur_list[1]==12 then		--洞天神石
		return 3,0,57440,100,0,0
	elseif cur_list[1]==14 then		--洞天神石
		return 3,0,57440,100,0,0

	elseif cur_list[1]==2 then		--洞天神石
		return 4,0,57440,10,0,0
	elseif cur_list[1]==8 then		--洞天神石
		return 4,0,57440,10,0,0
	elseif cur_list[1]==10 then		--洞天神石
		return 4,0,57440,10,0,0
	elseif cur_list[1]==16 then		--洞天神石
		return 4,0,57440,10,0,0
	else													--洞天神石
		return 4,0,57440,10,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType60300Cashing(cur_list)		---(鹊桥仙)
	if cur_list[1]==1 then 				--金凤玉露
		return 1,0,60261,1,0,0
	elseif cur_list[1]==5 then		--混元天晶9阶
		return 1,0,40668,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--魂珠礼包
		return 1,0,42862,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--造化珠
		return 3,0,21345,500,0,0
	elseif cur_list[1]==6 then		--天龙魔血
		return 3,0,6350,1,0,0
	elseif cur_list[1]==12 then		--造化珠
		return 3,0,21345,500,0,0
	elseif cur_list[1]==14 then		--天龙魔血
		return 3,0,6350,1,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType61139Cashing(cur_list)		---(少年游)
	if cur_list[1]==1 then 				--天琊
		return 1,0,44495,1,0,0
	elseif cur_list[1]==5 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==9 then		--噬魂
		return 1,0,50620,1,0,0
	elseif cur_list[1]==13 then		--神骆驼
		return 1,0,61138,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--造化珠
		return 3,0,21345,500,0,0
	elseif cur_list[1]==6 then		--天龙魔血
		return 3,0,6350,1,0,0
	elseif cur_list[1]==12 then		--造化珠
		return 3,0,21345,500,0,0
	elseif cur_list[1]==14 then		--天龙魔血
		return 3,0,6350,1,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType62459Cashing(cur_list)		---(渡江云)
	if cur_list[1]==1 then 				--青伞兑换券
		return 1,0,62460,1,0,0
	elseif cur_list[1]==5 then		--混元天晶9阶
		return 1,0,40668,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--龙筋礼包
		return 1,0,40510,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--造化珠
		return 3,0,21345,500,0,0
	elseif cur_list[1]==6 then		--天龙魔血
		return 3,0,6350,1,0,0
	elseif cur_list[1]==12 then		--造化珠
		return 3,0,21345,500,0,0
	elseif cur_list[1]==14 then		--天龙魔血
		return 3,0,6350,1,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType62462Cashing(cur_list)		---(仙豆)
	if cur_list[1]==1 then 				--洞天神石
		return 1,0,62457,1,0,0
	elseif cur_list[1]==5 then		--洞天神石
		return 1,0,62457,1,0,0
	elseif cur_list[1]==9 then		--洞天神石
		return 1,0,62457,1,0,0
	elseif cur_list[1]==13 then		--洞天神石
		return 1,0,62457,1,0,0

	elseif cur_list[1]==3 then		--洞天神石
		return 2,0,62453,1,0,0
	elseif cur_list[1]==7 then		--洞天神石
		return 2,0,62453,1,0,0
	elseif cur_list[1]==11 then		--洞天神石
		return 2,0,62453,1,0,0
	elseif cur_list[1]==15 then		--洞天神石
		return 2,0,62453,1,0,0

	elseif cur_list[1]==4 then		--洞天神石
		return 3,0,18794,30,0,0
	elseif cur_list[1]==6 then		--洞天神石
		return 3,0,18794,30,0,0
	elseif cur_list[1]==12 then		--洞天神石
		return 3,0,18794,30,0,0
	elseif cur_list[1]==14 then		--洞天神石
		return 3,0,18794,30,0,0

	elseif cur_list[1]==2 then		--洞天神石
		return 4,0,18794,3,0,0
	elseif cur_list[1]==8 then		--洞天神石
		return 4,0,18794,3,0,0
	elseif cur_list[1]==10 then		--洞天神石
		return 4,0,18794,3,0,0
	elseif cur_list[1]==16 then		--洞天神石
		return 4,0,18794,3,0,0
	else													--洞天神石
		return 4,0,18794,3,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType62773Cashing(cur_list)		---(落梅风)
	if cur_list[1]==1 then 				--金凤玉露
		return 1,0,62761,1,0,0
	elseif cur_list[1]==5 then		--混元天晶9阶
		return 1,0,40668,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32103,1,0,0
	elseif cur_list[1]==13 then		--魂珠礼包
		return 1,0,42862,1,0,0

	elseif cur_list[1]==3 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==7 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==11 then		--2等奖宝石袋
		return 2,0,49293,1,0,0
	elseif cur_list[1]==15 then		--2等奖宝石袋
		return 2,0,49293,1,0,0

	elseif cur_list[1]==4 then		--造化珠
		return 3,0,21345,500,0,0
	elseif cur_list[1]==6 then		--天龙魔血
		return 3,0,6350,1,0,0
	elseif cur_list[1]==12 then		--造化珠
		return 3,0,21345,500,0,0
	elseif cur_list[1]==14 then		--天龙魔血
		return 3,0,6350,1,0,0

	elseif cur_list[1]==2 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==8 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==10 then		--宝石袋
		return 4,0,42401,1,0,0
	elseif cur_list[1]==16 then		--宝石袋
		return 4,0,42401,1,0,0
	else													--宝石袋
		return 4,0,42401,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
------------------------------------------------煮元宵-------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType55442Cashing(cur_list)		---(测试彩票)
	if cur_list[1]==1 then 				--灵犀
		return 8,0,4015,2,0,0
	elseif cur_list[1]==2 then		--飞天神符
		return 8,0,8079,1,0,0
	elseif cur_list[1]==3 then		--0
		return 8,0,0,0,0,0
	elseif cur_list[1]==4 then		--0
		return 8,0,0,0,0,0

	elseif cur_list[1]==5 then		--归元灵玉
		return 7,0,52257,1,0,0
	elseif cur_list[1]==6 then		--造化珠
		return 7,0,21345,500,0,0
	elseif cur_list[1]==7 then		--0
		return 7,0,0,0,0,0
	elseif cur_list[1]==8 then		--0
		return 7,0,0,0,0,0

	elseif cur_list[1]==9 then		--归元灵玉
		return 6,0,52257,5,0,0
	elseif cur_list[1]==10 then		--星盘充能石
		return 6,0,35444,1,0,0
	elseif cur_list[1]==11 then		--0
		return 6,0,0,0,0,0
	elseif cur_list[1]==12 then		--0
		return 6,0,0,0,0,0

	elseif cur_list[1]==13 then		--归元灵玉
		return 5,0,52257,25,0,0
	elseif cur_list[1]==14 then		--秘闻石晶
		return 5,0,28587,3,0,0
	elseif cur_list[1]==15 then		--0
		return 5,0,0,0,0,0
	elseif cur_list[1]==16 then		--0
		return 5,0,0,0,0,0

	elseif cur_list[1]==17 then		--魂珠礼包
		return 4,0,40512,1,0,0
	elseif cur_list[1]==18 then		--传道宝诏
		return 4,0,55471,1,0,0
	elseif cur_list[1]==19 then		--0
		return 4,0,0,0,0,0
	elseif cur_list[1]==20 then		--0
		return 4,0,0,0,0,0

	elseif cur_list[1]==21 then		--龙筋礼包
		return 3,0,40510,2,0,0
	elseif cur_list[1]==22 then		--无极道藏
		return 3,0,40668,1,0,0
	elseif cur_list[1]==23 then		--天琊礼包
		return 3,0,44495,1,0,0
	elseif cur_list[1]==24 then		--噬魂礼包
		return 3,0,50620,1,0,0
		
	elseif cur_list[1]==25 then		--乾元秘宝
		return 2,0,55511,1,0,0
	elseif cur_list[1]==26 then		--太上符箓
		return 2,0,55512,1,0,0
	elseif cur_list[1]==27 then		--0
		return 2,0,0,0,0,0
	elseif cur_list[1]==28 then		--0
		return 2,0,0,0,0,0
		
	elseif cur_list[1]==29 then		--十铁
		return 1,0,32103,1,0,0
	elseif cur_list[1]==30 then		--妙法华章
		return 1,0,55475,1,0,0
	elseif cur_list[1]==31 then		--无极道果
		return 1,0,55476,1,0,0
	elseif cur_list[1]==32 then		--0
		return 1,0,0,0,0,0
		
	else													--灵犀
		return 8,0,4015,2,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType54909Cashing(cur_list)		---(国战用彩票)
	if cur_list[1]==1 then 				--混元天晶9阶
		return 1,0,53962,1,0,0
	elseif cur_list[1]==5 then		--魂珠礼包
		return 1,0,54912,1,0,0
	elseif cur_list[1]==9 then		--十级铁玉礼包
		return 1,0,32286,1,0,0
	elseif cur_list[1]==13 then		--魂珠礼包
		return 1,0,54912,1,0,0

	elseif cur_list[1]==3 then		--大红
		return 2,0,55218,1,0,0
	elseif cur_list[1]==7 then		--大蓝
		return 2,0,55219,1,0,0
	elseif cur_list[1]==11 then		--大红
		return 2,0,55218,1,0,0
	elseif cur_list[1]==15 then		--大蓝
		return 2,0,55219,1,0,0

	elseif cur_list[1]==4 then		--元神礼包
		return 3,0,41668,250,0,0
	elseif cur_list[1]==6 then		--元神礼包
		return 3,0,41668,250,0,0
	elseif cur_list[1]==12 then		--元神礼包
		return 3,0,41668,250,0,0
	elseif cur_list[1]==14 then		--元神礼包
		return 3,0,41668,250,0,0

	elseif cur_list[1]==2 then		--造化礼包
		return 4,0,22343,250,0,0
	elseif cur_list[1]==8 then		--造化礼包
		return 4,0,22343,250,0,0
	elseif cur_list[1]==10 then		--造化礼包
		return 4,0,22343,250,0,0
	elseif cur_list[1]==16 then		--造化礼包
		return 4,0,22343,250,0,0
	else													--造化礼包
		return 4,0,22343,250,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType61160Cashing(cur_list)		---(任务用帮派彩票)
	if cur_list[1]==1 then 				--十铁绑定
		return 1,0,61211,1,0,0
	elseif cur_list[1]==5 then		--十铁绑定
		return 1,0,61211,1,0,0
	elseif cur_list[1]==9 then		--十铁绑定
		return 1,0,61211,1,0,0
	elseif cur_list[1]==13 then		--十铁绑定
		return 1,0,61211,1,0,0

	elseif cur_list[1]==3 then		--十铁碎片
		return 2,0,49370,1,0,0
	elseif cur_list[1]==7 then		--十铁碎片
		return 2,0,49370,1,0,0
	elseif cur_list[1]==11 then		--十铁碎片
		return 2,0,49370,1,0,0
	elseif cur_list[1]==15 then		--十铁碎片
		return 2,0,49370,1,0,0

	elseif cur_list[1]==4 then		--元神珠
		return 3,0,41668,200,0,0
	elseif cur_list[1]==6 then		--元神珠
		return 3,0,41668,200,0,0
	elseif cur_list[1]==12 then		--元神珠
		return 3,0,41668,200,0,0
	elseif cur_list[1]==14 then		--元神珠
		return 3,0,41668,200,0,0

	elseif cur_list[1]==2 then		--造化珠
		return 4,0,22343,200,0,0
	elseif cur_list[1]==8 then		--造化珠
		return 4,0,22343,200,0,0
	elseif cur_list[1]==10 then		--造化珠
		return 4,0,22343,200,0,0
	elseif cur_list[1]==16 then		--造化珠
		return 4,0,22343,200,0,0
	else													--造化珠
		return 4,0,22343,200,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType54031Cashing(cur_list)		---(任务用天晶彩票)
	if cur_list[1]==1 then 				--八阶天晶
		return 1,0,53961,1,0,0
	elseif cur_list[1]==5 then		--强力buff
		return 1,0,54030,1,0,0
	elseif cur_list[1]==9 then		--八阶天晶
		return 1,0,53961,1,0,0
	elseif cur_list[1]==13 then		--强力buff
		return 1,0,54030,1,0,0

	elseif cur_list[1]==3 then		--风水盘
		return 2,0,51442,10,0,0
	elseif cur_list[1]==7 then		--4阶天晶
		return 2,0,53801,1,0,0
	elseif cur_list[1]==11 then		--风水盘
		return 2,0,51442,10,0,0
	elseif cur_list[1]==15 then		--4阶天晶
		return 2,0,53801,1,0,0

	elseif cur_list[1]==4 then		--3阶天晶
		return 3,0,53802,1,0,0
	elseif cur_list[1]==6 then		--声望卷轴
		return 3,0,54013,1,0,0
	elseif cur_list[1]==12 then		--声望卷轴
		return 3,0,54014,1,0,0
	elseif cur_list[1]==14 then		--声望卷轴
		return 3,0,54015,1,0,0

	elseif cur_list[1]==2 then		--2阶天晶
		return 4,0,53803,1,0,0
	elseif cur_list[1]==8 then		--低级元魂珠
		return 4,0,22459,30,0,0
	elseif cur_list[1]==10 then		--风水盘
		return 4,0,51442,3,0,0
	elseif cur_list[1]==16 then		--元神珠
		return 4,0,41668,80,0,0
	else													--元神珠
		return 4,0,41668,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType52751Cashing(cur_list)		---(天缘果)
	if cur_list[1]==1 then 				--战神炎魔
		return 1,0,20561,1,0,0
	elseif cur_list[1]==9 then		--幻精元魂
		return 1,0,42712,1,0,0
	elseif cur_list[1]==5 then		--称号卷轴
		return 1,0,42710,1,0,0
	elseif cur_list[1]==13 then		--称号卷轴
		return 1,0,42710,1,0,0

	elseif cur_list[1]==3 then		--龙筋
		return 2,0,23360,1,0,0
	elseif cur_list[1]==7 then		--龙筋
		return 2,0,23360,1,0,0
	elseif cur_list[1]==11 then		--龙筋
		return 2,0,23360,1,0,0
	elseif cur_list[1]==15 then		--龙筋
		return 2,0,23360,1,0,0
		
	elseif cur_list[1]==4 then		--天空宝珠
		return 3,0,23858,1,0,0
  elseif cur_list[1]==6 then		--千华宝钱
		return 3,0,41171,1,0,0
	elseif cur_list[1]==12 then		--楚汉青钱
		return 3,0,24708,1,0,0
	elseif cur_list[1]==14 then		--楚汉青钱
		return 3,0,24708,1,0,0

	elseif cur_list[1]==8 then		--造化珠
		return 4,0,22343,100,0,0
	elseif cur_list[1]==10 then		--元神珠
		return 4,0,41668,100,0,0
	elseif cur_list[1]==2 then		--金丹*60
		return 4,0,42720,1,0,0
	else													--金丹*60
		return 4,0,42720,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType50910Cashing(cur_list)		---(任务用金丹彩票)
	if cur_list[1]==1 then 				--金丹
		return 1,0,22103,150,0,0
	elseif cur_list[1]==5 then		--金丹
		return 1,0,22103,150,0,0
	elseif cur_list[1]==9 then		--金丹
		return 1,0,22103,150,0,0
	elseif cur_list[1]==13 then		--金丹
		return 1,0,22103,150,0,0

	elseif cur_list[1]==3 then		--金丹
		return 2,0,22103,120,0,0
	elseif cur_list[1]==7 then		--金丹
		return 2,0,22103,120,0,0
	elseif cur_list[1]==11 then		--金丹
		return 2,0,22103,120,0,0
	elseif cur_list[1]==15 then		--金丹
		return 2,0,22103,120,0,0

	elseif cur_list[1]==4 then		--金丹
		return 3,0,22103,100,0,0
	elseif cur_list[1]==6 then		--金丹
		return 3,0,22103,100,0,0
	elseif cur_list[1]==12 then		--金丹
		return 3,0,22103,100,0,0
	elseif cur_list[1]==14 then		--金丹
		return 3,0,22103,100,0,0

	elseif cur_list[1]==2 then		--金丹
		return 4,0,22103,80,0,0
	elseif cur_list[1]==8 then		--金丹
		return 4,0,22103,80,0,0
	elseif cur_list[1]==10 then		--金丹
		return 4,0,22103,80,0,0
	elseif cur_list[1]==16 then		--金丹
		return 4,0,22103,80,0,0
	else													--金丹
		return 4,0,22103,80,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType50911Cashing(cur_list)		---(任务用造化彩票)
	if cur_list[1]==1 then 				--造化
		return 1,0,22343,300,0,0
	elseif cur_list[1]==5 then		--造化
		return 1,0,22343,300,0,0
	elseif cur_list[1]==9 then		--造化
		return 1,0,22343,300,0,0
	elseif cur_list[1]==13 then		--造化
		return 1,0,22343,300,0,0

	elseif cur_list[1]==3 then		--造化
		return 2,0,22343,250,0,0
	elseif cur_list[1]==7 then		--造化
		return 2,0,22343,250,0,0
	elseif cur_list[1]==11 then		--造化
		return 2,0,22343,250,0,0
	elseif cur_list[1]==15 then		--造化
		return 2,0,22343,250,0,0

	elseif cur_list[1]==4 then		--造化
		return 3,0,22343,200,0,0
	elseif cur_list[1]==6 then		--造化
		return 3,0,22343,200,0,0
	elseif cur_list[1]==12 then		--造化
		return 3,0,22343,200,0,0
	elseif cur_list[1]==14 then		--造化
		return 3,0,22343,200,0,0

	elseif cur_list[1]==2 then		--造化
		return 4,0,22343,150,0,0
	elseif cur_list[1]==8 then		--造化
		return 4,0,22343,150,0,0
	elseif cur_list[1]==10 then		--造化
		return 4,0,22343,150,0,0
	elseif cur_list[1]==16 then		--造化
		return 4,0,22343,150,0,0
	else													--造化
		return 4,0,22343,150,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType50912Cashing(cur_list)		---(任务用元神珠彩票)
	if cur_list[1]==1 then 				--元神
		return 1,0,41668,150,0,0
	elseif cur_list[1]==5 then		--元神
		return 1,0,41668,150,0,0
	elseif cur_list[1]==9 then		--元神
		return 1,0,41668,150,0,0
	elseif cur_list[1]==13 then		--元神
		return 1,0,41668,150,0,0

	elseif cur_list[1]==3 then		--元神
		return 2,0,41668,120,0,0
	elseif cur_list[1]==7 then		--元神
		return 2,0,41668,120,0,0
	elseif cur_list[1]==11 then		--元神
		return 2,0,41668,120,0,0
	elseif cur_list[1]==15 then		--元神
		return 2,0,41668,120,0,0

	elseif cur_list[1]==4 then		--元神
		return 3,0,41668,100,0,0
	elseif cur_list[1]==6 then		--元神
		return 3,0,41668,100,0,0
	elseif cur_list[1]==12 then		--元神
		return 3,0,41668,100,0,0
	elseif cur_list[1]==14 then		--元神
		return 3,0,41668,100,0,0

	elseif cur_list[1]==2 then		--元神
		return 4,0,41668,80,0,0
	elseif cur_list[1]==8 then		--元神
		return 4,0,41668,80,0,0
	elseif cur_list[1]==10 then		--元神
		return 4,0,41668,80,0,0
	elseif cur_list[1]==16 then		--元神
		return 4,0,41668,80,0,0
	else													--元神
		return 4,0,41668,80,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType55493Cashing(cur_list)		---(任务用元神珠彩票)
	if cur_list[1]==1 then 				--元神
		return 1,0,55489,1,0,0
	elseif cur_list[1]==5 then		--元神
		return 1,0,55489,1,0,0
	elseif cur_list[1]==9 then		--元神
		return 1,0,55489,1,0,0
	elseif cur_list[1]==13 then		--元神
		return 1,0,55489,1,0,0

	elseif cur_list[1]==3 then		--元神
		return 2,0,55490,1,0,0
	elseif cur_list[1]==7 then		--元神
		return 2,0,55491,1,0,0
	elseif cur_list[1]==11 then		--元神
		return 2,0,55490,1,0,0
	elseif cur_list[1]==15 then		--元神
		return 2,0,55491,1,0,0

	elseif cur_list[1]==4 then		--元神
		return 3,0,55491,1,0,0
	elseif cur_list[1]==6 then		--元神
		return 3,0,55490,1,0,0
	elseif cur_list[1]==12 then		--元神
		return 3,0,55491,1,0,0
	elseif cur_list[1]==14 then		--元神
		return 3,0,55490,1,0,0

	elseif cur_list[1]==2 then		--元神
		return 4,0,55490,1,0,0
	elseif cur_list[1]==8 then		--元神
		return 4,0,55491,1,0,0
	elseif cur_list[1]==10 then		--元神
		return 4,0,55490,1,0,0
	elseif cur_list[1]==16 then		--元神
		return 4,0,55491,1,0,0
	else													--元神
		return 4,0,55491,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType50913Cashing(cur_list)		---(任务用宠物装备彩票)
	if cur_list[1]==1 then 				--135级面目
		return 1,0,50895,1,0,0
	elseif cur_list[1]==5 then		--135级手足
		return 1,0,50892,1,0,0
	elseif cur_list[1]==9 then		--135级金身
		return 1,0,50889,1,0,0
	elseif cur_list[1]==13 then		--135级运道
		return 1,0,50898,1,0,0

	elseif cur_list[1]==3 then		--105级面目
		return 2,0,50894,1,0,0
	elseif cur_list[1]==7 then		--105级手足
		return 2,0,50891,1,0,0
	elseif cur_list[1]==11 then		--105级金身
		return 2,0,50888,1,0,0
	elseif cur_list[1]==15 then		--105级运道
		return 2,0,50897,1,0,0

	elseif cur_list[1]==4 then		--90级手足
		return 3,0,50890,1,0,0
	elseif cur_list[1]==6 then		--90级手足
		return 3,0,50890,1,0,0
	elseif cur_list[1]==12 then		--90级金身
		return 3,0,50887,1,0,0
	elseif cur_list[1]==14 then		--90级金身
		return 3,0,50887,1,0,0

	elseif cur_list[1]==2 then		--90级面目
		return 4,0,50893,1,0,0
	elseif cur_list[1]==8 then		--90级面目
		return 4,0,50893,1,0,0
	elseif cur_list[1]==10 then		--90级运道
		return 4,0,50896,1,0,0
	elseif cur_list[1]==16 then		--90级运道
		return 4,0,50896,1,0,0
	else													--90级运道
		return 4,0,50896,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType50914Cashing(cur_list)		---(任务用日月星沙包彩票)
	if cur_list[1]==1 then 				--日月星沙包
		return 1,0,44463,4,0,0
	elseif cur_list[1]==5 then		--日月星沙包
		return 1,0,44463,4,0,0
	elseif cur_list[1]==9 then		--日月星沙包
		return 1,0,44463,4,0,0
	elseif cur_list[1]==13 then		--日月星沙包
		return 1,0,44463,4,0,0

	elseif cur_list[1]==3 then		--日月星沙包
		return 2,0,44463,3,0,0
	elseif cur_list[1]==7 then		--日月星沙包
		return 2,0,44463,3,0,0
	elseif cur_list[1]==11 then		--日月星沙包
		return 2,0,44463,3,0,0
	elseif cur_list[1]==15 then		--日月星沙包
		return 2,0,44463,3,0,0

	elseif cur_list[1]==4 then		--日月星沙包
		return 3,0,44463,2,0,0
	elseif cur_list[1]==6 then		--日月星沙包
		return 3,0,44463,2,0,0
	elseif cur_list[1]==12 then		--日月星沙包
		return 3,0,44463,2,0,0
	elseif cur_list[1]==14 then		--日月星沙包
		return 3,0,44463,2,0,0

	elseif cur_list[1]==2 then		--日月星沙包
		return 4,0,44463,1,0,0
	elseif cur_list[1]==8 then		--日月星沙包
		return 4,0,44463,1,0,0
	elseif cur_list[1]==10 then		--日月星沙包
		return 4,0,44463,1,0,0
	elseif cur_list[1]==16 then		--日月星沙包
		return 4,0,44463,1,0,0
	else													--日月星沙包
		return 4,0,44463,1,0,0
	end
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
function LotteryType58780Cashing(cur_list)		---(染色剂)
	if cur_list[1]>0 and cur_list[1]<=12 then
		return 1,0,58719+cur_list[1],1,0,0
	elseif cur_list[1]>12 and cur_list[1]<=24 then
		return 2,0,58719+cur_list[1],1,0,0
	elseif cur_list[1]>24 and cur_list[1]<=36 then
		return 3,0,58719+cur_list[1],1,0,0
	elseif cur_list[1]>36 and cur_list[1]<=48 then
		return 4,0,58719+cur_list[1],1,0,0
	elseif cur_list[1]>48 and cur_list[1]<=60 then
		return 5,0,58719+cur_list[1],1,0,0
	else 
		return 5,0,58779,1,0,0
	end		
end 
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
-------------------------------------------------------------------------------------------------------------
--[[
--   
--      接口函数LotterySelectNumber 
--	说明：表示生成一个彩票中的某位数字
--      参数：
--           id : 彩票的物品 id， 不同的彩票可能有不同的表现
--           max_count: 这张彩票总共会有几个数字组成
--           cur_list: 当前已经生成的彩票号码 #cur_list就可以得到当前生成的号码数量 
--                     cur_list[1] 到 cur_list[#cur_list] 就是当前已经生成的所有号码
--                     标准的概率生成可能无需这些参数
--      返回：这个函数应当返回一个1~255的数值表示本次生成的单个彩票号码，0被认为
--            错误的彩票号码，因而被作为错误情况进行返回．
--
--      ************************************************************************
--
--      接口函数LotteryCashing
--      说明：对一张已经生成完毕的彩票进行兑奖操作
--      参数：
--           id : 彩票的物品 id， 不同的彩票可能有不同的表现
--           cur_list: 当前已经生成的彩票号码 #cur_list就可以得到当前所有的号码
--                     cur_list[1] 到 cur_list[#cur_list] 表示了依照生成顺序的号码
--      返回：兑奖后的结果由此接口返回，兑奖总共有六个整数型参数进行返回。
--          返回的格式为 return b_level b_money b_item b_count b_bind b_period;
--          各个返回值解释如下：
--           b_level: 本次彩票的中奖等级，等级是一个大于等于0的整数
--           b_money: 本次彩票的中奖奖金，奖金是一个大于等于0的整数
--           b_item:　本次彩票的奖励物品id，如果为0或-1则表示无奖品
--           b_count: 本次彩票的奖励物品数目，这个值应当大于等于0并不大于堆叠上限
--           b_bind: 本次中奖发放的物品奖励是否绑定，有效值为0或者1。只有堆叠上限为1这个参数才有意义
--           b_period:本次中奖发放物品奖励是否有有效时间，这是一个大于等于0的整数，大于0表示存在有效值。
--                    只有堆叠上限为1这个参数才有意义。
--         当返回参数个数不满足要求，或者参数的范围不满足要求时，这个函数的返回被认为是错误的。
-------------------------------------------------------------------------------------------------------------
--]]
---
---  有关接口实现　后面的两个接口是一个参考实现?
---　使用if语句进行判断不同的彩票　然后调用不同彩票的生成和判断函数
---

function LotterySelectNumber(id, max_count,cur_list)
	if(id==6576) then
		return LotteryType6576SelectNumber(cur_list,max_count);
	elseif(id==7916) then
		return LotteryType7916SelectNumber(cur_list,max_count);
	elseif(id==10484) then
		return LotteryType10484SelectNumber(cur_list,max_count);
	elseif(id==10722) then
		return LotteryType10722SelectNumber(cur_list,max_count);	
	elseif(id==11537) then
		return LotteryType11537SelectNumber(cur_list,max_count);
	elseif(id==11742) then 
		return LotteryType11742SelectNumber(cur_list,max_count);
	elseif(id==11743) then 
		return LotteryType11743SelectNumber(cur_list,max_count);	
	elseif(id==12006) then 
		return LotteryType12006SelectNumber(cur_list,max_count);		
	elseif(id==13318) then 
		return LotteryType13318SelectNumber(cur_list,max_count);	
	elseif(id==13319) then 
		return LotteryType13319SelectNumber(cur_list,max_count);	
	elseif(id==13377) then 
		return LotteryType13377SelectNumber(cur_list,max_count);	
	elseif (id>=13320 and id<=13339)
			or (id>=18752 and id<=18756)
			or (id>=31493 and id<=31502)	-- 九黎、烈山护符彩票
			or (id>=40357 and id<=40366)	-- 怀光、天华护符彩票
			or (id>=50610 and id<=50619)	-- 焚香、太昊护符彩票
			or (id>=54766	and id<=54770)  -- 辰皇护符彩票
			or (id>=62174	and id<=62178)  -- 牵机护符彩票
			or (id>=62179	and id<=62183)  -- 英招护符彩票
			or (id == 62061	or id == 62062 or id == 62063)
			then
		return LotteryTypeCharmSeason1SelectNumber(cur_list,max_count,id);	
	elseif(id==10555 or id==6560) then 					---空置或用来测试的彩票，无效
		return 0;
	elseif(id==13593) then 
		return LotteryType13593SelectNumber(cur_list,max_count);	
	elseif(id==13905) then 
		return LotteryType13905SelectNumber(cur_list,max_count);
	elseif(id==15677) then 
		return LotteryType15677SelectNumber(cur_list,max_count);
	elseif(id==15866) then 
		return LotteryType15866SelectNumber(cur_list,max_count);
	elseif(id==16550) then 
		return LotteryType16550SelectNumber(cur_list,max_count);
	elseif(id==18283) then 
		return LotteryType18283SelectNumber(cur_list,max_count);
	elseif(id==18799) then 
		return LotteryType18799SelectNumber(cur_list,max_count);
	elseif(id==18890) then 
		return LotteryType18890SelectNumber(cur_list,max_count);
	elseif(id==19079 or id==19081 or id==19082 or id==19087) then 
		return LotteryTypeGoldSeason1SelectNumber(cur_list,max_count);
	elseif(id==19112 or id==19113 or id==19114) then 
		return LotteryTypeItemSeason1SelectNumber(cur_list,max_count);
	elseif(id==19115) then 
		return LotteryTypeItemSeason2SelectNumber(cur_list,max_count);
	elseif(id==19125) then 
		return LotteryType19125SelectNumber(cur_list,max_count);
	elseif(id==19297) then 
		return LotteryType19297SelectNumber(cur_list,max_count);
	elseif(id==19434) then 
		return LotteryType19434SelectNumber(cur_list,max_count);
	elseif(id>=19425 and id<=19433) then 
		return LotteryTypeGoldSeason2SelectNumber(cur_list,max_count);
	elseif(id==19558) then 
		return LotteryType19558SelectNumber(cur_list,max_count);
	elseif(id==19713) then 
		return LotteryType19713SelectNumber(cur_list,max_count);
	elseif(id==20097) then 
		return LotteryType20097SelectNumber(cur_list,max_count);
	elseif(id==20109) then 
		return LotteryType20109SelectNumber(cur_list,max_count);
	elseif(id==20284) then 
		return LotteryType20284SelectNumber(cur_list,max_count);
	elseif(id==20480) then 
		return LotteryType20480SelectNumber(cur_list,max_count);
	elseif(id==20482) then 
		return LotteryType20482SelectNumber(cur_list,max_count);
	elseif(id==20460) then 
		return LotteryType20460SelectNumber(cur_list,max_count);
	elseif(id==21194) then 
		return LotteryType21194SelectNumber(cur_list,max_count);
	elseif(id==21697) then 
		return LotteryType21697SelectNumber(cur_list,max_count);
	elseif(id==21752) then 
		return LotteryType21752SelectNumber(cur_list,max_count);
	elseif(id==22126) then 
		return LotteryType22126SelectNumber(cur_list,max_count);
	elseif(id==22127) then 
		return LotteryType22127SelectNumber(cur_list,max_count);
	elseif(id==23144) then 
		return LotteryType23144SelectNumber(cur_list,max_count);
	elseif(id==23162) then 
		return LotteryType23162SelectNumber(cur_list,max_count);
	elseif(id==23018) then 
		return LotteryType23018SelectNumber(cur_list,max_count);
	elseif(id==23295) then 
		return LotteryType23295SelectNumber(cur_list,max_count);
	elseif(id==23212) then 
		return LotteryType23212SelectNumber(cur_list,max_count);
	elseif(id==23336) then 
		return LotteryType23336SelectNumber(cur_list,max_count);
	elseif(id==23363) then 
		return LotteryType23363SelectNumber(cur_list,max_count);
	elseif(id==23422) then 
		return LotteryType23422SelectNumber(cur_list,max_count);
	elseif(id==23423) then 
		return LotteryType23423SelectNumber(cur_list,max_count);
	elseif(id==24343) then 
		return LotteryType24343SelectNumber(cur_list,max_count);
	elseif(id==24344) then 
		return LotteryType24344SelectNumber(cur_list,max_count);
	elseif(id==24993) then 
		return LotteryType24993SelectNumber(cur_list,max_count);
	elseif(id==24998) then 
		return LotteryType24998SelectNumber(cur_list,max_count);
	elseif(id==24999) then 
		return LotteryType24999SelectNumber(cur_list,max_count);
	elseif(id==25000) then 
		return LotteryType25000SelectNumber(cur_list,max_count);
	elseif(id==25305) then 
		return LotteryType25305SelectNumber(cur_list,max_count);
	elseif(id==25417) then 
		return LotteryType25417SelectNumber(cur_list,max_count);
	elseif(id==25418) then 
		return LotteryType25418SelectNumber(cur_list,max_count);
	elseif(id==25749) then 
		return LotteryType25749SelectNumber(cur_list,max_count);
	elseif(id==25750) then 
		return LotteryType25750SelectNumber(cur_list,max_count);
	elseif(id==28573) then 
		return LotteryType28573SelectNumber(cur_list,max_count);
	elseif(id==28703) then 
		return LotteryType28703SelectNumber(cur_list,max_count);
	elseif(id==32605) then 
		return LotteryType32605SelectNumber(cur_list,max_count);
	elseif(id==32692) then 
		return LotteryType32692SelectNumber(cur_list,max_count);
	elseif(id==33055) then 
		return LotteryType33055SelectNumber(cur_list,max_count);
	elseif(id==33122) then 
		return LotteryType33122SelectNumber(cur_list,max_count);
	elseif(id==33530) then 
		return LotteryType33530SelectNumber(cur_list,max_count);
	elseif(id==33822) then 
		return LotteryType33822SelectNumber(cur_list,max_count);
	elseif(id==33823) then 
		return LotteryType33823SelectNumber(cur_list,max_count);
	elseif(id==33988) then 
		return LotteryType33988SelectNumber(cur_list,max_count);
	elseif(id==34124) then 
		return LotteryType34124SelectNumber(cur_list,max_count);
	elseif(id==34125) then 
		return LotteryType34125SelectNumber(cur_list,max_count);
	elseif(id==34295) then 
		return LotteryType34295SelectNumber(cur_list,max_count);
	elseif(id==34296) then 
		return LotteryType34296SelectNumber(cur_list,max_count);
	elseif(id==34937) then 
		return LotteryType34937SelectNumber(cur_list,max_count);
	elseif(id==35165) then 
		return LotteryType35165SelectNumber(cur_list,max_count);
	elseif(id==35360) then 
		return LotteryType35360SelectNumber(cur_list,max_count);
	elseif(id==35361) then 
		return LotteryType35361SelectNumber(cur_list,max_count);
	elseif(id==35362) then 
		return LotteryType35362SelectNumber(cur_list,max_count);
	elseif(id==35363) then 
		return LotteryType35363SelectNumber(cur_list,max_count);
	elseif(id==40344) then 
		return LotteryType40344SelectNumber(cur_list,max_count);
	elseif(id==40345) then 
		return LotteryType40345SelectNumber(cur_list,max_count);
	elseif(id==40346) then 
		return LotteryType40346SelectNumber(cur_list,max_count);
	elseif(id==40529) then 
		return LotteryType40529SelectNumber(cur_list,max_count);
	elseif(id==40670) then 
		return LotteryType40670SelectNumber(cur_list,max_count);
	elseif(id==40928) then 
		return LotteryType40928SelectNumber(cur_list,max_count);
	elseif(id==41080) then 
		return LotteryType41080SelectNumber(cur_list,max_count);
	elseif(id==41307) then 
		return LotteryType41307SelectNumber(cur_list,max_count);
	elseif(id==41391) then 
		return LotteryType41391SelectNumber(cur_list,max_count);
	elseif(id==42338) then 
		return LotteryType42338SelectNumber(cur_list,max_count);
	elseif(id==42353) then 
		return LotteryType42353SelectNumber(cur_list,max_count);
	elseif(id==42659) then 
		return LotteryType42659SelectNumber(cur_list,max_count);
	elseif(id==42664) then 
		return LotteryType42664SelectNumber(cur_list,max_count);
	elseif(id==42665) then 
		return LotteryType42665SelectNumber(cur_list,max_count);
	elseif(id==42784) then 
		return LotteryType42784SelectNumber(cur_list,max_count);
	elseif(id==42868) then 
		return LotteryType42868SelectNumber(cur_list,max_count);
	elseif(id==43403) then 
		return LotteryType43403SelectNumber(cur_list,max_count);
	elseif(id==44403) then 
		return LotteryType44403SelectNumber(cur_list,max_count);
	elseif(id==44638) then 
		return LotteryType44638SelectNumber(cur_list,max_count);
	elseif(id==44764) then 
		return LotteryType44764SelectNumber(cur_list,max_count);
	elseif(id==44766) then 
		return LotteryType44766SelectNumber(cur_list,max_count);	
	elseif(id==44823) then 
		return LotteryType44823SelectNumber(cur_list,max_count);
	elseif(id==55590) then 
		return LotteryType55590SelectNumber(cur_list,max_count);
	elseif(id==55792) then 
		return LotteryType55792SelectNumber(cur_list,max_count);
	elseif(id==50621) then 
		return LotteryType50621SelectNumber(cur_list,max_count);
	elseif(id==50824) then 
		return LotteryType50824SelectNumber(cur_list,max_count);
	elseif(id==50984) then 
		return LotteryType50984SelectNumber(cur_list,max_count);
	elseif(id==51097) then 
		return LotteryType51097SelectNumber(cur_list,max_count);
	elseif(id==51203) then 
		return LotteryType51203SelectNumber(cur_list,max_count);
	elseif(id==51267) then 
		return LotteryType51267SelectNumber(cur_list,max_count);
	elseif(id==51452) then 
		return LotteryType51452SelectNumber(cur_list,max_count);
	elseif(id==52346) then 
		return LotteryType52346SelectNumber(cur_list,max_count);
	elseif(id==52457) then 
		return LotteryType52457SelectNumber(cur_list,max_count);
	elseif(id==52471) then 
		return LotteryType52471SelectNumber(cur_list,max_count);
	elseif(id==52680) then 
		return LotteryType52680SelectNumber(cur_list,max_count);
	elseif(id==54054) then 
		return LotteryType54054SelectNumber(cur_list,max_count);
	elseif(id==54858) then 
		return LotteryType54858SelectNumber(cur_list,max_count);
	elseif(id==54886) then 
		return LotteryType54886SelectNumber(cur_list,max_count);
	elseif(id==54031) then 
		return LotteryType54031SelectNumber(cur_list,max_count);
	elseif(id==61160) then 
		return LotteryType61160SelectNumber(cur_list,max_count);
	elseif(id==54929) then 
		return LotteryType54929SelectNumber(cur_list,max_count);
	elseif(id==55541) then 
		return LotteryType55541SelectNumber(cur_list,max_count);
	elseif(id==54909) then 
		return LotteryType54909SelectNumber(cur_list,max_count);
	elseif(id==52751) then 
		return LotteryType52751SelectNumber(cur_list,max_count);
	elseif(id==50910) then 
		return LotteryType50910SelectNumber(cur_list,max_count);
	elseif(id==50911) then 
		return LotteryType50911SelectNumber(cur_list,max_count);
	elseif(id==50912) then 
		return LotteryType50912SelectNumber(cur_list,max_count);
	elseif(id==55493) then 
		return LotteryType55493SelectNumber(cur_list,max_count);
	elseif(id==50913) then 
		return LotteryType50913SelectNumber(cur_list,max_count);
	elseif(id==55966) then 
		return LotteryType55966SelectNumber(cur_list,max_count);
	elseif(id==57426) then 
		return LotteryType57426SelectNumber(cur_list,max_count);
	elseif(id==58313) then 
		return LotteryType58313SelectNumber(cur_list,max_count);
	elseif(id==58365) then 
		return LotteryType58365SelectNumber(cur_list,max_count);
	elseif(id==58555) then 
		return LotteryType58555SelectNumber(cur_list,max_count);
	elseif(id==58866) then 
		return LotteryType58866SelectNumber(cur_list,max_count);
	elseif(id==60300) then 
		return LotteryType60300SelectNumber(cur_list,max_count);
	elseif(id==61139) then 
		return LotteryType61139SelectNumber(cur_list,max_count);
	elseif(id==62459) then 
		return LotteryType62459SelectNumber(cur_list,max_count);
	elseif(id==62462) then 
		return LotteryType62462SelectNumber(cur_list,max_count);
	elseif(id==62773) then 
		return LotteryType62773SelectNumber(cur_list,max_count);
	elseif(id==50914) then 
		return LotteryType50914SelectNumber(cur_list,max_count);
	elseif(id==55442) then 
		return LotteryType55442SelectNumber(cur_list,max_count);	
	elseif(id==58780) then 
		return LotteryType58780SelectNumber(cur_list,max_count);	
	else
		return 0;
	end 	
end

function LotteryCashing(id,cur_list)
	if(id == 6576) then
		return LotteryType6576Cashing(cur_list);
	elseif(id == 7916) then
		return LotteryType7916Cashing(cur_list);
	elseif(id == 10484) then
		return LotteryType10484Cashing(cur_list);
	elseif(id==10722) then
		return LotteryType10722Cashing(cur_list);
	elseif(id==11537) then
		return LotteryType11537Cashing(cur_list);	
	elseif(id==11742) then
		return LotteryType11742Cashing(cur_list);	
	elseif(id==11743) then
		return LotteryType11743Cashing(cur_list);	
	elseif(id==12006) then
		return LotteryType12006Cashing(cur_list);
	elseif(id==13318) then
		return LotteryType13318Cashing(cur_list);	
	elseif(id==13319) then
		return LotteryType13319Cashing(cur_list);		
	elseif(id==13377) then
		return LotteryType13377Cashing(cur_list);		
	elseif (id>=13320 and id<=13339) 
			or (id>=18752 and id<=18756)
			or (id>=31493 and id<=31502)	-- 九黎、烈山护符彩票
			or (id>=40357 and id<=40366)	-- 九黎、烈山护符彩票
			or (id>=50610 and id<=50619)	-- 焚香、太昊护符彩票
			or (id>=54766	and id<=54770)  -- 辰皇护符彩票
			or (id>=62174	and id<=62178)  -- 牵机护符彩票
			or (id>=62179	and id<=62183)  -- 英招护符彩票
			or (id == 62061	or id == 62062 or id == 62063)
			then
		return LotteryTypeCharmSeason1Cashing(cur_list,id);	
	elseif(id==10555 or id==6560) then 					---空置或用来测试的彩票，无效
		return 0;
	elseif(id==13593) then 
		return LotteryType13593Cashing(cur_list);	
	elseif(id==13905) then 
		return LotteryType13905Cashing(cur_list);	
	elseif(id==15677) then 
		return LotteryType15677Cashing(cur_list);	
	elseif(id==15866) then 
		return LotteryType15866Cashing(cur_list);
	elseif(id==16550) then 
		return LotteryType16550Cashing(cur_list);
	elseif(id==18283) then 
		return LotteryType18283Cashing(cur_list);
	elseif(id==18799) then 
		return LotteryType18799Cashing(cur_list);
	elseif(id==18890) then 
		return LotteryType18890Cashing(cur_list);
	elseif(id==19079 or id==19081 or id==19082 or id==19087) then
		return LotteryTypeGoldSeason1Cashing(cur_list,id);
	elseif(id==19112 or id==19113 or id==19114) then
		return LotteryTypeItemSeason1Cashing(cur_list,id);
	elseif(id==19115) then
		return LotteryTypeItemSeason2Cashing(cur_list,id);
	elseif(id==19125) then
		return LotteryType19125Cashing(cur_list);
	elseif(id==19297) then
		return LotteryType19297Cashing(cur_list);
	elseif(id==19434) then
		return LotteryType19434Cashing(cur_list);
	elseif(id>=19425 and id<=19433) then
		return LotteryTypeGoldSeason2Cashing(cur_list,id);
	elseif(id==19558) then
		return LotteryType19558Cashing(cur_list);
	elseif(id==19713) then
		return LotteryType19713Cashing(cur_list);
	elseif(id==20097) then
		return LotteryType20097Cashing(cur_list);
	elseif(id==20109) then
		return LotteryType20109Cashing(cur_list);
	elseif(id==20284) then
		return LotteryType20284Cashing(cur_list);
	elseif(id==20480) then
		return LotteryType20480Cashing(cur_list);
	elseif(id==20482) then
		return LotteryType20482Cashing(cur_list);
	elseif(id==20460) then
		return LotteryType20460Cashing(cur_list);
	elseif(id==21194) then
		return LotteryType21194Cashing(cur_list);
	elseif(id==21697) then
		return LotteryType21697Cashing(cur_list);
	elseif(id==21752) then
		return LotteryType21752Cashing(cur_list);
	elseif(id==22126) then
		return LotteryType22126Cashing(cur_list);
	elseif(id==22127) then
		return LotteryType22127Cashing(cur_list);
	elseif(id==23144) then
		return LotteryType23144Cashing(cur_list);
	elseif(id==23162) then
		return LotteryType23162Cashing(cur_list);
	elseif(id==23018) then
		return LotteryType23018Cashing(cur_list);
	elseif(id==23295) then
		return LotteryType23295Cashing(cur_list);
	elseif(id==23212) then
		return LotteryType23212Cashing(cur_list);
	elseif(id==23336) then
		return LotteryType23336Cashing(cur_list);
	elseif(id==23363) then
		return LotteryType23363Cashing(cur_list);
	elseif(id==23422) then
		return LotteryType23422Cashing(cur_list);
	elseif(id==23423) then
		return LotteryType23423Cashing(cur_list);
	elseif(id==24343) then
		return LotteryType24343Cashing(cur_list);
	elseif(id==24344) then
		return LotteryType24344Cashing(cur_list);
	elseif(id==24993) then
		return LotteryType24993Cashing(cur_list);
	elseif(id==24998) then
		return LotteryType24998Cashing(cur_list);
	elseif(id==24999) then
		return LotteryType24999Cashing(cur_list);
	elseif(id==25000) then
		return LotteryType25000Cashing(cur_list);
	elseif(id==25305) then
		return LotteryType25305Cashing(cur_list);
	elseif(id==25417) then
		return LotteryType25417Cashing(cur_list);
	elseif(id==25418) then
		return LotteryType25418Cashing(cur_list);
	elseif(id==25749) then
		return LotteryType25749Cashing(cur_list);
	elseif(id==25750) then
		return LotteryType25750Cashing(cur_list);
	elseif(id==28573) then
		return LotteryType28573Cashing(cur_list);
	elseif(id==28703) then
		return LotteryType28703Cashing(cur_list);
	elseif(id==32605) then
		return LotteryType32605Cashing(cur_list);
	elseif(id==32692) then
		return LotteryType32692Cashing(cur_list);
	elseif(id==33055) then
		return LotteryType33055Cashing(cur_list);
	elseif(id==33122) then
		return LotteryType33122Cashing(cur_list);
	elseif(id==33530) then
		return LotteryType33530Cashing(cur_list);
	elseif(id==33822) then
		return LotteryType33822Cashing(cur_list);
	elseif(id==33823) then
		return LotteryType33823Cashing(cur_list);
	elseif(id==33988) then
		return LotteryType33988Cashing(cur_list);
	elseif(id==34124) then
		return LotteryType34124Cashing(cur_list);
	elseif(id==34125) then
		return LotteryType34125Cashing(cur_list);
	elseif(id==34295) then
		return LotteryType34295Cashing(cur_list);
	elseif(id==34296) then
		return LotteryType34296Cashing(cur_list);
	elseif(id==34937) then
		return LotteryType34937Cashing(cur_list);
	elseif(id==35165) then
		return LotteryType35165Cashing(cur_list);
	elseif(id==35360) then
		return LotteryType35360Cashing(cur_list);
	elseif(id==35361) then
		return LotteryType35361Cashing(cur_list);
	elseif(id==35362) then
		return LotteryType35362Cashing(cur_list);
	elseif(id==35363) then
		return LotteryType35363Cashing(cur_list);
	elseif(id==40344) then
		return LotteryType40344Cashing(cur_list);
	elseif(id==40345) then
		return LotteryType40345Cashing(cur_list);
	elseif(id==40346) then
		return LotteryType40346Cashing(cur_list);
	elseif(id==40529) then
		return LotteryType40529Cashing(cur_list);
	elseif(id==40670) then
		return LotteryType40670Cashing(cur_list);
	elseif(id==40928) then
		return LotteryType40928Cashing(cur_list);
	elseif(id==41080) then
		return LotteryType41080Cashing(cur_list);
	elseif(id==41307) then
		return LotteryType41307Cashing(cur_list);
	elseif(id==41391) then
		return LotteryType41391Cashing(cur_list);
	elseif(id==42338) then
		return LotteryType42338Cashing(cur_list);
	elseif(id==42353) then
		return LotteryType42353Cashing(cur_list);
	elseif(id==42659) then
		return LotteryType42659Cashing(cur_list);
	elseif(id==42664) then
		return LotteryType42664Cashing(cur_list);
	elseif(id==42665) then
		return LotteryType42665Cashing(cur_list);
	elseif(id==42784) then
		return LotteryType42784Cashing(cur_list);
	elseif(id==42868) then
		return LotteryType42868Cashing(cur_list);
	elseif(id==43403) then
		return LotteryType43403Cashing(cur_list);
	elseif(id==44403) then
		return LotteryType44403Cashing(cur_list);
	elseif(id==44638) then
		return LotteryType44638Cashing(cur_list);
	elseif(id==44764) then
		return LotteryType44764Cashing(cur_list);
	elseif(id==44766) then
		return LotteryType44766Cashing(cur_list);
	elseif(id==44823) then
		return LotteryType44823Cashing(cur_list);
	elseif(id==55590) then
		return LotteryType55590Cashing(cur_list);
	elseif(id==55792) then
		return LotteryType55792Cashing(cur_list);
	elseif(id==50621) then
		return LotteryType50621Cashing(cur_list);
	elseif(id==50824) then
		return LotteryType50824Cashing(cur_list);
	elseif(id==50984) then
		return LotteryType50984Cashing(cur_list);
	elseif(id==51097) then
		return LotteryType51097Cashing(cur_list);
	elseif(id==51203) then
		return LotteryType51203Cashing(cur_list);
	elseif(id==51267) then
		return LotteryType51267Cashing(cur_list);
	elseif(id==51452) then
		return LotteryType51452Cashing(cur_list);
	elseif(id==52346) then
		return LotteryType52346Cashing(cur_list);
	elseif(id==52457) then
		return LotteryType52457Cashing(cur_list);
	elseif(id==52471) then
		return LotteryType52471Cashing(cur_list);
	elseif(id==52680) then
		return LotteryType52680Cashing(cur_list);
	elseif(id==54054) then
		return LotteryType54054Cashing(cur_list);
	elseif(id==54858) then
		return LotteryType54858Cashing(cur_list);
	elseif(id==54886) then
		return LotteryType54886Cashing(cur_list);
	elseif(id==54031) then
		return LotteryType54031Cashing(cur_list);
	elseif(id==61160) then
		return LotteryType61160Cashing(cur_list);
	elseif(id==54929) then
		return LotteryType54929Cashing(cur_list);
	elseif(id==55541) then
		return LotteryType55541Cashing(cur_list);
	elseif(id==54909) then
		return LotteryType54909Cashing(cur_list);
	elseif(id==52751) then
		return LotteryType52751Cashing(cur_list);
	elseif(id==50910) then
		return LotteryType50910Cashing(cur_list);
	elseif(id==50911) then
		return LotteryType50911Cashing(cur_list);
	elseif(id==50912) then
		return LotteryType50912Cashing(cur_list);
	elseif(id==55493) then
		return LotteryType55493Cashing(cur_list);
	elseif(id==50913) then
		return LotteryType50913Cashing(cur_list);
	elseif(id==55966) then
		return LotteryType55966Cashing(cur_list);
	elseif(id==57426) then
		return LotteryType57426Cashing(cur_list);
	elseif(id==58313) then
		return LotteryType58313Cashing(cur_list);
	elseif(id==58365) then
		return LotteryType58365Cashing(cur_list);
	elseif(id==58555) then
		return LotteryType58555Cashing(cur_list);
	elseif(id==58866) then
		return LotteryType58866Cashing(cur_list);
	elseif(id==60300) then
		return LotteryType60300Cashing(cur_list);
	elseif(id==61139) then
		return LotteryType61139Cashing(cur_list);
	elseif(id==62459) then
		return LotteryType62459Cashing(cur_list);
	elseif(id==62462) then
		return LotteryType62462Cashing(cur_list);
	elseif(id==62773) then
		return LotteryType62773Cashing(cur_list);
	elseif(id==50914) then
		return LotteryType50914Cashing(cur_list);
	elseif(id==55442) then
		return LotteryType55442Cashing(cur_list);
	elseif(id==58780) then
		return LotteryType58780Cashing(cur_list);

	else
		return 0;	-- 返回错误
	end 	
end

-------------------------------------------------------------------------------------------------------------
--[[
--  另外一个备选实现　使用table保存所有的彩票接口　调用时直接查询并调用之
--	才后面要用AddLotteryTemplate　将每种彩票的映射加入到其中.
--	这只是一个提示性代码，接口的名字被修改为XXXX_aux，主程序并不会调用这个名称的接口函数
--

function LotterySelectNumber_aux(id, max_count, cur_list)
	local m = lottery_template[id];
	if(m) then
	return m.Select(cur_list, max_count);
	end
	return 0; 	-- 返回错误
end

function LotteryCashing_aux(id , cur_list)
	local m = lottery_template[id];
	if(m) then
	return m.Cashing(cur_list);
	end
	return 0;	-- 返回错误
end

AddLotteryTemplate(3613, LotteryType3613SelectNumber,LotteryType3613Cashing);
--]]