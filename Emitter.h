//
// Created by Anthony Prancl on 6/12/22.
//

#ifndef ATLAS_COMPILER_EMITTER_H
#define ATLAS_COMPILER_EMITTER_H

#include <iostream>


class Emitter {

    // data members
    std::string fullPath;
    std::string header;
    std::string code;

public:

    Emitter() = default;

    Emitter(std::string fullPath){
        this->fullPath = fullPath;
        header = " ";
        code = " ";
    }



};


#endif //ATLAS_COMPILER_EMITTER_H
