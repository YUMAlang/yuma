#ifndef _LALIB_H_
#define _LALIB_H_

#include <vector>
#include <iostream>
#include "exception.hpp"

using namespace std;

// Available tables
/*
    Tables are:
    0 - Operators
    1 - Key words
    2 - Identifiers
    3 - Strings
    4 - Numbers
    5 - POLIZ_LABEL -- just an imaginary table
    6 - ASSIGN_POS -- yet another imaginary table (thanks to Anton)
*/
enum Table
{
    op,
    kw,
    id,
    str,
    num,
    POLIZ_LABEL,
    ASSIGN_POS
};

enum Numbers_in_Op
{
    EQU,
    NOT_EQU,
    LOGIC_AND,
    LOGIC_OR,
    LOGIC_NOT,
    PLUS,
    MINUS,
    MULT,
    DIV, // 8
    ASSIGN,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    OPEN_BRACE,
    CLOSE_BRACE, // 13
    SEMICOLON,
    LESS,
    GRTR,
    LS_EQU,
    GR_EQU,
    GOTO,
    GOTO_ON_FALSE,
    LIRE,
    ECRIRE
};

enum Numbers_in_KW
{
    WHILE,
    DO,
    IF,
    THEN,
    ELSE
};

// Available types
enum Type
{
    undef_type,
    number_type,
    string_type,
    pol_lab_type,
    ass_pos_type
};

// Lexeme structure
struct Lexeme
{
    Table table;
    int num;
    Lexeme(Table t, int n)
        : table(t)
        , num(n)
    {
    }
    Lexeme()
    {
    } // Antonov constructor
};

// Identifier's value type
struct Value
{
    Type type;
    string s;
    double n;
    int i;

public:
    Value()
        : type(undef_type)
    {
    }

    Value(double);
    Value(string);
    Value(Type, int);

    bool Set(const string& s);

    bool Set(double n);

    bool Set(int i, bool is_pol_lab);

    Type GetType()
    {
		return type;
    }

    bool TryGetVal(string& s);

    bool TryGetVal(double& n);

    bool TryGetVal(int& i);
};

// Identifier class
class Identifier
{
    string name;
    Value val;

public:
    // Creates a new identifier with a given name
    Identifier(const char* n)
        : name(n)
    {
    }

    // Sets identifier's type and value; returns false on failure
    bool Set(const string& s)
    {
		return val.Set(s);
    }

    // Sets identifier's type and value; returns false on failure
    bool Set(double n)
    {
		return val.Set(n);
    }

    // Gets identificator's type
    Type GetType()
    {
		return val.GetType();
    }

    // Gets the Value structure
    Value GetValue()
    {
		return val;
    }

    // Tries to get a string from the value; returns false on failure
    bool TryGetVal(string& s)
    {
		return val.TryGetVal(s);
    }

    // Tries to get a double from the value; returns false on failure
    bool TryGetVal(double& n)
    {
		return val.TryGetVal(n);
    }

    friend ostream& operator<<(ostream& out, const Identifier& id);

    bool operator==(const Identifier& id)
    {
		return name == id.name;
    }
};

// Lexic_analyzer class
class Lexic_analyzer
{
    // Constant tables
    static const char* const Operators[];
    static const char* const KeyWords[];

    // Other tables
    vector<Lexeme> Lexemes;
    vector<Identifier> Identifiers;
    vector<string> Strings;
    vector<double> Numbers;

    // Returns the first operator (the longest have higher priority) in a string or NULL if not found; num will get the
    // number of it in the table
    char* GetOperator(char* s, int& num);

    // Tries to interprete a string as a number. Return false on failure
    bool TryGetNum(const char* s, double& d);

    // Returns a number in key words table or -1 on failure
    int GetKWNum(const char* s);

    // Returns true if the string is an identifier
    bool IsIdentifier(const char* s);

    // Tries to processes a word for a number, a key word or an identifier. Throws an exception in case of failure.
    void TryProcess(const char* s);

    // Add a Lexeme of given type and change the table
    template <class T> void AddLex(vector<T>& table, const T& val, Table type);

public:
    string get_string(int i)
    {
		return Strings.at(i);
    }
    double get_num(int i)
    {
		return Numbers.at(i);
    }
    Value get_value(int i)
    {
		return Identifiers.at(i).GetValue();
    }
    Type get_type(int i)
    {
		return Identifiers.at(i).GetType();
    }

    bool set_value(int i, Value val)
    {
	if(val.type == number_type)
	    {
			return Identifiers.at(i).Set(val.n);
	    }
	else if(val.type == string_type)
	    {
			return Identifiers.at(i).Set(val.s);
	    }
	else
	    return false;
    }

    /*
      This function parses a string and tries to split it into lexemes.
      In case of success fills the vector with lexemes.
      In case of failure throws an exception (const char *).
    */
    void GetLexemes(char* s);

    Lexeme get_lex();

    // Print the Lexemes table
    void PrintLex() const;
};
#endif
