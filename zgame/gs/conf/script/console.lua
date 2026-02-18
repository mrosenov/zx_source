---控制台

dofile("local_implement.lua")
dofile("talisman_addon.lua")
dofile("talisman.lua")
dofile("talisman_combine.lua");

---存储所有法宝的数组
talismans = {}

quit = false

function printHelp()
	print("console: available command:")
	print("console: 1)help, 2)exit, 3)new, 4)list, 5)view, 6)viewall, 7)levelup ,8)kill")
end


printHelp()
repeat
	command = io.read()
	if command == "exit" then
		quit = true
	elseif command == "help" then
		printHelp()
	elseif command == "new" then
		if table.getn(talismans) == 0 then
			print("console: (tell) available talisman tmplIDs:")
			local i
			local tmplIDs = ZLuaTalAvailableTmplIDs()
			for i=1,table.getn(tmplIDs) do
				print("console: (tell)",tmplIDs[i])
			end
			print("console: (ask) the tempID of new talisman...")
			local id = io.read()
			id = tonumber(id)
			if id then
				if ZLuaTalExistInTmplIDs(id) then
					local n = table.getn(talismans)
					talismans[n+1] = Talisman:new(talismans[n+1])
					talismans[n+1]:LocalSetID(id)
					TalismanFunction( talismans[n+1], "init" )
					if talismans[n+1]:LocalConfirmData() then
						print("console: (tell) the new talisman has been saved.")
					else
						print("console: (warn) the new talisman has been saved, but with wrong data.")
					end
				else
					print("console: (warn) the tmplID is not exist.")
				end
			else
				print("console: (warn) the tmplID is not exist..")
			end
		else
			print("console: (warn) you can only create one talisman now.")
		end
	elseif command == "list" then
		print("console:","index","tmplID","level")
		local i
		for i=1,table.getn(talismans) do
			print("console:",i,talismans[i]:LocalGetID(),talismans[i]:QueryLevel())
		end
	elseif command == "viewall" then
		local t = talismans[1]
		if t then
			t:LocalPrint(100,10)
		else
			print("console: (warn) no talisman exists.")
		end
	elseif command == "view" then
		local t = talismans[1]
		if t then
			t:LocalPrint(0,10)
		else
			print("console: (warn) no talisman exists.")
		end
	elseif command == "levelup" then
		local t = talismans[1]
		if t then
			t:LocalAddLevel(1)
			t.addons = {}
			TalismanFunction(t,"levelup")
			print("console: (tell) the talisman has grew to level ",t:QueryLevel() )
		else
			print("console: (warn) no talisman exists.")
		end
	elseif command == "kill" then
		if table.getn(talismans) == 0 then
			print("console: (warn) no talisman exists.")
		else
			talismans[1]:LocalClearAttribute()
			talismans[1]:LocalClearAddon()
			talismans[1]:ClearData()
			talismans[1] = nil
			print("console: (tell) the talisman has been killed.")
		end
	else
		print("console: (warn) unavailable command")
	end
until quit




