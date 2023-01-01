# Atlas_Compiler

## Description
A source-to-source compiler that translates my languages, ATLAS, into C source code. I decided to embark on this project to learn more about: what happens when something is compiled, how C++ and C work, and what it means to design a good programming language.     

## Structure 
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

