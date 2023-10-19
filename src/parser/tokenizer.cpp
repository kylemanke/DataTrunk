// Local Includes
#include "tokenizer.h"

// Necessary Enums
enum class CharType : uint8_t {
    kALPHA,     // Alphabet
    kDIGIT,     // Digit
    kNEWLINE,   // '\n'
    kWS,        // White Space
    kBANG,      // !
    kDQUOTE,    // "
    kPOUND,     // #
    kDOLLAR,    // $
    kPERCENT,   // %
    kAND,       // &
    kSQUOTE,    // '
    kLP,        // (
    kRP,        // )
    kAST,       // *
    kPLUS,      // +
    kCOMMA,     // ,
    kDASH,      // -
    kDOT,       // .
    kFSLASH,    // /
    kCOLON,     // :
    kSCOLON,    // ;
    kLT,        // <
    kEQ,        // =
    kGT,        // >
    kQUEST,     // ?
    kAT,        // @
    kLBRACK,    // [
    kBSLASH,    // BackSlash
    kRBRACK,    // ]
    kCARET,     // ^
    kUNDER,      // _
    kGRAVE,     // `
    kLBRACE,    // {
    kPIPE,      // |
    kRBRACE,    // }
    kTILDE,     // ~
    kILLEGAL    // Not valid characters
};

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
    // ERRORS
    kERROR
};

// Define some maps for faster use
// Map for char type
static const CharType mCharType[] = {
/*         x0                  x1                  x2                  x3                  x4                  x5                  x6                  x7
           x8                  x9                  xa                  xb                  xc                  xd                  xe                  xf */
/* 0x */   CharType::kILLEGAL, CharType::kILLEGAL, CharType::kILLEGAL, CharType::kILLEGAL, CharType::kILLEGAL, CharType::kILLEGAL, CharType::kILLEGAL, CharType::kILLEGAL,
           CharType::kILLEGAL, CharType::kWS,      CharType::kNEWLINE, CharType::kWS,      CharType::kWS,      CharType::kWS,      CharType::kILLEGAL, CharType::kILLEGAL,
/* 1x */   CharType::kILLEGAL, CharType::kILLEGAL, CharType::kILLEGAL, CharType::kILLEGAL, CharType::kILLEGAL, CharType::kILLEGAL, CharType::kILLEGAL, CharType::kILLEGAL,
           CharType::kILLEGAL, CharType::kILLEGAL, CharType::kILLEGAL, CharType::kILLEGAL, CharType::kILLEGAL, CharType::kILLEGAL, CharType::kILLEGAL, CharType::kILLEGAL,
/* 2x */   CharType::kWS,      CharType::kBANG,    CharType::kDQUOTE,  CharType::kPOUND,   CharType::kDOLLAR,  CharType::kPERCENT, CharType::kAND,     CharType::kSQUOTE,
           CharType::kLP,      CharType::kRP,      CharType::kAST,     CharType::kPLUS,    CharType::kCOMMA,   CharType::kDASH,    CharType::kDOT,     CharType::kFSLASH,
/* 3x */   CharType::kDIGIT,   CharType::kDIGIT,   CharType::kDIGIT,   CharType::kDIGIT,   CharType::kDIGIT,   CharType::kDIGIT,   CharType::kDIGIT,   CharType::kDIGIT,
           CharType::kDIGIT,   CharType::kDIGIT,   CharType::kCOLON,   CharType::kSCOLON,  CharType::kLT,      CharType::kEQ,      CharType::kGT,      CharType::kQUEST,   
/* 4x */   CharType::kAT,      CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,
           CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,
/* 5x */   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   
           CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kLBRACK,  CharType::kBSLASH,  CharType::kRBRACK,  CharType::kCARET,   CharType::kUNDER,  
/* 6x */   CharType::kGRAVE,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,
           CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,
/* 7x */   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,
           CharType::kALPHA,   CharType::kALPHA,   CharType::kALPHA,   CharType::kLBRACE,  CharType::kPIPE,    CharType::kRBRACE,  CharType::kTILDE,   CharType::kILLEGAL
};

// Map to get lowercase (only change 'A' -> 'a')
static const char mLower[] = {
/*         x0    x1    x2    x3    x4    x5    x6    x7    x8    x9    xa    xb    xc    xd    xe    xf */
/* 0x */   0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
/* 1x */   0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
/* 2x */   0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
/* 3x */   0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
/* 4x */   0x40, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
/* 5x */   0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
/* 6x */   0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
/* 7x */   0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F
};

// Create some Token Types
