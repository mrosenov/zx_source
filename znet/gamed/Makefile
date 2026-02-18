
TOP_SRCDIR = ..

SINGLE_THREAD = false
DEBUG_VERSION = true

include ../mk/gcc.defs.mk

#DEFINES += -D_TESTCODE
#DEFINES += -D_WAIT_TIME 
DEFINES  += #-DUSE_EPOLL 
DEFINES  += -D__USE_SPEC_GAMEDATASEND__ -DUSE_LOGCLIENT -g -ggdb -O2 

INCLUDES += -I./header -I./header/include -I./header/include/common -I ../include
OBJS = gproviderclient.o state.o gamed.o stubs.o gsp_if.o 
CLEAN += *.a

LIBOBJS =  gsp_if.o factionlib.o mailsyslib.o auctionsyslib.o stubs.o state.o gproviderclient.o gmattr.o sellpointlib.o stocklib.o certverify.o

all : lib

share: $(SHAREOBJ)

gamed : $(SHAREOBJ) $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(SHAREOBJ) $(OBJS) $(SHARE_SOBJ)

lib :  $(LIBOBJS) $(SHAREOBJ)
	rm -f libgsPro2.a
	$(AR) crs libgsPro2.a $(LIBOBJS)

include ../mk/gcc.rules.mk
