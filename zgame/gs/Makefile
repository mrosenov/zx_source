BASEOBJ = start.o object.o grid.o netmsg.o world.o usermsg.o player.o npc.o gimp.o substance.o msgqueue.o actobject.o item.o matter.o filter.o filter_man.o attack.o item_list.o aggrolist.o actsession.o npcsession.o ainpc.o playercmd.o aipolicy.o playerctrl.o playertemplate.o npcgenerator.o terrain.o playerteam.o playercircleofdoom.o serviceprovider.o servicenpc.o guardnpc.o playertrade.o objmanager.o obj_interface.o potion_filter.o skill_filter.o playertrashbox.o worldmanager.o  invincible_filter.o pvplimit_filter.o playerstall.o gm_player.o npc_filter.o patrol_agent.o cooldown.o aei_filter.o userlogin.o petnpc.o dbgprt.o aitrigger.o serverstat.o aiman.o playerduel.o duel_filter.o playerbind.o playerlink.o mount_filter.o petdataman.o petman.o siegenpc.o amemobj.o towerbuild_filter.o global_drop.o pet_filter.o item_manager.o gmatrix.o playermall.o cmdprepare.o template_loader.o  playerstate.o playertitle.o playerdispatch.o pmvsession.o antiwallow.o reborn_skill.o fly_filter.o player_cult_pvp.o conf.o threadpool.o commonvalue.o commondata.o commondata_if.o general_indexer.o active_buff.o summonman.o summonnpc.o obj_script.o achievement.o playertransform.o transform_filter.o carriernpc.o player_race.o azone_filter.o protectednpc.o player_zone.o circleofdoom_filter.o timesegment.o raid_controller.o playervipaward.o playeronlineaward.o battleflag_filter.o buffarea.o kingdom_filter.o forbid_manager.o mobactivenpc.o mobactivedataman.o mobactiveman.o rage_filter.o facbase_controller.o playeractiveemote.o facbase_mall.o topic_site.o travel_filter.o

GSOBJ = global_message.o global_manager.o  global_world_stubs.o global_table.o 

SUBDIR = item template task ai battleground wallow battlefield territory raid kingdom factionbase
SUBLIB = usercmd/libusercmd.a
ITEMOBJ = item/*.o
TEMPLATEOBJ = template/*.o
TASKOBJ = task/taskman.o 
AIOBJ = ai/*.o
BGOBJ = battleground/*.o
TROBJ = territory/*.o
WALLOWOBJ = wallow/*.o
BFOBJ = battlefield/*.o
RFOBJ = raid/*.o
KDOBJ = kingdom/*.o
FBOBJ = factionbase/*.o
TASKOBJ2 = task/taskman.o task/TaskProcess.o  task/TaskTempl.o  task/TaskTemplMan.o task/TaskServer.o
SUBOBJ= $(ITEMOBJ) $(TEMPLATEOBJ) $(TASKOBJ) $(AIOBJ) $(BGOBJ) $(WALLOWOBJ) $(BFOBJ) $(TROBJ) $(RFOBJ) $(KDOBJ) $(FBOBJ)

all: 	gs

solib:libtask.so

gs:	$(BASEOBJ) $(GSOBJ) $(SUBDIR)  libtask.so
	$(LD) -fPIC $(BASEOBJ) $(GSOBJ) $(SUBOBJ) $(CMLIB) libtask.so libskill.so $(ALLLIB) -o gs 

test:	$(BASEOBJ) $(GSOBJ) $(SUBDIR) 
	$(AR) libtgs.a $(BASEOBJ) $(GSOBJ) $(SUBOBJ) $(CMLIB) 

$(SUBDIR): FORCE
	cd $@ ;make

lib: $(IOLIB) libtask.so collision
	cd ..; make gslib

libtask.so: task 
	cd task; make lib

collision: FORCE
	cd ..; cd collision; make

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
	rm -f *.o *.a;rm -f gs;rm -f libtask.so 
	-($(foreach dir,$(SUBDIR),$(MAKE) -C $(dir) clean;))

depend: FORCE
	make dep;
	-($(foreach dir,$(SUBDIR),$(MAKE) -C $(dir) dep;))

include ../Rules.make
