
TOP_SRCDIR = ..

SINGLE_THREAD = true
DEBUG_VERSION = false

include ../mk/gcc.defs.mk

DEFINES += -DUSE_LOGCLIENT
CLEAN += glog.o liblogCli.a


OBJS = logclientclient.o logclienttcpclient.o state.o stubs.o logclient.o log.o

LIBOBJS = logclientclient.o logclienttcpclient.o state.o stubs.o glog.o log.o

all : logclient

logclient : $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ) 

lib : $(LIBOBJS)
	ar crs liblogCli.a $(LIBOBJS)

include ../mk/gcc.rules.mk

