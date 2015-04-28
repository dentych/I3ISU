CC=g++
SOURCE=$(wildcard *.cpp)
OBJECT=$(SOURCE:.cpp=.o)
TARGET=prog
CXXFLAGS=--std=c++11

.PHONY: all clean
all: $(TARGET)
$(TARGET): $(OBJECT)
	$(CC) -o $(TARGET) $^

clean:
	rm -f $(TARGET) $(OBJECT)

run:
	./$(TARGET)
