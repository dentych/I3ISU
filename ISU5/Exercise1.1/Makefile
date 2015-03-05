OBJECTS = parkalot2000.o
TARGET = prog_parkalot2000
FLAGS = -l pthread

.PHONY: all, clean, run

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) -o $(TARGET) $^ $(FLAGS)

clean:
	@rm *.o $(TARGET)
	@echo CLEAN *.o $(TARGET)

run: $(TARGET)
	@./$(TARGET)
