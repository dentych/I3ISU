# Quick and dirty (does not handle changes in h-file)
SRCS=$(wildcard src/*.cpp)
OBJS=$(SRCS:.cpp=.o)
BASEPATH=..
# Determine whether this is a host build or not
ifndef TARGET
TARGET=host
endif

ifeq ($(TARGET), host)
CXX=g++
else
CXX=arm-angstrom-linux-gnueabi-g++
endif

# Determine whether this is a debug build or not
ifdef DEBUG
CXXFLAGS=-ggdb -O0
LIBPATH=$(BASEPATH)/lib/$(TARGET)/debug
else
CXXFLAGS=-O2
LIBPATH=$(BASEPATH)/lib/$(TARGET)/release
endif
# Setup the CFLAGS to ensure that the relevant warnings, includes and defines.
CXXFLAGS+=-Wall -D_REENTRANT -DOS_LINUX -I$(BASEPATH)/inc

#%.o : %cpp
#	g++ $(CXXFLAGS) -c -o $@ $^
# Then again, note how the flags are NOT part of the linking process
main: $(OBJS)
	$(CXX) -o main-$(TARGET) $(OBJS) -L$(LIBPATH) -lOSApi -lrt -lpthread

.PHONY: all clean cleanobj
all: main

clean: cleanobj
	@rm -f main-*
	@echo Cleaning executables

cleanobj:
	@rm -f *.o
	@echo Cleaning objects
