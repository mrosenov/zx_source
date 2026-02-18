
TOP_SRCDIR = ..

SINGLE_THREAD = true
DEBUG_VERSION = true

include ../mk/gcc.defs.mk

LOG_DIR = ../logclient
LOG2_DIR = $(TOP_SRCDIR)/log_client
INCLUDES += -I$(TOP_SRCDIR)/logclient -I$(TOP_SRCDIR)/log_inl -I$(TOP_SRCDIR)/include -I$(LOG2_DIR)
DEFINES += -DUSE_HASH_MAP -DUSE_EPOLL -DUSE_LOGCLIENT -O0 -g -ggdb -DCOMPILERNAME=\"$(USER)\" #-DDEBUG_RAID_ROBOT #-DDEBUG_RAID #-DUSE_LOG2
LDFLAGS += -DUSE_HASH_MAP -O0 -g -ggdb #-static 

LOG2OBJ = $(LOG2_DIR)/state.o $(LOG2_DIR)/stubs.o $(LOG2_DIR)/logger.o $(LOG2_DIR)/log2tcpclient.o $(LOG2_DIR)/log2udpclient.o
OUTEROBJS = $(CO_DIR)/matcher.o trade.o mailbox.o postoffice.o auctionmarket.o serverattr.o 
CLEAN += $(OUTEROBJS)

OBJS = map.o gdeliveryserver.o gauthclient.o gproviderserver.o ganticheatclient.o uniquenameclient.o state.o stubs.o \
	   gdeliveryd.o gamedbclient.o gmrestartserver.o groledbclient.o chatroom.o battlemanager.o domaindataman.o \
	   playerlogin.o factionlistener.o factionmanager.o topmanager.o stockexchange.o combatmanager.o gamemaster.o\
	   maptaskdata.o familymanager.o delhelper.o exparray.o domaindaemon.o siegemanager.o sectmanager.o  \
	   contestmanager.o questionlib.o instancingmanager.o certverify.o commonvalue.o commondata.o commondataauto.o \
	   snsplayerinfomanager.o snsmessagemanager.o billingagent.o referencemanager.o refspreadcode.o \
	   fungamemanager.o fungamebase.o landlordgame.o farmcompomanager.o hometownstateobj.o hometowntemplate.o \
	   substance.o hometownmanager.o hometownitem.o hometownitembody.o hometownitemfunc.o hometownactionpolicy.o \
	   netmarble.o territorymanager.o uniqueauctionmanager.o circlemanager.o circleusage.o consignmanager.o \
	   gwebtradeclient.o centraldeliveryclient.o centraldeliveryserver.o friendcallbackmanager.o gtplatformagent.o \
	   gamegateserver.o raidmanager.o vipinfocache.o blockedrole.o globaldropmanager.o namemanager.o \
	   crossbattlemanager.o kingdommanager.o testcasegen.o topflowermanager.o openbanquetmanager.o facbasemanager.o crssvrteamsinfo.o crssvrteamsbasemanager.o crssvrteamsmanager.o \
	   crosscrssvrteamsmanager.o tsplatformagent.o

all : gdeliveryd

gdeliveryd : $(SHAREOBJ) $(OBJS) $(OUTEROBJS) $(SHARE_SOBJ) $(LOGSTUB) $(LOGOBJ)
	$(LD) $(LDFLAGS) -o $@ $(OUTEROBJS) $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ) $(LOGSTUB) $(LOGOBJ) -lpcre -lssl -lcrypto

refspreadcode2 : ../common/octets.o refspreadcode2.o 
	$(LD) -o $@ refspreadcode2.o ../common/octets.o 

operationcmdgen : operationcmdgen.o ../common/octets.o $(SHARE_SOBJ)
	$(LD) -o $@ operationcmdgen.o ../common/octets.o $(SHARE_SOBJ)

include ../mk/gcc.rules.mk
