//
// Created by Anthony Prancl on 6/12/22.
//

#ifndef ATLAS_COMPILER_EMITTER_H
#define ATLAS_COMPILER_EMITTER_H

#include <iostream>
#include <fstream>


class Emitter {

    // data members
    std::string fullPath;
    std::string header;
    std::string outSource;

public:

    Emitter() = default;

    Emitter(std::string fullPath){
        this->fullPath = fullPath;
        header = " ";
        outSource = " ";
    }

    void emit(std::string outSource){
        this->outSource += outSource;
    }

    void emitLine(std::string outSource ){
        this->outSource += outSource + "\n";
    }
    void headerLine(std::string outSource){
        this->header += outSource + "\n";
    }

    void writeFile(){
        std::ofstream myOutFile(fullPath);
        myOutFile << header + outSource;
        myOutFile.close();
    }




};


#endif //ATLAS_COMPILER_EMITTER_H
