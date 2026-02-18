
TOP_SRCDIR = ..

SINGLE_THREAD = false
DEBUG_VERSION = true 

LOGSYSDIR = .
LOG_CLIENT = ../log_client

include ../mk/gcc.defs.mk

CLEAN += *.o


LDFLAGS += -DUSE_HASH_MAP -DUSE_EPOLL -D_FILE_OFFSET_BITS=64

DEFINES += -I/usr/include/libxml2  -I$(LOGSYSDIR) -I$(LOG_CLIENT)


LOGSYS_COMMON_DIR = $(LOGSYSDIR)/logcommon
LOG_COMM = $(LOGSYS_COMMON_DIR)/util.o  $(LOGSYS_COMMON_DIR)/xmldb.o \
		$(LOGSYS_COMMON_DIR)/xmlconfigdb.o $(LOGSYS_COMMON_DIR)/xmlobject.o \
		$(LOGSYS_COMMON_DIR)/appconfig2.o

OBJS = logtcpserver.o logudpserver.o logserver.o state.o logrecord.o 

OBJS1 = logrotate.o  rotatetimer.o $(LOG_CLIENT)/libLogClient.a

OBJS2 = logrotate2.o $(LOG_CLIENT)/libLogClient.a

LOGSYS_COMMON_DIR = $(LOGSYSDIR)/logcommon
LOG_COMM = $(LOGSYS_COMMON_DIR)/util.o  $(LOGSYS_COMMON_DIR)/xmldb.o \
                $(LOGSYS_COMMON_DIR)/xmlconfigdb.o $(LOGSYS_COMMON_DIR)/xmlobject.o \
                $(LOGSYS_COMMON_DIR)/appconfig2.o
INL = -lxml2

all : _all

_all: clean myall configure logserver logrotate logrotate2

logserver : $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ) $(LOG_COMM)
	$(LD) $(LDFLAGS) $(INL) -ggdb -o $@ $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ) $(LOG_COMM)

logrotate : $(LOG_COMM) $(OBJS1) $(SHAREOBJ) $(SHARE_SOBJ)
	$(LD) $(LDFLAGS) $(INL) -ggdb $(LOG_COMM) $(OBJS1) $(SHAREOBJ) $(SHARE_SOBJ)  -o logrotate

logrotate2 : $(LOG_COMM) $(OBJS2) $(SHAREOBJ) $(SHARE_SOBJ)
	$(LD) $(LDFLAGS) $(INL) -ggdb $(LOG_COMM) $(OBJS2) $(SHAREOBJ) $(SHARE_SOBJ)  -o logrotate2

.PHONY: myclean myall configure;
configure: 
	@./configure
myclean:
	rm -rf *.o;
myall:
	rm -rf *.o; rm -rf $(LOGSYS_COMMON_DIR)/*.o;
include ../mk/gcc.rules.mk
