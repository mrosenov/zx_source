BGOBJ = kingdom_player.o kingdom2_player.o kingdom_bath_player.o kingdom_world_manager.o kingdom2_world_manager.o kingdom_bath_world_manager.o kingdom_world_message_handler.o kingdom_battleprepare_filter.o kingdom2_world_message_handler.o kingdom_bath_filter.o

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
