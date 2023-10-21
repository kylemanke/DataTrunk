// C++ Includes
#include <iostream>
#include <cstdint>

// Local Includes
#include "parser/tokenizer.h"


int main(int argc, char* argv[]) {
    // Create the tokenizer
    if (argc != 2)
        return -1;

    Tokenizer token;
    token.OpenFile(argv[1]);

    while(token.NextToken() != TokenType::kEOF) {
        std::cout << token.GetDebugStr() << std::endl;
    }

    return 1;
}