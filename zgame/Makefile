SUBDIR=gs 
all: collision pathfinding lib $(SUBDIR)

$(SUBDIR): FORCE
	cd $@; make
	
lib: FORCE
	cd gs; make solib;make lib

collision: FORCE
	cd collision; make 

pathfinding: FORCE
	cd pathfinding; make 

gslib:	FORCE
	cd libgs;make
	
$(LIBSUBDIR): FORCE
	cd $@; make 
FORCE:

include Rules.make

