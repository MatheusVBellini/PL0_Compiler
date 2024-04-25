# inputs and outputs
COMPILER ?= gcc

IN = $(wildcard src/*.c)
OBJ = $(IN:.c=.o)
OUT = lexical_analyser

ZIP = LexicalAnalyser.zip

# compilation flags
FLAGS = -I./src -Wall -Wextra -pedantic -O3

all: $(OUT) 

$(OUT): $(OBJ)
	$(COMPILER) $(OBJ) -o $@ && rm -r $(OBJ)

src/%.o : src/%.c
	$(COMPILER) $(FLAGS) -c $^ -o $@

build: format lint all

clean:
	rm -f $(OUT) $(ZIP)

zip:
	zip -r $(ZIP) * -x $(OUT)

# dependecies: clang-format
format:
	clang-format -i $(IN)

# dependencies: cppcheck
lint:
	cppcheck $(IN) 
