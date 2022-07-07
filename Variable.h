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
    Variable *ptrVar;

public:
    //constructors
    Variable() = default;

    // normal variable
    Variable(std::string name, std::string value, TokenType dataType) {
        this->name = name;
        this->value = value;
        this->dataType = dataType;
        ptrVar = NULL;
    }

    // variable pointing to another variable...
    Variable(std::string name, Variable ptrVar) {
        this->name = name;
        this->ptrVar = &ptrVar;
        this->value = ptrVar.getValue();
        this->dataType = ptrVar.getDataType();
    }

    // getters

    std::string getName(){
        return name;
    }

    std::string getValue() {
        return value;
    }

    TokenType getDataType() {
        return dataType;
    }

    Variable* getptrVar(){
        return ptrVar;
    }

    //setters
    void setName(std::string name){
        this->name = name;
    }

    void setValue(std::string value) {
        this->value = value;
    }
    void setPtrVar(Variable ptrVar){
        new (this) Variable(this->name, ptrVar);
    }

    // helper methods

};


#endif //ATLAS_COMPILER_VARIABLE_H
