#include <iostream>
#include "Lexer.h"
#include "Parser.h"
#include "Emitter.h"
#include <map>
#include <fstream>
// test 1 2 3 is git working now?
// TODO (polish, but important polish) variables within a function should be disposed of after their use
//TODO (polish) dont allow variable reassignment of an int to float  and float to int <-- causes inaccuracies
//TODO (polish)Fix the way that float literal are saved. install of "fl3.14" it should just be 3.14


// search for the given file name on computer and return its text <- will be saved to source variable in main
std::string getSource(std::string fileName) {

    std::string line;
    std::string source;
    std::ifstream inFile(fileName);

    if (inFile.is_open()) {

        while (std::getline(inFile, line, '\0')) {
            for (char ch: line) {
                source.push_back(ch);
            }
        }
        return source;

    } else {
        return ANSI_COLOR_RED "Error: Input file not found\n";
    }

}


int main() {


    // header
    std::cout << "🌐 ATLAS COMPILER\n";

    // save the source code from the input file as a long string of text
    std::string input = getSource("/Users/anthonyprancl/Documents/Classes/Summer Project/Atlas Compiler/io/input.atlas");

    // create necessary object
    Lexer lexer(input);
    Emitter emitter("/Users/anthonyprancl/Documents/Classes/Summer Project/Atlas Compiler/io/out.c");
    Parser parser(lexer, emitter);


    // start the program
    parser.program();

    return 0;
}


