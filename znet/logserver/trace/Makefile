TOP_SRCDIR = ../..

SINGLE_THREAD = false
DEBUG_VERSION = true


include ../../mk/gcc.defs.mk

LDFLAGS += -DUSE_HASH_MAP -DUSE_EPOLL -D_FILE_OFFSET_BITS=64

LOGSYSDIR = ..
DEFINES += -I$(LOGSYSDIR) -I/usr/include/libxml2 

INL = -lxml2
LOGSYS_COMMON_DIR = $(LOGSYSDIR)/logcommon
LOG_COMM = $(LOGSYS_COMMON_DIR)/util.o  $(LOGSYS_COMMON_DIR)/xmldb.o \
                $(LOGSYS_COMMON_DIR)/xmlconfigdb.o $(LOGSYS_COMMON_DIR)/xmlobject.o \
                $(LOGSYS_COMMON_DIR)/appconfig2.o

OBJS = $(LOGSYSDIR)/state.o $(LOGSYSDIR)/stubs.o trace.o

all : _trace

_trace : myclean trace

trace : $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ) $(LOG_COMM) $(IDXOBJ)
	$(LD) $(LDFLAGS) $(INL) -o $@ $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ) $(LOG_COMM) $(IDXOBJ)

.PHONY: myclean myall
myall:
	rm -rf *.o; rm -rf ; rm -rf ../logcommon/*.o
include ../../mk/gcc.rules.mk

