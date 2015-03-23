target = parts
cc = g++

.PHONY: all
all: $(target)
$(target): main.o part1.o part2.o
   $(cc) -o $(target) $^

main.o: main.cpp part1.h part2.h
	$(cc) -c main.cpp

part1.o: part1.cpp part1.h
	$(cc) -c part1.cpp

part2.o: part2.cpp part2.h
	$(cc) -c part2.cpp

.PHONY: clean
clean:
	rm -f $(target) *.o

.PHONY: help
help:
	@echo Make targets: all, clean, help.
