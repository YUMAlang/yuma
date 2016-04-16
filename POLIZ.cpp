class POLIZ
{
	vector<Lexeme> code;
	int position;
	
public:

	POLIZ ()
	{
		position = 0;
		code.clear();
	}
	int get_pos () { return position; }
	void put_space () { position++ }
	void put_lex (Lexeme lex)
	{
		code.at(position) = lex;
		position++;
	}
	void put_lex (Lexeme lex, int forced_pos)
	{
		code.at(forced_pos) = lex;
	}
}