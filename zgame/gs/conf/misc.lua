
xdofile("script/menology_info.lua")
function Menology_Init() 
	AddActivityInfo = __ABASE_MISC_AddActivityInfo

MenologyTable = {MenologyRecommend, MenologyWeekly, MenologyBattleField, MenologyMonster, MenologyDailyMissionOne, MenologyDailyMissionTwo, MenologyDailyActivity, MenologyDailyBattle, MenologyDailyDuplicate, MenologySystemFunction}

	for i in pairs(MenologyTable) do
		for k in pairs(MenologyTable[i]) do
			if k ~= "GetSelf" then
				local activity_id = MenologyTable[i][k]["id"]
				local map_id = MenologyTable[i][k]["descmapid"]
				local dest = MenologyTable[i][k]["descpos"]

				local i1, j1, i2, j2
				local pos_x, pos_y, pos_z
				
				i1,j1 = string.find(dest, ",")
				pos_x = string.sub(dest,0, j1-1)

				i2,j2 = string.find(dest, ",", j1+1)
				pos_y = string.sub(dest, j1+1, j2-1)

				pos_z = string.sub(dest, j2+1)

				AddActivityInfo(activity_id, map_id, pos_x, pos_y, pos_z)
			end
		end
	end
end

