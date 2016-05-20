#ifndef _SALIB_H_
#define _SALIB_H_

#include "LexAnalysis.hpp"

class Synt_analyzer
{
	/*
	 * Syntax checking tool
	 */ 
	Lexeme curr_lex;
	
	
	
	//Functions for syntax analyzing.
	void prog (POLIZ &p);	//prog -> main () [{body}]
	void body (POLIZ &p);	//body -> com {; com}
	void com (POLIZ &p);	//com -> id = expr | if (expr) then (com) else (com) |
					//					 while (expr) do (com) |
				    //					 get (id) |
				    // 					 put (id)
	void expr (POLIZ &p);	//expr -> sum [= | > | < | !=] sum | sum
	void sum (POLIZ &p);	//sum -> prod {[+ | - | ||] prod}
	void prod (POLIZ &p);	//prod -> oprnd {[* | / | &&] oprnd}
	void oprnd (POLIZ &p);	//oprnd -> id | num | !oprnd | (expr)
	
	//See Gammar.txt for further info
	
	void next_lex ()
	{
		curr_lex = lan.get_lex();
	}
	
public:
	Lexic_analyzer lan;
	POLIZ poliz;
	void TryGetLexemes (FILE* fs);
	void process ();
};

#endif
