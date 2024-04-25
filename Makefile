# inputs and outputs
COMPILER ?= gcc

IN = src/main.c 
OUT = lexical_analyser

ZIP = LexicalAnalyser.zip

# compilation flags
FLAGS = -Wall -Wextra -pedantic -O3

all: 
	$(COMPILER) $(FLAGS) -o $(OUT) $(IN)

build: format lint all

clean:
	rm -f $(OUT) $(ZIP) saida.txt && touch saida.txt

zip:
	zip -r $(ZIP) * -x $(OUT)

# dependecies: clang-format
format:
	clang-format -i $(IN)

# dependencies: cppcheck
lint:
	cppcheck $(IN) 
