//
// Created by Anthony Prancl on 6/23/22.
//

#ifndef ATLAS_COMPILER_VARIABLE_H
#define ATLAS_COMPILER_VARIABLE_H

#include "Lexer.h"
#include <iostream>

class Variable {
    // data members
    std::string name;
    std::string value;
    TokenType dataType;
    std::string prefix;
    Variable *ptrVal;

public:
    //constructors
    Variable() = default;

    // normal variable
    Variable(std::string name, std::string value, TokenType dataType, std::string prefix) {
        this->name = name;
        this->value = value;
        this->dataType = dataType;
        this->prefix = prefix;
        ptrVal = NULL;
    }

    // variable pointing to another variable...
    Variable(std::string name, Variable ptrVal) {
        this->name = name;
        this->ptrVal = &ptrVal;
        this->value = ptrVal.getValue();
        this->dataType = ptrVal.getDataType();
        this->prefix = (dataType == StringLiteral) ? "S_" : (dataType == IntLiteral) ? "I_" : "F_";
    }

    // getters

    TokenType getDataType() {
        return dataType;
    }

    std::string getValue() {
        return value;
    }

    std::string getPrefix() {
        return prefix;
    }

    //setters
    void setPrefix(TokenType prefix) {
        this->prefix = prefix;
    }

    void setValue(std::string value) {
        this->value = value;
    }

    void setPrefix(std::string prefix) {
        this->prefix = prefix;
    }

    // helper method

};


#endif //ATLAS_COMPILER_VARIABLE_H
