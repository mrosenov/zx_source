ITEMOBJ = item_equip.o item_addon.o item_reinforce.o item_recipe.o item_poppet.o item_teleport.o item_potion.o item_taskdice.o item_dbexp.o item_pkreset.o item_skill.o item_mount.o item_couplejump.o item_lottery.o item_fireworks.o item_recorder.o item_rmcd.o item_talisman.o item_bugle.o item_townscroll.o item_control.o item_petbedge.o item_petequip.o item_petfood.o item_petautofood.o item_aircraft.o item_off_agent.o item_giftbox.o item_specname.o item_vipcard.o item_magic.o item_assistcard.o item_telestation.o item_lottery2.o item_lottery3.o item_onlineaward_giftbox.o item_treasure_digcount.o item_rune.o item_astrology.o item_battleflag.o item_bottle.o item_sale_promotion.o item_giftbox_delivery.o item_lottery_ty.o item_giftbox_set.o item_propadd.o item_fuwen.o item_colorant.o

all:	$(ITEMOBJ)
	
	
.c.o:
	$(CC) -c $(INC) $< -o $@

.cc.o:
	$(CPP) -c $(INC) $< -o $@

.cxx.o:
	$(CPP) -c $(INC) $< -o $@ 

.cpp.o:
	$(CPP) -c $(INC) $< -o $@ 

FORCE:

clean: 	FORCE
	rm -f *.o;rm -f .depend 

depend: FORCE
	make dep;

include ../../Rules.make
