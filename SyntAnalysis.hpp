#ifndef _SALIB_H_
#define _SALIB_H_

#include "POLIZ.hpp"
#include "LexAnalysis.hpp"

class Synt_analyzer
{
    /*
     * Syntax checking tool
     */
    Lexeme curr_lex;

    // Functions for syntax analyzing.
    void prog(); // prog -> main () [{body}]
    void body(); // body -> com {; com}
    void com(); // com -> id = expr | if (expr) then (com) else (com) |
    //					 while (expr) do (com) |
    //					 get (id) |
    // 					 put (id)
    void expr(); // expr -> sum [= | > | < | !=] sum | sum
    void sum(); // sum -> prod {[+ | - | ||] prod}
    void prod(); // prod -> oprnd {[* | / | &&] oprnd}
    void oprnd(); // oprnd -> id | num | !oprnd | (expr)

    // See Gammar.txt for further info

    void next_lex()
    {
	curr_lex = lan.get_lex();
    }

public:
    Lexic_analyzer lan;
    POLIZ poliz;
    void TryGetLexemes(FILE* fs);
    void process();
};

#endif
