
function Magic_InnerInit(id, item, ess,t)
	t.__ID = id;
	t.__ITEM = item;
	t.__ESS = ess;
	t.QueryLevel 		= __ABASE_MAGIC_QueryLevel;
	t.SetLevel 		= __ABASE_MAGIC_SetLevel;
	t.QueryExp 		= __ABASE_MAGIC_QueryExp;
	t.SetExp 		= __ABASE_MAGIC_SetExp;
	t.QueryMaxExp 		= __ABASE_MAGIC_QueryMaxExp;
	t.SetMaxExp 		= __ABASE_MAGIC_SetMaxExp;
	t.QueryExpLevel 	= __ABASE_MAGIC_QueryExpLevel;
	t.SetExpLevel		= __ABASE_MAGIC_SetExpLevel;
	t.SetExpSpeed		= __ABASE_MAGIC_SetExpSpeed;

	t.QueryData 		= __ABASE_MAGIC_QueryInnerData;
	t.GetDataCount 		= __ABASE_MAGIC_GetInnerDataCount;
	t.UpdateData  		= __ABASE_MAGIC_SetInnerData;
	t.ClearData  		= __ABASE_MAGIC_ClearInnerData;

	t.SetAddon 		= __ABASE_MAGIC_AddAddon;
	t.SetDuration 		= __ABASE_MAGIC_SetDuration ;
	t.SetMaxDuration 	= __ABASE_MAGIC_SetMaxDuration ;
	t.SetQuality 		= __ABASE_MAGIC_SetQuality ;
	t.SetHP 		= __ABASE_MAGIC_SetHP;
	t.SetMP 		= __ABASE_MAGIC_SetMP;
	t.SetDamage 		= __ABASE_MAGIC_SetDamage;
	t.SetAttack		= __ABASE_MAGIC_SetAttack;
	t.SetDefense		= __ABASE_MAGIC_SetDefense;
	t.SetArmor		= __ABASE_MAGIC_SetArmor;
	t.SetResistance 	= __ABASE_MAGIC_SetResistance;
end

function Magic_Init_Entrance(id, item, ess, player_imp) 
	local magic= {};
	Magic_InnerInit(id, item, ess, magic)

	local player = {}
	Object_InnerInit(player_imp, player, 1)

	return MagicInit(id, magic, player)

end

function Magic_ExpLevelUp_Entrance(id, item, ess, player_imp) 
	local magic= {};
	Magic_InnerInit(id, item, ess, magic)

	local player = {}
	Object_InnerInit(player_imp, player, 1)

	return MagicExpLevelUp(id, magic, player)

end


function Magic_Refine_Entrance( id, ess, cid, player_imp, refine_type, success_prob, match_value)
	local magic= {};
	Magic_InnerInit(id, nil, ess, magic)

	local player = {}
	Object_InnerInit(player_imp, player, 1)

	return MagicRefine(id, magic, player, refine_type, success_prob, match_value);
end

xdofile("object_script.lua")
xdofile("script/magic_interface.lua")
xdofile("script/magic_refine.lua")

