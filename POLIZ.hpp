#ifndef _POLLIB_H_
#define _POLLIB_H_

#include <vector>
#include "LexAnalysis.hpp"

class POLIZ
{
    vector<Lexeme> code;
    int position;

public:
    POLIZ();

    int get_pos() const
    {
		return position;
    }
    void put_lex(Lexeme lex);
    void put_lex(Lexeme lex, int forced_pos);
    void put_space();
    void print();

    int size() const
    {
		return code.size();
    }
    Lexeme at(int i) const
    {
		return code.at(i);
    }
};

#endif
