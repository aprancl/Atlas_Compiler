//
// Created by Anthony Prancl on 5/24/22.
//

#ifndef ATLAS_COMPILER_LEXER_H
#define ATLAS_COMPILER_LEXER_H

#include <string>
#include <sstream>
#include <iostream>
#include <map>
#include <stdio.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"

// enum more practical than array
enum TokenType {
    // NAVIGATIONAL TOKENS
    None = -2, // this is just a placeholder for the parser
    Eof = -1, // end of file
    NewLine = 0,
    Space = 1,
    Eos = 5, // end of statement // ;
    ParenthR = 201, // )
    ParenthL = 200, // (
    CrlbraceR = 301, // }
    CrlbraceL = 300, // {
    SqrbraceR = 401, // ]
    SqrbraceL = 400, // [
    Bslash = 199,
    At = 198,
    Comma = 197,
    Period = 21,


    // DATA TYPES
    NumKW = 25, // this will need to be refactored to keyword NUM
    StringKW = 75,

    // KEYWORDS...
    Keyword = 99, // this is too generic
    Write = 100, // this is the print function
    If = 101,
    Else = -101,
    Input = 102,
    While = 104,
    For = 105,
    Define = 8,
    Return = 9,

    // 'Objects'
    IntLiteral = 2,
    StringLiteral = 3,
    FloatLiteral = 7,
    Identifier = 4,
    Comment = 6, // sort of a keyword but it doesn't really do anything


    //OPERATORS
    Eq = 10, // math operators // was 10
    Plus = 11,
    Minus = 12,
    Asterisk = 13,
    Fslash = 14,
    Gt = 15, // Comparison Operators
    Lt = 16,
    EqEq = 17,
    NotEq = 18,
    GtEq = 19,
    LtEq = 20


};

class Token {
protected:
    // data members
    std::string tokenText;
    TokenType type;

public:
    // constructors
    Token() = default;

    Token(std::string tokenText, TokenType type) {
        this->tokenText = tokenText;
        this->type = type;
    }

    // getters/accessors
    std::string getTokenText() {
        return tokenText;
    }

    TokenType getType() {
        return type;
    }


    // enum types where prints as numbers not name of type
    static std::string typeToString(int tokenType) {
        std::map<int, std::string> typeMap;
        typeMap[-2] = "None";
        typeMap[-1] = "Eof";
        typeMap[0] = "NewLine";
        typeMap[1] = "Space";
        typeMap[2] = "IntLiteral";
        typeMap[3] = "StringLiteral";
        typeMap[4] = "Identifier";
        typeMap[5] = "Eos";
        typeMap[6] = "Comment";
        typeMap[7] = "FloatLiteral";
        typeMap[8] = "DefineKW";
        typeMap[9] = "Return";
        typeMap[201] = "ParenthR";
        typeMap[200] = "ParenthL";
        typeMap[301] = "ClrbraceR";
        typeMap[300] = "CrlbraceL";
        typeMap[401] = "SqrbraceR";
        typeMap[400] = "SqrbraceL";
        typeMap[10] = "Eq";
        typeMap[11] = "Plus";
        typeMap[12] = "Minus";
        typeMap[13] = "Asterisk";
        typeMap[14] = "Fslash";
        typeMap[15] = "Gt";
        typeMap[16] = "Lt";
        typeMap[17] = "EqEq";
        typeMap[18] = "NotEq";
        typeMap[19] = "GtEq";
        typeMap[20] = "LtEq";
        typeMap[21] = "Period";
        typeMap[25] = "IntKW";
        typeMap[50] = "FloKW";
        typeMap[75] = "StringKW";
        typeMap[99] = "Keyword";
        typeMap[197] = "Comma";
        typeMap[198] = "At";
        typeMap[199] = "Bslash";
        typeMap[100] = "Write";
        typeMap[101] = "If";
        typeMap[-101] = "Else";
        typeMap[102] = "Input";
        typeMap[103] = "Cc";
        typeMap[104] = "While";
        typeMap[105] = "For";

        return typeMap[tokenType];
    }


};


class Lexer { // this is the main portion of the lexer class

private:
    // in c++ class members are private by default
    std::string source;
    std::string curChar;
    std::string lastChar;
    int curPosition;
    int curLineNum;


    //  <> some private helper methods <>


    int isDigit(std::string num) {

        // solution to very odd edge case
        // method was returning true (1) to the '\0' (null) character when passed as input
        if (num == "\0") {
            return 0;
        }

        for (int i = 0; i < num.length(); i++) {
            if (!std::isdigit(num[i])) {
                return 0;
            }
        }
        return 1;
    }


    int isKeyWord(std::string text) {
        std::string key_words[] = {"WRITE", "INPUT", "IF", "WHILE", "STRING", "NUM", "FOR", "DEFINE", "RETURN", "ELSE"};

        for (std::string keyWord: key_words) {
            if (keyWord == text) {
                return 1;
            }
        }
        return 0;

    }

    TokenType getThisKeyword(std::string tokenText) {
        std::string key_words[] = {"WRITE", "INPUT", "IF", "WHILE", "STRING", "NUM", "FOR", "DEFINE", "RETURN", "ELSE"};

        if (tokenText == key_words[0]) {
            return Write;
        } else if (tokenText == key_words[1]) {
            return Input;
        } else if (tokenText == key_words[2]) {
            return If;
        } else if (tokenText == key_words[3]) {
            return While;
        } else if (tokenText == key_words[4]) {
            return StringKW;
        } else if (tokenText == key_words[5]) {
            return NumKW;
        } else if (tokenText == key_words[6]) {
            return For;
        } else if (tokenText == key_words[7]) {
            return Define;
        } else if (tokenText == key_words[8]) {
            return Return;
        } else if (tokenText == key_words[9]) {
            return Else;
        } else {
            return None; // this shouldn't happen
        }
    }


public:

// constructors
    Lexer() {
        curPosition = -1;
        this->nextChar();
        curLineNum = 1;
    }

    Lexer(std::string source) {
        this->source = source + "\n";
        curChar = "";
        curPosition = -1;
        this->nextChar();
        curLineNum = 1;

    }

//methods

// return the next token // this is the "meat" of the lexer // eventually will be controlled by the parser
    Token getToken() {
        Token *token; // pointer to an eventual token object existing on the heap

        // length 1 tokens (mostly basic math operators, parenthesis, and such )
        if (curChar == " ") { // we want to skip white space // spaces
            token = new Token(curChar, Space);
        } else if (curChar == "+") { // addition
            token = new Token(curChar, Plus);
        } else if (curChar == "-") { // subtraction
            token = new Token(curChar, Minus);
        } else if (curChar == "*") { // multiplication
            token = new Token(curChar, Asterisk);
        } else if (curChar == "/") { // division
            token = new Token(curChar, Fslash);
        } else if (curChar == "\\") { // backslash
            token = new Token(curChar, Bslash);
        } else if (curChar == ";") { // colon EOS
            token = new Token(curChar, Eos);
        } else if (curChar == "(") { // right parentheses
            token = new Token(curChar, ParenthL);
        } else if (curChar == ")") { // left parentheses
            token = new Token(curChar, ParenthR);
        } else if (curChar == "[") { // right square brace
            token = new Token(curChar, SqrbraceL);
        } else if (curChar == "]") { // left square brace
            token = new Token(curChar, SqrbraceR);
        } else if (curChar == "{") { // right curly bracket
            token = new Token(curChar, CrlbraceL);
        } else if (curChar == "}") { // left curly bracket
            token = new Token(curChar, CrlbraceR);
        } else if (curChar == "@") {
            token = new Token(curChar, At);
        } else if (curChar == ",") {
            token = new Token(curChar, Comma);
        } else if (curChar == ".") {
            token = new Token(curChar, Period);
        }
            // length 2 tokens (mostly boolean comparison operators)
        else if (curChar == "=") { // equals and equal-equals
            if (lookAhead() == "=") {
                lastChar = curChar;
                nextChar();
                token = new Token(curChar.append(lastChar), EqEq);
            } else {
                token = new Token(curChar, Eq);
            }
        } else if (curChar == ">") { // greater than and greater to or equal to
            if (lookAhead() == "=") {
                lastChar = curChar;
                nextChar();
                token = new Token(lastChar.append(curChar), GtEq);
            } else {
                token = new Token(curChar, Gt);
            }
        } else if (curChar == "<") {// less than and less than or equal to
            if (lookAhead() == "=") {
                lastChar = curChar;
                nextChar();
                token = new Token(lastChar.append(curChar), LtEq);
            } else {
                token = new Token(curChar, Lt);
            }
        } else if (curChar == "!") { // not equals
            if (lookAhead() == "=") {
                lastChar = curChar;
                nextChar();
                token = new Token(lastChar.append(curChar), NotEq);
            } else {
                printf(ANSI_COLOR_MAGENTA "Lexing error..Expected \"!=\" but got \"!%s\"", lookAhead().c_str());
                exit(25);
            }

        }
            // more complex tokens with indefinite lengths
        else if (curChar == "C" && lookAhead() == "C") { // comments

            nextChar();
            nextChar();
            std::string text = "";
            while (curChar != "\n") {
                text.append(curChar);
                nextChar();
            }
            text = text.substr(0).c_str();
            token = new Token(text, Comment);

        } else if (curChar == "\"") { // strings

            nextChar();
            std::string str = "";
            while (curChar != "\"") {
                if (curChar != "\\") { // illegal characters for now // will add escape sequences later
                    if (curChar == "\0") { // make sure that the string literal is closed
                        printf(ANSI_COLOR_MAGENTA "\nLexing error..expected another <\"> to close string literal <\"%s>",
                               str.substr(0, str.length() - 1).c_str());
                        exit(25);
                    }
                    str.append(curChar);
                    nextChar();
                } else {
                    printf(ANSI_COLOR_MAGENTA "Lexing error..illegal character: \"%s\"", curChar.c_str());
                    exit(25);
                }
            }
            token = new Token(str, StringLiteral);

        } else if (isDigit(curChar.c_str()) == 1) { // Integers
            std::string num = "";
            while (isDigit(curChar.c_str()) == 1) {
                num.append(curChar);
                nextChar();
            }
            --curPosition; // compensate for the next char at the end of the decision structure
            token = new Token(num, IntLiteral);
            // we have to go back one before we exit this part of the decision structure

        } else if (curChar == "f" && lookAhead() == "l") { // floats

            // skip characters <f> and <l> as they just mark the start of a float
            nextChar();
            nextChar();

            std::string float_literal = "";

            int decimal_counter = 0;
            while (isDigit(curChar.c_str()) || curChar == ".") {

                if (curChar == ".") {
                    decimal_counter++;
                }

                float_literal.append(curChar);
                nextChar();

            }

            if (decimal_counter > 1) {
                printf(ANSI_COLOR_MAGENTA "\nLexing error..cannot use more than one decimal per float literal.. <%s> ..line number: %d\n",
                       float_literal.c_str(), curLineNum);
            } else {
                curPosition--;
                token = new Token(float_literal, FloatLiteral);
            }

        } else if (isalpha(curChar[0])) { // Keywords and identifiers
            std::string tokenText = "";

            while (isalnum(curChar[0])) {
                tokenText.append(curChar);
                nextChar();
            }
            --curPosition;

            if (isKeyWord(tokenText)) {

                TokenType type = getThisKeyword(tokenText);
                token = new Token(tokenText, type);

            } else {
                token = new Token(tokenText, Identifier);

            }

        } else if (curChar == "\n") { // navigational tokens
            curLineNum++;
            token = new Token(curChar, NewLine);

        } else if (curChar == "\0") {
            token = new Token(curChar, Eof);
        } else {
            printf(ANSI_COLOR_MAGENTA "\nLexing error..unknown token <%s> at line number: %d", curChar.c_str(),
                   curLineNum);
            exit(25); //  stop program && lexical analysis
        }

        // leave the lexer at the start of the next token
        nextChar();
        return *token;
    }


    // navigational methods
// get the next character in the sequence of characters
    void nextChar() {
        // increment our space by one
        curPosition++;
        if (curPosition >= source.length()) {
            curChar = "\0";
        } else {
            curChar = source[curPosition];
        }

    }

// from the current position, peek at what the next character will be
    std::string lookAhead() {
        if (curPosition + 1 >= source.length()) {
            return "\0";
        } else {
            std::string charSeen = source.substr(curPosition + 1, 1);
            return charSeen;
        }

    }

    void setToLastEq() {

        while (curChar != "=") {
            curPosition--;
            this->setCurChar(source.substr(curPosition, 1));
        }

        curChar = std::to_string(source[curPosition]);
    }

    void setToStartOfName() {

        while (curChar != "E") {
            curPosition--;
            this->setCurChar(source.substr(curPosition, 1));
        }

        this->nextChar();

        curChar + source[curPosition];

    }

// getters and setters
    std::string getCurChar() {
        return curChar;
    }

    void setCurChar(std::string character) {
        this->curChar = character;
    }

    std::string getLastChar() {
        return lastChar;
    }

    int getCurPosition() {
        return curPosition;
    }

    void setCurPosition(int position) {
        this->curPosition = position;
    }

    std::string getSource() {
        return source;
    }

    int getCurLineNumber() {
        return curLineNum;
    }

    std::string getPeek() {
        return lookAhead();
    }

};


#endif //ATLAS_COMPILER_LEXER_H
