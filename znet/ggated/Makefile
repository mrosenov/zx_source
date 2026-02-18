
TOP_SRCDIR = ..

SINGLE_THREAD = true
DEBUG_VERSION = true

include ../mk/gcc.defs.mk
LOG_DIR = ../logclient
LOG2_DIR = $(TOP_SRCDIR)/log_client
INCLUDES += -I$(TOP_SRCDIR)/logclient -I$(TOP_SRCDIR)/log_inl -I$(TOP_SRCDIR)/include -I$(LOG2_DIR)
	DEFINES += -DUSE_HASH_MAP -DUSE_EPOLL -DUSE_LOGCLIENT -O0 -g -ggdb #-DUSE_LOG2
	LDFLAGS += -DUSE_HASH_MAP -static -O0 -g -ggdb

	LOG2OBJ = $(LOG2_DIR)/state.o $(LOG2_DIR)/stubs.o $(LOG2_DIR)/logger.o $(LOG2_DIR)/log2tcpclient.o $(LOG2_DIR)/log2udpclient.o
OBJS = gtplatformclient.o gamegateclient.o gamedbclient.o gtmanager.o state.o stubs.o ggated.o factionqueryagent.o userqueryagent2.o gaterolecache.o gtcommon.o simplequery.o kdsclient.o tokengenerator.o \
       gproviderserver.o snsplatformclient.o

all : ggated
ggated : $(SHAREOBJ) $(OBJS) $(OUTEROBJS) $(SHARE_SOBJ) $(LOGSTUB) $(LOGOBJ)
	        $(LD) $(LDFLAGS) -o $@ $(OUTEROBJS) $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ) $(LOGSTUB) $(LOGOBJ) -lcrypto

#ggated : $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ)
#	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ)

include ../mk/gcc.rules.mk

