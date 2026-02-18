BGOBJ = bg_player.o cb_player.o ch_player.o cs_team_player.o kf_player.o ar_player.o cs_player.o in_player.o bg_world_manager.o bg_world_message_handler.o cs_flow_player.o

all:	$(BGOBJ)
	
	
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
