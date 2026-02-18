
TOP_SRCDIR = ..
SINGLE_THREAD = true
DEBUG_VERSION = true

include ../mk/gcc.defs.mk

DEFINES += -ggdb -D_TESTCODE -DUSE_EPOLL -DUSE_HASH_MAP
LDFLAGS	+= -ggdb -DUSE_HASH_MAP -lpthread

#OUTEROBJS = ../io/pollio.o ../io/protocol.o ../io/security.o ../io/rpc.o ../common/thread.o ../common/conf.o ../common/timer.o ../io/proxyrpc.o ../common/itimer.o ../common/octets.o gdprocessor.o
OBJS = glinkclient.o state.o stubs.o debug_gs_cmd.o test_raid_enter.o

EXES = gclient

all : $(EXES)

windforce : windforce.o
	$(LD) $(LDFLAGS) -o $@ windforce.o
	
loginner_windforce : loginner_windforce.o
	$(LD) $(LDFLAGS) -o $@ loginner_windforce.o

loginner_gclient : $(SHAREOBJ) $(OBJS) loginner_main.o
	$(LD) $(LDFLAGS) -o $@ $(OBJS) loginner_main.o $(SHAREOBJ) $(SHARE_SOBJ)

$(EXES) : $(SHAREOBJ) $(OBJS) main.o
	$(LD) $(LDFLAGS) -o $@ $(OBJS) main.o $(SHAREOBJ) $(SHARE_SOBJ)

include ../mk/gcc.rules.mk

