#ifndef DT_SRC_PARSER_TOKENIZER_H
#define DT_SRC_PARSER_TOKENIZER_H

// C++ includes
#include <cstdint>

enum class CharType : uint8_t;
enum class TokenType : uint8_t;

/**
 * @brief Responsible for scanning tokens from either a file or SQL string
 */
class Tokenizer {
    Tokenizer();
};

#endif