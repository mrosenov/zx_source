
TOP_SRCDIR = ..

SINGLE_THREAD = true
DEBUG_VERSION =	false

include ../mk/gcc.defs.mk
LOG_DIR = ../logclient
LOG2_DIR = $(TOP_SRCDIR)/log_client
INCLUDES += -I$(TOP_SRCDIR)/logclient -I$(TOP_SRCDIR)/log_inl -I$(TOP_SRCDIR)/include -I$(LOG2_DIR)
DEFINES += -DUSE_HASH_MAP -DUSE_EPOLL -DUSE_LOGCLIENT -O3 -DCOMPILERNAME=\"$(USER)\" #-O3 -ipo# -pg # -DUSE_SELECT -ggdb 
LDFLAGS += -DUSE_HASH_MAP #-static #-O3 #  -g -ggdb #-O3 -ipo# -pg

LOG2OBJ = $(LOG2_DIR)/state.o $(LOG2_DIR)/stubs.o $(LOG2_DIR)/logger.o $(LOG2_DIR)/log2tcpclient.o $(LOG2_DIR)/log2udpclient.o
OBJS = glinkserver.o gdeliveryclient.o gproviderserver.o state.o stubs.o glinkd.o # $(LOG2OBJ)

all : glinkd

glinkd : $(SHAREOBJ) $(OBJS) $(SHARE_SOBJ) $(LOGSTUB) $(LOGOBJ)
	$(LD) $(LDFLAGS) -o $@ $(SHAREOBJ) $(OBJS) $(SHARE_SOBJ) $(LOGSTUB) $(LOGOBJ)

include ../mk/gcc.rules.mk

