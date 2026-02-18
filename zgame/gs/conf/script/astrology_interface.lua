------------------------------------------------------------------------------------------------------
-- 星座脚本
-- 以下所有脚本在星座系统相关的功能时可以调用

-----------------------------------------------------------------------------------------------------------
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

-----------------------------------------------星座鉴定服务函数-------------------------------------------------
----------------------------------------------------------------------------------------------------------------
---约定好相关参数的id号（和元魂相同）
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
function AstrologyIdentify(id, astrology, reborn_cnt, equip_mask)

    ---A-1)未飞升武器属性池1概率分布(每行10个)
    local AttributeLib1 = 
    			{
   				1200,1200,1500,1000,2000,1800,100,0,0,100,
   				0,100,0,0,0,0,0,0,0,150,
    			}
    ---A-2)未飞升帽子属性池2概率分布(每行10个)
    local AttributeLib2 =
    			{
    			1000,1500,1000,1000,1750,1750,0,100,0,0,
   				100,0,100,0,0,0,0,0,0,150,
   				0,600,600,
    			}
    ---A-3)未飞升衣服属性池3概率分布(每行10个)
    local AttributeLib3 = 
    			{
   				1000,1000,1000,1400,1900,1800,0,100,0,0,
   				100,0,0,0,0,0,0,0,0,150,
   				0,0,0,600,600,
    			}
    ---A-4)未飞升鞋子属性池4概率分布(每行10个)
    local AttributeLib4 =
    			{
    			1400,1000,900,1000,2000,1800,100,0,0,0,
   				100,0,100,0,0,0,0,0,0,0,
   				100,0,0,0,0,600,600,
    			}
    ---B-1)飞升武器属性池5概率分布(每行10个)
    local AttributeLib5 = 
    			{
   				600,600,900,700,950,950,100,0,100,100,
   				0,100,0,650,650,650,650,650,650,150,
   				0,0,0,0,0,0,0,50,150,150,
   				200,200,100,
    			}
    ---B-2)飞升帽子属性池6概率分布(每行10个)
    local AttributeLib6 =
    			{
    			600,900,450,1000,900,900,0,100,100,0,
   				100,0,100,650,650,650,650,650,650,150,
   				0,450,450,0,0,0,0,50,0,100,
   				150,200,100,
    			}
    ---B-3)飞升衣服属性池7概率分布(每行10个)
    local AttributeLib7 = 
    			{
   				500,500,500,900,800,800,0,100,100,0,
   				100,0,100,650,650,650,650,650,650,150,
   				0,0,0,450,450,0,0,50,150,0,
   				150,200,100,
    			}
    ---B-4)飞升鞋子属性池8概率分布(每行10个)
    local AttributeLib8 =
    			{
    			900,600,400,650,750,850,100,0,100,0,
   				100,0,100,650,650,650,650,650,650,0,
   				100,0,0,0,0,450,450,50,150,150,
   				0,100,100,
    			}
    ---生成条目的概率分布（1条到5条，未飞升型）
    local AttributeNumberList =
    			{
    			4000,5000,800,195,5,
    			}
    ---生成条目的概率分布（1条到5条，飞升型）
    local AttributeNumberList2 =
    			{
    			9996,1,1,1,1,
    			}
    ---数值等级的概率分布（共9档）
    local Proplevel =
    			{
    			9992,1,1,1,1,1,1,1,1,
    			}
     ---属性池中每条属性的每数值等级值(对应上面的id号，属性池有33条属性)
    local LevelsValue1 =
    			{
    			15000,15000,1800,750,135,105,135,135,3000,120,
    			320,180,360,120,120,120,120,120,120,600,
    			120,270,270,270,270,270,90,36,80,80,
    			80,60,48,
    			}
     local LevelsValue2 =
    			{
    			11250,11250,900,540,90,75,60,60,0,90,
    			200,90,200,0,0,0,0,0,0,210,
    			120,180,180,180,180,180,45,0,60,60,
    			60,45,0,
    			}
    ---属性池各属性权重（每行10个）
    local Propweight =
    			{
    			3,3,3,2,2,2,4,4,4,4,
    			4,4,4,2,2,2,2,2,2,3,
    			3,3,3,3,3,3,3,4,3,3,
    			3,2,4,
    			}
-------------------------------------------------------------------------------------------------------------
---获得属性条目数量

		function GetNumerList(s2)
				local k = 1
				k = ZLottery_Get_RanPoint(s2)
				return k
		end
		
---获得属性的id

		function GetId(s1)
				local	k1 = 1
				k1 = ZLottery_Get_RanPoint(s1)
				return k1
		end
---获得属性的数值位

		function GetGrade(s1)
				local	k1 = 1
				k1 = ZLottery_Get_RanPoint(s1)
				return k1
		end

---获得各项属性值
		local AttCnt
		local i
		local j = 0
		local j1 = 0
		local	a={}
		local b={}
		local c={}
		local d={}
		local Tempatt={}
		local Tempattt_size = 0
		

---未飞升型		
    if reborn_cnt>=0 and reborn_cnt<1 then
				AttCnt = GetNumerList(AttributeNumberList)
				if AttCnt>=5 then 
				   AttCnt = 5 
				end
				
				if AttCnt<1 then 
				   AttCnt = 1 
				end

---获得未飞升型具体属性的id			
				if equip_mask>0 and equip_mask<2 then
				
				Tempattt_size = table.getn(AttributeLib1)
				for i=1,Tempattt_size do
   					Tempatt[i] = AttributeLib1[i]        
				end
   			   for i=1,AttCnt do
						   a[i]=GetId(Tempatt)
						   Tempatt[a[i]]=0
				   end
				
				elseif equip_mask>=2 and equip_mask<4 then
				   Tempattt_size = table.getn(AttributeLib2)
				for i=1,Tempattt_size do
   					Tempatt[i] = AttributeLib2[i]        
				end
   			   for i=1,AttCnt do
						   a[i]=GetId(Tempatt)
						   Tempatt[a[i]]=0
				   end
				
				elseif equip_mask>=4 and equip_mask<8 then
					Tempattt_size = table.getn(AttributeLib3)
				for i=1,Tempattt_size do
   					Tempatt[i] = AttributeLib3[i]        
				end
   			   for i=1,AttCnt do
						   a[i]=GetId(Tempatt)
						   Tempatt[a[i]]=0
				   end
				
				elseif equip_mask>=8 and equip_mask<9 then
					 Tempattt_size = table.getn(AttributeLib4)
				for i=1,Tempattt_size do
   					Tempatt[i] = AttributeLib4[i]        
				end
   			   for i=1,AttCnt do
						   a[i]=GetId(Tempatt)
						   Tempatt[a[i]]=0
				   end
				
				end
---获得具体属性的值
      for i=1,AttCnt do
					b[i] = GetGrade(Proplevel)
			end
---每条属性的小档附加值
			for i=1,AttCnt do
					d[i] = math.random(1,10)*0.1
			end
---获得该属性的具体值
    	for i=1,AttCnt do
     			c[i] = LevelsValue2[a[i]]*(b[i]+d[i])
    	end				
				
				
				
---飞升型				
		elseif reborn_cnt>=1 then
				AttCnt = GetNumerList(AttributeNumberList2)
				if AttCnt>=5 then 
				   AttCnt = 5 
				end
				
				if AttCnt<1 then 
				   AttCnt = 1 
				end
---获得未飞升型具体属性的id			
				if equip_mask>0 and equip_mask<2 then
		Tempattt_size = table.getn(AttributeLib5)
				for i=1,Tempattt_size do
   					Tempatt[i] = AttributeLib5[i]        
				end
   			   for i=1,AttCnt do
						   a[i]=GetId(Tempatt)
						   Tempatt[a[i]]=0
				   end
				
				elseif equip_mask>=2 and equip_mask<4 then
				   Tempattt_size = table.getn(AttributeLib6)
				for i=1,Tempattt_size do
   					Tempatt[i] = AttributeLib6[i]        
				end
   			   for i=1,AttCnt do
						   a[i]=GetId(Tempatt)
						   Tempatt[a[i]]=0
				   end
				
				elseif equip_mask>=4 and equip_mask<8 then
					 Tempattt_size = table.getn(AttributeLib7)
				for i=1,Tempattt_size do
   					Tempatt[i] = AttributeLib7[i]        
				end
   			   for i=1,AttCnt do
						   a[i]=GetId(Tempatt)
						   Tempatt[a[i]]=0
				   end
				
				elseif equip_mask>=8 and equip_mask<9 then
					 Tempattt_size = table.getn(AttributeLib8)
				for i=1,Tempattt_size do
   					Tempatt[i] = AttributeLib8[i]        
				end
   			   for i=1,AttCnt do
						   a[i]=GetId(Tempatt)
						   Tempatt[a[i]]=0
				   end
				
				end
---获得具体属性的值
      for i=1,AttCnt do
					b[i] = GetGrade(Proplevel)
			end
---每条属性的小档附加值
			for i=1,AttCnt do
					d[i] = math.random(1,10)*0.1
			end
---获得该属性的具体值
    	for i=1,AttCnt do
     			c[i] = LevelsValue1[a[i]]*(b[i]+d[i])
    	end		
    end
    
 ---获得星座评分
    for i=1,AttCnt do
						j = j + Propweight[a[i]]*(b[i]+d[i])
				end
		astrology:SetScore(math.floor(j*5) + AttCnt*8)

---获得星座消耗值
    astrology:SetEnergyConsume(math.floor(j*0.1)+1)		

---设置元魂的属性id、值、档位、附加小档				
     			for i=1,AttCnt do
     					astrology:SetProp(i-1,a[i],c[i])
     			end       
    end 
    
		
		
		