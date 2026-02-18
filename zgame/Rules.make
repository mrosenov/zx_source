IOPATH=/root/zx/iozlib
BASEPATH=/root/zx/zgame

INC=-I$(BASEPATH)/collision -I$(BASEPATH)/include -I$(BASEPATH) -I$(IOPATH)/inc -I$(BASEPATH)/lua/include
IOLIB_OBJ=$(BASEPATH)/libgs/gs/*.o $(BASEPATH)/libgs/io/*.o $(BASEPATH)/libgs/db/*.o $(BASEPATH)/libgs/log/*.o 
#$(BASEPATH)/libgs/sk/*.o 
CMLIB=$(BASEPATH)/libcommon.a  $(IOLIB_OBJ) $(BASEPATH)/lua/lib/liblua.a $(BASEPATH)/collision/libTrace.a $(BASEPATH)/pathfinding/libPathfind.a
#$(BASEPATH)/libonline.a
DEF= -DLINUX -D_DEBUG  -D__THREAD_SPIN_LOCK__  -D__TEST_ATTACK_DELAY__NO_
#-D_CHECK_MEM_ALLOC

THREAD = -D_REENTRANT -D_THREAD_SAFE 
THREADLIB = -pthread  
ALLLIB = $(THREADLIB)  /usr/lib/libcrypto.a
CFLAGS  = -Wall -pipe -Wno-char-subscripts 
CPPFLAGS =-Wall -pipe -Wno-char-subscripts  
OPTIMIZE=-O0
CC=gcc  -Wall  $(DEF) $(OPTIMIZE) $(THREAD) $(CFLAGS) -g -ggdb
CPP=g++  -Wall $(DEF) $(OPTIMIZE) $(THREAD) $(CPPFLAGS) -g -ggdb
#-pedantic
LD=g++ -g  
AR=ar crs 
ARX=ar x

#
# include dependency files if they exist
#

ifneq ($(wildcard .depend),)
include .depend
endif

ifeq ($(TERM),cygwin)
THREADLIB = -lpthread
CMLIB += /usr/lib/libgmon.a
DEF += -D__CYGWIN__
endif

dep:
	$(CC) -MM $(INC)  -c *.c* > .depend

ifneq ($(wildcard $(BASEPATH)/MyRules),)
include $(BASEPATH)/MyRules
endif

