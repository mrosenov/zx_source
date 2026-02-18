--------------------------------------------------------------------------------------
---程序调用的服务器端变身脚本包含1、magic_interface.lua 2、magic_Refine.lua 3、transform_interface.lua
---magic_inertface.lua包含处理幻灵石相关数值生成及功能相关函数
---magic_Refine.lua包含幻灵石炼化相关处理函数
---transform_interface.lua处理变身附加的技能及技能编辑器与任务编辑器所引用的变身属性生成

--------------------------------------------------------------------------------------

---可以从服务器端查询到的人物属性
---obj.GetLevel                         player等级                                   
---obj.GetHP                            player气血                                   
---obj.GetMP                            player真气                                   
---obj.GetDamage                        player攻击（最低和最高）                                   
---obj.GetDefense                       player防御                                   
---obj.GetAttack                        player普攻命中
---                                      
---obj.GetArmor                         player普攻躲闪                                   
---obj.GetCrit                          player致命                                
---obj.GetSkillRate                     player技能命中                                   
---obj.GetAntiCrit                      player减致命率                                   
---obj.GetResistance                    player抗性

--------------------------------------------------------------------------------------

---和程序约定的对象接口
---QueryLevel 		                    查询幻灵石强化等级
---SetLevel 		                      设置幻灵石强化等级
---QueryExp 		                      查询幻灵石契合值当前值
---SetExp 		                        设置幻灵石契合值当前值
---QueryMaxExp 		                    查询幻灵石契合值最大值
---SetMaxExp 		                      设置幻灵石契合值最大值
---QueryExpLevel                      查询幻灵石契合等级
---SetExpLevel	  	                  设置幻灵石契合等级
---SetExpSpeed	  	                  设置幻灵石契合值增加速度

---QueryData 	    	                  查询幻灵石数据
---GetDataCount 		                  获取数据项
---UpdateData  		                    更新内部数据
---ClearData  		                    清空内部数据

---AddReputation(index,value)         设置提升的声望（只对Index=12设置：变身御灵值）
---SetAddon 		                      设置Addon
---SetSkill(id,level) 		            设置技能
---SetDuration 		                    设置灵气值当前值
---SetMaxDuration 	                  设置灵气值最大值
---SetQuality 		                    设置品级
---SetHP 		                          设置HP
---SetMP 		                          设置MP
---SetDamage 	    	                  设置伤害
---SetAttack	    	                  设置命中
---SetDefense	    	                  设置防御
---SetArmor		                        设置躲闪
---SetResistance  	                  设置抗性

---SetAddon(addonID)设置幻灵石附加属性，括号中为模板中的附加属性ID

---SetSkill(skillID,skillLevel)设置人物附加技能

---相关函数说明---------------------------------------------------------
---定义索引和固定数据
---ZLuaAvailableMagics()变身属性模板ID
---ZLuaExpValue()幻灵石每级契合值上限
---ZLuaExpIncreaseSpeed()幻灵石每级契合值增长速度
---ZLuaMagicLevelupProbability()幻灵石各级别固有提升概率
---ZLuaMagicDataIndex()幻化属性Data索引数组


---主要处理函数
---MagicInit()幻灵石初始化
---MagicExplevelup()幻灵石提升契合等级
---ZLuaMagicRebuild()幻灵石属性重新计算







-----------------------------------------------------------------------------------------------------------------
---定义索引
---已有的变身属性模板ID
function ZLuaAvailableMagics()
  local magics
  magics = {}
  
  ---测试用
  magics["TEST1"]               	= { magType = "item", tmplID = 27600, requirelevel = 20,  name = "变身道具测试" }
  magics["TEST2"]               	= { magType = "item", tmplID = 29899, requirelevel = 15,  name = "变身道具测试2" }
  magics["TEST3"]               	= { magType = "item", tmplID = 30936, requirelevel = 15,  name = "变身道具测试3" }
  magics["TEST4"]               	= { magType = "item", tmplID = 52761, requirelevel = 15,  name = "变身道具测试3" }
  magics["TEST5"]               	= { magType = "item", tmplID = 52762, requirelevel = 15,  name = "变身道具测试3" }
  magics["TEST6"]               	= { magType = "item", tmplID = 52763, requirelevel = 15,  name = "变身道具测试3" }
  magics["TEST7"]               	= { magType = "item", tmplID = 52764, requirelevel = 15,  name = "变身道具测试3" }

  ---回归奖励限时变身道具
  magics["paisong1"]            	= { magType = "item", tmplID = 31299, requirelevel = 5,  name = "幻灵石·雪魄女妖（限时）" }
  
  ---未使用
  magics["paisong2"]            	= { magType = "item", tmplID = 31768, requirelevel = 5,  name = "幻灵石·死泽铁鳄（限时）" }
  magics["paisong3"]            	= { magType = "item", tmplID = 31769, requirelevel = 5,  name = "幻灵石·狐妖（限时）" }

  ---充值奖励熊猫
  magics["daoxuan"]             	= { magType = "item", tmplID = 32105, requirelevel = 5,  name = "幻灵石·道玄（充值赠送）" }
  magics["daoxuan2"]             	= { magType = "item", tmplID = 40566, requirelevel = 5,  name = "幻灵石·道玄（台湾）" }
  
  ---奖励用葫芦娃
  magics["huluwa"]            	 	= { magType = "item", tmplID = 44632, requirelevel = 5,  name = "幻灵石·葫芦娃" }

  ---奖励用龙神20120221
  magics["longshen"]           	 	= { magType = "item", tmplID = 51348, requirelevel = 5,  name = "幻灵石·龙神" }

  ---2011跨服PK奖励蚩尤真身
  magics["chiyouzhenshen"]       	= { magType = "item", tmplID = 49536, requirelevel = 5,  name = "幻灵石·蚩尤" }

  ---国庆中秋活动用
  magics["guoqing01"]           	= { magType = "item", tmplID = 32108, requirelevel = 5,  name = "幻灵石·国庆01" }
  magics["guoqing02"]           	= { magType = "item", tmplID = 32109, requirelevel = 5,  name = "幻灵石·国庆02" }
  magics["guoqing03"]           	= { magType = "item", tmplID = 32110, requirelevel = 5,  name = "幻灵石·国庆03" }

  ---重阳活动用
  magics["chongyang"]           	= { magType = "item", tmplID = 32218, requirelevel = 5,  name = "幻灵石·菊花妖媚" }
	
  ---台历活动用
  magics["xueguoteshi"]          	= { magType = "item", tmplID = 55540, requirelevel = 5,  name = "幻灵石·雪国特使" }

	---新浪小浪人
  magics["xiaolangren"]          	= { magType = "item", tmplID = 54656, requirelevel = 5,  name = "幻灵石·小浪人" }
	
	---良缘火岩火娃蝰蛇
  magics["huoyan"]          			= { magType = "item", tmplID = 55119, requirelevel = 5,  name = "幻灵石·火岩" }
  magics["huowa"]         			 	= { magType = "item", tmplID = 55120, requirelevel = 5,  name = "幻灵石·火娃" }
  magics["kuishe"]         			 	= { magType = "item", tmplID = 55195, requirelevel = 5,  name = "幻灵石·蝰蛇" }
  magics["guilifei"]         		 	= { magType = "item", tmplID = 55443, requirelevel = 5,  name = "幻灵石·鬼厉肥" }
	
  ---雪人活动用
  magics["xueren01"]             	= { magType = "item", tmplID = 32911, requirelevel = 5,  name = "幻灵石·雪人" }
  magics["xueren02"]             	= { magType = "item", tmplID = 32924, requirelevel = 5,  name = "幻灵石·雪人" }
  magics["xueren03"]             	= { magType = "item", tmplID = 32925, requirelevel = 5,  name = "幻灵石·雪人" }
  magics["xueren04"]             	= { magType = "item", tmplID = 32926, requirelevel = 5,  name = "幻灵石·雪人" }

  ---三界乱斗BOSS形象
  magics["sanjie01"]		         	= { magType = "item", tmplID = 32954, requirelevel = 50,  name = "幻灵石·三界BOSS01" }
  magics["yueshen"]		         		= { magType = "item", tmplID = 42972, requirelevel = 120, name = "幻灵石·月神" }
  magics["shouhuzhe"]		       		= { magType = "item", tmplID = 52736, requirelevel = 120, name = "幻灵石·异界守护者" }
  magics["shennong"]		         	= { magType = "item", tmplID = 55181, requirelevel = 50,  name = "幻灵石·神农" }
  
  ---第一版NPC出售限时变身道具  
  magics["huamei"]             		= { magType = "item", tmplID = 31276, requirelevel = 5,  name = "幻灵石·花魅" }
  magics["hanfei"]              	= { magType = "item", tmplID = 31277, requirelevel = 5,  name = "幻灵石·悍匪" }
  magics["linghou"]     	    		= { magType = "item", tmplID = 31278, requirelevel = 5,  name = "幻灵石·灵猴" }
  magics["yingchifeng"]        		= { magType = "item", tmplID = 31503, requirelevel = 5,  name = "幻灵石·硬翅蜂" }
  magics["huayao"]              	= { magType = "item", tmplID = 31744, requirelevel = 5,  name = "幻灵石·花妖" }
  magics["guidaoshi"]            	= { magType = "item", tmplID = 31745, requirelevel = 5,  name = "幻灵石·鬼道士" }

  ---2010春节五福虎
  magics["wufuhu01"]		         	= { magType = "item", tmplID = 33795, requirelevel = 30,  name = "幻灵石·金虎" }
  magics["wufuhu02"]		         	= { magType = "item", tmplID = 33796, requirelevel = 30,  name = "幻灵石·木虎" }
  magics["wufuhu03"]		         	= { magType = "item", tmplID = 33797, requirelevel = 30,  name = "幻灵石·水虎" }
  magics["wufuhu04"]		         	= { magType = "item", tmplID = 33798, requirelevel = 30,  name = "幻灵石·火虎" }
  magics["wufuhu05"]		         	= { magType = "item", tmplID = 33799, requirelevel = 30,  name = "幻灵石·土虎" }
  magics["pengzhuang1"]		        = { magType = "item", tmplID = 54095, requirelevel = 30,  name = "幻灵石·萌鬼厉" }
  magics["pengzhuang2"]		        = { magType = "item", tmplID = 54096, requirelevel = 30,  name = "幻灵石·萌雪琪" }
  magics["pengzhuang3"]		        = { magType = "item", tmplID = 54097, requirelevel = 30,  name = "幻灵石·萌碧瑶" }
       
  ---2010-6-公会VIP礼包奖励
  magics["2010VIPman"]		       	= { magType = "item", tmplID = 38899, requirelevel = 5,  name = "幻灵石·VIP男" }
  magics["2010VIPwoman"]		     	= { magType = "item", tmplID = 38900, requirelevel = 5,  name = "幻灵石·VIP女" }

  magics["kuafushoushen"]		     	= { magType = "item", tmplID = 41926, requirelevel = 5,  name = "幻灵石·兽神" }
	
	---2012跨服PK奖励
  magics["kuafutianjienan"]		   	= { magType = "item", tmplID = 52632, requirelevel = 5,  name = "幻灵石·羽华" }
  magics["kuafutianjienv"]		   	= { magType = "item", tmplID = 52633, requirelevel = 5,  name = "幻灵石·玉宁子" }

  ---2011白色情人节情心幻灵石
  magics["qingxin"]		   			  	= { magType = "item", tmplID = 42885, requirelevel = 5,  name = "幻灵石·情心" }

  magics["2012VIPhuolong"]		   	= { magType = "item", tmplID = 51125, requirelevel = 5,  name = "幻灵石·火龙" }

  ---2012老玩家回归
  magics["2012huigui01"]		     	= { magType = "item", tmplID = 51781, requirelevel = 5,  name = "幻灵石·回归01" }
  magics["2012huigui02"]		     	= { magType = "item", tmplID = 51782, requirelevel = 5,  name = "幻灵石·回归02" }
  magics["2012huigui03"]		     	= { magType = "item", tmplID = 51783, requirelevel = 5,  name = "幻灵石·回归03" }
  magics["2012huigui04"]		     	= { magType = "item", tmplID = 51784, requirelevel = 5,  name = "幻灵石·回归04" }
  magics["2012huigui05"]		     	= { magType = "item", tmplID = 51785, requirelevel = 5,  name = "幻灵石·回归05" }
  magics["2012huigui06"]		     	= { magType = "item", tmplID = 51786, requirelevel = 5,  name = "幻灵石·回归06" }

  magics["yyxiongx"]		  	 	  	= { magType = "item", tmplID = 51558, requirelevel = 5,  name = "幻灵石·YY熊男" }
  magics["yyxiongy"]		  		   	= { magType = "item", tmplID = 51563, requirelevel = 5,  name = "幻灵石·YY熊女" }
  magics["zhouyixian"]		  	   	= { magType = "item", tmplID = 51564, requirelevel = 5,  name = "幻灵石·周一仙" }
  magics["feitianxianzi"]		     	= { magType = "item", tmplID = 51565, requirelevel = 5,  name = "幻灵石·飞天仙子" }
  magics["gongyuanyang"]		     	= { magType = "item", tmplID = 55477, requirelevel = 5,  name = "幻灵石·公鸳鸯" }
  magics["muyuanyang"]		    	 	= { magType = "item", tmplID = 55481, requirelevel = 5,  name = "幻灵石·母鸳鸯" }
  magics["kushuangtianshi"]		    = { magType = "item", tmplID = 55967, requirelevel = 5,  name = "幻灵石·酷爽天师" }
  magics["kushuangtianshi2"]		  = { magType = "item", tmplID = 55973, requirelevel = 5,  name = "幻灵石·酷爽天师" }
  magics["zhangyu"]		  					= { magType = "item", tmplID = 61069, requirelevel = 5,  name = "幻灵石·章鱼" }

  magics["caishen"]		  	   			= { magType = "item", tmplID = 58574, requirelevel = 5,  name = "幻灵石·财神" }
  magics["nantong"]		  	   			= { magType = "item", tmplID = 58575, requirelevel = 5,  name = "幻灵石·男童" }
  magics["nvtong"]		  	   			= { magType = "item", tmplID = 58576, requirelevel = 5,  name = "幻灵石·女童" }
  
  magics["xiaobai2013"]		  	   		= { magType = "item", tmplID = 61166, requirelevel = 5,  name = "幻灵石·小白" }
  magics["guili2013"]		  	   			= { magType = "item", tmplID = 61167, requirelevel = 5,  name = "幻灵石·鬼厉" }
  magics["xueqi2013"]		  	   			= { magType = "item", tmplID = 61168, requirelevel = 5,  name = "幻灵石·雪琪" }
  magics["biyao2013"]		  	   			= { magType = "item", tmplID = 61169, requirelevel = 5,  name = "幻灵石·碧瑶" }

  magics["2013dijun"]		  	   			= { magType = "item", tmplID = 60467, requirelevel = 5,  name = "幻灵石·帝俊" }

  return magics   
end




---幻灵石每级契合值上限--- MAX_EXP_VALUE = 65535,
function ZLuaExpValue()
  local expValue = {
  60,90,150,240,0,                    		     	---1分、1.5分、2.5分、4分
  120,180,300,480,0,                          	---2,3,5,8
  240,360,600,960,0,                          	---4,6,10,16
  600,900,1500,2400,0,                          ---10,15,25,40
  1200,1800,3000,4800,0,                      	---20,30,50,80

  1800,2700,4500,7200,0,                 	    	---30,45,75,120
  2400,3600,6000,9600,0,                 	    	---40,60,100,160
  3600,5400,9000,14400,0,                     	---60,90,150,240
  4800,7200,12000,19200,0,                    	---80,120,200,320
  6000,9000,15000,24000,0,                    	---100,150,250,400
---以下未增加，暂为10级数值
  6000,9000,15000,24000,0,                 	  	---160,240,400,640
  6000,9000,15000,24000,0,                    	---240,360,600,960
  6000,9000,15000,24000,0,                    	---300,450,750,1200
  6000,9000,15000,24000,0,                     	---400,600,1000,1600
  6000,9000,15000,24000,0,                    	---500,750,1250,2000

  6000,9000,15000,24000,0,                		 	---800,1200,2000,3200
  6000,9000,15000,24000,0,                 	  	---1200,1800,3000,4800
  6000,9000,15000,24000,0,                     	---1600,2400,4000,6400
  6000,9000,15000,24000,0,                     	---2000,3000,5000,8000
  6000,9000,15000,24000,0,                    	---2500,3750,6250,10000
  }
  
  return expValue
end

---幻灵石每级灵气值
function ZLuaDuration()
  local duration ={
  10,20,30,40,50,
  60,60,70,70,80,
  80,80,90,90,100,  
  100,100,110,110,120,  
  120,120,130,130,140,  

  140,140,150,150,160,  
  160,160,170,170,180,  
  180,180,190,190,200,  
  200,200,210,210,220,  
  220,220,230,230,240,  

  240,240,250,250,260,  
  260,260,270,270,280,  
  280,280,290,290,300,  
  300,300,310,310,320,  
  320,320,330,330,340,  

  340,340,350,350,360,  
  360,360,370,370,380,  
  380,380,390,390,400,  
  400,400,410,410,420,  
  420,420,430,430,500
  }
  return duration
end



---幻灵石各强化等级和契合等级下的契合值增长速度（int单位：number/m）        MAX_EXP_SPEED = 128,
function ZLuaExpIncreaseSpeed()
  local expIncreaseSpeed = {
  60,60,60,60,0,
  60,60,60,60,0,
  60,60,60,60,0,
  60,60,60,60,0,
  60,60,60,60,0,
  60,60,60,60,0,
  60,60,60,60,0,
  60,60,60,60,0,
  60,60,60,60,0,
  60,60,60,60,0,
  60,60,60,60,0,
  60,60,60,60,0,
  60,60,60,60,0,
  60,60,60,60,0,
  60,60,60,60,0,
  60,60,60,60,0,
  60,60,60,60,0,
  60,60,60,60,0,
  60,60,60,60,0,
  60,60,60,60,0,
  }
  
  return expIncreaseSpeed
end

---幻灵石各契合等级对应调整系数
function ZLuaMagicExpLevelQuo()
  local expLevelQuo = {
  0.3,0.4,0.5,0.6,0.8,									---强化等级1级各阶系数
  0.3,0.4,0.45,0.5,0.6,									---强化等级2级各阶系数
  0.3,0.35,0.41,0.48,0.53,							---强化等级3级各阶系数
  0.3,0.35,0.4,0.46,0.55,								---强化等级4级各阶系数
  0.3,0.4,0.45,0.5,0.6,						 			---强化等级5级各阶系数

--  0.3,0.4,0.5,0.6,0.67,								---强化等级1级各阶系数
--  0.3,0.4,0.45,0.5,0.68,							---强化等级2级各阶系数
--  0.3,0.35,0.41,0.48,0.69,						---强化等级3级各阶系数
--  0.3,0.35,0.4,0.46,0.70,							---强化等级4级各阶系数
--  0.3,0.4,0.45,0.5,0.71,						 	---强化等级5级各阶系数

  0.3,0.4,0.45,0.5,0.67,								---强化等级6级各阶系数
  0.3,0.4,0.45,0.5,0.69,								---强化等级7级各阶系数
  0.3,0.4,0.5,0.6,0.73,									---强化等级8级各阶系数
  0.3,0.4,0.5,0.6,0.78,									---强化等级9级各阶系数
  0.3,0.4,0.5,0.65,0.80,								---强化等级10级各阶系数

  0.3,0.4,0.55,0.7,0.87,								---强化等级11级各阶系数
  0.4,0.5,0.6,0.7,0.93,									---强化等级12级各阶系数
  0.4,0.56,0.67,0.8,1.02,								---强化等级13级各阶系数
  0.45,0.56,0.67,0.8,1.09,							---强化等级14级各阶系数
  0.45,0.55,0.65,0.8,1.15,							---强化等级15级各阶系数

  0.45,0.55,0.65,0.8,1.23,							---强化等级16级各阶系数
  0.45,0.55,0.65,0.8,1.29,							---强化等级17级各阶系数
  0.45,0.55,0.65,0.8,1.36,							---强化等级18级各阶系数
  0.45,0.55,0.65,0.8,1.41,							---强化等级19级各阶系数
  0.45,0.55,0.65,0.8,1.46					    	---强化等级20级各阶系数
  }
  return expLevelQuo
end

---幻灵石各级别固有提升概率
function ZLuaMagicLevelupProbability()
  local levelupProbability = {
  1,						0.888888,			0.7777777,		0.555555,			0.333333333,
  0.222222222, 	0.166666667,	0.1,					0.066666667,	0.013333333,
  0.004761905,	0.001626016, 	0.000546448,	0.000182648,	6.09385E-05,
  2.0319E-05,  	6.77369E-06, 	2.25797E-06,	7.52666E-07,
  }
  
  return levelupProbability
end

---幻灵石每次强化成功后提升人物本体的御灵值
function ZLuaMagicLevelupDriValue()
  local levelupDriValue = {
  0,		0,		0,		0,		1,
  2,		3,		5,		15,		25,
  100,	100,	100,	100,	100,
  100,	100,	100,	100,	100,
  }
  
  return levelupDriValue
end

function ZLuaMagicAddons(tmplID)
  if ZLuaMagicExistInTmplIDs(tmplID) == 0 then return end

  local addonlearnlevel
  local addonlearnexplevel
  local addonid
  local magics = ZLuaAvailableMagics()

  ---测试用1、2、3
  if tmplID == magics["TEST1"].tmplID then 
   addonlearnlevel                     = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   
  elseif tmplID == magics["TEST2"].tmplID then 
   addonlearnlevel                     = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 0,0,0,0,0,0,0,0,0,0,0,0 }

  elseif tmplID == magics["TEST3"].tmplID then 
   addonlearnlevel                     = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 0,0,0,0,0,0,0,0,0,0,0,0 }

  elseif tmplID == magics["TEST4"].tmplID then 
   addonlearnlevel                     = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 0,0,0,0,0,0,0,0,0,0,0,0 }
  
  elseif tmplID == magics["TEST5"].tmplID then 
   addonlearnlevel                     = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 0,0,0,0,0,0,0,0,0,0,0,0 }

  elseif tmplID == magics["TEST6"].tmplID then 
   addonlearnlevel                     = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 0,0,0,0,0,0,0,0,0,0,0,0 }

  elseif tmplID == magics["TEST7"].tmplID then 
   addonlearnlevel                     = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 0,0,0,0,0,0,0,0,0,0,0,0 }

  ---限时派送1，2，3
  elseif tmplID == magics["paisong1"].tmplID then 
   addonlearnlevel                     = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   
  elseif tmplID == magics["paisong2"].tmplID then 
   addonlearnlevel                     = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   
  elseif tmplID == magics["paisong3"].tmplID then 
   addonlearnlevel                     = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 0,0,0,0,0,0,0,0,0,0,0,0 }

  ---充值奖励熊猫无法强化
  elseif tmplID == magics["daoxuan"].tmplID or tmplID == magics["daoxuan2"].tmplID or tmplID == magics["huluwa"].tmplID or tmplID == magics["chiyouzhenshen"].tmplID or tmplID == magics["longshen"].tmplID
  then 
   addonlearnlevel                     = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 0,0,0,0,0,0,0,0,0,0,0,0 }

  ---国庆活动奖励三种
  elseif tmplID == magics["guoqing01"].tmplID then 
   addonlearnlevel                     = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 3669,0,0,0,0,0,0,0,0,0,0,0 }

  elseif tmplID == magics["guoqing02"].tmplID then 
   addonlearnlevel                     = { 1,0,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 1,0,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 3207,0,0,0,0,0,0,0,0,0,0,0 }

  elseif tmplID == magics["guoqing03"].tmplID then 
   addonlearnlevel                     = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 3701,3698,0,0,0,0,0,0,0,0,0,0 }

  ---重阳活动用限时
  elseif tmplID == magics["chongyang"].tmplID or tmplID == magics["xueguoteshi"].tmplID then 
   addonlearnlevel                     = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 0,0,0,0,0,0,0,0,0,0,0,0 }

  ---新浪小狼人
  elseif tmplID == magics["xiaolangren"].tmplID 
  or tmplID == magics["huoyan"].tmplID
  or tmplID == magics["huowa"].tmplID
  or tmplID == magics["kuishe"].tmplID
  or tmplID == magics["guilifei"].tmplID
  then 
   addonlearnlevel                     = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 0,0,0,0,0,0,0,0,0,0,0,0 }

  ---雪人活动用限时4个
  elseif tmplID == magics["xueren01"].tmplID then 
   addonlearnlevel                     = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 0,0,0,0,0,0,0,0,0,0,0,0 }
  
  elseif tmplID == magics["xueren02"].tmplID then 
   addonlearnlevel                     = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 0,0,0,0,0,0,0,0,0,0,0,0 }

  elseif tmplID == magics["xueren03"].tmplID then 
   addonlearnlevel                     = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 0,0,0,0,0,0,0,0,0,0,0,0 }

  elseif tmplID == magics["xueren04"].tmplID then 
   addonlearnlevel                     = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 0,0,0,0,0,0,0,0,0,0,0,0 }

  elseif tmplID == magics["pengzhuang1"].tmplID 
  or tmplID == magics["pengzhuang2"].tmplID 
  or tmplID == magics["pengzhuang3"].tmplID 
  then 
   addonlearnlevel                     = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 0,0,0,0,0,0,0,0,0,0,0,0 }

  ---三界乱斗活动BOSS掉落形象
  elseif tmplID == magics["sanjie01"].tmplID 
  or tmplID == magics["shouhuzhe"].tmplID
  then 
   addonlearnlevel                     = { 16,16,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 5,5,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 2360,2360,0,0,0,0,0,0,0,0,0,0 }

  elseif tmplID == magics["shennong"].tmplID then 
   addonlearnlevel                     = { 16,16,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 5,5,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 34474,34475,0,0,0,0,0,0,0,0,0,0 }

  ---天界月神幻灵石变身形象
  elseif tmplID == magics["yueshen"].tmplID then 
   addonlearnlevel                     = { 16,16,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 5,5,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 32950,32951,0,0,0,0,0,0,0,0,0,0 }

	---帝俊6v6dijun
  elseif tmplID == magics["2013dijun"].tmplID then 
   addonlearnlevel                     = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 0,0,0,0,0,0,0,0,0,0,0,0 }

  ---店卖道具 
  elseif tmplID == magics["huamei"].tmplID then 
   addonlearnlevel                     = { 0,6,9,12,15,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,1,1,1,1,0,0,0,0,0,0,0 } 
   addonid                             = { 0,3659,3663,3671,3667,0,0,0,0,0,0,0 }

  elseif tmplID == magics["hanfei"].tmplID then 
   addonlearnlevel                     = { 0,6,9,12,15,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,1,1,1,1,0,0,0,0,0,0,0 } 
   addonid                             = { 0,3655,3655,3679,3675,0,0,0,0,0,0,0 }

  elseif tmplID == magics["linghou"].tmplID then 
   addonlearnlevel                     = { 0,6,9,12,15,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,1,1,1,1,0,0,0,0,0,0,0 } 
   addonid                             = { 0,3659,3663,3671,3667,0,0,0,0,0,0,0 }

  elseif tmplID == magics["yingchifeng"].tmplID then 
   addonlearnlevel                     = { 0,6,9,12,15,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,1,1,1,1,0,0,0,0,0,0,0 } 
   addonid                             = { 0,3655,3655,3679,3675,0,0,0,0,0,0,0 }

  elseif tmplID == magics["huayao"].tmplID then 
   addonlearnlevel                     = { 0,6,9,12,15,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,1,1,1,1,0,0,0,0,0,0,0 } 
   addonid                             = { 0,3659,3663,3671,3667,0,0,0,0,0,0,0 }
   
  elseif tmplID == magics["guidaoshi"].tmplID then 
   addonlearnlevel                     = { 0,6,9,12,15,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,1,1,1,1,0,0,0,0,0,0,0 } 
   addonid                             = { 0,3655,3655,3679,3675,0,0,0,0,0,0,0 }

  ---2010春节五福虎
  elseif tmplID == magics["wufuhu01"].tmplID  then 
   addonlearnlevel                     = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 3798,3667,0,0,0,0,0,0,0,0,0,0 }
   
  elseif tmplID == magics["wufuhu02"].tmplID  then 
   addonlearnlevel                     = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 3799,3800,0,0,0,0,0,0,0,0,0,0 }

  elseif tmplID == magics["wufuhu03"].tmplID  then 
   addonlearnlevel                     = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 3801,3672,0,0,0,0,0,0,0,0,0,0 }

  elseif tmplID == magics["wufuhu04"].tmplID  then 
   addonlearnlevel                     = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 3802,3803,0,0,0,0,0,0,0,0,0,0 }

  elseif tmplID == magics["wufuhu05"].tmplID  then 
   addonlearnlevel                     = { 1,1,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 1,1,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 3804,3805,0,0,0,0,0,0,0,0,0,0 }

	---2010公会VIP奖励幻灵石 男/女
  elseif tmplID == magics["2010VIPman"].tmplID then 
   addonlearnlevel                     = { 0,6,9,12,15,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,1,1,1,1,0,0,0,0,0,0,0 } 
   addonid                             = { 0,3659,3663,3671,3667,0,0,0,0,0,0,0 }

  elseif tmplID == magics["2010VIPwoman"].tmplID then 
   addonlearnlevel                     = { 0,6,9,12,15,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,1,1,1,1,0,0,0,0,0,0,0 } 
   addonid                             = { 0,3655,3655,3679,3675,0,0,0,0,0,0,0 }

  ---跨服PK奖励兽神
  elseif tmplID == magics["kuafushoushen"].tmplID 
  or tmplID == magics["qingxin"].tmplID  
  or tmplID == magics["2012VIPhuolong"].tmplID 
  or tmplID == magics["2012huigui01"].tmplID 
  or tmplID == magics["2012huigui02"].tmplID 
  or tmplID == magics["2012huigui03"].tmplID 
  or tmplID == magics["2012huigui04"].tmplID 
  or tmplID == magics["2012huigui05"].tmplID 
  or tmplID == magics["2012huigui06"].tmplID 
  or tmplID == magics["yyxiongx"].tmplID 
  or tmplID == magics["yyxiongy"].tmplID 
  or tmplID == magics["zhouyixian"].tmplID 
  or tmplID == magics["feitianxianzi"].tmplID 
  or tmplID == magics["kuafutianjienan"].tmplID 
  or tmplID == magics["kuafutianjienv"].tmplID 
  or tmplID == magics["gongyuanyang"].tmplID 
  or tmplID == magics["muyuanyang"].tmplID 
  or tmplID == magics["kushuangtianshi"].tmplID 
  or tmplID == magics["kushuangtianshi2"].tmplID 
  or tmplID == magics["zhangyu"].tmplID 
  or tmplID == magics["caishen"].tmplID 
  or tmplID == magics["nantong"].tmplID 
  or tmplID == magics["nvtong"].tmplID 
  or tmplID == magics["xiaobai2013"].tmplID 
  or tmplID == magics["guili2013"].tmplID 
  or tmplID == magics["xueqi2013"].tmplID 
  or tmplID == magics["biyao2013"].tmplID 
  then 
   addonlearnlevel                     = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   addonlearnexplevel                  = { 0,0,0,0,0,0,0,0,0,0,0,0 } 
   addonid                             = { 0,0,0,0,0,0,0,0,0,0,0,0 }
   
  else return 0
  end

  return addonlearnlevel,addonlearnexplevel,addonid
end





---幻化属性Data索引数组
function ZLuaMagicDataIndex()
  local dataIndex
  dataIndex = {}

  dataIndex["quality"]                         	= 1  

  dataIndex["default_hp"]                      	= 2
  dataIndex["default_mp"]                      	= 3
  dataIndex["default_damage_low"]              	= 4
  dataIndex["default_damage_high"]             	= 5
  dataIndex["default_accuracy"]                	= 6
  dataIndex["default_dodge"]                   	= 7
  dataIndex["default_defense"]                 	= 8
  
  dataIndex["default_resis_stun"]              	= 9
  dataIndex["default_resis_sleep"]             	= 10
  dataIndex["default_resis_silence"]           	= 11
  dataIndex["default_resis_weakness"]          	= 12
  dataIndex["default_resis_enlace"]            	= 13
  dataIndex["default_resis_slow"]              	= 14
  
  dataIndex["grow_hp"]                         	= 15
  dataIndex["grow_mp"]                         	= 16
  dataIndex["grow_damage_low"]                 	= 17
  dataIndex["grow_damage_high"]                	= 18
  dataIndex["grow_accuracy"]                   	= 19
  dataIndex["grow_dodge"]                      	= 20
  dataIndex["grow_defense"]                    	= 21

  dataIndex["grow_resis_stun"]                 	= 22
  dataIndex["grow_resis_sleep"]                	= 23
  dataIndex["grow_resis_silence"]              	= 24
  dataIndex["grow_resis_weakness"]             	= 25
  dataIndex["grow_resis_enlace"]               	= 26
  dataIndex["grow_resis_slow"]                 	= 27
  dataIndex["reserve1"]													= 28
  dataIndex["reserve2"]													= 29
			 
  local resisIndex = {}

  resisIndex["resis_stun"]     									= 0
  resisIndex["resis_sleep"]    									= 1
  resisIndex["resis_silence"] 									= 2
  resisIndex["resis_weakness"] 									= 3
  resisIndex["resis_enlace"]   									= 4
  resisIndex["resis_slow"]     									= 5

  return dataIndex,resisIndex
end

---------------------------处理随机属性部分-----------------------


function ZLuaMagicGrades()
  local dataIndex,resisIndex = ZLuaMagicDataIndex()
 
  local nGrades
	nGrades = 50 	
	
	local valueBounds = {}
	valueBounds[dataIndex["default_hp"]]								=	{ 0, 50 }
	valueBounds[dataIndex["default_mp"]]								=	{ 0, 50 }
	valueBounds[dataIndex["default_damage_low"]]				=	{ 0, 20 }
	valueBounds[dataIndex["default_damage_high"]]				=	{ 0, 30 }
	valueBounds[dataIndex["default_accuracy"]]					=	{ 0, 5 }
	valueBounds[dataIndex["default_dodge"]]							=	{ 0, 5 }
	valueBounds[dataIndex["default_defense"]]						=	{ 0, 8 }

	valueBounds[dataIndex["default_resis_stun"]]				=	{ 0, 4 }
	valueBounds[dataIndex["default_resis_sleep"]]				=	{ 0, 4 }
	valueBounds[dataIndex["default_resis_silence"]]			=	{ 0, 4 }
	valueBounds[dataIndex["default_resis_weakness"]]		=	{ 0, 4 }
	valueBounds[dataIndex["default_resis_enlace"]]			=	{ 0, 4 }
	valueBounds[dataIndex["default_resis_slow"]]				=	{ 0, 0 }

	valueBounds[dataIndex["grow_hp"]]										=	{ 0, 400 }
	valueBounds[dataIndex["grow_mp"]]										=	{ 0, 500 }
	valueBounds[dataIndex["grow_damage_low"]]						=	{ 0, 34 }
	valueBounds[dataIndex["grow_damage_high"]]					=	{ 0, 50 }
	valueBounds[dataIndex["grow_accuracy"]]							=	{ 0, 9 }
	valueBounds[dataIndex["grow_dodge"]]								=	{ 0, 6 }
	valueBounds[dataIndex["grow_defense"]]							=	{ 0, 12 }

	valueBounds[dataIndex["grow_resis_stun"]]						=	{ 0, 4.5 }
	valueBounds[dataIndex["grow_resis_sleep"]]					=	{ 0, 4.5 }
	valueBounds[dataIndex["grow_resis_silence"]]				=	{ 0, 4.5 }
	valueBounds[dataIndex["grow_resis_weakness"]]				=	{ 0, 4.5 }
	valueBounds[dataIndex["grow_resis_enlace"]]					=	{ 0, 4.5 }
	valueBounds[dataIndex["grow_resis_slow"]]						=	{ 0, 0 }

	valueBounds[dataIndex["reserve1"]]									=	{ 0, 0 }
	valueBounds[dataIndex["reserve2"]]									=	{ 0, 0 }
	
---
	local valueGrades = {}
	
	valueGrades[dataIndex["default_hp"]] = {}
	valueGrades[dataIndex["default_mp"]] = {}
	valueGrades[dataIndex["default_damage_low"]] = {}
	valueGrades[dataIndex["default_damage_high"]] = {}
	valueGrades[dataIndex["default_accuracy"]] = {}
	valueGrades[dataIndex["default_dodge"]] = {}
	valueGrades[dataIndex["default_defense"]] = {}
	
	valueGrades[dataIndex["default_resis_stun"]] = {}
	valueGrades[dataIndex["default_resis_sleep"]] = {}
	valueGrades[dataIndex["default_resis_silence"]] = {}
	valueGrades[dataIndex["default_resis_weakness"]] = {}
	valueGrades[dataIndex["default_resis_enlace"]] = {}
	valueGrades[dataIndex["default_resis_slow"]] =	{}	
	
	valueGrades[dataIndex["grow_hp"]] = {}
	valueGrades[dataIndex["grow_mp"]] = {}
	valueGrades[dataIndex["grow_damage_low"]] = {}
	valueGrades[dataIndex["grow_damage_high"]] = {}
	valueGrades[dataIndex["grow_accuracy"]] = {}
	valueGrades[dataIndex["grow_dodge"]] = {}
	valueGrades[dataIndex["grow_defense"]] = {}
	
	valueGrades[dataIndex["grow_resis_stun"]] = {}
	valueGrades[dataIndex["grow_resis_sleep"]] = {}
	valueGrades[dataIndex["grow_resis_silence"]] = {}
	valueGrades[dataIndex["grow_resis_weakness"]] = {}
	valueGrades[dataIndex["grow_resis_enlace"]] = {} 
	valueGrades[dataIndex["grow_resis_slow"]]	=	{} 
	
	valueGrades[dataIndex["reserve1"]] = {} 
	valueGrades[dataIndex["reserve2"]]	=	{} 
 
	local i
	for i = 1, nGrades do
	
	valueGrades[dataIndex["default_hp"]][i] 		= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["default_hp"]][1], valueBounds[dataIndex["default_hp"]][2], i, nGrades )
	valueGrades[dataIndex["default_mp"]][i] 		= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["default_mp"]][1], valueBounds[dataIndex["default_mp"]][2], i, nGrades )
	valueGrades[dataIndex["default_damage_low"]][i] 	= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["default_damage_low"]][1], valueBounds[dataIndex["default_damage_low"]][2], i, nGrades )
	valueGrades[dataIndex["default_damage_high"]][i] 	= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["default_damage_high"]][1], valueBounds[dataIndex["default_damage_high"]][2], i, nGrades )
	valueGrades[dataIndex["default_accuracy"]][i] 		= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["default_accuracy"]][1], valueBounds[dataIndex["default_accuracy"]][2], i, nGrades )
	valueGrades[dataIndex["default_dodge"]][i] 		= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["default_dodge"]][1], valueBounds[dataIndex["default_dodge"]][2], i, nGrades )
	valueGrades[dataIndex["default_defense"]][i] 		= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["default_defense"]][1], valueBounds[dataIndex["default_defense"]][2], i, nGrades )
	valueGrades[dataIndex["default_resis_stun"]][i] 	= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["default_resis_stun"]][1], valueBounds[dataIndex["default_resis_stun"]][2], i, nGrades )
	valueGrades[dataIndex["default_resis_sleep"]][i] 	= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["default_resis_sleep"]][1], valueBounds[dataIndex["default_resis_sleep"]][2], i, nGrades )
	valueGrades[dataIndex["default_resis_silence"]][i] 	= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["default_resis_silence"]][1], valueBounds[dataIndex["default_resis_silence"]][2], i, nGrades )
	valueGrades[dataIndex["default_resis_weakness"]][i] 	= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["default_resis_weakness"]][1], valueBounds[dataIndex["default_resis_weakness"]][2], i, nGrades )
	valueGrades[dataIndex["default_resis_enlace"]][i] 	= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["default_resis_enlace"]][1], valueBounds[dataIndex["default_resis_enlace"]][2], i, nGrades )
	valueGrades[dataIndex["default_resis_slow"]][i] 	= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["default_resis_slow"]][1], valueBounds[dataIndex["default_resis_slow"]][2], i, nGrades )
		
	valueGrades[dataIndex["grow_hp"]][i] 			= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["grow_hp"]][1], valueBounds[dataIndex["grow_hp"]][2], i, nGrades )
	valueGrades[dataIndex["grow_mp"]][i] 			= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["grow_mp"]][1], valueBounds[dataIndex["grow_mp"]][2], i, nGrades )
	valueGrades[dataIndex["grow_damage_low"]][i] 		= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["grow_damage_low"]][1], valueBounds[dataIndex["grow_damage_low"]][2], i, nGrades )
	valueGrades[dataIndex["grow_damage_high"]][i] 		= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["grow_damage_high"]][1], valueBounds[dataIndex["grow_damage_high"]][2], i, nGrades )
	valueGrades[dataIndex["grow_accuracy"]][i] 		= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["grow_accuracy"]][1], valueBounds[dataIndex["grow_accuracy"]][2], i, nGrades )
	valueGrades[dataIndex["grow_dodge"]][i] 		= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["grow_dodge"]][1], valueBounds[dataIndex["grow_dodge"]][2], i, nGrades )
	valueGrades[dataIndex["grow_defense"]][i] 		= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["grow_defense"]][1], valueBounds[dataIndex["grow_defense"]][2], i, nGrades )
	valueGrades[dataIndex["grow_resis_stun"]][i] 		= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["grow_resis_stun"]][1], valueBounds[dataIndex["grow_resis_stun"]][2], i, nGrades )
	valueGrades[dataIndex["grow_resis_sleep"]][i] 		= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["grow_resis_sleep"]][1], valueBounds[dataIndex["grow_resis_sleep"]][2], i, nGrades )
	valueGrades[dataIndex["grow_resis_silence"]][i] 	= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["grow_resis_silence"]][1], valueBounds[dataIndex["grow_resis_silence"]][2], i, nGrades )
	valueGrades[dataIndex["grow_resis_weakness"]][i] 	= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["grow_resis_weakness"]][1], valueBounds[dataIndex["grow_resis_weakness"]][2], i, nGrades )
	valueGrades[dataIndex["grow_resis_enlace"]][i] 		= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["grow_resis_enlace"]][1], valueBounds[dataIndex["grow_resis_enlace"]][2], i, nGrades )
	valueGrades[dataIndex["grow_resis_slow"]][i] 		= ZLuaMagicGetValueWithBounds( valueBounds[dataIndex["grow_resis_slow"]][1], valueBounds[dataIndex["grow_resis_slow"]][2], i, nGrades )
	end
	
	return nGrades, valueGrades 
  	
end

function ZLuaMagicGetValueWithBounds(startValue, endValue, currentStep, allStep)
	if type(startValue) ~= "number" or type(endValue) ~= "number" or type(currentStep) ~= "number" or type(allStep) ~= "number" then return -1 end
	if allStep == 1 then return -1 end
	return startValue + ( endValue - startValue ) / ( allStep - 1 ) * ( currentStep - 1 )	
end

function ZLuaMagicRandomResult(tmplID)

	if ZLuaMagicExistInTmplIDs(tmplID) == 0 then return nil end
	local magics = ZLuaAvailableMagics()


	local dataIndex,resisIndex = ZLuaMagicDataIndex()  
	local nGrades, valueGrades = ZLuaMagicGrades()

	local valueWeights = ZLuaMagicRandomWeight(tmplID)
	if valueWeights == nil then return nil end

	local randomResult
	randomResult = {}
	
	local grades
	grades = {}

	grades[dataIndex["default_hp"]] 							= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["default_hp"]])
	grades[dataIndex["default_mp"]] 							= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["default_mp"]])
	grades[dataIndex["default_damage_low"]] 			= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["default_damage_low"]])
	grades[dataIndex["default_damage_high"]] 			= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["default_damage_high"]])
	grades[dataIndex["default_accuracy"]] 	  		= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["default_accuracy"]])
	grades[dataIndex["default_dodge"]] 	  	    	= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["default_dodge"]])
	grades[dataIndex["default_defense"]] 					= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["default_defense"]])
	grades[dataIndex["default_resis_stun"]] 			= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["default_resis_stun"]])
	grades[dataIndex["default_resis_sleep"]] 			= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["default_resis_sleep"]])
	grades[dataIndex["default_resis_silence"]] 		= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["default_resis_silence"]])
	grades[dataIndex["default_resis_weakness"]] 	= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["default_resis_weakness"]])
	grades[dataIndex["default_resis_enlace"]] 		= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["default_resis_enlace"]])
	grades[dataIndex["default_resis_slow"]]   		= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["default_resis_slow"]])

	grades[dataIndex["grow_hp"]] 									= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["grow_hp"]])
	grades[dataIndex["grow_mp"]] 									= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["grow_mp"]])
	grades[dataIndex["grow_damage_low"]] 					= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["grow_damage_low"]])
	grades[dataIndex["grow_damage_high"]] 				= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["grow_damage_high"]])
	grades[dataIndex["grow_accuracy"]] 		    		= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["grow_accuracy"]])
	grades[dataIndex["grow_dodge"]] 							= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["grow_dodge"]])
	grades[dataIndex["grow_defense"]] 						= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["grow_defense"]])
	grades[dataIndex["grow_resis_stun"]] 					= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["grow_resis_stun"]])
	grades[dataIndex["grow_resis_sleep"]] 				= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["grow_resis_sleep"]])
	grades[dataIndex["grow_resis_silence"]] 			= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["grow_resis_silence"]])
	grades[dataIndex["grow_resis_weakness"]] 			= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["grow_resis_weakness"]])
	grades[dataIndex["grow_resis_enlace"]] 				= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["grow_resis_enlace"]])
	grades[dataIndex["grow_resis_slow"]] 	  			= ZLuaMagic_Get_RanPoint(valueWeights[dataIndex["grow_resis_slow"]])


	randomResult[dataIndex["default_hp"]] 							= valueGrades[dataIndex["default_hp"]][grades[dataIndex["default_hp"]]]
	randomResult[dataIndex["default_mp"]] 							= valueGrades[dataIndex["default_mp"]][grades[dataIndex["default_mp"]]]
	randomResult[dataIndex["default_damage_low"]] 			= valueGrades[dataIndex["default_damage_low"]][grades[dataIndex["default_damage_low"]]]
	randomResult[dataIndex["default_damage_high"]] 			= valueGrades[dataIndex["default_damage_high"]][grades[dataIndex["default_damage_high"]]]
	randomResult[dataIndex["default_accuracy"]] 				= valueGrades[dataIndex["default_accuracy"]][grades[dataIndex["default_accuracy"]]]
	randomResult[dataIndex["default_dodge"]] 						= valueGrades[dataIndex["default_dodge"]][grades[dataIndex["default_dodge"]]]
	randomResult[dataIndex["default_defense"]] 					= valueGrades[dataIndex["default_defense"]][grades[dataIndex["default_defense"]]]
	randomResult[dataIndex["default_resis_stun"]] 			= valueGrades[dataIndex["default_resis_stun"]][grades[dataIndex["default_resis_stun"]]]
	randomResult[dataIndex["default_resis_sleep"]] 			= valueGrades[dataIndex["default_resis_sleep"]][grades[dataIndex["default_resis_sleep"]]]
	randomResult[dataIndex["default_resis_silence"]] 		= valueGrades[dataIndex["default_resis_silence"]][grades[dataIndex["default_resis_silence"]]]
	randomResult[dataIndex["default_resis_weakness"]] 	= valueGrades[dataIndex["default_resis_weakness"]][grades[dataIndex["default_resis_weakness"]]]
	randomResult[dataIndex["default_resis_enlace"]] 		= valueGrades[dataIndex["default_resis_enlace"]][grades[dataIndex["default_resis_enlace"]]]
	randomResult[dataIndex["default_resis_slow"]] 			= valueGrades[dataIndex["default_resis_slow"]][grades[dataIndex["default_resis_slow"]]]

	randomResult[dataIndex["grow_hp"]] 									= valueGrades[dataIndex["grow_hp"]][grades[dataIndex["grow_hp"]]]
	randomResult[dataIndex["grow_mp"]] 									= valueGrades[dataIndex["grow_mp"]][grades[dataIndex["grow_mp"]]]
	randomResult[dataIndex["grow_damage_low"]] 					= valueGrades[dataIndex["grow_damage_low"]][grades[dataIndex["grow_damage_low"]]]
	randomResult[dataIndex["grow_damage_high"]] 				= valueGrades[dataIndex["grow_damage_high"]][grades[dataIndex["grow_damage_high"]]]
	randomResult[dataIndex["grow_accuracy"]] 						= valueGrades[dataIndex["grow_accuracy"]][grades[dataIndex["grow_accuracy"]]]
	randomResult[dataIndex["grow_dodge"]] 							= valueGrades[dataIndex["grow_dodge"]][grades[dataIndex["grow_dodge"]]]
	randomResult[dataIndex["grow_defense"]] 						= valueGrades[dataIndex["grow_defense"]][grades[dataIndex["grow_defense"]]]
	randomResult[dataIndex["grow_resis_stun"]] 					= valueGrades[dataIndex["grow_resis_stun"]][grades[dataIndex["grow_resis_stun"]]]
	randomResult[dataIndex["grow_resis_sleep"]] 				= valueGrades[dataIndex["grow_resis_sleep"]][grades[dataIndex["grow_resis_sleep"]]]
	randomResult[dataIndex["grow_resis_silence"]] 			= valueGrades[dataIndex["grow_resis_silence"]][grades[dataIndex["grow_resis_silence"]]]
	randomResult[dataIndex["grow_resis_weakness"]] 			= valueGrades[dataIndex["grow_resis_weakness"]][grades[dataIndex["grow_resis_weakness"]]]
	randomResult[dataIndex["grow_resis_enlace"]] 				= valueGrades[dataIndex["grow_resis_enlace"]][grades[dataIndex["grow_resis_enlace"]]]
	randomResult[dataIndex["grow_resis_slow"]] 					= valueGrades[dataIndex["grow_resis_slow"]][grades[dataIndex["grow_resis_slow"]]]

  local quality
  quality = ZLuaMagicGenerateQuality(tmplID, grades, nGrades)

	return randomResult,quality
end

function ZLuaMagicRandomWeight(tmplID)
if ZLuaMagicExistInTmplIDs(tmplID) == 0 then return nil end

---取得一些常量
local dataIndex,resisIndex = ZLuaMagicDataIndex()
local magics = ZLuaAvailableMagics()

---数据修改区 开始
---开始设置每个属性的每个档位的权重
local valueWeights = {}
if tmplID == magics["TEST1"].tmplID then
	
	valueWeights[dataIndex["default_hp"]]	  		={0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.5,1,1,1,1,1,1,1,1,1,0.5,0.1,0.1}
	valueWeights[dataIndex["default_mp"]]			={0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.5,1,1,1,1,1,1,1,1,1,0.5,0.1,0.1}
	valueWeights[dataIndex["default_damage_low"]]		={0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.5,1,1,1,1,1,1,1,1,1,0.5,0.1,0.1}
	valueWeights[dataIndex["default_damage_high"]]		={0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.5,1,1,1,1,1,1,1,1,1,0.5,0.1,0.1}
	valueWeights[dataIndex["default_accuracy"]]   		={1,1,1,1,1,1,1,1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.666666666666667,.619047619047619,.571428571428571,.523809523809524,.476190476190476,.428571428571429,.380952380952381,.333333333333333,.285714285714286,.238095238095238,.19047619047619,.142857142857143,9.52380952380952E-02,4.76190476190476E-02,1.33333333333333E-02,1.23809523809524E-02,1.14285714285714E-02,1.04761904761905E-02,9.52380952380952E-03,8.57142857142857E-03,7.61904761904762E-03,6.66666666666667E-03,5.71428571428571E-03,4.76190476190476E-03,3.80952380952381E-03,2.85714285714286E-03,1.9047619047619E-03,9.52380952380951E-04}
	valueWeights[dataIndex["default_dodge"]] 		={1,1,1,1,1,1,1,1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.666666666666667,.619047619047619,.571428571428571,.523809523809524,.476190476190476,.428571428571429,.380952380952381,.333333333333333,.285714285714286,.238095238095238,.19047619047619,.142857142857143,9.52380952380952E-02,4.76190476190476E-02,1.33333333333333E-02,1.23809523809524E-02,1.14285714285714E-02,1.04761904761905E-02,9.52380952380952E-03,8.57142857142857E-03,7.61904761904762E-03,6.66666666666667E-03,5.71428571428571E-03,4.76190476190476E-03,3.80952380952381E-03,2.85714285714286E-03,1.9047619047619E-03,9.52380952380951E-04}
	valueWeights[dataIndex["default_defense"]] 		={0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.5,1,1,1,1,1,1,1,1,1,0.5,0.1,0.1}
	valueWeights[dataIndex["default_resis_stun"]]   	={0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,.666666666666667,.65,.633333333333333,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,7.5,15,22.5,30.6,2.5,1.66666666666667,.833333333333333,.05,3.33333333333333E-02,1.66666666666667E-02}
	valueWeights[dataIndex["default_resis_sleep"]]  	={0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,.666666666666667,.65,.633333333333333,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,7.5,15,22.5,30.6,2.5,1.66666666666667,.833333333333333,.05,3.33333333333333E-02,1.66666666666667E-02}
	valueWeights[dataIndex["default_resis_silence"]]	={0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,.666666666666667,.65,.633333333333333,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,7.5,15,22.5,30.6,2.5,1.66666666666667,.833333333333333,.05,3.33333333333333E-02,1.66666666666667E-02}
	valueWeights[dataIndex["default_resis_weakness"]]  	={0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,.666666666666667,.65,.633333333333333,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,7.5,15,22.5,30.6,2.5,1.66666666666667,.833333333333333,.05,3.33333333333333E-02,1.66666666666667E-02}
	valueWeights[dataIndex["default_resis_enlace"]]    	={0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,.666666666666667,.65,.633333333333333,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,7.5,15,22.5,30.6,2.5,1.66666666666667,.833333333333333,.05,3.33333333333333E-02,1.66666666666667E-02}
	valueWeights[dataIndex["default_resis_slow"]]      	={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
	valueWeights[dataIndex["grow_hp"]] 			={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
	valueWeights[dataIndex["grow_mp"]] 			={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
	valueWeights[dataIndex["grow_damage_low"]] 		={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
	valueWeights[dataIndex["grow_damage_high"]] 		={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
	valueWeights[dataIndex["grow_accuracy"]] 		={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
	valueWeights[dataIndex["grow_dodge"]] 			={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
	valueWeights[dataIndex["grow_defense"]] 		={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
	valueWeights[dataIndex["grow_resis_stun"]] 		={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
	valueWeights[dataIndex["grow_resis_sleep"]] 		={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
	valueWeights[dataIndex["grow_resis_silence"]] 		={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
	valueWeights[dataIndex["grow_resis_weakness"]] 		={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
	valueWeights[dataIndex["grow_resis_enlace"]] 		={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
	valueWeights[dataIndex["grow_resis_slow"]] 		={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}

  elseif tmplID == magics["TEST2"].tmplID then
	
	valueWeights[dataIndex["default_hp"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["default_mp"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["default_damage_low"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["default_damage_high"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["default_accuracy"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["default_dodge"]]           ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["default_defense"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["default_resis_stun"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["default_resis_sleep"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["default_resis_silence"]]   ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["default_resis_weakness"]]  ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["default_resis_enlace"]]    ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
	valueWeights[dataIndex["grow_hp"]]                 ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["grow_mp"]]                 ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["grow_damage_low"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["grow_damage_high"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["grow_accuracy"]]           ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["grow_dodge"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["grow_defense"]]            ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["grow_resis_stun"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["grow_resis_sleep"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["grow_resis_silence"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["grow_resis_weakness"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["grow_resis_enlace"]]       ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
	valueWeights[dataIndex["grow_resis_slow"]]         ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}

  elseif tmplID == magics["TEST3"].tmplID 
  or tmplID == magics["TEST4"].tmplID 
  or tmplID == magics["TEST5"].tmplID 
  or tmplID == magics["TEST6"].tmplID 
  or tmplID == magics["TEST7"].tmplID 
  then
	
		valueWeights[dataIndex["default_hp"]] ={0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.5,1,1,1,1,1,1,1,1,1,0.5,0.1,0.1}
		valueWeights[dataIndex["default_mp"]] ={0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.5,1,1,1,1,1,1,1,1,1,0.5,0.1,0.1}
		valueWeights[dataIndex["default_damage_low"]] ={0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.5,1,1,1,1,1,1,1,1,1,0.5,0.1,0.1}
		valueWeights[dataIndex["default_damage_high"]] ={0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.5,1,1,1,1,1,1,1,1,1,0.5,0.1,0.1}
		valueWeights[dataIndex["default_accuracy"]] ={1,1,1,1,1,1,1,1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.666666666666667,.619047619047619,.571428571428571,.523809523809524,.476190476190476,.428571428571429,.380952380952381,.333333333333333,.285714285714286,.238095238095238,.19047619047619,.142857142857143,9.52380952380952E-02,4.76190476190476E-02,1.33333333333333E-02,1.23809523809524E-02,1.14285714285714E-02,1.04761904761905E-02,9.52380952380952E-03,8.57142857142857E-03,7.61904761904762E-03,6.66666666666667E-03,5.71428571428571E-03,4.76190476190476E-03,3.80952380952381E-03,2.85714285714286E-03,1.9047619047619E-03,9.52380952380951E-04}
		valueWeights[dataIndex["default_dodge"]] ={1,1,1,1,1,1,1,1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.666666666666667,.619047619047619,.571428571428571,.523809523809524,.476190476190476,.428571428571429,.380952380952381,.333333333333333,.285714285714286,.238095238095238,.19047619047619,.142857142857143,9.52380952380952E-02,4.76190476190476E-02,1.33333333333333E-02,1.23809523809524E-02,1.14285714285714E-02,1.04761904761905E-02,9.52380952380952E-03,8.57142857142857E-03,7.61904761904762E-03,6.66666666666667E-03,5.71428571428571E-03,4.76190476190476E-03,3.80952380952381E-03,2.85714285714286E-03,1.9047619047619E-03,9.52380952380951E-04}
		valueWeights[dataIndex["default_defense"]] ={0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.1,0.5,1,1,1,1,1,1,1,1,1,0.5,0.1,0.1}
		valueWeights[dataIndex["default_resis_stun"]] ={0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,.666666666666667,.65,.633333333333333,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,7.5,15,22.5,30.6,2.5,1.66666666666667,.833333333333333,.05,3.33333333333333E-02,1.66666666666667E-02}
		valueWeights[dataIndex["default_resis_sleep"]] ={0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,.666666666666667,.65,.633333333333333,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,7.5,15,22.5,30.6,2.5,1.66666666666667,.833333333333333,.05,3.33333333333333E-02,1.66666666666667E-02}
		valueWeights[dataIndex["default_resis_silence"]] ={0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,.666666666666667,.65,.633333333333333,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,7.5,15,22.5,30.6,2.5,1.66666666666667,.833333333333333,.05,3.33333333333333E-02,1.66666666666667E-02}
		valueWeights[dataIndex["default_resis_weakness"]] ={0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,.666666666666667,.65,.633333333333333,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,7.5,15,22.5,30.6,2.5,1.66666666666667,.833333333333333,.05,3.33333333333333E-02,1.66666666666667E-02}
		valueWeights[dataIndex["default_resis_enlace"]] ={0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,.666666666666667,.65,.633333333333333,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,7.5,15,22.5,30.6,2.5,1.66666666666667,.833333333333333,.05,3.33333333333333E-02,1.66666666666667E-02}
		valueWeights[dataIndex["default_resis_slow"]] ={0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,0.6,.666666666666667,.65,.633333333333333,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.714285714285714,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,.75,7.5,15,22.5,30.6,2.5,1.66666666666667,.833333333333333,.05,3.33333333333333E-02,1.66666666666667E-02}
		
		valueWeights[dataIndex["grow_hp"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_mp"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_damage_low"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_damage_high"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_accuracy"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_dodge"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_defense"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_stun"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_sleep"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_silence"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_weakness"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_enlace"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["grow_resis_slow"]] ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}

  ---诛仙2充值奖励-永久幻灵石-熊猫-无法强化
  elseif tmplID == magics["daoxuan"].tmplID or tmplID == magics["daoxuan2"].tmplID or tmplID == magics["huluwa"].tmplID or tmplID == magics["chiyouzhenshen"].tmplID or tmplID == magics["longshen"].tmplID then

		valueWeights[dataIndex["default_hp"]]              ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_mp"]]              ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_damage_low"]]      ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_damage_high"]]     ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_accuracy"]]        ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_dodge"]]           ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_defense"]]         ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]]      ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_sleep"]]     ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_silence"]]   ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_weakness"]]  ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_enlace"]]    ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
		valueWeights[dataIndex["grow_hp"]]                 ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_mp"]]                 ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_damage_low"]]         ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_damage_high"]]        ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_accuracy"]]           ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_dodge"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_defense"]]            ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_stun"]]         ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_sleep"]]        ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_silence"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_weakness"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_enlace"]]       ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_slow"]]         ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}

  ---国庆中秋活动奖励三种
  elseif tmplID == magics["guoqing01"].tmplID then

		valueWeights[dataIndex["default_hp"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_mp"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_low"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_high"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_accuracy"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_dodge"]]           ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_defense"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_stun"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_sleep"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_silence"]]   ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_weakness"]]  ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_enlace"]]    ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
		valueWeights[dataIndex["grow_hp"]]                 ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]]                 ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_accuracy"]]           ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]]            ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_stun"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]]       ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_slow"]]         ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}

  elseif tmplID == magics["guoqing02"].tmplID then

		valueWeights[dataIndex["default_hp"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_mp"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_low"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_high"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_accuracy"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_dodge"]]           ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_defense"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_stun"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_sleep"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_silence"]]   ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_weakness"]]  ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_enlace"]]    ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
		valueWeights[dataIndex["grow_hp"]]                 ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]]                 ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_accuracy"]]           ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]]            ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_stun"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]]       ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_slow"]]         ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}

  elseif tmplID == magics["guoqing03"].tmplID then

		valueWeights[dataIndex["default_hp"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_mp"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_low"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_high"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_accuracy"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_dodge"]]           ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_defense"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_stun"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_sleep"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_silence"]]   ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_weakness"]]  ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_enlace"]]    ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
		valueWeights[dataIndex["grow_hp"]]                 ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]]                 ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_accuracy"]]           ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]]            ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_stun"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]]       ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_slow"]]         ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}

  ---重阳活动用限时
  elseif tmplID == magics["chongyang"].tmplID 
  or tmplID == magics["xiaolangren"].tmplID 
  or tmplID == magics["huoyan"].tmplID 
  or tmplID == magics["huowa"].tmplID 
  or tmplID == magics["kuishe"].tmplID 
  or tmplID == magics["guilifei"].tmplID 
  or tmplID == magics["xueguoteshi"].tmplID 
  	then

		valueWeights[dataIndex["default_hp"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_mp"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_low"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_high"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_accuracy"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_dodge"]]           ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_defense"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_stun"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_sleep"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_silence"]]   ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_weakness"]]  ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_enlace"]]    ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
		valueWeights[dataIndex["grow_hp"]]                 ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]]                 ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_accuracy"]]           ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]]            ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_stun"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]]       ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_slow"]]         ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}

  ---雪人活动用限时
  elseif tmplID == magics["xueren01"].tmplID then

		valueWeights[dataIndex["default_hp"]]              ={1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_mp"]]              ={1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_low"]]      ={1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_high"]]     ={1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_accuracy"]]        ={1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_dodge"]]           ={1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_defense"]]         ={1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_stun"]]      ={1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_sleep"]]     ={1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_silence"]]   ={1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_weakness"]]  ={1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_enlace"]]    ={1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
		valueWeights[dataIndex["grow_hp"]]                 ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]]                 ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_accuracy"]]           ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]]            ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_stun"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]]       ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_slow"]]         ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}

  elseif tmplID == magics["xueren02"].tmplID then

		valueWeights[dataIndex["default_hp"]]              ={0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_mp"]]              ={0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_low"]]      ={0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_high"]]     ={0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_accuracy"]]        ={0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_dodge"]]           ={0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_defense"]]         ={0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_stun"]]      ={0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_sleep"]]     ={0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_silence"]]   ={0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_weakness"]]  ={0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_enlace"]]    ={0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
		valueWeights[dataIndex["grow_hp"]]                 ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]]                 ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_accuracy"]]           ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]]            ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_stun"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]]       ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_slow"]]         ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}

  elseif tmplID == magics["xueren03"].tmplID then

		valueWeights[dataIndex["default_hp"]]              ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_mp"]]              ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_low"]]      ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_high"]]     ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_accuracy"]]        ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_dodge"]]           ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_defense"]]         ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_stun"]]      ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_sleep"]]     ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_silence"]]   ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_weakness"]]  ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_enlace"]]    ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
		valueWeights[dataIndex["grow_hp"]]                 ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]]                 ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_accuracy"]]           ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]]            ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_stun"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]]       ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_slow"]]         ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}

  elseif tmplID == magics["xueren04"].tmplID 
  or tmplID == magics["pengzhuang1"].tmplID
  or tmplID == magics["pengzhuang2"].tmplID
  or tmplID == magics["pengzhuang3"].tmplID
  then

		valueWeights[dataIndex["default_hp"]]              ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_mp"]]              ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_damage_low"]]      ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_damage_high"]]     ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_accuracy"]]        ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_dodge"]]           ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_defense"]]         ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]]      ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_sleep"]]     ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_silence"]]   ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_weakness"]]  ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_enlace"]]    ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
		valueWeights[dataIndex["grow_hp"]]                 ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]]                 ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_accuracy"]]           ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]]            ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_stun"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]]       ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_slow"]]         ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}

  ---三界乱斗BOSS掉落形象
  elseif tmplID == magics["sanjie01"].tmplID or tmplID == magics["shennong"].tmplID then

		valueWeights[dataIndex["default_hp"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_mp"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_low"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_high"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_accuracy"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_dodge"]]           ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_defense"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_stun"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_sleep"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_silence"]]   ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_weakness"]]  ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_enlace"]]    ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
		valueWeights[dataIndex["grow_hp"]]                 ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]]                 ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_accuracy"]]           ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]]            ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_stun"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]]       ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_slow"]]         ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}

  ---天界月神变身形象
  elseif tmplID == magics["yueshen"].tmplID or tmplID == magics["shouhuzhe"].tmplID or tmplID == magics["2013dijun"].tmplID then

		valueWeights[dataIndex["default_hp"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_mp"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_low"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_damage_high"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_accuracy"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_dodge"]]           ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_defense"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_stun"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_sleep"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_silence"]]   ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_weakness"]]  ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_enlace"]]    ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
		valueWeights[dataIndex["grow_hp"]]                 ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]]                 ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_accuracy"]]           ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]]              ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]]            ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_stun"]]         ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]]        ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]]      ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]]     ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]]       ={1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_slow"]]         ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}

  ---诛仙2派送用幻灵石-雪魄女妖
  elseif tmplID == magics["paisong1"].tmplID then

		valueWeights[dataIndex["default_hp"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_mp"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_damage_low"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_damage_high"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_accuracy"]]        ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_dodge"]]           ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_defense"]]         ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_stun"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_sleep"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_silence"]]   ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_weakness"]]  ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_enlace"]]    ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
		valueWeights[dataIndex["grow_hp"]]                 ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_mp"]]                 ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_damage_low"]]         ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_damage_high"]]        ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_accuracy"]]           ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_dodge"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_defense"]]            ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_stun"]]         ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_sleep"]]        ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_silence"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_weakness"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_enlace"]]       ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_slow"]]         ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}


  elseif tmplID == magics["paisong2"].tmplID then
	
		valueWeights[dataIndex["default_hp"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_mp"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_damage_low"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_damage_high"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_accuracy"]]        ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_dodge"]]           ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_defense"]]         ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_stun"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_sleep"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_silence"]]   ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_weakness"]]  ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_enlace"]]    ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
		valueWeights[dataIndex["grow_hp"]]                 ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_mp"]]                 ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_damage_low"]]         ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_damage_high"]]        ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_accuracy"]]           ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_dodge"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_defense"]]            ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_stun"]]         ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_sleep"]]        ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_silence"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_weakness"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_enlace"]]       ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_slow"]]         ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}

  elseif tmplID == magics["paisong3"].tmplID then
	
		valueWeights[dataIndex["default_hp"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_mp"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_damage_low"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_damage_high"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_accuracy"]]        ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_dodge"]]           ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_defense"]]         ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_stun"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_sleep"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_silence"]]   ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_weakness"]]  ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_enlace"]]    ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
		valueWeights[dataIndex["grow_hp"]]                 ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_mp"]]                 ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_damage_low"]]         ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_damage_high"]]        ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_accuracy"]]           ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_dodge"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_defense"]]            ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_stun"]]         ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_sleep"]]        ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_silence"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_weakness"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_enlace"]]       ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_slow"]]         ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}



  ---店卖变身道具01（0.3*1档数值）
  elseif tmplID == magics["huamei"].tmplID then
	
		valueWeights[dataIndex["default_hp"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_mp"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_damage_low"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_damage_high"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_accuracy"]]        ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_dodge"]]           ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_defense"]]         ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_stun"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_sleep"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_silence"]]   ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_weakness"]]  ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_enlace"]]    ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
		valueWeights[dataIndex["grow_hp"]]              ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]]              ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]]      ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]]     ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_accuracy"]]        ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]]           ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]]         ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_stun"]]      ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]]     ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]]   ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]]  ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]]    ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}

  elseif tmplID == magics["hanfei"].tmplID then
	
		valueWeights[dataIndex["default_hp"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_mp"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_damage_low"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_damage_high"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_accuracy"]]        ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_dodge"]]           ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_defense"]]         ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_stun"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_sleep"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_silence"]]   ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_weakness"]]  ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_enlace"]]    ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
		valueWeights[dataIndex["grow_hp"]]              ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]]              ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]]      ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]]     ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_accuracy"]]        ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]]           ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]]         ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_stun"]]      ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]]     ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]]   ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]]  ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]]    ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}

  elseif tmplID == magics["linghou"].tmplID then
	
		valueWeights[dataIndex["default_hp"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_mp"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_damage_low"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_damage_high"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_accuracy"]]        ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_dodge"]]           ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_defense"]]         ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_stun"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_sleep"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_silence"]]   ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_weakness"]]  ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_enlace"]]    ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
		valueWeights[dataIndex["grow_hp"]]              ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]]              ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]]      ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]]     ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_accuracy"]]        ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]]           ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]]         ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_stun"]]      ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]]     ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]]   ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]]  ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]]    ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}

  elseif tmplID == magics["yingchifeng"].tmplID then
	
		valueWeights[dataIndex["default_hp"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_mp"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_damage_low"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_damage_high"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_accuracy"]]        ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_dodge"]]           ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_defense"]]         ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_stun"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_sleep"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_silence"]]   ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_weakness"]]  ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_enlace"]]    ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
		valueWeights[dataIndex["grow_hp"]]              ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]]              ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]]      ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]]     ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_accuracy"]]        ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]]           ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]]         ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_stun"]]      ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]]     ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]]   ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]]  ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]]    ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
  
  elseif tmplID == magics["huayao"].tmplID then
	
		valueWeights[dataIndex["default_hp"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_mp"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_damage_low"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_damage_high"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_accuracy"]]        ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_dodge"]]           ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_defense"]]         ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_stun"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_sleep"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_silence"]]   ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_weakness"]]  ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_enlace"]]    ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
		valueWeights[dataIndex["grow_hp"]]              ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]]              ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]]      ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]]     ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_accuracy"]]        ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]]           ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]]         ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_stun"]]      ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]]     ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]]   ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]]  ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]]    ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}

  elseif tmplID == magics["guidaoshi"].tmplID then
	
		valueWeights[dataIndex["default_hp"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_mp"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_damage_low"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_damage_high"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_accuracy"]]        ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_dodge"]]           ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_defense"]]         ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_stun"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_sleep"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_silence"]]   ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_weakness"]]  ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_enlace"]]    ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
		valueWeights[dataIndex["grow_hp"]]              ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]]              ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]]      ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]]     ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_accuracy"]]        ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]]           ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]]         ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_stun"]]      ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]]     ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]]   ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]]  ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]]    ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}

  ---2010春节五福虎
  elseif tmplID == magics["wufuhu01"].tmplID 
  or tmplID == magics["wufuhu02"].tmplID 
  or tmplID == magics["wufuhu03"].tmplID 
  or tmplID == magics["wufuhu04"].tmplID 
  or tmplID == magics["wufuhu05"].tmplID then
	
		valueWeights[dataIndex["default_hp"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_mp"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_damage_low"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_damage_high"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_accuracy"]]        ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_dodge"]]           ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_defense"]]         ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_stun"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_sleep"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_silence"]]   ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_weakness"]]  ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_enlace"]]    ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
		valueWeights[dataIndex["grow_hp"]]              ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_mp"]]              ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_low"]]      ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_damage_high"]]     ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_accuracy"]]        ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_dodge"]]           ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_defense"]]         ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_stun"]]      ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_sleep"]]     ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_silence"]]   ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_weakness"]]  ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_enlace"]]    ={1,4,6,8,10,4,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,}
		valueWeights[dataIndex["grow_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}

  ---2010-6-公会VIP礼包奖励 男/女
  elseif tmplID == magics["2010VIPman"].tmplID 
  		or tmplID == magics["2010VIPwoman"].tmplID	then

		valueWeights[dataIndex["default_hp"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_mp"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_damage_low"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_damage_high"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_accuracy"]]        ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_dodge"]]           ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_defense"]]         ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_stun"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_sleep"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_silence"]]   ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_weakness"]]  ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_enlace"]]    ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
		valueWeights[dataIndex["grow_hp"]]                 ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_mp"]]                 ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_damage_low"]]         ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_damage_high"]]        ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_accuracy"]]           ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_dodge"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_defense"]]            ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_stun"]]         ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_sleep"]]        ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_silence"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_weakness"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_enlace"]]       ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_slow"]]         ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}

  ---2010跨服PK奖励shoushen
  elseif tmplID == magics["kuafushoushen"].tmplID or 
  tmplID == magics["qingxin"].tmplID or
  tmplID == magics["2012VIPhuolong"].tmplID or
  tmplID == magics["2012huigui01"].tmplID or
  tmplID == magics["2012huigui02"].tmplID or
  tmplID == magics["2012huigui03"].tmplID or
  tmplID == magics["2012huigui04"].tmplID or
  tmplID == magics["2012huigui05"].tmplID or
  tmplID == magics["2012huigui06"].tmplID or
  tmplID == magics["yyxiongx"].tmplID or
  tmplID == magics["yyxiongy"].tmplID or
  tmplID == magics["zhouyixian"].tmplID or
  tmplID == magics["caishen"].tmplID or
  tmplID == magics["nantong"].tmplID or
  tmplID == magics["nvtong"].tmplID or
  tmplID == magics["xiaobai2013"].tmplID or
  tmplID == magics["guili2013"].tmplID or
  tmplID == magics["xueqi2013"].tmplID or
  tmplID == magics["biyao2013"].tmplID or
  tmplID == magics["feitianxianzi"].tmplID or
  tmplID == magics["kuafutianjienan"].tmplID or
  tmplID == magics["kuafutianjienv"].tmplID or
  tmplID == magics["gongyuanyang"].tmplID or
  tmplID == magics["kushuangtianshi"].tmplID or
  tmplID == magics["kushuangtianshi2"].tmplID or
  tmplID == magics["zhangyu"].tmplID or
  tmplID == magics["muyuanyang"].tmplID 
  then

		valueWeights[dataIndex["default_hp"]]              ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_mp"]]              ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_damage_low"]]      ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_damage_high"]]     ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_accuracy"]]        ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_dodge"]]           ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_defense"]]         ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_stun"]]      ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_sleep"]]     ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_silence"]]   ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_weakness"]]  ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_enlace"]]    ={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,}
		valueWeights[dataIndex["default_resis_slow"]]      ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}
		
		valueWeights[dataIndex["grow_hp"]]                 ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_mp"]]                 ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_damage_low"]]         ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_damage_high"]]        ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_accuracy"]]           ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_dodge"]]              ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_defense"]]            ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_stun"]]         ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_sleep"]]        ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_silence"]]      ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_weakness"]]     ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_enlace"]]       ={0,0.1,1,1,1.66666666666667,1.66666666666667,1.66666666666667,7.5,15,22.5,31,.19047619047619,.142857142857143,9.52380952380952E-02,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
		valueWeights[dataIndex["grow_resis_slow"]]         ={1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,}


  end 
 	return valueWeights

end

---幻灵石品级定义函数
function ZLuaMagicGenerateQuality(tmplID, grades, nGrades)
	local dataIndex = ZLuaMagicDataIndex()
	local magics = ZLuaAvailableMagics()

	local sum
	sum=0

	sum = sum + grades[dataIndex["grow_hp"]] * 12
	sum = sum + grades[dataIndex["grow_mp"]] * 5
	sum = sum + grades[dataIndex["grow_damage_low"]] * 5
	sum = sum + grades[dataIndex["grow_damage_high"]] * 12
	sum = sum + grades[dataIndex["grow_accuracy"]] * 12
	sum = sum + grades[dataIndex["grow_dodge"]] * 12
	sum = sum + grades[dataIndex["grow_defense"]] * 12
	sum = sum + grades[dataIndex["grow_resis_stun"]] * 4
	sum = sum + grades[dataIndex["grow_resis_sleep"]] * 4
	sum = sum + grades[dataIndex["grow_resis_silence"]] * 4
	sum = sum + grades[dataIndex["grow_resis_weakness"]] * 4
	sum = sum + grades[dataIndex["grow_resis_enlace"]] * 4
	sum = sum + grades[dataIndex["grow_resis_slow"]] * 0
	
	local g = math.floor( sum / ( nGrades * 86 ) * 1000 )

	local l1,l2,l3,l4
	if tmplID == magics["TEST1"].tmplID then
				l1 = 500
				l2 = 750
    		l3 = 875
    		l4 = 940
  	elseif tmplID == magics["TEST2"].tmplID then
				l1 = 1
				l2 = 1
    		l3 = 1
    		l4 = 999
  	elseif tmplID == magics["TEST3"].tmplID 
		or tmplID == magics["TEST4"].tmplID 
		or tmplID == magics["TEST5"].tmplID 
		or tmplID == magics["TEST6"].tmplID 
		or tmplID == magics["TEST7"].tmplID 
  	then
				l1 = 430
				l2 = 580
    		l3 = 750
    		l4 = 800  

  ---赠送充值奖励固定品级优秀
  	elseif tmplID == magics["daoxuan"].tmplID 
  	or tmplID == magics["daoxuan2"].tmplID 
  	or tmplID == magics["kuafushoushen"].tmplID 
  	or tmplID == magics["qingxin"].tmplID 
  	or tmplID == magics["2012VIPhuolong"].tmplID 
  	or tmplID == magics["huluwa"].tmplID 
  	or tmplID == magics["longshen"].tmplID 
  	or tmplID == magics["chiyouzhenshen"].tmplID
  	or tmplID == magics["2012huigui01"].tmplID 
  	or tmplID == magics["2012huigui02"].tmplID 
  	or tmplID == magics["2012huigui03"].tmplID 
  	or tmplID == magics["2012huigui04"].tmplID 
  	or tmplID == magics["2012huigui05"].tmplID 
  	or tmplID == magics["2012huigui06"].tmplID 
  	or tmplID == magics["yyxiongx"].tmplID 
  	or tmplID == magics["yyxiongy"].tmplID 
  	or tmplID == magics["zhouyixian"].tmplID 
  	or tmplID == magics["caishen"].tmplID 
  	or tmplID == magics["nantong"].tmplID 
  	or tmplID == magics["nvtong"].tmplID 
  	or tmplID == magics["xiaobai2013"].tmplID 
  	or tmplID == magics["guili2013"].tmplID 
  	or tmplID == magics["xueqi2013"].tmplID 
  	or tmplID == magics["biyao2013"].tmplID 
  	or tmplID == magics["feitianxianzi"].tmplID 
  	or tmplID == magics["kuafutianjienan"].tmplID 
  	or tmplID == magics["kuafutianjienv"].tmplID 
  	or tmplID == magics["gongyuanyang"].tmplID 
  	or tmplID == magics["muyuanyang"].tmplID 
  	or tmplID == magics["kushuangtianshi"].tmplID 
  	or tmplID == magics["kushuangtianshi2"].tmplID 
  	or tmplID == magics["zhangyu"].tmplID 
  	then
				l1 = 1
				l2 = 1
    		l3 = 999
    		l4 = 999

  ---国庆活动奖励固定品级杰出
  	elseif tmplID == magics["guoqing01"].tmplID then
				l1 = 1
				l2 = 1
    		l3 = 1
    		l4 = 999

  	elseif tmplID == magics["guoqing02"].tmplID then
				l1 = 1
				l2 = 1
    		l3 = 1
    		l4 = 999

  	elseif tmplID == magics["guoqing03"].tmplID then
				l1 = 1
				l2 = 1
    		l3 = 1
    		l4 = 999

  ---重阳活动用
  	elseif tmplID == magics["chongyang"].tmplID 
  	or tmplID == magics["xiaolangren"].tmplID 
  	or tmplID == magics["huoyan"].tmplID 
  	or tmplID == magics["huowa"].tmplID 
  	or tmplID == magics["kuishe"].tmplID 
  	or tmplID == magics["guilifei"].tmplID 
  	or tmplID == magics["xueguoteshi"].tmplID 
  	then
				l1 = 1
				l2 = 1
    		l3 = 999
    		l4 = 999

  ---雪人活动用
  	elseif tmplID == magics["xueren01"].tmplID then
				l1 = 1
				l2 = 1
    		l3 = 999
    		l4 = 999

  	elseif tmplID == magics["xueren02"].tmplID then
				l1 = 1
				l2 = 1
    		l3 = 999
    		l4 = 999

  	elseif tmplID == magics["xueren03"].tmplID then
				l1 = 1
				l2 = 1
    		l3 = 999
    		l4 = 999

  	elseif tmplID == magics["xueren04"].tmplID
  	or tmplID == magics["pengzhuang1"].tmplID
  	or tmplID == magics["pengzhuang2"].tmplID
  	or tmplID == magics["pengzhuang3"].tmplID
  	then
				l1 = 1
				l2 = 1
    		l3 = 999
    		l4 = 999

  ---三界乱斗BOSS掉落形象01
  	elseif tmplID == magics["sanjie01"].tmplID or tmplID == magics["shennong"].tmplID then
				l1 = 1
				l2 = 1
    		l3 = 1
    		l4 = 999

  ---天界月神变身形象01
  	elseif tmplID == magics["yueshen"].tmplID or tmplID == magics["shouhuzhe"].tmplID or tmplID == magics["2013dijun"].tmplID then
				l1 = 1
				l2 = 1
    		l3 = 1
    		l4 = 999

  ---派送限时
  	elseif tmplID == magics["paisong1"].tmplID then
				l1 = 205
				l2 = 215
    		l3 = 225
    		l4 = 230 
  	elseif tmplID == magics["paisong2"].tmplID then
				l1 = 205
				l2 = 215
    		l3 = 225
    		l4 = 230 
  	elseif tmplID == magics["paisong3"].tmplID then
				l1 = 205
				l2 = 215
    		l3 = 225
    		l4 = 230 

  ---商店卖6种
  	elseif tmplID == magics["huamei"].tmplID then
				l1 = 90
				l2 = 105
    		l3 = 115
   			l4 = 125 
  	elseif tmplID == magics["hanfei"].tmplID then
				l1 = 90
				l2 = 105
    		l3 = 115
    		l4 = 125
  	elseif tmplID == magics["linghou"].tmplID then
				l1 = 90
				l2 = 105
    		l3 = 115
    		l4 = 125
  	elseif tmplID == magics["yingchifeng"].tmplID then
				l1 = 90
				l2 = 105
    		l3 = 115
    		l4 = 125
  	elseif tmplID == magics["huayao"].tmplID then
				l1 = 90
				l2 = 105
    		l3 = 115
    		l4 = 125
  	elseif tmplID == magics["guidaoshi"].tmplID then
				l1 = 90
				l2 = 105
    		l3 = 115
    		l4 = 125  

  ---2010春节五福虎
  	elseif tmplID == magics["wufuhu01"].tmplID 
  			or tmplID == magics["wufuhu02"].tmplID
  			or tmplID == magics["wufuhu03"].tmplID
  			or tmplID == magics["wufuhu04"].tmplID
  			or tmplID == magics["wufuhu05"].tmplID then
				l1 = 90
				l2 = 105
  	  	l3 = 115
   			l4 = 125 

		---2010公会VIP礼包
  	elseif tmplID == magics["2010VIPman"].tmplID 
  			or tmplID == magics["2010VIPwoman"].tmplID then
				l1 = 205
				l2 = 215
    		l3 = 225
    		l4 = 230 

  	end
  
 	local quality
	if g <= l1 then quality = 1
	elseif l1 < g and g <= l2 then quality = 2
	elseif l2 < g and g <= l3 then quality = 3
	elseif l3 < g and g <= l4 then quality = 4
	else quality = 5 end

	return quality
end

function ZLuaMagicGenerateQualityFromMagic(tmplID, magic)
	local dataIndex = ZLuaMagicDataIndex()
	local nGrades, valueGrades = ZLuaMagicGrades()
	local grades = {}
	grades[dataIndex["default_hp"]] 						= ZLuaMagicGetGradeFromValue(dataIndex["default_hp"],magic)
	grades[dataIndex["default_mp"]] 						= ZLuaMagicGetGradeFromValue(dataIndex["default_mp"],magic)
	grades[dataIndex["default_damage_low"]] 		= ZLuaMagicGetGradeFromValue(dataIndex["default_damage_low"],magic)
	grades[dataIndex["default_damage_high"]] 		= ZLuaMagicGetGradeFromValue(dataIndex["default_damage_high"],magic)
	grades[dataIndex["default_accuracy"]]     	= ZLuaMagicGetGradeFromValue(dataIndex["default_accuracy"],magic)
	grades[dataIndex["default_dodge"]] 		      = ZLuaMagicGetGradeFromValue(dataIndex["default_dodge"],magic)
	grades[dataIndex["default_defense"]] 		    = ZLuaMagicGetGradeFromValue(dataIndex["default_defense"],magic)
	grades[dataIndex["default_resis_stun"]] 		= ZLuaMagicGetGradeFromValue(dataIndex["default_resis_stun"],magic)
	grades[dataIndex["default_resis_sleep"]] 		= ZLuaMagicGetGradeFromValue(dataIndex["default_resis_sleep"],magic)
	grades[dataIndex["default_resis_silence"]] 	= ZLuaMagicGetGradeFromValue(dataIndex["default_resis_silence"],magic)
	grades[dataIndex["default_resis_weakness"]] = ZLuaMagicGetGradeFromValue(dataIndex["default_resis_weakness"],magic)
	grades[dataIndex["default_resis_enlace"]] 	= ZLuaMagicGetGradeFromValue(dataIndex["default_resis_enlace"],magic)
	grades[dataIndex["default_resis_slow"]] 		= ZLuaMagicGetGradeFromValue(dataIndex["default_resis_slow"],magic)
		
	grades[dataIndex["grow_hp"]] 								= ZLuaMagicGetGradeFromValue(dataIndex["grow_hp"],magic)
	grades[dataIndex["grow_mp"]] 								= ZLuaMagicGetGradeFromValue(dataIndex["grow_mp"],magic)
	grades[dataIndex["grow_damage_low"]] 				= ZLuaMagicGetGradeFromValue(dataIndex["grow_damage_low"],magic)
	grades[dataIndex["grow_damage_high"]] 			= ZLuaMagicGetGradeFromValue(dataIndex["grow_damage_high"],magic)
	grades[dataIndex["grow_accuracy"]] 		    	= ZLuaMagicGetGradeFromValue(dataIndex["grow_accuracy"],magic)
	grades[dataIndex["grow_dodge"]] 						= ZLuaMagicGetGradeFromValue(dataIndex["grow_dodge"],magic)
	grades[dataIndex["grow_defense"]] 		     	= ZLuaMagicGetGradeFromValue(dataIndex["grow_defense"],magic)
	grades[dataIndex["grow_resis_stun"]] 				= ZLuaMagicGetGradeFromValue(dataIndex["grow_resis_stun"],magic)
	grades[dataIndex["grow_resis_sleep"]] 			= ZLuaMagicGetGradeFromValue(dataIndex["grow_resis_sleep"],magic)
	grades[dataIndex["grow_resis_silence"]] 		= ZLuaMagicGetGradeFromValue(dataIndex["grow_resis_silence"],magic)
	grades[dataIndex["grow_resis_weakness"]] 		= ZLuaMagicGetGradeFromValue(dataIndex["grow_resis_weakness"],magic)
	grades[dataIndex["grow_resis_enlace"]] 			= ZLuaMagicGetGradeFromValue(dataIndex["grow_resis_enlace"],magic)
	grades[dataIndex["grow_resis_slow"]] 		  	= ZLuaMagicGetGradeFromValue(dataIndex["grow_resis_slow"],magic)

	return ZLuaMagicGenerateQuality(tmplID, grades, nGrades)
end

function ZLuaMagicGetGradeFromValue(index, magic)		 
	local nGrades, valueGrades = ZLuaMagicGrades()
	local delta = ( valueGrades[index][50] - valueGrades[index][1] ) / 49
	local v0 = valueGrades[index][1] - delta
	for i = 1, 555 do
		if magic:QueryData(index) < v0 + ( i * delta ) then
			return i
		end
	end
	return 1
end


function ZLuaMagic_Get_RanPoint(s1)
	if not ZLuaMagic_CheckIsNumberTable(s1,0) then return end 
	local js,ron1
	ron1= math.random()                

  local p1=ZLuaMagic_Chance_Norm(s1)
	for js=1,table.getn(p1) do
          if ron1<=p1[js] then 

    	  		return js
		  		end
	end
	return 1
end

function ZLuaMagic_CheckIsNumberTable(ANumTable,Numr_Min)
	local tm
	if type(ANumTable)~="table" then return false end  
	if type(ANumTable)=="table" then
		for tm=1,table.getn(ANumTable) do 
			if type(ANumTable[tm])~="number" or ANumTable[tm]<Numr_Min then
				return false 
            end 
		end  
	end  
	return true
end 

function ZLuaMagic_Chance_Norm(n1)
  if not ZLuaMagic_CheckIsNumberTable(n1,0) then return end 
	local n1_size=table.getn(n1)
	local Total_Chance=0
	local i,m,k
	---先求和
	for i=1,n1_size do
   		Total_Chance=n1[i]+Total_Chance          
	end
 	---生成概率组序列
	local nx={}
	for k=1,n1_size do
		if k==1 then 
			nx[1]=n1[k]
		else
	   	nx[k]=n1[k]+nx[k-1]              
	  end
	end

	---再生产标准化的概率组序列
	local n2={}
     for m=1,n1_size do
     		if Total_Chance==0 then                 ---对于全0序列的处理，返回{1,0,0...}
			 		n2[m]=0
					n2[1]=1
				else
					n2[m]=nx[m]/ Total_Chance
				end
	 	 end
	---返回一个概率序列 
	return n2
end


----------------------------主要处理函数---------------------------
---幻灵石初始化
function MagicInit(tmplID, magic)
	if ZLuaMagicExistInTmplIDs(tmplID) == 0 then return 0 end

	if magic == nil then return 0 end

	local magics = ZLuaAvailableMagics()
	local dataIndex,resisIndex = ZLuaMagicDataIndex()
	local randomResult,quality = ZLuaMagicRandomResult(tmplID)

	---生成幻灵石本体属性初值、增长量
	magic:UpdateData(dataIndex["default_hp"], math.floor(randomResult[dataIndex["default_hp"]]*100)/100);
	magic:UpdateData(dataIndex["default_mp"], math.floor(randomResult[dataIndex["default_mp"]]*100)/100);
	magic:UpdateData(dataIndex["default_damage_low"], math.floor(math.min(randomResult[dataIndex["default_damage_low"]],randomResult[dataIndex["default_damage_high"]])*100)/100);
	magic:UpdateData(dataIndex["default_damage_high"], math.floor(randomResult[dataIndex["default_damage_high"]]*100)/100);
	magic:UpdateData(dataIndex["default_accuracy"], math.floor(randomResult[dataIndex["default_accuracy"]]*100)/100);
	magic:UpdateData(dataIndex["default_dodge"], math.floor(randomResult[dataIndex["default_dodge"]]*100)/100);
	magic:UpdateData(dataIndex["default_defense"], math.floor(randomResult[dataIndex["default_defense"]]*100)/100);
	magic:UpdateData(dataIndex["default_resis_stun"], math.floor(randomResult[dataIndex["default_resis_stun"]]*100)/100);
	magic:UpdateData(dataIndex["default_resis_sleep"], math.floor(randomResult[dataIndex["default_resis_sleep"]]*100)/100);
	magic:UpdateData(dataIndex["default_resis_silence"], math.floor(randomResult[dataIndex["default_resis_silence"]]*100)/100);
	magic:UpdateData(dataIndex["default_resis_weakness"],math.floor(randomResult[dataIndex["default_resis_weakness"]]*100)/100);
	magic:UpdateData(dataIndex["default_resis_enlace"], math.floor(randomResult[dataIndex["default_resis_enlace"]]*100)/100);
	magic:UpdateData(dataIndex["default_resis_slow"], math.floor(randomResult[dataIndex["default_resis_slow"]]*100)/100);	

	
	
	magic:UpdateData(dataIndex["grow_hp"], math.floor(randomResult[dataIndex["grow_hp"]]*100)/100);
	magic:UpdateData(dataIndex["grow_mp"], math.floor(randomResult[dataIndex["grow_mp"]]*100)/100);
	magic:UpdateData(dataIndex["grow_damage_low"], math.floor(math.min(randomResult[dataIndex["grow_damage_low"]],randomResult[dataIndex["grow_damage_high"]])*100)/100);
	magic:UpdateData(dataIndex["grow_damage_high"], math.floor(randomResult[dataIndex["grow_damage_high"]]*100)/100);
	magic:UpdateData(dataIndex["grow_accuracy"], math.floor(randomResult[dataIndex["grow_accuracy"]]*100)/100);
	magic:UpdateData(dataIndex["grow_dodge"], math.floor(randomResult[dataIndex["grow_dodge"]]*100)/100);
	magic:UpdateData(dataIndex["grow_defense"], math.floor(randomResult[dataIndex["grow_defense"]]*100)/100);
	magic:UpdateData(dataIndex["grow_resis_stun"], math.floor(randomResult[dataIndex["grow_resis_stun"]]*100)/100);
	magic:UpdateData(dataIndex["grow_resis_sleep"], math.floor(randomResult[dataIndex["grow_resis_sleep"]]*100)/100);
	magic:UpdateData(dataIndex["grow_resis_silence"], math.floor(randomResult[dataIndex["grow_resis_silence"]]*100)/100);
	magic:UpdateData(dataIndex["grow_resis_weakness"],math.floor(randomResult[dataIndex["grow_resis_weakness"]]*100)/100);
	magic:UpdateData(dataIndex["grow_resis_enlace"], math.floor(randomResult[dataIndex["grow_resis_enlace"]]*100)/100);
	magic:UpdateData(dataIndex["grow_resis_slow"], math.floor(randomResult[dataIndex["grow_resis_slow"]]*100)/100);	

	
	---生成保留数据
	magic:UpdateData(dataIndex["reserve1"], 0)
	magic:UpdateData(dataIndex["reserve2"], 0)		

	
	---生成品质
  magic:UpdateData(dataIndex["quality"], quality)
  
  if tmplID == magics["TEST2"].tmplID 
  or tmplID == magics["TEST3"].tmplID 
  or tmplID == magics["TEST4"].tmplID 
  or tmplID == magics["TEST5"].tmplID 
  or tmplID == magics["TEST6"].tmplID 
  or tmplID == magics["TEST7"].tmplID 
  or tmplID == magics["TEST1"].tmplID 
  then
    magic:SetLevel(16)
    magic:SetExpLevel(5)
    magic:SetExp(0)
    magic:SetDuration(220)
  elseif tmplID == 32108 then
    magic:SetLevel(9)
    magic:SetExpLevel(5)
    magic:SetExp(0)
    magic:SetDuration(220)
  elseif tmplID == 32109 then
    magic:SetLevel(12)
    magic:SetExpLevel(5)
    magic:SetExp(0)
    magic:SetDuration(280)
  elseif tmplID == 32110 then
    magic:SetLevel(15)
    magic:SetExpLevel(5)
    magic:SetExp(0)
    magic:SetDuration(340)
  elseif tmplID == magics["sanjie01"].tmplID or tmplID == magics["shennong"].tmplID then
    magic:SetLevel(16)
    magic:SetExpLevel(5)
    magic:SetExp(0)
    magic:SetDuration(360)
  elseif tmplID == magics["yueshen"].tmplID or tmplID == magics["shouhuzhe"].tmplID or tmplID == magics["2013dijun"].tmplID then
    magic:SetLevel(16)
    magic:SetExpLevel(5)
    magic:SetExp(0)
    magic:SetDuration(360)
  else
    magic:SetLevel(1)
    magic:SetExpLevel(1)
    magic:SetExp(0)
    magic:SetDuration(10)
  end
  ZLuaMagicRebuild(magic,tmplID);

  return 1
end


---幻灵石提升契合等级（expLevelUp）
function MagicExpLevelUp(tmplID, magic)
  local dataIndex,resisIndex = ZLuaMagicDataIndex()
  local expLevel = magic:QueryExpLevel()
  ---判断当前契合值达到最大值
    if expLevel <= 4 and expLevel >= 1 then
      expLevel = expLevel + 1
    else return 0 end

  magic:SetExpLevel(math.floor(expLevel))

  ZLuaMagicRebuild(magic,tmplID);
  return 1
end





---幻灵石属性重新计算
function ZLuaMagicRebuild(magic,tmplID)
	local magics = ZLuaAvailableMagics()

  if ZLuaMagicExistInTmplIDs(tmplID) == 0 then return end

  local dataIndex,resisIndex = ZLuaMagicDataIndex()
	local expLevelQuo = ZLuaMagicExpLevelQuo()
	---取得幻灵石等级
  if magic:QueryLevel()==nil then return end
  if magic:QueryExpLevel()==nil then return end
  
  magic:SetQuality(magic:QueryData(dataIndex["quality"]))

	
	local level = magic:QueryLevel()
	local expLevel = magic:QueryExpLevel()

	
	---设置幻灵石灵气值上限
	local duration = ZLuaDuration()
	magic:SetMaxDuration( math.max(0,duration[(level-1)*5+expLevel]) );

	
	---设置幻灵石契合值上限
	local expValue = ZLuaExpValue()
	magic:SetMaxExp(math.max(0,expValue[(level-1)*5+expLevel]))
	
	---设置幻灵石契合值增长速度
	local expValueSpeed = ZLuaExpIncreaseSpeed()
	magic:SetExpSpeed(math.max(0,expValueSpeed[(level-1)*5+expLevel]))
	
	
				
	---设置幻灵石本体属性

	
	local defaultHP = math.max(0, magic:QueryData(dataIndex["default_hp"]))

	local defaultMP = math.max(0, magic:QueryData(dataIndex["default_mp"]))
	local defaultDamageLow = math.max(0, magic:QueryData(dataIndex["default_damage_low"]))
	local defaultDamageHigh = math.max(0, magic:QueryData(dataIndex["default_damage_high"]))
	local defaultAccuracy = math.max(0, magic:QueryData(dataIndex["default_accuracy"]))
	local defaultDodge = math.max(0, magic:QueryData(dataIndex["default_dodge"]))
	local defaultDefense = math.max(0, magic:QueryData(dataIndex["default_defense"]))
	local defaultResisStun = math.max(0, magic:QueryData(dataIndex["default_resis_stun"]))
	local defaultResisSleep = math.max(0, magic:QueryData(dataIndex["default_resis_sleep"]))
	local defaultResisSilence = math.max(0, magic:QueryData(dataIndex["default_resis_silence"]))
	local defaultResisWeakness = math.max(0, magic:QueryData(dataIndex["default_resis_weakness"]))
	local defaultResisEnlace = math.max(0, magic:QueryData(dataIndex["default_resis_enlace"]))
	local defaultResisSlow = math.max(0, magic:QueryData(dataIndex["default_resis_slow"]))
		
	local growHP = math.max(0, magic:QueryData(dataIndex["grow_hp"]))
	local growMP = math.max(0, magic:QueryData(dataIndex["grow_mp"]))
	local growDamageLow = math.max(0, magic:QueryData(dataIndex["grow_damage_low"]))
	local growDamageHigh = math.max(0, magic:QueryData(dataIndex["grow_damage_high"]))
	local growAccuracy = math.max(0, magic:QueryData(dataIndex["grow_accuracy"]))
	local growDodge = math.max(0, magic:QueryData(dataIndex["grow_dodge"]))
	local growDefense = math.max(0, magic:QueryData(dataIndex["grow_defense"]))
	local growResisStun = math.max(0, magic:QueryData(dataIndex["grow_resis_stun"]))
	local growResisSleep = math.max(0, magic:QueryData(dataIndex["grow_resis_sleep"]))
	local growResisSilence = math.max(0, magic:QueryData(dataIndex["grow_resis_silence"]))
	local growResisWeakness = math.max(0, magic:QueryData(dataIndex["grow_resis_weakness"]))
	local growResisEnlace = math.max(0, magic:QueryData(dataIndex["grow_resis_enlace"]))
	local growResisSlow = math.max(0, magic:QueryData(dataIndex["grow_resis_slow"]))

  ---国庆活动变身道具增加固定属性值，id = 32108，32109，32110
  if tmplID == magics["TEST2"].tmplID then          ---测试用29899

	magic:SetHP( 2000 )
	magic:SetMP( 2000 )
	magic:SetDamage( 100,100 )
	magic:SetAttack( 0 )
	magic:SetArmor( 0 )
	magic:SetDefense( 0 )
	magic:SetResistance( resisIndex["resis_stun"], 0 )
	magic:SetResistance( resisIndex["resis_sleep"], 0 )
	magic:SetResistance( resisIndex["resis_silence"], 0 )
	magic:SetResistance( resisIndex["resis_weakness"], 0 )
	magic:SetResistance( resisIndex["resis_enlace"], 0 )
	magic:SetResistance( resisIndex["resis_slow"], 0 )

  elseif tmplID == 32108 then

	magic:SetHP( 500 )
	magic:SetMP( 500 )
	magic:SetDamage( 50,50 )
	magic:SetAttack( 0 )
	magic:SetArmor( 0 )
	magic:SetDefense( 0 )
	magic:SetResistance( resisIndex["resis_stun"], 0 )
	magic:SetResistance( resisIndex["resis_sleep"], 0 )
	magic:SetResistance( resisIndex["resis_silence"], 0 )
	magic:SetResistance( resisIndex["resis_weakness"], 0 )
	magic:SetResistance( resisIndex["resis_enlace"], 0 )
	magic:SetResistance( resisIndex["resis_slow"], 0 )
  
  elseif tmplID == 32109 then

	magic:SetHP( 1000 )
	magic:SetMP( 1000 )
	magic:SetDamage( 100,100 )
	magic:SetAttack( 0 )
	magic:SetArmor( 0 )
	magic:SetDefense( 0 )
	magic:SetResistance( resisIndex["resis_stun"], 0 )
	magic:SetResistance( resisIndex["resis_sleep"], 0 )
	magic:SetResistance( resisIndex["resis_silence"], 0 )
	magic:SetResistance( resisIndex["resis_weakness"], 0 )
	magic:SetResistance( resisIndex["resis_enlace"], 0 )
	magic:SetResistance( resisIndex["resis_slow"], 0 )

  elseif tmplID == 32110 then

	magic:SetHP( 2000 )
	magic:SetMP( 2000 )
	magic:SetDamage( 100,100 )
	magic:SetAttack( 0 )
	magic:SetArmor( 0 )
	magic:SetDefense( 0 )
	magic:SetResistance( resisIndex["resis_stun"], 0 )
	magic:SetResistance( resisIndex["resis_sleep"], 0 )
	magic:SetResistance( resisIndex["resis_silence"], 0 )
	magic:SetResistance( resisIndex["resis_weakness"], 0 )
	magic:SetResistance( resisIndex["resis_enlace"], 0 )
	magic:SetResistance( resisIndex["resis_slow"], 0 )

  elseif tmplID == magics["sanjie01"].tmplID then          ---三界乱斗01

	magic:SetHP( 20000 )
	magic:SetMP( 10000 )
	magic:SetDamage( 2000,3000 )
	magic:SetAttack( 0 )
	magic:SetArmor( 0 )
	magic:SetDefense( 0 )
	magic:SetResistance( resisIndex["resis_stun"], 0 )
	magic:SetResistance( resisIndex["resis_sleep"], 0 )
	magic:SetResistance( resisIndex["resis_silence"], 0 )
	magic:SetResistance( resisIndex["resis_weakness"], 0 )
	magic:SetResistance( resisIndex["resis_enlace"], 0 )
	magic:SetResistance( resisIndex["resis_slow"], 0 )
  
  elseif tmplID == magics["shennong"].tmplID then          ---神农幻灵石

	magic:SetHP( 20000 )
	magic:SetMP( 12000 )
	magic:SetDamage( 0,0 )
	magic:SetAttack( 0 )
	magic:SetArmor( 0 )
	magic:SetDefense( 0 )
	magic:SetResistance( resisIndex["resis_stun"], 0 )
	magic:SetResistance( resisIndex["resis_sleep"], 0 )
	magic:SetResistance( resisIndex["resis_silence"], 0 )
	magic:SetResistance( resisIndex["resis_weakness"], 0 )
	magic:SetResistance( resisIndex["resis_enlace"], 0 )
	magic:SetResistance( resisIndex["resis_slow"], 0 )
  
  elseif tmplID == magics["yueshen"].tmplID or tmplID == magics["shouhuzhe"].tmplID or tmplID == magics["2013dijun"].tmplID then          ---天界月神

	magic:SetHP( 25000 )
	magic:SetMP( 15000 )
	magic:SetDamage( 2800,3200 )
	magic:SetAttack( 0 )
	magic:SetArmor( 0 )
	magic:SetDefense( 0 )
	magic:SetResistance( resisIndex["resis_stun"], 0 )
	magic:SetResistance( resisIndex["resis_sleep"], 0 )
	magic:SetResistance( resisIndex["resis_silence"], 0 )
	magic:SetResistance( resisIndex["resis_weakness"], 0 )
	magic:SetResistance( resisIndex["resis_enlace"], 0 )
	magic:SetResistance( resisIndex["resis_slow"], 0 )

  else
	
	magic:SetHP( math.floor(( defaultHP + growHP * (level-1) ) * expLevelQuo[(level-1)*5+expLevel] ))
	magic:SetMP( math.floor(( defaultMP + growMP * (level-1) ) * expLevelQuo[(level-1)*5+expLevel] ))
	magic:SetDamage( math.floor(( defaultDamageLow + growDamageLow * (level-1))* expLevelQuo[(level-1)*5+expLevel] ),  math.floor( ( defaultDamageHigh + growDamageHigh * (level-1))*expLevelQuo[(level-1)*5+expLevel] ) )
	magic:SetAttack( math.floor(( defaultAccuracy + growAccuracy * (level-1))* expLevelQuo[(level-1)*5+expLevel] ) )
	magic:SetArmor( math.floor( (defaultDodge + growDodge * (level-1))*expLevelQuo[(level-1)*5+expLevel] ) )
	magic:SetDefense( math.floor( (defaultDefense + growDefense * (level-1))*expLevelQuo[(level-1)*5+expLevel] ) )
	magic:SetResistance( resisIndex["resis_stun"], math.floor( (defaultResisStun + growResisStun * (level-1))*expLevelQuo[(level-1)*5+expLevel] ) )
	magic:SetResistance( resisIndex["resis_sleep"], math.floor( (defaultResisSleep + growResisSleep * (level-1))*expLevelQuo[(level-1)*5+expLevel] ) )
	magic:SetResistance( resisIndex["resis_silence"], math.floor( (defaultResisSilence + growResisSilence * (level-1))*expLevelQuo[(level-1)*5+expLevel] ) )
	magic:SetResistance( resisIndex["resis_weakness"], math.floor( (defaultResisWeakness + growResisWeakness * (level-1))*expLevelQuo[(level-1)*5+expLevel] ) )
	magic:SetResistance( resisIndex["resis_enlace"], math.floor( (defaultResisEnlace + growResisEnlace * (level-1))*expLevelQuo[(level-1)*5+expLevel] ) )
	magic:SetResistance( resisIndex["resis_slow"], math.floor( (defaultResisSlow + growResisSlow * (level-1))*expLevelQuo[(level-1)*5+expLevel] ) )
  
  end
  
  
  ---设置幻灵石附加属性	当前限制最多可附加AddOn个数为8个
	local addonLearnLevel = {}
	local addonLearnExpLevel = {}
	local addonID = {}
	
	local magicAddonLearnLevel, magicAddonLearnExpLevel,magicAddonID = ZLuaMagicAddons(tmplID)

	  for i=1,8 do
	   addonLearnLevel[i] = magicAddonLearnLevel[i]
	   addonLearnExpLevel[i] = magicAddonLearnExpLevel[i]
	   addonID[i] = magicAddonID[i]
	  end
	



	
	for i=1,8 do
		if addonLearnLevel[i] ~= nil and addonLearnLevel[i] > 0 and addonLearnExpLevel[i] ~= nil and addonLearnExpLevel[i] > 0 and addonID[i] ~= nil then
			local unknown = true
			for j=1,level do
				if addonLearnLevel[i] == j then
				  for k=1,expLevel do
	 			   if addonLearnExpLevel[i] == k then
  				  magic:SetAddon( addonID[i] )
					  unknown = false
					 end
					end
				end
			end
		end	
	end


end




---------------------------功能类函数-------------------------------
---判断是否此幻灵石的ID
function ZLuaMagicExistInTmplIDs(tmplID)
	if type(tmplID) ~= "number" then return 0 end
	local availableMagics = ZLuaAvailableMagics()
	local magics
	for _,magics in pairs(availableMagics) do
		if magics.tmplID == tmplID then return 1 end
	end
	return 0
end
