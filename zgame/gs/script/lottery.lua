local lottery_template = {};

function AddLotteryTemplate(id, s1, c1)
	lottery_template[tonumber(id)] = {};
	lottery_template[tonumber(id)].Select = s1;
	lottery_template[tonumber(id)].Cashing = c1;
end

function LotteryType3613SelectNumber(list, max_count)
	return math.random(1,6); 
end

function LotteryType3613Cashing(cur_list)
	-- ÊıÖµÇø¼äÎª cur_list[1] .. cur_list[n] 
	local i,j
	local t={1,1,1}
	for i=1,3 do
	for j=i+1,6 do
	if (cur_list[i] == cur_list[j]) then
	t[i] = t[i]+1  	   
	end
	end	
	end
	if (cur_list[1]..cur_list[2]..cur_list[3]..cur_list[4]..cur_list[5]..cur_list[6]=="123456") then
	return 1,0,28,10,0,0;	
	end
	for i=1,3 do
	if (t[i] == 6) then
	return 1,100000,0,0,0,0;
	end
	if (t[i] == 5) then
	return 2,10000,0,0,0,0;
	end
	if (t[i] == 4) then
	return 3,100,0,0,0,0;
	end
	end		
	return 3,100,0,0,0,0;
end

--------------------------------------------------------------------------------------------------------------
--
--      ²ÊÆ±¶Ò½±º¯Êı½Ó¿Ú ÕâĞ©½Ó¿Ú»á±»³ÌĞò²¿ÃÅµÄº¯Êıµ÷ÓÃ£¬ÓÃÓÚÊµÏÖ²ÊÆ±Ïà¹Ø¹¦ÄÜ£¬
--      ÕâĞ©½Ó¿ÚµÄ¾ßÌåÊµÏÖ¿ÉÒÔ±»¸ü¸Ä£¬µ«ÊÇ±¾×¢ÊÍºÍ½Ó¿ÚµÄÍâÔÚ±íÏÖ²»Ó¦±»ĞŞ¸Ä¡£
--   
--      ½Ó¿Úº¯ÊıLotterySelectNumber 
--	ËµÃ÷£º±íÊ¾Éú³ÉÒ»¸ö²ÊÆ±ÖĞµÄÄ³Î»Êı×Ö
--      ²ÎÊı£º
--           id : ²ÊÆ±µÄÎïÆ· id£¬ ²»Í¬µÄ²ÊÆ±¿ÉÄÜÓĞ²»Í¬µÄ±íÏÖ
--           max_count: ÕâÕÅ²ÊÆ±×Ü¹²»áÓĞ¼¸¸öÊı×Ö×é³É
--           cur_list: µ±Ç°ÒÑ¾­Éú³ÉµÄ²ÊÆ±ºÅÂë #cur_list¾Í¿ÉÒÔµÃµ½µ±Ç°Éú³ÉµÄºÅÂëÊıÁ¿ 
--                     cur_list[1] µ½ cur_list[#cur_list] ¾ÍÊÇµ±Ç°ÒÑ¾­Éú³ÉµÄËùÓĞºÅÂë
--                     ±ê×¼µÄ¸ÅÂÊÉú³É¿ÉÄÜÎŞĞèÕâĞ©²ÎÊı
--      ·µ»Ø£ºÕâ¸öº¯ÊıÓ¦µ±·µ»ØÒ»¸ö1~255µÄÊıÖµ±íÊ¾±¾´ÎÉú³ÉµÄµ¥¸ö²ÊÆ±ºÅÂë£¬0±»ÈÏÎª
--            ´íÎóµÄ²ÊÆ±ºÅÂë£¬Òò¶ø±»×÷Îª´íÎóÇé¿ö½øĞĞ·µ»Ø£®
--
--      ************************************************************************
--
--      ½Ó¿Úº¯ÊıLotteryCashing
--      ËµÃ÷£º¶ÔÒ»ÕÅÒÑ¾­Éú³ÉÍê±ÏµÄ²ÊÆ±½øĞĞ¶Ò½±²Ù×÷
--      ²ÎÊı£º
--           id : ²ÊÆ±µÄÎïÆ· id£¬ ²»Í¬µÄ²ÊÆ±¿ÉÄÜÓĞ²»Í¬µÄ±íÏÖ
--           cur_list: µ±Ç°ÒÑ¾­Éú³ÉµÄ²ÊÆ±ºÅÂë #cur_list¾Í¿ÉÒÔµÃµ½µ±Ç°ËùÓĞµÄºÅÂë
--                     cur_list[1] µ½ cur_list[#cur_list] ±íÊ¾ÁËÒÀÕÕÉú³ÉË³ĞòµÄºÅÂë
--      ·µ»Ø£º¶Ò½±ºóµÄ½á¹ûÓÉ´Ë½Ó¿Ú·µ»Ø£¬¶Ò½±×Ü¹²ÓĞÁù¸öÕûÊıĞÍ²ÎÊı½øĞĞ·µ»Ø¡£
--          ·µ»ØµÄ¸ñÊ½Îª return b_level b_money b_item b_count b_bind b_period;
--          ¸÷¸ö·µ»ØÖµ½âÊÍÈçÏÂ£º
--           b_level: ±¾´Î²ÊÆ±µÄÖĞ½±µÈ¼¶£¬µÈ¼¶ÊÇÒ»¸ö´óÓÚµÈÓÚ0µÄÕûÊı
--           b_money: ±¾´Î²ÊÆ±µÄÖĞ½±½±½ğ£¬½±½ğÊÇÒ»¸ö´óÓÚµÈÓÚ0µÄÕûÊı
--           b_item:¡¡±¾´Î²ÊÆ±µÄ½±ÀøÎïÆ·id£¬Èç¹ûÎª0»ò-1Ôò±íÊ¾ÎŞ½±Æ·
--           b_count: ±¾´Î²ÊÆ±µÄ½±ÀøÎïÆ·ÊıÄ¿£¬Õâ¸öÖµÓ¦µ±´óÓÚµÈÓÚ0²¢²»´óÓÚ¶ÑµşÉÏÏŞ
--           b_bind: ±¾´ÎÖĞ½±·¢·ÅµÄÎïÆ·½±ÀøÊÇ·ñ°ó¶¨£¬ÓĞĞ§ÖµÎª0»òÕß1¡£Ö»ÓĞ¶ÑµşÉÏÏŞÎª1Õâ¸ö²ÎÊı²ÅÓĞÒâÒå
--           b_period:±¾´ÎÖĞ½±·¢·ÅÎïÆ·½±ÀøÊÇ·ñÓĞÓĞĞ§Ê±¼ä£¬ÕâÊÇÒ»¸ö´óÓÚµÈÓÚ0µÄÕûÊı£¬´óÓÚ0±íÊ¾´æÔÚÓĞĞ§Öµ¡£
--                    Ö»ÓĞ¶ÑµşÉÏÏŞÎª1Õâ¸ö²ÎÊı²ÅÓĞÒâÒå¡£
--         µ±·µ»Ø²ÎÊı¸öÊı²»Âú×ãÒªÇó£¬»òÕß²ÎÊıµÄ·¶Î§²»Âú×ãÒªÇóÊ±£¬Õâ¸öº¯ÊıµÄ·µ»Ø±»ÈÏÎªÊÇ´íÎóµÄ¡£
-------------------------------------------------------------------------------------------------------------

--
--   ÓĞ¹Ø½Ó¿ÚÊµÏÖ¡¡ºóÃæµÄÁ½¸ö½Ó¿ÚÊÇÒ»¸ö²Î¿¼ÊµÏÖ¡
--¡¡ Ê¹ÓÃifÓï¾ä½øĞĞÅĞ¶Ï²»Í¬µÄ²ÊÆ±¡¡È»ºóµ÷ÓÃ²»Í¬²ÊÆ±µÄÉú³ÉºÍÅĞ¶Ïº¯Êı
--

function LotterySelectNumber(id, max_count, cur_list)
	if(id == 3613) then
		return LotteryType3613SelectNumber(cur_list,max_count);
	elseif(id == 3614) then 
		-- .... Èç¹û3614 Ò²ÊÇÄ³ÖÖ²ÊÆ±µÄ»°¿ÉÒÔ×öÄ³Ğ©ÊÂÇé
	end
	return 0;	-- ·µ»Ø´íÎó
end

function LotteryCashing(id , cur_list)
	if(id == 3613) then
		return LotteryType3613Cashing(cur_list);
	elseif(id == 3614) then 
		-- .... Èç¹û3614 Ò²ÊÇÄ³ÖÖ²ÊÆ±µÄ»°¿ÉÒÔ×öÄ³Ğ©ÊÂÇé
	end
	return 0,0,0,0,0,0;	-- ·µ»Ø´íÎó
end

-------------------------------------------------------------------------------------------------
--
--      ÁíÍâÒ»¸ö±¸Ñ¡ÊµÏÖ¡¡Ê¹ÓÃtable±£´æËùÓĞµÄ²ÊÆ±½Ó¿Ú¡¡µ÷ÓÃÊ±Ö±½Ó²éÑ¯²¢µ÷ÓÃÖ®
--	²ÅºóÃæÒªÓÃAddLotteryTemplate¡¡½«Ã¿ÖÖ²ÊÆ±µÄÓ³Éä¼ÓÈëµ½ÆäÖĞ.
--	ÕâÖ»ÊÇÒ»¸öÌáÊ¾ĞÔ´úÂë£¬½Ó¿ÚµÄÃû×Ö±»ĞŞ¸ÄÎªXXXX_aux£¬Ö÷³ÌĞò²¢²»»áµ÷ÓÃÕâ¸öÃû³ÆµÄ½Ó¿Úº¯Êı
--

function LotterySelectNumber_aux(id, max_count, cur_list)
	local m = lottery_template[id];
	if(m) then
	return m.Select(cur_list, max_count);
	end
	return 0; 	-- ·µ»Ø´íÎó
end

function LotteryCashing_aux(id , cur_list)
	local m = lottery_template[id];
	if(m) then
	return m.Cashing(cur_list);
	end
	return 0;	-- ·µ»Ø´íÎó
end

AddLotteryTemplate(3613, LotteryType3613SelectNumber,LotteryType3613Cashing);

