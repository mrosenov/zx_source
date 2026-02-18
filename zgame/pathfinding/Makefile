CS2OBJ = ChaseAgent.o  FleeAgent.o  GAStar.o  Graph.o  LayerMap.o  MapNode.o  MAStar.o  MoveMap.o  MultiSecGraph.o  pathfinding.o  PathOptimize.o  PathTrack.o  PfCommon.o  PfSec.o  RambleAgent.o  SectorCommon.o waterareamap.o

#EXTRA_DEF = -D_ASSERT_STRICT_

all:	$(CS2OBJ)
	$(AR) libPathfind.a $(CS2OBJ)
	
.c.o:
	$(CC) $(EXTRA_DEF) -c $(INC) $< -o $@

.cc.o:
	$(CPP) $(EXTRA_DEF) -c $(INC) $< -o $@

.cxx.o:
	$(CPP) $(EXTRA_DEF) -c $(INC) $< -o $@ 

.cpp.o:
	$(CPP) $(EXTRA_DEF) -c $(INC) $< -o $@ 

FORCE:

clean: 	FORCE
	rm -f *.o;rm -f .depend 

depend: FORCE
	make dep;

include ../Rules.make

INC += -I../collision/
