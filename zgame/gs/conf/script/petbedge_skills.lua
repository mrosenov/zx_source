--[[
//添加新宠物需要设置的数据区

1.根据宠种类和来源设置技能和技能等级
ZLuaPetBedgeSkills(pet_tid,horoscope,origin)

2.根据宠种类和来源设置天赋
ZLuaPetBedgeTalents(pet_tid,origin)

3.根据宠种类和来源设置生活相关能力
ZLuaPetBedgeLives(pet_tid,origin)

4.宠物技能因宠物天赋获取的级别收益
ZLuaPetBedgeSkillBenefitByTalenet(skill_id,talentslist,pet_level)

5.宠物轮回技能添加
ZLuaPetBedgeFatalitySkills(pet_tid,origin)

--]]

----根据宠物id决定技能和技能等级----------------------------------------------------------
function ZLuaPetBedgeSkills(pet_tid,pet,horoscope,origin)
 	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	local AvailablePets=ZLuaAvailablePets()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local serverorigin = origin or ServerOrigins["drop"]
	
	local skillslist={}		---//技能列表
	local skillschance={}	---//每个技能出现的概率
	local choosenum=3				---//选取的技能数量
	local maxsamecol=1			---//每个技能最多出现的次数
	local maxskillslevel=1	---//每个技能最多出现的级别
	
	local choosenumchance={250,500,1000,250,0,0,0}		---//出现技能的数量
	
	if pet:QueryRebornCnt() == 1 then 
		choosenumchance = {250,500,1000,250,80,50,30}
	end
	
	choosenum=math.max(math.min(ZPetGrade_Get_RanPoint(choosenumchance),7),1)
			
	local choosedskills={}
	local returnedskills={}
	local returnedskillslevel={}

	local Horoscope=ZLuaPetBedgeHoroscopeDesc()

	if 	pet:QueryRebornCnt() == 0 then 
	
  	if horoscope==Horoscope["riyao"]	then 
  		skillslist=		{1399,1400,1401,1402,1403,1404,1453,1454,1455,1456,1457,1458}
  		skillschance=	{1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000}
  	elseif horoscope==Horoscope["yueyao"]	then 
  		skillslist=		{1405,1406,1407,1408,1409,1410,1453,1454,1455,1456,1457,1458}
  		skillschance=	{1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000}
  	elseif horoscope==Horoscope["jinyao"]	then 
  		skillslist=		{1411,1412,1413,1414,1415,1416,1453,1454,1455,1456,1457,1458}
  		skillschance=	{1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000}
  	elseif horoscope==Horoscope["muyao"] then
  		skillslist=		{1417,1418,1419,1420,1421,1422,1453,1454,1455,1456,1457,1458}
  		skillschance=	{1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000}	
  	elseif horoscope==Horoscope["shuiyao"] then
  		skillslist=		{1423,1424,1425,1426,1427,1428,1453,1454,1455,1456,1457,1458}
  		skillschance=	{1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000}	
  	elseif horoscope==Horoscope["huoyao"]	then 
  		skillslist=		{1429,1430,1431,1432,1433,1434,1453,1454,1455,1456,1457,1458}
  		skillschance=	{1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000}
  	elseif horoscope==Horoscope["tuyao"] then	
  		skillslist=		{1435,1436,1437,1438,1439,1440,1453,1454,1455,1456,1457,1458}
  		skillschance=	{1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000}
  	elseif horoscope==Horoscope["luohou"]	then 
  		skillslist=		{1441,1442,1443,1444,1445,1446,1453,1454,1455,1456,1457,1458}
  		skillschance=	{1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000}
  	elseif horoscope==Horoscope["jidu"]	then 
  		skillslist=		{1447,1448,1449,1450,1451,1452,1453,1454,1455,1456,1457,1458}
  		skillschance=	{1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,1000}
  	else
  		skillslist=		{1453,1454,1455,1456,1457,1458}
  		skillschance=	{1000,1000,1000,1000,1000,1000}
  	end	
	
	elseif pet:QueryRebornCnt() == 1 then 
		
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
	
	end
		
	if choosenum>7 or choosenum>table.getn(skillslist) or table.getn(skillslist)~=table.getn(skillschance) then return end 
	choosedskills=ZLuaCheckOutNums(skillslist,skillschance,choosenum,maxsamecol)
	
	if pet_tid==AvailablePets["S_luxueqi"].pet_tid then
		choosedskills={1261,1273}
	elseif pet_tid==AvailablePets["S_luxueqi2"].pet_tid then
		choosedskills={1274,1275}
	elseif pet_tid==AvailablePets["S_biyao"].pet_tid then
		choosedskills={247,314,526,777}
	end 	
	returnedskills,returnedskillslevel=ZLuaPetBedgeMakeSkillsTable(choosedskills,choosenum,maxskillslevel)
	
	return returnedskills,returnedskillslevel
end 
------------------------------------------------------------------------------------------
----根据宠物id和来源设定天赋--------------------------------------------------------------
function ZLuaPetBedgeTalents(pet_tid,origin)
 	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	local AvailablePets=ZLuaAvailablePets()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local serverorigin = origin or ServerOrigins["drop"]
	
	local TalentsList={}
	local talentschance={}	---//每种天赋组合出现的概率
	local talentschoosed		---//选取的天赋组合
	
	---//13中天赋组合
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
		talentschance={10,10,10,10,10,10,10,10,10,10,10,10,10} 
		talentschoosed=ZPetGrade_Get_RanPoint(talentschance)

	if talentschoosed>13 or talentschoosed<1  then talentschoosed=13 end 

	return TalentsList[talentschoosed]
end 
------------------------------------------------------------------------------------------
----根据宠物id决定天命(轮回)技能---------------------------------------------------------------
function ZLuaPetBedgeFatalitySkills(pet_tid,origin)
	local AvailablePets=ZLuaAvailablePets()
	local fatalskills={1368,1369,1370,1371,1372,1373,1374,1375,1376,1377,
										 1378,1379,1380,1381,1382,1383,1384,1385,1386,1387,
										 1388,1389,1390,1391,1392,1393,1394,1397,1398}

	local fatalchance={1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
										 1000,1000,1000,1000,1000,1000,1000,1000,1000,1000,
										 1000,1000,1000,1000,1000,1000,1000,1000,1000}
	local fatalchoosed=fatalskills[ZPetGrade_Get_RanPoint(fatalchance)]
	
	if pet_tid==AvailablePets["S_luxueqi"].pet_tid then
		fatalchoosed=1395
	elseif pet_tid==AvailablePets["S_biyao"].pet_tid then
		fatalchoosed=1396
	end 
	
	return fatalchoosed
end 
------------------------------------------------------------------------------------------
------根据宠物id和来源设定生活相关数据----------------------------------------------------
function ZLuaPetBedgeLives(pet_tid,origin)
		--[[
		age				LivesAttr[1] 			acuity 		LivesAttr[7]	
		life			LivesAttr[2]			stamina 	LivesAttr[8]	
		maxhunger	LivesAttr[3]			sawy 			LivesAttr[9] 
		curhunger	LivesAttr[4]			dowry 		LivesAttr[10]		
		maxhonor	LivesAttr[5] 			wisdom 		LivesAttr[11]	
		curhonor	LivesAttr[6]			cnr 			LivesAttr[12]
		--]]
 	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	local AvailablePets=ZLuaAvailablePets()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local serverorigin = origin or ServerOrigins["drop"]
	local LivesAttr={}

	local AddLifeMulti=1							---//因宠物来源不同增加的寿命影响因子
	
	if origin==ServerOrigins["gshop"] then 
		AddLifeMulti=1.2
	elseif 	origin==ServerOrigins["npc"] then
		AddLifeMulti=0.8
	end 
	if pet_tid==AvailablePets["S_luxueqi"].pet_tid or pet_tid==AvailablePets["S_biyao"].pet_tid then 		
	---//陆雪琪的守护/碧瑶的爱之心
		LivesAttr[1]=ZLuaChooseNormalNums(10,20,1)
		LivesAttr[2]=1200
		LivesAttr[3]=ZLuaChooseNormalNums(500,500,1)
		LivesAttr[4]=ZLuaChooseNormalNums(500,500,1)
		LivesAttr[5]=ZLuaChooseNormalNums(240,240,1)
		LivesAttr[6]=ZLuaChooseNormalNums(20,20,1)
		
		LivesAttr[7],LivesAttr[8],LivesAttr[9],LivesAttr[10],
		LivesAttr[11],LivesAttr[12]=ZLuaPetBedgeLivesAbility(pet_tid,origin)
	elseif pet_tid==AvailablePets["Q_heyangqingwa"].pet_tid or pet_tid==AvailablePets["Q_heyangyelang"].pet_tid then 		
	---//河阳食物宠
		LivesAttr[1]=ZLuaChooseNormalNums(10,20,1)
		LivesAttr[2]=4320
		LivesAttr[3]=ZLuaChooseNormalNums(250,250,1)
		LivesAttr[4]=ZLuaChooseNormalNums(250,250,1)
		LivesAttr[5]=ZLuaChooseNormalNums(80,80,1)
		LivesAttr[6]=ZLuaChooseNormalNums(10,10,1)
		
		LivesAttr[7],LivesAttr[8],LivesAttr[9],LivesAttr[10],
		LivesAttr[11],LivesAttr[12]=ZLuaPetBedgeLivesAbility(pet_tid,origin)
	else
		LivesAttr[1]=ZLuaChooseNormalNums(10,20,1)
		LivesAttr[2]=ZLuaChooseNormalNums(8000*AddLifeMulti,10000*AddLifeMulti,1)
		LivesAttr[3]=ZLuaChooseNormalNums(500,500,1)
		LivesAttr[4]=ZLuaChooseNormalNums(500,500,1)
		LivesAttr[5]=ZLuaChooseNormalNums(240,240,1)
		LivesAttr[6]=ZLuaChooseNormalNums(20,20,1)
		
		LivesAttr[7],LivesAttr[8],LivesAttr[9],LivesAttr[10],
		LivesAttr[11],LivesAttr[12]=ZLuaPetBedgeLivesAbility(pet_tid,origin)
	end 
	return LivesAttr
end 
------根据宠物id设定生活相关数据(生产能力值部分)----------------------------------------------------
function ZLuaPetBedgeLivesAbility(pet_tid,origin)
 	if ZLuaPetBedgeExistInTmplIDs(pet_tid) == 0 then return end
	local AvailablePets=ZLuaAvailablePets()
	local ServerOrigins=ZLuaPetBedgeOrigins()
	local serverorigin = origin or ServerOrigins["drop"]
	local Race=ZLuaPetBedgeRaceDesc()
	
	local pet,petrace
	petrace=Race["renzu"]
	
	for _,pet in pairs(AvailablePets) do
		if pet.pet_tid == pet_tid then 
			petrace=pet.race 
		end
	end
	local P_acuity,P_stamina,P_sawy,P_dowry,P_wisdom,P_cnr=0,0,0,0,0,0

	if petrace==Race["renzu"] then 					---//人族
		P_acuity=ZLuaChooseNormalNums(5,15,1)
		P_stamina=0
		P_sawy=0
		P_dowry=ZLuaChooseNormalNums(5,15,1)
		P_wisdom=0
		P_cnr=ZLuaChooseNormalNums(15,25,1)
	elseif petrace==Race["yaozu"] then			---//妖族
		P_acuity=0
		P_stamina=0
		P_sawy=ZLuaChooseNormalNums(5,15,1)
		P_dowry=ZLuaChooseNormalNums(15,25,1)
		P_wisdom=ZLuaChooseNormalNums(5,15,1)
		P_cnr=0
	elseif petrace==Race["guizu"] then			---//鬼族
		P_acuity=0
		P_stamina=ZLuaChooseNormalNums(5,15,1)
		P_sawy=ZLuaChooseNormalNums(15,25,1)
		P_dowry=ZLuaChooseNormalNums(5,15,1)
		P_wisdom=0
		P_cnr=0
	elseif petrace==Race["yuzu"] then				---//羽族
		P_acuity=ZLuaChooseNormalNums(15,25,1)
		P_stamina=0
		P_sawy=ZLuaChooseNormalNums(5,15,1)
		P_dowry=ZLuaChooseNormalNums(5,15,1)
		P_wisdom=0
		P_cnr=0
	elseif petrace==Race["meizu"] then			---//魅族
		P_acuity=ZLuaChooseNormalNums(5,15,1)
		P_stamina=0
		P_sawy=0
		P_dowry=ZLuaChooseNormalNums(15,25,1)
		P_wisdom=0
		P_cnr=0
	elseif petrace==Race["tianjiang"] then	---//天将
		P_acuity=0
		P_stamina=ZLuaChooseNormalNums(5,15,1)
		P_sawy=0
		P_dowry=0
		P_wisdom=ZLuaChooseNormalNums(15,25,1)
		P_cnr=0
	elseif petrace==Race["xianling"] then		---//仙灵
		P_acuity=ZLuaChooseNormalNums(3,15,1)
		P_stamina=ZLuaChooseNormalNums(3,15,1)
		P_sawy=ZLuaChooseNormalNums(3,15,1)
		P_dowry=ZLuaChooseNormalNums(3,15,1)
		P_wisdom=ZLuaChooseNormalNums(3,15,1)
		P_cnr=ZLuaChooseNormalNums(3,15,1)
	elseif petrace==Race["shanjing"] then		---//山精
		P_acuity=0
		P_stamina=ZLuaChooseNormalNums(5,15,1)
		P_sawy=ZLuaChooseNormalNums(5,15,1)
		P_dowry=0
		P_wisdom=ZLuaChooseNormalNums(15,25,1)
		P_cnr=0
	elseif petrace==Race["yeshou"] then			---//野兽
		P_acuity=0
		P_stamina=ZLuaChooseNormalNums(15,25,1)
		P_sawy=ZLuaChooseNormalNums(5,15,1)
		P_dowry=0
		P_wisdom=0
		P_cnr=0
	elseif petrace==Race["yishou"] then			---//异兽
		P_acuity=0
		P_stamina=ZLuaChooseNormalNums(5,15,1)
		P_sawy=0
		P_dowry=0
		P_wisdom=ZLuaChooseNormalNums(5,15,1)
		P_cnr=ZLuaChooseNormalNums(15,25,1)
	elseif petrace==Race["shuizu"] then			---//水族
		P_acuity=ZLuaChooseNormalNums(15,25,1)
		P_stamina=0
		P_sawy=0
		P_dowry=0
		P_wisdom=0
		P_cnr=ZLuaChooseNormalNums(5,15,1)
	elseif petrace==Race["chonggu"] then		---//虫蛊
		P_acuity=0
		P_stamina=0
		P_sawy=0
		P_dowry=0
		P_wisdom=ZLuaChooseNormalNums(5,15,1)
		P_cnr=ZLuaChooseNormalNums(15,25,1)
	elseif petrace==Race["huanling"] then		---//幻灵
		P_acuity=ZLuaChooseNormalNums(5,15,1)
		P_stamina=0
		P_sawy=0
		P_dowry=ZLuaChooseNormalNums(15,25,1)
		P_wisdom=0
		P_cnr=ZLuaChooseNormalNums(5,15,1)
	elseif petrace==Race["siling"] then			---//死灵
		P_acuity=0
		P_stamina=ZLuaChooseNormalNums(5,15,1)
		P_sawy=0
		P_dowry=ZLuaChooseNormalNums(5,15,1)
		P_wisdom=ZLuaChooseNormalNums(15,25,1)
		P_cnr=0
	elseif petrace==Race["yuansu"] then			---//元素
		P_acuity=ZLuaChooseNormalNums(5,15,1)
		P_stamina=0
		P_sawy=ZLuaChooseNormalNums(15,25,1)
		P_dowry=0
		P_wisdom=0
		P_cnr=ZLuaChooseNormalNums(5,15,1)
	elseif petrace==Race["teshu"] then			---//特殊
		P_acuity=0
		P_stamina=0
		P_sawy=0
		P_dowry=0
		P_wisdom=0
		P_cnr=0
	end 	
	---//生产测试宠物
	if pet_tid==AvailablePets["TestPet"].pet_tid then 
		P_acuity=80
		P_stamina=80
		P_sawy=80
		P_dowry=80
		P_wisdom=80
		P_cnr=80
	elseif pet_tid==AvailablePets["S_luxueqi"].pet_tid or pet_tid==AvailablePets["Q_heyangqingwa"].pet_tid 
			or pet_tid==AvailablePets["Q_heyangyelang"].pet_tid or pet_tid==AvailablePets["S_biyao"].pet_tid then 
		P_acuity=0
		P_stamina=0
		P_sawy=0
		P_dowry=0
		P_wisdom=0
		P_cnr=0
	end 
	return P_acuity,P_stamina,P_sawy,P_dowry,P_wisdom,P_cnr
end 
------------------------------------------------------------------------------------------
---宠物技能从天赋获取的级别收益(适用于非种族/职业技能)
function ZLuaPetBedgeSkillBenefitByTalenet(skill_id,talentslist,pet_level)
	if skill_id==0 or type(skill_id)~="number" then return 0 end 
		
	local SkillsBindingTalents={}
	local TalentsID={}
		TalentsID[1210]=1			---//雷火质　	攻击增加类祝福效果增强；	攻击削减类诅咒效果增强
		TalentsID[1211]=2			---//气旋质　	真气/精力增益类祝福效果增强；	真气/精力凝滞类诅咒效果增强
		TalentsID[1212]=3			---//空冥质　	魅惑类诅咒效果增强；	禁食类/嘲讽类诅咒效果增强
		TalentsID[1213]=4			---//真血质　	防御增加类祝福效果增强；	气血流失类诅咒效果增强
		TalentsID[1214]=5			---//定心质　	定身类诅咒效果增强；	减速类/狂热类诅咒效果增强
		TalentsID[1215]=6			---//无念质　	昏睡类诅咒效果增强/气血回复类；	随机不利状态类诅咒效果增强
		TalentsID[1216]=7			---//尘封质　	眩晕类诅咒效果增强；	孤寂类诅咒效果增强
		TalentsID[1217]=8			---//潇雨质　	真气流失类/真气盾诅咒效果增强；	防御降低类诅咒效果增强
		TalentsID[1218]=9			---//太虚质　	虚弱类诅咒效果增强；	缴械类/汲取类诅咒效果增强
		TalentsID[1219]=10		---//玄通质　	气血增益类祝福效果增强；	气血凝滞类诅咒效果增强
		TalentsID[1220]=11		---//冲灵质　	抗性增益类祝福效果增强；	抗性削减类诅咒效果增强
		TalentsID[1221]=12		---//神影质　	躲闪增益类祝福效果增强；	躲闪削减类诅咒效果增强
		TalentsID[1222]=13		---//霹雳质　	命中增益类祝福效果增强；	命中削减类诅咒效果增强
	
	---//最多有6个天赋影响,不足以-100补足
	---//all the pet skills here
		SkillsBindingTalents[1399]={1213,-100,-100,-100,-100,-100}---//守护<日曜>
		SkillsBindingTalents[1400]={1219,-100,-100,-100,-100,-100}---//热忱<日曜>
		SkillsBindingTalents[1401]={1222,-100,-100,-100,-100,-100}---//闪耀<日曜>
		SkillsBindingTalents[1402]={1210,-100,-100,-100,-100,-100}---//光辉<日曜>
		SkillsBindingTalents[1403]={1213,-100,-100,-100,-100,-100}---//神聚<日曜>
		SkillsBindingTalents[1404]={1216,-100,-100,-100,-100,-100}---//中天<日曜>
		SkillsBindingTalents[5018]={1213,-100,-100,-100,-100,-100}---//神罗<日曜>
		SkillsBindingTalents[1405]={1210,-100,-100,-100,-100,-100}---//新月<月曜>
		SkillsBindingTalents[1406]={1211,-100,-100,-100,-100,-100}---//会神<月曜>
		SkillsBindingTalents[1407]={1222,-100,-100,-100,-100,-100}---//贯注<月曜>
		SkillsBindingTalents[1408]={1221,-100,-100,-100,-100,-100}---//流星<月曜>
		SkillsBindingTalents[1409]={1221,-100,-100,-100,-100,-100}---//月影<月曜>
		SkillsBindingTalents[1410]={1212,-100,-100,-100,-100,-100}---//水月<月曜>
		SkillsBindingTalents[5019]={1210,-100,-100,-100,-100,-100}---//月读<月曜>
		SkillsBindingTalents[1411]={1210,-100,-100,-100,-100,-100}---//芒刺<金曜>
		SkillsBindingTalents[1412]={1210,-100,-100,-100,-100,-100}---//怒嚎<金曜>
		SkillsBindingTalents[1413]={1222,-100,-100,-100,-100,-100}---//全力<金曜>
		SkillsBindingTalents[1414]={1217,-100,-100,-100,-100,-100}---//撕裂<金曜>
		SkillsBindingTalents[1415]={1210,-100,-100,-100,-100,-100}---//刺耳<金曜>
		SkillsBindingTalents[1416]={1212,-100,-100,-100,-100,-100}---//挑衅<金曜>
		SkillsBindingTalents[5020]={1210,-100,-100,-100,-100,-100}---//弥勒<金曜>
		SkillsBindingTalents[1417]={1219,-100,-100,-100,-100,-100}---//曜灵<木曜>
		SkillsBindingTalents[1418]={1213,-100,-100,-100,-100,-100}---//林荫<木曜>
		SkillsBindingTalents[1419]={1222,-100,-100,-100,-100,-100}---//猛击<木曜>
		SkillsBindingTalents[1420]={1213,-100,-100,-100,-100,-100}---//明灭<木曜>
		SkillsBindingTalents[1421]={1214,-100,-100,-100,-100,-100}---//羁绊<木曜>
		SkillsBindingTalents[1422]={1215,-100,-100,-100,-100,-100}---//疲惫<木曜>
		SkillsBindingTalents[5021]={1219,-100,-100,-100,-100,-100}---//万花<木曜>
		SkillsBindingTalents[1423]={1219,-100,-100,-100,-100,-100}---//甘泉<水曜>
		SkillsBindingTalents[1424]={1215,-100,-100,-100,-100,-100}---//祷告<水曜>
		SkillsBindingTalents[1425]={1222,-100,-100,-100,-100,-100}---//预言<水曜>
		SkillsBindingTalents[1426]={1217,-100,-100,-100,-100,-100}---//波纹<水曜>
		SkillsBindingTalents[1427]={1210,-100,-100,-100,-100,-100}---//涟漪<水曜>
		SkillsBindingTalents[1428]={1217,-100,-100,-100,-100,-100}---//思虑<水曜>
		SkillsBindingTalents[5022]={1219,-100,-100,-100,-100,-100}---//沧海<水曜>
		SkillsBindingTalents[1429]={1211,-100,-100,-100,-100,-100}---//火甲<火曜>
		SkillsBindingTalents[1430]={1210,-100,-100,-100,-100,-100}---//贯通<火曜>
		SkillsBindingTalents[1431]={1222,-100,-100,-100,-100,-100}---//酝酿<火曜>
		SkillsBindingTalents[1432]={1220,-100,-100,-100,-100,-100}---//灼热<火曜>
		SkillsBindingTalents[1433]={1212,-100,-100,-100,-100,-100}---//真火<火曜>
		SkillsBindingTalents[1434]={1213,-100,-100,-100,-100,-100}---//燃尽<火曜>
		SkillsBindingTalents[5023]={1211,-100,-100,-100,-100,-100}---//天映<火曜>
		SkillsBindingTalents[1435]={1220,-100,-100,-100,-100,-100}---//地祝<土曜>
		SkillsBindingTalents[1436]={1221,-100,-100,-100,-100,-100}---//洞察<土曜>
		SkillsBindingTalents[1437]={1222,-100,-100,-100,-100,-100}---//伤痕<土曜>
		SkillsBindingTalents[1438]={1214,-100,-100,-100,-100,-100}---//追迹<土曜>
		SkillsBindingTalents[1439]={1216,-100,-100,-100,-100,-100}---//沉沦<土曜>
		SkillsBindingTalents[1440]={1214,-100,-100,-100,-100,-100}---//陷地<土曜>
		SkillsBindingTalents[5024]={1220,-100,-100,-100,-100,-100}---//泰宗<土曜>
		SkillsBindingTalents[1441]={1217,-100,-100,-100,-100,-100}---//妄言<罗候>
		SkillsBindingTalents[1442]={1218,-100,-100,-100,-100,-100}---//黯淡<罗候>
		SkillsBindingTalents[1443]={1222,-100,-100,-100,-100,-100}---//宿命<罗候>
		SkillsBindingTalents[1444]={1219,-100,-100,-100,-100,-100}---//禁锢<罗候>
		SkillsBindingTalents[1445]={1211,-100,-100,-100,-100,-100}---//喝言<罗候>
		SkillsBindingTalents[1446]={1218,-100,-100,-100,-100,-100}---//苦痛<罗候>
		SkillsBindingTalents[5026]={1217,-100,-100,-100,-100,-100}---//六道<罗候>
		SkillsBindingTalents[1447]={1221,-100,-100,-100,-100,-100}---//闪避<计都>
		SkillsBindingTalents[1448]={1214,-100,-100,-100,-100,-100}---//藐视<计都>
		SkillsBindingTalents[1449]={1222,-100,-100,-100,-100,-100}---//锋利<计都>
		SkillsBindingTalents[1450]={1217,-100,-100,-100,-100,-100}---//厄运<计都>
		SkillsBindingTalents[1451]={1216,-100,-100,-100,-100,-100}---//忏悔<计都>
		SkillsBindingTalents[1452]={1215,-100,-100,-100,-100,-100}---//迷茫<计都>
		SkillsBindingTalents[5025]={1221,-100,-100,-100,-100,-100}---//卧龙<计都>
		SkillsBindingTalents[1453]={1222,-100,-100,-100,-100,-100}---//沸腾<通用>
		SkillsBindingTalents[1454]={1222,-100,-100,-100,-100,-100}---//凝华<通用>
		SkillsBindingTalents[1455]={1222,-100,-100,-100,-100,-100}---//挥洒<通用>
		SkillsBindingTalents[1456]={1222,-100,-100,-100,-100,-100}---//蛮力<通用>
		SkillsBindingTalents[1457]={1215,-100,-100,-100,-100,-100}---//修整<通用>
		SkillsBindingTalents[1458]={1218,-100,-100,-100,-100,-100}---//汲取<通用>
		SkillsBindingTalents[5027]={1222,-100,-100,-100,-100,-100}---//怒涛<通用>
		SkillsBindingTalents[5028]={1222,-100,-100,-100,-100,-100}---//怒涛<通用>
		SkillsBindingTalents[5029]={1222,-100,-100,-100,-100,-100}---//怒涛<通用>
		SkillsBindingTalents[5030]={1222,-100,-100,-100,-100,-100}---//怒涛<通用>
		SkillsBindingTalents[5031]={1222,-100,-100,-100,-100,-100}---//怒涛<通用>
		SkillsBindingTalents[5032]={1222,-100,-100,-100,-100,-100}---//怒涛<通用>
		SkillsBindingTalents[5033]={1222,-100,-100,-100,-100,-100}---//怒涛<通用>
		SkillsBindingTalents[5034]={1222,-100,-100,-100,-100,-100}---//怒涛<通用>
		SkillsBindingTalents[5035]={1222,-100,-100,-100,-100,-100}---//怒涛<通用>
		SkillsBindingTalents[5036]={1222,-100,-100,-100,-100,-100}---//怒涛<通用>
		SkillsBindingTalents[5037]={1222,-100,-100,-100,-100,-100}---//怒涛<通用>
		SkillsBindingTalents[5038]={1222,-100,-100,-100,-100,-100}---//怒涛<通用>

	if type(SkillsBindingTalents[skill_id])~="table" then return 0 end 	
	
	local addskills=0
	for i=1,table.getn(SkillsBindingTalents[skill_id]) do 
		if SkillsBindingTalents[skill_id][i]~=-100 then 
		 	 if talentslist[TalentsID[SkillsBindingTalents[skill_id][i]]]==10 then 
		 	 		addskills=addskills+30
		 	 elseif talentslist[TalentsID[SkillsBindingTalents[skill_id][i]]]==5 then
		 			addskills=addskills+10
		 	 else 
			 		addskills=0
			 end 
		else 
		end 	
	end 
		return addskills	
end 
------------------------------------------------------------------------------------------
---宠物技能从当前亲密度获取的级别收益(适用于非种族/职业技能)
function ZLuaPetBedgeSkillBenefitByHonor(skillid,curhonor)
	if skillid==0 or type(skillid)~="number" then return 0 end 
	if type(skillid)~="number" then return 0 end 
	local addskills=0
	local honorclass={0,80,240,480,960,1800,3600,7200,14400,28800,57600,99999}
	
	for i=1,table.getn(honorclass)-1 do 
		if curhonor>honorclass[i] and curhonor<=honorclass[i+1] then 
			addskills=i
		end 
	end 
	---//return addskills	
	---//亲密度影响技能级别暂时被屏蔽
	return 1
end
------------------------------------------------------------------------------------------
---宠物技能从当前亲密度获取的致命一击和致命一击伤害收益
function ZLuaPetBedgeCritBenefitByHonor(curhonor)
	local addcrit,addcritdmg=0,0
	local honorclass={0,80,240,480,960,1800,3600,7200,14400,28800,57600,99999}
	for i=1,table.getn(honorclass)-1 do 
		if curhonor>honorclass[i] and curhonor<=honorclass[i+1] then 
			addcrit=0.005*i
			addcritdmg=0.05*i
		end 
	end 
	return addcrit,addcritdmg
end
---从ilist中按照vlist规定的概率选取num个数字,每种数字最多出现maxsamcol次------------------
function ZLuaCheckOutNums(ilist,vlist,num,maxsamecol)
	if table.getn(ilist)~=table.getn(vlist) then return end
	if table.getn(ilist)<num then return end 
	
	local TheDesk={}	---已经被选取的数编号
	local TheHand={}	---返回的数字序列

	for i=1,num do
		TheDesk[i]=0
		TheHand[i]=0
	end 
	local findsamenum
	
	for j=1,num do 
			TheDesk[j]=ZPetGrade_Get_RanPoint(vlist)
			findsamenum=0
			for k=1,j do
					if TheDesk[k]==TheDesk[j] then 
						findsamenum=findsamenum+1
						if findsamenum>=maxsamecol then 
							vlist[TheDesk[j]]=0 
						end
					end
			end
			TheHand[j]=ilist[TheDesk[j]]
	end 
	return TheHand
end
---从一个单元段平均取数-------------------------------------------------------------------
function ZLuaChooseNormalNums(fromnum,endnum,mode)
	---//起始数,结束数,返回类型：1处理为整数 2不做处理
	if type(fromnum)~="number" or type(endnum)~="number" or fromnum>endnum or fromnum<0 then end
	fromnum=math.ceil(fromnum)
	endnum=math.ceil(endnum)
	
	if mode==nil then mode=1 end 
	
	local normalnum
	if mode==1 then
		normalnum=math.floor(math.random()*(endnum-fromnum))+1+fromnum
		if endnum==fromnum then normalnum=math.floor(endnum) end 
	else
		normalnum=math.random()*(endnum-fromnum)+1+fromnum
		if endnum==fromnum then normalnum=endnum end 
	end 
		return normalnum
end 
-----根据宠物的技能补齐到7个 并生成技能等级(8/9/10/11分别为轮回/星辰/特质/空白技能,不做处理)------
function ZLuaPetBedgeMakeSkillsTable(choosedskills,choosenum,maxskillslevel)
	local skills={}
	local skillslevel={}
	for i=1,7 do
		if i<=choosenum then 
				skills[i]=choosedskills[i]
				skillslevel[i]=1+math.floor(math.random()*maxskillslevel)
		else
				skills[i]=0
				skillslevel[i]=0
		end 
	end 
	return skills,skillslevel
end 
------------------------------------------------------------------------------------------