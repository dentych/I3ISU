CC=g++
SOURCE=$(wildcard *.cpp)
OBJECT=$(SOURCE:.cpp=.o)
TARGET=prog

.PHONY: all clean
all: $(TARGET)
$(TARGET): $(OBJECT)
	$(CC) -o $(TARGET) $^

clean:
	rm -f $(TARGET) $(OBJECT)

run:
	./$(TARGET)
