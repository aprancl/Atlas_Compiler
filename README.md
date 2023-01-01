# Atlas_Compiler

## Description
A source-to-source compiler that translates my languages, ATLAS, into C source code. I decided to embark on this project to learn more about: what happens when something is compiled, how C++ and C work, and what it means to design a good programming language.     

## Logical Components 
#### Consists of:
- Lexer (Tokenizer)
- Parser
- Code Emitter
- "Helper" classes
  - Variable.h
  - optimizer? (Future)
## Formal Syntax

### Rules <br>
::= <> consists of <br>
{x} <> one or more of x <br>
[x] <> zero or one of x <br>
() <> just grouping <br>
< x > <> one and only one of x <br>
|| <> means or ( like java) <br>
KW <> denotes that the thing is a keyword <br>
TK(S) <> token that we have already defined <br>
nl <> new line <br>
... <> ellipses means pattern <br>

### Structure

program ::= {statement} <br>
statement ::= KW"WRITE" (expression || TK.StringLiteral) TK.Eos <br>
&nbsp;&nbsp;&nbsp;&nbsp;|| KW."STRING" TK.Identifier TK."=" (TK.StringLiteral || TK.Identifier) TK.Eos <br>
&nbsp;&nbsp;&nbsp;&nbsp;|| KW."INT" TK.Identifier TK."=" (TK.IntLiteral || expression) TK.Eos <br>
&nbsp;&nbsp;&nbsp;&nbsp;|| KW."IF" <comparison> TKS."){" nl {statement} TKS."};" TK.Eos <br>
&nbsp;&nbsp;&nbsp;&nbsp;|| KW."WHILE" <comparison> TKS."]{" {statement} TKS"};" TK.Eos <br>
&nbsp;&nbsp;&nbsp;&nbsp;|| KW."INPUT" TK.Identifier TK.Eos <br>
&nbsp;&nbsp;&nbsp;&nbsp;|| KW."FOR" TK."[" KW."INT" TK."]" {statment} TK.EOS <br>
&nbsp;&nbsp;&nbsp;&nbsp;|| TK.identifier TK."=" (TK.StringLiteral || TK.IntLiteral) Tk.Eos <br>
comparison ::= expression {(">" || "<" || ">=" || "<=" || "==" || "!=") expression} <br>
expression ::= term {("+" || "-" term)} <br>
term ::= unary {("*" || "/") unary} <br>
unary ::= ["+" || "-"]primary <br>
primary ::= TK.StringLiteral || TK.identifier || TK.FloatLiteral <br>
TK.Eos = ";" End Of Statement -> EOS <br>


## Key Words

- CC - denotes a comment
- WRITE - standard output function
```javascript
WRITE "hello world"; CC this will print the given string literal
WRITE a * b + c; CC expressions and variables work as well
```
- IF - conditional branch
```javascript
IF [n >= 5]{
    WRITE "n is not 3";
};
```
- ELSE - conditional branch
```javascript
IF [n == 0]{
        RETURN 1;
    };

ELSE {
    NUM x = n * 10;
    NUM temp = @mystery(x,); CC calls function and saves value to temp
    RETURN  n * temp + 4;
};
```
- INPUT - takes input from user
```javascript
INPUT STRING name; CC INPUT <Data Type> <Variable Name>
```
WHILE - loop type
```javascript
CC Print all positive numbers less than a given n
INPUT NUM n;
NUM i = 0;

WHILE [ i < n]{
  WRITE i;
  i = i + 1;
};
```
- FOR - loop type
```javascript

CC you can loop for a given amount of times 
INPUT NUM x;

FOR [x]{
    WRITE "Hello";
};

CC you can also loop for the length of a string

FOR ["Strawberry"]{
    WRITE x;
};
```
- DEFINE - used to define a function
```javascript

CC DEFINE is followed by the function name
CC within the parameters of the function, local 
CC variable names and datatypes are seperated by a colon 
CC all local parameters are ended by a comma 
CC following the dollar sign, we have the return type
DEFINE factorial (n:NUM,) $ NUM {
  CC returns n! factorial

  IF [n == 0]{
    RETURN 1;
  };

  ELSE {
    NUM x = n - 1;
    NUM temp = @factorial(x,);
    RETURN  n * temp;
  };

  RETURN 1;

};

```
- RETURN - returns a values within a function
- NUM - a data type 
```javascript
CC all numbers in ATLAS are assumed to be integers unless otherwise specified
NUM x  = 10;
```
- fl - used to specify a float literal
```javascript
NUM pi = fl3.14;
```
- STRING - a data type
```javascript
STRING name = "Anthony";
```
- @ - designates a function call 
```javascript
CC here we have our factorial function again
DEFINE factorial (n:NUM,) $ NUM {
  CC returns n! factorial

  IF [n == 0]{
    RETURN 1;
  };

  ELSE {
    NUM x = n - 1;
    NUM temp = @factorial(x,); CC recursive call
    RETURN  n * temp;
  };

  RETURN 1;

};


WRITE "Please enter a number below";
INPUT NUM in;

NUM answer = @factorial(in,); CC call with variable input

WRITE answer;

```
