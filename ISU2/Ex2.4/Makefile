folder = host
targets = $(folder)/parts
CC = g++
Source = main.cpp part1.cpp part2.cpp
Object = $(addprefix $(folder)/,$(Source:.cpp=.o))
.PHONY: all
all: $(targets)

$(folder)/%.o: %.cpp
	$(CC) -c -o $@ $^

$(targets):$(folder) $(Object)
	$(CC) -o $@ $(Object)

$(folder):
	mkdir $(folder)

.PHONY: clean
clean:
	rm -rf $(folder)

.PHONY: help
help:
	@echo Targets er: all, clean og help.
