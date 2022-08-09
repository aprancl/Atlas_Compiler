//
// Created by Anthony Prancl on 5/29/22.
//

#ifndef ATLAS_COMPILER_PARSER_H
#define ATLAS_COMPILER_PARSER_H

#include "Lexer.h"
#include "Emitter.h"
#include "Variable.h"
#include <stdio.h>
#include <vector>
#include <map>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"

class Parser {

    // data members
    Lexer lexer;
    Emitter emitter;
    Token farBackToken;
    Token lastToken;
    Token curToken;
    Token nextToken;
    std::vector<Variable> variables; // hopefully a sufficient solution to my plights

public:
    // constructors

    Parser() = default;

    Parser(Lexer lexer, Emitter emitter) {
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


    // function related methods

    std::string readFuncName() {
        std::cout << "(FUNC)-reading name\n";
        std::string name;
        lexer.setToStartOfName();

        while (lexer.getCurChar() != "(") {

            if (!(lexer.getCurChar() == " ")) {
                name.append(lexer.getCurChar());
            }
            lexer.nextChar();
        }
        return name;

    }

    std::map<std::string, std::string> readLocalVars() {

        std::cout << "(FUNC)-reading local vars\n";

        // name, literal type
        std::map<std::string, std::string> data;

        lexer.nextChar(); // skip first '('
        while (lexer.getCurChar() != ")") {

            if (lexer.getCurChar() == " ") {
                lexer.nextChar();
                continue; // skip white space
            }

            // scan var name
            std::string varName;
            while (lexer.getCurChar() != ":") { // need to set so i have to close it with a comma or error
                varName.append(lexer.getCurChar());
                lexer.nextChar();
            }
            lexer.nextChar();

            // scan var token type
            std::string literalType;
            while (lexer.getCurChar() != ",") { // need to set so i have to close it with a comma or error
                literalType.append(lexer.getCurChar());
                lexer.nextChar();
            }
            lexer.nextChar();

            // write scanned data
            data[varName] = (literalType == "NUM") ? "float" : "char*";


        }

        return data;

    }


    std::string readReturnType() {

        std::cout << "(FUNC)-reading return type\n";
        std::string returnType;
        // skip to dollar sign
        while (lexer.getCurChar() != "$") {
            lexer.nextChar();
        }
        lexer.nextChar();

        // read string and save to var
        while (lexer.getCurChar() != "{") {

            if (lexer.getCurChar() == " ") {
                lexer.nextChar();
                continue;
            }

            returnType.append(lexer.getCurChar());
            lexer.nextChar();

        }

        return (returnType == "NUM") ? "float" : (returnType == "STRING") ? "char*" : "void";


    }


    Variable findVarByName(std::string name) {

        for (int i = 0; i < variables.size(); ++i) {

            std::string decVarName = variables[i].getName();

            if (decVarName == name) {
                return variables[i];
            }

        }

    }

    Variable *findVarPtrByName(std::string name) {

        for (int i = 0; i < variables.size(); ++i) {

            std::string decVarName = variables[i].getName();

            if (decVarName == name) {
                return &variables[i];
            }

        }

    }

    bool isUsedIdentifier(std::string varName) {

        if (variables.size() == 0) {
            return 0;

        } else {
            for (int i = 0; i < variables.size(); ++i) {

                std::string decVarName = variables[i].getName();

                if (varName == decVarName) {
                    return 1;
                }
            }
            return 0;
        }

    }

    bool isVarFloat() {

        int startPosition = lexer.getCurPosition();
        lexer.setToLastEq();

        while (lexer.getCurChar() != ";" && lexer.getCurChar() != "\0") {

            // if the value following the equals is explicitly a float or any type of expression: return true
            if ((lexer.getCurChar() == "f" && lexer.getPeek() == "l") ||
                (lexer.getCurChar() == "+" || lexer.getCurChar() == "-" || lexer.getCurChar() == "/" ||
                 lexer.getCurChar() == "*")) {
                lexer.setCurPosition(startPosition);
                lexer.setCurChar(lexer.getSource().substr(lexer.getCurPosition(), 1));
                return 1;
            }

            lexer.nextChar();

        }

        lexer.setCurPosition(startPosition);
        lexer.setCurChar(lexer.getSource().substr(lexer.getCurPosition(), 1));
        return 0;


    }

    std::string readExpression() {

        std::string expression;
        int startPosition = lexer.getCurPosition();
        lexer.setToLastEq();
        lexer.nextChar();

        while (lexer.getCurChar() != ";" && lexer.getCurChar() != "\0") {

            if (lexer.getCurChar() == " ") {
                lexer.nextChar();
                continue;
            }
            expression.append(lexer.getCurChar());
            lexer.nextChar();

        }

        lexer.setCurPosition(startPosition);
        lexer.setCurChar(lexer.getSource().substr(lexer.getCurPosition(), 1));
        return expression;
    }


    TokenType findIdentType(Token identifier) {

        if (isUsedIdentifier(identifier.getTokenText())) {

            std::string varName = identifier.getTokenText();
            for (int i = 0; i < variables.size(); ++i) {
                if (identifier.getTokenText() == varName) {
                    return variables[i].getDataType();
                }
            }

        } else {
            printf(ANSI_COLOR_CYAN"\nParsing error..variable <%s> referenced before definition\n",
                   identifier.getTokenText().c_str());
            exit(35);
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


    void clrFunctionvars() {

        for (int i = 0; i < variables.size(); ++i) {
            Variable ithVar = variables[i];

            if (ithVar.getIsFuncVar()){
                variables.erase(variables.begin() + i);
            }

        }
    }

    void parseFunction(bool isFuncStatement) {

        match(Identifier);
        std::string funcName = lastToken.getTokenText();

        match(ParenthL);

        // read the given variables
        if (compareToCurToken(ParenthR)) {
            advanceToken();

            if (!isFuncStatement) {
                emitter.emit(funcName + "(" + ");\n");
            } else {
                emitter.emitToUserFuncDefs(funcName + "(" + ");\n");
            }
            EOS();
            return;
        }
        std::string vars;
        while (curToken.getType() != Comma || nextToken.getType() != ParenthR) {

            std::string literal = (curToken.getType() == StringLiteral) ? "\"" + curToken.getTokenText() + "\""
                                                                        : curToken.getTokenText();

            vars.append(literal);
            advanceToken();
        }
        advanceToken();
        advanceToken();

        if (!isFuncStatement) {
            emitter.emit(funcName + "(" + vars + ");\n");
        } else {
            emitter.emitToUserFuncDefs(funcName + "(" + vars + ");\n");
        }
        EOS();

    }

    void parseFunctionBland(bool isFuncStatement) {

        match(Identifier);
        std::string funcName = lastToken.getTokenText();

        match(ParenthL);

        // read the given variables
        if (compareToCurToken(ParenthR)) {
            advanceToken();

            if (!isFuncStatement) {
                emitter.emit(funcName + "(" + ")");
            } else {
                emitter.emitToUserFuncDefs(funcName + "(" + ")");
            }
            EOS();
            return;
        }
        std::string vars;
        while (curToken.getType() != Comma || nextToken.getType() != ParenthR) {

            std::string literal = (curToken.getType() == StringLiteral) ? "\"" + curToken.getTokenText() + "\""
                                                                        : curToken.getTokenText();

            vars.append(literal);
            advanceToken();
        }
        advanceToken();
        advanceToken();

        if (!isFuncStatement) {
            emitter.emit(funcName + "(" + vars + ")");
        } else {
            emitter.emitToUserFuncDefs(funcName + "(" + vars + ")");
        }
        EOS();
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
    }


    void match(TokenType type) {//                      used to check tokens within a statement
        if (!(compareToCurToken(type))) {
            printf(ANSI_COLOR_CYAN "\nParsing error..expected <%s> but got <%s> ..line number: %d\n",
                   Token::typeToString(type).c_str(),
                   Token::typeToString(this->curToken.getType()).c_str(), lexer.getCurLineNumber());
            exit(35); //  stop program && parsing
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

    void primary(bool isFuncStatement) {


        printf("PRIMARY [%s -> %s]\n", Token::typeToString(curToken.getType()).c_str(),
               curToken.getTokenText().c_str());

        if (compareToCurToken(IntLiteral) || compareToCurToken(FloatLiteral)) {
            if (!isFuncStatement) {
                emitter.emit(curToken.getTokenText());
            } else {
                emitter.emitToUserFuncDefs(curToken.getTokenText());
            }
            advanceToken();
            return;

        } else if (compareToCurToken(Identifier)) {

            // nvm this is the line that is messing me up
            if (isUsedIdentifier(curToken.getTokenText())) {
                if (!isFuncStatement) {
                    emitter.emit(curToken.getTokenText());
                } else {
                    emitter.emitToUserFuncDefs(curToken.getTokenText());
                }
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

    void unary(bool isFuncStatement) {
        std::cout << "UNARY\n";

        if (compareToCurToken(Plus) || compareToCurToken(Minus)) {
            if (!isFuncStatement) {
                emitter.emit(" " + curToken.getTokenText() + " ");
            } else {
                emitter.emitToUserFuncDefs(" " + curToken.getTokenText() + " ");
            }
            advanceToken();
        }
        primary(isFuncStatement);
    }

    void term(bool isFuncStatement) {
        std::cout << "TERM\n";
        unary(isFuncStatement);

        // there can be 0 or more mult OR div sub expressions
        while (compareToCurToken(Asterisk) || compareToCurToken(Fslash)) {
            if (!isFuncStatement) {
                emitter.emit(" " + curToken.getTokenText() + " ");
            } else {
                emitter.emitToUserFuncDefs(" " + curToken.getTokenText() + " ");
            }
            advanceToken();
            unary(isFuncStatement);
        }
    }


    void expression(bool isFuncSatement) {

        std::cout << "..EXPRESSION\n";

        term(isFuncSatement);

        // there are 0 or more + OR - expression
        while (compareToCurToken(Plus) || compareToCurToken(Minus)) {
            if (!isFuncSatement) {
                emitter.emit(" " + curToken.getTokenText() + " ");
            } else {
                emitter.emitToUserFuncDefs(" " + curToken.getTokenText() + " ");
            }
            advanceToken();
            term(isFuncSatement);
        }

    }

    void comparison(bool isFuncStatement) {
        std::cout << "=COMPARISON=\n";

        expression(isFuncStatement);

        if (isComparisonOperator(curToken)) {
            if (!isFuncStatement) {
                emitter.emit(" " + curToken.getTokenText() + " ");
            } else {
                emitter.emitToUserFuncDefs(" " + curToken.getTokenText() + " ");
            }
            advanceToken();
            expression(isFuncStatement);
        } else {
            printf(ANSI_COLOR_CYAN "Parsing error..found incomplete boolean expression, at...\n%s <-*\n",
                   lexer.getSource().substr(0, lexer.getCurPosition()).c_str());
        }

        while (isComparisonOperator(curToken)) {
            if (!isFuncStatement) {
                emitter.emit(curToken.getTokenText());
            } else {
                emitter.emitToUserFuncDefs(curToken.getTokenText());
            }
            advanceToken();
            expression(isFuncStatement);
        }
    }

    void statement(bool isFuncStatement) { // this is the meat of the parser
        // the boolean var input is used to determine where to emit the code. different instances of statement parsing will have different destinations in the output file

        // for print (STATEMENT)
        if (compareToCurToken(Write)) { // print statement
            std::cout << "(STATEMENT)-WRITE\n";
            advanceToken();

            // search for string literal, identifier, expression, or function call (of STRING return type)
            // only these four things are allowed to compile and print, all else will cause break in compiler
            if (compareToCurToken(StringLiteral)) {

                if (!isFuncStatement) {
                    emitter.emitLine((std::string) ("printf(\"") + curToken.getTokenText() + "\\n\");");
                } else {
                    emitter.emitToUserFuncDefs((std::string) ("printf(\"") + curToken.getTokenText() + "\\n\");\n");
                }
                advanceToken();
            } else if (compareToCurToken(Identifier) && findIdentType(curToken) == StringLiteral) {
                if (!isFuncStatement) {
                    emitter.emitLine((std::string) ("printf(\"%s\\n\",") + curToken.getTokenText() + ");");
                } else {
                    emitter.emitToUserFuncDefs((std::string) ("printf(\"%s\\n\",") + curToken.getTokenText() + ");\n");
                }
                advanceToken();
            } else if (compareToCurToken(Identifier) || compareToCurToken(IntLiteral) ||
                       compareToCurToken(FloatLiteral)) {

                if (!isFuncStatement) {
                    emitter.emit((std::string) ("printf(\"%") + ".2f\\n\", (float)(");
                    expression(isFuncStatement);
                    emitter.emitLine("));");
                } else {
                    emitter.emitToUserFuncDefs((std::string) ("printf(\"%") + ".2f\\n\", (float)(");
                    expression(isFuncStatement);
                    emitter.emitToUserFuncDefs("));\n");
                }
            } else if (compareToCurToken(At)) {
                advanceToken();
                if (!isFuncStatement) {
                    emitter.emit("printf(\"%s\\n\", ");
                } else {
                    emitter.emitToUserFuncDefs("printf(\"%s\\n\", ");
                }
                parseFunctionBland(isFuncStatement);

                if (!isFuncStatement) {
                    emitter.emit(");\n");
                } else {
                    emitter.emitToUserFuncDefs(");\n");
                }
                return;
            } else {
                printf(ANSI_COLOR_CYAN "\nParsing error..only literals of type <STRING, INT, FLOAT> will print\n");
                exit(35);
            }


            EOS();


        } else if (compareToCurToken(StringKW)) { // variable assignment of type string
            std::cout << "(STATEMENT)-VARIABLE_ASSIGNMENT_STRING";

            // Source to be emitted at end of statement
            std::string outSource;
            advanceToken();

            if (compareToCurToken(Identifier) && isUsedIdentifier(curToken.getTokenText())) {
                printf(ANSI_COLOR_CYAN "\nParsing error..cannot redeclare instantiated variable <%s>..line number: %d",
                       curToken.getTokenText().c_str(),
                       lexer.getCurLineNumber());
                exit(35);
            }

            match(Identifier);
            outSource.append("char *" + lastToken.getTokenText());
            match(Eq);

            // search for string literal, string variable, or function call (STRING return type)
            if (compareToCurToken(StringLiteral)) {
                std::cout << "..LITERAL\n"; // header

                // new way of documenting var
                Variable str(farBackToken.getTokenText(), curToken.getTokenText(), StringLiteral, isFuncStatement);
                variables.push_back(str);
                // ---

                outSource.append(" = \"" + curToken.getTokenText() + "\\0\"");

            } else if (compareToCurToken(Identifier)) {
                std::cout << "..VARIABLE\n"; // header

                // new way of documenting var
                Variable str(farBackToken.getTokenText(), findVarByName(curToken.getTokenText()), isFuncStatement);
                variables.push_back(str);
                // ---
                outSource.append(" = \"" + str.getValue() + "\\0\"");
            } else if (compareToCurToken(At)) {
                // document variable for later
                int startIdx = lexer.getCurPosition(); //read the function call without throwing off lexer
                std::string startChar = lexer.getCurChar();
                std::string value = readExpression();
                lexer.setCurPosition(startIdx);
                lexer.setCurChar(startChar);

                Variable str (farBackToken.getTokenText(), value, StringLiteral, isFuncStatement);
                variables.push_back(str);
                advanceToken();
                // clear placeholder variables // clrGarbageVars

                if (!isFuncStatement){
                    emitter.emit("char *" + str.getName() + " = ");
                } else {

                    emitter.emitToUserFuncDefs("char *" + str.getName() + " = ");
                }
                parseFunction(isFuncStatement);
                return;


            } else {
                printf(ANSI_COLOR_CYAN "\nParsing error..must assign literal of type <STRING> to identifier of type <STRING>\n");
                exit(35);
            }
            advanceToken();


            EOS();
            outSource.append(";\n");
            if (!isFuncStatement) {
                emitter.emit(outSource);
            } else {
                emitter.emitToUserFuncDefs(outSource);
            }


        } else if (compareToCurToken(NumKW)) { // variable assignment of type int and float
            std::cout << "(STATEMENT)-VARIABLE_ASSIGNMENT_NUM"; // header
            advanceToken();
            std::string outSource;

            // check whether is used identifier && take appropriate action
            std::string identName;
            if (compareToCurToken(Identifier) && isUsedIdentifier(curToken.getTokenText())) {
                printf(ANSI_COLOR_CYAN "\nParsing error..cannot redeclare instantiated variable <%s>..line number: %d",
                       curToken.getTokenText().c_str(),
                       lexer.getCurLineNumber());
                exit(35);
            } else if (compareToCurToken(Identifier)) {
                identName = curToken.getTokenText();
                advanceToken();
                outSource.append(lastToken.getTokenText() + " ");
            }

            // check '='
            match(Eq);
            outSource.append("= ");

            // emit && document data type and value
            std::string identVal;
            TokenType identType;
            if (isVarFloat()) {
                outSource.insert(0, "float ");
                identType = FloatLiteral;
            } else {
                outSource.insert(0, "int ");
                identType = IntLiteral;
            }

            // this is the line that stops the function doMath()
            identVal = readExpression(); // if this is a float literal, I need to somehow get rid of the fl prefix

            // make and save variable object
            Variable variable(identName, identVal, identType, isFuncStatement);
            variables.push_back(variable);

            if (!isFuncStatement) {
                emitter.emit(outSource);
                expression(isFuncStatement);
                emitter.emit(";\n");
            } else {
                emitter.emitToUserFuncDefs(outSource);
                expression(isFuncStatement);
                emitter.emitToUserFuncDefs(";\n");
            }

            EOS();

        } else if (compareToCurToken(Comment)) { // comments
            std::cout << "COMMENT\n";

            if (!isFuncStatement) {
                emitter.emitLine("// " + curToken.getTokenText()); // prints to c.out without spaces - ruins readability
            } else {
                emitter.emitToUserFuncDefs(
                        "// " + curToken.getTokenText() + "\n"); // prints to c.out without spaces - ruins readability
            }
            advanceToken();


        } else if (compareToCurToken(If)) { // if statements
            std::cout << "(STATEMENT)-IF_CONDITION\n";

            if (!isFuncStatement) {
                emitter.emit("if (");
            } else {
                emitter.emitToUserFuncDefs("if (");
            }
            advanceToken();

            match(SqrbraceL);
            comparison(isFuncStatement);
            match(SqrbraceR);
            match(CrlbraceL);
            if (!isFuncStatement) {
                emitter.emit(")\n{\n");
            } else {
                emitter.emitToUserFuncDefs(")\n{\n");
            }

            // execute all statements in the code block
            while (!compareToCurToken(CrlbraceR)) { // this kind of hurts my brain, but it works
                statement(isFuncStatement);
            }
            EOCB();

            if (!isFuncStatement) {
                emitter.emit("\n}\n");
            } else {
                emitter.emitToUserFuncDefs("\n}\n");
            }

        } else if (compareToCurToken(While)) { // while loops
            std::cout << "(STATEMENT)-WHILE_LOOP\n";

            if (!isFuncStatement) {
                emitter.emit("while (");
            } else {
                emitter.emitToUserFuncDefs("while (");
            }
            advanceToken();

            match(SqrbraceL);
            comparison(isFuncStatement);
            match(SqrbraceR);
            match(CrlbraceL);
            if (!isFuncStatement) {
                emitter.emit(")\n{\n");
            } else {
                emitter.emitToUserFuncDefs(")\n{\n");
            }
            // execute all statements in the code block
            while (!compareToCurToken(CrlbraceR)) {
                statement(isFuncStatement);
            }

            if (!isFuncStatement) {
                emitter.emit("\n}\n");
            } else {
                emitter.emitToUserFuncDefs("\n}\n");
            }
            EOCB();

        } else if (compareToCurToken(For)) { // for loops
            std::cout << "(STATEMENT)-FOR_LOOP";

            advanceToken();

            match(SqrbraceL);

            if (!isFuncStatement) {
                emitter.emit("for ( ");
            } else {
                emitter.emitToUserFuncDefs("for ( ");
            }

            if (compareToCurToken(IntLiteral) || (compareToCurToken(Identifier) &&
                                                  (findIdentType(curToken) == IntLiteral ||
                                                   findIdentType(curToken) == FloatLiteral))) {
                std::cout << "..NUM_ || FLOAT_\n";

                if (!isFuncStatement) {
                    emitter.emit("int i = 0; i < " + curToken.getTokenText() + "; ++i)\n{\n");
                } else {
                    emitter.emitToUserFuncDefs("int i = 0; i < " + curToken.getTokenText() + "; ++i)\n{\n");
                }
                advanceToken();

            } else if (compareToCurToken(StringLiteral) ||
                       compareToCurToken(Identifier) && findIdentType(curToken) == StringLiteral) {
                std::cout << "..String_LITERAL || _VARIABLE\n";

                std::string outSource = (compareToCurToken(StringLiteral)) ? "int i = 0; i < sizeof(\"" +
                                                                             curToken.getTokenText() + "\"); ++i)\n{\n"
                                                                           : "int i = 0; i < sizeof(" +
                                                                             curToken.getTokenText() + "); ++i)\n{\n";

                if (!isFuncStatement) {
                    emitter.emit(outSource);
                } else {
                    emitter.emitToUserFuncDefs(outSource);
                }
                advanceToken();

            }

            match(SqrbraceR);
            match(CrlbraceL);
            // execute all statements in the code block
            while (!compareToCurToken(CrlbraceR)) {
                statement(isFuncStatement);
            }
            EOCB();

            if (!isFuncStatement) {
                emitter.emit("\n}\n");
            } else {
                emitter.emitToUserFuncDefs("\n}\n");
            }


        } else if (compareToCurToken(Input)) { // input statements
            std::cout << "(STATEMENT)-INPUT"; // header

            std::string outSource;
            advanceToken();

            if (curToken.getType() == StringKW) {
                std::cout << "..STRING\n";

                // declare the string in the c.out header
                std::string varName = nextToken.getTokenText();

                if (!isFuncStatement) {

                    emitter.emitHeader("char *" + varName + " = malloc(sizeof(" + varName + "));\n");

                    // emit info
                    emitter.emit("scanf(\"%s\", " + varName + ");\n");
                } else {

                    emitter.emitToUserFuncDefs("char *" + varName + " = malloc(sizeof(" + varName + "));\n");

                    // emit info
                    emitter.emitToUserFuncDefs("scanf(\"%s\", " + varName + ");\n");
                }


                // make variable
                Variable variable(varName, varName, StringLiteral, isFuncStatement);
                variables.push_back(variable);

            } else if (curToken.getType() == NumKW) {
                std::cout << "..NUM\n"; // subheader

                // declare the string in the c.out header
                std::string varName = nextToken.getTokenText();

                if (!isFuncStatement) {

                    emitter.emitHeader("float " + varName + ";\n");

                    // emit info
                    emitter.emit("scanf(\"%f\", &" + varName + ");\n");
                } else {

                    emitter.emitToUserFuncDefs("float " + varName + ";\n");

                    // emit info
                    emitter.emitToUserFuncDefs("scanf(\"%f\", &" + varName + ");\n");
                }


                // make variable
                Variable variable(varName, varName, FloatLiteral, isFuncStatement);
                variables.push_back(variable);

            }
            advanceToken();

            match(Identifier);


            EOS();

            if (!isFuncStatement) {
                emitter.emit(outSource);
            } else {
                emitter.emitToUserFuncDefs(outSource);
            }


        }
            // variable re-instantiation // x = 5; as opposed to the already declared NUM x = 4;
        else if (compareToCurToken(Identifier)) {
            std::cout << "(STATEMENT)-VARIABLE_RE-INSTANTIATION\n"; // header

            // check to see if the variable has already been declared (should be, else return parsing error)
            if (!isUsedIdentifier(curToken.getTokenText())) {
                printf(ANSI_COLOR_CYAN "\nParsing error..referencing variable <%s> before assignment..line number: %d",
                       curToken.getTokenText().c_str(), lexer.getCurLineNumber());
                exit(35);
            }

            // get the variable that needs to be manipulated and save another copy for comparison
            Variable originVar = findVarByName(curToken.getTokenText());
            Variable *varPtr = findVarPtrByName(curToken.getTokenText());
            advanceToken();

            match(Eq);

            // potential things found at end of "="
            // string literal
            if (curToken.getType() == StringLiteral && originVar.getDataType() == StringLiteral) {
                varPtr->setValue(curToken.getTokenText());

                if (!isFuncStatement) {
                    emitter.emit(originVar.getName() + " = \"" + varPtr->getValue() + "\\0\"");
                } else {
                    emitter.emitToUserFuncDefs(originVar.getName() + " = \"" + varPtr->getValue() + "\\0\"");
                }
                advanceToken();
            }
                // string variable
            else if (curToken.getType() == Identifier && varPtr->getDataType() == StringLiteral &&
                     originVar.getDataType() == StringLiteral) {

                varPtr->setPtrVar(findVarByName(curToken.getTokenText()));

                if (!isFuncStatement) {
                    emitter.emit(originVar.getName() + " = \"" + varPtr->getValue() + "\"");
                } else {
                    emitter.emitToUserFuncDefs(originVar.getName() + " = \"" + varPtr->getValue() + "\"");
                }
                advanceToken();
            } else {
                varPtr->setValue(readExpression());

                if (!isFuncStatement) {
                    emitter.emit(originVar.getName() + " = ");
                } else {
                    emitter.emitToUserFuncDefs(originVar.getName() + " = ");
                }
                std::string exprVal = readExpression();

                expression(isFuncStatement);
            }

            if (!isFuncStatement) {
                emitter.emit(";\n");
            } else {
                emitter.emitToUserFuncDefs(";\n");
            }
            EOS();

        } else if (compareToCurToken(Define)) { // defining functions
            std::cout << "(FUNC DEF)-Defining a function\n"; // header

            std::string funcName = readFuncName();
            std::map<std::string, std::string> localVars = readLocalVars(); // varName, varVal
            std::string returnType = readReturnType();

            // save local vars to memory

            std::map<std::string, std::string>::iterator it1;
            for (it1 = localVars.begin(); it1 != localVars.end(); it1++) {
                Variable var(it1->first, "",
                             (it1->second == "float") ? FloatLiteral : StringLiteral, true); // name, value, type
                variables.push_back(var);
            }


            // emit the function header
            std::string outSource;
            outSource.append(returnType + " " + funcName + "(");

            std::map<std::string, std::string>::iterator it2;
            for (it2 = localVars.begin(); it2 != localVars.end(); it2++) {
                outSource.append(it2->second + " " + it2->first);

                if (std::next(it2) != localVars.end()) {
                    outSource.append(",");
                }
            }

            outSource.append(") {\n");
            emitter.emitToUserFuncDefs(outSource);

            advanceToken();
            advanceToken();
            match(CrlbraceL);

            while (!compareToCurToken(CrlbraceR)) {
                statement(true); // solution. Make an optimizer !
            }
            emitter.emitToUserFuncDefs("}\n");
            EOCB();
            clrFunctionvars();

        } else if (compareToCurToken(At)) { // function call
            std::cout << "(CALL-FUNC)\n";

            advanceToken();
            parseFunction(isFuncStatement);

        } else if (compareToCurToken(Return)) {
            std::cout << "(FUNC)-RETURN STATEMENT\n";
            advanceToken();

            if (!isFuncStatement) {
                emitter.emit("return ");
            } else {
                emitter.emitToUserFuncDefs("return ");
            }

            if (curToken.getType() == StringLiteral ||
                compareToCurToken(Identifier) && findIdentType(curToken) == StringLiteral) {

                // something is wrong with my thought process
                std::string outSource = (curToken.getType() == StringLiteral) ? "\"" + curToken.getTokenText() + "\""
                                                                              : curToken.getTokenText();
                if (!isFuncStatement) {
                    emitter.emit(outSource);
                } else {
                    emitter.emitToUserFuncDefs(outSource);
                }
                advanceToken();
            } else {
                expression(isFuncStatement);
            }
            EOS();

            if (!isFuncStatement) {
                emitter.emit(";\n");
            } else {
                emitter.emitToUserFuncDefs(";\n");
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
            statement(false);
        }

        emitter.emitLine("\nreturn 0;");
        emitter.emitLine("}");
        emitter.writeFile();
        std::cout << "🌐 parsing complete\n";
        std::cout << "\n-OUTPUT-\n";
        system("clang out.c -o a.out && ./a.out");
    }


};


#endif //ATLAS_COMPILER_PARSER_H
