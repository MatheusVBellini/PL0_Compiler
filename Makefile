# inputs and outputs
COMPILER ?= gcc

SRCS := $(wildcard src/**/*.c) src/main.c
OBJ = $(SRCS:.c=.o)
OUT = lexical_analyser

ZIP = LexicalAnalyser.zip

# compilation flags
FLAGS = -I./src -Wall -Wextra -pedantic -O3 -g

all: $(OUT)

%.o : %.c
	$(COMPILER) $(FLAGS) -c $^ -o $@

$(OUT): $(OBJ)
	$(COMPILER) $^ -o $@ && rm -f $^

.PHONY: all

build: format lint all

clean:
	rm -f $(OUT) $(ZIP) $(OBJ)

zip:
	zip -r $(ZIP) * -x $(OUT)

run:
	./$(OUT)

# dependecies: clang-format
format:
	clang-format -i $(IN)

# dependencies: cppcheck
lint:
	cppcheck $(IN)
