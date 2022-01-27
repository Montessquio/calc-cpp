#include "tokenizer.h"

#include <stdlib.h>
#include <string>
#include <vector>

using std::string;

namespace calc {
    namespace lexer {
        char tokenize_expr(std::string input, std::vector<Token>* output) {
            string sb = ""; // Buffer for building current token.

            for(int i = 0; i < input.length(); i++) {
                switch(input[i]) {
                    case '\t':
                    case '\n':
                    case '\r':
                    case ' ':
                        continue; // Ignore whitespace.
                    
                    case '+':
                    case '-':
                    case '*':
                    case '/':
                    case '(':
                    case ')': {
                        // Encountering a transition character means the 
                        // previous token is over.
                        // Tokenize it before working on the next one.

                        // Check that the value stored is a valid double.
                        if (!sb.empty()) {
                            if (!is_double(sb)) {
                                return ERR_VALUE_NOT_RECOGNIZED;
                            }
                            Token num_token;
                            num_token.type = NUMBER;
                            num_token.raw = sb;
                            output->push_back(num_token);
                        }

                        Token transition_token;

                        // Assign the token type first.
                        switch(input[i]) {
                            case '+':
                            case '-':
                                transition_token.type = TERM;
                                break;
                            case '*':
                            case '/':
                                transition_token.type = FACTOR;
                                break;
                            case '(':
                            case ')':
                                transition_token.type = GROUP;
                                break;
                        }
                        string st_char(1, input[i]);
                        transition_token.raw = st_char;
                        output->push_back(transition_token);

                        sb = "";
                        continue;
                    }
                    default:
                        sb = sb + input[i];
                        continue;
                }
            }

            // Parse any remaining characters in the buffer
            // that may not have been terminated by an operator.
            if (!sb.empty()) {
                if (!is_double(sb)) {
                    return ERR_VALUE_NOT_RECOGNIZED;
                }
                Token num_token;
                num_token.type = NUMBER;
                num_token.raw = sb;
                output->push_back(num_token);
            }

            return ERR_NO_ERR;
        }

        // Check that a string is a double.
        bool is_double(std::string input) {
            char* endptr = 0;
            strtod(input.c_str(), &endptr);

            if(*endptr != '\0' || endptr == input)
                return false;
            return true;
        }
    }
}