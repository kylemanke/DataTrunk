# Directories
SRC = ./src
BIN = ./bin
EXCEPT = $(SRC)/exception
FILE = $(SRC)/file
TEST = $(SRC)/test
PARSER = $(SRC)/parser

# Compiler Flags
CC = g++
STD_FLAGS = -std=c++17 -Wall -Wextra -Werror -I$(SRC)
DEBUG_FLAGS = -g
OBJ_FLAGS = -c

test: test_tokenizer.out

test_tokenizer.out: $(BIN)/file/filereader.o $(BIN)/parser/tokenizer.o $(TEST)/test_tokenizer.cpp
	$(CC) $(STD_FLAGS) $(DEBUG_FLAGS) $^ -o $@

$(BIN)/parser/%.o: $(PARSER)/%.cpp $(PARSER)/%.h $(FILE)/filereader.h $(EXCEPT)/token_exception.h
	$(CC) $(STD_FLAGS) $(DEBUG_FLAGS) $(OBJ_FLAGS) $< -o $@

$(BIN)/file/%.o: $(FILE)/%.cpp $(FILE)/%.h $(EXCEPT)/file_exception.h
	$(CC) $(STD_FLAGS) $(DEBUG_FLAGS) $(OBJ_FLAGS) $< -o $@

clean:
	rm -rf $(BIN)/**/*.o *.out


