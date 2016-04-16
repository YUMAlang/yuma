#ifndef _LALIB_H_
#define _LALIB_H_

#include <vector>
#include <string>
#include <iostream>

using namespace std;

//Available tables
/*
    Tables are:
    0 - Operators
    1 - Key words
    2 - Identifiers
    3 - Strings
    4 - Numbers
	5 - POLIZ_LABEL: just an imaginary number, not a table
*/
enum Table { op, kw, id, str, num, POLIZ_LABEL };

//Available types
enum Type { undef_type, number_type, string_type };


//Lexeme structure
struct Lexeme
{
  Table table;
  int num;
  Lexeme (Table t, int n): table(t), num(n) {}
};


//Identifier class
class Identifier
{
  //Identifier's value type
  union Value
  {
    string *s;
    double n;
  };

  string name;
  Type type;
  Value val;

public:
  //Creates a new identificator with a given name
  Identifier (const char *n): name(n), type(undef_type){}

  //Sets identificator's type and value; returns false on failure
  bool Set (const string &s);

  //Sets identificator's type and value; returns false on failure
  bool Set (double n);

  //Gets identificator's type
  Type GetType () { return type; }

  //Tries to get a string from the value; returns false on failure
  bool TryGetVal (string &s);

  //Tries to get a double from the value; returns false on failure
  bool TryGetVal (double &n);

  ~Identifier () { if (type == string_type) delete val.s; }

  friend ostream& operator<< (ostream &out, const Identifier &id);
};


//Lexic_analyzer class
class Lexic_analyzer
{
public:

  //Constant tables
  static const char *const Operators[];
  static const char *const KeyWords[];

  //Other tables
  vector<Lexeme> Lexemes;
  vector<Identifier> Identifiers;
  vector<string> Strings;
  vector<double> Numbers;
  
private:

  //Returns the first operator in a string or NULL if not found; len will get the length of the found operator and n the number of it in the table
  char* GetOperator(char *s, int &len, int &n);

  //Tries to interprete a string as a number. Return false on failure
  bool TryGetNum(const char *s, double &d);

  //Returns a number in key words table or -1 on failure
  int GetKWNum (const char *s);

  //Returns true if the string is an identifier
  bool IsIdentifier (const char *s);

  //Tries to processes a word. Throws an exception in case of failure.
  void TryProcess (const char *s);
  
public:

	Lexic_analyzer ()
	{
		Lexemes.clear();
	}
  /*
    This function parses a string and tries to split it into lexemes.

    In case of success fills the vector with lexemes.
    In case of failure throws an exception (const char *).
  */
  void get_Lexemes (char *s);
  void PrintLex () const;
  /*
   * returns next lexeme from Lexemes
   */
  Lexeme get_lex ();
};

Lexeme Lexic_analyzer::get_lex()
{
	static i = 0;	//current position in Lexeme vector
	if (i < Lexemes.size())
	{
		i++;
		return Lexemes.at(i);
	}
	else			//current line processed
	{
		get_Lexemes (s);	//receiving next line
		i = 0;
		return Lexemes.at(0);
	}	  
}
#endif