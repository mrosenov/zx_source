---°æ±¾
---2008.5.4ÏÂÎç17:00 ×¼±¸¶ÔÍâ

---	talisman¶ÔÏó½Ó¿Ú·Ö³ÉÈý²¿·Ö£¬·Ö±ðÈçÏÂ£º
---	A:»ñÈ¡»ù±¾Êý¾Ý
---	QueryLevel()		µÃµ½·¨±¦¼¶±ð
---	QueryExp()		µÃµ½·¨±¦0µ±Ç°¾­Ñé

---	B:»ñÈ¡ºÍÉèÖÃ²ÎÊý 
---	  ËùÎ½²ÎÊý£¬¾ÍÊÇ½Å±¾¶¨ÒåµÄÊý¾Ý£¬ ²ÎÊý¶¼ÊÇfloatÀàÐÍ£¬ÒÔindex/valueÐÎÊ½±£´æºÍ¶¨Òå
---        ·¨±¦µÄ³õÊ¼ÊýÖµ³É³¤ÂÊµÈ¶¼ÒÔ²ÎÊýµÄ·½Ê½´æÔÚ¡£ ²ÎÊýÇøÕ¼ÓÃµÄ¿Õ¼äºÍGetDataCount(0)µÄ·µ»ØÖµ
---	  ³ÉÕý±È¡£ËùÒÔ²»ÒªËæÒâ¸øºÜ´óµÄindexË÷Òý£¬ÕâÑù»áÕ¼ÓÃ´óÁ¿µÄÄÚ´æºÍÊý¾Ý¿â¿Õ¼ä¡£
---	QueryData(0,index)	»ñÈ¡ÖÆ¶¨Ë÷ÒýµÄ²ÎÊý Èç¹û¸ÃË÷ÒýÎÞ¶ÔÓ¦²ÎÊý£¬Ôò·µ»Ø0.0
---	GetDataCount(0)		·µ»Øµ±Ç°ËùÓÐµÄ²ÎÊýÊýÄ¿ Õâ¸öÖµ¾ÍÊÇ×î´óµÄÓÐÐ§index+1
---	UpdateData(0,index, value)¸üÐÂÒ»¸ö²ÎÊý£¬Èç¹ûindex²»´æÔÚ£¬ÔòÔö¼ÓÒ»¸öÌõÄ¿ value±ØÐëÊÇÒ»¸öÊýÖµ
---	ClearData(0)		Çå³ýËùÓÐ´æÔÚµÄ²ÎÊý



---	C:ÉèÖÃ·¨±¦ÊôÐÔ
---	  ·¨±¦ÊôÐÔÊÇ·¨±¦ÕæÕýµÄÐ§¹û×÷ÓÃ£¬ ·¨±¦ÊôÐÔÓ¦¸Ã´Ó²ÎÊý¼ÆËãµÃµ½,·¨±¦ÊôÐÔ¶¼ÊÇÕûÊý,²»ÄÜÊ¹ÓÃ¸¡µãÊý.
---	  ×¢ÒâÃ¿´ÎÉèÖÃÇ°,·¨±¦ËùÓÐµÄÊôÐÔ°üÀ¨¸½¼ÓÊôÐÔ¶¼»á±»Çå¿Õ,ËùÒÔÃ¿´ÎÉèÖÃ¶¼Ó¦ÉèÖÃÈ«²¿ÊôÐÔ
---	SetStamina(value)	ÉèÖÃ·¨±¦×î´óµÄ¾«Á¦Öµ
---	SetQuality(value)	ÉèÖÃ·¨±¦µÄÆ·½×, ¹©¿Í»§¶ËÏÔÊ¾Ê¹ÓÃ
---	SetHP(value)		ÉèÖÃ·¨±¦Ôö¼ÓµÄÑªÁ¿
---	SetMP(value)		ÉèÖÃ·¨±¦Ôö¼ÓµÄÕæÆø
---	SetAttackEnhance(value)	ÉèÖÃ·¨±¦µÄ¹¥»÷¶þ´ÎÇ¿»¯Öµ
---	SetDamage(low,high)	ÉèÖÃ·¨±¦Ôö¼ÓµÄ¹¥»÷Á¦
---	SetMPEnhance(value)	ÉèÖÃ·¨±¦µÄÕæÆø¶þ´ÎÇ¿»¯Öµ
---	SetHPEnhance(value)	ÉèÖÃ·¨±¦µÄÆøÑª¶þ´ÎÇ¿»¯Öµ
---	SetResistance(index,value) ÉèÖÃ·¨±¦Ôö¼ÓµÄ¿¹ÐÔ
---	SetAddon(addon)		ÉèÖÃ·¨±¦µÄ¸½¼ÓÊôÐÔ, Ã¿´ÎÉèÖÃ¶¼»áÔö¼ÓÒ»¸ö¸½¼ÓÊôÐÔ addonÊÇÄ£°åÖÐµÄ¸½¼ÓÊôÐÔÄ£°åID

-------------------------------------------------------------------------------------------------------------------------------------------------
---	·¨±¦ÈÚºÏº¯ÊýËµÃ÷
---	CombineTalismans(id1,id2, talisman1,talisman2,cid,output)
---	id1 id2 ÎªÐèÒªÈÚºÏµÄÁ½¼þ·¨±¦µÄ id
---	talisman1 talisman2 ÎªÐèÒªÈÚºÏµÄÁ½¼þ·¨±¦µÄ¶ÔÏó ÕâÁ½¸ö¶ÔÏóÍ¬init reset levelupÈý¸öµ÷ÓÃ´«ÈëµÄtalisman
---	cid ÎªÈÚºÏµ÷ÕûµÀ¾ßµÄid
---	output Îª²ú³ö¿ØÖÆ¶ÔÏó£¬ ´Ë¶ÔÏó¿ÉÒÔµ÷ÓÃCreateItem½Ó¿Ú

---	output²ú³ö¿ØÖÆ¶ÔÏó½Ó¿ÚÈçÏÂ
---	CreateItem(id)		±íÃ÷Òª´´½¨Ò»¸öÐÂÎïÆ·£¬Èç¹û²»µ÷ÓÃ´Ë½Ó¿Ú£¬ ÔòÎÞ²ú³öÎïÆ·;
---				´´½¨Ê¹ÓÃ´Ë½Ó¿Ú´´½¨Ò»¸ö·¨±¦ÎïÆ·Ö®ºó£¬Ôò¿ÉÒÔ¿ªÊ¼Ê¹ÓÃ
---				talisman¶ÔÏóµÄËùÓÐÆäËû½Ó¿Ú£¬ ÈçQueryData UpdateData Set...ÏµÁÐ½Ó¿

---				Èç¹ûÃ»ÓÐ´´½¨ÎïÆ·»òÕß´´½¨ÁË·Ç·¨±¦ÎïÆ·£¬ÔòÒ²²»Ó¦µ÷ÓÃoutput¶ÔÏóµÄÆäËû½Ó¿Ú¡£
--- InheritCombineLevel(true);
---				¿ÉÒÔÉèÖÃ¡°³ö²ú·¨±¦outputÊÇ·ñ¼Ì³ÐÖ÷Î»ÖÃ·¨±¦µÄ¼¶±ð¡±¡£
---				Èç¹û¡°¼Ì³Ð¡±£¬ÄÇÃ´outputµÄ¼¶±ð±äÎªÖ÷Î»ÖÃ·¨±¦µÄ¼¶±ð£»Èç¹û¡°²»¼Ì³Ð¡±£¬ÄÇÃ´outputµÄ¼¶±ðÈÔÈ»Îª1¡£

---AddTalismanItem(tmplID, ZLuaTalInit,ZLuaTalLevelup,ZluaTalReset);  Ìí¼Óº¯Êý


---ÓÃµ½µÄº¯Êý£¬ÃüÃû¹æÔò Ç°×º ZLuaTal
-------Êý¾Ý----------------------------------------------------------------------------------------------------------
---ZLuaTalAvailableTalismans()	´æ´¢ÒÑÓÐµÄ·¨±¦Ä£°åID
---ZLuaTalStaminaGrowth()				·¨±¦Ã¿¼¶¾«Á¦Öµ³É³¤Öµ
---ZLuaTalGrades()							·¨±¦µµÎ»¡£·µ»Ø·¨±¦ÊôÐÔµµÎ»Êý¡¢¸÷µµÊýÖµ
---ZLuaTalGradesUpperLimit(tmplID)	Ã¿¸ö·¨±¦µÄ ×ÜµµÎ»ÉÏÏÞ ÒÔ¼° Ã¿¸öÊôÐÔµÄµµÎ»ÉÏÏÞ
---ZLuaTalRandomWeight(tmplID, origin)	  Ã¿¸ö·¨±¦µÄ Ã¿¸öÊôÐÔµÄ Ã¿¸öµµÎ»µÄ Ëæ»úÈ¨ÖØ¡£ ´«ÈëµÄÊÇ·¨±¦Ä£°åID
---ZLuaTalFeedCost(level)				È¡µÃÄ³¸öµÈ¼¶·¨±¦³åÌìÁúÄ§ÑªµÄ»¨·Ñ
---ZLuaTalGetUnLearnID(id)			¸ù¾Ý¸½¼ÓÈËÎï¼¼ÄÜµÄ¸½¼ÓÊôÐÔÈ¡µÃÎ´Ñ§»á¸Ã¼¼ÄÜµÄ¸½¼ÓÊôÐÔid

-------Ë÷Òý¶¨ÒåÓë×ª»»------------------------------------------------------------------------------------------------
---ZLuaTalDataIndex()						·¨±¦dataË÷ÒýÊý×é
---ZLuaTalGradeIndex()					gradesUpperLimit±íµÄË÷Òý£¬¸úËæZLuaTalGradesUpperLimitµÄ¶¨ÒåÖÐµÄ±íÖÐË³Ðò¶ø¸Ä±ä
---ZLuaTalTransIndex(index18)		°ÑdataindexµÄË÷Òý×ª±äÎªgradesindexµÄË÷Òý
---ZLuaTalServerOrigins()				·¨±¦À´Ô´£¨·þÎñÆ÷¶ËÔ¼¶¨£©
---ZLuaTalClientOrigins()				·¨±¦À´Ô´£¨¿Í»§¶ËÔ¼¶¨£©

-------Éú³ÉÊôÐÔ------------------------------------------------------------------------------------------------------
---ZLuaTalGenerateConfusion()		·¨±¦¸ÉÈÅÒò×Ó
---ZLuaTalGenerateSpeciality(tmplID,origin,quality,allGrades)		Éú³ÉÌØÐÔ
---ZLuaTalGenerateClientOrigin(serverOrigin)		È¡µÃ·¨±¦À´Ô´£¬ÓÃÓÚ¿Í»§¶ËÏÔÊ¾
---ZLuaTalGenerateServerOrigin(clientOrigin)		È¡µÃ·¨±¦À´Ô´£¬ÓÃÓÚ·þÎñÆ÷¶ËÅÐ¶Ï
---ZLuaTalGenerteBirthTime()		È¡µÃ·¨±¦Éú³ÉÊ±¼ä
---ZLuaTalGenerateQuality(tmplID, origin, grades, nGrades)		È¡µÃ·¨±¦Æ·½×
---ZLuaTalGenerateQualityFromTalisman(tmplID, talisman) 			È¡µÃ·¨±¦Æ·½×£¬´Ó¾ßÌå·¨±¦ÖÐ

-------Ö÷Òª´¦Àíº¯Êý--------------------------------------------------------------------------------------------------
---CombineTalismans(id1,id2, talisman1,talisman2,cid,output)	·¨±¦ºÏ³ÉÈë¿Ú£¨±»µ÷ÓÃ£©
---ZLuaTalInit(id, talisman, origin)		·¨±¦³õÊ¼»¯£¨×¢²áÓÃ£©
---ZLuaTalLevelup(id, talisman) 				·¨±¦Éý¼¶£¨×¢²áÓÃ£©
---ZLuaTalReset(id, talisman) 					·¨±¦Ï´Á·£¨×¢²áÓÃ£©
---ZLuaTalRebuild(talisman,id)					·¨±¦±¾ÌåÓë¸½¼ÓÊôÐÔÖØÐÂ¼ÆËã
---ZLuaTalRandomResult(tmplID, origin)	Ëæ»úº¯Êý£¬·µ»ØÖ¸¶¨·¨±¦Ä£°åµÄÃ¿¸öÊôÐÔËæ»úºóµÄ½á¹û
---ZLuaTalCombineInit(id, speciality, growDirInData, growIncrement, combineValuePlus, talisman, output )		ÈÛÁ¶½á¹ûÎªA»òBÊ±µÄ³õÊ¼»¯º¯Êý

--------¹¦ÄÜÐÔº¯Êý---------------------------------------------------------------------------------------------------
---ZLuaTalExistInTmplIDs(id)													¼ì²é´«ÈëµÄÄ£°åIDÊÇ·ñÊÇ·¨±¦£¬Èç¹ûÊÇµÄ»°£¬·µ»Ø¸Ã·¨±¦µÄÈËÎïÐèÇóµÈ¼¶
---ZLuaTalGetValueWithBounds(startValue, endValue, currentStep, allStep)	a[]ÊÇµÈ²îÊýÁÐ£¬¸ø¶¨a[1]=startValueºÍa[allStep]=endValue£¬Çóa[currentStep]¡£
---ZLuaTalGetGradeFromValue(speciality, talisman)			´Ó·¨±¦Ä³¸öÊôÐÔ¾ßÌåÖµÈ¡µÃµµÎ»Êý
---ZLuaTalGetTalTypeFromID(tmplID)										·µ»ØÒ»¸öid´ú±íµÄ·¨±¦µÄÖÖÀà£¬"fabao" "feijian" "yushou" "chibang"


--------µ÷ÊÔÓÃº¯Êý---------------------------------------------------------------------------------------------------
---ZLuaTalDebugDisplay(talisman,id,output)	ÓÃÓÚ°Ñ·¨±¦·þÎñÆ÷¶ËÊý¾ÝÏÔÊ¾µ½Ç°Ì¨£¨½ödebugÓÃ£¬ÇÐÇÐ£©


-------Êý¾Ý----------------------------------------------------------------------------------------------------------------------------

---ÒÑÓÐµÄ·¨±¦Ä£°åID
function ZLuaTalAvailableTalismans()
	local talismans
	talismans = {}

	-- ²âÊÔÓÃ·¨±¦
	talismans["TEST"]												= { talType = "fabao", tmplID =  4662, requireLevel =   1, price =0.1, school = 0, skillAddon =  399, name = "²âÊÔ·¨±¦" }		---²âÊÔ·¨±¦
	talismans["foodtalisman"]								= { talType = "fabao", tmplID = 19523, requireLevel = 135, price =  1, school = 0, skillAddon =  399, name = "Ê³Îï·¨±¦" }		---²âÊÔ·¨±¦  Ê³Îï·¨±¦

	-- ·¨±¦
	-- Ë÷ÒýÃüÃû¹æÔò£ºtype_requireLevel_name		
	-- type: ÉÙÏÀ(0) NW£¬ ÉñÒá(0) SY£¬ Í¨ÓÃ(0) US
	-- type: ¹íÍõ(1) GW£¬ ºÏ»¶(2) HH£¬ ÇàÔÆ(3) QY£¬ ÌìÒô( 4) TY£¬ ¹íµÀ( 5) GD,  ·ÙÏã( 6) FX
	-- type: ¾ÅÀè(7) JL£¬ ÁÒÉ½(8) LS£¬ »³¹â(9) HG£¬ Ìì»ª(10) TH£¬ Ì«ê»(11) TH£¬ ¶«ÒÄ(12) DY   
	-- type: Ç£»ú(13)QJ£¬ Ó¢ÕÐ(14)YZ
	talismans["NW_1_BaGuaShi"] 							= { talType = "fabao", tmplID =  4703, tmplID1 = 6033,tmplID2 = 6047, requireLevel =   1, price =0.1, school = 0, skillAddon =  399, name = "°ËØÔÊ¯" }			---ÉÙÏÀ·¨±¦  °ËØÔÊ¯
	talismans["SY_1_WuSeShi"] 							= { talType = "fabao", tmplID = 30076, tmplID1 = 30077,tmplID2 = 30086, requireLevel =   1, price =0.1, school = 0, skillAddon = 3610, name = "ÎåÉ«Ê¯" }			---ÉñÒá·¨±¦  ÎåÉ«Ê¯
	talismans["LZ_1_TianLingShi"] 					= { talType = "fabao", tmplID = 58825, tmplID1 = 58826,tmplID2 = 58827, requireLevel =   1, price =0.1, school = 0, skillAddon = 34662, name = "XXÊ¯" }			---Áé×å·¨±¦  XXÊ¯

	talismans["QY_15_LiuHeJing"] 						= { talType = "fabao", tmplID =  4704, tmplID1 = 6036,tmplID2 = 6050, requireLevel =  15, price =  1, school = 3, skillAddon =  400, name = "ÁùºÏ¾µ" }			---ÇàÔÆÒ»ÖØ  ÁùºÏ¾µ
	talismans["GW_15_ZhuQueYin"] 						= { talType = "fabao", tmplID =  4705, tmplID1 = 6034,tmplID2 = 6048, requireLevel =  15, price =  1, school = 1, skillAddon =  401, name = "ÖìÈ¸Ó¡" }			---¹íÍõÒ»ÖØ  ÖìÈ¸Ó¡
	talismans["TY_15_LunHuiZhu"] 						= { talType = "fabao", tmplID =  4706, tmplID1 = 6037,tmplID2 = 6051, requireLevel =  15, price =  1, school = 4, skillAddon =  402, name = "ÂÖ»ØÖé" }			---ÌìÒôÒ»ÖØ  ÂÖ»ØÖé
	talismans["HH_15_SheXinLing"] 					= { talType = "fabao", tmplID =  4707, tmplID1 = 6035,tmplID2 = 6049, requireLevel =  15, price =  1, school = 2, skillAddon =  403, name = "ÉåÐÄÁå" }			---ºÏ»¶Ò»ÖØ  ÉåÐÄÁå
	talismans["GD_15_XueYuGuPian"]					= { talType = "fabao", tmplID = 17583, tmplID1 = 17596,tmplID2 = 17600, requireLevel =  15, price =  1, school = 5, skillAddon = 1806, name = "ÑªÓñ¹ÇÆ¬" }		---¹íµÀÒ»ÖØ  ÑªÓñ¹ÇÆ¬
	talismans["FX_15_NanMingYin"]						= { talType = "fabao", tmplID = 45525, tmplID1 = 45533,tmplID2 = 45541, requireLevel =  15, price =  1, school = 6, skillAddon = 33347, name = "ÄÏÃ÷Ó¡" }		---·ÙÏãÒ»ÖØ  ÄÏÃ÷Ó¡
	talismans["JL_15_NuShouFan"] 						= { talType = "fabao", tmplID = 30068, tmplID1 = 30078,tmplID2 = 30087, requireLevel =  15, price =  1, school = 7, skillAddon = 3613, name = "Å­ÊÞá¦" }			---¾ÅÀèÒ»ÖØ  Å­ÊÞá¦
	talismans["LS_15_DanZhu"]								= { talType = "fabao", tmplID = 30072, tmplID1 = 30082,tmplID2 = 30091, requireLevel =  15, price =  1, school = 8, skillAddon = 3625, name = "µ¤Öì" }			  ---ÁÒÉ½Ò»ÖØ  µ¤Öì
	talismans["HG_15_QiSha"] 								= { talType = "fabao", tmplID = 38883, tmplID1 = 38903,tmplID2 = 38918, requireLevel =  15, price =  1, school = 9, skillAddon = 3943, name = "ÆßÉ±" }				---»³¹âÒ»ÖØ  ÆßÉ±
	talismans["TH_15_JiuYinLuo"]						= { talType = "fabao", tmplID = 38887, tmplID1 = 38907,tmplID2 = 38922, requireLevel =  15, price =  1, school =10, skillAddon = 3955, name = "¾ÅÒôÂÝ" }			---Ìì»ªÒ»ÖØ  ¾ÅÒôÂÝ
	talismans["TH_15_LeiGongMo"]						= { talType = "fabao", tmplID = 45529, tmplID1 = 45537,tmplID2 = 45545, requireLevel =  15, price =  1, school =11, skillAddon = 33359, name = "À×¹«Ä«" }		---Ì«ê»Ò»ÖØ  À×¹«Ä«
	talismans["CH_15_lvdouqi"]							= { talType = "fabao", tmplID = 53383, tmplID1 = 53389,tmplID2 = 53393, requireLevel =  15, price =  1, school =12, skillAddon = 34015, name = "ÂÊ¶µÆì" }		---³½»ÊÒ»ÖØ  ÂÊ¶µÆì
	talismans["QJ_15_tianjimu"]							= { talType = "fabao", tmplID = 58798, tmplID1 = 58806,tmplID2 = 58814, requireLevel =  15, price =  1, school =13, skillAddon = 34667, name = "Ìì¼«Ä¾" }		---Ç£»úÒ»ÖØ  Ìì¼«Ä¾
	talismans["YZ_15_lveying"]							= { talType = "fabao", tmplID = 58802, tmplID1 = 58810,tmplID2 = 58818, requireLevel =  15, price =  1, school =14, skillAddon = 34687, name = "ÂÓÓ°" }		---Ó¢ÕÐÒ»ÖØ  ÂÓÓ°

	talismans["US_45_ShenMuShai"] 					= { talType = "fabao", tmplID =  4713, tmplID1 = 6043,tmplID2 = 6057, requireLevel =  45, price = 20, school = 0, skillAddon =  409, name = "ÉñÄ¾÷»" }			---Í¨ÓÃ¶þÖØ  ÉñÄ¾÷»
	talismans["US_45_ShenMuShai_WZGL"]			= { talType = "fabao", tmplID = 24710, tmplID1 = 24721,tmplID2 = 24722, requireLevel =  45, price = 20, school = 0, skillAddon =  409, name = "ÉñÄ¾÷»" }			---Í¨ÓÃ¶þÖØ  ÉñÄ¾÷»(ÍõÕß¹éÀ´¼ÍÄî°æ)

	talismans["QY_75_QiXingJian"] 					= { talType = "fabao", tmplID =  4708, tmplID1 = 6040,tmplID2 = 6054, requireLevel =  75, price = 20, school = 3, skillAddon =  404, name = "ÆßÐÇ½£" }			---ÇàÔÆÈýÖØ  ÆßÐÇ½£
	talismans["GW_75_SiLingFan"] 						= { talType = "fabao", tmplID =  4709, tmplID1 = 6038,tmplID2 = 6052, requireLevel =  75, price = 20, school = 1, skillAddon =  405, name = "ËÄÁéá¦" }			---¹íÍõÈýÖØ  ËÄÁéá¦
	talismans["TY_75_JinGangChu"] 					= { talType = "fabao", tmplID =  4710, tmplID1 = 6041,tmplID2 = 6055, requireLevel =  75, price = 20, school = 4, skillAddon =  406, name = "½ð¸ÕèÆ" }			---ÌìÒôÈýÖØ  ½ð¸ÕèÆ
	talismans["HH_75_DuoQingHuan"]					= { talType = "fabao", tmplID =  4711, tmplID1 = 6039,tmplID2 = 6053, requireLevel =  75, price = 20, school = 2, skillAddon =  407, name = "¶àÇé»·" }			---ºÏ»¶ÈýÖØ  ¶àÇé»·
	talismans["GD_75_PoSha"] 								= { talType = "fabao", tmplID = 17584, tmplID1 = 17597,tmplID2 = 17601, requireLevel =  75, price = 20, school = 5, skillAddon = 1809, name = "ÆÆÉ²" }				---¹íµÀÈýÖØ  ÆÆÉ²
	talismans["FX_75_HengTianChi"] 					= { talType = "fabao", tmplID = 45526, tmplID1 = 45534,tmplID2 = 45542, requireLevel =  75, price = 20, school = 6, skillAddon = 33350, name = "ºâÌì³ß" }		---·ÙÏãÈýÖØ  ÆÆÉ²
	talismans["JL_75_XueFengQiang"]					= { talType = "fabao", tmplID = 30069, tmplID1 = 30079,tmplID2 = 30088, requireLevel =  75, price = 20, school = 7, skillAddon = 3616, name = "Ñª·ãÇ¹" }			---¾ÅÀèÈýÖØ  Ñª·ãÇ¹
	talismans["LS_75_FanHunXiang"] 					= { talType = "fabao", tmplID = 30073, tmplID1 = 30083,tmplID2 = 30092, requireLevel =  75, price = 20, school = 8, skillAddon = 3628, name = "·µ»êÏã" }			---ÁÒÉ½ÈýÖØ  ·µ»êÏã
	talismans["HG_75_PoJun"]								= { talType = "fabao", tmplID = 38884, tmplID1 = 38904,tmplID2 = 38919, requireLevel =  75, price = 20, school = 9, skillAddon = 3946, name = "ÆÆ¾ü" }				---»³¹âÈýÖØ  ÆÆ¾ü
	talismans["TH_75_YaFeng"] 							= { talType = "fabao", tmplID = 38888, tmplID1 = 38908,tmplID2 = 38923, requireLevel =  75, price = 20, school =10, skillAddon = 3958, name = "ÑÇ·ç" }				---Ìì»ªÈýÖØ  ÑÇ·ç
	talismans["TH_75_ChunQiuSuo"] 					= { talType = "fabao", tmplID = 45530, tmplID1 = 45538,tmplID2 = 45546, requireLevel =  75, price = 20, school =11, skillAddon = 33362, name = "´ºÇïËø" }		---Ì«ê»ÈýÖØ  ´ºÇïËø
	talismans["CH_75_tuogu"]								= { talType = "fabao", tmplID = 53384, tmplID1 = 53390,tmplID2 = 53394, requireLevel =  75, price = 20, school =12, skillAddon = 34018, name = "ö¾¹Ä" }			---³½»ÊÈýÖØ  ö¾¹Ä
	talismans["QJ_75_luohouyu"] 						= { talType = "fabao", tmplID = 58799, tmplID1 = 58807,tmplID2 = 58815, requireLevel =  75, price = 20, school =13, skillAddon = 34672, name = "ÂÞ²TÓñ" }		---Ç£»úÈýÖØ  ÂÞ²TÓñ
	talismans["YZ_75_poming"]								= { talType = "fabao", tmplID = 58803, tmplID1 = 58811,tmplID2 = 58819, requireLevel =  75, price = 20, school =14, skillAddon = 34692, name = "ÆÆÃü" }			---Ó¢ÕÐÈýÖØ  ÆÆÃü

	talismans["US_75_ShanHeShan"] 					= { talType = "fabao", tmplID =  4712, tmplID1 = 6042,tmplID2 = 6056, requireLevel =  75, price = 20, school = 0, skillAddon =  408, name = "É½ºÓÉÈ" }			---Í¨ÓÃÈýÖØ  É½ºÓÉÈ	
	talismans["US_75_WanDuDing"]						= { talType = "fabao", tmplID =  4714, tmplID1 = 6044,tmplID2 = 6058, requireLevel =  75, price = 20, school = 0, skillAddon =  410, name = "Íò¶¾¶¦" }			---Í¨ÓÃÈýÖØ  Íò¶¾¶¦
	talismans["US_75_YuJingPing"] 					= { talType = "fabao", tmplID =  4715, tmplID1 = 6045,tmplID2 = 6059, requireLevel =  75, price = 20, school = 0, skillAddon =  411, name = "Óñ¾»Æ¿" }			---Í¨ÓÃÈýÖØ  Óñ¾»Æ¿
	talismans["US_75_LingLongTa"] 					= { talType = "fabao", tmplID = 13316, tmplID1 = 13356,tmplID2 = 13357, requireLevel =  75, price = 20, school = 0, skillAddon = 1653, name = "ÁáççËþ" }			---Í¨ÓÃÈýÖØ  ÁáççËþ

	talismans["QY_105_HuPoZhuLing"] 				= { talType = "fabao", tmplID = 10866, tmplID1 = 10878,tmplID2 = 10890, requireLevel = 105, price = 50, school = 3, skillAddon =  860, name = "çúçêÖìç±" }		---ÇàÔÆËÄÖØ  çúçêÖìç±
	talismans["GW_105_QianKunQingGuangJie"] = { talType = "fabao", tmplID = 10871, tmplID1 = 10883,tmplID2 = 10895, requireLevel = 105, price = 50, school = 1, skillAddon =  865, name = "Ç¬À¤Çà¹â½ä" }	---¹íÍõËÄÖØ  Ç¬À¤Çà¹â½ä
	talismans["TY_105_RuYiQianKunDai"] 			= { talType = "fabao", tmplID = 10874, tmplID1 = 10886,tmplID2 = 10898, requireLevel = 105, price = 50, school = 4, skillAddon =  868, name = "ÈçÒâÇ¬À¤´ü" }	---ÌìÒôËÄÖØ  ÈçÒâÇ¬À¤´ü
	talismans["HH_105_QianMeiLian"] 				= { talType = "fabao", tmplID = 10870, tmplID1 = 10882,tmplID2 = 10894, requireLevel = 105, price = 50, school = 2, skillAddon =  864, name = "Ç§ÃÄÁ«" }			---ºÏ»¶ËÄÖØ  Ç§ÃÄÁ«
	talismans["GD_105_QianDuGu"] 						= { talType = "fabao", tmplID = 17585, tmplID1 = 17598,tmplID2 = 17602, requireLevel = 105, price = 50, school = 5, skillAddon = 1812, name = "Ç§¶¾¹Æ" }			---¹íµÀËÄÖØ  Ç§¶¾¹Æ
	talismans["FX_105_HuoLingLong"]					= { talType = "fabao", tmplID = 45527, tmplID1 = 45535,tmplID2 = 45543, requireLevel = 105, price = 50, school = 6, skillAddon = 33353, name = "»ðÁáçç" }		---·ÙÏãËÄÖØ  »ðÁáçç
	talismans["JL_105_GangLan"] 						= { talType = "fabao", tmplID = 30070, tmplID1 = 30080,tmplID2 = 30089, requireLevel = 105, price = 50, school = 7, skillAddon = 3619, name = "î¸á°" }				---¾ÅÀèËÄÖØ  î¸á°
	talismans["LS_105_FuYingGuJing"] 				= { talType = "fabao", tmplID = 30074, tmplID1 = 30084,tmplID2 = 30093, requireLevel = 105, price = 50, school = 8, skillAddon = 3631, name = "¸¡Ó°¹Å¾µ" }		---ÁÒÉ½ËÄÖØ  ¸¡Ó°¹Å¾µ
	talismans["HG_105_TanLang"] 						= { talType = "fabao", tmplID = 38885, tmplID1 = 38905,tmplID2 = 38920, requireLevel = 105, price = 50, school = 9, skillAddon = 3949, name = "Ì°ÀÇ" }				---»³¹âËÄÖØ  Ì°ÀÇ
	talismans["TH_105_LiuLiZhan"] 					= { talType = "fabao", tmplID = 38889, tmplID1 = 38909,tmplID2 = 38924, requireLevel = 105, price = 50, school =10, skillAddon = 3961, name = "ÁðÁ§Õµ" }			---Ìì»ªËÄÖØ  ÁðÁ§Õµ
	talismans["TH_105_ZiMuYinYangKou"]			= { talType = "fabao", tmplID = 45531, tmplID1 = 45539,tmplID2 = 45547, requireLevel = 105, price = 50, school =11, skillAddon = 33365, name = "×ÓÄ¸ÒõÑô¿Û" }---Ì«ê»ËÄÖØ  ×ÓÄ¸ÒõÑô¿Û
	talismans["CH_105_luobaojinqian"]				= { talType = "fabao", tmplID = 53385, tmplID1 = 53391,tmplID2 = 53395, requireLevel = 105, price = 50, school =12, skillAddon = 34021, name = "Âä±¦½ðÇ®" }	---³½»ÊËÄÖØ  Âä±¦½ðÇ®
	talismans["QJ_105_jinyubian"]						= { talType = "fabao", tmplID = 58800, tmplID1 = 58808,tmplID2 = 58816, requireLevel = 105, price = 50, school =13, skillAddon = 34677, name = "½ðÓð±ä" }---Ç£»úËÄÖØ  ½ðÓð±ä
	talismans["YZ_105_shuolei"]							= { talType = "fabao", tmplID = 58804, tmplID1 = 58812,tmplID2 = 58820, requireLevel = 105, price = 50, school =14, skillAddon = 34697, name = "Ë¸À×" }	---Ó¢ÕÐËÄÖØ  Ë¸À×

	talismans["US_105_JiuLongShenHuoZhao"]	= { talType = "fabao", tmplID = 10873, tmplID1 = 10885,tmplID2 = 10897, requireLevel = 105, price = 50, school = 0, skillAddon =  867, name = "¾ÅÁúÉñ»ðÕÖ" }	---Í¨ÓÃËÄÖØ  ¾ÅÁúÉñ»ðÕÖ	
	talismans["US_105_JinHuLu"] 						= { talType = "fabao", tmplID = 10869, tmplID1 = 10881,tmplID2 = 10893, requireLevel = 105, price = 50, school = 0, skillAddon =  863, name = "½ðºùÂ«" }			---Í¨ÓÃËÄÖØ  ½ðºùÂ«£¨ÉÌ³ÇÂôRMB£©
	talismans["US_105_YinYangJing"] 				= { talType = "fabao", tmplID = 10867, tmplID1 = 10879,tmplID2 = 10891, requireLevel = 105, price = 50, school = 0, skillAddon =  861, name = "ÒõÑô¾µ" }			---Í¨ÓÃËÄÖØ  ÒõÑô¾µ£¨2007-11-27²ÊÆ±³ö£©
	talismans["US_105_YinYangJing_WZGL"]		= { talType = "fabao", tmplID = 24711, tmplID1 = 24723,tmplID2 = 24724, requireLevel = 105, price = 50, school = 0, skillAddon =  861, name = "ÒõÑô¾µ" }			---Í¨ÓÃËÄÖØ  ÒõÑô¾µ(ÍõÕß¹éÀ´¼ÍÄî°æ)
	talismans["US_105_LuoHunDeng"] 					= { talType = "fabao", tmplID = 15868, tmplID1 = 15869,tmplID2 = 15870, requireLevel = 105, price = 50, school = 0, skillAddon = 1738, name = "Âä»êµÆ" }			---Í¨ÓÃËÄÖØ  Âä»êµÆ£¨2008-03-18²ÊÆ±³ö£©
	talismans["US_105_ZhenShanHeShan"] 			= { talType = "fabao", tmplID = 17829, tmplID1 = 17830,tmplID2 = 17831, requireLevel = 105, price = 50, school = 0, skillAddon = 1753, name = "ÕæÉ½ºÓÉÈ" }		---Í¨ÓÃËÄÖØ  åÐÒ£ÉÈ

	talismans["GW_135_TianMoZhi"] 					= { talType = "fabao", tmplID = 13774, tmplID1 = 13790,tmplID2 = 13794, requireLevel = 135, price = 50, school = 1, skillAddon = 1706, name = "ÌìÄ§Ö¸" }			---¹íÍõÎåÖØ  ÌìÄ§Ö¸
	talismans["QY_135_RuoXue"] 							= { talType = "fabao", tmplID = 13773, tmplID1 = 13789,tmplID2 = 13793, requireLevel = 135, price = 50, school = 3, skillAddon = 1703, name = "ÈôÑ©" }				---ÇàÔÆÎåÖØ  ÈôÑ©
	talismans["HH_135_BaJiYouLong"]					= { talType = "fabao", tmplID = 13771, tmplID1 = 13787,tmplID2 = 13791, requireLevel = 135, price = 50, school = 2, skillAddon = 1697, name = "°Ë¼«ÓÎÁú" }		---ºÏ»¶ÎåÖØ  °Ë¼«ÓÎÁú
	talismans["TY_135_LiuDaoShengMie"]			= { talType = "fabao", tmplID = 13772, tmplID1 = 13788,tmplID2 = 13792, requireLevel = 135, price = 50, school = 4, skillAddon = 1700, name = "ÁùµÀÉúÃð" }		---ÌìÒôÎåÖØ  ÁùµÀÉúÃð
	talismans["GD_135_GuiQing"] 						= { talType = "fabao", tmplID = 17586, tmplID1 = 17599,tmplID2 = 17603, requireLevel = 135, price = 50, school = 5, skillAddon = 1815, name = "¹íóÀ" }				---¹íµÀÎåÖØ  ¹íóÀ
	talismans["FX_135_JiuHanNingBingCi"] 		= { talType = "fabao", tmplID = 45528, tmplID1 = 45536,tmplID2 = 45544, requireLevel = 135, price = 50, school = 6, skillAddon = 33356, name = "¾Åº®Äý±ù´Ì" }---·ÙÏãÎåÖØ  ¾Åº®Äý±ù´Ì
	talismans["JL_135_SangMenJian"] 				= { talType = "fabao", tmplID = 30071, tmplID1 = 30081,tmplID2 = 30090, requireLevel = 135, price = 50, school = 7, skillAddon = 3622, name = "É¥ÃÅ½£" }			---¾ÅÀèÎåÖØ  É¥ÃÅ½£
	talismans["LS_135_LuoShu"] 							= { talType = "fabao", tmplID = 30075, tmplID1 = 30085,tmplID2 = 30094, requireLevel = 135, price = 50, school = 8, skillAddon = 3634, name = "ÂåÊé" }				---ÁÒÉ½ÎåÖØ  ÂåÊé
	talismans["HG_135_JueYing"] 						= { talType = "fabao", tmplID = 38886, tmplID1 = 38906,tmplID2 = 38921, requireLevel = 135, price = 50, school = 9, skillAddon = 3952, name = "¾øÓ°" }				---»³¹âÎåÖØ  ¾øÓ°
	talismans["TH_135_BiLuoFeiHong"] 				= { talType = "fabao", tmplID = 38890, tmplID1 = 38910,tmplID2 = 38925, requireLevel = 135, price = 50, school =10, skillAddon = 3964, name = "±ÌÂä·Éºè" }		---Ìì»ªÎåÖØ  ±ÌÂä·Éºè
	talismans["TH_135_XiHuangZhong"] 				= { talType = "fabao", tmplID = 45532, tmplID1 = 45540,tmplID2 = 45548, requireLevel = 135, price = 50, school =11, skillAddon = 33368, name = "ôË»ÊÖÓ" }		---Ì«ê»ÎåÖØ  ôË»ÊÖÓ
	talismans["CH_135_huangji"]							= { talType = "fabao", tmplID = 53386, tmplID1 = 53392,tmplID2 = 53396, requireLevel = 135, price = 50, school =12, skillAddon = 34024, name = "»Ê¼«" }		  ---³½»ÊÎåÖØ  »Ê¼«
	talismans["QJ_135_chixinglongyan"] 			= { talType = "fabao", tmplID = 58801, tmplID1 = 58809,tmplID2 = 58817, requireLevel = 135, price = 50, school =13, skillAddon = 34682, name = "³àÐÇÁúÑ×" }		---Ç£»úÎåÖØ  ³àÐÇÁúÑ×
	talismans["YZ_135_zhuihun"]							= { talType = "fabao", tmplID = 58805, tmplID1 = 58813,tmplID2 = 58821, requireLevel = 135, price = 50, school =14, skillAddon = 34702, name = "×·»ê" }		  ---Ó¢ÕÐÎåÖØ  ×·»ê

	talismans["US_135_PanLongFan"] 					= { talType = "fabao", tmplID = 10868, tmplID1 = 10880,tmplID2 = 10892, requireLevel = 135, price = 50, school = 0, skillAddon =  862, name = "ó´Áúá¦" }			---Í¨ÓÃÎåÖØ  ó´Áúá¦	
	talismans["US_135_WuLongLun"] 					= { talType = "fabao", tmplID = 10872, tmplID1 = 10884,tmplID2 = 10896, requireLevel = 135, price = 50, school = 0, skillAddon =  866, name = "ÎåÁúÂÖ" }			---Í¨ÓÃÎåÖØ  ÎåÁúÂÖ£¨ÉÌ³ÇÂôRMB£©
	talismans["US_135_FanTianYin"] 					= { talType = "fabao", tmplID = 20090, tmplID1 = 20091,tmplID2 = 20092, requireLevel = 135, price = 50, school = 0, skillAddon = 2037, name = "»ìÔª½ð¶·" }		---Í¨ÓÃÎåÖØ  »ìÔª½ð¶·£¨2008-09-23²ÊÆ±³ö£©
	talismans["US_135_ChangMingDeng"]				= { talType = "fabao", tmplID = 20491, tmplID1 = 20492,tmplID2 = 20493, requireLevel = 135, price = 50, school = 0, skillAddon = 2153, name = "öÉ¹âÁú¹Ò" }		---Í¨ÓÃÎåÖØ  öÉ¹âÁú¹Ò£¨2008-10-23²ÊÆ±³ö£©
	talismans["US_135_BaoKuZhiHun"]					= { talType = "fabao", tmplID = 34820, tmplID1 = 34934,tmplID2 = 34935, requireLevel = 135, price = 50, school = 0, skillAddon = 3825, name = "±¦¿âÖ®»ê" }		---Í¨ÓÃÎåÖØ  ±¦¿âÖ®»ê

	talismans["US_135_SheHun"] 							= { talType = "fabao", tmplID = 10875, tmplID1 = 10887,tmplID2 = 10899, requireLevel = 135, price = 100, school = 0, skillAddon = 869, name = "Éã»ê" }				---Í¨ÓÃÎåÖØ  Éã»ê
	talismans["US_145_ShiXueZhu"] 					= { talType = "fabao", tmplID = 10876, tmplID1 = 10888,tmplID2 = 10900, requireLevel = 135, price = 100, school = 0, skillAddon = 870, name = "ÊÉÑªÖé" }			---Í¨ÓÃÎåÖØ  ÊÉÑªÖé
	talismans["US_145_ShiHun"] 							= { talType = "fabao", tmplID = 10877, tmplID1 = 10889,tmplID2 = 10901, requireLevel = 135, price = 100, school = 0, skillAddon = 871, name = "ÊÉ»ê" }				---Í¨ÓÃÎåÖØ  ÊÉ»ê
	talismans["US_145_TianYa"] 							= { talType = "fabao", tmplID = 43475, tmplID1 = 43476,tmplID2 = 43478, requireLevel = 135, price = 100, school = 0, skillAddon = 32954, name = "Ììçð" }				---Í¨ÓÃÎåÖØ  Ììçð£¨20110406£©

	talismans["US_75_RiYueBaoJian"] 				= { talType = "fabao", tmplID = 22121, tmplID1 = 22122,tmplID2 = 22123, requireLevel = 135, price =  20, school = 0, skillAddon =2598, name = "ÈÕÔÂ±¦¼ø" }		---Í¨ÓÃÈýÖØ  ÈÕÔÂ±¦¼ø
	talismans["US_45_YuanXiaoDeng"] 				= { talType = "fabao", tmplID = 22906, tmplID1 = 6033,tmplID2 = 6047, requireLevel =  45, price =  20, school = 0, skillAddon =3088, name = "ÔªÏüµÆ" }			---Í¨ÓÃÒ»ÖØ  ÔªÏüµÆ
	talismans["US_105_FuTu"] 								= { talType = "fabao", tmplID = 23353, tmplID1 = 23354,tmplID2 = 23355, requireLevel = 135, price =  50, school = 0, skillAddon =3162, name = "¸¡ÍÀ" }				---Í¨ÓÃËÄÖØ  ¸¡ÍÀ
	talismans["US_135_YinHunFan"] 					= { talType = "fabao", tmplID = 25732, tmplID1 = 25737,tmplID2 = 25738, requireLevel = 135, price =  50, school = 0, skillAddon =3391, name = "Òý»êá¦" }			---Í¨ÓÃÎåÖØ  Òý»êá¦

	-- ²âÊÔÓÃ·É½£
--	talismans["ceshifeibao"]			= { talType = "feijian", tmplID = 18373, requireLevel = 1, price = 0.1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = false, name = "²âÊÔ·É±¦" }		---²âÊÔ·É½£  ÉñÐÐ°Ù±ä
	talismans["feiafei"]					= { talType = "feijian", tmplID =  4685, requireLevel = 1, price = 0.1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = false, name = "·É½£" }				---²âÊÔ·É½£  ·É¶î·É
	talismans["ceshixingxiang"]		= { talType = "feijian", tmplID = 18883, requireLevel = 1, price = 0.1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = false, name = "²âÊÔecm" }			---²âÊÔ·É½£
	talismans["ceshichibang"]			= { talType = "chibang", tmplID = 34010, requireLevel = 1, price = 0.1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = false, name = "²âÊÔ³á°ò" }		---²âÊÔ³á°ò

	-- ÔÚ½Å±¾Àï£º·É½£ºÍÓùÊÞµÄÎ¨Ò»Çø±ðÊÇÆ·ÖÊÃèÊöÓÃ´Ê²»Í¬
	-- ·É½£  HL:ºèÀûÉÌ³Ç  WZGL:±êÓÐÍõÕß¹éÀ´¼ÍÄî°æ  PM:Î¨Ò»×îµÍ¼ÛPM  LB:Àñ°ü  XS:ÐÂÊÖ  RYZL:ÈÙÒ«Ö®Â·¼ÍÄî°æ
	talismans["longxiang"]				= { talType = "feijian", tmplID = 18904, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "Áúæø" }				---²ÊÆ±·É½£  Áúæø
	talismans["longxiang_HL"]			= { talType = "feijian", tmplID = 31815, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = false, name = "Áúæø" }				---ºèÀûÉÌ³Ç  Áúæø£¨2009-09-08£©
	talismans["shuoguang"]				= { talType = "feijian", tmplID = 18905, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = false, name = "Ë¸¹â" }				---ÉÌ³Ç·É½£  Ë¸¹â
	talismans["shuoguang2"]				= { talType = "feijian", tmplID = 21176, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "Ë¸¹â" }				---ÀÏÍæ¼Ò»Ø¹é·É½£  ´óÀñ°üË¸¹â£¨2008-11-12£©
	talismans["shuoguang_HL"]			= { talType = "feijian", tmplID = 25119, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = false, name = "Ë¸¹â" }				---ºèÀûÉÌ³Ç  Ë¸¹â£¨2009-05-19£©
	talismans["shuoguang_XS"]			= { talType = "feijian", tmplID = 41795, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "Ë¸¹â" }				---ÐÂÊÖ¿¨  Ë¸¹â£¨2010-10-22£©
	talismans["jueying"]					= { talType = "feijian", tmplID = 18906, requireLevel = 60, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "¾øÓ°" }				---ÈÎÎñ½±ÀøÊ±ÏÞ·É½£  ¾øÓ°
	talismans["jueying2"]					= { talType = "feijian", tmplID = 18915, requireLevel = 60, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "¾øÓ°" }				---ÖÜÈÎÎñÊ±ÏÞ·É½£    ¾øÓ°
	talismans["liudaolunhui"]			= { talType = "feijian", tmplID = 18907, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = false, name = "ÁùµÀÂÖ»Ø" }		---ÐÂÊÖ½±Àø·É½£  ÁùµÀÂÖ»Ø
	talismans["qingshuang"]				= { talType = "feijian", tmplID = 18914, requireLevel = 60, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "ÇàËª" }				---ÐéÄâ±Ò·É½£    ÇàËª
	talismans["yunsuo"]						= { talType = "feijian", tmplID = 19264, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "ÔÆËó" }				---²ÊÆ±·É½£  ÔÆËó
	talismans["yuruyi"]						= { talType = "feijian", tmplID = 19526, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "ÓñÈçÒâ" }			---²ÊÆ±·É½£  ÓñÈçÒâ
	talismans["biluo"]						= { talType = "feijian", tmplID = 19676, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "±ÌÂä" }				---²ÊÆ±·É½£  ±ÌÂä£¨2008-09-25£©
	talismans["biluo_WZGL"]				= { talType = "feijian", tmplID = 24712, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "±ÌÂä" }				---ÍõÕß¹éÀ´¼ÍÄî°æ  ±ÌÂä£¨2008-05-13£©
	talismans["biluo2"]						= { talType = "feijian", tmplID = 24948, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "±ÌÂä" }				---´óÀñ°ü    ±ÌÂä£¨2008-05-13£©
	talismans["biluo_HL"]					= { talType = "feijian", tmplID = 31817, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = false, name = "±ÌÂä" }				---ºèÀûÉÌ³Ç  ±ÌÂä£¨2009-09-08£©
	talismans["hongfei"]					= { talType = "feijian", tmplID = 20282, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "ºè·É" }				---²ÊÆ±·É½£  ºè·É£¨2008-10-09£©
	talismans["hongfei_HL"]				= { talType = "feijian", tmplID = 31816, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = false, name = "ºè·É" }				---ºèÀûÉÌ³Ç  ºè·É£¨2009-09-08£©
	talismans["zimang"]						= { talType = "feijian", tmplID = 20490, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "×ÏÃ¢" }				---²ÊÆ±·É½£  ×ÏÃ¢£¨2008-10-23£©
	talismans["huiji"]						= { talType = "feijian", tmplID = 21209, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "î¸ÍØ" }				---²ÊÆ±·É½£  î¸ÍØ£¨2008-11-14£©
	talismans["huiji_HL"]					= { talType = "feijian", tmplID = 31818, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = false, name = "î¸ÍØ" }				---ºèÀûÉÌ³Ç  î¸ÍØ£¨2009-09-08£©
	talismans["dizi"]							= { talType = "feijian", tmplID = 23352, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "µÑ×Ó" }				---²ÊÆ±·É½£  ¿ÕÈð£¨2008-11-14£©
	talismans["dizi_WZGL"]				= { talType = "feijian", tmplID = 24713, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "µÑ×Ó" }				---ÍõÕß¹éÀ´¼ÍÄî°æ  ¿ÕÈð£¨2008-05-13£©
	talismans["hunyuanhulu"]			= { talType = "feijian", tmplID = 25741, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "»ìÔªºùÂ«" }		---²ÊÆ±·É½£  »ìÔªºùÂ«£¨2009-07-21£©
	talismans["hunyuanhulu_LB"]		= { talType = "feijian", tmplID = 42550, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "»ìÔªºùÂ«" }		---Àñ°ü·É½£  »ìÔªºùÂ«£¨2010-12-21£©
	talismans["zhanlong"]					= { talType = "feijian", tmplID = 32170, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = false, name = "Õ¶Áú" }				---ÉÌ³Ç·É½£  Õ¶Áú£¨2009-10-13£©
	talismans["zhanlong_RYZL"]		= { talType = "feijian", tmplID = 44640, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = false, name = "Õ¶Áú" }				---ÈÙÒ«Ö®Â·  Õ¶Áú£¨2011-06-03£©
	talismans["vip201001"]				= { talType = "feijian", tmplID = 33293, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = false, name = "VIP201001" }	---VIP·É½£  £¨2009-12-29£©
	talismans["vipfeiyu"]					= { talType = "feijian", tmplID = 34286, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "VIP·ÉÓã" }		---VIP·ÉÓã£¨2010-03-22£© fromLottery=true ±£Ö¤¸ßÆ·ÖÊ
	talismans["feiling"]					= { talType = "feijian", tmplID = 35327, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "·Éç±" }			  ---²ÊÆ±·É½£  ·Éç±£¨2010-05-04£©
	talismans["yuruyi_PM"]				= { talType = "feijian", tmplID = 35458, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "ÓñÈçÒâ" }			---ÅÄÂô·É½£  ÓñÈçÒâ(ÅÄÂôÆ·)
	talismans["longxiang_PM"]			= { talType = "feijian", tmplID = 35460, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "Áúæø" }				---ÅÄÂô·É½£  Áúæø(ÅÄÂôÆ·)
	talismans["zimang_PM"]				= { talType = "feijian", tmplID = 35462, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "×ÏÃ¢" }				---ÅÄÂô·É½£  ×ÏÃ¢(ÅÄÂôÆ·)
--talismans["hongfei_XS"]				= { talType = "feijian", tmplID = 40100, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "ºè·É" }				---ÐÂÊÖÔÚÏß  ºè·É£¨2010-06-11£©
--talismans["dizi_XS"]					= { talType = "feijian", tmplID = 40101, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "µÑ×Ó" }				---ÐÂÊÖÔÚÏß  ¿ÕÈð£¨2010-06-11£©
	talismans["hongfei_XS_new"]		= { talType = "feijian", tmplID = 40466, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "ºè·É" }				---ÐÂÊÖÔÚÏß  ºè·É£¨2010-06-11£©
	talismans["dizi_XS_new"]			= { talType = "feijian", tmplID = 40465, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "µÑ×Ó" }				---ÐÂÊÖÔÚÏß  ¿ÕÈð£¨2010-06-11£©
	talismans["pizza"]						= { talType = "feijian", tmplID = 41115, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "±ÈÈø" }				---º£Íâ·É½£  ±ÈÈø£¨2010-8-25£©
	talismans["fuyun"]						= { talType = "feijian", tmplID = 41947, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "¸¡ÔÆ" }				---²ÊÆ±·É½£  ¸¡ÔÆ£¨2010-10-24£©
	talismans["yuruyi_XS"]				= { talType = "feijian", tmplID = 44158, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "ÓñÈçÒâ" }				---ÐÂÊÖÔÚÏß  ÓñÈçÒâ£¨2011-04-27£©
	talismans["huiji_XS"]					= { talType = "feijian", tmplID = 44160, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "î¸ÍØ" }				---ÐÂÊÖÔÚÏß  î¸ÍØ£¨2011-04-27£©
	talismans["longxiang_XS"]			= { talType = "feijian", tmplID = 44161, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "Áúæø" }				---ÐÂÊÖÔÚÏß  Áúæø£¨2011-04-27£©
	talismans["suoying"]					= { talType = "feijian", tmplID = 52344, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "ËóÓ°" }				---²ÊÆ±·É½£  ËóÓ°£¨2012-04-26£©
	talismans["dizi_LB"]					= { talType = "feijian", tmplID = 52381, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "µÑ×Ó" }				---5ÖÜÄêÀñ°ü  ¿ÕÈð£¨2012-05-04£©
	talismans["dacheng"]					= { talType = "feijian", tmplID = 53709, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "´ó³Ë" }				---2012Äê9ÔÂ	´ó³Ë£¨2012-08-30£©
	talismans["lianhua"]					= { talType = "feijian", tmplID = 53711, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "Á«»¨" }				---2012Äê9ÔÂ	Á«»¨£¨2012-08-30£©
	talismans["qingshuang_LB"]		= { talType = "feijian", tmplID = 54057, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "ÇàËª" }				---2012Äê9ÔÂ	¼¼¼ÎÇàËª£¨2012-09-3£©
	talismans["liangyuanhuoyun"]	= { talType = "feijian", tmplID = 55184, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "Á«»¨" }				---2012Äê12ÔÂ	Á¼Ôµ»ðÔÆ£¨2012-11-20£©
	talismans["vipfeijian"]				= { talType = "feijian", tmplID = 55440, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "vip·É½£" }				---2013Äê1ÔÂ	vip·É½££¨2012-12-25£©
	talismans["caiyuan"]					= { talType = "feijian", tmplID = 58364, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "²Êð°" }				---2013Äê6ÔÂ	²Êð°£¨2013-06-26£©
	talismans["feiling2"]					= { talType = "feijian", tmplID = 61060, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "·Éôá" }				---2013Äê9ÔÂ	²Êð°£¨2013-06-26£©
	talismans["caiyi2"]						= { talType = "feijian", tmplID = 61062, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "ÂÞÒÂ" }		---²ÊÆ±ÓùÊÞ  ÂÞÒÂ2013-09

--	talismans["shihunfeijian"]					= { talType = "feijian", tmplID = 58559, requireLevel = 45, price = 1, school = 0, skillAddon = 34636, fromLottery = true,  timeLimit = false, name = "ÊÉ»ê" }				---2013Äê9ÔÂ	
	talismans["shihunfeijian"]					= { talType = "feijian", tmplID = 58559, requireLevel = 45, price = 1, school = 0, skillAddon = 34636, fromLottery = true,  timeLimit = false, name = "ÊÉ»ê" }				---2013Äê9ÔÂ	
	talismans["tianyafeijian"]					= { talType = "feijian", tmplID = 58558, requireLevel = 45, price = 1, school = 0, skillAddon = 34636, fromLottery = true,  timeLimit = false, name = "Ììçð" }				---2013Äê9ÔÂ	
	talismans["shehunfeijian"]					= { talType = "feijian", tmplID = 58561, requireLevel = 45, price = 1, school = 0, skillAddon = 34636, fromLottery = true,  timeLimit = false, name = "Éã»ê" }				---2013Äê9ÔÂ	
	talismans["sihxuefeijian"]					= { talType = "feijian", tmplID = 58562, requireLevel = 45, price = 1, school = 0, skillAddon = 34636, fromLottery = true,  timeLimit = false, name = "ÊÉÑª" }				---2013Äê9ÔÂ	
	talismans["shenmufeijian"]					= { talType = "feijian", tmplID = 58563, requireLevel = 45, price = 1, school = 0, skillAddon = 34636, fromLottery = true,  timeLimit = false, name = "ÉñÄ¾" }				---2013Äê9ÔÂ	

	-- ÓùÊÞ
	talismans["baidiao"]					= { talType = "yushou", tmplID = 22120, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "°×µñ" }				---²ÊÆ±ÓùÊÞ  Õ½öÀ£¨2008-12-17£©
	talismans["tiane"]						= { talType = "yushou", tmplID = 23124, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "Ìì¶ì" }				---²ÊÆ±ÓùÊÞ  ºèðÀ£¨2009-01-19£©
	talismans["fenghuang"]				= { talType = "yushou", tmplID = 23217, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "·ï»Ë" }				---²ÊÆ±ÓùÊÞ  »ð·ï£¨2009-03-13£©
	talismans["fenghuang_HL"]			= { talType = "yushou", tmplID = 25120, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = false, name = "·ï»Ë" }				---ºèÀûÉÌ³Ç  »ð·ï£¨2009-05-19£©
	talismans["maotouying"]				= { talType = "yushou", tmplID = 25337, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "Ã¨Í·Ó¥" }			---²ÊÆ±ÓùÊÞ  Ã¨Í·Ó¥£¨2009-07-21£©
	talismans["qingyifuwang"]			= { talType = "yushou", tmplID = 32014, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "ÇàÒíòðÍõ" }		---²ÊÆ±ÓùÊÞ  ÇàÒíòðÍõ£¨2009-09-20£©
	talismans["jinpeng"]					= { talType = "yushou", tmplID = 32711, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "½ðÅô" }				---²ÊÆ±ÓùÊÞ  ½ðÅô£¨2009-11-19£©
	talismans["meiying"]					= { talType = "yushou", tmplID = 34285, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "÷ÈÓ°" }				---²ÊÆ±ÓùÊÞ  ÷ÈÓ°£¨2010-03-22£©
	talismans["jinpeng_PM"]				= { talType = "yushou", tmplID = 35459, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "½ðÅô" }				---ÅÄÂôÓùÊÞ  ½ðÅô(ÅÄÂôÆ·)
	talismans["fenghuang_PM"]			= { talType = "yushou", tmplID = 35461, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "·ï»Ë" }				---ÅÄÂôÓùÊÞ  »ð·ï(ÅÄÂôÆ·)
--talismans["qingyifuwang_LB"]	= { talType = "yushou", tmplID = 39922, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "ÇàÒíòðÍõ" }		---ÐÂÊÖÀñ°ü  ÇàÒíòðÍõ£¨2010-06-11£©
--talismans["fenghuang_LB"]			= { talType = "yushou", tmplID = 39923, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "·ï»Ë" }				---ÐÂÊÖÀñ°ü  »ð·ï£¨2010-06-11£©
--talismans["tiane_XS"]					= { talType = "yushou", tmplID = 34070, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "Ìì¶ì" }				---ÐÂÊÖÔÚÏß  ºèðÀ£¨2010-06-11£©
--talismans["baidiao_XS"]				= { talType = "yushou", tmplID = 34071, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "°×µñ" }				---ÐÂÊÖÔÚÏß  Õ½öÀ£¨2010-06-11£©
	talismans["qingyifuwang_LB_new"]= { talType = "yushou", tmplID = 40467, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "ÇàÒíòðÍõ" }		---ÐÂÊÖÀñ°ü  ÇàÒíòðÍõ£¨2010-06-11£©
	talismans["fenghuang_LB_new"]	= { talType = "yushou", tmplID = 40468, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "·ï»Ë" }				---ÐÂÊÖÀñ°ü  »ð·ï£¨2010-06-11£©
	talismans["tiane_XS_new"]			= { talType = "yushou", tmplID = 40469, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "Ìì¶ì" }				---ÐÂÊÖÔÚÏß  ºèðÀ£¨2010-06-11£©
	talismans["baidiao_XS_new"]		= { talType = "yushou", tmplID = 40470, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "°×µñ" }				---ÐÂÊÖÔÚÏß  Õ½öÀ£¨2010-06-11£©
	talismans["qingyifuwang_XS"]	= { talType = "yushou", tmplID = 41791, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "ÇàÒíòðÍõ" }		---ÐÂÊÖ¿¨  ÇàÒíòðÍõ£¨2010-10-22£©
	talismans["fenghuang_XS"]			= { talType = "yushou", tmplID = 41792, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "·ï»Ë" }				---ÐÂÊÖ¿¨  ·ï»Ë£¨2010-10-22£©
	talismans["zhanxiao_XS"]			= { talType = "yushou", tmplID = 41793, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "°×µñ" }				---ÐÂÊÖ¿¨  °×µñ£¨2010-10-22£©
	talismans["tiane1_XS"]				= { talType = "yushou", tmplID = 41794, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true, name = "Ìì¶ì" }				---ÐÂÊÖ¿¨  Ìì¶ì£¨2010-10-22£©
	talismans["feimuji"]					= { talType = "yushou", tmplID = 42600, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "·ÊÄ¸¼¦" }				---²ÊÆ±ÓùÊÞ  ·ÊÄ¸¼¦£¨2010-12-23£©
	talismans["feimuji_BS"]				= { talType = "yushou", tmplID = 42663, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "·ÊÄ¸¼¦" }				---±¦Ê¯ÓùÊÞ  ·ÊÄ¸¼¦£¨2010-12-28£©
	talismans["meiying2"]					= { talType = "yushou", tmplID = 44787, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "÷ÈÓ°2" }				---²ÊÆ±ÓùÊÞ  ÷ÈÓ°2£¨2011-06-30£©
	talismans["dujiaoxian"]				= { talType = "yushou", tmplID = 49311, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "¶À½ÇÏÉ" }				---¿ç·þ½±ÀøÓùÊÞ  ¶À½ÇÏÉ£¨2011-08-07£©
	talismans["yunjing"]					= { talType = "yushou", tmplID = 52455, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "ÔÆ¾¨" }					---²ÊÆ±ÓùÊÞ  ÔÆ¾¨£¨2012-06-13£©
	talismans["dieying"]					= { talType = "yushou", tmplID = 52678, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "µûÓ°" }					---²ÊÆ±ÓùÊÞ  µûÓ°£¨2012-08-02£©
	talismans["heisha"]						= { talType = "yushou", tmplID = 54056, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "ºÚÉ·" }					---²ÊÆ±ÓùÊÞ  ºÚÉ·£¨2012-09-3£©
	talismans["dachong"]					= { talType = "yushou", tmplID = 54865, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "¹íÃæ´ó¶µ³æ" }		---²ÊÆ±ÓùÊÞ  ¹íÃæ´ó¶µ³æ2012-10-10
	talismans["haima"]						= { talType = "yushou", tmplID = 57431, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "º£Âí" }		---²ÊÆ±ÓùÊÞ  º£Âí2013-04-19
	talismans["luoyi"]						= { talType = "yushou", tmplID = 60297, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "ÂÞÒÂ" }		---²ÊÆ±ÓùÊÞ  ÂÞÒÂ2013-08-26
	talismans["huiji2"]						= { talType = "yushou", tmplID = 61064, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "ÂÞÒÂ" }		---²ÊÆ±ÓùÊÞ  ÂÞÒÂ2013-09
	talismans["dieying2"]					= { talType = "yushou", tmplID = 61067, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "ÂÞÒÂ" }		---²ÊÆ±ÓùÊÞ  ÂÞÒÂ2013-09
	talismans["qingsan"]					= { talType = "yushou", tmplID = 62461, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "ÇàÉ¡" }					---²ÊÆ±ÓùÊÞ  ÇàÉ¡£¨2013-10-31£©
	talismans["jiuxian"]					= { talType = "yushou", tmplID = 60272, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "¾ÆÏÉ" }					---²ÊÆ±ÓùÊÞ  ¾ÆÏÉ£¨2013-11-11£©
	talismans["jinxiao"]					= { talType = "yushou", tmplID = 60276, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "½ðöÀ" }					---²ÊÆ±ÓðÒí  ½ðöÀ(2013-11-12)
	talismans["caixue"]						= { talType = "yushou", tmplID = 62780, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "²ÃÑ©" }					---²ÊÆ±ÓðÒí  ²ÃÑ©(2013-11-29)

	-- ÓðÒí
	talismans["yuhuang"]					= { talType = "chibang",tmplID = 34041, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "Óð»Ê" }				---²ÊÆ±ÓðÒí  Óð»Ê(2010-03-03)
	talismans["yuhuang_PM"]				= { talType = "chibang",tmplID = 40505, requireLevel =  1, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "Óð»Ê" }				---ÅÄÂôÓðÒí  Óð»Ê(ÅÄÂôÆ·)(2010-06-21)
	talismans["yuhuang_SD"]				= { talType = "chibang",tmplID = 42647, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "Óð»Ê" }				---Ê¥µ®ÌØ¹©  Óð»Ê(2010-12-27)
	talismans["yuhuang_XS"]				= { talType = "chibang",tmplID = 51509, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "Óð»Ê" }				---ÐÂÊÖÌØ¹©  Óð»Ê(2012-04-20)
	talismans["caiyi"]						= { talType = "chibang",tmplID = 35164, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "²ÊÒí" }				---²ÊÆ±ÓðÒí  ²ÊÒí(2010-04-22)
	talismans["caiyi_SD"]					= { talType = "chibang",tmplID = 42648, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "²ÊÒí" }				---Ê¥µ®ÌØ¹©  ²ÊÒí(2010-12-27)
	talismans["tianshi"]					= { talType = "chibang",tmplID = 40962, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "ÌìÊ¹" }				---²ÊÆ±ÓðÒí  ÌìÊ¹(2010-08-11)
	talismans["tianshi_SD"]				= { talType = "chibang",tmplID = 42649, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "ÌìÊ¹" }				---Ê¥µ®ÌØ¹©  ÌìÊ¹(2010-12-27)
	talismans["fenghuangchibang"]	= { talType = "chibang",tmplID = 42150, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "·ï»Ë" }				---²ÊÆ±ÓðÒí  ·ï»Ë(2010-11-03)
	talismans["emochibang"]				= { talType = "chibang",tmplID = 43591, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "¶ñÄ§" }				---²ÊÆ±ÓðÒí  ¶ñÄ§(2011-04-14)
	talismans["emochibang_XS"]		= { talType = "chibang",tmplID = 50848, requireLevel = 1 , price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = false, name = "¶ñÄ§" }				---»î¶¯ÓðÒí  ¶ñÄ§(2011-10-18)
	talismans["caiyi_XS"]					= { talType = "chibang",tmplID = 44159, requireLevel = 1 , price = 1, school = 0, skillAddon = 0, fromLottery = false, timeLimit = true , name = "²ÊÒí" }				---²ÊÆ±ÓðÒí  ²ÊÒí(2011-04-27)
	talismans["heiyi"]						= { talType = "chibang",tmplID = 44759, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "¶ñÄ§" }				---²ÊÆ±ÓðÒí  ºÚÒí(2011-06-27)
	talismans["heiyi_HD"]					= { talType = "chibang",tmplID = 58390, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "¶ñÄ§" }				---²ÊÆ±ÓðÒí  Ä«Îè(2013-07-03)
	talismans["mingwang"]					= { talType = "chibang",tmplID = 51198, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "Ú¤Íõ" }				---²ÊÆ±ÓðÒí  Ú¤Íõ(2012-02-02)
	talismans["shengling"]				= { talType = "chibang",tmplID = 51264, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "Ê¥Áé" }				---²ÊÆ±ÓðÒí  Ê¥Áé(2012-02-20)
	talismans["zhiyuan"]					= { talType = "chibang",tmplID = 54006, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "Ö½ð°" }				---²ÊÆ±ÓðÒí  Ö½ð°(2012-09-3)
	talismans["guangming"]				= { talType = "chibang",tmplID = 55358, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "¹âÃ÷" }				---²ÊÆ±ÓðÒí  ¹âÃ÷(2012-12-17)
	talismans["shengling_XS"]			= { talType = "chibang",tmplID = 55665, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "Ê¥Áé" }				---²ÊÆ±ÓðÒí  Ê¥Áé(»î¶¯)(2013-03-01)	
	talismans["mingyin"]					= { talType = "chibang",tmplID = 55825, requireLevel = 45, price = 1, school = 0, skillAddon = 0, fromLottery = true,  timeLimit = false, name = "Ú¤Òý" }				---²ÊÆ±ÓðÒí  Ú¤Òý(2013-03-06)	

	talismans["6v6chibang01"]					= { talType = "chibang", tmplID = 60480, requireLevel = 45, price = 1, school = 0, skillAddon = 34636, fromLottery = true,  timeLimit = false, name = "³á°ò" }				---2013Äê6v6	
	talismans["6v6chibang02"]					= { talType = "chibang", tmplID = 60481, requireLevel = 45, price = 1, school = 0, skillAddon = 34636, fromLottery = true,  timeLimit = false, name = "³á°ò" }				---2013Äê6v6	
	talismans["6v6chibang03"]					= { talType = "chibang", tmplID = 60482, requireLevel = 45, price = 1, school = 0, skillAddon = 34636, fromLottery = true,  timeLimit = false, name = "³á°ò" }				---2013Äê6v6	

	return talismans
end

---·¨±¦Ã¿¼¶¾«Á¦Öµ³É³¤Öµ
function ZLuaTalStaminaGrowth()
	---staminaGroth[5] = 82µÄº¬ÒåÊÇ£¬·¨±¦Éýµ½µÚ5¼¶Ê±£¬ÆäÐÂµÄ¾«Á¦ÖµÉÏÏÞÎª defaultStamina + 82 * quo
	---ÄâºÏ¹«Ê½£º0.0884 * Lv * Lv + 20.026 * Lv - 20.233
	---Êý¾ÝÐÞ¸ÄÇø ¿ªÊ¼
	local staminaGrowth = {
		  0,  20,  41,  61,  82, 103, 124, 146, 167, 189,
		211, 233, 255, 277, 300, 323, 346, 369, 392, 416,
		439, 463, 487, 511, 536, 560, 585, 610, 635, 660,
		686, 711, 737, 763, 789, 815, 842, 868, 895, 922,
		949, 977,1004,1032,1060,1088,1116,1145,1173,1202}
	---Êý¾ÝÐÞ¸ÄÇø ½áÊø
	return staminaGrowth
end

---·¨±¦µµÎ»º¯Êý¡£·µ»Ø·¨±¦ÊôÐÔµµÎ»Êý¡¢¸÷µµÊýÖµ
function ZLuaTalGrades()

	---È¡µÃÒ»Ð©³£Á¿
	local dataIndex,resisIndex = ZLuaTalDataIndex()

	---Êý¾ÝÐÞ¸ÄÇø ¿ªÊ¼
	
	---Ã¿¸öÊôÐÔ·Ö¼¸µµ£¬Õâ½ö½öÊÇ³õÊ¼»¯Ê±£¬ÔÚÕâ¸ö·¶Î§ÄÚËæ»ú£¬¶øÑªÁ¶ºó¿ÉÄÜ»áÍ»ÆÆÕâ¸öµµÎ»Êý
	local nGrades
	nGrades = 50  ---must >= 3£¬Weights[]µÄ³¤¶È±ØÐë £½ nGrades

	---¸÷ÀàÊôÐÔµÄÉÏÏÂÏÞ
	local valueBounds = {}
	valueBounds[dataIndex["default_stamina"]]				=	{ 220, 260 }
	valueBounds[dataIndex["quo_stamina"]] 					=	{ 0.75, 1.2 }

	valueBounds[dataIndex["default_hp"]] 						= { 0, 50 }
	valueBounds[dataIndex["default_mp"]]            = { 0, 50 }
	valueBounds[dataIndex["default_attack_rate"]]   = { 0, 0 }
	valueBounds[dataIndex["default_damage_low"]]    = { 0, 10 }
	valueBounds[dataIndex["default_damage_high"]]   = { 0, 30 }
	valueBounds[dataIndex["default_dodge"]]         = { 0, 0 }
	valueBounds[dataIndex["default_defense"]]       = { 0, 0 }  ---±ØÐëÊÇ0
	valueBounds[dataIndex["default_resis_stun"]]     = { 0, 10 }
	valueBounds[dataIndex["default_resis_sleep"]]    = { 0, 10 }
	valueBounds[dataIndex["default_resis_silence"]]  = { 0, 10 }
	valueBounds[dataIndex["default_resis_weakness"]] = { 0, 10 }
	valueBounds[dataIndex["default_resis_enlace"]]   = { 0, 10 }

	valueBounds[dataIndex["grow_hp"]]               = { 1, 100.18 }
	valueBounds[dataIndex["grow_mp"]]               = { 1, 69.44 }
	valueBounds[dataIndex["grow_attack_rate"]]      = { 0, 0 }
	valueBounds[dataIndex["grow_damage_low"]]       = { 0.5, 15.51 / 2 }
	valueBounds[dataIndex["grow_damage_high"]]      = { 1, 15.51 * 1.5 }
	valueBounds[dataIndex["grow_dodge"]]            = { 0, 0 }
	valueBounds[dataIndex["grow_defense"]]          = { 0, 0 } ---±ØÐëÊÇ0
	valueBounds[dataIndex["grow_resis_stun"]]        = { 0.5, 4.76 }
	valueBounds[dataIndex["grow_resis_sleep"]]       = { 0.5, 4.76 }
	valueBounds[dataIndex["grow_resis_silence"]]     = { 0.5, 4.76 }
	valueBounds[dataIndex["grow_resis_weakness"]]    = { 0.5, 4.76 }
	valueBounds[dataIndex["grow_resis_enlace"]]      = { 0.5, 4.76 }

	---Êý¾ÝÐÞ¸ÄÇø ½áÊø

	---¸÷ÀàÊôÐÔµÄµµÎ»
	local valueGrades = {}
	valueGrades[dataIndex["default_stamina"]] = {}
	valueGrades[dataIndex["quo_stamina"]] = {}
	
	valueGrades[dataIndex["default_hp"]] = {}
	valueGrades[dataIndex["default_mp"]] = {}
	valueGrades[dataIndex["default_attack_rate"]] = {}
	valueGrades[dataIndex["default_damage_low"]] = {}
	valueGrades[dataIndex["default_damage_high"]] = {}
	valueGrades[dataIndex["default_dodge"]] = {}
	valueGrades[dataIndex["default_defense"]] = {}
	valueGrades[dataIndex["default_resis_stun"]] = {}
	valueGrades[dataIndex["default_resis_sleep"]] = {}
	valueGrades[dataIndex["default_resis_silence"]] = {}
	valueGrades[dataIndex["default_resis_weakness"]] = {}
	valueGrades[dataIndex["default_resis_enlace"]] = {}
	
	
	valueGrades[dataIndex["grow_hp"]] = {}
	valueGrades[dataIndex["grow_mp"]] = {}
	valueGrades[dataIndex["grow_attack_rate"]] = {}
	valueGrades[dataIndex["grow_damage_low"]] = {}
	valueGrades[dataIndex["grow_damage_high"]] = {}
	valueGrades[dataIndex["grow_dodge"]] = {}
	valueGrades[dataIndex["grow_defense"]] = {}
	valueGrades[dataIndex["grow_resis_stun"]] = {}
	valueGrades[dataIndex["grow_resis_sleep"]] = {}
	valueGrades[dataIndex["grow_resis_silence"]] = {}
	valueGrades[dataIndex["grow_resis_weakness"]] = {}
	valueGrades[dataIndex["grow_resis_enlace"]] = {}
		
	local i
	for i = 1, nGrades do
		valueGrades[dataIndex["default_stamina"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["default_stamina"]][1], valueBounds[dataIndex["default_stamina"]][2], i, nGrades )
		valueGrades[dataIndex["quo_stamina"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["quo_stamina"]][1], valueBounds[dataIndex["quo_stamina"]][2], i, nGrades )
		
		valueGrades[dataIndex["default_hp"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["default_hp"]][1], valueBounds[dataIndex["default_hp"]][2], i, nGrades )
		valueGrades[dataIndex["default_mp"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["default_mp"]][1], valueBounds[dataIndex["default_mp"]][2], i, nGrades )
		valueGrades[dataIndex["default_attack_rate"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["default_attack_rate"]][1], valueBounds[dataIndex["default_attack_rate"]][2], i, nGrades )
		valueGrades[dataIndex["default_damage_low"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["default_damage_low"]][1], valueBounds[dataIndex["default_damage_low"]][2], i, nGrades )
		valueGrades[dataIndex["default_damage_high"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["default_damage_high"]][1], valueBounds[dataIndex["default_damage_high"]][2], i, nGrades )
		valueGrades[dataIndex["default_dodge"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["default_dodge"]][1], valueBounds[dataIndex["default_dodge"]][2], i, nGrades )
		valueGrades[dataIndex["default_defense"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["default_defense"]][1], valueBounds[dataIndex["default_defense"]][2], i, nGrades )
		valueGrades[dataIndex["default_resis_stun"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["default_resis_stun"]][1], valueBounds[dataIndex["default_resis_stun"]][2], i, nGrades )
		valueGrades[dataIndex["default_resis_sleep"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["default_resis_sleep"]][1], valueBounds[dataIndex["default_resis_sleep"]][2], i, nGrades )
		valueGrades[dataIndex["default_resis_silence"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["default_resis_silence"]][1], valueBounds[dataIndex["default_resis_silence"]][2], i, nGrades )
		valueGrades[dataIndex["default_resis_weakness"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["default_resis_weakness"]][1], valueBounds[dataIndex["default_resis_weakness"]][2], i, nGrades )
		valueGrades[dataIndex["default_resis_enlace"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["default_resis_enlace"]][1], valueBounds[dataIndex["default_resis_enlace"]][2], i, nGrades )
		
		valueGrades[dataIndex["grow_hp"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["grow_hp"]][1], valueBounds[dataIndex["grow_hp"]][2], i, nGrades )
		valueGrades[dataIndex["grow_mp"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["grow_mp"]][1], valueBounds[dataIndex["grow_mp"]][2], i, nGrades )
		valueGrades[dataIndex["grow_attack_rate"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["grow_attack_rate"]][1], valueBounds[dataIndex["grow_attack_rate"]][2], i, nGrades )
		valueGrades[dataIndex["grow_damage_low"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["grow_damage_low"]][1], valueBounds[dataIndex["grow_damage_low"]][2], i, nGrades )
		valueGrades[dataIndex["grow_damage_high"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["grow_damage_high"]][1], valueBounds[dataIndex["grow_damage_high"]][2], i, nGrades )
		valueGrades[dataIndex["grow_dodge"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["grow_dodge"]][1], valueBounds[dataIndex["grow_dodge"]][2], i, nGrades )
		valueGrades[dataIndex["grow_defense"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["grow_defense"]][1], valueBounds[dataIndex["grow_defense"]][2], i, nGrades )
		valueGrades[dataIndex["grow_resis_stun"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["grow_resis_stun"]][1], valueBounds[dataIndex["grow_resis_stun"]][2], i, nGrades )
		valueGrades[dataIndex["grow_resis_sleep"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["grow_resis_sleep"]][1], valueBounds[dataIndex["grow_resis_sleep"]][2], i, nGrades )
		valueGrades[dataIndex["grow_resis_silence"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["grow_resis_silence"]][1], valueBounds[dataIndex["grow_resis_silence"]][2], i, nGrades )
		valueGrades[dataIndex["grow_resis_weakness"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["grow_resis_weakness"]][1], valueBounds[dataIndex["grow_resis_weakness"]][2], i, nGrades )
		valueGrades[dataIndex["grow_resis_enlace"]][i] = ZLuaTalGetValueWithBounds( valueBounds[dataIndex["grow_resis_enlace"]][1], valueBounds[dataIndex["grow_resis_enlace"]][2], i, nGrades )
	end
	
	return nGrades, valueGrades
end

---Ã¿¸ö·¨±¦µÄ ×ÜµµÎ»ÉÏÏÞ ÒÔ¼° Ã¿¸öÊôÐÔµÄµµÎ»ÉÏÏÞ
function ZLuaTalGradesUpperLimit(tmplID)
	if ZLuaTalExistInTmplIDs(tmplID) == 0 then return nil end
	local dataIndex,resisIndex = ZLuaTalDataIndex()
	local talismans = ZLuaTalAvailableTalismans()
	
	---¸ñÊ½{×ÜµµÎ»£¬×îÐ¡¹¥»÷£¬×î´ó¹¥»÷£¬HP£¬MP£¬ÃüÖÐ£¬¶ãÉÁ£¬Ñ£ÔÎ£¬ÐéÈõ£¬¶¨Éí£¬÷È»ó£¬»èË¯£¬±ù¶³?}
	local gradesUpperLimit

	---°ËØÔÊ¯ ÎåÉ«Ê¯
	if tmplID == talismans["NW_1_BaGuaShi"].tmplID or tmplID == talismans["SY_1_WuSeShi"].tmplID or tmplID == talismans["LZ_1_TianLingShi"].tmplID then
		gradesUpperLimit = { 82, 40, 40,	40,	40,	1,	1,	40,	40,	40,	40,	40,	1 }
	---ÖìÈ¸Ó¡	Å­ÊÞá¦
	elseif tmplID == talismans["GW_15_ZhuQueYin"].tmplID or tmplID == talismans["JL_15_NuShouFan"].tmplID or tmplID == talismans["QJ_15_tianjimu"].tmplID then
		gradesUpperLimit = { 148, 50, 50,	50,	50,	1,	1,	45,	45,	45,	45,	45,	1 }
	---ÉåÐÄÁå ÆßÉ±
	elseif tmplID == talismans["HH_15_SheXinLing"].tmplID or tmplID == talismans["HG_15_QiSha"].tmplID or tmplID == talismans["YZ_15_lveying"].tmplID then
		gradesUpperLimit = { 150, 50, 50,	50,	50,	1,	1,	45,	45,	45,	45,	45,	1 }
	---ÁùºÏ¾µ ¾ÅÒôÂÝ
	elseif tmplID == talismans["QY_15_LiuHeJing"].tmplID or tmplID == talismans["TH_15_JiuYinLuo"].tmplID then
		gradesUpperLimit = { 150, 50, 50,	50,	50,	1,	1,	45,	45,	45,	45,	45,	1 }
	---ÂÖ»ØÖé	µ¤Öì ÂÊ¶µÆì
	elseif tmplID == talismans["TY_15_LunHuiZhu"].tmplID or tmplID == talismans["LS_15_DanZhu"].tmplID or tmplID == talismans["CH_15_lvdouqi"].tmplID then
		gradesUpperLimit = { 146, 50, 50,	50,	50,	1,	1,	45,	45,	45,	45,	45,	1 }
	---ÑªÓñ¹ÇÆ¬ ÄÏÃ÷Ó¡ À×¹«Ä«
	elseif tmplID == talismans["GD_15_XueYuGuPian"].tmplID or tmplID == talismans["FX_15_NanMingYin"].tmplID or tmplID == talismans["TH_15_LeiGongMo"].tmplID then
		gradesUpperLimit = { 150, 50, 50,	50,	50,	1,	1,	45,	45,	45,	45,	45,	1 }
	---ËÄÁéá¦ Ñª·ãÇ¹
	elseif tmplID == talismans["GW_75_SiLingFan"].tmplID or tmplID == talismans["JL_75_XueFengQiang"].tmplID or tmplID == talismans["QJ_75_luohouyu"].tmplID then
		gradesUpperLimit = { 219, 55, 55,	55,	55,	1,	1,	50,	50,	50,	50,	50,	1 }
	---¶àÇé»· ÆÆ¾ü
	elseif tmplID == talismans["HH_75_DuoQingHuan"].tmplID or tmplID == talismans["HG_75_PoJun"].tmplID or tmplID == talismans["YZ_75_poming"].tmplID then
		gradesUpperLimit = { 225, 55, 55,	55,	55,	1,	1,	50,	50,	50,	50,	50,	1 }
	---ÆßÐÇ½£ ÑÇ·ç
	elseif tmplID == talismans["QY_75_QiXingJian"].tmplID or tmplID == talismans["TH_75_YaFeng"].tmplID then
		gradesUpperLimit = { 224, 55, 55,	55,	55,	1,	1,	50,	50,	50,	50,	50,	1 }
	---½ð¸ÕèÆ ·µ»êÏã ö¾¹Ä
	elseif tmplID == talismans["TY_75_JinGangChu"].tmplID or tmplID == talismans["LS_75_FanHunXiang"].tmplID or tmplID == talismans["CH_75_tuogu"].tmplID then
		gradesUpperLimit = { 219, 55, 55,	55,	55,	1,	1,	50,	50,	50,	50,	50,	1 }
	---ÆÆÉ² ºâÌì³ß ´ºÇïËø
	elseif tmplID == talismans["GD_75_PoSha"].tmplID or tmplID == talismans["FX_75_HengTianChi"].tmplID or tmplID == talismans["TH_75_ChunQiuSuo"].tmplID then
		gradesUpperLimit = { 225, 55, 55,	55,	55,	1,	1,	50,	50,	50,	50,	50,	1 }

	---45¼¶Í¨ÓÃ·¨±¦(ÉñÄ¾÷» ÔªÏüµÆ)
	elseif tmplID == talismans["US_45_ShenMuShai"].tmplID
			or tmplID == talismans["US_45_ShenMuShai_WZGL"].tmplID
			or tmplID == talismans["US_45_YuanXiaoDeng"].tmplID then
		gradesUpperLimit = { 225, 55, 55,	55,	55,	1,	1,	50,	50,	50,	50,	50,	1 }

	---75¼¶Í¨ÓÃ·¨±¦(É½ºÓÉÈ Íò¶¾¶¦ Óñ¾»Æ¿ ÁáççËþ)
	elseif tmplID == talismans["US_75_ShanHeShan"].tmplID 
			or tmplID == talismans["US_75_WanDuDing"].tmplID
			or tmplID == talismans["US_75_YuJingPing"].tmplID
			or tmplID == talismans["US_75_LingLongTa"].tmplID then
		gradesUpperLimit = { 250, 55, 55,	55,	55,	1,	1,	50,	50,	50,	50,	50,	1 }

	---105¼¶ÃÅÅÉ·¨±¦
	elseif tmplID == talismans["GW_105_QianKunQingGuangJie"].tmplID
			or tmplID == talismans["HH_105_QianMeiLian"].tmplID
			or tmplID == talismans["QY_105_HuPoZhuLing"].tmplID
			or tmplID == talismans["TY_105_RuYiQianKunDai"].tmplID 
			or tmplID == talismans["GD_105_QianDuGu"].tmplID
			or tmplID == talismans["FX_105_HuoLingLong"].tmplID
			or tmplID == talismans["JL_105_GangLan"].tmplID
			or tmplID == talismans["LS_105_FuYingGuJing"].tmplID
			or tmplID == talismans["HG_105_TanLang"].tmplID
			or tmplID == talismans["TH_105_LiuLiZhan"].tmplID
			or tmplID == talismans["TH_105_ZiMuYinYangKou"].tmplID
			or tmplID == talismans["CH_105_luobaojinqian"].tmplID
			or tmplID == talismans["QJ_105_jinyubian"].tmplID
			or tmplID == talismans["YZ_105_shuolei"].tmplID
		then
		gradesUpperLimit = { 270, 55, 55,	55,	55,	1,	1,	50,	50,	50,	50,	50,	1 }

	---105¼¶Í¨ÓÃ·¨±¦ ºÍ ó´Áúá¦
	elseif tmplID == talismans["US_105_YinYangJing"].tmplID 
			or tmplID == talismans["US_105_YinYangJing_WZGL"].tmplID
			or tmplID == talismans["US_135_PanLongFan"].tmplID
			or tmplID == talismans["US_105_JinHuLu"].tmplID
			or tmplID == talismans["US_105_JiuLongShenHuoZhao"].tmplID then
		gradesUpperLimit = { 300, 150, 55,	55,	55,		1,	1,	50,	50,	50,	50,	50,	1 }

	---ÕæÉ½ºÓÉÈ Í¨ÓÃ
	elseif tmplID == talismans["US_105_ZhenShanHeShan"].tmplID then
		gradesUpperLimit = { 300, 50, 55,	55,	55,		1,	1,	50,	50,	50,	50,	50,	1 }
	
	---Âä»êµÆ Í¨ÓÃ
	elseif tmplID == talismans["US_105_LuoHunDeng"].tmplID then
		gradesUpperLimit = { 300, 60, 60,	60,	60,		1,	1,	50,	50,	50,	50,	50,	1 }
		
	---135¼¶ÃÅÅÉ·¨±¦ ºÍ 135¼¶Í¨ÓÃ·¨±¦£¨³ýÁËó´Áúá¦£©
	elseif tmplID == talismans["GW_135_TianMoZhi"].tmplID
			or tmplID == talismans["QY_135_RuoXue"].tmplID
			or tmplID == talismans["HH_135_BaJiYouLong"].tmplID
			or tmplID == talismans["TY_135_LiuDaoShengMie"].tmplID 
			or tmplID == talismans["GD_135_GuiQing"].tmplID
			or tmplID == talismans["FX_135_JiuHanNingBingCi"].tmplID
			or tmplID == talismans["JL_135_SangMenJian"].tmplID
			or tmplID == talismans["LS_135_LuoShu"].tmplID
			or tmplID == talismans["HG_135_JueYing"].tmplID
			or tmplID == talismans["TH_135_BiLuoFeiHong"].tmplID
			or tmplID == talismans["TH_135_XiHuangZhong"].tmplID
			or tmplID == talismans["CH_135_huangji"].tmplID
			or tmplID == talismans["QJ_135_chixinglongyan"].tmplID
			or tmplID == talismans["YZ_135_zhuihun"].tmplID
			or tmplID == talismans["US_135_WuLongLun"].tmplID
			or tmplID == talismans["US_135_FanTianYin"].tmplID
			or tmplID == talismans["US_135_ChangMingDeng"].tmplID
			or tmplID == talismans["US_135_BaoKuZhiHun"].tmplID
			or tmplID == talismans["US_75_RiYueBaoJian"].tmplID
			or tmplID == talismans["US_105_FuTu"].tmplID
			or tmplID == talismans["US_135_YinHunFan"].tmplID
			then
		gradesUpperLimit = { 300, 60, 60,	60,	60,		1,	1,	50,	50,	50,	50,	50,	1 }
	
	---Éã»ê	ÊÉÑªÖé Ê³Îï·¨±¦
	elseif tmplID == talismans["US_135_SheHun"].tmplID
			or tmplID == talismans["US_145_ShiXueZhu"].tmplID
			or tmplID == talismans["foodtalisman"].tmplID then
		gradesUpperLimit = { 320, 60, 60,	60,	60,	1,	1,	50,	50,	50,	50,	50,	1 }

	-- ÊÉ»ê Ììçð
	elseif tmplID == talismans["US_145_ShiHun"].tmplID 
			or tmplID == talismans["US_145_TianYa"].tmplID then
		gradesUpperLimit = { 360, 65, 65,	65,	65,	1,	1,	55,	55,	55,	55,	55,	1 }
		
	---·É±¦ Í¨ÓÃ
	elseif ZLuaTalGetTalTypeFromID(tmplID) == "feijian"
			or ZLuaTalGetTalTypeFromID(tmplID) == "yushou"
			or ZLuaTalGetTalTypeFromID(tmplID) == "chibang"
			then
		gradesUpperLimit = { 300, 1, 1,	1,	1,	1,	1,	1,	1,	1,	1,	1,	1 }
	end

	return gradesUpperLimit
end

---Ã¿¸ö·¨±¦µÄ Ã¿¸öÊôÐÔµÄ Ã¿¸öµµÎ»µÄ Ëæ»úÈ¨ÖØ
function ZLuaTalRandomWeight(tmplID, origin)
	if ZLuaTalExistInTmplIDs(tmplID) == 0 then return nil end
	---È¡µÃÒ»Ð©³£Á¿
	local dataIndex,resisIndex = ZLuaTalDataIndex()
	local talismanOrigin = ZLuaTalServerOrigins()
	local talismans = ZLuaTalAvailableTalismans()

	---Êý¾ÝÐÞ¸ÄÇø ¿ªÊ¼

	---¿ªÊ¼ÉèÖÃÃ¿¸ö·¨±¦µÄÃ¿¸öÊôÐÔµÄÃ¿¸öµµÎ»µÄÈ¨ÖØ
	local valueWeights = {}
	
	---ÈÎÎñ¸øµÄ°ËØÔÊ¯ ÎåÉ«Ê¯
	if (tmplID == talismans["NW_1_BaGuaShi"].tmplID or tmplID == talismans["SY_1_WuSeShi"].tmplID or tmplID == talismans["LZ_1_TianLingShi"].tmplID) and origin == talismanOrigin["quest"] then

		valueWeights[dataIndex["default_hp"]] ={0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_mp"]] ={0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_attack_rate"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_low"]] ={0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_high"]] ={0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_dodge"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_sleep"]] ={0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_silence"]] ={0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_weakness"]] ={0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_enlace"]] ={0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		
		valueWeights[dataIndex["grow_hp"]] ={0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]] ={0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_attack_rate"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]] ={0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]] ={0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]] ={0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}

	---ÈÎÎñ¸øµÄÖìÈ¸Ó¡ Å­ÊÞá¦
	elseif (tmplID == talismans["GW_15_ZhuQueYin"].tmplID or tmplID == talismans["JL_15_NuShouFan"].tmplID or tmplID == talismans["QJ_15_tianjimu"].tmplID) and origin == talismanOrigin["quest"] then

		valueWeights[dataIndex["default_hp"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_mp"]] ={0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_attack_rate"]] ={0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_dodge"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		
		valueWeights[dataIndex["grow_hp"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]] ={0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_attack_rate"]] ={0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}

	---ÈÎÎñ¸øµÄÉåÐÄÁå ÆßÉ±
	elseif (tmplID == talismans["HH_15_SheXinLing"].tmplID or tmplID == talismans["HG_15_QiSha"].tmplID or tmplID == talismans["YZ_15_lveying"].tmplID) and origin == talismanOrigin["quest"] then
	
		valueWeights[dataIndex["default_hp"]] ={0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_mp"]] ={0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_attack_rate"]] ={0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_dodge"]] ={0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		
		valueWeights[dataIndex["grow_hp"]] ={0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]] ={0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_attack_rate"]] ={0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]] ={0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}

	---ÈÎÎñ¸øµÄÁùºÏ¾µ ¾ÅÒôÂÝ
	elseif (tmplID == talismans["QY_15_LiuHeJing"].tmplID or tmplID == talismans["TH_15_JiuYinLuo"].tmplID) and origin == talismanOrigin["quest"] then

		valueWeights[dataIndex["default_hp"]] ={0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_mp"]] ={0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_attack_rate"]] ={0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_dodge"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		
		valueWeights[dataIndex["grow_hp"]] ={0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]] ={0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_attack_rate"]] ={0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}

	---ÈÎÎñ¸øµÄÂÖ»ØÖé µ¤Öì ÂÊ¶µÆì
	elseif (tmplID == talismans["TY_15_LunHuiZhu"].tmplID or tmplID == talismans["LS_15_DanZhu"].tmplID or tmplID == talismans["CH_15_lvdouqi"].tmplID) and origin == talismanOrigin["quest"] then
	
		valueWeights[dataIndex["default_hp"]] ={0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_mp"]] ={0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_attack_rate"]] ={0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_dodge"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		
		valueWeights[dataIndex["grow_hp"]] ={0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]] ={0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_attack_rate"]] ={0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		
	---ÈÎÎñ¸øµÄÑªÓñ¹ÇÆ¬ ÄÏÃ÷Ó¡ À×¹«Ä«
	elseif (tmplID == talismans["GD_15_XueYuGuPian"].tmplID or tmplID == talismans["FX_15_NanMingYin"].tmplID or tmplID == talismans["TH_15_LeiGongMo"].tmplID) and origin == talismanOrigin["quest"] then
	
		valueWeights[dataIndex["default_hp"]] ={0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_mp"]] ={0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_attack_rate"]] ={0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_dodge"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		
		valueWeights[dataIndex["grow_hp"]] ={0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]] ={0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_attack_rate"]] ={0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}

	---°ËØÔÊ¯ ÎåÉ«Ê¯
	elseif tmplID == talismans["NW_1_BaGuaShi"].tmplID or tmplID == talismans["SY_1_WuSeShi"].tmplID or tmplID == talismans["LZ_1_TianLingShi"].tmplID then
	
		valueWeights[dataIndex["default_hp"]] ={5,15,12.5,25,37.5,2.5,1.66666666666667,.833333333333333,4.65116279069767E-03,4.54042081949059E-03,4.4296788482835E-03,4.31893687707641E-03,4.20819490586932E-03,4.09745293466224E-03,3.98671096345515E-03,3.87596899224806E-03,3.76522702104097E-03,3.65448504983389E-03,3.5437430786268E-03,3.43300110741971E-03,3.32225913621262E-03,3.21151716500554E-03,3.10077519379845E-03,2.99003322259136E-03,2.87929125138427E-03,2.76854928017719E-03,2.6578073089701E-03,2.54706533776301E-03,2.43632336655592E-03,2.32558139534884E-03,2.21483942414175E-03,2.10409745293466E-03,1.99335548172757E-03,1.88261351052049E-03,1.7718715393134E-03,1.66112956810631E-03,1.55038759689922E-03,1.43964562569214E-03,1.32890365448505E-03,1.21816168327796E-03,1.10741971207087E-03,9.96677740863787E-04,8.859357696567E-04,7.75193798449612E-04,6.64451827242524E-04,5.53709856035437E-04,4.42967884828349E-04,3.32225913621262E-04,2.21483942414174E-04,1.10741971207087E-04,}
		valueWeights[dataIndex["default_mp"]] ={5,15,12.5,25,37.5,2.5,1.66666666666667,.833333333333333,4.65116279069767E-03,4.54042081949059E-03,4.4296788482835E-03,4.31893687707641E-03,4.20819490586932E-03,4.09745293466224E-03,3.98671096345515E-03,3.87596899224806E-03,3.76522702104097E-03,3.65448504983389E-03,3.5437430786268E-03,3.43300110741971E-03,3.32225913621262E-03,3.21151716500554E-03,3.10077519379845E-03,2.99003322259136E-03,2.87929125138427E-03,2.76854928017719E-03,2.6578073089701E-03,2.54706533776301E-03,2.43632336655592E-03,2.32558139534884E-03,2.21483942414175E-03,2.10409745293466E-03,1.99335548172757E-03,1.88261351052049E-03,1.7718715393134E-03,1.66112956810631E-03,1.55038759689922E-03,1.43964562569214E-03,1.32890365448505E-03,1.21816168327796E-03,1.10741971207087E-03,9.96677740863787E-04,8.859357696567E-04,7.75193798449612E-04,6.64451827242524E-04,5.53709856035437E-04,4.42967884828349E-04,3.32225913621262E-04,2.21483942414174E-04,1.10741971207087E-04,}
		valueWeights[dataIndex["default_attack_rate"]] ={75,.004,3.91836734693878E-03,3.83673469387755E-03,3.75510204081633E-03,3.6734693877551E-03,3.59183673469388E-03,3.51020408163265E-03,3.42857142857143E-03,3.3469387755102E-03,3.26530612244898E-03,3.18367346938775E-03,3.10204081632653E-03,3.02040816326531E-03,2.93877551020408E-03,2.85714285714286E-03,2.77551020408163E-03,2.69387755102041E-03,2.61224489795918E-03,2.53061224489796E-03,2.44897959183673E-03,2.36734693877551E-03,2.28571428571429E-03,2.20408163265306E-03,2.12244897959184E-03,2.04081632653061E-03,1.95918367346939E-03,1.87755102040816E-03,1.79591836734694E-03,1.71428571428571E-03,1.63265306122449E-03,1.55102040816327E-03,1.46938775510204E-03,1.38775510204082E-03,1.30612244897959E-03,1.22448979591837E-03,1.14285714285714E-03,1.06122448979592E-03,9.79591836734694E-04,8.97959183673469E-04,8.16326530612245E-04,7.3469387755102E-04,6.53061224489796E-04,5.71428571428572E-04,4.89795918367347E-04,4.08163265306123E-04,3.26530612244898E-04,2.44897959183673E-04,1.63265306122449E-04,8.16326530612248E-05,}
		valueWeights[dataIndex["default_damage_low"]] ={2.5,2.5,7.5,7.5,7.5,15,22.5,30,2.5,1.66666666666667,.833333333333333,.005,4.87179487179487E-03,4.74358974358974E-03,4.61538461538462E-03,4.48717948717949E-03,4.35897435897436E-03,4.23076923076923E-03,4.1025641025641E-03,3.97435897435897E-03,3.84615384615385E-03,3.71794871794872E-03,3.58974358974359E-03,3.46153846153846E-03,3.33333333333333E-03,3.20512820512821E-03,3.07692307692308E-03,2.94871794871795E-03,2.82051282051282E-03,2.69230769230769E-03,2.56410256410256E-03,2.43589743589744E-03,2.30769230769231E-03,2.17948717948718E-03,2.05128205128205E-03,1.92307692307692E-03,1.7948717948718E-03,1.66666666666667E-03,1.53846153846154E-03,1.41025641025641E-03,1.28205128205128E-03,1.15384615384615E-03,1.02564102564103E-03,8.97435897435898E-04,7.69230769230769E-04,6.41025641025641E-04,5.12820512820513E-04,3.84615384615385E-04,2.56410256410256E-04,1.28205128205128E-04,}
		valueWeights[dataIndex["default_damage_high"]] ={2.5,2.5,7.5,7.5,7.5,15,22.5,30,2.5,1.66666666666667,.833333333333333,.005,4.87179487179487E-03,4.74358974358974E-03,4.61538461538462E-03,4.48717948717949E-03,4.35897435897436E-03,4.23076923076923E-03,4.1025641025641E-03,3.97435897435897E-03,3.84615384615385E-03,3.71794871794872E-03,3.58974358974359E-03,3.46153846153846E-03,3.33333333333333E-03,3.20512820512821E-03,3.07692307692308E-03,2.94871794871795E-03,2.82051282051282E-03,2.69230769230769E-03,2.56410256410256E-03,2.43589743589744E-03,2.30769230769231E-03,2.17948717948718E-03,2.05128205128205E-03,1.92307692307692E-03,1.7948717948718E-03,1.66666666666667E-03,1.53846153846154E-03,1.41025641025641E-03,1.28205128205128E-03,1.15384615384615E-03,1.02564102564103E-03,8.97435897435898E-04,7.69230769230769E-04,6.41025641025641E-04,5.12820512820513E-04,3.84615384615385E-04,2.56410256410256E-04,1.28205128205128E-04,}
		valueWeights[dataIndex["default_dodge"]] ={75,.004,3.91836734693878E-03,3.83673469387755E-03,3.75510204081633E-03,3.6734693877551E-03,3.59183673469388E-03,3.51020408163265E-03,3.42857142857143E-03,3.3469387755102E-03,3.26530612244898E-03,3.18367346938775E-03,3.10204081632653E-03,3.02040816326531E-03,2.93877551020408E-03,2.85714285714286E-03,2.77551020408163E-03,2.69387755102041E-03,2.61224489795918E-03,2.53061224489796E-03,2.44897959183673E-03,2.36734693877551E-03,2.28571428571429E-03,2.20408163265306E-03,2.12244897959184E-03,2.04081632653061E-03,1.95918367346939E-03,1.87755102040816E-03,1.79591836734694E-03,1.71428571428571E-03,1.63265306122449E-03,1.55102040816327E-03,1.46938775510204E-03,1.38775510204082E-03,1.30612244897959E-03,1.22448979591837E-03,1.14285714285714E-03,1.06122448979592E-03,9.79591836734694E-04,8.97959183673469E-04,8.16326530612245E-04,7.3469387755102E-04,6.53061224489796E-04,5.71428571428572E-04,4.89795918367347E-04,4.08163265306123E-04,3.26530612244898E-04,2.44897959183673E-04,1.63265306122449E-04,8.16326530612248E-05,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={2.5,2.5,5,5,5,5,10,15,20,25,2.5,1.66666666666667,.833333333333333,5.26315789473684E-03,5.12091038406828E-03,4.97866287339972E-03,4.83641536273115E-03,4.69416785206259E-03,4.55192034139403E-03,4.40967283072546E-03,4.2674253200569E-03,4.12517780938834E-03,3.98293029871977E-03,3.84068278805121E-03,3.69843527738265E-03,3.55618776671408E-03,3.41394025604552E-03,3.27169274537696E-03,3.12944523470839E-03,2.98719772403983E-03,2.84495021337127E-03,2.7027027027027E-03,2.56045519203414E-03,2.41820768136558E-03,2.27596017069701E-03,2.13371266002845E-03,1.99146514935989E-03,1.84921763869132E-03,1.70697012802276E-03,1.5647226173542E-03,1.42247510668563E-03,1.28022759601707E-03,1.13798008534851E-03,9.95732574679943E-04,8.53485064011381E-04,7.11237553342817E-04,5.68990042674253E-04,4.2674253200569E-04,2.84495021337127E-04,1.42247510668564E-04,}
		valueWeights[dataIndex["default_resis_sleep"]] ={2.5,2.5,5,5,5,5,10,15,20,25,2.5,1.66666666666667,.833333333333333,5.26315789473684E-03,5.12091038406828E-03,4.97866287339972E-03,4.83641536273115E-03,4.69416785206259E-03,4.55192034139403E-03,4.40967283072546E-03,4.2674253200569E-03,4.12517780938834E-03,3.98293029871977E-03,3.84068278805121E-03,3.69843527738265E-03,3.55618776671408E-03,3.41394025604552E-03,3.27169274537696E-03,3.12944523470839E-03,2.98719772403983E-03,2.84495021337127E-03,2.7027027027027E-03,2.56045519203414E-03,2.41820768136558E-03,2.27596017069701E-03,2.13371266002845E-03,1.99146514935989E-03,1.84921763869132E-03,1.70697012802276E-03,1.5647226173542E-03,1.42247510668563E-03,1.28022759601707E-03,1.13798008534851E-03,9.95732574679943E-04,8.53485064011381E-04,7.11237553342817E-04,5.68990042674253E-04,4.2674253200569E-04,2.84495021337127E-04,1.42247510668564E-04,}
		valueWeights[dataIndex["default_resis_silence"]] ={2.5,2.5,5,5,5,5,10,15,20,25,2.5,1.66666666666667,.833333333333333,5.26315789473684E-03,5.12091038406828E-03,4.97866287339972E-03,4.83641536273115E-03,4.69416785206259E-03,4.55192034139403E-03,4.40967283072546E-03,4.2674253200569E-03,4.12517780938834E-03,3.98293029871977E-03,3.84068278805121E-03,3.69843527738265E-03,3.55618776671408E-03,3.41394025604552E-03,3.27169274537696E-03,3.12944523470839E-03,2.98719772403983E-03,2.84495021337127E-03,2.7027027027027E-03,2.56045519203414E-03,2.41820768136558E-03,2.27596017069701E-03,2.13371266002845E-03,1.99146514935989E-03,1.84921763869132E-03,1.70697012802276E-03,1.5647226173542E-03,1.42247510668563E-03,1.28022759601707E-03,1.13798008534851E-03,9.95732574679943E-04,8.53485064011381E-04,7.11237553342817E-04,5.68990042674253E-04,4.2674253200569E-04,2.84495021337127E-04,1.42247510668564E-04,}
		valueWeights[dataIndex["default_resis_weakness"]] ={2.5,2.5,5,5,5,5,10,15,20,25,2.5,1.66666666666667,.833333333333333,5.26315789473684E-03,5.12091038406828E-03,4.97866287339972E-03,4.83641536273115E-03,4.69416785206259E-03,4.55192034139403E-03,4.40967283072546E-03,4.2674253200569E-03,4.12517780938834E-03,3.98293029871977E-03,3.84068278805121E-03,3.69843527738265E-03,3.55618776671408E-03,3.41394025604552E-03,3.27169274537696E-03,3.12944523470839E-03,2.98719772403983E-03,2.84495021337127E-03,2.7027027027027E-03,2.56045519203414E-03,2.41820768136558E-03,2.27596017069701E-03,2.13371266002845E-03,1.99146514935989E-03,1.84921763869132E-03,1.70697012802276E-03,1.5647226173542E-03,1.42247510668563E-03,1.28022759601707E-03,1.13798008534851E-03,9.95732574679943E-04,8.53485064011381E-04,7.11237553342817E-04,5.68990042674253E-04,4.2674253200569E-04,2.84495021337127E-04,1.42247510668564E-04,}
		valueWeights[dataIndex["default_resis_enlace"]] ={2.5,2.5,5,5,5,5,10,15,20,25,2.5,1.66666666666667,.833333333333333,5.26315789473684E-03,5.12091038406828E-03,4.97866287339972E-03,4.83641536273115E-03,4.69416785206259E-03,4.55192034139403E-03,4.40967283072546E-03,4.2674253200569E-03,4.12517780938834E-03,3.98293029871977E-03,3.84068278805121E-03,3.69843527738265E-03,3.55618776671408E-03,3.41394025604552E-03,3.27169274537696E-03,3.12944523470839E-03,2.98719772403983E-03,2.84495021337127E-03,2.7027027027027E-03,2.56045519203414E-03,2.41820768136558E-03,2.27596017069701E-03,2.13371266002845E-03,1.99146514935989E-03,1.84921763869132E-03,1.70697012802276E-03,1.5647226173542E-03,1.42247510668563E-03,1.28022759601707E-03,1.13798008534851E-03,9.95732574679943E-04,8.53485064011381E-04,7.11237553342817E-04,5.68990042674253E-04,4.2674253200569E-04,2.84495021337127E-04,1.42247510668564E-04,}
		
		valueWeights[dataIndex["grow_hp"]] ={5,15,7.5,15,22.5,30,2.5,1.66666666666667,.833333333333333,4.76190476190476E-03,4.64576074332172E-03,4.52961672473868E-03,4.41347270615563E-03,4.29732868757259E-03,4.18118466898955E-03,4.0650406504065E-03,3.94889663182346E-03,3.83275261324042E-03,3.71660859465738E-03,3.60046457607433E-03,3.48432055749129E-03,3.36817653890825E-03,3.2520325203252E-03,3.13588850174216E-03,3.01974448315912E-03,2.90360046457607E-03,2.78745644599303E-03,2.67131242740999E-03,2.55516840882695E-03,2.4390243902439E-03,2.32288037166086E-03,2.20673635307782E-03,2.09059233449477E-03,1.97444831591173E-03,1.85830429732869E-03,1.74216027874565E-03,1.6260162601626E-03,1.50987224157956E-03,1.39372822299652E-03,1.27758420441347E-03,1.16144018583043E-03,1.04529616724739E-03,9.29152148664344E-04,8.13008130081301E-04,6.96864111498259E-04,5.80720092915215E-04,4.64576074332172E-04,3.48432055749129E-04,2.32288037166087E-04,1.16144018583043E-04,}
		valueWeights[dataIndex["grow_mp"]] ={5,15,7.5,15,22.5,30,2.5,1.66666666666667,.833333333333333,4.76190476190476E-03,4.64576074332172E-03,4.52961672473868E-03,4.41347270615563E-03,4.29732868757259E-03,4.18118466898955E-03,4.0650406504065E-03,3.94889663182346E-03,3.83275261324042E-03,3.71660859465738E-03,3.60046457607433E-03,3.48432055749129E-03,3.36817653890825E-03,3.2520325203252E-03,3.13588850174216E-03,3.01974448315912E-03,2.90360046457607E-03,2.78745644599303E-03,2.67131242740999E-03,2.55516840882695E-03,2.4390243902439E-03,2.32288037166086E-03,2.20673635307782E-03,2.09059233449477E-03,1.97444831591173E-03,1.85830429732869E-03,1.74216027874565E-03,1.6260162601626E-03,1.50987224157956E-03,1.39372822299652E-03,1.27758420441347E-03,1.16144018583043E-03,1.04529616724739E-03,9.29152148664344E-04,8.13008130081301E-04,6.96864111498259E-04,5.80720092915215E-04,4.64576074332172E-04,3.48432055749129E-04,2.32288037166087E-04,1.16144018583043E-04,}
		valueWeights[dataIndex["grow_attack_rate"]] ={75,.004,3.91836734693878E-03,3.83673469387755E-03,3.75510204081633E-03,3.6734693877551E-03,3.59183673469388E-03,3.51020408163265E-03,3.42857142857143E-03,3.3469387755102E-03,3.26530612244898E-03,3.18367346938775E-03,3.10204081632653E-03,3.02040816326531E-03,2.93877551020408E-03,2.85714285714286E-03,2.77551020408163E-03,2.69387755102041E-03,2.61224489795918E-03,2.53061224489796E-03,2.44897959183673E-03,2.36734693877551E-03,2.28571428571429E-03,2.20408163265306E-03,2.12244897959184E-03,2.04081632653061E-03,1.95918367346939E-03,1.87755102040816E-03,1.79591836734694E-03,1.71428571428571E-03,1.63265306122449E-03,1.55102040816327E-03,1.46938775510204E-03,1.38775510204082E-03,1.30612244897959E-03,1.22448979591837E-03,1.14285714285714E-03,1.06122448979592E-03,9.79591836734694E-04,8.97959183673469E-04,8.16326530612245E-04,7.3469387755102E-04,6.53061224489796E-04,5.71428571428572E-04,4.89795918367347E-04,4.08163265306123E-04,3.26530612244898E-04,2.44897959183673E-04,1.63265306122449E-04,8.16326530612248E-05,}
		valueWeights[dataIndex["grow_damage_low"]] ={2.5,2.5,7.5,7.5,5,10,15,20,25,2.5,1.66666666666667,.833333333333333,5.12820512820513E-03,4.99325236167341E-03,4.8582995951417E-03,4.72334682860999E-03,4.58839406207827E-03,4.45344129554656E-03,4.31848852901484E-03,4.18353576248313E-03,4.04858299595142E-03,3.9136302294197E-03,3.77867746288799E-03,3.64372469635628E-03,3.50877192982456E-03,3.37381916329285E-03,3.23886639676113E-03,3.10391363022942E-03,2.96896086369771E-03,2.83400809716599E-03,2.69905533063428E-03,2.56410256410256E-03,2.42914979757085E-03,2.29419703103914E-03,2.15924426450742E-03,2.02429149797571E-03,1.88933873144399E-03,1.75438596491228E-03,1.61943319838057E-03,1.48448043184885E-03,1.34952766531714E-03,1.21457489878542E-03,1.07962213225371E-03,9.44669365721997E-04,8.09716599190284E-04,6.7476383265857E-04,5.39811066126856E-04,4.04858299595142E-04,2.69905533063428E-04,1.34952766531714E-04,}
		valueWeights[dataIndex["grow_damage_high"]] ={2.5,2.5,7.5,7.5,5,10,15,20,25,2.5,1.66666666666667,.833333333333333,5.12820512820513E-03,4.99325236167341E-03,4.8582995951417E-03,4.72334682860999E-03,4.58839406207827E-03,4.45344129554656E-03,4.31848852901484E-03,4.18353576248313E-03,4.04858299595142E-03,3.9136302294197E-03,3.77867746288799E-03,3.64372469635628E-03,3.50877192982456E-03,3.37381916329285E-03,3.23886639676113E-03,3.10391363022942E-03,2.96896086369771E-03,2.83400809716599E-03,2.69905533063428E-03,2.56410256410256E-03,2.42914979757085E-03,2.29419703103914E-03,2.15924426450742E-03,2.02429149797571E-03,1.88933873144399E-03,1.75438596491228E-03,1.61943319838057E-03,1.48448043184885E-03,1.34952766531714E-03,1.21457489878542E-03,1.07962213225371E-03,9.44669365721997E-04,8.09716599190284E-04,6.7476383265857E-04,5.39811066126856E-04,4.04858299595142E-04,2.69905533063428E-04,1.34952766531714E-04,}
		valueWeights[dataIndex["grow_dodge"]] ={75,.004,3.91836734693878E-03,3.83673469387755E-03,3.75510204081633E-03,3.6734693877551E-03,3.59183673469388E-03,3.51020408163265E-03,3.42857142857143E-03,3.3469387755102E-03,3.26530612244898E-03,3.18367346938775E-03,3.10204081632653E-03,3.02040816326531E-03,2.93877551020408E-03,2.85714285714286E-03,2.77551020408163E-03,2.69387755102041E-03,2.61224489795918E-03,2.53061224489796E-03,2.44897959183673E-03,2.36734693877551E-03,2.28571428571429E-03,2.20408163265306E-03,2.12244897959184E-03,2.04081632653061E-03,1.95918367346939E-03,1.87755102040816E-03,1.79591836734694E-03,1.71428571428571E-03,1.63265306122449E-03,1.55102040816327E-03,1.46938775510204E-03,1.38775510204082E-03,1.30612244897959E-03,1.22448979591837E-03,1.14285714285714E-03,1.06122448979592E-03,9.79591836734694E-04,8.97959183673469E-04,8.16326530612245E-04,7.3469387755102E-04,6.53061224489796E-04,5.71428571428572E-04,4.89795918367347E-04,4.08163265306123E-04,3.26530612244898E-04,2.44897959183673E-04,1.63265306122449E-04,8.16326530612248E-05,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={2.5,2.5,5,5,5,5.83333333333333,11.6666666666667,17.5,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,5.26315789473684E-03,5.12091038406828E-03,4.97866287339972E-03,4.83641536273115E-03,4.69416785206259E-03,4.55192034139403E-03,4.40967283072546E-03,4.2674253200569E-03,4.12517780938834E-03,3.98293029871977E-03,3.84068278805121E-03,3.69843527738265E-03,3.55618776671408E-03,3.41394025604552E-03,3.27169274537696E-03,3.12944523470839E-03,2.98719772403983E-03,2.84495021337127E-03,2.7027027027027E-03,2.56045519203414E-03,2.41820768136558E-03,2.27596017069701E-03,2.13371266002845E-03,1.99146514935989E-03,1.84921763869132E-03,1.70697012802276E-03,1.5647226173542E-03,1.42247510668563E-03,1.28022759601707E-03,1.13798008534851E-03,9.95732574679943E-04,8.53485064011381E-04,7.11237553342817E-04,5.68990042674253E-04,4.2674253200569E-04,2.84495021337127E-04,1.42247510668564E-04,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={2.5,2.5,5,5,5,5.83333333333333,11.6666666666667,17.5,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,5.26315789473684E-03,5.12091038406828E-03,4.97866287339972E-03,4.83641536273115E-03,4.69416785206259E-03,4.55192034139403E-03,4.40967283072546E-03,4.2674253200569E-03,4.12517780938834E-03,3.98293029871977E-03,3.84068278805121E-03,3.69843527738265E-03,3.55618776671408E-03,3.41394025604552E-03,3.27169274537696E-03,3.12944523470839E-03,2.98719772403983E-03,2.84495021337127E-03,2.7027027027027E-03,2.56045519203414E-03,2.41820768136558E-03,2.27596017069701E-03,2.13371266002845E-03,1.99146514935989E-03,1.84921763869132E-03,1.70697012802276E-03,1.5647226173542E-03,1.42247510668563E-03,1.28022759601707E-03,1.13798008534851E-03,9.95732574679943E-04,8.53485064011381E-04,7.11237553342817E-04,5.68990042674253E-04,4.2674253200569E-04,2.84495021337127E-04,1.42247510668564E-04,}
		valueWeights[dataIndex["grow_resis_silence"]] ={2.5,2.5,5,5,5,5.83333333333333,11.6666666666667,17.5,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,5.26315789473684E-03,5.12091038406828E-03,4.97866287339972E-03,4.83641536273115E-03,4.69416785206259E-03,4.55192034139403E-03,4.40967283072546E-03,4.2674253200569E-03,4.12517780938834E-03,3.98293029871977E-03,3.84068278805121E-03,3.69843527738265E-03,3.55618776671408E-03,3.41394025604552E-03,3.27169274537696E-03,3.12944523470839E-03,2.98719772403983E-03,2.84495021337127E-03,2.7027027027027E-03,2.56045519203414E-03,2.41820768136558E-03,2.27596017069701E-03,2.13371266002845E-03,1.99146514935989E-03,1.84921763869132E-03,1.70697012802276E-03,1.5647226173542E-03,1.42247510668563E-03,1.28022759601707E-03,1.13798008534851E-03,9.95732574679943E-04,8.53485064011381E-04,7.11237553342817E-04,5.68990042674253E-04,4.2674253200569E-04,2.84495021337127E-04,1.42247510668564E-04,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={2.5,2.5,5,5,5,5.83333333333333,11.6666666666667,17.5,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,5.26315789473684E-03,5.12091038406828E-03,4.97866287339972E-03,4.83641536273115E-03,4.69416785206259E-03,4.55192034139403E-03,4.40967283072546E-03,4.2674253200569E-03,4.12517780938834E-03,3.98293029871977E-03,3.84068278805121E-03,3.69843527738265E-03,3.55618776671408E-03,3.41394025604552E-03,3.27169274537696E-03,3.12944523470839E-03,2.98719772403983E-03,2.84495021337127E-03,2.7027027027027E-03,2.56045519203414E-03,2.41820768136558E-03,2.27596017069701E-03,2.13371266002845E-03,1.99146514935989E-03,1.84921763869132E-03,1.70697012802276E-03,1.5647226173542E-03,1.42247510668563E-03,1.28022759601707E-03,1.13798008534851E-03,9.95732574679943E-04,8.53485064011381E-04,7.11237553342817E-04,5.68990042674253E-04,4.2674253200569E-04,2.84495021337127E-04,1.42247510668564E-04,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={2.5,2.5,5,5,5,5.83333333333333,11.6666666666667,17.5,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,5.26315789473684E-03,5.12091038406828E-03,4.97866287339972E-03,4.83641536273115E-03,4.69416785206259E-03,4.55192034139403E-03,4.40967283072546E-03,4.2674253200569E-03,4.12517780938834E-03,3.98293029871977E-03,3.84068278805121E-03,3.69843527738265E-03,3.55618776671408E-03,3.41394025604552E-03,3.27169274537696E-03,3.12944523470839E-03,2.98719772403983E-03,2.84495021337127E-03,2.7027027027027E-03,2.56045519203414E-03,2.41820768136558E-03,2.27596017069701E-03,2.13371266002845E-03,1.99146514935989E-03,1.84921763869132E-03,1.70697012802276E-03,1.5647226173542E-03,1.42247510668563E-03,1.28022759601707E-03,1.13798008534851E-03,9.95732574679943E-04,8.53485064011381E-04,7.11237553342817E-04,5.68990042674253E-04,4.2674253200569E-04,2.84495021337127E-04,1.42247510668564E-04,}

	---ÖìÈ¸Ó¡ Å­ÊÞá¦
	elseif tmplID == talismans["GW_15_ZhuQueYin"].tmplID or tmplID == talismans["JL_15_NuShouFan"].tmplID or tmplID == talismans["QJ_15_tianjimu"].tmplID then

		valueWeights[dataIndex["default_hp"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3.75,3.75,3.75,3.75,1.36363636363636,2.72727272727273,4.09090909090909,5.45454545454545,6.81818181818182,8.18181818181818,9.54545454545455,10.9090909090909,12.2727272727273,13.6363636363636,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,6.89655172413793E-03,6.65024630541872E-03,6.40394088669951E-03,6.1576354679803E-03,5.91133004926108E-03,5.66502463054187E-03,5.41871921182266E-03,5.17241379310345E-03,4.92610837438424E-03,4.67980295566502E-03,4.43349753694581E-03,4.1871921182266E-03,3.94088669950739E-03,3.69458128078818E-03,3.44827586206897E-03,3.20197044334975E-03,2.95566502463054E-03,2.70935960591133E-03,2.46305418719212E-03,2.21674876847291E-03,1.97044334975369E-03,1.72413793103448E-03,1.47783251231527E-03,1.23152709359606E-03,9.85221674876848E-04,7.38916256157635E-04,4.92610837438423E-04,2.46305418719212E-04,}
		valueWeights[dataIndex["default_mp"]] ={5,7.5,7.5,7.5,15,22.5,30,3.33333333333333,1.66666666666667,4.76190476190476E-03,4.64576074332172E-03,4.52961672473868E-03,4.41347270615563E-03,4.29732868757259E-03,4.18118466898955E-03,4.0650406504065E-03,3.94889663182346E-03,3.83275261324042E-03,3.71660859465738E-03,3.60046457607433E-03,3.48432055749129E-03,3.36817653890825E-03,3.2520325203252E-03,3.13588850174216E-03,3.01974448315912E-03,2.90360046457607E-03,2.78745644599303E-03,2.67131242740999E-03,2.55516840882695E-03,2.4390243902439E-03,2.32288037166086E-03,2.20673635307782E-03,2.09059233449477E-03,1.97444831591173E-03,1.85830429732869E-03,1.74216027874565E-03,1.6260162601626E-03,1.50987224157956E-03,1.39372822299652E-03,1.27758420441347E-03,1.16144018583043E-03,1.04529616724739E-03,9.29152148664344E-04,8.13008130081301E-04,6.96864111498259E-04,5.80720092915215E-04,4.64576074332172E-04,3.48432055749129E-04,2.32288037166087E-04,1.16144018583043E-04,}
		valueWeights[dataIndex["default_attack_rate"]] ={15,25,50,3.33333333333333,1.66666666666667,4.34782608695652E-03,4.2512077294686E-03,4.15458937198068E-03,4.05797101449275E-03,3.96135265700483E-03,3.86473429951691E-03,3.76811594202899E-03,3.67149758454106E-03,3.57487922705314E-03,3.47826086956522E-03,3.3816425120773E-03,3.28502415458937E-03,3.18840579710145E-03,3.09178743961353E-03,2.9951690821256E-03,2.89855072463768E-03,2.80193236714976E-03,2.70531400966184E-03,2.60869565217391E-03,2.51207729468599E-03,2.41545893719807E-03,2.31884057971015E-03,2.22222222222222E-03,2.1256038647343E-03,2.02898550724638E-03,1.93236714975845E-03,1.83574879227053E-03,1.73913043478261E-03,1.64251207729469E-03,1.54589371980676E-03,1.44927536231884E-03,1.35265700483092E-03,.001256038647343,1.15942028985507E-03,1.06280193236715E-03,9.66183574879228E-04,8.69565217391305E-04,7.72946859903382E-04,6.76328502415459E-04,5.79710144927537E-04,4.83091787439614E-04,3.86473429951691E-04,2.89855072463769E-04,1.93236714975846E-04,9.66183574879235E-05,}
		valueWeights[dataIndex["default_damage_low"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3,3,3,3,3,2.67857142857143,5.35714285714286,8.03571428571428,10.7142857142857,13.3928571428571,16.0714285714286,18.75,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,6.45161290322581E-03,6.23655913978495E-03,6.02150537634409E-03,5.80645161290323E-03,5.59139784946237E-03,5.37634408602151E-03,5.16129032258065E-03,4.94623655913978E-03,4.73118279569892E-03,4.51612903225806E-03,4.3010752688172E-03,4.08602150537634E-03,3.87096774193548E-03,3.65591397849462E-03,3.44086021505376E-03,3.2258064516129E-03,3.01075268817204E-03,2.79569892473118E-03,2.58064516129032E-03,2.36559139784946E-03,2.1505376344086E-03,1.93548387096774E-03,1.72043010752688E-03,1.50537634408602E-03,1.29032258064516E-03,1.0752688172043E-03,8.60215053763441E-04,6.45161290322581E-04,4.3010752688172E-04,2.1505376344086E-04,}
		valueWeights[dataIndex["default_damage_high"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3,3,3,3,3,2.67857142857143,5.35714285714286,8.03571428571428,10.7142857142857,13.3928571428571,16.0714285714286,18.75,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,6.45161290322581E-03,6.23655913978495E-03,6.02150537634409E-03,5.80645161290323E-03,5.59139784946237E-03,5.37634408602151E-03,5.16129032258065E-03,4.94623655913978E-03,4.73118279569892E-03,4.51612903225806E-03,4.3010752688172E-03,4.08602150537634E-03,3.87096774193548E-03,3.65591397849462E-03,3.44086021505376E-03,3.2258064516129E-03,3.01075268817204E-03,2.79569892473118E-03,2.58064516129032E-03,2.36559139784946E-03,2.1505376344086E-03,1.93548387096774E-03,1.72043010752688E-03,1.50537634408602E-03,1.29032258064516E-03,1.0752688172043E-03,8.60215053763441E-04,6.45161290322581E-04,4.3010752688172E-04,2.1505376344086E-04,}
		valueWeights[dataIndex["default_dodge"]] ={15,75,3.33333333333333,1.66666666666667,4.25531914893617E-03,4.16281221091582E-03,4.07030527289547E-03,3.97779833487512E-03,3.88529139685476E-03,3.79278445883441E-03,3.70027752081406E-03,3.60777058279371E-03,3.51526364477336E-03,3.42275670675301E-03,3.33024976873266E-03,3.2377428307123E-03,3.14523589269195E-03,3.0527289546716E-03,2.96022201665125E-03,2.8677150786309E-03,2.77520814061055E-03,2.68270120259019E-03,2.59019426456984E-03,2.49768732654949E-03,2.40518038852914E-03,2.31267345050879E-03,2.22016651248844E-03,2.12765957446809E-03,2.03515263644773E-03,1.94264569842738E-03,1.85013876040703E-03,1.75763182238668E-03,1.66512488436633E-03,1.57261794634598E-03,1.48011100832562E-03,1.38760407030527E-03,1.29509713228492E-03,1.20259019426457E-03,1.11008325624422E-03,1.01757631822387E-03,9.25069380203515E-04,8.32562442183164E-04,7.40055504162812E-04,6.47548566142461E-04,5.55041628122109E-04,4.62534690101757E-04,3.70027752081406E-04,2.77520814061055E-04,1.85013876040703E-04,9.25069380203515E-05,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3,3,3,3,3,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,7.14285714285714E-03,6.87830687830688E-03,6.61375661375661E-03,6.34920634920635E-03,6.08465608465609E-03,5.82010582010582E-03,5.55555555555556E-03,5.29100529100529E-03,5.02645502645503E-03,4.76190476190476E-03,4.4973544973545E-03,4.23280423280423E-03,3.96825396825397E-03,3.7037037037037E-03,3.43915343915344E-03,3.17460317460317E-03,2.91005291005291E-03,2.64550264550265E-03,2.38095238095238E-03,2.11640211640212E-03,1.85185185185185E-03,1.58730158730159E-03,1.32275132275132E-03,1.05820105820106E-03,7.93650793650794E-04,5.29100529100529E-04,2.64550264550265E-04,}
		valueWeights[dataIndex["default_resis_sleep"]] ={1.25,1.25,1.25,1.25,2.5,2.5,2.5,2.5,2.5,2.5,1.36363636363636,2.72727272727273,4.09090909090909,5.45454545454545,6.81818181818182,8.18181818181818,9.54545454545455,10.9090909090909,12.2727272727273,13.6363636363636,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["default_resis_silence"]] ={2.5,2.5,3.75,3.75,3.75,3.75,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,2.5,1.66666666666667,.833333333333333,5.55555555555556E-03,5.3968253968254E-03,5.23809523809524E-03,5.07936507936508E-03,4.92063492063492E-03,4.76190476190476E-03,4.6031746031746E-03,4.44444444444444E-03,4.28571428571429E-03,4.12698412698413E-03,3.96825396825397E-03,3.80952380952381E-03,3.65079365079365E-03,3.49206349206349E-03,3.33333333333333E-03,3.17460317460317E-03,3.01587301587302E-03,2.85714285714286E-03,2.6984126984127E-03,2.53968253968254E-03,2.38095238095238E-03,2.22222222222222E-03,2.06349206349206E-03,1.9047619047619E-03,1.74603174603175E-03,1.58730158730159E-03,1.42857142857143E-03,1.26984126984127E-03,1.11111111111111E-03,9.52380952380953E-04,7.93650793650793E-04,6.34920634920635E-04,4.76190476190476E-04,3.17460317460318E-04,1.58730158730159E-04,}
		valueWeights[dataIndex["default_resis_weakness"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3,3,3,3,3,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,2,1.5,1,.5,6.45161290322581E-03,6.23655913978495E-03,6.02150537634409E-03,5.80645161290323E-03,5.59139784946237E-03,5.37634408602151E-03,5.16129032258065E-03,4.94623655913978E-03,4.73118279569892E-03,4.51612903225806E-03,4.3010752688172E-03,4.08602150537634E-03,3.87096774193548E-03,3.65591397849462E-03,3.44086021505376E-03,3.2258064516129E-03,3.01075268817204E-03,2.79569892473118E-03,2.58064516129032E-03,2.36559139784946E-03,2.1505376344086E-03,1.93548387096774E-03,1.72043010752688E-03,1.50537634408602E-03,1.29032258064516E-03,1.0752688172043E-03,8.60215053763441E-04,6.45161290322581E-04,4.3010752688172E-04,2.1505376344086E-04,}
		valueWeights[dataIndex["default_resis_enlace"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3,3,3,3,3,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,2,1.5,1,.5,6.45161290322581E-03,6.23655913978495E-03,6.02150537634409E-03,5.80645161290323E-03,5.59139784946237E-03,5.37634408602151E-03,5.16129032258065E-03,4.94623655913978E-03,4.73118279569892E-03,4.51612903225806E-03,4.3010752688172E-03,4.08602150537634E-03,3.87096774193548E-03,3.65591397849462E-03,3.44086021505376E-03,3.2258064516129E-03,3.01075268817204E-03,2.79569892473118E-03,2.58064516129032E-03,2.36559139784946E-03,2.1505376344086E-03,1.93548387096774E-03,1.72043010752688E-03,1.50537634408602E-03,1.29032258064516E-03,1.0752688172043E-03,8.60215053763441E-04,6.45161290322581E-04,4.3010752688172E-04,2.1505376344086E-04,}
		
		valueWeights[dataIndex["grow_hp"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3.75,3.75,3.75,3.75,1.36363636363636,2.72727272727273,4.09090909090909,5.45454545454545,6.81818181818182,8.18181818181818,9.54545454545455,10.9090909090909,12.2727272727273,13.6363636363636,2,1.5,1,.5,6.66666666666667E-03,6.4367816091954E-03,6.20689655172414E-03,5.97701149425287E-03,5.74712643678161E-03,5.51724137931035E-03,5.28735632183908E-03,5.05747126436782E-03,4.82758620689655E-03,4.59770114942529E-03,4.36781609195402E-03,4.13793103448276E-03,3.90804597701149E-03,3.67816091954023E-03,3.44827586206897E-03,3.2183908045977E-03,2.98850574712644E-03,2.75862068965517E-03,2.52873563218391E-03,2.29885057471264E-03,2.06896551724138E-03,1.83908045977012E-03,1.60919540229885E-03,1.37931034482759E-03,1.14942528735632E-03,9.19540229885057E-04,6.89655172413793E-04,4.59770114942529E-04,2.29885057471264E-04,}
		valueWeights[dataIndex["grow_mp"]] ={5,7.5,7.5,7.5,15,22.5,30,3.33333333333333,1.66666666666667,4.76190476190476E-03,4.64576074332172E-03,4.52961672473868E-03,4.41347270615563E-03,4.29732868757259E-03,4.18118466898955E-03,4.0650406504065E-03,3.94889663182346E-03,3.83275261324042E-03,3.71660859465738E-03,3.60046457607433E-03,3.48432055749129E-03,3.36817653890825E-03,3.2520325203252E-03,3.13588850174216E-03,3.01974448315912E-03,2.90360046457607E-03,2.78745644599303E-03,2.67131242740999E-03,2.55516840882695E-03,2.4390243902439E-03,2.32288037166086E-03,2.20673635307782E-03,2.09059233449477E-03,1.97444831591173E-03,1.85830429732869E-03,1.74216027874565E-03,1.6260162601626E-03,1.50987224157956E-03,1.39372822299652E-03,1.27758420441347E-03,1.16144018583043E-03,1.04529616724739E-03,9.29152148664344E-04,8.13008130081301E-04,6.96864111498259E-04,5.80720092915215E-04,4.64576074332172E-04,3.48432055749129E-04,2.32288037166087E-04,1.16144018583043E-04,}
		valueWeights[dataIndex["grow_attack_rate"]] ={15,25,50,3.33333333333333,1.66666666666667,4.34782608695652E-03,4.2512077294686E-03,4.15458937198068E-03,4.05797101449275E-03,3.96135265700483E-03,3.86473429951691E-03,3.76811594202899E-03,3.67149758454106E-03,3.57487922705314E-03,3.47826086956522E-03,3.3816425120773E-03,3.28502415458937E-03,3.18840579710145E-03,3.09178743961353E-03,2.9951690821256E-03,2.89855072463768E-03,2.80193236714976E-03,2.70531400966184E-03,2.60869565217391E-03,2.51207729468599E-03,2.41545893719807E-03,2.31884057971015E-03,2.22222222222222E-03,2.1256038647343E-03,2.02898550724638E-03,1.93236714975845E-03,1.83574879227053E-03,1.73913043478261E-03,1.64251207729469E-03,1.54589371980676E-03,1.44927536231884E-03,1.35265700483092E-03,.001256038647343,1.15942028985507E-03,1.06280193236715E-03,9.66183574879228E-04,8.69565217391305E-04,7.72946859903382E-04,6.76328502415459E-04,5.79710144927537E-04,4.83091787439614E-04,3.86473429951691E-04,2.89855072463769E-04,1.93236714975846E-04,9.66183574879235E-05,}
		valueWeights[dataIndex["grow_damage_low"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3,3,3,3,3,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,6.66666666666667E-03,6.4367816091954E-03,6.20689655172414E-03,5.97701149425287E-03,5.74712643678161E-03,5.51724137931035E-03,5.28735632183908E-03,5.05747126436782E-03,4.82758620689655E-03,4.59770114942529E-03,4.36781609195402E-03,4.13793103448276E-03,3.90804597701149E-03,3.67816091954023E-03,3.44827586206897E-03,3.2183908045977E-03,2.98850574712644E-03,2.75862068965517E-03,2.52873563218391E-03,2.29885057471264E-03,2.06896551724138E-03,1.83908045977012E-03,1.60919540229885E-03,1.37931034482759E-03,1.14942528735632E-03,9.19540229885057E-04,6.89655172413793E-04,4.59770114942529E-04,2.29885057471264E-04,}
		valueWeights[dataIndex["grow_damage_high"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3,3,3,3,3,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,6.66666666666667E-03,6.4367816091954E-03,6.20689655172414E-03,5.97701149425287E-03,5.74712643678161E-03,5.51724137931035E-03,5.28735632183908E-03,5.05747126436782E-03,4.82758620689655E-03,4.59770114942529E-03,4.36781609195402E-03,4.13793103448276E-03,3.90804597701149E-03,3.67816091954023E-03,3.44827586206897E-03,3.2183908045977E-03,2.98850574712644E-03,2.75862068965517E-03,2.52873563218391E-03,2.29885057471264E-03,2.06896551724138E-03,1.83908045977012E-03,1.60919540229885E-03,1.37931034482759E-03,1.14942528735632E-03,9.19540229885057E-04,6.89655172413793E-04,4.59770114942529E-04,2.29885057471264E-04,}
		valueWeights[dataIndex["grow_dodge"]] ={15,75,3.33333333333333,1.66666666666667,4.25531914893617E-03,4.16281221091582E-03,4.07030527289547E-03,3.97779833487512E-03,3.88529139685476E-03,3.79278445883441E-03,3.70027752081406E-03,3.60777058279371E-03,3.51526364477336E-03,3.42275670675301E-03,3.33024976873266E-03,3.2377428307123E-03,3.14523589269195E-03,3.0527289546716E-03,2.96022201665125E-03,2.8677150786309E-03,2.77520814061055E-03,2.68270120259019E-03,2.59019426456984E-03,2.49768732654949E-03,2.40518038852914E-03,2.31267345050879E-03,2.22016651248844E-03,2.12765957446809E-03,2.03515263644773E-03,1.94264569842738E-03,1.85013876040703E-03,1.75763182238668E-03,1.66512488436633E-03,1.57261794634598E-03,1.48011100832562E-03,1.38760407030527E-03,1.29509713228492E-03,1.20259019426457E-03,1.11008325624422E-03,1.01757631822387E-03,9.25069380203515E-04,8.32562442183164E-04,7.40055504162812E-04,6.47548566142461E-04,5.55041628122109E-04,4.62534690101757E-04,3.70027752081406E-04,2.77520814061055E-04,1.85013876040703E-04,9.25069380203515E-05,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={3.33333333333333,3.33333333333333,3.33333333333333,7,7,7,7,7,1.38888888888889,2.77777777777778,4.16666666666667,5.55555555555556,6.94444444444444,8.33333333333333,9.72222222222222,11.1111111111111,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,7.14285714285714E-03,6.87830687830688E-03,6.61375661375661E-03,6.34920634920635E-03,6.08465608465609E-03,5.82010582010582E-03,5.55555555555556E-03,5.29100529100529E-03,5.02645502645503E-03,4.76190476190476E-03,4.4973544973545E-03,4.23280423280423E-03,3.96825396825397E-03,3.7037037037037E-03,3.43915343915344E-03,3.17460317460317E-03,2.91005291005291E-03,2.64550264550265E-03,2.38095238095238E-03,2.11640211640212E-03,1.85185185185185E-03,1.58730158730159E-03,1.32275132275132E-03,1.05820105820106E-03,7.93650793650794E-04,5.29100529100529E-04,2.64550264550265E-04,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={2.5,2.5,2.5,2.5,5.83333333333333,5.83333333333333,5.83333333333333,5.83333333333333,5.83333333333333,5.83333333333333,.909090909090909,1.81818181818182,2.72727272727273,3.63636363636364,4.54545454545455,5.45454545454545,6.36363636363636,7.27272727272727,8.18181818181818,9.09090909090909,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["grow_resis_silence"]] ={5,5,8.75,8.75,8.75,8.75,2.38095238095238,4.76190476190476,7.14285714285714,9.52380952380952,11.9047619047619,14.2857142857143,2.5,1.66666666666667,.833333333333333,5.55555555555556E-03,5.3968253968254E-03,5.23809523809524E-03,5.07936507936508E-03,4.92063492063492E-03,4.76190476190476E-03,4.6031746031746E-03,4.44444444444444E-03,4.28571428571429E-03,4.12698412698413E-03,3.96825396825397E-03,3.80952380952381E-03,3.65079365079365E-03,3.49206349206349E-03,3.33333333333333E-03,3.17460317460317E-03,3.01587301587302E-03,2.85714285714286E-03,2.6984126984127E-03,2.53968253968254E-03,2.38095238095238E-03,2.22222222222222E-03,2.06349206349206E-03,1.9047619047619E-03,1.74603174603175E-03,1.58730158730159E-03,1.42857142857143E-03,1.26984126984127E-03,1.11111111111111E-03,9.52380952380953E-04,7.93650793650793E-04,6.34920634920635E-04,4.76190476190476E-04,3.17460317460318E-04,1.58730158730159E-04,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={3.33333333333333,3.33333333333333,3.33333333333333,7,7,7,7,7,1.38888888888889,2.77777777777778,4.16666666666667,5.55555555555556,6.94444444444444,8.33333333333333,9.72222222222222,11.1111111111111,2,1.5,1,.5,6.45161290322581E-03,6.23655913978495E-03,6.02150537634409E-03,5.80645161290323E-03,5.59139784946237E-03,5.37634408602151E-03,5.16129032258065E-03,4.94623655913978E-03,4.73118279569892E-03,4.51612903225806E-03,4.3010752688172E-03,4.08602150537634E-03,3.87096774193548E-03,3.65591397849462E-03,3.44086021505376E-03,3.2258064516129E-03,3.01075268817204E-03,2.79569892473118E-03,2.58064516129032E-03,2.36559139784946E-03,2.1505376344086E-03,1.93548387096774E-03,1.72043010752688E-03,1.50537634408602E-03,1.29032258064516E-03,1.0752688172043E-03,8.60215053763441E-04,6.45161290322581E-04,4.3010752688172E-04,2.1505376344086E-04,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={3.33333333333333,3.33333333333333,3.33333333333333,7,7,7,7,7,1.38888888888889,2.77777777777778,4.16666666666667,5.55555555555556,6.94444444444444,8.33333333333333,9.72222222222222,11.1111111111111,2,1.5,1,.5,6.45161290322581E-03,6.23655913978495E-03,6.02150537634409E-03,5.80645161290323E-03,5.59139784946237E-03,5.37634408602151E-03,5.16129032258065E-03,4.94623655913978E-03,4.73118279569892E-03,4.51612903225806E-03,4.3010752688172E-03,4.08602150537634E-03,3.87096774193548E-03,3.65591397849462E-03,3.44086021505376E-03,3.2258064516129E-03,3.01075268817204E-03,2.79569892473118E-03,2.58064516129032E-03,2.36559139784946E-03,2.1505376344086E-03,1.93548387096774E-03,1.72043010752688E-03,1.50537634408602E-03,1.29032258064516E-03,1.0752688172043E-03,8.60215053763441E-04,6.45161290322581E-04,4.3010752688172E-04,2.1505376344086E-04,}

	---ÉåÐÄÁå ÆßÉ±
	elseif tmplID == talismans["HH_15_SheXinLing"].tmplID or tmplID == talismans["HG_15_QiSha"].tmplID or tmplID == talismans["YZ_15_lveying"].tmplID then
		
		valueWeights[dataIndex["default_hp"]] ={1.66666666666667,1.66666666666667,1.66666666666667,5,5,5,2.67857142857143,5.35714285714286,8.03571428571428,10.7142857142857,13.3928571428571,16.0714285714286,18.75,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,6.06060606060606E-03,5.87121212121212E-03,5.68181818181818E-03,5.49242424242424E-03,5.3030303030303E-03,5.11363636363636E-03,4.92424242424242E-03,4.73484848484849E-03,4.54545454545455E-03,4.35606060606061E-03,4.16666666666667E-03,3.97727272727273E-03,3.78787878787879E-03,3.59848484848485E-03,3.40909090909091E-03,3.21969696969697E-03,3.03030303030303E-03,2.84090909090909E-03,2.65151515151515E-03,2.46212121212121E-03,2.27272727272727E-03,2.08333333333333E-03,1.89393939393939E-03,1.70454545454545E-03,1.51515151515152E-03,1.32575757575758E-03,1.13636363636364E-03,9.46969696969697E-04,7.57575757575757E-04,5.68181818181818E-04,3.78787878787879E-04,1.8939393939394E-04,}
		valueWeights[dataIndex["default_mp"]] ={5,7.5,7.5,7.5,15,22.5,30,3.33333333333333,1.66666666666667,4.76190476190476E-03,4.64576074332172E-03,4.52961672473868E-03,4.41347270615563E-03,4.29732868757259E-03,4.18118466898955E-03,4.0650406504065E-03,3.94889663182346E-03,3.83275261324042E-03,3.71660859465738E-03,3.60046457607433E-03,3.48432055749129E-03,3.36817653890825E-03,3.2520325203252E-03,3.13588850174216E-03,3.01974448315912E-03,2.90360046457607E-03,2.78745644599303E-03,2.67131242740999E-03,2.55516840882695E-03,2.4390243902439E-03,2.32288037166086E-03,2.20673635307782E-03,2.09059233449477E-03,1.97444831591173E-03,1.85830429732869E-03,1.74216027874565E-03,1.6260162601626E-03,1.50987224157956E-03,1.39372822299652E-03,1.27758420441347E-03,1.16144018583043E-03,1.04529616724739E-03,9.29152148664344E-04,8.13008130081301E-04,6.96864111498259E-04,5.80720092915215E-04,4.64576074332172E-04,3.48432055749129E-04,2.32288037166087E-04,1.16144018583043E-04,}
		valueWeights[dataIndex["default_attack_rate"]] ={2.5,2.5,5,5,5,5,10,15,20,25,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,5.55555555555556E-03,5.3968253968254E-03,5.23809523809524E-03,5.07936507936508E-03,4.92063492063492E-03,4.76190476190476E-03,4.6031746031746E-03,4.44444444444444E-03,4.28571428571429E-03,4.12698412698413E-03,3.96825396825397E-03,3.80952380952381E-03,3.65079365079365E-03,3.49206349206349E-03,3.33333333333333E-03,3.17460317460317E-03,3.01587301587302E-03,2.85714285714286E-03,2.6984126984127E-03,2.53968253968254E-03,2.38095238095238E-03,2.22222222222222E-03,2.06349206349206E-03,1.9047619047619E-03,1.74603174603175E-03,1.58730158730159E-03,1.42857142857143E-03,1.26984126984127E-03,1.11111111111111E-03,9.52380952380953E-04,7.93650793650793E-04,6.34920634920635E-04,4.76190476190476E-04,3.17460317460318E-04,1.58730158730159E-04,}
		valueWeights[dataIndex["default_damage_low"]] ={1.25,1.25,1.25,1.25,3,3,3,3,3,1.66666666666667,3.33333333333333,5,6.66666666666667,8.33333333333333,10,11.6666666666667,13.3333333333333,15,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.14285714285714E-03,6.87830687830688E-03,6.61375661375661E-03,6.34920634920635E-03,6.08465608465609E-03,5.82010582010582E-03,5.55555555555556E-03,5.29100529100529E-03,5.02645502645503E-03,4.76190476190476E-03,4.4973544973545E-03,4.23280423280423E-03,3.96825396825397E-03,3.7037037037037E-03,3.43915343915344E-03,3.17460317460317E-03,2.91005291005291E-03,2.64550264550265E-03,2.38095238095238E-03,2.11640211640212E-03,1.85185185185185E-03,1.58730158730159E-03,1.32275132275132E-03,1.05820105820106E-03,7.93650793650794E-04,5.29100529100529E-04,2.64550264550265E-04,}
		valueWeights[dataIndex["default_damage_high"]] ={1.25,1.25,1.25,1.25,3,3,3,3,3,1.66666666666667,3.33333333333333,5,6.66666666666667,8.33333333333333,10,11.6666666666667,13.3333333333333,15,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.14285714285714E-03,6.87830687830688E-03,6.61375661375661E-03,6.34920634920635E-03,6.08465608465609E-03,5.82010582010582E-03,5.55555555555556E-03,5.29100529100529E-03,5.02645502645503E-03,4.76190476190476E-03,4.4973544973545E-03,4.23280423280423E-03,3.96825396825397E-03,3.7037037037037E-03,3.43915343915344E-03,3.17460317460317E-03,2.91005291005291E-03,2.64550264550265E-03,2.38095238095238E-03,2.11640211640212E-03,1.85185185185185E-03,1.58730158730159E-03,1.32275132275132E-03,1.05820105820106E-03,7.93650793650794E-04,5.29100529100529E-04,2.64550264550265E-04,}
		valueWeights[dataIndex["default_dodge"]] ={2.5,2.5,5,5,5,5,10,15,20,25,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,5.55555555555556E-03,5.3968253968254E-03,5.23809523809524E-03,5.07936507936508E-03,4.92063492063492E-03,4.76190476190476E-03,4.6031746031746E-03,4.44444444444444E-03,4.28571428571429E-03,4.12698412698413E-03,3.96825396825397E-03,3.80952380952381E-03,3.65079365079365E-03,3.49206349206349E-03,3.33333333333333E-03,3.17460317460317E-03,3.01587301587302E-03,2.85714285714286E-03,2.6984126984127E-03,2.53968253968254E-03,2.38095238095238E-03,2.22222222222222E-03,2.06349206349206E-03,1.9047619047619E-03,1.74603174603175E-03,1.58730158730159E-03,1.42857142857143E-03,1.26984126984127E-03,1.11111111111111E-03,9.52380952380953E-04,7.93650793650793E-04,6.34920634920635E-04,4.76190476190476E-04,3.17460317460318E-04,1.58730158730159E-04,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3,3,3,3,3,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,2,1.5,1,.5,6.45161290322581E-03,6.23655913978495E-03,6.02150537634409E-03,5.80645161290323E-03,5.59139784946237E-03,5.37634408602151E-03,5.16129032258065E-03,4.94623655913978E-03,4.73118279569892E-03,4.51612903225806E-03,4.3010752688172E-03,4.08602150537634E-03,3.87096774193548E-03,3.65591397849462E-03,3.44086021505376E-03,3.2258064516129E-03,3.01075268817204E-03,2.79569892473118E-03,2.58064516129032E-03,2.36559139784946E-03,2.1505376344086E-03,1.93548387096774E-03,1.72043010752688E-03,1.50537634408602E-03,1.29032258064516E-03,1.0752688172043E-03,8.60215053763441E-04,6.45161290322581E-04,4.3010752688172E-04,2.1505376344086E-04,}
		valueWeights[dataIndex["default_resis_sleep"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3,3,3,3,3,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,2,1.5,1,.5,6.45161290322581E-03,6.23655913978495E-03,6.02150537634409E-03,5.80645161290323E-03,5.59139784946237E-03,5.37634408602151E-03,5.16129032258065E-03,4.94623655913978E-03,4.73118279569892E-03,4.51612903225806E-03,4.3010752688172E-03,4.08602150537634E-03,3.87096774193548E-03,3.65591397849462E-03,3.44086021505376E-03,3.2258064516129E-03,3.01075268817204E-03,2.79569892473118E-03,2.58064516129032E-03,2.36559139784946E-03,2.1505376344086E-03,1.93548387096774E-03,1.72043010752688E-03,1.50537634408602E-03,1.29032258064516E-03,1.0752688172043E-03,8.60215053763441E-04,6.45161290322581E-04,4.3010752688172E-04,2.1505376344086E-04,}
		valueWeights[dataIndex["default_resis_silence"]] ={2.5,2.5,3.75,3.75,3.75,3.75,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,2.5,1.66666666666667,.833333333333333,5.55555555555556E-03,5.3968253968254E-03,5.23809523809524E-03,5.07936507936508E-03,4.92063492063492E-03,4.76190476190476E-03,4.6031746031746E-03,4.44444444444444E-03,4.28571428571429E-03,4.12698412698413E-03,3.96825396825397E-03,3.80952380952381E-03,3.65079365079365E-03,3.49206349206349E-03,3.33333333333333E-03,3.17460317460317E-03,3.01587301587302E-03,2.85714285714286E-03,2.6984126984127E-03,2.53968253968254E-03,2.38095238095238E-03,2.22222222222222E-03,2.06349206349206E-03,1.9047619047619E-03,1.74603174603175E-03,1.58730158730159E-03,1.42857142857143E-03,1.26984126984127E-03,1.11111111111111E-03,9.52380952380953E-04,7.93650793650793E-04,6.34920634920635E-04,4.76190476190476E-04,3.17460317460318E-04,1.58730158730159E-04,}
		valueWeights[dataIndex["default_resis_weakness"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3,3,3,3,3,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,7.14285714285714E-03,6.87830687830688E-03,6.61375661375661E-03,6.34920634920635E-03,6.08465608465609E-03,5.82010582010582E-03,5.55555555555556E-03,5.29100529100529E-03,5.02645502645503E-03,4.76190476190476E-03,4.4973544973545E-03,4.23280423280423E-03,3.96825396825397E-03,3.7037037037037E-03,3.43915343915344E-03,3.17460317460317E-03,2.91005291005291E-03,2.64550264550265E-03,2.38095238095238E-03,2.11640211640212E-03,1.85185185185185E-03,1.58730158730159E-03,1.32275132275132E-03,1.05820105820106E-03,7.93650793650794E-04,5.29100529100529E-04,2.64550264550265E-04,}
		valueWeights[dataIndex["default_resis_enlace"]] ={1.25,1.25,1.25,1.25,2.5,2.5,2.5,2.5,2.5,2.5,1.36363636363636,2.72727272727273,4.09090909090909,5.45454545454545,6.81818181818182,8.18181818181818,9.54545454545455,10.9090909090909,12.2727272727273,13.6363636363636,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		
		valueWeights[dataIndex["grow_hp"]] ={1.66666666666667,1.66666666666667,1.66666666666667,5,5,5,2.67857142857143,5.35714285714286,8.03571428571428,10.7142857142857,13.3928571428571,16.0714285714286,18.75,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,6.06060606060606E-03,5.87121212121212E-03,5.68181818181818E-03,5.49242424242424E-03,5.3030303030303E-03,5.11363636363636E-03,4.92424242424242E-03,4.73484848484849E-03,4.54545454545455E-03,4.35606060606061E-03,4.16666666666667E-03,3.97727272727273E-03,3.78787878787879E-03,3.59848484848485E-03,3.40909090909091E-03,3.21969696969697E-03,3.03030303030303E-03,2.84090909090909E-03,2.65151515151515E-03,2.46212121212121E-03,2.27272727272727E-03,2.08333333333333E-03,1.89393939393939E-03,1.70454545454545E-03,1.51515151515152E-03,1.32575757575758E-03,1.13636363636364E-03,9.46969696969697E-04,7.57575757575757E-04,5.68181818181818E-04,3.78787878787879E-04,1.8939393939394E-04,}
		valueWeights[dataIndex["grow_mp"]] ={5,7.5,7.5,7.5,15,22.5,30,3.33333333333333,1.66666666666667,4.76190476190476E-03,4.64576074332172E-03,4.52961672473868E-03,4.41347270615563E-03,4.29732868757259E-03,4.18118466898955E-03,4.0650406504065E-03,3.94889663182346E-03,3.83275261324042E-03,3.71660859465738E-03,3.60046457607433E-03,3.48432055749129E-03,3.36817653890825E-03,3.2520325203252E-03,3.13588850174216E-03,3.01974448315912E-03,2.90360046457607E-03,2.78745644599303E-03,2.67131242740999E-03,2.55516840882695E-03,2.4390243902439E-03,2.32288037166086E-03,2.20673635307782E-03,2.09059233449477E-03,1.97444831591173E-03,1.85830429732869E-03,1.74216027874565E-03,1.6260162601626E-03,1.50987224157956E-03,1.39372822299652E-03,1.27758420441347E-03,1.16144018583043E-03,1.04529616724739E-03,9.29152148664344E-04,8.13008130081301E-04,6.96864111498259E-04,5.80720092915215E-04,4.64576074332172E-04,3.48432055749129E-04,2.32288037166087E-04,1.16144018583043E-04,}
		valueWeights[dataIndex["grow_attack_rate"]] ={2.5,2.5,5,5,5,5,10,15,20,25,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,5.55555555555556E-03,5.3968253968254E-03,5.23809523809524E-03,5.07936507936508E-03,4.92063492063492E-03,4.76190476190476E-03,4.6031746031746E-03,4.44444444444444E-03,4.28571428571429E-03,4.12698412698413E-03,3.96825396825397E-03,3.80952380952381E-03,3.65079365079365E-03,3.49206349206349E-03,3.33333333333333E-03,3.17460317460317E-03,3.01587301587302E-03,2.85714285714286E-03,2.6984126984127E-03,2.53968253968254E-03,2.38095238095238E-03,2.22222222222222E-03,2.06349206349206E-03,1.9047619047619E-03,1.74603174603175E-03,1.58730158730159E-03,1.42857142857143E-03,1.26984126984127E-03,1.11111111111111E-03,9.52380952380953E-04,7.93650793650793E-04,6.34920634920635E-04,4.76190476190476E-04,3.17460317460318E-04,1.58730158730159E-04,}
		valueWeights[dataIndex["grow_damage_low"]] ={.75,.75,.75,.75,3,3,3,3,3,1.36363636363636,2.72727272727273,4.09090909090909,5.45454545454545,6.81818181818182,8.18181818181818,9.54545454545455,10.9090909090909,12.2727272727273,13.6363636363636,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.40740740740741E-03,7.12250712250712E-03,6.83760683760684E-03,6.55270655270655E-03,6.26780626780627E-03,5.98290598290598E-03,5.6980056980057E-03,5.41310541310541E-03,5.12820512820513E-03,4.84330484330484E-03,4.55840455840456E-03,4.27350427350427E-03,3.98860398860399E-03,3.7037037037037E-03,3.41880341880342E-03,3.13390313390313E-03,2.84900284900285E-03,2.56410256410256E-03,2.27920227920228E-03,1.99430199430199E-03,1.70940170940171E-03,1.42450142450143E-03,1.13960113960114E-03,8.54700854700855E-04,5.6980056980057E-04,2.84900284900285E-04,}
		valueWeights[dataIndex["grow_damage_high"]] ={.75,.75,.75,.75,3,3,3,3,3,1.36363636363636,2.72727272727273,4.09090909090909,5.45454545454545,6.81818181818182,8.18181818181818,9.54545454545455,10.9090909090909,12.2727272727273,13.6363636363636,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.40740740740741E-03,7.12250712250712E-03,6.83760683760684E-03,6.55270655270655E-03,6.26780626780627E-03,5.98290598290598E-03,5.6980056980057E-03,5.41310541310541E-03,5.12820512820513E-03,4.84330484330484E-03,4.55840455840456E-03,4.27350427350427E-03,3.98860398860399E-03,3.7037037037037E-03,3.41880341880342E-03,3.13390313390313E-03,2.84900284900285E-03,2.56410256410256E-03,2.27920227920228E-03,1.99430199430199E-03,1.70940170940171E-03,1.42450142450143E-03,1.13960113960114E-03,8.54700854700855E-04,5.6980056980057E-04,2.84900284900285E-04,}
		valueWeights[dataIndex["grow_dodge"]] ={2.5,2.5,5,5,5,5,10,15,20,25,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,5.55555555555556E-03,5.3968253968254E-03,5.23809523809524E-03,5.07936507936508E-03,4.92063492063492E-03,4.76190476190476E-03,4.6031746031746E-03,4.44444444444444E-03,4.28571428571429E-03,4.12698412698413E-03,3.96825396825397E-03,3.80952380952381E-03,3.65079365079365E-03,3.49206349206349E-03,3.33333333333333E-03,3.17460317460317E-03,3.01587301587302E-03,2.85714285714286E-03,2.6984126984127E-03,2.53968253968254E-03,2.38095238095238E-03,2.22222222222222E-03,2.06349206349206E-03,1.9047619047619E-03,1.74603174603175E-03,1.58730158730159E-03,1.42857142857143E-03,1.26984126984127E-03,1.11111111111111E-03,9.52380952380953E-04,7.93650793650793E-04,6.34920634920635E-04,4.76190476190476E-04,3.17460317460318E-04,1.58730158730159E-04,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={3.33333333333333,3.33333333333333,3.33333333333333,7,7,7,7,7,1.38888888888889,2.77777777777778,4.16666666666667,5.55555555555556,6.94444444444444,8.33333333333333,9.72222222222222,11.1111111111111,2,1.5,1,.5,6.45161290322581E-03,6.23655913978495E-03,6.02150537634409E-03,5.80645161290323E-03,5.59139784946237E-03,5.37634408602151E-03,5.16129032258065E-03,4.94623655913978E-03,4.73118279569892E-03,4.51612903225806E-03,4.3010752688172E-03,4.08602150537634E-03,3.87096774193548E-03,3.65591397849462E-03,3.44086021505376E-03,3.2258064516129E-03,3.01075268817204E-03,2.79569892473118E-03,2.58064516129032E-03,2.36559139784946E-03,2.1505376344086E-03,1.93548387096774E-03,1.72043010752688E-03,1.50537634408602E-03,1.29032258064516E-03,1.0752688172043E-03,8.60215053763441E-04,6.45161290322581E-04,4.3010752688172E-04,2.1505376344086E-04,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={3.33333333333333,3.33333333333333,3.33333333333333,7,7,7,7,7,1.38888888888889,2.77777777777778,4.16666666666667,5.55555555555556,6.94444444444444,8.33333333333333,9.72222222222222,11.1111111111111,2,1.5,1,.5,6.45161290322581E-03,6.23655913978495E-03,6.02150537634409E-03,5.80645161290323E-03,5.59139784946237E-03,5.37634408602151E-03,5.16129032258065E-03,4.94623655913978E-03,4.73118279569892E-03,4.51612903225806E-03,4.3010752688172E-03,4.08602150537634E-03,3.87096774193548E-03,3.65591397849462E-03,3.44086021505376E-03,3.2258064516129E-03,3.01075268817204E-03,2.79569892473118E-03,2.58064516129032E-03,2.36559139784946E-03,2.1505376344086E-03,1.93548387096774E-03,1.72043010752688E-03,1.50537634408602E-03,1.29032258064516E-03,1.0752688172043E-03,8.60215053763441E-04,6.45161290322581E-04,4.3010752688172E-04,2.1505376344086E-04,}
		valueWeights[dataIndex["grow_resis_silence"]] ={5,5,8.75,8.75,8.75,8.75,2.38095238095238,4.76190476190476,7.14285714285714,9.52380952380952,11.9047619047619,14.2857142857143,2.5,1.66666666666667,.833333333333333,5.55555555555556E-03,5.3968253968254E-03,5.23809523809524E-03,5.07936507936508E-03,4.92063492063492E-03,4.76190476190476E-03,4.6031746031746E-03,4.44444444444444E-03,4.28571428571429E-03,4.12698412698413E-03,3.96825396825397E-03,3.80952380952381E-03,3.65079365079365E-03,3.49206349206349E-03,3.33333333333333E-03,3.17460317460317E-03,3.01587301587302E-03,2.85714285714286E-03,2.6984126984127E-03,2.53968253968254E-03,2.38095238095238E-03,2.22222222222222E-03,2.06349206349206E-03,1.9047619047619E-03,1.74603174603175E-03,1.58730158730159E-03,1.42857142857143E-03,1.26984126984127E-03,1.11111111111111E-03,9.52380952380953E-04,7.93650793650793E-04,6.34920634920635E-04,4.76190476190476E-04,3.17460317460318E-04,1.58730158730159E-04,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={3.33333333333333,3.33333333333333,3.33333333333333,7,7,7,7,7,1.38888888888889,2.77777777777778,4.16666666666667,5.55555555555556,6.94444444444444,8.33333333333333,9.72222222222222,11.1111111111111,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,7.14285714285714E-03,6.87830687830688E-03,6.61375661375661E-03,6.34920634920635E-03,6.08465608465609E-03,5.82010582010582E-03,5.55555555555556E-03,5.29100529100529E-03,5.02645502645503E-03,4.76190476190476E-03,4.4973544973545E-03,4.23280423280423E-03,3.96825396825397E-03,3.7037037037037E-03,3.43915343915344E-03,3.17460317460317E-03,2.91005291005291E-03,2.64550264550265E-03,2.38095238095238E-03,2.11640211640212E-03,1.85185185185185E-03,1.58730158730159E-03,1.32275132275132E-03,1.05820105820106E-03,7.93650793650794E-04,5.29100529100529E-04,2.64550264550265E-04,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={2.5,2.5,2.5,2.5,5.83333333333333,5.83333333333333,5.83333333333333,5.83333333333333,5.83333333333333,5.83333333333333,.909090909090909,1.81818181818182,2.72727272727273,3.63636363636364,4.54545454545455,5.45454545454545,6.36363636363636,7.27272727272727,8.18181818181818,9.09090909090909,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}

	---ÁùºÏ¾µ ¾ÅÒôÂÝ
	elseif tmplID == talismans["QY_15_LiuHeJing"].tmplID or tmplID == talismans["TH_15_JiuYinLuo"].tmplID then

		valueWeights[dataIndex["default_hp"]] ={5,7.5,7.5,7.5,15,22.5,30,3.33333333333333,1.66666666666667,4.76190476190476E-03,4.64576074332172E-03,4.52961672473868E-03,4.41347270615563E-03,4.29732868757259E-03,4.18118466898955E-03,4.0650406504065E-03,3.94889663182346E-03,3.83275261324042E-03,3.71660859465738E-03,3.60046457607433E-03,3.48432055749129E-03,3.36817653890825E-03,3.2520325203252E-03,3.13588850174216E-03,3.01974448315912E-03,2.90360046457607E-03,2.78745644599303E-03,2.67131242740999E-03,2.55516840882695E-03,2.4390243902439E-03,2.32288037166086E-03,2.20673635307782E-03,2.09059233449477E-03,1.97444831591173E-03,1.85830429732869E-03,1.74216027874565E-03,1.6260162601626E-03,1.50987224157956E-03,1.39372822299652E-03,1.27758420441347E-03,1.16144018583043E-03,1.04529616724739E-03,9.29152148664344E-04,8.13008130081301E-04,6.96864111498259E-04,5.80720092915215E-04,4.64576074332172E-04,3.48432055749129E-04,2.32288037166087E-04,1.16144018583043E-04,}
		valueWeights[dataIndex["default_mp"]] ={2.5,2.5,5,5,5,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,2.5,1.66666666666667,.833333333333333,5.40540540540541E-03,5.25525525525526E-03,5.10510510510511E-03,4.95495495495496E-03,4.80480480480481E-03,4.65465465465465E-03,4.5045045045045E-03,4.35435435435435E-03,4.2042042042042E-03,4.05405405405405E-03,3.9039039039039E-03,3.75375375375375E-03,3.6036036036036E-03,3.45345345345345E-03,3.3033033033033E-03,3.15315315315315E-03,.003003003003003,2.85285285285285E-03,2.7027027027027E-03,2.55255255255255E-03,2.4024024024024E-03,2.25225225225225E-03,2.1021021021021E-03,1.95195195195195E-03,1.8018018018018E-03,1.65165165165165E-03,1.5015015015015E-03,1.35135135135135E-03,1.2012012012012E-03,1.05105105105105E-03,9.009009009009E-04,7.5075075075075E-04,6.006006006006E-04,4.5045045045045E-04,3.003003003003E-04,1.5015015015015E-04,}
		valueWeights[dataIndex["default_attack_rate"]] ={15,25,50,3.33333333333333,1.66666666666667,4.34782608695652E-03,4.2512077294686E-03,4.15458937198068E-03,4.05797101449275E-03,3.96135265700483E-03,3.86473429951691E-03,3.76811594202899E-03,3.67149758454106E-03,3.57487922705314E-03,3.47826086956522E-03,3.3816425120773E-03,3.28502415458937E-03,3.18840579710145E-03,3.09178743961353E-03,2.9951690821256E-03,2.89855072463768E-03,2.80193236714976E-03,2.70531400966184E-03,2.60869565217391E-03,2.51207729468599E-03,2.41545893719807E-03,2.31884057971015E-03,2.22222222222222E-03,2.1256038647343E-03,2.02898550724638E-03,1.93236714975845E-03,1.83574879227053E-03,1.73913043478261E-03,1.64251207729469E-03,1.54589371980676E-03,1.44927536231884E-03,1.35265700483092E-03,.001256038647343,1.15942028985507E-03,1.06280193236715E-03,9.66183574879228E-04,8.69565217391305E-04,7.72946859903382E-04,6.76328502415459E-04,5.79710144927537E-04,4.83091787439614E-04,3.86473429951691E-04,2.89855072463769E-04,1.93236714975846E-04,9.66183574879235E-05,}
		valueWeights[dataIndex["default_damage_low"]] ={1.25,1.25,1.25,1.25,2.5,2.5,2.5,2.5,2.5,2.5,1.66666666666667,3.33333333333333,5,6.66666666666667,8.33333333333333,10,11.6666666666667,13.3333333333333,15,1.42857142857143,1.19047619047619,.952380952380952,.714285714285714,.476190476190476,.238095238095238,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["default_damage_high"]] ={1.25,1.25,1.25,1.25,2.5,2.5,2.5,2.5,2.5,2.5,1.66666666666667,3.33333333333333,5,6.66666666666667,8.33333333333333,10,11.6666666666667,13.3333333333333,15,1.42857142857143,1.19047619047619,.952380952380952,.714285714285714,.476190476190476,.238095238095238,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["default_dodge"]] ={15,75,3.33333333333333,1.66666666666667,4.25531914893617E-03,4.16281221091582E-03,4.07030527289547E-03,3.97779833487512E-03,3.88529139685476E-03,3.79278445883441E-03,3.70027752081406E-03,3.60777058279371E-03,3.51526364477336E-03,3.42275670675301E-03,3.33024976873266E-03,3.2377428307123E-03,3.14523589269195E-03,3.0527289546716E-03,2.96022201665125E-03,2.8677150786309E-03,2.77520814061055E-03,2.68270120259019E-03,2.59019426456984E-03,2.49768732654949E-03,2.40518038852914E-03,2.31267345050879E-03,2.22016651248844E-03,2.12765957446809E-03,2.03515263644773E-03,1.94264569842738E-03,1.85013876040703E-03,1.75763182238668E-03,1.66512488436633E-03,1.57261794634598E-03,1.48011100832562E-03,1.38760407030527E-03,1.29509713228492E-03,1.20259019426457E-03,1.11008325624422E-03,1.01757631822387E-03,9.25069380203515E-04,8.32562442183164E-04,7.40055504162812E-04,6.47548566142461E-04,5.55041628122109E-04,4.62534690101757E-04,3.70027752081406E-04,2.77520814061055E-04,1.85013876040703E-04,9.25069380203515E-05,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={1.25,1.25,1.25,1.25,2.5,2.5,2.5,2.5,2.5,2.5,1.36363636363636,2.72727272727273,4.09090909090909,5.45454545454545,6.81818181818182,8.18181818181818,9.54545454545455,10.9090909090909,12.2727272727273,13.6363636363636,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["default_resis_sleep"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3,3,3,3,3,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,2,1.5,1,.5,6.45161290322581E-03,6.23655913978495E-03,6.02150537634409E-03,5.80645161290323E-03,5.59139784946237E-03,5.37634408602151E-03,5.16129032258065E-03,4.94623655913978E-03,4.73118279569892E-03,4.51612903225806E-03,4.3010752688172E-03,4.08602150537634E-03,3.87096774193548E-03,3.65591397849462E-03,3.44086021505376E-03,3.2258064516129E-03,3.01075268817204E-03,2.79569892473118E-03,2.58064516129032E-03,2.36559139784946E-03,2.1505376344086E-03,1.93548387096774E-03,1.72043010752688E-03,1.50537634408602E-03,1.29032258064516E-03,1.0752688172043E-03,8.60215053763441E-04,6.45161290322581E-04,4.3010752688172E-04,2.1505376344086E-04,}
		valueWeights[dataIndex["default_resis_silence"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3,3,3,3,3,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,5,5.88235294117647E-03,5.70409982174688E-03,5.52584670231729E-03,5.3475935828877E-03,5.16934046345811E-03,4.99108734402852E-03,4.81283422459893E-03,4.63458110516934E-03,4.45632798573975E-03,4.27807486631016E-03,4.09982174688057E-03,3.92156862745098E-03,3.74331550802139E-03,3.5650623885918E-03,3.38680926916221E-03,3.20855614973262E-03,3.03030303030303E-03,2.85204991087344E-03,2.67379679144385E-03,2.49554367201426E-03,2.31729055258467E-03,2.13903743315508E-03,1.96078431372549E-03,1.7825311942959E-03,1.60427807486631E-03,1.42602495543672E-03,1.24777183600713E-03,1.06951871657754E-03,8.9126559714795E-04,7.1301247771836E-04,5.3475935828877E-04,3.5650623885918E-04,1.7825311942959E-04,}
		valueWeights[dataIndex["default_resis_weakness"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3,3,3,3,3,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,7.14285714285714E-03,6.87830687830688E-03,6.61375661375661E-03,6.34920634920635E-03,6.08465608465609E-03,5.82010582010582E-03,5.55555555555556E-03,5.29100529100529E-03,5.02645502645503E-03,4.76190476190476E-03,4.4973544973545E-03,4.23280423280423E-03,3.96825396825397E-03,3.7037037037037E-03,3.43915343915344E-03,3.17460317460317E-03,2.91005291005291E-03,2.64550264550265E-03,2.38095238095238E-03,2.11640211640212E-03,1.85185185185185E-03,1.58730158730159E-03,1.32275132275132E-03,1.05820105820106E-03,7.93650793650794E-04,5.29100529100529E-04,2.64550264550265E-04,}
		valueWeights[dataIndex["default_resis_enlace"]] ={2.5,2.5,3.75,3.75,3.75,3.75,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,1.42857142857143,1.19047619047619,.952380952380952,.714285714285714,.476190476190476,.238095238095238,6.06060606060606E-03,5.87121212121212E-03,5.68181818181818E-03,5.49242424242424E-03,5.3030303030303E-03,5.11363636363636E-03,4.92424242424242E-03,4.73484848484849E-03,4.54545454545455E-03,4.35606060606061E-03,4.16666666666667E-03,3.97727272727273E-03,3.78787878787879E-03,3.59848484848485E-03,3.40909090909091E-03,3.21969696969697E-03,3.03030303030303E-03,2.84090909090909E-03,2.65151515151515E-03,2.46212121212121E-03,2.27272727272727E-03,2.08333333333333E-03,1.89393939393939E-03,1.70454545454545E-03,1.51515151515152E-03,1.32575757575758E-03,1.13636363636364E-03,9.46969696969697E-04,7.57575757575757E-04,5.68181818181818E-04,3.78787878787879E-04,1.8939393939394E-04,}
		
		valueWeights[dataIndex["grow_hp"]] ={5,7.5,7.5,7.5,15,22.5,30,3.33333333333333,1.66666666666667,4.76190476190476E-03,4.64576074332172E-03,4.52961672473868E-03,4.41347270615563E-03,4.29732868757259E-03,4.18118466898955E-03,4.0650406504065E-03,3.94889663182346E-03,3.83275261324042E-03,3.71660859465738E-03,3.60046457607433E-03,3.48432055749129E-03,3.36817653890825E-03,3.2520325203252E-03,3.13588850174216E-03,3.01974448315912E-03,2.90360046457607E-03,2.78745644599303E-03,2.67131242740999E-03,2.55516840882695E-03,2.4390243902439E-03,2.32288037166086E-03,2.20673635307782E-03,2.09059233449477E-03,1.97444831591173E-03,1.85830429732869E-03,1.74216027874565E-03,1.6260162601626E-03,1.50987224157956E-03,1.39372822299652E-03,1.27758420441347E-03,1.16144018583043E-03,1.04529616724739E-03,9.29152148664344E-04,8.13008130081301E-04,6.96864111498259E-04,5.80720092915215E-04,4.64576074332172E-04,3.48432055749129E-04,2.32288037166087E-04,1.16144018583043E-04,}
		valueWeights[dataIndex["grow_mp"]] ={2.5,2.5,5,5,5,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,2.5,1.66666666666667,.833333333333333,5.40540540540541E-03,5.25525525525526E-03,5.10510510510511E-03,4.95495495495496E-03,4.80480480480481E-03,4.65465465465465E-03,4.5045045045045E-03,4.35435435435435E-03,4.2042042042042E-03,4.05405405405405E-03,3.9039039039039E-03,3.75375375375375E-03,3.6036036036036E-03,3.45345345345345E-03,3.3033033033033E-03,3.15315315315315E-03,.003003003003003,2.85285285285285E-03,2.7027027027027E-03,2.55255255255255E-03,2.4024024024024E-03,2.25225225225225E-03,2.1021021021021E-03,1.95195195195195E-03,1.8018018018018E-03,1.65165165165165E-03,1.5015015015015E-03,1.35135135135135E-03,1.2012012012012E-03,1.05105105105105E-03,9.009009009009E-04,7.5075075075075E-04,6.006006006006E-04,4.5045045045045E-04,3.003003003003E-04,1.5015015015015E-04,}
		valueWeights[dataIndex["grow_attack_rate"]] ={15,25,50,3.33333333333333,1.66666666666667,4.34782608695652E-03,4.2512077294686E-03,4.15458937198068E-03,4.05797101449275E-03,3.96135265700483E-03,3.86473429951691E-03,3.76811594202899E-03,3.67149758454106E-03,3.57487922705314E-03,3.47826086956522E-03,3.3816425120773E-03,3.28502415458937E-03,3.18840579710145E-03,3.09178743961353E-03,2.9951690821256E-03,2.89855072463768E-03,2.80193236714976E-03,2.70531400966184E-03,2.60869565217391E-03,2.51207729468599E-03,2.41545893719807E-03,2.31884057971015E-03,2.22222222222222E-03,2.1256038647343E-03,2.02898550724638E-03,1.93236714975845E-03,1.83574879227053E-03,1.73913043478261E-03,1.64251207729469E-03,1.54589371980676E-03,1.44927536231884E-03,1.35265700483092E-03,.001256038647343,1.15942028985507E-03,1.06280193236715E-03,9.66183574879228E-04,8.69565217391305E-04,7.72946859903382E-04,6.76328502415459E-04,5.79710144927537E-04,4.83091787439614E-04,3.86473429951691E-04,2.89855072463769E-04,1.93236714975846E-04,9.66183574879235E-05,}
		valueWeights[dataIndex["grow_damage_low"]] ={.75,.75,.75,.75,2.5,2.5,2.5,2.5,2.5,2.5,1.36363636363636,2.72727272727273,4.09090909090909,5.45454545454545,6.81818181818182,8.18181818181818,9.54545454545455,10.9090909090909,12.2727272727273,13.6363636363636,1.42857142857143,1.19047619047619,.952380952380952,.714285714285714,.476190476190476,.238095238095238,.008,7.66666666666667E-03,7.33333333333333E-03,.007,6.66666666666667E-03,6.33333333333333E-03,.006,5.66666666666667E-03,5.33333333333333E-03,.005,4.66666666666667E-03,4.33333333333333E-03,.004,3.66666666666667E-03,3.33333333333333E-03,.003,2.66666666666667E-03,2.33333333333333E-03,.002,1.66666666666667E-03,1.33333333333333E-03,9.99999999999999E-04,6.66666666666666E-04,3.33333333333332E-04,}
		valueWeights[dataIndex["grow_damage_high"]] ={.75,.75,.75,.75,2.5,2.5,2.5,2.5,2.5,2.5,1.36363636363636,2.72727272727273,4.09090909090909,5.45454545454545,6.81818181818182,8.18181818181818,9.54545454545455,10.9090909090909,12.2727272727273,13.6363636363636,1.42857142857143,1.19047619047619,.952380952380952,.714285714285714,.476190476190476,.238095238095238,.008,7.66666666666667E-03,7.33333333333333E-03,.007,6.66666666666667E-03,6.33333333333333E-03,.006,5.66666666666667E-03,5.33333333333333E-03,.005,4.66666666666667E-03,4.33333333333333E-03,.004,3.66666666666667E-03,3.33333333333333E-03,.003,2.66666666666667E-03,2.33333333333333E-03,.002,1.66666666666667E-03,1.33333333333333E-03,9.99999999999999E-04,6.66666666666666E-04,3.33333333333332E-04,}
		valueWeights[dataIndex["grow_dodge"]] ={15,75,3.33333333333333,1.66666666666667,4.25531914893617E-03,4.16281221091582E-03,4.07030527289547E-03,3.97779833487512E-03,3.88529139685476E-03,3.79278445883441E-03,3.70027752081406E-03,3.60777058279371E-03,3.51526364477336E-03,3.42275670675301E-03,3.33024976873266E-03,3.2377428307123E-03,3.14523589269195E-03,3.0527289546716E-03,2.96022201665125E-03,2.8677150786309E-03,2.77520814061055E-03,2.68270120259019E-03,2.59019426456984E-03,2.49768732654949E-03,2.40518038852914E-03,2.31267345050879E-03,2.22016651248844E-03,2.12765957446809E-03,2.03515263644773E-03,1.94264569842738E-03,1.85013876040703E-03,1.75763182238668E-03,1.66512488436633E-03,1.57261794634598E-03,1.48011100832562E-03,1.38760407030527E-03,1.29509713228492E-03,1.20259019426457E-03,1.11008325624422E-03,1.01757631822387E-03,9.25069380203515E-04,8.32562442183164E-04,7.40055504162812E-04,6.47548566142461E-04,5.55041628122109E-04,4.62534690101757E-04,3.70027752081406E-04,2.77520814061055E-04,1.85013876040703E-04,9.25069380203515E-05,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={2.5,2.5,2.5,2.5,5.83333333333333,5.83333333333333,5.83333333333333,5.83333333333333,5.83333333333333,5.83333333333333,.909090909090909,1.81818181818182,2.72727272727273,3.63636363636364,4.54545454545455,5.45454545454545,6.36363636363636,7.27272727272727,8.18181818181818,9.09090909090909,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={3.33333333333333,3.33333333333333,3.33333333333333,7,7,7,7,7,1.38888888888889,2.77777777777778,4.16666666666667,5.55555555555556,6.94444444444444,8.33333333333333,9.72222222222222,11.1111111111111,2,1.5,1,.5,6.45161290322581E-03,6.23655913978495E-03,6.02150537634409E-03,5.80645161290323E-03,5.59139784946237E-03,5.37634408602151E-03,5.16129032258065E-03,4.94623655913978E-03,4.73118279569892E-03,4.51612903225806E-03,4.3010752688172E-03,4.08602150537634E-03,3.87096774193548E-03,3.65591397849462E-03,3.44086021505376E-03,3.2258064516129E-03,3.01075268817204E-03,2.79569892473118E-03,2.58064516129032E-03,2.36559139784946E-03,2.1505376344086E-03,1.93548387096774E-03,1.72043010752688E-03,1.50537634408602E-03,1.29032258064516E-03,1.0752688172043E-03,8.60215053763441E-04,6.45161290322581E-04,4.3010752688172E-04,2.1505376344086E-04,}
		valueWeights[dataIndex["grow_resis_silence"]] ={3.33333333333333,3.33333333333333,3.33333333333333,7,7,7,7,7,1.38888888888889,2.77777777777778,4.16666666666667,5.55555555555556,6.94444444444444,8.33333333333333,9.72222222222222,11.1111111111111,5,5.88235294117647E-03,5.70409982174688E-03,5.52584670231729E-03,5.3475935828877E-03,5.16934046345811E-03,4.99108734402852E-03,4.81283422459893E-03,4.63458110516934E-03,4.45632798573975E-03,4.27807486631016E-03,4.09982174688057E-03,3.92156862745098E-03,3.74331550802139E-03,3.5650623885918E-03,3.38680926916221E-03,3.20855614973262E-03,3.03030303030303E-03,2.85204991087344E-03,2.67379679144385E-03,2.49554367201426E-03,2.31729055258467E-03,2.13903743315508E-03,1.96078431372549E-03,1.7825311942959E-03,1.60427807486631E-03,1.42602495543672E-03,1.24777183600713E-03,1.06951871657754E-03,8.9126559714795E-04,7.1301247771836E-04,5.3475935828877E-04,3.5650623885918E-04,1.7825311942959E-04,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={3.33333333333333,3.33333333333333,3.33333333333333,7,7,7,7,7,1.38888888888889,2.77777777777778,4.16666666666667,5.55555555555556,6.94444444444444,8.33333333333333,9.72222222222222,11.1111111111111,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,7.14285714285714E-03,6.87830687830688E-03,6.61375661375661E-03,6.34920634920635E-03,6.08465608465609E-03,5.82010582010582E-03,5.55555555555556E-03,5.29100529100529E-03,5.02645502645503E-03,4.76190476190476E-03,4.4973544973545E-03,4.23280423280423E-03,3.96825396825397E-03,3.7037037037037E-03,3.43915343915344E-03,3.17460317460317E-03,2.91005291005291E-03,2.64550264550265E-03,2.38095238095238E-03,2.11640211640212E-03,1.85185185185185E-03,1.58730158730159E-03,1.32275132275132E-03,1.05820105820106E-03,7.93650793650794E-04,5.29100529100529E-04,2.64550264550265E-04,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={5,5,8.75,8.75,8.75,8.75,2.38095238095238,4.76190476190476,7.14285714285714,9.52380952380952,11.9047619047619,14.2857142857143,1.42857142857143,1.19047619047619,.952380952380952,.714285714285714,.476190476190476,.238095238095238,6.06060606060606E-03,5.87121212121212E-03,5.68181818181818E-03,5.49242424242424E-03,5.3030303030303E-03,5.11363636363636E-03,4.92424242424242E-03,4.73484848484849E-03,4.54545454545455E-03,4.35606060606061E-03,4.16666666666667E-03,3.97727272727273E-03,3.78787878787879E-03,3.59848484848485E-03,3.40909090909091E-03,3.21969696969697E-03,3.03030303030303E-03,2.84090909090909E-03,2.65151515151515E-03,2.46212121212121E-03,2.27272727272727E-03,2.08333333333333E-03,1.89393939393939E-03,1.70454545454545E-03,1.51515151515152E-03,1.32575757575758E-03,1.13636363636364E-03,9.46969696969697E-04,7.57575757575757E-04,5.68181818181818E-04,3.78787878787879E-04,1.8939393939394E-04,}

	---ÂÖ»ØÖé µ¤Öì ÂÊ¶µÆì
	elseif tmplID == talismans["TY_15_LunHuiZhu"].tmplID or tmplID == talismans["LS_15_DanZhu"].tmplID or tmplID == talismans["CH_15_lvdouqi"].tmplID then
			
		valueWeights[dataIndex["default_hp"]] ={5,5,5,5,7.5,15,22.5,30,3.33333333333333,1.66666666666667,4.87804878048781E-03,4.75609756097561E-03,4.63414634146342E-03,4.51219512195122E-03,4.39024390243902E-03,4.26829268292683E-03,4.14634146341464E-03,4.02439024390244E-03,3.90243902439024E-03,3.78048780487805E-03,3.65853658536585E-03,3.53658536585366E-03,3.41463414634146E-03,3.29268292682927E-03,3.17073170731707E-03,3.04878048780488E-03,2.92682926829268E-03,2.80487804878049E-03,2.68292682926829E-03,2.5609756097561E-03,2.4390243902439E-03,2.31707317073171E-03,2.19512195121951E-03,2.07317073170732E-03,1.95121951219512E-03,1.82926829268293E-03,1.70731707317073E-03,1.58536585365854E-03,1.46341463414634E-03,1.34146341463415E-03,1.21951219512195E-03,1.09756097560976E-03,9.75609756097561E-04,8.53658536585367E-04,7.31707317073171E-04,6.09756097560976E-04,4.87804878048781E-04,3.65853658536586E-04,2.4390243902439E-04,1.21951219512195E-04,}
		valueWeights[dataIndex["default_mp"]] ={2.5,2.5,5,5,5,5,10,15,20,25,2,1.5,1,.5,5.40540540540541E-03,5.25525525525526E-03,5.10510510510511E-03,4.95495495495496E-03,4.80480480480481E-03,4.65465465465465E-03,4.5045045045045E-03,4.35435435435435E-03,4.2042042042042E-03,4.05405405405405E-03,3.9039039039039E-03,3.75375375375375E-03,3.6036036036036E-03,3.45345345345345E-03,3.3033033033033E-03,3.15315315315315E-03,.003003003003003,2.85285285285285E-03,2.7027027027027E-03,2.55255255255255E-03,2.4024024024024E-03,2.25225225225225E-03,2.1021021021021E-03,1.95195195195195E-03,1.8018018018018E-03,1.65165165165165E-03,1.5015015015015E-03,1.35135135135135E-03,1.2012012012012E-03,1.05105105105105E-03,9.009009009009E-04,7.5075075075075E-04,6.006006006006E-04,4.5045045045045E-04,3.003003003003E-04,1.5015015015015E-04,}
		valueWeights[dataIndex["default_attack_rate"]] ={15,25,50,3.33333333333333,1.66666666666667,4.34782608695652E-03,4.2512077294686E-03,4.15458937198068E-03,4.05797101449275E-03,3.96135265700483E-03,3.86473429951691E-03,3.76811594202899E-03,3.67149758454106E-03,3.57487922705314E-03,3.47826086956522E-03,3.3816425120773E-03,3.28502415458937E-03,3.18840579710145E-03,3.09178743961353E-03,2.9951690821256E-03,2.89855072463768E-03,2.80193236714976E-03,2.70531400966184E-03,2.60869565217391E-03,2.51207729468599E-03,2.41545893719807E-03,2.31884057971015E-03,2.22222222222222E-03,2.1256038647343E-03,2.02898550724638E-03,1.93236714975845E-03,1.83574879227053E-03,1.73913043478261E-03,1.64251207729469E-03,1.54589371980676E-03,1.44927536231884E-03,1.35265700483092E-03,.001256038647343,1.15942028985507E-03,1.06280193236715E-03,9.66183574879228E-04,8.69565217391305E-04,7.72946859903382E-04,6.76328502415459E-04,5.79710144927537E-04,4.83091787439614E-04,3.86473429951691E-04,2.89855072463769E-04,1.93236714975846E-04,9.66183574879235E-05,}
		valueWeights[dataIndex["default_damage_low"]] ={1.66666666666667,1.66666666666667,1.66666666666667,2.5,2.5,2.5,2.5,2.5,2.5,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,6.89655172413793E-03,6.65024630541872E-03,6.40394088669951E-03,6.1576354679803E-03,5.91133004926108E-03,5.66502463054187E-03,5.41871921182266E-03,5.17241379310345E-03,4.92610837438424E-03,4.67980295566502E-03,4.43349753694581E-03,4.1871921182266E-03,3.94088669950739E-03,3.69458128078818E-03,3.44827586206897E-03,3.20197044334975E-03,2.95566502463054E-03,2.70935960591133E-03,2.46305418719212E-03,2.21674876847291E-03,1.97044334975369E-03,1.72413793103448E-03,1.47783251231527E-03,1.23152709359606E-03,9.85221674876848E-04,7.38916256157635E-04,4.92610837438423E-04,2.46305418719212E-04,}
		valueWeights[dataIndex["default_damage_high"]] ={1.66666666666667,1.66666666666667,1.66666666666667,2.5,2.5,2.5,2.5,2.5,2.5,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,6.89655172413793E-03,6.65024630541872E-03,6.40394088669951E-03,6.1576354679803E-03,5.91133004926108E-03,5.66502463054187E-03,5.41871921182266E-03,5.17241379310345E-03,4.92610837438424E-03,4.67980295566502E-03,4.43349753694581E-03,4.1871921182266E-03,3.94088669950739E-03,3.69458128078818E-03,3.44827586206897E-03,3.20197044334975E-03,2.95566502463054E-03,2.70935960591133E-03,2.46305418719212E-03,2.21674876847291E-03,1.97044334975369E-03,1.72413793103448E-03,1.47783251231527E-03,1.23152709359606E-03,9.85221674876848E-04,7.38916256157635E-04,4.92610837438423E-04,2.46305418719212E-04,}
		valueWeights[dataIndex["default_dodge"]] ={15,75,3.33333333333333,1.66666666666667,4.25531914893617E-03,4.16281221091582E-03,4.07030527289547E-03,3.97779833487512E-03,3.88529139685476E-03,3.79278445883441E-03,3.70027752081406E-03,3.60777058279371E-03,3.51526364477336E-03,3.42275670675301E-03,3.33024976873266E-03,3.2377428307123E-03,3.14523589269195E-03,3.0527289546716E-03,2.96022201665125E-03,2.8677150786309E-03,2.77520814061055E-03,2.68270120259019E-03,2.59019426456984E-03,2.49768732654949E-03,2.40518038852914E-03,2.31267345050879E-03,2.22016651248844E-03,2.12765957446809E-03,2.03515263644773E-03,1.94264569842738E-03,1.85013876040703E-03,1.75763182238668E-03,1.66512488436633E-03,1.57261794634598E-03,1.48011100832562E-03,1.38760407030527E-03,1.29509713228492E-03,1.20259019426457E-03,1.11008325624422E-03,1.01757631822387E-03,9.25069380203515E-04,8.32562442183164E-04,7.40055504162812E-04,6.47548566142461E-04,5.55041628122109E-04,4.62534690101757E-04,3.70027752081406E-04,2.77520814061055E-04,1.85013876040703E-04,9.25069380203515E-05,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3,3,3,3,3,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,2,1.5,1,.5,6.45161290322581E-03,6.23655913978495E-03,6.02150537634409E-03,5.80645161290323E-03,5.59139784946237E-03,5.37634408602151E-03,5.16129032258065E-03,4.94623655913978E-03,4.73118279569892E-03,4.51612903225806E-03,4.3010752688172E-03,4.08602150537634E-03,3.87096774193548E-03,3.65591397849462E-03,3.44086021505376E-03,3.2258064516129E-03,3.01075268817204E-03,2.79569892473118E-03,2.58064516129032E-03,2.36559139784946E-03,2.1505376344086E-03,1.93548387096774E-03,1.72043010752688E-03,1.50537634408602E-03,1.29032258064516E-03,1.0752688172043E-03,8.60215053763441E-04,6.45161290322581E-04,4.3010752688172E-04,2.1505376344086E-04,}
		valueWeights[dataIndex["default_resis_sleep"]] ={2.5,2.5,3.75,3.75,3.75,3.75,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,2.5,1.66666666666667,.833333333333333,5.55555555555556E-03,5.3968253968254E-03,5.23809523809524E-03,5.07936507936508E-03,4.92063492063492E-03,4.76190476190476E-03,4.6031746031746E-03,4.44444444444444E-03,4.28571428571429E-03,4.12698412698413E-03,3.96825396825397E-03,3.80952380952381E-03,3.65079365079365E-03,3.49206349206349E-03,3.33333333333333E-03,3.17460317460317E-03,3.01587301587302E-03,2.85714285714286E-03,2.6984126984127E-03,2.53968253968254E-03,2.38095238095238E-03,2.22222222222222E-03,2.06349206349206E-03,1.9047619047619E-03,1.74603174603175E-03,1.58730158730159E-03,1.42857142857143E-03,1.26984126984127E-03,1.11111111111111E-03,9.52380952380953E-04,7.93650793650793E-04,6.34920634920635E-04,4.76190476190476E-04,3.17460317460318E-04,1.58730158730159E-04,}
		valueWeights[dataIndex["default_resis_silence"]] ={1.25,1.25,1.25,1.25,2.5,2.5,2.5,2.5,2.5,2.5,1.36363636363636,2.72727272727273,4.09090909090909,5.45454545454545,6.81818181818182,8.18181818181818,9.54545454545455,10.9090909090909,12.2727272727273,13.6363636363636,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["default_resis_weakness"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3,3,3,3,3,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,2,1.5,1,.5,6.45161290322581E-03,6.23655913978495E-03,6.02150537634409E-03,5.80645161290323E-03,5.59139784946237E-03,5.37634408602151E-03,5.16129032258065E-03,4.94623655913978E-03,4.73118279569892E-03,4.51612903225806E-03,4.3010752688172E-03,4.08602150537634E-03,3.87096774193548E-03,3.65591397849462E-03,3.44086021505376E-03,3.2258064516129E-03,3.01075268817204E-03,2.79569892473118E-03,2.58064516129032E-03,2.36559139784946E-03,2.1505376344086E-03,1.93548387096774E-03,1.72043010752688E-03,1.50537634408602E-03,1.29032258064516E-03,1.0752688172043E-03,8.60215053763441E-04,6.45161290322581E-04,4.3010752688172E-04,2.1505376344086E-04,}
		valueWeights[dataIndex["default_resis_enlace"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3,3,3,3,3,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,7.14285714285714E-03,6.87830687830688E-03,6.61375661375661E-03,6.34920634920635E-03,6.08465608465609E-03,5.82010582010582E-03,5.55555555555556E-03,5.29100529100529E-03,5.02645502645503E-03,4.76190476190476E-03,4.4973544973545E-03,4.23280423280423E-03,3.96825396825397E-03,3.7037037037037E-03,3.43915343915344E-03,3.17460317460317E-03,2.91005291005291E-03,2.64550264550265E-03,2.38095238095238E-03,2.11640211640212E-03,1.85185185185185E-03,1.58730158730159E-03,1.32275132275132E-03,1.05820105820106E-03,7.93650793650794E-04,5.29100529100529E-04,2.64550264550265E-04,}
		
		valueWeights[dataIndex["grow_hp"]] ={5,5,5,5,7.5,15,22.5,30,3.33333333333333,1.66666666666667,4.87804878048781E-03,4.75609756097561E-03,4.63414634146342E-03,4.51219512195122E-03,4.39024390243902E-03,4.26829268292683E-03,4.14634146341464E-03,4.02439024390244E-03,3.90243902439024E-03,3.78048780487805E-03,3.65853658536585E-03,3.53658536585366E-03,3.41463414634146E-03,3.29268292682927E-03,3.17073170731707E-03,3.04878048780488E-03,2.92682926829268E-03,2.80487804878049E-03,2.68292682926829E-03,2.5609756097561E-03,2.4390243902439E-03,2.31707317073171E-03,2.19512195121951E-03,2.07317073170732E-03,1.95121951219512E-03,1.82926829268293E-03,1.70731707317073E-03,1.58536585365854E-03,1.46341463414634E-03,1.34146341463415E-03,1.21951219512195E-03,1.09756097560976E-03,9.75609756097561E-04,8.53658536585367E-04,7.31707317073171E-04,6.09756097560976E-04,4.87804878048781E-04,3.65853658536586E-04,2.4390243902439E-04,1.21951219512195E-04,}
		valueWeights[dataIndex["grow_mp"]] ={2.5,2.5,5,5,5,5,10,15,20,25,2,1.5,1,.5,5.40540540540541E-03,5.25525525525526E-03,5.10510510510511E-03,4.95495495495496E-03,4.80480480480481E-03,4.65465465465465E-03,4.5045045045045E-03,4.35435435435435E-03,4.2042042042042E-03,4.05405405405405E-03,3.9039039039039E-03,3.75375375375375E-03,3.6036036036036E-03,3.45345345345345E-03,3.3033033033033E-03,3.15315315315315E-03,.003003003003003,2.85285285285285E-03,2.7027027027027E-03,2.55255255255255E-03,2.4024024024024E-03,2.25225225225225E-03,2.1021021021021E-03,1.95195195195195E-03,1.8018018018018E-03,1.65165165165165E-03,1.5015015015015E-03,1.35135135135135E-03,1.2012012012012E-03,1.05105105105105E-03,9.009009009009E-04,7.5075075075075E-04,6.006006006006E-04,4.5045045045045E-04,3.003003003003E-04,1.5015015015015E-04,}
		valueWeights[dataIndex["grow_attack_rate"]] ={15,25,50,3.33333333333333,1.66666666666667,4.34782608695652E-03,4.2512077294686E-03,4.15458937198068E-03,4.05797101449275E-03,3.96135265700483E-03,3.86473429951691E-03,3.76811594202899E-03,3.67149758454106E-03,3.57487922705314E-03,3.47826086956522E-03,3.3816425120773E-03,3.28502415458937E-03,3.18840579710145E-03,3.09178743961353E-03,2.9951690821256E-03,2.89855072463768E-03,2.80193236714976E-03,2.70531400966184E-03,2.60869565217391E-03,2.51207729468599E-03,2.41545893719807E-03,2.31884057971015E-03,2.22222222222222E-03,2.1256038647343E-03,2.02898550724638E-03,1.93236714975845E-03,1.83574879227053E-03,1.73913043478261E-03,1.64251207729469E-03,1.54589371980676E-03,1.44927536231884E-03,1.35265700483092E-03,.001256038647343,1.15942028985507E-03,1.06280193236715E-03,9.66183574879228E-04,8.69565217391305E-04,7.72946859903382E-04,6.76328502415459E-04,5.79710144927537E-04,4.83091787439614E-04,3.86473429951691E-04,2.89855072463769E-04,1.93236714975846E-04,9.66183574879235E-05,}
		valueWeights[dataIndex["grow_damage_low"]] ={1.66666666666667,1.66666666666667,1.66666666666667,2.5,2.5,2.5,2.5,2.5,2.5,1.66666666666667,3.33333333333333,5,6.66666666666667,8.33333333333333,10,11.6666666666667,13.3333333333333,15,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.14285714285714E-03,6.87830687830688E-03,6.61375661375661E-03,6.34920634920635E-03,6.08465608465609E-03,5.82010582010582E-03,5.55555555555556E-03,5.29100529100529E-03,5.02645502645503E-03,4.76190476190476E-03,4.4973544973545E-03,4.23280423280423E-03,3.96825396825397E-03,3.7037037037037E-03,3.43915343915344E-03,3.17460317460317E-03,2.91005291005291E-03,2.64550264550265E-03,2.38095238095238E-03,2.11640211640212E-03,1.85185185185185E-03,1.58730158730159E-03,1.32275132275132E-03,1.05820105820106E-03,7.93650793650794E-04,5.29100529100529E-04,2.64550264550265E-04,}
		valueWeights[dataIndex["grow_damage_high"]] ={1.66666666666667,1.66666666666667,1.66666666666667,2.5,2.5,2.5,2.5,2.5,2.5,1.66666666666667,3.33333333333333,5,6.66666666666667,8.33333333333333,10,11.6666666666667,13.3333333333333,15,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.14285714285714E-03,6.87830687830688E-03,6.61375661375661E-03,6.34920634920635E-03,6.08465608465609E-03,5.82010582010582E-03,5.55555555555556E-03,5.29100529100529E-03,5.02645502645503E-03,4.76190476190476E-03,4.4973544973545E-03,4.23280423280423E-03,3.96825396825397E-03,3.7037037037037E-03,3.43915343915344E-03,3.17460317460317E-03,2.91005291005291E-03,2.64550264550265E-03,2.38095238095238E-03,2.11640211640212E-03,1.85185185185185E-03,1.58730158730159E-03,1.32275132275132E-03,1.05820105820106E-03,7.93650793650794E-04,5.29100529100529E-04,2.64550264550265E-04,}
		valueWeights[dataIndex["grow_dodge"]] ={15,75,3.33333333333333,1.66666666666667,4.25531914893617E-03,4.16281221091582E-03,4.07030527289547E-03,3.97779833487512E-03,3.88529139685476E-03,3.79278445883441E-03,3.70027752081406E-03,3.60777058279371E-03,3.51526364477336E-03,3.42275670675301E-03,3.33024976873266E-03,3.2377428307123E-03,3.14523589269195E-03,3.0527289546716E-03,2.96022201665125E-03,2.8677150786309E-03,2.77520814061055E-03,2.68270120259019E-03,2.59019426456984E-03,2.49768732654949E-03,2.40518038852914E-03,2.31267345050879E-03,2.22016651248844E-03,2.12765957446809E-03,2.03515263644773E-03,1.94264569842738E-03,1.85013876040703E-03,1.75763182238668E-03,1.66512488436633E-03,1.57261794634598E-03,1.48011100832562E-03,1.38760407030527E-03,1.29509713228492E-03,1.20259019426457E-03,1.11008325624422E-03,1.01757631822387E-03,9.25069380203515E-04,8.32562442183164E-04,7.40055504162812E-04,6.47548566142461E-04,5.55041628122109E-04,4.62534690101757E-04,3.70027752081406E-04,2.77520814061055E-04,1.85013876040703E-04,9.25069380203515E-05,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={3.33333333333333,3.33333333333333,3.33333333333333,7,7,7,7,7,1.38888888888889,2.77777777777778,4.16666666666667,5.55555555555556,6.94444444444444,8.33333333333333,9.72222222222222,11.1111111111111,2,1.5,1,.5,6.45161290322581E-03,6.23655913978495E-03,6.02150537634409E-03,5.80645161290323E-03,5.59139784946237E-03,5.37634408602151E-03,5.16129032258065E-03,4.94623655913978E-03,4.73118279569892E-03,4.51612903225806E-03,4.3010752688172E-03,4.08602150537634E-03,3.87096774193548E-03,3.65591397849462E-03,3.44086021505376E-03,3.2258064516129E-03,3.01075268817204E-03,2.79569892473118E-03,2.58064516129032E-03,2.36559139784946E-03,2.1505376344086E-03,1.93548387096774E-03,1.72043010752688E-03,1.50537634408602E-03,1.29032258064516E-03,1.0752688172043E-03,8.60215053763441E-04,6.45161290322581E-04,4.3010752688172E-04,2.1505376344086E-04,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={5,5,8.75,8.75,8.75,8.75,2.38095238095238,4.76190476190476,7.14285714285714,9.52380952380952,11.9047619047619,14.2857142857143,2.5,1.66666666666667,.833333333333333,5.55555555555556E-03,5.3968253968254E-03,5.23809523809524E-03,5.07936507936508E-03,4.92063492063492E-03,4.76190476190476E-03,4.6031746031746E-03,4.44444444444444E-03,4.28571428571429E-03,4.12698412698413E-03,3.96825396825397E-03,3.80952380952381E-03,3.65079365079365E-03,3.49206349206349E-03,3.33333333333333E-03,3.17460317460317E-03,3.01587301587302E-03,2.85714285714286E-03,2.6984126984127E-03,2.53968253968254E-03,2.38095238095238E-03,2.22222222222222E-03,2.06349206349206E-03,1.9047619047619E-03,1.74603174603175E-03,1.58730158730159E-03,1.42857142857143E-03,1.26984126984127E-03,1.11111111111111E-03,9.52380952380953E-04,7.93650793650793E-04,6.34920634920635E-04,4.76190476190476E-04,3.17460317460318E-04,1.58730158730159E-04,}
		valueWeights[dataIndex["grow_resis_silence"]] ={2.5,2.5,2.5,2.5,5.83333333333333,5.83333333333333,5.83333333333333,5.83333333333333,5.83333333333333,5.83333333333333,.909090909090909,1.81818181818182,2.72727272727273,3.63636363636364,4.54545454545455,5.45454545454545,6.36363636363636,7.27272727272727,8.18181818181818,9.09090909090909,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={3.33333333333333,3.33333333333333,3.33333333333333,7,7,7,7,7,1.38888888888889,2.77777777777778,4.16666666666667,5.55555555555556,6.94444444444444,8.33333333333333,9.72222222222222,11.1111111111111,2,1.5,1,.5,6.45161290322581E-03,6.23655913978495E-03,6.02150537634409E-03,5.80645161290323E-03,5.59139784946237E-03,5.37634408602151E-03,5.16129032258065E-03,4.94623655913978E-03,4.73118279569892E-03,4.51612903225806E-03,4.3010752688172E-03,4.08602150537634E-03,3.87096774193548E-03,3.65591397849462E-03,3.44086021505376E-03,3.2258064516129E-03,3.01075268817204E-03,2.79569892473118E-03,2.58064516129032E-03,2.36559139784946E-03,2.1505376344086E-03,1.93548387096774E-03,1.72043010752688E-03,1.50537634408602E-03,1.29032258064516E-03,1.0752688172043E-03,8.60215053763441E-04,6.45161290322581E-04,4.3010752688172E-04,2.1505376344086E-04,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={3.33333333333333,3.33333333333333,3.33333333333333,7,7,7,7,7,1.38888888888889,2.77777777777778,4.16666666666667,5.55555555555556,6.94444444444444,8.33333333333333,9.72222222222222,11.1111111111111,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,7.14285714285714E-03,6.87830687830688E-03,6.61375661375661E-03,6.34920634920635E-03,6.08465608465609E-03,5.82010582010582E-03,5.55555555555556E-03,5.29100529100529E-03,5.02645502645503E-03,4.76190476190476E-03,4.4973544973545E-03,4.23280423280423E-03,3.96825396825397E-03,3.7037037037037E-03,3.43915343915344E-03,3.17460317460317E-03,2.91005291005291E-03,2.64550264550265E-03,2.38095238095238E-03,2.11640211640212E-03,1.85185185185185E-03,1.58730158730159E-03,1.32275132275132E-03,1.05820105820106E-03,7.93650793650794E-04,5.29100529100529E-04,2.64550264550265E-04,}
		
	---ÑªÓñ¹ÇÆ¬ ÄÏÃ÷Ó¡ À×¹«Ä«
	elseif tmplID == talismans["GD_15_XueYuGuPian"].tmplID or tmplID == talismans["FX_15_NanMingYin"].tmplID or tmplID == talismans["TH_15_LeiGongMo"].tmplID then
			
		valueWeights[dataIndex["default_hp"]] ={5,5,5,5,7.5,15,22.5,30,3.33333333333333,1.66666666666667,4.87804878048781E-03,4.75609756097561E-03,4.63414634146342E-03,4.51219512195122E-03,4.39024390243902E-03,4.26829268292683E-03,4.14634146341464E-03,4.02439024390244E-03,3.90243902439024E-03,3.78048780487805E-03,3.65853658536585E-03,3.53658536585366E-03,3.41463414634146E-03,3.29268292682927E-03,3.17073170731707E-03,3.04878048780488E-03,2.92682926829268E-03,2.80487804878049E-03,2.68292682926829E-03,2.5609756097561E-03,2.4390243902439E-03,2.31707317073171E-03,2.19512195121951E-03,2.07317073170732E-03,1.95121951219512E-03,1.82926829268293E-03,1.70731707317073E-03,1.58536585365854E-03,1.46341463414634E-03,1.34146341463415E-03,1.21951219512195E-03,1.09756097560976E-03,9.75609756097561E-04,8.53658536585367E-04,7.31707317073171E-04,6.09756097560976E-04,4.87804878048781E-04,3.65853658536586E-04,2.4390243902439E-04,1.21951219512195E-04,}
		valueWeights[dataIndex["default_mp"]] ={2.5,2.5,5,5,5,5,10,15,20,25,2,1.5,1,.5,5.40540540540541E-03,5.25525525525526E-03,5.10510510510511E-03,4.95495495495496E-03,4.80480480480481E-03,4.65465465465465E-03,4.5045045045045E-03,4.35435435435435E-03,4.2042042042042E-03,4.05405405405405E-03,3.9039039039039E-03,3.75375375375375E-03,3.6036036036036E-03,3.45345345345345E-03,3.3033033033033E-03,3.15315315315315E-03,.003003003003003,2.85285285285285E-03,2.7027027027027E-03,2.55255255255255E-03,2.4024024024024E-03,2.25225225225225E-03,2.1021021021021E-03,1.95195195195195E-03,1.8018018018018E-03,1.65165165165165E-03,1.5015015015015E-03,1.35135135135135E-03,1.2012012012012E-03,1.05105105105105E-03,9.009009009009E-04,7.5075075075075E-04,6.006006006006E-04,4.5045045045045E-04,3.003003003003E-04,1.5015015015015E-04,}
		valueWeights[dataIndex["default_attack_rate"]] ={15,25,50,3.33333333333333,1.66666666666667,4.34782608695652E-03,4.2512077294686E-03,4.15458937198068E-03,4.05797101449275E-03,3.96135265700483E-03,3.86473429951691E-03,3.76811594202899E-03,3.67149758454106E-03,3.57487922705314E-03,3.47826086956522E-03,3.3816425120773E-03,3.28502415458937E-03,3.18840579710145E-03,3.09178743961353E-03,2.9951690821256E-03,2.89855072463768E-03,2.80193236714976E-03,2.70531400966184E-03,2.60869565217391E-03,2.51207729468599E-03,2.41545893719807E-03,2.31884057971015E-03,2.22222222222222E-03,2.1256038647343E-03,2.02898550724638E-03,1.93236714975845E-03,1.83574879227053E-03,1.73913043478261E-03,1.64251207729469E-03,1.54589371980676E-03,1.44927536231884E-03,1.35265700483092E-03,.001256038647343,1.15942028985507E-03,1.06280193236715E-03,9.66183574879228E-04,8.69565217391305E-04,7.72946859903382E-04,6.76328502415459E-04,5.79710144927537E-04,4.83091787439614E-04,3.86473429951691E-04,2.89855072463769E-04,1.93236714975846E-04,9.66183574879235E-05,}
		valueWeights[dataIndex["default_damage_low"]] ={1.66666666666667,1.66666666666667,1.66666666666667,2.5,2.5,2.5,2.5,2.5,2.5,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,6.89655172413793E-03,6.65024630541872E-03,6.40394088669951E-03,6.1576354679803E-03,5.91133004926108E-03,5.66502463054187E-03,5.41871921182266E-03,5.17241379310345E-03,4.92610837438424E-03,4.67980295566502E-03,4.43349753694581E-03,4.1871921182266E-03,3.94088669950739E-03,3.69458128078818E-03,3.44827586206897E-03,3.20197044334975E-03,2.95566502463054E-03,2.70935960591133E-03,2.46305418719212E-03,2.21674876847291E-03,1.97044334975369E-03,1.72413793103448E-03,1.47783251231527E-03,1.23152709359606E-03,9.85221674876848E-04,7.38916256157635E-04,4.92610837438423E-04,2.46305418719212E-04,}
		valueWeights[dataIndex["default_damage_high"]] ={1.66666666666667,1.66666666666667,1.66666666666667,2.5,2.5,2.5,2.5,2.5,2.5,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,6.89655172413793E-03,6.65024630541872E-03,6.40394088669951E-03,6.1576354679803E-03,5.91133004926108E-03,5.66502463054187E-03,5.41871921182266E-03,5.17241379310345E-03,4.92610837438424E-03,4.67980295566502E-03,4.43349753694581E-03,4.1871921182266E-03,3.94088669950739E-03,3.69458128078818E-03,3.44827586206897E-03,3.20197044334975E-03,2.95566502463054E-03,2.70935960591133E-03,2.46305418719212E-03,2.21674876847291E-03,1.97044334975369E-03,1.72413793103448E-03,1.47783251231527E-03,1.23152709359606E-03,9.85221674876848E-04,7.38916256157635E-04,4.92610837438423E-04,2.46305418719212E-04,}
		valueWeights[dataIndex["default_dodge"]] ={15,75,3.33333333333333,1.66666666666667,4.25531914893617E-03,4.16281221091582E-03,4.07030527289547E-03,3.97779833487512E-03,3.88529139685476E-03,3.79278445883441E-03,3.70027752081406E-03,3.60777058279371E-03,3.51526364477336E-03,3.42275670675301E-03,3.33024976873266E-03,3.2377428307123E-03,3.14523589269195E-03,3.0527289546716E-03,2.96022201665125E-03,2.8677150786309E-03,2.77520814061055E-03,2.68270120259019E-03,2.59019426456984E-03,2.49768732654949E-03,2.40518038852914E-03,2.31267345050879E-03,2.22016651248844E-03,2.12765957446809E-03,2.03515263644773E-03,1.94264569842738E-03,1.85013876040703E-03,1.75763182238668E-03,1.66512488436633E-03,1.57261794634598E-03,1.48011100832562E-03,1.38760407030527E-03,1.29509713228492E-03,1.20259019426457E-03,1.11008325624422E-03,1.01757631822387E-03,9.25069380203515E-04,8.32562442183164E-04,7.40055504162812E-04,6.47548566142461E-04,5.55041628122109E-04,4.62534690101757E-04,3.70027752081406E-04,2.77520814061055E-04,1.85013876040703E-04,9.25069380203515E-05,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3,3,3,3,3,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,2,1.5,1,.5,6.45161290322581E-03,6.23655913978495E-03,6.02150537634409E-03,5.80645161290323E-03,5.59139784946237E-03,5.37634408602151E-03,5.16129032258065E-03,4.94623655913978E-03,4.73118279569892E-03,4.51612903225806E-03,4.3010752688172E-03,4.08602150537634E-03,3.87096774193548E-03,3.65591397849462E-03,3.44086021505376E-03,3.2258064516129E-03,3.01075268817204E-03,2.79569892473118E-03,2.58064516129032E-03,2.36559139784946E-03,2.1505376344086E-03,1.93548387096774E-03,1.72043010752688E-03,1.50537634408602E-03,1.29032258064516E-03,1.0752688172043E-03,8.60215053763441E-04,6.45161290322581E-04,4.3010752688172E-04,2.1505376344086E-04,}
		valueWeights[dataIndex["default_resis_sleep"]] ={2.5,2.5,3.75,3.75,3.75,3.75,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,2.5,1.66666666666667,.833333333333333,5.55555555555556E-03,5.3968253968254E-03,5.23809523809524E-03,5.07936507936508E-03,4.92063492063492E-03,4.76190476190476E-03,4.6031746031746E-03,4.44444444444444E-03,4.28571428571429E-03,4.12698412698413E-03,3.96825396825397E-03,3.80952380952381E-03,3.65079365079365E-03,3.49206349206349E-03,3.33333333333333E-03,3.17460317460317E-03,3.01587301587302E-03,2.85714285714286E-03,2.6984126984127E-03,2.53968253968254E-03,2.38095238095238E-03,2.22222222222222E-03,2.06349206349206E-03,1.9047619047619E-03,1.74603174603175E-03,1.58730158730159E-03,1.42857142857143E-03,1.26984126984127E-03,1.11111111111111E-03,9.52380952380953E-04,7.93650793650793E-04,6.34920634920635E-04,4.76190476190476E-04,3.17460317460318E-04,1.58730158730159E-04,}
		valueWeights[dataIndex["default_resis_silence"]] ={1.25,1.25,1.25,1.25,2.5,2.5,2.5,2.5,2.5,2.5,1.36363636363636,2.72727272727273,4.09090909090909,5.45454545454545,6.81818181818182,8.18181818181818,9.54545454545455,10.9090909090909,12.2727272727273,13.6363636363636,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["default_resis_weakness"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3,3,3,3,3,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,2,1.5,1,.5,6.45161290322581E-03,6.23655913978495E-03,6.02150537634409E-03,5.80645161290323E-03,5.59139784946237E-03,5.37634408602151E-03,5.16129032258065E-03,4.94623655913978E-03,4.73118279569892E-03,4.51612903225806E-03,4.3010752688172E-03,4.08602150537634E-03,3.87096774193548E-03,3.65591397849462E-03,3.44086021505376E-03,3.2258064516129E-03,3.01075268817204E-03,2.79569892473118E-03,2.58064516129032E-03,2.36559139784946E-03,2.1505376344086E-03,1.93548387096774E-03,1.72043010752688E-03,1.50537634408602E-03,1.29032258064516E-03,1.0752688172043E-03,8.60215053763441E-04,6.45161290322581E-04,4.3010752688172E-04,2.1505376344086E-04,}
		valueWeights[dataIndex["default_resis_enlace"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3,3,3,3,3,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,7.14285714285714E-03,6.87830687830688E-03,6.61375661375661E-03,6.34920634920635E-03,6.08465608465609E-03,5.82010582010582E-03,5.55555555555556E-03,5.29100529100529E-03,5.02645502645503E-03,4.76190476190476E-03,4.4973544973545E-03,4.23280423280423E-03,3.96825396825397E-03,3.7037037037037E-03,3.43915343915344E-03,3.17460317460317E-03,2.91005291005291E-03,2.64550264550265E-03,2.38095238095238E-03,2.11640211640212E-03,1.85185185185185E-03,1.58730158730159E-03,1.32275132275132E-03,1.05820105820106E-03,7.93650793650794E-04,5.29100529100529E-04,2.64550264550265E-04,}
		
		valueWeights[dataIndex["grow_hp"]] ={5,5,5,5,7.5,15,22.5,30,3.33333333333333,1.66666666666667,4.87804878048781E-03,4.75609756097561E-03,4.63414634146342E-03,4.51219512195122E-03,4.39024390243902E-03,4.26829268292683E-03,4.14634146341464E-03,4.02439024390244E-03,3.90243902439024E-03,3.78048780487805E-03,3.65853658536585E-03,3.53658536585366E-03,3.41463414634146E-03,3.29268292682927E-03,3.17073170731707E-03,3.04878048780488E-03,2.92682926829268E-03,2.80487804878049E-03,2.68292682926829E-03,2.5609756097561E-03,2.4390243902439E-03,2.31707317073171E-03,2.19512195121951E-03,2.07317073170732E-03,1.95121951219512E-03,1.82926829268293E-03,1.70731707317073E-03,1.58536585365854E-03,1.46341463414634E-03,1.34146341463415E-03,1.21951219512195E-03,1.09756097560976E-03,9.75609756097561E-04,8.53658536585367E-04,7.31707317073171E-04,6.09756097560976E-04,4.87804878048781E-04,3.65853658536586E-04,2.4390243902439E-04,1.21951219512195E-04,}
		valueWeights[dataIndex["grow_mp"]] ={2.5,2.5,5,5,5,5,10,15,20,25,2,1.5,1,.5,5.40540540540541E-03,5.25525525525526E-03,5.10510510510511E-03,4.95495495495496E-03,4.80480480480481E-03,4.65465465465465E-03,4.5045045045045E-03,4.35435435435435E-03,4.2042042042042E-03,4.05405405405405E-03,3.9039039039039E-03,3.75375375375375E-03,3.6036036036036E-03,3.45345345345345E-03,3.3033033033033E-03,3.15315315315315E-03,.003003003003003,2.85285285285285E-03,2.7027027027027E-03,2.55255255255255E-03,2.4024024024024E-03,2.25225225225225E-03,2.1021021021021E-03,1.95195195195195E-03,1.8018018018018E-03,1.65165165165165E-03,1.5015015015015E-03,1.35135135135135E-03,1.2012012012012E-03,1.05105105105105E-03,9.009009009009E-04,7.5075075075075E-04,6.006006006006E-04,4.5045045045045E-04,3.003003003003E-04,1.5015015015015E-04,}
		valueWeights[dataIndex["grow_attack_rate"]] ={15,25,50,3.33333333333333,1.66666666666667,4.34782608695652E-03,4.2512077294686E-03,4.15458937198068E-03,4.05797101449275E-03,3.96135265700483E-03,3.86473429951691E-03,3.76811594202899E-03,3.67149758454106E-03,3.57487922705314E-03,3.47826086956522E-03,3.3816425120773E-03,3.28502415458937E-03,3.18840579710145E-03,3.09178743961353E-03,2.9951690821256E-03,2.89855072463768E-03,2.80193236714976E-03,2.70531400966184E-03,2.60869565217391E-03,2.51207729468599E-03,2.41545893719807E-03,2.31884057971015E-03,2.22222222222222E-03,2.1256038647343E-03,2.02898550724638E-03,1.93236714975845E-03,1.83574879227053E-03,1.73913043478261E-03,1.64251207729469E-03,1.54589371980676E-03,1.44927536231884E-03,1.35265700483092E-03,.001256038647343,1.15942028985507E-03,1.06280193236715E-03,9.66183574879228E-04,8.69565217391305E-04,7.72946859903382E-04,6.76328502415459E-04,5.79710144927537E-04,4.83091787439614E-04,3.86473429951691E-04,2.89855072463769E-04,1.93236714975846E-04,9.66183574879235E-05,}
		valueWeights[dataIndex["grow_damage_low"]] ={1.66666666666667,1.66666666666667,1.66666666666667,2.5,2.5,2.5,2.5,2.5,2.5,1.66666666666667,3.33333333333333,5,6.66666666666667,8.33333333333333,10,11.6666666666667,13.3333333333333,15,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.14285714285714E-03,6.87830687830688E-03,6.61375661375661E-03,6.34920634920635E-03,6.08465608465609E-03,5.82010582010582E-03,5.55555555555556E-03,5.29100529100529E-03,5.02645502645503E-03,4.76190476190476E-03,4.4973544973545E-03,4.23280423280423E-03,3.96825396825397E-03,3.7037037037037E-03,3.43915343915344E-03,3.17460317460317E-03,2.91005291005291E-03,2.64550264550265E-03,2.38095238095238E-03,2.11640211640212E-03,1.85185185185185E-03,1.58730158730159E-03,1.32275132275132E-03,1.05820105820106E-03,7.93650793650794E-04,5.29100529100529E-04,2.64550264550265E-04,}
		valueWeights[dataIndex["grow_damage_high"]] ={1.66666666666667,1.66666666666667,1.66666666666667,2.5,2.5,2.5,2.5,2.5,2.5,1.66666666666667,3.33333333333333,5,6.66666666666667,8.33333333333333,10,11.6666666666667,13.3333333333333,15,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.14285714285714E-03,6.87830687830688E-03,6.61375661375661E-03,6.34920634920635E-03,6.08465608465609E-03,5.82010582010582E-03,5.55555555555556E-03,5.29100529100529E-03,5.02645502645503E-03,4.76190476190476E-03,4.4973544973545E-03,4.23280423280423E-03,3.96825396825397E-03,3.7037037037037E-03,3.43915343915344E-03,3.17460317460317E-03,2.91005291005291E-03,2.64550264550265E-03,2.38095238095238E-03,2.11640211640212E-03,1.85185185185185E-03,1.58730158730159E-03,1.32275132275132E-03,1.05820105820106E-03,7.93650793650794E-04,5.29100529100529E-04,2.64550264550265E-04,}
		valueWeights[dataIndex["grow_dodge"]] ={15,75,3.33333333333333,1.66666666666667,4.25531914893617E-03,4.16281221091582E-03,4.07030527289547E-03,3.97779833487512E-03,3.88529139685476E-03,3.79278445883441E-03,3.70027752081406E-03,3.60777058279371E-03,3.51526364477336E-03,3.42275670675301E-03,3.33024976873266E-03,3.2377428307123E-03,3.14523589269195E-03,3.0527289546716E-03,2.96022201665125E-03,2.8677150786309E-03,2.77520814061055E-03,2.68270120259019E-03,2.59019426456984E-03,2.49768732654949E-03,2.40518038852914E-03,2.31267345050879E-03,2.22016651248844E-03,2.12765957446809E-03,2.03515263644773E-03,1.94264569842738E-03,1.85013876040703E-03,1.75763182238668E-03,1.66512488436633E-03,1.57261794634598E-03,1.48011100832562E-03,1.38760407030527E-03,1.29509713228492E-03,1.20259019426457E-03,1.11008325624422E-03,1.01757631822387E-03,9.25069380203515E-04,8.32562442183164E-04,7.40055504162812E-04,6.47548566142461E-04,5.55041628122109E-04,4.62534690101757E-04,3.70027752081406E-04,2.77520814061055E-04,1.85013876040703E-04,9.25069380203515E-05,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={3.33333333333333,3.33333333333333,3.33333333333333,7,7,7,7,7,1.38888888888889,2.77777777777778,4.16666666666667,5.55555555555556,6.94444444444444,8.33333333333333,9.72222222222222,11.1111111111111,2,1.5,1,.5,6.45161290322581E-03,6.23655913978495E-03,6.02150537634409E-03,5.80645161290323E-03,5.59139784946237E-03,5.37634408602151E-03,5.16129032258065E-03,4.94623655913978E-03,4.73118279569892E-03,4.51612903225806E-03,4.3010752688172E-03,4.08602150537634E-03,3.87096774193548E-03,3.65591397849462E-03,3.44086021505376E-03,3.2258064516129E-03,3.01075268817204E-03,2.79569892473118E-03,2.58064516129032E-03,2.36559139784946E-03,2.1505376344086E-03,1.93548387096774E-03,1.72043010752688E-03,1.50537634408602E-03,1.29032258064516E-03,1.0752688172043E-03,8.60215053763441E-04,6.45161290322581E-04,4.3010752688172E-04,2.1505376344086E-04,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={5,5,8.75,8.75,8.75,8.75,2.38095238095238,4.76190476190476,7.14285714285714,9.52380952380952,11.9047619047619,14.2857142857143,2.5,1.66666666666667,.833333333333333,5.55555555555556E-03,5.3968253968254E-03,5.23809523809524E-03,5.07936507936508E-03,4.92063492063492E-03,4.76190476190476E-03,4.6031746031746E-03,4.44444444444444E-03,4.28571428571429E-03,4.12698412698413E-03,3.96825396825397E-03,3.80952380952381E-03,3.65079365079365E-03,3.49206349206349E-03,3.33333333333333E-03,3.17460317460317E-03,3.01587301587302E-03,2.85714285714286E-03,2.6984126984127E-03,2.53968253968254E-03,2.38095238095238E-03,2.22222222222222E-03,2.06349206349206E-03,1.9047619047619E-03,1.74603174603175E-03,1.58730158730159E-03,1.42857142857143E-03,1.26984126984127E-03,1.11111111111111E-03,9.52380952380953E-04,7.93650793650793E-04,6.34920634920635E-04,4.76190476190476E-04,3.17460317460318E-04,1.58730158730159E-04,}
		valueWeights[dataIndex["grow_resis_silence"]] ={2.5,2.5,2.5,2.5,5.83333333333333,5.83333333333333,5.83333333333333,5.83333333333333,5.83333333333333,5.83333333333333,.909090909090909,1.81818181818182,2.72727272727273,3.63636363636364,4.54545454545455,5.45454545454545,6.36363636363636,7.27272727272727,8.18181818181818,9.09090909090909,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={3.33333333333333,3.33333333333333,3.33333333333333,7,7,7,7,7,1.38888888888889,2.77777777777778,4.16666666666667,5.55555555555556,6.94444444444444,8.33333333333333,9.72222222222222,11.1111111111111,2,1.5,1,.5,6.45161290322581E-03,6.23655913978495E-03,6.02150537634409E-03,5.80645161290323E-03,5.59139784946237E-03,5.37634408602151E-03,5.16129032258065E-03,4.94623655913978E-03,4.73118279569892E-03,4.51612903225806E-03,4.3010752688172E-03,4.08602150537634E-03,3.87096774193548E-03,3.65591397849462E-03,3.44086021505376E-03,3.2258064516129E-03,3.01075268817204E-03,2.79569892473118E-03,2.58064516129032E-03,2.36559139784946E-03,2.1505376344086E-03,1.93548387096774E-03,1.72043010752688E-03,1.50537634408602E-03,1.29032258064516E-03,1.0752688172043E-03,8.60215053763441E-04,6.45161290322581E-04,4.3010752688172E-04,2.1505376344086E-04,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={3.33333333333333,3.33333333333333,3.33333333333333,7,7,7,7,7,1.38888888888889,2.77777777777778,4.16666666666667,5.55555555555556,6.94444444444444,8.33333333333333,9.72222222222222,11.1111111111111,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,7.14285714285714E-03,6.87830687830688E-03,6.61375661375661E-03,6.34920634920635E-03,6.08465608465609E-03,5.82010582010582E-03,5.55555555555556E-03,5.29100529100529E-03,5.02645502645503E-03,4.76190476190476E-03,4.4973544973545E-03,4.23280423280423E-03,3.96825396825397E-03,3.7037037037037E-03,3.43915343915344E-03,3.17460317460317E-03,2.91005291005291E-03,2.64550264550265E-03,2.38095238095238E-03,2.11640211640212E-03,1.85185185185185E-03,1.58730158730159E-03,1.32275132275132E-03,1.05820105820106E-03,7.93650793650794E-04,5.29100529100529E-04,2.64550264550265E-04,}

	---ËÄÁéá¦ Ñª·ãÇ¹
	elseif tmplID == talismans["GW_75_SiLingFan"].tmplID or tmplID == talismans["JL_75_XueFengQiang"].tmplID or tmplID == talismans["QJ_75_luohouyu"].tmplID then
		
		valueWeights[dataIndex["default_hp"]] ={.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,25,50,.909090909090909,.818181818181818,.727272727272727,.636363636363636,.545454545454546,.454545454545455,.363636363636364,.272727272727273,.181818181818182,9.09090909090909E-02,8.69565217391304E-03,.008300395256917,7.90513833992095E-03,7.5098814229249E-03,7.11462450592885E-03,6.71936758893281E-03,6.32411067193676E-03,5.92885375494071E-03,5.53359683794466E-03,5.13833992094862E-03,4.74308300395257E-03,4.34782608695652E-03,3.95256916996047E-03,3.55731225296443E-03,3.16205533596838E-03,2.76679841897233E-03,2.37154150197628E-03,1.97628458498024E-03,1.58102766798419E-03,1.18577075098814E-03,7.90513833992096E-04,3.95256916996047E-04,}
		valueWeights[dataIndex["default_mp"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3.75,3.75,3.75,3.75,75,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,5.26315789473684E-03,5.12091038406828E-03,4.97866287339972E-03,4.83641536273115E-03,4.69416785206259E-03,4.55192034139403E-03,4.40967283072546E-03,4.2674253200569E-03,4.12517780938834E-03,3.98293029871977E-03,3.84068278805121E-03,3.69843527738265E-03,3.55618776671408E-03,3.41394025604552E-03,3.27169274537696E-03,3.12944523470839E-03,2.98719772403983E-03,2.84495021337127E-03,2.7027027027027E-03,2.56045519203414E-03,2.41820768136558E-03,2.27596017069701E-03,2.13371266002845E-03,1.99146514935989E-03,1.84921763869132E-03,1.70697012802276E-03,1.5647226173542E-03,1.42247510668563E-03,1.28022759601707E-03,1.13798008534851E-03,9.95732574679943E-04,8.53485064011381E-04,7.11237553342817E-04,5.68990042674253E-04,4.2674253200569E-04,2.84495021337127E-04,1.42247510668564E-04,}
		valueWeights[dataIndex["default_attack_rate"]] ={5,5,5,5,25,50,3.33333333333333,1.66666666666667,4.65116279069767E-03,4.54042081949059E-03,4.4296788482835E-03,4.31893687707641E-03,4.20819490586932E-03,4.09745293466224E-03,3.98671096345515E-03,3.87596899224806E-03,3.76522702104097E-03,3.65448504983389E-03,3.5437430786268E-03,3.43300110741971E-03,3.32225913621262E-03,3.21151716500554E-03,3.10077519379845E-03,2.99003322259136E-03,2.87929125138427E-03,2.76854928017719E-03,2.6578073089701E-03,2.54706533776301E-03,2.43632336655592E-03,2.32558139534884E-03,2.21483942414175E-03,2.10409745293466E-03,1.99335548172757E-03,1.88261351052049E-03,1.7718715393134E-03,1.66112956810631E-03,1.55038759689922E-03,1.43964562569214E-03,1.32890365448505E-03,1.21816168327796E-03,1.10741971207087E-03,9.96677740863787E-04,8.859357696567E-04,7.75193798449612E-04,6.64451827242524E-04,5.53709856035437E-04,4.42967884828349E-04,3.32225913621262E-04,2.21483942414174E-04,1.10741971207087E-04,}
		valueWeights[dataIndex["default_damage_low"]] ={.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,1.875,1.875,1.875,1.875,1.875,1.875,1.875,1.875,25,50,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["default_damage_high"]] ={.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,1.875,1.875,1.875,1.875,1.875,1.875,1.875,1.875,25,50,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["default_dodge"]] ={5,7.5,7.5,25,50,3.33333333333333,1.66666666666667,4.54545454545455E-03,4.43974630021142E-03,4.33403805496829E-03,4.22832980972516E-03,4.12262156448203E-03,4.0169133192389E-03,3.91120507399577E-03,3.80549682875264E-03,3.69978858350951E-03,3.59408033826639E-03,3.48837209302326E-03,3.38266384778013E-03,.003276955602537,3.17124735729387E-03,3.06553911205074E-03,2.95983086680761E-03,2.85412262156448E-03,2.74841437632135E-03,2.64270613107822E-03,2.5369978858351E-03,2.43128964059197E-03,2.32558139534884E-03,2.21987315010571E-03,2.11416490486258E-03,2.00845665961945E-03,1.90274841437632E-03,1.79704016913319E-03,1.69133192389006E-03,1.58562367864694E-03,1.47991543340381E-03,1.37420718816068E-03,1.26849894291755E-03,1.16279069767442E-03,1.05708245243129E-03,9.51374207188161E-04,8.45665961945032E-04,7.39957716701903E-04,6.34249471458774E-04,5.28541226215646E-04,4.22832980972516E-04,3.17124735729387E-04,2.11416490486258E-04,1.05708245243129E-04,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}
		valueWeights[dataIndex["default_resis_sleep"]] ={.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}
		valueWeights[dataIndex["default_resis_silence"]] ={.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,7.5,15,22.5,30,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,.008,7.66666666666667E-03,7.33333333333333E-03,.007,6.66666666666667E-03,6.33333333333333E-03,.006,5.66666666666667E-03,5.33333333333333E-03,.005,4.66666666666667E-03,4.33333333333333E-03,.004,3.66666666666667E-03,3.33333333333333E-03,.003,2.66666666666667E-03,2.33333333333333E-03,.002,1.66666666666667E-03,1.33333333333333E-03,9.99999999999999E-04,6.66666666666666E-04,3.33333333333332E-04,}
		valueWeights[dataIndex["default_resis_weakness"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["default_resis_enlace"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		
		valueWeights[dataIndex["grow_hp"]] ={.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,25,50,.909090909090909,.818181818181818,.727272727272727,.636363636363636,.545454545454546,.454545454545455,.363636363636364,.272727272727273,.181818181818182,9.09090909090909E-02,8.69565217391304E-03,.008300395256917,7.90513833992095E-03,7.5098814229249E-03,7.11462450592885E-03,6.71936758893281E-03,6.32411067193676E-03,5.92885375494071E-03,5.53359683794466E-03,5.13833992094862E-03,4.74308300395257E-03,4.34782608695652E-03,3.95256916996047E-03,3.55731225296443E-03,3.16205533596838E-03,2.76679841897233E-03,2.37154150197628E-03,1.97628458498024E-03,1.58102766798419E-03,1.18577075098814E-03,7.90513833992096E-04,3.95256916996047E-04,}
		valueWeights[dataIndex["grow_mp"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3.75,3.75,3.75,3.75,75,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,5.26315789473684E-03,5.12091038406828E-03,4.97866287339972E-03,4.83641536273115E-03,4.69416785206259E-03,4.55192034139403E-03,4.40967283072546E-03,4.2674253200569E-03,4.12517780938834E-03,3.98293029871977E-03,3.84068278805121E-03,3.69843527738265E-03,3.55618776671408E-03,3.41394025604552E-03,3.27169274537696E-03,3.12944523470839E-03,2.98719772403983E-03,2.84495021337127E-03,2.7027027027027E-03,2.56045519203414E-03,2.41820768136558E-03,2.27596017069701E-03,2.13371266002845E-03,1.99146514935989E-03,1.84921763869132E-03,1.70697012802276E-03,1.5647226173542E-03,1.42247510668563E-03,1.28022759601707E-03,1.13798008534851E-03,9.95732574679943E-04,8.53485064011381E-04,7.11237553342817E-04,5.68990042674253E-04,4.2674253200569E-04,2.84495021337127E-04,1.42247510668564E-04,}
		valueWeights[dataIndex["grow_attack_rate"]] ={5,5,5,5,25,50,3.33333333333333,1.66666666666667,4.65116279069767E-03,4.54042081949059E-03,4.4296788482835E-03,4.31893687707641E-03,4.20819490586932E-03,4.09745293466224E-03,3.98671096345515E-03,3.87596899224806E-03,3.76522702104097E-03,3.65448504983389E-03,3.5437430786268E-03,3.43300110741971E-03,3.32225913621262E-03,3.21151716500554E-03,3.10077519379845E-03,2.99003322259136E-03,2.87929125138427E-03,2.76854928017719E-03,2.6578073089701E-03,2.54706533776301E-03,2.43632336655592E-03,2.32558139534884E-03,2.21483942414175E-03,2.10409745293466E-03,1.99335548172757E-03,1.88261351052049E-03,1.7718715393134E-03,1.66112956810631E-03,1.55038759689922E-03,1.43964562569214E-03,1.32890365448505E-03,1.21816168327796E-03,1.10741971207087E-03,9.96677740863787E-04,8.859357696567E-04,7.75193798449612E-04,6.64451827242524E-04,5.53709856035437E-04,4.42967884828349E-04,3.32225913621262E-04,2.21483942414174E-04,1.10741971207087E-04,}
		valueWeights[dataIndex["grow_damage_low"]] ={.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,1.875,1.875,1.875,1.875,1.875,1.875,1.875,1.875,25,50,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["grow_damage_high"]] ={.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,1.875,1.875,1.875,1.875,1.875,1.875,1.875,1.875,25,50,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["grow_dodge"]] ={5,7.5,7.5,25,50,3.33333333333333,1.66666666666667,4.54545454545455E-03,4.43974630021142E-03,4.33403805496829E-03,4.22832980972516E-03,4.12262156448203E-03,4.0169133192389E-03,3.91120507399577E-03,3.80549682875264E-03,3.69978858350951E-03,3.59408033826639E-03,3.48837209302326E-03,3.38266384778013E-03,.003276955602537,3.17124735729387E-03,3.06553911205074E-03,2.95983086680761E-03,2.85412262156448E-03,2.74841437632135E-03,2.64270613107822E-03,2.5369978858351E-03,2.43128964059197E-03,2.32558139534884E-03,2.21987315010571E-03,2.11416490486258E-03,2.00845665961945E-03,1.90274841437632E-03,1.79704016913319E-03,1.69133192389006E-03,1.58562367864694E-03,1.47991543340381E-03,1.37420718816068E-03,1.26849894291755E-03,1.16279069767442E-03,1.05708245243129E-03,9.51374207188161E-04,8.45665961945032E-04,7.39957716701903E-04,6.34249471458774E-04,5.28541226215646E-04,4.22832980972516E-04,3.17124735729387E-04,2.11416490486258E-04,1.05708245243129E-04,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}
		valueWeights[dataIndex["grow_resis_silence"]] ={.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,7.5,15,22.5,30,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,.008,7.66666666666667E-03,7.33333333333333E-03,.007,6.66666666666667E-03,6.33333333333333E-03,.006,5.66666666666667E-03,5.33333333333333E-03,.005,4.66666666666667E-03,4.33333333333333E-03,.004,3.66666666666667E-03,3.33333333333333E-03,.003,2.66666666666667E-03,2.33333333333333E-03,.002,1.66666666666667E-03,1.33333333333333E-03,9.99999999999999E-04,6.66666666666666E-04,3.33333333333332E-04,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}

	---¶àÇé»· ÆÆ¾ü
	elseif tmplID == talismans["HH_75_DuoQingHuan"].tmplID or tmplID == talismans["HG_75_PoJun"].tmplID or tmplID == talismans["YZ_75_poming"].tmplID then

		valueWeights[dataIndex["default_hp"]] ={.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,1.875,1.875,1.875,1.875,1.875,1.875,1.875,1.875,25,50,1,.888888888888889,.777777777777778,.666666666666667,.555555555555556,.444444444444444,.333333333333333,.222222222222222,.111111111111111,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["default_mp"]] ={1.66666666666667,1.66666666666667,1.66666666666667,5,5,5,25,50,1.42857142857143,1.19047619047619,.952380952380952,.714285714285714,.476190476190476,.238095238095238,5.40540540540541E-03,5.25525525525526E-03,5.10510510510511E-03,4.95495495495496E-03,4.80480480480481E-03,4.65465465465465E-03,4.5045045045045E-03,4.35435435435435E-03,4.2042042042042E-03,4.05405405405405E-03,3.9039039039039E-03,3.75375375375375E-03,3.6036036036036E-03,3.45345345345345E-03,3.3033033033033E-03,3.15315315315315E-03,.003003003003003,2.85285285285285E-03,2.7027027027027E-03,2.55255255255255E-03,2.4024024024024E-03,2.25225225225225E-03,2.1021021021021E-03,1.95195195195195E-03,1.8018018018018E-03,1.65165165165165E-03,1.5015015015015E-03,1.35135135135135E-03,1.2012012012012E-03,1.05105105105105E-03,9.009009009009E-04,7.5075075075075E-04,6.006006006006E-04,4.5045045045045E-04,3.003003003003E-04,1.5015015015015E-04,}
		valueWeights[dataIndex["default_attack_rate"]] ={.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["default_damage_low"]] ={.625,.625,.625,.625,.625,.625,.625,.625,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,12.5,25,37.5,1,.888888888888889,.777777777777778,.666666666666667,.555555555555556,.444444444444444,.333333333333333,.222222222222222,.111111111111111,9.09090909090909E-03,8.65800865800866E-03,8.22510822510823E-03,7.79220779220779E-03,7.35930735930736E-03,6.92640692640693E-03,6.49350649350649E-03,6.06060606060606E-03,5.62770562770563E-03,5.19480519480519E-03,4.76190476190476E-03,4.32900432900433E-03,3.8961038961039E-03,3.46320346320346E-03,3.03030303030303E-03,2.5974025974026E-03,2.16450216450216E-03,1.73160173160173E-03,1.2987012987013E-03,8.65800865800865E-04,4.32900432900432E-04,}
		valueWeights[dataIndex["default_damage_high"]] ={.625,.625,.625,.625,.625,.625,.625,.625,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,12.5,25,37.5,1,.888888888888889,.777777777777778,.666666666666667,.555555555555556,.444444444444444,.333333333333333,.222222222222222,.111111111111111,9.09090909090909E-03,8.65800865800866E-03,8.22510822510823E-03,7.79220779220779E-03,7.35930735930736E-03,6.92640692640693E-03,6.49350649350649E-03,6.06060606060606E-03,5.62770562770563E-03,5.19480519480519E-03,4.76190476190476E-03,4.32900432900433E-03,3.8961038961039E-03,3.46320346320346E-03,3.03030303030303E-03,2.5974025974026E-03,2.16450216450216E-03,1.73160173160173E-03,1.2987012987013E-03,8.65800865800865E-04,4.32900432900432E-04,}
		valueWeights[dataIndex["default_dodge"]] ={.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["default_resis_sleep"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["default_resis_silence"]] ={.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,7.5,15,22.5,30,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,.008,7.66666666666667E-03,7.33333333333333E-03,.007,6.66666666666667E-03,6.33333333333333E-03,.006,5.66666666666667E-03,5.33333333333333E-03,.005,4.66666666666667E-03,4.33333333333333E-03,.004,3.66666666666667E-03,3.33333333333333E-03,.003,2.66666666666667E-03,2.33333333333333E-03,.002,1.66666666666667E-03,1.33333333333333E-03,9.99999999999999E-04,6.66666666666666E-04,3.33333333333332E-04,}
		valueWeights[dataIndex["default_resis_weakness"]] ={.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}
		valueWeights[dataIndex["default_resis_enlace"]] ={.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}
		
		valueWeights[dataIndex["grow_hp"]] ={.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,1.875,1.875,1.875,1.875,1.875,1.875,1.875,1.875,25,50,1,.888888888888889,.777777777777778,.666666666666667,.555555555555556,.444444444444444,.333333333333333,.222222222222222,.111111111111111,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["grow_mp"]] ={1.66666666666667,1.66666666666667,1.66666666666667,5,5,5,25,50,1.42857142857143,1.19047619047619,.952380952380952,.714285714285714,.476190476190476,.238095238095238,5.40540540540541E-03,5.25525525525526E-03,5.10510510510511E-03,4.95495495495496E-03,4.80480480480481E-03,4.65465465465465E-03,4.5045045045045E-03,4.35435435435435E-03,4.2042042042042E-03,4.05405405405405E-03,3.9039039039039E-03,3.75375375375375E-03,3.6036036036036E-03,3.45345345345345E-03,3.3033033033033E-03,3.15315315315315E-03,.003003003003003,2.85285285285285E-03,2.7027027027027E-03,2.55255255255255E-03,2.4024024024024E-03,2.25225225225225E-03,2.1021021021021E-03,1.95195195195195E-03,1.8018018018018E-03,1.65165165165165E-03,1.5015015015015E-03,1.35135135135135E-03,1.2012012012012E-03,1.05105105105105E-03,9.009009009009E-04,7.5075075075075E-04,6.006006006006E-04,4.5045045045045E-04,3.003003003003E-04,1.5015015015015E-04,}
		valueWeights[dataIndex["grow_attack_rate"]] ={.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["grow_damage_low"]] ={.625,.625,.625,.625,.625,.625,.625,.625,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,12.5,25,37.5,1,.888888888888889,.777777777777778,.666666666666667,.555555555555556,.444444444444444,.333333333333333,.222222222222222,.111111111111111,9.09090909090909E-03,8.65800865800866E-03,8.22510822510823E-03,7.79220779220779E-03,7.35930735930736E-03,6.92640692640693E-03,6.49350649350649E-03,6.06060606060606E-03,5.62770562770563E-03,5.19480519480519E-03,4.76190476190476E-03,4.32900432900433E-03,3.8961038961039E-03,3.46320346320346E-03,3.03030303030303E-03,2.5974025974026E-03,2.16450216450216E-03,1.73160173160173E-03,1.2987012987013E-03,8.65800865800865E-04,4.32900432900432E-04,}
		valueWeights[dataIndex["grow_damage_high"]] ={.625,.625,.625,.625,.625,.625,.625,.625,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,12.5,25,37.5,1,.888888888888889,.777777777777778,.666666666666667,.555555555555556,.444444444444444,.333333333333333,.222222222222222,.111111111111111,9.09090909090909E-03,8.65800865800866E-03,8.22510822510823E-03,7.79220779220779E-03,7.35930735930736E-03,6.92640692640693E-03,6.49350649350649E-03,6.06060606060606E-03,5.62770562770563E-03,5.19480519480519E-03,4.76190476190476E-03,4.32900432900433E-03,3.8961038961039E-03,3.46320346320346E-03,3.03030303030303E-03,2.5974025974026E-03,2.16450216450216E-03,1.73160173160173E-03,1.2987012987013E-03,8.65800865800865E-04,4.32900432900432E-04,}
		valueWeights[dataIndex["grow_dodge"]] ={.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["grow_resis_silence"]] ={.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,7.5,15,22.5,30,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,.008,7.66666666666667E-03,7.33333333333333E-03,.007,6.66666666666667E-03,6.33333333333333E-03,.006,5.66666666666667E-03,5.33333333333333E-03,.005,4.66666666666667E-03,4.33333333333333E-03,.004,3.66666666666667E-03,3.33333333333333E-03,.003,2.66666666666667E-03,2.33333333333333E-03,.002,1.66666666666667E-03,1.33333333333333E-03,9.99999999999999E-04,6.66666666666666E-04,3.33333333333332E-04,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}

	---ÆßÐÇ½£ ÑÇ·ç
	elseif tmplID == talismans["QY_75_QiXingJian"].tmplID or tmplID == talismans["TH_75_YaFeng"].tmplID then
		
		valueWeights[dataIndex["default_hp"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3.75,3.75,3.75,3.75,75,2,1.5,1,.5,5.12820512820513E-03,4.99325236167341E-03,4.8582995951417E-03,4.72334682860999E-03,4.58839406207827E-03,4.45344129554656E-03,4.31848852901484E-03,4.18353576248313E-03,4.04858299595142E-03,3.9136302294197E-03,3.77867746288799E-03,3.64372469635628E-03,3.50877192982456E-03,3.37381916329285E-03,3.23886639676113E-03,3.10391363022942E-03,2.96896086369771E-03,2.83400809716599E-03,2.69905533063428E-03,2.56410256410256E-03,2.42914979757085E-03,2.29419703103914E-03,2.15924426450742E-03,2.02429149797571E-03,1.88933873144399E-03,1.75438596491228E-03,1.61943319838057E-03,1.48448043184885E-03,1.34952766531714E-03,1.21457489878542E-03,1.07962213225371E-03,9.44669365721997E-04,8.09716599190284E-04,6.7476383265857E-04,5.39811066126856E-04,4.04858299595142E-04,2.69905533063428E-04,1.34952766531714E-04,}
		valueWeights[dataIndex["default_mp"]] ={1.25,1.25,1.25,1.25,2.5,2.5,2.5,2.5,2.5,2.5,25,50,.833333333333333,.757575757575758,.681818181818182,.606060606060606,.53030303030303,.454545454545455,.378787878787879,.303030303030303,.227272727272727,.151515151515151,7.57575757575758E-02,7.14285714285714E-03,6.87830687830688E-03,6.61375661375661E-03,6.34920634920635E-03,6.08465608465609E-03,5.82010582010582E-03,5.55555555555556E-03,5.29100529100529E-03,5.02645502645503E-03,4.76190476190476E-03,4.4973544973545E-03,4.23280423280423E-03,3.96825396825397E-03,3.7037037037037E-03,3.43915343915344E-03,3.17460317460317E-03,2.91005291005291E-03,2.64550264550265E-03,2.38095238095238E-03,2.11640211640212E-03,1.85185185185185E-03,1.58730158730159E-03,1.32275132275132E-03,1.05820105820106E-03,7.93650793650794E-04,5.29100529100529E-04,2.64550264550265E-04,}
		valueWeights[dataIndex["default_attack_rate"]] ={5,5,5,5,25,50,3.33333333333333,1.66666666666667,4.65116279069767E-03,4.54042081949059E-03,4.4296788482835E-03,4.31893687707641E-03,4.20819490586932E-03,4.09745293466224E-03,3.98671096345515E-03,3.87596899224806E-03,3.76522702104097E-03,3.65448504983389E-03,3.5437430786268E-03,3.43300110741971E-03,3.32225913621262E-03,3.21151716500554E-03,3.10077519379845E-03,2.99003322259136E-03,2.87929125138427E-03,2.76854928017719E-03,2.6578073089701E-03,2.54706533776301E-03,2.43632336655592E-03,2.32558139534884E-03,2.21483942414175E-03,2.10409745293466E-03,1.99335548172757E-03,1.88261351052049E-03,1.7718715393134E-03,1.66112956810631E-03,1.55038759689922E-03,1.43964562569214E-03,1.32890365448505E-03,1.21816168327796E-03,1.10741971207087E-03,9.96677740863787E-04,8.859357696567E-04,7.75193798449612E-04,6.64451827242524E-04,5.53709856035437E-04,4.42967884828349E-04,3.32225913621262E-04,2.21483942414174E-04,1.10741971207087E-04,}
		valueWeights[dataIndex["default_damage_low"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,12.5,25,37.5,.909090909090909,.818181818181818,.727272727272727,.636363636363636,.545454545454546,.454545454545455,.363636363636364,.272727272727273,.181818181818182,9.09090909090909E-02,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["default_damage_high"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,12.5,25,37.5,.909090909090909,.818181818181818,.727272727272727,.636363636363636,.545454545454546,.454545454545455,.363636363636364,.272727272727273,.181818181818182,9.09090909090909E-02,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["default_dodge"]] ={5,7.5,7.5,25,50,3.33333333333333,1.66666666666667,4.54545454545455E-03,4.43974630021142E-03,4.33403805496829E-03,4.22832980972516E-03,4.12262156448203E-03,4.0169133192389E-03,3.91120507399577E-03,3.80549682875264E-03,3.69978858350951E-03,3.59408033826639E-03,3.48837209302326E-03,3.38266384778013E-03,.003276955602537,3.17124735729387E-03,3.06553911205074E-03,2.95983086680761E-03,2.85412262156448E-03,2.74841437632135E-03,2.64270613107822E-03,2.5369978858351E-03,2.43128964059197E-03,2.32558139534884E-03,2.21987315010571E-03,2.11416490486258E-03,2.00845665961945E-03,1.90274841437632E-03,1.79704016913319E-03,1.69133192389006E-03,1.58562367864694E-03,1.47991543340381E-03,1.37420718816068E-03,1.26849894291755E-03,1.16279069767442E-03,1.05708245243129E-03,9.51374207188161E-04,8.45665961945032E-04,7.39957716701903E-04,6.34249471458774E-04,5.28541226215646E-04,4.22832980972516E-04,3.17124735729387E-04,2.11416490486258E-04,1.05708245243129E-04,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}
		valueWeights[dataIndex["default_resis_sleep"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["default_resis_silence"]] ={.625,.625,.625,.625,.625,.625,.625,.625,1.875,1.875,1.875,1.875,1.875,1.875,1.875,1.875,25,50,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,.008,7.66666666666667E-03,7.33333333333333E-03,.007,6.66666666666667E-03,6.33333333333333E-03,.006,5.66666666666667E-03,5.33333333333333E-03,.005,4.66666666666667E-03,4.33333333333333E-03,.004,3.66666666666667E-03,3.33333333333333E-03,.003,2.66666666666667E-03,2.33333333333333E-03,.002,1.66666666666667E-03,1.33333333333333E-03,9.99999999999999E-04,6.66666666666666E-04,3.33333333333332E-04,}
		valueWeights[dataIndex["default_resis_weakness"]] ={.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}
		valueWeights[dataIndex["default_resis_enlace"]] ={.625,.625,.625,.625,.625,.625,.625,.625,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,2.67857142857143,5.35714285714286,8.03571428571428,10.7142857142857,13.3928571428571,16.0714285714286,18.75,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		
		valueWeights[dataIndex["grow_hp"]] ={1.66666666666667,1.66666666666667,1.66666666666667,3.75,3.75,3.75,3.75,75,2,1.5,1,.5,5.12820512820513E-03,4.99325236167341E-03,4.8582995951417E-03,4.72334682860999E-03,4.58839406207827E-03,4.45344129554656E-03,4.31848852901484E-03,4.18353576248313E-03,4.04858299595142E-03,3.9136302294197E-03,3.77867746288799E-03,3.64372469635628E-03,3.50877192982456E-03,3.37381916329285E-03,3.23886639676113E-03,3.10391363022942E-03,2.96896086369771E-03,2.83400809716599E-03,2.69905533063428E-03,2.56410256410256E-03,2.42914979757085E-03,2.29419703103914E-03,2.15924426450742E-03,2.02429149797571E-03,1.88933873144399E-03,1.75438596491228E-03,1.61943319838057E-03,1.48448043184885E-03,1.34952766531714E-03,1.21457489878542E-03,1.07962213225371E-03,9.44669365721997E-04,8.09716599190284E-04,6.7476383265857E-04,5.39811066126856E-04,4.04858299595142E-04,2.69905533063428E-04,1.34952766531714E-04,}
		valueWeights[dataIndex["grow_mp"]] ={1.25,1.25,1.25,1.25,2.5,2.5,2.5,2.5,2.5,2.5,25,50,.833333333333333,.757575757575758,.681818181818182,.606060606060606,.53030303030303,.454545454545455,.378787878787879,.303030303030303,.227272727272727,.151515151515151,7.57575757575758E-02,7.14285714285714E-03,6.87830687830688E-03,6.61375661375661E-03,6.34920634920635E-03,6.08465608465609E-03,5.82010582010582E-03,5.55555555555556E-03,5.29100529100529E-03,5.02645502645503E-03,4.76190476190476E-03,4.4973544973545E-03,4.23280423280423E-03,3.96825396825397E-03,3.7037037037037E-03,3.43915343915344E-03,3.17460317460317E-03,2.91005291005291E-03,2.64550264550265E-03,2.38095238095238E-03,2.11640211640212E-03,1.85185185185185E-03,1.58730158730159E-03,1.32275132275132E-03,1.05820105820106E-03,7.93650793650794E-04,5.29100529100529E-04,2.64550264550265E-04,}
		valueWeights[dataIndex["grow_attack_rate"]] ={5,5,5,5,25,50,3.33333333333333,1.66666666666667,4.65116279069767E-03,4.54042081949059E-03,4.4296788482835E-03,4.31893687707641E-03,4.20819490586932E-03,4.09745293466224E-03,3.98671096345515E-03,3.87596899224806E-03,3.76522702104097E-03,3.65448504983389E-03,3.5437430786268E-03,3.43300110741971E-03,3.32225913621262E-03,3.21151716500554E-03,3.10077519379845E-03,2.99003322259136E-03,2.87929125138427E-03,2.76854928017719E-03,2.6578073089701E-03,2.54706533776301E-03,2.43632336655592E-03,2.32558139534884E-03,2.21483942414175E-03,2.10409745293466E-03,1.99335548172757E-03,1.88261351052049E-03,1.7718715393134E-03,1.66112956810631E-03,1.55038759689922E-03,1.43964562569214E-03,1.32890365448505E-03,1.21816168327796E-03,1.10741971207087E-03,9.96677740863787E-04,8.859357696567E-04,7.75193798449612E-04,6.64451827242524E-04,5.53709856035437E-04,4.42967884828349E-04,3.32225913621262E-04,2.21483942414174E-04,1.10741971207087E-04,}
		valueWeights[dataIndex["grow_damage_low"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,12.5,25,37.5,.909090909090909,.818181818181818,.727272727272727,.636363636363636,.545454545454546,.454545454545455,.363636363636364,.272727272727273,.181818181818182,9.09090909090909E-02,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["grow_damage_high"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,12.5,25,37.5,.909090909090909,.818181818181818,.727272727272727,.636363636363636,.545454545454546,.454545454545455,.363636363636364,.272727272727273,.181818181818182,9.09090909090909E-02,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["grow_dodge"]] ={5,7.5,7.5,25,50,3.33333333333333,1.66666666666667,4.54545454545455E-03,4.43974630021142E-03,4.33403805496829E-03,4.22832980972516E-03,4.12262156448203E-03,4.0169133192389E-03,3.91120507399577E-03,3.80549682875264E-03,3.69978858350951E-03,3.59408033826639E-03,3.48837209302326E-03,3.38266384778013E-03,.003276955602537,3.17124735729387E-03,3.06553911205074E-03,2.95983086680761E-03,2.85412262156448E-03,2.74841437632135E-03,2.64270613107822E-03,2.5369978858351E-03,2.43128964059197E-03,2.32558139534884E-03,2.21987315010571E-03,2.11416490486258E-03,2.00845665961945E-03,1.90274841437632E-03,1.79704016913319E-03,1.69133192389006E-03,1.58562367864694E-03,1.47991543340381E-03,1.37420718816068E-03,1.26849894291755E-03,1.16279069767442E-03,1.05708245243129E-03,9.51374207188161E-04,8.45665961945032E-04,7.39957716701903E-04,6.34249471458774E-04,5.28541226215646E-04,4.22832980972516E-04,3.17124735729387E-04,2.11416490486258E-04,1.05708245243129E-04,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["grow_resis_silence"]] ={.625,.625,.625,.625,.625,.625,.625,.625,1.875,1.875,1.875,1.875,1.875,1.875,1.875,1.875,25,50,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,.008,7.66666666666667E-03,7.33333333333333E-03,.007,6.66666666666667E-03,6.33333333333333E-03,.006,5.66666666666667E-03,5.33333333333333E-03,.005,4.66666666666667E-03,4.33333333333333E-03,.004,3.66666666666667E-03,3.33333333333333E-03,.003,2.66666666666667E-03,2.33333333333333E-03,.002,1.66666666666667E-03,1.33333333333333E-03,9.99999999999999E-04,6.66666666666666E-04,3.33333333333332E-04,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={.625,.625,.625,.625,.625,.625,.625,.625,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,2.67857142857143,5.35714285714286,8.03571428571428,10.7142857142857,13.3928571428571,16.0714285714286,18.75,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}

	---½ð¸ÕèÆ ·µ»êÏã ö¾¹Ä
	elseif tmplID == talismans["TY_75_JinGangChu"].tmplID or tmplID == talismans["LS_75_FanHunXiang"].tmplID or tmplID == talismans["CH_75_tuogu"].tmplID then
		
		valueWeights[dataIndex["default_hp"]] ={1.25,1.25,1.25,1.25,3.75,3.75,3.75,3.75,25,50,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,5.55555555555556E-03,5.3968253968254E-03,5.23809523809524E-03,5.07936507936508E-03,4.92063492063492E-03,4.76190476190476E-03,4.6031746031746E-03,4.44444444444444E-03,4.28571428571429E-03,4.12698412698413E-03,3.96825396825397E-03,3.80952380952381E-03,3.65079365079365E-03,3.49206349206349E-03,3.33333333333333E-03,3.17460317460317E-03,3.01587301587302E-03,2.85714285714286E-03,2.6984126984127E-03,2.53968253968254E-03,2.38095238095238E-03,2.22222222222222E-03,2.06349206349206E-03,1.9047619047619E-03,1.74603174603175E-03,1.58730158730159E-03,1.42857142857143E-03,1.26984126984127E-03,1.11111111111111E-03,9.52380952380953E-04,7.93650793650793E-04,6.34920634920635E-04,4.76190476190476E-04,3.17460317460318E-04,1.58730158730159E-04,}
		valueWeights[dataIndex["default_mp"]] ={1,1,1,1,1,3,3,3,3,3,25,50,1,.888888888888889,.777777777777778,.666666666666667,.555555555555556,.444444444444444,.333333333333333,.222222222222222,.111111111111111,6.66666666666667E-03,6.4367816091954E-03,6.20689655172414E-03,5.97701149425287E-03,5.74712643678161E-03,5.51724137931035E-03,5.28735632183908E-03,5.05747126436782E-03,4.82758620689655E-03,4.59770114942529E-03,4.36781609195402E-03,4.13793103448276E-03,3.90804597701149E-03,3.67816091954023E-03,3.44827586206897E-03,3.2183908045977E-03,2.98850574712644E-03,2.75862068965517E-03,2.52873563218391E-03,2.29885057471264E-03,2.06896551724138E-03,1.83908045977012E-03,1.60919540229885E-03,1.37931034482759E-03,1.14942528735632E-03,9.19540229885057E-04,6.89655172413793E-04,4.59770114942529E-04,2.29885057471264E-04,}
		valueWeights[dataIndex["default_attack_rate"]] ={5,5,5,5,25,50,3.33333333333333,1.66666666666667,4.65116279069767E-03,4.54042081949059E-03,4.4296788482835E-03,4.31893687707641E-03,4.20819490586932E-03,4.09745293466224E-03,3.98671096345515E-03,3.87596899224806E-03,3.76522702104097E-03,3.65448504983389E-03,3.5437430786268E-03,3.43300110741971E-03,3.32225913621262E-03,3.21151716500554E-03,3.10077519379845E-03,2.99003322259136E-03,2.87929125138427E-03,2.76854928017719E-03,2.6578073089701E-03,2.54706533776301E-03,2.43632336655592E-03,2.32558139534884E-03,2.21483942414175E-03,2.10409745293466E-03,1.99335548172757E-03,1.88261351052049E-03,1.7718715393134E-03,1.66112956810631E-03,1.55038759689922E-03,1.43964562569214E-03,1.32890365448505E-03,1.21816168327796E-03,1.10741971207087E-03,9.96677740863787E-04,8.859357696567E-04,7.75193798449612E-04,6.64451827242524E-04,5.53709856035437E-04,4.42967884828349E-04,3.32225913621262E-04,2.21483942414174E-04,1.10741971207087E-04,}
		valueWeights[dataIndex["default_damage_low"]] ={.625,.625,.625,.625,.625,.625,.625,.625,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,25,50,1,.888888888888889,.777777777777778,.666666666666667,.555555555555556,.444444444444444,.333333333333333,.222222222222222,.111111111111111,8.69565217391304E-03,.008300395256917,7.90513833992095E-03,7.5098814229249E-03,7.11462450592885E-03,6.71936758893281E-03,6.32411067193676E-03,5.92885375494071E-03,5.53359683794466E-03,5.13833992094862E-03,4.74308300395257E-03,4.34782608695652E-03,3.95256916996047E-03,3.55731225296443E-03,3.16205533596838E-03,2.76679841897233E-03,2.37154150197628E-03,1.97628458498024E-03,1.58102766798419E-03,1.18577075098814E-03,7.90513833992096E-04,3.95256916996047E-04,}
		valueWeights[dataIndex["default_damage_high"]] ={.625,.625,.625,.625,.625,.625,.625,.625,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,25,50,1,.888888888888889,.777777777777778,.666666666666667,.555555555555556,.444444444444444,.333333333333333,.222222222222222,.111111111111111,8.69565217391304E-03,.008300395256917,7.90513833992095E-03,7.5098814229249E-03,7.11462450592885E-03,6.71936758893281E-03,6.32411067193676E-03,5.92885375494071E-03,5.53359683794466E-03,5.13833992094862E-03,4.74308300395257E-03,4.34782608695652E-03,3.95256916996047E-03,3.55731225296443E-03,3.16205533596838E-03,2.76679841897233E-03,2.37154150197628E-03,1.97628458498024E-03,1.58102766798419E-03,1.18577075098814E-03,7.90513833992096E-04,3.95256916996047E-04,}
		valueWeights[dataIndex["default_dodge"]] ={5,7.5,7.5,25,50,3.33333333333333,1.66666666666667,4.54545454545455E-03,4.43974630021142E-03,4.33403805496829E-03,4.22832980972516E-03,4.12262156448203E-03,4.0169133192389E-03,3.91120507399577E-03,3.80549682875264E-03,3.69978858350951E-03,3.59408033826639E-03,3.48837209302326E-03,3.38266384778013E-03,.003276955602537,3.17124735729387E-03,3.06553911205074E-03,2.95983086680761E-03,2.85412262156448E-03,2.74841437632135E-03,2.64270613107822E-03,2.5369978858351E-03,2.43128964059197E-03,2.32558139534884E-03,2.21987315010571E-03,2.11416490486258E-03,2.00845665961945E-03,1.90274841437632E-03,1.79704016913319E-03,1.69133192389006E-03,1.58562367864694E-03,1.47991543340381E-03,1.37420718816068E-03,1.26849894291755E-03,1.16279069767442E-03,1.05708245243129E-03,9.51374207188161E-04,8.45665961945032E-04,7.39957716701903E-04,6.34249471458774E-04,5.28541226215646E-04,4.22832980972516E-04,3.17124735729387E-04,2.11416490486258E-04,1.05708245243129E-04,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["default_resis_sleep"]] ={.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,7.5,15,22.5,30,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,.008,7.66666666666667E-03,7.33333333333333E-03,.007,6.66666666666667E-03,6.33333333333333E-03,.006,5.66666666666667E-03,5.33333333333333E-03,.005,4.66666666666667E-03,4.33333333333333E-03,.004,3.66666666666667E-03,3.33333333333333E-03,.003,2.66666666666667E-03,2.33333333333333E-03,.002,1.66666666666667E-03,1.33333333333333E-03,9.99999999999999E-04,6.66666666666666E-04,3.33333333333332E-04,}
		valueWeights[dataIndex["default_resis_silence"]] ={.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}
		valueWeights[dataIndex["default_resis_weakness"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["default_resis_enlace"]] ={.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}
		
		valueWeights[dataIndex["grow_hp"]] ={1.25,1.25,1.25,1.25,3.75,3.75,3.75,3.75,25,50,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,5.55555555555556E-03,5.3968253968254E-03,5.23809523809524E-03,5.07936507936508E-03,4.92063492063492E-03,4.76190476190476E-03,4.6031746031746E-03,4.44444444444444E-03,4.28571428571429E-03,4.12698412698413E-03,3.96825396825397E-03,3.80952380952381E-03,3.65079365079365E-03,3.49206349206349E-03,3.33333333333333E-03,3.17460317460317E-03,3.01587301587302E-03,2.85714285714286E-03,2.6984126984127E-03,2.53968253968254E-03,2.38095238095238E-03,2.22222222222222E-03,2.06349206349206E-03,1.9047619047619E-03,1.74603174603175E-03,1.58730158730159E-03,1.42857142857143E-03,1.26984126984127E-03,1.11111111111111E-03,9.52380952380953E-04,7.93650793650793E-04,6.34920634920635E-04,4.76190476190476E-04,3.17460317460318E-04,1.58730158730159E-04,}
		valueWeights[dataIndex["grow_mp"]] ={1,1,1,1,1,3,3,3,3,3,25,50,1,.888888888888889,.777777777777778,.666666666666667,.555555555555556,.444444444444444,.333333333333333,.222222222222222,.111111111111111,6.66666666666667E-03,6.4367816091954E-03,6.20689655172414E-03,5.97701149425287E-03,5.74712643678161E-03,5.51724137931035E-03,5.28735632183908E-03,5.05747126436782E-03,4.82758620689655E-03,4.59770114942529E-03,4.36781609195402E-03,4.13793103448276E-03,3.90804597701149E-03,3.67816091954023E-03,3.44827586206897E-03,3.2183908045977E-03,2.98850574712644E-03,2.75862068965517E-03,2.52873563218391E-03,2.29885057471264E-03,2.06896551724138E-03,1.83908045977012E-03,1.60919540229885E-03,1.37931034482759E-03,1.14942528735632E-03,9.19540229885057E-04,6.89655172413793E-04,4.59770114942529E-04,2.29885057471264E-04,}
		valueWeights[dataIndex["grow_attack_rate"]] ={5,5,5,5,25,50,3.33333333333333,1.66666666666667,4.65116279069767E-03,4.54042081949059E-03,4.4296788482835E-03,4.31893687707641E-03,4.20819490586932E-03,4.09745293466224E-03,3.98671096345515E-03,3.87596899224806E-03,3.76522702104097E-03,3.65448504983389E-03,3.5437430786268E-03,3.43300110741971E-03,3.32225913621262E-03,3.21151716500554E-03,3.10077519379845E-03,2.99003322259136E-03,2.87929125138427E-03,2.76854928017719E-03,2.6578073089701E-03,2.54706533776301E-03,2.43632336655592E-03,2.32558139534884E-03,2.21483942414175E-03,2.10409745293466E-03,1.99335548172757E-03,1.88261351052049E-03,1.7718715393134E-03,1.66112956810631E-03,1.55038759689922E-03,1.43964562569214E-03,1.32890365448505E-03,1.21816168327796E-03,1.10741971207087E-03,9.96677740863787E-04,8.859357696567E-04,7.75193798449612E-04,6.64451827242524E-04,5.53709856035437E-04,4.42967884828349E-04,3.32225913621262E-04,2.21483942414174E-04,1.10741971207087E-04,}
		valueWeights[dataIndex["grow_damage_low"]] ={.625,.625,.625,.625,.625,.625,.625,.625,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,25,50,1,.888888888888889,.777777777777778,.666666666666667,.555555555555556,.444444444444444,.333333333333333,.222222222222222,.111111111111111,8.69565217391304E-03,.008300395256917,7.90513833992095E-03,7.5098814229249E-03,7.11462450592885E-03,6.71936758893281E-03,6.32411067193676E-03,5.92885375494071E-03,5.53359683794466E-03,5.13833992094862E-03,4.74308300395257E-03,4.34782608695652E-03,3.95256916996047E-03,3.55731225296443E-03,3.16205533596838E-03,2.76679841897233E-03,2.37154150197628E-03,1.97628458498024E-03,1.58102766798419E-03,1.18577075098814E-03,7.90513833992096E-04,3.95256916996047E-04,}
		valueWeights[dataIndex["grow_damage_high"]] ={.625,.625,.625,.625,.625,.625,.625,.625,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,25,50,1,.888888888888889,.777777777777778,.666666666666667,.555555555555556,.444444444444444,.333333333333333,.222222222222222,.111111111111111,8.69565217391304E-03,.008300395256917,7.90513833992095E-03,7.5098814229249E-03,7.11462450592885E-03,6.71936758893281E-03,6.32411067193676E-03,5.92885375494071E-03,5.53359683794466E-03,5.13833992094862E-03,4.74308300395257E-03,4.34782608695652E-03,3.95256916996047E-03,3.55731225296443E-03,3.16205533596838E-03,2.76679841897233E-03,2.37154150197628E-03,1.97628458498024E-03,1.58102766798419E-03,1.18577075098814E-03,7.90513833992096E-04,3.95256916996047E-04,}
		valueWeights[dataIndex["grow_dodge"]] ={5,7.5,7.5,25,50,3.33333333333333,1.66666666666667,4.54545454545455E-03,4.43974630021142E-03,4.33403805496829E-03,4.22832980972516E-03,4.12262156448203E-03,4.0169133192389E-03,3.91120507399577E-03,3.80549682875264E-03,3.69978858350951E-03,3.59408033826639E-03,3.48837209302326E-03,3.38266384778013E-03,.003276955602537,3.17124735729387E-03,3.06553911205074E-03,2.95983086680761E-03,2.85412262156448E-03,2.74841437632135E-03,2.64270613107822E-03,2.5369978858351E-03,2.43128964059197E-03,2.32558139534884E-03,2.21987315010571E-03,2.11416490486258E-03,2.00845665961945E-03,1.90274841437632E-03,1.79704016913319E-03,1.69133192389006E-03,1.58562367864694E-03,1.47991543340381E-03,1.37420718816068E-03,1.26849894291755E-03,1.16279069767442E-03,1.05708245243129E-03,9.51374207188161E-04,8.45665961945032E-04,7.39957716701903E-04,6.34249471458774E-04,5.28541226215646E-04,4.22832980972516E-04,3.17124735729387E-04,2.11416490486258E-04,1.05708245243129E-04,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,7.5,15,22.5,30,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,.008,7.66666666666667E-03,7.33333333333333E-03,.007,6.66666666666667E-03,6.33333333333333E-03,.006,5.66666666666667E-03,5.33333333333333E-03,.005,4.66666666666667E-03,4.33333333333333E-03,.004,3.66666666666667E-03,3.33333333333333E-03,.003,2.66666666666667E-03,2.33333333333333E-03,.002,1.66666666666667E-03,1.33333333333333E-03,9.99999999999999E-04,6.66666666666666E-04,3.33333333333332E-04,}
		valueWeights[dataIndex["grow_resis_silence"]] ={.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}

---ÆÆÉ²
	elseif tmplID == talismans["GD_75_PoSha"].tmplID or tmplID == talismans["FX_75_HengTianChi"].tmplID or tmplID == talismans["TH_75_ChunQiuSuo"].tmplID then
		
		valueWeights[dataIndex["default_hp"]] ={1.25,1.25,1.25,1.25,3.75,3.75,3.75,3.75,25,50,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,5.55555555555556E-03,5.3968253968254E-03,5.23809523809524E-03,5.07936507936508E-03,4.92063492063492E-03,4.76190476190476E-03,4.6031746031746E-03,4.44444444444444E-03,4.28571428571429E-03,4.12698412698413E-03,3.96825396825397E-03,3.80952380952381E-03,3.65079365079365E-03,3.49206349206349E-03,3.33333333333333E-03,3.17460317460317E-03,3.01587301587302E-03,2.85714285714286E-03,2.6984126984127E-03,2.53968253968254E-03,2.38095238095238E-03,2.22222222222222E-03,2.06349206349206E-03,1.9047619047619E-03,1.74603174603175E-03,1.58730158730159E-03,1.42857142857143E-03,1.26984126984127E-03,1.11111111111111E-03,9.52380952380953E-04,7.93650793650793E-04,6.34920634920635E-04,4.76190476190476E-04,3.17460317460318E-04,1.58730158730159E-04,}
		valueWeights[dataIndex["default_mp"]] ={1,1,1,1,1,3,3,3,3,3,25,50,1,.888888888888889,.777777777777778,.666666666666667,.555555555555556,.444444444444444,.333333333333333,.222222222222222,.111111111111111,6.66666666666667E-03,6.4367816091954E-03,6.20689655172414E-03,5.97701149425287E-03,5.74712643678161E-03,5.51724137931035E-03,5.28735632183908E-03,5.05747126436782E-03,4.82758620689655E-03,4.59770114942529E-03,4.36781609195402E-03,4.13793103448276E-03,3.90804597701149E-03,3.67816091954023E-03,3.44827586206897E-03,3.2183908045977E-03,2.98850574712644E-03,2.75862068965517E-03,2.52873563218391E-03,2.29885057471264E-03,2.06896551724138E-03,1.83908045977012E-03,1.60919540229885E-03,1.37931034482759E-03,1.14942528735632E-03,9.19540229885057E-04,6.89655172413793E-04,4.59770114942529E-04,2.29885057471264E-04,}
		valueWeights[dataIndex["default_attack_rate"]] ={5,5,5,5,25,50,3.33333333333333,1.66666666666667,4.65116279069767E-03,4.54042081949059E-03,4.4296788482835E-03,4.31893687707641E-03,4.20819490586932E-03,4.09745293466224E-03,3.98671096345515E-03,3.87596899224806E-03,3.76522702104097E-03,3.65448504983389E-03,3.5437430786268E-03,3.43300110741971E-03,3.32225913621262E-03,3.21151716500554E-03,3.10077519379845E-03,2.99003322259136E-03,2.87929125138427E-03,2.76854928017719E-03,2.6578073089701E-03,2.54706533776301E-03,2.43632336655592E-03,2.32558139534884E-03,2.21483942414175E-03,2.10409745293466E-03,1.99335548172757E-03,1.88261351052049E-03,1.7718715393134E-03,1.66112956810631E-03,1.55038759689922E-03,1.43964562569214E-03,1.32890365448505E-03,1.21816168327796E-03,1.10741971207087E-03,9.96677740863787E-04,8.859357696567E-04,7.75193798449612E-04,6.64451827242524E-04,5.53709856035437E-04,4.42967884828349E-04,3.32225913621262E-04,2.21483942414174E-04,1.10741971207087E-04,}
		valueWeights[dataIndex["default_damage_low"]] ={.625,.625,.625,.625,.625,.625,.625,.625,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,25,50,1,.888888888888889,.777777777777778,.666666666666667,.555555555555556,.444444444444444,.333333333333333,.222222222222222,.111111111111111,8.69565217391304E-03,.008300395256917,7.90513833992095E-03,7.5098814229249E-03,7.11462450592885E-03,6.71936758893281E-03,6.32411067193676E-03,5.92885375494071E-03,5.53359683794466E-03,5.13833992094862E-03,4.74308300395257E-03,4.34782608695652E-03,3.95256916996047E-03,3.55731225296443E-03,3.16205533596838E-03,2.76679841897233E-03,2.37154150197628E-03,1.97628458498024E-03,1.58102766798419E-03,1.18577075098814E-03,7.90513833992096E-04,3.95256916996047E-04,}
		valueWeights[dataIndex["default_damage_high"]] ={.625,.625,.625,.625,.625,.625,.625,.625,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,25,50,1,.888888888888889,.777777777777778,.666666666666667,.555555555555556,.444444444444444,.333333333333333,.222222222222222,.111111111111111,8.69565217391304E-03,.008300395256917,7.90513833992095E-03,7.5098814229249E-03,7.11462450592885E-03,6.71936758893281E-03,6.32411067193676E-03,5.92885375494071E-03,5.53359683794466E-03,5.13833992094862E-03,4.74308300395257E-03,4.34782608695652E-03,3.95256916996047E-03,3.55731225296443E-03,3.16205533596838E-03,2.76679841897233E-03,2.37154150197628E-03,1.97628458498024E-03,1.58102766798419E-03,1.18577075098814E-03,7.90513833992096E-04,3.95256916996047E-04,}
		valueWeights[dataIndex["default_dodge"]] ={5,7.5,7.5,25,50,3.33333333333333,1.66666666666667,4.54545454545455E-03,4.43974630021142E-03,4.33403805496829E-03,4.22832980972516E-03,4.12262156448203E-03,4.0169133192389E-03,3.91120507399577E-03,3.80549682875264E-03,3.69978858350951E-03,3.59408033826639E-03,3.48837209302326E-03,3.38266384778013E-03,.003276955602537,3.17124735729387E-03,3.06553911205074E-03,2.95983086680761E-03,2.85412262156448E-03,2.74841437632135E-03,2.64270613107822E-03,2.5369978858351E-03,2.43128964059197E-03,2.32558139534884E-03,2.21987315010571E-03,2.11416490486258E-03,2.00845665961945E-03,1.90274841437632E-03,1.79704016913319E-03,1.69133192389006E-03,1.58562367864694E-03,1.47991543340381E-03,1.37420718816068E-03,1.26849894291755E-03,1.16279069767442E-03,1.05708245243129E-03,9.51374207188161E-04,8.45665961945032E-04,7.39957716701903E-04,6.34249471458774E-04,5.28541226215646E-04,4.22832980972516E-04,3.17124735729387E-04,2.11416490486258E-04,1.05708245243129E-04,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["default_resis_sleep"]] ={.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,7.5,15,22.5,30,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,.008,7.66666666666667E-03,7.33333333333333E-03,.007,6.66666666666667E-03,6.33333333333333E-03,.006,5.66666666666667E-03,5.33333333333333E-03,.005,4.66666666666667E-03,4.33333333333333E-03,.004,3.66666666666667E-03,3.33333333333333E-03,.003,2.66666666666667E-03,2.33333333333333E-03,.002,1.66666666666667E-03,1.33333333333333E-03,9.99999999999999E-04,6.66666666666666E-04,3.33333333333332E-04,}
		valueWeights[dataIndex["default_resis_silence"]] ={.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}
		valueWeights[dataIndex["default_resis_weakness"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["default_resis_enlace"]] ={.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}
		
		valueWeights[dataIndex["grow_hp"]] ={1.25,1.25,1.25,1.25,3.75,3.75,3.75,3.75,25,50,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,5.55555555555556E-03,5.3968253968254E-03,5.23809523809524E-03,5.07936507936508E-03,4.92063492063492E-03,4.76190476190476E-03,4.6031746031746E-03,4.44444444444444E-03,4.28571428571429E-03,4.12698412698413E-03,3.96825396825397E-03,3.80952380952381E-03,3.65079365079365E-03,3.49206349206349E-03,3.33333333333333E-03,3.17460317460317E-03,3.01587301587302E-03,2.85714285714286E-03,2.6984126984127E-03,2.53968253968254E-03,2.38095238095238E-03,2.22222222222222E-03,2.06349206349206E-03,1.9047619047619E-03,1.74603174603175E-03,1.58730158730159E-03,1.42857142857143E-03,1.26984126984127E-03,1.11111111111111E-03,9.52380952380953E-04,7.93650793650793E-04,6.34920634920635E-04,4.76190476190476E-04,3.17460317460318E-04,1.58730158730159E-04,}
		valueWeights[dataIndex["grow_mp"]] ={1,1,1,1,1,3,3,3,3,3,25,50,1,.888888888888889,.777777777777778,.666666666666667,.555555555555556,.444444444444444,.333333333333333,.222222222222222,.111111111111111,6.66666666666667E-03,6.4367816091954E-03,6.20689655172414E-03,5.97701149425287E-03,5.74712643678161E-03,5.51724137931035E-03,5.28735632183908E-03,5.05747126436782E-03,4.82758620689655E-03,4.59770114942529E-03,4.36781609195402E-03,4.13793103448276E-03,3.90804597701149E-03,3.67816091954023E-03,3.44827586206897E-03,3.2183908045977E-03,2.98850574712644E-03,2.75862068965517E-03,2.52873563218391E-03,2.29885057471264E-03,2.06896551724138E-03,1.83908045977012E-03,1.60919540229885E-03,1.37931034482759E-03,1.14942528735632E-03,9.19540229885057E-04,6.89655172413793E-04,4.59770114942529E-04,2.29885057471264E-04,}
		valueWeights[dataIndex["grow_attack_rate"]] ={5,5,5,5,25,50,3.33333333333333,1.66666666666667,4.65116279069767E-03,4.54042081949059E-03,4.4296788482835E-03,4.31893687707641E-03,4.20819490586932E-03,4.09745293466224E-03,3.98671096345515E-03,3.87596899224806E-03,3.76522702104097E-03,3.65448504983389E-03,3.5437430786268E-03,3.43300110741971E-03,3.32225913621262E-03,3.21151716500554E-03,3.10077519379845E-03,2.99003322259136E-03,2.87929125138427E-03,2.76854928017719E-03,2.6578073089701E-03,2.54706533776301E-03,2.43632336655592E-03,2.32558139534884E-03,2.21483942414175E-03,2.10409745293466E-03,1.99335548172757E-03,1.88261351052049E-03,1.7718715393134E-03,1.66112956810631E-03,1.55038759689922E-03,1.43964562569214E-03,1.32890365448505E-03,1.21816168327796E-03,1.10741971207087E-03,9.96677740863787E-04,8.859357696567E-04,7.75193798449612E-04,6.64451827242524E-04,5.53709856035437E-04,4.42967884828349E-04,3.32225913621262E-04,2.21483942414174E-04,1.10741971207087E-04,}
		valueWeights[dataIndex["grow_damage_low"]] ={.625,.625,.625,.625,.625,.625,.625,.625,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,25,50,1,.888888888888889,.777777777777778,.666666666666667,.555555555555556,.444444444444444,.333333333333333,.222222222222222,.111111111111111,8.69565217391304E-03,.008300395256917,7.90513833992095E-03,7.5098814229249E-03,7.11462450592885E-03,6.71936758893281E-03,6.32411067193676E-03,5.92885375494071E-03,5.53359683794466E-03,5.13833992094862E-03,4.74308300395257E-03,4.34782608695652E-03,3.95256916996047E-03,3.55731225296443E-03,3.16205533596838E-03,2.76679841897233E-03,2.37154150197628E-03,1.97628458498024E-03,1.58102766798419E-03,1.18577075098814E-03,7.90513833992096E-04,3.95256916996047E-04,}
		valueWeights[dataIndex["grow_damage_high"]] ={.625,.625,.625,.625,.625,.625,.625,.625,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,25,50,1,.888888888888889,.777777777777778,.666666666666667,.555555555555556,.444444444444444,.333333333333333,.222222222222222,.111111111111111,8.69565217391304E-03,.008300395256917,7.90513833992095E-03,7.5098814229249E-03,7.11462450592885E-03,6.71936758893281E-03,6.32411067193676E-03,5.92885375494071E-03,5.53359683794466E-03,5.13833992094862E-03,4.74308300395257E-03,4.34782608695652E-03,3.95256916996047E-03,3.55731225296443E-03,3.16205533596838E-03,2.76679841897233E-03,2.37154150197628E-03,1.97628458498024E-03,1.58102766798419E-03,1.18577075098814E-03,7.90513833992096E-04,3.95256916996047E-04,}
		valueWeights[dataIndex["grow_dodge"]] ={5,7.5,7.5,25,50,3.33333333333333,1.66666666666667,4.54545454545455E-03,4.43974630021142E-03,4.33403805496829E-03,4.22832980972516E-03,4.12262156448203E-03,4.0169133192389E-03,3.91120507399577E-03,3.80549682875264E-03,3.69978858350951E-03,3.59408033826639E-03,3.48837209302326E-03,3.38266384778013E-03,.003276955602537,3.17124735729387E-03,3.06553911205074E-03,2.95983086680761E-03,2.85412262156448E-03,2.74841437632135E-03,2.64270613107822E-03,2.5369978858351E-03,2.43128964059197E-03,2.32558139534884E-03,2.21987315010571E-03,2.11416490486258E-03,2.00845665961945E-03,1.90274841437632E-03,1.79704016913319E-03,1.69133192389006E-03,1.58562367864694E-03,1.47991543340381E-03,1.37420718816068E-03,1.26849894291755E-03,1.16279069767442E-03,1.05708245243129E-03,9.51374207188161E-04,8.45665961945032E-04,7.39957716701903E-04,6.34249471458774E-04,5.28541226215646E-04,4.22832980972516E-04,3.17124735729387E-04,2.11416490486258E-04,1.05708245243129E-04,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,7.5,15,22.5,30,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,.008,7.66666666666667E-03,7.33333333333333E-03,.007,6.66666666666667E-03,6.33333333333333E-03,.006,5.66666666666667E-03,5.33333333333333E-03,.005,4.66666666666667E-03,4.33333333333333E-03,.004,3.66666666666667E-03,3.33333333333333E-03,.003,2.66666666666667E-03,2.33333333333333E-03,.002,1.66666666666667E-03,1.33333333333333E-03,9.99999999999999E-04,6.66666666666666E-04,3.33333333333332E-04,}
		valueWeights[dataIndex["grow_resis_silence"]] ={.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,.416666666666667,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,.454545454545455,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}

	---done,2007-10-06
	---É½ºÓÉÈ	Í¨ÓÃ 07¹úÇì³äÖµ¶Ò½±ËùÐè£¬±ØÐëÖÁÉÙÊÇÏÉÆ·
	elseif (tmplID == talismans["US_75_ShanHeShan"].tmplID and origin == talismanOrigin["lottery"])
	---Íò¶¾¶¦	Í¨ÓÃ 07¹úÇì³äÖµ¶Ò½±ËùÐè£¬±ØÐëÖÁÉÙÊÇÏÉÆ·
	or (tmplID == talismans["US_75_WanDuDing"].tmplID and origin == talismanOrigin["lottery"]) then
		
		valueWeights[dataIndex["default_hp"]] ={0,0,0,0,0,0,0,0,0,0,0,11,10,.1,.09,.08,.07,.06,.05,3.99999999999999E-02,2.99999999999999E-02,1.99999999999999E-02,9.99999999999991E-03,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_mp"]] ={10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_attack_rate"]] ={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,10,.1,.09,.08,.07,.06,.05,3.99999999999999E-02,2.99999999999999E-02,1.99999999999999E-02,9.99999999999991E-03,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,10,.1,.09,.08,.07,.06,.05,3.99999999999999E-02,2.99999999999999E-02,1.99999999999999E-02,9.99999999999991E-03,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_dodge"]] ={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,10,.1,.09,.08,.07,.06,.05,3.99999999999999E-02,2.99999999999999E-02,1.99999999999999E-02,9.99999999999991E-03,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,10,.1,.09,.08,.07,.06,.05,3.99999999999999E-02,2.99999999999999E-02,1.99999999999999E-02,9.99999999999991E-03,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,10,.1,.09,.08,.07,.06,.05,3.99999999999999E-02,2.99999999999999E-02,1.99999999999999E-02,9.99999999999991E-03,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,10,.1,.09,.08,.07,.06,.05,3.99999999999999E-02,2.99999999999999E-02,1.99999999999999E-02,9.99999999999991E-03,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,10,.1,.09,.08,.07,.06,.05,3.99999999999999E-02,2.99999999999999E-02,1.99999999999999E-02,9.99999999999991E-03,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		
		valueWeights[dataIndex["grow_hp"]] ={0,0,0,0,0,0,0,0,10,10,.1,.06,.02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]] ={10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_attack_rate"]] ={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,10,10,.1,.06,.02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,10,10,.1,.06,.02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]] ={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,10,.1,.08,.06,.04,.02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,10,.1,.08,.06,.04,.02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,10,.1,.08,.06,.04,.02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,10,.1,.08,.06,.04,.02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,10,.1,.08,.06,.04,.02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}

	---ÁáççËþ	Í¨ÓÃ 2007Äê11ÔÂ27ÈÕ¸üÐÂ£¬22ÈÕ³äÖµ¶Ò½±ËùÐè½±Æ·£¬±ØÐëÖÁÉÙÊÇÏÉÆ·
	elseif tmplID == talismans["US_75_LingLongTa"].tmplID then

		valueWeights[dataIndex["default_hp"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,.1,.09,.08,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_mp"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_attack_rate"]] ={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,.1,.09,.08,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,.1,.09,.08,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_dodge"]] ={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,10,.1,.09,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,10,.1,.09,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,10,.1,.09,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,10,.1,.09,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,10,.1,.09,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		
		valueWeights[dataIndex["grow_hp"]] ={0,0,0,0,0,0,0,0,0,0,0,10,10,.1,.06,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,10,.1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_attack_rate"]] ={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,1,10,10,.1,.06,.02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,1,10,10,.1,.06,.02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]] ={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,11,1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,11,1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,11,1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,11,1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,11,1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}

	---É½ºÓÉÈ	Í¨ÓÃ
	elseif tmplID == talismans["US_75_ShanHeShan"].tmplID 
	---Íò¶¾¶¦	Í¨ÓÃ
	or tmplID == talismans["US_75_WanDuDing"].tmplID
	---Óñ¾»Æ¿	Í¨ÓÃ
	or tmplID == talismans["US_75_YuJingPing"].tmplID then
		
		valueWeights[dataIndex["default_hp"]] ={2.5,2.5,7.5,7.5,7.5,15,22.5,30,2,1.5,1,.5,5.12820512820513E-03,4.99325236167341E-03,4.8582995951417E-03,4.72334682860999E-03,4.58839406207827E-03,4.45344129554656E-03,4.31848852901484E-03,4.18353576248313E-03,4.04858299595142E-03,3.9136302294197E-03,3.77867746288799E-03,3.64372469635628E-03,3.50877192982456E-03,3.37381916329285E-03,3.23886639676113E-03,3.10391363022942E-03,2.96896086369771E-03,2.83400809716599E-03,2.69905533063428E-03,2.56410256410256E-03,2.42914979757085E-03,2.29419703103914E-03,2.15924426450742E-03,2.02429149797571E-03,1.88933873144399E-03,1.75438596491228E-03,1.61943319838057E-03,1.48448043184885E-03,1.34952766531714E-03,1.21457489878542E-03,1.07962213225371E-03,9.44669365721997E-04,8.09716599190284E-04,6.7476383265857E-04,5.39811066126856E-04,4.04858299595142E-04,2.69905533063428E-04,1.34952766531714E-04,}
		valueWeights[dataIndex["default_mp"]] ={2.5,2.5,7.5,7.5,7.5,15,22.5,30,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,5.26315789473684E-03,5.12091038406828E-03,4.97866287339972E-03,4.83641536273115E-03,4.69416785206259E-03,4.55192034139403E-03,4.40967283072546E-03,4.2674253200569E-03,4.12517780938834E-03,3.98293029871977E-03,3.84068278805121E-03,3.69843527738265E-03,3.55618776671408E-03,3.41394025604552E-03,3.27169274537696E-03,3.12944523470839E-03,2.98719772403983E-03,2.84495021337127E-03,2.7027027027027E-03,2.56045519203414E-03,2.41820768136558E-03,2.27596017069701E-03,2.13371266002845E-03,1.99146514935989E-03,1.84921763869132E-03,1.70697012802276E-03,1.5647226173542E-03,1.42247510668563E-03,1.28022759601707E-03,1.13798008534851E-03,9.95732574679943E-04,8.53485064011381E-04,7.11237553342817E-04,5.68990042674253E-04,4.2674253200569E-04,2.84495021337127E-04,1.42247510668564E-04,}
		valueWeights[dataIndex["default_attack_rate"]] ={5,5,5,5,25,50,3.33333333333333,1.66666666666667,4.65116279069767E-03,4.54042081949059E-03,4.4296788482835E-03,4.31893687707641E-03,4.20819490586932E-03,4.09745293466224E-03,3.98671096345515E-03,3.87596899224806E-03,3.76522702104097E-03,3.65448504983389E-03,3.5437430786268E-03,3.43300110741971E-03,3.32225913621262E-03,3.21151716500554E-03,3.10077519379845E-03,2.99003322259136E-03,2.87929125138427E-03,2.76854928017719E-03,2.6578073089701E-03,2.54706533776301E-03,2.43632336655592E-03,2.32558139534884E-03,2.21483942414175E-03,2.10409745293466E-03,1.99335548172757E-03,1.88261351052049E-03,1.7718715393134E-03,1.66112956810631E-03,1.55038759689922E-03,1.43964562569214E-03,1.32890365448505E-03,1.21816168327796E-03,1.10741971207087E-03,9.96677740863787E-04,8.859357696567E-04,7.75193798449612E-04,6.64451827242524E-04,5.53709856035437E-04,4.42967884828349E-04,3.32225913621262E-04,2.21483942414174E-04,1.10741971207087E-04,}
		valueWeights[dataIndex["default_damage_low"]] ={1.25,1.25,1.25,1.25,3,3,3,3,3,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["default_damage_high"]] ={1.25,1.25,1.25,1.25,3,3,3,3,3,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["default_dodge"]] ={5,7.5,7.5,25,50,3.33333333333333,1.66666666666667,4.54545454545455E-03,4.43974630021142E-03,4.33403805496829E-03,4.22832980972516E-03,4.12262156448203E-03,4.0169133192389E-03,3.91120507399577E-03,3.80549682875264E-03,3.69978858350951E-03,3.59408033826639E-03,3.48837209302326E-03,3.38266384778013E-03,.003276955602537,3.17124735729387E-03,3.06553911205074E-03,2.95983086680761E-03,2.85412262156448E-03,2.74841437632135E-03,2.64270613107822E-03,2.5369978858351E-03,2.43128964059197E-03,2.32558139534884E-03,2.21987315010571E-03,2.11416490486258E-03,2.00845665961945E-03,1.90274841437632E-03,1.79704016913319E-03,1.69133192389006E-03,1.58562367864694E-03,1.47991543340381E-03,1.37420718816068E-03,1.26849894291755E-03,1.16279069767442E-03,1.05708245243129E-03,9.51374207188161E-04,8.45665961945032E-04,7.39957716701903E-04,6.34249471458774E-04,5.28541226215646E-04,4.22832980972516E-04,3.17124735729387E-04,2.11416490486258E-04,1.05708245243129E-04,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["default_resis_sleep"]] ={.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,7.5,15,22.5,30,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,.008,7.66666666666667E-03,7.33333333333333E-03,.007,6.66666666666667E-03,6.33333333333333E-03,.006,5.66666666666667E-03,5.33333333333333E-03,.005,4.66666666666667E-03,4.33333333333333E-03,.004,3.66666666666667E-03,3.33333333333333E-03,.003,2.66666666666667E-03,2.33333333333333E-03,.002,1.66666666666667E-03,1.33333333333333E-03,9.99999999999999E-04,6.66666666666666E-04,3.33333333333332E-04,}
		valueWeights[dataIndex["default_resis_silence"]] ={.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,7.5,15,22.5,30,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,.008,7.66666666666667E-03,7.33333333333333E-03,.007,6.66666666666667E-03,6.33333333333333E-03,.006,5.66666666666667E-03,5.33333333333333E-03,.005,4.66666666666667E-03,4.33333333333333E-03,.004,3.66666666666667E-03,3.33333333333333E-03,.003,2.66666666666667E-03,2.33333333333333E-03,.002,1.66666666666667E-03,1.33333333333333E-03,9.99999999999999E-04,6.66666666666666E-04,3.33333333333332E-04,}
		valueWeights[dataIndex["default_resis_weakness"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["default_resis_enlace"]] ={.625,.625,.625,.625,.625,.625,.625,.625,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,2.67857142857143,5.35714285714286,8.03571428571428,10.7142857142857,13.3928571428571,16.0714285714286,18.75,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		
		valueWeights[dataIndex["grow_hp"]] ={2.5,2.5,7.5,7.5,7.5,15,22.5,30,2,1.5,1,.5,5.12820512820513E-03,4.99325236167341E-03,4.8582995951417E-03,4.72334682860999E-03,4.58839406207827E-03,4.45344129554656E-03,4.31848852901484E-03,4.18353576248313E-03,4.04858299595142E-03,3.9136302294197E-03,3.77867746288799E-03,3.64372469635628E-03,3.50877192982456E-03,3.37381916329285E-03,3.23886639676113E-03,3.10391363022942E-03,2.96896086369771E-03,2.83400809716599E-03,2.69905533063428E-03,2.56410256410256E-03,2.42914979757085E-03,2.29419703103914E-03,2.15924426450742E-03,2.02429149797571E-03,1.88933873144399E-03,1.75438596491228E-03,1.61943319838057E-03,1.48448043184885E-03,1.34952766531714E-03,1.21457489878542E-03,1.07962213225371E-03,9.44669365721997E-04,8.09716599190284E-04,6.7476383265857E-04,5.39811066126856E-04,4.04858299595142E-04,2.69905533063428E-04,1.34952766531714E-04,}
		valueWeights[dataIndex["grow_mp"]] ={2.5,2.5,7.5,7.5,7.5,15,22.5,30,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,5.26315789473684E-03,5.12091038406828E-03,4.97866287339972E-03,4.83641536273115E-03,4.69416785206259E-03,4.55192034139403E-03,4.40967283072546E-03,4.2674253200569E-03,4.12517780938834E-03,3.98293029871977E-03,3.84068278805121E-03,3.69843527738265E-03,3.55618776671408E-03,3.41394025604552E-03,3.27169274537696E-03,3.12944523470839E-03,2.98719772403983E-03,2.84495021337127E-03,2.7027027027027E-03,2.56045519203414E-03,2.41820768136558E-03,2.27596017069701E-03,2.13371266002845E-03,1.99146514935989E-03,1.84921763869132E-03,1.70697012802276E-03,1.5647226173542E-03,1.42247510668563E-03,1.28022759601707E-03,1.13798008534851E-03,9.95732574679943E-04,8.53485064011381E-04,7.11237553342817E-04,5.68990042674253E-04,4.2674253200569E-04,2.84495021337127E-04,1.42247510668564E-04,}
		valueWeights[dataIndex["grow_attack_rate"]] ={5,5,5,5,25,50,3.33333333333333,1.66666666666667,4.65116279069767E-03,4.54042081949059E-03,4.4296788482835E-03,4.31893687707641E-03,4.20819490586932E-03,4.09745293466224E-03,3.98671096345515E-03,3.87596899224806E-03,3.76522702104097E-03,3.65448504983389E-03,3.5437430786268E-03,3.43300110741971E-03,3.32225913621262E-03,3.21151716500554E-03,3.10077519379845E-03,2.99003322259136E-03,2.87929125138427E-03,2.76854928017719E-03,2.6578073089701E-03,2.54706533776301E-03,2.43632336655592E-03,2.32558139534884E-03,2.21483942414175E-03,2.10409745293466E-03,1.99335548172757E-03,1.88261351052049E-03,1.7718715393134E-03,1.66112956810631E-03,1.55038759689922E-03,1.43964562569214E-03,1.32890365448505E-03,1.21816168327796E-03,1.10741971207087E-03,9.96677740863787E-04,8.859357696567E-04,7.75193798449612E-04,6.64451827242524E-04,5.53709856035437E-04,4.42967884828349E-04,3.32225913621262E-04,2.21483942414174E-04,1.10741971207087E-04,}
		valueWeights[dataIndex["grow_damage_low"]] ={1.25,1.25,1.25,1.25,3,3,3,3,3,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["grow_damage_high"]] ={1.25,1.25,1.25,1.25,3,3,3,3,3,2.08333333333333,4.16666666666667,6.25,8.33333333333333,10.4166666666667,12.5,14.5833333333333,16.6666666666667,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,7.69230769230769E-03,7.38461538461539E-03,7.07692307692308E-03,6.76923076923077E-03,6.46153846153846E-03,6.15384615384615E-03,5.84615384615385E-03,5.53846153846154E-03,5.23076923076923E-03,4.92307692307692E-03,4.61538461538462E-03,4.30769230769231E-03,.004,3.69230769230769E-03,3.38461538461539E-03,3.07692307692308E-03,2.76923076923077E-03,2.46153846153846E-03,2.15384615384615E-03,1.84615384615385E-03,1.53846153846154E-03,1.23076923076923E-03,9.23076923076923E-04,6.15384615384616E-04,3.07692307692307E-04,}
		valueWeights[dataIndex["grow_dodge"]] ={5,7.5,7.5,25,50,3.33333333333333,1.66666666666667,4.54545454545455E-03,4.43974630021142E-03,4.33403805496829E-03,4.22832980972516E-03,4.12262156448203E-03,4.0169133192389E-03,3.91120507399577E-03,3.80549682875264E-03,3.69978858350951E-03,3.59408033826639E-03,3.48837209302326E-03,3.38266384778013E-03,.003276955602537,3.17124735729387E-03,3.06553911205074E-03,2.95983086680761E-03,2.85412262156448E-03,2.74841437632135E-03,2.64270613107822E-03,2.5369978858351E-03,2.43128964059197E-03,2.32558139534884E-03,2.21987315010571E-03,2.11416490486258E-03,2.00845665961945E-03,1.90274841437632E-03,1.79704016913319E-03,1.69133192389006E-03,1.58562367864694E-03,1.47991543340381E-03,1.37420718816068E-03,1.26849894291755E-03,1.16279069767442E-03,1.05708245243129E-03,9.51374207188161E-04,8.45665961945032E-04,7.39957716701903E-04,6.34249471458774E-04,5.28541226215646E-04,4.22832980972516E-04,3.17124735729387E-04,2.11416490486258E-04,1.05708245243129E-04,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,7.5,15,22.5,30,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,.008,7.66666666666667E-03,7.33333333333333E-03,.007,6.66666666666667E-03,6.33333333333333E-03,.006,5.66666666666667E-03,5.33333333333333E-03,.005,4.66666666666667E-03,4.33333333333333E-03,.004,3.66666666666667E-03,3.33333333333333E-03,.003,2.66666666666667E-03,2.33333333333333E-03,.002,1.66666666666667E-03,1.33333333333333E-03,9.99999999999999E-04,6.66666666666666E-04,3.33333333333332E-04,}
		valueWeights[dataIndex["grow_resis_silence"]] ={.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,7.5,15,22.5,30,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,.008,7.66666666666667E-03,7.33333333333333E-03,.007,6.66666666666667E-03,6.33333333333333E-03,.006,5.66666666666667E-03,5.33333333333333E-03,.005,4.66666666666667E-03,4.33333333333333E-03,.004,3.66666666666667E-03,3.33333333333333E-03,.003,2.66666666666667E-03,2.33333333333333E-03,.002,1.66666666666667E-03,1.33333333333333E-03,9.99999999999999E-04,6.66666666666666E-04,3.33333333333332E-04,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,.555555555555556,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={.625,.625,.625,.625,.625,.625,.625,.625,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,2.67857142857143,5.35714285714286,8.03571428571428,10.7142857142857,13.3928571428571,16.0714285714286,18.75,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}

	---ÉñÄ¾÷»	Í¨ÓÃ
	elseif tmplID == talismans["US_45_ShenMuShai"].tmplID 
			or tmplID == talismans["US_45_ShenMuShai_WZGL"].tmplID then
		
		valueWeights[dataIndex["default_hp"]] ={0,0,0,0,0,0,0,0,0,5,10,15,2.5,1.66666666666667,.833333333333333,5.55555555555556E-03,5.3968253968254E-03,5.23809523809524E-03,5.07936507936508E-03,4.92063492063492E-03,4.76190476190476E-03,4.6031746031746E-03,4.44444444444444E-03,4.28571428571429E-03,4.12698412698413E-03,3.96825396825397E-03,3.80952380952381E-03,3.65079365079365E-03,3.49206349206349E-03,3.33333333333333E-03,3.17460317460317E-03,3.01587301587302E-03,2.85714285714286E-03,2.6984126984127E-03,2.53968253968254E-03,2.38095238095238E-03,2.22222222222222E-03,2.06349206349206E-03,1.9047619047619E-03,1.74603174603175E-03,1.58730158730159E-03,1.42857142857143E-03,1.26984126984127E-03,1.11111111111111E-03,9.52380952380953E-04,7.93650793650793E-04,6.34920634920635E-04,4.76190476190476E-04,3.17460317460318E-04,1.58730158730159E-04,}
		valueWeights[dataIndex["default_mp"]] ={0,0,0,0,0,0,0,0,5,10,15,2.5,1.66666666666667,.833333333333333,5.40540540540541E-03,5.25525525525526E-03,5.10510510510511E-03,4.95495495495496E-03,4.80480480480481E-03,4.65465465465465E-03,4.5045045045045E-03,4.35435435435435E-03,4.2042042042042E-03,4.05405405405405E-03,3.9039039039039E-03,3.75375375375375E-03,3.6036036036036E-03,3.45345345345345E-03,3.3033033033033E-03,3.15315315315315E-03,.003003003003003,2.85285285285285E-03,2.7027027027027E-03,2.55255255255255E-03,2.4024024024024E-03,2.25225225225225E-03,2.1021021021021E-03,1.95195195195195E-03,1.8018018018018E-03,1.65165165165165E-03,1.5015015015015E-03,1.35135135135135E-03,1.2012012012012E-03,1.05105105105105E-03,9.009009009009E-04,7.5075075075075E-04,6.006006006006E-04,4.5045045045045E-04,3.003003003003E-04,1.5015015015015E-04,}
		valueWeights[dataIndex["default_attack_rate"]] ={0,0,0,0,10,20,2.5,1.66666666666667,.833333333333333,4.76190476190476E-03,4.64576074332172E-03,4.52961672473868E-03,4.41347270615563E-03,4.29732868757259E-03,4.18118466898955E-03,4.0650406504065E-03,3.94889663182346E-03,3.83275261324042E-03,3.71660859465738E-03,3.60046457607433E-03,3.48432055749129E-03,3.36817653890825E-03,3.2520325203252E-03,3.13588850174216E-03,3.01974448315912E-03,2.90360046457607E-03,2.78745644599303E-03,2.67131242740999E-03,2.55516840882695E-03,2.4390243902439E-03,2.32288037166086E-03,2.20673635307782E-03,2.09059233449477E-03,1.97444831591173E-03,1.85830429732869E-03,1.74216027874565E-03,1.6260162601626E-03,1.50987224157956E-03,1.39372822299652E-03,1.27758420441347E-03,1.16144018583043E-03,1.04529616724739E-03,9.29152148664344E-04,8.13008130081301E-04,6.96864111498259E-04,5.80720092915215E-04,4.64576074332172E-04,3.48432055749129E-04,2.32288037166087E-04,1.16144018583043E-04,}
		valueWeights[dataIndex["default_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,4,6,8,10,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.008,7.66666666666667E-03,7.33333333333333E-03,.007,6.66666666666667E-03,6.33333333333333E-03,.006,5.66666666666667E-03,5.33333333333333E-03,.005,4.66666666666667E-03,4.33333333333333E-03,.004,3.66666666666667E-03,3.33333333333333E-03,.003,2.66666666666667E-03,2.33333333333333E-03,.002,1.66666666666667E-03,1.33333333333333E-03,9.99999999999999E-04,6.66666666666666E-04,3.33333333333332E-04,}
		valueWeights[dataIndex["default_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,4,6,8,10,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.008,7.66666666666667E-03,7.33333333333333E-03,.007,6.66666666666667E-03,6.33333333333333E-03,.006,5.66666666666667E-03,5.33333333333333E-03,.005,4.66666666666667E-03,4.33333333333333E-03,.004,3.66666666666667E-03,3.33333333333333E-03,.003,2.66666666666667E-03,2.33333333333333E-03,.002,1.66666666666667E-03,1.33333333333333E-03,9.99999999999999E-04,6.66666666666666E-04,3.33333333333332E-04,}
		valueWeights[dataIndex["default_dodge"]] ={0,0,0,0,10,20,3.33333333333333,1.66666666666667,4.65116279069767E-03,4.54042081949059E-03,4.4296788482835E-03,4.31893687707641E-03,4.20819490586932E-03,4.09745293466224E-03,3.98671096345515E-03,3.87596899224806E-03,3.76522702104097E-03,3.65448504983389E-03,3.5437430786268E-03,3.43300110741971E-03,3.32225913621262E-03,3.21151716500554E-03,3.10077519379845E-03,2.99003322259136E-03,2.87929125138427E-03,2.76854928017719E-03,2.6578073089701E-03,2.54706533776301E-03,2.43632336655592E-03,2.32558139534884E-03,2.21483942414175E-03,2.10409745293466E-03,1.99335548172757E-03,1.88261351052049E-03,1.7718715393134E-03,1.66112956810631E-03,1.55038759689922E-03,1.43964562569214E-03,1.32890365448505E-03,1.21816168327796E-03,1.10741971207087E-03,9.96677740863787E-04,8.859357696567E-04,7.75193798449612E-04,6.64451827242524E-04,5.53709856035437E-04,4.42967884828349E-04,3.32225913621262E-04,2.21483942414174E-04,1.10741971207087E-04,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,4,6,8,10,1.42857142857143,1.19047619047619,.952380952380952,.714285714285714,.476190476190476,.238095238095238,8.69565217391304E-03,.008300395256917,7.90513833992095E-03,7.5098814229249E-03,7.11462450592885E-03,6.71936758893281E-03,6.32411067193676E-03,5.92885375494071E-03,5.53359683794466E-03,5.13833992094862E-03,4.74308300395257E-03,4.34782608695652E-03,3.95256916996047E-03,3.55731225296443E-03,3.16205533596838E-03,2.76679841897233E-03,2.37154150197628E-03,1.97628458498024E-03,1.58102766798419E-03,1.18577075098814E-03,7.90513833992096E-04,3.95256916996047E-04,}
		valueWeights[dataIndex["default_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,6,9,12,1.42857142857143,1.19047619047619,.952380952380952,.714285714285714,.476190476190476,.238095238095238,7.40740740740741E-03,7.12250712250712E-03,6.83760683760684E-03,6.55270655270655E-03,6.26780626780627E-03,5.98290598290598E-03,5.6980056980057E-03,5.41310541310541E-03,5.12820512820513E-03,4.84330484330484E-03,4.55840455840456E-03,4.27350427350427E-03,3.98860398860399E-03,3.7037037037037E-03,3.41880341880342E-03,3.13390313390313E-03,2.84900284900285E-03,2.56410256410256E-03,2.27920227920228E-03,1.99430199430199E-03,1.70940170940171E-03,1.42450142450143E-03,1.13960113960114E-03,8.54700854700855E-04,5.6980056980057E-04,2.84900284900285E-04,}
		valueWeights[dataIndex["default_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,6,9,12,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.14285714285714E-03,6.87830687830688E-03,6.61375661375661E-03,6.34920634920635E-03,6.08465608465609E-03,5.82010582010582E-03,5.55555555555556E-03,5.29100529100529E-03,5.02645502645503E-03,4.76190476190476E-03,4.4973544973545E-03,4.23280423280423E-03,3.96825396825397E-03,3.7037037037037E-03,3.43915343915344E-03,3.17460317460317E-03,2.91005291005291E-03,2.64550264550265E-03,2.38095238095238E-03,2.11640211640212E-03,1.85185185185185E-03,1.58730158730159E-03,1.32275132275132E-03,1.05820105820106E-03,7.93650793650794E-04,5.29100529100529E-04,2.64550264550265E-04,}
		valueWeights[dataIndex["default_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,4,6,8,10,1.42857142857143,1.19047619047619,.952380952380952,.714285714285714,.476190476190476,.238095238095238,8.33333333333333E-03,7.97101449275362E-03,7.60869565217391E-03,7.2463768115942E-03,6.88405797101449E-03,6.52173913043478E-03,6.15942028985507E-03,5.79710144927536E-03,5.43478260869565E-03,5.07246376811594E-03,4.71014492753623E-03,4.34782608695652E-03,3.98550724637681E-03,3.6231884057971E-03,3.26086956521739E-03,2.89855072463768E-03,2.53623188405797E-03,2.17391304347826E-03,1.81159420289855E-03,1.44927536231884E-03,1.08695652173913E-03,7.2463768115942E-04,3.6231884057971E-04,}
		valueWeights[dataIndex["default_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,4,6,8,10,1.42857142857143,1.19047619047619,.952380952380952,.714285714285714,.476190476190476,.238095238095238,8.33333333333333E-03,7.97101449275362E-03,7.60869565217391E-03,7.2463768115942E-03,6.88405797101449E-03,6.52173913043478E-03,6.15942028985507E-03,5.79710144927536E-03,5.43478260869565E-03,5.07246376811594E-03,4.71014492753623E-03,4.34782608695652E-03,3.98550724637681E-03,3.6231884057971E-03,3.26086956521739E-03,2.89855072463768E-03,2.53623188405797E-03,2.17391304347826E-03,1.81159420289855E-03,1.44927536231884E-03,1.08695652173913E-03,7.2463768115942E-04,3.6231884057971E-04,}
		
		valueWeights[dataIndex["grow_hp"]] ={0,0,0,0,0,0,0,0,0,0,6.66666666666667,13.3333333333333,20,2.5,1.66666666666667,.833333333333333,5.71428571428571E-03,5.54621848739496E-03,5.3781512605042E-03,5.21008403361345E-03,5.04201680672269E-03,4.87394957983193E-03,4.70588235294118E-03,4.53781512605042E-03,4.36974789915966E-03,4.20168067226891E-03,4.03361344537815E-03,3.86554621848739E-03,3.69747899159664E-03,3.52941176470588E-03,3.36134453781513E-03,3.19327731092437E-03,3.02521008403361E-03,2.85714285714286E-03,2.6890756302521E-03,2.52100840336134E-03,2.35294117647059E-03,2.18487394957983E-03,2.01680672268908E-03,1.84873949579832E-03,1.68067226890756E-03,1.51260504201681E-03,1.34453781512605E-03,1.17647058823529E-03,1.00840336134454E-03,8.40336134453781E-04,6.72268907563025E-04,5.04201680672269E-04,3.36134453781512E-04,1.68067226890756E-04,}
		valueWeights[dataIndex["grow_mp"]] ={0,0,0,0,0,0,0,0,6.66666666666667,13.3333333333333,20,2.5,1.66666666666667,.833333333333333,5.40540540540541E-03,5.25525525525526E-03,5.10510510510511E-03,4.95495495495496E-03,4.80480480480481E-03,4.65465465465465E-03,4.5045045045045E-03,4.35435435435435E-03,4.2042042042042E-03,4.05405405405405E-03,3.9039039039039E-03,3.75375375375375E-03,3.6036036036036E-03,3.45345345345345E-03,3.3033033033033E-03,3.15315315315315E-03,.003003003003003,2.85285285285285E-03,2.7027027027027E-03,2.55255255255255E-03,2.4024024024024E-03,2.25225225225225E-03,2.1021021021021E-03,1.95195195195195E-03,1.8018018018018E-03,1.65165165165165E-03,1.5015015015015E-03,1.35135135135135E-03,1.2012012012012E-03,1.05105105105105E-03,9.009009009009E-04,7.5075075075075E-04,6.006006006006E-04,4.5045045045045E-04,3.003003003003E-04,1.5015015015015E-04,}
		valueWeights[dataIndex["grow_attack_rate"]] ={0,0,0,0,13.3333333333333,26.6666666666667,2.5,1.66666666666667,.833333333333333,4.76190476190476E-03,4.64576074332172E-03,4.52961672473868E-03,4.41347270615563E-03,4.29732868757259E-03,4.18118466898955E-03,4.0650406504065E-03,3.94889663182346E-03,3.83275261324042E-03,3.71660859465738E-03,3.60046457607433E-03,3.48432055749129E-03,3.36817653890825E-03,3.2520325203252E-03,3.13588850174216E-03,3.01974448315912E-03,2.90360046457607E-03,2.78745644599303E-03,2.67131242740999E-03,2.55516840882695E-03,2.4390243902439E-03,2.32288037166086E-03,2.20673635307782E-03,2.09059233449477E-03,1.97444831591173E-03,1.85830429732869E-03,1.74216027874565E-03,1.6260162601626E-03,1.50987224157956E-03,1.39372822299652E-03,1.27758420441347E-03,1.16144018583043E-03,1.04529616724739E-03,9.29152148664344E-04,8.13008130081301E-04,6.96864111498259E-04,5.80720092915215E-04,4.64576074332172E-04,3.48432055749129E-04,2.32288037166087E-04,1.16144018583043E-04,}
		valueWeights[dataIndex["grow_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2.66666666666667,5.33333333333333,8,10.6666666666667,13.3333333333333,2,1.5,1,.5,8.33333333333333E-03,7.97101449275362E-03,7.60869565217391E-03,7.2463768115942E-03,6.88405797101449E-03,6.52173913043478E-03,6.15942028985507E-03,5.79710144927536E-03,5.43478260869565E-03,5.07246376811594E-03,4.71014492753623E-03,4.34782608695652E-03,3.98550724637681E-03,3.6231884057971E-03,3.26086956521739E-03,2.89855072463768E-03,2.53623188405797E-03,2.17391304347826E-03,1.81159420289855E-03,1.44927536231884E-03,1.08695652173913E-03,7.2463768115942E-04,3.6231884057971E-04,}
		valueWeights[dataIndex["grow_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2.66666666666667,5.33333333333333,8,10.6666666666667,13.3333333333333,2,1.5,1,.5,8.33333333333333E-03,7.97101449275362E-03,7.60869565217391E-03,7.2463768115942E-03,6.88405797101449E-03,6.52173913043478E-03,6.15942028985507E-03,5.79710144927536E-03,5.43478260869565E-03,5.07246376811594E-03,4.71014492753623E-03,4.34782608695652E-03,3.98550724637681E-03,3.6231884057971E-03,3.26086956521739E-03,2.89855072463768E-03,2.53623188405797E-03,2.17391304347826E-03,1.81159420289855E-03,1.44927536231884E-03,1.08695652173913E-03,7.2463768115942E-04,3.6231884057971E-04,}
		valueWeights[dataIndex["grow_dodge"]] ={0,0,0,0,13.3333333333333,26.6666666666667,3.33333333333333,1.66666666666667,4.65116279069767E-03,4.54042081949059E-03,4.4296788482835E-03,4.31893687707641E-03,4.20819490586932E-03,4.09745293466224E-03,3.98671096345515E-03,3.87596899224806E-03,3.76522702104097E-03,3.65448504983389E-03,3.5437430786268E-03,3.43300110741971E-03,3.32225913621262E-03,3.21151716500554E-03,3.10077519379845E-03,2.99003322259136E-03,2.87929125138427E-03,2.76854928017719E-03,2.6578073089701E-03,2.54706533776301E-03,2.43632336655592E-03,2.32558139534884E-03,2.21483942414175E-03,2.10409745293466E-03,1.99335548172757E-03,1.88261351052049E-03,1.7718715393134E-03,1.66112956810631E-03,1.55038759689922E-03,1.43964562569214E-03,1.32890365448505E-03,1.21816168327796E-03,1.10741971207087E-03,9.96677740863787E-04,8.859357696567E-04,7.75193798449612E-04,6.64451827242524E-04,5.53709856035437E-04,4.42967884828349E-04,3.32225913621262E-04,2.21483942414174E-04,1.10741971207087E-04,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1.66666666666667,3.33333333333333,5,6.66666666666667,8.33333333333333,1.42857142857143,1.19047619047619,.952380952380952,.714285714285714,.476190476190476,.238095238095238,8.69565217391304E-03,.008300395256917,7.90513833992095E-03,7.5098814229249E-03,7.11462450592885E-03,6.71936758893281E-03,6.32411067193676E-03,5.92885375494071E-03,5.53359683794466E-03,5.13833992094862E-03,4.74308300395257E-03,4.34782608695652E-03,3.95256916996047E-03,3.55731225296443E-03,3.16205533596838E-03,2.76679841897233E-03,2.37154150197628E-03,1.97628458498024E-03,1.58102766798419E-03,1.18577075098814E-03,7.90513833992096E-04,3.95256916996047E-04,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,2.5,5,7.5,10,1.42857142857143,1.19047619047619,.952380952380952,.714285714285714,.476190476190476,.238095238095238,7.40740740740741E-03,7.12250712250712E-03,6.83760683760684E-03,6.55270655270655E-03,6.26780626780627E-03,5.98290598290598E-03,5.6980056980057E-03,5.41310541310541E-03,5.12820512820513E-03,4.84330484330484E-03,4.55840455840456E-03,4.27350427350427E-03,3.98860398860399E-03,3.7037037037037E-03,3.41880341880342E-03,3.13390313390313E-03,2.84900284900285E-03,2.56410256410256E-03,2.27920227920228E-03,1.99430199430199E-03,1.70940170940171E-03,1.42450142450143E-03,1.13960113960114E-03,8.54700854700855E-04,5.6980056980057E-04,2.84900284900285E-04,}
		valueWeights[dataIndex["grow_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,2.5,5,7.5,10,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,7.14285714285714E-03,6.87830687830688E-03,6.61375661375661E-03,6.34920634920635E-03,6.08465608465609E-03,5.82010582010582E-03,5.55555555555556E-03,5.29100529100529E-03,5.02645502645503E-03,4.76190476190476E-03,4.4973544973545E-03,4.23280423280423E-03,3.96825396825397E-03,3.7037037037037E-03,3.43915343915344E-03,3.17460317460317E-03,2.91005291005291E-03,2.64550264550265E-03,2.38095238095238E-03,2.11640211640212E-03,1.85185185185185E-03,1.58730158730159E-03,1.32275132275132E-03,1.05820105820106E-03,7.93650793650794E-04,5.29100529100529E-04,2.64550264550265E-04,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1.66666666666667,3.33333333333333,5,6.66666666666667,8.33333333333333,1.42857142857143,1.19047619047619,.952380952380952,.714285714285714,.476190476190476,.238095238095238,8.33333333333333E-03,7.97101449275362E-03,7.60869565217391E-03,7.2463768115942E-03,6.88405797101449E-03,6.52173913043478E-03,6.15942028985507E-03,5.79710144927536E-03,5.43478260869565E-03,5.07246376811594E-03,4.71014492753623E-03,4.34782608695652E-03,3.98550724637681E-03,3.6231884057971E-03,3.26086956521739E-03,2.89855072463768E-03,2.53623188405797E-03,2.17391304347826E-03,1.81159420289855E-03,1.44927536231884E-03,1.08695652173913E-03,7.2463768115942E-04,3.6231884057971E-04,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1.66666666666667,3.33333333333333,5,6.66666666666667,8.33333333333333,1.42857142857143,1.19047619047619,.952380952380952,.714285714285714,.476190476190476,.238095238095238,8.33333333333333E-03,7.97101449275362E-03,7.60869565217391E-03,7.2463768115942E-03,6.88405797101449E-03,6.52173913043478E-03,6.15942028985507E-03,5.79710144927536E-03,5.43478260869565E-03,5.07246376811594E-03,4.71014492753623E-03,4.34782608695652E-03,3.98550724637681E-03,3.6231884057971E-03,3.26086956521739E-03,2.89855072463768E-03,2.53623188405797E-03,2.17391304347826E-03,1.81159420289855E-03,1.44927536231884E-03,1.08695652173913E-03,7.2463768115942E-04,3.6231884057971E-04,}


	---Ç¬À¤Çà¹â½ä	î¸á°
	elseif tmplID == talismans["GW_105_QianKunQingGuangJie"].tmplID or tmplID == talismans["JL_105_GangLan"].tmplID then

		valueWeights[dataIndex["default_hp"]] ={4.76190476190476E-02,4.66666666666667E-02,4.57142857142857E-02,4.47619047619048E-02,4.38095238095238E-02,4.28571428571429E-02,4.19047619047619E-02,4.09523809523809E-02,1,1,1,1,1,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,5,10,15,20,25,2,1.5,1,.5,1.33333333333333E-02,1.23809523809524E-02,1.14285714285714E-02,1.04761904761905E-02,9.52380952380952E-03,8.57142857142857E-03,7.61904761904762E-03,6.66666666666667E-03,5.71428571428571E-03,4.76190476190476E-03,3.80952380952381E-03,2.85714285714286E-03,1.9047619047619E-03,9.52380952380951E-04,}
		valueWeights[dataIndex["default_mp"]] ={1.00806451612903E-02,9.87903225806452E-03,9.67741935483871E-03,9.4758064516129E-03,2.5,2.5,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,12.5,25,37.5,2.5,1.66666666666667,.833333333333333,.00625,6.04838709677419E-03,5.84677419354839E-03,5.64516129032258E-03,5.44354838709677E-03,5.24193548387097E-03,5.04032258064516E-03,4.83870967741936E-03,4.63709677419355E-03,4.43548387096774E-03,4.23387096774194E-03,4.03225806451613E-03,3.83064516129032E-03,3.62903225806452E-03,3.42741935483871E-03,3.2258064516129E-03,3.0241935483871E-03,2.82258064516129E-03,2.62096774193548E-03,2.41935483870968E-03,2.21774193548387E-03,2.01612903225806E-03,1.81451612903226E-03,1.61290322580645E-03,1.41129032258065E-03,1.20967741935484E-03,1.00806451612903E-03,8.06451612903226E-04,6.0483870967742E-04,4.03225806451613E-04,2.01612903225807E-04,}
		valueWeights[dataIndex["default_attack_rate"]] ={6.41025641025641E-03,6.28205128205128E-03,5,3.75,3.75,3.75,3.75,25,50,3.33333333333333,1.66666666666667,.005,4.87179487179487E-03,4.74358974358974E-03,4.61538461538462E-03,4.48717948717949E-03,4.35897435897436E-03,4.23076923076923E-03,4.1025641025641E-03,3.97435897435897E-03,3.84615384615385E-03,3.71794871794872E-03,3.58974358974359E-03,3.46153846153846E-03,3.33333333333333E-03,3.20512820512821E-03,3.07692307692308E-03,2.94871794871795E-03,2.82051282051282E-03,2.69230769230769E-03,2.56410256410256E-03,2.43589743589744E-03,2.30769230769231E-03,2.17948717948718E-03,2.05128205128205E-03,1.92307692307692E-03,1.7948717948718E-03,1.66666666666667E-03,1.53846153846154E-03,1.41025641025641E-03,1.28205128205128E-03,1.15384615384615E-03,1.02564102564103E-03,8.97435897435898E-04,7.69230769230769E-04,6.41025641025641E-04,5.12820512820513E-04,3.84615384615385E-04,2.56410256410256E-04,1.28205128205128E-04,}
		valueWeights[dataIndex["default_damage_low"]] ={2.92397660818713E-02,2.86549707602339E-02,2.80701754385965E-02,2.74853801169591E-02,2.69005847953216E-02,2.63157894736842E-02,2.57309941520468E-02,2.51461988304094E-02,1.25,1.25,1.25,1.25,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,7.5,15,22.5,30,3.33333333333333,1.66666666666667,1.05263157894737E-02,9.94152046783626E-03,9.35672514619883E-03,8.7719298245614E-03,8.18713450292398E-03,7.60233918128655E-03,7.01754385964912E-03,6.4327485380117E-03,5.84795321637427E-03,5.26315789473684E-03,4.67836257309942E-03,4.09356725146199E-03,3.50877192982456E-03,2.92397660818713E-03,2.33918128654971E-03,1.75438596491228E-03,1.16959064327485E-03,5.84795321637426E-04,}
		valueWeights[dataIndex["default_damage_high"]] ={2.92397660818713E-02,2.86549707602339E-02,2.80701754385965E-02,2.74853801169591E-02,2.69005847953216E-02,2.63157894736842E-02,2.57309941520468E-02,2.51461988304094E-02,1.25,1.25,1.25,1.25,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,7.5,15,22.5,30,3.33333333333333,1.66666666666667,1.05263157894737E-02,9.94152046783626E-03,9.35672514619883E-03,8.7719298245614E-03,8.18713450292398E-03,7.60233918128655E-03,7.01754385964912E-03,6.4327485380117E-03,5.84795321637427E-03,5.26315789473684E-03,4.67836257309942E-03,4.09356725146199E-03,3.50877192982456E-03,2.92397660818713E-03,2.33918128654971E-03,1.75438596491228E-03,1.16959064327485E-03,5.84795321637426E-04,}
		valueWeights[dataIndex["default_dodge"]] ={6.09756097560976E-03,5,3.75,3.75,3.75,3.75,25,50,3.33333333333333,1.66666666666667,4.87804878048781E-03,4.75609756097561E-03,4.63414634146342E-03,4.51219512195122E-03,4.39024390243902E-03,4.26829268292683E-03,4.14634146341464E-03,4.02439024390244E-03,3.90243902439024E-03,3.78048780487805E-03,3.65853658536585E-03,3.53658536585366E-03,3.41463414634146E-03,3.29268292682927E-03,3.17073170731707E-03,3.04878048780488E-03,2.92682926829268E-03,2.80487804878049E-03,2.68292682926829E-03,2.5609756097561E-03,2.4390243902439E-03,2.31707317073171E-03,2.19512195121951E-03,2.07317073170732E-03,1.95121951219512E-03,1.82926829268293E-03,1.70731707317073E-03,1.58536585365854E-03,1.46341463414634E-03,1.34146341463415E-03,1.21951219512195E-03,1.09756097560976E-03,9.75609756097561E-04,8.53658536585367E-04,7.31707317073171E-04,6.09756097560976E-04,4.87804878048781E-04,3.65853658536586E-04,2.4390243902439E-04,1.21951219512195E-04,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={.178571428571429,.175,.171428571428571,.167857142857143,.164285714285714,.160714285714286,.157142857142857,.153571428571429,.15,.146428571428571,.142857142857143,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,2.5,1.66666666666667,.833333333333333,.025,2.14285714285714E-02,1.78571428571429E-02,1.42857142857143E-02,1.07142857142857E-02,7.14285714285714E-03,3.57142857142857E-03,}
		valueWeights[dataIndex["default_resis_sleep"]] ={.178571428571429,.175,.171428571428571,.167857142857143,.164285714285714,.160714285714286,.157142857142857,.153571428571429,.15,.146428571428571,.142857142857143,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,2.5,1.66666666666667,.833333333333333,.025,2.14285714285714E-02,1.78571428571429E-02,1.42857142857143E-02,1.07142857142857E-02,7.14285714285714E-03,3.57142857142857E-03,}
		valueWeights[dataIndex["default_resis_silence"]] ={6.41025641025641E-02,6.28205128205128E-02,6.15384615384615E-02,6.02564102564103E-02,.058974358974359,5.76923076923077E-02,5.64102564102564E-02,5.51282051282051E-02,1.25,1.25,1.25,1.25,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.66666666666667,3.33333333333333,5,6.66666666666667,8.33333333333333,10,11.6666666666667,13.3333333333333,15,2,1.5,1,.5,1.53846153846154E-02,1.41025641025641E-02,1.28205128205128E-02,1.15384615384615E-02,1.02564102564103E-02,8.97435897435898E-03,7.69230769230769E-03,6.41025641025641E-03,5.12820512820513E-03,3.84615384615385E-03,2.56410256410257E-03,1.28205128205128E-03,}
		valueWeights[dataIndex["default_resis_weakness"]] ={.111111111111111,.108888888888889,.106666666666667,.104444444444444,.102222222222222,.1,9.77777777777778E-02,9.55555555555556E-02,9.33333333333333E-02,1,1,1,1,1,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,2.67857142857143,5.35714285714286,8.03571428571428,10.7142857142857,13.3928571428571,16.0714285714286,18.75,2,1.5,1,.5,.02,1.77777777777778E-02,1.55555555555556E-02,1.33333333333333E-02,1.11111111111111E-02,8.88888888888889E-03,6.66666666666667E-03,4.44444444444445E-03,2.22222222222222E-03,}
		valueWeights[dataIndex["default_resis_enlace"]] ={.111111111111111,.108888888888889,.106666666666667,.104444444444444,.102222222222222,.1,9.77777777777778E-02,9.55555555555556E-02,9.33333333333333E-02,1,1,1,1,1,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,2.67857142857143,5.35714285714286,8.03571428571428,10.7142857142857,13.3928571428571,16.0714285714286,18.75,2,1.5,1,.5,.02,1.77777777777778E-02,1.55555555555556E-02,1.33333333333333E-02,1.11111111111111E-02,8.88888888888889E-03,6.66666666666667E-03,4.44444444444445E-03,2.22222222222222E-03,}
		
		valueWeights[dataIndex["grow_hp"]] ={3.26797385620915E-02,3.20261437908497E-02,3.13725490196078E-02,.030718954248366,3.00653594771242E-02,2.94117647058824E-02,2.87581699346405E-02,2.81045751633987E-02,1.25,1.25,1.25,1.25,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,12.5,25,37.5,2,1.5,1,.5,1.11111111111111E-02,1.04575163398693E-02,9.80392156862745E-03,9.15032679738562E-03,8.49673202614379E-03,7.84313725490196E-03,7.18954248366013E-03,6.5359477124183E-03,5.88235294117647E-03,5.22875816993464E-03,4.57516339869281E-03,3.92156862745098E-03,3.26797385620915E-03,2.61437908496732E-03,1.96078431372549E-03,1.30718954248366E-03,6.53594771241829E-04,}
		valueWeights[dataIndex["grow_mp"]] ={8.40336134453781E-03,8.23529411764706E-03,8.0672268907563E-03,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,25,50,2.5,1.66666666666667,.833333333333333,5.71428571428571E-03,5.54621848739496E-03,5.3781512605042E-03,5.21008403361345E-03,5.04201680672269E-03,4.87394957983193E-03,4.70588235294118E-03,4.53781512605042E-03,4.36974789915966E-03,4.20168067226891E-03,4.03361344537815E-03,3.86554621848739E-03,3.69747899159664E-03,3.52941176470588E-03,3.36134453781513E-03,3.19327731092437E-03,3.02521008403361E-03,2.85714285714286E-03,2.6890756302521E-03,2.52100840336134E-03,2.35294117647059E-03,2.18487394957983E-03,2.01680672268908E-03,1.84873949579832E-03,1.68067226890756E-03,1.51260504201681E-03,1.34453781512605E-03,1.17647058823529E-03,1.00840336134454E-03,8.40336134453781E-04,6.72268907563025E-04,5.04201680672269E-04,3.36134453781512E-04,1.68067226890756E-04,}
		valueWeights[dataIndex["grow_attack_rate"]] ={6.41025641025641E-03,6.28205128205128E-03,5,3.75,3.75,3.75,3.75,25,50,3.33333333333333,1.66666666666667,.005,4.87179487179487E-03,4.74358974358974E-03,4.61538461538462E-03,4.48717948717949E-03,4.35897435897436E-03,4.23076923076923E-03,4.1025641025641E-03,3.97435897435897E-03,3.84615384615385E-03,3.71794871794872E-03,3.58974358974359E-03,3.46153846153846E-03,3.33333333333333E-03,3.20512820512821E-03,3.07692307692308E-03,2.94871794871795E-03,2.82051282051282E-03,2.69230769230769E-03,2.56410256410256E-03,2.43589743589744E-03,2.30769230769231E-03,2.17948717948718E-03,2.05128205128205E-03,1.92307692307692E-03,1.7948717948718E-03,1.66666666666667E-03,1.53846153846154E-03,1.41025641025641E-03,1.28205128205128E-03,1.15384615384615E-03,1.02564102564103E-03,8.97435897435898E-04,7.69230769230769E-04,6.41025641025641E-04,5.12820512820513E-04,3.84615384615385E-04,2.56410256410256E-04,1.28205128205128E-04,}
		valueWeights[dataIndex["grow_damage_low"]] ={1.97628458498024E-02,1.93675889328063E-02,1.89723320158103E-02,1.85770750988142E-02,1.81818181818182E-02,1.77865612648221E-02,1.73913043478261E-02,1.25,1.25,1.25,1.25,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,25,50,3.33333333333333,1.66666666666667,8.69565217391304E-03,.008300395256917,7.90513833992095E-03,7.5098814229249E-03,7.11462450592885E-03,6.71936758893281E-03,6.32411067193676E-03,5.92885375494071E-03,5.53359683794466E-03,5.13833992094862E-03,4.74308300395257E-03,4.34782608695652E-03,3.95256916996047E-03,3.55731225296443E-03,3.16205533596838E-03,2.76679841897233E-03,2.37154150197628E-03,1.97628458498024E-03,1.58102766798419E-03,1.18577075098814E-03,7.90513833992096E-04,3.95256916996047E-04,}
		valueWeights[dataIndex["grow_damage_high"]] ={1.97628458498024E-02,1.93675889328063E-02,1.89723320158103E-02,1.85770750988142E-02,1.81818181818182E-02,1.77865612648221E-02,1.73913043478261E-02,1.25,1.25,1.25,1.25,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,25,50,3.33333333333333,1.66666666666667,8.69565217391304E-03,.008300395256917,7.90513833992095E-03,7.5098814229249E-03,7.11462450592885E-03,6.71936758893281E-03,6.32411067193676E-03,5.92885375494071E-03,5.53359683794466E-03,5.13833992094862E-03,4.74308300395257E-03,4.34782608695652E-03,3.95256916996047E-03,3.55731225296443E-03,3.16205533596838E-03,2.76679841897233E-03,2.37154150197628E-03,1.97628458498024E-03,1.58102766798419E-03,1.18577075098814E-03,7.90513833992096E-04,3.95256916996047E-04,}
		valueWeights[dataIndex["grow_dodge"]] ={6.09756097560976E-03,5,3.75,3.75,3.75,3.75,25,50,3.33333333333333,1.66666666666667,4.87804878048781E-03,4.75609756097561E-03,4.63414634146342E-03,4.51219512195122E-03,4.39024390243902E-03,4.26829268292683E-03,4.14634146341464E-03,4.02439024390244E-03,3.90243902439024E-03,3.78048780487805E-03,3.65853658536585E-03,3.53658536585366E-03,3.41463414634146E-03,3.29268292682927E-03,3.17073170731707E-03,3.04878048780488E-03,2.92682926829268E-03,2.80487804878049E-03,2.68292682926829E-03,2.5609756097561E-03,2.4390243902439E-03,2.31707317073171E-03,2.19512195121951E-03,2.07317073170732E-03,1.95121951219512E-03,1.82926829268293E-03,1.70731707317073E-03,1.58536585365854E-03,1.46341463414634E-03,1.34146341463415E-03,1.21951219512195E-03,1.09756097560976E-03,9.75609756097561E-04,8.53658536585367E-04,7.31707317073171E-04,6.09756097560976E-04,4.87804878048781E-04,3.65853658536586E-04,2.4390243902439E-04,1.21951219512195E-04,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={5.49450549450549E-02,5.38461538461538E-02,5.27472527472527E-02,5.16483516483516E-02,5.05494505494506E-02,4.94505494505494E-02,4.83516483516483E-02,4.72527472527473E-02,4.61538461538461E-02,4.50549450549451E-02,1,1,1,1,1,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,6.66666666666667,13.3333333333333,20,3.33333333333333,1.66666666666667,1.42857142857143E-02,1.31868131868132E-02,1.20879120879121E-02,.010989010989011,9.89010989010989E-03,8.79120879120879E-03,7.69230769230769E-03,6.59340659340659E-03,5.49450549450549E-03,4.3956043956044E-03,3.2967032967033E-03,2.1978021978022E-03,1.0989010989011E-03,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={5.49450549450549E-02,5.38461538461538E-02,5.27472527472527E-02,5.16483516483516E-02,5.05494505494506E-02,4.94505494505494E-02,4.83516483516483E-02,4.72527472527473E-02,4.61538461538461E-02,4.50549450549451E-02,1,1,1,1,1,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,6.66666666666667,13.3333333333333,20,3.33333333333333,1.66666666666667,1.42857142857143E-02,1.31868131868132E-02,1.20879120879121E-02,.010989010989011,9.89010989010989E-03,8.79120879120879E-03,7.69230769230769E-03,6.59340659340659E-03,5.49450549450549E-03,4.3956043956044E-03,3.2967032967033E-03,2.1978021978022E-03,1.0989010989011E-03,}
		valueWeights[dataIndex["grow_resis_silence"]] ={2.16450216450216E-02,2.12121212121212E-02,2.07792207792208E-02,2.03463203463203E-02,1.99134199134199E-02,1.94805194805195E-02,1.25,1.25,1.25,1.25,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,2.66666666666667,5.33333333333333,8,10.6666666666667,13.3333333333333,2.5,1.66666666666667,.833333333333333,9.09090909090909E-03,8.65800865800866E-03,8.22510822510823E-03,7.79220779220779E-03,7.35930735930736E-03,6.92640692640693E-03,6.49350649350649E-03,6.06060606060606E-03,5.62770562770563E-03,5.19480519480519E-03,4.76190476190476E-03,4.32900432900433E-03,3.8961038961039E-03,3.46320346320346E-03,3.03030303030303E-03,2.5974025974026E-03,2.16450216450216E-03,1.73160173160173E-03,1.2987012987013E-03,8.65800865800865E-04,4.32900432900432E-04,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={3.67647058823529E-02,3.60294117647059E-02,3.52941176470588E-02,3.45588235294118E-02,3.38235294117647E-02,3.30882352941176E-02,3.23529411764706E-02,3.16176470588235E-02,1,1,1,1,1,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,4,8,12,16,2.5,1.66666666666667,.833333333333333,1.17647058823529E-02,1.10294117647059E-02,1.02941176470588E-02,9.55882352941177E-03,8.82352941176471E-03,8.08823529411765E-03,7.35294117647059E-03,6.61764705882353E-03,5.88235294117647E-03,5.14705882352941E-03,4.41176470588235E-03,3.67647058823529E-03,2.94117647058824E-03,2.20588235294118E-03,1.47058823529412E-03,7.3529411764706E-04,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={3.67647058823529E-02,3.60294117647059E-02,3.52941176470588E-02,3.45588235294118E-02,3.38235294117647E-02,3.30882352941176E-02,3.23529411764706E-02,3.16176470588235E-02,1,1,1,1,1,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,4,8,12,16,2.5,1.66666666666667,.833333333333333,1.17647058823529E-02,1.10294117647059E-02,1.02941176470588E-02,9.55882352941177E-03,8.82352941176471E-03,8.08823529411765E-03,7.35294117647059E-03,6.61764705882353E-03,5.88235294117647E-03,5.14705882352941E-03,4.41176470588235E-03,3.67647058823529E-03,2.94117647058824E-03,2.20588235294118E-03,1.47058823529412E-03,7.3529411764706E-04,}

	---Ç§ÃÄÁ«	Ì°ÀÇ
	elseif tmplID == talismans["HH_105_QianMeiLian"].tmplID or tmplID == talismans["HG_105_TanLang"].tmplID then

		valueWeights[dataIndex["default_hp"]] ={2.63157894736842E-02,2.57894736842105E-02,2.52631578947368E-02,2.47368421052632E-02,2.42105263157895E-02,2.36842105263158E-02,2.31578947368421E-02,1,1,1,1,1,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,7.5,15,22.5,30,2.5,1.66666666666667,.833333333333333,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["default_mp"]] ={1.14942528735632E-02,.011264367816092,1.10344827586207E-02,1.08045977011494E-02,2.5,2.5,1.875,1.875,1.875,1.875,1.875,1.875,1.875,1.875,7.5,15,22.5,30,2.5,1.66666666666667,.833333333333333,6.66666666666667E-03,6.4367816091954E-03,6.20689655172414E-03,5.97701149425287E-03,5.74712643678161E-03,5.51724137931035E-03,5.28735632183908E-03,5.05747126436782E-03,4.82758620689655E-03,4.59770114942529E-03,4.36781609195402E-03,4.13793103448276E-03,3.90804597701149E-03,3.67816091954023E-03,3.44827586206897E-03,3.2183908045977E-03,2.98850574712644E-03,2.75862068965517E-03,2.52873563218391E-03,2.29885057471264E-03,2.06896551724138E-03,1.83908045977012E-03,1.60919540229885E-03,1.37931034482759E-03,1.14942528735632E-03,9.19540229885057E-04,6.89655172413793E-04,4.59770114942529E-04,2.29885057471264E-04,}
		valueWeights[dataIndex["default_attack_rate"]] ={4.16666666666667E-02,4.08333333333333E-02,.04,3.91666666666667E-02,3.83333333333333E-02,.0375,3.66666666666667E-02,1,1,1,1,1,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}
		valueWeights[dataIndex["default_damage_low"]] ={6.41025641025641E-02,6.28205128205128E-02,6.15384615384615E-02,6.02564102564103E-02,.058974358974359,5.76923076923077E-02,5.64102564102564E-02,5.51282051282051E-02,5.38461538461538E-02,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,10,15,20,25,2.5,1.66666666666667,.833333333333333,1.53846153846154E-02,1.41025641025641E-02,1.28205128205128E-02,1.15384615384615E-02,1.02564102564103E-02,8.97435897435898E-03,7.69230769230769E-03,6.41025641025641E-03,5.12820512820513E-03,3.84615384615385E-03,2.56410256410257E-03,1.28205128205128E-03,}
		valueWeights[dataIndex["default_damage_high"]] ={6.41025641025641E-02,6.28205128205128E-02,6.15384615384615E-02,6.02564102564103E-02,.058974358974359,5.76923076923077E-02,5.64102564102564E-02,5.51282051282051E-02,5.38461538461538E-02,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,5,10,15,20,25,2.5,1.66666666666667,.833333333333333,1.53846153846154E-02,1.41025641025641E-02,1.28205128205128E-02,1.15384615384615E-02,1.02564102564103E-02,8.97435897435898E-03,7.69230769230769E-03,6.41025641025641E-03,5.12820512820513E-03,3.84615384615385E-03,2.56410256410257E-03,1.28205128205128E-03,}
		valueWeights[dataIndex["default_dodge"]] ={4.16666666666667E-02,4.08333333333333E-02,.04,3.91666666666667E-02,3.83333333333333E-02,.0375,3.66666666666667E-02,1,1,1,1,1,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={.111111111111111,.108888888888889,.106666666666667,.104444444444444,.102222222222222,.1,9.77777777777778E-02,9.55555555555556E-02,9.33333333333333E-02,1,1,1,1,1,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,2.67857142857143,5.35714285714286,8.03571428571428,10.7142857142857,13.3928571428571,16.0714285714286,18.75,2,1.5,1,.5,.02,1.77777777777778E-02,1.55555555555556E-02,1.33333333333333E-02,1.11111111111111E-02,8.88888888888889E-03,6.66666666666667E-03,4.44444444444445E-03,2.22222222222222E-03,}
		valueWeights[dataIndex["default_resis_sleep"]] ={.111111111111111,.108888888888889,.106666666666667,.104444444444444,.102222222222222,.1,9.77777777777778E-02,9.55555555555556E-02,9.33333333333333E-02,1,1,1,1,1,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,2.67857142857143,5.35714285714286,8.03571428571428,10.7142857142857,13.3928571428571,16.0714285714286,18.75,2,1.5,1,.5,.02,1.77777777777778E-02,1.55555555555556E-02,1.33333333333333E-02,1.11111111111111E-02,8.88888888888889E-03,6.66666666666667E-03,4.44444444444445E-03,2.22222222222222E-03,}
		valueWeights[dataIndex["default_resis_silence"]] ={6.41025641025641E-02,6.28205128205128E-02,6.15384615384615E-02,6.02564102564103E-02,.058974358974359,5.76923076923077E-02,5.64102564102564E-02,5.51282051282051E-02,1.25,1.25,1.25,1.25,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.66666666666667,3.33333333333333,5,6.66666666666667,8.33333333333333,10,11.6666666666667,13.3333333333333,15,2,1.5,1,.5,1.53846153846154E-02,1.41025641025641E-02,1.28205128205128E-02,1.15384615384615E-02,1.02564102564103E-02,8.97435897435898E-03,7.69230769230769E-03,6.41025641025641E-03,5.12820512820513E-03,3.84615384615385E-03,2.56410256410257E-03,1.28205128205128E-03,}
		valueWeights[dataIndex["default_resis_weakness"]] ={.178571428571429,.175,.171428571428571,.167857142857143,.164285714285714,.160714285714286,.157142857142857,.153571428571429,.15,.146428571428571,.142857142857143,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,2.5,1.66666666666667,.833333333333333,.025,2.14285714285714E-02,1.78571428571429E-02,1.42857142857143E-02,1.07142857142857E-02,7.14285714285714E-03,3.57142857142857E-03,}
		valueWeights[dataIndex["default_resis_enlace"]] ={.178571428571429,.175,.171428571428571,.167857142857143,.164285714285714,.160714285714286,.157142857142857,.153571428571429,.15,.146428571428571,.142857142857143,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,2.5,1.66666666666667,.833333333333333,.025,2.14285714285714E-02,1.78571428571429E-02,1.42857142857143E-02,1.07142857142857E-02,7.14285714285714E-03,3.57142857142857E-03,}
		
		valueWeights[dataIndex["grow_hp"]] ={1.97628458498024E-02,1.93675889328063E-02,1.89723320158103E-02,1.85770750988142E-02,1.81818181818182E-02,1.77865612648221E-02,1.73913043478261E-02,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,25,50,2.5,1.66666666666667,.833333333333333,8.69565217391304E-03,.008300395256917,7.90513833992095E-03,7.5098814229249E-03,7.11462450592885E-03,6.71936758893281E-03,6.32411067193676E-03,5.92885375494071E-03,5.53359683794466E-03,5.13833992094862E-03,4.74308300395257E-03,4.34782608695652E-03,3.95256916996047E-03,3.55731225296443E-03,3.16205533596838E-03,2.76679841897233E-03,2.37154150197628E-03,1.97628458498024E-03,1.58102766798419E-03,1.18577075098814E-03,7.90513833992096E-04,3.95256916996047E-04,}
		valueWeights[dataIndex["grow_mp"]] ={9.46969696969697E-03,9.28030303030303E-03,9.09090909090909E-03,1.66666666666667,1.66666666666667,1.66666666666667,2.5,2.5,2.5,2.5,2.5,2.5,12.5,25,37.5,2.5,1.66666666666667,.833333333333333,6.06060606060606E-03,5.87121212121212E-03,5.68181818181818E-03,5.49242424242424E-03,5.3030303030303E-03,5.11363636363636E-03,4.92424242424242E-03,4.73484848484849E-03,4.54545454545455E-03,4.35606060606061E-03,4.16666666666667E-03,3.97727272727273E-03,3.78787878787879E-03,3.59848484848485E-03,3.40909090909091E-03,3.21969696969697E-03,3.03030303030303E-03,2.84090909090909E-03,2.65151515151515E-03,2.46212121212121E-03,2.27272727272727E-03,2.08333333333333E-03,1.89393939393939E-03,1.70454545454545E-03,1.51515151515152E-03,1.32575757575758E-03,1.13636363636364E-03,9.46969696969697E-04,7.57575757575757E-04,5.68181818181818E-04,3.78787878787879E-04,1.8939393939394E-04,}
		valueWeights[dataIndex["grow_attack_rate"]] ={4.16666666666667E-02,4.08333333333333E-02,.04,3.91666666666667E-02,3.83333333333333E-02,.0375,3.66666666666667E-02,1,1,1,1,1,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}
		valueWeights[dataIndex["grow_damage_low"]] ={3.67647058823529E-02,3.60294117647059E-02,3.52941176470588E-02,3.45588235294118E-02,3.38235294117647E-02,3.30882352941176E-02,3.23529411764706E-02,3.16176470588235E-02,3.08823529411765E-02,1,1,1,1,1,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,12.5,25,37.5,2.5,1.66666666666667,.833333333333333,1.17647058823529E-02,1.10294117647059E-02,1.02941176470588E-02,9.55882352941177E-03,8.82352941176471E-03,8.08823529411765E-03,7.35294117647059E-03,6.61764705882353E-03,5.88235294117647E-03,5.14705882352941E-03,4.41176470588235E-03,3.67647058823529E-03,2.94117647058824E-03,2.20588235294118E-03,1.47058823529412E-03,7.3529411764706E-04,}
		valueWeights[dataIndex["grow_damage_high"]] ={3.67647058823529E-02,3.60294117647059E-02,3.52941176470588E-02,3.45588235294118E-02,3.38235294117647E-02,3.30882352941176E-02,3.23529411764706E-02,3.16176470588235E-02,3.08823529411765E-02,1,1,1,1,1,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,12.5,25,37.5,2.5,1.66666666666667,.833333333333333,1.17647058823529E-02,1.10294117647059E-02,1.02941176470588E-02,9.55882352941177E-03,8.82352941176471E-03,8.08823529411765E-03,7.35294117647059E-03,6.61764705882353E-03,5.88235294117647E-03,5.14705882352941E-03,4.41176470588235E-03,3.67647058823529E-03,2.94117647058824E-03,2.20588235294118E-03,1.47058823529412E-03,7.3529411764706E-04,}
		valueWeights[dataIndex["grow_dodge"]] ={4.16666666666667E-02,4.08333333333333E-02,.04,3.91666666666667E-02,3.83333333333333E-02,.0375,3.66666666666667E-02,1,1,1,1,1,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.0125,1.16666666666667E-02,1.08333333333333E-02,.01,9.16666666666667E-03,8.33333333333334E-03,.0075,6.66666666666667E-03,5.83333333333333E-03,.005,4.16666666666667E-03,3.33333333333333E-03,.0025,1.66666666666667E-03,8.33333333333334E-04,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={3.67647058823529E-02,3.60294117647059E-02,3.52941176470588E-02,3.45588235294118E-02,3.38235294117647E-02,3.30882352941176E-02,3.23529411764706E-02,3.16176470588235E-02,1,1,1,1,1,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,4,8,12,16,2.5,1.66666666666667,.833333333333333,1.17647058823529E-02,1.10294117647059E-02,1.02941176470588E-02,9.55882352941177E-03,8.82352941176471E-03,8.08823529411765E-03,7.35294117647059E-03,6.61764705882353E-03,5.88235294117647E-03,5.14705882352941E-03,4.41176470588235E-03,3.67647058823529E-03,2.94117647058824E-03,2.20588235294118E-03,1.47058823529412E-03,7.3529411764706E-04,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={3.67647058823529E-02,3.60294117647059E-02,3.52941176470588E-02,3.45588235294118E-02,3.38235294117647E-02,3.30882352941176E-02,3.23529411764706E-02,3.16176470588235E-02,1,1,1,1,1,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,4,8,12,16,2.5,1.66666666666667,.833333333333333,1.17647058823529E-02,1.10294117647059E-02,1.02941176470588E-02,9.55882352941177E-03,8.82352941176471E-03,8.08823529411765E-03,7.35294117647059E-03,6.61764705882353E-03,5.88235294117647E-03,5.14705882352941E-03,4.41176470588235E-03,3.67647058823529E-03,2.94117647058824E-03,2.20588235294118E-03,1.47058823529412E-03,7.3529411764706E-04,}
		valueWeights[dataIndex["grow_resis_silence"]] ={2.16450216450216E-02,2.12121212121212E-02,2.07792207792208E-02,2.03463203463203E-02,1.99134199134199E-02,1.94805194805195E-02,1.25,1.25,1.25,1.25,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,2.66666666666667,5.33333333333333,8,10.6666666666667,13.3333333333333,2.5,1.66666666666667,.833333333333333,9.09090909090909E-03,8.65800865800866E-03,8.22510822510823E-03,7.79220779220779E-03,7.35930735930736E-03,6.92640692640693E-03,6.49350649350649E-03,6.06060606060606E-03,5.62770562770563E-03,5.19480519480519E-03,4.76190476190476E-03,4.32900432900433E-03,3.8961038961039E-03,3.46320346320346E-03,3.03030303030303E-03,2.5974025974026E-03,2.16450216450216E-03,1.73160173160173E-03,1.2987012987013E-03,8.65800865800865E-04,4.32900432900432E-04,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={5.49450549450549E-02,5.38461538461538E-02,5.27472527472527E-02,5.16483516483516E-02,5.05494505494506E-02,4.94505494505494E-02,4.83516483516483E-02,4.72527472527473E-02,4.61538461538461E-02,4.50549450549451E-02,1,1,1,1,1,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,6.66666666666667,13.3333333333333,20,3.33333333333333,1.66666666666667,1.42857142857143E-02,1.31868131868132E-02,1.20879120879121E-02,.010989010989011,9.89010989010989E-03,8.79120879120879E-03,7.69230769230769E-03,6.59340659340659E-03,5.49450549450549E-03,4.3956043956044E-03,3.2967032967033E-03,2.1978021978022E-03,1.0989010989011E-03,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={5.49450549450549E-02,5.38461538461538E-02,5.27472527472527E-02,5.16483516483516E-02,5.05494505494506E-02,4.94505494505494E-02,4.83516483516483E-02,4.72527472527473E-02,4.61538461538461E-02,4.50549450549451E-02,1,1,1,1,1,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,6.66666666666667,13.3333333333333,20,3.33333333333333,1.66666666666667,1.42857142857143E-02,1.31868131868132E-02,1.20879120879121E-02,.010989010989011,9.89010989010989E-03,8.79120879120879E-03,7.69230769230769E-03,6.59340659340659E-03,5.49450549450549E-03,4.3956043956044E-03,3.2967032967033E-03,2.1978021978022E-03,1.0989010989011E-03,}

	---çúçêÖìç±	ÁðÁ§Õµ Âä±¦½ðÇ®
	elseif tmplID == talismans["QY_105_HuPoZhuLing"].tmplID or tmplID == talismans["TH_105_LiuLiZhan"].tmplID or tmplID == talismans["CH_105_luobaojinqian"].tmplID or tmplID == talismans["QJ_105_jinyubian"].tmplID or tmplID == talismans["YZ_105_shuolei"].tmplID then

		valueWeights[dataIndex["default_hp"]] ={7.93650793650794E-03,7.77777777777778E-03,7.61904761904762E-03,2.5,2.5,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,25,50,5,5.55555555555556E-03,5.3968253968254E-03,5.23809523809524E-03,5.07936507936508E-03,4.92063492063492E-03,4.76190476190476E-03,4.6031746031746E-03,4.44444444444444E-03,4.28571428571429E-03,4.12698412698413E-03,3.96825396825397E-03,3.80952380952381E-03,3.65079365079365E-03,3.49206349206349E-03,3.33333333333333E-03,3.17460317460317E-03,3.01587301587302E-03,2.85714285714286E-03,2.6984126984127E-03,2.53968253968254E-03,2.38095238095238E-03,2.22222222222222E-03,2.06349206349206E-03,1.9047619047619E-03,1.74603174603175E-03,1.58730158730159E-03,1.42857142857143E-03,1.26984126984127E-03,1.11111111111111E-03,9.52380952380953E-04,7.93650793650793E-04,6.34920634920635E-04,4.76190476190476E-04,3.17460317460318E-04,1.58730158730159E-04,}
		valueWeights[dataIndex["default_mp"]] ={3.67647058823529E-02,3.60294117647059E-02,3.52941176470588E-02,3.45588235294118E-02,3.38235294117647E-02,3.30882352941176E-02,3.23529411764706E-02,1.25,1.25,1.25,1.25,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,2.67857142857143,5.35714285714286,8.03571428571428,10.7142857142857,13.3928571428571,16.0714285714286,18.75,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,1.17647058823529E-02,1.10294117647059E-02,1.02941176470588E-02,9.55882352941177E-03,8.82352941176471E-03,8.08823529411765E-03,7.35294117647059E-03,6.61764705882353E-03,5.88235294117647E-03,5.14705882352941E-03,4.41176470588235E-03,3.67647058823529E-03,2.94117647058824E-03,2.20588235294118E-03,1.47058823529412E-03,7.3529411764706E-04,}
		valueWeights[dataIndex["default_attack_rate"]] ={6.41025641025641E-03,6.28205128205128E-03,5,3.75,3.75,3.75,3.75,25,50,3.33333333333333,1.66666666666667,.005,4.87179487179487E-03,4.74358974358974E-03,4.61538461538462E-03,4.48717948717949E-03,4.35897435897436E-03,4.23076923076923E-03,4.1025641025641E-03,3.97435897435897E-03,3.84615384615385E-03,3.71794871794872E-03,3.58974358974359E-03,3.46153846153846E-03,3.33333333333333E-03,3.20512820512821E-03,3.07692307692308E-03,2.94871794871795E-03,2.82051282051282E-03,2.69230769230769E-03,2.56410256410256E-03,2.43589743589744E-03,2.30769230769231E-03,2.17948717948718E-03,2.05128205128205E-03,1.92307692307692E-03,1.7948717948718E-03,1.66666666666667E-03,1.53846153846154E-03,1.41025641025641E-03,1.28205128205128E-03,1.15384615384615E-03,1.02564102564103E-03,8.97435897435898E-04,7.69230769230769E-04,6.41025641025641E-04,5.12820512820513E-04,3.84615384615385E-04,2.56410256410256E-04,1.28205128205128E-04,}
		valueWeights[dataIndex["default_damage_low"]] ={.111111111111111,.108888888888889,.106666666666667,.104444444444444,.102222222222222,.1,9.77777777777778E-02,9.55555555555556E-02,9.33333333333333E-02,9.11111111111111E-02,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,5,10,15,20,25,2.5,1.66666666666667,.833333333333333,.02,1.77777777777778E-02,1.55555555555556E-02,1.33333333333333E-02,1.11111111111111E-02,8.88888888888889E-03,6.66666666666667E-03,4.44444444444445E-03,2.22222222222222E-03,}
		valueWeights[dataIndex["default_damage_high"]] ={.111111111111111,.108888888888889,.106666666666667,.104444444444444,.102222222222222,.1,9.77777777777778E-02,9.55555555555556E-02,9.33333333333333E-02,9.11111111111111E-02,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,5,10,15,20,25,2.5,1.66666666666667,.833333333333333,.02,1.77777777777778E-02,1.55555555555556E-02,1.33333333333333E-02,1.11111111111111E-02,8.88888888888889E-03,6.66666666666667E-03,4.44444444444445E-03,2.22222222222222E-03,}
		valueWeights[dataIndex["default_dodge"]] ={6.09756097560976E-03,5,3.75,3.75,3.75,3.75,25,50,3.33333333333333,1.66666666666667,4.87804878048781E-03,4.75609756097561E-03,4.63414634146342E-03,4.51219512195122E-03,4.39024390243902E-03,4.26829268292683E-03,4.14634146341464E-03,4.02439024390244E-03,3.90243902439024E-03,3.78048780487805E-03,3.65853658536585E-03,3.53658536585366E-03,3.41463414634146E-03,3.29268292682927E-03,3.17073170731707E-03,3.04878048780488E-03,2.92682926829268E-03,2.80487804878049E-03,2.68292682926829E-03,2.5609756097561E-03,2.4390243902439E-03,2.31707317073171E-03,2.19512195121951E-03,2.07317073170732E-03,1.95121951219512E-03,1.82926829268293E-03,1.70731707317073E-03,1.58536585365854E-03,1.46341463414634E-03,1.34146341463415E-03,1.21951219512195E-03,1.09756097560976E-03,9.75609756097561E-04,8.53658536585367E-04,7.31707317073171E-04,6.09756097560976E-04,4.87804878048781E-04,3.65853658536586E-04,2.4390243902439E-04,1.21951219512195E-04,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={.178571428571429,.175,.171428571428571,.167857142857143,.164285714285714,.160714285714286,.157142857142857,.153571428571429,.15,.146428571428571,.142857142857143,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,2.5,1.66666666666667,.833333333333333,.025,2.14285714285714E-02,1.78571428571429E-02,1.42857142857143E-02,1.07142857142857E-02,7.14285714285714E-03,3.57142857142857E-03,}
		valueWeights[dataIndex["default_resis_sleep"]] ={.111111111111111,.108888888888889,.106666666666667,.104444444444444,.102222222222222,.1,9.77777777777778E-02,9.55555555555556E-02,9.33333333333333E-02,1,1,1,1,1,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,2.67857142857143,5.35714285714286,8.03571428571428,10.7142857142857,13.3928571428571,16.0714285714286,18.75,2,1.5,1,.5,.02,1.77777777777778E-02,1.55555555555556E-02,1.33333333333333E-02,1.11111111111111E-02,8.88888888888889E-03,6.66666666666667E-03,4.44444444444445E-03,2.22222222222222E-03,}
		valueWeights[dataIndex["default_resis_silence"]] ={6.41025641025641E-02,6.28205128205128E-02,6.15384615384615E-02,6.02564102564103E-02,.058974358974359,5.76923076923077E-02,5.64102564102564E-02,5.51282051282051E-02,1.25,1.25,1.25,1.25,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.66666666666667,3.33333333333333,5,6.66666666666667,8.33333333333333,10,11.6666666666667,13.3333333333333,15,2,1.5,1,.5,1.53846153846154E-02,1.41025641025641E-02,1.28205128205128E-02,1.15384615384615E-02,1.02564102564103E-02,8.97435897435898E-03,7.69230769230769E-03,6.41025641025641E-03,5.12820512820513E-03,3.84615384615385E-03,2.56410256410257E-03,1.28205128205128E-03,}
		valueWeights[dataIndex["default_resis_weakness"]] ={.178571428571429,.175,.171428571428571,.167857142857143,.164285714285714,.160714285714286,.157142857142857,.153571428571429,.15,.146428571428571,.142857142857143,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,2.5,1.66666666666667,.833333333333333,.025,2.14285714285714E-02,1.78571428571429E-02,1.42857142857143E-02,1.07142857142857E-02,7.14285714285714E-03,3.57142857142857E-03,}
		valueWeights[dataIndex["default_resis_enlace"]] ={.111111111111111,.108888888888889,.106666666666667,.104444444444444,.102222222222222,.1,9.77777777777778E-02,9.55555555555556E-02,9.33333333333333E-02,1,1,1,1,1,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,2.67857142857143,5.35714285714286,8.03571428571428,10.7142857142857,13.3928571428571,16.0714285714286,18.75,2,1.5,1,.5,.02,1.77777777777778E-02,1.55555555555556E-02,1.33333333333333E-02,1.11111111111111E-02,8.88888888888889E-03,6.66666666666667E-03,4.44444444444445E-03,2.22222222222222E-03,}
		
		valueWeights[dataIndex["grow_hp"]] ={7.11237553342817E-03,6.9701280227596E-03,6.82788051209104E-03,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,75,5,5.26315789473684E-03,5.12091038406828E-03,4.97866287339972E-03,4.83641536273115E-03,4.69416785206259E-03,4.55192034139403E-03,4.40967283072546E-03,4.2674253200569E-03,4.12517780938834E-03,3.98293029871977E-03,3.84068278805121E-03,3.69843527738265E-03,3.55618776671408E-03,3.41394025604552E-03,3.27169274537696E-03,3.12944523470839E-03,2.98719772403983E-03,2.84495021337127E-03,2.7027027027027E-03,2.56045519203414E-03,2.41820768136558E-03,2.27596017069701E-03,2.13371266002845E-03,1.99146514935989E-03,1.84921763869132E-03,1.70697012802276E-03,1.5647226173542E-03,1.42247510668563E-03,1.28022759601707E-03,1.13798008534851E-03,9.95732574679943E-04,8.53485064011381E-04,7.11237553342817E-04,5.68990042674253E-04,4.2674253200569E-04,2.84495021337127E-04,1.42247510668564E-04,}
		valueWeights[dataIndex["grow_mp"]] ={2.63157894736842E-02,2.57894736842105E-02,2.52631578947368E-02,2.47368421052632E-02,2.42105263157895E-02,2.36842105263158E-02,1.25,1.25,1.25,1.25,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,5,10,15,20,25,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["grow_attack_rate"]] ={6.41025641025641E-03,6.28205128205128E-03,5,3.75,3.75,3.75,3.75,25,50,3.33333333333333,1.66666666666667,.005,4.87179487179487E-03,4.74358974358974E-03,4.61538461538462E-03,4.48717948717949E-03,4.35897435897436E-03,4.23076923076923E-03,4.1025641025641E-03,3.97435897435897E-03,3.84615384615385E-03,3.71794871794872E-03,3.58974358974359E-03,3.46153846153846E-03,3.33333333333333E-03,3.20512820512821E-03,3.07692307692308E-03,2.94871794871795E-03,2.82051282051282E-03,2.69230769230769E-03,2.56410256410256E-03,2.43589743589744E-03,2.30769230769231E-03,2.17948717948718E-03,2.05128205128205E-03,1.92307692307692E-03,1.7948717948718E-03,1.66666666666667E-03,1.53846153846154E-03,1.41025641025641E-03,1.28205128205128E-03,1.15384615384615E-03,1.02564102564103E-03,8.97435897435898E-04,7.69230769230769E-04,6.41025641025641E-04,5.12820512820513E-04,3.84615384615385E-04,2.56410256410256E-04,1.28205128205128E-04,}
		valueWeights[dataIndex["grow_damage_low"]] ={5.49450549450549E-02,5.38461538461538E-02,5.27472527472527E-02,5.16483516483516E-02,5.05494505494506E-02,4.94505494505494E-02,4.83516483516483E-02,4.72527472527473E-02,4.61538461538461E-02,4.50549450549451E-02,1,1,1,1,1,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,12.5,25,37.5,2.5,1.66666666666667,.833333333333333,1.42857142857143E-02,1.31868131868132E-02,1.20879120879121E-02,.010989010989011,9.89010989010989E-03,8.79120879120879E-03,7.69230769230769E-03,6.59340659340659E-03,5.49450549450549E-03,4.3956043956044E-03,3.2967032967033E-03,2.1978021978022E-03,1.0989010989011E-03,}
		valueWeights[dataIndex["grow_damage_high"]] ={5.49450549450549E-02,5.38461538461538E-02,5.27472527472527E-02,5.16483516483516E-02,5.05494505494506E-02,4.94505494505494E-02,4.83516483516483E-02,4.72527472527473E-02,4.61538461538461E-02,4.50549450549451E-02,1,1,1,1,1,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,12.5,25,37.5,2.5,1.66666666666667,.833333333333333,1.42857142857143E-02,1.31868131868132E-02,1.20879120879121E-02,.010989010989011,9.89010989010989E-03,8.79120879120879E-03,7.69230769230769E-03,6.59340659340659E-03,5.49450549450549E-03,4.3956043956044E-03,3.2967032967033E-03,2.1978021978022E-03,1.0989010989011E-03,}
		valueWeights[dataIndex["grow_dodge"]] ={6.09756097560976E-03,5,3.75,3.75,3.75,3.75,25,50,3.33333333333333,1.66666666666667,4.87804878048781E-03,4.75609756097561E-03,4.63414634146342E-03,4.51219512195122E-03,4.39024390243902E-03,4.26829268292683E-03,4.14634146341464E-03,4.02439024390244E-03,3.90243902439024E-03,3.78048780487805E-03,3.65853658536585E-03,3.53658536585366E-03,3.41463414634146E-03,3.29268292682927E-03,3.17073170731707E-03,3.04878048780488E-03,2.92682926829268E-03,2.80487804878049E-03,2.68292682926829E-03,2.5609756097561E-03,2.4390243902439E-03,2.31707317073171E-03,2.19512195121951E-03,2.07317073170732E-03,1.95121951219512E-03,1.82926829268293E-03,1.70731707317073E-03,1.58536585365854E-03,1.46341463414634E-03,1.34146341463415E-03,1.21951219512195E-03,1.09756097560976E-03,9.75609756097561E-04,8.53658536585367E-04,7.31707317073171E-04,6.09756097560976E-04,4.87804878048781E-04,3.65853658536586E-04,2.4390243902439E-04,1.21951219512195E-04,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={5.49450549450549E-02,5.38461538461538E-02,5.27472527472527E-02,5.16483516483516E-02,5.05494505494506E-02,4.94505494505494E-02,4.83516483516483E-02,4.72527472527473E-02,4.61538461538461E-02,4.50549450549451E-02,1,1,1,1,1,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,6.66666666666667,13.3333333333333,20,3.33333333333333,1.66666666666667,1.42857142857143E-02,1.31868131868132E-02,1.20879120879121E-02,.010989010989011,9.89010989010989E-03,8.79120879120879E-03,7.69230769230769E-03,6.59340659340659E-03,5.49450549450549E-03,4.3956043956044E-03,3.2967032967033E-03,2.1978021978022E-03,1.0989010989011E-03,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={3.67647058823529E-02,3.60294117647059E-02,3.52941176470588E-02,3.45588235294118E-02,3.38235294117647E-02,3.30882352941176E-02,3.23529411764706E-02,3.16176470588235E-02,1,1,1,1,1,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,4,8,12,16,2.5,1.66666666666667,.833333333333333,1.17647058823529E-02,1.10294117647059E-02,1.02941176470588E-02,9.55882352941177E-03,8.82352941176471E-03,8.08823529411765E-03,7.35294117647059E-03,6.61764705882353E-03,5.88235294117647E-03,5.14705882352941E-03,4.41176470588235E-03,3.67647058823529E-03,2.94117647058824E-03,2.20588235294118E-03,1.47058823529412E-03,7.3529411764706E-04,}
		valueWeights[dataIndex["grow_resis_silence"]] ={2.16450216450216E-02,2.12121212121212E-02,2.07792207792208E-02,2.03463203463203E-02,1.99134199134199E-02,1.94805194805195E-02,1.25,1.25,1.25,1.25,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,2.66666666666667,5.33333333333333,8,10.6666666666667,13.3333333333333,2.5,1.66666666666667,.833333333333333,9.09090909090909E-03,8.65800865800866E-03,8.22510822510823E-03,7.79220779220779E-03,7.35930735930736E-03,6.92640692640693E-03,6.49350649350649E-03,6.06060606060606E-03,5.62770562770563E-03,5.19480519480519E-03,4.76190476190476E-03,4.32900432900433E-03,3.8961038961039E-03,3.46320346320346E-03,3.03030303030303E-03,2.5974025974026E-03,2.16450216450216E-03,1.73160173160173E-03,1.2987012987013E-03,8.65800865800865E-04,4.32900432900432E-04,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={5.49450549450549E-02,5.38461538461538E-02,5.27472527472527E-02,5.16483516483516E-02,5.05494505494506E-02,4.94505494505494E-02,4.83516483516483E-02,4.72527472527473E-02,4.61538461538461E-02,4.50549450549451E-02,1,1,1,1,1,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,6.66666666666667,13.3333333333333,20,3.33333333333333,1.66666666666667,1.42857142857143E-02,1.31868131868132E-02,1.20879120879121E-02,.010989010989011,9.89010989010989E-03,8.79120879120879E-03,7.69230769230769E-03,6.59340659340659E-03,5.49450549450549E-03,4.3956043956044E-03,3.2967032967033E-03,2.1978021978022E-03,1.0989010989011E-03,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={3.67647058823529E-02,3.60294117647059E-02,3.52941176470588E-02,3.45588235294118E-02,3.38235294117647E-02,3.30882352941176E-02,3.23529411764706E-02,3.16176470588235E-02,1,1,1,1,1,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,4,8,12,16,2.5,1.66666666666667,.833333333333333,1.17647058823529E-02,1.10294117647059E-02,1.02941176470588E-02,9.55882352941177E-03,8.82352941176471E-03,8.08823529411765E-03,7.35294117647059E-03,6.61764705882353E-03,5.88235294117647E-03,5.14705882352941E-03,4.41176470588235E-03,3.67647058823529E-03,2.94117647058824E-03,2.20588235294118E-03,1.47058823529412E-03,7.3529411764706E-04,}
		
	---Ç§¶¾¹Æ	¹íµÀ3 »ðÁáçç ·ÙÏã3 ×ÓÄ¸ÒõÑô¿Û Ì«ê»3
	elseif tmplID == talismans["GD_105_QianDuGu"].tmplID or tmplID == talismans["FX_105_HuoLingLong"].tmplID or tmplID == talismans["TH_105_ZiMuYinYangKou"].tmplID then

		valueWeights[dataIndex["default_hp"]] ={7.93650793650794E-03,7.77777777777778E-03,7.61904761904762E-03,2.5,2.5,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,25,50,5,5.55555555555556E-03,5.3968253968254E-03,5.23809523809524E-03,5.07936507936508E-03,4.92063492063492E-03,4.76190476190476E-03,4.6031746031746E-03,4.44444444444444E-03,4.28571428571429E-03,4.12698412698413E-03,3.96825396825397E-03,3.80952380952381E-03,3.65079365079365E-03,3.49206349206349E-03,3.33333333333333E-03,3.17460317460317E-03,3.01587301587302E-03,2.85714285714286E-03,2.6984126984127E-03,2.53968253968254E-03,2.38095238095238E-03,2.22222222222222E-03,2.06349206349206E-03,1.9047619047619E-03,1.74603174603175E-03,1.58730158730159E-03,1.42857142857143E-03,1.26984126984127E-03,1.11111111111111E-03,9.52380952380953E-04,7.93650793650793E-04,6.34920634920635E-04,4.76190476190476E-04,3.17460317460318E-04,1.58730158730159E-04,}
		valueWeights[dataIndex["default_mp"]] ={3.67647058823529E-02,3.60294117647059E-02,3.52941176470588E-02,3.45588235294118E-02,3.38235294117647E-02,3.30882352941176E-02,3.23529411764706E-02,1.25,1.25,1.25,1.25,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,2.67857142857143,5.35714285714286,8.03571428571428,10.7142857142857,13.3928571428571,16.0714285714286,18.75,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,1.17647058823529E-02,1.10294117647059E-02,1.02941176470588E-02,9.55882352941177E-03,8.82352941176471E-03,8.08823529411765E-03,7.35294117647059E-03,6.61764705882353E-03,5.88235294117647E-03,5.14705882352941E-03,4.41176470588235E-03,3.67647058823529E-03,2.94117647058824E-03,2.20588235294118E-03,1.47058823529412E-03,7.3529411764706E-04,}
		valueWeights[dataIndex["default_attack_rate"]] ={6.41025641025641E-03,6.28205128205128E-03,5,3.75,3.75,3.75,3.75,25,50,3.33333333333333,1.66666666666667,.005,4.87179487179487E-03,4.74358974358974E-03,4.61538461538462E-03,4.48717948717949E-03,4.35897435897436E-03,4.23076923076923E-03,4.1025641025641E-03,3.97435897435897E-03,3.84615384615385E-03,3.71794871794872E-03,3.58974358974359E-03,3.46153846153846E-03,3.33333333333333E-03,3.20512820512821E-03,3.07692307692308E-03,2.94871794871795E-03,2.82051282051282E-03,2.69230769230769E-03,2.56410256410256E-03,2.43589743589744E-03,2.30769230769231E-03,2.17948717948718E-03,2.05128205128205E-03,1.92307692307692E-03,1.7948717948718E-03,1.66666666666667E-03,1.53846153846154E-03,1.41025641025641E-03,1.28205128205128E-03,1.15384615384615E-03,1.02564102564103E-03,8.97435897435898E-04,7.69230769230769E-04,6.41025641025641E-04,5.12820512820513E-04,3.84615384615385E-04,2.56410256410256E-04,1.28205128205128E-04,}
		valueWeights[dataIndex["default_damage_low"]] ={.111111111111111,.108888888888889,.106666666666667,.104444444444444,.102222222222222,.1,9.77777777777778E-02,9.55555555555556E-02,9.33333333333333E-02,9.11111111111111E-02,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,5,10,15,20,25,2.5,1.66666666666667,.833333333333333,.02,1.77777777777778E-02,1.55555555555556E-02,1.33333333333333E-02,1.11111111111111E-02,8.88888888888889E-03,6.66666666666667E-03,4.44444444444445E-03,2.22222222222222E-03,}
		valueWeights[dataIndex["default_damage_high"]] ={.111111111111111,.108888888888889,.106666666666667,.104444444444444,.102222222222222,.1,9.77777777777778E-02,9.55555555555556E-02,9.33333333333333E-02,9.11111111111111E-02,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,5,10,15,20,25,2.5,1.66666666666667,.833333333333333,.02,1.77777777777778E-02,1.55555555555556E-02,1.33333333333333E-02,1.11111111111111E-02,8.88888888888889E-03,6.66666666666667E-03,4.44444444444445E-03,2.22222222222222E-03,}
		valueWeights[dataIndex["default_dodge"]] ={6.09756097560976E-03,5,3.75,3.75,3.75,3.75,25,50,3.33333333333333,1.66666666666667,4.87804878048781E-03,4.75609756097561E-03,4.63414634146342E-03,4.51219512195122E-03,4.39024390243902E-03,4.26829268292683E-03,4.14634146341464E-03,4.02439024390244E-03,3.90243902439024E-03,3.78048780487805E-03,3.65853658536585E-03,3.53658536585366E-03,3.41463414634146E-03,3.29268292682927E-03,3.17073170731707E-03,3.04878048780488E-03,2.92682926829268E-03,2.80487804878049E-03,2.68292682926829E-03,2.5609756097561E-03,2.4390243902439E-03,2.31707317073171E-03,2.19512195121951E-03,2.07317073170732E-03,1.95121951219512E-03,1.82926829268293E-03,1.70731707317073E-03,1.58536585365854E-03,1.46341463414634E-03,1.34146341463415E-03,1.21951219512195E-03,1.09756097560976E-03,9.75609756097561E-04,8.53658536585367E-04,7.31707317073171E-04,6.09756097560976E-04,4.87804878048781E-04,3.65853658536586E-04,2.4390243902439E-04,1.21951219512195E-04,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={.178571428571429,.175,.171428571428571,.167857142857143,.164285714285714,.160714285714286,.157142857142857,.153571428571429,.15,.146428571428571,.142857142857143,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,2.5,1.66666666666667,.833333333333333,.025,2.14285714285714E-02,1.78571428571429E-02,1.42857142857143E-02,1.07142857142857E-02,7.14285714285714E-03,3.57142857142857E-03,}
		valueWeights[dataIndex["default_resis_sleep"]] ={.111111111111111,.108888888888889,.106666666666667,.104444444444444,.102222222222222,.1,9.77777777777778E-02,9.55555555555556E-02,9.33333333333333E-02,1,1,1,1,1,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,2.67857142857143,5.35714285714286,8.03571428571428,10.7142857142857,13.3928571428571,16.0714285714286,18.75,2,1.5,1,.5,.02,1.77777777777778E-02,1.55555555555556E-02,1.33333333333333E-02,1.11111111111111E-02,8.88888888888889E-03,6.66666666666667E-03,4.44444444444445E-03,2.22222222222222E-03,}
		valueWeights[dataIndex["default_resis_silence"]] ={6.41025641025641E-02,6.28205128205128E-02,6.15384615384615E-02,6.02564102564103E-02,.058974358974359,5.76923076923077E-02,5.64102564102564E-02,5.51282051282051E-02,1.25,1.25,1.25,1.25,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.66666666666667,3.33333333333333,5,6.66666666666667,8.33333333333333,10,11.6666666666667,13.3333333333333,15,2,1.5,1,.5,1.53846153846154E-02,1.41025641025641E-02,1.28205128205128E-02,1.15384615384615E-02,1.02564102564103E-02,8.97435897435898E-03,7.69230769230769E-03,6.41025641025641E-03,5.12820512820513E-03,3.84615384615385E-03,2.56410256410257E-03,1.28205128205128E-03,}
		valueWeights[dataIndex["default_resis_weakness"]] ={.178571428571429,.175,.171428571428571,.167857142857143,.164285714285714,.160714285714286,.157142857142857,.153571428571429,.15,.146428571428571,.142857142857143,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,2.5,1.66666666666667,.833333333333333,.025,2.14285714285714E-02,1.78571428571429E-02,1.42857142857143E-02,1.07142857142857E-02,7.14285714285714E-03,3.57142857142857E-03,}
		valueWeights[dataIndex["default_resis_enlace"]] ={.111111111111111,.108888888888889,.106666666666667,.104444444444444,.102222222222222,.1,9.77777777777778E-02,9.55555555555556E-02,9.33333333333333E-02,1,1,1,1,1,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,2.67857142857143,5.35714285714286,8.03571428571428,10.7142857142857,13.3928571428571,16.0714285714286,18.75,2,1.5,1,.5,.02,1.77777777777778E-02,1.55555555555556E-02,1.33333333333333E-02,1.11111111111111E-02,8.88888888888889E-03,6.66666666666667E-03,4.44444444444445E-03,2.22222222222222E-03,}
		
		valueWeights[dataIndex["grow_hp"]] ={7.11237553342817E-03,6.9701280227596E-03,6.82788051209104E-03,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,75,5,5.26315789473684E-03,5.12091038406828E-03,4.97866287339972E-03,4.83641536273115E-03,4.69416785206259E-03,4.55192034139403E-03,4.40967283072546E-03,4.2674253200569E-03,4.12517780938834E-03,3.98293029871977E-03,3.84068278805121E-03,3.69843527738265E-03,3.55618776671408E-03,3.41394025604552E-03,3.27169274537696E-03,3.12944523470839E-03,2.98719772403983E-03,2.84495021337127E-03,2.7027027027027E-03,2.56045519203414E-03,2.41820768136558E-03,2.27596017069701E-03,2.13371266002845E-03,1.99146514935989E-03,1.84921763869132E-03,1.70697012802276E-03,1.5647226173542E-03,1.42247510668563E-03,1.28022759601707E-03,1.13798008534851E-03,9.95732574679943E-04,8.53485064011381E-04,7.11237553342817E-04,5.68990042674253E-04,4.2674253200569E-04,2.84495021337127E-04,1.42247510668564E-04,}
		valueWeights[dataIndex["grow_mp"]] ={2.63157894736842E-02,2.57894736842105E-02,2.52631578947368E-02,2.47368421052632E-02,2.42105263157895E-02,2.36842105263158E-02,1.25,1.25,1.25,1.25,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,5,10,15,20,25,1.66666666666667,1.33333333333333,1,.666666666666667,.333333333333333,.01,9.47368421052632E-03,8.94736842105263E-03,8.42105263157895E-03,7.89473684210526E-03,7.36842105263158E-03,6.8421052631579E-03,6.31578947368421E-03,5.78947368421053E-03,5.26315789473684E-03,4.73684210526316E-03,4.21052631578947E-03,3.68421052631579E-03,3.15789473684211E-03,2.63157894736842E-03,2.10526315789474E-03,1.57894736842105E-03,1.05263157894737E-03,5.26315789473686E-04,}
		valueWeights[dataIndex["grow_attack_rate"]] ={6.41025641025641E-03,6.28205128205128E-03,5,3.75,3.75,3.75,3.75,25,50,3.33333333333333,1.66666666666667,.005,4.87179487179487E-03,4.74358974358974E-03,4.61538461538462E-03,4.48717948717949E-03,4.35897435897436E-03,4.23076923076923E-03,4.1025641025641E-03,3.97435897435897E-03,3.84615384615385E-03,3.71794871794872E-03,3.58974358974359E-03,3.46153846153846E-03,3.33333333333333E-03,3.20512820512821E-03,3.07692307692308E-03,2.94871794871795E-03,2.82051282051282E-03,2.69230769230769E-03,2.56410256410256E-03,2.43589743589744E-03,2.30769230769231E-03,2.17948717948718E-03,2.05128205128205E-03,1.92307692307692E-03,1.7948717948718E-03,1.66666666666667E-03,1.53846153846154E-03,1.41025641025641E-03,1.28205128205128E-03,1.15384615384615E-03,1.02564102564103E-03,8.97435897435898E-04,7.69230769230769E-04,6.41025641025641E-04,5.12820512820513E-04,3.84615384615385E-04,2.56410256410256E-04,1.28205128205128E-04,}
		valueWeights[dataIndex["grow_damage_low"]] ={5.49450549450549E-02,5.38461538461538E-02,5.27472527472527E-02,5.16483516483516E-02,5.05494505494506E-02,4.94505494505494E-02,4.83516483516483E-02,4.72527472527473E-02,4.61538461538461E-02,4.50549450549451E-02,1,1,1,1,1,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,12.5,25,37.5,2.5,1.66666666666667,.833333333333333,1.42857142857143E-02,1.31868131868132E-02,1.20879120879121E-02,.010989010989011,9.89010989010989E-03,8.79120879120879E-03,7.69230769230769E-03,6.59340659340659E-03,5.49450549450549E-03,4.3956043956044E-03,3.2967032967033E-03,2.1978021978022E-03,1.0989010989011E-03,}
		valueWeights[dataIndex["grow_damage_high"]] ={5.49450549450549E-02,5.38461538461538E-02,5.27472527472527E-02,5.16483516483516E-02,5.05494505494506E-02,4.94505494505494E-02,4.83516483516483E-02,4.72527472527473E-02,4.61538461538461E-02,4.50549450549451E-02,1,1,1,1,1,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,12.5,25,37.5,2.5,1.66666666666667,.833333333333333,1.42857142857143E-02,1.31868131868132E-02,1.20879120879121E-02,.010989010989011,9.89010989010989E-03,8.79120879120879E-03,7.69230769230769E-03,6.59340659340659E-03,5.49450549450549E-03,4.3956043956044E-03,3.2967032967033E-03,2.1978021978022E-03,1.0989010989011E-03,}
		valueWeights[dataIndex["grow_dodge"]] ={6.09756097560976E-03,5,3.75,3.75,3.75,3.75,25,50,3.33333333333333,1.66666666666667,4.87804878048781E-03,4.75609756097561E-03,4.63414634146342E-03,4.51219512195122E-03,4.39024390243902E-03,4.26829268292683E-03,4.14634146341464E-03,4.02439024390244E-03,3.90243902439024E-03,3.78048780487805E-03,3.65853658536585E-03,3.53658536585366E-03,3.41463414634146E-03,3.29268292682927E-03,3.17073170731707E-03,3.04878048780488E-03,2.92682926829268E-03,2.80487804878049E-03,2.68292682926829E-03,2.5609756097561E-03,2.4390243902439E-03,2.31707317073171E-03,2.19512195121951E-03,2.07317073170732E-03,1.95121951219512E-03,1.82926829268293E-03,1.70731707317073E-03,1.58536585365854E-03,1.46341463414634E-03,1.34146341463415E-03,1.21951219512195E-03,1.09756097560976E-03,9.75609756097561E-04,8.53658536585367E-04,7.31707317073171E-04,6.09756097560976E-04,4.87804878048781E-04,3.65853658536586E-04,2.4390243902439E-04,1.21951219512195E-04,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={5.49450549450549E-02,5.38461538461538E-02,5.27472527472527E-02,5.16483516483516E-02,5.05494505494506E-02,4.94505494505494E-02,4.83516483516483E-02,4.72527472527473E-02,4.61538461538461E-02,4.50549450549451E-02,1,1,1,1,1,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,6.66666666666667,13.3333333333333,20,3.33333333333333,1.66666666666667,1.42857142857143E-02,1.31868131868132E-02,1.20879120879121E-02,.010989010989011,9.89010989010989E-03,8.79120879120879E-03,7.69230769230769E-03,6.59340659340659E-03,5.49450549450549E-03,4.3956043956044E-03,3.2967032967033E-03,2.1978021978022E-03,1.0989010989011E-03,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={3.67647058823529E-02,3.60294117647059E-02,3.52941176470588E-02,3.45588235294118E-02,3.38235294117647E-02,3.30882352941176E-02,3.23529411764706E-02,3.16176470588235E-02,1,1,1,1,1,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,4,8,12,16,2.5,1.66666666666667,.833333333333333,1.17647058823529E-02,1.10294117647059E-02,1.02941176470588E-02,9.55882352941177E-03,8.82352941176471E-03,8.08823529411765E-03,7.35294117647059E-03,6.61764705882353E-03,5.88235294117647E-03,5.14705882352941E-03,4.41176470588235E-03,3.67647058823529E-03,2.94117647058824E-03,2.20588235294118E-03,1.47058823529412E-03,7.3529411764706E-04,}
		valueWeights[dataIndex["grow_resis_silence"]] ={2.16450216450216E-02,2.12121212121212E-02,2.07792207792208E-02,2.03463203463203E-02,1.99134199134199E-02,1.94805194805195E-02,1.25,1.25,1.25,1.25,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,2.66666666666667,5.33333333333333,8,10.6666666666667,13.3333333333333,2.5,1.66666666666667,.833333333333333,9.09090909090909E-03,8.65800865800866E-03,8.22510822510823E-03,7.79220779220779E-03,7.35930735930736E-03,6.92640692640693E-03,6.49350649350649E-03,6.06060606060606E-03,5.62770562770563E-03,5.19480519480519E-03,4.76190476190476E-03,4.32900432900433E-03,3.8961038961039E-03,3.46320346320346E-03,3.03030303030303E-03,2.5974025974026E-03,2.16450216450216E-03,1.73160173160173E-03,1.2987012987013E-03,8.65800865800865E-04,4.32900432900432E-04,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={5.49450549450549E-02,5.38461538461538E-02,5.27472527472527E-02,5.16483516483516E-02,5.05494505494506E-02,4.94505494505494E-02,4.83516483516483E-02,4.72527472527473E-02,4.61538461538461E-02,4.50549450549451E-02,1,1,1,1,1,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,6.66666666666667,13.3333333333333,20,3.33333333333333,1.66666666666667,1.42857142857143E-02,1.31868131868132E-02,1.20879120879121E-02,.010989010989011,9.89010989010989E-03,8.79120879120879E-03,7.69230769230769E-03,6.59340659340659E-03,5.49450549450549E-03,4.3956043956044E-03,3.2967032967033E-03,2.1978021978022E-03,1.0989010989011E-03,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={3.67647058823529E-02,3.60294117647059E-02,3.52941176470588E-02,3.45588235294118E-02,3.38235294117647E-02,3.30882352941176E-02,3.23529411764706E-02,3.16176470588235E-02,1,1,1,1,1,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,4,8,12,16,2.5,1.66666666666667,.833333333333333,1.17647058823529E-02,1.10294117647059E-02,1.02941176470588E-02,9.55882352941177E-03,8.82352941176471E-03,8.08823529411765E-03,7.35294117647059E-03,6.61764705882353E-03,5.88235294117647E-03,5.14705882352941E-03,4.41176470588235E-03,3.67647058823529E-03,2.94117647058824E-03,2.20588235294118E-03,1.47058823529412E-03,7.3529411764706E-04,}
		

	---ÈçÒâÇ¬À¤´ü ¸¡Ó°¹Å¾µ
	elseif tmplID == talismans["TY_105_RuYiQianKunDai"].tmplID or tmplID == talismans["LS_105_FuYingGuJing"].tmplID then

		valueWeights[dataIndex["default_hp"]] ={9.46969696969697E-03,9.28030303030303E-03,9.09090909090909E-03,8.90151515151515E-03,2.5,2.5,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,2.14285714285714,12.5,25,37.5,3.33333333333333,1.66666666666667,6.06060606060606E-03,5.87121212121212E-03,5.68181818181818E-03,5.49242424242424E-03,5.3030303030303E-03,5.11363636363636E-03,4.92424242424242E-03,4.73484848484849E-03,4.54545454545455E-03,4.35606060606061E-03,4.16666666666667E-03,3.97727272727273E-03,3.78787878787879E-03,3.59848484848485E-03,3.40909090909091E-03,3.21969696969697E-03,3.03030303030303E-03,2.84090909090909E-03,2.65151515151515E-03,2.46212121212121E-03,2.27272727272727E-03,2.08333333333333E-03,1.89393939393939E-03,1.70454545454545E-03,1.51515151515152E-03,1.32575757575758E-03,1.13636363636364E-03,9.46969696969697E-04,7.57575757575757E-04,5.68181818181818E-04,3.78787878787879E-04,1.8939393939394E-04,}
		valueWeights[dataIndex["default_mp"]] ={2.16450216450216E-02,2.12121212121212E-02,2.07792207792208E-02,2.03463203463203E-02,1.99134199134199E-02,1.94805194805195E-02,1.25,1.25,1.25,1.25,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,2,1.5,1,.5,9.09090909090909E-03,8.65800865800866E-03,8.22510822510823E-03,7.79220779220779E-03,7.35930735930736E-03,6.92640692640693E-03,6.49350649350649E-03,6.06060606060606E-03,5.62770562770563E-03,5.19480519480519E-03,4.76190476190476E-03,4.32900432900433E-03,3.8961038961039E-03,3.46320346320346E-03,3.03030303030303E-03,2.5974025974026E-03,2.16450216450216E-03,1.73160173160173E-03,1.2987012987013E-03,8.65800865800865E-04,4.32900432900432E-04,}
		valueWeights[dataIndex["default_attack_rate"]] ={6.41025641025641E-03,6.28205128205128E-03,5,3.75,3.75,3.75,3.75,25,50,3.33333333333333,1.66666666666667,.005,4.87179487179487E-03,4.74358974358974E-03,4.61538461538462E-03,4.48717948717949E-03,4.35897435897436E-03,4.23076923076923E-03,4.1025641025641E-03,3.97435897435897E-03,3.84615384615385E-03,3.71794871794872E-03,3.58974358974359E-03,3.46153846153846E-03,3.33333333333333E-03,3.20512820512821E-03,3.07692307692308E-03,2.94871794871795E-03,2.82051282051282E-03,2.69230769230769E-03,2.56410256410256E-03,2.43589743589744E-03,2.30769230769231E-03,2.17948717948718E-03,2.05128205128205E-03,1.92307692307692E-03,1.7948717948718E-03,1.66666666666667E-03,1.53846153846154E-03,1.41025641025641E-03,1.28205128205128E-03,1.15384615384615E-03,1.02564102564103E-03,8.97435897435898E-04,7.69230769230769E-04,6.41025641025641E-04,5.12820512820513E-04,3.84615384615385E-04,2.56410256410256E-04,1.28205128205128E-04,}
		valueWeights[dataIndex["default_damage_low"]] ={4.76190476190476E-02,4.66666666666667E-02,4.57142857142857E-02,4.47619047619048E-02,4.38095238095238E-02,4.28571428571429E-02,4.19047619047619E-02,4.09523809523809E-02,.04,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7.5,15,22.5,30,2.5,1.66666666666667,.833333333333333,1.33333333333333E-02,1.23809523809524E-02,1.14285714285714E-02,1.04761904761905E-02,9.52380952380952E-03,8.57142857142857E-03,7.61904761904762E-03,6.66666666666667E-03,5.71428571428571E-03,4.76190476190476E-03,3.80952380952381E-03,2.85714285714286E-03,1.9047619047619E-03,9.52380952380951E-04,}
		valueWeights[dataIndex["default_damage_high"]] ={4.76190476190476E-02,4.66666666666667E-02,4.57142857142857E-02,4.47619047619048E-02,4.38095238095238E-02,4.28571428571429E-02,4.19047619047619E-02,4.09523809523809E-02,.04,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7.5,15,22.5,30,2.5,1.66666666666667,.833333333333333,1.33333333333333E-02,1.23809523809524E-02,1.14285714285714E-02,1.04761904761905E-02,9.52380952380952E-03,8.57142857142857E-03,7.61904761904762E-03,6.66666666666667E-03,5.71428571428571E-03,4.76190476190476E-03,3.80952380952381E-03,2.85714285714286E-03,1.9047619047619E-03,9.52380952380951E-04,}
		valueWeights[dataIndex["default_dodge"]] ={6.09756097560976E-03,5,3.75,3.75,3.75,3.75,25,50,3.33333333333333,1.66666666666667,4.87804878048781E-03,4.75609756097561E-03,4.63414634146342E-03,4.51219512195122E-03,4.39024390243902E-03,4.26829268292683E-03,4.14634146341464E-03,4.02439024390244E-03,3.90243902439024E-03,3.78048780487805E-03,3.65853658536585E-03,3.53658536585366E-03,3.41463414634146E-03,3.29268292682927E-03,3.17073170731707E-03,3.04878048780488E-03,2.92682926829268E-03,2.80487804878049E-03,2.68292682926829E-03,2.5609756097561E-03,2.4390243902439E-03,2.31707317073171E-03,2.19512195121951E-03,2.07317073170732E-03,1.95121951219512E-03,1.82926829268293E-03,1.70731707317073E-03,1.58536585365854E-03,1.46341463414634E-03,1.34146341463415E-03,1.21951219512195E-03,1.09756097560976E-03,9.75609756097561E-04,8.53658536585367E-04,7.31707317073171E-04,6.09756097560976E-04,4.87804878048781E-04,3.65853658536586E-04,2.4390243902439E-04,1.21951219512195E-04,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={.111111111111111,.108888888888889,.106666666666667,.104444444444444,.102222222222222,.1,9.77777777777778E-02,9.55555555555556E-02,9.33333333333333E-02,1,1,1,1,1,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,2.67857142857143,5.35714285714286,8.03571428571428,10.7142857142857,13.3928571428571,16.0714285714286,18.75,2,1.5,1,.5,.02,1.77777777777778E-02,1.55555555555556E-02,1.33333333333333E-02,1.11111111111111E-02,8.88888888888889E-03,6.66666666666667E-03,4.44444444444445E-03,2.22222222222222E-03,}
		valueWeights[dataIndex["default_resis_sleep"]] ={6.41025641025641E-02,6.28205128205128E-02,6.15384615384615E-02,6.02564102564103E-02,.058974358974359,5.76923076923077E-02,5.64102564102564E-02,5.51282051282051E-02,1.25,1.25,1.25,1.25,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.15384615384615,1.66666666666667,3.33333333333333,5,6.66666666666667,8.33333333333333,10,11.6666666666667,13.3333333333333,15,2,1.5,1,.5,1.53846153846154E-02,1.41025641025641E-02,1.28205128205128E-02,1.15384615384615E-02,1.02564102564103E-02,8.97435897435898E-03,7.69230769230769E-03,6.41025641025641E-03,5.12820512820513E-03,3.84615384615385E-03,2.56410256410257E-03,1.28205128205128E-03,}
		valueWeights[dataIndex["default_resis_silence"]] ={.178571428571429,.175,.171428571428571,.167857142857143,.164285714285714,.160714285714286,.157142857142857,.153571428571429,.15,.146428571428571,.142857142857143,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,2.5,1.66666666666667,.833333333333333,.025,2.14285714285714E-02,1.78571428571429E-02,1.42857142857143E-02,1.07142857142857E-02,7.14285714285714E-03,3.57142857142857E-03,}
		valueWeights[dataIndex["default_resis_weakness"]] ={.111111111111111,.108888888888889,.106666666666667,.104444444444444,.102222222222222,.1,9.77777777777778E-02,9.55555555555556E-02,9.33333333333333E-02,1,1,1,1,1,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,.9375,2.67857142857143,5.35714285714286,8.03571428571428,10.7142857142857,13.3928571428571,16.0714285714286,18.75,2,1.5,1,.5,.02,1.77777777777778E-02,1.55555555555556E-02,1.33333333333333E-02,1.11111111111111E-02,8.88888888888889E-03,6.66666666666667E-03,4.44444444444445E-03,2.22222222222222E-03,}
		valueWeights[dataIndex["default_resis_enlace"]] ={.178571428571429,.175,.171428571428571,.167857142857143,.164285714285714,.160714285714286,.157142857142857,.153571428571429,.15,.146428571428571,.142857142857143,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,3.57142857142857,7.14285714285714,10.7142857142857,14.2857142857143,17.8571428571429,21.4285714285714,2.5,1.66666666666667,.833333333333333,.025,2.14285714285714E-02,1.78571428571429E-02,1.42857142857143E-02,1.07142857142857E-02,7.14285714285714E-03,3.57142857142857E-03,}
		
		valueWeights[dataIndex["grow_hp"]] ={7.93650793650794E-03,7.77777777777778E-03,7.61904761904762E-03,1.66666666666667,1.66666666666667,1.66666666666667,2.5,2.5,2.5,2.5,2.5,2.5,25,50,5,5.55555555555556E-03,5.3968253968254E-03,5.23809523809524E-03,5.07936507936508E-03,4.92063492063492E-03,4.76190476190476E-03,4.6031746031746E-03,4.44444444444444E-03,4.28571428571429E-03,4.12698412698413E-03,3.96825396825397E-03,3.80952380952381E-03,3.65079365079365E-03,3.49206349206349E-03,3.33333333333333E-03,3.17460317460317E-03,3.01587301587302E-03,2.85714285714286E-03,2.6984126984127E-03,2.53968253968254E-03,2.38095238095238E-03,2.22222222222222E-03,2.06349206349206E-03,1.9047619047619E-03,1.74603174603175E-03,1.58730158730159E-03,1.42857142857143E-03,1.26984126984127E-03,1.11111111111111E-03,9.52380952380953E-04,7.93650793650793E-04,6.34920634920635E-04,4.76190476190476E-04,3.17460317460318E-04,1.58730158730159E-04,}
		valueWeights[dataIndex["grow_mp"]] ={1.66666666666667E-02,1.63333333333333E-02,.016,1.56666666666667E-02,1.53333333333333E-02,.015,1.66666666666667,1.66666666666667,1.66666666666667,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,12.5,25,37.5,2,1.5,1,.5,.008,7.66666666666667E-03,7.33333333333333E-03,.007,6.66666666666667E-03,6.33333333333333E-03,.006,5.66666666666667E-03,5.33333333333333E-03,.005,4.66666666666667E-03,4.33333333333333E-03,.004,3.66666666666667E-03,3.33333333333333E-03,.003,2.66666666666667E-03,2.33333333333333E-03,.002,1.66666666666667E-03,1.33333333333333E-03,9.99999999999999E-04,6.66666666666666E-04,3.33333333333332E-04,}
		valueWeights[dataIndex["grow_attack_rate"]] ={6.41025641025641E-03,6.28205128205128E-03,5,3.75,3.75,3.75,3.75,25,50,3.33333333333333,1.66666666666667,.005,4.87179487179487E-03,4.74358974358974E-03,4.61538461538462E-03,4.48717948717949E-03,4.35897435897436E-03,4.23076923076923E-03,4.1025641025641E-03,3.97435897435897E-03,3.84615384615385E-03,3.71794871794872E-03,3.58974358974359E-03,3.46153846153846E-03,3.33333333333333E-03,3.20512820512821E-03,3.07692307692308E-03,2.94871794871795E-03,2.82051282051282E-03,2.69230769230769E-03,2.56410256410256E-03,2.43589743589744E-03,2.30769230769231E-03,2.17948717948718E-03,2.05128205128205E-03,1.92307692307692E-03,1.7948717948718E-03,1.66666666666667E-03,1.53846153846154E-03,1.41025641025641E-03,1.28205128205128E-03,1.15384615384615E-03,1.02564102564103E-03,8.97435897435898E-04,7.69230769230769E-04,6.41025641025641E-04,5.12820512820513E-04,3.84615384615385E-04,2.56410256410256E-04,1.28205128205128E-04,}
		valueWeights[dataIndex["grow_damage_low"]] ={2.92397660818713E-02,2.86549707602339E-02,2.80701754385965E-02,2.74853801169591E-02,2.69005847953216E-02,2.63157894736842E-02,2.57309941520468E-02,2.51461988304094E-02,1,1,1,1,1,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,25,50,2.5,1.66666666666667,.833333333333333,1.05263157894737E-02,9.94152046783626E-03,9.35672514619883E-03,8.7719298245614E-03,8.18713450292398E-03,7.60233918128655E-03,7.01754385964912E-03,6.4327485380117E-03,5.84795321637427E-03,5.26315789473684E-03,4.67836257309942E-03,4.09356725146199E-03,3.50877192982456E-03,2.92397660818713E-03,2.33918128654971E-03,1.75438596491228E-03,1.16959064327485E-03,5.84795321637426E-04,}
		valueWeights[dataIndex["grow_damage_high"]] ={2.92397660818713E-02,2.86549707602339E-02,2.80701754385965E-02,2.74853801169591E-02,2.69005847953216E-02,2.63157894736842E-02,2.57309941520468E-02,2.51461988304094E-02,1,1,1,1,1,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,1.07142857142857,25,50,2.5,1.66666666666667,.833333333333333,1.05263157894737E-02,9.94152046783626E-03,9.35672514619883E-03,8.7719298245614E-03,8.18713450292398E-03,7.60233918128655E-03,7.01754385964912E-03,6.4327485380117E-03,5.84795321637427E-03,5.26315789473684E-03,4.67836257309942E-03,4.09356725146199E-03,3.50877192982456E-03,2.92397660818713E-03,2.33918128654971E-03,1.75438596491228E-03,1.16959064327485E-03,5.84795321637426E-04,}
		valueWeights[dataIndex["grow_dodge"]] ={6.09756097560976E-03,5,3.75,3.75,3.75,3.75,25,50,3.33333333333333,1.66666666666667,4.87804878048781E-03,4.75609756097561E-03,4.63414634146342E-03,4.51219512195122E-03,4.39024390243902E-03,4.26829268292683E-03,4.14634146341464E-03,4.02439024390244E-03,3.90243902439024E-03,3.78048780487805E-03,3.65853658536585E-03,3.53658536585366E-03,3.41463414634146E-03,3.29268292682927E-03,3.17073170731707E-03,3.04878048780488E-03,2.92682926829268E-03,2.80487804878049E-03,2.68292682926829E-03,2.5609756097561E-03,2.4390243902439E-03,2.31707317073171E-03,2.19512195121951E-03,2.07317073170732E-03,1.95121951219512E-03,1.82926829268293E-03,1.70731707317073E-03,1.58536585365854E-03,1.46341463414634E-03,1.34146341463415E-03,1.21951219512195E-03,1.09756097560976E-03,9.75609756097561E-04,8.53658536585367E-04,7.31707317073171E-04,6.09756097560976E-04,4.87804878048781E-04,3.65853658536586E-04,2.4390243902439E-04,1.21951219512195E-04,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={3.67647058823529E-02,3.60294117647059E-02,3.52941176470588E-02,3.45588235294118E-02,3.38235294117647E-02,3.30882352941176E-02,3.23529411764706E-02,3.16176470588235E-02,1,1,1,1,1,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,4,8,12,16,2.5,1.66666666666667,.833333333333333,1.17647058823529E-02,1.10294117647059E-02,1.02941176470588E-02,9.55882352941177E-03,8.82352941176471E-03,8.08823529411765E-03,7.35294117647059E-03,6.61764705882353E-03,5.88235294117647E-03,5.14705882352941E-03,4.41176470588235E-03,3.67647058823529E-03,2.94117647058824E-03,2.20588235294118E-03,1.47058823529412E-03,7.3529411764706E-04,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={2.16450216450216E-02,2.12121212121212E-02,2.07792207792208E-02,2.03463203463203E-02,1.99134199134199E-02,1.94805194805195E-02,1.25,1.25,1.25,1.25,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,3.18181818181818,2.66666666666667,5.33333333333333,8,10.6666666666667,13.3333333333333,2.5,1.66666666666667,.833333333333333,9.09090909090909E-03,8.65800865800866E-03,8.22510822510823E-03,7.79220779220779E-03,7.35930735930736E-03,6.92640692640693E-03,6.49350649350649E-03,6.06060606060606E-03,5.62770562770563E-03,5.19480519480519E-03,4.76190476190476E-03,4.32900432900433E-03,3.8961038961039E-03,3.46320346320346E-03,3.03030303030303E-03,2.5974025974026E-03,2.16450216450216E-03,1.73160173160173E-03,1.2987012987013E-03,8.65800865800865E-04,4.32900432900432E-04,}
		valueWeights[dataIndex["grow_resis_silence"]] ={5.49450549450549E-02,5.38461538461538E-02,5.27472527472527E-02,5.16483516483516E-02,5.05494505494506E-02,4.94505494505494E-02,4.83516483516483E-02,4.72527472527473E-02,4.61538461538461E-02,4.50549450549451E-02,1,1,1,1,1,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,6.66666666666667,13.3333333333333,20,3.33333333333333,1.66666666666667,1.42857142857143E-02,1.31868131868132E-02,1.20879120879121E-02,.010989010989011,9.89010989010989E-03,8.79120879120879E-03,7.69230769230769E-03,6.59340659340659E-03,5.49450549450549E-03,4.3956043956044E-03,3.2967032967033E-03,2.1978021978022E-03,1.0989010989011E-03,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={3.67647058823529E-02,3.60294117647059E-02,3.52941176470588E-02,3.45588235294118E-02,3.38235294117647E-02,3.30882352941176E-02,3.23529411764706E-02,3.16176470588235E-02,1,1,1,1,1,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,2.5,4,8,12,16,2.5,1.66666666666667,.833333333333333,1.17647058823529E-02,1.10294117647059E-02,1.02941176470588E-02,9.55882352941177E-03,8.82352941176471E-03,8.08823529411765E-03,7.35294117647059E-03,6.61764705882353E-03,5.88235294117647E-03,5.14705882352941E-03,4.41176470588235E-03,3.67647058823529E-03,2.94117647058824E-03,2.20588235294118E-03,1.47058823529412E-03,7.3529411764706E-04,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={5.49450549450549E-02,5.38461538461538E-02,5.27472527472527E-02,5.16483516483516E-02,5.05494505494506E-02,4.94505494505494E-02,4.83516483516483E-02,4.72527472527473E-02,4.61538461538461E-02,4.50549450549451E-02,1,1,1,1,1,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,2.05882352941176,6.66666666666667,13.3333333333333,20,3.33333333333333,1.66666666666667,1.42857142857143E-02,1.31868131868132E-02,1.20879120879121E-02,.010989010989011,9.89010989010989E-03,8.79120879120879E-03,7.69230769230769E-03,6.59340659340659E-03,5.49450549450549E-03,4.3956043956044E-03,3.2967032967033E-03,2.1978021978022E-03,1.0989010989011E-03,}

	---¾ÅÁúÉñ»ðÕÖ	Í¨ÓÃ
	elseif tmplID == talismans["US_105_JiuLongShenHuoZhao"].tmplID then
	
		valueWeights[dataIndex["default_hp"]]= {.012, .012, .012, .012, .011, 1.667, 1.667, 1.667, 1.667, 1.667, 1.667, 1.667, 1.667, 1.667, 1.667, 1.667, 1.667, 10, 10, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  }
		valueWeights[dataIndex["default_mp"]]= {.014, .014, .014, .013, .013, 1.667, 1.667, 1.667, 1.875, 1.875, 1.875, 1.875, 1.875, 1.875, 1.875, 1.875, 5, 10, 15, 20, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  }
		valueWeights[dataIndex["default_attack_rate"]]= {.01, .01, .01, .009, 2.5, 2.5, 2.143, 2.143, 2.143, 2.143, 2.143, 2.143, 2.143, 12.5, 25, 37.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  }
		valueWeights[dataIndex["default_damage_low"]]= {.042, .041, .04, .039, .038, .038, .037, .036, .035, 1.25, 1.25, 1.25, 1.25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 10, 15, 20, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  }
		valueWeights[dataIndex["default_damage_high"]]= {.042, .041, .04, .039, .038, .038, .037, .036, .035, 1.25, 1.25, 1.25, 1.25, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 5, 10, 15, 20, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  }
		valueWeights[dataIndex["default_dodge"]]= {.01, .01, .01, 1.667, 1.667, 1.667, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 7.5, 15, 22.5, 30, 2.5, 1.667, .833, .006, .006, .006, .006, .005, .005, .005, .005, .005, .004, .004, .004, .004, .004, .003, .003, .003, .003, .003, .002, .002, .002, .002, .002, .001, .001, .001, .001, .001, 0, 0,  }
		valueWeights[dataIndex["default_defense"]]= {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  }
		valueWeights[dataIndex["default_resis_stun"]]= {.139, .136, .133, .131, .128, .125, .122, .119, .117, .114, 1, 1, 1, 1, 1, .938, .938, .938, .938, .938, .938, .938, .938, .938, .938, .938, .938, .938, .938, .938, .938, 2.679, 5.357, 8.036, 10.714, 13.393, 16.071, 18.75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  }
		valueWeights[dataIndex["default_resis_sleep"]]= {.091, .089, .087, .085, .084, .082, .08, .078, .076, 1, 1, 1, 1, 1, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 2.083, 4.167, 6.25, 8.333, 10.417, 12.5, 14.583, 16.667, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  }
		valueWeights[dataIndex["default_resis_silence"]]= {.091, .089, .087, .085, .084, .082, .08, .078, .076, 1, 1, 1, 1, 1, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 2.083, 4.167, 6.25, 8.333, 10.417, 12.5, 14.583, 16.667, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  }
		valueWeights[dataIndex["default_resis_weakness"]]= {.139, .136, .133, .131, .128, .125, .122, .119, .117, .114, 1, 1, 1, 1, 1, .938, .938, .938, .938, .938, .938, .938, .938, .938, .938, .938, .938, .938, .938, .938, .938, 2.679, 5.357, 8.036, 10.714, 13.393, 16.071, 18.75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  }
		valueWeights[dataIndex["default_resis_enlace"]]= {.139, .136, .133, .131, .128, .125, .122, .119, .117, .114, 1, 1, 1, 1, 1, .938, .938, .938, .938, .938, .938, .938, .938, .938, .938, .938, .938, .938, .938, .938, .938, 2.679, 5.357, 8.036, 10.714, 13.393, 16.071, 18.75, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  }
		
		valueWeights[dataIndex["grow_hp"]]= {.011, .011, .01, .01, .01, 0, 0, 1.667, 1.667, 1.667, 1.667, 1.667, 1.667, 1.667, 1.667, 1.667, 25, 50, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  }
		valueWeights[dataIndex["grow_mp"]]= {.011, .011, .011, .011, 0, 0, 0, 1.875, 1.875, 1.875, 1.875, 1.875, 1.875, 1.875, 1.875, 12.5, 25, 37.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  }
		valueWeights[dataIndex["grow_attack_rate"]]= {.01, .01, .01, .009, 0, 0, 2.143, 2.143, 2.143, 2.143, 2.143, 2.143, 2.143, 12.5, 25, 37.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  }
		valueWeights[dataIndex["grow_damage_low"]]= {.026, .026, .025, .025, .024, .024, .023, .023, 0, 0, 0, 0, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 12.5, 25, 37.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  }
		valueWeights[dataIndex["grow_damage_high"]]= {.026, .026, .025, .025, .024, .024, .023, .023, 0, 0, 0, 0, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 1.071, 12.5, 25, 37.5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  }
		valueWeights[dataIndex["grow_dodge"]]= {.01, .01, .01, 0, 0, 0, 2.5, 2.5, 2.5, 2.5, 2.5, 2.5, 7.5, 15, 22.5, 30, 2.5, 1.667, .833, .006, .006, .006, .006, .005, .005, .005, .005, .005, .004, .004, .004, .004, .004, .003, .003, .003, .003, .003, .002, .002, .002, .002, .002, .001, .001, .001, .001, .001, 0, 0,  }
		valueWeights[dataIndex["grow_defense"]]= {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,  }
		valueWeights[dataIndex["grow_resis_stun"]]= {.042, .041, .04, .039, .038, .038, .037, .036, .035, 0, 0, 0, 0, 0, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 6.667, 13.333, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  }
		valueWeights[dataIndex["grow_resis_sleep"]]= {.029, .029, .028, .027, .027, .026, .026, 0, 0, 0, 0, 0, 2.692, 2.692, 2.692, 2.692, 2.692, 2.692, 2.692, 2.692, 2.692, 2.692, 2.692, 2.692, 2.692, 4, 8, 12, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  }
		valueWeights[dataIndex["grow_resis_silence"]]= {.029, .029, .028, .027, .027, .026, .026, 0, 0, 0, 0, 0, 2.692, 2.692, 2.692, 2.692, 2.692, 2.692, 2.692, 2.692, 2.692, 2.692, 2.692, 2.692, 2.692, 4, 8, 12, 16, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  }
		valueWeights[dataIndex["grow_resis_weakness"]]= {.042, .041, .04, .039, .038, .038, .037, .036, .035, 0, 0, 0, 0, 0, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 6.667, 13.333, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  }
		valueWeights[dataIndex["grow_resis_enlace"]]= {.042, .041, .04, .039, .038, .038, .037, .036, .035, 0, 0, 0, 0, 0, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 2.333, 6.667, 13.333, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  }

	---½ðºùÂ«	Í¨ÓÃ 2007£­11£­06²ÊÆ±¶þµÈ½±£¬ÖÁÉÙÊÇÏÉÆ·
	elseif tmplID == talismans["US_105_JinHuLu"].tmplID then

		valueWeights[dataIndex["default_hp"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,10,10,10,10,10,.1,.06,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_mp"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,10,10,10,10,10,.1,.06,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_attack_rate"]] ={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,10,10,.1,.06,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,10,10,.1,.06,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_dodge"]] ={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,0,0,}
		
		valueWeights[dataIndex["grow_hp"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_attack_rate"]] ={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]] ={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}

	---ÒõÑô¾µ	Í¨ÓÃ 2007£­11£­27 ´óÏä×Ó²ÊÆ±Ò»µÈ½±£¬ÖÁÉÙÊÇÏÉÆ·
	elseif tmplID == talismans["US_105_YinYangJing"].tmplID 
			or tmplID == talismans["US_105_YinYangJing_WZGL"].tmplID then
		
		valueWeights[dataIndex["default_hp"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,10,10,10,10,10,.1,.06,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_mp"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,10,10,10,10,10,.1,.06,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_attack_rate"]] ={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,10,10,.1,.06,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,10,10,.1,.06,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_dodge"]] ={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,0,0,}
		
		valueWeights[dataIndex["grow_hp"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_attack_rate"]] ={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]] ={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}

	---ó´Áúá¦	Í¨ÓÃ
	elseif tmplID == talismans["US_135_PanLongFan"].tmplID then

		valueWeights[dataIndex["default_hp"]] ={5.49450549450549E-02,5.38461538461538E-02,5.27472527472527E-02,5.16483516483516E-02,5.05494505494506E-02,4.94505494505494E-02,1.25,1.25,1.25,1.25,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,1.36363636363636,12.5,25,37.5,.714285714285714,.659340659340659,.604395604395604,.54945054945055,.494505494505495,.43956043956044,.384615384615385,.32967032967033,.274725274725275,.21978021978022,.164835164835165,.10989010989011,.054945054945055,1.42857142857143E-02,1.31868131868132E-02,1.20879120879121E-02,.010989010989011,9.89010989010989E-03,8.79120879120879E-03,7.69230769230769E-03,6.59340659340659E-03,5.49450549450549E-03,4.3956043956044E-03,3.2967032967033E-03,2.1978021978022E-03,1.0989010989011E-03,}
		valueWeights[dataIndex["default_mp"]] ={7.57575757575758E-02,7.42424242424242E-02,7.27272727272727E-02,7.12121212121212E-02,6.96969696969697E-02,6.81818181818182E-02,6.66666666666667E-02,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,1.25,7.5,15,22.5,30,.769230769230769,.705128205128205,.641025641025641,.576923076923077,.512820512820513,.448717948717949,.384615384615385,.32051282051282,.256410256410256,.192307692307692,.128205128205128,6.41025641025641E-02,1.66666666666667E-02,1.51515151515152E-02,1.36363636363636E-02,1.21212121212121E-02,1.06060606060606E-02,9.09090909090909E-03,7.57575757575758E-03,6.06060606060606E-03,4.54545454545455E-03,3.03030303030303E-03,1.51515151515151E-03,}
		valueWeights[dataIndex["default_attack_rate"]] ={4.76190476190476E-02,4.66666666666667E-02,4.57142857142857E-02,4.47619047619048E-02,4.38095238095238E-02,1.25,1.25,1.25,1.25,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,30,.666666666666667,.619047619047619,.571428571428571,.523809523809524,.476190476190476,.428571428571429,.380952380952381,.333333333333333,.285714285714286,.238095238095238,.19047619047619,.142857142857143,9.52380952380952E-02,4.76190476190476E-02,1.33333333333333E-02,1.23809523809524E-02,1.14285714285714E-02,1.04761904761905E-02,9.52380952380952E-03,8.57142857142857E-03,7.61904761904762E-03,6.66666666666667E-03,5.71428571428571E-03,4.76190476190476E-03,3.80952380952381E-03,2.85714285714286E-03,1.9047619047619E-03,9.52380952380951E-04,}
		valueWeights[dataIndex["default_damage_low"]] ={.238095238095238,.233333333333333,.228571428571429,.223809523809524,.219047619047619,.214285714285714,.20952380952381,.204761904761905,.2,.195238095238095,.19047619047619,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,7.5,15,22.5,30,1.42857142857143,1.19047619047619,.952380952380952,.714285714285714,.476190476190476,.238095238095238,2.85714285714286E-02,2.38095238095238E-02,.019047619047619,1.42857142857143E-02,9.52380952380952E-03,4.76190476190476E-03,}
		valueWeights[dataIndex["default_damage_high"]] ={.238095238095238,.233333333333333,.228571428571429,.223809523809524,.219047619047619,.214285714285714,.20952380952381,.204761904761905,.2,.195238095238095,.19047619047619,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,7.5,15,22.5,30,1.42857142857143,1.19047619047619,.952380952380952,.714285714285714,.476190476190476,.238095238095238,2.85714285714286E-02,2.38095238095238E-02,.019047619047619,1.42857142857143E-02,9.52380952380952E-03,4.76190476190476E-03,}
		valueWeights[dataIndex["default_dodge"]] ={4.76190476190476E-02,4.66666666666667E-02,4.57142857142857E-02,4.47619047619048E-02,4.38095238095238E-02,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,30,.625,.583333333333333,.541666666666667,.5,.458333333333333,.416666666666667,.375,.333333333333333,.291666666666667,.25,.208333333333333,.166666666666667,.125,8.33333333333334E-02,4.16666666666667E-02,1.33333333333333E-02,1.23809523809524E-02,1.14285714285714E-02,1.04761904761905E-02,9.52380952380952E-03,8.57142857142857E-03,7.61904761904762E-03,6.66666666666667E-03,5.71428571428571E-03,4.76190476190476E-03,3.80952380952381E-03,2.85714285714286E-03,1.9047619047619E-03,9.52380952380951E-04,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={.833333333333333,.816666666666667,.8,.783333333333333,.766666666666667,.75,.733333333333333,.716666666666667,.7,.683333333333333,.666666666666667,.65,.633333333333333,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,7.5,15,22.5,30,2.5,1.66666666666667,.833333333333333,.05,3.33333333333333E-02,1.66666666666667E-02,}
		valueWeights[dataIndex["default_resis_sleep"]] ={.833333333333333,.816666666666667,.8,.783333333333333,.766666666666667,.75,.733333333333333,.716666666666667,.7,.683333333333333,.666666666666667,.65,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,7.5,15,22.5,30,2,1.5,1,.5,.05,3.33333333333333E-02,1.66666666666667E-02,}
		valueWeights[dataIndex["default_resis_silence"]] ={.833333333333333,.816666666666667,.8,.783333333333333,.766666666666667,.75,.733333333333333,.716666666666667,.7,.683333333333333,.666666666666667,.65,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,7.5,15,22.5,30,2,1.5,1,.5,.05,3.33333333333333E-02,1.66666666666667E-02,}
		valueWeights[dataIndex["default_resis_weakness"]] ={.833333333333333,.816666666666667,.8,.783333333333333,.766666666666667,.75,.733333333333333,.716666666666667,.7,.683333333333333,.666666666666667,.65,.633333333333333,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,7.5,15,22.5,30,2.5,1.66666666666667,.833333333333333,.05,3.33333333333333E-02,1.66666666666667E-02,}
		valueWeights[dataIndex["default_resis_enlace"]] ={.833333333333333,.816666666666667,.8,.783333333333333,.766666666666667,.75,.733333333333333,.716666666666667,.7,.683333333333333,.666666666666667,.65,.633333333333333,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,7.5,15,22.5,30,2.5,1.66666666666667,.833333333333333,.05,3.33333333333333E-02,1.66666666666667E-02,}
		
		valueWeights[dataIndex["grow_hp"]] ={4.76190476190476E-02,4.66666666666667E-02,4.57142857142857E-02,4.47619047619048E-02,4.38095238095238E-02,4.28571428571429E-02,0,0,0,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,12.5,25,37.5,.666666666666667,.619047619047619,.571428571428571,.523809523809524,.476190476190476,.428571428571429,.380952380952381,.333333333333333,.285714285714286,.238095238095238,.19047619047619,.142857142857143,9.52380952380952E-02,4.76190476190476E-02,1.33333333333333E-02,1.23809523809524E-02,1.14285714285714E-02,1.04761904761905E-02,9.52380952380952E-03,8.57142857142857E-03,7.61904761904762E-03,6.66666666666667E-03,5.71428571428571E-03,4.76190476190476E-03,3.80952380952381E-03,2.85714285714286E-03,1.9047619047619E-03,9.52380952380951E-04,}
		valueWeights[dataIndex["grow_mp"]] ={5.49450549450549E-02,5.38461538461538E-02,5.27472527472527E-02,5.16483516483516E-02,5.05494505494506E-02,4.94505494505494E-02,0,0,0,0,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,.769230769230769,.705128205128205,.641025641025641,.576923076923077,.512820512820513,.448717948717949,.384615384615385,.32051282051282,.256410256410256,.192307692307692,.128205128205128,6.41025641025641E-02,1.42857142857143E-02,1.31868131868132E-02,1.20879120879121E-02,.010989010989011,9.89010989010989E-03,8.79120879120879E-03,7.69230769230769E-03,6.59340659340659E-03,5.49450549450549E-03,4.3956043956044E-03,3.2967032967033E-03,2.1978021978022E-03,1.0989010989011E-03,}
		valueWeights[dataIndex["grow_attack_rate"]] ={4.76190476190476E-02,4.66666666666667E-02,4.57142857142857E-02,4.47619047619048E-02,4.38095238095238E-02,0,0,0,0,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,30,.666666666666667,.619047619047619,.571428571428571,.523809523809524,.476190476190476,.428571428571429,.380952380952381,.333333333333333,.285714285714286,.238095238095238,.19047619047619,.142857142857143,9.52380952380952E-02,4.76190476190476E-02,1.33333333333333E-02,1.23809523809524E-02,1.14285714285714E-02,1.04761904761905E-02,9.52380952380952E-03,8.57142857142857E-03,7.61904761904762E-03,6.66666666666667E-03,5.71428571428571E-03,4.76190476190476E-03,3.80952380952381E-03,2.85714285714286E-03,1.9047619047619E-03,9.52380952380951E-04,}
		valueWeights[dataIndex["grow_damage_low"]] ={.138888888888889,.136111111111111,.133333333333333,.130555555555556,.127777777777778,.125,.122222222222222,.119444444444444,.116666666666667,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7.5,15,22.5,30,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,2.22222222222222E-02,1.94444444444444E-02,1.66666666666667E-02,1.38888888888889E-02,1.11111111111111E-02,8.33333333333333E-03,5.55555555555556E-03,2.77777777777778E-03,}
		valueWeights[dataIndex["grow_damage_high"]] ={.138888888888889,.136111111111111,.133333333333333,.130555555555556,.127777777777778,.125,.122222222222222,.119444444444444,.116666666666667,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7.5,15,22.5,30,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,2.22222222222222E-02,1.94444444444444E-02,1.66666666666667E-02,1.38888888888889E-02,1.11111111111111E-02,8.33333333333333E-03,5.55555555555556E-03,2.77777777777778E-03,}
		valueWeights[dataIndex["grow_dodge"]] ={4.76190476190476E-02,4.66666666666667E-02,4.57142857142857E-02,4.47619047619048E-02,4.38095238095238E-02,0,0,0,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,30,.625,.583333333333333,.541666666666667,.5,.458333333333333,.416666666666667,.375,.333333333333333,.291666666666667,.25,.208333333333333,.166666666666667,.125,8.33333333333334E-02,4.16666666666667E-02,1.33333333333333E-02,1.23809523809524E-02,1.14285714285714E-02,1.04761904761905E-02,9.52380952380952E-03,8.57142857142857E-03,7.61904761904762E-03,6.66666666666667E-03,5.71428571428571E-03,4.76190476190476E-03,3.80952380952381E-03,2.85714285714286E-03,1.9047619047619E-03,9.52380952380951E-04,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={.238095238095238,.233333333333333,.228571428571429,.223809523809524,.219047619047619,.214285714285714,.20952380952381,.204761904761905,.2,.195238095238095,.19047619047619,0,0,0,0,0,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,6.66666666666667,13.3333333333333,20,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,2.85714285714286E-02,2.38095238095238E-02,.019047619047619,1.42857142857143E-02,9.52380952380952E-03,4.76190476190476E-03,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={.138888888888889,.136111111111111,.133333333333333,.130555555555556,.127777777777778,.125,.122222222222222,.119444444444444,.116666666666667,.113888888888889,0,0,0,0,0,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,6.66666666666667,13.3333333333333,20,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,2.22222222222222E-02,1.94444444444444E-02,1.66666666666667E-02,1.38888888888889E-02,1.11111111111111E-02,8.33333333333333E-03,5.55555555555556E-03,2.77777777777778E-03,}
		valueWeights[dataIndex["grow_resis_silence"]] ={.138888888888889,.136111111111111,.133333333333333,.130555555555556,.127777777777778,.125,.122222222222222,.119444444444444,.116666666666667,.113888888888889,0,0,0,0,0,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,6.66666666666667,13.3333333333333,20,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,2.22222222222222E-02,1.94444444444444E-02,1.66666666666667E-02,1.38888888888889E-02,1.11111111111111E-02,8.33333333333333E-03,5.55555555555556E-03,2.77777777777778E-03,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={.238095238095238,.233333333333333,.228571428571429,.223809523809524,.219047619047619,.214285714285714,.20952380952381,.204761904761905,.2,.195238095238095,.19047619047619,0,0,0,0,0,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,6.66666666666667,13.3333333333333,20,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,2.85714285714286E-02,2.38095238095238E-02,.019047619047619,1.42857142857143E-02,9.52380952380952E-03,4.76190476190476E-03,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={.238095238095238,.233333333333333,.228571428571429,.223809523809524,.219047619047619,.214285714285714,.20952380952381,.204761904761905,.2,.195238095238095,.19047619047619,0,0,0,0,0,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,6.66666666666667,13.3333333333333,20,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,2.85714285714286E-02,2.38095238095238E-02,.019047619047619,1.42857142857143E-02,9.52380952380952E-03,4.76190476190476E-03,}

	---	135ÃÅÅÉ·¨±¦
	elseif tmplID == talismans["GW_135_TianMoZhi"].tmplID
			or tmplID == talismans["QY_135_RuoXue"].tmplID
			or tmplID == talismans["HH_135_BaJiYouLong"].tmplID
			or tmplID == talismans["TY_135_LiuDaoShengMie"].tmplID 
			or tmplID == talismans["GD_135_GuiQing"].tmplID
			or tmplID == talismans["FX_135_JiuHanNingBingCi"].tmplID
			or tmplID == talismans["JL_135_SangMenJian"].tmplID
			or tmplID == talismans["LS_135_LuoShu"].tmplID
			or tmplID == talismans["HG_135_JueYing"].tmplID
			or tmplID == talismans["TH_135_BiLuoFeiHong"].tmplID
			or tmplID == talismans["TH_135_XiHuangZhong"].tmplID
			or tmplID == talismans["CH_135_huangji"].tmplID
			or tmplID == talismans["QJ_135_chixinglongyan"].tmplID
			or tmplID == talismans["YZ_135_zhuihun"].tmplID
			then
	
		valueWeights[dataIndex["default_hp"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,10,10,10,10,10,.1,.06,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_mp"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,10,10,10,10,10,.1,.06,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_attack_rate"]] ={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,10,10,.1,.06,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,10,10,.1,.06,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_dodge"]] ={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,0,0,}
		
		valueWeights[dataIndex["grow_hp"]] ={4.76190476190476E-02,4.66666666666667E-02,4.57142857142857E-02,4.47619047619048E-02,4.38095238095238E-02,4.28571428571429E-02,0,0,0,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,12.5,25,37.5,.666666666666667,.619047619047619,.571428571428571,.523809523809524,.476190476190476,.428571428571429,.380952380952381,.333333333333333,.285714285714286,.238095238095238,.19047619047619,.142857142857143,9.52380952380952E-02,4.76190476190476E-02,1.33333333333333E-02,1.23809523809524E-02,1.14285714285714E-02,1.04761904761905E-02,9.52380952380952E-03,8.57142857142857E-03,7.61904761904762E-03,6.66666666666667E-03,5.71428571428571E-03,4.76190476190476E-03,3.80952380952381E-03,2.85714285714286E-03,1.9047619047619E-03,9.52380952380951E-04,}
		valueWeights[dataIndex["grow_mp"]] ={5.49450549450549E-02,5.38461538461538E-02,5.27472527472527E-02,5.16483516483516E-02,5.05494505494506E-02,4.94505494505494E-02,0,0,0,0,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,1.5,5,10,15,20,25,.769230769230769,.705128205128205,.641025641025641,.576923076923077,.512820512820513,.448717948717949,.384615384615385,.32051282051282,.256410256410256,.192307692307692,.128205128205128,6.41025641025641E-02,1.42857142857143E-02,1.31868131868132E-02,1.20879120879121E-02,.010989010989011,9.89010989010989E-03,8.79120879120879E-03,7.69230769230769E-03,6.59340659340659E-03,5.49450549450549E-03,4.3956043956044E-03,3.2967032967033E-03,2.1978021978022E-03,1.0989010989011E-03,}
		valueWeights[dataIndex["grow_attack_rate"]] ={4.76190476190476E-02,4.66666666666667E-02,4.57142857142857E-02,4.47619047619048E-02,4.38095238095238E-02,0,0,0,0,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,30,.666666666666667,.619047619047619,.571428571428571,.523809523809524,.476190476190476,.428571428571429,.380952380952381,.333333333333333,.285714285714286,.238095238095238,.19047619047619,.142857142857143,9.52380952380952E-02,4.76190476190476E-02,1.33333333333333E-02,1.23809523809524E-02,1.14285714285714E-02,1.04761904761905E-02,9.52380952380952E-03,8.57142857142857E-03,7.61904761904762E-03,6.66666666666667E-03,5.71428571428571E-03,4.76190476190476E-03,3.80952380952381E-03,2.85714285714286E-03,1.9047619047619E-03,9.52380952380951E-04,}
		valueWeights[dataIndex["grow_damage_low"]] ={.138888888888889,.136111111111111,.133333333333333,.130555555555556,.127777777777778,.125,.122222222222222,.119444444444444,.116666666666667,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7.5,15,22.5,30,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,2.22222222222222E-02,1.94444444444444E-02,1.66666666666667E-02,1.38888888888889E-02,1.11111111111111E-02,8.33333333333333E-03,5.55555555555556E-03,2.77777777777778E-03,}
		valueWeights[dataIndex["grow_damage_high"]] ={.138888888888889,.136111111111111,.133333333333333,.130555555555556,.127777777777778,.125,.122222222222222,.119444444444444,.116666666666667,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,7.5,15,22.5,30,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,2.22222222222222E-02,1.94444444444444E-02,1.66666666666667E-02,1.38888888888889E-02,1.11111111111111E-02,8.33333333333333E-03,5.55555555555556E-03,2.77777777777778E-03,}
		valueWeights[dataIndex["grow_dodge"]] ={4.76190476190476E-02,4.66666666666667E-02,4.57142857142857E-02,4.47619047619048E-02,4.38095238095238E-02,0,0,0,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,30,.625,.583333333333333,.541666666666667,.5,.458333333333333,.416666666666667,.375,.333333333333333,.291666666666667,.25,.208333333333333,.166666666666667,.125,8.33333333333334E-02,4.16666666666667E-02,1.33333333333333E-02,1.23809523809524E-02,1.14285714285714E-02,1.04761904761905E-02,9.52380952380952E-03,8.57142857142857E-03,7.61904761904762E-03,6.66666666666667E-03,5.71428571428571E-03,4.76190476190476E-03,3.80952380952381E-03,2.85714285714286E-03,1.9047619047619E-03,9.52380952380951E-04,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={.238095238095238,.233333333333333,.228571428571429,.223809523809524,.219047619047619,.214285714285714,.20952380952381,.204761904761905,.2,.195238095238095,.19047619047619,0,0,0,0,0,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,6.66666666666667,13.3333333333333,20,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,2.85714285714286E-02,2.38095238095238E-02,.019047619047619,1.42857142857143E-02,9.52380952380952E-03,4.76190476190476E-03,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={.138888888888889,.136111111111111,.133333333333333,.130555555555556,.127777777777778,.125,.122222222222222,.119444444444444,.116666666666667,.113888888888889,0,0,0,0,0,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,6.66666666666667,13.3333333333333,20,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,2.22222222222222E-02,1.94444444444444E-02,1.66666666666667E-02,1.38888888888889E-02,1.11111111111111E-02,8.33333333333333E-03,5.55555555555556E-03,2.77777777777778E-03,}
		valueWeights[dataIndex["grow_resis_silence"]] ={.138888888888889,.136111111111111,.133333333333333,.130555555555556,.127777777777778,.125,.122222222222222,.119444444444444,.116666666666667,.113888888888889,0,0,0,0,0,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,2.1875,6.66666666666667,13.3333333333333,20,1.11111111111111,.972222222222222,.833333333333333,.694444444444444,.555555555555556,.416666666666667,.277777777777778,.138888888888889,2.22222222222222E-02,1.94444444444444E-02,1.66666666666667E-02,1.38888888888889E-02,1.11111111111111E-02,8.33333333333333E-03,5.55555555555556E-03,2.77777777777778E-03,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={.238095238095238,.233333333333333,.228571428571429,.223809523809524,.219047619047619,.214285714285714,.20952380952381,.204761904761905,.2,.195238095238095,.19047619047619,0,0,0,0,0,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,6.66666666666667,13.3333333333333,20,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,2.85714285714286E-02,2.38095238095238E-02,.019047619047619,1.42857142857143E-02,9.52380952380952E-03,4.76190476190476E-03,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={.238095238095238,.233333333333333,.228571428571429,.223809523809524,.219047619047619,.214285714285714,.20952380952381,.204761904761905,.2,.195238095238095,.19047619047619,0,0,0,0,0,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,1.94444444444444,6.66666666666667,13.3333333333333,20,1.25,1.07142857142857,.892857142857143,.714285714285714,.535714285714286,.357142857142857,.178571428571429,2.85714285714286E-02,2.38095238095238E-02,.019047619047619,1.42857142857143E-02,9.52380952380952E-03,4.76190476190476E-03,}

	---135Í¨ÓÃ·¨±¦
	elseif tmplID == talismans["US_135_WuLongLun"].tmplID
			or tmplID == talismans["US_135_FanTianYin"].tmplID
			or tmplID == talismans["US_135_ChangMingDeng"].tmplID
			or tmplID == talismans["US_135_BaoKuZhiHun"].tmplID
			or tmplID == talismans["US_135_SheHun"].tmplID
			or tmplID == talismans["US_145_ShiXueZhu"].tmplID
			or tmplID == talismans["US_145_ShiHun"].tmplID
			or tmplID == talismans["US_145_TianYa"].tmplID
			or tmplID == talismans["foodtalisman"].tmplID then
	
		valueWeights[dataIndex["default_hp"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.1,0.1,0.5,1,1,1,1,1,1,1,1,1,0.5,0.1,0.1,}
		valueWeights[dataIndex["default_mp"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.1,0.1,0.5,1,1,1,1,1,1,1,1,1,0.5,0.1,0.1,}
		valueWeights[dataIndex["default_attack_rate"]] ={0,0,0,0,0,0,0,0,0,0,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,30,.666666666666667,.619047619047619,.571428571428571,.523809523809524,.476190476190476,.428571428571429,.380952380952381,.333333333333333,.285714285714286,.238095238095238,.19047619047619,.142857142857143,9.52380952380952E-02,4.76190476190476E-02,1.33333333333333E-02,1.23809523809524E-02,1.14285714285714E-02,1.04761904761905E-02,9.52380952380952E-03,8.57142857142857E-03,7.61904761904762E-03,6.66666666666667E-03,5.71428571428571E-03,4.76190476190476E-03,3.80952380952381E-03,2.85714285714286E-03,1.9047619047619E-03,9.52380952380951E-04,}
		valueWeights[dataIndex["default_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.1,0.1,0.5,1,1,1,1,1,1,1,1,1,0.5,0.1,0.1,}
		valueWeights[dataIndex["default_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.1,0.1,0.5,1,1,1,1,1,1,1,1,1,0.5,0.1,0.1,}
		valueWeights[dataIndex["default_dodge"]] ={0,0,0,0,0,0,0,0,0,0,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,30,.625,.583333333333333,.541666666666667,.5,.458333333333333,.416666666666667,.375,.333333333333333,.291666666666667,.25,.208333333333333,.166666666666667,.125,8.33333333333334E-02,4.16666666666667E-02,1.33333333333333E-02,1.23809523809524E-02,1.14285714285714E-02,1.04761904761905E-02,9.52380952380952E-03,8.57142857142857E-03,7.61904761904762E-03,6.66666666666667E-03,5.71428571428571E-03,4.76190476190476E-03,3.80952380952381E-03,2.85714285714286E-03,1.9047619047619E-03,9.52380952380951E-04,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,.666666666666667,.65,.633333333333333,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,7.5,15,22.5,30,2.5,1.66666666666667,.833333333333333,.05,3.33333333333333E-02,1.66666666666667E-02,}
		valueWeights[dataIndex["default_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,.666666666666667,.65,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,7.5,15,22.5,30,2,1.5,1,.5,.05,3.33333333333333E-02,1.66666666666667E-02,}
		valueWeights[dataIndex["default_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,.666666666666667,.65,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,7.5,15,22.5,30,2,1.5,1,.5,.05,3.33333333333333E-02,1.66666666666667E-02,}
		valueWeights[dataIndex["default_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,.666666666666667,.65,.633333333333333,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,7.5,15,22.5,30,2.5,1.66666666666667,.833333333333333,.05,3.33333333333333E-02,1.66666666666667E-02,}
		valueWeights[dataIndex["default_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,.666666666666667,.65,.633333333333333,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,7.5,15,22.5,30,2.5,1.66666666666667,.833333333333333,.05,3.33333333333333E-02,1.66666666666667E-02,}
		
		valueWeights[dataIndex["grow_hp"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,1,1,1,1,1,0.8,0.7,0.6,0.5,0.4,0.3,0.2,0.1,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,1,1,1,1,1,0.8,0.7,0.6,0.5,0.4,0.3,0.2,0.1,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_attack_rate"]] ={0,0,0,0,0,0,0,0,0,0,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,30,.666666666666667,.619047619047619,.571428571428571,.523809523809524,.476190476190476,.428571428571429,.380952380952381,.333333333333333,.285714285714286,.238095238095238,.19047619047619,.142857142857143,9.52380952380952E-02,4.76190476190476E-02,1.33333333333333E-02,1.23809523809524E-02,1.14285714285714E-02,1.04761904761905E-02,9.52380952380952E-03,8.57142857142857E-03,7.61904761904762E-03,6.66666666666667E-03,5.71428571428571E-03,4.76190476190476E-03,3.80952380952381E-03,2.85714285714286E-03,1.9047619047619E-03,9.52380952380951E-04,}
		valueWeights[dataIndex["grow_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,.19047619047619,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,7.5,15,22.5,30,1.42857142857143,1.19047619047619,.952380952380952,.714285714285714,.476190476190476,.238095238095238,2.85714285714286E-02,2.38095238095238E-02,.019047619047619,1.42857142857143E-02,9.52380952380952E-03,4.76190476190476E-03,}
		valueWeights[dataIndex["grow_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,.19047619047619,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.833333333333333,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,.882352941176471,7.5,15,22.5,30,1.42857142857143,1.19047619047619,.952380952380952,.714285714285714,.476190476190476,.238095238095238,2.85714285714286E-02,2.38095238095238E-02,.019047619047619,1.42857142857143E-02,9.52380952380952E-03,4.76190476190476E-03,}
		valueWeights[dataIndex["grow_dodge"]] ={0,0,0,0,0,0,0,0,0,0,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,30,.625,.583333333333333,.541666666666667,.5,.458333333333333,.416666666666667,.375,.333333333333333,.291666666666667,.25,.208333333333333,.166666666666667,.125,8.33333333333334E-02,4.16666666666667E-02,1.33333333333333E-02,1.23809523809524E-02,1.14285714285714E-02,1.04761904761905E-02,9.52380952380952E-03,8.57142857142857E-03,7.61904761904762E-03,6.66666666666667E-03,5.71428571428571E-03,4.76190476190476E-03,3.80952380952381E-03,2.85714285714286E-03,1.9047619047619E-03,9.52380952380951E-04,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,.666666666666667,.65,.633333333333333,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,4,8,12,16,2.5,1.66666666666667,.833333333333333,.05,3.33333333333333E-02,1.66666666666667E-02,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,.666666666666667,.65,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,4,8,12,16,2,1.5,1,.5,.05,3.33333333333333E-02,1.66666666666667E-02,}
		valueWeights[dataIndex["grow_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,.666666666666667,.65,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,4,8,12,16,2,1.5,1,.5,.05,3.33333333333333E-02,1.66666666666667E-02,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,.666666666666667,.65,.633333333333333,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,4,8,12,16,2.5,1.66666666666667,.833333333333333,.05,3.33333333333333E-02,1.66666666666667E-02,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,.666666666666667,.65,.633333333333333,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,1.75,4,8,12,16,2.5,1.66666666666667,.833333333333333,.05,3.33333333333333E-02,1.66666666666667E-02,}

	
	---Âä»êµÆ	Í¨ÓÃ 2008£­3£­18 ²ÊÆ±¶þµÈ½±£¬ÖÁÉÙÊÇÏÉÆ·
	elseif tmplID == talismans["US_105_LuoHunDeng"].tmplID then
		
	  valueWeights[dataIndex["default_hp"]]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,10,10,10,.1,.06,0,0,0,0,0,0,0,}
	  valueWeights[dataIndex["default_mp"]] 						={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,10,10,10,.1,.06,0,0,0,0,0,0,0,}
	  valueWeights[dataIndex["default_attack_rate"]] 		={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	  valueWeights[dataIndex["default_damage_low"]] 		={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,10,10,.1,.06,0,0,0,0,0,0,0,}
	  valueWeights[dataIndex["default_damage_high"]] 		={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,10,10,.1,.06,0,0,0,0,0,0,0,}
	  valueWeights[dataIndex["default_dodge"]] 					={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
    valueWeights[dataIndex["default_defense"]] 				={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
    valueWeights[dataIndex["default_resis_stun"]] 		={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,}
	  valueWeights[dataIndex["default_resis_sleep"]] 		={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,}
	  valueWeights[dataIndex["default_resis_silence"]] 	={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,}
	  valueWeights[dataIndex["default_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,}
	  valueWeights[dataIndex["default_resis_enlace"]] 	={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,}

		valueWeights[dataIndex["grow_hp"]]								={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]] 								={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_attack_rate"]] 			={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
  	valueWeights[dataIndex["grow_damage_low"]] 				={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]] 			={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]] 						={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]] 					={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] 				={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]] 			={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]] 		={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]] 		={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]] 			={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}

---ÕæÉ½ºÓÉÈ	Í¨ÓÃ && ÈÕÔÂ±¦¼ø && ÔªÏüµÆ
	elseif tmplID == talismans["US_105_ZhenShanHeShan"].tmplID
			or tmplID == talismans["US_75_RiYueBaoJian"].tmplID
			or tmplID == talismans["US_45_YuanXiaoDeng"].tmplID
			or tmplID == talismans["US_105_FuTu"].tmplID
			or tmplID == talismans["US_135_YinHunFan"].tmplID then
		
		valueWeights[dataIndex["default_hp"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,10,10,10,10,10,.1,.06,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_mp"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,10,10,10,10,10,.1,.06,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_attack_rate"]] ={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,10,10,.1,.06,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,10,10,.1,.06,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_dodge"]] ={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,11,11,10,.1,.08,.06,0,0,0,0,0,0,0,0,}
		
		valueWeights[dataIndex["grow_hp"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_attack_rate"]] ={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]] ={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		
---·É½£	Í¨ÓÃ
	elseif ZLuaTalGetTalTypeFromID(tmplID) == "feijian"
			or ZLuaTalGetTalTypeFromID(tmplID) == "yushou"
			or ZLuaTalGetTalTypeFromID(tmplID) == "chibang"
			then
		
		valueWeights[dataIndex["default_hp"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_mp"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_attack_rate"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_low"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_high"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_dodge"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_defense"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_stun"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_sleep"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_silence"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_weakness"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_enlace"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		
		valueWeights[dataIndex["grow_hp"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_attack_rate"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_stun"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}

	---ÆäËûÈÎºÎ
	else
		
		valueWeights[dataIndex["default_hp"]] 						= {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_mp"]] 						= {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_attack_rate"]] 		= {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_low"]] 		= {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_high"]] 		= {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_dodge"]] 					= {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_defense"]] 				= {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_stun"]] 		= {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_sleep"]] 		= {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_silence"]] 	= {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_weakness"]] = {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_enlace"]] 	= {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		
		valueWeights[dataIndex["grow_hp"]]								={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]] 								={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_attack_rate"]] 			={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]] 				={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]] 			={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]] 						={5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]] 					={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] 				={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]] 			={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]] 		={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]] 		={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]] 			={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,1,.1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}

	end
	
	---ÉèÖÃ·¨±¦¾«Á¦Öµ³õÊ¼ÖµÓë³É³¤ÂÊÏµÊý
	for _,v in pairs(talismans) do
		if tmplID == v.tmplID then
			---·É±¦
			if v.talType == "feijian" or v.talType == "yushou" or v.talType == "chibang" then
				valueWeights[dataIndex["default_stamina"]] 				= {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
				valueWeights[dataIndex["quo_stamina"]] 						= {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
			---ÈÎÎñ¸øµÄÐÂÊÖ·¨±¦
			elseif origin == talismanOrigin["quest"] and v.requireLevel == 1 then
				valueWeights[dataIndex["default_stamina"]] 				= {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
				valueWeights[dataIndex["quo_stamina"]] 						= {0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
			---ÈÎÎñ¸øµÄ15¼¶ÃÅÅÉ·¨±¦
			elseif origin == talismanOrigin["quest"] and v.requireLevel == 15 and v.school ~= 0 then
				valueWeights[dataIndex["default_stamina"]] 				= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
				valueWeights[dataIndex["quo_stamina"]] 						= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
			---ÐÂÊÖ·¨±¦
			elseif v.requireLevel == 1 then
				valueWeights[dataIndex["default_stamina"]] 				= {10,10,10,10,10,10,10,10,10,10,5,5,5,5,5,5,5,5,5,5,1,1,1,1,1,1,1,1,1,1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.02,.02,.02,.02,.02,.02,.02,.02,.02,.02,}
				valueWeights[dataIndex["quo_stamina"]] 						= {10,10,10,10,10,10,10,10,10,10,5,5,5,5,5,5,5,5,5,5,1,1,1,1,1,1,1,1,1,1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.02,.02,.02,.02,.02,.02,.02,.02,.02,.02,}
			---15¼¶ÃÅÅÉ·¨±¦
			elseif v.requireLevel == 15 and v.school ~= 0 then
				valueWeights[dataIndex["default_stamina"]] 				= {5,5,5,5,5,5,5,5,5,5,10,10,10,10,10,10,10,10,10,10,5,5,5,5,5,5,5,5,5,5,1,1,1,1,1,1,1,1,1,1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,}
				valueWeights[dataIndex["quo_stamina"]] 						= {5,5,5,5,5,5,5,5,5,5,10,10,10,10,10,10,10,10,10,10,5,5,5,5,5,5,5,5,5,5,1,1,1,1,1,1,1,1,1,1,.1,.1,.1,.1,.1,.1,.1,.1,.1,.1,}
			---75¼¶ÃÅÅÉ·¨±¦
			elseif v.requireLevel <= 75 and v.school ~= 0 then
				valueWeights[dataIndex["default_stamina"]] 				= {5,5,5,5,5,5,5,5,5,5,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,5,5,5,5,5,5,5,5,5,5,1,1,1,1,1,1,1,1,1,1,}
				valueWeights[dataIndex["quo_stamina"]] 						= {5,5,5,5,5,5,5,5,5,5,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,5,5,5,5,5,5,5,5,5,5,1,1,1,1,1,1,1,1,1,1,}
			---ËùÓÐ75¼¶ÒÔÉÏ·¨±¦  ËùÓÐÍ¨ÓÃ·¨±¦
			elseif v.requireLevel > 75 or v.school == 0 then
				valueWeights[dataIndex["default_stamina"]] 				= {5,5,5,5,5,5,5,5,5,5,10,10,10,10,10,10,10,10,10,10,15,15,15,15,15,15,15,15,15,15,5,5,5,5,5,5,5,5,5,5,1,1,1,1,1,1,1,1,1,1,}
				valueWeights[dataIndex["quo_stamina"]] 						= {5,5,5,5,5,5,5,5,5,5,10,10,10,10,10,10,10,10,10,10,15,15,15,15,15,15,15,15,15,15,5,5,5,5,5,5,5,5,5,5,1,1,1,1,1,1,1,1,1,1,}
			else
				valueWeights[dataIndex["default_stamina"]] 				= {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
				valueWeights[dataIndex["quo_stamina"]] 						= {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
			end 
		end
	end
	---Êý¾ÝÐÞ¸ÄÇø ½áÊø
	
	return valueWeights
end		



---È¡µÃÄ³¸öµÈ¼¶·¨±¦³åÌìÁúÄ§ÑªµÄ»¨·Ñ 
function ZLuaTalFeedCost(level)
	if type(level) ~= "number" or level < 1 or level > 50 then return 0 end
	local currentExp = {
		   0,  350,  600,  860, 1130, 1420, 1720, 2030, 2350, 2690,
		3030, 3390, 3940, 4350, 4780, 5220, 5680, 6160, 6630, 7130,
		7660, 8190, 8750, 9330,	9920,10550,11240,11970,12840,13900,
	 17270,21740,25470,32340,50570,100000,200000,200000000,200000000,200000000,
	 200000000,200000000,200000000,200000000,200000000,200000000,200000000,200000000,200000000,200000000}

	local accumulateExp = 0
	for i = 1,math.floor(level) do
		accumulateExp = accumulateExp + currentExp[i]
	end
	---Ò»¸öÌìÁúÄ§Ñª500µã£¬1¿éÇ®
	return math.ceil(accumulateExp/500)
end

---	¸ù¾Ý¸½¼ÓÈËÎï¼¼ÄÜµÄ¸½¼ÓÊôÐÔÈ¡µÃÎ´Ñ§»á¸Ã¼¼ÄÜµÄ¸½¼ÓÊôÐÔid
function ZLuaTalGetUnLearnID(id)
	if id == nil then return 0 end
	local transfer = {}
	transfer[337] = 1328
	transfer[338] = 1329
	transfer[339] = 1330
	transfer[340] = 1331
	transfer[341] = 1332
	transfer[342] = 1333
	transfer[343] = 1334
	transfer[344] = 1335
	transfer[345] = 1336
	transfer[346] = 1337
	transfer[347] = 1338
	transfer[348] = 1339
	transfer[349] = 1340
	transfer[350] = 1341
	transfer[351] = 1342
	transfer[352] = 1343
	transfer[353] = 1344
	transfer[354] = 1345
	transfer[355] = 1346
	transfer[356] = 1347
	transfer[357] = 1348
	transfer[358] = 1349
	transfer[359] = 1350
	transfer[360] = 1351
	transfer[361] = 1352
	transfer[362] = 1353
	transfer[363] = 1354
	transfer[364] = 1355
	transfer[365] = 1356
	transfer[366] = 1357
	transfer[367] = 1358
	transfer[368] = 1359
	transfer[369] = 1360
	transfer[370] = 1361
	transfer[371] = 1362
	transfer[372] = 1363
	transfer[373] = 1364
	transfer[374] = 1365
	transfer[375] = 1366
	transfer[376] = 1367
	transfer[377] = 1368
	transfer[378] = 1369
	transfer[379] = 1370
	transfer[380] = 1371
	transfer[381] = 1372
	transfer[382] = 1373
	transfer[383] = 1374
	transfer[384] = 1375
	transfer[385] = 1376
	transfer[386] = 1377
	transfer[387] = 1378
	transfer[388] = 1379
	transfer[389] = 1380
	transfer[390] = 1381
	transfer[391] = 1382
	transfer[392] = 1383
	transfer[393] = 1384
	transfer[394] = 1385
	transfer[395] = 1386
	transfer[396] = 1387
	transfer[397] = 1388
	transfer[398] = 1389
	transfer[872] = 1390
	transfer[873] = 1391
	transfer[874] = 1392
	transfer[875] = 1393
	transfer[876] = 1394
	transfer[877] = 1395
	transfer[878] = 1396
	transfer[879] = 1397
	transfer[880] = 1398
	transfer[881] = 1399
	transfer[882] = 1400
	transfer[883] = 1401
	transfer[884] = 1402
	transfer[885] = 1403
	transfer[886] = 1404
	transfer[887] = 1405
	transfer[888] = 1406
	transfer[889] = 1407
	transfer[890] = 1408
	transfer[891] = 1409
	transfer[892] = 1410
	transfer[893] = 1411
	transfer[894] = 1412
	transfer[895] = 1413
	transfer[896] = 1414
	transfer[897] = 1415
	transfer[898] = 1416
	transfer[899] = 1417
	transfer[900] = 1418
	transfer[901] = 1419
	transfer[902] = 1420
	transfer[903] = 1421
	transfer[904] = 1422
	transfer[905] = 1423
	transfer[906] = 1424
	transfer[907] = 1425
	transfer[1758] = 1782
	transfer[1759] = 1783
	transfer[1760] = 1784
	transfer[1761] = 1785
	transfer[1762] = 1786
	transfer[1763] = 1787
	transfer[1764] = 1788
	transfer[1765] = 1789
	transfer[1766] = 1790
	transfer[1767] = 1791
	transfer[1768] = 1792
	transfer[1769] = 1793
	transfer[1770] = 1794
	transfer[1771] = 1795
	transfer[1772] = 1796
	transfer[1773] = 1797
	transfer[1774] = 1798
	transfer[1775] = 1799
	transfer[1776] = 1800
	transfer[1777] = 1801
	transfer[1778] = 1802
	transfer[1779] = 1803
	transfer[1780] = 1804
	transfer[1781] = 1805
	transfer[3510] = 3558
	transfer[3511] = 3559
	transfer[3512] = 3560
	transfer[3513] = 3561
	transfer[3514] = 3562
	transfer[3515] = 3563
	transfer[3516] = 3564
	transfer[3517] = 3565
	transfer[3518] = 3566
	transfer[3519] = 3567
	transfer[3520] = 3568
	transfer[3521] = 3569
	transfer[3522] = 3570
	transfer[3523] = 3571
	transfer[3524] = 3572
	transfer[3525] = 3573
	transfer[3526] = 3574
	transfer[3527] = 3575
	transfer[3528] = 3576
	transfer[3529] = 3577
	transfer[3530] = 3578
	transfer[3531] = 3579
	transfer[3532] = 3580
	transfer[3533] = 3581
	transfer[3534] = 3582
	transfer[3535] = 3583
	transfer[3536] = 3584
	transfer[3537] = 3585
	transfer[3538] = 3586
	transfer[3539] = 3587
	transfer[3540] = 3588
	transfer[3541] = 3589
	transfer[3542] = 3590
	transfer[3543] = 3591
	transfer[3544] = 3592
	transfer[3545] = 3593
	transfer[3546] = 3594
	transfer[3547] = 3595
	transfer[3548] = 3596
	transfer[3549] = 3597
	transfer[3550] = 3598
	transfer[3551] = 3599
	transfer[3552] = 3600
	transfer[3553] = 3601
	transfer[3554] = 3602
	transfer[3555] = 3603
	transfer[3556] = 3604
	transfer[3557] = 3605
	transfer[3972] = 4020
	transfer[3973] = 4021
	transfer[3974] = 4022
	transfer[3975] = 4023
	transfer[3976] = 4024
	transfer[3977] = 4025
	transfer[3978] = 4026
	transfer[3979] = 4027
	transfer[3980] = 4028
	transfer[3981] = 4029
	transfer[3982] = 4030
	transfer[3983] = 4031
	transfer[3984] = 4032
	transfer[3985] = 4033
	transfer[3986] = 4034
	transfer[3987] = 4035
	transfer[3988] = 4036
	transfer[3989] = 4037
	transfer[3990] = 4038
	transfer[3991] = 4039
	transfer[3992] = 4040
	transfer[3993] = 4041
	transfer[3994] = 4042
	transfer[3995] = 4043
	transfer[3996] = 4044
	transfer[3997] = 4045
	transfer[3998] = 4046
	transfer[3999] = 4047
	transfer[4000] = 4048
	transfer[4001] = 4049
	transfer[4002] = 4050
	transfer[4003] = 4051
	transfer[4004] = 4052
	transfer[4005] = 4053
	transfer[4006] = 4054
	transfer[4007] = 4055
	transfer[4008] = 4056
	transfer[4009] = 4057
	transfer[4010] = 4058
	transfer[4011] = 4059
	transfer[4012] = 4060
	transfer[4013] = 4061
	transfer[4014] = 4062
	transfer[4015] = 4063
	transfer[4016] = 4064
	transfer[4017] = 4065
	transfer[4018] = 4066
	transfer[4019] = 4067	
	transfer[33387] = 33435
	transfer[33388] = 33436
	transfer[33389] = 33437
	transfer[33390] = 33438
	transfer[33391] = 33439
	transfer[33392] = 33440
	transfer[33393] = 33441
	transfer[33394] = 33442
	transfer[33395] = 33443
	transfer[33396] = 33444
	transfer[33397] = 33445
	transfer[33398] = 33446
	transfer[33399] = 33447
	transfer[33400] = 33448
	transfer[33401] = 33449
	transfer[33402] = 33450
	transfer[33403] = 33451
	transfer[33404] = 33452
	transfer[33405] = 33453
	transfer[33406] = 33454
	transfer[33407] = 33455
	transfer[33408] = 33456
	transfer[33409] = 33457
	transfer[33410] = 33458
	transfer[33411] = 33459
	transfer[33412] = 33460
	transfer[33413] = 33461
	transfer[33414] = 33462
	transfer[33415] = 33463
	transfer[33416] = 33464
	transfer[33417] = 33465
	transfer[33418] = 33466
	transfer[33419] = 33467
	transfer[33420] = 33468
	transfer[33421] = 33469
	transfer[33422] = 33470
	transfer[33423] = 33471
	transfer[33424] = 33472
	transfer[33425] = 33473
	transfer[33426] = 33474
	transfer[33427] = 33475
	transfer[33428] = 33476
	transfer[33429] = 33477
	transfer[33430] = 33478
	transfer[33431] = 33479
	transfer[33432] = 33480
	transfer[33433] = 33481
	transfer[33434] = 33482

  transfer[34035] = 34059
  transfer[34036]	= 34060
  transfer[34037]	= 34061
  transfer[34038]	= 34062
  transfer[34039]	= 34063
  transfer[34040]	= 34064
  transfer[34041]	= 34065
  transfer[34042]	= 34066
  transfer[34043]	= 34067
  transfer[34044]	= 34068
  transfer[34045]	= 34069
  transfer[34046]	= 34070
  transfer[34047]	= 34071
  transfer[34048]	= 34072
  transfer[34049]	= 34073
  transfer[34050]	= 34074
  transfer[34051]	= 34075
  transfer[34052]	= 34076
  transfer[34053]	= 34077
  transfer[34054]	= 34078
  transfer[34055]	= 34079
  transfer[34056]	= 34080
  transfer[34057]	= 34081
  transfer[34058]	= 34082

  transfer[34707] = 34755
  transfer[34708]	= 34756
  transfer[34709]	= 34757
  transfer[34710]	= 34758
  transfer[34711]	= 34759
  transfer[34712]	= 34760
  transfer[34713]	= 34761
  transfer[34714]	= 34762
  transfer[34715]	= 34763
  transfer[34716]	= 34764
  transfer[34717]	= 34765
  transfer[34718]	= 34766
  transfer[34719]	= 34767
  transfer[34720]	= 34768
  transfer[34721]	= 34769
  transfer[34722]	= 34770
  transfer[34723]	= 34771
  transfer[34724]	= 34772
  transfer[34725]	= 34773
  transfer[34726]	= 34774
  transfer[34727]	= 34775
  transfer[34728]	= 34776
  transfer[34729]	= 34777
  transfer[34730]	= 34778

  transfer[34731]	= 34779
  transfer[34732]	= 34780
  transfer[34733]	= 34781
  transfer[34734]	= 34782
  transfer[34735]	= 34783
  transfer[34736]	= 34784
  transfer[34737]	= 34785
  transfer[34738]	= 34786
  transfer[34739]	= 34787
  transfer[34740]	= 34788
  transfer[34741]	= 34789
  transfer[34742]	= 34790
  transfer[34743]	= 34791
  transfer[34744]	= 34792
  transfer[34745]	= 34793
  transfer[34746]	= 34794
  transfer[34747]	= 34795
  transfer[34748]	= 34796
  transfer[34749]	= 34797
  transfer[34750]	= 34798
  transfer[34751]	= 34799
  transfer[34752]	= 34800
  transfer[34753]	= 34801
  transfer[34754] = 34802

	return transfer[id]

end



-------Ë÷Òý¶¨ÒåÓë×ª»»------------------------------------------------------------------------------------------------

---·¨±¦dataË÷ÒýÊý×é
function ZLuaTalDataIndex()
	local dataIndex = {}
	dataIndex["reserve1"]							 = 0
	dataIndex["reserve2"]							 = 1
	dataIndex["tmpl_id"] 	             = 2
	dataIndex["skill_addon_id"]        = 3
	dataIndex["default_stamina"]       = 4
	dataIndex["quo_stamina"]           = 5
	dataIndex["default_hp"]            = 6
	dataIndex["default_mp"]            = 7
	dataIndex["default_attack_rate"]   = 8
	dataIndex["default_damage_low"]    = 9
	dataIndex["default_damage_high"]   = 10
	dataIndex["default_dodge"]         = 11
	dataIndex["default_defense"]       = 12
	dataIndex["default_resis_stun"]    = 13
	dataIndex["default_resis_sleep"]   = 14
	dataIndex["default_resis_silence"] = 15
	dataIndex["default_resis_weakness"]= 16
	dataIndex["default_resis_enlace"]  = 17
	dataIndex["grow_hp"]               = 18
	dataIndex["grow_mp"]               = 19
	dataIndex["grow_attack_rate"]      = 20
	dataIndex["grow_damage_low"]       = 21
	dataIndex["grow_damage_high"]      = 22
	dataIndex["grow_dodge"]            = 23
	dataIndex["grow_defense"]          = 24
	dataIndex["grow_resis_stun"]       = 25
	dataIndex["grow_resis_sleep"]      = 26
	dataIndex["grow_resis_silence"]    = 27
	dataIndex["grow_resis_weakness"]   = 28
	dataIndex["grow_resis_enlace"]     = 29
	dataIndex["addon_1_learn_level"]   = 30
	dataIndex["addon_1_id"]            = 31
	dataIndex["addon_2_learn_level"]   = 32
	dataIndex["addon_2_id"]            = 33
	dataIndex["addon_3_learn_level"]   = 34
	dataIndex["addon_3_id"]            = 35
	dataIndex["addon_4_learn_level"]   = 36
	dataIndex["addon_4_id"]            = 37
	dataIndex["addon_5_learn_level"]   = 38
	dataIndex["addon_5_id"]            = 39 
	dataIndex["addon_6_learn_level"]   = 40
	dataIndex["addon_6_id"]            = 41
	dataIndex["addon_7_learn_level"]   = 42
	dataIndex["addon_7_id"]            = 43
	dataIndex["addon_8_learn_level"]   = 44
	dataIndex["addon_8_id"]            = 45
	dataIndex["addon_9_learn_level"]   = 46
	dataIndex["addon_9_id"]            = 47
	dataIndex["addon_10_learn_level"]  = 48
	dataIndex["addon_10_id"]           = 49
	dataIndex["addon_11_learn_level"]  = 50
	dataIndex["addon_11_id"]           = 51
	dataIndex["artpath_1_level"]       = 52
	dataIndex["artpath_2_level"]       = 53
	dataIndex["artpath_3_level"]       = 54
	dataIndex["artpath_4_level"]       = 55
	dataIndex["confusion_1_level"]		 = 56
	dataIndex["confusion_1_quo"]			 = 57
	dataIndex["confusion_2_level"]		 = 58
	dataIndex["confusion_2_quo"]			 = 59
	dataIndex["confusion_3_level"]		 = 60
	dataIndex["confusion_3_quo"]			 = 61
	dataIndex["confusion_4_level"]		 = 62
	dataIndex["confusion_4_quo"]			 = 63
	dataIndex["confusion_5_level"]		 = 64
	dataIndex["confusion_5_quo"]			 = 65
	dataIndex["origin"]								 = 66
	dataIndex["birth_hour12"]					 = 67
	dataIndex["birth_min3"]						 = 68
	dataIndex["quality"]							 = 69
	dataIndex["speciality"]						 = 70
	dataIndex["buff_type"]						 = 71
	dataIndex["ext_id"]								 = 72
	dataIndex["skill_visible"]				 = 73
	dataIndex["combine_times"]				 = 74
	dataIndex["enchant_times"]				 = 75
	dataIndex["feed_baoqi"]						 = 76
	dataIndex["feed_xianpin"]					 = 77
	dataIndex["feed_shenpin"]					 = 78
	dataIndex["feed_cost"]						 = 79
	dataIndex["change_skillnum"]			 = 80
	dataIndex["feed_level_cost"]			 = 81
	dataIndex["is_fly"]			           = 82
	dataIndex["hp_rise"]			         = 83
	dataIndex["mp_rise"]			         = 84
	dataIndex["minattack_rise"]			   = 85
	dataIndex["maxattack_rise"]			   = 86
	dataIndex["stun_rise"]			       = 87
	dataIndex["weakness_rise"]			   = 88
	dataIndex["enlace_rise"]			     = 89
	dataIndex["silence_rise"]			     = 90
	dataIndex["sleep_rise"]			       = 91
	dataIndex["all_rise"]			         = 92
	dataIndex["hp_enhance"]						 = 93
	dataIndex["mp_enhance"]						 = 94
	dataIndex["atk_enhance"]					 = 95
	dataIndex["skillrefine_level"]		 = 96
	dataIndex["skillrefine_0"]				 = 97
	dataIndex["skillrefine_1"]				 = 98
	dataIndex["skillrefine_2"]				 = 99
	dataIndex["skillrefine_3"]				 = 100
	dataIndex["skillrefine_4"]				 = 101
	dataIndex["skillrefine_5"]				 = 102
	dataIndex["skillrefine_6"]				 = 103
	dataIndex["skillrefine_7"]				 = 104
	dataIndex["skillrefine_8"]				 = 105
	dataIndex["skillrefinetmp_1"]				 = 106
	dataIndex["skillrefinetmp_2"]				 = 107
	dataIndex["skillrefinetmp_3"]				 = 108
	dataIndex["skillrefinetmp_4"]				 = 109
	dataIndex["skillrefinetmp_5"]				 = 110
	dataIndex["skillrefinetmp_6"]				 = 111
	dataIndex["skillrefinetmp_7"]				 = 112
	dataIndex["skillrefinetmp_8"]				 = 113
	dataIndex["newaddon_id_1"]				 = 114
	dataIndex["newaddon_id_2"]				 = 115
	dataIndex["newaddon_id_3"]				 = 116
	dataIndex["newaddon_id_4"]				 = 117
	dataIndex["newaddon_id_5"]				 = 118
	dataIndex["reborncont"]						 = 119
	dataIndex["skillrefinecont"]			 = 120

	dataIndex["skill_addon_id_temp"]			 = 121
	dataIndex["addon_1_id_temp"]			 = 122
	dataIndex["addon_2_id_temp"]			 = 123
	dataIndex["addon_3_id_temp"]			 = 124
	dataIndex["addon_4_id_temp"]			 = 125
	dataIndex["addon_5_id_temp"]			 = 126
	dataIndex["addon_6_id_temp"]			 = 127
	dataIndex["addon_1_id_learn_level_temp"]			 = 128
	dataIndex["addon_2_id_learn_level_temp"]			 = 129
	dataIndex["addon_3_id_learn_level_temp"]			 = 130
	dataIndex["addon_4_id_learn_level_temp"]			 = 131
	dataIndex["addon_5_id_learn_level_temp"]			 = 132
	dataIndex["addon_6_id_learn_level_temp"]			 = 133

	
	local resisIndex = {}
	resisIndex["resis_stun"]     = 0
	resisIndex["resis_weakness"] = 1
	resisIndex["resis_enlace"]   = 2
	resisIndex["resis_silence"]  = 3
	resisIndex["resis_sleep"]    = 4

	-- data2IndexÖÐ´æµÄÊÇdataIndexÀïÐèÒª¿Í»§¶ËÖªµÀµÄ²ÎÊý(ÆúÓÃ!!!)
	local data2Index = {}
	data2Index["hp_enhance"]		= 0
	data2Index["mp_enhance"]		= 1
	data2Index["atk_enhance"]		= 2

	return dataIndex, resisIndex, data2Index
end


---gradesUpperLimit±íµÄË÷Òý£¬¸úËæZLuaTalGradesUpperLimitµÄ¶¨ÒåÖÐµÄ±íÖÐË³Ðò¶ø¸Ä±ä
---Í¬Ê±Óë¿Í»§¶ËÐü¸¡½çÃæÏÔÊ¾µÄ¹æÔòÒ²Ó¦ÓÃ´Ë¹æÔò
function ZLuaTalGradeIndex()
	local gradesIndex = {}
	gradesIndex["null"] 					= 0
	gradesIndex["all"] 						= 1
	gradesIndex["damage_low"] 		= 2
	gradesIndex["damage_high"] 		= 3
	gradesIndex["hp"] 						= 4
	gradesIndex["mp"] 						= 5
	gradesIndex["attack_rate"] 		 = 6
	gradesIndex["dodge"] 					 = 7
	gradesIndex["resis_stun"] 		 = 8
	gradesIndex["resis_weakness"]  = 9
	gradesIndex["resis_enlace"] 	 = 10
	gradesIndex["resis_silence"] 	= 11
	gradesIndex["resis_sleep"] 		= 12
	gradesIndex["defense"]				= 13

	return gradesIndex
end


---°ÑdataindexµÄË÷Òý×ª±äÎªgradesindexµÄË÷Òý
function ZLuaTalTransIndex(index18)
	local dataIndex = ZLuaTalDataIndex()
	local gradesIndex = ZLuaTalGradeIndex()
	if index18 == dataIndex["grow_hp"] then return gradesIndex["hp"] end
	if index18 == dataIndex["grow_mp"] then return gradesIndex["mp"] end
	if index18 == dataIndex["grow_damage_low"] then return gradesIndex["damage_low"] end
	if index18 == dataIndex["grow_damage_high"] then return gradesIndex["damage_high"] end
	if index18 == dataIndex["grow_resis_stun"] then return gradesIndex["resis_stun"] end
	if index18 == dataIndex["grow_resis_sleep"] then return gradesIndex["resis_sleep"] end
	if index18 == dataIndex["grow_resis_silence"] then return gradesIndex["resis_silence"] end
	if index18 == dataIndex["grow_resis_weakness"] then return gradesIndex["resis_weakness"] end
	if index18 == dataIndex["grow_resis_enlace"] then return gradesIndex["resis_enlace"] end
	return 0
end


---¶¨Òå·¨±¦À´Ô´£¨·þÎñÆ÷Ô¼¶¨£©¡£¶¨Òå¸úËæ³ÌÐòÔ¼¶¨¶ø¸Ä±ä¡£
function ZLuaTalServerOrigins()
	local serverOrigins = {}
	serverOrigins["null"] = -1
	serverOrigins["quest"] = 0
	serverOrigins["shop"] = 1
	serverOrigins["npc"] = 2	
	serverOrigins["lottery"] = 3
	serverOrigins["combine"] = 5	---Õâ¸ö²»ÊÇÓë³ÌÐòÔ¼¶¨µÄ¡£ÓÃÓÚÈÃtalisman_combine.lua¸æÖªtalisman.lua¸Ã·¨±¦ÊÇÑªÁ¶µÄ
	return serverOrigins
end

---¶¨Òå·¨±¦À´Ô´£¨¿Í»§¶ËÔ¼¶¨£©
function ZLuaTalClientOrigins()
	local clientOrigins = {}
	clientOrigins["quest"] = 0
	clientOrigins["shop"] = 1
	clientOrigins["npc"] = 2
	clientOrigins["combine"] = 3
	clientOrigins["lottery"] = 4
	clientOrigins["unknown"] = 5
	return clientOrigins
end	


---È¡µÃ·¨±¦À´Ô´£¬ÓÃÓÚ¿Í»§¶ËÏÔÊ¾
function ZLuaTalGenerateClientOrigin(serverOrigin)
	---¿Í»§¶ËÏÔÊ¾¹æÔò
	---0 ÈÎÎñ
	---1 ±¦¿â
	---2 ½»Ò×
	---3 ÑªÁ¶
	---4 ²ÊÆ±
	---5 Î´Öª
	local serverOrigins = ZLuaTalServerOrigins()
	local clientOrigins = ZLuaTalClientOrigins()
	local clientOrigin
	if serverOrigin == serverOrigins["quest"] then
		clientOrigin = clientOrigins["quest"]
	elseif serverOrigin == serverOrigins["shop"] then
		clientOrigin = clientOrigins["shop"]
	elseif serverOrigin == serverOrigins["npc"] then
		clientOrigin = clientOrigins["npc"]
	elseif serverOrigin == serverOrigins["combine"] then
		clientOrigin = clientOrigins["combine"]
	elseif serverOrigin == serverOrigins["lottery"] then
		clientOrigin = clientOrigins["shop"]
	else
		clientOrigin = clientOrigins["unknown"]
	end	
	return clientOrigin
end

---»¹Ô­·¨±¦À´Ô´£¬ÓÃÓÚ·þÎñÆ÷¶ËÅÐ¶Ï
function ZLuaTalGenerateServerOrigin(clientOrigin)
	local serverOrigins = ZLuaTalServerOrigins()
	local clientOrigins = ZLuaTalClientOrigins()
	local serverOrigin
	if clientOrigin == clientOrigins["quest"] then
		serverOrigin = serverOrigins["quest"]
	elseif clientOrigin == clientOrigins["shop"] then
		serverOrigin = serverOrigins["shop"]
	elseif clientOrigin == clientOrigins["npc"] then
		serverOrigin = serverOrigins["npc"]
	elseif clientOrigin == clientOrigins["combine"] then
		serverOrigin = serverOrigins["combine"]
	else
		serverOrigin = serverOrigins["null"]
	end
end


-------Éú³ÉÊôÐÔ------------------------------------------------------------------------------------------------------

---·¨±¦¸ÉÈÅÏµÊý
function ZLuaTalGenerateConfusion( tmplID, origin )
	local talismans = ZLuaTalAvailableTalismans()
	local origins = ZLuaTalServerOrigins()
	local confusion = {}
	local i
	for i=1,5 do
		confusion[i] = { quo = 1, level = 1 }
	end
	---È¡µÃÒ»Ð©³£Á¿
	local dataIndex,resisIndex = ZLuaTalDataIndex()
	---¸ÉÈÅÏµÊýÉÏÏÂÏÞ
	local low = 0.35
	local high = 1.65
	---±»¸ÉÈÅµÄµÈ¼¶
	local bound = {}
	bound[1] = { down = 2, up = 2 }
	bound[2] = { down = 3, up = 4 }
	bound[3] = { down = 5, up = 10 }
	bound[4] = { down = 11, up = 20 }
	bound[5] = { down = 21, up = 30 }

	---Éú³É¸ÉÈÅÏµÊý
	for i=1,5 do
		---Èç¹ûÊÇÈÎÎñ¸øµÄ·¨±¦£¬¾Í²»Éè¸ÉÈÅÏµÊý
		if ( tmplID == talismans["NW_1_BaGuaShi"].tmplID
		or tmplID == talismans["GW_15_ZhuQueYin"].tmplID
		or tmplID == talismans["HH_15_SheXinLing"].tmplID
		or tmplID == talismans["QY_15_LiuHeJing"].tmplID
		or tmplID == talismans["GD_15_XueYuGuPian"].tmplID
		or tmplID == talismans["FX_15_NanMingYin"].tmplID
		or tmplID == talismans["TY_15_LunHuiZhu"].tmplID
		or tmplID == talismans["SY_1_WuSeShi"].tmplID
		or tmplID == talismans["JL_15_NuShouFan"].tmplID
		or tmplID == talismans["LS_15_DanZhu"].tmplID
		or tmplID == talismans["HG_15_QiSha"].tmplID
		or tmplID == talismans["TH_15_JiuYinLuo"].tmplID
		or tmplID == talismans["TH_15_LeiGongMo"].tmplID
		or tmplID == talismans["CH_15_lvdouqi"].tmplID 
		or tmplID == talismans["LZ_1_TianLingShi"].tmplID
		or tmplID == talismans["QJ_15_tianjimu"].tmplID		
		or tmplID == talismans["YZ_15_lveying"].tmplID
		)and origin == origins["quest"] then
			confusion[i].quo = 1
			confusion[i].level = i

		elseif ZLuaTalGetTalTypeFromID(tmplID) == "feijian"
				or ZLuaTalGetTalTypeFromID(tmplID) == "yushou"
				or ZLuaTalGetTalTypeFromID(tmplID) == "chibang"
		then
			confusion[i].quo = 1
			confusion[i].level = i

		else
			confusion[i].quo = math.random() * ( high - low ) + low
			confusion[i].level = math.floor( math.random() * ( bound[i].up + 1 - bound[i].down ) + bound[i].down )
		end
	end

	return confusion
end	
	
---Éú³ÉÌØÐÔ
function ZLuaTalGenerateSpeciality(tmplID,origin,quality,allGrades)
	local dataIndex = ZLuaTalDataIndex()
	local gradeIndex = ZLuaTalGradeIndex()
	local origins = ZLuaTalServerOrigins()
	local talismans = ZLuaTalAvailableTalismans()
	local gradesUpperLimit = ZLuaTalGradesUpperLimit(tmplID)
	
	local generate = false
	
	---¹ºÂòµÄ°ËØÔÊ¯ÎåÉ«Ê¯ÓÐ¸ÅÂÊÓÐÌØÐÔ
	if (tmplID == talismans["NW_1_BaGuaShi"].tmplID or tmplID == talismans["SY_1_WuSeShi"].tmplID or tmplID == talismans["LZ_1_TianLingShi"].tmplID) and origin == origins["npc"] then
		if math.random() < 0.4 then
			generate = true
		end
	---²ÊÆ±µÄ°ËØÔÊ¯ÎåÉ«Ê¯±ØÈ»ÓÐÌØÐÔ
	elseif (tmplID == talismans["NW_1_BaGuaShi"].tmplID or tmplID == talismans["SY_1_WuSeShi"].tmplID or tmplID == talismans["LZ_1_TianLingShi"].tmplID) and origin == origins["lottery"] then
		generate = true
	end

	---Èç¹ûËæµ½ÁË¡°Éú³É¡±£¬¾ÍÉú³É£¬Éú³ÉµÄ´úÂë
	if generate then
		---´ÓËùÓÐµµÎ»ÖÐÈ¡³öÓÐÓÃµÄ
		local grades = {}
		grades[dataIndex["grow_hp"]]							= allGrades[dataIndex["grow_hp"]]
		grades[dataIndex["grow_mp"]]							= allGrades[dataIndex["grow_mp"]]
		grades[dataIndex["grow_damage_low"]]			= allGrades[dataIndex["grow_damage_low"]]
		grades[dataIndex["grow_damage_high"]]			= allGrades[dataIndex["grow_damage_high"]]
		grades[dataIndex["grow_resis_stun"]]			= allGrades[dataIndex["grow_resis_stun"]]
		grades[dataIndex["grow_resis_sleep"]]			= allGrades[dataIndex["grow_resis_sleep"]]
		grades[dataIndex["grow_resis_silence"]]		= allGrades[dataIndex["grow_resis_silence"]]
		grades[dataIndex["grow_resis_weakness"]]	= allGrades[dataIndex["grow_resis_weakness"]]
		grades[dataIndex["grow_resis_enlace"]]		= allGrades[dataIndex["grow_resis_enlace"]]

		---È¥µô2¸öÏà¶Ô×î¸ßµµÎ»µÄÊôÐÔ£¬ÔÚÊ£ÏÂµÄÀïÃæËæ
		---È¡µÃÏà¶Ô×î¸ßµµÎ»µÄÊôÐÔ
		local relativeMaxGradeDIdx = dataIndex["grow_damage_high"]
		local relativeMaxGradeGIdx = ZLuaTalTransIndex(relativeMaxGradeDIdx)
		for dataIdx,grade in pairs(grades) do
			local gradeIdx = ZLuaTalTransIndex(dataIdx)
			if grade/gradesUpperLimit[gradeIdx] > grades[relativeMaxGradeDIdx]/gradesUpperLimit[relativeMaxGradeGIdx] then
				relativeMaxGradeDIdx = dataIdx
				relativeMaxGradeGIdx = ZLuaTalTransIndex(relativeMaxGradeDIdx)
			end
		end
		---È¡µÃÏà¶Ô´Î¸ßµµÎ»µÄÊôÐÔ
		local relativeMax2GradeDIdx
		if relativeMaxGradeDIdx == dataIndex["grow_damage_high"] then 
			relativeMax2GradeDIdx = dataIndex["grow_hp"]
		else
			relativeMax2GradeDIdx = dataIndex["grow_damage_high"]
		end
		local relativeMax2GradeGIdx = ZLuaTalTransIndex(relativeMax2GradeDIdx)
		for dataIdx,grade in pairs(grades) do
			local gradeIdx = ZLuaTalTransIndex(dataIdx)
			if grade/gradesUpperLimit[gradeIdx] > grades[relativeMax2GradeDIdx]/gradesUpperLimit[relativeMax2GradeGIdx] and grade ~= maxGradeIndex then 
				relativeMax2GradeDIdx = dataIdx
				relativeMax2GradeGIdx = ZLuaTalTransIndex(relativeMax2GradeDIdx)
			end
		end
		---ÔÚÊ£ÏÂÀïÃæËæ»ú		
		local randomSpace = {}
		for index,grade in pairs(grades) do
			if index ~= maxGradeIndex and index ~= max2GradeIndex then 
				table.insert(randomSpace,index) 
			end
		end
		local res = math.floor(math.random()*table.getn(randomSpace)) + 1
		---°ÑÉú³ÉµÄdataË÷ÒýµÄ³É³¤·½Ïò·­ÒëÎªgradeË÷ÒýµÄ³É³¤·½Ïò£¬·µ»Ø
		return randomSpace[res]
	---Èç¹ûÃ»Ëæµ½¡°Éú³É¡±£¬¾Í·µ»Ø0£¬±íÊ¾ÎÞÌØÐÔ
	else
		return 0
	end	
end


---È¡µÃ·¨±¦Éú³ÉÊ±¼ä
function ZLuaTalGenerteBirthTime()
	local birthday = os.date("*t")
	local t = birthday.hour + birthday.min / 60 + birthday.sec / 3600	---µ±ÌìÕæÊµÊ±¼äµÄÐ¡Ê±Êý£¨¸¡µã£©£¬0¡«23.99999
	local hoursOneDayInGame = 4
	local gameTime = math.fmod(t,hoursOneDayInGame) / hoursOneDayInGame * 24	---ÓÎÏ·Ê±¼äµÄÐ¡Ê±Êý£¨¸¡µã£©£¬0¡«23.9999
	local gameHour12 = math.fmod( math.floor( ( gameTime + 1 ) / 2 ), 12 )
	local gameMin3 = math.floor( math.fmod( gameTime + 1 , 2 ) / 2 * 4 )
	return gameHour12, gameMin3
end

---È¡µÃ·¨±¦Æ·½×
function ZLuaTalGenerateQuality(tmplID, origin, grades, nGrades)
	local dataIndex = ZLuaTalDataIndex()
	local origins = ZLuaTalServerOrigins()
	local talismans = ZLuaTalAvailableTalismans()

	local sum
	sum=0
	
	sum = sum + grades[dataIndex["default_hp"]]
	sum = sum + grades[dataIndex["default_mp"]]
	sum = sum + grades[dataIndex["default_damage_low"]]
	sum = sum + grades[dataIndex["default_damage_high"]]
	sum = sum + grades[dataIndex["default_resis_stun"]]
	sum = sum + grades[dataIndex["default_resis_sleep"]]
	sum = sum + grades[dataIndex["default_resis_silence"]]
	sum = sum + grades[dataIndex["default_resis_weakness"]]
	sum = sum + grades[dataIndex["default_resis_enlace"]]

	sum = sum + grades[dataIndex["grow_hp"]] * 10
	sum = sum + grades[dataIndex["grow_mp"]] * 4
	sum = sum + grades[dataIndex["grow_damage_low"]] * 3
	sum = sum + grades[dataIndex["grow_damage_high"]] * 10
	sum = sum + grades[dataIndex["grow_resis_stun"]] * 4
	sum = sum + grades[dataIndex["grow_resis_sleep"]] * 4
	sum = sum + grades[dataIndex["grow_resis_silence"]] * 4
	sum = sum + grades[dataIndex["grow_resis_weakness"]] * 4
	sum = sum + grades[dataIndex["grow_resis_enlace"]] * 4
	
	local g = math.floor( sum / ( nGrades * 56 ) * 1000 )

	local l1,l2
	if tmplID == talismans["NW_1_BaGuaShi"].tmplID or tmplID == talismans["SY_1_WuSeShi"].tmplID or tmplID == talismans["LZ_1_TianLingShi"].tmplID then
		l1 = 140
		l2 = 160
	elseif ZLuaTalGetTalTypeFromID(tmplID) == "feijian"
			or ZLuaTalGetTalTypeFromID(tmplID) == "yushou"
			or ZLuaTalGetTalTypeFromID(tmplID) == "chibang"
			then
		l1 = 700
		l2 = 800
	elseif (tmplID == talismans["GW_15_ZhuQueYin"].tmplID or tmplID == talismans["JL_15_NuShouFan"].tmplID or tmplID == talismans["QJ_15_tianjimu"].tmplID) and origin == origins["quest"] then
		l1 = 300
		l2 = 400
	elseif (tmplID == talismans["HH_15_SheXinLing"].tmplID or tmplID == talismans["HG_15_QiSha"].tmplID or tmplID == talismans["YZ_15_lveying"].tmplID) and origin == origins["quest"] then
		l1 = 200
		l2 = 400
	elseif (tmplID == talismans["QY_15_LiuHeJing"].tmplID or tmplID == talismans["TH_15_JiuYinLuo"].tmplID) and origin == origins["quest"] then
		l1 = 300
		l2 = 400
	elseif (tmplID == talismans["TY_15_LunHuiZhu"].tmplID or tmplID == talismans["LS_15_DanZhu"].tmplID or tmplID == talismans["CH_15_lvdouqi"].tmplID) and origin == origins["quest"] then
		l1 = 300
		l2 = 400
	elseif (tmplID == talismans["GD_15_XueYuGuPian"].tmplID or tmplID == talismans["FX_15_NanMingYin"].tmplID or tmplID == talismans["TH_15_LeiGongMo"].tmplID) and origin == origins["quest"] then
		l1 = 300
		l2 = 400
	elseif tmplID == talismans["GW_15_ZhuQueYin"].tmplID or tmplID == talismans["JL_15_NuShouFan"].tmplID or tmplID == talismans["QJ_15_tianjimu"].tmplID then
		l1 = 267
		l2 = 294
	elseif tmplID == talismans["HH_15_SheXinLing"].tmplID or tmplID == talismans["HG_15_QiSha"].tmplID or tmplID == talismans["YZ_15_lveying"].tmplID then
		l1 = 264
		l2 = 283
	elseif tmplID == talismans["QY_15_LiuHeJing"].tmplID or tmplID == talismans["TH_15_JiuYinLuo"].tmplID then
		l1 = 252
		l2 = 271
	elseif tmplID == talismans["TY_15_LunHuiZhu"].tmplID or tmplID == talismans["LS_15_DanZhu"].tmplID or tmplID == talismans["CH_15_lvdouqi"].tmplID then
		l1 = 245
		l2 = 280
	elseif tmplID == talismans["GD_15_XueYuGuPian"].tmplID or tmplID == talismans["FX_15_NanMingYin"].tmplID or tmplID == talismans["TH_15_LeiGongMo"].tmplID then
		l1 = 245
		l2 = 280
	elseif tmplID == talismans["GW_75_SiLingFan"].tmplID or tmplID == talismans["JL_75_XueFengQiang"].tmplID or tmplID == talismans["QJ_75_luohouyu"].tmplID then
		l1 = 399
		l2 = 436
	elseif tmplID == talismans["HH_75_DuoQingHuan"].tmplID or tmplID == talismans["HG_75_PoJun"].tmplID or tmplID == talismans["YZ_75_poming"].tmplID then
		l1 = 407
		l2 = 435
	elseif tmplID == talismans["QY_75_QiXingJian"].tmplID or tmplID == talismans["TH_75_YaFeng"].tmplID then
		l1 = 385
		l2 = 415
	elseif tmplID == talismans["TY_75_JinGangChu"].tmplID or tmplID == talismans["LS_75_FanHunXiang"].tmplID or tmplID == talismans["CH_75_tuogu"].tmplID then
		l1 = 383
		l2 = 413
	elseif tmplID == talismans["GD_75_PoSha"].tmplID or tmplID == talismans["FX_75_HengTianChi"].tmplID or tmplID == talismans["TH_75_ChunQiuSuo"].tmplID then
		l1 = 383
		l2 = 413
	elseif tmplID == talismans["US_45_ShenMuShai"].tmplID
			or tmplID == talismans["US_45_ShenMuShai_WZGL"].tmplID
			or tmplID == talismans["US_45_YuanXiaoDeng"].tmplID then
		l1 = 300
		l2 = 374
	elseif tmplID == talismans["US_75_ShanHeShan"].tmplID
	or tmplID == talismans["US_75_WanDuDing"].tmplID
	or tmplID == talismans["US_75_YuJingPing"].tmplID then
		l1 = 317
		l2 = 348
	elseif tmplID == talismans["US_75_LingLongTa"].tmplID then
		l1 = 310
		l2 = 410
	elseif tmplID == talismans["GW_105_QianKunQingGuangJie"].tmplID or tmplID == talismans["JL_105_GangLan"].tmplID then
		l1 = 555
		l2 = 575
	elseif tmplID == talismans["HH_105_QianMeiLian"].tmplID or tmplID == talismans["HG_105_TanLang"].tmplID then
		l1 = 570
		l2 = 590
	elseif tmplID == talismans["QY_105_HuPoZhuLing"].tmplID or tmplID == talismans["TH_105_LiuLiZhan"].tmplID then
		l1 = 550
		l2 = 570
	elseif tmplID == talismans["TY_105_RuYiQianKunDai"].tmplID or tmplID == talismans["LS_105_FuYingGuJing"].tmplID or tmplID == talismans["CH_105_luobaojinqian"].tmplID or tmplID == talismans["QJ_105_jinyubian"].tmplID or tmplID == talismans["YZ_105_shuolei"].tmplID then
		l1 = 528
		l2 = 548
	elseif tmplID == talismans["GD_105_QianDuGu"].tmplID or tmplID == talismans["FX_105_HuoLingLong"].tmplID or tmplID == talismans["TH_105_ZiMuYinYangKou"].tmplID then
		l1 = 528
		l2 = 548
	elseif tmplID == talismans["US_105_JiuLongShenHuoZhao"].tmplID then
		l1 = 533
		l2 = 550
	elseif tmplID == talismans["US_105_JinHuLu"].tmplID then
		l1 = 552
		l2 = 600
	elseif tmplID == talismans["US_105_YinYangJing"].tmplID
			or tmplID == talismans["US_105_YinYangJing_WZGL"].tmplID then
		l1 = 552
		l2 = 630
	elseif tmplID == talismans["US_105_ZhenShanHeShan"].tmplID then
		l1 = 552
		l2 = 630
	elseif tmplID == talismans["US_135_PanLongFan"].tmplID then
		l1 = 638
		l2 = 660
	elseif tmplID == talismans["GW_135_TianMoZhi"].tmplID
	or tmplID == talismans["QY_135_RuoXue"].tmplID
	or tmplID == talismans["HH_135_BaJiYouLong"].tmplID
	or tmplID == talismans["TY_135_LiuDaoShengMie"].tmplID 
	or tmplID == talismans["GD_135_GuiQing"].tmplID
	or tmplID == talismans["FX_135_JiuHanNingBingCi"].tmplID
	or tmplID == talismans["JL_135_SangMenJian"].tmplID
	or tmplID == talismans["LS_135_LuoShu"].tmplID
	or tmplID == talismans["HG_135_JueYing"].tmplID
	or tmplID == talismans["TH_135_BiLuoFeiHong"].tmplID
	or tmplID == talismans["TH_135_XiHuangZhong"].tmplID
	or tmplID == talismans["CH_135_huangji"].tmplID
	or tmplID == talismans["QJ_135_chixinglongyan"].tmplID
	or tmplID == talismans["YZ_135_zhuihun"].tmplID
	then
		l1 = 725
		l2 = 775
	elseif tmplID == talismans["US_135_SheHun"].tmplID
	or tmplID == talismans["US_145_ShiXueZhu"].tmplID
	or tmplID == talismans["US_145_ShiHun"].tmplID 
	or tmplID == talismans["US_145_TianYa"].tmplID then
		l1 = 725
		l2 = 775
	elseif tmplID == talismans["US_135_WuLongLun"].tmplID
	or tmplID == talismans["US_135_FanTianYin"].tmplID
	or tmplID == talismans["US_135_ChangMingDeng"].tmplID
	or tmplID == talismans["US_135_BaoKuZhiHun"].tmplID
	or tmplID == talismans["US_75_RiYueBaoJian"].tmplID
	or tmplID == talismans["US_105_FuTu"].tmplID
	or tmplID == talismans["US_135_YinHunFan"].tmplID then
		l1 = 300
		l2 = 775
	elseif tmplID == talismans["US_105_LuoHunDeng"].tmplID then
		l1 = 590
		l2 = 650
	elseif tmplID == talismans["foodtalisman"].tmplID then
		l1 = 1
		l2 = 3
	else
		l1 = 750
		l2 = 800
	end
	local quality
	if g <= l1 then quality = 0
	elseif l1 < g and g <= l2 then quality = 1
	else quality = 2 end
	return quality
end

---È¡µÃ·¨±¦Æ·½×£¬´Ó¾ßÌå·¨±¦
function ZLuaTalGenerateQualityFromTalisman(tmplID, talisman)
	local dataIndex = ZLuaTalDataIndex()
	local nGrades, valueGrades = ZLuaTalGrades()
	local grades = {}
	grades[dataIndex["default_hp"]] 						= ZLuaTalGetGradeFromValue(dataIndex["default_hp"],talisman)
	grades[dataIndex["default_mp"]] 						= ZLuaTalGetGradeFromValue(dataIndex["default_mp"],talisman)
	grades[dataIndex["default_damage_low"]] 		= ZLuaTalGetGradeFromValue(dataIndex["default_damage_low"],talisman)
	grades[dataIndex["default_damage_high"]] 		= ZLuaTalGetGradeFromValue(dataIndex["default_damage_high"],talisman)
	grades[dataIndex["default_resis_stun"]] 		= ZLuaTalGetGradeFromValue(dataIndex["default_resis_stun"],talisman)
	grades[dataIndex["default_resis_sleep"]] 		= ZLuaTalGetGradeFromValue(dataIndex["default_resis_sleep"],talisman)
	grades[dataIndex["default_resis_silence"]] 	= ZLuaTalGetGradeFromValue(dataIndex["default_resis_silence"],talisman)
	grades[dataIndex["default_resis_weakness"]] = ZLuaTalGetGradeFromValue(dataIndex["default_resis_weakness"],talisman)
	grades[dataIndex["default_resis_enlace"]] 	= ZLuaTalGetGradeFromValue(dataIndex["default_resis_enlace"],talisman)
		
	grades[dataIndex["grow_hp"]] 								= ZLuaTalGetGradeFromValue(dataIndex["grow_hp"],talisman)
	grades[dataIndex["grow_mp"]] 								= ZLuaTalGetGradeFromValue(dataIndex["grow_mp"],talisman)
	grades[dataIndex["grow_damage_low"]] 				= ZLuaTalGetGradeFromValue(dataIndex["grow_damage_low"],talisman)
	grades[dataIndex["grow_damage_high"]] 			= ZLuaTalGetGradeFromValue(dataIndex["grow_damage_high"],talisman)
	grades[dataIndex["grow_resis_stun"]] 				= ZLuaTalGetGradeFromValue(dataIndex["grow_resis_stun"],talisman)
	grades[dataIndex["grow_resis_sleep"]] 			= ZLuaTalGetGradeFromValue(dataIndex["grow_resis_sleep"],talisman)
	grades[dataIndex["grow_resis_silence"]] 		= ZLuaTalGetGradeFromValue(dataIndex["grow_resis_silence"],talisman)
	grades[dataIndex["grow_resis_weakness"]] 		= ZLuaTalGetGradeFromValue(dataIndex["grow_resis_weakness"],talisman)
	grades[dataIndex["grow_resis_enlace"]] 			= ZLuaTalGetGradeFromValue(dataIndex["grow_resis_enlace"],talisman)

	local clientOrigin = talisman:QueryData(0,dataIndex["origin"])
	local serverOrigin = ZLuaTalGenerateServerOrigin(clientOrigin)
	return ZLuaTalGenerateQuality(tmplID, serverOrigin, grades, nGrades)
end

-------Ö÷Òª´¦Àíº¯Êý--------------------------------------------------------------------------------------------------

---·¨±¦ºÏ³É
function CombineTalismans(id1,id2, talisman1,talisman2,cid,output,locked1,locked2)
	-- Ã»×¢²á¹ýµÄ·¨±¦·É½££¬½ûÖ¹ÑªÁ¶
	local talismans = ZLuaTalAvailableTalismans()
	local Registed1 = false
	local Registed2 = false
	-- 75¼¶ºÍ105¼¶ÃÅÅÉ·¨±¦ÓÃÓÚÉñÆ·ÑªÁ¶±êÊ¶
	local RegistedMaxQuality = 0
	for _,v in pairs(talismans) do
		if v.tmplID == id1 then Registed1 = true	end
		if v.tmplID == id2 then Registed2 = true	end
		if v.tmplID == id1 and v.requireLevel == 75 and v.school ~= 0 then  RegistedMaxQuality = 1 end
		if v.tmplID == id1 and v.requireLevel == 105 and v.school ~= 0 then RegistedMaxQuality = 2 end
	end
	if Registed1 == false or Registed2 == false then
		output:CreateItem(-1)
		return
	end

	-- Èç¹ûÓÐËø¶¨µÄ·¨±¦£¬²»ÐíÑªÁ¶
	if locked1 == true or locked2 == true then
		output:CreateItem(-1)
	elseif cid == 11784 or cid == 12021 then
		---11784ÊÇ"ÏÔÓ°¼Á"£¬ÓÃÓÚ°Ñ·¨±¦µÄºóÌ¨Êý¾Ý°áµ½Ç°Ì¨
		---12021ÊÇ"³É³¤ÂÊÏÔÊ¾¼Á"£¬ÓÃÓÚ°Ñ·¨±¦Ã¿ÏîÊôÐÔµÄ³É³¤ÂÊµµÎ»ÊýÏÔÊ¾µ½ÏàÓ¦ÊôÐÔÉÏ
		ZLuaTalDebugDisplay(talisman1,id1,cid,output)
	elseif cid == 22017 and id2 == 19523 then
		--Ê¹ÓÃÁéÃ½Ö®ÍõºÍÊ³Îï·¨±¦£¬½«·¨±¦µÄÖ÷ÊôÐÔÒ»´ÎÐÔÎ¹Âú¡£
		ZLuaTalDebugMaxGrade(talisman1,id1,cid,output)
	elseif (cid == 50975 and ZLuaTalGenerateQualityFromTalisman(id1, talisman1) ~= 2 and RegistedMaxQuality == 1) or (cid == 50976 and ZLuaTalGenerateQualityFromTalisman(id1, talisman1) ~= 2 and RegistedMaxQuality == 2)
	 	---Ê¹ÓÃÑªÁ¶Æ·ÖÊÁéÃ½ºÍÈÎÒâ·¨±¦£¬½«ÃÅÅÉ75/105·¨±¦ÑªÁ¶³ÉÉñÆ·
	then
		ZLuaTalMaxQuality(talisman1,id1,cid,output)
	else
		--Õý³£ÑªÁ¶
		ZLuaTalisman_Combine(id1,id2, talisman1,talisman2,cid,output,locked1,locked2)
	end
end


---Ëæ»úº¯Êý£¬·µ»ØÖ¸¶¨·¨±¦Ä£°åµÄÃ¿¸öÊôÐÔËæ»úºóµÄ½á¹û
---»áµ÷ÓÃtalisman_addon.luaÀïÃæµÄZLuaTal_Get_RanPoint(array)º¯Êý
function ZLuaTalRandomResult(tmplID, origin)
	if ZLuaTalExistInTmplIDs(tmplID) == 0 then return nil end
	local talismans = ZLuaTalAvailableTalismans()
	---È¡µÃÒ»Ð©³£Á¿
	local dataIndex,resisIndex = ZLuaTalDataIndex()
	local origins = ZLuaTalServerOrigins()
	---È¡µÃ·¨±¦ÊôÐÔµÄµµÎ»
	local nGrades, valueGrades = ZLuaTalGrades()
	---È¡µÃ´Ë·¨±¦µÄËæ»úÈ¨ÖØ
	local valueWeights = ZLuaTalRandomWeight(tmplID, origin)
	if valueWeights == nil then return nil end

	local randomResult
	randomResult = {}
	
	---È¡µÃ·¨±¦¼¼ÄÜµÄ¸½¼ÓÊôÐÔID
	randomResult[dataIndex["skill_addon_id"]] = 399
	local tal
	for _,tal in pairs( ZLuaTalAvailableTalismans() ) do
		if tmplID == tal.tmplID then
			randomResult[dataIndex["skill_addon_id"]] = tal.skillAddon
			break
		end
	end

	---È¡µÃ¾«Á¦Öµ¡¢±¾ÌåÊôÐÔµÄ³õÊ¼ÖµÓë³É³¤ÂÊµµÎ»
	local grades
	grades = {}
	grades[dataIndex["default_stamina"]] 				= ZLuaTal_Get_RanPoint(valueWeights[dataIndex["default_stamina"]])
	grades[dataIndex["quo_stamina"]] 						= ZLuaTal_Get_RanPoint(valueWeights[dataIndex["quo_stamina"]])

	grades[dataIndex["default_hp"]] 						= ZLuaTal_Get_RanPoint(valueWeights[dataIndex["default_hp"]])
	grades[dataIndex["default_mp"]] 						= ZLuaTal_Get_RanPoint(valueWeights[dataIndex["default_mp"]])
	grades[dataIndex["default_attack_rate"]] 		= 1
	grades[dataIndex["default_damage_low"]] 		= ZLuaTal_Get_RanPoint(valueWeights[dataIndex["default_damage_low"]])
	grades[dataIndex["default_damage_high"]] 		= ZLuaTal_Get_RanPoint(valueWeights[dataIndex["default_damage_high"]])
	grades[dataIndex["default_dodge"]] 					= 1
	grades[dataIndex["default_defense"]] 				= 1
	grades[dataIndex["default_resis_stun"]] 		= ZLuaTal_Get_RanPoint(valueWeights[dataIndex["default_resis_stun"]])
	grades[dataIndex["default_resis_sleep"]] 		= ZLuaTal_Get_RanPoint(valueWeights[dataIndex["default_resis_sleep"]])
	grades[dataIndex["default_resis_silence"]] 	= ZLuaTal_Get_RanPoint(valueWeights[dataIndex["default_resis_silence"]])
	grades[dataIndex["default_resis_weakness"]] = ZLuaTal_Get_RanPoint(valueWeights[dataIndex["default_resis_weakness"]])
	grades[dataIndex["default_resis_enlace"]] 	= ZLuaTal_Get_RanPoint(valueWeights[dataIndex["default_resis_enlace"]])

	grades[dataIndex["grow_hp"]] 								= ZLuaTal_Get_RanPoint(valueWeights[dataIndex["grow_hp"]])
	grades[dataIndex["grow_mp"]] 								= ZLuaTal_Get_RanPoint(valueWeights[dataIndex["grow_mp"]])
	grades[dataIndex["grow_attack_rate"]] 			= 1
	grades[dataIndex["grow_damage_low"]] 				= ZLuaTal_Get_RanPoint(valueWeights[dataIndex["grow_damage_low"]])
	grades[dataIndex["grow_damage_high"]] 			= ZLuaTal_Get_RanPoint(valueWeights[dataIndex["grow_damage_high"]])
	grades[dataIndex["grow_dodge"]] 						= 1
	grades[dataIndex["grow_defense"]] 					= 1
	grades[dataIndex["grow_resis_stun"]] 				= ZLuaTal_Get_RanPoint(valueWeights[dataIndex["grow_resis_stun"]])
	grades[dataIndex["grow_resis_sleep"]] 			= ZLuaTal_Get_RanPoint(valueWeights[dataIndex["grow_resis_sleep"]])
	grades[dataIndex["grow_resis_silence"]] 		= ZLuaTal_Get_RanPoint(valueWeights[dataIndex["grow_resis_silence"]])
	grades[dataIndex["grow_resis_weakness"]] 		= ZLuaTal_Get_RanPoint(valueWeights[dataIndex["grow_resis_weakness"]])
	grades[dataIndex["grow_resis_enlace"]] 			= ZLuaTal_Get_RanPoint(valueWeights[dataIndex["grow_resis_enlace"]])



	---ÇóµÃ·¨±¦ÌØÐÔ
	local speciality = ZLuaTalGenerateSpeciality(tmplID, origin, quality, grades)

	---È¡µÃ¾«Á¦Öµ¡¢±¾ÌåÊôÐÔµÄ³õÊ¼ÖµÓë³É³¤ÂÊÕæÊµÖµ
	---Èç¹ûÊÇ·ÉÐÐ·¨±¦£¬Ôò³õÊ¼ÖµºÍ³É³¤ÂÊ¶¼Îª0
	if ZLuaTalGetTalTypeFromID(tmplID) == "feijian"
	or ZLuaTalGetTalTypeFromID(tmplID) == "yushou"
	or ZLuaTalGetTalTypeFromID(tmplID) == "chibang"
	then

		randomResult[dataIndex["is_fly"]] = 1
		randomResult[dataIndex["default_stamina"]] = 0
		randomResult[dataIndex["quo_stamina"]] = 0
	
		randomResult[dataIndex["default_hp"]] = 0
		randomResult[dataIndex["default_mp"]] = 0
		randomResult[dataIndex["default_attack_rate"]] = 0
		randomResult[dataIndex["default_damage_low"]] = 0
		randomResult[dataIndex["default_damage_high"]] = 0
		randomResult[dataIndex["default_dodge"]] = 0
		randomResult[dataIndex["default_defense"]] = 0
		randomResult[dataIndex["default_resis_stun"]] = 0
		randomResult[dataIndex["default_resis_sleep"]] = 0
		randomResult[dataIndex["default_resis_silence"]] = 0
		randomResult[dataIndex["default_resis_weakness"]] = 0
		randomResult[dataIndex["default_resis_enlace"]] = 0
	
		randomResult[dataIndex["grow_hp"]] = 0
		randomResult[dataIndex["grow_mp"]] = 0
		randomResult[dataIndex["grow_attack_rate"]] = 0
		randomResult[dataIndex["grow_damage_low"]] = 0
		randomResult[dataIndex["grow_damage_high"]] = 0
		randomResult[dataIndex["grow_dodge"]] = 0
		randomResult[dataIndex["grow_defense"]] = 0
		randomResult[dataIndex["grow_resis_stun"]] = 0
		randomResult[dataIndex["grow_resis_sleep"]] = 0
		randomResult[dataIndex["grow_resis_silence"]] = 0
		randomResult[dataIndex["grow_resis_weakness"]] = 0
		randomResult[dataIndex["grow_resis_enlace"]] = 0

	else
	randomResult[dataIndex["is_fly"]] = 0
	randomResult[dataIndex["default_stamina"]] = valueGrades[dataIndex["default_stamina"]][grades[dataIndex["default_stamina"]]]
	randomResult[dataIndex["quo_stamina"]] = valueGrades[dataIndex["quo_stamina"]][grades[dataIndex["quo_stamina"]]]

	randomResult[dataIndex["default_hp"]] = valueGrades[dataIndex["default_hp"]][grades[dataIndex["default_hp"]]]
	randomResult[dataIndex["default_mp"]] = valueGrades[dataIndex["default_mp"]][grades[dataIndex["default_mp"]]]
	randomResult[dataIndex["default_attack_rate"]] = valueGrades[dataIndex["default_attack_rate"]][grades[dataIndex["default_attack_rate"]]]
	randomResult[dataIndex["default_damage_low"]] = valueGrades[dataIndex["default_damage_low"]][grades[dataIndex["default_damage_low"]]]
	randomResult[dataIndex["default_damage_high"]] = valueGrades[dataIndex["default_damage_high"]][grades[dataIndex["default_damage_high"]]]
	randomResult[dataIndex["default_dodge"]] = valueGrades[dataIndex["default_dodge"]][grades[dataIndex["default_dodge"]]]
	randomResult[dataIndex["default_defense"]] = valueGrades[dataIndex["default_defense"]][grades[dataIndex["default_defense"]]]
	randomResult[dataIndex["default_resis_stun"]] = valueGrades[dataIndex["default_resis_stun"]][grades[dataIndex["default_resis_stun"]]]
	randomResult[dataIndex["default_resis_sleep"]] = valueGrades[dataIndex["default_resis_sleep"]][grades[dataIndex["default_resis_sleep"]]]
	randomResult[dataIndex["default_resis_silence"]] = valueGrades[dataIndex["default_resis_silence"]][grades[dataIndex["default_resis_silence"]]]
	randomResult[dataIndex["default_resis_weakness"]] = valueGrades[dataIndex["default_resis_weakness"]][grades[dataIndex["default_resis_weakness"]]]
	randomResult[dataIndex["default_resis_enlace"]] = valueGrades[dataIndex["default_resis_enlace"]][grades[dataIndex["default_resis_enlace"]]]

	randomResult[dataIndex["grow_hp"]] = valueGrades[dataIndex["grow_hp"]][grades[dataIndex["grow_hp"]]]
	randomResult[dataIndex["grow_mp"]] = valueGrades[dataIndex["grow_mp"]][grades[dataIndex["grow_mp"]]]
	randomResult[dataIndex["grow_attack_rate"]] = valueGrades[dataIndex["grow_attack_rate"]][grades[dataIndex["grow_attack_rate"]]]
	randomResult[dataIndex["grow_damage_low"]] = valueGrades[dataIndex["grow_damage_low"]][grades[dataIndex["grow_damage_low"]]]
	randomResult[dataIndex["grow_damage_high"]] = valueGrades[dataIndex["grow_damage_high"]][grades[dataIndex["grow_damage_high"]]]
	randomResult[dataIndex["grow_dodge"]] = valueGrades[dataIndex["grow_dodge"]][grades[dataIndex["grow_dodge"]]]
	randomResult[dataIndex["grow_defense"]] = valueGrades[dataIndex["grow_defense"]][grades[dataIndex["grow_defense"]]]
	randomResult[dataIndex["grow_resis_stun"]] = valueGrades[dataIndex["grow_resis_stun"]][grades[dataIndex["grow_resis_stun"]]]
	randomResult[dataIndex["grow_resis_sleep"]] = valueGrades[dataIndex["grow_resis_sleep"]][grades[dataIndex["grow_resis_sleep"]]]
	randomResult[dataIndex["grow_resis_silence"]] = valueGrades[dataIndex["grow_resis_silence"]][grades[dataIndex["grow_resis_silence"]]]
	randomResult[dataIndex["grow_resis_weakness"]] = valueGrades[dataIndex["grow_resis_weakness"]][grades[dataIndex["grow_resis_weakness"]]]
	randomResult[dataIndex["grow_resis_enlace"]] = valueGrades[dataIndex["grow_resis_enlace"]][grades[dataIndex["grow_resis_enlace"]]]
  end
	---È¡µÃËùÓÐ¸½¼ÓÊôÐÔÖµ¶Ô£¨Ñ§»áµÈ¼¶£¬¸½¼ÓÊôÐÔID£©
	local addonIDs,addonLevels
	addonIDs, addonLevels = ZLuaTal_Talisman_Trans_ID(tmplID, origin)
	
	---Ô¼¶¨£º
	---Èç¹û ZLuaTal_Talisman_Trans_ID ·µ»Ø false,false£¬ËµÃ÷tmplIDÓÐÎÊÌâ
	---Èç¹û ZLuaTal_Talisman_Trans_ID ·µ»Ø false,true£¬ ËµÃ÷ZLuaTal_Talisman_Trans_IDº¯ÊýÄÚ²¿ÓÐÆäËû´íÎó
	if addonIDs ~= false then  
		randomResult[dataIndex["addon_1_learn_level"]] = addonLevels[1]
		randomResult[dataIndex["addon_2_learn_level"]] = addonLevels[2]
		randomResult[dataIndex["addon_3_learn_level"]] = addonLevels[3]
		randomResult[dataIndex["addon_4_learn_level"]] = addonLevels[4]
		randomResult[dataIndex["addon_5_learn_level"]] = addonLevels[5]
		randomResult[dataIndex["addon_6_learn_level"]] = addonLevels[6]
		randomResult[dataIndex["addon_7_learn_level"]] = addonLevels[7]
		randomResult[dataIndex["addon_8_learn_level"]] = addonLevels[8]
		randomResult[dataIndex["addon_9_learn_level"]] = addonLevels[9]
		randomResult[dataIndex["addon_10_learn_level"]] = addonLevels[10]
		randomResult[dataIndex["addon_11_learn_level"]] = addonLevels[11]
		randomResult[dataIndex["addon_1_id"]] = addonIDs[1]
		randomResult[dataIndex["addon_2_id"]] = addonIDs[2]
		randomResult[dataIndex["addon_3_id"]] = addonIDs[3]
		randomResult[dataIndex["addon_4_id"]] = addonIDs[4]
		randomResult[dataIndex["addon_5_id"]] = addonIDs[5]
		randomResult[dataIndex["addon_6_id"]] = addonIDs[6]
		randomResult[dataIndex["addon_7_id"]] = addonIDs[7]
		randomResult[dataIndex["addon_8_id"]] = addonIDs[8]
		randomResult[dataIndex["addon_9_id"]] = addonIDs[9]
		randomResult[dataIndex["addon_10_id"]] = addonIDs[10]
		randomResult[dataIndex["addon_11_id"]] = addonIDs[11]
	else
		randomResult[dataIndex["addon_1_learn_level"]] = 0
		randomResult[dataIndex["addon_2_learn_level"]] = 0
		randomResult[dataIndex["addon_3_learn_level"]] = 0
		randomResult[dataIndex["addon_4_learn_level"]] = 0
		randomResult[dataIndex["addon_5_learn_level"]] = 0
		randomResult[dataIndex["addon_6_learn_level"]] = 0
		randomResult[dataIndex["addon_7_learn_level"]] = 0
		randomResult[dataIndex["addon_8_learn_level"]] = 0
		randomResult[dataIndex["addon_9_learn_level"]] = 0
		randomResult[dataIndex["addon_10_learn_level"]] = 0
		randomResult[dataIndex["addon_11_learn_level"]] = 0
		randomResult[dataIndex["addon_1_id"]] = 0
		randomResult[dataIndex["addon_2_id"]] = 0
		randomResult[dataIndex["addon_3_id"]] = 0
		randomResult[dataIndex["addon_4_id"]] = 0
		randomResult[dataIndex["addon_5_id"]] = 0
		randomResult[dataIndex["addon_6_id"]] = 0
		randomResult[dataIndex["addon_7_id"]] = 0
		randomResult[dataIndex["addon_8_id"]] = 0
		randomResult[dataIndex["addon_9_id"]] = 0
		randomResult[dataIndex["addon_10_id"]] = 0
		randomResult[dataIndex["addon_11_id"]] = 0
	end
	
	---ÇóµÃ¸½¼ÓÊôÐÔ¸öÊý
	local addonnumber=0
  for i = 1,7 do
    if addonIDs[i] ~=nil then
    addonnumber=addonnumber+1
    end
  end
  ---ÇóµÃ·¨±¦×ÛºÏÆ·½×
  local quality
  if randomResult[dataIndex["is_fly"]]==1 and addonnumber<4 then
  	quality = 0
  elseif randomResult[dataIndex["is_fly"]]==1 and addonnumber==4 then
  	quality = 1
  elseif randomResult[dataIndex["is_fly"]]==1 and addonnumber>4 then
  	quality = 2
  elseif randomResult[dataIndex["is_fly"]] == 0 then
  	quality = ZLuaTalGenerateQuality(tmplID, origin, grades, nGrades)
  end	
  
	
	
	---½â¾ö07ÄêÔªµ©Ñ©ÈË»î¶¯µôÂä¾ÅÁúÉñ»ðÕÖÃ»ÊôÐÔÎÊÌâ£¬ÓÃ¶Ò»»ÈÎÎñ»»³ÉÓÐÊôÐÔµÄ£¬Îª·ÀË¢Îª°Ñ¼¼ÄÜÖÃÎª¹Ì¶¨×éºÏ uncomplete
	if tmplID == talismans["US_105_JiuLongShenHuoZhao"].tmplID then
		randomResult[dataIndex["addon_1_learn_level"]] = 5
		randomResult[dataIndex["addon_2_learn_level"]] = 10
		randomResult[dataIndex["addon_3_learn_level"]] = 15
		randomResult[dataIndex["addon_4_learn_level"]] = 0
		randomResult[dataIndex["addon_5_learn_level"]] = 0
		randomResult[dataIndex["addon_6_learn_level"]] = 0
		randomResult[dataIndex["addon_7_learn_level"]] = 0
		randomResult[dataIndex["addon_8_learn_level"]] = 0
		randomResult[dataIndex["addon_9_learn_level"]] = 0
		randomResult[dataIndex["addon_10_learn_level"]] = 0
		randomResult[dataIndex["addon_11_learn_level"]] = 0
		randomResult[dataIndex["addon_4_id"]] = 0
		randomResult[dataIndex["addon_5_id"]] = 0
		randomResult[dataIndex["addon_6_id"]] = 0
		randomResult[dataIndex["addon_7_id"]] = 0
		randomResult[dataIndex["addon_8_id"]] = 0
		randomResult[dataIndex["addon_9_id"]] = 0
		randomResult[dataIndex["addon_10_id"]] = 0
		randomResult[dataIndex["addon_11_id"]] = 0
		
		local r = math.random()
		if r < 0.2 then
			---ÇàÔÆ
			randomResult[dataIndex["addon_1_id"]] = 369
			randomResult[dataIndex["addon_2_id"]] = 375
			randomResult[dataIndex["addon_3_id"]] = 380
		elseif r < 0.4 then
			---¹íÍõ
			randomResult[dataIndex["addon_1_id"]] = 340
			randomResult[dataIndex["addon_2_id"]] = 345
			randomResult[dataIndex["addon_3_id"]] = 351
		elseif r < 0.6 then
			---ÌìÒô
			randomResult[dataIndex["addon_1_id"]] = 384
			randomResult[dataIndex["addon_2_id"]] = 389
			randomResult[dataIndex["addon_3_id"]] = 394
		elseif r < 0.8 then
			---¹íµÀ
			randomResult[dataIndex["addon_1_id"]] = 1758
			randomResult[dataIndex["addon_2_id"]] = 1759
			randomResult[dataIndex["addon_3_id"]] = 1760
		else
			---ºÏ»¶
			randomResult[dataIndex["addon_1_id"]] = 354
			randomResult[dataIndex["addon_2_id"]] = 361
			randomResult[dataIndex["addon_3_id"]] = 365
		end
	end
				
	

	return randomResult, quality, speciality
end

---·¨±¦³õÊ¼»¯
function ZLuaTalInit(tmplID, talisman, originFromServer)
	if ZLuaTalExistInTmplIDs(tmplID) == 0 then return end
	if talisman == nil then return end
	
	---È¡µÃÒ»Ð©³£Á¿
	local dataIndex,resisIndex,data2Index = ZLuaTalDataIndex()
	local staminaGrowth = ZLuaTalStaminaGrowth()
	local serverOrigins = ZLuaTalServerOrigins()
	
	---È¡µÃ·É½£Ä£Ê½
	---0 Óù½£;3 ÓùÊÞ;6 ³á°ò,Ã¿´ÎÌí¼ÓÐÂµÄ·É½£ÐèÒªÐÞ¸Ä´Ë´¦.
	local flymod
	
	if ZLuaTalGetTalTypeFromID(tmplID) == "feijian" then
		flymod=0
	elseif ZLuaTalGetTalTypeFromID(tmplID) == "yushou" then
		flymod=3
	elseif ZLuaTalGetTalTypeFromID(tmplID) == "chibang" then
		flymod=6
	else
		flymod=3
	end

	---È¡µÃ·¨±¦À´Ô´£¨·þÎñÆ÷Ô¼¶¨£©
	local serverOrigin = originFromServer or serverOrigins["null"]
	

	---È¡µÃ·¨±¦Ëæ»ú½á¹û
	local randomResult, quality, speciality = ZLuaTalRandomResult(tmplID, serverOrigin)
	
	---Éè¶¨·¨±¦ÊÇ·ñ·ÉÐÐ·¨±¦
	talisman:UpdateData(0,dataIndex["is_fly"], randomResult[dataIndex["is_fly"]]);
	

	---Éú³É·¨±¦Ä£°åID
	talisman:UpdateData(0,dataIndex["tmpl_id"], tmplID)
	
	---Éú³É±£ÁôÊý¾Ý
	talisman:UpdateData(0,dataIndex["reserve1"], 0)
	talisman:UpdateData(0,dataIndex["reserve2"], 0)

	---Éú³É·¨±¦¼¼ÄÜ
	talisman:UpdateData(0,dataIndex["skill_addon_id"], randomResult[dataIndex["skill_addon_id"]]);

	---Éú³É¾«Á¦Öµ³õÊ¼Öµ¡¢³É³¤ÏµÊý
	talisman:UpdateData(0,dataIndex["default_stamina"], math.floor(randomResult[dataIndex["default_stamina"]]));
	talisman:UpdateData(0,dataIndex["quo_stamina"], math.floor(randomResult[dataIndex["quo_stamina"]]*100)/100);
	
	---Éú³É·¨±¦±¾ÌåÊôÐÔ³õÖµ¡¢Ôö³¤Á¿
	talisman:UpdateData(0,dataIndex["default_hp"], math.floor(randomResult[dataIndex["default_hp"]]));
	talisman:UpdateData(0,dataIndex["default_mp"], math.floor(randomResult[dataIndex["default_mp"]]));
	talisman:UpdateData(0,dataIndex["default_attack_rate"], 0);
	talisman:UpdateData(0,dataIndex["default_damage_low"], math.floor(math.min(randomResult[dataIndex["default_damage_low"]],randomResult[dataIndex["default_damage_high"]])));
	talisman:UpdateData(0,dataIndex["default_damage_high"], math.floor(randomResult[dataIndex["default_damage_high"]]));
	talisman:UpdateData(0,dataIndex["default_dodge"], 0);
	talisman:UpdateData(0,dataIndex["default_defense"], 0);
	talisman:UpdateData(0,dataIndex["default_resis_stun"], math.floor(randomResult[dataIndex["default_resis_stun"]]));
	talisman:UpdateData(0,dataIndex["default_resis_weakness"], math.floor(randomResult[dataIndex["default_resis_weakness"]]));
	talisman:UpdateData(0,dataIndex["default_resis_silence"], math.floor(randomResult[dataIndex["default_resis_silence"]]));
	talisman:UpdateData(0,dataIndex["default_resis_sleep"], math.floor(randomResult[dataIndex["default_resis_sleep"]]));
	talisman:UpdateData(0,dataIndex["default_resis_enlace"], math.floor(randomResult[dataIndex["default_resis_enlace"]]));

	talisman:UpdateData(0,dataIndex["grow_hp"], math.floor(randomResult[dataIndex["grow_hp"]]*100)/100);
	talisman:UpdateData(0,dataIndex["grow_mp"], math.floor(randomResult[dataIndex["grow_mp"]]*100)/100);
	talisman:UpdateData(0,dataIndex["grow_attack_rate"], 0);
	talisman:UpdateData(0,dataIndex["grow_damage_low"], math.floor(math.min(randomResult[dataIndex["grow_damage_low"]],randomResult[dataIndex["grow_damage_high"]])*100)/100);
	talisman:UpdateData(0,dataIndex["grow_damage_high"], math.floor(randomResult[dataIndex["grow_damage_high"]]*100)/100);
	talisman:UpdateData(0,dataIndex["grow_dodge"], 0);
	talisman:UpdateData(0,dataIndex["grow_defense"], 0);
	talisman:UpdateData(0,dataIndex["grow_resis_stun"], math.floor(randomResult[dataIndex["grow_resis_stun"]]*100)/100);
	talisman:UpdateData(0,dataIndex["grow_resis_weakness"], math.floor(randomResult[dataIndex["grow_resis_weakness"]]*100)/100);
	talisman:UpdateData(0,dataIndex["grow_resis_silence"], math.floor(randomResult[dataIndex["grow_resis_silence"]]*100)/100);
	talisman:UpdateData(0,dataIndex["grow_resis_sleep"], math.floor(randomResult[dataIndex["grow_resis_sleep"]]*100)/100);
	talisman:UpdateData(0,dataIndex["grow_resis_enlace"], math.floor(randomResult[dataIndex["grow_resis_enlace"]]*100)/100);
	
	---Éú³É·¨±¦¸½¼ÓÊôÐÔÑ§Ï°µÈ¼¶¡¢¸½¼ÓÊôÐÔId
	talisman:UpdateData(0,dataIndex["addon_1_learn_level"], (randomResult[dataIndex["addon_1_learn_level"]] or 0));
	talisman:UpdateData(0,dataIndex["addon_2_learn_level"], (randomResult[dataIndex["addon_2_learn_level"]] or 0));
	talisman:UpdateData(0,dataIndex["addon_3_learn_level"], (randomResult[dataIndex["addon_3_learn_level"]] or 0));
	talisman:UpdateData(0,dataIndex["addon_4_learn_level"], (randomResult[dataIndex["addon_4_learn_level"]] or 0));
	talisman:UpdateData(0,dataIndex["addon_5_learn_level"], (randomResult[dataIndex["addon_5_learn_level"]] or 0));
	talisman:UpdateData(0,dataIndex["addon_6_learn_level"], (randomResult[dataIndex["addon_6_learn_level"]] or 0));
	talisman:UpdateData(0,dataIndex["addon_7_learn_level"], (randomResult[dataIndex["addon_7_learn_level"]] or 0));
	talisman:UpdateData(0,dataIndex["addon_8_learn_level"], (randomResult[dataIndex["addon_8_learn_level"]] or 0));
	talisman:UpdateData(0,dataIndex["addon_9_learn_level"], (randomResult[dataIndex["addon_9_learn_level"]] or 0));
	talisman:UpdateData(0,dataIndex["addon_10_learn_level"], (randomResult[dataIndex["addon_10_learn_level"]] or 0));
	talisman:UpdateData(0,dataIndex["addon_11_learn_level"], (randomResult[dataIndex["addon_11_learn_level"]] or 0));
	talisman:UpdateData(0,dataIndex["addon_1_id"], (randomResult[dataIndex["addon_1_id"]] or 0));
	talisman:UpdateData(0,dataIndex["addon_2_id"], (randomResult[dataIndex["addon_2_id"]] or 0));
	talisman:UpdateData(0,dataIndex["addon_3_id"], (randomResult[dataIndex["addon_3_id"]] or 0));
	talisman:UpdateData(0,dataIndex["addon_4_id"], (randomResult[dataIndex["addon_4_id"]] or 0));
	talisman:UpdateData(0,dataIndex["addon_5_id"], (randomResult[dataIndex["addon_5_id"]] or 0));
	talisman:UpdateData(0,dataIndex["addon_6_id"], (randomResult[dataIndex["addon_6_id"]] or 0));
	talisman:UpdateData(0,dataIndex["addon_7_id"], (randomResult[dataIndex["addon_7_id"]] or 0));
	talisman:UpdateData(0,dataIndex["addon_8_id"], (randomResult[dataIndex["addon_8_id"]] or 0));
	talisman:UpdateData(0,dataIndex["addon_9_id"], (randomResult[dataIndex["addon_9_id"]] or 0));
	talisman:UpdateData(0,dataIndex["addon_10_id"],(randomResult[dataIndex["addon_10_id"]] or 0));
	talisman:UpdateData(0,dataIndex["addon_11_id"],(randomResult[dataIndex["addon_11_id"]] or 0));
  
  
  
	---debug£¬²âÊÔ×î´ó³¤¶È·¨±¦£¬ÊÉ»ê
	if tmplID == 999999 then
		talisman:UpdateData(0,dataIndex["addon_1_learn_level"], 1);
		talisman:UpdateData(0,dataIndex["addon_2_learn_level"], 1);
		talisman:UpdateData(0,dataIndex["addon_3_learn_level"], 1);
		talisman:UpdateData(0,dataIndex["addon_4_learn_level"], 1);
		talisman:UpdateData(0,dataIndex["addon_5_learn_level"], 1);
		talisman:UpdateData(0,dataIndex["addon_6_learn_level"], 1);
		talisman:UpdateData(0,dataIndex["addon_7_learn_level"], 1);
		talisman:UpdateData(0,dataIndex["addon_8_learn_level"], 1);
		talisman:UpdateData(0,dataIndex["addon_9_learn_level"], 1);
		talisman:UpdateData(0,dataIndex["addon_10_learn_level"], 1);
		talisman:UpdateData(0,dataIndex["addon_11_learn_level"], 1);
		talisman:UpdateData(0,dataIndex["addon_1_id"], 370);
		talisman:UpdateData(0,dataIndex["addon_2_id"], 370);
		talisman:UpdateData(0,dataIndex["addon_3_id"], 370);
		talisman:UpdateData(0,dataIndex["addon_4_id"], 370);
		talisman:UpdateData(0,dataIndex["addon_5_id"], 370);
		talisman:UpdateData(0,dataIndex["addon_6_id"], 370);
		talisman:UpdateData(0,dataIndex["addon_7_id"], 370);
		talisman:UpdateData(0,dataIndex["addon_8_id"], 370);
		talisman:UpdateData(0,dataIndex["addon_9_id"], 370);
		talisman:UpdateData(0,dataIndex["addon_10_id"], 370);
		talisman:UpdateData(0,dataIndex["addon_11_id"], 370);
	end	
	---debug

	---Éú³É·¨±¦ÃÀ¹¤Â·¾¶

	talisman:UpdateData(0,dataIndex["artpath_1_level"], 1);
	talisman:UpdateData(0,dataIndex["artpath_2_level"], 10);
	talisman:UpdateData(0,dataIndex["artpath_3_level"], 20);
	talisman:UpdateData(0,dataIndex["artpath_4_level"], 30);

	
	---Éú³É·¨±¦¸ÉÈÅÏµÊý
	local confusions = ZLuaTalGenerateConfusion( tmplID, origin );
	talisman:UpdateData(0,dataIndex["confusion_1_level"], confusions[1].level)
	talisman:UpdateData(0,dataIndex["confusion_1_quo"], confusions[1].quo)
	talisman:UpdateData(0,dataIndex["confusion_2_level"], confusions[2].level)
	talisman:UpdateData(0,dataIndex["confusion_2_quo"], confusions[2].quo)
	talisman:UpdateData(0,dataIndex["confusion_3_level"], confusions[3].level)
	talisman:UpdateData(0,dataIndex["confusion_3_quo"], confusions[3].quo)
	talisman:UpdateData(0,dataIndex["confusion_4_level"], confusions[4].level)
	talisman:UpdateData(0,dataIndex["confusion_4_quo"], confusions[4].quo)
	talisman:UpdateData(0,dataIndex["confusion_5_level"], confusions[5].level)
	talisman:UpdateData(0,dataIndex["confusion_5_quo"], confusions[5].quo)
	
	---¼ÆËã³öÉú³ÉÊ±¿Ì
	local gameHour12, gameMin3 = ZLuaTalGenerteBirthTime()
	talisman:UpdateData(0,dataIndex["birth_hour12"], gameHour12)
	talisman:UpdateData(0,dataIndex["birth_min3"], gameMin3)
	
	---¼ÆËã³ö·¨±¦À´Ô´
	local clientOrigin = ZLuaTalGenerateClientOrigin(serverOrigin)
	talisman:UpdateData(0,dataIndex["origin"], clientOrigin)

	---Ð´ÈëÆ·ÖÊ
	local realquality
		realquality=quality+flymod
	if talisman:QueryData(0,dataIndex["is_fly"]) == 1 then
		---
		talisman:UpdateData(0,dataIndex["quality"], realquality)
	else
		talisman:UpdateData(0,dataIndex["quality"], quality)
	end
	
	---Ð´ÈëÌØÐÔ
	talisman:UpdateData(0,dataIndex["speciality"], speciality)
	
	---Ð´Èë·¨±¦¹àÄ§Ïà¹ØÖµ
	talisman:UpdateData(0,dataIndex["buff_type"], 0)
	talisman:UpdateData(0,dataIndex["ext_id"], 0)
	talisman:UpdateData(0,dataIndex["skill_visible"], 0)
	
	---Ð´Èë¹Û²ìÖµ
	talisman:UpdateData(0,dataIndex["combine_times"], 0)
	talisman:UpdateData(0,dataIndex["enchant_times"], 0)
	talisman:UpdateData(0,dataIndex["feed_baoqi"], 0)
	talisman:UpdateData(0,dataIndex["feed_xianpin"], 0)
	talisman:UpdateData(0,dataIndex["feed_shenpin"], 0)
	talisman:UpdateData(0,dataIndex["feed_cost"], 0)
	talisman:UpdateData(0,dataIndex["change_skillnum"], 0)
	talisman:UpdateData(0,dataIndex["feed_level_cost"], 0)
	
	---Ð´ÈëµµÎ»ÌáÉýÐÅÏ¢
	talisman:UpdateData(0,dataIndex["hp_rise"], 0)
	talisman:UpdateData(0,dataIndex["mp_rise"], 0)
	talisman:UpdateData(0,dataIndex["minattack_rise"], 0)
	talisman:UpdateData(0,dataIndex["maxattack_rise"], 0)
	talisman:UpdateData(0,dataIndex["stun_rise"], 0)
	talisman:UpdateData(0,dataIndex["weakness_rise"], 0)
	talisman:UpdateData(0,dataIndex["enlace_rise"], 0)
	talisman:UpdateData(0,dataIndex["silence_rise"], 0)
	talisman:UpdateData(0,dataIndex["sleep_rise"], 0)
	talisman:UpdateData(0,dataIndex["all_rise"], 0)

	-- Ð´ÈëÖ÷ÊôÐÔÇ¿»¯ÐÅÏ¢
	talisman:UpdateData(0,dataIndex["hp_enhance"], 0)
	talisman:UpdateData(0,dataIndex["mp_enhance"], 0)
	talisman:UpdateData(0,dataIndex["atk_enhance"], 0)
	talisman:UpdateData(1,data2Index["hp_enhance"], 0)
	talisman:UpdateData(1,data2Index["mp_enhance"], 0)
	talisman:UpdateData(1,data2Index["atk_enhance"], 0)

	-- Ð´ÈëÏ´Á¶¼¼ÄÜÐÅÏ¢	
	talisman:UpdateData(0,dataIndex["skillrefine_level"], 0)
	talisman:UpdateData(0,dataIndex["skillrefine_0"], 0)
	talisman:UpdateData(0,dataIndex["skillrefine_1"], 0)
	talisman:UpdateData(0,dataIndex["skillrefine_2"], 0)
	talisman:UpdateData(0,dataIndex["skillrefine_3"], 0)
	talisman:UpdateData(0,dataIndex["skillrefine_4"], 0)
	talisman:UpdateData(0,dataIndex["skillrefine_5"], 0)
	talisman:UpdateData(0,dataIndex["skillrefine_6"], 0)
	talisman:UpdateData(0,dataIndex["skillrefine_7"], 0)
	talisman:UpdateData(0,dataIndex["skillrefine_8"], 0)
	talisman:UpdateData(0,dataIndex["skillrefinetmp_0"], 0)
	talisman:UpdateData(0,dataIndex["skillrefinetmp_1"], 0)
	talisman:UpdateData(0,dataIndex["skillrefinetmp_2"], 0)
	talisman:UpdateData(0,dataIndex["skillrefinetmp_3"], 0)
	talisman:UpdateData(0,dataIndex["skillrefinetmp_4"], 0)
	talisman:UpdateData(0,dataIndex["skillrefinetmp_5"], 0)
	talisman:UpdateData(0,dataIndex["skillrefinetmp_6"], 0)
	talisman:UpdateData(0,dataIndex["skillrefinetmp_7"], 0)
	talisman:UpdateData(0,dataIndex["skillrefinetmp_8"], 0)

	-- Ð´ÈëÏâÇ¶¼¼ÄÜÐÅÏ¢	
	talisman:UpdateData(0,dataIndex["newaddon_id_1"], 0)
	talisman:UpdateData(0,dataIndex["newaddon_id_2"], 0)
	talisman:UpdateData(0,dataIndex["newaddon_id_3"], 0)
	talisman:UpdateData(0,dataIndex["newaddon_id_4"], 0)
	talisman:UpdateData(0,dataIndex["newaddon_id_5"], 0)

	-- Ð´Èë·ÉÉý´ÎÊýÐÅÏ¢
	talisman:UpdateData(0,dataIndex["reborncont"], 0)

	-- Ð´Èë¼¼ÄÜÏ´Á¶´ÎÊýÐÅÏ¢
	talisman:UpdateData(0,dataIndex["skillrefinecont"], 0)

	-- Ð´ÈëÐÂµÄ´æÏ´Á¶·¨±¦¸ñÊýÊý¾Ý
	talisman:UpdateData(0,dataIndex["skill_addon_id_temp"], 0)
	talisman:UpdateData(0,dataIndex["addon_1_id_temp"], 0)
	talisman:UpdateData(0,dataIndex["addon_2_id_temp"], 0)
	talisman:UpdateData(0,dataIndex["addon_3_id_temp"], 0)
	talisman:UpdateData(0,dataIndex["addon_4_id_temp"], 0)
	talisman:UpdateData(0,dataIndex["addon_5_id_temp"], 0)
	talisman:UpdateData(0,dataIndex["addon_6_id_temp"], 0)
	talisman:UpdateData(0,dataIndex["addon_1_id_learn_level_temp"], 0)
	talisman:UpdateData(0,dataIndex["addon_2_id_learn_level_temp"], 0)
	talisman:UpdateData(0,dataIndex["addon_3_id_learn_level_temp"], 0)
	talisman:UpdateData(0,dataIndex["addon_4_id_learn_level_temp"], 0)
	talisman:UpdateData(0,dataIndex["addon_5_id_learn_level_temp"], 0)
	talisman:UpdateData(0,dataIndex["addon_6_id_learn_level_temp"], 0)
	

	ZLuaTalRebuild(talisman,tmplID);
end

---·¨±¦Éý¼¶
function ZLuaTalLevelup(tmplID, talisman)
	---¸ü»»·¨ÉíÐ§¹û
	ZLuaTalChangeExt(talisman);

	ZLuaTalRebuild(talisman,tmplID);
end

---·¨±¦Ï´Á¶
function ZLuaTalReset(tmplID, talisman)
	local dataIndex = ZLuaTalDataIndex()
	talisman:UpdateData(0,dataIndex["buff_type"],0)
	talisman:UpdateData(0,dataIndex["ext_id"],0)
	ZLuaTalRebuild(talisman,tmplID);
end

---·¨±¦±¾ÌåÊôÐÔÓë¸½¼ÓÊôÐÔÖØÐÂ¼ÆËã
function ZLuaTalRebuild(talisman,tmplID)
	---È¡µÃÒ»Ð©³£Á¿
	local dataIndex,resisIndex,data2Index = ZLuaTalDataIndex()
	local staminaGrowth = ZLuaTalStaminaGrowth()
	---ÔÙ´ÎÖØÐÂ¼ÆËã·¨±¦Æ·½×£¬ÓÃÓÚ¾ÀÕý20070918ÕýÊ½°æµÄ½Å±¾ÖÐÉ½ºÓÉÈ¡¢Óñ¾»Æ¿µÄÆ·½×´íÎó
	---ÔÚdebugÊ±×¢ÊÍµô uncomplete
	local correct_quality = ZLuaTalGenerateQualityFromTalisman(tmplID, talisman)
	if talisman:QueryData(0,dataIndex["is_fly"])==0 then
	talisman:UpdateData(0,dataIndex["quality"], correct_quality)
	end

	-------------Èç¹ûÃ»ÓÐÐÂµÄÊý¾ÝÏî£¬ÔòÉú³É----------------------------------
	if talisman:QueryData(0,dataIndex["hp_rise"])==nil then
		talisman:UpdateData(0,dataIndex["hp_rise"],0)
	end
	if talisman:QueryData(0,dataIndex["mp_rise"])==nil then
		talisman:UpdateData(0,dataIndex["mp_rise"],0)
	end
	if talisman:QueryData(0,dataIndex["minattack_rise"])==nil then
		talisman:UpdateData(0,dataIndex["minattack_rise"],0)
	end
	if talisman:QueryData(0,dataIndex["maxattack_rise"])==nil then
		talisman:UpdateData(0,dataIndex["maxattack_rise"],0)
	end
	if talisman:QueryData(0,dataIndex["stun_rise"])==nil then
		talisman:UpdateData(0,dataIndex["stun_rise"],0)
	end
	if talisman:QueryData(0,dataIndex["weakness_rise"])==nil then
		talisman:UpdateData(0,dataIndex["weakness_rise"],0)
	end
	if talisman:QueryData(0,dataIndex["enlace_rise"])==nil then
		talisman:UpdateData(0,dataIndex["enlace_rise"],0)
	end
	if talisman:QueryData(0,dataIndex["silence_rise"])==nil then
		talisman:UpdateData(0,dataIndex["silence_rise"],0)
	end
	if talisman:QueryData(0,dataIndex["sleep_rise"])==nil then
		talisman:UpdateData(0,dataIndex["sleep_rise"],0)
	end
	if talisman:QueryData(0,dataIndex["all_rise"])==nil then
		talisman:UpdateData(0,dataIndex["all_rise"],0)
	end	

	if talisman:QueryData(0,dataIndex["hp_enhance"])==nil then
		talisman:UpdateData(0,dataIndex["hp_enhance"],0)
	end	
	if talisman:QueryData(0,dataIndex["mp_enhance"])==nil then
		talisman:UpdateData(0,dataIndex["mp_enhance"],0)
	end	
	if talisman:QueryData(0,dataIndex["atk_enhance"])==nil then
		talisman:UpdateData(0,dataIndex["atk_enhance"],0)
	end	
	if talisman:QueryData(1,data2Index["hp_enhance"])==nil then
		talisman:UpdateData(1,data2Index["hp_enhance"],0)
	end	
	if talisman:QueryData(1,data2Index["mp_enhance"])==nil then
		talisman:UpdateData(1,data2Index["mp_enhance"],0)
	end	
	if talisman:QueryData(1,data2Index["atk_enhance"])==nil then
		talisman:UpdateData(1,data2Index["atk_enhance"],0)
	end	
	if talisman:QueryData(0,dataIndex["skillrefine_level"])==nil then
		talisman:UpdateData(0,dataIndex["skillrefine_level"],0)
	end	
	if talisman:QueryData(0,dataIndex["skillrefine_0"])==nil then
		talisman:UpdateData(0,dataIndex["skillrefine_0"],0)
	end	
	if talisman:QueryData(0,dataIndex["skillrefine_1"])==nil then
		talisman:UpdateData(0,dataIndex["skillrefine_1"],0)
	end	
	if talisman:QueryData(0,dataIndex["skillrefine_2"])==nil then
		talisman:UpdateData(0,dataIndex["skillrefine_2"],0)
	end	
	if talisman:QueryData(0,dataIndex["skillrefine_3"])==nil then
		talisman:UpdateData(0,dataIndex["skillrefine_3"],0)
	end	
	if talisman:QueryData(0,dataIndex["skillrefine_4"])==nil then
		talisman:UpdateData(0,dataIndex["skillrefine_4"],0)
	end	
	if talisman:QueryData(0,dataIndex["skillrefine_5"])==nil then
		talisman:UpdateData(0,dataIndex["skillrefine_5"],0)
	end	
	if talisman:QueryData(0,dataIndex["skillrefine_6"])==nil then
		talisman:UpdateData(0,dataIndex["skillrefine_6"],0)
	end	
	if talisman:QueryData(0,dataIndex["skillrefine_7"])==nil then
		talisman:UpdateData(0,dataIndex["skillrefine_7"],0)
	end	
	if talisman:QueryData(0,dataIndex["skillrefine_8"])==nil then
		talisman:UpdateData(0,dataIndex["skillrefine_8"],0)
	end	
	if talisman:QueryData(0,dataIndex["skillrefinetmp_1"])==nil then
		talisman:UpdateData(0,dataIndex["skillrefinetmp_1"],0)
	end	
	if talisman:QueryData(0,dataIndex["skillrefinetmp_2"])==nil then
		talisman:UpdateData(0,dataIndex["skillrefinetmp_2"],0)
	end	
	if talisman:QueryData(0,dataIndex["skillrefinetmp_3"])==nil then
		talisman:UpdateData(0,dataIndex["skillrefinetmp_3"],0)
	end	
	if talisman:QueryData(0,dataIndex["skillrefinetmp_4"])==nil then
		talisman:UpdateData(0,dataIndex["skillrefinetmp_4"],0)
	end	
	if talisman:QueryData(0,dataIndex["skillrefinetmp_5"])==nil then
		talisman:UpdateData(0,dataIndex["skillrefinetmp_5"],0)
	end	
	if talisman:QueryData(0,dataIndex["skillrefinetmp_6"])==nil then
		talisman:UpdateData(0,dataIndex["skillrefinetmp_6"],0)
	end	
	if talisman:QueryData(0,dataIndex["skillrefinetmp_7"])==nil then
		talisman:UpdateData(0,dataIndex["skillrefinetmp_7"],0)
	end	
	if talisman:QueryData(0,dataIndex["skillrefinetmp_8"])==nil then
		talisman:UpdateData(0,dataIndex["skillrefinetmp_8"],0)
	end	
	if talisman:QueryData(0,dataIndex["newaddon_id_1"])==nil then
		talisman:UpdateData(0,dataIndex["newaddon_id_1"],0)
	end	
	if talisman:QueryData(0,dataIndex["newaddon_id_2"])==nil then
		talisman:UpdateData(0,dataIndex["newaddon_id_2"],0)
	end	
	if talisman:QueryData(0,dataIndex["newaddon_id_3"])==nil then
		talisman:UpdateData(0,dataIndex["newaddon_id_3"],0)
	end	
	if talisman:QueryData(0,dataIndex["newaddon_id_4"])==nil then
		talisman:UpdateData(0,dataIndex["newaddon_id_4"],0)
	end	
	if talisman:QueryData(0,dataIndex["newaddon_id_5"])==nil then
		talisman:UpdateData(0,dataIndex["newaddon_id_5"],0)
	end	
	if talisman:QueryData(0,dataIndex["reborncont"])==nil then
		talisman:UpdateData(0,dataIndex["reborncont"],0)
	end	
	if talisman:QueryData(0,dataIndex["skillrefinecont"])==nil then
		talisman:UpdateData(0,dataIndex["skillrefinecont"],0)
	end	

	if talisman:QueryData(0,dataIndex["skill_addon_id_temp"])==nil then
		talisman:UpdateData(0,dataIndex["skill_addon_id_temp"],0)
	end	
	if talisman:QueryData(0,dataIndex["addon_1_id_temp"])==nil then
		talisman:UpdateData(0,dataIndex["addon_1_id_temp"],0)
	end	
	if talisman:QueryData(0,dataIndex["addon_2_id_temp"])==nil then
		talisman:UpdateData(0,dataIndex["addon_2_id_temp"],0)
	end	
	if talisman:QueryData(0,dataIndex["addon_3_id_temp"])==nil then
		talisman:UpdateData(0,dataIndex["addon_3_id_temp"],0)
	end	
	if talisman:QueryData(0,dataIndex["addon_4_id_temp"])==nil then
		talisman:UpdateData(0,dataIndex["addon_4_id_temp"],0)
	end	
	if talisman:QueryData(0,dataIndex["addon_5_id_temp"])==nil then
		talisman:UpdateData(0,dataIndex["addon_5_id_temp"],0)
	end	
	if talisman:QueryData(0,dataIndex["addon_6_id_temp"])==nil then
		talisman:UpdateData(0,dataIndex["addon_6_id_temp"],0)
	end	
	if talisman:QueryData(0,dataIndex["addon_1_id_learn_level_temp"])==nil then
		talisman:UpdateData(0,dataIndex["addon_1_id_learn_level_temp"],0)
	end	
	if talisman:QueryData(0,dataIndex["addon_2_id_learn_level_temp"])==nil then
		talisman:UpdateData(0,dataIndex["addon_2_id_learn_level_temp"],0)
	end	
	if talisman:QueryData(0,dataIndex["addon_3_id_learn_level_temp"])==nil then
		talisman:UpdateData(0,dataIndex["addon_3_id_learn_level_temp"],0)
	end	
	if talisman:QueryData(0,dataIndex["addon_4_id_learn_level_temp"])==nil then
		talisman:UpdateData(0,dataIndex["addon_4_id_learn_level_temp"],0)
	end	
	if talisman:QueryData(0,dataIndex["addon_5_id_learn_level_temp"])==nil then
		talisman:UpdateData(0,dataIndex["addon_5_id_learn_level_temp"],0)
	end	
	if talisman:QueryData(0,dataIndex["addon_6_id_learn_level_temp"])==nil then
		talisman:UpdateData(0,dataIndex["addon_6_id_learn_level_temp"],0)
	end	
	
--	

	---È¡µÃ·¨±¦µÈ¼¶
	local level = talisman:QueryLevel();
	---È¡µÃ¸ÉÈÅ½á¹û
	local confusionQuo = 0
	local confusions = {}
	local i
	for i=1,5 do
		confusions[i] = { quo = 1, level = 1 }
	end
	confusions[1].quo = talisman:QueryData(0,dataIndex["confusion_1_quo"])
	confusions[2].quo = talisman:QueryData(0,dataIndex["confusion_2_quo"])
	confusions[3].quo = talisman:QueryData(0,dataIndex["confusion_3_quo"])
	confusions[4].quo = talisman:QueryData(0,dataIndex["confusion_4_quo"])
	confusions[5].quo = talisman:QueryData(0,dataIndex["confusion_5_quo"])
	confusions[1].level = talisman:QueryData(0,dataIndex["confusion_1_level"])
	confusions[2].level = talisman:QueryData(0,dataIndex["confusion_2_level"])
	confusions[3].level = talisman:QueryData(0,dataIndex["confusion_3_level"])
	confusions[4].level = talisman:QueryData(0,dataIndex["confusion_4_level"])
	confusions[5].level = talisman:QueryData(0,dataIndex["confusion_5_level"])
	
	for i=1,5 do
		if level >= confusions[i].level then confusionQuo = confusionQuo + ( confusions[i].quo - 1 ) end
	end
	
	---ÉèÖÃ·¨±¦¾«Á·ÖµÉÏÏÞ
	local defaultStamina = talisman:QueryData(0,dataIndex["default_stamina"])
	local quoStamina = talisman:QueryData(0,dataIndex["quo_stamina"])
	talisman:SetStamina( math.floor( defaultStamina + quoStamina * staminaGrowth[level] ) );

	---ÉèÖÃ·ÉÉýÓ°ÏìÏµÊý
	local holyLevelUpConf1 = 1
	local holyLevelUpConf2 = 1
	if talisman:QueryData(0,dataIndex["reborncont"]) ~= 0 then 
		holyLevelUpConf1 = 1.1
		holyLevelUpConf2 = 1.2
	end
	
	---ÉèÖÃ·¨±¦±¾ÌåÊôÐÔ
	local defaultHP = math.max(0, talisman:QueryData(0,dataIndex["default_hp"]))
	local defaultMP = math.max(0, talisman:QueryData(0,dataIndex["default_mp"]))
	local defaultDamageLow = math.max(0, talisman:QueryData(0,dataIndex["default_damage_low"]))
	local defaultDamageHigh = math.max(0, talisman:QueryData(0,dataIndex["default_damage_high"]))
	local defaultResisStun = math.max(0, talisman:QueryData(0,dataIndex["default_resis_stun"]))
	local defaultResisSleep = math.max(0, talisman:QueryData(0,dataIndex["default_resis_sleep"]))
	local defaultResisSilence = math.max(0, talisman:QueryData(0,dataIndex["default_resis_silence"]))
	local defaultResisWeakness = math.max(0, talisman:QueryData(0,dataIndex["default_resis_weakness"]))
	local defaultResisEnlace = math.max(0, talisman:QueryData(0,dataIndex["default_resis_enlace"]))
	
	local growHP = math.max(0, talisman:QueryData(0,dataIndex["grow_hp"])*holyLevelUpConf1)
	local growMP = math.max(0, talisman:QueryData(0,dataIndex["grow_mp"])*holyLevelUpConf1)
	local growDamageLow = math.max(0, talisman:QueryData(0,dataIndex["grow_damage_low"])*holyLevelUpConf1)
	local growDamageHigh = math.max(0, talisman:QueryData(0,dataIndex["grow_damage_high"])*holyLevelUpConf1)
	local growResisStun = math.max(0, talisman:QueryData(0,dataIndex["grow_resis_stun"])*holyLevelUpConf1)
	local growResisSleep = math.max(0, talisman:QueryData(0,dataIndex["grow_resis_sleep"])*holyLevelUpConf1)
	local growResisSilence = math.max(0, talisman:QueryData(0,dataIndex["grow_resis_silence"])*holyLevelUpConf1)
	local growResisWeakness = math.max(0, talisman:QueryData(0,dataIndex["grow_resis_weakness"])*holyLevelUpConf1)
	local growResisEnlace = math.max(0, talisman:QueryData(0,dataIndex["grow_resis_enlace"])*holyLevelUpConf1)

	local hp_enhance = math.max(0, math.min(200 * level*holyLevelUpConf2, talisman:QueryData(0,dataIndex["hp_enhance"]))*holyLevelUpConf2)
	local mp_enhance = math.max(0, math.min(200 * level*holyLevelUpConf2, talisman:QueryData(0,dataIndex["mp_enhance"]))*holyLevelUpConf2)
	local atk_enhance= math.max(0, math.min( 30 * level*holyLevelUpConf2, talisman:QueryData(0,dataIndex["atk_enhance"]))*holyLevelUpConf2)

	talisman:SetHP( math.floor( defaultHP + growHP * (level-1+confusionQuo) + hp_enhance) )
	talisman:SetMP( math.floor( defaultMP + growMP * (level-1) + mp_enhance ) )
	talisman:SetDamage( math.floor( defaultDamageLow + growDamageLow * (level-1+confusionQuo) + atk_enhance ), 
											math.floor( defaultDamageHigh + growDamageHigh * (level-1+confusionQuo) + atk_enhance ) )

	talisman:SetHPEnhance( hp_enhance )
	talisman:SetMPEnhance( mp_enhance )
	talisman:SetAttackEnhance( atk_enhance )

	talisman:SetResistance( resisIndex["resis_stun"], math.floor( defaultResisStun + growResisStun * (level-1+confusionQuo) ) )
	talisman:SetResistance( resisIndex["resis_sleep"], math.floor( defaultResisSleep + growResisSleep * (level-1+confusionQuo) ) )
	talisman:SetResistance( resisIndex["resis_silence"], math.floor( defaultResisSilence + growResisSilence * (level-1+confusionQuo) ) )
	talisman:SetResistance( resisIndex["resis_weakness"], math.floor( defaultResisWeakness + growResisWeakness * (level-1+confusionQuo) ) )
	talisman:SetResistance( resisIndex["resis_enlace"], math.floor( defaultResisEnlace + growResisEnlace * (level-1+confusionQuo) ) )
	
	---ÉèÖÃ¸½¼ÓÊôÐÔ	
	local addonLearnLevel = {}
	local addonID = {}

	addonLearnLevel[1] = 1
	addonID[1]         = talisman:QueryData(0,dataIndex["skill_addon_id"])
	addonLearnLevel[2] = talisman:QueryData(0,dataIndex["addon_1_learn_level"])
	addonID[2]         = talisman:QueryData(0,dataIndex["addon_1_id"])
	addonLearnLevel[3] = talisman:QueryData(0,dataIndex["addon_2_learn_level"])
	addonID[3]         = talisman:QueryData(0,dataIndex["addon_2_id"])
	addonLearnLevel[4] = talisman:QueryData(0,dataIndex["addon_3_learn_level"])
	addonID[4]         = talisman:QueryData(0,dataIndex["addon_3_id"])
	addonLearnLevel[5] = talisman:QueryData(0,dataIndex["addon_4_learn_level"])
	addonID[5]         = talisman:QueryData(0,dataIndex["addon_4_id"])
	addonLearnLevel[6] = talisman:QueryData(0,dataIndex["addon_5_learn_level"])
	addonID[6]         = talisman:QueryData(0,dataIndex["addon_5_id"])
	addonLearnLevel[7] = talisman:QueryData(0,dataIndex["addon_6_learn_level"])
	addonID[7]         = talisman:QueryData(0,dataIndex["addon_6_id"])
	addonLearnLevel[8] = talisman:QueryData(0,dataIndex["addon_7_learn_level"])
	addonID[8]         = talisman:QueryData(0,dataIndex["addon_7_id"])
	addonLearnLevel[9] = talisman:QueryData(0,dataIndex["addon_8_learn_level"])
	addonID[9]         = talisman:QueryData(0,dataIndex["addon_8_id"])
	addonLearnLevel[10] = talisman:QueryData(0,dataIndex["addon_9_learn_level"])
	addonID[10]         = talisman:QueryData(0,dataIndex["addon_9_id"])
	addonLearnLevel[11] = talisman:QueryData(0,dataIndex["addon_10_learn_level"])
	addonID[11]         = talisman:QueryData(0,dataIndex["addon_10_id"])
	addonLearnLevel[12] = talisman:QueryData(0,dataIndex["addon_11_learn_level"])
	addonID[12]         = talisman:QueryData(0,dataIndex["addon_11_id"])
	
	for i=1,7 do
		if addonLearnLevel[i] ~= nil and addonID[i] ~= nil and addonLearnLevel[i] > 0 then
			local unknown = true
			for j=1,level do
				if addonLearnLevel[i] == j then
					talisman:SetAddon( addonID[i] )
					unknown = false
				end
			end
			if unknown and talisman:QueryData(0,dataIndex["skill_visible"]) == 1 then
				talisman:SetAddon( ZLuaTalGetUnLearnID(addonID[i]) )
			end
		end
	end

	---Èç¹ûÊÇ·É½££¬ÔÚÕâÀïÖØÐÂ¼ÆËã·¨±¦Æ·ÖÊ
	local flymod

	if ZLuaTalGetTalTypeFromID(tmplID) == "feijian" then flymod=0
	elseif ZLuaTalGetTalTypeFromID(tmplID) == "yushou" then flymod=3
	elseif ZLuaTalGetTalTypeFromID(tmplID) == "chibang" then flymod=6
	else flymod=3
	end

	if talisman:QueryData(0,dataIndex["is_fly"])==1 then
		local addonID_number=0
		local realquality
		for q=2,8 do
			if addonID[q] > 0 then
				addonID_number=addonID_number+1
			end
		end
		if addonID_number > 4 then realquality=flymod+2
		elseif addonID_number == 4 then realquality=flymod+1
		elseif addonID_number < 4 then realquality=flymod
		end
		talisman:UpdateData(0,dataIndex["quality"], realquality)
	end

	local extID
-- ´¦Àí30+¼¶ÌìÈËºÏÒ»¹àÄ§ÊôÐÔ²»Ôö³¤µÄbug
	local buffType = talisman:QueryData(0,dataIndex["buff_type"])
	local TalismanEnchantEx=ZLuaTalismanEnchantEx()
	for i,v in pairs(TalismanEnchantEx) do
		if v.bufftype==buffType and v.exlevel==level then 
			extID = i
		end 
	end
-- ´¦Àí30+¼¶ÌìÈËºÏÒ»¹àÄ§ÊôÐÔ²»Ôö³¤µÄbug
	if extID ~= 0 then
		talisman:UpdateData(0,dataIndex["ext_id"],extID)
		talisman:SetAddon( extID ) 
	end

	---Éú³ÉQualityÊôÐÔ
	---¶Á³ö·¨±¦ÌØÐÔ¡¢Æ·½×¡¢Ê±³½¡¢Ê±¿Ì¡¢À´Ô´
	local speciality = ZLuaTalTransIndex(talisman:QueryData(0,dataIndex["speciality"]))
	local quality = talisman:QueryData(0,dataIndex["quality"])
	local hour12 = talisman:QueryData(0,dataIndex["birth_hour12"])
	local min3 = talisman:QueryData(0,dataIndex["birth_min3"])
	local origin = talisman:QueryData(0,dataIndex["origin"])

	---¼ÆËãÃÀ¹¤Â·¾¶
	local artpath1Level = talisman:QueryData(0,dataIndex["artpath_1_level"])
	local artpath2Level = talisman:QueryData(0,dataIndex["artpath_2_level"])
	local artpath3Level = talisman:QueryData(0,dataIndex["artpath_3_level"])
	local artpath4Level = talisman:QueryData(0,dataIndex["artpath_4_level"])
	local artpath
	if talisman:QueryData(0,dataIndex["reborncont"]) == 3 then
		artpath = 7
	elseif talisman:QueryData(0,dataIndex["reborncont"]) == 2 then
		artpath = 6
	elseif talisman:QueryData(0,dataIndex["reborncont"]) == 1 then
		artpath = 5
	elseif artpath4Level ~= nil and level >= artpath4Level then 
		artpath = 4
	elseif artpath3Level ~= nil and level >= artpath3Level then 
		artpath = 3
	elseif artpath2Level ~= nil and level >= artpath2Level then 
		artpath = 2
	else
		artpath = 1
	end

	---Éú³ÉQualityÊôÐÔµÄ¸ß16Î»
	local High16 = 2^(4+2+4+16)*quality + 2^(2+4+16)*hour12 + 2^(4+16)*min3 + 2^16*origin
	
	---QualityÊôÐÔµÄµÍ16Î»
	local Low16 = 2^12*speciality + artpath
	
	---Éú³ÉQualityÊôÐÔ
	talisman:SetQuality( High16 + Low16 )

end


---ÈÛÁ¶½á¹ûÎªA»òBÊ±µÄ³õÊ¼»¯º¯Êý
function ZLuaTalCombineInit(id, speciality, growDirInData, growGrades, talisman, output )		
	---´¦ÀíÒ»Ð©ÒâÍâÇé¿ö
	if type(id) ~= "number" or type(speciality) ~= "number" or type(growDirInData) ~= "number" then
		return
	end

	local dataIndex = ZLuaTalDataIndex()
	local gradeIndex = ZLuaTalGradeIndex()
	local nGrades, valueGrades = ZLuaTalGrades()
	local serverOrigins = ZLuaTalServerOrigins()

	---È¡µÃµ±Ç°·¨±¦µÄËùÓÐÊôÐÔµÄ³É³¤ÂÊÉÏÏÞ£¬Õâ¸ö±íÊÇÒÔgradeIndexÎªË÷ÒýµÄ
	local gradesUpperLimit = ZLuaTalGradesUpperLimit(id)
	---ÅÐ¶ÏÊÇ·ñ·ÉÉý·¨±¦£¬·ÉÉýºóµÄ·¨±¦³É³¤ÂÊÉÏÏÞÈ¡ÐÂÖµ
--[[
	local n = gradesUpperLimit
	if talisman:QueryData(0,dataIndex["reborncont"]) ~= 0 then
		gradesUpperLimit = {n[1]+90,n[2]+15,n[3]+15,n[4]+15,n[5]+15,n[6],n[7],n[8]+15,n[9]+15,n[10]+15,n[11]+15,n[12]+15,n[13]}
	end
--]]	
	---°ÑdataË÷Òý±äÎªgradesË÷Òý£¬ÒÔ±ãÓÚÊ¹ÓÃÉÏÃæµÄgradesUpperLimit±í
	local growDirInGrade = ZLuaTalTransIndex(growDirInData)
	
	---Ê×ÏÈ°ÑtalismanµÄËùÓÐdataÊý¾ÝcopyÖÁoutput
	for str,index in pairs(dataIndex) do
		output:UpdateData(0,index, talisman:QueryData(0,index) )
	end
	
	---´¦ÀíÀÏ·¨±¦ÎÞµµÎ»ÌáÉý¼ÇÂ¼µÄÇé¿ö
	output:UpdateData(0,dataIndex["hp_rise"], ( output:QueryData(0,dataIndex["hp_rise"]) or 0 ))
	output:UpdateData(0,dataIndex["mp_rise"], ( output:QueryData(0,dataIndex["mp_rise"]) or 0 ))
	output:UpdateData(0,dataIndex["minattack_rise"], ( output:QueryData(0,dataIndex["minattack_rise"]) or 0 ))
	output:UpdateData(0,dataIndex["maxattact_rise"], ( output:QueryData(0,dataIndex["maxattact_rise"]) or 0 ))
	output:UpdateData(0,dataIndex["stun_rise"], ( output:QueryData(0,dataIndex["stun_rise"]) or 0 ))
	output:UpdateData(0,dataIndex["weakness_rise"], ( output:QueryData(0,dataIndex["weakness_rise"]) or 0 ))
	output:UpdateData(0,dataIndex["enlace_rise"], ( output:QueryData(0,dataIndex["enlace_rise"]) or 0 ))
	output:UpdateData(0,dataIndex["silence_rise"], ( output:QueryData(0,dataIndex["silence_rise"]) or 0 ))
	output:UpdateData(0,dataIndex["sleep_rise"], ( output:QueryData(0,dataIndex["sleep_rise"]) or 0 ))
	output:UpdateData(0,dataIndex["all_rise"], ( output:QueryData(0,dataIndex["all_rise"]) or 0 ))
	
	---ÌØ±ð´¦ÀíÀÏ·¨±¦µÄ¼¸¸ö¹Û²ìÖµ
	output:UpdateData(0,dataIndex["combine_times"], ( output:QueryData(0,dataIndex["combine_times"]) or 0 ))
	output:UpdateData(0,dataIndex["enchant_times"], ( output:QueryData(0,dataIndex["enchant_times"]) or 0 ))
	output:UpdateData(0,dataIndex["feed_baoqi"], ( output:QueryData(0,dataIndex["feed_baoqi"]) or 0 ))
	output:UpdateData(0,dataIndex["feed_xianpin"], ( output:QueryData(0,dataIndex["feed_xianpin"]) or 0 ))
	output:UpdateData(0,dataIndex["feed_shenpin"], ( output:QueryData(0,dataIndex["feed_shenpin"]) or 0 ))
	output:UpdateData(0,dataIndex["feed_cost"], ( output:QueryData(0,dataIndex["feed_cost"]) or 0 ))
	output:UpdateData(0,dataIndex["change_skillnum"], ( output:QueryData(0,dataIndex["change_skillnum"]) or 0 ))
	output:UpdateData(0,dataIndex["feed_level_cost"], ( output:QueryData(0,dataIndex["feed_level_cost"]) or 0 ))
	output:UpdateData(0,dataIndex["is_fly"], ( output:QueryData(0,dataIndex["is_fly"]) or 0 ))
	
	---Çå¿Õ·¨ÉíÐ§¹û
	output:UpdateData(0,dataIndex["buff_type"], 0)
	output:UpdateData(0,dataIndex["ext_id"], 0)
	
	---Èç¹ûÃ»ÓÐÔö³¤£¬»òÕßÔö³¤0µµ
	if growDirInData == 0 or growGrades == 0 then
		---²»×÷ÈÎºÎ¶îÍâÊÂÇé
	
	---Èç¹ûÈ·ÓÐÔö³¤
	else
		---È¡µÃ±¾³É³¤·½ÏòµÄ³É³¤ÂÊµµÎ»Êý
		local grade = ZLuaTalGetGradeFromValue(growDirInData, talisman)
		---È¡µÃ±¾³É³¤·½ÏòµÄµµÎ»ÉÏÏÞ(2008.08.21ÐÞ¸Ä,ÒýÈë·¨±¦µµÎ»ÌáÉýÖµ) 
		local growdirection = ZluaTransRiseIndex(growDirInData)
		local gradeUpperLimit = gradesUpperLimit[growDirInGrade]+output:QueryData(0,growdirection)
		---Èç¹û³É³¤ÒÔºóµÄµµÎ»³¬¹ýÁËµµÎ»ÉÏÏÞ£¬ÄÇÃ´¾ÍÖÃÎªµµÎ»ÉÏÏÞ
		local newGrade = math.max(1,math.min(gradeUpperLimit, grade + growGrades))
		---³É³¤
		local delta = ( valueGrades[growDirInData][50] - valueGrades[growDirInData][1] ) / 49
		local v0 = valueGrades[growDirInData][1] - delta
		output:UpdateData(0,growDirInData, math.floor( ( v0 + newGrade * delta ) *100)/100)
		---Èç¹û×îÐ¡¹¥»÷³¬¹ýÁË×î´ó¹¥»÷£¬ÔòÇ¿ÖÆ°Ñ×îÐ¡¹¥»÷±äÎª×î´ó¹¥»÷
		if output:QueryData(0,dataIndex["grow_damage_low"]) > output:QueryData(0,dataIndex["grow_damage_high"]) then
			output:UpdateData(0,dataIndex["grow_damage_low"],output:QueryData(0,dataIndex["grow_damage_high"]))
		end
		
		---¼ÇÔØÏÂÃ¿¸öÊôÐÔµÄµµÎ»£¬³õÊ¼ÖµºÍ³É³¤ÂÊÓÃÓÚ´«ÈëÆ·½×º¯Êý»ñÈ¡Æ·½×£¬³É³¤ÂÊÓÃÓÚ±¾º¯ÊýÄÚÅÐ¶ÏÊÇ·ñ³¬¹ý×ÜµµÎ»
		local grades = {}
		grades[dataIndex["default_hp"]] 						= ZLuaTalGetGradeFromValue(dataIndex["default_hp"],output)
		grades[dataIndex["default_mp"]] 						= ZLuaTalGetGradeFromValue(dataIndex["default_mp"],output)
		grades[dataIndex["default_damage_low"]] 		= ZLuaTalGetGradeFromValue(dataIndex["default_damage_low"],output)
		grades[dataIndex["default_damage_high"]] 		= ZLuaTalGetGradeFromValue(dataIndex["default_damage_high"],output)
		grades[dataIndex["default_resis_stun"]] 		= ZLuaTalGetGradeFromValue(dataIndex["default_resis_stun"],output)
		grades[dataIndex["default_resis_sleep"]] 		= ZLuaTalGetGradeFromValue(dataIndex["default_resis_sleep"],output)
		grades[dataIndex["default_resis_silence"]] 	= ZLuaTalGetGradeFromValue(dataIndex["default_resis_silence"],output)
		grades[dataIndex["default_resis_weakness"]] = ZLuaTalGetGradeFromValue(dataIndex["default_resis_weakness"],output)
		grades[dataIndex["default_resis_enlace"]] 	= ZLuaTalGetGradeFromValue(dataIndex["default_resis_enlace"],output)
		
		grades[dataIndex["grow_hp"]] 								= ZLuaTalGetGradeFromValue(dataIndex["grow_hp"],output)
		grades[dataIndex["grow_mp"]] 								= ZLuaTalGetGradeFromValue(dataIndex["grow_mp"],output)
		grades[dataIndex["grow_damage_low"]] 				= ZLuaTalGetGradeFromValue(dataIndex["grow_damage_low"],output)
		grades[dataIndex["grow_damage_high"]] 			= ZLuaTalGetGradeFromValue(dataIndex["grow_damage_high"],output)
		grades[dataIndex["grow_resis_stun"]] 				= ZLuaTalGetGradeFromValue(dataIndex["grow_resis_stun"],output)
		grades[dataIndex["grow_resis_sleep"]] 			= ZLuaTalGetGradeFromValue(dataIndex["grow_resis_sleep"],output)
		grades[dataIndex["grow_resis_silence"]] 		= ZLuaTalGetGradeFromValue(dataIndex["grow_resis_silence"],output)
		grades[dataIndex["grow_resis_weakness"]] 		= ZLuaTalGetGradeFromValue(dataIndex["grow_resis_weakness"],output)
		grades[dataIndex["grow_resis_enlace"]] 			= ZLuaTalGetGradeFromValue(dataIndex["grow_resis_enlace"],output)
			
		---Èç¹û×ÜÌå³¬±ê¾ÍÁíÍâËæ»úÕÒÒ»¸öÊôÐÔ½µÒ»µ²
		local sumGrades = 0
		sumGrades = sumGrades + grades[dataIndex["grow_hp"]]
		sumGrades = sumGrades + grades[dataIndex["grow_mp"]]
		sumGrades = sumGrades + grades[dataIndex["grow_damage_low"]]
		sumGrades = sumGrades + grades[dataIndex["grow_damage_high"]]
		sumGrades = sumGrades + grades[dataIndex["grow_resis_stun"]]
		sumGrades = sumGrades + grades[dataIndex["grow_resis_sleep"]]
		sumGrades = sumGrades + grades[dataIndex["grow_resis_silence"]]
		sumGrades = sumGrades + grades[dataIndex["grow_resis_weakness"]]
		sumGrades = sumGrades + grades[dataIndex["grow_resis_enlace"]]
		if sumGrades > (gradesUpperLimit[gradeIndex["all"]]+output:QueryData(0,dataIndex["all_rise"])) then
			local randomSpace = {}
			if growDirInData ~= dataIndex["grow_hp"] then table.insert(randomSpace,dataIndex["grow_hp"]) end
			if growDirInData ~= dataIndex["grow_mp"] then table.insert(randomSpace,dataIndex["grow_mp"]) end
			if growDirInData ~= dataIndex["grow_damage_low"] then table.insert(randomSpace,dataIndex["grow_damage_low"]) end
			if growDirInData ~= dataIndex["grow_damage_high"] then table.insert(randomSpace,dataIndex["grow_damage_high"]) end
			if growDirInData ~= dataIndex["grow_resis_stun"] then table.insert(randomSpace,dataIndex["grow_resis_stun"]) end
			if growDirInData ~= dataIndex["grow_resis_sleep"] then table.insert(randomSpace,dataIndex["grow_resis_sleep"]) end
			if growDirInData ~= dataIndex["grow_resis_silence"] then table.insert(randomSpace,dataIndex["grow_resis_silence"]) end
			if growDirInData ~= dataIndex["grow_resis_weakness"] then table.insert(randomSpace,dataIndex["grow_resis_weakness"]) end
			if growDirInData ~= dataIndex["grow_resis_enlace"] then table.insert(randomSpace,dataIndex["grow_resis_enlace"]) end
			local dropAttrib = randomSpace[ math.floor(math.random()*table.getn(randomSpace)+1) ]
			local valueOneGrade = valueGrades[dropAttrib][2]-valueGrades[dropAttrib][1]
			output:UpdateData(0,dropAttrib, math.floor( (output:QueryData(0,dropAttrib)-valueOneGrade) *100) /100)
		end
			
		---ÖØÐÂÆÀ¼Û·¨±¦Æ·¼¶
		local quality = ZLuaTalGenerateQuality(id, serverOrigins["combine"], grades, nGrades)
		output:UpdateData(0,dataIndex["quality"], quality)
	end

	---ÉèÖÃÄ£°åID£¨ÒÔÈ·±£ÎÞtmpl_idË÷ÒýµÄÀÏ·¨±¦Ò²ÓµÓÐ´ËdataÏî£©
	output:UpdateData(0,dataIndex["tmpl_id"], id)
	
	---ÉèÖÃÌØÐÔ	£¨ÀÏ·¨±¦ÎÞ´ËdataÏî£©
	output:UpdateData(0,dataIndex["speciality"], speciality)
	
	---¼ÆËã³ö·¨±¦À´Ô´
	local clientOrigin = ZLuaTalGenerateClientOrigin(serverOrigins["combine"])
	output:UpdateData(0,dataIndex["origin"], clientOrigin)
	
	---¼ÆËã³öÉú³ÉÊ±¿Ì
	local gameHour12, gameMin3 = ZLuaTalGenerteBirthTime()
	output:UpdateData(0,dataIndex["birth_hour12"], gameHour12)
	output:UpdateData(0,dataIndex["birth_min3"], gameMin3)
	
	ZLuaTalRebuild(output,id);
end


-------¹¦ÄÜÀà----------------------------------------------------------------------------------------------------------------------------------

---¼ì²éÊÇ·ñ·¨±¦ÊÇ·ñ´Ë·¨±¦µÄÄ£°åID
function ZLuaTalExistInTmplIDs(tmplID)
	if type(tmplID) ~= "number" then return 0 end
	local availableTalismans = ZLuaTalAvailableTalismans()
	local talisman
	for _,talisman in pairs(availableTalismans) do
		if talisman.tmplID == tmplID then return talisman.requireLevel end
	end
	return 0
end


---a[]ÊÇµÈ²îÊýÁÐ£¬¸ø¶¨a[1]=startValueºÍa[allStep]=endValue£¬Çóa[currentStep]
function ZLuaTalGetValueWithBounds(startValue, endValue, currentStep, allStep)
	if type(startValue) ~= "number" or type(endValue) ~= "number" or type(currentStep) ~= "number" or type(allStep) ~= "number" then return -1 end
	if allStep == 1 then return -1 end
	return startValue + ( endValue - startValue ) / ( allStep - 1 ) * ( currentStep - 1 )	
end


---´Ó·¨±¦Ä³¸öÊôÐÔ¾ßÌåÖµÈ¡µÃµµÎ»Êý
function ZLuaTalGetGradeFromValue(index, talisman)		 
	local nGrades, valueGrades = ZLuaTalGrades()
	local delta = ( valueGrades[index][50] - valueGrades[index][1] ) / 49
	local v0 = valueGrades[index][1] - delta
	for i = 1, 555 do
		if talisman:QueryData(0,index) < v0 + ( i * delta ) then
			return i
		end
	end
	return 1
end

-- ·µ»ØÒ»¸öid´ú±íµÄ·¨±¦µÄÖÖÀà£¬"fabao" "feijian" "yushou" "chibang"
function ZLuaTalGetTalTypeFromID(tmplID)
	if type(tmplID) ~= "number" then return 0 end
	local availableTalismans = ZLuaTalAvailableTalismans()
	local talisman
	for _,talisman in pairs(availableTalismans) do
		if talisman.tmplID == tmplID then return talisman.talType end
	end
	return 0
end

--------µ÷ÊÔÓÃº¯Êý---------------------------------------------------------------------------------------------------

---ÓÃÓÚ°Ñ·¨±¦·þÎñÆ÷¶ËÊý¾ÝÏÔÊ¾µ½Ç°Ì¨£¨½ödebugÓÃ£¬ÇÐÇÐ£©
function ZLuaTalDebugDisplay(talisman,id,cid,output)		
	---Ê×ÏÈ°Ñ·¨±¦µÄÍâÔÚÊôÐÔ¼ÆËãÒ»±é
	local dataIndex, resisIndex = ZLuaTalDataIndex()
	local temp_cid=ZLuaTalisman_CombineAppendix()
	--
	if talisman:QueryData(0,dataIndex["reserve2"]) ~= 0 then 
		if temp_cid[cid].cidusetype ~= 1 then return end
		output:UpdateData(0,0,talisman:QueryData(0,1)) 
--		if output:QueryData(0,0) == 9 then return end
		return			
	elseif cid ~= 11784 or cid ~= 12021 then 
		return 
	end 
	--
	output:CreateItem(id)
	for _,index in pairs(dataIndex) do
		output:UpdateData(0,index, talisman:QueryData(0,index))
	end	
	output:InheritCombineLevel(true)	
	ZLuaTalRebuild(output,id)

	if cid == 11784 then
		---¸³ÓèÍâ²¿ÊôÐÔ
		output:SetHP((output:QueryData(0,dataIndex["combine_times"]) or 0))
		output:SetMP((output:QueryData(0,dataIndex["feed_baoqi"]) or 0))
		output:SetAttackEnhance((output:QueryData(0,dataIndex["feed_xianpin"]) or 0))
		output:SetDamage(0,output:QueryData(0,dataIndex["feed_shenpin"]) )
		output:SetMPEnhance((output:QueryData(0,dataIndex["feed_cost"]) or 0))
		output:SetHPEnhance((output:QueryData(0,dataIndex["change_skillnum"]) or 0))
		output:SetResistance(resisIndex["resis_stun"], (output:QueryData(0,dataIndex["feed_level_cost"]) or 0))
		output:SetResistance(resisIndex["resis_sleep"], (output:QueryData(0,dataIndex["enchant_times"]) or 0))
	elseif cid == 12021 then
		---°¤¸ö¸³ÓèÍâ²¿ÊôÐÔÒÔÄÚ²¿³É³¤ÂÊ
		output:SetHP(ZLuaTalDebugGetGradeFromValue(dataIndex["grow_hp"],talisman))
		output:SetMP(ZLuaTalDebugGetGradeFromValue(dataIndex["grow_mp"],talisman))
		output:SetDamage(ZLuaTalDebugGetGradeFromValue(dataIndex["grow_damage_low"],talisman),ZLuaTalDebugGetGradeFromValue(dataIndex["grow_damage_high"],talisman))
		output:SetResistance(resisIndex["resis_stun"], ZLuaTalDebugGetGradeFromValue(dataIndex["grow_resis_stun"],talisman))
		output:SetResistance(resisIndex["resis_sleep"], ZLuaTalDebugGetGradeFromValue(dataIndex["grow_resis_sleep"],talisman))
		output:SetResistance(resisIndex["resis_silence"], ZLuaTalDebugGetGradeFromValue(dataIndex["grow_resis_silence"],talisman))
		output:SetResistance(resisIndex["resis_weakness"], ZLuaTalDebugGetGradeFromValue(dataIndex["grow_resis_weakness"],talisman))
		output:SetResistance(resisIndex["resis_enlace"], ZLuaTalDebugGetGradeFromValue(dataIndex["grow_resis_enlace"],talisman))
	end
end

---Ê¹ÓÃÈÎÒâ·¨±¦×ö¸¨·¨±¦£¬ÐÂÑªÁ¶²ÄÁÏ×öÁéÃ½À´ÑªÁ¶ÃÅÅÉ75¼¶ºÍ105¼¶·¨±¦
function ZLuaTalMaxQuality(talisman1,id1,cid,output)
	if cid == 50975 or cid == 50976 then 
	else return end
		
	local dataIndex, resisIndex = ZLuaTalDataIndex()
	local gradeIndex = ZLuaTalGradeIndex()
	local nGrades,valueGrades = ZLuaTalGrades()
	local gradesUpperLimit = ZLuaTalGradesUpperLimit(id1)
	local serverOrigins = ZLuaTalServerOrigins()

	output:CreateItem(id1)
	for _,index in pairs(dataIndex) do
		output:UpdateData(0,index,talisman1:QueryData(0,index))
	end

	output:UpdateData(0,dataIndex["hp_rise"],10)
	output:UpdateData(0,dataIndex["mp_rise"],10)
	output:UpdateData(0,dataIndex["minattack_rise"],10)
	output:UpdateData(0,dataIndex["maxattack_rise"],10)
	output:UpdateData(0,dataIndex["stun_rise"],10)
	output:UpdateData(0,dataIndex["weakness_rise"],10)
	output:UpdateData(0,dataIndex["enlace_rise"],10)
	output:UpdateData(0,dataIndex["silence_rise"],10)
	output:UpdateData(0,dataIndex["sleep_rise"],10)
	output:UpdateData(0,dataIndex["all_rise"],90)
	output:InheritCombineLevel(true)

	local grades = {}
	grades[dataIndex["default_hp"]] 						= ZLuaTalGetGradeFromValue(dataIndex["default_hp"],output)
	grades[dataIndex["default_mp"]] 						= ZLuaTalGetGradeFromValue(dataIndex["default_mp"],output)
	grades[dataIndex["default_damage_low"]] 		= ZLuaTalGetGradeFromValue(dataIndex["default_damage_low"],output)
	grades[dataIndex["default_damage_high"]] 		= ZLuaTalGetGradeFromValue(dataIndex["default_damage_high"],output)
	grades[dataIndex["default_resis_stun"]] 		= ZLuaTalGetGradeFromValue(dataIndex["default_resis_stun"],output)
	grades[dataIndex["default_resis_sleep"]] 		= ZLuaTalGetGradeFromValue(dataIndex["default_resis_sleep"],output)
	grades[dataIndex["default_resis_silence"]] 	= ZLuaTalGetGradeFromValue(dataIndex["default_resis_silence"],output)
	grades[dataIndex["default_resis_weakness"]] = ZLuaTalGetGradeFromValue(dataIndex["default_resis_weakness"],output)
	grades[dataIndex["default_resis_enlace"]] 	= ZLuaTalGetGradeFromValue(dataIndex["default_resis_enlace"],output)

	grades[dataIndex["grow_hp"]] 								= ZLuaTalGetGradeFromValue(dataIndex["grow_hp"],output)
	grades[dataIndex["grow_mp"]] 								= ZLuaTalGetGradeFromValue(dataIndex["grow_mp"],output)
	grades[dataIndex["grow_damage_low"]] 				= ZLuaTalGetGradeFromValue(dataIndex["grow_damage_low"],output)
	grades[dataIndex["grow_damage_high"]] 			= ZLuaTalGetGradeFromValue(dataIndex["grow_damage_high"],output)
	grades[dataIndex["grow_resis_stun"]] 				= ZLuaTalGetGradeFromValue(dataIndex["grow_resis_stun"],output)
	grades[dataIndex["grow_resis_sleep"]] 			= ZLuaTalGetGradeFromValue(dataIndex["grow_resis_sleep"],output)
	grades[dataIndex["grow_resis_silence"]] 		= ZLuaTalGetGradeFromValue(dataIndex["grow_resis_silence"],output)
	grades[dataIndex["grow_resis_weakness"]] 		= ZLuaTalGetGradeFromValue(dataIndex["grow_resis_weakness"],output)
	grades[dataIndex["grow_resis_enlace"]] 			= ZLuaTalGetGradeFromValue(dataIndex["grow_resis_enlace"],output)

	local sumGrades = 0
	sumGrades = sumGrades + grades[dataIndex["grow_hp"]]
	sumGrades = sumGrades + grades[dataIndex["grow_mp"]]
	sumGrades = sumGrades + grades[dataIndex["grow_damage_low"]]
	sumGrades = sumGrades + grades[dataIndex["grow_damage_high"]]
	sumGrades = sumGrades + grades[dataIndex["grow_resis_stun"]]
	sumGrades = sumGrades + grades[dataIndex["grow_resis_sleep"]]
	sumGrades = sumGrades + grades[dataIndex["grow_resis_silence"]]
	sumGrades = sumGrades + grades[dataIndex["grow_resis_weakness"]]
	sumGrades = sumGrades + grades[dataIndex["grow_resis_enlace"]]
	
	local i
	local m = (gradesUpperLimit[gradeIndex["all"]]+output:QueryData(0,dataIndex["all_rise"])) - sumGrades
	local delta
	local v0
	
		--//¾¡Á¿¼ÓÂúÕæÆø
	local n = (gradesUpperLimit[gradeIndex["mp"]]+output:QueryData(0,dataIndex["mp_rise"])) - grades[dataIndex["grow_mp"]]
	if m > 0 and n > 0 then
		i = math.min(m,n)
		grades[dataIndex["grow_mp"]] = grades[dataIndex["grow_mp"]] + i
		sumGrades = sumGrades + i
		m = m - i
		delta = ( valueGrades[dataIndex["grow_mp"]][50] - valueGrades[dataIndex["grow_mp"]][1] ) / 49
		v0 = valueGrades[dataIndex["grow_mp"]][1] - delta
		output:UpdateData(0,dataIndex["grow_mp"], math.floor( ( v0 + grades[dataIndex["grow_mp"]] * delta ) *100)/100)
	end
	
	
	--//¾¡Á¿¼ÓÂúweakness
	local n = (gradesUpperLimit[gradeIndex["resis_weakness"]]+output:QueryData(0,dataIndex["weakness_rise"])) - grades[dataIndex["grow_resis_weakness"]]
	if m > 0 and n > 0 then
		i = math.min(m,n)
		grades[dataIndex["grow_resis_weakness"]] = grades[dataIndex["grow_resis_weakness"]] + i
		sumGrades = sumGrades + i
		m = m - i
		delta = ( valueGrades[dataIndex["grow_resis_weakness"]][50] - valueGrades[dataIndex["grow_resis_weakness"]][1] ) / 49
		v0 = valueGrades[dataIndex["grow_resis_weakness"]][1] - delta
		output:UpdateData(0,dataIndex["grow_resis_weakness"], math.floor( ( v0 + grades[dataIndex["grow_resis_weakness"]] * delta ) *100)/100)
	end

	--//¾¡Á¿¼ÓÂúÆøÑª
	local n = (gradesUpperLimit[gradeIndex["hp"]]+output:QueryData(0,dataIndex["hp_rise"])) - grades[dataIndex["grow_hp"]]
	if m > 0 and n > 0 then
		i = math.min(m,n)
		grades[dataIndex["grow_hp"]] = grades[dataIndex["grow_hp"]] + i
		sumGrades = sumGrades + i
		m = m - i
		delta = ( valueGrades[dataIndex["grow_hp"]][50] - valueGrades[dataIndex["grow_hp"]][1] ) / 49
		v0 = valueGrades[dataIndex["grow_hp"]][1] - delta
		output:UpdateData(0,dataIndex["grow_hp"], math.floor( ( v0 + grades[dataIndex["grow_hp"]] * delta ) *100)/100)
	end

	--//¾¡Á¿¼ÓÂúenlace
	local n = (gradesUpperLimit[gradeIndex["resis_enlace"]]+output:QueryData(0,dataIndex["enlace_rise"])) - grades[dataIndex["grow_resis_enlace"]]
	if m > 0 and n > 0 then
		i = math.min(m,n)
		grades[dataIndex["grow_resis_enlace"]] = grades[dataIndex["grow_resis_enlace"]] + i
		sumGrades = sumGrades + i
		m = m - i
		delta = ( valueGrades[dataIndex["grow_resis_enlace"]][50] - valueGrades[dataIndex["grow_resis_enlace"]][1] ) / 49
		v0 = valueGrades[dataIndex["grow_resis_enlace"]][1] - delta
		output:UpdateData(0,dataIndex["grow_resis_enlace"], math.floor( ( v0 + grades[dataIndex["grow_resis_enlace"]] * delta ) *100)/100)
	end

	--//¾¡Á¿¼ÓÂústun
	local n = (gradesUpperLimit[gradeIndex["resis_stun"]]+output:QueryData(0,dataIndex["stun_rise"])) - grades[dataIndex["grow_resis_stun"]]
	if m > 0 and n > 0 then
		i = math.min(m,n)
		grades[dataIndex["grow_resis_stun"]] = grades[dataIndex["grow_resis_stun"]] + i
		sumGrades = sumGrades + i
		m = m - i
		delta = ( valueGrades[dataIndex["grow_resis_stun"]][50] - valueGrades[dataIndex["grow_resis_stun"]][1] ) / 49
		v0 = valueGrades[dataIndex["grow_resis_stun"]][1] - delta
		output:UpdateData(0,dataIndex["grow_resis_stun"], math.floor( ( v0 + grades[dataIndex["grow_resis_stun"]] * delta ) *100)/100)
	end
	
	--//¾¡Á¿¼ÓÂúsleep
	local n = (gradesUpperLimit[gradeIndex["resis_sleep"]]+output:QueryData(0,dataIndex["sleep_rise"])) - grades[dataIndex["grow_resis_sleep"]]
	if m > 0 and n > 0 then
		i = math.min(m,n)
		grades[dataIndex["grow_resis_sleep"]] = grades[dataIndex["grow_resis_sleep"]] + i
		sumGrades = sumGrades + i
		m = m - i
		delta = ( valueGrades[dataIndex["grow_resis_sleep"]][50] - valueGrades[dataIndex["grow_resis_sleep"]][1] ) / 49
		v0 = valueGrades[dataIndex["grow_resis_sleep"]][1] - delta
		output:UpdateData(0,dataIndex["grow_resis_sleep"], math.floor( ( v0 + grades[dataIndex["grow_resis_sleep"]] * delta ) *100)/100)
	end
	
	--//¾¡Á¿¼ÓÂúsilence
	local n = (gradesUpperLimit[gradeIndex["resis_silence"]]+output:QueryData(0,dataIndex["silence_rise"])) - grades[dataIndex["grow_resis_silence"]]
	if m > 0 and n > 0 then
		i = math.min(m,n)
		grades[dataIndex["grow_resis_silence"]] = grades[dataIndex["grow_resis_silence"]] + i
		sumGrades = sumGrades + i
		m = m - i
		delta = ( valueGrades[dataIndex["grow_resis_silence"]][50] - valueGrades[dataIndex["grow_resis_silence"]][1] ) / 49
		v0 = valueGrades[dataIndex["grow_resis_silence"]][1] - delta
		output:UpdateData(0,dataIndex["grow_resis_silence"], math.floor( ( v0 + grades[dataIndex["grow_resis_silence"]] * delta ) *100)/100)
	end
	
		
	--//¾¡Á¿¼ÓÂú´ó¹¥
	local n = (gradesUpperLimit[gradeIndex["damage_high"]]+output:QueryData(0,dataIndex["maxattack_rise"])) - grades[dataIndex["grow_damage_high"]]
	if m > 0 and n > 0 then
		i = math.min(m,n)
		grades[dataIndex["grow_damage_high"]] = grades[dataIndex["grow_damage_high"]] + i
		sumGrades = sumGrades + i
		m = m - i
		delta = ( valueGrades[dataIndex["grow_damage_high"]][50] - valueGrades[dataIndex["grow_damage_high"]][1] ) / 49
		v0 = valueGrades[dataIndex["grow_damage_high"]][1] - delta
		output:UpdateData(0,dataIndex["grow_damage_high"], math.floor( ( v0 + grades[dataIndex["grow_damage_high"]] * delta ) *100)/100)
	end

	--//¾¡Á¿¼ÓÂúÐ¡¹¥
	local n = (gradesUpperLimit[gradeIndex["damage_low"]]+output:QueryData(0,dataIndex["minattack_rise"])) - grades[dataIndex["grow_damage_low"]]
	if m > 0 and n > 0 then
		i = math.min(m,n)
		grades[dataIndex["grow_damage_low"]] = grades[dataIndex["grow_damage_low"]] + i
		sumGrades = sumGrades + i
		m = m - i
		delta = ( valueGrades[dataIndex["grow_damage_low"]][50] - valueGrades[dataIndex["grow_damage_low"]][1] ) / 49
		v0 = valueGrades[dataIndex["grow_damage_low"]][1] - delta
		output:UpdateData(0,dataIndex["grow_damage_low"], math.floor( ( v0 + grades[dataIndex["grow_damage_low"]] * delta ) *100)/100)
	end
	
	

	---ÖØÐÂÆÀ¼Û·¨±¦Æ·¼¶
	local quality = ZLuaTalGenerateQuality(id1, serverOrigins["combine"], grades, nGrades)
	output:UpdateData(0,dataIndex["quality"], quality)
	local clientOrigin = ZLuaTalGenerateClientOrigin(serverOrigins["combine"])
	output:UpdateData(0,dataIndex["origin"], clientOrigin)

	ZLuaTalRebuild(output,id1)
	
	
end

---Ê¹ÓÃÁéÃ½Ö®ÍõºÍÊ³Îï·¨±¦£¬½«·¨±¦µÄÖ÷ÊôÐÔÒ»´ÎÐÔÎ¹Âú£¨debugÊ¹ÓÃ£©
function ZLuaTalDebugMaxGrade(talisman1,id1,cid,output)
	if cid ~= 22017 then return end
	local dataIndex, resisIndex = ZLuaTalDataIndex()
	local gradeIndex = ZLuaTalGradeIndex()
	local nGrades,valueGrades = ZLuaTalGrades()
	local gradesUpperLimit = ZLuaTalGradesUpperLimit(id1)
	local serverOrigins = ZLuaTalServerOrigins()

	output:CreateItem(id1)
	for _,index in pairs(dataIndex) do
		output:UpdateData(0,index,talisman1:QueryData(0,index))
	end

	output:UpdateData(0,dataIndex["hp_rise"],10)
	output:UpdateData(0,dataIndex["mp_rise"],10)
	output:UpdateData(0,dataIndex["minattack_rise"],10)
	output:UpdateData(0,dataIndex["maxattack_rise"],10)
	output:UpdateData(0,dataIndex["stun_rise"],10)
	output:UpdateData(0,dataIndex["weakness_rise"],10)
	output:UpdateData(0,dataIndex["enlace_rise"],10)
	output:UpdateData(0,dataIndex["silence_rise"],10)
	output:UpdateData(0,dataIndex["sleep_rise"],10)
	output:UpdateData(0,dataIndex["all_rise"],90)
	output:InheritCombineLevel(true)

	local grades = {}
	grades[dataIndex["default_hp"]] 						= ZLuaTalGetGradeFromValue(dataIndex["default_hp"],output)
	grades[dataIndex["default_mp"]] 						= ZLuaTalGetGradeFromValue(dataIndex["default_mp"],output)
	grades[dataIndex["default_damage_low"]] 		= ZLuaTalGetGradeFromValue(dataIndex["default_damage_low"],output)
	grades[dataIndex["default_damage_high"]] 		= ZLuaTalGetGradeFromValue(dataIndex["default_damage_high"],output)
	grades[dataIndex["default_resis_stun"]] 		= ZLuaTalGetGradeFromValue(dataIndex["default_resis_stun"],output)
	grades[dataIndex["default_resis_sleep"]] 		= ZLuaTalGetGradeFromValue(dataIndex["default_resis_sleep"],output)
	grades[dataIndex["default_resis_silence"]] 	= ZLuaTalGetGradeFromValue(dataIndex["default_resis_silence"],output)
	grades[dataIndex["default_resis_weakness"]] = ZLuaTalGetGradeFromValue(dataIndex["default_resis_weakness"],output)
	grades[dataIndex["default_resis_enlace"]] 	= ZLuaTalGetGradeFromValue(dataIndex["default_resis_enlace"],output)

	grades[dataIndex["grow_hp"]] 								= ZLuaTalGetGradeFromValue(dataIndex["grow_hp"],output)
	grades[dataIndex["grow_mp"]] 								= ZLuaTalGetGradeFromValue(dataIndex["grow_mp"],output)
	grades[dataIndex["grow_damage_low"]] 				= ZLuaTalGetGradeFromValue(dataIndex["grow_damage_low"],output)
	grades[dataIndex["grow_damage_high"]] 			= ZLuaTalGetGradeFromValue(dataIndex["grow_damage_high"],output)
	grades[dataIndex["grow_resis_stun"]] 				= ZLuaTalGetGradeFromValue(dataIndex["grow_resis_stun"],output)
	grades[dataIndex["grow_resis_sleep"]] 			= ZLuaTalGetGradeFromValue(dataIndex["grow_resis_sleep"],output)
	grades[dataIndex["grow_resis_silence"]] 		= ZLuaTalGetGradeFromValue(dataIndex["grow_resis_silence"],output)
	grades[dataIndex["grow_resis_weakness"]] 		= ZLuaTalGetGradeFromValue(dataIndex["grow_resis_weakness"],output)
	grades[dataIndex["grow_resis_enlace"]] 			= ZLuaTalGetGradeFromValue(dataIndex["grow_resis_enlace"],output)

	local sumGrades = 0
	sumGrades = sumGrades + grades[dataIndex["grow_hp"]]
	sumGrades = sumGrades + grades[dataIndex["grow_mp"]]
	sumGrades = sumGrades + grades[dataIndex["grow_damage_low"]]
	sumGrades = sumGrades + grades[dataIndex["grow_damage_high"]]
	sumGrades = sumGrades + grades[dataIndex["grow_resis_stun"]]
	sumGrades = sumGrades + grades[dataIndex["grow_resis_sleep"]]
	sumGrades = sumGrades + grades[dataIndex["grow_resis_silence"]]
	sumGrades = sumGrades + grades[dataIndex["grow_resis_weakness"]]
	sumGrades = sumGrades + grades[dataIndex["grow_resis_enlace"]]
	
	local i
	local m = (gradesUpperLimit[gradeIndex["all"]]+output:QueryData(0,dataIndex["all_rise"])) - sumGrades
	local delta
	local v0

	--//¾¡Á¿¼ÓÂúÕæÆø
	local n = (gradesUpperLimit[gradeIndex["mp"]]+output:QueryData(0,dataIndex["mp_rise"])) - grades[dataIndex["grow_mp"]]
	if m > 0 and n > 0 then
		i = 0
		grades[dataIndex["grow_mp"]] = grades[dataIndex["grow_mp"]] + i
		sumGrades = sumGrades + i
		--//m = m - i
		delta = ( valueGrades[dataIndex["grow_mp"]][50] - valueGrades[dataIndex["grow_mp"]][1] ) / 49
		v0 = valueGrades[dataIndex["grow_mp"]][1] - delta
		output:UpdateData(0,dataIndex["grow_mp"], math.floor( 1 )/100)
	end
	--//¾¡Á¿¼ÓÂústun
	local n = (gradesUpperLimit[gradeIndex["resis_stun"]]+output:QueryData(0,dataIndex["stun_rise"])) - grades[dataIndex["grow_resis_stun"]]
	if m > 0 and n > 0 then
		i = math.min(m,n)
		grades[dataIndex["grow_resis_stun"]] = grades[dataIndex["grow_resis_stun"]] + i
		sumGrades = sumGrades + i
		m = m - i
		delta = ( valueGrades[dataIndex["grow_resis_stun"]][50] - valueGrades[dataIndex["grow_resis_stun"]][1] ) / 49
		v0 = valueGrades[dataIndex["grow_resis_stun"]][1] - delta
		output:UpdateData(0,dataIndex["grow_resis_stun"], math.floor( ( v0 + grades[dataIndex["grow_resis_stun"]] * delta ) *100)/100)
	end
	--//¾¡Á¿¼ÓÂúsleep
	local n = (gradesUpperLimit[gradeIndex["resis_sleep"]]+output:QueryData(0,dataIndex["sleep_rise"])) - grades[dataIndex["grow_resis_sleep"]]
	if m > 0 and n > 0 then
		i = math.min(m,n)
		grades[dataIndex["grow_resis_sleep"]] = grades[dataIndex["grow_resis_sleep"]] + i
		sumGrades = sumGrades + i
		m = m - i
		delta = ( valueGrades[dataIndex["grow_resis_sleep"]][50] - valueGrades[dataIndex["grow_resis_sleep"]][1] ) / 49
		v0 = valueGrades[dataIndex["grow_resis_sleep"]][1] - delta
		output:UpdateData(0,dataIndex["grow_resis_sleep"], math.floor( ( v0 + grades[dataIndex["grow_resis_sleep"]] * delta ) *100)/100)
	end
	--//¾¡Á¿¼ÓÂúsilence
	local n = (gradesUpperLimit[gradeIndex["resis_silence"]]+output:QueryData(0,dataIndex["silence_rise"])) - grades[dataIndex["grow_resis_silence"]]
	if m > 0 and n > 0 then
		i = math.min(m,n)
		grades[dataIndex["grow_resis_silence"]] = grades[dataIndex["grow_resis_silence"]] + i
		sumGrades = sumGrades + i
		m = m - i
		delta = ( valueGrades[dataIndex["grow_resis_silence"]][50] - valueGrades[dataIndex["grow_resis_silence"]][1] ) / 49
		v0 = valueGrades[dataIndex["grow_resis_silence"]][1] - delta
		output:UpdateData(0,dataIndex["grow_resis_silence"], math.floor( ( v0 + grades[dataIndex["grow_resis_silence"]] * delta ) *100)/100)
	end
	--//¾¡Á¿¼ÓÂúenlace
	local n = (gradesUpperLimit[gradeIndex["resis_enlace"]]+output:QueryData(0,dataIndex["enlace_rise"])) - grades[dataIndex["grow_resis_enlace"]]
	if m > 0 and n > 0 then
		i = math.min(m,n)
		grades[dataIndex["grow_resis_enlace"]] = grades[dataIndex["grow_resis_enlace"]] + i
		sumGrades = sumGrades + i
		m = m - i
		delta = ( valueGrades[dataIndex["grow_resis_enlace"]][50] - valueGrades[dataIndex["grow_resis_enlace"]][1] ) / 49
		v0 = valueGrades[dataIndex["grow_resis_enlace"]][1] - delta
		output:UpdateData(0,dataIndex["grow_resis_enlace"], math.floor( ( v0 + grades[dataIndex["grow_resis_enlace"]] * delta ) *100)/100)
	end
	--//¾¡Á¿¼ÓÂúweakness
	local n = (gradesUpperLimit[gradeIndex["resis_weakness"]]+output:QueryData(0,dataIndex["weakness_rise"])) - grades[dataIndex["grow_resis_weakness"]]
	if m > 0 and n > 0 then
		i = math.min(m,n)
		grades[dataIndex["grow_resis_weakness"]] = grades[dataIndex["grow_resis_weakness"]] + i
		sumGrades = sumGrades + i
		m = m - i
		delta = ( valueGrades[dataIndex["grow_resis_weakness"]][50] - valueGrades[dataIndex["grow_resis_weakness"]][1] ) / 49
		v0 = valueGrades[dataIndex["grow_resis_weakness"]][1] - delta
		output:UpdateData(0,dataIndex["grow_resis_weakness"], math.floor( ( v0 + grades[dataIndex["grow_resis_weakness"]] * delta ) *100)/100)
	end
	--//¾¡Á¿¼ÓÂú´ó¹¥
	local n = (gradesUpperLimit[gradeIndex["damage_high"]]+output:QueryData(0,dataIndex["maxattack_rise"])) - grades[dataIndex["grow_damage_high"]]
	if m > 0 and n > 0 then
		i = math.min(m,n)
		grades[dataIndex["grow_damage_high"]] = grades[dataIndex["grow_damage_high"]] + i
		sumGrades = sumGrades + i
		m = m - i
		delta = ( valueGrades[dataIndex["grow_damage_high"]][50] - valueGrades[dataIndex["grow_damage_high"]][1] ) / 49
		v0 = valueGrades[dataIndex["grow_damage_high"]][1] - delta
		output:UpdateData(0,dataIndex["grow_damage_high"], math.floor( ( v0 + grades[dataIndex["grow_damage_high"]] * delta ) *100)/100)
	end
	--//¾¡Á¿¼ÓÂúÆøÑª
	local n = (gradesUpperLimit[gradeIndex["hp"]]+output:QueryData(0,dataIndex["hp_rise"])) - grades[dataIndex["grow_hp"]]
	if m > 0 and n > 0 then
		i = math.min(m,n)
		grades[dataIndex["grow_hp"]] = grades[dataIndex["grow_hp"]] + i
		sumGrades = sumGrades + i
		m = m - i
		delta = ( valueGrades[dataIndex["grow_hp"]][50] - valueGrades[dataIndex["grow_hp"]][1] ) / 49
		v0 = valueGrades[dataIndex["grow_hp"]][1] - delta
		output:UpdateData(0,dataIndex["grow_hp"], math.floor( ( v0 + grades[dataIndex["grow_hp"]] * delta ) *100)/100)
	end
	--//¾¡Á¿¼ÓÂúÐ¡¹¥
	local n = (gradesUpperLimit[gradeIndex["damage_low"]]+output:QueryData(0,dataIndex["minattack_rise"])) - grades[dataIndex["grow_damage_low"]]
	if m > 0 and n > 0 then
		i = math.min(m,n)
		grades[dataIndex["grow_damage_low"]] = grades[dataIndex["grow_damage_low"]] + i
		sumGrades = sumGrades + i
		m = m - i
		delta = ( valueGrades[dataIndex["grow_damage_low"]][50] - valueGrades[dataIndex["grow_damage_low"]][1] ) / 49
		v0 = valueGrades[dataIndex["grow_damage_low"]][1] - delta
		output:UpdateData(0,dataIndex["grow_damage_low"], math.floor( ( v0 + grades[dataIndex["grow_damage_low"]] * delta ) *100)/100)
	end

	---ÖØÐÂÆÀ¼Û·¨±¦Æ·¼¶
	local quality = ZLuaTalGenerateQuality(id1, serverOrigins["combine"], grades, nGrades)
	output:UpdateData(0,dataIndex["quality"], quality)

	-- ÐÂ·¨±¦²âÊÔ¼¼ÄÜÓÃ
	local debug_TalismanSkillIDs={}
	debug_TalismanSkillIDs[399]={399,1278,1279}---//_°ËØÔÊ¯.ext
	debug_TalismanSkillIDs[400]={400,1292,1293}---//_ÁùºÏ¾µ.ext
	debug_TalismanSkillIDs[401]={401,1326,1327}---//_ÖìÈ¸Ó¡.ext
	debug_TalismanSkillIDs[402]={402,1294,1295}---//_ÂÖ»ØÖé.ext
	debug_TalismanSkillIDs[403]={403,1308,1309}---//_ÉåÐÄÁå.ext
	debug_TalismanSkillIDs[404]={404,1298,1299}---//_ÆßÐÇ½£.ext
	debug_TalismanSkillIDs[405]={405,1318,1319}---//_ËÄÁéá¦.ext
	debug_TalismanSkillIDs[406]={406,1284,1285}---//_½ð¸ÕèÆ.ext
	debug_TalismanSkillIDs[407]={407,1280,1281}---//_¶àÇé»·.ext
	debug_TalismanSkillIDs[408]={408,1306,1307}---//_É½ºÓÉÈ.ext
	debug_TalismanSkillIDs[409]={409,1312,1313}---//_ÉñÄ¾÷».ext
	debug_TalismanSkillIDs[410]={410,1320,1321}---//_Íò¶¾´ü(Íò¶¾¶¦).ext
	debug_TalismanSkillIDs[411]={411,1300,1301}---//_Ç§×íÌ³(Óñ¾»Æ¿).ext
	debug_TalismanSkillIDs[860]={860,1282,1283}---//_çúçêÖìç±.ext
	debug_TalismanSkillIDs[861]={861,1324,1325}---//_ÒõÑô¾µ.ext
	debug_TalismanSkillIDs[862]={862,1296,1297}---//_ó´Áúá¦.ext
	debug_TalismanSkillIDs[863]={863,1286,1287}---//_½ðºùÂ«.ext
	debug_TalismanSkillIDs[864]={864,1290,1291}---//_Á«»¨.ext
	debug_TalismanSkillIDs[865]={865,1302,1303}---//_Ç¬À¤Çà¹â½ä.ext
	debug_TalismanSkillIDs[866]={866,1322,1323}---//_ÎåÁúÂÖ.ext
	debug_TalismanSkillIDs[867]={867,1288,1289}---//_¾ÅÁúÉñ»ðÕÖ.ext
	debug_TalismanSkillIDs[868]={868,1304,1305}---//_ÈçÒâÇ¬À¤´ü.ext
	debug_TalismanSkillIDs[869]={869,1310,1311}---//_Éã»ê.ext
	debug_TalismanSkillIDs[870]={870,1316,1317}---//_ÊÉÑªÖé.ext
	debug_TalismanSkillIDs[871]={871,1314,1315}---//_ÊÉ»ê.ext
	debug_TalismanSkillIDs[1653]={1653,1654,1655}---//_ÁáççËþ.ext
	debug_TalismanSkillIDs[1697]={1697,1698,1699}---//³à½ðç±.ext
	debug_TalismanSkillIDs[1700]={1700,1701,1702}---//_ÂÖ»ØÅÌ.ext
	debug_TalismanSkillIDs[1703]={1703,1704,1705}---//_Ç¬À¤Ëø.ext
	debug_TalismanSkillIDs[1706]={1706,1707,1708}---//ÓÄÚ¤¹íÊÖ_.ext
	debug_TalismanSkillIDs[1738]={1738,1739,1740}---//ÆßÐÇÓ¡.ext
	debug_TalismanSkillIDs[1753]={1753,1754,1755}---//ÕæÉ½ºÓÉÈ.ext
	debug_TalismanSkillIDs[1806]={1806,1807,1808}---//ÑªÓñ¹ÇÆ¬.ext
	debug_TalismanSkillIDs[1809]={1809,1810,1811}---//ÆÆÉ².ext
	debug_TalismanSkillIDs[1812]={1812,1813,1814}---//Ç§¶¾¹Æ.ext
	debug_TalismanSkillIDs[1815]={1815,1816,1817}---//¹íóÀ.ext
	debug_TalismanSkillIDs[2037]={2037,2038,2039}---//_·­ÌìÓ¡.ext
	debug_TalismanSkillIDs[2153]={2153,2154,2155}---//³¤Ã÷µÆ
	debug_TalismanSkillIDs[2598]={2598,2599,2600}---//ÈÕÔÂ±¦¼ø
	debug_TalismanSkillIDs[3088]={3088,3088,3088}---//ÔªÏüµÆ
	debug_TalismanSkillIDs[3162]={3162,3163,3164}---//¸¡ÍÀ
	debug_TalismanSkillIDs[3391]={3391,3392,3393}---//Òý»êá¦
	debug_TalismanSkillIDs[3825]={3825,3826,3827}---//±¦¿âÖ®»ê
	debug_TalismanSkillIDs[32954]={32954,32955,32956}---//Ììçð.ext

	debug_TalismanSkillIDs[3610]={3610,3611,3612}---//Òý»êá¦
	debug_TalismanSkillIDs[3613]={3613,3614,3615}---//Òý»êá¦
	debug_TalismanSkillIDs[3616]={3616,3617,3618}---//Òý»êá¦
	debug_TalismanSkillIDs[3619]={3619,3620,3621}---//Òý»êá¦
	debug_TalismanSkillIDs[3622]={3622,3623,3624}---//Òý»êá¦
	debug_TalismanSkillIDs[3625]={3625,3626,3627}---//Òý»êá¦
	debug_TalismanSkillIDs[3628]={3628,3629,3630}---//Òý»êá¦
	debug_TalismanSkillIDs[3631]={3631,3632,3633}---//Òý»êá¦
	debug_TalismanSkillIDs[3634]={3634,3635,3636}---//Òý»êá¦
	debug_TalismanSkillIDs[3943]={3943,3944,3945}---//»³¹â
	debug_TalismanSkillIDs[3946]={3946,3947,3948}---//»³¹â
	debug_TalismanSkillIDs[3949]={3949,3950,3951}---//»³¹â
	debug_TalismanSkillIDs[3952]={3952,3953,3954}---//»³¹â
	debug_TalismanSkillIDs[3955]={3955,3956,3957}---//Ìì»ª
	debug_TalismanSkillIDs[3958]={3958,3959,3960}---//Ìì»ª
	debug_TalismanSkillIDs[3961]={3961,3962,3963}---//Ìì»ª
	debug_TalismanSkillIDs[3964]={3964,3965,3966}---//Ìì»ª

	debug_TalismanSkillIDs[33347]={33347,33348,33349}---//·ÙÏã
	debug_TalismanSkillIDs[33350]={33350,33351,33352}---//·ÙÏã
	debug_TalismanSkillIDs[33353]={33353,33354,33355}---//·ÙÏã
	debug_TalismanSkillIDs[33356]={33356,33357,33358}---//·ÙÏã
	debug_TalismanSkillIDs[33359]={33359,33360,33361}---//Ì«ê»
	debug_TalismanSkillIDs[33362]={33362,33363,33364}---//Ì«ê»
	debug_TalismanSkillIDs[33365]={33365,33366,33367}---//Ì«ê»
	debug_TalismanSkillIDs[33368]={33368,33369,33370}---//Ì«ê»
	debug_TalismanSkillIDs[34015]={34015,34016,34017}---//³½»Ê
	debug_TalismanSkillIDs[34018]={34018,34019,34020}---//³½»Ê
	debug_TalismanSkillIDs[34021]={34021,34022,34023}---//³½»Ê
	debug_TalismanSkillIDs[34024]={34024,34025,34026}---//³½»Ê

	debug_TalismanSkillIDs[34662]={34662,34663,34664}---//Áé×å

	debug_TalismanSkillIDs[34667]={34667,34668,34669}---//Ç£»ú
	debug_TalismanSkillIDs[34672]={34672,34673,34674}---//Ç£»ú
	debug_TalismanSkillIDs[34677]={34677,34678,34679}---//Ç£»ú
	debug_TalismanSkillIDs[34682]={34682,34683,34684}---//Ç£»ú
	debug_TalismanSkillIDs[34687]={34687,34688,34689}---//Ó¢ÕÐ
	debug_TalismanSkillIDs[34692]={34692,34693,34694}---//Ó¢ÕÐ
	debug_TalismanSkillIDs[34697]={34697,34698,34699}---//Ó¢ÕÐ
	debug_TalismanSkillIDs[34702]={34702,34703,34704}---//Ó¢ÕÐ

  	debug_TalismanSkillIDs[33723]={33723,33724,33725}---//°ËØÔÉñÕÆ_°ËØÔÊ¯.ext
  	debug_TalismanSkillIDs[33729]={33729,33730,33731}---//ÁùºÏ×Ýºá_ÁùºÏ¾µ.ext
  	debug_TalismanSkillIDs[33732]={33732,33733,33734}---//ÁÒÑæÑªÓê_ÖìÈ¸Ó¡.ext
  	debug_TalismanSkillIDs[33735]={33735,33736,33737}---//·¨º£ÂÖ»Ø_ÂÖ»ØÖé.ext
  	debug_TalismanSkillIDs[33738]={33738,33739,33740}---//Éã»ê¶áÆÇ_ÉåÐÄÁå.ext
  	debug_TalismanSkillIDs[33765]={33765,33766,33767}---//Á÷ÐÇ¸ÏÔÂ_ÆßÐÇ½£.ext
  	debug_TalismanSkillIDs[33768]={33768,33769,33770}---//ËÄÁéÑªÖä_ËÄÁéá¦.ext
  	debug_TalismanSkillIDs[33771]={33771,33772,33773}---//½ð¸Õ½µÄ§_½ð¸ÕèÆ.ext
  	debug_TalismanSkillIDs[33774]={33774,33775,33776}---//É²ÄÇ·¼»ª_¶àÇé»·.ext
  	debug_TalismanSkillIDs[33879]={33879,33880,33881}---//½­É½Èç»­_É½ºÓÉÈ.ext
  	debug_TalismanSkillIDs[33873]={33873,33874,33875}---//·ñ¼«Ì©À´_ÉñÄ¾÷».ext
  	debug_TalismanSkillIDs[33882]={33882,33883,33884}---//Íò¶¾ÊÉÌì_Íò¶¾´ü(Íò¶¾¶¦).ext
  	debug_TalismanSkillIDs[33885]={33885,33886,33887}---//Óñ¾»Ìì»ª_Ç§×íÌ³(Óñ¾»Æ¿).ext
  	debug_TalismanSkillIDs[33801]={33801,33802,33803}---//Ç§ç±Ñ¤Îè_çúçêÖìç±.ext
  	debug_TalismanSkillIDs[33900]={33900,33901,33902}---//ÒõÑôÏàÉú_ÒõÑô¾µ.ext
  	debug_TalismanSkillIDs[33912]={33912,33913,33914}---//ò°ÁúÁÒÑ×_ó´Áúá¦.ext
  	debug_TalismanSkillIDs[33897]={33897,33898,33899}---//·½´çÇ¬À¤_½ðºùÂ«.ext
  	debug_TalismanSkillIDs[33810]={33810,33811,33812}---//°µÏãÎÞ¼Ê_Á«»¨.ext
  	debug_TalismanSkillIDs[33804]={33804,33805,33806}---//Çà¹âÕ§ÏÖ_Ç¬À¤Çà¹â½ä.ext
  	debug_TalismanSkillIDs[33915]={33915,33916,33917}---//ÎåÁúÓùÌì_ÎåÁúÂÖ.ext
  	debug_TalismanSkillIDs[33894]={33894,33895,33896}---//Á¶ÓüÉñ»ð_¾ÅÁúÉñ»ðÕÖ.ext
  	debug_TalismanSkillIDs[33807]={33807,33808,33809}---//Ç¬À¤ÈçÒâ_ÈçÒâÇ¬À¤´ü.ext
  	debug_TalismanSkillIDs[33930]={33930,33931,33932}---//Éã»êÐÄ³Û_Éã»ê.ext
  	debug_TalismanSkillIDs[33933]={33933,33934,33935}---//ÊÉÑªÐ°Áé_ÊÉÑªÖé.ext
  	debug_TalismanSkillIDs[33936]={33936,33937,33938}---//ÊÉ»êÎÞË«_ÊÉ»ê.ext
  --	TalismanSkillIDs[1653]={1653,1654,1655,33888,33889,33890}---//±¦ËþÕòºÓÑý_ÁáççËþ.ext
  	debug_TalismanSkillIDs[33846]={33846,33847,33848}---//ÃÎÁîÖ®À½_³à½ðç±.ext
  	debug_TalismanSkillIDs[33843]={33843,33844,33845}---//¼ÅÃðÌìÎè_ÂÖ»ØÅÌ.ext
  	debug_TalismanSkillIDs[33837]={33837,33838,33839}---//³¾¹âÌ¾Ãî_Ç¬À¤Ëø.ext
  	debug_TalismanSkillIDs[33840]={33840,33841,33842}---//¹íÓòÓÄ¸è_ÓÄÚ¤¹íÊÖ_.ext
  	debug_TalismanSkillIDs[33903]={33903,33904,33905}---//»ªÕÃÐÇÔÉ_ÆßÐÇÓ¡.ext
  	debug_TalismanSkillIDs[33906]={33906,33907,33908}---//åÐÒ£ÈçÃÎ_ÕæÉ½ºÓÉÈ.ext
  	debug_TalismanSkillIDs[33741]={33741,33742,33743}---//ËéÓñ»ÃÌì_ÑªÓñ¹ÇÆ¬.ext
  	debug_TalismanSkillIDs[33777]={33777,33778,33779}---//¾øìå»ª¹â_ÆÆÉ².ext
  	debug_TalismanSkillIDs[33813]={33813,33814,33815}---//ËÀÍö²¨ÎÆ_Ç§¶¾¹Æ.ext
  	debug_TalismanSkillIDs[33849]={33849,33850,33851}---//ÃÔ×ÙÖ®¼Å_¹íóÀ.ext
  	debug_TalismanSkillIDs[33918]={33918,33919,33920}---//ÎåÔÀÏÌ·þ_·­ÌìÓ¡.ext
  	debug_TalismanSkillIDs[33921]={33921,33922,33923}---//º®ÔÂÕòÌ¶_³¤Ã÷µÆ
  	debug_TalismanSkillIDs[33891]={33891,33892,33893}---//´ó·²Èô¼ò_ÈÕÔÂ±¦¼ø
  --	TalismanSkillIDs[3088]={3088,3088,3088}---//ÔªÏüµÆ×£_ÔªÏüµÆ
  	debug_TalismanSkillIDs[33909]={33909,33910,33911}---//Ê¥Ëþ½µÄ§_¸¡ÍÀ
  	debug_TalismanSkillIDs[33924]={33924,33925,33926}---//°×¹ÇÖ®Öä_Òý»êá¦
  	debug_TalismanSkillIDs[33927]={33927,33928,33929}---//±ÌÌìÓî»ª_±¦¿âÖ®»ê
  	debug_TalismanSkillIDs[33939]={33939,33940,33941}---//Ììçð¾øÊÀ_Ììçð.ext
  
  	debug_TalismanSkillIDs[33726]={33726,33727,33728}---//Íò²Ê»­Ò»_ÉñÒá
  	debug_TalismanSkillIDs[33747]={33747,33748,33749}---//Çý»¢Öð±ª_¾ÅÀè
  	debug_TalismanSkillIDs[33783]={33783,33784,33785}---//·ãÁÖÉ·Ñª_¾ÅÀè
  	debug_TalismanSkillIDs[33819]={33819,33820,33821}---//¿ñÀ½ÃðÉ±_¾ÅÀè
  	debug_TalismanSkillIDs[33855]={33855,33856,33857}---//Íö»êÉ¥ÆÇ_¾ÅÀè
  	debug_TalismanSkillIDs[33750]={33750,33751,33752}---//ÔÂÓ°µ¤Öì_ÁÒÉ½
  	debug_TalismanSkillIDs[33786]={33786,33787,33788}---//¶áÆÇ·µ»ê_ÁÒÉ½
  	debug_TalismanSkillIDs[33822]={33822,33823,33824}---//¸¡¹âÀ»Ó°_ÁÒÉ½
  	debug_TalismanSkillIDs[33858]={33858,33859,33860}---//êØ»ÊÉñÍþ_ÁÒÉ½
  	debug_TalismanSkillIDs[33753]={33753,33754,33755}---//ÑªÉ±Ç§Àï_»³¹â
  	debug_TalismanSkillIDs[33789]={33789,33790,33791}---//ºáÉ¨Ç§¾ü_»³¹â
  	debug_TalismanSkillIDs[33825]={33825,33826,33827}---//ÆÆ»êÇ§½Ù_»³¹â
  	debug_TalismanSkillIDs[33861]={33861,33862,33863}---//ÆÅæ¶Ó°»Ê_»³¹â
  	debug_TalismanSkillIDs[33756]={33756,33757,33758}---//Çí»ªÌìô¥_Ìì»ª
  	debug_TalismanSkillIDs[33792]={33792,33793,33794}---//²Ôñ··ï¸è_Ìì»ª
  	debug_TalismanSkillIDs[33828]={33828,33829,33830}---//Óî¹âÅîÀ³_Ìì»ª
  	debug_TalismanSkillIDs[33864]={33864,33865,33866}---//ÌìÄ§½µ·ü_Ìì»ª
  
  	debug_TalismanSkillIDs[33744]={33744,33745,33746}---//Òµ»ðÌìÑô_·ÙÏã
  	debug_TalismanSkillIDs[33780]={33780,33781,33782}---//ÌìµØÐþ¼«_·ÙÏã
  	debug_TalismanSkillIDs[33816]={33816,33817,33818}---//Áè»ðÃ÷Ñô_·ÙÏã
  	debug_TalismanSkillIDs[33852]={33852,33853,33854}---//Ì«ÓÄÐþ±ù_·ÙÏã
  	debug_TalismanSkillIDs[33759]={33759,33760,33761}---//ÐÌÌìÅ­À×_Ì«ê»
  	debug_TalismanSkillIDs[33795]={33795,33796,33797}---//éäÂäÁ÷»ª_Ì«ê»
  	debug_TalismanSkillIDs[33831]={33831,33832,33833}---//×ÓÎçÐÄ»ê_Ì«ê»
  	debug_TalismanSkillIDs[33867]={33867,33868,33869}---//ºÆÆøÔÆÌì_Ì«ê»
		debug_TalismanSkillIDs[33762]={33762,33763,33764}---//³½»ÊÒ»¼¶_³½»Ê
		debug_TalismanSkillIDs[33798]={33798,33799,33800}---//³½»Ê¶þ¼¶_³½»Ê
		debug_TalismanSkillIDs[33834]={33834,33835,33836}---//³½»ÊÈý¼¶_³½»Ê
		debug_TalismanSkillIDs[33870]={33870,33871,33872}---//³½»ÊËÄ¼¶_³½»Ê

		debug_TalismanSkillIDs[34803]={34803,34804,34805}---//Áé×åÐÂÊÖ_Áé×å

  	debug_TalismanSkillIDs[34807]={34807,34808,34809}---//Ç£»úÒ»¼¶_Ç£»ú
  	debug_TalismanSkillIDs[34811]={34811,34812,34813}---//Ç£»ú¶þ¼¶_Ç£»ú
  	debug_TalismanSkillIDs[34815]={34815,34816,34817}---//Ç£»úÈý¼¶_Ç£»ú
  	debug_TalismanSkillIDs[34819]={34819,34820,34821}---//Ç£»úËÄ¼¶_Ç£»ú
		debug_TalismanSkillIDs[34823]={34823,34824,34825}---//Ó¢ÕÐÒ»¼¶_Ó¢ÕÐ
		debug_TalismanSkillIDs[34827]={34827,34828,34829}---//Ó¢ÕÐ¶þ¼¶_Ó¢ÕÐ
		debug_TalismanSkillIDs[34831]={34831,34832,34833}---//Ó¢ÕÐÈý¼¶_Ó¢ÕÐ
		debug_TalismanSkillIDs[34835]={34835,34836,34837}---//Ó¢ÕÐËÄ¼¶_Ó¢ÕÐ

	local skill_addon_id = output:QueryData(0,dataIndex["skill_addon_id"])
	local skill_addon_id_new
	for _,v in pairs(debug_TalismanSkillIDs) do
		if v[1] == skill_addon_id or v[2] == skill_addon_id then
			skill_addon_id = v[3]
		end
	end
	output:UpdateData(0,dataIndex["skill_addon_id"],skill_addon_id)
	output:UpdateData(0,dataIndex["skillrefine_level"],3)

	ZLuaTalRebuild(output,id1)
end


---ÓÃÓÚ°Ñ·¨±¦¾ßÌå³É³¤ÂÊ·­ÒëÎªµµÎ»Êý£¬ÓëGetGradeFromValueËù²»Í¬µÄÊÇ£¬±¾º¯Êý»á°Ñ¸ºµÄ³É³¤ÂÊ·­ÒëÎª¸ºµÄµµÎ»Êý£¬¶ø²»ÊÇ1
function ZLuaTalDebugGetGradeFromValue(index, talisman)
	local nGrades, valueGrades = ZLuaTalGrades()
	local delta = ( valueGrades[index][50] - valueGrades[index][1] ) / 49
	local v0 = valueGrades[index][1] - delta
	if talisman:QueryData(0,index) < v0 + ( -99 * delta ) then
		return -100
	end
	if talisman:QueryData(0,index) >= v0 + ( 100 * delta ) then
		return 100
	end
	for i = -100, 100 do
		if talisman:QueryData(0,index) < v0 + ( i * delta ) then
			return i
		end
	end
	return -200
end

----°ÑÏòÐÔ¶ÔÓ¦µÄIndexÖµ×ª»¯ÎªDataIndexÖµ
function ZluaTransRiseIndex(growdirection)
	if growdirection==18 then return 83 end
	if growdirection==19 then return 84 end
	if growdirection==21 then return 85 end
	if growdirection==22 then return 86 end
	if growdirection==25 then return 87 end
	if growdirection==26 then return 91 end
	if growdirection==27 then return 90 end
	if growdirection==28 then return 88 end
	if growdirection==29 then return 89	end
	return 0
end

-- ×¢²áËùÓÐ·¨±¦ºÍ·É±¦(·É½£¡¢Ê¥ÁéºÍÓðÒí)
for _,tal in pairs( ZLuaTalAvailableTalismans() ) do
	AddTalismanItem(tal.tmplID, ZLuaTalInit,ZLuaTalLevelup,ZLuaTalReset)
end
