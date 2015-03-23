target = parts
cc = g++

sources = main.cpp part1.cpp part2.cpp
objects = $(sources : .cpp = .o)

.PHONY: all
all: $(target)
$(target): $(objects)
	$(cc) -o $(target) $^

%.o: %.cpp
	$(cc) -c -o $@ $^

.PHONY: clean
clean:
	rm -f $(target) *.o

.PHONY: help
help:
	@echo Make targets: all, clean, help.
