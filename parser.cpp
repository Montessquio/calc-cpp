#include "parser.h"
#include "tokenizer.h"

#include <stdlib.h>
#include <string>
#include <vector>

#include <iostream>
using std::cout;
using std::cin;
using std::endl;

using std::string;
using namespace ::calc::lexer;

namespace calc {
    namespace parser {
        /*
         *
         *  Parser Class
         * 
        */

        Parser::Parser(std::vector<lexer::Token>* toks) {
            tokens = toks;
            current = 0;
        }

        Expression* Parser::expr() {
            return term();
        }

        Expression* Parser::term() {
            Expression* ex = factor();

            while(check(lexer::TERM)) {
                lexer::Token* op = consume();
                Expression* right = factor();
                ex = new OpExpr(ex, right, op);
            }

            return ex;
        }

        Expression* Parser::factor() {
            Expression* ex = primary();

            while(check(lexer::FACTOR)) {
                lexer::Token* op = consume();
                Expression* right = primary();
                ex = new OpExpr(ex, right, op);
            }

            return ex;
        }

        Expression* Parser::primary() {
            if(check(lexer::NUMBER)) {
                // If the current token is a number...
                lexer::Token* t = consume();

                // This should never fail because the tokenizer verifies that
		        // this kind of token is purely numeric.
                return new NumExpr(strtod(t->raw.c_str(), NULL));
            }

            if(check(lexer::GROUP)) {
                consume();

                // In the case of a group, recurse back to the lowest priority and build a new subtree.
                Expression* ex = expr();

                // Expect a closing paren.
                try {
                    if(check(lexer::GROUP) && peek()->raw == ")") {
                        consume();
                        return ex;
                    }
                }
                catch (char _) {
                    throw ERR_UNMATCHED_PAREN;
                }
            }
        }

        Token* Parser::consume() {
            if(!is_at_end()) {
                // Advance the cursor and return whatever was before it.
                current += 1;
                return &tokens->at(current - 1);
            }
            // If we are at the end, then there's only one token left to consume.
            return &tokens->at(current);
        }

        bool Parser::check(char token_type) {
            try {
                return peek()->type == token_type;
            }
            catch(char _) {
                return false;
            }
        }

        lexer::Token* Parser::peek() {
            try {
                return &tokens->at(current);
            }
            catch (const std::out_of_range& oor) {
                throw ERR_UNEXPECTED_END;
            }
        }

        bool Parser::is_at_end() {
            return current >= tokens->size();
        }

        /*
         *
         *  Expression Classes
         * 
        */

        NumExpr::NumExpr(double value) {
            inner = value;
        }

        double NumExpr::eval() {
            return inner;
        }

        void NumExpr::display() {
            cout << "(" << inner << ")"; 
        }

        OpExpr::OpExpr(Expression* l, Expression* r, lexer::Token* oper) {
            left = l;
            right = r;
            op = oper;
        }

        double OpExpr::eval() {
            if(left && right) {
                switch(op->raw[0]) {
                    case '+':
                        return double(left->eval() + right->eval());
                    case '-':
                        return double(left->eval() - right->eval());
                    case '*':
                        return double(left->eval() * right->eval());
                    case '/': 
                        return double(left->eval() / right->eval()); 
                }
            }
            throw ERR_INVALID_SYNTAX;
        }

        void OpExpr::display() {
            cout << "[";
            if(left) {
                left->display();
            } else {
                cout << "NULL";
            }
            cout << " " << op->raw << " ";
            if(right) {
                right->display();
            } else {
                cout << "NULL";
            }
            cout << "]";
        }

        OpExpr::~OpExpr() {
            delete left;
            delete right;
            delete op;
        }
    }
}