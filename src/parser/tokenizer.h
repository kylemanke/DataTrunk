#ifndef DT_SRC_PARSER_TOKENIZER_H
#define DT_SRC_PARSER_TOKENIZER_H

// C++ includes
#include <cstdint>
#include <string>

// Local Includes
#include <file/filereader.h>

enum class TokenState : uint16_t;

enum class TokenType : uint8_t {
    // DATA TYPES
    kSTRUCT,
    kBYTE,
    kSHORT,
    kINT,
    kLONG,
    kFLOAT,
    kDOUBLE,
    kSTRING,
    // BOOLEANS
    kBOOL,
    kTRUE,
    kFALSE,
    // MODIFIERS
    kCONST,
    kSIGNED,
    kUNSIGNED,
    kNOTNULL,
    kPRIMARY,
    kSECONDARY,
    // CREATION/DELETION Keywords
    kNEW,
    kDELETE,
    kCOMMENT,
    // LOOPS,
    kFOR,
    kWHILE,
    // FUNCTIONS
    kRETURN,
    // NAMES
    kIDENTIFIER,
    // NUMBERS
    kINTEGER,
    kREAL_NUMBER,
    // STRINGS
    kSTRING_LITERAL,
    // BOOLEAN OPERATORS
    kBOOL_AND,
    kBOOL_OR,
    kLT,
    kLTE,
    kEQUALITY,
    kDNE,
    kNOT,
    kGT,
    kGTE,
    // ARITHMETIC_OPERATORS
    kPLUS,
    kMINUS,
    kASTERISK,
    kDIVIDE,
    kMODULO,
    // ASSIGNMENT_OPERATORS
    kASSIGN,
    kADD_EQU,
    kSUB_EQU,
    kMULT_EQU,
    kDIV_EQU,
    kMOD_EQU,
    // SPECIAL CHARACTERS
    kLBRACK,
    kRBRACK,
    kLPAREN,
    kRPAREN,
    kLBRACE,
    kRBRACE,
    kCOMMA,
    kTILDE,
    kDOT,
    kCOLON,
    kSCOLON,
    // Function
    kEOF,
    kBEGIN,
    // ERRORS
    kERROR
};


/**
 * @brief Responsible for scanning tokens from either a file or SQL string
 */
class Tokenizer {
public:
    /**
     * TORS
     */
    Tokenizer() noexcept;
    ~Tokenizer() noexcept;

    /**
     * SETUP
     */
    void OpenString(const std::string& trunk_str);
    void OpenString(const char* trunk_str);
    void OpenFile(const std::string& file_name);
    void OpenFile(const char* file_name);

    /**
     * TOKENIZATION 
     */
    TokenType CurrToken() const noexcept;
    TokenType NextToken();
    std::string CurrTokenValue() const noexcept;

    /**
     * DEBUG 
     */
    std::string GetDebugStr() const noexcept;

private:
    /**
     * HELPERS 
     */
    uint8_t NextChar();
    uint8_t PeekChar();

    TokenState ScanIdentifier();
    TokenState ScanKeyword();
    TokenState ScanNumber();
    TokenState ScanString();

    /**
     * STATE INFO 
     */
    uint32_t str_i_;
    std::string str_;
    FileReader fr_;

    /**
     * CURR TOKEN INFO 
     */
    TokenType curr_token_;
    std::string curr_token_val_;
};

#endif