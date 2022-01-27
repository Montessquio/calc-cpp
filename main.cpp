#include "tokenizer.h"
#include "parser.h"

#include <cstdlib>
#include <iostream>
#include <string>

using std::string;
using std::cout;
using std::cin;
using std::endl;

using namespace calc;

void display_token(lexer::Token*);
void display_expr(parser::Expression*);

int main() {
    while(true) {
        // Prompt user input.
        string line;
        cout << "> ";
        getline(cin, line);
        // Tokenize.
        std::vector<lexer::Token> tokens;
        char err = lexer::tokenize_expr(line, &tokens);

        // Check for and print lexer errors.
        if(err != lexer::ERR_NO_ERR) {
            char buffer[4];
            cout << "Error parsing string (code " << itoa(err, buffer, 10) << ")" << endl;
            continue;
        }

        /*
        // If no error, then print tokens.
        cout << "Tokens: [\"";
        for(int i = 0; i < tokens.size() - 1; i += 1) {
            display_token(&tokens[i]);
            cout << "\", ";
        }
        display_token(&tokens[tokens.size() - 1]);
        cout << "\"]" << endl;
        */

        // Parse tokens.
        parser::Parser p = parser::Parser(&tokens);
        
        try {
            parser::Expression* ex = p.expr();
            // Print computed result.
            //ex->display();
            cout << ex->eval() << endl;
        }
        catch (char err) {
            char buffer[4];
            cout << "Invalid Syntax (code " << itoa(err, buffer, 10) << ")" << endl;
            continue;
        }
    }
    return 0;
}

void display_token(lexer::Token* t) {
    cout << "(";
    switch(t->type) {
        case lexer::TERM:
            cout << "TERM";
            break;
        case lexer::FACTOR:
            cout << "FACTOR";
            break;
        case lexer::GROUP:
            cout << "GROUP";
            break;
        case lexer::NUMBER:
            cout << "NUMBER";
            break;
        default:
            cout << t->type;
            break;
    }
    cout << ", " << t->raw << ")";
}