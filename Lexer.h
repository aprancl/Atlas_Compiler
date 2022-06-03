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


    // DATA TYPES
    IntKW = 25, // perhaps just make everything floats and call this token NUM
    DeciKW = 50,
    StringKW = 75,

    // KEYWORDS...
    Keyword = 99, // this is too generic
    Write = 100, // this is the print function
    If = 101,
    Input = 102,
    While = 104,
    For = 105,

    // Objects
    IntLiteral = 2,
    StringLiteral = 3,
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

// Note to self: I am making token objects and storing them on the heap. are their constructors being
// called implicitly or am I creating a massive memory leak ?
class Token {
private:
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

    // getters/accessors (don't need setters)
    std::string getTokenText() {
        return tokenText;
    }

    TokenType getType() {
        return type;
    }


    // band-aid solution to minor issue of representing what a given type is...
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
        typeMap[25] = "IntKW";
        typeMap[50] = "DeciKW";
        typeMap[75] = "StringKW";
        typeMap[99] = "Keyword";
        typeMap[199] = "Bslash";
        typeMap[100] = "Write";
        typeMap[101] = "If";
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
        std::string key_words[] = {"WRITE", "INPUT", "IF", "WHILE", "STRING", "INT", "FOR"};

        for (std::string keyWord: key_words) {
            if (keyWord == text) {
                return 1;
            }
        }
        return 0;

    }

    TokenType getThisKeyword(std::string tokenText) {
        std::string key_words[] = {"WRITE", "INPUT", "IF", "WHILE", "STRING", "INT", "FOR"};

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
            return IntKW;
        } else if (tokenText == key_words[6]) {
            return For;
        } else {
            return None; // this shouldn't happen
        }
    }


public:

// constructors
    Lexer() {
        curPosition = -1;
        this->nextChar();
    }

    Lexer(std::string
          source) {
        this->
                source = source + "\n";
        curChar = "";
        curPosition = -1;
        this->

                nextChar();

    }

//methods

// return the next token // this is the "meat" of the lexer // eventually will be controlled by the parser
    Token getToken() {
        Token *token; // pointer to an eventual token object existing on the heap

        // length 1 tokens (mostly basic math operators, parenthesis, and such )
        if (curChar == " ") { // we want to skip white space
            token = new Token(curChar, Space);
        } else if (curChar == "+") {
            token = new Token(curChar, Plus);
        } else if (curChar == "-") {
            token = new Token(curChar, Minus);
        } else if (curChar == "*") {
            token = new Token(curChar, Asterisk);
        } else if (curChar == "/") {
            token = new Token(curChar, Fslash);
        } else if (curChar == "\\") {
            token = new Token(curChar, Bslash);
        } else if (curChar == ";") {
            token = new Token(curChar, Eos);
        } else if (curChar == "(") {
            token = new Token(curChar, ParenthL);
        } else if (curChar == ")") {
            token = new Token(curChar, ParenthR);
        } else if (curChar == "[") {
            token = new Token(curChar, SqrbraceL);
        } else if (curChar == "]") {
            token = new Token(curChar, SqrbraceR);
        } else if (curChar == "{") {
            token = new Token(curChar, CrlbraceL);
        } else if (curChar == "}") {
            token = new Token(curChar, CrlbraceR);
        }
            // length 2 tokens (mostly boolean comparison operators)
        else if (curChar == "=") {
            if (lookAhead() == "=") {
                lastChar = curChar;
                nextChar();
                token = new Token(curChar.append(lastChar), EqEq);
            } else {
                token = new Token(curChar, Eq);
            }
        } else if (curChar == ">") {
            if (lookAhead() == "=") {
                lastChar = curChar;
                nextChar();
                token = new Token(curChar.append(lastChar), GtEq);
            } else {
                token = new Token(curChar, Gt);
            }
        } else if (curChar == "<") {
            if (lookAhead() == "=") {
                lastChar = curChar;
                nextChar();
                token = new Token(curChar.append(lastChar), LtEq);
            } else {
                token = new Token(curChar, Lt);
            }
        } else if (curChar == "!") {
            if (lookAhead() == "=") {
                lastChar = curChar;
                nextChar();
                token = new Token(curChar.append(lastChar), NotEq);
            } else {
                printf(ANSI_COLOR_MAGENTA "Lexing error..Expected \"!=\" but got \"!%s\"", lookAhead().c_str());
                exit(25);
            }

        }
            // more complex tokens with indefinite lengths
        else if (curChar == "C" && lookAhead() == "C") { // comments
            std::string text = "";
            while (curChar != "\n") {
                nextChar();

                if (curChar != "C" && curChar != " ") {
                    text.append(curChar);
                }
            }
            text = text.substr(0, text.length() - 1).c_str();
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

        } else if (isDigit(curChar.c_str()) == 1) { // Integers and numbers
            std::string num = "";
            while (isDigit(curChar.c_str()) == 1) {
                num.append(curChar);
                nextChar();
            }
            --curPosition; // compensate for the next char at the end of the decision structure
            token = new Token(num, IntLiteral);
            // we have to go back one before we exit this part of the decision structure

        } else if (isalpha(curChar[0])) {
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
            // else { make token with keyword identifier}

        }

            // navigational tokens
        else if (curChar == "\n") {
            token = new Token(curChar, NewLine);
        } else if (curChar == "\0") {
            token = new Token(curChar, Eof);
        } else {
            printf(ANSI_COLOR_MAGENTA "Lexing error..unknown token <%s>", curChar.c_str());
            exit(25); //  stop program && lexical analysis
        }

        // move the lexer from the end of this token to the beginning of the next (ideally)
        // for now, just go ahead by one because all of our tokens are len of 1
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

// getters and setters
    std::string getCurChar() {
        return curChar;
    }

    std::string getLastChar() {
        return lastChar;
    }

    int getCurPosition() {
        return curPosition;
    }

    std::string getSource() {
        return source;
    }

};


#endif //ATLAS_COMPILER_LEXER_H
