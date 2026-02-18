---//所有涉及函数描述--------------------------------------------------------------
---//数据参数相关定义
---//ZLuaTalisman_CombineAppendix() 血炼用灵媒参数
---//ZLuaTalismanEnchantEx()				天人合一属性灌魔灵媒参数
---//ZLuaTalismanEnchantSkillCid()	技能灌魔灵媒参数
---//ZLuaTalismanEnchantSkillEx()		技能灌魔中技能与模板信息参数
---//ZLuaTalismanSkillsExInfo(skillid,nowaddtimes) 技能的可叠加概率参数

---//法宝血炼相关函数
---//ZLuaTalisman_Combine(id1,id2,talisman1,talisman2,cid,output)	法宝血炼主函数
---//*法宝1的id,法宝2的id,法宝1对象,法宝2对象,辅助灵媒,输出法宝*
---//ZLuaTalisman_CombineRechooseSkill(id1,id2,talisman1,talisman2,output) 法宝血炼中交换法宝附加人物技能
---//*法宝1的id,法宝2的id,法宝1对象,法宝2对象,输出法宝*
---//ZLuaTalisman_BurnMatrix(a_id,b_id,aqlevel,bqlevel,atime,btime,atime_ex,btime_ex,cid) 确定血炼公式的采用原则
---//*法宝1的id,法宝2的id,法宝1需求级别,法宝2需求级别,法宝1生成主时间,法宝2生成主时间,法宝1生成次时间,法宝2生成次时间,辅助灵媒*
---//ZLuaTalisman_GrowMatrix(ischangespeciality,thetypewesee,mothertalisman,fathertalisman,baseid,foodid,combineindex,cid) 血炼造成的档位成长量
---//*是否交换主成长类型,可见的成长类型,母本法宝(吃),父本法宝(被吃),作为食物的法宝id,血炼值档位参数,辅助灵媒*
---//ZLuaSpecialTalismanOut(cid,output,origin) 法宝血炼特殊生成物函数
---//*血炼产出,生成物源头标志(仅当产物为法宝时)*
---//ZLuaTalisman_GrowChanceDeal(thetype,mothertalisman,PriceBase,PriceAdd,combineindex)
---//*成长类型,母本法宝(吃),母本法宝价值,父本法宝价值(食物价值),血炼值档位参数*
---//ZLuaTalisman_CombineValueGrow(alevel,blevel,aqlevel,bqlevel,combineindex,multi)血炼增加熔炼值计算
---//*法宝1级别,法宝2级别,法宝1需求级别,法宝2需求级别,血炼值档位参数,加乘系数*
---//ZLuaTalisman_IsIgnoreChange(combineindex) 血炼值是否格档品质下降
---//*血炼值档位参数*
---//ZLuaTalAttributeValue(talisman,growDir18) 评估法宝的价值
---//*法宝对象,评估方向(成长方向)*
---//ZLuaTalisman_FoodAttributeValue(foodtalisman,foodid,nowmothertalismanlimit) 评估法宝食物的价值
---//*法宝对象,法宝对象id,当前主法宝的档位*
---//ZLuaTalisman_TimeMatrix(atime,btime,atime_ex,btime_ex,cid)法宝血炼时辰与生成物的关系处理函数
---//*法宝1生成主时间,法宝2生成主时间,法宝1生成次时间,法宝2生成次时间,辅助灵媒*


---//法宝灌魔相关函数
---//EnchantTalismans(id,talisman1,cid,output)	法宝灌魔主函数
---//*法宝1的id,法宝1对象,辅助灵媒,输出法宝*
---//ZLuaTalisman_EnchantBotEx(id,talisman1,cid,output,exid,bufftype) 天人合一灌魔主函数
---//*法宝1的id,法宝1对象,辅助灵媒,输出法宝,天人合一附加属性id,天人合一附加属性type*
---//ZLuaTalChangeExt(talisman) 法宝升级对天人合一类型灌魔的处理
---//*法宝对象*-
---//ZLuaExistInTalismanEnchantEx(cid,level) 查找与当前法宝匹配的天人合一灵媒
---//*辅助灵媒,法宝级别*
---//ZLuaExistInTalismanEnchantSkillCid(cid) 查找技能灌魔灵媒对应信息
---//*辅助灵媒*
---//ZLuaTalisman_EnchantSkill(id, talisman1,cid,output,prof,enchantskilltype,enchantskillid,maxsameskillcol) 附加技能灌魔
---//*法宝1的id,法宝1对象,辅助灵媒,输出法宝,法宝的职业归属信息,技能灌魔类型,技能灌魔技能id,最多允许的同种技能数量*
---//ZLuaTalisman_ChooseEnchantSkill(prof,num,maxsamecol)	随机法宝的人物技能(根据需求特定选取)
---//*法宝从属职业,选取技能数量,最多允许的同种技能数量*
---//*ZLuaTalisman_ChooseTalismanSkill(TalismanSkillID) 变更法宝携带的法宝技能
---//*法宝技能ID*

---//法宝飞升相关函数
---//ZLuaTalHolyLevelUp(id,talisman,cid,output)	法宝飞升主函数
---//*法宝的id,法宝对象,飞升灵媒,输出法宝*

---//法宝技能洗炼相关函数
---//ZLuaTalSkillRefine(id1,id2,talisman1,talisman2,cid,output)	法宝技能洗炼主函数
---//*法宝1的id,法宝2的id,法宝1对象,法宝2对象,洗炼灵媒,输出法宝*

---//法宝技能镶嵌相关函数
---//ZLuaTalSkillAdd(id1,id2,talisman1,talisman2,cid1,cid2,output)	法宝技能镶嵌主函数
---//*法宝1的id,法宝2的id,法宝1对象,法宝2对象,灵媒1,灵媒2,输出法宝*

---//ZluaTalSkillRefineResult(id1, talisman1, result, ouput);

---//参量获得辅助函数
---//ZLuaTalisman_CombineValueDeal(combinevalue) 获得人物血炼值档位参数
---//*当前血炼值*
---//ZLuaTalisman_GetProfInfo(id,SkillNoSeq)	获取法宝职业归属信息
---//*法宝id,法宝的附加人物技能序列*

---//算法辅助函数
---//ZLuaCheckOutNums(ilist,vlist,klist,num,maxsamecol)
---//*从ilist(数组)中按照vlist(数组)规定的权重选取num个数字,且一一对应klist中的数字,每种数字最多出现maxsamcol次*
---//ZLuaTalisman_InsertSkillSeq(insertid,ilist,vlist,talismanskillprof)
---//*将insertid插入ilist中,vlist为ilist一一对应序列,talismanskillprof法宝职业

-----------------------------------------------------------------------------------
-----------------------------------------------------------------------------------
---//法宝熔炼剂静态数据区
function ZLuaTalisman_CombineAppendix()
	local Temp_TalismanIDs=ZLuaTalAvailableTalismans()
	local cidmatrix={}
	---//cidusetype: 	1:普通熔炼剂;2:特殊产出熔炼剂;3:混合熔炼剂;4:天人合一灌魔剂;5:法宝附加人物技能切换灵媒
	---//cidmulti: 		特殊产出熔炼剂专用属性，确认放大倍数
	---//cidwithlvl:	混合熔炼剂专用属性；对于主件级别生效(0:通用/1:1级/2:15级/3:45级/4:75级)
	---//cidcombo:		混合熔炼剂专用属性，提高主件被混合的可能性
	
	cidmatrix[6338]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["QY_15_LiuHeJing"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[6336]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["GW_15_ZhuQueYin"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[6339]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["TY_15_LunHuiZhu"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[6337]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["HH_15_SheXinLing"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[17855]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["GD_15_XueYuGuPian"].tmplID,cidmulti=1,cidcombo=0}	

	cidmatrix[6342]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["QY_75_QiXingJian"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[6340]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["GW_75_SiLingFan"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[6343]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["TY_75_JinGangChu"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[6341]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["HH_75_DuoQingHuan"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[17856]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["GD_75_PoSha"].tmplID,cidmulti=1,cidcombo=0}	
	cidmatrix[6470]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["US_75_YuJingPing"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[6471]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["US_45_ShenMuShai"].tmplID,cidloveid2=Temp_TalismanIDs["US_45_ShenMuShai_WZGL"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[6472]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["US_75_WanDuDing"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[6344]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["US_75_ShanHeShan"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[13359]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["US_75_LingLongTa"].tmplID,cidmulti=1,cidcombo=0}

	cidmatrix[7920]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=0}
	
	cidmatrix[10914]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["QY_105_HuPoZhuLing"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[10917]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["US_105_JinHuLu"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[10918]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["HH_105_QianMeiLian"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[10919]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["GW_105_QianKunQingGuangJie"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[10921]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["US_105_JiuLongShenHuoZhao"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[10922]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["TY_105_RuYiQianKunDai"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[17858]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["GD_105_QianDuGu"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[10915]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["US_105_YinYangJing"].tmplID,cidloveid2=Temp_TalismanIDs["US_105_YinYangJing_WZGL"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[15867]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["US_105_LuoHunDeng"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[17833]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["US_105_ZhenShanHeShan"].tmplID,cidmulti=1,cidcombo=0}
	
	cidmatrix[10916]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["US_135_PanLongFan"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[10920]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["US_135_WuLongLun"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[20094]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["US_135_FanTianYin"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[20501]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["US_135_ChangMingDeng"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[34852]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["US_135_BaoKuZhiHun"].tmplID,cidmulti=1,cidcombo=0}

	cidmatrix[10923]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["US_135_SheHun"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[10924]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["US_145_ShiXueZhu"].tmplID,cidmulti=1,cidcombo=0}	
	cidmatrix[10925]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["US_145_ShiHun"].tmplID,cidmulti=1,cidcombo=0}		
	cidmatrix[43480]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["US_145_TianYa"].tmplID,cidmulti=1,cidcombo=0}		

	cidmatrix[22128]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["US_75_RiYueBaoJian"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[23356]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["US_105_FuTu"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[25739]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["US_135_YinHunFan"].tmplID,cidmulti=1,cidcombo=0}

	cidmatrix[13798]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["GW_135_TianMoZhi"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[13797]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["QY_135_RuoXue"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[13795]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["HH_135_BaJiYouLong"].tmplID,cidmulti=1,cidcombo=0}	
	cidmatrix[13796]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["TY_135_LiuDaoShengMie"].tmplID,cidmulti=1,cidcombo=0}
	cidmatrix[17859]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["GD_135_GuiQing"].tmplID,cidmulti=1,cidcombo=0}				

	cidmatrix[31504]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["SY_1_WuSeShi"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[31505]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["JL_15_NuShouFan"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[31506]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["JL_75_XueFengQiang"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[31507]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["JL_105_GangLan"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[31508]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["JL_135_SangMenJian"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[31509]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["LS_15_DanZhu"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[31510]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["LS_75_FanHunXiang"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[31511]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["LS_105_FuYingGuJing"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[31512]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["LS_135_LuoShu"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[38891]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["HG_15_QiSha"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[38892]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["HG_75_PoJun"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[38893]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["HG_105_TanLang"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[38894]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["HG_135_JueYing"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[38895]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["TH_15_JiuYinLuo"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[38896]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["TH_75_YaFeng"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[38897]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["TH_105_LiuLiZhan"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[38898]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["TH_135_BiLuoFeiHong"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[49372]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["FX_15_NanMingYin"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[49373]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["FX_75_HengTianChi"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[49374]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["FX_105_HuoLingLong"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[49375]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["FX_135_JiuHanNingBingCi"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[49376]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["TH_15_LeiGongMo"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[49377]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["TH_75_ChunQiuSuo"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[49378]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["TH_105_ZiMuYinYangKou"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[49379]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["TH_135_XiHuangZhong"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[54047]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["CH_15_lvdouqi"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[54048]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["CH_75_tuogu"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[54049]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["CH_105_luobaojinqian"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[54050]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["CH_135_huangji"].tmplID,cidmulti=1,cidcombo=0}				

	cidmatrix[58841]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["LZ_1_TianLingShi"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[58842]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["QJ_15_tianjimu"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[58843]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["QJ_75_luohouyu"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[58844]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["QJ_105_jinyubian"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[58845]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["QJ_135_chixinglongyan"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[58846]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["YZ_15_lveying"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[58847]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["YZ_75_poming"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[58848]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["YZ_105_shuolei"].tmplID,cidmulti=1,cidcombo=0}				
	cidmatrix[58849]={cidwithlvl=0,cidusetype=1,cidloveid=Temp_TalismanIDs["YZ_135_zhuihun"].tmplID,cidmulti=1,cidcombo=0}				

	cidmatrix[6345]={cidwithlvl=0,cidusetype=2,cidloveid=Temp_TalismanIDs["US_75_ShanHeShan"].tmplID,cidmulti=20,cidcombo=0}
	cidmatrix[6346]={cidwithlvl=0,cidusetype=2,cidloveid=Temp_TalismanIDs["US_75_ShanHeShan"].tmplID,cidmulti=100,cidcombo=0}
	cidmatrix[27150]={cidwithlvl=0,cidusetype=2,cidloveid=Temp_TalismanIDs["US_145_ShiHun"].tmplID,cidmulti=100,cidcombo=0}
	cidmatrix[38204]={cidwithlvl=0,cidusetype=2,cidloveid=Temp_TalismanIDs["US_145_ShiHun"].tmplID,cidmulti=100,cidcombo=0}
		
	cidmatrix[7921]={cidwithlvl=1,cidusetype=3,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=0.1}
	cidmatrix[7922]={cidwithlvl=15,cidusetype=3,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=0.1}
	cidmatrix[7923]={cidwithlvl=45,cidusetype=3,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=0.1}
	cidmatrix[7924]={cidwithlvl=75,cidusetype=3,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=0.1}
	
	cidmatrix[7926]={cidwithlvl=1,cidusetype=3,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=0.55}
	cidmatrix[7929]={cidwithlvl=15,cidusetype=3,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=0.55}
	cidmatrix[7933]={cidwithlvl=45,cidusetype=3,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=0.55}
	cidmatrix[7936]={cidwithlvl=75,cidusetype=3,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=0.55}
	
	cidmatrix[7927]={cidwithlvl=1,cidusetype=3,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=3.33}
	cidmatrix[7930]={cidwithlvl=15,cidusetype=3,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=3.33}
	cidmatrix[7934]={cidwithlvl=45,cidusetype=3,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=3.33}
	cidmatrix[7938]={cidwithlvl=75,cidusetype=3,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=3.33}
	
	cidmatrix[7928]={cidwithlvl=1,cidusetype=3,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=999}
	cidmatrix[7932]={cidwithlvl=15,cidusetype=3,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=999}
	cidmatrix[7935]={cidwithlvl=45,cidusetype=3,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=999}
	cidmatrix[7939]={cidwithlvl=75,cidusetype=3,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=999}	

	cidmatrix[11385]={cidwithlvl=1,cidusetype=4,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=999}
	cidmatrix[11386]={cidwithlvl=1,cidusetype=4,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=999}
	cidmatrix[11387]={cidwithlvl=1,cidusetype=4,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=999}
	cidmatrix[11388]={cidwithlvl=1,cidusetype=4,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=999}	
	cidmatrix[11389]={cidwithlvl=1,cidusetype=4,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=999}
	cidmatrix[11390]={cidwithlvl=1,cidusetype=4,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=999}	
	
	cidmatrix[11735]={cidwithlvl=1,cidusetype=5,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=999}
	---法宝变飞剑
	cidmatrix[60820]={cidwithlvl=1,cidusetype=100,cidloveid=Temp_TalismanIDs["NW_1_BaGuaShi"].tmplID,cidmulti=1,cidcombo=999}

	cidmatrix[61205]={cidwithlvl=1,cidusetype=100,cidloveid=Temp_TalismanIDs["US_145_ShiHun"].tmplID,cidloveid2=Temp_TalismanIDs["shihunfeijian"].tmplID,cidmulti=1,cidcombo=999}
	cidmatrix[61206]={cidwithlvl=1,cidusetype=100,cidloveid=Temp_TalismanIDs["US_145_TianYa"].tmplID,cidloveid2=Temp_TalismanIDs["tianyafeijian"].tmplID,cidmulti=1,cidcombo=999}
	cidmatrix[61207]={cidwithlvl=1,cidusetype=100,cidloveid=Temp_TalismanIDs["US_135_SheHun"].tmplID,cidloveid2=Temp_TalismanIDs["shehunfeijian"].tmplID,cidmulti=1,cidcombo=999}
	cidmatrix[61208]={cidwithlvl=1,cidusetype=100,cidloveid=Temp_TalismanIDs["US_145_ShiXueZhu"].tmplID,cidloveid2=Temp_TalismanIDs["sihxuefeijian"].tmplID,cidmulti=1,cidcombo=999}
	cidmatrix[61209]={cidwithlvl=1,cidusetype=100,cidloveid=Temp_TalismanIDs["US_45_ShenMuShai"].tmplID,cidloveid2=Temp_TalismanIDs["shenmufeijian"].tmplID,cidmulti=1,cidcombo=999}
	
	cidmatrix[0]={cidwithlvl=-1,cidusetype=-1,cidloveid=0,cidmulti=1,cidcombo=0}
	return cidmatrix
end

---//法宝血炼特殊生成物的处理函数
function ZLuaSpecialTalismanOut(cid,output,origin)
	local temp_cid=ZLuaTalisman_CombineAppendix()
	local Temp_TalismanIDs=ZLuaTalAvailableTalismans()
	local outid
	
	-- 如果是山河扇合成灵媒
	if temp_cid[cid].cidloveid == Temp_TalismanIDs["US_75_ShanHeShan"].tmplID then
		local SpecialOut={}
		SpecialOut[4712]=50---//山河扇
		SpecialOut[4006]=100---//补天符
		SpecialOut[5527]=200---//梦幻重剑
		SpecialOut[5528]=100---//碎竹之刀 
		SpecialOut[5529]=100---//今晚打老虎
		SpecialOut[5525]=100---//晨风之心 
		SpecialOut[5526]=100---//月芒之心 
		SpecialOut[5536]=10
		SpecialOut[5537]=10
		SpecialOut[5538]=10
		SpecialOut[5539]=10---//点金术残页A/B/C/D
		SpecialOut[5540]=100---//三碗不过岗
		SpecialOut[5541]=100---//飘香竹筒饭
		SpecialOut[7792]=100---//绝代双骄
		SpecialOut[6428]=150---//自然的祝福
		SpecialOut[6451]=160---//传世一戒
		SpecialOut[6452]=100---//传世二戒
		SpecialOut[6453]=100---//冰咒指环
		SpecialOut[6455]=100---//碎水指环
		SpecialOut[6456]=100---//狂魔指环
		SpecialOut[6454]=100---//降服指环
		SpecialOut[6449]=100---//久久久指环
		SpecialOut[6450]=100---//我就是爱指环
		local allspecialout={}
		local allspecialchance={}
		for i,v in pairs(SpecialOut) do 
			table.insert(allspecialout,i)
			table.insert(allspecialchance,v)
		end 
		outid=allspecialout[ZLuaTal_Get_RanPoint(allspecialchance)]
	-- 如果是嗜血珠合成灵媒
	elseif temp_cid[cid].cidloveid == Temp_TalismanIDs["US_145_ShiHun"].tmplID then
		outid=Temp_TalismanIDs["US_145_ShiHun"].tmplID
	end

	if ZLuaTalExistInTmplIDs(outid) then
		output:CreateItem(outid)			
		ZLuaTalInit(outid,output,origin)	
	else 
		output:CreateItem(outid)
	end 
	outid=nil
end 

---//法宝血炼时辰与生成物的关系处理函数
function ZLuaTalisman_TimeMatrix(atime,btime,atime_ex,btime_ex,cid)
	if atime==btime and atime_ex==btime_ex then 
		return 1
	else 
		return 1
	end 		
end 
---//人物熔炼值与附加概率的关系处理函数[血炼]
function ZLuaTalisman_CombineValueDeal(combinevalue)
	if type(combinevalue)~="number" then return end 
	local combineindex
	if combinevalue>=0 and combinevalue<=10 then 
			combineindex=1  
	elseif combinevalue>10 and combinevalue<=60 then 
			combineindex=2 
	elseif combinevalue>60 and combinevalue<=180 then 
			combineindex=3
	elseif combinevalue>180 and combinevalue<=720 then 
			combineindex=4
	elseif combinevalue>720 and combinevalue<=1800 then 
			combineindex=5
	elseif combinevalue>1800 and combinevalue<=3200 then 
			combineindex=6
	elseif combinevalue>3200 and combinevalue<=5600 then 
			combineindex=7
	elseif combinevalue>5600 and combinevalue<=9999 then 
			combineindex=8
	elseif combinevalue>9999 and combinevalue<=23456 then 
			combineindex=9
	elseif combinevalue>23456 then 
			combineindex=10	
	else
			combineindex=1	
	end 	
	return combineindex
end 
---//处理法宝所有参数与增加熔炼值的关系[血炼]---------------------------------------------------------------
function ZLuaTalisman_CombineValueGrow(alevel,blevel,aqlevel,bqlevel,combineindex,multi)
	if type(alevel)~="number" or type(blevel)~="number" then return end
	if type(aqlevel)~="number" or type(bqlevel)~="number" then return end
	
	local c1,c2,c3,CombineValueGrowth
		c1=2
		c2=(aqlevel+bqlevel)/16+0.004*aqlevel*bqlevel
		c3=0
		CombineValueGrowth=math.floor(multi*(c1+c2+c3)/combineindex)
		if CombineValueGrowth>=25 then CombineValueGrowth=25 end
		if CombineValueGrowth<=0 then CombineValueGrowth=0 end 
		return CombineValueGrowth
end 
---//取得 法宝主体的价值[血炼] 
function ZLuaTalAttributeValue(talisman,growDir18)
	local grade = ZLuaTalGetGradeFromValue(growDir18,talisman) or 1
	local price = 1
	if grade <= 5 then 
		price = 0.1
	elseif grade <= 10 then
		price = 0.5
	elseif grade <= 15 then
		price = 0.8
	elseif grade <= 17 then
		price = 1
	elseif grade>17 and grade<=50 then
		price = math.max(1, 0.019592*grade^3 - 1.0611*grade^2 + 19.79*grade - 125 )
	elseif grade>50 and grade<80 then
		price = math.max(1, 40*grade-1200)
	else
			price=1
	end
	return price
end
---//处理 法宝食物的价值[血炼]-------------------------------------------------------------------------------
function ZLuaTalisman_FoodAttributeValue(foodtalisman,foodid,nowmothertalismanlimit)
	local TalismanQlevelPrice
	local reqlvl=ZLuaTalExistInTmplIDs(foodid)
	---//根据本体法宝档位决定食物价值
	if nowmothertalismanlimit>50 then 
		if reqlvl==1 then 
				TalismanQlevelPrice={1,2,8}
		elseif reqlvl==15 then 
				TalismanQlevelPrice={1,3,12}
		else
				TalismanQlevelPrice={1,4,20}
		end 
	else
		if reqlvl==1 then 
				TalismanQlevelPrice={1,20,80}
		elseif reqlvl==15 then 
				TalismanQlevelPrice={1,30,120}
		else
				TalismanQlevelPrice={1,40,200}
		end 
	end 
	
	local RmbVsGold=1/10
	local A=ZLuaTalAvailableTalismans()
	local BuyPrice			
	---//买入价
	for i,v in pairs(A) do 
		if v.tmplID==foodid then 
			BuyPrice=v.price
		end 
	end 
	
	local dataIndex=ZLuaTalDataIndex()
	local QlevelPrice=TalismanQlevelPrice[foodtalisman:QueryData(0,dataIndex["quality"])+1]		
	---//品质价
	local zorigin=ZLuaTalClientOrigins()
	if foodtalisman:QueryData(0,dataIndex["origin"])==zorigin["quest"] then 
		QlevelPrice=1
	end 
	
	local UpLevelPrice=ZLuaTalFeedCost(foodtalisman:QueryLevel())
	---//聚气价
	local TalismanFoodPrice=BuyPrice*QlevelPrice*RmbVsGold+UpLevelPrice
	local TalismanFeedLevelPrice=UpLevelPrice
	
	return TalismanFoodPrice,TalismanFeedLevelPrice
end 
---//处理生成谁以及成长方向的问题[血炼]
function ZLuaTalisman_BurnMatrix(a_id,b_id,aqlevel,bqlevel,atime,btime,atime_ex,btime_ex,cid)
---//A法宝ID,B法宝ID,A的需求级别,B的需求级别,A的生辰时间1,B的生成时间1,A的生辰时间2,B的生辰时间2,熔炼剂
	local tempcid=ZLuaTalisman_CombineAppendix()	
	local Temp_TalismanIDs=ZLuaTalAvailableTalismans()
	local timemulti,amulti,bmulti,cmulti,dmulti,outtype
	---//时间影响因子，对A.B.C.D四种产出加成，
	---//输出的outtype(1,2,3,4,5分别代表：
	---//1:产出为A法宝 A成长型
	---//2:法宝为B法宝 B成长型
	---//3:法宝为特殊法宝 或特殊物品	
	---//4:法宝为A法宝 B成长型
	---//5:法宝为B法宝 A成长型
	local outtypelist={}
	timemulti=ZLuaTalisman_TimeMatrix(atime,btime,atime_ex,btime_ex,cid)
	
	---//无灵媒
	if cid==0 then
		if aqlevel>bqlevel then 
			---//时辰的影响 目前未设定
			amulti=timemulti
			if bqlevel==1 then amulti=aqlevel end 
			bmulti=1
		elseif aqlevel==bqlevel then 
			amulti=1
			bmulti=1
		else 
			amulti=1
			bmulti=timemulti
		end 			
			outtypelist[1]={600,400,(aqlevel+bqlevel)/150}
			outtype=ZLuaTal_Get_RanPoint(outtypelist[1])
	---//有灵媒
	else
		if tempcid[cid].cidusetype==1  then
			if tempcid[cid].cidloveid==a_id or tempcid[cid].cidloveid2==a_id then
				outtype=1
			end 		
			if outtype<=0 or outtype>2 then outtype=1 end
		elseif tempcid[cid].cidusetype==2 and tempcid[cid].cidloveid==Temp_TalismanIDs["US_75_ShanHeShan"].tmplID then
			outtypelist[2]={600,400,(aqlevel+bqlevel)*tempcid[cid].cidmulti/150}
			outtype=ZLuaTal_Get_RanPoint(outtypelist[2])
			if outtype<=0 or outtype>3 then outtype=1 end 
		elseif tempcid[cid].cidusetype==2 and tempcid[cid].cidloveid==Temp_TalismanIDs["US_145_ShiHun"].tmplID then
			outtype=3
		elseif tempcid[cid].cidusetype==3 then 
			outtypelist[3]={10,0,0,tempcid[cid].cidcombo,0}
			outtype=ZLuaTal_Get_RanPoint(outtypelist[3])
			if tempcid[cid].cidcombo>=900 then outtype=4 end 
			if outtype==2 or outtype==3 or outtype>4 or outtype<=0 then outtype=1 end  
		end 
	end 
	return outtype
end 
---//根据熔炼值决定是否抵挡了降档[血炼]
function ZLuaTalisman_IsIgnoreChange(combineindex)
	if type(combineindex)~="number" or combineindex<0 or combineindex>10 then combineindex=0 end 
	local ingorechance={2*combineindex,100-2*combineindex}
	local c=0
	if ZLuaTal_Get_RanPoint(ingorechance)==1 then
		c=1
	end 	 
		return c
end 
---//根据价格比决定档位成长[血炼]
function ZLuaTalisman_GrowChanceDeal(thetype,mothertalisman,PriceBase,PriceAdd,combineindex)
		local GrowDirection={-2,-1,0,1,2,3,4}
		local HowManyGrow
		local GrowChance1={}
			GrowChance1[1]={0.01,0.1,0.84,0.05,0,0,0}
			GrowChance1[2]={0,0.01,0.85,0.14,0,0,0}
			GrowChance1[3]={0,0,0.819,0.18,0.001,0,0}
			GrowChance1[4]={0,0,0.78,0.22,0,0,0}
			GrowChance1[5]={0,0,0.74,0.26,0,0,0}
			GrowChance1[6]={0,0,0.7,0.3,0,0,0}
			GrowChance1[7]={0,0,0.51,0.49,0,0,0}
			GrowChance1[8]={0,0,0.46,0.54,0,0,0}
			GrowChance1[9]={0,0,0.41,0.59,0,0,0}
			GrowChance1[10]={0,0,0.33,0.66,0.01,0,0}
			GrowChance1[11]={0,0,0.325,0.66,0.015,0,0}
			GrowChance1[12]={0,0,0.32,0.66,0.02,0,0}
			GrowChance1[13]={0,0,0.315,0.66,0.025,0,0}
			GrowChance1[14]={0,0,0.31,0.66,0.03,0,0}
			GrowChance1[15]={0,0,0.304,0.66,0.035,0.001,0}
			GrowChance1[16]={0,0,0.296,0.66,0.04,0.004,0}
			GrowChance1[17]={0,0,0.288,0.66,0.045,0.007,0}
			GrowChance1[18]={0,0,0.28,0.66,0.05,0.01,0}
			GrowChance1[19]={0,0,0.272,0.66,0.055,0.013,0}
			GrowChance1[20]={0,0,0.173,0.75,0.06,0.016,0.001}
			GrowChance1[21]={0,0,0.162,0.75,0.065,0.019,0.004}
			GrowChance1[22]={0,0,0.151,0.75,0.07,0.022,0.007}
			GrowChance1[23]={0,0,0.14,0.75,0.075,0.025,0.01}
			GrowChance1[24]={0,0,0.129,0.75,0.08,0.028,0.013}
			GrowChance1[25]={0,0,0.118,0.75,0.085,0.031,0.016}
			GrowChance1[26]={0,0,0.107,0.75,0.09,0.034,0.019}
			GrowChance1[27]={0,0,0.096,0.75,0.095,0.037,0.022}
			GrowChance1[28]={0,0,0.085,0.75,0.1,0.04,0.025}
			GrowChance1[29]={0,0,0.074,0.75,0.105,0.043,0.028}
			GrowChance1[30]={0,0,0,0.813,0.11,0.046,0.031}
			GrowChance1[31]={0,0,0,0.802,0.115,0.049,0.034}
			GrowChance1[32]={0,0,0,0.791,0.12,0.052,0.037}
			GrowChance1[33]={0,0,0,0.78,0.125,0.055,0.04}
			GrowChance1[34]={0,0,0,0.769,0.13,0.058,0.043}
			GrowChance1[35]={0,0,0,0.758,0.135,0.061,0.046}
			GrowChance1[36]={0,0,0,0.747,0.14,0.064,0.049}
			GrowChance1[37]={0,0,0,0.736,0.145,0.067,0.052}
			GrowChance1[38]={0,0,0,0.725,0.15,0.07,0.055}
			GrowChance1[39]={0,0,0,0.714,0.155,0.073,0.058}
			GrowChance1[40]={0,0,0,0.703,0.16,0.076,0.061}
		local GrowChance2={}
			GrowChance2[1]={0,0,0.5,0.499,0.001,0,0}
			GrowChance2[2]={0,0.01,0.45,0.538,0.002,0,0}
			GrowChance2[3]={0,0,0.4,0.597,0.003,0,0}
			GrowChance2[4]={0,0,0.35,0.646,0.004,0,0}
			GrowChance2[5]={0,0,0.3,0.695,0.005,0,0}
			GrowChance2[6]={0,0,0.25,0.744,0.006,0,0}
			GrowChance2[7]={0,0,0.2,0.793,0.007,0,0}
			GrowChance2[8]={0,0,0.15,0.842,0.008,0,0}
			GrowChance2[9]={0,0,0.1,0.891,0.009,0,0}
			GrowChance2[10]={0,0,0.05,0.939,0.01,0.001,0}
			GrowChance2[11]={0,0,0.05,0.937,0.011,0.002,0}
			GrowChance2[12]={0,0,0.05,0.935,0.012,0.003,0}
			GrowChance2[13]={0,0,0.05,0.933,0.013,0.004,0}
			GrowChance2[14]={0,0,0.05,0.931,0.014,0.005,0}
			GrowChance2[15]={0,0,0.05,0.929,0.015,0.006,0}
			GrowChance2[16]={0,0,0.05,0.927,0.016,0.007,0}
			GrowChance2[17]={0,0,0.05,0.925,0.017,0.008,0}
			GrowChance2[18]={0,0,0.05,0.923,0.018,0.009,0}
			GrowChance2[19]={0,0,0.05,0.921,0.019,0.01,0}
			GrowChance2[20]={0,0,0.05,0.918,0.02,0.011,0.001}
			GrowChance2[21]={0,0,0,0.965,0.021,0.012,0.002}
			GrowChance2[22]={0,0,0,0.962,0.022,0.013,0.003}
			GrowChance2[23]={0,0,0,0.959,0.023,0.014,0.004}
			GrowChance2[24]={0,0,0,0.956,0.024,0.015,0.005}
			GrowChance2[25]={0,0,0,0.953,0.025,0.016,0.006}
			GrowChance2[26]={0,0,0,0.95,0.026,0.017,0.007}
			GrowChance2[27]={0,0,0,0.947,0.027,0.018,0.008}
			GrowChance2[28]={0,0,0,0.944,0.028,0.019,0.009}
			GrowChance2[29]={0,0,0,0.941,0.029,0.02,0.01}
			GrowChance2[30]={0,0,0,0.938,0.03,0.021,0.011}
			GrowChance2[31]={0,0,0,0.935,0.031,0.022,0.012}
			GrowChance2[32]={0,0,0,0.932,0.032,0.023,0.013}
			GrowChance2[33]={0,0,0,0.929,0.033,0.024,0.014}
			GrowChance2[34]={0,0,0,0.926,0.034,0.025,0.015}
			GrowChance2[35]={0,0,0,0.923,0.035,0.026,0.016}
			GrowChance2[36]={0,0,0,0.92,0.036,0.027,0.017}
			GrowChance2[37]={0,0,0,0.917,0.037,0.028,0.018}
			GrowChance2[38]={0,0,0,0.914,0.038,0.029,0.019}
			GrowChance2[39]={0,0,0,0.911,0.039,0.03,0.02}
			GrowChance2[40]={0,0,0,0.908,0.04,0.031,0.021}
		local GrowChance3={}
			GrowChance3[1]={0.05,0.12,0.75,0.03,0,0,0}
			GrowChance3[2]={0,0.1,0.7,0.2,0,0,0}
			GrowChance3[3]={0,0.09,0.7,0.21,0,0,0}
			GrowChance3[4]={0,0.08,0.68,0.24,0,0,0}
			GrowChance3[5]={0,0.07,0.66,0.27,0,0,0}
			GrowChance3[6]={0,0.06,0.64,0.3,0,0,0}
			GrowChance3[7]={0,0.05,0.62,0.33,0,0,0}
			GrowChance3[8]={0,0.04,0.6,0.36,0,0,0}
			GrowChance3[9]={0,0.03,0.58,0.39,0,0,0}
			GrowChance3[10]={0,0.02,0.56,0.42,0.001,0,0}
			GrowChance3[11]={0,0.01,0.540,0.448,0.002,0,0}
			GrowChance3[12]={0,0,0.52,0.477,0.003,0,0}
			GrowChance3[13]={0,0,0.50,0.496,0.004,0,0}
			GrowChance3[14]={0,0,0.48,0.515,0.005,0,0}
			GrowChance3[15]={0,0,0.46,0.533,0.006,0.001,0}
			GrowChance3[16]={0,0,0.44,0.551,0.007,0.002,0}
			GrowChance3[17]={0,0,0.42,0.569,0.008,0.003,0}
			GrowChance3[18]={0,0,0.40,0.587,0.009,0.004,0}
			GrowChance3[19]={0,0,0.38,0.605,0.01,0.005,0}
			GrowChance3[20]={0,0,0.36,0.623,0.011,0.006,0}
			GrowChance3[21]={0,0,0.34,0.641,0.012,0.007,0}
			GrowChance3[22]={0,0,0.32,0.659,0.013,0.008,0}
			GrowChance3[23]={0,0,0.30,0.677,0.014,0.009,0}
			GrowChance3[24]={0,0,0.28,0.6958,0.015,0.01,0}
			GrowChance3[25]={0,0,0.26,0.713,0.016,0.011,0}
			GrowChance3[26]={0,0,0.24,0.731,0.017,0.012,0}
			GrowChance3[27]={0,0,0.22,0.749,0.018,0.013,0}
			GrowChance3[28]={0,0,0.20,0.766,0.019,0.014,0.001}
			GrowChance3[29]={0,0,0.18,0.783,0.02,0.015,0.002}
			GrowChance3[30]={0,0,0.16,0.80,0.021,0.016,0.003}
			GrowChance3[31]={0,0,0.14,0.817,0.022,0.017,0.004}
			GrowChance3[32]={0,0,0.12,0.834,0.023,0.018,0.005}
			GrowChance3[33]={0,0,0.10,0.851,0.024,0.019,0.006}
			GrowChance3[34]={0,0,0.08,0.868,0.025,0.02,0.007}
			GrowChance3[35]={0,0,0.06,0.885,0.026,0.021,0.008}
			GrowChance3[36]={0,0,0.04,0.902,0.027,0.022,0.009}
			GrowChance3[37]={0,0,0.02,0.919,0.028,0.023,0.01}
			GrowChance3[38]={0,0,0,0.936,0.029,0.024,0.011}
			GrowChance3[39]={0,0,0,0.933,0.03,0.025,0.012}
			GrowChance3[40]={0,0,0,0.93,0.031,0.026,0.013}
			
		local GrowChanceIndex=math.floor(10*PriceAdd/PriceBase)
		if GrowChanceIndex<=0 then GrowChanceIndex=1 end 
		if GrowChanceIndex>40 then GrowChanceIndex=40 end 
		if PriceBase<800 then 
			if PriceAdd>=33.33 then 
				HowManyGrow=GrowDirection[ZLuaTal_Get_RanPoint(GrowChance2[GrowChanceIndex])]
			else 
				HowManyGrow=GrowDirection[ZLuaTal_Get_RanPoint(GrowChance1[GrowChanceIndex])]
			end 
		else 
				GrowChanceIndex=math.floor(80*PriceAdd/PriceBase)
				if GrowChanceIndex<=0 then GrowChanceIndex=1 end 
				if GrowChanceIndex>40 then GrowChanceIndex=40 end 
				HowManyGrow=GrowDirection[ZLuaTal_Get_RanPoint(GrowChance3[GrowChanceIndex])]
		end 	
		if ZLuaTalGetGradeFromValue(thetype,mothertalisman)<=24 and HowManyGrow<1 then 
			HowManyGrow=1
		end 
		if ZLuaTalGetGradeFromValue(thetype,mothertalisman)<=17 then 
			HowManyGrow=2
		end 

		if HowManyGrow<0 then 
			if ZLuaTalisman_IsIgnoreChange(combineindex)==1 then HowManyGrow=0 end 
			---//熔炼值格档下降
		end 
		return HowManyGrow
end 

---//根据成长方向决定成长多少的问题[血炼]
function ZLuaTalisman_GrowMatrix(ischangespeciality,thetypewesee,mothertalisman,fathertalisman,baseid,foodid,combineindex,cid)
---//是否是交换类型,我们看到的成长类型,母本法宝,父本法宝,作为食物法宝的id,当前熔炼值参数,熔炼剂
	local T_dataindex=ZLuaTalDataIndex()
	local WhatTypeWeGrow,HowManyGrow
	local RandomDirectionID={T_dataindex["grow_hp"],T_dataindex["grow_mp"],T_dataindex["grow_damage_low"],
														T_dataindex["grow_damage_high"],T_dataindex["grow_resis_stun"],T_dataindex["grow_resis_sleep"],
															T_dataindex["grow_resis_silence"],T_dataindex["grow_resis_weakness"],T_dataindex["grow_resis_enlace"]}
	local RandomDirectionChance={100,200,150,100,100,150,100,100,150}
	
	if thetypewesee==0 or type(thetypewesee)~="number" then
		---//无成长类型则随机选取成长类型
		WhatTypeWeGrow=RandomDirectionID[ZLuaTal_Get_RanPoint(RandomDirectionChance)]
	else 
		---//有则同成长类型相同
 		WhatTypeWeGrow=thetypewesee
	end 		
	
	local temp_cid=ZLuaTalisman_CombineAppendix()
	if ischangespeciality==1 or temp_cid[cid].cidusetype==3 then
	---//处理"交易类型"时的情况
		HowManyGrow=0
	else 
		local TalismanBasePrice,TalismanFoodPrice
	 	---//法宝本体价格以及食物法宝价格
		TalismanBasePrice=ZLuaTalAttributeValue(mothertalisman,WhatTypeWeGrow)
		
		---取得本体法宝的档位信息
		local GradeLimitsList=ZLuaTalGradesUpperLimit(baseid)
--[[
		local n = GradeLimitsList
		if mothertalisman:QueryData(0,dataIndex["reborncont"]) ~= 0 then
			GradeLimitsList = {n[1]+90,n[2]+15,n[3]+15,n[4]+15,n[5]+15,n[6],n[7],n[8]+15,n[9]+15,n[10]+15,n[11]+15,n[12]+15,n[13]}
		end
--]]
		
		local growdirection=ZluaTransRiseIndex(WhatTypeWeGrow)
		local mothertalismanlimit
		if mothertalisman:QueryData(0,growdirection)~=nil then
			mothertalismanlimit=GradeLimitsList[ZLuaTalTransIndex(WhatTypeWeGrow)]+mothertalisman:QueryData(0,growdirection)
		else mothertalismanlimit=GradeLimitsList[ZLuaTalTransIndex(WhatTypeWeGrow)]
		end
		local nowmothertalismanlimit=ZLuaTalGetGradeFromValue(WhatTypeWeGrow,mothertalisman)
		if nowmothertalismanlimit>mothertalismanlimit then nowmothertalismanlimit=mothertalismanlimit end 
				
		TalismanFoodPrice,_=ZLuaTalisman_FoodAttributeValue(fathertalisman,foodid,nowmothertalismanlimit)
		HowManyGrow=ZLuaTalisman_GrowChanceDeal(WhatTypeWeGrow,mothertalisman,TalismanBasePrice,TalismanFoodPrice,combineindex)
	end 	
		return thetypewesee,WhatTypeWeGrow,HowManyGrow
		---//产出的成长类型，成长类型，成长幅度
end

---//法宝熔炼(血炼)主函数
function ZLuaTalisman_Combine(id1,id2,talisman1,talisman2,cid,output,locked1,locked2)

	local dataIndex=ZLuaTalDataIndex()
	local talismans=ZLuaTalAvailableTalismans()
	local temp_cid=ZLuaTalisman_CombineAppendix()

	---// 法宝变飞剑
	
	---如果主法宝ID为飞宝，则血炼失败
	if talisman1:QueryData(0,dataIndex["is_fly"]) == 1 or talisman2:QueryData(0,dataIndex["is_fly"]) == 1 then 
		output:CreateItem(-1)
  ---元宵灯不允许血炼
  elseif id1 == talismans["US_45_YuanXiaoDeng"].tmplID or id2 == talismans["US_45_YuanXiaoDeng"].tmplID then
		output:CreateItem(-1)
	elseif locked1 == true or locked2 == true then
		output:CreateItem(-1)
	
	else
	local iswronginput,isspecialoutput
	local iswronginput=0		---错误输入标志
	local isspecialoutput=0	---特殊产出标志
	
	---//取得法宝A和法宝B的聚气等级
	local NowAlevel=talisman1:QueryLevel()
	local NowBlevel=talisman2:QueryLevel()

	---//取得法宝A和法宝B的当前的生成时辰
	local NowATime=talisman1:QueryData(0,dataIndex["birth_hour12"])		
	local NowBTime=talisman2:QueryData(0,dataIndex["birth_hour12"])	
	local NowATimeEx=talisman1:QueryData(0,dataIndex["birth_min3"])		
	local NowBTimeEx=talisman2:QueryData(0,dataIndex["birth_min3"])	

	---//取得法宝A和法宝B的Qlevel(需求级别）	
	local NowAReqLvl=ZLuaTalExistInTmplIDs(id1)
	local NowBReqLvl=ZLuaTalExistInTmplIDs(id2)
	
	---//取得法宝A和法宝B的成长取向
	local NowAGrowType=talisman1:QueryData(0,dataIndex["speciality"]) or 0 	
	local NowBGrowType=talisman2:QueryData(0,dataIndex["speciality"]) or 0	

	---//处理cid不符合情况的返回处理
	
	-- 无灵媒，禁止血炼
	if type(cid) ~= "number" or cid == 0 then
		iswronginput = 1
		output:CreateItem(-1)
	end
--[[
	-- 普通血炼，灵媒和主法宝不匹配，禁止血炼	
	if temp_cid[cid].cidusetype == 1 
	and temp_cid[cid].cidloveid == talismans["NW_1_BaGuaShi"].tmplID 
	and temp_cid[cid].cidloveid == id2 
	and temp_cid[cid].cidloveid ~= id1 then
		talisman2:UpdateData(0,120,talisman2:QueryData(0,120)+1)
		talisman2:UpdateData(0,dataIndex["reborncont"],1)
		iswronginput=1
		output:CreateItem(-1)
	elseif temp_cid[cid].cidusetype == 1 
	and temp_cid[cid].cidloveid ~= id1 
	and temp_cid[cid].cidloveid2 ~= id1 then
		iswronginput=1
		output:CreateItem(-1)
	
	end 
--]]
	-- 普通血炼，灵媒和主法宝不匹配，禁止血炼	
	if temp_cid[cid].cidusetype == 1 
	and temp_cid[cid].cidloveid ~= id1 
	and temp_cid[cid].cidloveid2 ~= id1 then
		iswronginput=1
		output:CreateItem(-1)
	end 

	-- 向性血炼，灵媒和主法宝不匹配，禁止血炼	
	if temp_cid[cid].cidusetype == 3 and temp_cid[cid].cidwithlvl ~= NowAReqLvl then
		if temp_cid[cid].cidwithlvl ~= 75 or NowAReqLvl < 75 then
			iswronginput=1
			output:CreateItem(-1)
		end
	end 




	-- 噬魂合成血炼，材料必须是嗜血珠和摄魂	
	if temp_cid[cid].cidusetype == 2 and temp_cid[cid].cidloveid == talismans["US_145_ShiHun"].tmplID then
		if id1 == talismans["US_135_SheHun"].tmplID and id2 == talismans["US_145_ShiXueZhu"].tmplID then
			iswronginput = 0
		elseif id1 == talismans["US_145_ShiXueZhu"].tmplID and id2 == talismans["US_135_SheHun"].tmplID then
			iswronginput = 0
		else
			iswronginput = 1
			output:CreateItem(-1)
		end
	end
	
	---//处理法宝灌魔的特殊情况
	if temp_cid[cid].cidusetype==4 then
		iswronginput=1
		output:CreateItem(-1)
	elseif temp_cid[cid].cidusetype==5 then
		iswronginput=1
		ZLuaTalisman_CombineRechooseSkill(id1,id2,talisman1,talisman2,output)
	end 

	---//当前熔炼产出种类的声明
	local NowOrigin=ZLuaTalServerOrigins()
		
	---//当前熔炼值对于概率影响	
	local CombineIndex,CombineValueGrow	
	CombineIndex=ZLuaTalisman_CombineValueDeal(output:GetCombineValue())	
	
	if iswronginput~=1 then 	
		local WhatObjectTypeWeGet,WhatIDWeGet,WhatGrowTypeWeSee,mothertalisman,fathertalisman,ischangespeciality,foodid
		WhatObjectTypeWeGet=ZLuaTalisman_BurnMatrix(id1,id2,NowAReqLvl,NowBReqLvl,NowATime,NowBTime,NowATimeEx,NowBTimeEx,cid)
		if WhatObjectTypeWeGet==1 then 
			WhatIDWeGet=id1
			WhatGrowTypeWeSee=NowAGrowType
			mothertalisman=talisman1
			fathertalisman=talisman2
			foodid=id2					 		
			ischangespeciality=0
		elseif WhatObjectTypeWeGet==2 then 
			WhatIDWeGet=id2
			WhatGrowTypeWeSee=NowBGrowType 
			mothertalisman=talisman2
			fathertalisman=talisman1
			foodid=id1
			ischangespeciality=0	 
		elseif WhatObjectTypeWeGet==3 then 
		---//特殊产出
			WhatIDWeGet=ZLuaSpecialTalismanOut(cid,output,NowOrigin["combine"])
			ischangespeciality=0	 
			isspecialoutput=1
		elseif WhatObjectTypeWeGet==4 then 
			WhatIDWeGet=id1
			WhatGrowTypeWeSee=NowBGrowType
			mothertalisman=talisman1
			fathertalisman=talisman2
			foodid=id2
			ischangespeciality=1
		elseif WhatObjectTypeWeGet==5 then 
			WhatIDWeGet=id2
			WhatGrowTypeWeSee=NowAGrowType
			mothertalisman=talisman2
			fathertalisman=talisman1
			foodid=id1
			ischangespeciality=1
		end 
			
		---如果不是特殊产出类型则设定相关变化
		if isspecialoutput~=1 then 
						
			local WhatTypeWeGrow,HowManyGrow
			WhatGrowTypeWeSee,WhatTypeWeGrow,HowManyGrow=ZLuaTalisman_GrowMatrix(ischangespeciality,WhatGrowTypeWeSee,mothertalisman,fathertalisman,WhatIDWeGet,foodid,CombineIndex,cid)
			---表观成长类型,实际成长类型,实际成长幅度
				
			output:CreateItem(WhatIDWeGet)	
				
			---如果生成主法宝 不降级别
			if WhatIDWeGet==id1 then 
				output:InheritCombineLevel(true)		
	  --    if id1==talismans["NW_1_BaGuaShi"].tmplID and id2==temp_cid[cid].cidloveid then talisman1:UpdateData(0,1,talisman1:QueryData(0,1)+1) ZLuaTalDebugDisplay(talisman2,id1,cid,output)	end   --Debug
			end  
								
			---传递当前法宝类型的相应档位的成长上限信息
			local GradeLimitsList=ZLuaTalGradesUpperLimit(WhatIDWeGet)
--[[
			local n = GradeLimitsList
			if talisman1:QueryData(0,dataIndex["reborncont"]) ~= 0 then
				GradeLimitsList = {n[1]+90,n[2]+15,n[3]+15,n[4]+15,n[5]+15,n[6],n[7],n[8]+15,n[9]+15,n[10]+15,n[11]+15,n[12]+15,n[13]}
			end
--]]
			
			local growdirection=ZluaTransRiseIndex(WhatTypeWeGrow)
			local Limit
			if mothertalisman:QueryData(0,growdirection)~=nil then
				Limit=GradeLimitsList[ZLuaTalTransIndex(WhatTypeWeGrow)]+mothertalisman:QueryData(0,growdirection)
			else Limit=GradeLimitsList[ZLuaTalTransIndex(WhatTypeWeGrow)]
			end
					
			---如果不满挡就传递成长信息
			local nowLimit=ZLuaTalGetGradeFromValue(WhatTypeWeGrow,mothertalisman)
			if nowLimit>=Limit and ischangespeciality~=1 then 	
				output:CreateItem(-1)
			else 
	  		ZLuaTalCombineInit(WhatIDWeGet,WhatGrowTypeWeSee,WhatTypeWeGrow,HowManyGrow,mothertalisman,output)
			---//ID,表观成长类型,实际成长类型,实际成长幅度,母本法宝,output				
				
			---设定血炼相关记录
		 		if ischangespeciality==0 then ---(成长类型更换不计入血炼相关记录)
	 				output:UpdateData(0,dataIndex["combine_times"],output:QueryData(0,dataIndex["combine_times"])+1)
	 				local feedcosta,feedcostb=ZLuaTalisman_FoodAttributeValue(fathertalisman,foodid,nowLimit)
	 				output:UpdateData(0,dataIndex["feed_cost"],output:QueryData(0,dataIndex["feed_cost"])+feedcosta)
	 				output:UpdateData(0,dataIndex["feed_level_cost"],output:QueryData(0,dataIndex["feed_level_cost"])+feedcostb)
		 			if fathertalisman:QueryData(0,dataIndex["quality"])==0 then 
		 				output:UpdateData(0,dataIndex["feed_baoqi"],output:QueryData(0,dataIndex["feed_baoqi"])+1)
		 			elseif fathertalisman:QueryData(0,dataIndex["quality"])==1 then 
		 				output:UpdateData(0,dataIndex["feed_xianpin"],output:QueryData(0,dataIndex["feed_xianpin"])+1)
		 			elseif fathertalisman:QueryData(0,dataIndex["quality"])==2 then 
		 				output:UpdateData(0,dataIndex["feed_shenpin"],output:QueryData(0,dataIndex["feed_shenpin"])+1)
		 			else
		 			end
		 		end
			end
 	 	end  -- end of isspecialout

		----熔炼之后熔炼值变化,任何情况下增长血炼值
		if ischangespeciality==0 then 
			CombineValueGrow=ZLuaTalisman_CombineValueGrow(NowAlevel,NowBlevel,NowAReqLvl,NowBReqLvl,CombineIndex,1)
			output:SetCombineValue(output:GetCombineValue()+CombineValueGrow)
		end 

	end  --end of iswronginput
	end 
end 

----------------将辅法宝的技能切换到主法宝------------------------------------------------------------
function ZLuaTalisman_CombineRechooseSkill(id1,id2,talisman1,talisman2,output)
	local SkillNoSeqA={}				
	local SkillReqLvlSeqA={}
	
	local SkillNoSeqB={}				
	local SkillReqLvlSeqB={}
	
	local SkillNoSeqResult={}
	local SkillReqLvlSeqResult={}
	local SkillBurnChance={}
	
	local TrueSkillNumA=0
	local TrueSkillNumB=0
	local maxsamecol=1
	
	---//获得法宝目前的技能信息(技能ID,技能需求级别,实际技能数目)
	local dataIndex=ZLuaTalDataIndex()
		SkillNoSeqA[1]=talisman1:QueryData(0,dataIndex["addon_1_id"])
		SkillReqLvlSeqA[1]=talisman1:QueryData(0,dataIndex["addon_1_learn_level"])
		SkillNoSeqA[2]=talisman1:QueryData(0,dataIndex["addon_2_id"])
		SkillReqLvlSeqA[2]=talisman1:QueryData(0,dataIndex["addon_2_learn_level"])
		SkillNoSeqA[3]=talisman1:QueryData(0,dataIndex["addon_3_id"])
		SkillReqLvlSeqA[3]=talisman1:QueryData(0,dataIndex["addon_3_learn_level"])
		SkillNoSeqA[4]=talisman1:QueryData(0,dataIndex["addon_4_id"])
		SkillReqLvlSeqA[4]=talisman1:QueryData(0,dataIndex["addon_4_learn_level"])
		SkillNoSeqA[5]=talisman1:QueryData(0,dataIndex["addon_5_id"])
		SkillReqLvlSeqA[5]=talisman1:QueryData(0,dataIndex["addon_5_learn_level"])
		SkillNoSeqA[6]=talisman1:QueryData(0,dataIndex["addon_6_id"])
		SkillReqLvlSeqA[6]=talisman1:QueryData(0,dataIndex["addon_6_learn_level"])
		
		SkillNoSeqB[1]=talisman2:QueryData(0,dataIndex["addon_1_id"])
		SkillReqLvlSeqB[1]=talisman2:QueryData(0,dataIndex["addon_1_learn_level"])
		SkillNoSeqB[2]=talisman2:QueryData(0,dataIndex["addon_2_id"])
		SkillReqLvlSeqB[2]=talisman2:QueryData(0,dataIndex["addon_2_learn_level"])
		SkillNoSeqB[3]=talisman2:QueryData(0,dataIndex["addon_3_id"])
		SkillReqLvlSeqB[3]=talisman2:QueryData(0,dataIndex["addon_3_learn_level"])
		SkillNoSeqB[4]=talisman2:QueryData(0,dataIndex["addon_4_id"])
		SkillReqLvlSeqB[4]=talisman2:QueryData(0,dataIndex["addon_4_learn_level"])
		SkillNoSeqB[5]=talisman2:QueryData(0,dataIndex["addon_5_id"])
		SkillReqLvlSeqB[5]=talisman2:QueryData(0,dataIndex["addon_5_learn_level"])
		SkillNoSeqB[6]=talisman2:QueryData(0,dataIndex["addon_6_id"])
		SkillReqLvlSeqB[6]=talisman2:QueryData(0,dataIndex["addon_6_learn_level"])

	local talismanprofA,talismanskillprofA=ZLuaTalisman_GetProfInfo(id1,SkillNoSeqA)
	local talismanprofB,talismanskillprofB=ZLuaTalisman_GetProfInfo(id2,SkillNoSeqB)
	---//根据法宝的id和法宝上已经存在的技能决定法宝的门派属相
	
	for i=1,6 do
		if SkillNoSeqB[i]~=0 and SkillReqLvlSeqB[i]~=0 and type(SkillReqLvlSeqB[i])=="number" then 
			TrueSkillNumB=TrueSkillNumB+1
		end 
	end	
	
	if talismanskillprofA==talismanskillprofB then 
			---//去掉A中同B相同的技能			
			for j=1,6 do
				for k=1,6 do
					if SkillNoSeqA[j]==SkillNoSeqB[k] and SkillNoSeqA[j]~=0 then
						SkillNoSeqA[j]=0
						SkillReqLvlSeqA[j]=0
					end 
				end 
			end 		
			---//将A和B合并
			for m=1,6 do
				SkillNoSeqB[6+m]=SkillNoSeqA[m]
				SkillReqLvlSeqB[6+m]=SkillReqLvlSeqA[m]
				SkillNoSeqA[m]=0
				SkillReqLvlSeqA[m]=0
			end 

			for n=1,12 do
				if SkillNoSeqB[n]~=0 then 
					SkillBurnChance[n]=100
				else
					SkillBurnChance[n]=0
				end 		
			end 	
			 
			SkillNoSeqA,SkillReqLvlSeqA=ZLuaCheckOutNums(SkillNoSeqB,SkillBurnChance,SkillReqLvlSeqB,TrueSkillNumB,maxsamecol)	
			
			output:CreateItem(id1)
			for _,index in pairs(dataIndex) do
				output:UpdateData(0,index,talisman1:QueryData(0,index))
			end
			output:UpdateData(0,dataIndex["addon_1_id"],SkillNoSeqA[1]or 0)		
			output:UpdateData(0,dataIndex["addon_1_learn_level"],SkillReqLvlSeqA[1]or 0)
			output:UpdateData(0,dataIndex["addon_2_id"],SkillNoSeqA[2]or 0)		
			output:UpdateData(0,dataIndex["addon_2_learn_level"],SkillReqLvlSeqA[2]or 0)
			output:UpdateData(0,dataIndex["addon_3_id"],SkillNoSeqA[3]or 0)		
			output:UpdateData(0,dataIndex["addon_3_learn_level"],SkillReqLvlSeqA[3]or 0)
			output:UpdateData(0,dataIndex["addon_4_id"],SkillNoSeqA[4]or 0)		
			output:UpdateData(0,dataIndex["addon_4_learn_level"],SkillReqLvlSeqA[4]or 0)
			output:UpdateData(0,dataIndex["addon_5_id"],SkillNoSeqA[5]or 0)		
			output:UpdateData(0,dataIndex["addon_5_learn_level"],SkillReqLvlSeqA[5]or 0)
			output:UpdateData(0,dataIndex["addon_6_id"],SkillNoSeqA[6]or 0)		
			output:UpdateData(0,dataIndex["addon_6_learn_level"],SkillReqLvlSeqA[6]or 0)
			output:InheritCombineExp(true)
			ZLuaTalRebuild(output,id1)

	else 
		output:CreateItem(-1)
	end 
	
end 
-------------------------------------------------------------------------------------------------------------------
---//天人合一属性灌魔灵媒的分割线
--------------------------------------------------
function ZLuaTalismanEnchantEx()
	local TalismanEnchantEx={}
		---附加属性ID,灵媒ID,buff类型,对应法宝级别
		TalismanEnchantEx[909]={cidid=11385,bufftype=1,exlevel=1}
		TalismanEnchantEx[910]={cidid=11385,bufftype=1,exlevel=2}
		TalismanEnchantEx[911]={cidid=11385,bufftype=1,exlevel=3}
		TalismanEnchantEx[912]={cidid=11385,bufftype=1,exlevel=4}
		TalismanEnchantEx[913]={cidid=11385,bufftype=1,exlevel=5}
		TalismanEnchantEx[914]={cidid=11385,bufftype=1,exlevel=6}
		TalismanEnchantEx[915]={cidid=11385,bufftype=1,exlevel=7}
		TalismanEnchantEx[916]={cidid=11385,bufftype=1,exlevel=8}
		TalismanEnchantEx[917]={cidid=11385,bufftype=1,exlevel=9}
		TalismanEnchantEx[918]={cidid=11385,bufftype=1,exlevel=10}
		TalismanEnchantEx[919]={cidid=11385,bufftype=1,exlevel=11}
		TalismanEnchantEx[920]={cidid=11385,bufftype=1,exlevel=12}
		TalismanEnchantEx[921]={cidid=11385,bufftype=1,exlevel=13}
		TalismanEnchantEx[922]={cidid=11385,bufftype=1,exlevel=14}
		TalismanEnchantEx[923]={cidid=11385,bufftype=1,exlevel=15}
		TalismanEnchantEx[924]={cidid=11385,bufftype=1,exlevel=16}
		TalismanEnchantEx[925]={cidid=11385,bufftype=1,exlevel=17}
		TalismanEnchantEx[926]={cidid=11385,bufftype=1,exlevel=18}
		TalismanEnchantEx[927]={cidid=11385,bufftype=1,exlevel=19}
		TalismanEnchantEx[928]={cidid=11385,bufftype=1,exlevel=20}
		TalismanEnchantEx[929]={cidid=11385,bufftype=1,exlevel=21}
		TalismanEnchantEx[930]={cidid=11385,bufftype=1,exlevel=22}
		TalismanEnchantEx[931]={cidid=11385,bufftype=1,exlevel=23}
		TalismanEnchantEx[932]={cidid=11385,bufftype=1,exlevel=24}
		TalismanEnchantEx[933]={cidid=11385,bufftype=1,exlevel=25}
		TalismanEnchantEx[934]={cidid=11385,bufftype=1,exlevel=26}
		TalismanEnchantEx[935]={cidid=11385,bufftype=1,exlevel=27}
		TalismanEnchantEx[936]={cidid=11385,bufftype=1,exlevel=28}
		TalismanEnchantEx[937]={cidid=11385,bufftype=1,exlevel=29}
		TalismanEnchantEx[938]={cidid=11385,bufftype=1,exlevel=30}
		TalismanEnchantEx[3125]={cidid=11385,bufftype=1,exlevel=31}
		TalismanEnchantEx[3126]={cidid=11385,bufftype=1,exlevel=32}
		TalismanEnchantEx[3127]={cidid=11385,bufftype=1,exlevel=33}
		TalismanEnchantEx[3128]={cidid=11385,bufftype=1,exlevel=34}
		TalismanEnchantEx[3129]={cidid=11385,bufftype=1,exlevel=35}
		TalismanEnchantEx[34631]={cidid=11385,bufftype=1,exlevel=36}
		TalismanEnchantEx[34632]={cidid=11385,bufftype=1,exlevel=37}

		TalismanEnchantEx[939]={cidid=11386,bufftype=2,exlevel=1}
		TalismanEnchantEx[940]={cidid=11386,bufftype=2,exlevel=2}
		TalismanEnchantEx[941]={cidid=11386,bufftype=2,exlevel=3}
		TalismanEnchantEx[942]={cidid=11386,bufftype=2,exlevel=4}
		TalismanEnchantEx[943]={cidid=11386,bufftype=2,exlevel=5}
		TalismanEnchantEx[944]={cidid=11386,bufftype=2,exlevel=6}
		TalismanEnchantEx[945]={cidid=11386,bufftype=2,exlevel=7}
		TalismanEnchantEx[946]={cidid=11386,bufftype=2,exlevel=8}
		TalismanEnchantEx[947]={cidid=11386,bufftype=2,exlevel=9}
		TalismanEnchantEx[948]={cidid=11386,bufftype=2,exlevel=10}
		TalismanEnchantEx[949]={cidid=11386,bufftype=2,exlevel=11}
		TalismanEnchantEx[950]={cidid=11386,bufftype=2,exlevel=12}
		TalismanEnchantEx[951]={cidid=11386,bufftype=2,exlevel=13}
		TalismanEnchantEx[952]={cidid=11386,bufftype=2,exlevel=14}
		TalismanEnchantEx[953]={cidid=11386,bufftype=2,exlevel=15}
		TalismanEnchantEx[954]={cidid=11386,bufftype=2,exlevel=16}
		TalismanEnchantEx[955]={cidid=11386,bufftype=2,exlevel=17}
		TalismanEnchantEx[956]={cidid=11386,bufftype=2,exlevel=18}
		TalismanEnchantEx[957]={cidid=11386,bufftype=2,exlevel=19}
		TalismanEnchantEx[958]={cidid=11386,bufftype=2,exlevel=20}
		TalismanEnchantEx[959]={cidid=11386,bufftype=2,exlevel=21}
		TalismanEnchantEx[960]={cidid=11386,bufftype=2,exlevel=22}
		TalismanEnchantEx[961]={cidid=11386,bufftype=2,exlevel=23}
		TalismanEnchantEx[962]={cidid=11386,bufftype=2,exlevel=24}
		TalismanEnchantEx[963]={cidid=11386,bufftype=2,exlevel=25}
		TalismanEnchantEx[964]={cidid=11386,bufftype=2,exlevel=26}
		TalismanEnchantEx[965]={cidid=11386,bufftype=2,exlevel=27}
		TalismanEnchantEx[966]={cidid=11386,bufftype=2,exlevel=28}
		TalismanEnchantEx[967]={cidid=11386,bufftype=2,exlevel=29}
		TalismanEnchantEx[968]={cidid=11386,bufftype=2,exlevel=30}
		TalismanEnchantEx[3120]={cidid=11386,bufftype=2,exlevel=31}
		TalismanEnchantEx[3121]={cidid=11386,bufftype=2,exlevel=32}
		TalismanEnchantEx[3122]={cidid=11386,bufftype=2,exlevel=33}
		TalismanEnchantEx[3123]={cidid=11386,bufftype=2,exlevel=34}
		TalismanEnchantEx[3124]={cidid=11386,bufftype=2,exlevel=35}
		TalismanEnchantEx[34629]={cidid=11386,bufftype=2,exlevel=36}
		TalismanEnchantEx[34630]={cidid=11386,bufftype=2,exlevel=37}

		TalismanEnchantEx[969]={cidid=11387,bufftype=3,exlevel=1}
		TalismanEnchantEx[970]={cidid=11387,bufftype=3,exlevel=2}
		TalismanEnchantEx[971]={cidid=11387,bufftype=3,exlevel=3}
		TalismanEnchantEx[972]={cidid=11387,bufftype=3,exlevel=4}
		TalismanEnchantEx[973]={cidid=11387,bufftype=3,exlevel=5}
		TalismanEnchantEx[974]={cidid=11387,bufftype=3,exlevel=6}
		TalismanEnchantEx[975]={cidid=11387,bufftype=3,exlevel=7}
		TalismanEnchantEx[976]={cidid=11387,bufftype=3,exlevel=8}
		TalismanEnchantEx[977]={cidid=11387,bufftype=3,exlevel=9}
		TalismanEnchantEx[978]={cidid=11387,bufftype=3,exlevel=10}
		TalismanEnchantEx[979]={cidid=11387,bufftype=3,exlevel=11}
		TalismanEnchantEx[980]={cidid=11387,bufftype=3,exlevel=12}
		TalismanEnchantEx[981]={cidid=11387,bufftype=3,exlevel=13}
		TalismanEnchantEx[982]={cidid=11387,bufftype=3,exlevel=14}
		TalismanEnchantEx[983]={cidid=11387,bufftype=3,exlevel=15}
		TalismanEnchantEx[984]={cidid=11387,bufftype=3,exlevel=16}
		TalismanEnchantEx[985]={cidid=11387,bufftype=3,exlevel=17}
		TalismanEnchantEx[986]={cidid=11387,bufftype=3,exlevel=18}
		TalismanEnchantEx[987]={cidid=11387,bufftype=3,exlevel=19}
		TalismanEnchantEx[988]={cidid=11387,bufftype=3,exlevel=20}
		TalismanEnchantEx[989]={cidid=11387,bufftype=3,exlevel=21}
		TalismanEnchantEx[990]={cidid=11387,bufftype=3,exlevel=22}
		TalismanEnchantEx[991]={cidid=11387,bufftype=3,exlevel=23}
		TalismanEnchantEx[992]={cidid=11387,bufftype=3,exlevel=24}
		TalismanEnchantEx[993]={cidid=11387,bufftype=3,exlevel=25}
		TalismanEnchantEx[994]={cidid=11387,bufftype=3,exlevel=26}
		TalismanEnchantEx[995]={cidid=11387,bufftype=3,exlevel=27}
		TalismanEnchantEx[996]={cidid=11387,bufftype=3,exlevel=28}
		TalismanEnchantEx[997]={cidid=11387,bufftype=3,exlevel=29}
		TalismanEnchantEx[998]={cidid=11387,bufftype=3,exlevel=30}
		TalismanEnchantEx[3110]={cidid=11387,bufftype=3,exlevel=31}
		TalismanEnchantEx[3111]={cidid=11387,bufftype=3,exlevel=32}
		TalismanEnchantEx[3112]={cidid=11387,bufftype=3,exlevel=33}
		TalismanEnchantEx[3113]={cidid=11387,bufftype=3,exlevel=34}
		TalismanEnchantEx[3114]={cidid=11387,bufftype=3,exlevel=35}
		TalismanEnchantEx[34627]={cidid=11387,bufftype=3,exlevel=36}
		TalismanEnchantEx[34628]={cidid=11387,bufftype=3,exlevel=37}

		TalismanEnchantEx[999]={cidid=11388,bufftype=4,exlevel=1}
		TalismanEnchantEx[1000]={cidid=11388,bufftype=4,exlevel=2}
		TalismanEnchantEx[1001]={cidid=11388,bufftype=4,exlevel=3}
		TalismanEnchantEx[1002]={cidid=11388,bufftype=4,exlevel=4}
		TalismanEnchantEx[1003]={cidid=11388,bufftype=4,exlevel=5}
		TalismanEnchantEx[1004]={cidid=11388,bufftype=4,exlevel=6}
		TalismanEnchantEx[1005]={cidid=11388,bufftype=4,exlevel=7}
		TalismanEnchantEx[1006]={cidid=11388,bufftype=4,exlevel=8}
		TalismanEnchantEx[1007]={cidid=11388,bufftype=4,exlevel=9}
		TalismanEnchantEx[1008]={cidid=11388,bufftype=4,exlevel=10}
		TalismanEnchantEx[1009]={cidid=11388,bufftype=4,exlevel=11}
		TalismanEnchantEx[1010]={cidid=11388,bufftype=4,exlevel=12}
		TalismanEnchantEx[1011]={cidid=11388,bufftype=4,exlevel=13}
		TalismanEnchantEx[1012]={cidid=11388,bufftype=4,exlevel=14}
		TalismanEnchantEx[1013]={cidid=11388,bufftype=4,exlevel=15}
		TalismanEnchantEx[1014]={cidid=11388,bufftype=4,exlevel=16}
		TalismanEnchantEx[1015]={cidid=11388,bufftype=4,exlevel=17}
		TalismanEnchantEx[1016]={cidid=11388,bufftype=4,exlevel=18}
		TalismanEnchantEx[1017]={cidid=11388,bufftype=4,exlevel=19}
		TalismanEnchantEx[1018]={cidid=11388,bufftype=4,exlevel=20}
		TalismanEnchantEx[1019]={cidid=11388,bufftype=4,exlevel=21}
		TalismanEnchantEx[1020]={cidid=11388,bufftype=4,exlevel=22}
		TalismanEnchantEx[1021]={cidid=11388,bufftype=4,exlevel=23}
		TalismanEnchantEx[1022]={cidid=11388,bufftype=4,exlevel=24}
		TalismanEnchantEx[1023]={cidid=11388,bufftype=4,exlevel=25}
		TalismanEnchantEx[1024]={cidid=11388,bufftype=4,exlevel=26}
		TalismanEnchantEx[1025]={cidid=11388,bufftype=4,exlevel=27}
		TalismanEnchantEx[1026]={cidid=11388,bufftype=4,exlevel=28}
		TalismanEnchantEx[1027]={cidid=11388,bufftype=4,exlevel=29}
		TalismanEnchantEx[1028]={cidid=11388,bufftype=4,exlevel=30}
		TalismanEnchantEx[3105]={cidid=11388,bufftype=4,exlevel=31}
		TalismanEnchantEx[3106]={cidid=11388,bufftype=4,exlevel=32}
		TalismanEnchantEx[3107]={cidid=11388,bufftype=4,exlevel=33}
		TalismanEnchantEx[3108]={cidid=11388,bufftype=4,exlevel=34}
		TalismanEnchantEx[3109]={cidid=11388,bufftype=4,exlevel=35}
		TalismanEnchantEx[34625]={cidid=11388,bufftype=4,exlevel=36}
		TalismanEnchantEx[34626]={cidid=11388,bufftype=4,exlevel=37}

		TalismanEnchantEx[1029]={cidid=11389,bufftype=5,exlevel=1}
		TalismanEnchantEx[1030]={cidid=11389,bufftype=5,exlevel=2}
		TalismanEnchantEx[1031]={cidid=11389,bufftype=5,exlevel=3}
		TalismanEnchantEx[1032]={cidid=11389,bufftype=5,exlevel=4}
		TalismanEnchantEx[1033]={cidid=11389,bufftype=5,exlevel=5}
		TalismanEnchantEx[1034]={cidid=11389,bufftype=5,exlevel=6}
		TalismanEnchantEx[1035]={cidid=11389,bufftype=5,exlevel=7}
		TalismanEnchantEx[1036]={cidid=11389,bufftype=5,exlevel=8}
		TalismanEnchantEx[1037]={cidid=11389,bufftype=5,exlevel=9}
		TalismanEnchantEx[1038]={cidid=11389,bufftype=5,exlevel=10}
		TalismanEnchantEx[1039]={cidid=11389,bufftype=5,exlevel=11}
		TalismanEnchantEx[1040]={cidid=11389,bufftype=5,exlevel=12}
		TalismanEnchantEx[1041]={cidid=11389,bufftype=5,exlevel=13}
		TalismanEnchantEx[1042]={cidid=11389,bufftype=5,exlevel=14}
		TalismanEnchantEx[1043]={cidid=11389,bufftype=5,exlevel=15}
		TalismanEnchantEx[1044]={cidid=11389,bufftype=5,exlevel=16}
		TalismanEnchantEx[1045]={cidid=11389,bufftype=5,exlevel=17}
		TalismanEnchantEx[1046]={cidid=11389,bufftype=5,exlevel=18}
		TalismanEnchantEx[1047]={cidid=11389,bufftype=5,exlevel=19}
		TalismanEnchantEx[1048]={cidid=11389,bufftype=5,exlevel=20}
		TalismanEnchantEx[1049]={cidid=11389,bufftype=5,exlevel=21}
		TalismanEnchantEx[1050]={cidid=11389,bufftype=5,exlevel=22}
		TalismanEnchantEx[1051]={cidid=11389,bufftype=5,exlevel=23}
		TalismanEnchantEx[1052]={cidid=11389,bufftype=5,exlevel=24}
		TalismanEnchantEx[1053]={cidid=11389,bufftype=5,exlevel=25}
		TalismanEnchantEx[1054]={cidid=11389,bufftype=5,exlevel=26}
		TalismanEnchantEx[1055]={cidid=11389,bufftype=5,exlevel=27}
		TalismanEnchantEx[1056]={cidid=11389,bufftype=5,exlevel=28}
		TalismanEnchantEx[1057]={cidid=11389,bufftype=5,exlevel=29}
		TalismanEnchantEx[1058]={cidid=11389,bufftype=5,exlevel=30}
		TalismanEnchantEx[3115]={cidid=11389,bufftype=5,exlevel=31}
		TalismanEnchantEx[3116]={cidid=11389,bufftype=5,exlevel=32}
		TalismanEnchantEx[3117]={cidid=11389,bufftype=5,exlevel=33}
		TalismanEnchantEx[3118]={cidid=11389,bufftype=5,exlevel=34}
		TalismanEnchantEx[3119]={cidid=11389,bufftype=5,exlevel=35}
		TalismanEnchantEx[34633]={cidid=11389,bufftype=5,exlevel=36}
		TalismanEnchantEx[34634]={cidid=11389,bufftype=5,exlevel=37}
		
		TalismanEnchantEx[1059]={cidid=11390,bufftype=6,exlevel=1}
		TalismanEnchantEx[1060]={cidid=11390,bufftype=6,exlevel=2}
		TalismanEnchantEx[1061]={cidid=11390,bufftype=6,exlevel=3}
		TalismanEnchantEx[1062]={cidid=11390,bufftype=6,exlevel=4}
		TalismanEnchantEx[1063]={cidid=11390,bufftype=6,exlevel=5}
		TalismanEnchantEx[1064]={cidid=11390,bufftype=6,exlevel=6}
		TalismanEnchantEx[1065]={cidid=11390,bufftype=6,exlevel=7}
		TalismanEnchantEx[1066]={cidid=11390,bufftype=6,exlevel=8}
		TalismanEnchantEx[1067]={cidid=11390,bufftype=6,exlevel=9}
		TalismanEnchantEx[1068]={cidid=11390,bufftype=6,exlevel=10}
		TalismanEnchantEx[1069]={cidid=11390,bufftype=6,exlevel=11}
		TalismanEnchantEx[1070]={cidid=11390,bufftype=6,exlevel=12}
		TalismanEnchantEx[1071]={cidid=11390,bufftype=6,exlevel=13}
		TalismanEnchantEx[1072]={cidid=11390,bufftype=6,exlevel=14}
		TalismanEnchantEx[1073]={cidid=11390,bufftype=6,exlevel=15}
		TalismanEnchantEx[1074]={cidid=11390,bufftype=6,exlevel=16}
		TalismanEnchantEx[1075]={cidid=11390,bufftype=6,exlevel=17}
		TalismanEnchantEx[1076]={cidid=11390,bufftype=6,exlevel=18}
		TalismanEnchantEx[1077]={cidid=11390,bufftype=6,exlevel=19}
		TalismanEnchantEx[1078]={cidid=11390,bufftype=6,exlevel=20}
		TalismanEnchantEx[1079]={cidid=11390,bufftype=6,exlevel=21}
		TalismanEnchantEx[1080]={cidid=11390,bufftype=6,exlevel=22}
		TalismanEnchantEx[1081]={cidid=11390,bufftype=6,exlevel=23}
		TalismanEnchantEx[1082]={cidid=11390,bufftype=6,exlevel=24}
		TalismanEnchantEx[1083]={cidid=11390,bufftype=6,exlevel=25}
		TalismanEnchantEx[1084]={cidid=11390,bufftype=6,exlevel=26}
		TalismanEnchantEx[1085]={cidid=11390,bufftype=6,exlevel=27}
		TalismanEnchantEx[1086]={cidid=11390,bufftype=6,exlevel=28}
		TalismanEnchantEx[1087]={cidid=11390,bufftype=6,exlevel=29}
		TalismanEnchantEx[1088]={cidid=11390,bufftype=6,exlevel=30}
		TalismanEnchantEx[3100]={cidid=11390,bufftype=6,exlevel=31}
		TalismanEnchantEx[3101]={cidid=11390,bufftype=6,exlevel=32}
		TalismanEnchantEx[3102]={cidid=11390,bufftype=6,exlevel=33}
		TalismanEnchantEx[3103]={cidid=11390,bufftype=6,exlevel=34}
		TalismanEnchantEx[3104]={cidid=11390,bufftype=6,exlevel=35}
		TalismanEnchantEx[34623]={cidid=11390,bufftype=6,exlevel=36}
		TalismanEnchantEx[34624]={cidid=11390,bufftype=6,exlevel=37}
	return TalismanEnchantEx
end 
-------附加技能的属性信息序列(技能灌魔)---------------------------------------------------------
function ZLuaTalismanEnchantSkillEx()
	local SkillsExList={}
			---[附加属性(对应技能)ID]
			---prof:归属职业(0,1,2,3,4,5,6,9=少侠,青云,鬼王,合欢,天音,鬼道,冗余,通用)
			---bornchance:技能需求级别--对应的法宝级别
			---replacechance:被替代几率参数
			SkillsExList[337]={prof=21,reqlevel=1,bornchance=720,replacechance=100}---//技能:重击
			SkillsExList[338]={prof=21,reqlevel=2,bornchance=720,replacechance=100}---//技能:疗伤

			SkillsExList[339]={prof=1,reqlevel=4,bornchance=640,replacechance=120}---//技能:追风咒
			SkillsExList[340]={prof=1,reqlevel=3,bornchance=640,replacechance=120}---//技能:疯魔斩
			SkillsExList[341]={prof=1,reqlevel=5,bornchance=640,replacechance=120}---//技能:火焰刀
			SkillsExList[342]={prof=1,reqlevel=8,bornchance=640,replacechance=120}---//技能:寒冰刃
			SkillsExList[343]={prof=1,reqlevel=6,bornchance=640,replacechance=120}---//技能:逆水咒
			SkillsExList[344]={prof=1,reqlevel=14,bornchance=560,replacechance=180}---//技能:失魂斩
			SkillsExList[345]={prof=1,reqlevel=9,bornchance=560,replacechance=180}---//技能:迅雷斩
			SkillsExList[346]={prof=1,reqlevel=12,bornchance=560,replacechance=180}---//技能:焚心斩
			SkillsExList[347]={prof=1,reqlevel=11,bornchance=560,replacechance=180}---//技能:无妄斩
			SkillsExList[348]={prof=1,reqlevel=10,bornchance=560,replacechance=180}---//技能:猛火咒
			SkillsExList[349]={prof=1,reqlevel=17,bornchance=240,replacechance=240}---//技能:噬血斩
			SkillsExList[350]={prof=1,reqlevel=18,bornchance=240,replacechance=240}---//技能:天魔舞
			SkillsExList[351]={prof=1,reqlevel=15,bornchance=240,replacechance=240}---//技能:夺神斩
			SkillsExList[352]={prof=1,reqlevel=20,bornchance=240,replacechance=240}---//技能:修罗斩
			SkillsExList[353]={prof=1,reqlevel=16,bornchance=240,replacechance=240}---//技能:痴情咒
			SkillsExList[872]={prof=1,reqlevel=21,bornchance=40,replacechance=360}---//技能:霸天戮杀
			SkillsExList[873]={prof=1,reqlevel=22,bornchance=40,replacechance=360}---//技能:聚灵咒
			SkillsExList[874]={prof=1,reqlevel=23,bornchance=40,replacechance=360}---//技能:狱龙破
			SkillsExList[875]={prof=1,reqlevel=24,bornchance=40,replacechance=360}---//技能:旋龙幻杀
			SkillsExList[876]={prof=1,reqlevel=26,bornchance=40,replacechance=360}---//技能:苍劫阵
			SkillsExList[877]={prof=1,reqlevel=27,bornchance=20,replacechance=480}---//技能:魔魂天咒
			SkillsExList[878]={prof=1,reqlevel=28,bornchance=20,replacechance=480}---//技能:狂灵反噬
			SkillsExList[900]={prof=1,reqlevel=29,bornchance=20,replacechance=480}---//技能:毁天灭地
			SkillsExList[901]={prof=1,reqlevel=30,bornchance=20,replacechance=480}---//技能:未名斩

			SkillsExList[354]={prof=2,reqlevel=3,bornchance=640,replacechance=120}---//技能:断水
			SkillsExList[355]={prof=2,reqlevel=6,bornchance=640,replacechance=120}---//技能:情网
			SkillsExList[356]={prof=2,reqlevel=5,bornchance=640,replacechance=120}---//技能:烟雨
			SkillsExList[357]={prof=2,reqlevel=7,bornchance=640,replacechance=120}---//技能:热血
			SkillsExList[358]={prof=2,reqlevel=8,bornchance=640,replacechance=120}---//技能:破云
			SkillsExList[359]={prof=2,reqlevel=11,bornchance=560,replacechance=180}---//技能:飞花
			SkillsExList[360]={prof=2,reqlevel=9,bornchance=560,replacechance=180}---//技能:暗香
			SkillsExList[361]={prof=2,reqlevel=10,bornchance=560,replacechance=180}---//技能:迷离
			SkillsExList[362]={prof=2,reqlevel=12,bornchance=560,replacechance=180}---//技能:销魂
			SkillsExList[363]={prof=2,reqlevel=11,bornchance=560,replacechance=180}---//技能:相思
			SkillsExList[364]={prof=2,reqlevel=16,bornchance=240,replacechance=240}---//技能:温柔
			SkillsExList[365]={prof=2,reqlevel=17,bornchance=240,replacechance=240}---//技能:缠绵
			SkillsExList[366]={prof=2,reqlevel=16,bornchance=240,replacechance=240}---//技能:倾城
			SkillsExList[367]={prof=2,reqlevel=15,bornchance=240,replacechance=240}---//技能:蝶舞
			SkillsExList[368]={prof=2,reqlevel=18,bornchance=240,replacechance=240}---//技能:山盟
			SkillsExList[879]={prof=2,reqlevel=21,bornchance=40,replacechance=360}---//技能:碧影
			SkillsExList[880]={prof=2,reqlevel=22,bornchance=40,replacechance=360}---//技能:落霞
			SkillsExList[881]={prof=2,reqlevel=23,bornchance=40,replacechance=360}---//技能:云雨
			SkillsExList[882]={prof=2,reqlevel=24,bornchance=40,replacechance=360}---//技能:江南
			SkillsExList[883]={prof=2,reqlevel=25,bornchance=40,replacechance=360}---//技能:忘情
			SkillsExList[884]={prof=2,reqlevel=27,bornchance=20,replacechance=480}---//技能:旧梦
			SkillsExList[885]={prof=2,reqlevel=28,bornchance=20,replacechance=480}---//技能:惜春
			SkillsExList[902]={prof=2,reqlevel=29,bornchance=20,replacechance=480}---//技能:怜影
			SkillsExList[903]={prof=2,reqlevel=30,bornchance=20,replacechance=480}---//技能:风花雪月

			SkillsExList[369]={prof=3,reqlevel=5,bornchance=640,replacechance=120}---//技能:御剑诀
			SkillsExList[370]={prof=3,reqlevel=3,bornchance=640,replacechance=120}---//技能:寒冰咒
			SkillsExList[371]={prof=3,reqlevel=6,bornchance=640,replacechance=120}---//技能:驭雷术
			SkillsExList[372]={prof=3,reqlevel=4,bornchance=640,replacechance=120}---//技能:寒霜剑气
			SkillsExList[373]={prof=3,reqlevel=8,bornchance=640,replacechance=120}---//技能:归元剑气
			SkillsExList[374]={prof=3,reqlevel=14,bornchance=560,replacechance=180}---//技能:破魔剑气
			SkillsExList[375]={prof=3,reqlevel=9,bornchance=560,replacechance=180}---//技能:玄冰刺
			SkillsExList[376]={prof=3,reqlevel=12,bornchance=560,replacechance=180}---//技能:雷云风暴
			SkillsExList[377]={prof=3,reqlevel=11,bornchance=560,replacechance=180}---//技能:少阳剑气
			SkillsExList[378]={prof=3,reqlevel=10,bornchance=560,replacechance=180}---//技能:炼气还神
			SkillsExList[379]={prof=3,reqlevel=17,bornchance=240,replacechance=240}---//技能:怒剑狂花
			SkillsExList[380]={prof=3,reqlevel=15,bornchance=240,replacechance=240}---//技能:五雷轰顶
			SkillsExList[381]={prof=3,reqlevel=20,bornchance=240,replacechance=240}---//技能:天外飞仙
			SkillsExList[382]={prof=3,reqlevel=18,bornchance=240,replacechance=240}---//技能:雷神之锥
			SkillsExList[383]={prof=3,reqlevel=16,bornchance=240,replacechance=240}---//技能:雷霆震怒
			SkillsExList[886]={prof=3,reqlevel=21,bornchance=40,replacechance=360}---//技能:霜天雪舞
			SkillsExList[887]={prof=3,reqlevel=22,bornchance=40,replacechance=360}---//技能:天玄冰
			SkillsExList[888]={prof=3,reqlevel=23,bornchance=40,replacechance=360}---//技能:天诛剑气
			SkillsExList[889]={prof=3,reqlevel=24,bornchance=40,replacechance=360}---//技能:天地不仁
			SkillsExList[890]={prof=3,reqlevel=25,bornchance=40,replacechance=360}---//技能:天机印
			SkillsExList[891]={prof=3,reqlevel=27,bornchance=20,replacechance=480}---//技能:雷光遁龙诀
			SkillsExList[892]={prof=3,reqlevel=28,bornchance=20,replacechance=480}---//技能:天尊法身
			SkillsExList[904]={prof=3,reqlevel=29,bornchance=20,replacechance=480}---//技能:太极玄天真诀
			SkillsExList[905]={prof=3,reqlevel=30,bornchance=20,replacechance=480}---//技能:神剑御雷真诀

			SkillsExList[384]={prof=4,reqlevel=4,bornchance=640,replacechance=120}---//技能:大悲咒
			SkillsExList[385]={prof=4,reqlevel=3,bornchance=640,replacechance=120}---//技能:降魔杖
			SkillsExList[386]={prof=4,reqlevel=8,bornchance=640,replacechance=120}---//技能:狮子吼
			SkillsExList[387]={prof=4,reqlevel=6,bornchance=640,replacechance=120}---//技能:诵经
			SkillsExList[388]={prof=4,reqlevel=4,bornchance=640,replacechance=120}---//技能:易筋经
			SkillsExList[389]={prof=4,reqlevel=10,bornchance=560,replacechance=180}---//技能:宏愿
			SkillsExList[390]={prof=4,reqlevel=9,bornchance=560,replacechance=180}---//技能:般若经
			SkillsExList[391]={prof=4,reqlevel=12,bornchance=560,replacechance=180}---//技能:金刚经
			SkillsExList[392]={prof=4,reqlevel=14,bornchance=560,replacechance=180}---//技能:苦海无边
			SkillsExList[393]={prof=4,reqlevel=11,bornchance=560,replacechance=180}---//技能:四大皆空:火
			SkillsExList[394]={prof=4,reqlevel=13,bornchance=240,replacechance=240}---//技能:四大皆空:风
			SkillsExList[395]={prof=4,reqlevel=18,bornchance=240,replacechance=240}---//技能:同归极乐
			SkillsExList[396]={prof=4,reqlevel=16,bornchance=240,replacechance=240}---//技能:佛光普照
			SkillsExList[397]={prof=4,reqlevel=15,bornchance=240,replacechance=240}---//技能:伏魔经
			SkillsExList[398]={prof=4,reqlevel=20,bornchance=240,replacechance=240}---//技能:天地同寿
			SkillsExList[893]={prof=4,reqlevel=21,bornchance=40,replacechance=360}---//技能:拈花妙谛
			SkillsExList[894]={prof=4,reqlevel=22,bornchance=40,replacechance=360}---//技能:净土真言
			SkillsExList[895]={prof=4,reqlevel=23,bornchance=40,replacechance=360}---//技能:因果循环
			SkillsExList[896]={prof=4,reqlevel=24,bornchance=40,replacechance=360}---//技能:六道轮回
			SkillsExList[897]={prof=4,reqlevel=25,bornchance=40,replacechance=360}---//技能:慈航法愿
			SkillsExList[898]={prof=4,reqlevel=27,bornchance=20,replacechance=480}---//技能:雷音法谕
			SkillsExList[899]={prof=4,reqlevel=28,bornchance=20,replacechance=480}---//技能:无量真言
			SkillsExList[906]={prof=4,reqlevel=29,bornchance=20,replacechance=480}---//技能:大凡般若
			SkillsExList[907]={prof=4,reqlevel=30,bornchance=20,replacechance=480}---//技能:万象生佛

			SkillsExList[1758]={prof=5,reqlevel=3,bornchance=640,replacechance=120}---//技能:剔骨凶牙
			SkillsExList[1759]={prof=5,reqlevel=4,bornchance=640,replacechance=120}---//技能:化骨毒砂
			SkillsExList[1760]={prof=5,reqlevel=5,bornchance=640,replacechance=120}---//技能:幽魂利爪
			SkillsExList[1761]={prof=5,reqlevel=6,bornchance=640,replacechance=120}---//技能:浸地毒芒
			SkillsExList[1762]={prof=5,reqlevel=7,bornchance=640,replacechance=120}---//技能:噬魂血牙
			SkillsExList[1763]={prof=5,reqlevel=9,bornchance=560,replacechance=180}---//技能:旋风乱舞
			SkillsExList[1764]={prof=5,reqlevel=11,bornchance=560,replacechance=180}---//技能:蚀心毒蛊
			SkillsExList[1765]={prof=5,reqlevel=12,bornchance=560,replacechance=180}---//技能:含沙射影
			SkillsExList[1766]={prof=5,reqlevel=14,bornchance=560,replacechance=180}---//技能:凝血妖蛊
			SkillsExList[1767]={prof=5,reqlevel=13,bornchance=560,replacechance=180}---//技能:破刃邪魂
			SkillsExList[1768]={prof=5,reqlevel=15,bornchance=240,replacechance=240}---//技能:暴沙漫卷
			SkillsExList[1769]={prof=5,reqlevel=15,bornchance=240,replacechance=240}---//技能:怨鬼心魔
			SkillsExList[1770]={prof=5,reqlevel=17,bornchance=240,replacechance=240}---//技能:缚足蛇蛊
			SkillsExList[1771]={prof=5,reqlevel=18,bornchance=240,replacechance=240}---//技能:炼骨地刺
			SkillsExList[1772]={prof=5,reqlevel=19,bornchance=240,replacechance=240}---//技能:碎魂天袭
			SkillsExList[1773]={prof=5,reqlevel=21,bornchance=40,replacechance=360}---//技能:幕天狂沙
			SkillsExList[1774]={prof=5,reqlevel=23,bornchance=40,replacechance=360}---//技能:百蛊啖魂
			SkillsExList[1775]={prof=5,reqlevel=23,bornchance=40,replacechance=360}---//技能:燃魂劫火
			SkillsExList[1776]={prof=5,reqlevel=24,bornchance=40,replacechance=360}---//技能:毒龙蚀天
			SkillsExList[1777]={prof=5,reqlevel=25,bornchance=40,replacechance=360}---//技能:森罗三笑
			SkillsExList[1778]={prof=5,reqlevel=27,bornchance=20,replacechance=480}---//技能:灭魂狂飙
			SkillsExList[1779]={prof=5,reqlevel=28,bornchance=20,replacechance=480}---//技能:邪神傲世
			SkillsExList[1780]={prof=5,reqlevel=29,bornchance=20,replacechance=480}---//技能:神鬼乱舞
			SkillsExList[1781]={prof=5,reqlevel=30,bornchance=20,replacechance=480}---//技能:天选道怨

			SkillsExList[33387]={prof=6,reqlevel=3,bornchance=640,replacechance=120}---//技能:
			SkillsExList[33388]={prof=6,reqlevel=4,bornchance=640,replacechance=120}---//技能:
			SkillsExList[33389]={prof=6,reqlevel=5,bornchance=640,replacechance=120}---//技能:
			SkillsExList[33390]={prof=6,reqlevel=6,bornchance=640,replacechance=120}---//技能:
			SkillsExList[33391]={prof=6,reqlevel=7,bornchance=640,replacechance=120}---//技能:
			SkillsExList[33392]={prof=6,reqlevel=9,bornchance=560,replacechance=180}---//技能:
			SkillsExList[33393]={prof=6,reqlevel=11,bornchance=560,replacechance=180}---//技能:
			SkillsExList[33394]={prof=6,reqlevel=12,bornchance=560,replacechance=180}---//技能:
			SkillsExList[33395]={prof=6,reqlevel=14,bornchance=560,replacechance=180}---//技能:
			SkillsExList[33396]={prof=6,reqlevel=13,bornchance=560,replacechance=180}---//技能:
			SkillsExList[33397]={prof=6,reqlevel=15,bornchance=240,replacechance=240}---//技能:
			SkillsExList[33398]={prof=6,reqlevel=15,bornchance=240,replacechance=240}---//技能:
			SkillsExList[33399]={prof=6,reqlevel=17,bornchance=240,replacechance=240}---//技能:
			SkillsExList[33400]={prof=6,reqlevel=18,bornchance=240,replacechance=240}---//技能:
			SkillsExList[33401]={prof=6,reqlevel=19,bornchance=240,replacechance=240}---//技能:
			SkillsExList[33402]={prof=6,reqlevel=21,bornchance=40,replacechance=360}---//技能:
			SkillsExList[33403]={prof=6,reqlevel=23,bornchance=40,replacechance=360}---//技能:
			SkillsExList[33404]={prof=6,reqlevel=23,bornchance=40,replacechance=360}---//技能:
			SkillsExList[33405]={prof=6,reqlevel=24,bornchance=40,replacechance=360}---//技能:
			SkillsExList[33406]={prof=6,reqlevel=25,bornchance=40,replacechance=360}---//技能:
			SkillsExList[33407]={prof=6,reqlevel=27,bornchance=20,replacechance=480}---//技能:
			SkillsExList[33408]={prof=6,reqlevel=28,bornchance=20,replacechance=480}---//技能:
			SkillsExList[33409]={prof=6,reqlevel=29,bornchance=20,replacechance=480}---//技能:
			SkillsExList[33410]={prof=6,reqlevel=30,bornchance=20,replacechance=480}---//技能:

			SkillsExList[3510]={prof=7,reqlevel=3,bornchance=640,replacechance=120}---//技能:一击
			SkillsExList[3511]={prof=7,reqlevel=4,bornchance=640,replacechance=120}---//技能:怒喝
			SkillsExList[3512]={prof=7,reqlevel=5,bornchance=640,replacechance=120}---//技能:苦行
			SkillsExList[3513]={prof=7,reqlevel=6,bornchance=640,replacechance=120}---//技能:履尾
			SkillsExList[3514]={prof=7,reqlevel=7,bornchance=640,replacechance=120}---//技能:冷嗤
			SkillsExList[3515]={prof=7,reqlevel=9,bornchance=560,replacechance=180}---//技能:炙焰
			SkillsExList[3516]={prof=7,reqlevel=11,bornchance=560,replacechance=180}---//技能:雷霆
			SkillsExList[3517]={prof=7,reqlevel=12,bornchance=560,replacechance=180}---//技能:暗伤
			SkillsExList[3518]={prof=7,reqlevel=14,bornchance=560,replacechance=180}---//技能:沥血
			SkillsExList[3519]={prof=7,reqlevel=13,bornchance=560,replacechance=180}---//技能:破坚
			SkillsExList[3520]={prof=7,reqlevel=15,bornchance=240,replacechance=240}---//技能:山岚
			SkillsExList[3521]={prof=7,reqlevel=15,bornchance=240,replacechance=240}---//技能:虎扑
			SkillsExList[3522]={prof=7,reqlevel=17,bornchance=240,replacechance=240}---//技能:狂言
			SkillsExList[3523]={prof=7,reqlevel=18,bornchance=240,replacechance=240}---//技能:缠龙
			SkillsExList[3524]={prof=7,reqlevel=19,bornchance=240,replacechance=240}---//技能:破煞
			SkillsExList[3525]={prof=7,reqlevel=21,bornchance=40,replacechance=360}---//技能:连山
			SkillsExList[3526]={prof=7,reqlevel=23,bornchance=40,replacechance=360}---//技能:百烈
			SkillsExList[3527]={prof=7,reqlevel=23,bornchance=40,replacechance=360}---//技能:裂空
			SkillsExList[3528]={prof=7,reqlevel=24,bornchance=40,replacechance=360}---//技能:杀戮
			SkillsExList[3529]={prof=7,reqlevel=25,bornchance=40,replacechance=360}---//技能:碎刃
			SkillsExList[3530]={prof=7,reqlevel=27,bornchance=20,replacechance=480}---//技能:乱刃
			SkillsExList[3531]={prof=7,reqlevel=28,bornchance=20,replacechance=480}---//技能:八肱
			SkillsExList[3532]={prof=7,reqlevel=29,bornchance=20,replacechance=480}---//技能:蚩魂
			SkillsExList[3533]={prof=7,reqlevel=30,bornchance=20,replacechance=480}---//技能:鏖战

			SkillsExList[3534]={prof=8,reqlevel=3,bornchance=640,replacechance=120}---//技能:风鸣矢
			SkillsExList[3535]={prof=8,reqlevel=4,bornchance=640,replacechance=120}---//技能:狼牙矢
			SkillsExList[3536]={prof=8,reqlevel=5,bornchance=640,replacechance=120}---//技能:星芒击
			SkillsExList[3537]={prof=8,reqlevel=6,bornchance=640,replacechance=120}---//技能:毒炎咒
			SkillsExList[3538]={prof=8,reqlevel=7,bornchance=640,replacechance=120}---//技能:疾如风
			SkillsExList[3539]={prof=8,reqlevel=9,bornchance=560,replacechance=180}---//技能:凝血矢
			SkillsExList[3540]={prof=8,reqlevel=11,bornchance=560,replacechance=180}---//技能:眩光矢
			SkillsExList[3541]={prof=8,reqlevel=12,bornchance=560,replacechance=180}---//技能:空灵闪
			SkillsExList[3542]={prof=8,reqlevel=14,bornchance=560,replacechance=180}---//技能:缚神诀
			SkillsExList[3543]={prof=8,reqlevel=13,bornchance=560,replacechance=180}---//技能:徐如林
			SkillsExList[3544]={prof=8,reqlevel=15,bornchance=240,replacechance=240}---//技能:星月击
			SkillsExList[3545]={prof=8,reqlevel=15,bornchance=240,replacechance=240}---//技能:退魔矢
			SkillsExList[3546]={prof=8,reqlevel=17,bornchance=240,replacechance=240}---//技能:沧澜破
			SkillsExList[3547]={prof=8,reqlevel=18,bornchance=240,replacechance=240}---//技能:如梦令
			SkillsExList[3548]={prof=8,reqlevel=19,bornchance=240,replacechance=240}---//技能:侵如火
			SkillsExList[3549]={prof=8,reqlevel=21,bornchance=40,replacechance=360}---//技能:双飞火
			SkillsExList[3550]={prof=8,reqlevel=23,bornchance=40,replacechance=360}---//技能:镇龙击
			SkillsExList[3551]={prof=8,reqlevel=23,bornchance=40,replacechance=360}---//技能:赤乌坠
			SkillsExList[3552]={prof=8,reqlevel=24,bornchance=40,replacechance=360}---//技能:失魂引
			SkillsExList[3553]={prof=8,reqlevel=25,bornchance=40,replacechance=360}---//技能:安如山
			SkillsExList[3554]={prof=8,reqlevel=27,bornchance=20,replacechance=480}---//技能:三云龙
			SkillsExList[3555]={prof=8,reqlevel=28,bornchance=20,replacechance=480}---//技能:大羿王
			SkillsExList[3556]={prof=8,reqlevel=29,bornchance=20,replacechance=480}---//技能:轮回闪
			SkillsExList[3557]={prof=8,reqlevel=30,bornchance=20,replacechance=480}---//技能:龙须虎

			SkillsExList[3972]={prof=9,reqlevel=3,bornchance=640,replacechance=120}---//技能:猛袭
			SkillsExList[3973]={prof=9,reqlevel=4,bornchance=640,replacechance=120}---//技能:风切
			SkillsExList[3974]={prof=9,reqlevel=5,bornchance=640,replacechance=120}---//技能:侵杀
			SkillsExList[3975]={prof=9,reqlevel=6,bornchance=640,replacechance=120}---//技能:血毒
			SkillsExList[3976]={prof=9,reqlevel=7,bornchance=640,replacechance=120}---//技能:化影
			SkillsExList[3977]={prof=9,reqlevel=9,bornchance=560,replacechance=180}---//技能:潜袭
			SkillsExList[3978]={prof=9,reqlevel=11,bornchance=560,replacechance=180}---//技能:鬼切
			SkillsExList[3979]={prof=9,reqlevel=12,bornchance=560,replacechance=180}---//技能:瞬杀
			SkillsExList[3980]={prof=9,reqlevel=14,bornchance=560,replacechance=180}---//技能:乘崩
			SkillsExList[3981]={prof=9,reqlevel=13,bornchance=560,replacechance=180}---//技能:当身
			SkillsExList[3982]={prof=9,reqlevel=15,bornchance=240,replacechance=240}---//技能:猎袭
			SkillsExList[3983]={prof=9,reqlevel=15,bornchance=240,replacechance=240}---//技能:雷切
			SkillsExList[3984]={prof=9,reqlevel=17,bornchance=240,replacechance=240}---//技能:突杀
			SkillsExList[3985]={prof=9,reqlevel=18,bornchance=240,replacechance=240}---//技能:鬼幕
			SkillsExList[3986]={prof=9,reqlevel=19,bornchance=240,replacechance=240}---//技能:影返
			SkillsExList[3987]={prof=9,reqlevel=21,bornchance=40,replacechance=360}---//技能:牙袭
			SkillsExList[3988]={prof=9,reqlevel=23,bornchance=40,replacechance=360}---//技能:龙切
			SkillsExList[3989]={prof=9,reqlevel=23,bornchance=40,replacechance=360}---//技能:斗杀
			SkillsExList[3990]={prof=9,reqlevel=24,bornchance=40,replacechance=360}---//技能:分筋
			SkillsExList[3991]={prof=9,reqlevel=25,bornchance=40,replacechance=360}---//技能:受身
			SkillsExList[3992]={prof=9,reqlevel=27,bornchance=20,replacechance=480}---//技能:无想
			SkillsExList[3993]={prof=9,reqlevel=28,bornchance=20,replacechance=480}---//技能:天诛
			SkillsExList[3994]={prof=9,reqlevel=29,bornchance=20,replacechance=480}---//技能:一闪
			SkillsExList[3995]={prof=9,reqlevel=30,bornchance=20,replacechance=480}---//技能:影舞·极

			SkillsExList[3996]={prof=10,reqlevel=3,bornchance=640,replacechance=120}---//技能:弥纱音刃
			SkillsExList[3997]={prof=10,reqlevel=4,bornchance=640,replacechance=120}---//技能:语光音刃
			SkillsExList[3998]={prof=10,reqlevel=5,bornchance=640,replacechance=120}---//技能:悲秋华章
			SkillsExList[3999]={prof=10,reqlevel=6,bornchance=640,replacechance=120}---//技能:松风雅韵
			SkillsExList[4000]={prof=10,reqlevel=7,bornchance=640,replacechance=120}---//技能:鸣泉雅韵
			SkillsExList[4001]={prof=10,reqlevel=9,bornchance=560,replacechance=180}---//技能:地崖音刃
			SkillsExList[4002]={prof=10,reqlevel=11,bornchance=560,replacechance=180}---//技能:红鸾音刃
			SkillsExList[4003]={prof=10,reqlevel=12,bornchance=560,replacechance=180}---//技能:惊弦华章
			SkillsExList[4004]={prof=10,reqlevel=14,bornchance=560,replacechance=180}---//技能:青冥雅韵
			SkillsExList[4005]={prof=10,reqlevel=13,bornchance=560,replacechance=180}---//技能:天籁：平沙落雁
			SkillsExList[4006]={prof=10,reqlevel=15,bornchance=240,replacechance=240}---//技能:中平音刃
			SkillsExList[4007]={prof=10,reqlevel=15,bornchance=240,replacechance=240}---//技能:济岚音刃
			SkillsExList[4008]={prof=10,reqlevel=17,bornchance=240,replacechance=240}---//技能:朔月华章
			SkillsExList[4009]={prof=10,reqlevel=18,bornchance=240,replacechance=240}---//技能:云水雅韵
			SkillsExList[4010]={prof=10,reqlevel=19,bornchance=240,replacechance=240}---//技能:天籁：金蛇狂舞
			SkillsExList[4011]={prof=10,reqlevel=21,bornchance=40,replacechance=360}---//技能:庆龙音刃
			SkillsExList[4012]={prof=10,reqlevel=23,bornchance=40,replacechance=360}---//技能:升皇音刃
			SkillsExList[4013]={prof=10,reqlevel=23,bornchance=40,replacechance=360}---//技能:秋声雅韵
			SkillsExList[4014]={prof=10,reqlevel=24,bornchance=40,replacechance=360}---//技能:天籁：春江月夜
			SkillsExList[4015]={prof=10,reqlevel=25,bornchance=40,replacechance=360}---//技能:天籁：阳春白雪
			SkillsExList[4016]={prof=10,reqlevel=27,bornchance=20,replacechance=480}---//技能:神引：阳关三叠
			SkillsExList[4017]={prof=10,reqlevel=28,bornchance=20,replacechance=480}---//技能:九宵绝响
			SkillsExList[4018]={prof=10,reqlevel=29,bornchance=20,replacechance=480}---//技能:神引：梅花三弄
			SkillsExList[4019]={prof=10,reqlevel=30,bornchance=20,replacechance=480}---//技能:琼台雅韵

			SkillsExList[33411]={prof=11,reqlevel=3,bornchance=640,replacechance=120}---//技能:
			SkillsExList[33412]={prof=11,reqlevel=4,bornchance=640,replacechance=120}---//技能:
			SkillsExList[33413]={prof=11,reqlevel=5,bornchance=640,replacechance=120}---//技能:
			SkillsExList[33414]={prof=11,reqlevel=6,bornchance=640,replacechance=120}---//技能:
			SkillsExList[33415]={prof=11,reqlevel=7,bornchance=640,replacechance=120}---//技能:
			SkillsExList[33416]={prof=11,reqlevel=9,bornchance=560,replacechance=180}---//技能:
			SkillsExList[33417]={prof=11,reqlevel=11,bornchance=560,replacechance=180}---//技能:
			SkillsExList[33418]={prof=11,reqlevel=12,bornchance=560,replacechance=180}---//技能:
			SkillsExList[33419]={prof=11,reqlevel=14,bornchance=560,replacechance=180}---//技能:
			SkillsExList[33420]={prof=11,reqlevel=13,bornchance=560,replacechance=180}---//技能:
			SkillsExList[33421]={prof=11,reqlevel=15,bornchance=240,replacechance=240}---//技能:
			SkillsExList[33422]={prof=11,reqlevel=15,bornchance=240,replacechance=240}---//技能:
			SkillsExList[33423]={prof=11,reqlevel=17,bornchance=240,replacechance=240}---//技能:
			SkillsExList[33424]={prof=11,reqlevel=18,bornchance=240,replacechance=240}---//技能:
			SkillsExList[33425]={prof=11,reqlevel=19,bornchance=240,replacechance=240}---//技能:
			SkillsExList[33426]={prof=11,reqlevel=21,bornchance=40,replacechance=360}---//技能:
			SkillsExList[33427]={prof=11,reqlevel=23,bornchance=40,replacechance=360}---//技能:
			SkillsExList[33428]={prof=11,reqlevel=23,bornchance=40,replacechance=360}---//技能:
			SkillsExList[33429]={prof=11,reqlevel=24,bornchance=40,replacechance=360}---//技能:
			SkillsExList[33430]={prof=11,reqlevel=25,bornchance=40,replacechance=360}---//技能:
			SkillsExList[33431]={prof=11,reqlevel=27,bornchance=20,replacechance=480}---//技能:
			SkillsExList[33432]={prof=11,reqlevel=28,bornchance=20,replacechance=480}---//技能:
			SkillsExList[33433]={prof=11,reqlevel=29,bornchance=20,replacechance=480}---//技能:
			SkillsExList[33434]={prof=11,reqlevel=30,bornchance=20,replacechance=480}---//技能:

			SkillsExList[34035]={prof=12,reqlevel=3,bornchance=640,replacechance=120}---//技能:
			SkillsExList[34036]={prof=12,reqlevel=4,bornchance=640,replacechance=120}---//技能:
			SkillsExList[34037]={prof=12,reqlevel=5,bornchance=640,replacechance=120}---//技能:
			SkillsExList[34038]={prof=12,reqlevel=6,bornchance=640,replacechance=120}---//技能:
			SkillsExList[34039]={prof=12,reqlevel=7,bornchance=640,replacechance=120}---//技能:
			SkillsExList[34040]={prof=12,reqlevel=9,bornchance=560,replacechance=180}---//技能:
			SkillsExList[34041]={prof=12,reqlevel=11,bornchance=560,replacechance=180}---//技能:
			SkillsExList[34042]={prof=12,reqlevel=12,bornchance=560,replacechance=180}---//技能:
			SkillsExList[34043]={prof=12,reqlevel=14,bornchance=560,replacechance=180}---//技能:
			SkillsExList[34044]={prof=12,reqlevel=13,bornchance=560,replacechance=180}---//技能:
			SkillsExList[34045]={prof=12,reqlevel=15,bornchance=240,replacechance=240}---//技能:
			SkillsExList[34046]={prof=12,reqlevel=15,bornchance=240,replacechance=240}---//技能:
			SkillsExList[34047]={prof=12,reqlevel=17,bornchance=240,replacechance=240}---//技能:
			SkillsExList[34048]={prof=12,reqlevel=18,bornchance=240,replacechance=240}---//技能:
			SkillsExList[34049]={prof=12,reqlevel=19,bornchance=240,replacechance=240}---//技能:
			SkillsExList[34050]={prof=12,reqlevel=21,bornchance=40,replacechance=360}---//技能:
			SkillsExList[34051]={prof=12,reqlevel=23,bornchance=40,replacechance=360}---//技能:
			SkillsExList[34052]={prof=12,reqlevel=23,bornchance=40,replacechance=360}---//技能:
			SkillsExList[34053]={prof=12,reqlevel=24,bornchance=40,replacechance=360}---//技能:
			SkillsExList[34054]={prof=12,reqlevel=25,bornchance=40,replacechance=360}---//技能:
			SkillsExList[34055]={prof=12,reqlevel=27,bornchance=20,replacechance=480}---//技能:
			SkillsExList[34056]={prof=12,reqlevel=28,bornchance=20,replacechance=480}---//技能:
			SkillsExList[34057]={prof=12,reqlevel=29,bornchance=20,replacechance=480}---//技能:
			SkillsExList[34058]={prof=12,reqlevel=30,bornchance=20,replacechance=480}---//技能:
			
			---牵机
			SkillsExList[34707]={prof=13,reqlevel=3,bornchance=640,replacechance=120}---//技能:
			SkillsExList[34708]={prof=13,reqlevel=4,bornchance=640,replacechance=120}---//技能:
			SkillsExList[34709]={prof=13,reqlevel=5,bornchance=640,replacechance=120}---//技能:
			SkillsExList[34710]={prof=13,reqlevel=6,bornchance=640,replacechance=120}---//技能:
			SkillsExList[34711]={prof=13,reqlevel=7,bornchance=640,replacechance=120}---//技能:
			SkillsExList[34712]={prof=13,reqlevel=9,bornchance=560,replacechance=180}---//技能:
			SkillsExList[34713]={prof=13,reqlevel=11,bornchance=560,replacechance=180}---//技能:
			SkillsExList[34714]={prof=13,reqlevel=12,bornchance=560,replacechance=180}---//技能:
			SkillsExList[34715]={prof=13,reqlevel=14,bornchance=560,replacechance=180}---//技能:
			SkillsExList[34716]={prof=13,reqlevel=13,bornchance=560,replacechance=180}---//技能:
			SkillsExList[34717]={prof=13,reqlevel=15,bornchance=240,replacechance=240}---//技能:
			SkillsExList[34718]={prof=13,reqlevel=15,bornchance=240,replacechance=240}---//技能:
			SkillsExList[34719]={prof=13,reqlevel=17,bornchance=240,replacechance=240}---//技能:
			SkillsExList[34720]={prof=13,reqlevel=18,bornchance=240,replacechance=240}---//技能:
			SkillsExList[34721]={prof=13,reqlevel=19,bornchance=240,replacechance=240}---//技能:
			SkillsExList[34722]={prof=13,reqlevel=21,bornchance=40,replacechance=360}---//技能:
			SkillsExList[34723]={prof=13,reqlevel=23,bornchance=40,replacechance=360}---//技能:
			SkillsExList[34724]={prof=13,reqlevel=23,bornchance=40,replacechance=360}---//技能:
			SkillsExList[34725]={prof=13,reqlevel=24,bornchance=40,replacechance=360}---//技能:
			SkillsExList[34726]={prof=13,reqlevel=25,bornchance=40,replacechance=360}---//技能:
			SkillsExList[34727]={prof=13,reqlevel=27,bornchance=20,replacechance=480}---//技能:
			SkillsExList[34728]={prof=13,reqlevel=28,bornchance=20,replacechance=480}---//技能:
			SkillsExList[34729]={prof=13,reqlevel=29,bornchance=20,replacechance=480}---//技能:
			SkillsExList[34730]={prof=13,reqlevel=30,bornchance=20,replacechance=480}---//技能:

			---英招
			SkillsExList[34731]={prof=14,reqlevel=3,bornchance=640,replacechance=120}---//技能:
			SkillsExList[34732]={prof=14,reqlevel=4,bornchance=640,replacechance=120}---//技能:
			SkillsExList[34733]={prof=14,reqlevel=5,bornchance=640,replacechance=120}---//技能:
			SkillsExList[34734]={prof=14,reqlevel=6,bornchance=640,replacechance=120}---//技能:
			SkillsExList[34735]={prof=14,reqlevel=7,bornchance=640,replacechance=120}---//技能:
			SkillsExList[34736]={prof=14,reqlevel=9,bornchance=560,replacechance=180}---//技能:
			SkillsExList[34737]={prof=14,reqlevel=11,bornchance=560,replacechance=180}---//技能:
			SkillsExList[34738]={prof=14,reqlevel=12,bornchance=560,replacechance=180}---//技能:
			SkillsExList[34739]={prof=14,reqlevel=14,bornchance=560,replacechance=180}---//技能:
			SkillsExList[34740]={prof=14,reqlevel=13,bornchance=560,replacechance=180}---//技能:
			SkillsExList[34741]={prof=14,reqlevel=15,bornchance=240,replacechance=240}---//技能:
			SkillsExList[34742]={prof=14,reqlevel=15,bornchance=240,replacechance=240}---//技能:
			SkillsExList[34743]={prof=14,reqlevel=17,bornchance=240,replacechance=240}---//技能:
			SkillsExList[34744]={prof=14,reqlevel=18,bornchance=240,replacechance=240}---//技能:
			SkillsExList[34745]={prof=14,reqlevel=19,bornchance=240,replacechance=240}---//技能:
			SkillsExList[34746]={prof=14,reqlevel=21,bornchance=40,replacechance=360}---//技能:
			SkillsExList[34747]={prof=14,reqlevel=23,bornchance=40,replacechance=360}---//技能:
			SkillsExList[34748]={prof=14,reqlevel=23,bornchance=40,replacechance=360}---//技能:
			SkillsExList[34749]={prof=14,reqlevel=24,bornchance=40,replacechance=360}---//技能:
			SkillsExList[34750]={prof=14,reqlevel=25,bornchance=40,replacechance=360}---//技能:
			SkillsExList[34751]={prof=14,reqlevel=27,bornchance=20,replacechance=480}---//技能:
			SkillsExList[34752]={prof=14,reqlevel=28,bornchance=20,replacechance=480}---//技能:
			SkillsExList[34753]={prof=14,reqlevel=29,bornchance=20,replacechance=480}---//技能:
			SkillsExList[34754]={prof=14,reqlevel=30,bornchance=20,replacechance=480}---//技能:

	return SkillsExList
end 
-------------------------------------------------------------------------------------------------
----------描述技能灌魔灵媒的属性-----------------------------------------------------------------
function ZLuaTalismanEnchantSkillCid()
	local TalismanEnchantSkillCid={}
		---[灵媒ID]
		---prof:归属职业(0,1,2,3,4,5,6,9=少侠,鬼王,合欢,青云,天音,鬼道,冗余,通用)
		---enchantskilltype:对应技能灌魔类型(0=格数重随;1=重新随机(通用法宝换门派采用);2=门派随机技能灌魔;3=单个技能灌魔;
		---4=附加人物技能可见)
		---enchantskillid如果为单个技能灌魔,指向的技能号(附加属性号)
		---maxsameskillcol:如果不为单个技能灌魔,允许的最多同种技能出现次数
		TalismanEnchantSkillCid[11681]={prof=99,enchantskilltype=0,enchantskillid=337,maxsameskillcol=1} ---//格数重随
		TalismanEnchantSkillCid[23882]={prof=99,enchantskilltype=0,enchantskillid=337,maxsameskillcol=1} ---//格数重随

		TalismanEnchantSkillCid[62752]={prof=99,enchantskilltype=0,enchantskillid=337,maxsameskillcol=1} ---//新格数重随
		TalismanEnchantSkillCid[62753]={prof=99,enchantskilltype=0,enchantskillid=337,maxsameskillcol=1} ---//固定3阶2技能
		TalismanEnchantSkillCid[62754]={prof=99,enchantskilltype=0,enchantskillid=337,maxsameskillcol=1} ---//仅门派格数重随
		
		---//TalismanEnchantSkillCid[11680]={prof=9,enchantskilltype=1,enchantskillid=337,maxsameskillcol=1} ---//通用重随		
		---//功能与enchantskilltype=2合并
			
		TalismanEnchantSkillCid[11676]={prof=1,enchantskilltype=2,enchantskillid=337,maxsameskillcol=1} ---//门派重随->鬼王
		TalismanEnchantSkillCid[11677]={prof=2,enchantskilltype=2,enchantskillid=337,maxsameskillcol=1} ---//门派重随->合欢
		TalismanEnchantSkillCid[11678]={prof=3,enchantskilltype=2,enchantskillid=337,maxsameskillcol=1} ---//门派重随->青云
		TalismanEnchantSkillCid[11679]={prof=4,enchantskilltype=2,enchantskillid=337,maxsameskillcol=1} ---//门派重随->天音
		TalismanEnchantSkillCid[17832]={prof=5,enchantskilltype=2,enchantskillid=337,maxsameskillcol=1} ---//门派重随->鬼道
		TalismanEnchantSkillCid[49380]={prof=6,enchantskilltype=2,enchantskillid=337,maxsameskillcol=1} ---//门派重随->焚香
		TalismanEnchantSkillCid[31623]={prof=7,enchantskilltype=2,enchantskillid=337,maxsameskillcol=1} ---//门派重随->九黎
		TalismanEnchantSkillCid[31624]={prof=8,enchantskilltype=2,enchantskillid=337,maxsameskillcol=1} ---//门派重随->烈山
		TalismanEnchantSkillCid[39925]={prof=9,enchantskilltype=2,enchantskillid=337,maxsameskillcol=1} ---//门派重随->怀光
		TalismanEnchantSkillCid[39926]={prof=10,enchantskilltype=2,enchantskillid=337,maxsameskillcol=1} ---//门派重随->天华
		TalismanEnchantSkillCid[49381]={prof=11,enchantskilltype=2,enchantskillid=337,maxsameskillcol=1} ---//门派重随->太昊
		TalismanEnchantSkillCid[54051]={prof=12,enchantskilltype=2,enchantskillid=337,maxsameskillcol=1} ---//门派重随->辰皇
		TalismanEnchantSkillCid[58850]={prof=13,enchantskilltype=2,enchantskillid=337,maxsameskillcol=1} ---//门派重随->牵机
		TalismanEnchantSkillCid[58851]={prof=14,enchantskilltype=2,enchantskillid=337,maxsameskillcol=1} ---//门派重随->英招

		TalismanEnchantSkillCid[11572]={prof=21,enchantskilltype=3,enchantskillid=337,maxsameskillcol=1} ---//重击->灵媒
		TalismanEnchantSkillCid[11571]={prof=21,enchantskilltype=3,enchantskillid=338,maxsameskillcol=1} ---//疗伤->灵媒

		TalismanEnchantSkillCid[11577]={prof=1,enchantskilltype=3,enchantskillid=339,maxsameskillcol=1} ---//追风咒->灵媒
		TalismanEnchantSkillCid[11573]={prof=1,enchantskilltype=3,enchantskillid=340,maxsameskillcol=1} ---//疯魔斩->灵媒
		TalismanEnchantSkillCid[11575]={prof=1,enchantskilltype=3,enchantskillid=341,maxsameskillcol=1} ---//火焰刀->灵媒
		TalismanEnchantSkillCid[11574]={prof=1,enchantskilltype=3,enchantskillid=342,maxsameskillcol=1} ---//寒冰刃->灵媒
		TalismanEnchantSkillCid[11576]={prof=1,enchantskilltype=3,enchantskillid=343,maxsameskillcol=1} ---//逆水咒->灵媒
		TalismanEnchantSkillCid[11580]={prof=1,enchantskilltype=3,enchantskillid=344,maxsameskillcol=1} ---//失魂斩->灵媒
		TalismanEnchantSkillCid[11582]={prof=1,enchantskilltype=3,enchantskillid=345,maxsameskillcol=1} ---//迅雷斩->灵媒
		TalismanEnchantSkillCid[11578]={prof=1,enchantskilltype=3,enchantskillid=346,maxsameskillcol=1} ---//焚心斩->灵媒
		TalismanEnchantSkillCid[11581]={prof=1,enchantskilltype=3,enchantskillid=347,maxsameskillcol=1} ---//无妄斩->灵媒
		TalismanEnchantSkillCid[11579]={prof=1,enchantskilltype=3,enchantskillid=348,maxsameskillcol=1} ---//猛火咒->灵媒
		TalismanEnchantSkillCid[11585]={prof=1,enchantskilltype=3,enchantskillid=349,maxsameskillcol=1} ---//噬血斩->灵媒
		TalismanEnchantSkillCid[11586]={prof=1,enchantskilltype=3,enchantskillid=350,maxsameskillcol=1} ---//天魔舞->灵媒
		TalismanEnchantSkillCid[11584]={prof=1,enchantskilltype=3,enchantskillid=351,maxsameskillcol=1} ---//夺神斩->灵媒
		TalismanEnchantSkillCid[11587]={prof=1,enchantskilltype=3,enchantskillid=352,maxsameskillcol=1} ---//修罗斩->灵媒
		TalismanEnchantSkillCid[11583]={prof=1,enchantskilltype=3,enchantskillid=353,maxsameskillcol=1} ---//痴情咒->灵媒
		TalismanEnchantSkillCid[11590]={prof=1,enchantskilltype=3,enchantskillid=872,maxsameskillcol=1} ---//霸天戮杀->灵媒
		TalismanEnchantSkillCid[11592]={prof=1,enchantskilltype=3,enchantskillid=873,maxsameskillcol=1} ---//聚灵咒->灵媒
		TalismanEnchantSkillCid[11594]={prof=1,enchantskilltype=3,enchantskillid=874,maxsameskillcol=1} ---//狱龙破->灵媒
		TalismanEnchantSkillCid[11593]={prof=1,enchantskilltype=3,enchantskillid=875,maxsameskillcol=1} ---//旋龙幻杀->灵媒
		TalismanEnchantSkillCid[11591]={prof=1,enchantskilltype=3,enchantskillid=876,maxsameskillcol=1} ---//苍劫阵->灵媒
		TalismanEnchantSkillCid[11597]={prof=1,enchantskilltype=3,enchantskillid=877,maxsameskillcol=1} ---//魔魂天咒->灵媒
		TalismanEnchantSkillCid[11596]={prof=1,enchantskilltype=3,enchantskillid=878,maxsameskillcol=1} ---//狂灵反噬->灵媒
		TalismanEnchantSkillCid[11595]={prof=1,enchantskilltype=3,enchantskillid=900,maxsameskillcol=1} ---//毁天灭地->灵媒
		TalismanEnchantSkillCid[11598]={prof=1,enchantskilltype=3,enchantskillid=901,maxsameskillcol=1} ---//未名斩->灵媒

		TalismanEnchantSkillCid[11599]={prof=2,enchantskilltype=3,enchantskillid=354,maxsameskillcol=1} ---//断水->灵媒
		TalismanEnchantSkillCid[11601]={prof=2,enchantskilltype=3,enchantskillid=355,maxsameskillcol=1} ---//情网->灵媒
		TalismanEnchantSkillCid[11603]={prof=2,enchantskilltype=3,enchantskillid=356,maxsameskillcol=1} ---//烟雨->灵媒
		TalismanEnchantSkillCid[11602]={prof=2,enchantskilltype=3,enchantskillid=357,maxsameskillcol=1} ---//热血->灵媒
		TalismanEnchantSkillCid[11600]={prof=2,enchantskilltype=3,enchantskillid=358,maxsameskillcol=1} ---//破云->灵媒
		TalismanEnchantSkillCid[11605]={prof=2,enchantskilltype=3,enchantskillid=359,maxsameskillcol=1} ---//飞花->灵媒
		TalismanEnchantSkillCid[11604]={prof=2,enchantskilltype=3,enchantskillid=360,maxsameskillcol=1} ---//暗香->灵媒
		TalismanEnchantSkillCid[11606]={prof=2,enchantskilltype=3,enchantskillid=361,maxsameskillcol=1} ---//迷离->灵媒
		TalismanEnchantSkillCid[11608]={prof=2,enchantskilltype=3,enchantskillid=362,maxsameskillcol=1} ---//销魂->灵媒
		TalismanEnchantSkillCid[11607]={prof=2,enchantskilltype=3,enchantskillid=363,maxsameskillcol=1} ---//相思->灵媒
		TalismanEnchantSkillCid[11613]={prof=2,enchantskilltype=3,enchantskillid=364,maxsameskillcol=1} ---//温柔->灵媒
		TalismanEnchantSkillCid[11609]={prof=2,enchantskilltype=3,enchantskillid=365,maxsameskillcol=1} ---//缠绵->灵媒
		TalismanEnchantSkillCid[11611]={prof=2,enchantskilltype=3,enchantskillid=366,maxsameskillcol=1} ---//倾城->灵媒
		TalismanEnchantSkillCid[11610]={prof=2,enchantskilltype=3,enchantskillid=367,maxsameskillcol=1} ---//蝶舞->灵媒
		TalismanEnchantSkillCid[11612]={prof=2,enchantskilltype=3,enchantskillid=368,maxsameskillcol=1} ---//山盟->灵媒
		TalismanEnchantSkillCid[11614]={prof=2,enchantskilltype=3,enchantskillid=879,maxsameskillcol=1} ---//碧影->灵媒
		TalismanEnchantSkillCid[11616]={prof=2,enchantskilltype=3,enchantskillid=880,maxsameskillcol=1} ---//落霞->灵媒
		TalismanEnchantSkillCid[11618]={prof=2,enchantskilltype=3,enchantskillid=881,maxsameskillcol=1} ---//云雨->灵媒
		TalismanEnchantSkillCid[11615]={prof=2,enchantskilltype=3,enchantskillid=882,maxsameskillcol=1} ---//江南->灵媒
		TalismanEnchantSkillCid[11617]={prof=2,enchantskilltype=3,enchantskillid=883,maxsameskillcol=1} ---//忘情->灵媒
		TalismanEnchantSkillCid[11620]={prof=2,enchantskilltype=3,enchantskillid=884,maxsameskillcol=1} ---//旧梦->灵媒
		TalismanEnchantSkillCid[11622]={prof=2,enchantskilltype=3,enchantskillid=885,maxsameskillcol=1} ---//惜春->灵媒
		TalismanEnchantSkillCid[11621]={prof=2,enchantskilltype=3,enchantskillid=902,maxsameskillcol=1} ---//怜影->灵媒
		TalismanEnchantSkillCid[11619]={prof=2,enchantskilltype=3,enchantskillid=903,maxsameskillcol=1} ---//风花雪月->灵媒

		TalismanEnchantSkillCid[11627]={prof=3,enchantskilltype=3,enchantskillid=369,maxsameskillcol=1} ---//御剑诀->灵媒
		TalismanEnchantSkillCid[11624]={prof=3,enchantskilltype=3,enchantskillid=370,maxsameskillcol=1} ---//寒冰咒->灵媒
		TalismanEnchantSkillCid[11626]={prof=3,enchantskilltype=3,enchantskillid=371,maxsameskillcol=1} ---//驭雷术->灵媒
		TalismanEnchantSkillCid[11625]={prof=3,enchantskilltype=3,enchantskillid=372,maxsameskillcol=1} ---//寒霜剑气->灵媒
		TalismanEnchantSkillCid[11623]={prof=3,enchantskilltype=3,enchantskillid=373,maxsameskillcol=1} ---//归元剑气->灵媒
		TalismanEnchantSkillCid[11630]={prof=3,enchantskilltype=3,enchantskillid=374,maxsameskillcol=1} ---//破魔剑气->灵媒
		TalismanEnchantSkillCid[11632]={prof=3,enchantskilltype=3,enchantskillid=375,maxsameskillcol=1} ---//玄冰刺->灵媒
		TalismanEnchantSkillCid[11628]={prof=3,enchantskilltype=3,enchantskillid=376,maxsameskillcol=1} ---//雷云风暴->灵媒
		TalismanEnchantSkillCid[11631]={prof=3,enchantskilltype=3,enchantskillid=377,maxsameskillcol=1} ---//少阳剑气->灵媒
		TalismanEnchantSkillCid[11629]={prof=3,enchantskilltype=3,enchantskillid=378,maxsameskillcol=1} ---//炼气还神->灵媒
		TalismanEnchantSkillCid[11635]={prof=3,enchantskilltype=3,enchantskillid=379,maxsameskillcol=1} ---//怒剑狂花->灵媒
		TalismanEnchantSkillCid[11637]={prof=3,enchantskilltype=3,enchantskillid=380,maxsameskillcol=1} ---//五雷轰顶->灵媒
		TalismanEnchantSkillCid[11636]={prof=3,enchantskilltype=3,enchantskillid=381,maxsameskillcol=1} ---//天外飞仙->灵媒
		TalismanEnchantSkillCid[11633]={prof=3,enchantskilltype=3,enchantskillid=382,maxsameskillcol=1} ---//雷神之锥->灵媒
		TalismanEnchantSkillCid[11634]={prof=3,enchantskilltype=3,enchantskillid=383,maxsameskillcol=1} ---//雷霆震怒->灵媒
		TalismanEnchantSkillCid[11638]={prof=3,enchantskilltype=3,enchantskillid=886,maxsameskillcol=1} ---//霜天雪舞->灵媒
		TalismanEnchantSkillCid[11641]={prof=3,enchantskilltype=3,enchantskillid=887,maxsameskillcol=1} ---//天玄冰->灵媒
		TalismanEnchantSkillCid[11642]={prof=3,enchantskilltype=3,enchantskillid=888,maxsameskillcol=1} ---//天诛剑气->灵媒
		TalismanEnchantSkillCid[11639]={prof=3,enchantskilltype=3,enchantskillid=889,maxsameskillcol=1} ---//天地不仁->灵媒
		TalismanEnchantSkillCid[11640]={prof=3,enchantskilltype=3,enchantskillid=890,maxsameskillcol=1} ---//天机印->灵媒
		TalismanEnchantSkillCid[11643]={prof=3,enchantskilltype=3,enchantskillid=891,maxsameskillcol=1} ---//雷光遁龙诀->灵媒
		TalismanEnchantSkillCid[11646]={prof=3,enchantskilltype=3,enchantskillid=892,maxsameskillcol=1} ---//天尊法身->灵媒
		TalismanEnchantSkillCid[11645]={prof=3,enchantskilltype=3,enchantskillid=904,maxsameskillcol=1} ---//太极玄天真诀->灵媒
		TalismanEnchantSkillCid[11644]={prof=3,enchantskilltype=3,enchantskillid=905,maxsameskillcol=1} ---//神剑御雷真诀->灵媒

		TalismanEnchantSkillCid[11647]={prof=4,enchantskilltype=3,enchantskillid=384,maxsameskillcol=1} ---//大悲咒->灵媒
		TalismanEnchantSkillCid[11648]={prof=4,enchantskilltype=3,enchantskillid=385,maxsameskillcol=1} ---//降魔杖->灵媒
		TalismanEnchantSkillCid[11649]={prof=4,enchantskilltype=3,enchantskillid=386,maxsameskillcol=1} ---//狮子吼->灵媒
		TalismanEnchantSkillCid[11650]={prof=4,enchantskilltype=3,enchantskillid=387,maxsameskillcol=1} ---//诵经->灵媒
		TalismanEnchantSkillCid[11651]={prof=4,enchantskilltype=3,enchantskillid=388,maxsameskillcol=1} ---//易筋经->灵媒
		TalismanEnchantSkillCid[11653]={prof=4,enchantskilltype=3,enchantskillid=389,maxsameskillcol=1} ---//宏愿->灵媒
		TalismanEnchantSkillCid[11652]={prof=4,enchantskilltype=3,enchantskillid=390,maxsameskillcol=1} ---//般若经->灵媒
		TalismanEnchantSkillCid[11654]={prof=4,enchantskilltype=3,enchantskillid=391,maxsameskillcol=1} ---//金刚经->灵媒
		TalismanEnchantSkillCid[11655]={prof=4,enchantskilltype=3,enchantskillid=392,maxsameskillcol=1} ---//苦海无边->灵媒
		TalismanEnchantSkillCid[11658]={prof=4,enchantskilltype=3,enchantskillid=393,maxsameskillcol=1} ---//四大皆空:火->灵媒
		TalismanEnchantSkillCid[11663]={prof=4,enchantskilltype=3,enchantskillid=394,maxsameskillcol=1} ---//四大皆空:风->灵媒
		TalismanEnchantSkillCid[11665]={prof=4,enchantskilltype=3,enchantskillid=395,maxsameskillcol=1} ---//同归极乐->灵媒
		TalismanEnchantSkillCid[11659]={prof=4,enchantskilltype=3,enchantskillid=396,maxsameskillcol=1} ---//佛光普照->灵媒
		TalismanEnchantSkillCid[11662]={prof=4,enchantskilltype=3,enchantskillid=397,maxsameskillcol=1} ---//伏魔经->灵媒
		TalismanEnchantSkillCid[11664]={prof=4,enchantskilltype=3,enchantskillid=398,maxsameskillcol=1} ---//天地同寿->灵媒
		TalismanEnchantSkillCid[11669]={prof=4,enchantskilltype=3,enchantskillid=893,maxsameskillcol=1} ---//拈花妙谛->灵媒
		TalismanEnchantSkillCid[11667]={prof=4,enchantskilltype=3,enchantskillid=894,maxsameskillcol=1} ---//净土真言->灵媒
		TalismanEnchantSkillCid[11670]={prof=4,enchantskilltype=3,enchantskillid=895,maxsameskillcol=1} ---//因果循环->灵媒
		TalismanEnchantSkillCid[11668]={prof=4,enchantskilltype=3,enchantskillid=896,maxsameskillcol=1} ---//六道轮回->灵媒
		TalismanEnchantSkillCid[11666]={prof=4,enchantskilltype=3,enchantskillid=897,maxsameskillcol=1} ---//慈航法愿->灵媒
		TalismanEnchantSkillCid[11672]={prof=4,enchantskilltype=3,enchantskillid=898,maxsameskillcol=1} ---//雷音法谕->灵媒
		TalismanEnchantSkillCid[11674]={prof=4,enchantskilltype=3,enchantskillid=899,maxsameskillcol=1} ---//无量真言->灵媒
		TalismanEnchantSkillCid[11671]={prof=4,enchantskilltype=3,enchantskillid=906,maxsameskillcol=1} ---//大凡般若->灵媒
		TalismanEnchantSkillCid[11673]={prof=4,enchantskilltype=3,enchantskillid=907,maxsameskillcol=1} ---//万象生佛->灵媒

		TalismanEnchantSkillCid[17862]={prof=5,enchantskilltype=3,enchantskillid=1758,maxsameskillcol=1}---//剔骨凶牙->灵媒
		TalismanEnchantSkillCid[17864]={prof=5,enchantskilltype=3,enchantskillid=1759,maxsameskillcol=1}---//化骨毒砂->灵媒
		TalismanEnchantSkillCid[17866]={prof=5,enchantskilltype=3,enchantskillid=1760,maxsameskillcol=1}---//幽魂利爪->灵媒
		TalismanEnchantSkillCid[17867]={prof=5,enchantskilltype=3,enchantskillid=1761,maxsameskillcol=1}---//浸地毒芒->灵媒
		TalismanEnchantSkillCid[17868]={prof=5,enchantskilltype=3,enchantskillid=1762,maxsameskillcol=1}---//噬魂血牙->灵媒
		TalismanEnchantSkillCid[17869]={prof=5,enchantskilltype=3,enchantskillid=1763,maxsameskillcol=1}---//旋风乱舞->灵媒
		TalismanEnchantSkillCid[17870]={prof=5,enchantskilltype=3,enchantskillid=1764,maxsameskillcol=1}---//蚀心毒蛊->灵媒
		TalismanEnchantSkillCid[17871]={prof=5,enchantskilltype=3,enchantskillid=1765,maxsameskillcol=1}---//含沙射影->灵媒
		TalismanEnchantSkillCid[17872]={prof=5,enchantskilltype=3,enchantskillid=1766,maxsameskillcol=1}---//凝血妖蛊->灵媒
		TalismanEnchantSkillCid[17873]={prof=5,enchantskilltype=3,enchantskillid=1767,maxsameskillcol=1}---//破刃邪魂->灵媒
		TalismanEnchantSkillCid[17874]={prof=5,enchantskilltype=3,enchantskillid=1768,maxsameskillcol=1}---//暴沙漫卷->灵媒
		TalismanEnchantSkillCid[17875]={prof=5,enchantskilltype=3,enchantskillid=1769,maxsameskillcol=1}---//怨鬼心魔->灵媒
		TalismanEnchantSkillCid[17876]={prof=5,enchantskilltype=3,enchantskillid=1770,maxsameskillcol=1}---//缚足蛇蛊->灵媒
		TalismanEnchantSkillCid[17877]={prof=5,enchantskilltype=3,enchantskillid=1771,maxsameskillcol=1}---//炼骨地刺->灵媒
		TalismanEnchantSkillCid[17878]={prof=5,enchantskilltype=3,enchantskillid=1772,maxsameskillcol=1}---//碎魂天袭->灵媒
		TalismanEnchantSkillCid[17879]={prof=5,enchantskilltype=3,enchantskillid=1773,maxsameskillcol=1}---//幕天狂沙->灵媒
		TalismanEnchantSkillCid[17880]={prof=5,enchantskilltype=3,enchantskillid=1774,maxsameskillcol=1}---//百蛊啖魂->灵媒
		TalismanEnchantSkillCid[17881]={prof=5,enchantskilltype=3,enchantskillid=1775,maxsameskillcol=1}---//燃魂劫火->灵媒
		TalismanEnchantSkillCid[17882]={prof=5,enchantskilltype=3,enchantskillid=1776,maxsameskillcol=1}---//毒龙蚀天->灵媒
		TalismanEnchantSkillCid[17883]={prof=5,enchantskilltype=3,enchantskillid=1777,maxsameskillcol=1}---//森罗三笑->灵媒
		TalismanEnchantSkillCid[17884]={prof=5,enchantskilltype=3,enchantskillid=1778,maxsameskillcol=1}---//灭魂狂飙->灵媒
		TalismanEnchantSkillCid[17885]={prof=5,enchantskilltype=3,enchantskillid=1779,maxsameskillcol=1}---//邪神傲世->灵媒
		TalismanEnchantSkillCid[17886]={prof=5,enchantskilltype=3,enchantskillid=1780,maxsameskillcol=1}---//神鬼乱舞->灵媒
		TalismanEnchantSkillCid[17887]={prof=5,enchantskilltype=3,enchantskillid=1781,maxsameskillcol=1}---//天选道怨->灵媒

		TalismanEnchantSkillCid[49392]={prof=6,enchantskilltype=3,enchantskillid=33387,maxsameskillcol=1}---//星火->灵媒
		TalismanEnchantSkillCid[49393]={prof=6,enchantskilltype=3,enchantskillid=33388,maxsameskillcol=1}---//孪星->灵媒
		TalismanEnchantSkillCid[49394]={prof=6,enchantskilltype=3,enchantskillid=33389,maxsameskillcol=1}---//原灵->灵媒
		TalismanEnchantSkillCid[49395]={prof=6,enchantskilltype=3,enchantskillid=33390,maxsameskillcol=1}---//聚铄->灵媒
		TalismanEnchantSkillCid[49396]={prof=6,enchantskilltype=3,enchantskillid=33391,maxsameskillcol=1}---//天守->灵媒
		TalismanEnchantSkillCid[49397]={prof=6,enchantskilltype=3,enchantskillid=33392,maxsameskillcol=1}---//烈焰袭->灵媒
		TalismanEnchantSkillCid[49398]={prof=6,enchantskilltype=3,enchantskillid=33393,maxsameskillcol=1}---//幽炎咒->灵媒
		TalismanEnchantSkillCid[49399]={prof=6,enchantskilltype=3,enchantskillid=33394,maxsameskillcol=1}---//一线天->灵媒
		TalismanEnchantSkillCid[49400]={prof=6,enchantskilltype=3,enchantskillid=33395,maxsameskillcol=1}---//业火杀->灵媒
		TalismanEnchantSkillCid[49401]={prof=6,enchantskilltype=3,enchantskillid=33396,maxsameskillcol=1}---//霸邪障->灵媒
		TalismanEnchantSkillCid[49402]={prof=6,enchantskilltype=3,enchantskillid=33397,maxsameskillcol=1}---//三味真火->灵媒
		TalismanEnchantSkillCid[49403]={prof=6,enchantskilltype=3,enchantskillid=33398,maxsameskillcol=1}---//炎兵炙魂->灵媒
		TalismanEnchantSkillCid[49404]={prof=6,enchantskilltype=3,enchantskillid=33399,maxsameskillcol=1}---//火猎护主->灵媒
		TalismanEnchantSkillCid[49405]={prof=6,enchantskilltype=3,enchantskillid=33400,maxsameskillcol=1}---//天锁囚怨->灵媒
		TalismanEnchantSkillCid[49406]={prof=6,enchantskilltype=3,enchantskillid=33401,maxsameskillcol=1}---//劫火明尊->灵媒
		TalismanEnchantSkillCid[49407]={prof=6,enchantskilltype=3,enchantskillid=33402,maxsameskillcol=1}---//缚足锁魂->灵媒
		TalismanEnchantSkillCid[49408]={prof=6,enchantskilltype=3,enchantskillid=33403,maxsameskillcol=1}---//炽炎千里->灵媒
		TalismanEnchantSkillCid[49409]={prof=6,enchantskilltype=3,enchantskillid=33404,maxsameskillcol=1}---//玄阳火典->灵媒
		TalismanEnchantSkillCid[49410]={prof=6,enchantskilltype=3,enchantskillid=33405,maxsameskillcol=1}---//炼狱劫波->灵媒
		TalismanEnchantSkillCid[49411]={prof=6,enchantskilltype=3,enchantskillid=33406,maxsameskillcol=1}---//祝融真典->灵媒
		TalismanEnchantSkillCid[49413]={prof=6,enchantskilltype=3,enchantskillid=33407,maxsameskillcol=1}---//南巫天火->灵媒
		TalismanEnchantSkillCid[49414]={prof=6,enchantskilltype=3,enchantskillid=33408,maxsameskillcol=1}---//太渊业海->灵媒
		TalismanEnchantSkillCid[49415]={prof=6,enchantskilltype=3,enchantskillid=33409,maxsameskillcol=1}---//焚香玉册->灵媒
		TalismanEnchantSkillCid[49416]={prof=6,enchantskilltype=3,enchantskillid=33410,maxsameskillcol=1}---//八凶玄火->灵媒

		TalismanEnchantSkillCid[31331]={prof=7,enchantskilltype=3,enchantskillid=3510,maxsameskillcol=1}--//一击->灵媒
		TalismanEnchantSkillCid[31332]={prof=7,enchantskilltype=3,enchantskillid=3511,maxsameskillcol=1}--//怒喝->灵媒
		TalismanEnchantSkillCid[31333]={prof=7,enchantskilltype=3,enchantskillid=3512,maxsameskillcol=1}--//苦行->灵媒
		TalismanEnchantSkillCid[31334]={prof=7,enchantskilltype=3,enchantskillid=3513,maxsameskillcol=1}--//履尾->灵媒
		TalismanEnchantSkillCid[31335]={prof=7,enchantskilltype=3,enchantskillid=3514,maxsameskillcol=1}--//冷嗤->灵媒
		TalismanEnchantSkillCid[31336]={prof=7,enchantskilltype=3,enchantskillid=3515,maxsameskillcol=1}--//炙焰->灵媒
		TalismanEnchantSkillCid[31337]={prof=7,enchantskilltype=3,enchantskillid=3516,maxsameskillcol=1}--//雷霆->灵媒
		TalismanEnchantSkillCid[31338]={prof=7,enchantskilltype=3,enchantskillid=3517,maxsameskillcol=1}--//暗伤->灵媒
		TalismanEnchantSkillCid[31339]={prof=7,enchantskilltype=3,enchantskillid=3518,maxsameskillcol=1}--//沥血->灵媒
		TalismanEnchantSkillCid[31340]={prof=7,enchantskilltype=3,enchantskillid=3519,maxsameskillcol=1}--//破坚->灵媒
		TalismanEnchantSkillCid[31341]={prof=7,enchantskilltype=3,enchantskillid=3520,maxsameskillcol=1}--//山岚->灵媒
		TalismanEnchantSkillCid[31342]={prof=7,enchantskilltype=3,enchantskillid=3521,maxsameskillcol=1}--//虎扑->灵媒
		TalismanEnchantSkillCid[31343]={prof=7,enchantskilltype=3,enchantskillid=3522,maxsameskillcol=1}--//狂言->灵媒
		TalismanEnchantSkillCid[31344]={prof=7,enchantskilltype=3,enchantskillid=3523,maxsameskillcol=1}--//缠龙->灵媒
		TalismanEnchantSkillCid[31345]={prof=7,enchantskilltype=3,enchantskillid=3524,maxsameskillcol=1}--//破煞->灵媒
		TalismanEnchantSkillCid[31346]={prof=7,enchantskilltype=3,enchantskillid=3525,maxsameskillcol=1}--//连山->灵媒
		TalismanEnchantSkillCid[31347]={prof=7,enchantskilltype=3,enchantskillid=3526,maxsameskillcol=1}--//百烈->灵媒
		TalismanEnchantSkillCid[31348]={prof=7,enchantskilltype=3,enchantskillid=3527,maxsameskillcol=1}--//裂空->灵媒
		TalismanEnchantSkillCid[31349]={prof=7,enchantskilltype=3,enchantskillid=3528,maxsameskillcol=1}--//杀戮->灵媒
		TalismanEnchantSkillCid[31350]={prof=7,enchantskilltype=3,enchantskillid=3529,maxsameskillcol=1}--//碎刃->灵媒
		TalismanEnchantSkillCid[31351]={prof=7,enchantskilltype=3,enchantskillid=3530,maxsameskillcol=1}--//乱刃->灵媒
		TalismanEnchantSkillCid[31352]={prof=7,enchantskilltype=3,enchantskillid=3531,maxsameskillcol=1}--//八肱->灵媒
		TalismanEnchantSkillCid[31353]={prof=7,enchantskilltype=3,enchantskillid=3532,maxsameskillcol=1}--//蚩魂->灵媒
		TalismanEnchantSkillCid[31354]={prof=7,enchantskilltype=3,enchantskillid=3533,maxsameskillcol=1}--//鏖战->灵媒

		TalismanEnchantSkillCid[31355]={prof=8,enchantskilltype=3,enchantskillid=3534,maxsameskillcol=1}--//风鸣矢->灵媒
		TalismanEnchantSkillCid[31356]={prof=8,enchantskilltype=3,enchantskillid=3535,maxsameskillcol=1}--//狼牙矢->灵媒
		TalismanEnchantSkillCid[31357]={prof=8,enchantskilltype=3,enchantskillid=3536,maxsameskillcol=1}--//星芒击->灵媒
		TalismanEnchantSkillCid[31358]={prof=8,enchantskilltype=3,enchantskillid=3537,maxsameskillcol=1}--//毒炎咒->灵媒
		TalismanEnchantSkillCid[31359]={prof=8,enchantskilltype=3,enchantskillid=3538,maxsameskillcol=1}--//疾如风->灵媒
		TalismanEnchantSkillCid[31360]={prof=8,enchantskilltype=3,enchantskillid=3539,maxsameskillcol=1}--//凝血矢->灵媒
		TalismanEnchantSkillCid[31361]={prof=8,enchantskilltype=3,enchantskillid=3540,maxsameskillcol=1}--//眩光矢->灵媒
		TalismanEnchantSkillCid[31362]={prof=8,enchantskilltype=3,enchantskillid=3541,maxsameskillcol=1}--//空灵闪->灵媒
		TalismanEnchantSkillCid[31363]={prof=8,enchantskilltype=3,enchantskillid=3542,maxsameskillcol=1}--//缚神诀->灵媒
		TalismanEnchantSkillCid[31364]={prof=8,enchantskilltype=3,enchantskillid=3543,maxsameskillcol=1}--//徐如林->灵媒
		TalismanEnchantSkillCid[31365]={prof=8,enchantskilltype=3,enchantskillid=3544,maxsameskillcol=1}--//星月击->灵媒
		TalismanEnchantSkillCid[31366]={prof=8,enchantskilltype=3,enchantskillid=3545,maxsameskillcol=1}--//退魔矢->灵媒
		TalismanEnchantSkillCid[31367]={prof=8,enchantskilltype=3,enchantskillid=3546,maxsameskillcol=1}--//沧澜破->灵媒
		TalismanEnchantSkillCid[31368]={prof=8,enchantskilltype=3,enchantskillid=3547,maxsameskillcol=1}--//如梦令->灵媒
		TalismanEnchantSkillCid[31369]={prof=8,enchantskilltype=3,enchantskillid=3548,maxsameskillcol=1}--//侵如火->灵媒
		TalismanEnchantSkillCid[31370]={prof=8,enchantskilltype=3,enchantskillid=3549,maxsameskillcol=1}--//双飞火->灵媒
		TalismanEnchantSkillCid[31371]={prof=8,enchantskilltype=3,enchantskillid=3550,maxsameskillcol=1}--//镇龙击->灵媒
		TalismanEnchantSkillCid[31372]={prof=8,enchantskilltype=3,enchantskillid=3551,maxsameskillcol=1}--//赤乌坠->灵媒
		TalismanEnchantSkillCid[31373]={prof=8,enchantskilltype=3,enchantskillid=3552,maxsameskillcol=1}--//失魂引->灵媒
		TalismanEnchantSkillCid[31618]={prof=8,enchantskilltype=3,enchantskillid=3553,maxsameskillcol=1}--//安如山->灵媒
		TalismanEnchantSkillCid[31374]={prof=8,enchantskilltype=3,enchantskillid=3554,maxsameskillcol=1}--//三云龙->灵媒
		TalismanEnchantSkillCid[31375]={prof=8,enchantskilltype=3,enchantskillid=3555,maxsameskillcol=1}--//大羿王->灵媒
		TalismanEnchantSkillCid[31376]={prof=8,enchantskilltype=3,enchantskillid=3556,maxsameskillcol=1}--//轮回闪->灵媒
		TalismanEnchantSkillCid[31377]={prof=8,enchantskilltype=3,enchantskillid=3557,maxsameskillcol=1}--//龙须虎->灵媒

		TalismanEnchantSkillCid[39990]={prof=9,enchantskilltype=3,enchantskillid=3972,maxsameskillcol=1}
		TalismanEnchantSkillCid[39991]={prof=9,enchantskilltype=3,enchantskillid=3973,maxsameskillcol=1}
		TalismanEnchantSkillCid[39992]={prof=9,enchantskilltype=3,enchantskillid=3974,maxsameskillcol=1}
		TalismanEnchantSkillCid[39993]={prof=9,enchantskilltype=3,enchantskillid=3975,maxsameskillcol=1}
		TalismanEnchantSkillCid[39994]={prof=9,enchantskilltype=3,enchantskillid=3976,maxsameskillcol=1}
		TalismanEnchantSkillCid[39995]={prof=9,enchantskilltype=3,enchantskillid=3977,maxsameskillcol=1}
		TalismanEnchantSkillCid[39996]={prof=9,enchantskilltype=3,enchantskillid=3978,maxsameskillcol=1}
		TalismanEnchantSkillCid[39997]={prof=9,enchantskilltype=3,enchantskillid=3979,maxsameskillcol=1}
		TalismanEnchantSkillCid[39998]={prof=9,enchantskilltype=3,enchantskillid=3980,maxsameskillcol=1}
		TalismanEnchantSkillCid[39999]={prof=9,enchantskilltype=3,enchantskillid=3981,maxsameskillcol=1}
		TalismanEnchantSkillCid[40000]={prof=9,enchantskilltype=3,enchantskillid=3982,maxsameskillcol=1}
		TalismanEnchantSkillCid[40001]={prof=9,enchantskilltype=3,enchantskillid=3983,maxsameskillcol=1}
		TalismanEnchantSkillCid[40002]={prof=9,enchantskilltype=3,enchantskillid=3984,maxsameskillcol=1}
		TalismanEnchantSkillCid[40003]={prof=9,enchantskilltype=3,enchantskillid=3985,maxsameskillcol=1}
		TalismanEnchantSkillCid[40004]={prof=9,enchantskilltype=3,enchantskillid=3986,maxsameskillcol=1}
		TalismanEnchantSkillCid[40005]={prof=9,enchantskilltype=3,enchantskillid=3987,maxsameskillcol=1}
		TalismanEnchantSkillCid[40006]={prof=9,enchantskilltype=3,enchantskillid=3988,maxsameskillcol=1}
		TalismanEnchantSkillCid[40007]={prof=9,enchantskilltype=3,enchantskillid=3989,maxsameskillcol=1}
		TalismanEnchantSkillCid[40008]={prof=9,enchantskilltype=3,enchantskillid=3990,maxsameskillcol=1}
		TalismanEnchantSkillCid[40009]={prof=9,enchantskilltype=3,enchantskillid=3991,maxsameskillcol=1}
		TalismanEnchantSkillCid[40010]={prof=9,enchantskilltype=3,enchantskillid=3992,maxsameskillcol=1}
		TalismanEnchantSkillCid[40011]={prof=9,enchantskilltype=3,enchantskillid=3993,maxsameskillcol=1}
		TalismanEnchantSkillCid[40012]={prof=9,enchantskilltype=3,enchantskillid=3994,maxsameskillcol=1}
		TalismanEnchantSkillCid[40013]={prof=9,enchantskilltype=3,enchantskillid=3995,maxsameskillcol=1}

		TalismanEnchantSkillCid[40014]={prof=10,enchantskilltype=3,enchantskillid=3996,maxsameskillcol=1}
		TalismanEnchantSkillCid[40015]={prof=10,enchantskilltype=3,enchantskillid=3997,maxsameskillcol=1}
		TalismanEnchantSkillCid[40016]={prof=10,enchantskilltype=3,enchantskillid=3998,maxsameskillcol=1}
		TalismanEnchantSkillCid[40017]={prof=10,enchantskilltype=3,enchantskillid=3999,maxsameskillcol=1}
		TalismanEnchantSkillCid[40018]={prof=10,enchantskilltype=3,enchantskillid=4000,maxsameskillcol=1}
		TalismanEnchantSkillCid[40019]={prof=10,enchantskilltype=3,enchantskillid=4001,maxsameskillcol=1}
		TalismanEnchantSkillCid[40020]={prof=10,enchantskilltype=3,enchantskillid=4002,maxsameskillcol=1}
		TalismanEnchantSkillCid[40021]={prof=10,enchantskilltype=3,enchantskillid=4003,maxsameskillcol=1}
		TalismanEnchantSkillCid[40022]={prof=10,enchantskilltype=3,enchantskillid=4004,maxsameskillcol=1}
		TalismanEnchantSkillCid[40023]={prof=10,enchantskilltype=3,enchantskillid=4005,maxsameskillcol=1}
		TalismanEnchantSkillCid[40024]={prof=10,enchantskilltype=3,enchantskillid=4006,maxsameskillcol=1}
		TalismanEnchantSkillCid[40025]={prof=10,enchantskilltype=3,enchantskillid=4007,maxsameskillcol=1}
		TalismanEnchantSkillCid[40026]={prof=10,enchantskilltype=3,enchantskillid=4008,maxsameskillcol=1}
		TalismanEnchantSkillCid[40027]={prof=10,enchantskilltype=3,enchantskillid=4009,maxsameskillcol=1}
		TalismanEnchantSkillCid[40028]={prof=10,enchantskilltype=3,enchantskillid=4010,maxsameskillcol=1}
		TalismanEnchantSkillCid[40029]={prof=10,enchantskilltype=3,enchantskillid=4011,maxsameskillcol=1}
		TalismanEnchantSkillCid[40030]={prof=10,enchantskilltype=3,enchantskillid=4012,maxsameskillcol=1}
		TalismanEnchantSkillCid[40031]={prof=10,enchantskilltype=3,enchantskillid=4013,maxsameskillcol=1}
		TalismanEnchantSkillCid[40032]={prof=10,enchantskilltype=3,enchantskillid=4014,maxsameskillcol=1}
		TalismanEnchantSkillCid[40033]={prof=10,enchantskilltype=3,enchantskillid=4015,maxsameskillcol=1}
		TalismanEnchantSkillCid[40034]={prof=10,enchantskilltype=3,enchantskillid=4016,maxsameskillcol=1}
		TalismanEnchantSkillCid[40035]={prof=10,enchantskilltype=3,enchantskillid=4017,maxsameskillcol=1}
		TalismanEnchantSkillCid[40036]={prof=10,enchantskilltype=3,enchantskillid=4018,maxsameskillcol=1}
		TalismanEnchantSkillCid[40037]={prof=10,enchantskilltype=3,enchantskillid=4019,maxsameskillcol=1}

		TalismanEnchantSkillCid[49543]={prof=11,enchantskilltype=3,enchantskillid=33411,maxsameskillcol=1}
		TalismanEnchantSkillCid[49544]={prof=11,enchantskilltype=3,enchantskillid=33412,maxsameskillcol=1}
		TalismanEnchantSkillCid[49545]={prof=11,enchantskilltype=3,enchantskillid=33413,maxsameskillcol=1}
		TalismanEnchantSkillCid[49546]={prof=11,enchantskilltype=3,enchantskillid=33414,maxsameskillcol=1}
		TalismanEnchantSkillCid[49547]={prof=11,enchantskilltype=3,enchantskillid=33415,maxsameskillcol=1}
		TalismanEnchantSkillCid[49548]={prof=11,enchantskilltype=3,enchantskillid=33416,maxsameskillcol=1}
		TalismanEnchantSkillCid[49549]={prof=11,enchantskilltype=3,enchantskillid=33417,maxsameskillcol=1}
		TalismanEnchantSkillCid[49550]={prof=11,enchantskilltype=3,enchantskillid=33418,maxsameskillcol=1}
		TalismanEnchantSkillCid[49551]={prof=11,enchantskilltype=3,enchantskillid=33419,maxsameskillcol=1}
		TalismanEnchantSkillCid[49552]={prof=11,enchantskilltype=3,enchantskillid=33420,maxsameskillcol=1}
		TalismanEnchantSkillCid[49553]={prof=11,enchantskilltype=3,enchantskillid=33421,maxsameskillcol=1}
		TalismanEnchantSkillCid[49554]={prof=11,enchantskilltype=3,enchantskillid=33422,maxsameskillcol=1}
		TalismanEnchantSkillCid[49555]={prof=11,enchantskilltype=3,enchantskillid=33423,maxsameskillcol=1}
		TalismanEnchantSkillCid[49556]={prof=11,enchantskilltype=3,enchantskillid=33424,maxsameskillcol=1}
		TalismanEnchantSkillCid[49557]={prof=11,enchantskilltype=3,enchantskillid=33425,maxsameskillcol=1}
		TalismanEnchantSkillCid[49558]={prof=11,enchantskilltype=3,enchantskillid=33426,maxsameskillcol=1}
		TalismanEnchantSkillCid[49559]={prof=11,enchantskilltype=3,enchantskillid=33427,maxsameskillcol=1}
		TalismanEnchantSkillCid[49560]={prof=11,enchantskilltype=3,enchantskillid=33428,maxsameskillcol=1}
		TalismanEnchantSkillCid[49561]={prof=11,enchantskilltype=3,enchantskillid=33429,maxsameskillcol=1}
		TalismanEnchantSkillCid[49562]={prof=11,enchantskilltype=3,enchantskillid=33430,maxsameskillcol=1}
		TalismanEnchantSkillCid[49563]={prof=11,enchantskilltype=3,enchantskillid=33431,maxsameskillcol=1}
		TalismanEnchantSkillCid[49564]={prof=11,enchantskilltype=3,enchantskillid=33432,maxsameskillcol=1}
		TalismanEnchantSkillCid[49565]={prof=11,enchantskilltype=3,enchantskillid=33433,maxsameskillcol=1}
		TalismanEnchantSkillCid[49566]={prof=11,enchantskilltype=3,enchantskillid=33434,maxsameskillcol=1}
		
		TalismanEnchantSkillCid[53646]={prof=12,enchantskilltype=3,enchantskillid=34035,maxsameskillcol=1}
		TalismanEnchantSkillCid[53647]={prof=12,enchantskilltype=3,enchantskillid=34036,maxsameskillcol=1}
		TalismanEnchantSkillCid[53648]={prof=12,enchantskilltype=3,enchantskillid=34037,maxsameskillcol=1}
		TalismanEnchantSkillCid[53649]={prof=12,enchantskilltype=3,enchantskillid=34038,maxsameskillcol=1}
		TalismanEnchantSkillCid[53650]={prof=12,enchantskilltype=3,enchantskillid=34039,maxsameskillcol=1}
		TalismanEnchantSkillCid[53651]={prof=12,enchantskilltype=3,enchantskillid=34040,maxsameskillcol=1}
		TalismanEnchantSkillCid[53652]={prof=12,enchantskilltype=3,enchantskillid=34041,maxsameskillcol=1}
		TalismanEnchantSkillCid[53653]={prof=12,enchantskilltype=3,enchantskillid=34042,maxsameskillcol=1}
		TalismanEnchantSkillCid[53654]={prof=12,enchantskilltype=3,enchantskillid=34043,maxsameskillcol=1}
		TalismanEnchantSkillCid[53655]={prof=12,enchantskilltype=3,enchantskillid=34044,maxsameskillcol=1}
		TalismanEnchantSkillCid[53656]={prof=12,enchantskilltype=3,enchantskillid=34045,maxsameskillcol=1}
		TalismanEnchantSkillCid[53657]={prof=12,enchantskilltype=3,enchantskillid=34046,maxsameskillcol=1}
		TalismanEnchantSkillCid[53658]={prof=12,enchantskilltype=3,enchantskillid=34047,maxsameskillcol=1}
		TalismanEnchantSkillCid[53659]={prof=12,enchantskilltype=3,enchantskillid=34048,maxsameskillcol=1}
		TalismanEnchantSkillCid[53660]={prof=12,enchantskilltype=3,enchantskillid=34049,maxsameskillcol=1}
		TalismanEnchantSkillCid[53661]={prof=12,enchantskilltype=3,enchantskillid=34050,maxsameskillcol=1}
		TalismanEnchantSkillCid[53662]={prof=12,enchantskilltype=3,enchantskillid=34051,maxsameskillcol=1}
		TalismanEnchantSkillCid[53663]={prof=12,enchantskilltype=3,enchantskillid=34052,maxsameskillcol=1}
		TalismanEnchantSkillCid[53664]={prof=12,enchantskilltype=3,enchantskillid=34053,maxsameskillcol=1}
		TalismanEnchantSkillCid[53665]={prof=12,enchantskilltype=3,enchantskillid=34054,maxsameskillcol=1}
		TalismanEnchantSkillCid[53666]={prof=12,enchantskilltype=3,enchantskillid=34055,maxsameskillcol=1}
		TalismanEnchantSkillCid[53667]={prof=12,enchantskilltype=3,enchantskillid=34056,maxsameskillcol=1}
		TalismanEnchantSkillCid[53668]={prof=12,enchantskilltype=3,enchantskillid=34057,maxsameskillcol=1}
		TalismanEnchantSkillCid[53669]={prof=12,enchantskilltype=3,enchantskillid=34058,maxsameskillcol=1}

		TalismanEnchantSkillCid[60072]={prof=13,enchantskilltype=3,enchantskillid=34707,maxsameskillcol=1}
		TalismanEnchantSkillCid[60073]={prof=13,enchantskilltype=3,enchantskillid=34708,maxsameskillcol=1}
		TalismanEnchantSkillCid[60074]={prof=13,enchantskilltype=3,enchantskillid=34709,maxsameskillcol=1}
		TalismanEnchantSkillCid[60075]={prof=13,enchantskilltype=3,enchantskillid=34710,maxsameskillcol=1}
		TalismanEnchantSkillCid[60076]={prof=13,enchantskilltype=3,enchantskillid=34711,maxsameskillcol=1}
		TalismanEnchantSkillCid[60077]={prof=13,enchantskilltype=3,enchantskillid=34712,maxsameskillcol=1}
		TalismanEnchantSkillCid[60078]={prof=13,enchantskilltype=3,enchantskillid=34713,maxsameskillcol=1}
		TalismanEnchantSkillCid[60079]={prof=13,enchantskilltype=3,enchantskillid=34714,maxsameskillcol=1}
		TalismanEnchantSkillCid[60080]={prof=13,enchantskilltype=3,enchantskillid=34715,maxsameskillcol=1}
		TalismanEnchantSkillCid[60081]={prof=13,enchantskilltype=3,enchantskillid=34716,maxsameskillcol=1}
		TalismanEnchantSkillCid[60082]={prof=13,enchantskilltype=3,enchantskillid=34717,maxsameskillcol=1}
		TalismanEnchantSkillCid[60083]={prof=13,enchantskilltype=3,enchantskillid=34718,maxsameskillcol=1}
		TalismanEnchantSkillCid[60084]={prof=13,enchantskilltype=3,enchantskillid=34719,maxsameskillcol=1}
		TalismanEnchantSkillCid[60085]={prof=13,enchantskilltype=3,enchantskillid=34720,maxsameskillcol=1}
		TalismanEnchantSkillCid[60086]={prof=13,enchantskilltype=3,enchantskillid=34721,maxsameskillcol=1}
		TalismanEnchantSkillCid[60088]={prof=13,enchantskilltype=3,enchantskillid=34722,maxsameskillcol=1}
		TalismanEnchantSkillCid[60089]={prof=13,enchantskilltype=3,enchantskillid=34723,maxsameskillcol=1}
		TalismanEnchantSkillCid[60090]={prof=13,enchantskilltype=3,enchantskillid=34724,maxsameskillcol=1}
		TalismanEnchantSkillCid[60091]={prof=13,enchantskilltype=3,enchantskillid=34725,maxsameskillcol=1}
		TalismanEnchantSkillCid[60092]={prof=13,enchantskilltype=3,enchantskillid=34726,maxsameskillcol=1}
		TalismanEnchantSkillCid[60093]={prof=13,enchantskilltype=3,enchantskillid=34727,maxsameskillcol=1}
		TalismanEnchantSkillCid[60094]={prof=13,enchantskilltype=3,enchantskillid=34728,maxsameskillcol=1}
		TalismanEnchantSkillCid[60095]={prof=13,enchantskilltype=3,enchantskillid=34729,maxsameskillcol=1}
		TalismanEnchantSkillCid[60096]={prof=13,enchantskilltype=3,enchantskillid=34730,maxsameskillcol=1}

		TalismanEnchantSkillCid[60097]={prof=14,enchantskilltype=3,enchantskillid=34731,maxsameskillcol=1}
		TalismanEnchantSkillCid[60098]={prof=14,enchantskilltype=3,enchantskillid=34732,maxsameskillcol=1}
		TalismanEnchantSkillCid[60099]={prof=14,enchantskilltype=3,enchantskillid=34733,maxsameskillcol=1}
		TalismanEnchantSkillCid[60100]={prof=14,enchantskilltype=3,enchantskillid=34734,maxsameskillcol=1}
		TalismanEnchantSkillCid[60101]={prof=14,enchantskilltype=3,enchantskillid=34735,maxsameskillcol=1}
		TalismanEnchantSkillCid[60102]={prof=14,enchantskilltype=3,enchantskillid=34736,maxsameskillcol=1}
		TalismanEnchantSkillCid[60104]={prof=14,enchantskilltype=3,enchantskillid=34737,maxsameskillcol=1}
		TalismanEnchantSkillCid[60105]={prof=14,enchantskilltype=3,enchantskillid=34738,maxsameskillcol=1}
		TalismanEnchantSkillCid[60106]={prof=14,enchantskilltype=3,enchantskillid=34739,maxsameskillcol=1}
		TalismanEnchantSkillCid[60107]={prof=14,enchantskilltype=3,enchantskillid=34740,maxsameskillcol=1}
		TalismanEnchantSkillCid[60108]={prof=14,enchantskilltype=3,enchantskillid=34741,maxsameskillcol=1}
		TalismanEnchantSkillCid[60109]={prof=14,enchantskilltype=3,enchantskillid=34742,maxsameskillcol=1}
		TalismanEnchantSkillCid[60110]={prof=14,enchantskilltype=3,enchantskillid=34743,maxsameskillcol=1}
		TalismanEnchantSkillCid[60111]={prof=14,enchantskilltype=3,enchantskillid=34744,maxsameskillcol=1}
		TalismanEnchantSkillCid[60113]={prof=14,enchantskilltype=3,enchantskillid=34745,maxsameskillcol=1}
		TalismanEnchantSkillCid[60114]={prof=14,enchantskilltype=3,enchantskillid=34746,maxsameskillcol=1}
		TalismanEnchantSkillCid[60115]={prof=14,enchantskilltype=3,enchantskillid=34747,maxsameskillcol=1}
		TalismanEnchantSkillCid[60116]={prof=14,enchantskilltype=3,enchantskillid=34748,maxsameskillcol=1}
		TalismanEnchantSkillCid[60117]={prof=14,enchantskilltype=3,enchantskillid=34749,maxsameskillcol=1}
		TalismanEnchantSkillCid[60118]={prof=14,enchantskilltype=3,enchantskillid=34750,maxsameskillcol=1}
		TalismanEnchantSkillCid[60119]={prof=14,enchantskilltype=3,enchantskillid=34751,maxsameskillcol=1}
		TalismanEnchantSkillCid[60120]={prof=14,enchantskilltype=3,enchantskillid=34752,maxsameskillcol=1}
		TalismanEnchantSkillCid[60121]={prof=14,enchantskilltype=3,enchantskillid=34753,maxsameskillcol=1}
		TalismanEnchantSkillCid[60122]={prof=14,enchantskilltype=3,enchantskillid=34754,maxsameskillcol=1}

		TalismanEnchantSkillCid[11767]={prof=99,enchantskilltype=4,enchantskillid=337,maxsameskillcol=1} ---//属性可见
		TalismanEnchantSkillCid[23883]={prof=99,enchantskilltype=4,enchantskillid=337,maxsameskillcol=1} ---//属性可见
		TalismanEnchantSkillCid[40472]={prof=99,enchantskilltype=4,enchantskillid=337,maxsameskillcol=1} ---//属性可见
	return TalismanEnchantSkillCid
end 	
-------------------------------------------------------------------------------------------------
---根据(天人合一类型灵媒)的ID和法宝级别『查找』某灵媒对应的天人合一附加属性id和附加属性类型
function ZLuaExistInTalismanEnchantEx(cid,level)
	if type(cid)~="number" or type(level)~="number" then
		return 0,0
	end
	if level<1 then level=1 end
	if level>35 then level=35 end	-- 30级以上法宝临时按30级处理
	local TalismanEnchantEx=ZLuaTalismanEnchantEx()
	for i,v in pairs(TalismanEnchantEx) do
		if v.cidid==cid and v.exlevel==level then 
			return i,v.bufftype  ---返回附加属性id和附加属性所属类型 
		end 
	end 
	return 0,0
end 
---根据(技能灌魔类型灵媒)的ID『查找』某灵媒对应的职业、技能灌魔类型和指向技能等信息
function ZLuaExistInTalismanEnchantSkillCid(cid)
	if type(cid)~="number" then return -1 end 
	local TalismanEnchantSkillCid=ZLuaTalismanEnchantSkillCid()
	for i,v in pairs(TalismanEnchantSkillCid) do
		if i==cid then 
			return v.prof,v.enchantskilltype,v.enchantskillid,v.maxsameskillcol
		end 
	end 
	return -1
end 

------------法宝灌魔主函数--------------------------------------------------------------------------
function EnchantTalismans(id, talisman1, cid, output, locked)
	local dataIndex=ZLuaTalDataIndex()
	local talismans=ZLuaTalAvailableTalismans()

	-- 没有灵媒不允许灌魔
	if cid == 0 then
		output:CreateItem(-1)
		return
	end
	
	-- 没注册过的法宝飞剑，禁止灌魔
	local Registed = false
	for _,v in pairs(talismans) do
		if v.tmplID == id then Registed = true end
	end
	if Registed == false then
		output:CreateItem(-1)
		return
	end

	-- 如果法宝ID为飞宝ID，则灌魔失败
	if talisman1:QueryData(0,dataIndex["is_fly"])==1 then
		-- 苍穹之心可以改变飞剑品质
		if cid == 40272 then
			if ZLuaTalisman_ImproveFlyerQuality(id, talisman1, cid, output) == false then
				output:CreateItem(-1)
			end
		else
			output:CreateItem(-1)
		end
		return
	end

	-- 元宵灯不允许灌魔
	if id == talismans["US_45_YuanXiaoDeng"].tmplID then
		output:CreateItem(-1)
		return
	end

	-- 天人合一灌魔
	local Alevel = talisman1:QueryLevel()
	local exid,bufftype = ZLuaExistInTalismanEnchantEx(cid,Alevel)
	if exid ~= 0 then
		ZLuaTalisman_EnchantBotEx(id, talisman1, cid, output, exid, bufftype)
		return
	end

	-- 法宝档位上限提升灌魔
	if cid == 19512 or cid == 19513 or cid == 19514 or cid == 19515 
	or cid == 19516 or cid == 19517 or cid == 19518 or cid == 19519 or cid == 19520 then
		ZLuaTalisman_Rise(id, talisman1, cid, output)
		return
	end

	-- 法宝主属性二次强化灌魔
	if cid == 23860 or cid == 23861 or cid == 23862 or cid == 11784 or cid == 12021 or cid == 19716 then
		ZLuaTalisman_Rise2(id, talisman1, cid, output)
		return
	end

	-- 附加技能灌魔
	local tempprof,tempenchantskilltype,tempenchantskillid,tempmaxsameskillcol=ZLuaExistInTalismanEnchantSkillCid(cid)
	if tempprof~=-1 and locked == false then
		ZLuaTalisman_EnchantSkill(id, talisman1, cid, output,tempprof,tempenchantskilltype,tempenchantskillid,tempmaxsameskillcol)
		return
	end

	-- 其他	
	output:CreateItem(-1)
end

------------法宝天人合一灌魔主函数---------------------------------------------------------------------
function ZLuaTalisman_EnchantBotEx(id, talisman1, cid, output,exid, bufftype)
		if exid~=0 and bufftype~=0 then 			---//天人合一灌魔
			output:CreateItem(id)
			local dataIndex=ZLuaTalDataIndex()
			local nowbufftype
			nowbufftype=talisman1:QueryData(0,dataIndex["buff_type"])
			if nowbufftype==bufftype then 
					output:CreateItem(-1)
			else 
				for _,index in pairs(dataIndex) do
					output:UpdateData(0,index,talisman1:QueryData(0,index))
				end
				output:UpdateData(0,dataIndex["buff_type"],bufftype)
				output:UpdateData(0,dataIndex["ext_id"],exid)
				output:InheritCombineExp(true)
				ZLuaTalRebuild(output,id)
			end 
		else
				output:CreateItem(-1)
		end 
end 

----法宝特殊新道具洗炼接口
function ZLuaTalSkillRandomResult(id,talisman,result,output)
	local dataIndex=ZLuaTalDataIndex()
	local talismans=ZLuaTalAvailableTalismans()

	if talisman:QueryData(0,dataIndex["skill_addon_id_temp"]) == 0 then
		output:CreateItem(-1)
		return
	end

	output:CreateItem(id)
	for _,index in pairs(dataIndex) do
		output:UpdateData(0,index,talisman:QueryData(0,index))
	end

	if result == 0 then			
		output:UpdateData(0,dataIndex["skill_addon_id_temp"],0)
		output:UpdateData(0,dataIndex["addon_1_id_temp"],0)
		output:UpdateData(0,dataIndex["addon_2_id_temp"],0)
		output:UpdateData(0,dataIndex["addon_3_id_temp"],0)
		output:UpdateData(0,dataIndex["addon_4_id_temp"],0)
		output:UpdateData(0,dataIndex["addon_5_id_temp"],0)
		output:UpdateData(0,dataIndex["addon_6_id_temp"],0)
		output:UpdateData(0,dataIndex["addon_1_id_learn_level_temp"],0)
		output:UpdateData(0,dataIndex["addon_2_id_learn_level_temp"],0)
		output:UpdateData(0,dataIndex["addon_3_id_learn_level_temp"],0)
		output:UpdateData(0,dataIndex["addon_4_id_learn_level_temp"],0)
		output:UpdateData(0,dataIndex["addon_5_id_learn_level_temp"],0)
		output:UpdateData(0,dataIndex["addon_6_id_learn_level_temp"],0)
	elseif result == 1 then
		output:UpdateData(0,dataIndex["skill_addon_id"],talisman:QueryData(0,dataIndex["skill_addon_id_temp"]))
		output:UpdateData(0,dataIndex["addon_1_id"],talisman:QueryData(0,dataIndex["addon_1_id_temp"]))
		output:UpdateData(0,dataIndex["addon_2_id"],talisman:QueryData(0,dataIndex["addon_2_id_temp"]))
		output:UpdateData(0,dataIndex["addon_3_id"],talisman:QueryData(0,dataIndex["addon_3_id_temp"]))
		output:UpdateData(0,dataIndex["addon_4_id"],talisman:QueryData(0,dataIndex["addon_4_id_temp"]))
		output:UpdateData(0,dataIndex["addon_5_id"],talisman:QueryData(0,dataIndex["addon_5_id_temp"]))
		output:UpdateData(0,dataIndex["addon_6_id"],talisman:QueryData(0,dataIndex["addon_6_id_temp"]))
		output:UpdateData(0,dataIndex["addon_1_learn_level"],talisman:QueryData(0,dataIndex["addon_1_id_learn_level_temp"]))
		output:UpdateData(0,dataIndex["addon_2_learn_level"],talisman:QueryData(0,dataIndex["addon_2_id_learn_level_temp"]))
		output:UpdateData(0,dataIndex["addon_3_learn_level"],talisman:QueryData(0,dataIndex["addon_3_id_learn_level_temp"]))
		output:UpdateData(0,dataIndex["addon_4_learn_level"],talisman:QueryData(0,dataIndex["addon_4_id_learn_level_temp"]))
		output:UpdateData(0,dataIndex["addon_5_learn_level"],talisman:QueryData(0,dataIndex["addon_5_id_learn_level_temp"]))
		output:UpdateData(0,dataIndex["addon_6_learn_level"],talisman:QueryData(0,dataIndex["addon_6_id_learn_level_temp"]))
		output:UpdateData(0,dataIndex["skill_addon_id_temp"],0)
		output:UpdateData(0,dataIndex["addon_1_id_temp"],0)
		output:UpdateData(0,dataIndex["addon_2_id_temp"],0)
		output:UpdateData(0,dataIndex["addon_3_id_temp"],0)
		output:UpdateData(0,dataIndex["addon_4_id_temp"],0)
		output:UpdateData(0,dataIndex["addon_5_id_temp"],0)
		output:UpdateData(0,dataIndex["addon_6_id_temp"],0)
		output:UpdateData(0,dataIndex["addon_1_id_learn_level_temp"],0)
		output:UpdateData(0,dataIndex["addon_2_id_learn_level_temp"],0)
		output:UpdateData(0,dataIndex["addon_3_id_learn_level_temp"],0)
		output:UpdateData(0,dataIndex["addon_4_id_learn_level_temp"],0)
		output:UpdateData(0,dataIndex["addon_5_id_learn_level_temp"],0)
		output:UpdateData(0,dataIndex["addon_6_id_learn_level_temp"],0)
	end
	output:InheritCombineExp(true)
	ZLuaTalRebuild(output,id)
end

----法宝特殊新道具洗炼接口

------------法宝附加技能灌魔主函数---------------------------------------------------------------------
function ZLuaTalisman_EnchantSkill(id, talisman1, cid, output, prof, enchantskilltype, enchantskillid, maxsameskillcol)
		
	local SkillNoSeq={}				
	local SkillReqLvlSeq={}
	local TalismanSkillID
	local TrueSkillNum=0
	---//获得法宝目前的技能信息(技能ID,技能需求级别,法宝技能ID,实际技能数目)
	local dataIndex=ZLuaTalDataIndex()
		SkillNoSeq[1]=talisman1:QueryData(0,dataIndex["addon_1_id"])
		SkillReqLvlSeq[1]=talisman1:QueryData(0,dataIndex["addon_1_learn_level"])
		SkillNoSeq[2]=talisman1:QueryData(0,dataIndex["addon_2_id"])
		SkillReqLvlSeq[2]=talisman1:QueryData(0,dataIndex["addon_2_learn_level"])
		SkillNoSeq[3]=talisman1:QueryData(0,dataIndex["addon_3_id"])
		SkillReqLvlSeq[3]=talisman1:QueryData(0,dataIndex["addon_3_learn_level"])
		SkillNoSeq[4]=talisman1:QueryData(0,dataIndex["addon_4_id"])
		SkillReqLvlSeq[4]=talisman1:QueryData(0,dataIndex["addon_4_learn_level"])
		SkillNoSeq[5]=talisman1:QueryData(0,dataIndex["addon_5_id"])
		SkillReqLvlSeq[5]=talisman1:QueryData(0,dataIndex["addon_5_learn_level"])
		SkillNoSeq[6]=talisman1:QueryData(0,dataIndex["addon_6_id"])
		SkillReqLvlSeq[6]=talisman1:QueryData(0,dataIndex["addon_6_learn_level"])
		
		TalismanSkillID=talisman1:QueryData(0,dataIndex["skill_addon_id"])
	---//获得实际的有效技能数量
	for j=1,6 do
		if SkillNoSeq[j]~=0 and SkillReqLvlSeq[j]~=0 and type(SkillReqLvlSeq[j])=="number" then 
			TrueSkillNum=TrueSkillNum+1
		end 
	end 
	
	local talismanprof,talismanskillprof -- 法宝本体的职业归属,法宝现携带技能职业归属
	talismanprof,talismanskillprof = ZLuaTalisman_GetProfInfo(id,SkillNoSeq)
	---//根据法宝的id和法宝上已经存在的技能决定法宝的门派属相
	local FailtoCreatTalisman=0
	local SkillHighLvlVisible=talisman1:QueryData(0,dataIndex["skill_visible"])
	---附加人物技能在未达到级别时是否可见
			
	if enchantskilltype==0 then
---//local skillnumchanceseq={0,200,900,1200,600,100}
---//旧太一1-6技能概率
		local skillnumchanceseq={0,200,900,1200,600,100}
		if cid == 23882 then skillnumchanceseq={0,200,900,1290,600,10} end
		---//礼包太一1-6技能调整概率
		if cid == 62752 then skillnumchanceseq={0,400,900,1215,400,85} end
		---//新太一1-6技能概率
		if cid == 62754 then skillnumchanceseq={975,975,975,70,4,1} end
		---//单独洗格数道具概率
		local HowManySkills
		if cid == 62753 then HowManySkills=2 else
			HowManySkills=ZLuaTal_Get_RanPoint(skillnumchanceseq)
		end
		---//选取了哪些附加技能
		SkillNoSeq,SkillReqLvlSeq=ZLuaTalisman_ChooseEnchantSkill(talismanskillprof,HowManySkills,maxsameskillcol)	
		---//选取了哪个级别的法宝技能
		if cid ~= 62754 then 
			TalismanSkillID=ZLuaTalisman_ChooseTalismanSkill(TalismanSkillID,cid)
		end
		if TalismanSkillID == -1 then 
			FailtoCreatTalisman=1
			output:CreateItem(-1)
		end
	---//elseif enchantskilltype==1 then 
	---//		SkillNoSeq,SkillReqLvlSeq=ZLuaTalisman_ChooseEnchantSkill(talismanprof,TrueSkillNum,maxsameskillcol)	
	---//功能与enchantskilltype==2合并
	elseif enchantskilltype==2 then 
		if talismanskillprof==prof 
		or talismanprof==99 
		or (talismanprof==21 and (prof==1 or prof==2 or prof==3 or prof==4 or prof==5 or prof==6))	--添加新职业需修改这行
		or (talismanprof==22 and (prof==7 or prof==8 or prof==9 or prof==10 or prof==11 or prof==12))						--添加新职业需修改这行
		or (talismanprof==33 and (prof==13 or prof==14))						--添加新职业需修改这行
		then 
			SkillNoSeq,SkillReqLvlSeq=ZLuaTalisman_ChooseEnchantSkill(prof,TrueSkillNum,maxsameskillcol)
		else 
			FailtoCreatTalisman=1
			output:CreateItem(-1)
		end 
	elseif (enchantskilltype==3 and prof==talismanskillprof)
			or (enchantskilltype==3 and prof==21 and talismanprof~=99 and talismanprof~=22 and talismanprof~=33 and talismanprof~=7 and talismanprof~=8 and talismanprof~=9 and talismanprof~=10 and talismanprof~=11 and talismanprof~=12 and talismanprof~=13 and talismanprof~=14)	--添加新职业需修改这行
			or (enchantskilltype==3 and prof==21 and talismanprof==99 and talismanskillprof~=7 and talismanskillprof~=8 and talismanskillprof~=9 and talismanskillprof~=10 and talismanskillprof~=11 and talismanskillprof~=12 and talismanskillprof~=13 and talismanskillprof~=14)		--添加新职业需修改这行
			or (enchantskilltype==3 and talismanprof==99 and talismanskillprof==99)
			then 
			SkillNoSeq,SkillReqLvlSeq=ZLuaTalisman_InsertSkillSeq(enchantskillid,SkillNoSeq,SkillReqLvlSeq,talismanskillprof)
		---根据需要插入的法宝技能和法宝目前的技能序列决定结果，返回新的SkillNoSeq,SkillReqLvlSeq
	elseif enchantskilltype==4 and SkillHighLvlVisible~=1 then
			SkillHighLvlVisible=1
	else
			FailtoCreatTalisman=1
			output:CreateItem(-1)
	end 
	
	---因为同类技能数目达到上限导致的灌魔失败
	if type(SkillNoSeq)==nil or type(SkillReqLvlSeq)==nil then 
			FailtoCreatTalisman=1
			output:CreateItem(-1)
	end 
		
	---//更新新的附加人物技能属性信息	
		if 	FailtoCreatTalisman==0 then 
			output:CreateItem(id)
			for _,index in pairs(dataIndex) do
				output:UpdateData(0,index,talisman1:QueryData(0,index))
			end
			---缓存---
			if cid == 62752 or cid == 62754 then 
  			output:UpdateData(0,dataIndex["addon_1_id_temp"],SkillNoSeq[1]or 0)		
  			output:UpdateData(0,dataIndex["addon_1_id_learn_level_temp"],SkillReqLvlSeq[1]or 0)
  			output:UpdateData(0,dataIndex["addon_2_id_temp"],SkillNoSeq[2]or 0)		
  			output:UpdateData(0,dataIndex["addon_2_id_learn_level_temp"],SkillReqLvlSeq[2]or 0)
  			output:UpdateData(0,dataIndex["addon_3_id_temp"],SkillNoSeq[3]or 0)		
  			output:UpdateData(0,dataIndex["addon_3_id_learn_level_temp"],SkillReqLvlSeq[3]or 0)
  			output:UpdateData(0,dataIndex["addon_4_id_temp"],SkillNoSeq[4]or 0)		
  			output:UpdateData(0,dataIndex["addon_4_id_learn_level_temp"],SkillReqLvlSeq[4]or 0)
  			output:UpdateData(0,dataIndex["addon_5_id_temp"],SkillNoSeq[5]or 0)		
  			output:UpdateData(0,dataIndex["addon_5_id_learn_level_temp"],SkillReqLvlSeq[5]or 0)
  			output:UpdateData(0,dataIndex["addon_6_id_temp"],SkillNoSeq[6]or 0)		
  			output:UpdateData(0,dataIndex["addon_6_id_learn_level_temp"],SkillReqLvlSeq[6]or 0)
  			output:UpdateData(0,dataIndex["skill_addon_id_temp"],TalismanSkillID)
			---缓存---
			else
  			output:UpdateData(0,dataIndex["addon_1_id"],SkillNoSeq[1]or 0)		
  			output:UpdateData(0,dataIndex["addon_1_learn_level"],SkillReqLvlSeq[1]or 0)
  			output:UpdateData(0,dataIndex["addon_2_id"],SkillNoSeq[2]or 0)		
  			output:UpdateData(0,dataIndex["addon_2_learn_level"],SkillReqLvlSeq[2]or 0)
  			output:UpdateData(0,dataIndex["addon_3_id"],SkillNoSeq[3]or 0)		
  			output:UpdateData(0,dataIndex["addon_3_learn_level"],SkillReqLvlSeq[3]or 0)
  			output:UpdateData(0,dataIndex["addon_4_id"],SkillNoSeq[4]or 0)		
  			output:UpdateData(0,dataIndex["addon_4_learn_level"],SkillReqLvlSeq[4]or 0)
  			output:UpdateData(0,dataIndex["addon_5_id"],SkillNoSeq[5]or 0)		
  			output:UpdateData(0,dataIndex["addon_5_learn_level"],SkillReqLvlSeq[5]or 0)
  			output:UpdateData(0,dataIndex["addon_6_id"],SkillNoSeq[6]or 0)		
  			output:UpdateData(0,dataIndex["addon_6_learn_level"],SkillReqLvlSeq[6]or 0)
  			---//更新新的法宝技能属性信息		
  			output:UpdateData(0,dataIndex["skill_addon_id"],TalismanSkillID)
			end
		---//设定法宝的所有附加人物技能可见
			if SkillHighLvlVisible==1 then 
				output:UpdateData(0,dataIndex["skill_visible"],SkillHighLvlVisible)
			end 
		---//设定灌魔系列参数	 
			if enchantskilltype==0 then 
				output:UpdateData(0,dataIndex["change_skillnum"],output:QueryData(0,dataIndex["change_skillnum"])+1)			
			elseif enchantskilltype==2 or enchantskilltype==3 then 
				output:UpdateData(0,dataIndex["enchant_times"],output:QueryData(0,dataIndex["enchant_times"])+1)		
			end 
			output:InheritCombineExp(true)
			ZLuaTalRebuild(output,id)
	end 
	
end 
-------用于法宝levelup时处理天人合一类型灌魔的函数-------------------------------------------------------------
function ZLuaTalChangeExt(talisman)
	local Alevel=talisman:QueryLevel()
	local dataIndex=ZLuaTalDataIndex()
	local ABuffType=talisman:QueryData(0,dataIndex["buff_type"])
	
	local TalismanEnchantEx=ZLuaTalismanEnchantEx()
	local exid
	if Alevel > 30 then Alevel = 30 end	-- 30级以上法宝临时按30级处理
	for i,v in pairs(TalismanEnchantEx) do
		if v.bufftype==ABuffType and v.exlevel==Alevel then 
			 exid=i  ---返回附加属性id 
			 talisman:UpdateData(0,dataIndex["ext_id"],exid)
		else 
		end 
	end 
end 
------------------------------------------------------------------------------------------------------------
-----------描述技能多次叠加/保持/替代的概率参数(技能灌魔)---------------------------------------------------
function ZLuaTalismanSkillsExInfo(skillid,nowaddtimes)
	local SkillsExInfo={}
		---//[附加属性ID]
		---//prof
		---//叠加1次概率参数－6次概率参数
		SkillsExInfo[337]={9,100,0,0,100,100,100,100,100,100}---//技能:重击
		SkillsExInfo[338]={9,100,0,0,100,100,100,100,100,100}---//技能:疗伤

		SkillsExInfo[339]={1,100,0,0,100,100,100,100,100,100}---//技能:追风咒
		SkillsExInfo[340]={1,100,0,0,100,100,100,100,100,100}---//技能:疯魔斩
		SkillsExInfo[341]={1,100,0,0,100,100,100,100,100,100}---//技能:火焰刀
		SkillsExInfo[342]={1,100,0,0,100,100,100,100,100,100}---//技能:寒冰刃
		SkillsExInfo[343]={1,100,0,0,100,100,100,100,100,100}---//技能:逆水咒
		SkillsExInfo[344]={1,100,0,0,100,100,100,100,100,100}---//技能:失魂斩
		SkillsExInfo[345]={1,100,0,0,100,100,100,100,100,100}---//技能:迅雷斩
		SkillsExInfo[346]={1,100,0,0,100,100,100,100,100,100}---//技能:焚心斩
		SkillsExInfo[347]={1,100,0,0,100,100,100,100,100,100}---//技能:无妄斩
		SkillsExInfo[348]={1,100,0,0,100,100,100,100,100,100}---//技能:猛火咒
		SkillsExInfo[349]={1,100,20,12,100,100,100,100,100,100}---//技能:噬血斩
		SkillsExInfo[350]={1,100,20,12,100,100,100,100,100,100}---//技能:天魔舞
		SkillsExInfo[351]={1,100,20,12,100,100,100,100,100,100}---//技能:夺神斩
		SkillsExInfo[352]={1,100,20,12,100,100,100,100,100,100}---//技能:修罗斩
		SkillsExInfo[353]={1,100,20,12,100,100,100,100,100,100}---//技能:痴情咒
		SkillsExInfo[872]={1,100,100,43,800,800,800,800,800,800}---//技能:霸天戮杀
		SkillsExInfo[873]={1,100,100,43,800,800,800,800,800,800}---//技能:聚灵咒
		SkillsExInfo[874]={1,100,100,43,800,800,800,800,800,800}---//技能:狱龙破
		SkillsExInfo[875]={1,100,100,43,800,800,800,800,800,800}---//技能:旋龙幻杀
		SkillsExInfo[876]={1,100,100,43,800,800,800,800,800,800}---//技能:苍劫阵
		SkillsExInfo[877]={1,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:魔魂天咒
		SkillsExInfo[878]={1,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:狂灵反噬
		SkillsExInfo[900]={1,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:毁天灭地
		SkillsExInfo[901]={1,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:未名斩

		SkillsExInfo[354]={2,100,0,0,100,100,100,100,100,100}---//技能:断水
		SkillsExInfo[355]={2,100,0,0,100,100,100,100,100,100}---//技能:情网
		SkillsExInfo[356]={2,100,0,0,100,100,100,100,100,100}---//技能:烟雨
		SkillsExInfo[357]={2,100,0,0,100,100,100,100,100,100}---//技能:热血
		SkillsExInfo[358]={2,100,0,0,100,100,100,100,100,100}---//技能:破云
		SkillsExInfo[359]={2,100,0,0,100,100,100,100,100,100}---//技能:飞花
		SkillsExInfo[360]={2,100,0,0,100,100,100,100,100,100}---//技能:暗香
		SkillsExInfo[361]={2,100,0,0,100,100,100,100,100,100}---//技能:迷离
		SkillsExInfo[362]={2,100,0,0,100,100,100,100,100,100}---//技能:销魂
		SkillsExInfo[363]={2,100,0,0,100,100,100,100,100,100}---//技能:相思
		SkillsExInfo[364]={2,100,20,12,100,100,100,100,100,100}---//技能:温柔
		SkillsExInfo[365]={2,100,20,12,100,100,100,100,100,100}---//技能:缠绵
		SkillsExInfo[366]={2,100,20,12,100,100,100,100,100,100}---//技能:倾城
		SkillsExInfo[367]={2,100,20,12,100,100,100,100,100,100}---//技能:蝶舞
		SkillsExInfo[368]={2,100,20,12,100,100,100,100,100,100}---//技能:山盟
		SkillsExInfo[879]={2,100,100,43,800,800,800,800,800,800}---//技能:碧影
		SkillsExInfo[880]={2,100,100,43,800,800,800,800,800,800}---//技能:落霞
		SkillsExInfo[881]={2,100,100,43,800,800,800,800,800,800}---//技能:云雨
		SkillsExInfo[882]={2,100,100,43,800,800,800,800,800,800}---//技能:江南
		SkillsExInfo[883]={2,100,100,43,800,800,800,800,800,800}---//技能:忘情
		SkillsExInfo[884]={2,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:旧梦
		SkillsExInfo[885]={2,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:惜春
		SkillsExInfo[902]={2,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:怜影
		SkillsExInfo[903]={2,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:风花雪月

		SkillsExInfo[369]={3,100,0,0,100,100,100,100,100,100}---//技能:御剑诀
		SkillsExInfo[370]={3,100,0,0,100,100,100,100,100,100}---//技能:寒冰咒
		SkillsExInfo[371]={3,100,0,0,100,100,100,100,100,100}---//技能:驭雷术
		SkillsExInfo[372]={3,100,0,0,100,100,100,100,100,100}---//技能:寒霜剑气
		SkillsExInfo[373]={3,100,0,0,100,100,100,100,100,100}---//技能:归元剑气
		SkillsExInfo[374]={3,100,0,0,100,100,100,100,100,100}---//技能:破魔剑气
		SkillsExInfo[375]={3,100,0,0,100,100,100,100,100,100}---//技能:玄冰刺
		SkillsExInfo[376]={3,100,0,0,100,100,100,100,100,100}---//技能:雷云风暴
		SkillsExInfo[377]={3,100,0,0,100,100,100,100,100,100}---//技能:少阳剑气
		SkillsExInfo[378]={3,100,0,0,100,100,100,100,100,100}---//技能:炼气还神
		SkillsExInfo[379]={3,100,20,12,100,100,100,100,100,100}---//技能:怒剑狂花
		SkillsExInfo[380]={3,100,20,12,100,100,100,100,100,100}---//技能:五雷轰顶
		SkillsExInfo[381]={3,100,20,12,100,100,100,100,100,100}---//技能:天外飞仙
		SkillsExInfo[382]={3,100,20,12,100,100,100,100,100,100}---//技能:雷神之锥
		SkillsExInfo[383]={3,100,20,12,100,100,100,100,100,100}---//技能:雷霆震怒
		SkillsExInfo[886]={3,100,100,43,800,800,800,800,800,800}---//技能:霜天雪舞
		SkillsExInfo[887]={3,100,100,43,800,800,800,800,800,800}---//技能:天玄冰
		SkillsExInfo[888]={3,100,100,43,800,800,800,800,800,800}---//技能:天诛剑气
		SkillsExInfo[889]={3,100,100,43,800,800,800,800,800,800}---//技能:天地不仁
		SkillsExInfo[890]={3,100,100,43,800,800,800,800,800,800}---//技能:天机印
		SkillsExInfo[891]={3,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:雷光遁龙诀
		SkillsExInfo[892]={3,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:天尊法身
		SkillsExInfo[904]={3,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:太极玄天真诀
		SkillsExInfo[905]={3,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:神剑御雷真诀

		SkillsExInfo[384]={4,100,0,0,100,100,100,100,100,100}---//技能:大悲咒
		SkillsExInfo[385]={4,100,0,0,100,100,100,100,100,100}---//技能:降魔杖
		SkillsExInfo[386]={4,100,0,0,100,100,100,100,100,100}---//技能:狮子吼
		SkillsExInfo[387]={4,100,0,0,100,100,100,100,100,100}---//技能:诵经
		SkillsExInfo[388]={4,100,0,0,100,100,100,100,100,100}---//技能:易筋经
		SkillsExInfo[389]={4,100,0,0,100,100,100,100,100,100}---//技能:宏愿
		SkillsExInfo[390]={4,100,0,0,100,100,100,100,100,100}---//技能:般若经
		SkillsExInfo[391]={4,100,0,0,100,100,100,100,100,100}---//技能:金刚经
		SkillsExInfo[392]={4,100,0,0,100,100,100,100,100,100}---//技能:苦海无边
		SkillsExInfo[393]={4,100,0,0,100,100,100,100,100,100}---//技能:四大皆空:火
		SkillsExInfo[394]={4,100,20,12,100,100,100,100,100,100}---//技能:四大皆空:风
		SkillsExInfo[395]={4,100,20,12,100,100,100,100,100,100}---//技能:同归极乐
		SkillsExInfo[396]={4,100,20,12,100,100,100,100,100,100}---//技能:佛光普照
		SkillsExInfo[397]={4,100,20,12,100,100,100,100,100,100}---//技能:伏魔经
		SkillsExInfo[398]={4,100,20,12,100,100,100,100,100,100}---//技能:天地同寿
		SkillsExInfo[893]={4,100,100,43,800,800,800,800,800,800}---//技能:拈花妙谛
		SkillsExInfo[894]={4,100,100,43,800,800,800,800,800,800}---//技能:净土真言
		SkillsExInfo[895]={4,100,100,43,800,800,800,800,800,800}---//技能:因果循环
		SkillsExInfo[896]={4,100,100,43,800,800,800,800,800,800}---//技能:六道轮回
		SkillsExInfo[897]={4,100,100,43,800,800,800,800,800,800}---//技能:慈航法愿
		SkillsExInfo[898]={4,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:雷音法谕
		SkillsExInfo[899]={4,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:无量真言
		SkillsExInfo[906]={4,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:大凡般若
		SkillsExInfo[907]={4,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:万象生佛

		SkillsExInfo[1758]={5,100,0,0,100,100,100,100,100,100}---//技能:剔骨凶牙
		SkillsExInfo[1759]={5,100,0,0,100,100,100,100,100,100}---//技能:化骨毒砂
		SkillsExInfo[1760]={5,100,0,0,100,100,100,100,100,100}---//技能:幽魂利爪
		SkillsExInfo[1761]={5,100,0,0,100,100,100,100,100,100}---//技能:浸地毒芒
		SkillsExInfo[1762]={5,100,0,0,100,100,100,100,100,100}---//技能:噬魂血牙
		SkillsExInfo[1763]={5,100,0,0,100,100,100,100,100,100}---//技能:旋风乱舞
		SkillsExInfo[1764]={5,100,0,0,100,100,100,100,100,100}---//技能:蚀心毒蛊
		SkillsExInfo[1765]={5,100,0,0,100,100,100,100,100,100}---//技能:含沙射影
		SkillsExInfo[1766]={5,100,0,0,100,100,100,100,100,100}---//技能:凝血妖蛊
		SkillsExInfo[1767]={5,100,0,0,100,100,100,100,100,100}---//技能:破刃邪魂
		SkillsExInfo[1768]={5,100,20,12,100,100,100,100,100,100}---//技能:暴沙漫卷
		SkillsExInfo[1769]={5,100,20,12,100,100,100,100,100,100}---//技能:怨鬼心魔
		SkillsExInfo[1770]={5,100,20,12,100,100,100,100,100,100}---//技能:缚足蛇蛊
		SkillsExInfo[1771]={5,100,20,12,100,100,100,100,100,100}---//技能:炼骨地刺
		SkillsExInfo[1772]={5,100,20,12,100,100,100,100,100,100}---//技能:碎魂天袭
		SkillsExInfo[1773]={5,100,100,43,800,800,800,800,800,800}---//技能:幕天狂沙
		SkillsExInfo[1774]={5,100,100,43,800,800,800,800,800,800}---//技能:百蛊啖魂
		SkillsExInfo[1775]={5,100,100,43,800,800,800,800,800,800}---//技能:燃魂劫火
		SkillsExInfo[1776]={5,100,100,43,800,800,800,800,800,800}---//技能:毒龙蚀天
		SkillsExInfo[1777]={5,100,100,43,800,800,800,800,800,800}---//技能:森罗三笑
		SkillsExInfo[1778]={5,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:灭魂狂飙
		SkillsExInfo[1779]={5,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:邪神傲世
		SkillsExInfo[1780]={5,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:神鬼乱舞
		SkillsExInfo[1781]={5,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:天选道怨

		SkillsExInfo[33387]={6,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33388]={6,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33389]={6,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33390]={6,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33391]={6,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33392]={6,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33393]={6,100,0,0,100,100,100,100,100,100}---//技能
		SkillsExInfo[33394]={6,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33395]={6,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33396]={6,100,0,0,100,100,100,100,100,100}---//技能
		SkillsExInfo[33397]={6,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33398]={6,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33399]={6,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33400]={6,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33401]={6,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33402]={6,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[33403]={6,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[33404]={6,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[33405]={6,100,100,43,800,800,800,800,800,800}---//技能
		SkillsExInfo[33406]={6,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[33407]={6,100,400,100,1500,1500,1500,1500,1500,1500}---//技能
		SkillsExInfo[33408]={6,100,400,100,1500,1500,1500,1500,1500,1500}---//技能
		SkillsExInfo[33409]={6,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:
		SkillsExInfo[33410]={6,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:

		SkillsExInfo[3510]={7,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3511]={7,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3512]={7,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3513]={7,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3514]={7,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3515]={7,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3516]={7,100,0,0,100,100,100,100,100,100}---//技能
		SkillsExInfo[3517]={7,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3518]={7,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3519]={7,100,0,0,100,100,100,100,100,100}---//技能
		SkillsExInfo[3520]={7,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3521]={7,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3522]={7,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3523]={7,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3524]={7,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3525]={7,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[3526]={7,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[3527]={7,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[3528]={7,100,100,43,800,800,800,800,800,800}---//技能
		SkillsExInfo[3529]={7,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[3530]={7,100,400,100,1500,1500,1500,1500,1500,1500}---//技能
		SkillsExInfo[3531]={7,100,400,100,1500,1500,1500,1500,1500,1500}---//技能
		SkillsExInfo[3532]={7,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:
		SkillsExInfo[3533]={7,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:

		SkillsExInfo[3534]={8,100,0,0,100,100,100,100,100,100}---//技能
		SkillsExInfo[3535]={8,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3536]={8,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3537]={8,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3538]={8,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3539]={8,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3540]={8,100,0,0,100,100,100,100,100,100}---//技能
		SkillsExInfo[3541]={8,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3542]={8,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3543]={8,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3544]={8,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3545]={8,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3546]={8,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3547]={8,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3548]={8,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3549]={8,100,100,43,800,800,800,800,800,800}---//技能
		SkillsExInfo[3550]={8,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[3551]={8,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[3552]={8,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[3553]={8,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[3554]={8,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:
		SkillsExInfo[3555]={8,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:
		SkillsExInfo[3556]={8,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:
		SkillsExInfo[3557]={8,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:

		SkillsExInfo[3972]={9,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3973]={9,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3974]={9,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3975]={9,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3976]={9,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3977]={9,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3978]={9,100,0,0,100,100,100,100,100,100}---//技能
		SkillsExInfo[3979]={9,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3980]={9,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3981]={9,100,0,0,100,100,100,100,100,100}---//技能
		SkillsExInfo[3982]={9,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3983]={9,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3984]={9,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3985]={9,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3986]={9,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3987]={9,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[3988]={9,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[3989]={9,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[3990]={9,100,100,43,800,800,800,800,800,800}---//技能
		SkillsExInfo[3991]={9,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[3992]={9,100,400,100,1500,1500,1500,1500,1500,1500}---//技能
		SkillsExInfo[3993]={9,100,400,100,1500,1500,1500,1500,1500,1500}---//技能
		SkillsExInfo[3994]={9,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:
		SkillsExInfo[3995]={9,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:

		SkillsExInfo[3996]={10,100,0,0,100,100,100,100,100,100}---//技能
		SkillsExInfo[3997]={10,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3998]={10,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[3999]={10,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[4000]={10,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[4001]={10,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[4002]={10,100,0,0,100,100,100,100,100,100}---//技能
		SkillsExInfo[4003]={10,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[4004]={10,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[4005]={10,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[4006]={10,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[4007]={10,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[4008]={10,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[4009]={10,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[4010]={10,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[4011]={10,100,100,43,800,800,800,800,800,800}---//技能
		SkillsExInfo[4012]={10,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[4013]={10,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[4014]={10,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[4015]={10,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[4016]={10,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:
		SkillsExInfo[4017]={10,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:
		SkillsExInfo[4018]={10,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:
		SkillsExInfo[4019]={10,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:

		SkillsExInfo[33411]={11,100,0,0,100,100,100,100,100,100}---//技能
		SkillsExInfo[33412]={11,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33413]={11,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33414]={11,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33415]={11,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33416]={11,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33417]={11,100,0,0,100,100,100,100,100,100}---//技能
		SkillsExInfo[33418]={11,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33419]={11,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33420]={11,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33421]={11,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33422]={11,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33423]={11,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33424]={11,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33425]={11,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[33426]={11,100,100,43,800,800,800,800,800,800}---//技能
		SkillsExInfo[33427]={11,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[33428]={11,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[33429]={11,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[33430]={11,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[33431]={11,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:
		SkillsExInfo[33432]={11,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:
		SkillsExInfo[33433]={11,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:
		SkillsExInfo[33434]={11,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:

		SkillsExInfo[34035]={12,100,0,0,100,100,100,100,100,100}---//技能
		SkillsExInfo[34036]={12,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34037]={12,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34038]={12,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34039]={12,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34040]={12,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34041]={12,100,0,0,100,100,100,100,100,100}---//技能
		SkillsExInfo[34042]={12,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34043]={12,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34044]={12,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34045]={12,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34046]={12,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34047]={12,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34048]={12,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34049]={12,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34050]={12,100,100,43,800,800,800,800,800,800}---//技能
		SkillsExInfo[34051]={12,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[34052]={12,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[34053]={12,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[34054]={12,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[34055]={12,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:
		SkillsExInfo[34056]={12,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:
		SkillsExInfo[34057]={12,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:
		SkillsExInfo[34058]={12,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:

		SkillsExInfo[34707]={13,100,0,0,100,100,100,100,100,100}---//技能
		SkillsExInfo[34708]={13,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34709]={13,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34710]={13,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34711]={13,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34712]={13,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34713]={13,100,0,0,100,100,100,100,100,100}---//技能
		SkillsExInfo[34714]={13,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34715]={13,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34716]={13,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34717]={13,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34718]={13,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34719]={13,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34720]={13,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34721]={13,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34722]={13,100,100,43,800,800,800,800,800,800}---//技能
		SkillsExInfo[34723]={13,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[34724]={13,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[34725]={13,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[34726]={13,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[34727]={13,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:
		SkillsExInfo[34728]={13,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:
		SkillsExInfo[34729]={13,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:
		SkillsExInfo[34730]={13,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:

		SkillsExInfo[34731]={14,100,0,0,100,100,100,100,100,100}---//技能
		SkillsExInfo[34732]={14,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34733]={14,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34734]={14,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34735]={14,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34736]={14,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34737]={14,100,0,0,100,100,100,100,100,100}---//技能
		SkillsExInfo[34738]={14,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34739]={14,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34740]={14,100,0,0,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34741]={14,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34742]={14,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34743]={14,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34744]={14,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34745]={14,100,20,12,100,100,100,100,100,100}---//技能:
		SkillsExInfo[34746]={14,100,100,43,800,800,800,800,800,800}---//技能
		SkillsExInfo[34747]={14,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[34748]={14,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[34749]={14,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[34750]={14,100,100,43,800,800,800,800,800,800}---//技能:
		SkillsExInfo[34751]={14,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:
		SkillsExInfo[34752]={14,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:
		SkillsExInfo[34753]={14,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:
		SkillsExInfo[34754]={14,100,400,100,1500,1500,1500,1500,1500,1500}---//技能:

			---//{prof,addchancebase,failchancebase,add[1]--add[5]}
	local addchance=0
	local addchancecalc={}
	local addchancebase=0
	local failchancebase=0

	if nowaddtimes>=1 then 
		return 4
	else	 
		for i,v in pairs(SkillsExInfo) do
			if i==skillid then 
				---addchance=v[nowaddtimes+4]
				addchance=v[nowaddtimes+4]+v[3]
				addchancebase=v[2]
				---failchancebase=v[3]
				failchancebase=v[3]-v[3]
			end 
		end	
		addchancecalc={addchancebase,addchance,failchancebase}
		return ZLuaTal_Get_RanPoint(addchancecalc)
	end 
	---return 1代表添加成功 2代表失败但法宝不变 3代表失败且替换一个技能(从默认技能选取)
end 
-----------------根据职业选取一定数量(限定重复)的技能(技能灌魔)---------------------------------------------
function ZLuaTalisman_ChooseEnchantSkill(prof,num,maxsamecol)
	---prof,num,maxsamecol  职业,技能数量,允许的相同技能出现次数
	
	local SkillsExLists=ZLuaTalismanEnchantSkillEx()
	local CheckedSkills={}
	local CheckedSkillsBornChance={}
	local CheckedSkillsReqLvl={}
	local zchoice = {}
	local zrand
	
	if     prof==99 then
		zchoice = {1,2,3,4,5,6,7,8,9,10,11,12,13,14}	--添加新职业需修改这行
		zrand = math.ceil(math.random(table.getn(zchoice)))
		prof = zchoice[zrand]
	elseif prof==21 then
		zchoice = {1,2,3,4,5,6}	--添加新职业需修改这行
		zrand = math.ceil(math.random(table.getn(zchoice)))
		prof = zchoice[zrand]
	elseif prof==22 then
		zchoice = {7,8,9,10,11,12}	--添加新职业需修改这行
		zrand = math.ceil(math.random(table.getn(zchoice)))
		prof = zchoice[zrand]
	elseif prof==33 then
		zchoice = {13,14}	--添加新职业需修改这行
		zrand = math.ceil(math.random(table.getn(zchoice)))
		prof = zchoice[zrand]
	end
	
	for i,v in pairs(SkillsExLists) do
		if v.prof==prof or v.prof==0 then 
			table.insert(CheckedSkills,i)
			table.insert(CheckedSkillsBornChance,v.bornchance)
			table.insert(CheckedSkillsReqLvl,v.reqlevel)
		end 
	end 
	return ZLuaCheckOutNums(CheckedSkills,CheckedSkillsBornChance,CheckedSkillsReqLvl,num,maxsamecol)
end 	
----------------------根据法宝技能id随机选取法宝技能的级别变化(不同ID)---------------------------------------
function ZLuaTalisman_ChooseTalismanSkill(TalismanSkillID,cid)
	local TalismanSkillIDs={}
	TalismanSkillIDs[399]={399,1278,1279}---//_八卦石.ext
	TalismanSkillIDs[400]={400,1292,1293}---//_六合镜.ext
	TalismanSkillIDs[401]={401,1326,1327}---//_朱雀印.ext
	TalismanSkillIDs[402]={402,1294,1295}---//_轮回珠.ext
	TalismanSkillIDs[403]={403,1308,1309}---//_慑心铃.ext
	TalismanSkillIDs[404]={404,1298,1299}---//_七星剑.ext
	TalismanSkillIDs[405]={405,1318,1319}---//_四灵幡.ext
	TalismanSkillIDs[406]={406,1284,1285}---//_金刚杵.ext
	TalismanSkillIDs[407]={407,1280,1281}---//_多情环.ext
	TalismanSkillIDs[408]={408,1306,1307}---//_山河扇.ext
	TalismanSkillIDs[409]={409,1312,1313}---//_神木骰.ext
	TalismanSkillIDs[410]={410,1320,1321}---//_万毒袋(万毒鼎).ext
	TalismanSkillIDs[411]={411,1300,1301}---//_千醉坛(玉净瓶).ext
	TalismanSkillIDs[860]={860,1282,1283}---//_琥珀朱绫.ext
	TalismanSkillIDs[861]={861,1324,1325}---//_阴阳镜.ext
	TalismanSkillIDs[862]={862,1296,1297}---//_蟠龙幡.ext
	TalismanSkillIDs[863]={863,1286,1287}---//_金葫芦.ext
	TalismanSkillIDs[864]={864,1290,1291}---//_莲花.ext
	TalismanSkillIDs[865]={865,1302,1303}---//_乾坤青光戒.ext
	TalismanSkillIDs[866]={866,1322,1323}---//_五龙轮.ext
	TalismanSkillIDs[867]={867,1288,1289}---//_九龙神火罩.ext
	TalismanSkillIDs[868]={868,1304,1305}---//_如意乾坤袋.ext
	TalismanSkillIDs[869]={869,1310,1311}---//_摄魂.ext
	TalismanSkillIDs[870]={870,1316,1317}---//_噬血珠.ext
	TalismanSkillIDs[871]={871,1314,1315}---//_噬魂.ext
	TalismanSkillIDs[1653]={1653,1654,1655}---//_玲珑塔.ext
	TalismanSkillIDs[1697]={1697,1698,1699}---//赤金绫.ext
	TalismanSkillIDs[1700]={1700,1701,1702}---//_轮回盘.ext
	TalismanSkillIDs[1703]={1703,1704,1705}---//_乾坤锁.ext
	TalismanSkillIDs[1706]={1706,1707,1708}---//幽冥鬼手_.ext
	TalismanSkillIDs[1738]={1738,1739,1740}---//七星印.ext
	TalismanSkillIDs[1753]={1753,1754,1755}---//真山河扇.ext
	TalismanSkillIDs[1806]={1806,1807,1808}---//血玉骨片.ext
	TalismanSkillIDs[1809]={1809,1810,1811}---//破刹.ext
	TalismanSkillIDs[1812]={1812,1813,1814}---//千毒蛊.ext
	TalismanSkillIDs[1815]={1815,1816,1817}---//鬼罄.ext
	TalismanSkillIDs[2037]={2037,2038,2039}---//_翻天印.ext
	TalismanSkillIDs[2153]={2153,2154,2155}---//长明灯
	TalismanSkillIDs[2598]={2598,2599,2600}---//日月宝鉴
	TalismanSkillIDs[3088]={3088,3088,3088}---//元宵灯
	TalismanSkillIDs[3162]={3162,3163,3164}---//浮屠
	TalismanSkillIDs[3391]={3391,3392,3393}---//引魂幡
	TalismanSkillIDs[3825]={3825,3826,3827}---//宝库之魂
	TalismanSkillIDs[32954]={32954,32955,32956}---//天琊.ext

	TalismanSkillIDs[3610]={3610,3611,3612}---//神裔
	TalismanSkillIDs[3613]={3613,3614,3615}---//九黎
	TalismanSkillIDs[3616]={3616,3617,3618}---//九黎
	TalismanSkillIDs[3619]={3619,3620,3621}---//九黎
	TalismanSkillIDs[3622]={3622,3623,3624}---//九黎
	TalismanSkillIDs[3625]={3625,3626,3627}---//烈山
	TalismanSkillIDs[3628]={3628,3629,3630}---//烈山
	TalismanSkillIDs[3631]={3631,3632,3633}---//烈山
	TalismanSkillIDs[3634]={3634,3635,3636}---//烈山
	TalismanSkillIDs[3943]={3943,3944,3945}---//怀光
	TalismanSkillIDs[3946]={3946,3947,3948}---//怀光
	TalismanSkillIDs[3949]={3949,3950,3951}---//怀光
	TalismanSkillIDs[3952]={3952,3953,3954}---//怀光
	TalismanSkillIDs[3955]={3955,3956,3957}---//天华
	TalismanSkillIDs[3958]={3958,3959,3960}---//天华
	TalismanSkillIDs[3961]={3961,3962,3963}---//天华
	TalismanSkillIDs[3964]={3964,3965,3966}---//天华

	TalismanSkillIDs[33347]={33347,33348,33349}---//焚香
	TalismanSkillIDs[33350]={33350,33351,33352}---//焚香
	TalismanSkillIDs[33353]={33353,33354,33355}---//焚香
	TalismanSkillIDs[33356]={33356,33357,33358}---//焚香
	TalismanSkillIDs[33359]={33359,33360,33361}---//太昊
	TalismanSkillIDs[33362]={33362,33363,33364}---//太昊
	TalismanSkillIDs[33365]={33365,33366,33367}---//太昊
	TalismanSkillIDs[33368]={33368,33369,33370}---//太昊
	TalismanSkillIDs[34015]={34015,34016,34017}---//辰皇
	TalismanSkillIDs[34018]={34018,34019,34020}---//辰皇
	TalismanSkillIDs[34021]={34021,34022,34023}---//辰皇
	TalismanSkillIDs[34024]={34024,34025,34026}---//辰皇

	TalismanSkillIDs[34662]={34662,34663,34664}---//灵族

	TalismanSkillIDs[34667]={34667,34668,34669}---//牵机
	TalismanSkillIDs[34672]={34672,34673,34674}---//牵机
	TalismanSkillIDs[34677]={34677,34678,34679}---//牵机
	TalismanSkillIDs[34682]={34682,34683,34684}---//牵机
	TalismanSkillIDs[34687]={34687,34688,34689}---//英招
	TalismanSkillIDs[34692]={34692,34693,34694}---//英招
	TalismanSkillIDs[34697]={34697,34698,34699}---//英招
	TalismanSkillIDs[34702]={34702,34703,34704}---//英招


  	TalismanSkillIDs[33723]={33723,33724,33725}---//八卦神掌_八卦石.ext
  	TalismanSkillIDs[33729]={33729,33730,33731}---//六合纵横_六合镜.ext
  	TalismanSkillIDs[33732]={33732,33733,33734}---//烈焰血雨_朱雀印.ext
  	TalismanSkillIDs[33735]={33735,33736,33737}---//法海轮回_轮回珠.ext
  	TalismanSkillIDs[33738]={33738,33739,33740}---//摄魂夺魄_慑心铃.ext
  	TalismanSkillIDs[33765]={33765,33766,33767}---//流星赶月_七星剑.ext
  	TalismanSkillIDs[33768]={33768,33769,33770}---//四灵血咒_四灵幡.ext
  	TalismanSkillIDs[33771]={33771,33772,33773}---//金刚降魔_金刚杵.ext
  	TalismanSkillIDs[33774]={33774,33775,33776}---//刹那芳华_多情环.ext
  	TalismanSkillIDs[33879]={33879,33880,33881}---//江山如画_山河扇.ext
  	TalismanSkillIDs[33873]={33873,33874,33875}---//否极泰来_神木骰.ext
  	TalismanSkillIDs[33882]={33882,33883,33884}---//万毒噬天_万毒袋(万毒鼎).ext
  	TalismanSkillIDs[33885]={33885,33886,33887}---//玉净天华_千醉坛(玉净瓶).ext
  	TalismanSkillIDs[33801]={33801,33802,33803}---//千绫绚舞_琥珀朱绫.ext
  	TalismanSkillIDs[33900]={33900,33901,33902}---//阴阳相生_阴阳镜.ext
  	TalismanSkillIDs[33912]={33912,33913,33914}---//虬龙烈炎_蟠龙幡.ext
  	TalismanSkillIDs[33897]={33897,33898,33899}---//方寸乾坤_金葫芦.ext
  	TalismanSkillIDs[33810]={33810,33811,33812}---//暗香无际_莲花.ext
  	TalismanSkillIDs[33804]={33804,33805,33806}---//青光乍现_乾坤青光戒.ext
  	TalismanSkillIDs[33915]={33915,33916,33917}---//五龙御天_五龙轮.ext
  	TalismanSkillIDs[33894]={33894,33895,33896}---//炼狱神火_九龙神火罩.ext
  	TalismanSkillIDs[33807]={33807,33808,33809}---//乾坤如意_如意乾坤袋.ext
  	TalismanSkillIDs[33930]={33930,33931,33932}---//摄魂心驰_摄魂.ext
  	TalismanSkillIDs[33933]={33933,33934,33935}---//噬血邪灵_噬血珠.ext
  	TalismanSkillIDs[33936]={33936,33937,33938}---//噬魂无双_噬魂.ext
  --	TalismanSkillIDs[1653]={1653,1654,1655,33888,33889,33890}---//宝塔镇河妖_玲珑塔.ext
  	TalismanSkillIDs[33846]={33846,33847,33848}---//梦令之澜_赤金绫.ext
  	TalismanSkillIDs[33843]={33843,33844,33845}---//寂灭天舞_轮回盘.ext
  	TalismanSkillIDs[33837]={33837,33838,33839}---//尘光叹妙_乾坤锁.ext
  	TalismanSkillIDs[33840]={33840,33841,33842}---//鬼域幽歌_幽冥鬼手_.ext
  	TalismanSkillIDs[33903]={33903,33904,33905}---//华彰星陨_七星印.ext
  	TalismanSkillIDs[33906]={33906,33907,33908}---//逍遥如梦_真山河扇.ext
  	TalismanSkillIDs[33741]={33741,33742,33743}---//碎玉幻天_血玉骨片.ext
  	TalismanSkillIDs[33777]={33777,33778,33779}---//绝戾华光_破刹.ext
  	TalismanSkillIDs[33813]={33813,33814,33815}---//死亡波纹_千毒蛊.ext
  	TalismanSkillIDs[33849]={33849,33850,33851}---//迷踪之寂_鬼罄.ext
  	TalismanSkillIDs[33918]={33918,33919,33920}---//五岳咸服_翻天印.ext
  	TalismanSkillIDs[33921]={33921,33922,33923}---//寒月镇潭_长明灯
  	TalismanSkillIDs[33891]={33891,33892,33893}---//大凡若简_日月宝鉴
  --	TalismanSkillIDs[3088]={3088,3088,3088}---//元宵灯祝_元宵灯
  	TalismanSkillIDs[33909]={33909,33910,33911}---//圣塔降魔_浮屠
  	TalismanSkillIDs[33924]={33924,33925,33926}---//白骨之咒_引魂幡
  	TalismanSkillIDs[33927]={33927,33928,33929}---//碧天宇华_宝库之魂
  	TalismanSkillIDs[33939]={33939,33940,33941}---//天琊绝世_天琊.ext
  
  	TalismanSkillIDs[33726]={33726,33727,33728}---//万彩画一_神裔
  	TalismanSkillIDs[33747]={33747,33748,33749}---//驱虎逐豹_九黎
  	TalismanSkillIDs[33783]={33783,33784,33785}---//枫林煞血_九黎
  	TalismanSkillIDs[33819]={33819,33820,33821}---//狂澜灭杀_九黎
  	TalismanSkillIDs[33855]={33855,33856,33857}---//亡魂丧魄_九黎
  	TalismanSkillIDs[33750]={33750,33751,33752}---//月影丹朱_烈山
  	TalismanSkillIDs[33786]={33786,33787,33788}---//夺魄返魂_烈山
  	TalismanSkillIDs[33822]={33822,33823,33824}---//浮光阑影_烈山
  	TalismanSkillIDs[33858]={33858,33859,33860}---//曦皇神威_烈山
  	TalismanSkillIDs[33753]={33753,33754,33755}---//血杀千里_怀光
  	TalismanSkillIDs[33789]={33789,33790,33791}---//横扫千军_怀光
  	TalismanSkillIDs[33825]={33825,33826,33827}---//破魂千劫_怀光
  	TalismanSkillIDs[33861]={33861,33862,33863}---//婆娑影皇_怀光
  	TalismanSkillIDs[33756]={33756,33757,33758}---//琼华天籁_天华
  	TalismanSkillIDs[33792]={33792,33793,33794}---//苍穹凤歌_天华
  	TalismanSkillIDs[33828]={33828,33829,33830}---//宇光蓬莱_天华
  	TalismanSkillIDs[33864]={33864,33865,33866}---//天魔降伏_天华
  
  	TalismanSkillIDs[33744]={33744,33745,33746}---//业火天阳_焚香
  	TalismanSkillIDs[33780]={33780,33781,33782}---//天地玄极_焚香
  	TalismanSkillIDs[33816]={33816,33817,33818}---//凌火明阳_焚香
  	TalismanSkillIDs[33852]={33852,33853,33854}---//太幽玄冰_焚香
  	TalismanSkillIDs[33759]={33759,33760,33761}---//刑天怒雷_太昊
  	TalismanSkillIDs[33795]={33795,33796,33797}---//殇落流华_太昊
  	TalismanSkillIDs[33831]={33831,33832,33833}---//子午心魂_太昊
  	TalismanSkillIDs[33867]={33867,33868,33869}---//浩气云天_太昊
		TalismanSkillIDs[33762]={33762,33763,33764}---//辰皇一级_辰皇
		TalismanSkillIDs[33798]={33798,33799,33800}---//辰皇二级_辰皇
		TalismanSkillIDs[33834]={33834,33835,33836}---//辰皇三级_辰皇
		TalismanSkillIDs[33870]={33870,33871,33872}---//辰皇四级_辰皇

		TalismanSkillIDs[34803]={34803,34804,34805}---//灵族新手_灵族

  	TalismanSkillIDs[34807]={34807,34808,34809}---//牵机一级_牵机
  	TalismanSkillIDs[34811]={34811,34812,34813}---//牵机二级_牵机
  	TalismanSkillIDs[34815]={34815,34816,34817}---//牵机三级_牵机
  	TalismanSkillIDs[34819]={34819,34820,34821}---//牵机四级_牵机
		TalismanSkillIDs[34823]={34823,34824,34825}---//英招一级_英招
		TalismanSkillIDs[34827]={34827,34828,34829}---//英招二级_英招
		TalismanSkillIDs[34831]={34831,34832,34833}---//英招三级_英招
		TalismanSkillIDs[34835]={34835,34836,34837}---//英招四级_英招



	local skilllvlchanceseq={275,24,1}
	if cid == 23882 then skilllvlchanceseq={2750,249,1} end  ---//礼包
	if cid == 62752 then skilllvlchanceseq={225,73,2} end    ---//新
	if cid == 62753 then 
		skilllvlchanceseq={0,0,1}
		for i,v in pairs(TalismanSkillIDs) do
			if TalismanSkillID==v[3] then
				return -1 
			end
		end 
	end    
---	if cid == 62753 then skilllvlchanceseq={0,0,1} end       ---//洗3阶
	local newholyskilllevel
		---//1-3级别技能的出现概率
	for i,v in pairs(TalismanSkillIDs) do
		if TalismanSkillID==i or TalismanSkillID==v[1] or TalismanSkillID==v[2] or TalismanSkillID==v[3] then 
			return TalismanSkillIDs[i][ZLuaTal_Get_RanPoint(skilllvlchanceseq)]or TalismanSkillID
		end 
	end
	
end 
----------------------根据法宝id和法宝携带技能的id判断归属职业-----------------------------------------------
function ZLuaTalisman_GetProfInfo(id,SkillNoSeq)
	local prof_fromid=99
	local prof_fromskill=99
	
	local talismans=ZLuaTalAvailableTalismans()
	for i,v in pairs(talismans) do
		if id == v.tmplID then 
			prof_fromid = v.school
		end 
		if prof_fromid == 0 then 
			prof_fromid = 99
		end 
		if id == 4703 then
			prof_fromid = 21
		end
		if id == 30076 then
			prof_fromid = 22
		end
		if id == 58825 then
			prof_fromid = 33
		end
	end 

	if prof_fromid == 21 or prof_fromid == 22 or prof_fromid == 33	then
		prof_fromskill = prof_fromid
	end 

	local SkillsExLists=ZLuaTalismanEnchantSkillEx()
	for i,v in pairs(SkillsExLists) do
		for j=1,table.getn(SkillNoSeq) do 
			if i==SkillNoSeq[j] and (v.prof==1 or v.prof==2 or v.prof==3 or v.prof==4 or v.prof==5 or v.prof==6 or v.prof==7 or v.prof==8 or v.prof==9 or v.prof==10 or v.prof==11 or v.prof==12 or v.prof==13 or v.prof==14) then 		--添加新职业需修改这行
				prof_fromskill = v.prof
			end 
		end 
	end 

	if prof_fromid==1 or prof_fromid==2 or prof_fromid==3 or prof_fromid==4 or prof_fromid==5 or prof_fromid==6 or prof_fromid==7 or prof_fromid==8 or prof_fromid==9 or prof_fromid==10 or prof_fromid==11 or prof_fromid==12 or prof_fromid==13 or prof_fromid==14  		--添加新职业需修改这行
	then 
		prof_fromskill=prof_fromid
	end 

	return  prof_fromid,prof_fromskill
	
end 

------------------法宝飞升主函数-----------------------------------------------------------------------------

function ZLuaTalHolyLevelUp(id,talisman,cid,output)	
	local dataIndex=ZLuaTalDataIndex()
	local talismans=ZLuaTalAvailableTalismans()
	local outid
	
---	print("dataIndex["reserve1"]=",talisman:QueryData(0,dataIndex["reserve1"]))
---	print("dataIndex["reserve2"]=",talisman:QueryData(0,dataIndex["reserve2"]))
--[[
  print("reserve1=",talisman:QueryData(0,dataIndex["reserve1"]))
  print("reserve2=",talisman:QueryData(0,dataIndex["reserve2"]))
  print("tmpl_id=",talisman:QueryData(0,dataIndex["tmpl_id"]))
  print("skill_addon_id=",talisman:QueryData(0,dataIndex["skill_addon_id"]))
  print("default_stamina=",talisman:QueryData(0,dataIndex["default_stamina"]))
  print("quo_stamina=",talisman:QueryData(0,dataIndex["quo_stamina"]))
  print("default_hp=",talisman:QueryData(0,dataIndex["default_hp"]))
  print("default_mp=",talisman:QueryData(0,dataIndex["default_mp"]))
  print("default_attack_rate=",talisman:QueryData(0,dataIndex["default_attack_rate"]))
  print("default_damage_low=",talisman:QueryData(0,dataIndex["default_damage_low"]))
  print("default_damage_high=",talisman:QueryData(0,dataIndex["default_damage_high"]))
  print("default_dodge=",talisman:QueryData(0,dataIndex["default_dodge"]))
  print("default_defense=",talisman:QueryData(0,dataIndex["default_defense"]))
  print("default_resis_stun=",talisman:QueryData(0,dataIndex["default_resis_stun"]))
  print("default_resis_sleep=",talisman:QueryData(0,dataIndex["default_resis_sleep"]))
  print("default_resis_silence=",talisman:QueryData(0,dataIndex["default_resis_silence"]))
  print("default_resis_weakness=",talisman:QueryData(0,dataIndex["default_resis_weakness"]))
  print("default_resis_enlace=",talisman:QueryData(0,dataIndex["default_resis_enlace"]))
  print("grow_hp=",talisman:QueryData(0,dataIndex["grow_hp"]))
  print("grow_mp=",talisman:QueryData(0,dataIndex["grow_mp"]))
  print("grow_attack_rate=",talisman:QueryData(0,dataIndex["grow_attack_rate"]))
  print("grow_damage_low=",talisman:QueryData(0,dataIndex["grow_damage_low"]))
  print("grow_damage_high=",talisman:QueryData(0,dataIndex["grow_damage_high"]))
  print("grow_dodge=",talisman:QueryData(0,dataIndex["grow_dodge"]))
  print("grow_defense=",talisman:QueryData(0,dataIndex["grow_defense"]))
  print("grow_resis_stun=",talisman:QueryData(0,dataIndex["grow_resis_stun"]))
  print("grow_resis_sleep=",talisman:QueryData(0,dataIndex["grow_resis_sleep"]))
  print("grow_resis_silence=",talisman:QueryData(0,dataIndex["grow_resis_silence"]))
  print("grow_resis_weakness=",talisman:QueryData(0,dataIndex["grow_resis_weakness"]))
  print("grow_resis_enlace=",talisman:QueryData(0,dataIndex["grow_resis_enlace"]))
  print("addon_1_learn_level=",talisman:QueryData(0,dataIndex["addon_1_learn_level"]))
  print("addon_1_id=",talisman:QueryData(0,dataIndex["addon_1_id"]))
  print("addon_2_learn_level=",talisman:QueryData(0,dataIndex["addon_2_learn_level"]))
  print("addon_2_id=",talisman:QueryData(0,dataIndex["addon_2_id"]))
  print("addon_3_learn_level=",talisman:QueryData(0,dataIndex["addon_3_learn_level"]))
  print("addon_3_id=",talisman:QueryData(0,dataIndex["addon_3_id"]))
  print("addon_4_learn_level=",talisman:QueryData(0,dataIndex["addon_4_learn_level"]))
  print("addon_4_id=",talisman:QueryData(0,dataIndex["addon_4_id"]))
  print("addon_5_learn_level=",talisman:QueryData(0,dataIndex["addon_5_learn_level"]))
  print("addon_5_id=",talisman:QueryData(0,dataIndex["addon_5_id"]))
  print("addon_6_learn_level=",talisman:QueryData(0,dataIndex["addon_6_learn_level"]))
  print("addon_6_id=",talisman:QueryData(0,dataIndex["addon_6_id"]))
  print("addon_7_learn_level=",talisman:QueryData(0,dataIndex["addon_7_learn_level"]))
  print("addon_7_id=",talisman:QueryData(0,dataIndex["addon_7_id"]))
  print("addon_8_learn_level=",talisman:QueryData(0,dataIndex["addon_8_learn_level"]))
  print("addon_8_id=",talisman:QueryData(0,dataIndex["addon_8_id"]))
  print("addon_9_learn_level=",talisman:QueryData(0,dataIndex["addon_9_learn_level"]))
  print("addon_9_id=",talisman:QueryData(0,dataIndex["addon_9_id"]))
  print("addon_10_learn_level=",talisman:QueryData(0,dataIndex["addon_10_learn_level"]))
  print("addon_10_id=",talisman:QueryData(0,dataIndex["addon_10_id"]))
  print("addon_11_learn_level=",talisman:QueryData(0,dataIndex["addon_11_learn_level"]))
  print("addon_11_id=",talisman:QueryData(0,dataIndex["addon_11_id"]))
  print("artpath_1_level=",talisman:QueryData(0,dataIndex["artpath_1_level"]))
  print("artpath_2_level=",talisman:QueryData(0,dataIndex["artpath_2_level"]))
  print("artpath_3_level=",talisman:QueryData(0,dataIndex["artpath_3_level"]))
  print("artpath_4_level=",talisman:QueryData(0,dataIndex["artpath_4_level"]))
  print("confusion_1_level=",talisman:QueryData(0,dataIndex["confusion_1_level"]))
  print("confusion_1_quo=",talisman:QueryData(0,dataIndex["confusion_1_quo"]))
  print("confusion_2_level=",talisman:QueryData(0,dataIndex["confusion_2_level"]))
  print("confusion_2_quo=",talisman:QueryData(0,dataIndex["confusion_2_quo"]))
  print("confusion_3_level=",talisman:QueryData(0,dataIndex["confusion_3_level"]))
  print("confusion_3_quo=",talisman:QueryData(0,dataIndex["confusion_3_quo"]))
  print("confusion_4_level=",talisman:QueryData(0,dataIndex["confusion_4_level"]))
  print("confusion_4_quo=",talisman:QueryData(0,dataIndex["confusion_4_quo"]))
  print("confusion_5_level=",talisman:QueryData(0,dataIndex["confusion_5_level"]))
  print("confusion_5_quo=",talisman:QueryData(0,dataIndex["confusion_5_quo"]))
  print("origin=",talisman:QueryData(0,dataIndex["origin"]))
  print("birth_hour12=",talisman:QueryData(0,dataIndex["birth_hour12"]))
  print("birth_min3=",talisman:QueryData(0,dataIndex["birth_min3"]))
  print("quality=",talisman:QueryData(0,dataIndex["quality"]))
  print("speciality=",talisman:QueryData(0,dataIndex["speciality"]))
  print("buff_type=",talisman:QueryData(0,dataIndex["buff_type"]))
  print("ext_id=",talisman:QueryData(0,dataIndex["ext_id"]))
  print("skill_visible=",talisman:QueryData(0,dataIndex["skill_visible"]))
  print("combine_times=",talisman:QueryData(0,dataIndex["combine_times"]))
  print("enchant_times=",talisman:QueryData(0,dataIndex["enchant_times"]))
  print("feed_baoqi=",talisman:QueryData(0,dataIndex["feed_baoqi"]))
  print("feed_xianpin=",talisman:QueryData(0,dataIndex["feed_xianpin"]))
  print("feed_shenpin=",talisman:QueryData(0,dataIndex["feed_shenpin"]))
  print("feed_cost=",talisman:QueryData(0,dataIndex["feed_cost"]))
  print("change_skillnum=",talisman:QueryData(0,dataIndex["change_skillnum"]))
  print("feed_level_cost=",talisman:QueryData(0,dataIndex["feed_level_cost"]))
  print("is_fly=",talisman:QueryData(0,dataIndex["is_fly"]))
  print("hp_rise=",talisman:QueryData(0,dataIndex["hp_rise"]))
  print("mp_rise=",talisman:QueryData(0,dataIndex["mp_rise"]))
  print("minattack_rise=",talisman:QueryData(0,dataIndex["minattack_rise"]))
  print("maxattack_rise=",talisman:QueryData(0,dataIndex["maxattack_rise"]))
  print("stun_rise=",talisman:QueryData(0,dataIndex["stun_rise"]))
  print("weakness_rise=",talisman:QueryData(0,dataIndex["weakness_rise"]))
  print("enlace_rise=",talisman:QueryData(0,dataIndex["enlace_rise"]))
  print("silence_rise=",talisman:QueryData(0,dataIndex["silence_rise"]))
  print("sleep_rise=",talisman:QueryData(0,dataIndex["sleep_rise"]))
  print("all_rise=",talisman:QueryData(0,dataIndex["all_rise"]))
  print("hp_enhance=",talisman:QueryData(0,dataIndex["hp_enhance"]))
  print("mp_enhance=",talisman:QueryData(0,dataIndex["mp_enhance"]))
  print("atk_enhance=",talisman:QueryData(0,dataIndex["atk_enhance"]))
  print("skillrefine_level=",talisman:QueryData(0,dataIndex["skillrefine_level"]))
  print("skillrefine_0=",talisman:QueryData(0,dataIndex["skillrefine_0"]))
  print("skillrefine_1=",talisman:QueryData(0,dataIndex["skillrefine_1"]))
  print("skillrefine_2=",talisman:QueryData(0,dataIndex["skillrefine_2"]))
  print("skillrefine_3=",talisman:QueryData(0,dataIndex["skillrefine_3"]))
  print("skillrefine_4=",talisman:QueryData(0,dataIndex["skillrefine_4"]))
  print("skillrefine_5=",talisman:QueryData(0,dataIndex["skillrefine_5"]))
  print("skillrefine_6=",talisman:QueryData(0,dataIndex["skillrefine_6"]))
  print("skillrefine_7=",talisman:QueryData(0,dataIndex["skillrefine_7"]))
  print("skillrefine_8=",talisman:QueryData(0,dataIndex["skillrefine_8"]))
  print("skillrefinetmp_1=",talisman:QueryData(0,dataIndex["skillrefinetmp_1"]))
  print("skillrefinetmp_2=",talisman:QueryData(0,dataIndex["skillrefinetmp_2"]))
  print("skillrefinetmp_3=",talisman:QueryData(0,dataIndex["skillrefinetmp_3"]))
  print("skillrefinetmp_4=",talisman:QueryData(0,dataIndex["skillrefinetmp_4"]))
  print("skillrefinetmp_5=",talisman:QueryData(0,dataIndex["skillrefinetmp_5"]))
  print("skillrefinetmp_6=",talisman:QueryData(0,dataIndex["skillrefinetmp_6"]))
  print("skillrefinetmp_7=",talisman:QueryData(0,dataIndex["skillrefinetmp_7"]))
  print("skillrefinetmp_8=",talisman:QueryData(0,dataIndex["skillrefinetmp_8"]))
  print("newaddon_id_1=",talisman:QueryData(0,dataIndex["newaddon_id_1"]))
  print("newaddon_id_2=",talisman:QueryData(0,dataIndex["newaddon_id_2"]))
  print("newaddon_id_3=",talisman:QueryData(0,dataIndex["newaddon_id_3"]))
  print("newaddon_id_4=",talisman:QueryData(0,dataIndex["newaddon_id_4"]))
  print("newaddon_id_5=",talisman:QueryData(0,dataIndex["newaddon_id_5"]))
  print("reborncont=",talisman:QueryData(0,dataIndex["reborncont"]))
  print("skillrefinecont=",talisman:QueryData(0,dataIndex["skillrefinecont"]))
--]]
  ---没有灵媒无法飞升-----------------------
	if cid == 0 then
		output:CreateItem(-1)
		return
	end
  ---没注册过的法宝飞剑，禁止飞升----------------
	local Registed = false
	for _,v in pairs(talismans) do
		if v.tmplID == id then Registed = true end
	end
	if Registed == false then
		output:CreateItem(-1)
		return
	end


 ---这里是策划内部特殊处理函数，仅限在内服制作任意数值的法宝，用作生成待复制到外服的法宝，针对单独玩家修复BUG用------
 ---注意，每次生成法宝要对每一项dataIndex精确定义，否则法宝仍有可能出现不可预知的问题-------
	
  if cid == 12021 then
  	outid = id
 		output:CreateItem(outid)			
		for _,index in pairs(dataIndex) do
			output:UpdateData(0,index,talisman:QueryData(0,index))
		end

	 	output:UpdateData(0,dataIndex["reserve1"], 0)
	 	output:UpdateData(0,dataIndex["reserve2"], 0)
---	 	output:UpdateData(0,dataIndex["tmpl_id"], 43475)
	 	output:UpdateData(0,dataIndex["skill_addon_id"], 33941)

	 	output:UpdateData(0,dataIndex["default_stamina"], 232)
	 	output:UpdateData(0,dataIndex["quo_stamina"], 0.98000001907349)

	 	output:UpdateData(0,dataIndex["default_hp"], 40)
	 	output:UpdateData(0,dataIndex["default_mp"], 47)
	 	output:UpdateData(0,dataIndex["default_attack_rate"], 0)
	 	output:UpdateData(0,dataIndex["default_damage_low"], 9)
	 	output:UpdateData(0,dataIndex["default_damage_high"], 27)
	 	output:UpdateData(0,dataIndex["default_dodge"], 0)
	 	output:UpdateData(0,dataIndex["default_defense"], 0)
	 	output:UpdateData(0,dataIndex["default_resis_stun"], 8)
	 	output:UpdateData(0,dataIndex["default_resis_sleep"], 8)
	 	output:UpdateData(0,dataIndex["default_resis_silence"], 8)
	 	output:UpdateData(0,dataIndex["default_resis_weakness"], 8)
	 	output:UpdateData(0,dataIndex["default_resis_enlace"], 8)

	 	output:UpdateData(0,dataIndex["grow_hp"], 150.7799987793)
	 	output:UpdateData(0,dataIndex["grow_mp"], -24.280000686646)
	 	output:UpdateData(0,dataIndex["grow_attack_rate"], 0)
	 	output:UpdateData(0,dataIndex["grow_damage_low"], -2.2699999809265)
	 	output:UpdateData(0,dataIndex["grow_damage_high"], 34.619998931885)
	 	output:UpdateData(0,dataIndex["grow_dodge"], 0)
	 	output:UpdateData(0,dataIndex["grow_defense"], 0)
	 	output:UpdateData(0,dataIndex["grow_resis_stun"], 6.0599999427795)
	 	output:UpdateData(0,dataIndex["grow_resis_sleep"], 6.0599999427795)
	 	output:UpdateData(0,dataIndex["grow_resis_silence"], 6.0599999427795)
	 	output:UpdateData(0,dataIndex["grow_resis_weakness"], 6.0599999427795)
	 	output:UpdateData(0,dataIndex["grow_resis_enlace"], 6.0599999427795)

	 	output:UpdateData(0,dataIndex["addon_1_learn_level"], 25)
	 	output:UpdateData(0,dataIndex["addon_1_id"], 897)
	 	output:UpdateData(0,dataIndex["addon_2_learn_level"], 28)
	 	output:UpdateData(0,dataIndex["addon_2_id"], 899)
	 	output:UpdateData(0,dataIndex["addon_3_learn_level"], 20)
	 	output:UpdateData(0,dataIndex["addon_3_id"], 398)
	 	output:UpdateData(0,dataIndex["addon_4_learn_level"], 11)
	 	output:UpdateData(0,dataIndex["addon_4_id"], 393)
	 	output:UpdateData(0,dataIndex["addon_5_learn_level"], 13)
	 	output:UpdateData(0,dataIndex["addon_5_id"], 394)
	 	output:UpdateData(0,dataIndex["addon_6_learn_level"], 0)
	 	output:UpdateData(0,dataIndex["addon_6_id"], 0)
	 	output:UpdateData(0,dataIndex["addon_7_learn_level"], 0)
	 	output:UpdateData(0,dataIndex["addon_7_id"], 0)
	 	output:UpdateData(0,dataIndex["addon_8_learn_level"], 0)
	 	output:UpdateData(0,dataIndex["addon_8_id"], 0)
	 	output:UpdateData(0,dataIndex["addon_9_learn_level"], 0)
	 	output:UpdateData(0,dataIndex["addon_9_id"], 0)
	 	output:UpdateData(0,dataIndex["addon_10_learn_level"], 0)
	 	output:UpdateData(0,dataIndex["addon_10_id"], 0)
	 	output:UpdateData(0,dataIndex["addon_11_learn_level"], 0)
	 	output:UpdateData(0,dataIndex["addon_11_id"], 0)

	 	output:UpdateData(0,dataIndex["artpath_1_level"], 1)
	 	output:UpdateData(0,dataIndex["artpath_2_level"], 10)
	 	output:UpdateData(0,dataIndex["artpath_3_level"], 20)
	 	output:UpdateData(0,dataIndex["artpath_4_level"], 30)

	 	output:UpdateData(0,dataIndex["confusion_1_level"], 2)
	 	output:UpdateData(0,dataIndex["confusion_1_quo"], 1.440212726593)
	 	output:UpdateData(0,dataIndex["confusion_2_level"], 4)
	 	output:UpdateData(0,dataIndex["confusion_2_quo"], 1.3351782560349)
	 	output:UpdateData(0,dataIndex["confusion_3_level"], 6)
	 	output:UpdateData(0,dataIndex["confusion_3_quo"], 1.352597951889)
	 	output:UpdateData(0,dataIndex["confusion_4_level"], 17)
	 	output:UpdateData(0,dataIndex["confusion_4_quo"], 1.348880648613)
	 	output:UpdateData(0,dataIndex["confusion_5_level"], 21)
	 	output:UpdateData(0,dataIndex["confusion_5_quo"], 1.574632525444)

	 	output:UpdateData(0,dataIndex["origin"], 3)
	 	output:UpdateData(0,dataIndex["birth_hour12"], 3)
	 	output:UpdateData(0,dataIndex["birth_min3"], 0)
	 	output:UpdateData(0,dataIndex["quality"], 2)
	 	output:UpdateData(0,dataIndex["speciality"], 0)
	 	output:UpdateData(0,dataIndex["buff_type"], 3)
	 	output:UpdateData(0,dataIndex["ext_id"], 34628)
	 	output:UpdateData(0,dataIndex["skill_visible"], 0)
	 	output:UpdateData(0,dataIndex["combine_times"], 1882)
	 	output:UpdateData(0,dataIndex["enchant_times"], 681)
	 	output:UpdateData(0,dataIndex["feed_baoqi"], 16)
	 	output:UpdateData(0,dataIndex["feed_xianpin"], 50)
	 	output:UpdateData(0,dataIndex["feed_shenpin"], 1816)
	 	output:UpdateData(0,dataIndex["feed_cost"], 261380)
	 	output:UpdateData(0,dataIndex["change_skillnum"], 658)
	 	output:UpdateData(0,dataIndex["feed_level_cost"], 73803)
	 	output:UpdateData(0,dataIndex["is_fly"], 0)

	 	output:UpdateData(0,dataIndex["hp_rise"], 10)
	 	output:UpdateData(0,dataIndex["mp_rise"], 10)
	 	output:UpdateData(0,dataIndex["minattack_rise"], 10)
	 	output:UpdateData(0,dataIndex["maxattack_rise"], 10)
	 	output:UpdateData(0,dataIndex["stun_rise"], 10)
	 	output:UpdateData(0,dataIndex["weakness_rise"], 10)
	 	output:UpdateData(0,dataIndex["enlace_rise"], 10)
	 	output:UpdateData(0,dataIndex["silence_rise"], 10)
	 	output:UpdateData(0,dataIndex["sleep_rise"], 10)
	 	output:UpdateData(0,dataIndex["all_rise"], 90)

	 	output:UpdateData(0,dataIndex["hp_enhance"], 7000)
	 	output:UpdateData(0,dataIndex["mp_enhance"], 7000)
	 	output:UpdateData(0,dataIndex["atk_enhance"], 1050)

	 	output:UpdateData(0,dataIndex["skillrefine_level"], 3)
	 	output:UpdateData(0,dataIndex["skillrefine_0"], 4112)
	 	output:UpdateData(0,dataIndex["skillrefine_1"], 4279)
	 	output:UpdateData(0,dataIndex["skillrefine_2"], 4291)
	 	output:UpdateData(0,dataIndex["skillrefine_3"], 4349)
	 	output:UpdateData(0,dataIndex["skillrefine_4"], 4350)
	 	output:UpdateData(0,dataIndex["skillrefine_5"], 0)
	 	output:UpdateData(0,dataIndex["skillrefine_6"], 0)
	 	output:UpdateData(0,dataIndex["skillrefine_7"], 0)
	 	output:UpdateData(0,dataIndex["skillrefine_8"], 0)

	 	output:UpdateData(0,dataIndex["skillrefinetmp_1"], 0)
	 	output:UpdateData(0,dataIndex["skillrefinetmp_2"], 0)
	 	output:UpdateData(0,dataIndex["skillrefinetmp_3"], 0)
	 	output:UpdateData(0,dataIndex["skillrefinetmp_4"], 0)
	 	output:UpdateData(0,dataIndex["skillrefinetmp_5"], 0)
	 	output:UpdateData(0,dataIndex["skillrefinetmp_6"], 0)
	 	output:UpdateData(0,dataIndex["skillrefinetmp_7"], 0)
	 	output:UpdateData(0,dataIndex["skillrefinetmp_8"], 0)

	 	output:UpdateData(0,dataIndex["newaddon_id_1"], 34287)
	 	output:UpdateData(0,dataIndex["newaddon_id_2"], 34467)
	 	output:UpdateData(0,dataIndex["newaddon_id_3"], 34288)
	 	output:UpdateData(0,dataIndex["newaddon_id_4"], 34470)
	 	output:UpdateData(0,dataIndex["newaddon_id_5"], 0)

	 	output:UpdateData(0,dataIndex["reborncont"], 2)
	 	output:UpdateData(0,dataIndex["skillrefinecont"], 293)


		output:InheritCombineExp(true)

		ZLuaTalRebuild(output,outid)
		return
  end


 ---如果法宝ID为飞宝ID，则禁止飞升--------------
	if talisman:QueryData(0,dataIndex["is_fly"])==1 then
		output:CreateItem(-1)
		return
	end
 ---法宝元宵灯和玲珑塔禁止飞升------------------
	if id == talismans["US_45_YuanXiaoDeng"].tmplID or id == talismans["US_75_LingLongTa"].tmplID then
		output:CreateItem(-1)
		return
	end

 ---已经飞升的法宝无法进行飞升操作--------------
	if talisman:QueryData(0,dataIndex["reborncont"]) >= 1 or talisman:QueryData(0,dataIndex["skillrefine_0"]) > 0 then
		output:CreateItem(-1)
		return
	end

 ---等级不足35级无法飞升------------------------
	if talisman:QueryLevel() < 30 then
		output:CreateItem(-1)
		return
	end


	if cid ~= 53576 and cid ~= 55602 then 
		output:CreateItem(-1)
		return
	end
 ---法宝飞升内部数据处理------------------------
	if cid == 53576 or cid == 55602 then
		outid = id
		output:CreateItem(outid)			
		for _,index in pairs(dataIndex) do
			output:UpdateData(0,index,talisman:QueryData(0,index))
		end
---		ZLuaTalisman_InitHolySkillID(id,output)

  	local HolySkillLevel
  	local HolySkillAddonID
  	local HolySkillId0
  	local HolySkillId1
  	local HolySkillId2
  	local HolySkillId3
  	local HolySkillId4
  	local HolySkillId5
  	local HolySkillId6
  	local HolySkillId7
  	local HolySkillId8
  	
  	local TalismanSkillIDs={}
  ---技能序列addon1，addon2，addon3，飞升addon1，飞升addon2，飞升addon3，飞升addon1对应技能id，飞升技能初始效果ID1,ID2,ID3,ID4,ID5,ID6,ID7,ID8
  	TalismanSkillIDs[399]={399,1278,1279,33723,33724,33725,4044,4166,4167,0,0,0,0,0,0}---//八卦神掌_八卦石.ext
  	TalismanSkillIDs[400]={400,1292,1293,33729,33730,33731,4046,4170,4171,0,0,0,0,0,0}---//六合纵横_六合镜.ext
  	TalismanSkillIDs[401]={401,1326,1327,33732,33733,33734,4047,4172,4173,0,0,0,0,0,0}---//烈焰血雨_朱雀印.ext
  	TalismanSkillIDs[402]={402,1294,1295,33735,33736,33737,4048,4174,4175,0,0,0,0,0,0}---//法海轮回_轮回珠.ext
  	TalismanSkillIDs[403]={403,1308,1309,33738,33739,33740,4049,4176,4177,0,0,0,0,0,0}---//摄魂夺魄_慑心铃.ext
  	TalismanSkillIDs[404]={404,1298,1299,33765,33766,33767,4057,4193,4194,0,0,0,0,0,0}---//流星赶月_七星剑.ext
  	TalismanSkillIDs[405]={405,1318,1319,33768,33769,33770,4058,4196,4197,0,0,0,0,0,0}---//四灵血咒_四灵幡.ext
  	TalismanSkillIDs[406]={406,1284,1285,33771,33772,33773,4059,4199,4200,0,0,0,0,0,0}---//金刚降魔_金刚杵.ext
  	TalismanSkillIDs[407]={407,1280,1281,33774,33775,33776,4060,4202,4203,0,0,0,0,0,0}---//刹那芳华_多情环.ext
  	TalismanSkillIDs[408]={408,1306,1307,33879,33880,33881,4092,4291,0   ,0,0,0,0,0,0}---//江山如画_山河扇.ext
  	TalismanSkillIDs[409]={409,1312,1313,33873,33874,33875,4090,4290,0   ,0,0,0,0,0,0}---//否极泰来_神木骰.ext
  	TalismanSkillIDs[410]={410,1320,1321,33882,33883,33884,4093,4292,0   ,0,0,0,0,0,0}---//万毒噬天_万毒袋(万毒鼎).ext
  	TalismanSkillIDs[411]={411,1300,1301,33885,33886,33887,4094,4293,0   ,0,0,0,0,0,0}---//玉净天华_千醉坛(玉净瓶).ext
  	TalismanSkillIDs[860]={860,1282,1283,33801,33802,33803,4068,4221,4222,4223,0,0,0,0,0}---//千绫绚舞_琥珀朱绫.ext
  	TalismanSkillIDs[861]={861,1324,1325,33900,33901,33902,4099,4303,4304,4305,0,0,0,0,0}---//阴阳相生_阴阳镜.ext
  	TalismanSkillIDs[862]={862,1296,1297,33912,33913,33914,4103,4316,4317,0,0,0,0,0,0}---//虬龙烈炎_蟠龙幡.ext
  	TalismanSkillIDs[863]={863,1286,1287,33897,33898,33899,4098,4300,4301,4302,0,0,0,0,0}---//方寸乾坤_金葫芦.ext
  	TalismanSkillIDs[864]={864,1290,1291,33810,33811,33812,4071,4230,4231,4232,0,0,0,0,0}---//暗香无际_莲花.ext
  	TalismanSkillIDs[865]={865,1302,1303,33804,33805,33806,4069,4224,4225,4226,0,0,0,0,0}---//青光乍现_乾坤青光戒.ext
  	TalismanSkillIDs[866]={866,1322,1323,33915,33916,33917,4104,4318,4319,4320,0,0,0,0,0}---//五龙御天_五龙轮.ext
  	TalismanSkillIDs[867]={867,1288,1289,33894,33895,33896,4097,4297,4298,4299,0,0,0,0,0}---//炼狱神火_九龙神火罩.ext
  	TalismanSkillIDs[868]={868,1304,1305,33807,33808,33809,4070,4227,4228,4229,0,0,0,0,0}---//乾坤如意_如意乾坤袋.ext
  	TalismanSkillIDs[869]={869,1310,1311,33930,33931,33932,4109,4334,4335,4336,0,0,0,0,0}---//摄魂心驰_摄魂.ext
  	TalismanSkillIDs[870]={870,1316,1317,33933,33934,33935,4110,4337,4338,4339,4340,0,0,0,0}---//噬血邪灵_噬血珠.ext
  	TalismanSkillIDs[871]={871,1314,1315,33936,33937,33938,4111,4342,4343,4344,4345,4346,0,0,0}---//噬魂无双_噬魂.ext
  --	TalismanSkillIDs[1653]={1653,1654,1655,33888,33889,33890}---//宝塔镇河妖_玲珑塔.ext
  	TalismanSkillIDs[1697]={1697,1698,1699,33846,33847,33848,4082,4265,4266,4267,4268,0,0,0,0}---//梦令之澜_赤金绫.ext
  	TalismanSkillIDs[1700]={1700,1701,1702,33843,33844,33845,4081,4261,4262,4263,4264,0,0,0,0}---//寂灭天舞_轮回盘.ext
  	TalismanSkillIDs[1703]={1703,1704,1705,33837,33838,33839,4079,4253,4254,4255,4256,0,0,0,0}---//尘光叹妙_乾坤锁.ext
  	TalismanSkillIDs[1706]={1706,1707,1708,33840,33841,33842,4080,4257,4258,4259,4260,0,0,0,0}---//鬼域幽歌_幽冥鬼手_.ext
  	TalismanSkillIDs[1738]={1738,1739,1740,33903,33904,33905,4100,4306,4307,4308,4309,0,0,0,0}---//华彰星陨_七星印.ext
  	TalismanSkillIDs[1753]={1753,1754,1755,33906,33907,33908,4101,4310,4311,0,0,0,0,0,0}---//逍遥如梦_真山河扇.ext
  	TalismanSkillIDs[1806]={1806,1807,1808,33741,33742,33743,4050,4178,4179,0,0,0,0,0,0}---//碎玉幻天_血玉骨片.ext
  	TalismanSkillIDs[1809]={1809,1810,1811,33777,33778,33779,4061,4204,4205,0,0,0,0,0,0}---//绝戾华光_破刹.ext
  	TalismanSkillIDs[1812]={1812,1813,1814,33813,33814,33815,4072,4233,4234,4235,0,0,0,0,0}---//死亡波纹_千毒蛊.ext
  	TalismanSkillIDs[1815]={1815,1816,1817,33849,33850,33851,4083,4269,4270,4271,0,0,0,0,0}---//迷踪之寂_鬼罄.ext
  	TalismanSkillIDs[2037]={2037,2038,2039,33918,33919,33920,4105,4321,4322,4323,0,0,0,0,0}---//五岳咸服_翻天印.ext
  	TalismanSkillIDs[2153]={2153,2154,2155,33921,33922,33923,4106,4324,4325,4326,0,0,0,0,0}---//寒月镇潭_长明灯
  	TalismanSkillIDs[2598]={2598,2599,2600,33891,33892,33893,4096,4294,4295,4296,0,0,0,0,0}---//大凡若简_日月宝鉴
  --	TalismanSkillIDs[3088]={3088,3088,3088}---//元宵灯祝_元宵灯
  	TalismanSkillIDs[3162]={3162,3163,3164,33909,33910,33911,4102,4312,4313,4314,4315,0,0,0,0}---//圣塔降魔_浮屠
  	TalismanSkillIDs[3391]={3391,3392,3393,33924,33925,33926,4107,4327,4328,4329,4330,0,0,0,0}---//白骨之咒_引魂幡
  	TalismanSkillIDs[3825]={3825,3826,3827,33927,33928,33929,4108,4331,4332,4333,0,0,0,0,0}---//碧天宇华_宝库之魂
  	TalismanSkillIDs[32954]={32954,32955,32956,33939,33940,33941,4112,4347,4348,4349,4350,0,0,0,0}---//天琊绝世_天琊.ext
  
  	TalismanSkillIDs[3610]={3610,3611,3612,33726,33727,33728,4045,4168,4169,0,0,0,0,0,0}---//万彩画一_神裔
  	TalismanSkillIDs[3613]={3613,3614,3615,33747,33748,33749,4052,4182,4183,0,0,0,0,0,0}---//驱虎逐豹_九黎
  	TalismanSkillIDs[3616]={3616,3617,3618,33783,33784,33785,4063,4208,4209,0,0,0,0,0,0}---//枫林煞血_九黎
  	TalismanSkillIDs[3619]={3619,3620,3621,33819,33820,33821,4074,4239,4240,4241,0,0,0,0,0}---//狂澜灭杀_九黎
  	TalismanSkillIDs[3622]={3622,3623,3624,33855,33856,33857,4085,4274,4275,4276,0,0,0,0,0}---//亡魂丧魄_九黎
  	TalismanSkillIDs[3625]={3625,3626,3627,33750,33751,33752,4053,4184,4185,0,0,0,0,0,0}---//月影丹朱_烈山
  	TalismanSkillIDs[3628]={3628,3629,3630,33786,33787,33788,4064,4210,4211,0,0,0,0,0,0}---//夺魄返魂_烈山
  	TalismanSkillIDs[3631]={3631,3632,3633,33822,33823,33824,4075,4242,4243,4244,0,0,0,0,0}---//浮光阑影_烈山
  	TalismanSkillIDs[3634]={3634,3635,3636,33858,33859,33860,4086,4278,4279,4280,0,0,0,0,0}---//曦皇神威_烈山
  	TalismanSkillIDs[3943]={3943,3944,3945,33753,33754,33755,4054,4186,4187,0,0,0,0,0,0}---//血杀千里_怀光
  	TalismanSkillIDs[3946]={3946,3947,3948,33789,33790,33791,4065,4213,4214,0,0,0,0,0,0}---//横扫千军_怀光
  	TalismanSkillIDs[3949]={3949,3950,3951,33825,33826,33827,4076,4245,4246,0,0,0,0,0,0}---//破魂千劫_怀光
  	TalismanSkillIDs[3952]={3952,3953,3954,33861,33862,33863,4087,4281,4282,4283,0,0,0,0,0}---//婆娑影皇_怀光
  	TalismanSkillIDs[3955]={3955,3956,3957,33756,33757,33758,4055,4189,4190,0,0,0,0,0,0}---//琼华天籁_天华
  	TalismanSkillIDs[3958]={3958,3959,3960,33792,33793,33794,4066,4215,4216,4217,0,0,0,0,0}---//苍穹凤歌_天华
  	TalismanSkillIDs[3961]={3961,3962,3963,33828,33829,33830,4077,4247,4248,4249,0,0,0,0,0}---//宇光蓬莱_天华
  	TalismanSkillIDs[3964]={3964,3965,3966,33864,33865,33866,4088,4284,4285,4286,0,0,0,0,0}---//天魔降伏_天华
  
  	TalismanSkillIDs[33347]={33347,33348,33349,33744,33745,33746,4051,4180,4181,0,0,0,0,0,0}---//业火天阳_焚香
  	TalismanSkillIDs[33350]={33350,33351,33352,33780,33781,33782,4062,4206,4207,0,0,0,0,0,0}---//天地玄极_焚香
  	TalismanSkillIDs[33353]={33353,33354,33355,33816,33817,33818,4073,4236,4237,4238,0,0,0,0,0}---//凌火明阳_焚香
  	TalismanSkillIDs[33356]={33356,33357,33358,33852,33853,33854,4084,4272,4273,0,0,0,0,0,0}---//太幽玄冰_焚香
  	TalismanSkillIDs[33359]={33359,33360,33361,33759,33760,33761,4056,4191,4192,0,0,0,0,0,0}---//刑天怒雷_太昊
  	TalismanSkillIDs[33362]={33362,33363,33364,33795,33796,33797,4067,4218,4219,0,0,0,0,0,0}---//殇落流华_太昊
  	TalismanSkillIDs[33365]={33365,33366,33367,33831,33832,33833,4078,4250,4251,4252,0,0,0,0,0}---//子午心魂_太昊
  	TalismanSkillIDs[33368]={33368,33369,33370,33867,33868,33869,4089,4287,4288,4289,0,0,0,0,0}---//浩气云天_太昊
		TalismanSkillIDs[34015]={34015,34016,34017,33762,33763,33764,4366,4370,4371,0,0,0,0,0,0}---//辰皇一级_辰皇
		TalismanSkillIDs[34018]={34018,34019,34020,33798,33799,33800,4367,4372,4373,0,0,0,0,0,0}---//辰皇二级_辰皇
		TalismanSkillIDs[34021]={34021,34022,34023,33834,33835,33836,4368,4375,4376,4377,0,0,0,0,0}---//辰皇三级_辰皇
		TalismanSkillIDs[34024]={34024,34025,34026,33870,33871,33872,4369,4378,4379,4380,0,0,0,0,0}---//辰皇四级_辰皇
 
		TalismanSkillIDs[34662]={34662,34663,34664,34803,34804,34805,4936,4937,4938,0,0,0,0,0,0}---//灵族新手_灵族
		TalismanSkillIDs[34667]={34667,34668,34669,34807,34808,34809,4940,4941,4942,0,0,0,0,0,0}---//牵机一级_牵机
		TalismanSkillIDs[34672]={34672,34673,34674,34811,34812,34813,4944,4945,4946,0,0,0,0,0,0}---//牵机二级_牵机
		TalismanSkillIDs[34677]={34677,34678,34679,34815,34816,34817,4948,4949,4950,0,0,0,0,0,0}---//牵机三级_牵机
		TalismanSkillIDs[34682]={34682,34683,34684,34819,34820,34821,4953,4954,4955,4956,0,0,0,0,0}---//牵机四级_牵机
		TalismanSkillIDs[34687]={34687,34688,34689,34823,34824,34825,4959,4960,4961,0,0,0,0,0,0}---//英招一级_英招
		TalismanSkillIDs[34692]={34692,34693,34694,34827,34828,34829,4963,4964,4965,0,0,0,0,0,0}---//英招二级_英招
		TalismanSkillIDs[34697]={34697,34698,34699,34831,34832,34833,4967,4968,4969,0,0,0,0,0,0}---//英招三级_英招
		TalismanSkillIDs[34702]={34702,34703,34704,34835,34836,34837,4972,4973,4974,4975,0,0,0,0,0}---//英招四级_英招

 		for i,v in pairs(TalismanSkillIDs) do
  	 for k = 1,15 do
  	 	if talisman:QueryData(0,dataIndex["skill_addon_id"]) == v[k] then
  	 		HolySkillLevel = k
  	 		HolySkillAddonID = v[k+3]
  	 		HolySkillId0 = v[7]
  	 		HolySkillId1 = v[8]
  	 		HolySkillId2 = v[9]
  	 		HolySkillId3 = v[10]
  	 		HolySkillId4 = v[11]
  	 		HolySkillId5 = v[12]
  	 		HolySkillId6 = v[13]
  	 		HolySkillId7 = v[14]
  	 		HolySkillId8 = v[15]
  	 	end
  	 end 
  	end
  
  	output:UpdateData(0,dataIndex["skill_addon_id"],HolySkillAddonID)
  	output:UpdateData(0,dataIndex["skillrefine_level"],HolySkillLevel)
  	output:UpdateData(0,dataIndex["skillrefine_0"],HolySkillId0)
  	output:UpdateData(0,dataIndex["skillrefine_1"],HolySkillId1)
  	output:UpdateData(0,dataIndex["skillrefine_2"],HolySkillId2)
  	output:UpdateData(0,dataIndex["skillrefine_3"],HolySkillId3)
  	output:UpdateData(0,dataIndex["skillrefine_4"],HolySkillId4)
  	output:UpdateData(0,dataIndex["skillrefine_5"],HolySkillId5)
  	output:UpdateData(0,dataIndex["skillrefine_6"],HolySkillId6)
  	output:UpdateData(0,dataIndex["skillrefine_7"],HolySkillId7)
  	output:UpdateData(0,dataIndex["skillrefine_8"],HolySkillId8)
		output:UpdateData(0,dataIndex["reborncont"],1)

		output:InheritCombineExp(true)
		---设置法宝等级---
		local newlevel = 1
		if talisman:QueryLevel() == 31 then newlevel = 11
		elseif talisman:QueryLevel() == 32 then newlevel = 16
		elseif talisman:QueryLevel() == 33 then newlevel = 20
		elseif talisman:QueryLevel() == 34 then newlevel = 23
		elseif talisman:QueryLevel() == 35 then newlevel = 28
		elseif talisman:QueryLevel() == 36 then newlevel = 29
		elseif talisman:QueryLevel() == 37 then newlevel = 30
		else
		end

		output:SetExp(1)
		output:SetLevel(newlevel)
		ZLuaTalRebuild(output,outid)
	end		

end

--------------------------------------------------------------------
------------------法宝技能洗炼主函数--------------------------------
function ZLuaTalSkillRefine(id1,id2,talisman1,talisman2,cid,output)
	local dataIndex=ZLuaTalDataIndex()
	local talismans=ZLuaTalAvailableTalismans()
	local outid

	---没有灵媒不能洗炼技能---
	if cid == 0 then
		output:CreateItem(-1)
		return
	end

	---没飞升的法宝不能作为主法宝---
	if talisman1:QueryData(0,dataIndex["reborncont"]) == 0 then
		output:CreateItem(-1)
		return
	end	

	---mei飞升的法宝不能作为辅法宝---
	if talisman2:QueryData(0,dataIndex["reborncont"]) == 0 then
		output:CreateItem(-1)
		return
	end

  ---没注册过的法宝飞剑，禁止洗炼----------------
	local Registed1 = false
	for _,v in pairs(talismans) do
		if v.tmplID == id1 then Registed1 = true end
	end
	local Registed2 = false
	for _,v in pairs(talismans) do
		if v.tmplID == id2 then Registed2 = true end
	end
	if Registed1 == false or Registed2 == false  then
		output:CreateItem(-1)
		return
	end

 ---如果法宝ID为飞宝ID，则禁止洗炼--------------
	if talisman1:QueryData(0,dataIndex["is_fly"])==1 or talisman2:QueryData(0,dataIndex["is_fly"])==1 then
		output:CreateItem(-1)
		return
	end

 ---法宝元宵灯和玲珑塔禁止洗炼------------------
	if id1 == talismans["US_45_YuanXiaoDeng"].tmplID or id1 == talismans["US_75_LingLongTa"].tmplID or id2 == talismans["US_45_YuanXiaoDeng"].tmplID or id2 == talismans["US_75_LingLongTa"].tmplID then
		output:CreateItem(-1)
		return
	end
		
	---洗炼操作---
	if cid == 53577 or cid == 55603 then
		ZLuaTalSkillRefineRull(id1,id2,talisman1,talisman2,output)
		return
	end

 	output:CreateItem(-1)

end

----------------------------------------------------------------
------------------洗炼规则--------------------------------------
function ZLuaTalSkillRefineRull(id1,id2,talisman1,talisman2,output)

	local dataIndex=ZLuaTalDataIndex()
	local talismans=ZLuaTalAvailableTalismans()

	local inData1={}
	local Datacont1 = 0
	local Datacont2 = 0
	local inData2={}
	local OutData={0,0,0,0,0,0,0,0,}
	local AllSkillProbData={}
	local ALLSkillIDInfo={}
	local SkillSeqPro={}

--
	---技能概率库---
	AllSkillProbData={
	250,150,---//新手1级人族
	150,100,---//新手1级神族
	150,100,---//新手1级灵族

	100,100,---//门派15级法宝青云
	150,100,---//门派15级法宝鬼王
	250,250,---//门派15级法宝天音
	200,100,---//门派15级法宝合欢
	80,80,---//门派15级法宝鬼道
	80,100,---//门派15级法宝焚香
	100,100,---//门派15级法宝九黎
	80,250,---//门派15级法宝烈山
	100,100,---//门派15级法宝怀光
	250,80,---//门派15级法宝天华
	100,80,---//门派15级法宝太昊
	120,150,---//门派15级法宝辰皇
	120,150,---//门派15级法宝牵机
	120,150,---//门派15级法宝英招

	100,150,---//门派75级法宝青云
	150,150,---//门派75级法宝鬼王
	100,250,---//门派75级法宝天音
	80,100,---//门派75级法宝合欢
	80,100,---//门派75级法宝鬼道
	120,100,---//门派75级法宝焚香
	80,100,---//门派75级法宝九黎
	80,150,---//门派75级法宝烈山
	250,100,---//门派75级法宝怀光
	200,80,---//门派75级法宝天华
	100,250,---//门派75级法宝太昊
	80,120,---//门派75级法宝辰皇
	80,120,---//门派75级法宝牵机
	80,120,---//门派75级法宝英招

	100,150,100,---//门派105级法宝青云
	150,150,100,---//门派105级法宝鬼王
	250,100,150,---//门派105级法宝天音
	100,300,150,---//门派105级法宝合欢
	150,300,100,---//门派105级法宝鬼道
	100,300,150,---//门派105级法宝焚香
	80,200,250,---//门派105级法宝九黎
	200,100,80,---//门派105级法宝烈山
	100,80,---//门派105级法宝怀光
	300,150,100,---//门派105级法宝天华
	150,80,100,---//门派105级法宝太昊
	150,80,120,---//门派105级法宝辰皇
	150,80,---//门派105级法宝牵机
	150,80,---//门派105级法宝英招
	
	100,80,300,250,---//门派135级法宝青云
	150,80,80,300,---//门派135级法宝鬼王
	300,150,100,150,---//门派135级法宝天音
	150,150,150,300,---//门派135级法宝合欢
	150,150,100,---//门派135级法宝鬼道
	100,100,---//门派135级法宝焚香
	150,300,150,---//门派135级法宝九黎
	250,40,100,---//门派135级法宝烈山
	150,100,100,---//门派135级法宝怀光
	150,300,80,---//门派135级法宝天华
	150,80,100,---//门派135级法宝太昊
	60,80,80,---//门派135级法宝辰皇
	150,80,80,---//门派135级法宝牵机
	150,80,80,---//门派135级法宝英招
	
	150,---//45神木
	150,---//75山河
	150,---//75万毒
	300,---//75玉净瓶
	200,150,100,---//75新月
	300,150,200,---//105九龙
	200,100,80,---//105金葫芦
	100,100,300,---//105阴阳
	80,120,150,150,---//105落魂
	100,80,---//105逍遥
	150,100,80,80,---//105浮屠
	150,100,---//135蟠龙
	60,300,80,---//135五龙
	100,80,150,---//135混元
	80,100,150,---//135龙挂
	80,80,150,60,---//135引魂
	150,40,150,---//135无间
	30,150,80,---//135摄魂
	150,50,90,150,---//135噬血珠
	30,80,80,150,150,---//135噬魂
	80,80,30,100,---//135天琊
	}
	
	---技能ID信息库，与概率库对应----
	ALLSkillIDInfo={
	4166,4167,---//新手1级人族
	4168,4169,---//新手1级神族
	4937,4938,---//新手1级灵族

	4170,4171,---//门派15级法宝青云
	4172,4173,---//门派15级法宝鬼王
	4174,4175,---//门派15级法宝天音
	4176,4177,---//门派15级法宝合欢
	4178,4179,---//门派15级法宝鬼道
	4180,4181,---//门派15级法宝焚香
	4182,4183,---//门派15级法宝九黎
	4184,4185,---//门派15级法宝烈山
	4186,4187,---//门派15级法宝怀光
	4189,4190,---//门派15级法宝天华
	4191,4192,---//门派15级法宝太昊
	4370,4371,---//门派15级法宝辰皇
	4941,4942,---//门派15级法宝牵机
	4960,4961,---//门派15级法宝英招

	4193,4194,---//门派75级法宝青云
	4196,4197,---//门派75级法宝鬼王
	4199,4200,---//门派75级法宝天音
	4202,4203,---//门派75级法宝合欢
	4204,4205,---//门派75级法宝鬼道
	4206,4207,---//门派75级法宝焚香
	4208,4209,---//门派75级法宝九黎
	4210,4211,---//门派75级法宝烈山
	4213,4214,---//门派75级法宝怀光
	4215,4216,---//门派75级法宝天华
	4218,4219,---//门派75级法宝太昊
	4372,4373,---//门派75级法宝辰皇
	4945,4946,---//门派75级法宝牵机
	4964,4965,---//门派75级法宝英招
	
	4221,4222,4223,---//门派105级法宝青云
	4224,4225,4226,---//门派105级法宝鬼王
	4227,4228,4229,---//门派105级法宝天音
	4230,4231,4232,---//门派105级法宝合欢
	4233,4234,4235,---//门派105级法宝鬼道
	4236,4237,4238,---//门派105级法宝焚香
	4239,4240,4241,---//门派105级法宝九黎
	4242,4243,4244,---//门派105级法宝烈山
	4245,4246,---//门派105级法宝怀光
	4247,4248,4249,---//门派105级法宝天华
	4250,4251,4252,---//门派105级法宝太昊
	4375,4376,4377,---//门派105级法宝辰皇
	4949,4950,---//门派105级法宝牵机
	4968,4969,---//门派105级法宝英招
	
	4253,4254,4255,4256,---//门派135级法宝青云
	4257,4258,4259,4260,---//门派135级法宝鬼王
	4261,4262,4263,4264,---//门派135级法宝天音
	4265,4266,4267,4268,---//门派135级法宝合欢
	4269,4270,4271,---//门派135级法宝鬼道
	4272,4273,---//门派135级法宝焚香
	4274,4275,4276,---//门派135级法宝九黎
	4278,4279,4280,---//门派135级法宝烈山
	4281,4282,4283,---//门派135级法宝怀光
	4284,4285,4286,---//门派135级法宝天华
	4287,4288,4289,---//门派135级法宝太昊
	4378,4379,4380,---//门派135级法宝辰皇
	4954,4955,4956,---//门派135级法宝牵机
	4973,4974,4975,---//门派135级法宝英招
	
	4290,---//45神木
	4291,---//75山河
	4292,---//75万毒
	4293,---//75玉净瓶
	4294,4295,4296,---//75新月
	4297,4298,4299,---//105九龙
	4300,4301,4302,---//105金葫芦
	4303,4304,4305,---//105阴阳
	4306,4307,4308,4309,---//105落魂
	4310,4311,---//105逍遥
	4312,4313,4314,4315,---//105浮屠
	4316,4317,---//135蟠龙
	4318,4319,4320,---//135五龙
	4321,4322,4323,---//135混元
	4324,4325,4326,---//135龙挂
	4327,4328,4329,4330,---//135引魂
	4331,4332,4333,---//135无间
	4334,4335,4336,---//135摄魂
	4337,4338,4339,4340,---//135噬血珠
	4342,4343,4344,4345,4346,---//135噬魂
	4347,4348,4349,4350,---//135天琊
	}
	
	local transBUGskillID = {}
	----------------强ID----弱ID-调整概率-所属法宝ID
	transBUGskillID[4349]	= {4452,80,43475}
	transBUGskillID[4342]	= {4453,80,10877}
	transBUGskillID[4338]	= {4454,80,10876}
	transBUGskillID[4334]	= {4455,80,10875}
	transBUGskillID[4332]	= {4456,80,34820}
	transBUGskillID[4330]	= {4457,80,25732}
	transBUGskillID[4322]	= {4458,80,20090}
	transBUGskillID[4318]	= {4459,80,10872}

	---主法宝技能数据---
	inData1 = {
	talisman1:QueryData(0,dataIndex["skillrefine_1"]),
	talisman1:QueryData(0,dataIndex["skillrefine_2"]),
	talisman1:QueryData(0,dataIndex["skillrefine_3"]),
	talisman1:QueryData(0,dataIndex["skillrefine_4"]),
	talisman1:QueryData(0,dataIndex["skillrefine_5"]),
	talisman1:QueryData(0,dataIndex["skillrefine_6"]),
	talisman1:QueryData(0,dataIndex["skillrefine_7"]),
	talisman1:QueryData(0,dataIndex["skillrefine_8"]),
	}
	---将转义效果的的特定技能转义成原ID--
	for k=1,8 do
		for i,v in pairs(transBUGskillID) do
			if inData1[k] == v[1] then
				inData1[k] = i
			end
		end
	end
	
	---计算主法宝有效技能个数---
	for i=1,8 do
		if inData1[i]~=0 then Datacont1=Datacont1+1 end
	end

	---辅法宝技能数据---
	inData2 = {
	talisman2:QueryData(0,dataIndex["skillrefine_1"]),
	talisman2:QueryData(0,dataIndex["skillrefine_2"]),
	talisman2:QueryData(0,dataIndex["skillrefine_3"]),
	talisman2:QueryData(0,dataIndex["skillrefine_4"]),
	talisman2:QueryData(0,dataIndex["skillrefine_5"]),
	talisman2:QueryData(0,dataIndex["skillrefine_6"]),
	talisman2:QueryData(0,dataIndex["skillrefine_7"]),
	talisman2:QueryData(0,dataIndex["skillrefine_8"]),
	}

	---将转义效果的的特定技能转义成原ID--
	for k=1,8 do
		for i,v in pairs(transBUGskillID) do
			if inData2[k] == v[1] then
				inData2[k] = i
			end
		end
	end

  ---去掉重复技能数据---	
	for i=1,8 do
		for j=1,8 do
			if inData2[i]==inData1[j] and inData2[i]~=0 then
				inData2[i]=0
			end
		end
	end
	---计算辅法宝有效技能个数（去掉重复技能数据后）---
	for i=1,8 do
		if inData2[i]~=0 then Datacont2=Datacont2+1 end
	end
	---融合前临时技能数据库---	
	local BaseDataIn={}
	BaseDataIn = {
	inData1[1],
	inData1[2],
	inData1[3],
	inData1[4],
	inData1[5],
	inData1[6],
	inData1[7],
	inData1[8],
	inData2[1],
	inData2[2],
	inData2[3],
	inData2[4],
	inData2[5],
	inData2[6],
	inData2[7],
	inData2[8],
	}	
	---首次融合额外附加一定的隐藏效果奖励概率
	local firstrefine
	if talisman1:QueryData(0,dataIndex["skillrefinecont"])==1 then
		firstrefine = 1
	else
		firstrefine = 0
	end
	for i=1,16 do
		for j=1,table.getn(ALLSkillIDInfo) do
			if ALLSkillIDInfo[j]==BaseDataIn[i] then
				SkillSeqPro[i]=AllSkillProbData[j] + firstrefine*100
			else
				SkillSeqPro[i]=80	---空技能概率
			end
		end
	end

	---随机洗炼次数调整值---
	local NumChance = 0
	local a = math.random(1,100)
	local b = math.random(1,100)
	if a<math.floor(100-100*Datacont1/8) then
		NumChance=1
	elseif b<math.floor(100*Datacont1/8) then
		NumChance=-1
	else
		NumChance=0
	end
	
	---洗炼次数上限---
	local DatacontOut = Datacont1+Datacont2 +  NumChance
	if DatacontOut >= 8 then 
		DatacontOut = 8
	elseif DatacontOut < 1 then
		DatacontOut = 1
	else
	end
	
	---按概率数组随机选取技能，遇到重复技能清零---
	for i=1,DatacontOut do
		OutData[i]=BaseDataIn[ZLuaTal_Get_RanPoint(SkillSeqPro)]
		if i~=1 then
			for j=1,i-1 do
				if OutData[i] == OutData[j] and OutData[i] ~=0 then
					OutData[i]=0
				end
			end
		end
	end
	
	local OutDataRull={0,0,0,0,0,0,0,0,}
	local k = 0
	for i=1,8 do
		if OutData[i] ~= 0 then
			OutDataRull[k+1] = OutData[i]
			k = k + 1
		end
	end
	
	---将BUG技能ID转义成隐藏效果ID
	for i=1,8 do
		for k,v in pairs(transBUGskillID) do
			if OutDataRull[i] == k and v[3] ~= id1 then
				OutDataRull[i] = v[1]
			end
		end
	end

	---对于总效果数大于3个且随机效果数小于3个时的特殊处理
	local tmpOutData={}
	local tmpOutDataPro={}
	local tmp1=1
	for i=1,table.getn(BaseDataIn) do
		if BaseDataIn[i] ~= 0 then
			tmpOutData[tmp1] = BaseDataIn[i]
			tmp1=tmp1+1
		end
	end
	
	for m=1,table.getn(tmpOutData) do
		for i=1,table.getn(ALLSkillIDInfo) do
			if ALLSkillIDInfo[i] == tmpOutData[m] then
				tmpOutDataPro[m] = AllSkillProbData[i]
			end
		end
	end
	---排序后tmpOutData技能ID按照设定概率由高到低排列
	local tmpData1
	local tmpDataPro1
	for i=1,table.getn(tmpOutData)-1 do
		for j=1,table.getn(tmpOutData)-1 do
			if tmpOutDataPro[j]<tmpOutDataPro[j+1] then
				tmpDataPro1 				= tmpOutDataPro[j]
				tmpOutDataPro[j]		= tmpOutDataPro[j+1]
				tmpOutDataPro[j+1] 	= tmpDataPro1
				tmpData1 						= tmpOutData[j]
				tmpOutData[j]				= tmpOutData[j+1]
				tmpOutData[j+1]			= tmpData1
			end
		end
	end

	---将BUG技能ID转义成隐藏效果ID
	for i=1,8 do
		for k,v in pairs(transBUGskillID) do
			if tmpOutData[i] == k and v[3] ~= id1 then
				tmpOutData[i] = v[1]
			end
		end
	end

	local OutDataRullCont = 0
	for i=1,8 do
		if OutDataRull[i] ~= 0 then
			OutDataRullCont = OutDataRullCont+1
		end
	end
	
	---如果总效果只有2个，则全融合
	if Datacont1+Datacont2 == 2 then
		OutDataRull[1] = talisman1:QueryData(0,dataIndex["skillrefine_1"])
		OutDataRull[2] = talisman2:QueryData(0,dataIndex["skillrefine_1"])
	---如果总效果大于2个，且随机结果小于3个，则至少保留3个高概率效果
	elseif OutDataRullCont < 3 and Datacont1+Datacont2 > 2 then
		OutDataRull[1] = tmpOutData[1]
		OutDataRull[2] = tmpOutData[2]
		OutDataRull[3] = tmpOutData[3]
	end
	
	---赋值---	
	output:CreateItem(id1)
	for _,index in pairs(dataIndex) do
		output:UpdateData(0,index,talisman1:QueryData(0,index))
	end

	output:UpdateData(0,dataIndex["skillrefinetmp_1"],OutDataRull[1])
	output:UpdateData(0,dataIndex["skillrefinetmp_2"],OutDataRull[2])
	output:UpdateData(0,dataIndex["skillrefinetmp_3"],OutDataRull[3])
	output:UpdateData(0,dataIndex["skillrefinetmp_4"],OutDataRull[4])
	output:UpdateData(0,dataIndex["skillrefinetmp_5"],OutDataRull[5])
	output:UpdateData(0,dataIndex["skillrefinetmp_6"],OutDataRull[6])
	output:UpdateData(0,dataIndex["skillrefinetmp_7"],OutDataRull[7])
	output:UpdateData(0,dataIndex["skillrefinetmp_8"],OutDataRull[8])

	output:UpdateData(0,dataIndex["skillrefinecont"],talisman1:QueryData(0,dataIndex["skillrefinecont"])+1)

	output:InheritCombineExp(true)
	ZLuaTalRebuild(output,id1)
	return
end

-----------------------------------------------------------------
------------------法宝技能洗炼确认主函数-----------------------------------------------------------------------------
function ZluaTalSkillRefineResult(id1, talisman1, result, output)
	local dataIndex=ZLuaTalDataIndex()
	local talismans=ZLuaTalAvailableTalismans()
	local outid = id1

	if cid == 0 then
		output:CreateItem(-1)
		return
	end

---没飞升的法宝不可以洗炼-----------------------
	if talisman1:QueryData(0,dataIndex["reborncont"]) == 0 then
		output:CreateItem(-1)
		return
	end

---如果主法宝没有缓存技能效果信息则洗练失败-----
	if talisman1:QueryData(0,dataIndex["skillrefinetmp_1"]) == 0 then
		output:CreateItem(-1)
		return
	end

	output:CreateItem(outid)
	for _,index in pairs(dataIndex) do
		output:UpdateData(0,index,talisman1:QueryData(0,index))
	end

	if result == 0 then			
		output:UpdateData(0,dataIndex["skillrefinetmp_1"],0)
		output:UpdateData(0,dataIndex["skillrefinetmp_2"],0)
		output:UpdateData(0,dataIndex["skillrefinetmp_3"],0)
		output:UpdateData(0,dataIndex["skillrefinetmp_4"],0)
		output:UpdateData(0,dataIndex["skillrefinetmp_5"],0)
		output:UpdateData(0,dataIndex["skillrefinetmp_6"],0)
		output:UpdateData(0,dataIndex["skillrefinetmp_7"],0)
		output:UpdateData(0,dataIndex["skillrefinetmp_8"],0)
	elseif result == 1 then
		output:UpdateData(0,dataIndex["skillrefine_1"],talisman1:QueryData(0,dataIndex["skillrefinetmp_1"]))
		output:UpdateData(0,dataIndex["skillrefine_2"],talisman1:QueryData(0,dataIndex["skillrefinetmp_2"]))
		output:UpdateData(0,dataIndex["skillrefine_3"],talisman1:QueryData(0,dataIndex["skillrefinetmp_3"]))
		output:UpdateData(0,dataIndex["skillrefine_4"],talisman1:QueryData(0,dataIndex["skillrefinetmp_4"]))
		output:UpdateData(0,dataIndex["skillrefine_5"],talisman1:QueryData(0,dataIndex["skillrefinetmp_5"]))
		output:UpdateData(0,dataIndex["skillrefine_6"],talisman1:QueryData(0,dataIndex["skillrefinetmp_6"]))
		output:UpdateData(0,dataIndex["skillrefine_7"],talisman1:QueryData(0,dataIndex["skillrefinetmp_7"]))
		output:UpdateData(0,dataIndex["skillrefine_8"],talisman1:QueryData(0,dataIndex["skillrefinetmp_8"]))
		output:UpdateData(0,dataIndex["skillrefinetmp_1"],0)
		output:UpdateData(0,dataIndex["skillrefinetmp_2"],0)
		output:UpdateData(0,dataIndex["skillrefinetmp_3"],0)
		output:UpdateData(0,dataIndex["skillrefinetmp_4"],0)
		output:UpdateData(0,dataIndex["skillrefinetmp_5"],0)
		output:UpdateData(0,dataIndex["skillrefinetmp_6"],0)
		output:UpdateData(0,dataIndex["skillrefinetmp_7"],0)
		output:UpdateData(0,dataIndex["skillrefinetmp_8"],0)
	end

	local RefineCont = 0
	local DataSkillRefine={}
	DataSkillRefine={
	talisman1:QueryData(0,dataIndex["skillrefine_1"]),
	talisman1:QueryData(0,dataIndex["skillrefine_2"]),
	talisman1:QueryData(0,dataIndex["skillrefine_3"]),
	talisman1:QueryData(0,dataIndex["skillrefine_4"]),
	talisman1:QueryData(0,dataIndex["skillrefine_5"]),
	talisman1:QueryData(0,dataIndex["skillrefine_6"]),
	talisman1:QueryData(0,dataIndex["skillrefine_7"]),
	talisman1:QueryData(0,dataIndex["skillrefine_8"]),
	}
	for i=1,8 do
		if DataSkillRefine[i]~=0 then
			RefineCont=RefineCont+1
		end
	end
	local finalLevel
	if RefineCont == 8 then
		finalLevel = 3
	elseif RefineCont>=1 or RefineCont<8 then
		finalLevel = 2
	else
		finalLevel = 1
	end
	output:UpdateData(0,dataIndex["reborncont"],finalLevel)
	output:InheritCombineExp(true)
	ZLuaTalRebuild(output,outid)
	
end

------------------法宝技能镶嵌主函数-----------------------------------------------------------------------------
function ZLuaTalSkillAdd(id1,id2,talisman1,talisman2,cid1,cid2,output)
	local dataIndex=ZLuaTalDataIndex()
	local talismans=ZLuaTalAvailableTalismans()
	local outid = id1

	if cid == 0 then
		output:CreateItem(-1)
		return
	end

  ---没注册过的法宝飞剑，禁止洗炼----------------
	local Registed1 = false
	for _,v in pairs(talismans) do
		if v.tmplID == id1 then Registed1 = true end
	end
	local Registed2 = false
	for _,v in pairs(talismans) do
		if v.tmplID == id2 then Registed2 = true end
	end
	if Registed1 == false or Registed2 == false  then
		output:CreateItem(-1)
		return
	end

 ---如果法宝ID为飞宝ID，则禁止镶嵌--------------
	if talisman1:QueryData(0,dataIndex["is_fly"])==1 or talisman2:QueryData(0,dataIndex["is_fly"])==1 then
		output:CreateItem(-1)
		return
	end

 ---法宝元宵灯和玲珑塔禁止镶嵌------------------
	if id1 == talismans["US_45_YuanXiaoDeng"].tmplID or id1 == talismans["US_75_LingLongTa"].tmplID or id2 == talismans["US_45_YuanXiaoDeng"].tmplID or id2 == talismans["US_75_LingLongTa"].tmplID then
		output:CreateItem(-1)
		return
	end

	---主法宝必须飞升-----------------------
	if talisman1:QueryData(0,dataIndex["reborncont"]) == 0 then
		output:CreateItem(-1)
		return
	end

	---辅法宝必须为未飞升-----------------------
	if talisman2:QueryData(0,dataIndex["reborncont"]) >= 1 then
		output:CreateItem(-1)
		return
	end

	---附件秘笈必须和辅法宝配套---
	for _,v in pairs(talismans) do
		if v.tmplID == id2 then
			if v.tmplID1 ~= cid1 or v.tmplID2 ~= cid2 then
				output:CreateItem(-1)
				return
			end
		end
	end	
	
	local NumChance={120,80,80,45,15,}	---//每个位置选取的概率序列
	local addSkillIn	= {}	---//镶嵌前的技能序列
	local addSkillOut	= {}	---//镶嵌后的技能序列
	local whatskilltoadd	---//辅法宝所附带的技能即将要被镶嵌的技能
	whatskilltoadd = talisman2:QueryData(0,dataIndex["skill_addon_id"])
--------------
	local TalismanSkillIDs={}
	TalismanSkillIDs[399]={399,1278,1279,34088,34089,34259,34330,34401,}---//八卦神掌_八卦石.ext
	TalismanSkillIDs[400]={400,1292,1293,34140,34141,34261,34332,34403,}---//六合纵横_六合镜.ext
	TalismanSkillIDs[401]={401,1326,1327,34232,34233,34262,34333,34404,}---//烈焰血雨_朱雀印.ext
	TalismanSkillIDs[402]={402,1294,1295,34144,34145,34263,34334,34405,}---//法海轮回_轮回珠.ext
	TalismanSkillIDs[403]={403,1308,1309,34168,34169,34264,34335,34406,}---//摄魂夺魄_慑心铃.ext
	TalismanSkillIDs[404]={404,1298,1299,34150,34151,34273,34344,34415,}---//流星赶月_七星剑.ext
	TalismanSkillIDs[405]={405,1318,1319,34196,34197,34274,34345,34416,}---//四灵血咒_四灵幡.ext
	TalismanSkillIDs[406]={406,1284,1285,34130,34131,34275,34346,34417,}---//金刚降魔_金刚杵.ext
	TalismanSkillIDs[407]={407,1280,1281,34104,34105,34276,34347,34418,}---//刹那芳华_多情环.ext
	TalismanSkillIDs[408]={408,1306,1307,34166,34167,34310,34381,34452,}---//江山如画_山河扇.ext
	TalismanSkillIDs[409]={409,1312,1313,34172,34173,34309,34380,34451,}---//否极泰来_神木骰.ext
	TalismanSkillIDs[410]={410,1320,1321,34216,34217,34311,34382,34453,}---//万毒噬天_万毒袋(万毒鼎).ext
	TalismanSkillIDs[411]={411,1300,1301,34156,34157,34312,34383,34454,}---//玉净天华_千醉坛(玉净瓶).ext
	TalismanSkillIDs[860]={860,1282,1283,34120,34121,34285,34356,34427,}---//千绫绚舞_琥珀朱绫.ext
	TalismanSkillIDs[861]={861,1324,1325,34224,34225,34316,34387,34458,}---//阴阳相生_阴阳镜.ext
	TalismanSkillIDs[862]={862,1296,1297,34146,34147,34320,34391,34462,}---//虬龙烈炎_蟠龙幡.ext
	TalismanSkillIDs[863]={863,1286,1287,34132,34133,34315,34386,34457,}---//方寸乾坤_金葫芦.ext
	TalismanSkillIDs[864]={864,1290,1291,34136,34137,34288,34359,34430,}---//暗香无际_莲花.ext
	TalismanSkillIDs[865]={865,1302,1303,34158,34159,34286,34357,34428,}---//青光乍现_乾坤青光戒.ext
	TalismanSkillIDs[866]={866,1322,1323,34218,34219,34321,34392,34463,}---//五龙御天_五龙轮.ext
	TalismanSkillIDs[867]={867,1288,1289,34134,34135,34314,34385,34456,}---//炼狱神火_九龙神火罩.ext
	TalismanSkillIDs[868]={868,1304,1305,34164,34165,34287,34358,34429,}---//乾坤如意_如意乾坤袋.ext
	TalismanSkillIDs[869]={869,1310,1311,34170,34171,34326,34397,34468,}---//摄魂心驰_摄魂.ext
	TalismanSkillIDs[870]={870,1316,1317,34194,34195,34327,34398,34469,}---//噬血邪灵_噬血珠.ext
	TalismanSkillIDs[871]={871,1314,1315,34192,34193,34328,34399,34470,}---//噬魂无双_噬魂.ext
--	TalismanSkillIDs[1653]={1653,1654,1655,33888,33889,33890}---//宝塔镇河妖_玲珑塔.ext
	TalismanSkillIDs[1697]={1697,1698,1699,34102,34103,34300,34371,34442,}---//梦令之澜_赤金绫.ext
	TalismanSkillIDs[1700]={1700,1701,1702,34142,34143,34299,34370,34441,}---//寂灭天舞_轮回盘.ext
	TalismanSkillIDs[1703]={1703,1704,1705,34160,34161,34297,34368,34439,}---//尘光叹妙_乾坤锁.ext
	TalismanSkillIDs[1706]={1706,1707,1708,34228,34229,34298,34369,34440,}---//鬼域幽歌_幽冥鬼手_.ext
	TalismanSkillIDs[1738]={1738,1739,1740,34152,34153,34317,34388,34459,}---//华彰星陨_七星印.ext
	TalismanSkillIDs[1753]={1753,1754,1755,34220,34221,34318,34389,34460,}---//逍遥如梦_真山河扇.ext
	TalismanSkillIDs[1806]={1806,1807,1808,34222,34223,34265,34336,34407,}---//碎玉幻天_血玉骨片.ext
	TalismanSkillIDs[1809]={1809,1810,1811,34148,34149,34277,34348,34419,}---//绝戾华光_破刹.ext
	TalismanSkillIDs[1812]={1812,1813,1814,34154,34155,34289,34360,34431,}---//死亡波纹_千毒蛊.ext
	TalismanSkillIDs[1815]={1815,1816,1817,34118,34119,34301,34372,34443,}---//迷踪之寂_鬼罄.ext
	TalismanSkillIDs[2037]={2037,2038,2039,34106,34107,34322,34393,34464,}---//五岳咸服_翻天印.ext
	TalismanSkillIDs[2153]={2153,2154,2155,34092,34093,34323,34394,34465,}---//寒月镇潭_长明灯
	TalismanSkillIDs[2598]={2598,2599,2600,34162,34163,34313,34384,34455,}---//大凡若简_日月宝鉴
--	TalismanSkillIDs[3088]={3088,3088,3088}---//元宵灯祝_元宵灯
	TalismanSkillIDs[3162]={3162,3163,3164,34116,34117,34319,34390,34461,}---//圣塔降魔_浮屠
	TalismanSkillIDs[3391]={3391,3392,3393,34226,34227,34324,34395,34466,}---//白骨之咒_引魂幡
	TalismanSkillIDs[3825]={3825,3826,3827,34090,34091,34325,34396,34467,}---//碧天宇华_宝库之魂
	TalismanSkillIDs[32954]={32954,32955,32956,34214,34215,34329,34400,34471,}---//天琊绝世_天琊.ext

	TalismanSkillIDs[3610]={3610,3611,3612,34188,34189,34260,34331,34402,}---//万彩画一_神裔
	TalismanSkillIDs[3613]={3613,3614,3615,34184,34185,34267,34338,34409,}---//驱虎逐豹_九黎
	TalismanSkillIDs[3616]={3616,3617,3618,34190,34191,34279,34350,34421,}---//枫林煞血_九黎
	TalismanSkillIDs[3619]={3619,3620,3621,34180,34181,34291,34362,34433,}---//狂澜灭杀_九黎
	TalismanSkillIDs[3622]={3622,3623,3624,34186,34187,34303,34374,34445,}---//亡魂丧魄_九黎
	TalismanSkillIDs[3625]={3625,3626,3627,34174,34175,34268,34339,34410,}---//月影丹朱_烈山
	TalismanSkillIDs[3628]={3628,3629,3630,34176,34177,34280,34351,34422,}---//夺魄返魂_烈山
	TalismanSkillIDs[3631]={3631,3632,3633,34178,34179,34292,34363,34434,}---//浮光阑影_烈山
	TalismanSkillIDs[3634]={3634,3635,3636,34182,34183,34304,34375,34446,}---//曦皇神威_烈山
	TalismanSkillIDs[3943]={3943,3944,3945,34126,34127,34269,34340,34411,}---//血杀千里_怀光
	TalismanSkillIDs[3946]={3946,3947,3948,34124,34125,34281,34352,34423,}---//横扫千军_怀光
	TalismanSkillIDs[3949]={3949,3950,3951,34128,34129,34293,34364,34435,}---//破魂千劫_怀光
	TalismanSkillIDs[3952]={3952,3953,3954,34122,34123,34305,34376,34447,}---//婆娑影皇_怀光
	TalismanSkillIDs[3955]={3955,3956,3957,34208,34209,34270,34341,34412,}---//琼华天籁_天华
	TalismanSkillIDs[3958]={3958,3959,3960,34212,34213,34282,34353,34424,}---//苍穹凤歌_天华
	TalismanSkillIDs[3961]={3961,3962,3963,34210,34211,34294,34365,34436,}---//宇光蓬莱_天华
	TalismanSkillIDs[3964]={3964,3965,3966,34206,34207,34306,34377,34448,}---//天魔降伏_天华

	TalismanSkillIDs[33347]={33347,33348,33349,34114,34115,34266,34337,34408,}---//业火天阳_焚香
	TalismanSkillIDs[33350]={33350,33351,33352,34108,34109,34278,34349,34420,}---//天地玄极_焚香
	TalismanSkillIDs[33353]={33353,33354,33355,34110,34111,34290,34361,34432,}---//凌火明阳_焚香
	TalismanSkillIDs[33356]={33356,33357,33358,34112,34113,34302,34373,34444,}---//太幽玄冰_焚香
	TalismanSkillIDs[33359]={33359,33360,33361,34200,34201,34271,34342,34413,}---//刑天怒雷_太昊
	TalismanSkillIDs[33362]={33362,33363,33364,34198,34199,34283,34354,34425,}---//殇落流华_太昊
	TalismanSkillIDs[33365]={33365,33366,33367,34204,34205,34295,34366,34437,}---//子午心魂_太昊
	TalismanSkillIDs[33368]={33368,33369,33370,34202,34203,34307,34378,34449,}---//浩气云天_太昊
	TalismanSkillIDs[34015]={34015,34016,34017,34096,34097,34272,34343,34414,}---//辰皇一级_辰皇
	TalismanSkillIDs[34018]={34018,34019,34020,34100,34101,34284,34355,34426,}---//辰皇二级_辰皇
	TalismanSkillIDs[34021]={34021,34022,34023,34098,34099,34296,34367,34438,}---//辰皇三级_辰皇
	TalismanSkillIDs[34024]={34024,34025,34026,34094,34095,34308,34379,34450,}---//辰皇四级_辰皇

	TalismanSkillIDs[34662]={34662,34663,34664,34665,34666,34839,34840,34841,}---//灵族新手_灵族
	TalismanSkillIDs[34667]={34667,34668,34669,34670,34671,34842,34843,34844,}---//牵机一级_牵机
	TalismanSkillIDs[34672]={34672,34673,34674,34675,34676,34845,34846,34847,}---//牵机二级_牵机
	TalismanSkillIDs[34677]={34677,34678,34679,34680,34681,34848,34849,34850,}---//牵机三级_牵机
	TalismanSkillIDs[34682]={34682,34683,34684,34685,34686,34851,34852,34853,}---//牵机四级_牵机
	TalismanSkillIDs[34687]={34687,34688,34689,34690,34691,34854,34855,34856,}---//英招一级_英招
	TalismanSkillIDs[34692]={34692,34693,34694,34695,34696,34857,34858,34859,}---//英招二级_英招
	TalismanSkillIDs[34697]={34697,34698,34699,34700,34701,34860,34861,34862,}---//英招三级_英招
	TalismanSkillIDs[34702]={34702,34703,34704,34705,34706,34863,34864,34865,}---//英招四级_英招

--	
	local transskilladd
	for i,v in pairs(TalismanSkillIDs) do
		for j=1,3 do
			if whatskilltoadd == v[j] then
				transskilladd = v[j+5]
			end
		end
	end
--	
	
	addSkillIn={
	talisman1:QueryData(0,dataIndex["newaddon_id_1"]),
	talisman1:QueryData(0,dataIndex["newaddon_id_2"]),
	talisman1:QueryData(0,dataIndex["newaddon_id_3"]),
	talisman1:QueryData(0,dataIndex["newaddon_id_4"]),
	talisman1:QueryData(0,dataIndex["newaddon_id_5"]),
	}
	
	---有重复类型技能镶嵌失败
	for i,v in pairs(TalismanSkillIDs) do
		for k=1,5 do
			for m=6,8 do
				if addSkillIn[k] == v[m] and ( transskilladd==v[6] or transskilladd==v[7] or transskilladd==v[8] ) then
					output:CreateItem(-1)
					return
				end
			end
		end
	end
	
	local pos = ZLuaTal_Get_RanPoint(NumChance)
	for i=1,5 do
		if i == pos then
			addSkillOut[i] = transskilladd
		else
			addSkillOut[i] = addSkillIn[i]
		end
	end

	local addSkillOutRull={0,0,0,0,0,}
	local k = 0
	for i=1,5 do
		if addSkillOut[i] ~= 0 then
			addSkillOutRull[k+1] = addSkillOut[i]
			k=k+1
		end
	end
	
	output:CreateItem(outid)
	for _,index in pairs(dataIndex) do
		output:UpdateData(0,index,talisman1:QueryData(0,index))
	end
	output:UpdateData(0,dataIndex["newaddon_id_1"],addSkillOutRull[1])
	output:UpdateData(0,dataIndex["newaddon_id_2"],addSkillOutRull[2])
	output:UpdateData(0,dataIndex["newaddon_id_3"],addSkillOutRull[3])
	output:UpdateData(0,dataIndex["newaddon_id_4"],addSkillOutRull[4])
	output:UpdateData(0,dataIndex["newaddon_id_5"],addSkillOutRull[5])
	output:InheritCombineExp(true)
	ZLuaTalRebuild(output,outid)

end

------------------抽象的选取数字函数-------------------------------------------------------------------------
---从ilist中按照vlist规定的概率选取数字，且一一对应klist中的数字，每种数字最多出现maxsamcol次
function ZLuaCheckOutNums(ilist,vlist,klist,num,maxsamecol)
	if table.getn(ilist)~=table.getn(vlist) then return end
	if table.getn(ilist)<num then return end 
	
	local TheDesk={}	---已经被选取的数编号
	local TheHand={}	---返回的数字序列
	local TheExInfo={}	---返回的额外对应序列
	for i=1,num do
		TheDesk[i]=0
		TheHand[i]=0
	end 
	local findsamenum
	
	for j=1,num do 
			TheDesk[j]=ZLuaTal_Get_RanPoint(vlist)
			findsamenum=0
			for k=1,j do
					if TheDesk[k]==TheDesk[j] then 
						findsamenum=findsamenum+1
							if findsamenum>=maxsamecol then 
							vlist[TheDesk[j]]=0 
						end
					end
			end
			TheHand[j]=ilist[TheDesk[j]]
			TheExInfo[j]=klist[TheDesk[j]]
	end 
	return TheHand,TheExInfo
end 
---------抽象的插入数字函数------------------------------------------------------------------------------------
---将insertid插入ilist中,vlist为ilist一一对应序列
function ZLuaTalisman_InsertSkillSeq(insertid,ilist,vlist,talismanskillprof)
	local insertid_num=0  ---//ilist中同insertid相同的数字数量
	for i=1,table.getn(ilist) do
		if insertid==ilist[i] then
			insertid_num=insertid_num+1
		end 
	end 	
	
	local insertcase=ZLuaTalismanSkillsExInfo(insertid,insertid_num)
	---//获得插入技能的结果类型
	local SkillsExLists=ZLuaTalismanEnchantSkillEx()
	
	local differentskillreplacechance={} 	---附加人物技能中不同于需添加技能构成的序列
	local differentskillposition={}				---附加人物技能中不同于需添加技能需求级别构成的序列
	local IsFindDifferentIDs=0						
	
	for i,v in pairs(SkillsExLists) do
		for j=1,table.getn(ilist) do 
			if ilist[j]==i and ilist[j]~=0 and ilist[j]~=insertid then 
				table.insert(differentskillreplacechance,v.replacechance)
				table.insert(differentskillposition,j)
				IsFindDifferentIDs=1
			end 
		end 
	end 
	
	if insertcase==1 and IsFindDifferentIDs==1 then 			---//成功插入至序列
		local c=ZLuaTal_Get_RanPoint(differentskillreplacechance)
		ilist[differentskillposition[c]]=insertid
		vlist[differentskillposition[c]]=SkillsExLists[insertid].reqlevel
	elseif insertcase==2  then		---//失败但维持序列,do nothing
																		
	elseif insertcase==3 and IsFindDifferentIDs==1 then 	---//失败且随机更换
		local k=math.ceil(math.random(table.getn(ilist)))
		local FailSkillList={}
			FailSkillList[1]={337,338,339,340,341,342,343}	---//鬼王
			FailSkillList[2]={337,338,354,355,356,357,358}	---//合欢
			FailSkillList[3]={337,338,369,370,371,372,373}	---//青云
			FailSkillList[4]={337,338,384,385,386,387,388}	---//天音
			FailSkillList[5]={337,338,1758,1759,1760,1761,1762}	---//鬼道
			FailSkillList[6]={337,338,33387,33388,33389,33390,33391}	---//焚香
			FailSkillList[7]={3510,3511,3512,3513,3514,3515,3516}	---//九黎
			FailSkillList[8]={3534,3535,3536,3537,3538,3539,3540}	---//烈山
			FailSkillList[9]={3972,3973,3974,3975,3976,3977,3978}	---//怀光
			FailSkillList[10]={3996,3997,3998,3999,4000,4001,4002}	---//天华
			FailSkillList[11]={33411,33412,33413,33414,33415,33416,33417}	---//太昊
			FailSkillList[12]={34035,34036,34037,34038,34039,34040,34041}	---//辰皇
			FailSkillList[13]={34707,34708,34709,34710,34711,34712,34713}	---//牵机
			FailSkillList[14]={34731,34732,34733,34734,34735,34736,34737}	---//英招
			FailSkillList[99]={337,338,337,338,337,338,337}	---//通用
		local replaceskill=FailSkillList[talismanskillprof][math.ceil(math.random(7))]
					
			if ilist[k]~=0 then 
				ilist[k]=replaceskill or 337
				vlist[k]=SkillsExLists[replaceskill].reqlevel or 1
			end 		
		---///337为重击类需要替换的技能
	elseif insertcase==4 then																---//失败(因为已经达到最大同类技能数)
		ilist=nil
		vlist=nil
	else 
		return ilist,vlist
	end 
	return ilist,vlist
end 

---------------------------------------------------------------------------------------
-- 法宝档位上限提升 -------------------------------------------------------------------
function ZLuaTalisman_Rise(id,talisman1,cid,output)
	local dataIndex = ZLuaTalDataIndex()
	local maxlevel=10
	local whattorise=ZLuaTalisman_chooserise(cid)
	local currenlevel=talisman1:QueryData(0,whattorise) or 0
	local totalgrade=talisman1:QueryData(0,dataIndex["all_rise"]) or 0

	if currenlevel<maxlevel then 
		output:CreateItem(id)
		for _,index in pairs(dataIndex) do
			output:UpdateData(0,index,talisman1:QueryData(0,index))
		end
		output:UpdateData(0,whattorise,currenlevel+1)
		output:UpdateData(0,dataIndex["all_rise"],totalgrade+1)
		output:InheritCombineExp(true)
		ZLuaTalRebuild(output,id)
	else
		output:CreateItem(-1)
	end
end
---------------------------------------------------------------------------------------
-- 法宝主属性二次强化 -----------------------------------------------------------------
function ZLuaTalisman_Rise2(id,talisman1,cid,output)
	local dataIndex = ZLuaTalDataIndex()
	local whattorise, valueCap, valueAdd, chanceLowCap, chanceHigCap, chance
	local current
	local talLevel = talisman1:QueryLevel()
	
	if talLevel > 35 then talLevel = 35 end
	
	if cid == 23860 then 
		whattorise = dataIndex["hp_enhance"]
		valueCap = 200 * talLevel
		valueAdd = 10
		chanceLowCap = 0.15
	elseif cid == 23861 then 
		whattorise = dataIndex["mp_enhance"] 
		valueCap = 200 * talLevel
		valueAdd = 10
		chanceLowCap = 0.10
	elseif cid == 23862 then 
		whattorise = dataIndex["atk_enhance"]
		valueCap = 30 * talLevel
		valueAdd = 2
		chanceLowCap = 0.10
	elseif cid == 11784 then 
		whattorise = dataIndex["hp_enhance"]
		valueCap = 200 * talLevel
		valueAdd = 7000
		chanceLowCap = 0.15
	elseif cid == 12021 then 
		whattorise = dataIndex["mp_enhance"]
		valueCap = 200 * talLevel
		valueAdd = 7000
		chanceLowCap = 0.15
	elseif cid == 19716 then 
		whattorise = dataIndex["atk_enhance"]
		valueCap = 30 * talLevel
		valueAdd = 1050
		chanceLowCap = 0.10

	end
	
	current = talisman1:QueryData(0,whattorise) or 0
	if 0 <= current and current < valueCap then
		output:CreateItem(id)
		for _,index in pairs(dataIndex) do
			output:UpdateData(0,index,talisman1:QueryData(0,index))
		end
		chance = 1.0 - 0.002*current/valueAdd
		chance = math.max(chance,chanceLowCap)
		chance = math.min(chance,1.0)
		if math.random() <= chance then
			output:UpdateData(0,whattorise,math.min(current+valueAdd,valueCap))
		end
		output:InheritCombineExp(true)
		ZLuaTalRebuild(output,id)
	else
		output:CreateItem(-1)
	end
end
---------------------------------------------------------------------------------------
----------------------根据灵媒种类选择扩容项-------------------------------------------
function ZLuaTalisman_chooserise(cid)
	local dataIndex = ZLuaTalDataIndex()
	if cid==19512 then return 83 end
	if cid==19513 then return 84 end
	if cid==19514 then return 85 end
	if cid==19515 then return 86 end
	if cid==19516 then return 87 end
	if cid==19517 then return 88 end
	if cid==19518 then return 89 end
	if cid==19519 then return 90 end
	if cid==19520 then return 91 end
	return 0
end
-- 飞剑品质概率提升 -------------------------------------------------------------------
function ZLuaTalisman_ImproveFlyerQuality(id, talisman1, cid, output)
	local dataIndex=ZLuaTalDataIndex()
	local SkillNoSeq={}				
	local SkillReqLvlSeq={}
	local skills_amount=0
	local fail=0
	

	-- 赠送类的时限飞剑不能灌魔提升品质
	local talismans = ZLuaTalAvailableTalismans()
	local timeLimit = true
	for i,v in pairs(talismans) do
		if v.tmplID == id and v.timeLimit == false then
			timeLimit = false
		end
	end
	if timeLimit == true then
--		return false
	end

	SkillNoSeq[1]=talisman1:QueryData(0,dataIndex["addon_1_id"])
	SkillReqLvlSeq[1]=talisman1:QueryData(0,dataIndex["addon_1_learn_level"])
	SkillNoSeq[2]=talisman1:QueryData(0,dataIndex["addon_2_id"])
	SkillReqLvlSeq[2]=talisman1:QueryData(0,dataIndex["addon_2_learn_level"])
	SkillNoSeq[3]=talisman1:QueryData(0,dataIndex["addon_3_id"])
	SkillReqLvlSeq[3]=talisman1:QueryData(0,dataIndex["addon_3_learn_level"])
	SkillNoSeq[4]=talisman1:QueryData(0,dataIndex["addon_4_id"])
	SkillReqLvlSeq[4]=talisman1:QueryData(0,dataIndex["addon_4_learn_level"])
	SkillNoSeq[5]=talisman1:QueryData(0,dataIndex["addon_5_id"])
	SkillReqLvlSeq[5]=talisman1:QueryData(0,dataIndex["addon_5_learn_level"])
	SkillNoSeq[6]=talisman1:QueryData(0,dataIndex["addon_6_id"])
	SkillReqLvlSeq[6]=talisman1:QueryData(0,dataIndex["addon_6_learn_level"])

	for i = 1,6 do
		if SkillNoSeq[i] > 0 then
			skills_amount = skills_amount + 1
		end
	end

	output:CreateItem(id)
	for _,index in pairs(dataIndex) do
		output:UpdateData(0,index,talisman1:QueryData(0,index))
	end

 	local r1 = math.random()
 	local r2 = math.random()
	local Mchoise = {}
  local RSkillExInfo_1={1922,1923,1924,1925,1926,1927,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	local RSkillExInfo_2={}    ----用于返回的附加属性ID序列
	local RSkillReqInfo={}     ----法宝技能需求法宝级别信息

	local Skill_AddonInfo={1922,1923,1924,1925,1926,1927}
	local	Skill_ReqLvlInfo={1,10,15,20,25,30}

	-- 只提升不降低方式
	if skills_amount < 4 then
		if r1 > 0.9 and r2 > 0.5 then
			Mchoise = {2,3,4,6}
		else
			Mchoise = {2,4,6}
		end
	elseif skills_amount == 4 then
		if r1 > 0.9 and r2 > 0.8 then
			Mchoise = {2,3,4,5,6}
		else
			Mchoise = {2,3,4,6}
		end
	elseif skills_amount > 4 then
		return false
	end

	-- 重新随机生成方式
--	if r1 > 0.9 and r2 > 0.9 then
--		Mchoise = {2,3,4,5,6}
--	elseif r1 > 0.9 then
--		Mchoise = {2,3,4,6}
--	else
--		Mchoise = {2,4,6}
--	end

	for mm=1,table.getn(Mchoise) do
    if Mchoise[mm]==65536 then
   		RSkillExInfo_2[mm]=0
			RSkillReqInfo[mm]=0
		else
			RSkillExInfo_2[mm]=RSkillExInfo_1[Mchoise[mm]]
			-- if table.getn(Skill_AddonInfo)~=table.getn(Skill_ReqLvlInfo) then return end
			RSkillReqInfo[mm]= Skill_ReqLvlInfo[ZLuaTal_FindSkillReqInfo(RSkillExInfo_2[mm],Skill_AddonInfo)]
		end
		-- if RSkillReqInfo[mm]>MaxPosLvl then RSkillReqInfo[mm]=MaxPosLvl end
	end

	-- local addonIDs,addonLevels
	-- addonIDs, addonLevels = ZLuaTal_SkillMakeSeq(RSkillExInfo_2, RSkillReqInfo)

	output:UpdateData(0,dataIndex["addon_1_id"],RSkillExInfo_2[1]or 0)		
	output:UpdateData(0,dataIndex["addon_1_learn_level"],RSkillReqInfo[1]or 0)
	output:UpdateData(0,dataIndex["addon_2_id"],RSkillExInfo_2[2]or 0)		
	output:UpdateData(0,dataIndex["addon_2_learn_level"],RSkillReqInfo[2]or 0)
	output:UpdateData(0,dataIndex["addon_3_id"],RSkillExInfo_2[3]or 0)		
	output:UpdateData(0,dataIndex["addon_3_learn_level"],RSkillReqInfo[3]or 0)
	output:UpdateData(0,dataIndex["addon_4_id"],RSkillExInfo_2[4]or 0)		
	output:UpdateData(0,dataIndex["addon_4_learn_level"],RSkillReqInfo[4]or 0)
	output:UpdateData(0,dataIndex["addon_5_id"],RSkillExInfo_2[5]or 0)		
	output:UpdateData(0,dataIndex["addon_5_learn_level"],RSkillReqInfo[5]or 0)
	output:UpdateData(0,dataIndex["addon_6_id"],RSkillExInfo_2[6]or 0)		
	output:UpdateData(0,dataIndex["addon_6_learn_level"],RSkillReqInfo[6]or 0)

	--//更新新的法宝技能属性信息		
	--output:UpdateData(0,dataIndex["skill_addon_id"],TalismanSkillID)

	--//设定法宝的所有附加人物技能可见
	--if SkillHighLvlVisible==1 then 
	--	output:UpdateData(0,dataIndex["skill_visible"],SkillHighLvlVisible)
	--end 

	output:InheritCombineExp(true)
	ZLuaTalRebuild(output,id)
end
