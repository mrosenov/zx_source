TOP_SRCDIR = ../..

SINGLE_THREAD = false
DEBUG_VERSION = false 

LOGSYSDIR = ..

include ../../mk/gcc.defs.mk


LDFLAGS += -DUSE_HASH_MAP -DUSE_EPOLL -D_FILE_OFFSET_BITS=64 

DEFINES += -I/usr/include/libxml2  -I$(LOGSYSDIR) 

LOGSYS_COMMON_DIR = $(LOGSYSDIR)/logcommon
LOG_COMM = \
		$(LOGSYS_COMMON_DIR)/util.o  $(LOGSYS_COMMON_DIR)/xmldb.o \
                $(LOGSYS_COMMON_DIR)/xmlconfigdb.o $(LOGSYS_COMMON_DIR)/xmlobject.o \
                $(LOGSYS_COMMON_DIR)/appconfig2.o

IDXPARSER_DIR = $(LOGSYSDIR)/logcommon
IDXOBJ = \
		$(IDXPARSER_DIR)/indexmaker.o $(IDXPARSER_DIR)/indexdb.o \
		$(IDXPARSER_DIR)/indexquery.o

INL = -lxml2

OBJ2 = demoindexmaker.o 
OBJ3 = demoindexquery.o 


EXEC = demoindexmaker demoindexquery 

all : $(EXEC)

demoindexmaker : $(OBJ2) $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ) $(LOG_COMM) $(IDXOBJ)
	$(LD) $(LDFLAGS) $(INL) -ggdb -o $@ $(OBJ2) $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ) $(LOG_COMM) $(IDXOBJ)
demoindexquery : $(OBJ3) $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ) $(LOG_COMM) $(IDXOBJ)
	$(LD) $(LDFLAGS) $(INL) -ggdb -o $@ $(OBJ3) $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ) $(LOG_COMM) $(IDXOBJ)

.PHONY: myclean myall
myclean:
	rm -rf *.o; rm -rf ../logcommon/*.o
include ../../mk/gcc.rules.mk

