#include "POLIZ.hpp"

POLIZ::POLIZ ()
	{
		position = 0;
		code.clear();
	}
	
void POLIZ::put_lex (Lexeme lex)
	{
		code.push_back(lex);
		position++;
	}
	
void POLIZ::put_lex (Lexeme lex, int forced_pos)
	{
		code.at(forced_pos) = lex;
	}
