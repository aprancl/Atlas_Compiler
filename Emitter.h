//
// Created by Anthony Prancl on 6/12/22.
//

#ifndef ATLAS_COMPILER_EMITTER_H
#define ATLAS_COMPILER_EMITTER_H
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_YELLOW "\u001b[33m"

#include <iostream>
#include <fstream>
#include "Lexer.h"


class Emitter {

    // data members
    std::string fullPath;
    std::string includeStatements = "#include <stdio.h>\n#include <stdlib.h>\n";
    std::string userFuncDefs;
    std::string funcMainHeader = "int main() {\n";
    std::string header; //used for variable declarations
    std::string outSource;
//    std::string setUp = "#include <stdio.h>\nint main() {\n";

public:

    Emitter() = default;

    Emitter(std::string fullPath) {
        this->fullPath = fullPath;
        header = "";
        outSource = "";
    }

    void emit(std::string outSource) {
        this->outSource += outSource;
    }

    void emitLine(std::string outSource) {
        this->outSource += outSource + "\n";
    }

    void emitHeaderLine(std::string outSource) {
        this->header += outSource + "\n";
    }

    void emitHeader(std::string outSource) {
        this->header += outSource;
    }

    void emitToUserFuncDefs(std::string outSource) {
        this->userFuncDefs += outSource;
    }

    // other methods
    void optimize(std::string source) {

        source.insert(0," ");
        Lexer lexer(source);

        int cpyStartPos;
        int cpyEndPos;
        Token lastToken;
        Token curToken = lexer.getToken();
        while (curToken.getType() != Eof) {
            cpyStartPos = lexer.getCurPosition();

            if (curToken.getType() == Space){
                curToken = lexer.getToken();
                continue;
            } else if (curToken.getType() == CreturnType ){
                curToken = lexer.getToken();

                if (curToken.getType() == Space) {
                    curToken = lexer.getToken();
                } else if (curToken.getType() == Identifier && lexer.lookAhead() == "(") {
                    while (lexer.getCurChar() != "}"){
                        lexer.nextChar();
                    }
                    cpyEndPos = lexer.getCurPosition();

                    // copy and delete the section of code
                    emitToUserFuncDefs(source.substr(cpyStartPos, cpyEndPos - cpyStartPos));


                }
            } else {
                curToken = lexer.getToken();
                continue;
            }

        }


    }

    void writeFile() {
        std::ofstream myOutFile;
        myOutFile.open(fullPath);

        optimize(outSource);

        if (myOutFile.is_open()) {
            myOutFile << includeStatements + userFuncDefs + funcMainHeader + "\n" + header + "\n" + outSource;
            myOutFile.close();
        } else {
            printf(ANSI_COLOR_YELLOW "\nEmitting error..could not open file path...\n%s\n...for writing\n",
                   fullPath.c_str());
            exit(45);
        }

    }


};


#endif //ATLAS_COMPILER_EMITTER_H
