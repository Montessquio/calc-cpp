#pragma once

#include "tokenizer.h"
#include <string>
#include <vector> // Needed for tokenize_expr proto

namespace calc {
    namespace parser {
        using ::calc::lexer::Token;

        // =======================================
        //
        // This module is a simple parser for a 
        // basic arithmetic grammar.
        //
        // Usage:
        //      // Assuming `tokens` is a valid vec<Token>.
        //      calc::parser::Parser p(tokens);
        //
        //      char err;
        //      calc::parser::Expr e = p.expr(&err);
        //      if(err != 0) {
        //          cout << "There was some error!" << endl;
        //      }
        //      double result = e.eval();
        //      cout << "Final result of calculation is: " << result << endl;
        //
        //  The text will be parsed according to the following grammar:
        //      	Expr	=> Term
        //      	Term	=> Factor  ([ '+' | '-' ]) Factor)*
        //      	Factor 	=> Primary ([ '*' | '/' ] Primary)*
        //      	Primary => '(' Expr ')' | NUMBER
        //
        // This module also contains const
        // definitions of error codes that can
        // be returned by the function.
        //
        // For more information, see the documentation
        // for each respective member.
        //
        // =======================================

        // Expression is the base class for all other
        // Ast node classes. It provides the eval() interface
        // which is overridden by each of its children
        // to provide the calculated result of that child's members.
        class Expression {
            public:
                virtual double eval() {};
                virtual void display() {};
        };

        // A numeric expression simply contains a double.
        class NumExpr : public Expression {
            private:
                double inner;
            public:
                NumExpr(double);
                double eval();
                void display();
        };

        // An operation expression is a computable arithmetic expression.
        class OpExpr : public Expression {
            private:
                Expression* left;
                Expression* right;
                Token* op;
            public:
                OpExpr(Expression*, Expression*, Token*);
                ~OpExpr();
                double eval();
                void display();
        };

        class Parser {
            private:
                std::vector< Token >* tokens;
                long current;

                // Term satisfies the rule for `Term	=> Factor  ([ '+' | '-' ]) Factor)*`
                Expression* term();
                // Factor satisfies the rule for `Factor 	=> Primary ([ '*' | '/' ] Primary)*`
                Expression* factor();
                // Primary satisfies the rule for `Primary => '(' Expr ')' | DIE | NUMBER`
                Expression* primary();

                // Consumes the current token if it matches the given type,
                // advancing the cursor and returning it. Otherwise does nothing.
                Token* consume();
                // Returns whether the token is of the given type. Does not consume.
                bool check(char);
                // Get the current token without advancing nor consuming it.
                // Can return Null if the current token is out of bounds.
                Token* peek();
                // Returns whether the "current" field = len(tokens) - 1
                bool is_at_end();
            public:
                Parser(std::vector<Token>*);
                // Expr satisfies the rule `Expr => Term`.
                Expression* expr();
        };

        static const char ERR_NO_ERR = 0;
        static const char ERR_UNMATCHED_PAREN = 1;
        static const char ERR_INVALID_SYNTAX = 2;
        static const char ERR_UNEXPECTED_END = 3;
    }
}