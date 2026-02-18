
TOP_SRCDIR = ..

SINGLE_THREAD = false
DEBUG_VERSION = true

include ../mk/gcc.defs.mk

INCLUDES += -I$(TOP_SRCDIR)/storage -I$(TOP_SRCDIR)/include
LDFLAGS  += -static -g -ggdb 
CFLAGS   += -g -ggdb
DEFINES  += -DUSE_WDB -DUSE_LOGCLIENT -DMPPC_4WAY -DUSE_TRANSACTION -D_FILE_OFFSET_BITS=64 -DCOMPILERNAME=\"$(USER)\" #-DCHARSET_UTF8

OBJS = ../storage/storage.o ../storage/storagetool.o uniquenameserver.o state.o stubs.o uniquenamed.o accessdb.o

all : uniquenamed

uniquenamed : $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ) $(LOGOBJ) $(LOGSTUB)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ) $(SHARELIB) $(LOGOBJ) $(LOGSTUB)

include ../mk/gcc.rules.mk

