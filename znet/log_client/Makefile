
TOP_SRCDIR = ..

SINGLE_THREAD = false
DEBUG_VERSION = true

LOGSYS = ..

include ../mk/gcc.defs.mk

OBJS = logger.o log2tcpclient.o log2udpclient.o state.o 

DEFINES += -I$(LOGSYS)

all :lib
lib:  $(OBJS) 
	$(AR) crs ./libLogClient.a  $(OBJS) 
include ../mk/gcc.rules.mk

