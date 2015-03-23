cc = gcc
target = helloworld

all: $(target)
$(target): helloworld.o
	$(cc) -o $(target) $^ -l ncurses

helloworld.o: helloworld.c
	$(cc) -c $^

.PHONY: clean
clean:
	rm -f $(target) *.o

.PHONY: help
help:
	@echo Available targets: all, clean, help.
