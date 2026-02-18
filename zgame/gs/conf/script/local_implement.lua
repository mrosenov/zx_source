----talisman方法本地实现
----2007.04.21


----	接口分成三部分，分别如下：
---	A:获取基本数据
---	QueryLevel()		得到法宝级别
---	QueryExp()		得到法宝当前经验



---	B:获取和设置参数 
---	  所谓参数，就是脚本定义的数据， 参数都是float类型，以index/value形式保存和定义
---        法宝的初始数值成长率等都以参数的方式存在。 参数区占用的空间和GetDataCount()的返回值
---	  成正比。所以不要随意给很大的index索引，这样会占用大量的内存和数据库空间。
---	QueryData(index)	获取制定索引的参数 如果该索引无对应参数，则返回0.0
---	GetDataCount()		返回当前所有的参数数目 这个值就是最大的有效index+1
---	UpdateData(index, value)更新一个参数，如果index不存在，则增加一个条目 value必须是一个数值
---	ClearData()		清除所有存在的参数



---	C:设置法宝属性
---	  法宝属性是法宝真正的效果作用， 法宝属性应该从参数计算得到,法宝属性都是整数 ,不能使用浮点数.
---	  注意每次设置前 ,法宝所有的属性包括附加属性都会被清空 ,所以每次设置都应设置全部属性
---	SetStamina(value)	设置法宝最大的精力值
---	SetQuality(value)	设置法宝的品阶, 供客户端显示使用
---	SetHP(value)		设置法宝增加的血量
---	SetMP(value)		设置法宝增加的真气
---	SetAttackRate(value)	设置法宝增加的命中
---	SetDamage(low ,high)	设置法宝增加的攻击力
---	SetDodge(value)		设置法宝增加的闪躲
---	SetDefense(value)	设置法宝增加的防御
---	SetResistance(index,value) 设置法宝增加的抗性
---	SetAddon(addon)		设置法宝的附加属性 , 每次设置都会增加一个附加属性 addon是模板中的附加属性模板ID

--[[
附件是测试法宝4662的脚本例子，将此文件上传到服务器根目录，重启服务器的话此文件将被加载。以后正式加入法宝时,需要将正式法宝的脚本文件通过邮件提交给我.

由于语法语义和逻辑错误而产生的服务器输出并不会在客户端体现出来，为了方便测试，你们一定要建立自己的调试和测试运行环境，不应依赖服务器环境进行脚本调试。

法宝只有在任务发放/彩票发放/NPC贩卖/商城贩卖这四种情况下会进行脚本调用。

原有的2001等调试命令无法生成有效的法宝，为此，添加了新的调试命令10807专供法宝生成，例如：

d_c2scmd 10807 4662

另外，为了方便测试，提供了刷新脚本文件的功能，如下：

d_c2scmd 10808

这里会使当前所在的线重新读入test_talisman.lua文件，可以不重启服务器更新脚本内容。

如有任何疑问，请反馈。
--]]

math.randomseed(os.time())

Talisman = { 
	id,
	level = 1,
	exp = 0,
	stamina = 0,
	quality = 0,
	hp = 0,
	mp = 0,
	attackRate = 0,
	lowDamage = 0,
	highDamage = 0,
	dodge = 0,
	defense = 0,
	resisStun = 0,
	resisSleep = 0,
	resisWeakness = 0,
	resisEnlace = 0,
	resisSilence = 0,
	data = {},
	addons = {},
	MAX_DATA = 100,
	MAX_ADDON = 10,
	}

---类生成对象函数
function Talisman:new( o )
	o = o or {}	--- create object if user does not provide one
	setmetatable(o, self)
	self.__index = self
	return o
end

---	A:获取基本数据

---	QueryLevel()		得到法宝级别
function Talisman:QueryLevel()
	return self.level
end

---	QueryExp()		得到法宝当前经验
function Talisman:QueryExp()
	return self.exp
end

---	B:获取和设置参数 
---	  所谓参数，就是脚本定义的数据， 参数都是float类型，以index/value形式保存和定义
---   法宝的初始数值成长率等都以参数的方式存在。 参数区占用的空间和GetDataCount()的返回值
---	  成正比。所以不要随意给很大的index索引，这样会占用大量的内存和数据库空间。

---	QueryData(index)	获取制定索引的参数 如果该索引无对应参数，则返回0.0
function Talisman:QueryData(index)
	if type(index) ~= "number" or index < 0 or index >= self.MAX_DATA then return 0 end
	if self.data[index] then
		return self.data[index]
	else
		return 0
	end
end

---	GetDataCount()		返回当前所有的参数数目 这个值就是最大的有效index+1
function Talisman:GetDataCount()
	return table.getn(self.data)
end

---	UpdateData(index , value)更新一个参数，如果index不存在，则增加一个条目 value必须是一个数值
function Talisman:UpdateData(index , value)
	if type(index) ~= "number" or index < 0 or index >= self.MAX_DATA then return end
	if type(value) ~= "number" then return end
	self.data[index] = value
end

---	ClearData()		清除所有存在的参数
function Talisman:ClearData()
	local i
	for i = 0, self.MAX_DATA-1 do
		self.data[i] = 0
	end
end




---	C:设置法宝属性
---	  法宝属性是法宝真正的效果作用， 法宝属性应该从参数计算得到 ,法宝属性都是整数 ,不能使用浮点数.
---	  注意每次设置前 ,法宝所有的属性包括附加属性都会被清空 ,所以每次设置都应设置全部属性

---	SetStamina(value)	设置法宝最大的精力值
function Talisman:SetStamina(value)
	if type(value) ~= "number" then return end
	self.stamina = value
end

---	SetQuality(value)	设置法宝的品阶 , 供客户端显示使用，现在挪用做美工路径ID
function Talisman:SetQuality(value)
	if type(value) ~= "number" then return end
	self.quality = value
end

---	SetHP(value)		设置法宝增加的血量
function Talisman:SetHP(value)
	if type(value) ~= "number" then return end
	self.hp = value
end

---	SetMP(value)		设置法宝增加的真气
function Talisman:SetMP(value)
	if type(value) ~= "number" then return end
	self.mp = value
end	
	
---	SetAttackRate(value)	设置法宝增加的命中
function Talisman:SetAttackRate(value)
	if type(value) ~= "number" then return end
	self.attackRate = value
end

---	SetDamage(low,high)	设置法宝增加的攻击力
function Talisman:SetDamage(low ,high)
	if type(low) ~= "number" or type(high) ~= "number" then return end
	self.lowDamage = low
	self.highDamage = high
end

---	SetDodge(value)		设置法宝增加的闪躲
function Talisman:SetDodge(value)
	if type(value) ~= "number" then return end
	self.dodge = value
end

---	SetDefense(value)	设置法宝增加的防御
function Talisman:SetDefense(value)
	if type(value) ~= "number" then return end
	self.defense = value
end

---	SetResistance(index ,value) 设置法宝增加的抗性
function Talisman:SetResistance(index,value)
	if type(index) ~= "number" or type(value) ~= "number" then return end
	if index == 0 then self.resisStun = value end
	if index == 1 then self.resisWeakness = value end
	if index == 2 then self.resisEnlace = value end
	if index == 3 then self.resisSilence = value end
	if index == 4 then self.resisSleep = value end
end
		
---	SetAddon(addon)		设置法宝的附加属性 , 每次设置都会增加一个附加属性 addon是模板中的附加属性模板ID
function Talisman:SetAddon(addon)
	if type(addon) ~= "number" then return end
	local n 
	n = table.getn(self.addons)
	---debug
	---print(n)
	---debug
	if n >= self.MAX_ADDON then return end
	self.addons[n+1] = addon
end

--- CreateItem(id)生成法宝
function Talisman:CreateItem(id)
end





---本地使用成员函数

---设置法宝模板ID
function Talisman:LocalSetID(value)
	if type(value) ~= "number" then return end
	self.id = value
end

---升级函数
function Talisman:LocalAddLevel(value)
	if type(value) ~= "number" then return end
	self.level = self.level + math.floor(value)
end

---取得法宝模板ID
function Talisman:LocalGetID()
	return self.id
end

--- LocalClearAddon()  消除所有附加属性 （本地使用)
function Talisman:LocalClearAddon()
	local i
	for i = 0, self.MAX_ADDON-1 do
		self.addons[i] = nil
	end
end

--- LocalClearAttribute() 消除所有法宝本体属性 （本地使用）
function Talisman:LocalClearAttribute()
	self.stamina = 0
	self.quality = 0
	self.hp = 0
	self.mp = 0
	self.attackRate = 0
	self.lowDamage = 0
	self.highDamage = 0
	self.dodge = 0
	self.defense = 0
	self.resisStun = 0
	self.resisSleep = 0
	self.resisWeakness = 0
	self.resisEnlace = 0
	self.resisSilence = 0
end

--- LocalConfirmData() 检查data数据，如果从0开始有值，并且任何2个值之间无nil，则返回true
function Talisman:LocalConfirmData()
	local i,n
	n=0
	if self.data[0] == nil then return false end
	for i=1,self.MAX_DATA do
		if self.data[i] ~= nil then n=n+1 end
	end
	if n == table.getn(self.data) then
		return true
	else
		return false
	end
end

--- LocalPrint(maxData,maxAddon) 显示法宝当前属性（本地使用）
function Talisman:LocalPrint(maxData,maxAddon)
	print("-------------------------------")
	print("      tmpl id :",self.id)
	print("        level :",self.level)
	print("          exp :",self.exp)
	print("      stamina :",self.stamina)
	print("      quality :",self.quality)
	print("           hp :",self.hp)
	print("           mp :",self.mp)
	print("   attackRate :",self.attackRate)
	print("       damage :",self.lowDamage,self.highDamage)
	print("        dodge :",self.dodge)
	print("      defense :",self.defense)
	print("    resisStun :",self.resisStun)
	print("   resisSleep :",self.resisSleep)
	print("resisWeakness :",self.resisWeakness)
	print("  resisEnlace :",self.resisEnlace)
	print(" resisSilence :",self.resisSilence)
	maxData = maxData or 100
	maxAddon = maxAddon or 10
	local i, nPrinted
	nPrinted = 0
	for i=1,self.MAX_ADDON do
		if self.addons[i] then
			if nPrinted < maxAddon then
				print("       addon :",i,self.addons[i])
				nPrinted = nPrinted + 1
			end
		end
	end
	nPrinted = 0
	for i=0,self.MAX_DATA-1 do
		if self.data[i] then
			if nPrinted < maxData then
				print("         data :",i,self.data[i])
				nPrinted = nPrinted + 1
			end
		end
	end
end


---存储所有函数（本地使用）
---结构是 { { tmpl_id=number, init=function, levelup=function, reset=function },{tmpl_id=number, init=function, levelup=function, reset=function } ...}
funcs = {}

---AddTalismanItem
function AddTalismanItem(id, talisman_init,talisman_levelup,talisman_reset)
	if type(id) ~= "number" or type(talisman_init) ~= "function" or type(talisman_levelup) ~= "function" or type(talisman_reset) ~= "function" then 
		return
	end
	funcs[id] = {}
	funcs[id]["tmpl_id"] = id
	funcs[id]["init"] = talisman_init
	funcs[id]["levelup"] = talisman_levelup
	funcs[id]["reset"] = talisman_reset
end


---用于调用函数，talisman是法宝对象，operate是操作类型string，目前仅"init","levelup","reset"可用
function TalismanFunction(talisman,operate)
	if talisman == nil then return end
	local id,func
	id = talisman:LocalGetID()
	if funcs[id] and funcs[id][operate] and type(funcs[id][operate]) == "function" then
		func = funcs[id][operate]
		return func(id,talisman)
	end
end




