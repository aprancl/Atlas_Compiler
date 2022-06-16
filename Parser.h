//
// Created by Anthony Prancl on 5/29/22.
//

#ifndef ATLAS_COMPILER_PARSER_H
#define ATLAS_COMPILER_PARSER_H

#include "Lexer.h"
#include "Emitter.h"
#include <stdio.h>
#include <vector>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"


class Parser {


    Lexer lexer;
    Emitter emitter;
    Token farBackToken;
    Token lastToken;
    Token curToken;
    Token nextToken;
    std::vector<std::string> declared_vars;
    std::vector<std::vector<std::string>> varDict;
    int varDict_ptr;

public:
    // constructors

    Parser() = default;

    Parser(Lexer lexer, Emitter emitter) {
        varDict_ptr = 0;
        this->lexer = lexer;
        this->emitter = emitter;
        farBackToken = Token("", None);
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

        while (lexer.getCurChar() != ";" && lexer.getCurChar() != "\0") {

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

        farBackToken = lastToken;
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

    void docVar(Token variable) {

        std::string varText;

        if (lastToken.getType() == StringKW) {
            varText = "S_" + curToken.getTokenText();

        } else if (lastToken.getType() == NumKW) {
            if (isVarFloat() || farBackToken.getType() == Input) {
                varText = "F_" + curToken.getTokenText();
                emitter.emitHeader("float ");
            } else {
                varText = "I_" + curToken.getTokenText();
                emitter.emitHeader("int ");
            }
            emitter.emitHeaderLine(varText.substr(2) + ";");

        }

        varDict[varDict_ptr][0] = varText;


    }


    void docVal(Token value) {
        varDict[varDict_ptr][0] = value.getTokenText();
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
                // header append not necessary
            } else if (lastToken.getType() == NumKW) {
                if (isVarFloat() || farBackToken.getType() == Input) {
                    token_text = "F_" + curToken.getTokenText();
                    emitter.emitHeader("float ");
                } else {
                    token_text = "I_" + curToken.getTokenText();
                    emitter.emitHeader("int ");
                }
                emitter.emitHeaderLine(token_text.substr(2) + ";");
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

        if (!(compareToCurToken(Eos))) {
            printf(ANSI_COLOR_CYAN "%s\nParsing error..reached end of file before termination of statement..line number: %d",
                   (lexer.getSource().substr(0, lexer.getCurPosition()) + " <-*").c_str(), lexer.getCurLineNumber());
            exit(35);
        }

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
            emitter.emit(curToken.getTokenText());
            advanceToken();
            return;

        } else if (compareToCurToken(Identifier)) {

            if (isUsedIdentifier(curToken.getTokenText())) {
                emitter.emit(curToken.getTokenText());
                advanceToken();
                return;
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
            emitter.emit(" " + curToken.getTokenText() + " ");
            advanceToken();
        }
        primary();
    }

    void term() {
        std::cout << "TERM\n";
        unary();

        // there can be 0 or more mult OR div sub expressions
        while (compareToCurToken(Asterisk) || compareToCurToken(Fslash)) {
            emitter.emit(" " + curToken.getTokenText() + " ");
            advanceToken();
            unary();
        }
    }


    void expression() {

        std::cout << "..EXPRESSION\n";

        term();

        // there are 0 or more + OR - expression
        while (compareToCurToken(Plus) || compareToCurToken(Minus)) {
            emitter.emit(" " + curToken.getTokenText() + " ");
            advanceToken();
            term();
        }

    }

    void comparison() {
        std::cout << "=COMPARISON=\n";

        expression();

        if (isComparisonOperator(curToken)) {
            emitter.emit(" " + curToken.getTokenText() + " ");
            advanceToken();
            expression();
        } else {
            printf(ANSI_COLOR_CYAN "Parsing error..found incomplete boolean expression, at...\n%s <-*\n",
                   lexer.getSource().substr(0, lexer.getCurPosition()).c_str());
        }

        while (isComparisonOperator(curToken)) {
            emitter.emit(curToken.getTokenText());
            advanceToken();
            expression();
        }
    }

// statement ::= "WRITE" (expression || TK.StringLiteral) TK.Eos || etc...
    void statement() { // this is the meat of the parser

        // for print (STATEMENT)
        if (compareToCurToken(Write)) { //  System.out.println() statement
            std::cout << "(STATEMENT)-WRITE\n";
            advanceToken();

            // search for string literal, identifier, or  expression
            if (compareToCurToken(StringLiteral)) {
                emitter.emitLine((std::string) ("printf(\"") + curToken.getTokenText() + "\\n\");");
                advanceToken();
            } else if (compareToCurToken(Identifier) && getVarType(curToken) == StringLiteral) {
                emitter.emitLine((std::string) ("printf(\"%s\\n\",") + curToken.getTokenText() + ");");
                advanceToken();
            } else if (compareToCurToken(Identifier) || compareToCurToken(IntLiteral) ||
                       compareToCurToken(FloatLiteral)) {
                emitter.emit((std::string) ("printf(\"%") + ".2f\\n\", (float)(");
                expression();
                emitter.emitLine("));");
            } else {
                printf(ANSI_COLOR_CYAN "\nParsing error..only literals of type <STRING, INT, FLOAT> will print\n");
                exit(35);
            }


            EOS();


        } else if (compareToCurToken(StringKW)) { // variable assignment of type string

            std::string outSource;
            std::string strLen;

            std::cout << "(STATEMENT)-VARIABLE_ASSIGNMENT_STRING";
            advanceToken();


            if (compareToCurToken(Identifier) && isUsedIdentifier(curToken.getTokenText())) {
                printf(ANSI_COLOR_CYAN "\nParsing error..cannot redeclare instantiated variable <%s>..line number: %d",
                       curToken.getTokenText().c_str(),
                       lexer.getCurLineNumber());
                exit(35);
            }

            match(Identifier);
            outSource.append("char " + lastToken.getTokenText());
            match(Eq);

            if (compareToCurToken(StringLiteral)) {
                std::cout << "..LITERAL\n";

                strLen = std::to_string(curToken.getTokenText().length());
                outSource.append("[" + strLen + "] = ");
                outSource.append("\"" + curToken.getTokenText() + "\"");
                advanceToken();

            } else if (compareToCurToken(Identifier)) {
                std::cout << "..VARIABLE\n";
                advanceToken();
            } else {
                printf(ANSI_COLOR_CYAN "\nParsing error..must assign literal of type <STRING> to identifier of type <STRING>\n");
                exit(35);
            }


            EOS();
            outSource.append(";\n");
            emitter.emit(outSource);


        } else if (compareToCurToken(NumKW)) { // variable assignment of type int and float
            std::cout << "(STATEMENT)-VARIABLE_ASSIGNMENT_NUM";

            advanceToken();

            if (compareToCurToken(Identifier) && isUsedIdentifier(curToken.getTokenText())) {
                printf(ANSI_COLOR_CYAN "\nParsing error..cannot redeclare instantiated variable <%s>..line number: %d",
                       curToken.getTokenText().c_str(),
                       lexer.getCurLineNumber());
                exit(35);
            }

            match(Identifier);
            emitter.emit(lastToken.getTokenText() + " ");
            match(Eq);
            emitter.emit(lastToken.getTokenText() + " ");
            expression();
            EOS();
            emitter.emit(lastToken.getTokenText() + "\n");

        } else if (compareToCurToken(Comment)) { // comments
            std::cout << "COMMENT\n";
            emitter.emitLine("// " + curToken.getTokenText()); // prints to c.out without spaces - ruins readability
            advanceToken();


        } else if (compareToCurToken(If)) { // if statements
            std::cout << "(STATEMENT)-IF_CONDITION\n";
            emitter.emit("if (");
            advanceToken();

            match(SqrbraceL);
            comparison();
            match(SqrbraceR);
            match(CrlbraceL);
            emitter.emit(")\n{\n");

            // execute all statements in the code block
            while (!compareToCurToken(CrlbraceR)) { // this kind of hurts my brain, but it works
                statement();
            }
            EOCB();
            emitter.emit("\n}\n");

        } else if (compareToCurToken(While)) { // while loops
            std::cout << "(STATEMENT)-WHILE_LOOP\n";
            emitter.emit("while (");
            advanceToken();

            match(SqrbraceL);
            comparison();
            match(SqrbraceR);
            match(CrlbraceL);
            emitter.emit(")\n{\n");
            // execute all statements in the code block
            while (!compareToCurToken(CrlbraceR)) {
                statement();
            }
            emitter.emit("\n}\n");
            EOCB();

        } else if (compareToCurToken(For)) { // for loops
            std::string outSource;
            std::cout << "(STATEMENT)-FOR_LOOP";

            advanceToken();

            match(SqrbraceL);
            outSource.append("for ( ");

            if (compareToCurToken(IntLiteral)) {
                std::cout << "..NUM_LITERAL\n";
                outSource.append("int i = 0; i < " + curToken.getTokenText() + "; ++i)\n{\n");
                advanceToken();

            } else if (compareToCurToken(Identifier)) {
                std::cout << "..VARIABLE\n";

                std::string iterator;

                if (compareToCurToken(Identifier) && getVarType(curToken) == StringLiteral) {
                    iterator = std::to_string(curToken.getTokenText().length());
                } else {
                    iterator = curToken.getTokenText();
                }
                outSource.append("int i = 0; i < " + curToken.getTokenText() + "; ++i)\n{\n");

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
            std::cout << "(STATEMENT)-INPUT";
            std::string outSource;

            outSource.append("scanf(");
            advanceToken();

            std::string inputType;
            if (curToken.getType() == StringKW) {
                std::cout << "..STRING\n";
                inputType = "s";
            } else if (curToken.getType() == NumKW) {
                std::cout << "..NUM";
                inputType = "f";
            }
            advanceToken();
            outSource.append("\"%" + inputType + "\",");

            match(Identifier);
            outSource.append("&" + lastToken.getTokenText() + ")");
            outSource.append(curToken.getTokenText() + "\n");
            EOS();
            emitter.emit(outSource);

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
                emitter.emit(curToken.getTokenText());
                advanceToken();
            } else {
                printf(ANSI_COLOR_CYAN "\nParsing error..invalid statement on line: %d ...\n%s<-*",
                       lexer.getCurLineNumber(),
                       lexer.getSource().substr(0, lexer.getCurPosition() + 1).c_str());
                exit(36);
            }

            match(Eq); // advance
            emitter.emit(" " + lastToken.getTokenText() + " ");

            // get literal type
            TokenType literalType = curToken.getType();


            if (variableType == literalType || literalType == Identifier) {
                if (!(literalType == StringLiteral)) {
                    expression();
                } else if (literalType == StringLiteral) {
                    match(literalType);
                }
                emitter.emit(curToken.getTokenText() + "\n");
                EOS();


            } else {
                printf(ANSI_COLOR_CYAN "Parsing error..variable type <%s> and literal type <%s> do not match",
                       Token::typeToString(variableType).c_str(), Token::typeToString(literalType).c_str());
                exit(35);
            }

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

        emitter.emitLine("\nreturn 0;");
        emitter.emitLine("}");
        emitter.writeFile();
        std::cout << "🌐 parsing complete\n";
    }


};


#endif //ATLAS_COMPILER_PARSER_H
