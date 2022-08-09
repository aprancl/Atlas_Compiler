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
    bool isFuncVar;

public:
    //constructors
    Variable() = default;

    // normal variable
    Variable(std::string name, std::string value, TokenType dataType, bool isFuncVar) {
        this->name = name;
        this->value = value;
        this->dataType = dataType;
        ptrVar = NULL;
        this->isFuncVar = isFuncVar;
    }

    // variable pointing to another variable...
    Variable(std::string name, Variable ptrVar, bool isFuncVar) {
        this->name = name;
        this->ptrVar = &ptrVar;
        this->value = ptrVar.getValue();
        this->dataType = ptrVar.getDataType();
        this->isFuncVar = ptrVar.getIsFuncVar();
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

    bool getIsFuncVar(){
        return isFuncVar;
    }

    //setters
    void setName(std::string name){
        this->name = name;
    }

    void setValue(std::string value) {
        this->value = value;
    }
    void setPtrVar(Variable ptrVar){
        new (this) Variable(this->name, ptrVar, isFuncVar);
    }

    // helper methods

};


#endif //ATLAS_COMPILER_VARIABLE_H
