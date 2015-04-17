CC=g++
CCLIB=-l pthread
TARGET=prog
SOURCES=Main.cpp Message.cpp MsgQueue.cpp
SRCDIR=../Exercise1

$(TARGET): $(SOURCES:.cpp=.o)
	$(CC) -o $@ $^ $(CCLIB)

Main.o: Main.cpp
	$(CC) -c -o $@ $^

%.o: $(SRCDIR)/%.cpp
	$(CC) -c -o $@ $^

.PHONY: clean run
clean:
	rm -f *.o $(TARGET)

run:
	./$(TARGET)
