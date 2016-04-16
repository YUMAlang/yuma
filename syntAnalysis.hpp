class Synt_analyzer
{
	/*
	 * Syntax checking tool
	 */ 
	Lexeme curr_lex;
	
	Lexic_analyzer lan;
	
	//Functions for syntax analyzing.
	void prog ();	//prog -> main () [{body}]
	void body ();	//body -> com {; com}
	void com ();	//com -> id = expr | if (expr) then (com) else (com) |
					//					 while (expr) do (com) |
				    //					 get (id) |
				    // 					 put (id)
	void expr ();	//expr -> sum [= | > | < | !=] sum | sum
	void sum ();	//sum -> prod {[+ | - | ||] prod}
	void prod ();	//prod -> oprnd {[* | / | &&] oprnd}
	void oprnd ();	//oprnd -> id | num | !oprnd | (expr)
	
	//See Gammar.txt for further info
	
	void next_lex ()
	{
		curr_lex = lan.get_lex();
	}
	
public:

	POLIZ poliz;
	Synt_analyzer (const char* program) : lan(), poliz (1000);
	void process ();
};
