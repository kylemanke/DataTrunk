// C++ Includes
#include <cstdint>

// Local Includes
#include "exception/token_exception.h"
#include "tokenizer.h"

// Necessary Enums
enum class CharType : uint8_t {
    kNULL,      // \0
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
    // Function
    kEOF,
    // ERRORS
    kERROR
};

enum class TokenState : uint16_t {
    // Book keeping states
    kS0, kS_DONE, kS_ERROR,
    // Keyword States
    kS_S, kS_ST, kS_STR, kS_STRU, kS_STRUC, kS_STRUCT,
    kS_B, kS_BY, kS_BYT, kS_BYTE,
    kS_SH, kS_SHO, kS_SHOR, kS_SHORT,
    kS_I, kS_IN, kS_INT,
    kS_L, kS_LO, kS_LON, kS_LONG,
    kS_F, kS_FL, kS_FLO, kS_FLOA, kS_FLOAT,
    kS_D, kS_DO, kS_DOU, kS_DOUB, kS_DOUBL, kS_DOUBLE,
    kS_STRI, kS_STRIN, kS_STRING,
    kS_C, kS_CO, kS_CON, kS_CONS, kS_CONST,
    kS_SI, kS_SIG, kS_SIGN, kS_SIGNE, kS_SIGNED,
    kS_U, kS_UN, kS_UNS, kS_UNSI, kS_UNSIG, kS_UNSIGN, kS_UNSIGNE, kS_UNSIGNED,
    kS_N, kS_NO, kS_NOT, kS_NOTN, kS_NOTN, kS_NOTNU, kS_NOTNUL, kS_NOTNULL,
    kS_P, kS_PR, kS_PRI, kS_PRIM, kS_PRIMA, kS_PRIMAR, kS_PRIMARY,
    kS_SE, kS_SEC, kS_SECO, kS_SECON, kS_SECOND, kS_SECONDA, kS_SECONDAR, kS_SECONDARY,
    kS_NE, kS_NEW,
    kS_DE, kS_DEL, kS_DELE, kS_DELET, kS_DELETE,
    // Comments
    kS_COMMENT,
    // Identifiers
    kS_IDENTIFIER,
    // Numbers
    kS_INTEGER,
    kS_REAL_NUMBER,
    // Strings
    kS_STRING_LITERAL,
    // Boolean Operators
    kS_AND, kS_OR, kS_LT, kS_GT, kS_NOT, kS_EQUAL,
    // Arithmetic Operators
    kS_PLUS, kS_MINUS, kS_AST, kS_FORWARD_SLASH, kS_MOD,
    // Special Characters
    kS_LBRACK, kS_RBRACK, kS_LPAREN, kS_RPAREN, kS_LBRACE, kS_RBRACE, kS_COMMA,
    kS_DOT, kS_COLON, kS_SCOLON, kS_NULL, kS_ESCAPE
};

// Define some maps for faster use
// Map for char type
static const CharType mCharType[] = {
/*         x0                  x1                  x2                  x3                  x4                  x5                  x6                  x7
           x8                  x9                  xa                  xb                  xc                  xd                  xe                  xf */
/* 0x */   CharType::kNULL, CharType::kILLEGAL, CharType::kILLEGAL, CharType::kILLEGAL, CharType::kILLEGAL, CharType::kILLEGAL, CharType::kILLEGAL, CharType::kILLEGAL,
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

Tokenizer::Tokenizer() noexcept
: str_i_(0), str_(""), curr_token_(TokenType::kEOF), curr_token_val_("")
{}

Tokenizer::~Tokenizer() noexcept {}

void Tokenizer::OpenString(const std::string& trunk_str) {
    fr_.Close();
    str_i_ = 0;
    str_ = trunk_str;
}

void Tokenizer::OpenString(const char* trunk_str) {
    fr_.Close();
    str_i_ = 0;
    str_ = std::string(trunk_str);
}

void Tokenizer::OpenFile(const std::string& file_name) {
    str_ = "";
    fr_.Open(file_name);
}

void Tokenizer::OpenFile(const char* file_name) {
    str_ = "";
    fr_.Open(file_name);
}

TokenType Tokenizer::CurrToken() const noexcept {
    return curr_token_;
}

std::string Tokenizer::CurrTokenValue() const noexcept {
    return curr_token_val_;
}

uint8_t Tokenizer::NextChar() {
    uint8_t ret_val = '\0';
    if (str_ != "" && str_i_ != static_cast<uint32_t>(str_.size())) {
        ret_val = str_[str_i_++];
    } else if (!fr_.End()) {
        ret_val = fr_.ReadByte();
    }
    return ret_val;
}

uint8_t Tokenizer::PeekChar() {
    uint8_t ret_val = '\0';
    if (str_ != "" && str_i_ != static_cast<uint32_t>(str_.size())) {
        ret_val = str_[str_i_];
    } else if (!fr_.End()) {
        ret_val = fr_.PeekByte();
    }
    return ret_val;
}

TokenType Tokenizer::NextToken() {
    curr_token_ = TokenType::kEOF;
    curr_token_val_ = "";
    TokenState curr_state = TokenState::kS0;
    while (curr_state != TokenState::kS_DONE) {
        switch(curr_state) {
            case TokenState::kS0: {
                switch(mCharType[PeekChar()]) {
                    case CharType::kWS: {
                        NextChar();
                    } break;
                    case CharType::kNEWLINE: {
                        NextChar();
                    } break;
                    case CharType::kUNDER: {
                        curr_state = ScanIdentifier();
                    } break;
                    case CharType::kALPHA: {
                        curr_state = ScanKeyword();
                    } break;
                    case CharType::kDIGIT: {
                        curr_state = ScanNumber();
                    } break;
                    case CharType::kDQUOTE: {
                        curr_state = ScanString();
                    } break;
                    case CharType::kFSLASH: {
                        curr_token_val_ += NextChar();
                        switch(mCharType[PeekChar()]) {
                            case CharType::kEQ: {
                                curr_token_val_ += NextChar();
                                curr_token_ = TokenType::kDIV_EQU;
                                curr_state = TokenState::kS_DONE;
                            } break;
                            case CharType::kFSLASH: {
                                NextChar();
                                curr_state = TokenState::kS_COMMENT;
                            } break;
                            default: {
                                curr_token_ = TokenType::kDIVIDE;
                                curr_state = TokenState::kS_DONE;
                            }
                        }
                    } break;
                    case CharType::kAND: {
                        curr_token_val_ += NextChar();
                        switch(mCharType[PeekChar()]) {
                            case CharType::kAND: {
                                curr_token_val_ += NextChar();
                                curr_token_ = TokenType::kBOOL_AND;
                                curr_state = TokenState::kS_DONE;
                            } break;
                            default: {
                                curr_token_ = TokenType::kERROR;
                                curr_token_val_ = "Expected &";
                                curr_state = TokenState::kS_ERROR;
                            }
                        }
                    } break;
                    case CharType::kPIPE: {
                        curr_token_val_ += NextChar();
                        switch(mCharType[PeekChar()]) {
                            case CharType::kPIPE: {
                                curr_token_val_ += NextChar();
                                curr_token_ = TokenType::kBOOL_OR;
                                curr_state = TokenState::kS_DONE;
                            } break;
                            default: {
                                curr_token_ = TokenType::kERROR;
                                curr_token_val_ = "Expected |";
                                curr_state = TokenState::kS_ERROR;
                            }
                        }
                    } break;
                    case CharType::kLT: {
                        curr_token_val_ += NextChar();
                        switch(mCharType[PeekChar()]) {
                            case CharType::kEQ: {
                                curr_token_val_ += NextChar();
                                curr_token_ = TokenType::kLTE;
                                curr_state = TokenState::kS_DONE;
                            } break;
                            default: {
                                curr_token_ = TokenType::kLT;
                                curr_state = TokenState::kS_DONE;
                            }
                        }
                    } break;
                    case CharType::kGT: {
                        curr_token_val_ += NextChar();
                        switch(mCharType[PeekChar()]) {
                            case CharType::kEQ: {
                                curr_token_val_ += NextChar();
                                curr_token_ = TokenType::kGTE;
                                curr_state = TokenState::kS_DONE;
                            } break;
                            default: {
                                curr_token_ = TokenType::kGT;
                                curr_state = TokenState::kS_DONE;
                            }
                        }
                    } break;
                    case CharType::kEQ: {
                        curr_token_val_ += NextChar();
                        switch(mCharType[PeekChar()]) {
                            case CharType::kEQ: {
                                curr_token_val_ += NextChar();
                                curr_token_ = TokenType::kEQUALITY;
                                curr_state = TokenState::kS_DONE;
                            } break;
                            default: {
                                curr_token_ = TokenType::kASSIGN;
                                curr_state = TokenState::kS_DONE;
                            }
                        }
                    } break;
                    case CharType::kBANG: {
                        curr_token_val_ += NextChar();
                        switch(mCharType[PeekChar()]) {
                            case CharType::kEQ: {
                                curr_token_val_ += NextChar();
                                curr_token_ = TokenType::kDNE;
                                curr_state = TokenState::kS_DONE;
                            } break;
                            default: {
                                curr_token_ = TokenType::kNOT;
                                curr_state = TokenState::kS_DONE;
                            }
                        }
                    } break;
                    case CharType::kPLUS: {
                        curr_token_val_ += NextChar();
                        switch(mCharType[PeekChar()]) {
                            case CharType::kEQ: {
                                curr_token_val_ += NextChar();
                                curr_token_ = TokenType::kADD_EQU;
                                curr_state = TokenState::kS_DONE;
                            } break;
                            default: {
                                curr_token_ = TokenType::kPLUS;
                                curr_state = TokenState::kS_DONE;
                            }
                        }
                    } break;
                    case CharType::kDASH: {
                        curr_token_val_ += NextChar();
                        switch(mCharType[PeekChar()]) {
                            case CharType::kEQ: {
                                curr_token_val_ += NextChar();
                                curr_token_ = TokenType::kSUB_EQU;
                                curr_state = TokenState::kS_DONE;
                            } break;
                            default: {
                                curr_token_ = TokenType::kMINUS;
                                curr_state = TokenState::kS_DONE;
                            }
                        }
                    } break;
                    case CharType::kAST: {
                        curr_token_val_ += NextChar();
                        switch(mCharType[PeekChar()]) {
                            case CharType::kEQ: {
                                curr_token_val_ += NextChar();
                                curr_token_ = TokenType::kMULT_EQU;
                                curr_state = TokenState::kS_DONE;
                            } break;
                            default: {
                                curr_token_ = TokenType::kASTERISK;
                                curr_state = TokenState::kS_DONE;
                            }
                        }
                    } break;
                    case CharType::kPERCENT: {
                        curr_token_val_ += NextChar();
                        switch(mCharType[PeekChar()]) {
                            case CharType::kEQ: {
                                curr_token_val_ += NextChar();
                                curr_token_ = TokenType::kMOD_EQU;
                                curr_state = TokenState::kS_DONE;
                            } break;
                            default: {
                                curr_token_ = TokenType::kMODULO;
                                curr_state = TokenState::kS_DONE;
                            }
                        }
                    } break;
                    case CharType::kLBRACK: {
                        curr_token_val_ += NextChar();
                        curr_token_ = TokenType::kLBRACK;
                        curr_state = TokenState::kS_DONE;
                    } break;
                    case CharType::kRBRACK: {
                        curr_token_val_ += NextChar();
                        curr_token_ = TokenType::kRBRACK;
                        curr_state = TokenState::kS_DONE;
                    } break;
                    case CharType::kLP: {
                        curr_token_val_ += NextChar();
                        curr_token_ = TokenType::kLPAREN;
                        curr_state = TokenState::kS_DONE;
                    } break;
                    case CharType::kRP: {
                        curr_token_val_ += NextChar();
                        curr_token_ = TokenType::kRPAREN;
                        curr_state = TokenState::kS_DONE;
                    } break;
                    case CharType::kLBRACE: {
                        curr_token_val_ += NextChar();
                        curr_token_ = TokenType::kLBRACE;
                        curr_state = TokenState::kS_DONE;
                    } break;
                    case CharType::kRBRACE: {
                        curr_token_val_ += NextChar();
                        curr_token_ = TokenType::kRBRACE;
                        curr_state = TokenState::kS_DONE;
                    } break;
                    case CharType::kCOMMA: {
                        curr_token_val_ += NextChar();
                        curr_token_ = TokenType::kCOMMA;
                        curr_state = TokenState::kS_DONE;
                    } break;
                    case CharType::kDOT: {
                        curr_token_val_ += NextChar();
                        switch(mCharType[PeekChar()]) {
                            case CharType::kDIGIT: {
                                curr_state = TokenState::kS_REAL_NUMBER;
                            } break;
                            default: {
                                curr_token_ = TokenType::kDOT;
                                curr_state = TokenState::kS_DONE;
                            }
                        }
                    } break;
                    case CharType::kCOLON: {
                        curr_token_val_ += NextChar();
                        curr_token_ = TokenType::kCOLON;
                        curr_state = TokenState::kS_DONE;
                    } break;
                    case CharType::kSCOLON: {
                        curr_token_val_ += NextChar();
                        curr_token_ = TokenType::kSCOLON;
                        curr_state = TokenState::kS_DONE;
                    } break;
                    case CharType::kNULL: {
                        curr_state = TokenState::kS_NULL;
                    }break;
                    default: {
                        curr_token_val_ = "Illegal character encountered";
                        curr_token_ = TokenType::kERROR;
                        curr_state = TokenState::kS_ERROR;
                    }
                }
            } break;
            case TokenState::kS_IDENTIFIER: {
                curr_state = ScanIdentifier();
            } break;
            case TokenState::kS_REAL_NUMBER: {
                curr_state = ScanNumber();
            }
            case TokenState::kS_COMMENT: {
                curr_token_val_ = "";
                curr_token_ = TokenType::kEOF;
                while(PeekChar() != '\n' || PeekChar() != '\0') {
                    NextChar();
                }
                if (PeekChar() == '\0') {
                    curr_state = TokenState::kS_NULL;
                } else {
                    NextChar();
                    curr_state = TokenState::kS0;
                }
            } break;
            case TokenState::kS_NULL: {
                if (fr_.End() || str_i_ == static_cast<uint32_t>(str_.size())) {
                    curr_token_ = TokenType::kEOF;
                    curr_state = TokenState::kS_DONE;
                } else {
                    curr_token_val_ = "Illegal character encountered";
                    curr_token_ = TokenType::kERROR;
                    curr_state = TokenState::kS_ERROR;
                }
            } break;
            case TokenState::kS_ERROR: {
                throw TokenException(curr_token_val_);
            } break;
            default: {
                throw TokenException("Illegal state encountered");
            }
        }
    }
    return curr_token_;
}

TokenState Tokenizer::ScanIdentifier() {
    TokenState curr_state = TokenState::kS_IDENTIFIER;
    while (curr_state == TokenState::kS_IDENTIFIER) {
        switch(mCharType[PeekChar()]) {
            case CharType::kUNDER: {
                curr_token_val_ += NextChar();
            } break;
            case CharType::kALPHA: {
                curr_token_val_ += NextChar();
            } break;
            case CharType::kDIGIT: {
                curr_token_val_ += NextChar();
            } break;
            default: {
                curr_token_ = TokenType::kIDENTIFIER;
                curr_state = TokenState::kS_DONE;
            }
        }
    }
    return curr_state;
}

TokenState Tokenizer::ScanNumber() {
    TokenState curr_state = TokenState::kS_INTEGER;
    if (curr_token_val_ == ".") {
        curr_state = TokenState::kS_REAL_NUMBER;
    }
    while (curr_state == TokenState::kS_INTEGER || curr_state == TokenState::kS_REAL_NUMBER || curr_state == TokenState::kS_DOT) {
        switch(curr_state) {
            case TokenState::kS_INTEGER: {
                switch(mCharType[PeekChar()]) {
                    case CharType::kDIGIT: {
                        curr_token_val_ += NextChar();
                    } break;
                    case CharType::kDOT: {
                        curr_state = TokenState::kS_DOT;
                    } break;
                    default: {
                        curr_state = TokenState::kS_DONE;
                    }
                }
            } break;
            case TokenState::kS_REAL_NUMBER: {
                switch(mCharType[PeekChar()]) {
                    case CharType::kDIGIT: {
                        curr_token_val_ += NextChar();
                    } break;
                    default: {
                        curr_state = TokenState::kS_DONE;
                    }
                }
            } break;
            case TokenState::kS_DOT: {
                curr_token_val_ += NextChar();
                curr_state = TokenState::kS_REAL_NUMBER;
            } break;
            default: {
                curr_state = TokenState::kS_DONE;
            }
        }
    }
    return curr_state;
}

TokenState Tokenizer::ScanString() {
    NextChar(); // Read in the start quote
    TokenState curr_state = TokenState::kS_STRING_LITERAL;
    while(curr_state == TokenState::kS_STRING_LITERAL || curr_state == TokenState::kS_FORWARD_SLASH) {
        switch(curr_state) {
            case TokenState::kS_STRING_LITERAL: {
                switch(mCharType[PeekChar()]) {
                    case CharType::kDQUOTE: {
                        curr_state = TokenState::kS_DONE;
                    } break;
                    case CharType::kFSLASH: {
                        NextChar();
                        curr_state = TokenState::kS_ESCAPE;
                    } break; 
                    case CharType::kNULL: {
                        curr_token_val_ = "Unexpected null character";
                        curr_state = TokenState::kS_ERROR;
                    } break;
                    case CharType::kNEWLINE: {
                        curr_token_val_ = "Missing \"";
                        curr_state = TokenState::kS_ERROR;
                    }
                    default: {
                        curr_token_val_ += NextChar();
                    }
                }
            } break;
            case TokenState::kS_ESCAPE: {
                switch(PeekChar()) {
                    case 'b': {
                        NextChar();
                        curr_token_val_ += '\b';
                        curr_state = TokenState::kS_STRING_LITERAL;
                    } break;
                    case 'a': {
                        NextChar();
                        curr_token_val_ += '\b';
                        curr_state = TokenState::kS_STRING_LITERAL;
                    } break;
                    case 'f': {
                        NextChar();
                        curr_token_val_ += '\f';
                        curr_state = TokenState::kS_STRING_LITERAL;
                    } break;
                    case 'n': {
                        NextChar();
                        curr_token_val_ += '\n';
                        curr_state = TokenState::kS_STRING_LITERAL;
                    } break;
                    case 'r': {
                        NextChar();
                        curr_token_val_ += '\r';
                        curr_state = TokenState::kS_STRING_LITERAL;
                    } break;
                    case 't': {
                        NextChar();
                        curr_token_val_ += '\t';
                        curr_state = TokenState::kS_STRING_LITERAL;
                    } break;
                    case 'v': {
                        NextChar();
                        curr_token_val_ += '\v';
                        curr_state = TokenState::kS_STRING_LITERAL;
                    } break;
                    case '\'': {
                        NextChar();
                        curr_token_val_ += '\'';
                        curr_state = TokenState::kS_STRING_LITERAL;
                    } break;
                    case '\"': {
                        NextChar();
                        curr_token_val_ += '\"';
                        curr_state = TokenState::kS_STRING_LITERAL;
                    } break;
                    case '\\': {
                        NextChar();
                        curr_token_val_ += '\\';
                        curr_state = TokenState::kS_STRING_LITERAL;
                    } break;
                    case '?': {
                        NextChar();
                        curr_token_val_ += '?';
                        curr_state = TokenState::kS_STRING_LITERAL;
                    } break;
                    default: {
                        curr_token_val_ = "Unkown escape character";
                        curr_state = TokenState::kS_ERROR;
                    }
                }
            } break;
            default:
                throw TokenException("Invalid state in ScanString");
        }
    }
    return curr_state;
}

TokenState Tokenizer::ScanIdentifier() {
    
}