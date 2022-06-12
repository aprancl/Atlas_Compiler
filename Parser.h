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
                if (var.substr(2) == tokenText) {
                    return 1;
                }
            }
            return 0;

        }

    }


    TokenType getVarType(Token variable) {

        if (variable.getType() == Identifier) {

            for (std::string dec_var: declared_vars) {

                if (dec_var.substr(2) == variable.getTokenText()) {
                    return (dec_var[0] == 'S') ? StringLiteral : (dec_var[0] == 'I') ? IntLiteral : FloatLiteral;
                }

            }

            printf(ANSI_COLOR_CYAN"\nParsing error..variable either of unknown type or is repeat..idk bro, this should never print\n");
            exit(-1);


        } else {
            // error of type referencing variable <var> before declaration
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

    bool isVarFloat() {

        int startPosition = lexer.getCurPosition();

        while (lexer.getCurChar() != ";") {

            if (lexer.getCurChar() == "f" && lexer.getPeek() == "l") {
                lexer.setCurPostion(startPosition);
                lexer.setCurChar(lexer.getSource().substr(lexer.getCurPosition(), 1));
                return 1;
            }

            lexer.nextChar();

        }

        lexer.setCurPostion(startPosition);
        lexer.setCurChar(lexer.getSource().substr(lexer.getCurPosition(), 1));
        return 0;


    }

    // compare a given token to what the current token is
    bool compareToLastToken(TokenType type) {
        return (type == this->lastToken.getType());
    }

    bool compareToCurToken(TokenType type) { //             used to check the first token of each statement
        return (type == this->curToken.getType());
    }

    bool compareToNextToken(TokenType type) {
        return (type == this->nextToken.getType());
    }


    void advanceToken() {

        lastToken = curToken;
        curToken = nextToken;

        // potential next token
        Token nToken = lexer.getToken();
        // filter out all spaces and newlines
        while (nToken.getType() == Space || nToken.getType() == NewLine) {
            nToken = lexer.getToken();
        }
        nextToken = nToken;
        int x = 4;
    }

    void match(TokenType type) {//                      used to check tokens within a statement
        if (!(compareToCurToken(type))) {
            printf(ANSI_COLOR_CYAN "\nParsing error..expected <%s> but got <%s> ..line number: %d\n",
                   Token::typeToString(type).c_str(),
                   Token::typeToString(this->curToken.getType()).c_str(), lexer.getCurLineNumber());
            exit(35); //  stop program && parsing
        } else if (compareToCurToken(Identifier)) {

            // if we are declaring a variable, we want to document it and its data type
            std::string token_text;
            if (lastToken.getType() == StringKW) {
                token_text = "S_" + curToken.getTokenText();
            } else if (lastToken.getType() == NumKW) {
                if (isVarFloat()) {
                    token_text = "F_" + curToken.getTokenText();
                } else {
                    token_text = "I_" + curToken.getTokenText();
                }
            }
            declared_vars.push_back(token_text); // the variable is finally documented
        }
        advanceToken();
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


        printf("PRIMARY [%s -> %s]\n", Token::typeToString(curToken.getType()).c_str(),
               curToken.getTokenText().c_str());

        if (compareToCurToken(IntLiteral) || compareToCurToken(FloatLiteral)) {
            advanceToken();

        } else if (compareToCurToken(Identifier)) {

            if (isUsedIdentifier(curToken.getTokenText())) {
                advanceToken();
            } else {
                // want to come back to this
                printf(ANSI_COLOR_CYAN "\nParsing error..referencing undeclared variable <%s> before assignment..line number: %d\n",
                       curToken.getTokenText().c_str(), lexer.getCurLineNumber());
                exit(35);
            }


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


            // search for string literal, identifier, or  expression
            if (compareToCurToken(StringLiteral)) {
                advanceToken();
            } else if (compareToCurToken(Identifier)) {
                advanceToken();
            } else {
                expression(); // if we didn't find a string, we will want to print this expression
            }


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

            if (compareToCurToken(Identifier) && isUsedIdentifier(curToken.getTokenText())) {
                printf(ANSI_COLOR_CYAN "\nParsing error..cannot redeclare instantiated variable <%s>..line number: %d",
                       curToken.getTokenText().c_str(),
                       lexer.getCurLineNumber());
                exit(35);
            }

            match(Identifier);
            match(Eq);

            if (compareToCurToken(StringLiteral)) {
                std::cout << "..LITERAL\n";
                advanceToken();

            } else if (compareToCurToken(Identifier)) {
                std::cout << "..VARIABLE\n";
                advanceToken();
            } else {
                printf(ANSI_COLOR_CYAN "\nParsing error..must assign literal of type <STRING> to identifier of type <STRING>\n");
                exit(35);
            }


            EOS();

        } else if (compareToCurToken(NumKW)) { // variable assignment of type int and float
            std::cout << "(STATEMENT)-VARIABLE_ASSIGNMENT_NUM";

            advanceToken();

            if (compareToCurToken(Identifier) && isUsedIdentifier(curToken.getTokenText())) {
                printf(ANSI_COLOR_CYAN "\nParsing error..cannot redeclare instantiated variable <%s>..line number: %d",
                       curToken.getTokenText().c_str(),
                       lexer.getCurLineNumber());
                exit(35);
            }

            int x = 5;
            match(Identifier);
            match(Eq);
            expression();
            EOS();

        } else if (compareToCurToken(Comment)) { // comments
            std::cout << "COMMENT\n";
            advanceToken();


        } else if (compareToCurToken(If)) { // if statements
            std::cout << "(STATEMENT)-IF_CONDITION\n";
            advanceToken();

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

            match(SqrbraceL);

            if (compareToCurToken(IntLiteral)) {
                std::cout << "..NUM_LITERAL\n";
                advanceToken();

            } else if (compareToCurToken(Identifier)) {
                std::cout << "..VARIABLE\n";
                advanceToken();

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

            match(Identifier);
            EOS();
        }
            // variable re-instantiation // x = 5; as opposed to the already declared NUM x = 4;
        else if (compareToCurToken(Identifier)) {
            std::cout << "(STATEMENT)-VARIABLE_RE-INSTANTIATION\n";

            // check to see if the variable has already been declared (should be, else return parsing error)
            if (!isUsedIdentifier(curToken.getTokenText())) {
                printf(ANSI_COLOR_CYAN "\nParsing error..referencing variable <%s> before assignment..line number: %d",
                       curToken.getTokenText().c_str(), lexer.getCurLineNumber());
                exit(35);
            }
            // get variable type

            TokenType variableType = getVarType(curToken);

//            match(Identifier); // advance

            if (compareToCurToken(Identifier)) {
                advanceToken();
            } else {
                printf(ANSI_COLOR_CYAN "\nParsing error..invalid statement on line: %d ...\n%s<-*",
                       lexer.getCurLineNumber(),
                       lexer.getSource().substr(0, lexer.getCurPosition() + 1).c_str());
                exit(36);
            }

            match(Eq); // advance

            // get literal type
            TokenType literalType = curToken.getType();


            if (variableType == literalType || literalType == Identifier) {
                if (!(literalType == StringLiteral)) {
                    expression();
                } else if (literalType == StringLiteral) {
                    match(literalType);
                }
                EOS();

            } else {
                printf(ANSI_COLOR_CYAN "Parsing error..variable type <%s> and literal type <%s> do not match",
                       Token::typeToString(variableType).c_str(), Token::typeToString(literalType).c_str());
                exit(35);
            }


//            // .... if they match allow, else do not
//
//            // thinking about it, this will not work, and will probably need to be changed on account that it allows any data type to be assigned to any variable
//            if (curToken.getType() == StringLiteral || curToken.getType() == IntLiteral ||
//                curToken.getType() == FloatLiteral) {
//                match(literalType);
//                EOS();
//            } else {
//                //This should be a parsing error about assigning string to int or int to string
//                //somehow need to check A) What type the variable is and B) if the literal matches that type
//            }
        } else {
            printf(ANSI_COLOR_CYAN "\nParsing error..invalid statement on line: %d ...\n%s<-*",
                   lexer.getCurLineNumber(),
                   lexer.getSource().substr(0, lexer.getCurPosition() + 1).c_str());
            exit(36);
        }

    }


public:

// program ::= {statement}
    void program() {
        std::cout << "PROGRAM\n";



        // parse through all given statements
        while (!(compareToCurToken(Eof))) {
            statement();
        }
        int x = 4; // debug marker
        std::cout << "🌐 parsing complete\n";
    }


};


#endif //ATLAS_COMPILER_PARSER_H
