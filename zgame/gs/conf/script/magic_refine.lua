------------------------------------------------------------------------------

---MagicRefine()炼化主函数

---MagicLevelup()幻灵石强化（LevelUp）
---ZLuaMagicReset()幻灵石归元（初始化等级和契合等级，初始值固定）
---ZLuaMagicExpRise()幻灵石提升契合值




function MagicRefine(id, magic, player, refine_type, success_prob, match_value)
  if ZLuaMagicExistInTmplIDs(id) == 0 then return 0,0,0 end
  if id == 32105 ---赠送充值奖励无法炼化
  	or id == 41926 ---2010跨服PK奖励兽神
	  or id == 40566 ---台湾
	  or id == 32108
	  or id == 32109
	  or id == 32110 
	  or id == 32954
	  or id == 33795 ---2010春节五福虎
	  or id == 33796
	  or id == 33797
	  or id == 33798
	  or id == 33799
  then return 0,0,0 end                        
  if refine_type ~= 0 and refine_type ~= 1 and refine_type ~= 2 then return 0,0,0 end
  if refine_type == 1 then 
   return ZLuaMagicLevelUp(id, magic,player, refine_type, success_prob)
  elseif refine_type == 0 then
   return ZLuaMagicReset(id, magic,refine_type)
  elseif refine_type == 2 then
   return ZLuaMagicExpRise(id, magic,refine_type,match_value)
  end
end




---幻灵石强化（LevelUp）
function ZLuaMagicLevelUp(id, magic,player,refine_type, success_prob)
  local dataIndex,resisIndex = ZLuaMagicDataIndex()
  local level = magic:QueryLevel()
  local expLevel = magic:QueryExpLevel()
  local levelupProbability = ZLuaMagicLevelupProbability()
  local r
  local duration = ZLuaDuration()
  local driValue = ZLuaMagicLevelupDriValue()
  local grow = 0
  local channelid = 0
  local msgid = 0
  for i=1,100 do
    r = math.random()
  end
  if expLevel == 5 then
    magic:SetExp(0)

    if r < levelupProbability[level] + tonumber(success_prob) then
      level = level + 1
      magic:SetExpLevel( 1 )
      grow = driValue[level - 1]
      player:AddReputation(12,grow)
      magic:SetLevel( math.floor(level) )  
      magic:SetExp(0)
      ZLuaMagicRebuild(magic,id)
      msgid = 903
        
    else
      magic:SetExpLevel( 4 )
      magic:SetExp(0)
      ZLuaMagicRebuild(magic,id)  
      magic:SetDuration(duration[(level-1)*5+4])
      msgid = 904
    end

    return 1,channelid,msgid
  else return 0,channelid,msgid
  end      

end

---幻灵石归元（初始化等级和契合等级，初始值固定）
function ZLuaMagicReset(id, magic,refine_type)

  local dataIndex,resisIndex = ZLuaMagicDataIndex()
  local channelid = 0
  local msgid = 0
  
  magic:SetLevel( 1 )
  magic:SetExpLevel( 1 )
  magic:SetExp(0)
  ZLuaMagicRebuild(magic,id);
  masgid = 901
  return 1,channelid,msgid
end

---幻灵石契合（提升相应契合值，达到上限自动提升契合等级）
function ZLuaMagicExpRise(id, magic,refine_type,match_value)

  local dataIndex,resisIndex = ZLuaMagicDataIndex()
  local totalValue = math.floor(tonumber(match_value))
  local expValue = ZLuaExpValue()
  local level = magic:QueryLevel()
  local expLevel = math.floor(magic:QueryExpLevel())
  local maxExp = magic:QueryMaxExp()
  local isExpLevelUp = false
  local curExp = magic:QueryExp()
  local channelid = 0
  local msgid = 0
  
  if expLevel==5 then return 0,0,0 end
  while totalValue > 0 and expLevel ~= 5 do
    if ( expValue[(level-1)*5 + expLevel] - curExp ) <= totalValue then
      local i = expValue[(level-1)*5 + expLevel]-curExp

      magic:SetExpLevel(expLevel+1)
      expLevel = expLevel + 1
      magic:SetExp(0)
      isExpLevelUp = true
      curExp = 0

      totalValue = totalValue - i
    else 
      magic:SetExp(curExp+totalValue)
      ZLuaMagicRebuild(magic,id) 
      totalValue = 0
    end
  end
  if isExpLevelUp == true then 
    ZLuaMagicRebuild(magic,id) 
  end
  msgid = 902
  return 1,channelid,msgid
end

