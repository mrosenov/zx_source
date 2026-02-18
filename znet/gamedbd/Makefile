
TOP_SRCDIR = ..

SINGLE_THREAD = false
DEBUG_VERSION = true

include ../mk/gcc.defs.mk

LOG2_DIR = $(TOP_SRCDIR)/log_client
INCLUDES += -I$(TOP_SRCDIR)/storage -I$(TOP_SRCDIR)/include -I$(LOG2_DIR)
CFLAGS += -g -ggdb #-O0 -O2 #-static
#LDFLAGS += -static 
DEFINES += -DUSE_DB -DUSE_WDB -DUSE_LOGCLIENT -DMPPC_4WAY -DUSE_TRANSACTION -D_FILE_OFFSET_BITS=64 -DCOMPILERNAME=\"$(USER)\"

LOG2SRC = $(LOG2_DIR)/logger.cpp $(LOG2_DIR)/log2tcpclient.cpp $(LOG2_DIR)/log2udpclient.cpp
LOGSRC  += $(LOG2SRC)
LOG2STUBSRC = $(LOG2_DIR)/state.cxx $(LOG2_DIR)/stubs.cxx 
OBJS = ../storage/storage.o ../storage/storagetool.o gamedbserver.o state.o stubs.o gamedbd.o gamedbmanager.o accessdb.o mergedb.o verifydb.o utilfunction.o gamedbclient.o familycache.o domaincmd.o forbidpolicy.o commonvalue.o circlepool.o consigntool.o equipscan.o abstractplayers.o sysmailcontainer.o dbconsignsoldrole.o itemowner.o acforbidrule.o

ifeq ($(SINGLE_THREAD),true)
	LOG2OBJ := $(LOG2SRC:%.cpp=%.o)
	LOG2STUB := $(LOG2STUBSRC:%.cxx=%.o)
else
	LOG2OBJ := $(LOG2SRC:%.cpp=%_m.o)
	LOG2STUB := $(LOG2STUBSRC:%.cxx=%_m.o)
endif

all : gamedbd.wdb

gamedbd.wdb : $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ) $(LOGOBJ) $(LOGSTUB) $(LOG2OBJ) $(LOG2STUB)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ) $(LOGOBJ) $(LOGSTUB) $(LOG2OBJ) $(LOG2STUB) -lcrypto -ldl

toplist : $(SHAREOBJ) $(SHARE_SOBJ) ../storage/storage.o toplist.o $(LOGOBJ) $(LOGSTUB)
	$(LD) $(LDFLAGS) -o $@ toplist.o $(SHAREOBJ) $(SHARE_SOBJ) $(LOGOBJ) $(LOGSTUB) ../storage/storage.o

repairrolename : $(SHAREOBJ) $(SHARE_SOBJ) ../storage/storage.o repairrolename.o $(LOGOBJ) $(LOGSTUB)
	$(LD) $(LDFLAGS) -o $@ repairrolename.o $(SHAREOBJ) $(SHARE_SOBJ) $(LOGOBJ) $(LOGSTUB) ../storage/storage.o

repairuserid : $(SHAREOBJ) $(SHARE_SOBJ) ../storage/storage.o repairuserid.o $(LOGOBJ) $(LOGSTUB)
	$(LD) $(LDFLAGS) -o $@ repairuserid.o $(SHAREOBJ) $(SHARE_SOBJ) $(LOGOBJ) $(LOGSTUB) ../storage/storage.o

#cashstat: $(SHAREOBJ) $(SHARE_SOBJ) ../storage/storage.o statistic.cpp
#	$(LD) $(LDFLAGS) -D_REENTRANT_ -O2 -o $@ statistic.cpp $(SHAREOBJ) $(SHARE_SOBJ) ../storage/storage.o -I$(INCLUDES) -DUSE_WDB
cashstat: $(SHAREOBJ) $(SHARE_SOBJ) ../storage/storage.o statistic.o $(LOGOBJ) $(LOGSTUB)
	$(LD) $(LDFLAGS) -o $@ statistic.o $(SHAREOBJ) $(SHARE_SOBJ) $(LOGOBJ) $(LOGSTUB) ../storage/storage.o

#refstat: $(SHAREOBJ) $(SHARE_SOBJ) ../storage/storage.o refstat.cpp
#	$(LD) $(LDFLAGS) -D_REENTRANT_ -O2 -o $@ refstat.cpp $(SHAREOBJ) $(SHARE_SOBJ) ../storage/storage.o -I$(INCLUDES) -DUSE_WDB
refstat: $(SHAREOBJ) $(SHARE_SOBJ) ../storage/storage.o refstat.o $(LOGOBJ) $(LOGSTUB)
	$(LD) $(LDFLAGS) -o $@ refstat.o $(SHAREOBJ) $(SHARE_SOBJ) $(LOGOBJ) $(LOGSTUB) ../storage/storage.o -I$(INCLUDES) -DUSE_WDB

gtimport: $(SHAREOBJ) $(SHARE_SOBJ) ../storage/storage.o ../storage/storagetool.o gtimport.o $(LOGOBJ) $(LOGSTUB) 
	$(LD) $(LDFLAGS) -O2 -o $@ gtimport.o $(SHAREOBJ) $(SHARE_SOBJ) $(LOGOBJ) $(LOGSTUB) ../storage/storage.o ../storage/storagetool.o #-I$(INCLUDES)

recoverdb : $(SHAREOBJ) $(SHARE_SOBJ) ../storage/storage.o recoverdb.o $(LOGOBJ) $(LOGSTUB)
	$(LD) $(LDFLAGS) -o $@ recoverdb.o $(SHAREOBJ) $(SHARE_SOBJ) $(LOGOBJ) $(LOGSTUB) ../storage/storage.o

checkabnormalrole: $(SHAREOBJ) $(SHARE_SOBJ) ../storage/storage.o checkabnormalrole.o $(LOGOBJ) $(LOGSTUB)
	$(LD) $(LDFLAGS) -o $@ checkabnormalrole.o $(SHAREOBJ) $(SHARE_SOBJ) $(LOGOBJ) $(LOGSTUB) ../storage/storage.o

ifeq ($(SINGLE_THREAD),true)
$(LOG2OBJ): %.o: %.cpp
	$(CC) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $< -o $@
$(LOG2STUB): %.o: %.cxx
	$(CC) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $< -o $@
else
$(LOG2OBJ): %_m.o: %.cpp
	$(CC) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $< -o $@
$(LOG2STUB): %_m.o: %.cxx
	$(CC) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $< -o $@
endif

include ../mk/gcc.rules.mk

