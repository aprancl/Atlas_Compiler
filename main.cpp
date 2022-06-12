#include <iostream>
#include "Lexer.h"
#include "Parser.h"
#include "Emitter.h"
#include <map>
#include <fstream>


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
    std::string input = getSource("input.atlas");

    // create necessary object
    Lexer lexer(input);
    Parser parser(lexer);
    // emitter = Emitter("out.c");

    // start the program
    parser.program();
    // emitter.writeFile();

    return 0;
}



// old versions of main function I don't want to lose quite yet


//while (lexer.lookAhead() != "\0"){ // "\0" is EOF
//std::cout << lexer.getCurChar() << std::endl;
//lexer.nextChar();
//}

// test source tokens
//+=>>=- <==!=*/==!= +++CCabc+++   +1INT-2"Hello world"*123"Hello again0
// IF+-123 foo*WHILE/"Anthony"+=>>=- <==!=*/==!= +++CCabc+++ ;  +1INT-2"Hello world"*123"Hello again0"
//




//std::string input = getSource("input.atlas");
//Lexer lexer(input);
//
//Token token = lexer.getToken();
//while (token.getType() != Eof) { // "\0" is EOF
//
//if (token.getType() != Space) { // just ignore the spaces for now, they don't do anything
//std::cout << token.getType() << " " << Token::typeToString(token.getType()) << std::endl;
//}
//
//token = lexer.getToken();
//}