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

/*
char *sliceString(char *str, int start, int end)
{

    int i;
    int size = (end - start) + 2;
    char *output = (char *)malloc(size * sizeof(char));

    for (i = 0; start <= end; start++, i++)
    {
        output[i] = str[start];
    }

    output[size] = '\0';

    return output;
}
*/

/*
float length(char* a){

    char curChar = &a;
    int counter = 0;
    while (curChar != '\0'){
        counter++;
        curChar = a[counter];

    }

    return counter;
}
 */

class Emitter {

private:

    // data members
    std::string fullPath;
    std::string includeStatements = "#include <stdio.h>\n#include <stdlib.h>\n";
    std::string userFuncDefs;
    std::string funcMainHeader = "int main() {\n";
    std::string header; //used for variable declarations
    std::string outSource;


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

    void appendStringSliceMethod() {
        userFuncDefs.insert(0,
                            "char *sliceString(char *str, int start, int end) {\nint i;\nint size = (end - start) + 2;\nchar *output = (char *) malloc(size * sizeof(char));\nfor (i = 0; start <= end; start++, i++) {\noutput[i] = str[start];\n\n}\noutput[size] = '\\0';\nreturn output;\n\n}\n");
    }

    // other methods

    void writeFile() {
        std::ofstream myOutFile;
        myOutFile.open(fullPath);

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
