												//Dedicated to Kódryan

#include <stdio.h>
#include "syntAnalysis.hpp"

void Synt_analyzer::prog ()
{
	//Performing lexic analysis
	//The program must start with {
	if (curr_lex.table == op && curr_lex.num == OPEN_BRACE)
	{
		next_lex();
	}
	else
	{
		throw "Sign { expected at the begining of the program";
	}
	body();
	//The program must finish with }
	if (curr_lex.table != op || curr_lex.num != CLOSE_BRACE)	
	{
		cout << curr_lex.table << " " << curr_lex.num << endl;
		throw "Sign } expected at the end of program";
	}
}

void Synt_analyzer::body ()
{
	com();
	while (curr_lex.table == op && curr_lex.num == SEMICOLON)
	{
		next_lex();
		com();
	}
	//On '}' body() will finish without reading next lexeme
}

void Synt_analyzer::com ()
{
	/* labels for POLIZ representation of <if> & <while>
	 * if A then B else C 
	 * [A] [if (!A) then goto do_else] [B] [goto after_then] [C]
	 *												 do_else^   ^after_then
	 * POLIZ: ...[A][d_e][!goto][B][a_t][goto](d_e->[C])(a_t->)...
	 */
	int l_true, l_false, l_loop, l_end;
	if (curr_lex.table == kw && curr_lex.num == IF)	//Command is of (if_then_else) type
	{
		next_lex();
		expr();	
                //Analyzing condition
		l_false = poliz.get_pos();	//saving position for do_else label
		poliz.put_space();			//allocating place in POLIZ for do_else label
		poliz.put_lex (Lexeme (op, GOTO_ON_FALSE));
		if (curr_lex.table == kw && curr_lex.num == THEN)	//Analyzing then_instructions
		{
			next_lex();
			//then_instr must start with "{"
			if (curr_lex.table == op && curr_lex.num == OPEN_BRACE)
			{
				next_lex();
				body();
				//and end with "}"
				if (curr_lex.table != op || curr_lex.num != CLOSE_BRACE)
				{
					throw "Sign } expected at the end of THEN";
				}
				next_lex();		//getting first lexeme after "{"
								//if it is ";", the if_instr ends here;
			}
			else
			{
				throw "Sign { expected at the begining of THEN";
			}
			l_true = poliz.get_pos(); //saving position for after_then label
			poliz.put_space();		  //allocating place
			poliz.put_lex (Lexeme(op, GOTO));
			//filling the place of else_do label
			poliz.put_lex (Lexeme (POLIZ_LABEL, poliz.get_pos()), l_false);
			if (curr_lex.table == kw && curr_lex.num == ELSE)
			{
				next_lex();
				//else_instr must start with "{"
				if (curr_lex.table == op && curr_lex.num == OPEN_BRACE)
				{
					next_lex();
					body();
					//and end with "}"
					if (curr_lex.table != op || curr_lex.num != CLOSE_BRACE)
					{
						throw "Sign } expected at the end of ELSE";
					}
					next_lex();				//getting first lexeme after "}"
					if (curr_lex.table != op || curr_lex.num != SEMICOLON)
					{
						throw "Sign } expected at the end of THEN";
					}
				}
				else
				{
					throw "Sign { expected at the begining of ELSE";
				}
				//filling the place of after_then label
				poliz.put_lex(Lexeme (POLIZ_LABEL, poliz.get_pos()), l_true);
			}
			else if (curr_lex.table != op || curr_lex.num != SEMICOLON)
			{
				throw "Sign ; expected at the end of command";
			}
		}
		else
		{
			throw "THEN expected after IF";
		}
	}	//<if> processed. Now <curr_lex> contains <SEMICOLON>
	else if (curr_lex.table == kw && curr_lex.num == WHILE)	//Command is of (while_do) type
	{
		l_loop = poliz.get_pos();	//saving iteration point
		next_lex();
		expr();
		l_end = poliz.get_pos();	//saving position of loop_end label
		poliz.put_space();			//allocating place
		poliz.put_lex(Lexeme (op, GOTO_ON_FALSE));
		if (curr_lex.table == kw && curr_lex.num == DO)
		{
			next_lex();
			////do_instr must start with "{"
			if (curr_lex.table == op && curr_lex.num == OPEN_BRACE)
			{
				next_lex();
				body();
				////and end with "}"
				if (curr_lex.table != op || curr_lex.num != CLOSE_BRACE)
				{
					cout << curr_lex.table << " " << curr_lex.num << endl;
					throw "Sign } expected at the end of WHILE";
				}
				next_lex();		//getting first lexeme after "{"
				if (curr_lex.table != op || curr_lex.num != SEMICOLON)	//it must be ";"
				{
					throw "Sign ; expected at the end of command";
				}
			}
			else
			{
				throw "Sign { expected at the beginning of WHILE";
			}
			poliz.put_lex(Lexeme (POLIZ_LABEL, l_loop));	//next iteration
			poliz.put_lex(Lexeme (op, GOTO));
			//filling the place of loop_end label
			poliz.put_lex(Lexeme (POLIZ_LABEL, poliz.get_pos()), l_end);
		}
		else
		{
			throw "DO expected after WHILE";
		}
	}	//<while> processed. Now <curr_lex> contains <SEMICOLON>
	else if (curr_lex.table == kw && curr_lex.num == LIRE)	//Command is (lire id)
	{
		next_lex();
		if (curr_lex.table == id)
		{
			//Here there is smth strange in <metodichka>
			//Check it later
			poliz.put_lex(curr_lex);
			poliz.put_lex(Lexeme (op, LIRE));
			next_lex();
		}
		else
		{
			throw "Identifier must follow LIRE";
		}
	}	//<lire> processed
	else if (curr_lex.table == kw && curr_lex.num == ECRIRE)	//Command is (ecrire expr)
	{
		next_lex();
		expr();
		poliz.put_lex(Lexeme (op, ECRIRE));
	}	//<ecrire> processed
	else if (curr_lex.table == id)		//Command is (id = expr)
	{
		poliz.put_lex(curr_lex);
		next_lex();
		if (curr_lex.table == op && curr_lex.num == ASSIGN)
		{
			next_lex();
			expr();
			poliz.put_lex(Lexeme (op, ASSIGN));
		}
		else
		{
			throw "Sign = expected";
		}
	}	//<assign> processed
	else if (curr_lex.table != op || curr_lex.num != CLOSE_BRACE)
	{
		throw "Unexpected lexeme in the beginning of a command";
	}
	if ((curr_lex.table != op) && (curr_lex.num != SEMICOLON || curr_lex.num != CLOSE_BRACE))
	{
		throw "Sign ; expected at the end of command";
	}
}

void Synt_analyzer::expr()
{
	sum();
	if (curr_lex.table == op && (curr_lex.num == EQU  ||
								 curr_lex.num == LESS ||
								 curr_lex.num == GRTR ||
								 curr_lex.num == LS_EQU ||
								 curr_lex.num == GR_EQU || 
								 curr_lex.num == NOT_EQU))
	{
		next_lex();
		sum();
	}
}

void Synt_analyzer::sum()
{
	prod();
	while (curr_lex.table == op && (curr_lex.num == PLUS  ||
									curr_lex.num == MINUS ||
									curr_lex.num == LOGIC_OR))
	{
		next_lex();
		prod();
	}
}

void Synt_analyzer::prod()
{
	oprnd();
	while (curr_lex.table == op && (curr_lex.num == MULT ||
									curr_lex.num == DIV  ||
									curr_lex.num == LOGIC_AND))
	{
		next_lex();
		prod();
	}
}

void Synt_analyzer::oprnd()
{
	if (curr_lex.table == id  ||
		curr_lex.table == num)
	{
		poliz.put_lex(curr_lex);
		next_lex();
	}
	else if (curr_lex.table == op && curr_lex.num == LOGIC_NOT)
	{
		next_lex();
		oprnd();
	}
	else if (curr_lex.table == op && curr_lex.num == OPEN_BRACKET)
	{
		next_lex();
		expr();
		if (curr_lex.table == op && curr_lex.num == CLOSE_BRACKET)
		{
			next_lex();
		}
		else
		{
			throw "Unpaired brackets";
		}
	}
	else
	{
		throw "Unexpected operand";
	}
}

void Synt_analyzer::TryGetLexemes (FILE* fs)
{
	char s [256];
	
	while ( fgets(s, sizeof(s), fs) )
    {
      try
		{
			lan.GetLexemes(s);
		}
      catch (const char *s)
		{
			throw;
		}
    }
}

void Synt_analyzer::process()
{
	try 
	{
		next_lex();
		prog();
	}
	catch (const char *s)
	{
		cerr << "OH NO MY GOD ERROR OCCURED: " << s << endl;
	}
}
