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

void POLIZ::print()
{
	for (unsigned int i = 0; i < code.size(); i++)
	{
		cout << "[" << code.at(i).table << " : " << code.at(i).num << "]";
	}
}

void POLIZ::put_space()
{
	code.push_back(Lexeme (POLIZ_LABEL, 1001));
	position++;
}

