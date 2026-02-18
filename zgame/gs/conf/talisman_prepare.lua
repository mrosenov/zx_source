talisman_tab = {};

function AddTalismanItem(id, init, levelup, reset)
	talisman_tab[id] = {};
	talisman_tab[id].Init = init;
	talisman_tab[id].LevelUp = levelup;
	talisman_tab[id].Reset = reset;
end

--	talisman¶ÔÏó½Ó¿Ú·Ö³ÉÈı²¿·Ö£¬·Ö±ğÈçÏÂ£º
--	A:»ñÈ¡»ù±¾Êı¾İ
--	QueryLevel()		µÃµ½·¨±¦¼¶±ğ
--	QueryExp()		µÃµ½·¨±¦µ±Ç°¾­Ñé

--	B:»ñÈ¡ºÍÉèÖÃ²ÎÊı 
--	  ËùÎ½²ÎÊı£¬¾ÍÊÇ½Å±¾¶¨ÒåµÄÊı¾İ£¬ ²ÎÊı¶¼ÊÇfloatÀàĞÍ£¬ÒÔindex/valueĞÎÊ½±£´æºÍ¶¨Òå
--        ·¨±¦µÄ³õÊ¼ÊıÖµ³É³¤ÂÊµÈ¶¼ÒÔ²ÎÊıµÄ·½Ê½´æÔÚ¡£ ²ÎÊıÇøÕ¼ÓÃµÄ¿Õ¼äºÍGetDataCount()µÄ·µ»ØÖµ
--	  ³ÉÕı±È¡£ËùÒÔ²»ÒªËæÒâ¸øºÜ´óµÄindexË÷Òı£¬ÕâÑù»áÕ¼ÓÃ´óÁ¿µÄÄÚ´æºÍÊı¾İ¿â¿Õ¼ä¡£
--	QueryData(index)	»ñÈ¡ÖÆ¶¨Ë÷ÒıµÄ²ÎÊı Èç¹û¸ÃË÷ÒıÎŞ¶ÔÓ¦²ÎÊı£¬Ôò·µ»Ø0.0
--	GetDataCount()		·µ»Øµ±Ç°ËùÓĞµÄ²ÎÊıÊıÄ¿ Õâ¸öÖµ¾ÍÊÇ×î´óµÄÓĞĞ§index+1
--	UpdateData(index, value)¸üĞÂÒ»¸ö²ÎÊı£¬Èç¹ûindex²»´æÔÚ£¬ÔòÔö¼ÓÒ»¸öÌõÄ¿ value±ØĞëÊÇÒ»¸öÊıÖµ
--	ClearData()		Çå³ıËùÓĞ´æÔÚµÄ²ÎÊı



--	C:ÉèÖÃ·¨±¦ÊôĞÔ
--	  ·¨±¦ÊôĞÔÊÇ·¨±¦ÕæÕıµÄĞ§¹û×÷ÓÃ£¬ ·¨±¦ÊôĞÔÓ¦¸Ã´Ó²ÎÊı¼ÆËãµÃµ½,·¨±¦ÊôĞÔ¶¼ÊÇÕûÊı,²»ÄÜÊ¹ÓÃ¸¡µãÊı.
--	  ×¢ÒâÃ¿´ÎÉèÖÃÇ°,·¨±¦ËùÓĞµÄÊôĞÔ°üÀ¨¸½¼ÓÊôĞÔ¶¼»á±»Çå¿Õ,ËùÒÔÃ¿´ÎÉèÖÃ¶¼Ó¦ÉèÖÃÈ«²¿ÊôĞÔ
--	SetStamina(value)	ÉèÖÃ·¨±¦×î´óµÄ¾«Á¦Öµ
--	SetQuality(value)	ÉèÖÃ·¨±¦µÄÆ·½×, ¹©¿Í»§¶ËÏÔÊ¾Ê¹ÓÃ
--	SetHP(value)		ÉèÖÃ·¨±¦Ôö¼ÓµÄÑªÁ¿
--	SetMP(value)		ÉèÖÃ·¨±¦Ôö¼ÓµÄÕæÆø
--	SetAttackRate(value)	ÉèÖÃ·¨±¦Ôö¼ÓµÄÃüÖĞ
--	SetDamage(low,high)	ÉèÖÃ·¨±¦Ôö¼ÓµÄ¹¥»÷Á¦
--	SetDodge(value)		ÉèÖÃ·¨±¦Ôö¼ÓµÄÉÁ¶ã
--	SetDefense(value)	ÉèÖÃ·¨±¦Ôö¼ÓµÄ·ÀÓù
--	SetResistance(index,value) ÉèÖÃ·¨±¦Ôö¼ÓµÄ¿¹ĞÔ
--	SetAddon(addon)		ÉèÖÃ·¨±¦µÄ¸½¼ÓÊôĞÔ, Ã¿´ÎÉèÖÃ¶¼»áÔö¼ÓÒ»¸ö¸½¼ÓÊôĞÔ addonÊÇÄ£°åÖĞµÄ¸½¼ÓÊôĞÔÄ£°åID

-------------------------------------------------------------------------------------------------
--	·¨±¦ÈÚºÏº¯ÊıËµÃ÷
--	CombineTalismans(id1,id2, talisman1,talisman2,cid,output)
--	id1 id2 ÎªĞèÒªÈÚºÏµÄÁ½¼ş·¨±¦µÄ id
--	talisman1 talisman2 ÎªĞèÒªÈÚºÏµÄÁ½¼ş·¨±¦µÄ¶ÔÏó ÕâÁ½¸ö¶ÔÏóÍ¬init reset levelupÈı¸öµ÷ÓÃ´«ÈëµÄtalisman
--	cid ÎªÈÚºÏµ÷ÕûµÀ¾ßµÄid
--	output Îª²ú³ö¿ØÖÆ¶ÔÏó£¬ ´Ë¶ÔÏó¿ÉÒÔµ÷ÓÃCreateItem½Ó¿Ú

--	output²ú³ö¿ØÖÆ¶ÔÏó½Ó¿ÚÈçÏÂ
--	CreateItem(id)		±íÃ÷Òª´´½¨Ò»¸öĞÂÎïÆ·£¬Èç¹û²»µ÷ÓÃ´Ë½Ó¿Ú£¬ ÔòÎŞ²ú³öÎïÆ·;
--				´´½¨Ê¹ÓÃ´Ë½Ó¿Ú´´½¨Ò»¸ö·¨±¦ÎïÆ·Ö®ºó£¬Ôò¿ÉÒÔ¿ªÊ¼Ê¹ÓÃ
--				talisman¶ÔÏóµÄËùÓĞÆäËû½Ó¿Ú£¬ ÈçQueryData UpdateData Set...ÏµÁĞ½Ó¿
--				Èç¹ûÃ»ÓĞ´´½¨ÎïÆ·»òÕß´´½¨ÁË·Ç·¨±¦ÎïÆ·£¬ÔòÒ²²»Ó¦µ÷ÓÃoutput¶ÔÏóµÄÆäËû½Ó¿Ú¡£

function test_rebuild(talisman)
	local level = talisman:QueryLevel();
	talisman:SetHP(level*talisman:QueryData(0) + 10);
	talisman:SetMP(level*talisman:QueryData(1) + 10);
	talisman:SetAttackRate(level*talisman:QueryData(2) + 10);
	talisman:SetResistance(1, level*talisman:QueryData(3) + 10);
	talisman:SetAddon(7);
	
	local value = talisman:QueryData(4);
	if(value > 0) then
		talisman:SetAddon(8);
		talisman:SetAddon(9);
		talisman:SetAddon(10);
	end
end

function test_init(id, talisman)

--Éú³É³õÖµ
	talisman:UpdateData(0, math.random() + 1.0);
	talisman:UpdateData(1, math.random() + 1.0);
	talisman:UpdateData(2, math.random() + 1.0);
	talisman:UpdateData(3, math.random() + 1.0);
	
	test_rebuild(talisman);
end

function test_levelup(id, talisman)
	test_rebuild(talisman);
end

function test_reset(id, talisman)
	test_rebuild(talisman);
end

AddTalismanItem(4662, test_init,test_levelup,test_reset);


function CombineTalismans(id1,id2, talisman1,talisman2,cid,output)
	if(cid ~= 0) then
		output:CreateItem(4662);		--Èç¹ûÓĞµ÷ÕûµÀ¾ß£¬´´½¨Ò»¸öĞÂµÄ¸ß¼¶·¨±¦
							--ºóÃæ¾Í¿ÉÒÔÕı³£Éú³ÉÎïÆ·ÁË
		output:UpdateData(4, 1);
		test_init(4662, output);
	else
		output:CreateItem(4670);		--Ã»ÓĞµ÷ÕûµÀ¾ß£¬´´½¨Ò»¸öÆÕÍ¨ÎïÆ·
							--ÕâÀï²»Ó¦ÔÚµ÷ÓÃÆäËû½Ó¿Ú
	end
end

xdofile("script/talisman.lua");
xdofile("script/talisman_addon.lua");
xdofile("script/talisman_combine.lua");

