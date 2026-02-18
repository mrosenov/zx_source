


------------------------------------------------------------------------------------------------------
-- 元魂脚本
-- 以下所有脚本在元魂系统相关的功能时可以调用



-------------------------------------元魂基本数据结构------------------------------------------------------
----1：ID
----2：等级
----3：剩余经验
----4：品质评价
----5：属性1的ID
----6：属性1的具体值
----7：属性2的ID
----8：属性2的具体值
----9：属性3的ID
---10：属性3的具体值
---11：属性4的ID
---12：属性4的具体值
---13：属性5的ID
---14：属性5的具体值
---15：属性6的ID
---16：属性6的具体值
---17：属性7的ID
---18：属性7的具体值
---19：属性8的ID
---20：属性8的具体值
---21：属性1的档位（需存储）
---22：属性1的小档位（需存储）
---23：属性2的档位（需存储）
---24：属性2的小档位（需存储）
---25：属性3的档位（需存储）
---26：属性3的小档位（需存储）
---27：属性4的档位（需存储）
---28：属性4的小档位（需存储）
---29：属性5的档位（需存储）
---30：属性5的小档位（需存储）
---31：属性6的档位（需存储）
---32：属性6的小档位（需存储）
---33：属性7的档位（需存储）
---34：属性7的小档位（需存储）
---35：属性8的档位（需存储）
---36：属性8的小档位（需存储）
---37：洗练次数（需存储）
---38：归元次数（需存储）
---39：平均档位（需存储）
---40：备用（需存储）
---41：备用（需存储）
---42：备用（需存储）
---43：备用（需存储）
---44：备用（需存储）
---45：备用（需存储）
---46：备用（需存储）
---47：备用（需存储）
---48：备用（需存储）

-----------------------------------------------------------------------------------------------------------



--元魂属性初始化
---------------------------------------------------------------------------------------------------- 

 ----出现几率正态化-------------------------------------------------------------------------------------------
math.randomseed(os.time())
math.random()
math.random()
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
   				if Total_Chance==0 then                
		 				n2[m]=0
						n2[1]=1
					else
						n2[m]=nx[m]/ Total_Chance
					end
			end
			return n2
end
-------------------------------------------------------------------------------------------------------------
----按照出现概率随机应该选取的位置---------------------------------------------------------------------------

function ZLottery_Get_RanPoint(s1)
			local js,ron1
			ron1= math.random()                
 		 local p1=ZLottery_Chance_Norm(s1)
			for js=1,table.getn(p1) do
  		        if ron1<=p1[js] then 
   		 	  		return js
				  		end
			end
			return 1
end

-----------------------------------------------元魂鉴定服务函数-------------------------------------------------
----------------------------------------------------------------------------------------------------------------
---约定好相关参数的id号
---1：气血
---2：真气
---3：攻击
---4：防御
---5：命中
---6：躲闪
---7：技能命中
---8：技能躲闪
---9：元力值
---10：致命一击率
---11：致命一击伤害
---12：减免致命一击率
---13：减免致命一击伤害
---14：御仙
---15：御魔
---16：御佛
---17：克仙
---18：克魔
---19：克佛
---20：减免固定伤害
---21：减免伤害百分比
---22：眩晕抗性
---23：定身抗性
---24：魅惑抗性
---25：虚弱抗性
---26：昏睡抗性
---27：减速抗性
---28：无视伤害减免
---29：气血增加百分比
---30：真气增加百分比
---31：攻击增加百分比
---32：防御增加百分比
---33：元神威能
-------------------------------------------------------------------
---id：元婴id，score：道胎值，c_type激活界面类型
function RuneInit(id, rune,score,c_type)

    ---属性池1概率分布(每行10个)
    local AttributeLib = 
    			{
   				1400,1600,1000,1000,3000,2800,500,400,300,200,
   				200,300,200,400,400,400,400,400,400,800,
   				100,900,900,900,900,900,0,
    			}
    ---属性池2概率分布(每行10个)
    local AttributeLib2 =
    			{
    			1500,1700,1000,1200,3200,3000,500,400,300,200,
   				200,300,200,400,400,400,400,400,400,800,
   				100,900,900,900,900,900,0,100,200,300,
   				300,400,100,

    			}
        ---初始档位概率分布(每行10个)
    local AttributeGrades =
    			{
    			5000,2500,1200,500,300,200,150,120,90,80,
    			70,40,30,15,10,8,6,4,2,1,
    			}  
    ---生成条目的概率分布（2条到8条）
    local AttributeNumberList =
    			{
    			500,700,600,150,45,4,1,
    			--1,1,1,1,1,1,10000,
    			}
    ---属性池中每条属性的档位值(对应上面的id号，属性池有33条属性)
    local GradesValue =
    			{
    			2800,3200,330,190,40,30,
    			40,30,800,14,90,16,
    			110,30,30,30,30,30,
    			30,130,16,30,30,30,
    			30,30,10,20,30,30,
    			40,30,40,
    			}
    ---最少生成3条的概率分布
    local AttExtraCnt1 =
    			{
    			1200,600,150,45,4,1,
    			}
    ---最少生成4条的概率分布
    local AttExtraCnt2 =
    			{
    			1800,150,45,4,1,
    			}
    ---最少生成5条的概率分布
    local AttExtraCnt3 =
    			{
    			1950,45,4,1,
    			}
    ---最少生成6条的概率分布
    local AttExtraCnt4 =
    			{
    			1995,4,1,
    			}
		
-------------------------------------------------------------------------------------------------------------
---获得属性条目数量

		function GetNumerList(s2)
				local k = 2
				k = ZLottery_Get_RanPoint(s2) + 1
				return k
		end
		
---获得属性的id

		function GetId(s1)
				local	k1 = 1
				k1 = ZLottery_Get_RanPoint(s1)
				return k1
		end
		
---获得属性的档位

		function GetGrade(s1)
				local	k1 = 1
				k1 = ZLottery_Get_RanPoint(s1)
				return k1
		end
				
	
				
		
---获得各项属性值
		local AttCnt
		local i
		local j = 0
		local	a={}
		local b={}
		local c={}
		local d={}
		local Att = {}
		local Att1 = {}
		
		local num = 0
		if c_type == 0 then
				if score<2001 then
						AttCnt = GetNumerList(AttributeNumberList)
				elseif score>2000 and score<10001 then
						AttCnt = GetNumerList(AttExtraCnt1) + 1
				elseif score>10000 and score<60001 then
						AttCnt = GetNumerList(AttExtraCnt2) + 2
				elseif score>60000 and score<200001 then
						AttCnt = GetNumerList(AttExtraCnt3) + 3
				elseif score>200000  then
						AttCnt = GetNumerList(AttExtraCnt4) + 4
				end
		elseif c_type == 1 then
				AttCnt = 5	
		elseif c_type == 2 then
				AttCnt = 6
		elseif c_type == 3 then
				AttCnt = 8
		elseif c_type == 4 then
				AttCnt = 8
		end
---获得元魂属性id，分为2种情况
---一种为大于4条属性，前4条属性走1号属性库，大于4条的属性走2号库
---另一种为小于4条属性，只走1号库		
		if AttCnt > 1 then  
		if 	AttCnt>4 then
		local Att_size1 = table.getn(AttributeLib2)
		local Att_size = table.getn(AttributeLib)
				for i=1,Att_size1 do
   					Att1[i] = AttributeLib2[i]        
				end
				for i=1,Att_size do
   					Att[i] = AttributeLib[i]        
				end		
				for i=1,4 do
						a[i]=GetId(Att1)
						Att1[a[i]] = 0
				end		
				for i=5,AttCnt do
						a[i]=GetId(Att)
						Att[a[i]] = 0
				end
		elseif AttCnt<=4 then		
		local Att_size = table.getn(AttributeLib)
				for i=1,Att_size do
   					Att[i] = AttributeLib[i]        
				end
				for i=1,AttCnt do
						a[i]=GetId(Att)
						Att[a[i]] = 0
				end
		end
---获得所有的属性档位		
			for i=1,AttCnt do
					b[i] = GetGrade(AttributeGrades)
			end
---每条属性的小档附加值
			for i=1,AttCnt do
					d[i] = math.random(1,10)*0.1
			end
---获得该属性的具体值
    	for i=1,AttCnt do
     			c[i] = GradesValue[a[i]]*(b[i]+d[i])
    	end


---获得平均等级
				for i=1,AttCnt do
						j = j + b[i]
				end
				rune:SetAvgGrade(math.floor(j/AttCnt))
---计算元魂的评价
				rune:SetQuality(AttCnt*5+math.floor(j/AttCnt)*3)  
---设置元魂的属性id、值、档位、附加小档				
     			for i=1,AttCnt do
     					rune:SetAtt(i-1,a[i],c[i],b[i],d[i])
     			end
     			return 1
     	else return 0   
     	end  
		

	    
end
---------------------------------------------------------------------------------------------------------------------
---------------------------------------------------元魂归元服务---------------------------------------------------
function RuneReset(id, rune,score)
    ---属性池1概率分布(每行10个)
    local AttributeLib = 
    			{
   				1400,1600,1000,1000,3000,2800,500,400,300,300,
   				200,300,300,400,400,400,400,400,400,800,
   				100,900,900,900,900,900,0,
    			}
    ---属性池2概率分布(每行10个)
    local AttributeLib2 =
    			{
    			1500,1700,1000,1200,3200,3000,500,400,300,200,
   				200,300,200,400,400,400,400,400,400,800,
   				100,900,900,900,900,900,0,100,200,300,
   				300,400,100,

    			}

    ---属性池3的概率分布(用于道胎到达5级的前4条属性)
    local AttributeLib3 = 
    			{
   				1400,1600,1000,1000,3000,0,500,400,300,300,
   				200,300,300,400,400,400,400,400,400,800,
   				100,900,900,900,900,900,0,
    			}  
    ---属性池4的概率分布（用于道胎到达5级的后4条属性）
    local AttributeLib4 =
    			{
    			1400,1600,1000,1000,3000,0,500,400,300,300,
   				200,300,300,400,400,400,400,400,400,800,
   				100,900,900,900,900,900,0,100,300,300,
   				400,300,100,

    			}    			
    ---属性池5的概率分布(用于道胎到达6级的前4条属性)
    local AttributeLib5 = 
    			{
   				1400,1600,1000,1000,0,0,500,400,300,300,
   				200,300,300,400,400,400,400,400,400,800,
   				100,900,900,900,900,900,0,
    			}  
    ---属性池6的概率分布（用于道胎到达6级的后4条属性）
    local AttributeLib6 =
    			{
    			1400,1600,1000,1000,0,0,500,400,300,300,
   				200,300,300,400,400,400,400,400,400,800,
   				100,900,900,900,900,900,0,100,300,300,
   				400,300,100,

    			} 
    ---初始档位概率分布(每行10个)
    local AttributeGrades =
    			{
    			5000,2500,1200,500,300,200,150,120,90,80,
    			70,40,30,15,10,8,6,4,2,1,
    			}  
    ---生成条目的概率分布（2条到8条）
    local AttributeNumberList =
    			{
    			500,700,600,150,45,4,1,
    			--1,1,1,1,1,1,10000,
    			}
    ---最少生成3条的概率分布
    local AttExtraCnt1 =
    			{
    			1200,600,150,45,4,1,
    			}
    ---最少生成4条的概率分布
    local AttExtraCnt2 =
    			{
    			1800,150,45,4,1,
    			}
    ---最少生成5条的概率分布
    local AttExtraCnt3 =
    			{
    			1950,45,4,1,
    			}
    ---最少生成6条的概率分布
    local AttExtraCnt4 =
    			{
    			1995,4,1,
    			}
    ---属性池中每条属性的档位值(对应上面的id号，属性池有33条属性)
    local GradesValue =
    			{
    			2800,3200,330,190,40,30,
    			40,30,800,14,90,16,
    			110,30,30,30,30,30,
    			30,130,16,30,30,30,
    			30,30,10,20,30,30,
    			40,30,40,
    			}

---获得属性条目数量

		function GetNumerList(s2)
				local k = 2
				k = ZLottery_Get_RanPoint(s2) + 1
				return k
		end
		
---获得属性的id

		function GetId(s1)
				local	k1 = 1
				k1 = ZLottery_Get_RanPoint(s1)
				return k1
		end
		
---获得属性的档位

		function GetGrade(s1)
				local	k1 = 1
				k1 = ZLottery_Get_RanPoint(s1)
				return k1
		end
---重新设置元魂原来的各项属性数值为0		
     			for i=1,8 do
     					rune:SetAtt(i-1,0,0,0,0)
     			end
		
---获得各项属性值
		local AttCnt
		local i
		local j = 0
		local	a={}
		local b={}
		local c={}
		local d={}
		local Att = {}
		local Att1 = {}
		
		local num = 0
		if score<2001 then
				AttCnt = GetNumerList(AttributeNumberList)
		elseif score>2000 and score<10001 then
				AttCnt = GetNumerList(AttExtraCnt1) + 1
		elseif score>10000 and score<60001 then
				AttCnt = GetNumerList(AttExtraCnt2) + 2
		elseif score>60000 and score<200001 then
				AttCnt = GetNumerList(AttExtraCnt3) + 3
		elseif score>200000  then
				AttCnt = GetNumerList(AttExtraCnt4) + 4
		end
		
---获得元魂属性id，分为2种情况
---一种为大于4条属性，前4条属性走1号属性库，大于4条的属性走2号库
---另一种为小于4条属性，只走1号库		
		if AttCnt > 1 then  
		if 	AttCnt>4 then
		local Att_size1
		local Att_size
---判断道胎值选择属性库
			if score>60000 and score<200001 then
					 Att_size1 = table.getn(AttributeLib4)
		 	     Att_size = table.getn(AttributeLib3)
				   for i=1,Att_size1 do
   					   Att1[i] = AttributeLib4[i]        
				   end
				   for i=1,Att_size do
   			    		Att[i] = AttributeLib3[i]        
				   end		
				   for i=1,4 do
						    a[i]=GetId(Att1)
						    Att1[a[i]] = 0
				   end		
				   for i=5,AttCnt do
				    		a[i]=GetId(Att)
					    	Att[a[i]] = 0
				   end					 	     
		 	     
 	     			     
			elseif score>200000  then
					 Att_size1 = table.getn(AttributeLib6)
		 	     Att_size = table.getn(AttributeLib5)		
				   for i=1,Att_size1 do
   					   Att1[i] = AttributeLib6[i]        
				   end
				   for i=1,Att_size do
   			    		Att[i] = AttributeLib5[i]        
				   end		
				   for i=1,4 do
						    a[i]=GetId(Att1)
						    Att1[a[i]] = 0
				   end		
				   for i=5,AttCnt do
				    		a[i]=GetId(Att)
					    	Att[a[i]] = 0
				   end				 	     
		 	     
	 	     	     
			else 
					 Att_size1 = table.getn(AttributeLib2)
		 	     Att_size = table.getn(AttributeLib)
				   for i=1,Att_size1 do
   					   Att1[i] = AttributeLib2[i]        
				   end
				   for i=1,Att_size do
   			    		Att[i] = AttributeLib[i]        
				   end		
				   for i=1,4 do
						    a[i]=GetId(Att1)
						    Att1[a[i]] = 0
				   end		
				   for i=5,AttCnt do
				    		a[i]=GetId(Att)
					    	Att[a[i]] = 0
				   end		 	     
		 	     	 	     
			end


		elseif AttCnt<=4 then		
		local Att_size = table.getn(AttributeLib)
				for i=1,Att_size do
   					Att[i] = AttributeLib[i]        
				end
				for i=1,AttCnt do
						a[i]=GetId(Att)
						Att[a[i]] = 0
				end
		end
---获得所有的属性档位		
			for i=1,AttCnt do
					b[i] = GetGrade(AttributeGrades)
			end
---每条属性的小档附加值
			for i=1,AttCnt do
					d[i] = math.random(1,10)*0.1
			end
---获得该属性的具体值
    	for i=1,AttCnt do
     			c[i] = GradesValue[a[i]]*(b[i]+d[i])
    	end


---获得平均等级
				for i=1,AttCnt do
						j = j + b[i]
				end
				rune:SetAvgGrade(math.floor(j/AttCnt))
---计算元魂的评价
				rune:SetQuality(AttCnt*5+math.floor(j/AttCnt)*3)  
---设置元魂的属性id、值、档位、附加小档				
     			for i=1,AttCnt do
     					rune:SetAtt(i-1,a[i],c[i],b[i],d[i])
     			end
     			return 1
     	else return 0   
     	end  
    
    			   			
end
---------------------------------------------------------------------------------------------------------------------
----------------------------------------------------元魂属性洗练-----------------------------------------------------
function RuneRefine(id, rune,score)
	local GradesValue =
    			{
    			2800,3200,330,190,40,30,
    			40,30,800,14,90,16,
    			110,30,30,30,30,30,
    			30,130,16,30,30,30,
    			30,30,10,20,30,30,
    			40,30,40,
    			}
	local UpperList={}
		UpperList[0]={30,35,35}	
		UpperList[1]={31,35,34}	
		UpperList[2]={32,35,33}		
		UpperList[3]={33,35,32}	
		UpperList[4]={34,35,31}	
		UpperList[5]={35,30,30}		
		UpperList[6]={36,30,28}	
		UpperList[7]={37,30,27}	
		UpperList[8]={38,30,25}	
		UpperList[9]={40,30,23}	
		UpperList[10]={42,30,22}	
		UpperList[11]={43,30,21}
		UpperList[12]={44,30,20}	
		UpperList[13]={45,27,18}	
		UpperList[14]={46,27,16}	
		UpperList[15]={47,27,15}	
		UpperList[16]={48,27,14}	
		UpperList[17]={49,25,13}	
		UpperList[18]={50,25,12}	
		UpperList[19]={55,25,11}
			
	local quality,avg,AttCnt
	local i,j,k
	local CurrentGrades = {}
	local GradeRandom
	local a={}
	local b={}
	local c={}
	local d={}

	quality = rune:QueryQuality()
	avg = rune:GetAvgGrade()
	AttCnt = rune:GetAttCnt()

	
	
	if avg ==0 then avg = 1 end
	if avg>0 and AttCnt>0 then 
			for i=1,3 do
					CurrentGrades[i] = UpperList[avg][i]
			end
	

			for i=1,AttCnt do
			--id、值、档位、小档值
					a[i],b[i],c[i],d[i]=rune:GetAtt(i-1)
			----当档位在15档以上时做特殊处理，防止单一属性容易上涨
					if c[i] > 14 and (c[i]-avg) > 4 then
							for i=1,3 do
									CurrentGrades[i] = UpperList[avg+4][i]
							end	
					end
			----------------------------------------------------------
					GradeRandom = ZLottery_Get_RanPoint(CurrentGrades)
					
					if GradeRandom == 1 then
							if c[i] == 1 or c[i]== 0 then
									c[i] = 1
									d[i] = math.random(1,10)*0.1
									b[i] = GradesValue[a[i]]*(c[i]+d[i])
									rune:SetRefineAtt(i-1,a[i],b[i],c[i],d[i])	
							elseif c[i] >1 then
									c[i] = c[i] - 1
									d[i] = math.random(1,10)*0.1
									b[i] = GradesValue[a[i]]*(c[i]+d[i])
									rune:SetRefineAtt(i-1,a[i],b[i],c[i],d[i])
							end
					elseif GradeRandom == 2 then
							if  c[i]== 0 then
									c[i] = 1
									d[i] = math.random(1,10)*0.1
									b[i] = GradesValue[a[i]]*(c[i]+d[i])
									rune:SetRefineAtt(i-1,a[i],b[i],c[i],d[i])
							elseif c[i] >0 then
									rune:SetRefineAtt(i-1,a[i],b[i],c[i],d[i])						
							end
							
					elseif GradeRandom == 3 then
							if c[i] < 20 and c[i] >= 0 then 
									c[i] = c[i] + 1
									d[i] = math.random(1,10)*0.1
									b[i] = GradesValue[a[i]]*(c[i]+d[i])
									rune:SetRefineAtt(i-1,a[i],b[i],c[i],d[i])
							elseif c[i] == 20 then
									d[i] = math.random(1,10)*0.1
									b[i] = GradesValue[a[i]]*(c[i]+d[i])
									rune:SetRefineAtt(i-1,a[i],b[i],c[i],d[i])
							end						
					end
			end
	end
	j=0
	for i=1,AttCnt do
		j=j+c[i]
	end
	avg=math.floor(j/AttCnt)
	rune:SetAvgGrade(avg)
	quality = avg*3+AttCnt*5 
	rune:SetRefineQuality(quality)
	return 1
	
	
end

---------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------元魂分解--------------------------------------------------------
function RuneDecompose(id, rune,score)
	local level,quality,value
	level = rune:QueryLevel()
	quality =rune:QueryQuality()
	value = quality + math.random(0,20)
	if level >9 then
		if value < 86 and value > 0 then return 52248
		elseif value < 106 and value > 85 then return 52249
		elseif value < 118 and value > 105 then return 52251
		elseif value > 117  then return 52252
		end
	else return 0
	end
end

