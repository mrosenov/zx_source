
TOP_SRCDIR = ..

SINGLE_THREAD = false
DEBUG_VERSION = false

include ../mk/gcc.defs.mk

OBJS = logserviceserver.o logservicetcpserver.o state.o stubs.o logservice.o logdispatch.o 

all : logservice

logservice : $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ)

include ../mk/gcc.rules.mk

