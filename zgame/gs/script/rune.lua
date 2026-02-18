function Rune_InnerInit(id, item, ess, t)
	t.__ID = id;
	t.__ITEM = item;
	t.__ESS = ess;
	
	t.QueryLevel 		= __ABASE_RUNE_QueryLevel;
	t.QueryExp 		= __ABASE_RUNE_QueryExp;
	t.QueryHole		= __ABASE_RUNE_QueryHole;
	t.QueryQuality		= __ABASE_RUNE_QueryQuality;
	t.SetQuality		= __ABASE_RUNE_SetQuality;
	t.SetRefineQuality	= __ABASE_RUNE_SetRefineQuality;

	t.GetAttCnt		= __ABASE_RUNE_GetAttCnt;
	t.GetAtt		= __ABASE_RUNE_GetAtt;
	t.SetAtt		= __ABASE_RUNE_SetAtt;
	t.SetRefineAtt		= __ABASE_RUNE_SetRefineAtt;

	t.GetRefineCnt		= __ABASE_RUNE_GetRefineCnt;
	t.GetResetCnt		= __ABASE_RUNE_GetResetCnt;

	t.GetAvgGrade		= __ABASE_RUNE_GetAvgGrade;
	t.SetAvgGrade		= __ABASE_RUNE_SetAvgGrade;
end


--鉴定
function Rune_Init_Entrance(id, item, ess, score, c_type)
	local rune = {};
	Rune_InnerInit(id, item, ess, rune);

	return RuneInit(id, rune, score, c_type);
end


--归元
function Rune_Reset_Entrance(id, item, ess, score)
	local rune = {};
	Rune_InnerInit(id, item, ess, rune);

	return RuneReset(id, rune, score);
end


--洗练
function Rune_Refine_Entrance(id, item, ess, score)
	local rune = {};
	Rune_InnerInit(id, item, ess, rune);

	return RuneRefine(id, rune, score);
end


--分解
function Rune_Decompose_Entrance(id, item, ess, score)
	local rune = {};
	Rune_InnerInit(id, item, ess, rune);

	return RuneDecompose(id, rune, score);
end

xdofile("script/runeInterface.lua");


