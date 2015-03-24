CC=g++
CCLIB=-l pthread
TARGET=prog
SOURCES=Main.cpp Message.cpp MsgQueue.cpp

$(TARGET): $(SOURCES:.cpp=.o)
	$(CC) -o $@ $^ $(CCLIB)

%.o: %.cpp
	$(CC) -c -o $@ $^

.PHONY: clean
clean:
	rm -f *.o $(TARGET)
