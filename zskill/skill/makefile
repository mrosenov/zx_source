CPP = g++

DEFINES  = -D_SKILL_SERVER 
CPPFLAGS = -Wall -g -ggdb -O0 -fPIC -shared

HEADERS = *.h
CPPFILES = playerwrapper.cpp  skill.cpp  skillfilter.cpp  skillwrapper.cpp skillfunction.cpp #base_filter.cpp 
OBJS = skill.o skillwrapper.o playerwrapper.o skillfilter.o skillfunction.o #base_filter.o 
INCLUDES = -I. -I../header -I../header/include -I../common
STUBOBJS := $(patsubst %.cpp,%.o,$(wildcard ../skills/stubs*.cpp))
#STUBOBJS += $(patsubst %.cpp,%.o,$(wildcard ))
LIB = libskill.so

all : $(LIB)

$(LIB) : $(OBJS) $(STUBOBJS)
	$(CPP) -fPIC --shared -o $(LIB) $(OBJS) $(STUBOBJS)

$(OBJS) : $(HEADERS)

.cpp.o:
	$(CPP) -c $(DEFINES) $(INCLUDES) $(CPPFLAGS) $< -o $@

clean:
	rm -rf $(OBJS) $(STUBOBJS) $(LIB) $(CLEAN) 

