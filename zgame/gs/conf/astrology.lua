
function Astrology_InnerInit(id, item, ess, t)
	t.__ID = id;
	t.__ITEM = item;
	t.__ESS = ess;
	
	t.GetLevel 		= __ABASE_ASTROLOGY_GetLevel;
	t.SetLevel 		= __ABASE_ASTROLOGY_SetLevel;
	t.GetScore 		= __ABASE_ASTROLOGY_GetScore;
	t.SetScore 		= __ABASE_ASTROLOGY_SetScore;
	t.GetEnergyConsume	= __ABASE_ASTROLOGY_GetEnergyConsume;
	t.SetEnergyConsume	= __ABASE_ASTROLOGY_SetEnergyConsume;
	t.GetProp		= __ABASE_ASTROLOGY_GetProp;
	t.SetProp		= __ABASE_ASTROLOGY_SetProp;
end

--¼ø¶¨
function Astrology_Identify_Entrance(id, item, ess, reborn_cnt, equip_mask)
	local astrology  = {};
	Astrology_InnerInit(id, item, ess, astrology );

	return AstrologyIdentify(id, astrology, reborn_cnt, equip_mask);
end


xdofile("script/astrology_interface.lua");
