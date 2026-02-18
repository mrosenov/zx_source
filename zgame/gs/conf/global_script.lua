
function ClearAll(debugmode)
-- 关闭所有应该关闭的内容
	local ns = {};
	ns.date = os.date;
	ns.time = os.time;
	os = ns;
	io = nil;
	file = nil;
	if(not debugmode) then
	--	xdofile = nil
	end
	dofile = nil;
	load = nil;
	loadstring = nil;
	loadfile = nil;
	package = nil;
	math.random = __ABASE_Random;

end

function InitClear()
-- do nothing here
end

function Init(index, debugmode)
	xdofile = dofile
	if(index == 0) then
		ClearAll(debugmode);
		return "lottery.lua";
	elseif(index == 1) then
		xdofile("talisman_prepare.lua");
		ClearAll(debugmode);
		return "talisman.lua";
	elseif(index == 2) then
		ClearAll();
		return "petbedge.lua"
	elseif(index == 3) then
		ClearAll(debugmode);
		return "player.lua";
	elseif(index == 4) then
		ClearAll(debugmode);
		return "summon.lua"
	elseif(index == 5) then
		ClearAll(debugmode);
		return "magic.lua"
	elseif(index == 6) then
		ClearAll(debugmode);
		return "transform.lua"
	elseif(index == 7) then
		ClearAll(debugmode);
		return "misc.lua"
	elseif(index == 8) then
		ClearAll(debugmode);
		return "rune.lua"
	elseif(index == 9) then
		ClearAll(debugmode);
		return "astrology.lua"
	else
		error("wrong input")
	end
end

function Entry_Lottery_Generate(id, max_count, cur_count, ...)
	local t= {...};
	return LotterySelectNumber(id, max_count, t);
end

function Entry_Lottery_Cashing(id, ...)
	local t= {...};
	return LotteryCashing(id, t);
end

