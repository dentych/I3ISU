cc = g++
targets = hello

.PHONY: all
all: $(targets)
hello: hello.o
	$(cc) hello.o -o hello

hello.o: hello.cpp
	$(cc) -c hello.cpp

.PHONY: clean
clean:
	rm -f $(targets) *.o

.PHONY: help
help:
	@echo \'make\' to build executable. \'make clean\' to remove built files. \'make help\' for a list of options.
