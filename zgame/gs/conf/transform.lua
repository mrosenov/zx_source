
--变身脚本
--策划应该实现相应的接口


function Init_Transform_Skill(template_id, transform_type, level, exp_level, player_imp)
	local player = {}
	Object_InnerInit(player_imp, player, -1)

	return InitTransformSkill(template_id, transform_type, level, exp_level, player)
end


function Init_Normal_Transform_Prop(template_id, transform_type, level, exp_level, player_imp)
	local player = {}
	Object_InnerInit(player_imp, player, 0)

	return InitNormalTransformProp(template_id, transform_type, level, exp_level, player)
end
		
function Init_Replace_Transform_Prop(template_id, transform_type, level, exp_level, player_imp)
	local player = {}
	Object_InnerInit(player_imp, player, 0)

	return InitReplaceTransformProp(template_id, transform_type, level, exp_level, player)
end

xdofile("object_script.lua")
xdofile("script/transform_interface.lua")
