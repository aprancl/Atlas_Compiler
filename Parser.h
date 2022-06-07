//
// Created by Anthony Prancl on 5/29/22.
//

#ifndef ATLAS_COMPILER_PARSER_H
#define ATLAS_COMPILER_PARSER_H

#include "Lexer.h"
#include <stdio.h>
#include <vector>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"


class Parser {


    Lexer lexer;
    Token lastToken;
    Token curToken;
    Token nextToken;
    std::vector<std::string> declared_vars;

public:
    // constructors

    Parser() = default;

    Parser(Lexer lexer) {
        this->lexer = lexer;
        lastToken = Token("", None);
        curToken = Token("", None);
        nextToken = Token("", None);
        advanceToken();
        advanceToken();

    }


private:

    // helper methods


    bool isUsedIdentifier(std::string tokenText) {
        if (this->declared_vars.size() == 0) {
            return 0;

        } else {
            for (std::string var: declared_vars) {
                if (tokenText == var) {
                    return 1;
                }
            }
            return 0;

        }

    }


    void isValidIdentifier() {

        if (compareToCurToken(
                Identifier)) { // identifiers need to be handled such that duplicates are not allowed

            if (!isUsedIdentifier(curToken.getTokenText())) {
                printf(ANSI_COLOR_CYAN "Parsing error..referencing variable <%s> before assignment");
            }


            // this system removes duplicates, but which is good, but it doesn't belong here
//            if (!isUsedIdentifier(curToken.getTokenText())) {
//                declared_vars.push_back(curToken.getTokenText());
//                advanceToken();
//                skipWhiteSpaces();
//            } else { // in the case that the variable has already been used
//                printf(ANSI_COLOR_CYAN "\nParsing error..cannot redeclare instantiated variable..line number: %d\nexpected token(s) <%s> but got <%s>",
//                       lexer.getCurLineNumber() + 2,// <-* ended here
//                       Token::typeToString(type).c_str(),
//                       Token::typeToString(this->curToken.getType()).c_str());
//                exit(35);
//            }
        }
    }

    bool isComparisonOperator(Token token) {
        return (token.getType() == EqEq || token.getType() == NotEq || token.getType() == GtEq ||
                token.getType() == LtEq || token.getType() == Gt || token.getType() == Lt);
    }

    bool isMathOperator(Token token) {
        return (token.getType() == Plus || token.getType() == Minus || token.getType() == Asterisk ||
                token.getType() == Fslash);
    }

    void skipWhiteSpaces() {

        while (compareToCurToken(Space) || compareToCurToken(NewLine)) {
            advanceToken();
        }
    }

    // compare a given token to what the current token is
    bool compareToCurToken(TokenType type) { //             used to check the first token of each statement
        return (type == this->curToken.getType());
    }

    bool compareToNextToken(TokenType type) {
        return (type == this->nextToken.getType());
    }


    void advanceToken() {
        lastToken = curToken;
        curToken = nextToken;
        nextToken = lexer.getToken();
    }

    void match(TokenType type) {//                      used to check tokens within a statement
        if (!(compareToCurToken(type))) {
            printf(ANSI_COLOR_CYAN "\nParsing error..expected <%s> but got <%s> ..line number: %d\n",
                   Token::typeToString(type).c_str(),
                   Token::typeToString(this->curToken.getType()).c_str(), lexer.getCurLineNumber());
            exit(35); //  stop program && lexical analysis
        } else if (compareToCurToken(Identifier)) {
            declared_vars.push_back(curToken.getTokenText());
            advanceToken();
            skipWhiteSpaces();
        } else {
            advanceToken();
            skipWhiteSpaces();
        }
    }


    // the following is a series of functions that represent the various levels of abstraction in this programming language's grammar
    // ie. a program consists of statements and statement consists of a combination of keywords and symbols etc...
    // for more details, see README


    // End Of Code Block
    void EOCB() {


        std::cout << "END OF CODE BLOCK" << std::endl;

        if (compareToCurToken(CrlbraceR) && compareToNextToken(Eos)) {
            advanceToken();
            advanceToken();
            skipWhiteSpaces();

        } else {
            printf(ANSI_COLOR_CYAN "Parsing error..reached end of file before code block terminated");
            exit(35);
        }

    }

    // End Of Statement
    void EOS() {

        std::cout << "END OF STATEMENT" << std::endl;

        while (compareToCurToken(Eos) || compareToCurToken(NewLine) || compareToCurToken(Space)) {
            advanceToken();
        }

    }


    // parsing through expressions

    void primary() {
        skipWhiteSpaces();

        printf("PRIMARY [%s -> %s]\n", Token::typeToString(curToken.getType()).c_str(),
               curToken.getTokenText().c_str());

        if (compareToCurToken(IntLiteral) || compareToCurToken(FloatLiteral)) {
            advanceToken();
            skipWhiteSpaces();
        } else if (compareToCurToken(Identifier)) {
            advanceToken();
            skipWhiteSpaces();
        } else {

            if (compareToCurToken(StringLiteral)) {
                printf(ANSI_COLOR_CYAN "Parsing error..cannot use string literals in calculations..line number: %d",
                       lexer.getCurLineNumber());
                exit(35);
            } else {
                printf(ANSI_COLOR_CYAN "Parsing error..unexpected token in expression <%s>..line number: %d",
                       curToken.getTokenText().c_str(), lexer.getCurLineNumber());
                exit(35);
            }

        }


    }

    void unary() {
        std::cout << "UNARY\n";

        if (compareToCurToken(Plus) || compareToCurToken(Minus)) {
            advanceToken();
        }
        primary();
    }

    void term() {
        std::cout << "TERM\n";
        unary();

        // there can be 0 or more mult OR div sub expressions
        while (compareToCurToken(Asterisk) || compareToCurToken(Fslash)) {
            advanceToken();
            unary();
        }
    }


    void expression() {

        std::cout << "..EXPRESSION\n";

        term();

        // there are 0 or more + OR - expression
        while (compareToCurToken(Plus) || compareToCurToken(Minus)) {
            advanceToken();
            term();
        }

    }

    void comparison() {
        std::cout << "=COMPARISON=\n";

        expression();

        if (isComparisonOperator(curToken)) {
            advanceToken();
            expression();
        } else {
            printf(ANSI_COLOR_CYAN "Parsing error..found incomplete boolean expression, at...\n%s <-*\n",
                   lexer.getSource().substr(0, lexer.getCurPosition()).c_str());
        }

        while (isComparisonOperator(curToken)) {
            advanceToken();
            expression();
        }
    }

    // statement ::= "WRITE" (expression || TK.StringLiteral) TK.Eos || etc...
    void statement() { // this is the meat of the parser

        // for print (STATEMENT)
        if (compareToCurToken(Write)) { //  System.out.println() statement
            std::cout << "(STATEMENT)-WRITE\n";
            advanceToken(); // the next token is a space, and it gets stuck


            // skip extra white space
            skipWhiteSpaces();

            // search for string literal, identifier, or  expression
            if (compareToCurToken(StringLiteral)) {
                advanceToken();
            } else if (compareToCurToken(Identifier)) {
                advanceToken();
            } else {
                expression(); // if we didn't find a string, we will want to print this expression
            }

            skipWhiteSpaces();
            // check for the final EOS statement
            //match(Eos);

            if (compareToCurToken(Eos)) {
                EOS();
            } else {
                printf(ANSI_COLOR_CYAN "Parsing error..expected <;> (update this with \\n characters later)\n");
                exit(35);
            }

        } else if (compareToCurToken(StringKW)) { // variable assignment of type string
            std::cout << "(STATEMENT)-VARIABLE_ASSIGNMENT_STRING";

            advanceToken();
            skipWhiteSpaces();

            match(Identifier);
            match(Eq);
            if (compareToCurToken(StringLiteral)) {
                std::cout << "..LITERAL\n";
                advanceToken();
                skipWhiteSpaces();
            } else if (compareToCurToken(Identifier)) {
                std::cout << "..VARIABLE\n";
                advanceToken();
                skipWhiteSpaces();
            } else {
                printf(ANSI_COLOR_CYAN "Parsing error..must assign literal of type <STRING> to identifier of type <STRING>\n");
                exit(35);
            }

            EOS();

        } else if (compareToCurToken(NumKW)) { // variable assignment of type int and float
            std::cout << "(STATEMENT)-VARIABLE_ASSIGNMENT_NUM";

            advanceToken();
            skipWhiteSpaces();

            match(Identifier);
            match(Eq);
            expression();
            EOS();

        } else if (compareToCurToken(Comment)) { // comments
            std::cout << "COMMENT\n";
            advanceToken();
            skipWhiteSpaces();


        } else if (compareToCurToken(If)) { // if statements
            std::cout << "(STATEMENT)-IF_CONDITION\n";
            advanceToken();
            skipWhiteSpaces();
            match(SqrbraceL);
            comparison();
            match(SqrbraceR);
            match(CrlbraceL);
            // execute all statements in the code block
            while (!compareToCurToken(CrlbraceR)) { // this kind of hurts my brain, but it works
                statement();
            }
            EOCB();

        } else if (compareToCurToken(While)) { // while loops
            std::cout << "(STATEMENT)-WHILE_LOOP\n";
            advanceToken();
            skipWhiteSpaces();
            match(SqrbraceL);
            comparison();
            match(SqrbraceR);
            match(CrlbraceL);
            // execute all statements in the code block
            while (!compareToCurToken(CrlbraceR)) {
                statement();
            }
            EOCB();
        } else if (compareToCurToken(For)) { // for loops
            std::cout << "(STATEMENT)-FOR_LOOP";

            advanceToken();
            skipWhiteSpaces();
            match(SqrbraceL);

            if (compareToCurToken(IntLiteral)) {
                std::cout << "..NUM_LITERAL\n";
                advanceToken();
                skipWhiteSpaces();
            } else if (compareToCurToken(Identifier)) {
                std::cout << "..VARIABLE\n";
                advanceToken();
                skipWhiteSpaces();
            }

            match(SqrbraceR);
            match(CrlbraceL);
            // execute all statements in the code block
            while (!compareToCurToken(CrlbraceR)) {
                statement();
            }
            EOCB();

        } else if (compareToCurToken(Input)) { // input statements
            std::cout << "(STATEMENT)-INPUT\n";
            advanceToken();
            skipWhiteSpaces();
            match(Identifier);
            EOS();
        }
            // variable re-instantiation // x = 5; as opposed to the already declared NUM x = 4;
        else if (compareToCurToken(Identifier)) {
            match(Identifier);
            match(Eq);

            // check to see if the variable has already been declared (should be, else return parsing error)
            if (!isUsedIdentifier(curToken.getTokenText())) {
                printf(ANSI_COLOR_CYAN "\nParsing error..referencing variable <%s> before assignment..line number: %d",
                       curToken.getTokenText().c_str(), lexer.getCurLineNumber());
                exit(35);
            }
            TokenType type = curToken.getType();

            // thinking about it, this will not work, and will probably need to be changed on account that it allows any data type to be assigned to any variable
            if (curToken.getType() == StringLiteral || curToken.getType() == IntLiteral ||
                curToken.getType() == FloatLiteral) {
                match(type);
                EOS();
            } else {
                //This should be a parsing error about assigning string to int or int to string
                //somehow need to check A) What type the variable is and B) if the literal matches that type
            }
        } else {
            printf(ANSI_COLOR_CYAN "\nParsing error..invalid statement on line: %d ...\n%s <-*",
                   lexer.getCurLineNumber(),
                   lexer.getSource().substr(0, lexer.getCurPosition() + 1).c_str());
            exit(36);
        }

    }


public:

// program ::= {statement}
    void program() {
        std::cout << "PROGRAM\n";

        skipWhiteSpaces();

        // parse through all given statements
        while (!(compareToCurToken(Eof))) {
            statement();
        }
        std::cout << "🌐 parsing complete\n";
    }


};


#endif //ATLAS_COMPILER_PARSER_H
