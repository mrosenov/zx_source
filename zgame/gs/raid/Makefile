bfOBJ = raid_player.o raid_world_manager.o raid_world_message_handler.o tower_raid_world_manager.o tower_raid_player.o collision_raid_world_manager.o collision_raid_player.o mafia_battle_raid_world_manager.o mafia_battle_raid_player.o level_raid_world_manager.o level_raid_player.o transform_raid_world_manager.o transform_raid_player.o step_raid_world_manager.o step_raid_player.o seek_raid_world_manager.o seek_raid_player.o capture_raid_world_manager.o capture_raid_player.o

all:	$(bfOBJ)
	
	
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
