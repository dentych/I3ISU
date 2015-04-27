#Makefile test of wildcard
INCDIR=inc
SRCDIR=src
EX1DIR=../Exercise1
SOURCE=$(wildcard $(SRCDIR)/*.cpp) $(EX1DIR)/Message.cpp $(EX1DIR)/MsgQueue.cpp
OBJECT=$(SOURCE:.cpp=.o)
DEPS=$(SOURCE:.cpp=.d)
TARGET=prog
FLAGS=-lpthread
CXX=g++

.PHONY: all help clean
all: $(TARGET)
$(TARGET): $(OBJECT) $(DEPS)
	$(CXX) -o $(TARGET) $(OBJECT) $(FLAGS)
	
%.o: %.cpp
	$(CXX) -c -o $@ $(filter %.cpp, $^)

# Dependency stuff
%.d: %.cpp
	$(CXX) -MM -MT$(@:.d=.o) $^ > $@

help:
	@echo "Objects: $(OBJECT)"
	@echo "Source: $(SOURCE)"
	@echo "Deps: $(DEPS)"

clean:
	rm -rf $(TARGET) $(DEPS) $(OBJECT)

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif
