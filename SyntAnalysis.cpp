// Dedicated to Kódryan

#include <stdio.h>
#include "SyntAnalysis.hpp"

void Synt_analyzer::prog(POLIZ &p)
{
  // Performing lexic analysis
  // The program must start with {
  if(curr_lex.table == op && curr_lex.num == OPEN_BRACE)
    {
      next_lex();
    }
  else
    {
      throw Exception("Sign { expected at the begining of the program");
    }
  body(p);
  // The program must finish with }
  if(curr_lex.table != op || curr_lex.num != CLOSE_BRACE)
    {
      cout << curr_lex.table << " " << curr_lex.num << endl;
      throw Exception("Sign } expected at the end of program");
    }
}

void Synt_analyzer::body(POLIZ &p)
{
  com(p);
  while(curr_lex.table == op && curr_lex.num == SEMICOLON)
    {
      next_lex();
      com(p);
    }
  // On '}' body(p) will finish without reading next lexeme
}

void Synt_analyzer::com(POLIZ &p)
{
  /* labels for POLIZ representation of <if> & <while>
   * if A then B else C
   * [A] [if (!A) then goto do_else] [B] [goto after_then] [C]
   *												 do_else^   ^after_then
   * POLIZ: ...[A][d_e][!goto][B][a_t][goto](d_e->[C])(a_t->)...
   */
  int l_true, l_false, l_loop, l_end;
  if(curr_lex.table == kw && curr_lex.num == IF) // Command is of (if_then_else) type
    {
      next_lex();
      expr(p);
      // Analyzing condition
      l_false = p.get_pos(); // saving position for do_else label
      p.put_space(); // allocating place in POLIZ for do_else label
      p.put_lex(Lexeme(op, GOTO_ON_FALSE));
      if(curr_lex.table == kw && curr_lex.num == THEN) // Analyzing then_instructions
	{
	  next_lex();
	  // then_instr must start with "{"
	  if(curr_lex.table == op && curr_lex.num == OPEN_BRACE)
	    {
	      next_lex();
	      body(p);
	      // and end with "}"
	      if(curr_lex.table != op || curr_lex.num != CLOSE_BRACE)
		{
		  throw Exception("Sign } expected at the end of IF");
		}
	      next_lex(); // getting first lexeme after "{"
	      // if it is ";", the if_instr ends here;
	    }
	  else
	    {
	      throw Exception("Sign { expected at the begining of THEN");
	    }

	  if(curr_lex.table == kw && curr_lex.num == ELSE)
	    {
	      l_true = p.get_pos(); // saving position for after_then label
	      p.put_space(); // allocating place
	      p.put_lex(Lexeme(op, GOTO));
	      // filling the place of else_do label
	      p.put_lex(Lexeme(POLIZ_LABEL, p.get_pos()), l_false);
	      next_lex();
	      // else_instr must start with "{"
	      if(curr_lex.table == op && curr_lex.num == OPEN_BRACE)
		{
		  next_lex();
		  body(p);
		  // and end with "}"
		  if(curr_lex.table != op || curr_lex.num != CLOSE_BRACE)
		    {
		      throw Exception("Sign } expected at the end of ELSE");
		    }
		  next_lex(); // getting first lexeme after "}"
		}
	      else
		{
		  throw Exception("Sign { expected at the begining of ELSE");
		}
	      // filling the place of after_then label
	      p.put_lex(Lexeme(POLIZ_LABEL, p.get_pos()), l_true);
	    }
	  else if(curr_lex.table == op && curr_lex.num == SEMICOLON)
	    {
	      p.put_lex(Lexeme(POLIZ_LABEL, p.get_pos()), l_false);
	    }
	}
      else
	{
	  throw Exception("THEN expected after IF");
	}
    } //<if> processed. Now <curr_lex> contains <SEMICOLON>
  
  else if(curr_lex.table == kw && curr_lex.num == WHILE) // Command is of (while_do) type
    {
      l_loop = p.get_pos(); // saving iteration point
      next_lex();
      expr(p);
      l_end = p.get_pos(); // saving position of loop_end label
      p.put_space(); // allocating place
      p.put_lex(Lexeme(op, GOTO_ON_FALSE));
      if(curr_lex.table == kw && curr_lex.num == DO)
	{
	  next_lex();
	  ////do_instr must start with "{"
	  if(curr_lex.table == op && curr_lex.num == OPEN_BRACE)
	    {
	      next_lex();
	      body(p);
	      ////and end with "}"
	      if(curr_lex.table != op || curr_lex.num != CLOSE_BRACE)
		{
		  cout << curr_lex.table << " " << curr_lex.num << endl;
		  throw Exception("Sign } expected at the end of WHILE");
		}
	      next_lex(); // getting first lexeme after "{"
	    }
	  else
	    {
	      throw Exception("Sign { expected at the beginning of WHILE");
	    }
	  p.put_lex(Lexeme(POLIZ_LABEL, l_loop)); // next iteration
	  p.put_lex(Lexeme(op, GOTO));
	  // filling the place of loop_end label
	  p.put_lex(Lexeme(POLIZ_LABEL, p.get_pos()), l_end);
	}
      else
	{
	  throw Exception("DO expected after WHILE");
	}
    } //<while> processed. Now <curr_lex> contains <SEMICOLON>
  
  else if (curr_lex.table == kw && curr_lex.num == FUNC) // Command is of (func) type: func <id> { body } 
    {
      next_lex();
      
      if (curr_lex.table != id)
	throw Exception("Wrong <func>-command syntax: expected identifier");
      
      int n = curr_lex.num;
      POLIZ buf;

      p.put_lex(Lexeme(ASSIGN_POS, n));
      p.put_lex(Lexeme(op, IS_FUNC)); //put the label to change type dynamically
      
      next_lex();
      prog(buf); //get the function's poliz
      next_lex();
      
      lan.set_value(n, buf); //set the identifier's poliz
    } //<func> processed
  
  else if(curr_lex.table == kw && curr_lex.num == LIRE_KW) // Command is (lire id)
    {
      next_lex();
      
      if(curr_lex.table == id)
	{
	  p.put_lex(Lexeme(ASSIGN_POS, curr_lex.num));
	  next_lex();
	  
	  if (curr_lex.table == op && curr_lex.num == OPEN_SQ_BRACKET)
	    {
	      next_lex();
	      
	      if (curr_lex.table != id && curr_lex.table != num)
		{
		  throw Exception ("Array index must be a number");
		}
	      
	      p.put_lex (curr_lex);
	      next_lex();
	      
	      if (curr_lex.table != op || curr_lex.num != CLOSE_SQ_BRACKET)
		{
		  throw Exception ("Expected ] after array index");
		}
	      
	      p.put_lex (Lexeme(op, LIRE_ELEM));
	      next_lex();
	    }
	  
	  else
	    {
	      p.put_lex(Lexeme(op, LIRE));
	    }
	}
      else
	{
	  throw Exception("Identifier must follow LIRE");
	}
    } //<lire> processed
  
  else if(curr_lex.table == kw && curr_lex.num == ECRIRE_KW) // Command is (ecrire expr)
    {
      int ecr_count = 0;
      do
	{
	  next_lex();
	  expr(p);
	  ecr_count++;
	}
      while (curr_lex.table == op && curr_lex.num == COMMA);
      p.put_lex(Lexeme(POLIZ_LABEL, ecr_count));
      p.put_lex(Lexeme(op, ECRIRE));
    } //<ecrire> processed
  
  else if(curr_lex.table == id) // Command is (id = expr) or (id()) or id[]
    {
      int n = curr_lex.num;
      next_lex();

      if (curr_lex.table == op && curr_lex.num == OPEN_SQ_BRACKET) // a[10] = ...
	{
	  p.put_lex(Lexeme(ASSIGN_POS, n));
	  next_lex();
	  try
	    {
	      expr(p);
	    }
	  catch(Exception& ex)
	    {
	      string s = "-array index must be a calculateble expression-";
	      s += ex.what();

	      throw Exception(s);
	    }
	  
	  if (curr_lex.table != op || curr_lex.num != CLOSE_SQ_BRACKET)
	    {
	      throw Exception ("Expected ] after array index");
	    }
	  
	  next_lex();
	  
	  if (curr_lex.table == op && curr_lex.num == OPEN_SQ_BRACKET)
	    {
	      throw Exception ("multidimensional arrays are strictly prohibited. All your code will be deleted");
	    }
	  
	  if (curr_lex.table == op && curr_lex.num == ASSIGN)
	    {
	      next_lex();
	      expr(p);
	      p.put_lex(Lexeme(op, ASSIGN_ELEM));
	    }
	  else throw Exception ("sign = expected");
	}
      
      else if(curr_lex.table == op && curr_lex.num == ASSIGN)
	{
	  p.put_lex(Lexeme(ASSIGN_POS, n));
	  next_lex();
	  
	  if (curr_lex.table == kw && curr_lex.num == ARRAY)
	    {
	      p.put_lex(Lexeme (op, CREATE_ARRAY));
	      next_lex();
	    }
	  else
	    {
	      expr(p);
	      p.put_lex(Lexeme(op, ASSIGN));
	    }
	} //<assign> processed
      
      else if (curr_lex.table == op && curr_lex.num == OPEN_BRACKET)
	{
	  next_lex();
	  if (curr_lex.table != op || curr_lex.num != CLOSE_BRACKET)
	    throw Exception("Wrong <func()>-command syntax: ')' expected");
	  p.put_lex(Lexeme(id, n));
	  p.put_lex(Lexeme(op, RUN));
	  next_lex();
	} //<func()> processed
      else
	{
	  throw Exception("Sign = or () expected");
	}
    } 
  
  else if(curr_lex.table == op && curr_lex.num == CLOSE_BRACE)
    {
      return;
    }
  
  if(curr_lex.table != op || curr_lex.num != SEMICOLON)
    {
      throw Exception("Sign ; expected at the end of command");
    }
}

void Synt_analyzer::expr(POLIZ &p)
{
  Lexeme buf_lex;
  sum(p);
  if(curr_lex.table == op && (curr_lex.num == EQU || curr_lex.num == LESS || curr_lex.num == GRTR ||
			      curr_lex.num == LS_EQU || curr_lex.num == GR_EQU || curr_lex.num == NOT_EQU))
    {
      buf_lex = curr_lex; // memorize the operation
      next_lex();
      sum(p);
      p.put_lex(buf_lex); // put the operation to POLIZ after operands
    }
}

void Synt_analyzer::sum(POLIZ &p)
{
  Lexeme buf_lex;
  prod(p);
  while(curr_lex.table == op && (curr_lex.num == PLUS || curr_lex.num == MINUS || curr_lex.num == LOGIC_OR))
    {
      buf_lex = curr_lex;
      next_lex();
      prod(p);
      p.put_lex(buf_lex);
    }
}

void Synt_analyzer::prod(POLIZ &p)
{
  Lexeme buf_lex;
  oprnd(p);
  while(curr_lex.table == op && (curr_lex.num == MULT || curr_lex.num == DIV || curr_lex.num == LOGIC_AND))
    {
      buf_lex = curr_lex;
      next_lex();
      oprnd(p);
      p.put_lex(buf_lex);
    }
}

void Synt_analyzer::oprnd(POLIZ &p)
{
  if (curr_lex.table == num || curr_lex.table == str)
    {
      p.put_lex(curr_lex);
      next_lex();
    }
  
  else if(curr_lex.table == id)
    {
      p.put_lex(curr_lex);
      next_lex();
      
      if (curr_lex.table == op && curr_lex.num == OPEN_SQ_BRACKET)
	{
	  next_lex();
	  
	  try
	    {
	      expr(p);
	    }
	  catch(Exception& ex)
	    {
	      string s = "-array index must be a calculateble expression-";
	      s += ex.what();

	      throw Exception(s);
	    }
	  
	  p.put_lex(Lexeme (op, GET_ELEM));
	  
	  if (curr_lex.table != op || curr_lex.num != CLOSE_SQ_BRACKET)
	    {
	      throw Exception ("Expected ] after array index");
	    }
	  
	  next_lex();
	  
	  if (curr_lex.table == op && curr_lex.num == OPEN_SQ_BRACKET)
	    {
	      throw Exception ("multidimensional arrays are strictly prohibited. All your code will be deleted");
	    }
	}
    }
  
  else if(curr_lex.table == op && curr_lex.num == LOGIC_NOT)
    {
      next_lex();
      oprnd(p);
      p.put_lex(Lexeme(op, LOGIC_NOT));
    }
  
  else if(curr_lex.table == op && curr_lex.num == OPEN_BRACKET)
    {
      next_lex();
      expr(p);
      
      if(curr_lex.table == op && curr_lex.num == CLOSE_BRACKET)
	{
	  next_lex();
	}
      
      else
	{
	  throw Exception("Closing bracket expected at the end of expression");
	}
    }
  
  else
    {
      throw Exception("Operand expected");
    }
}

void Synt_analyzer::TryGetLexemes(FILE* fs)
{
  char s[256];

  while(fgets(s, sizeof(s), fs))
    {
      try
	{
	  lan.GetLexemes(s);
	}
      catch(Exception& ex)
	{
	  string s = "Lexic error: ";
	  s += ex.what();

	  throw Exception(s);
	}
    }
}

void Synt_analyzer::process()
{
  try
    {
      next_lex();
      prog(poliz);
    }
  catch(Exception& ex)
    {
      string s = "Syntax error: ";
      s += ex.what();

      throw Exception(s);
    }
}
