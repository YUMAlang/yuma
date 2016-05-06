#ifndef _POLLIB_H_
#define _POLLIB_H_

#include <vector>
#include "LexAnalysis.hpp"

class POLIZ
{
	
	int position;
	
public:

	vector<Lexeme> code;
	POLIZ ();
	int get_pos () const { return position; }
	void put_space () { position++; }
	void put_lex (Lexeme lex);
	void put_lex (Lexeme lex, int forced_pos);
};

#endif
