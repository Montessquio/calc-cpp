#pragma once

#include <string>
#include <vector> // Needed for tokenize_expr proto

// =======================================
//
// This module is a simple lexer for a 
// basic arithmetic grammar. It maintains
// no state across calls, and thus can be
// used with just a single function call.
//
// Usage:
//      char err_code = tokenize_expr(string, &);
//
// This module also contains const
// definitions of error codes that can
// be returned by the function.
//
// For more information, see the documentation
// for each respective member.
//
// =======================================
namespace calc {
    namespace lexer {

        // An error code of zero means the lexer
        // did not encounter any errors during
        // operation.
        static const char ERR_NO_ERR = 0;

        // Returned when a numeric value is not recognized as
        // a valid number.
        static const char ERR_VALUE_NOT_RECOGNIZED = 1;

        // A single token recognized by the lexer.
        struct Token {
            // What kind of token this is.
            // See the static consts below.
            char type;

            // The unmodified substring that
            // was matched to produce this token.
            std::string raw;
        };

        // These constants represent
        // all the possible tokens that
        // can be encountered in a valid
        // tokenizer run.

        // A number, which means a double.
        static const char NUMBER = 0;

        // An addition or subtraction token.
        static const char TERM = 1;

        // A multiplication or division token.
        static const char FACTOR = 2;

        // A parenthetical statement.
        static const char GROUP = 3;

        // An error token with undefined state.
        static const char UNDEFINED = 255;

        // =======================================
        // 
        // Split a string containing an arithmetic
        // expression into tokens.
        //
        // Parameters:
        //      string:  
        //              The raw input from the user,
        //              must be valid ASCII.
        //
        //      pointer to vec<Token>: 
        //              A vector of Tokens which will
        //              contain all the tokens
        //              recognized by the lexer.
        //
        // Outputs:
        //      char:   An error code. Zero means
        //              no error.
        //
        // =======================================
        char tokenize_expr(std::string, std::vector<Token>*);

        bool is_double(std::string input);
    }
}