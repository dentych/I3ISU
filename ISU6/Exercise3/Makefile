#Makefile test of wildcard
INCDIR=inc
SRCDIR=src
SOURCE=$(wildcard $(SRCDIR)/*.cpp)
OBJECT=$(SOURCE:.cpp=.o)
DEPS=$(SOURCE:.cpp=.d)
TARGET=prog
FLAGS=-lpthread
CXX=g++

.PHONY: all help clean
all: $(TARGET)
$(TARGET): $(OBJECT)
	$(CXX) -o $(TARGET) $(OBJECT)
	
%.o: %.cpp
	$(CXX) -c -o $@ $(filter %.cpp, $^) $(FLAGS)

help:
	@echo "Objects: $(OBJECT)"
	@echo "Source: $(SOURCE)"
	@echo "Deps: $(DEPS)"

clean:
	rm -rf $(TARGET) $(OBJECT) $(DEPS)

# Dependency stuff
%.d: %.cpp
	$(CXX) -MM -MT$(@:.d=.o) $^ > $@

-include $(DEPS)
