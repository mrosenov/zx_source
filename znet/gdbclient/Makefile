
TOP_SRCDIR = ..

SINGLE_THREAD = false
DEBUG_VERSION = false

include ../mk/gcc.defs.mk

TOP_SRCDIR = ..
INCLUDES += -I$(TOP_SRCDIR)/include

DEFINES += -ggdb -O2 #-DUSE_EPOLL -ggdb

OBJS = gamedbclient.o state.o stubs.o dbclient.o db_if.o
LIBOBJS = state.o stubs.o db_if.o gamedbclient.o
CLEAN += *.a

all : lib

lib: $(LIBOBJS)
	rm libdbCli.a -f
	$(AR) crs libdbCli.a $(LIBOBJS)

dbclient : $(SHAREOBJ) $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(SHAREOBJ) $(OBJS)  $(SHARE_SOBJ)

include ../mk/gcc.rules.mk

