# inputs and outputs
COMPILER ?= gcc

IN = main.c 
OUT = lexical_analyser

ZIP = LexicalAnalyser.zip

# compilation flags
FLAGS = -Wall -Wextra -pedantic -O3

all: 
	$(COMPILER) $(FLAGS) -o $(OUT) $(IN)

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
