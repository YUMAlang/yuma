#ifndef _LALIB_H_
#define _LALIB_H_

#include <vector>
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
    5 - POLIZ_LABEL -- just an imaginary table
*/
enum Table { op, kw, id, str, num, POLIZ_LABEL};

enum Numbers_in_Op
{
	EQU, NOT_EQU, LOGIC_AND, LOGIC_OR, LOGIC_NOT, PLUS, MINUS, MULT, DIV,
	ASSIGN, OPEN_BRACKET, CLOSE_BRACKET, OPEN_BRACE, CLOSE_BRACE,
	SEMICOLON, LESS, GRTR, LS_EQU, GR_EQU, GOTO, GOTO_ON_FALSE
};

enum Numbers_in_KW
{
	WHILE, DO, IF, THEN, ELSE, LIRE, ECRIRE
};

//Available types
enum Type { undef_type, number_type, string_type };


//Lexeme structure
struct Lexeme
{
  Table table;
  int num;
  Lexeme (Table t, int n): table(t), num(n) {}
  Lexeme () {} //Antonov constructor
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
  //Constant tables
  static const char *const Operators[];
  static const char *const KeyWords[];
  
  //Other tables
  vector<Lexeme> Lexemes;
  vector<Identifier> Identifiers;
  vector<string> Strings;
  vector<double> Numbers;

  //Returns the first operator (the longest have higher priority) in a string or NULL if not found; num will get the number of it in the table
  char* GetOperator(char *s, int &num);

  //Tries to interprete a string as a number. Return false on failure
  bool TryGetNum(const char *s, double &d);

  //Returns a number in key words table or -1 on failure
  int GetKWNum (const char *s);

  //Returns true if the string is an identifier
  bool IsIdentifier (const char *s);

  //Tries to processes a word for a number, a key word or an identifier. Throws an exception in case of failure.
  void TryProcess (const char *s);
public:
  /*
    This function parses a string and tries to split it into lexemes.

    In case of success fills the vector with lexemes.
    In case of failure throws an exception (const char *).
  */
  void GetLexemes (char *s);
  Lexeme get_lex ();
  
  //Print the Lexemes table
  void PrintLex () const;
};
#endif