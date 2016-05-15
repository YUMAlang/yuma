#include <string.h>
#include <ctype.h>
#include <algorithm>
#include "LexAnalysis.hpp"

using namespace std;

Value::Value(string s)
{
    type = string_type;
    this->s = s;
}

Value::Value(double n)
{
    type = number_type;
    this->n = n;
}

Value::Value(Type t, int i)
{
    type = t;
    this->i = i;
}

bool Value::Set(const string& s)
{
    type = string_type;
    this->s = s;
    return true;
}

bool Value::Set(double n)
{
    type = number_type;
    this->n = n;
    return true;
}

bool Value::Set(int i, bool is_pol_lab)
{
    if(type == undef_type || type == pol_lab_type || type == ass_pos_type)
	{
	    if((type == pol_lab_type) != is_pol_lab)
		return false;

	    type = is_pol_lab ? pol_lab_type : ass_pos_type;
	    this->i = i;
	    return true;
	}
    else
	return false;
}

bool Value::TryGetVal(string& s)
{
    if(type != string_type)
	return false;

    s = this->s;
    return true;
}

bool Value::TryGetVal(double& n)
{
    if(type != number_type)
	return false;

    n = this->n;
    return true;
}

bool Value::TryGetVal(int& i)
{
    if(type != pol_lab_type || type != ass_pos_type)
	return false;

    i = this->i;
    return true;
}

ostream& operator<<(ostream& out, const Identifier& id)
{
    out << id.name;
    return out;
}

const char* const Lexic_analyzer::Operators[] = { "==", "!=", "&&", "||", "!", "+", "-", "*", "/", "=", "(", ")", "{",
    "}", ";", "<", ">", "<=", ">=", "goto", "goto_f", "lire", "ecrire" };
const char* const Lexic_analyzer::KeyWords[] = { "while", "do", "if", "then", "else" };

char* Lexic_analyzer::GetOperator(char* s, int& num)
{
    static int size = sizeof(Operators) / sizeof(char*);

    char* result = NULL;
    unsigned int len;

    for(int i = 0; i < size; i++)
	{
	    char* tmp = strstr(s, Operators[i]);

	    if(tmp)
		if(!result || result > tmp || (result == tmp && len < strlen(Operators[i]))) // Bad-bad check :(
		    {
			result = tmp;
			num = i;
			len = strlen(Operators[num]);
		    }
	}

    return result;
}

bool Lexic_analyzer::TryGetNum(const char* s, double& d)
{
    if(!s || !s[0])
	return false;

    double res = 0, frac = 1;
    bool got_dot = false;

    for(int i = 0; s[i]; i++)
	if(isdigit(s[i]))
	    {
		res = 10 * res + (s[i] - '0');
		if(got_dot)
		    frac *= 10;
	    }
	else if('.' == s[i])
	    {
		if(!got_dot)
		    got_dot = true;
		else
		    return false;
	    }
	else
	    return false;

    d = res / frac;
    return true;
}

int Lexic_analyzer::GetKWNum(const char* s)
{
    static int size = sizeof(KeyWords) / sizeof(char*);

    for(int i = 0; i < size; i++)
	if(0 == strcmp(s, KeyWords[i]))
	    return i;

    return -1;
}

bool Lexic_analyzer::IsIdentifier(const char* s)
{
    if(!s || !s[0])
	return false;

    if(!(isalpha(s[0]) || s[0] == '_'))
	return false;

    for(int i = 1; s[i]; i++)
	if(!(isalnum(s[i]) || s[i] == '_'))
	    return false;

    return true;
}

template <class T> void Lexic_analyzer::AddLex(vector<T>& table, const T& val, Table type)
{
    typename vector<T>::iterator it;

    if((it = find(table.begin(), table.end(), val)) == table.end())
	{
	    table.push_back(val); // add the value in the table
	    Lexemes.push_back(Lexeme(type, table.size() - 1));
	}
    else
	{
	    Lexemes.push_back(Lexeme(type, distance(table.begin(), it)));
	}
}

void Lexic_analyzer::TryProcess(const char* s)
{
    const char* const IDENT_ERR = "Bad identifier";
    const char* const NUM_ERR = "Bad number";

    int n = GetKWNum(s);

    if(-1 != n)
	Lexemes.push_back(Lexeme(kw, n));

    else if(isdigit(s[0]) || '.' == s[0])
	{
	    double res;
	    if(TryGetNum(s, res))
		AddLex(Numbers, res, num);
	    else
		throw Exception(NUM_ERR);
	}

    else if(IsIdentifier(s))
	AddLex(Identifiers, Identifier(s), id);

    else
	throw Exception(IDENT_ERR);
}

void Lexic_analyzer::GetLexemes(char* s)
{
#define SPACE_TOKENS " \n\t\r"

    const char* const COMMA_ERR = "Not paired commas";

    // the commas and comments check
    if(s)
	{
	    char *comma_start, *comma_end, *comment_start;

	    comment_start = strstr(s, "#");
	    comma_start = strstr(s, "\"");

	    if(comment_start)
		{
		    if(!comma_start || comment_start < comma_start)
			{
			    *comment_start = '\0';

			    GetLexemes(s);

			    return;
			}
		}

	    if(comma_start) // no comment - check commas
		{
		    if(!(comma_end = strstr(comma_start + 1, "\"")))
			throw Exception(COMMA_ERR);

		    *comma_start = '\0';
		    *comma_end = '\0';

		    GetLexemes(s); // interprete the first part

		    AddLex(Strings, string(comma_start + 1), str);

		    GetLexemes(comma_end + 1); // interprete the last part

		    return;
		}
	}

    // split the string into space-tokens, get a long word

    char* long_word = strtok(s, SPACE_TOKENS);
    if(long_word == NULL)
	return;

    // now make words from a long word

    char* cur_pos = long_word;
    char* next_pos;
    int num;

    // process words in cycle while there are operators in the long word
    while((next_pos = GetOperator(cur_pos, num)))
	{
	    if(next_pos != cur_pos)
		{
		    // get a word
		    int n = next_pos - cur_pos;
		    char buf[n + 1];
		    strncpy(buf, cur_pos, n);
		    buf[n] = '\0';

		    // try to process it
		    try
			{
			    TryProcess(buf);
			}
		    catch(...)
			{
			    throw;
			}
		}

	    Lexemes.push_back(Lexeme(op, num));

	    cur_pos = next_pos + strlen(Operators[num]); // move to the next position
	}

    // process the rest
    if(*cur_pos)
	{
	    try
		{
		    TryProcess(cur_pos);
		}
	    catch(...)
		{
		    throw;
		}
	}

    // and the tail
    GetLexemes(NULL);
}

void Lexic_analyzer::PrintLex() const
{
    for(vector<Lexeme>::const_iterator it = Lexemes.begin(); it != Lexemes.end(); ++it)
	{
	    cout << "Table: ";

	    switch(it->table)
		{
		case op:
		    cout << "Operators; Value: " << Operators[it->num];
		    break;

		case kw:
		    cout << "Key words; Value: " << KeyWords[it->num];
		    break;

		case id:
		    cout << "Identifiers; Value: " << Identifiers[it->num];
		    break;

		case str:
		    cout << "Strings; Value: " << Strings[it->num];
		    break;

		case num:
		    cout << "Numbers; Value: " << Numbers[it->num];
		    break;

		default:
		    break;
		}

	    cout << "; Position: " << it->num << endl;
	}
}

Lexeme Lexic_analyzer::get_lex()
{
    static unsigned int i = 0; // current position in Lexeme vector
    if(i < Lexemes.size())
	{
	    return Lexemes.at(i++);
	}
    else
	{
	    throw Exception("END_OF_FILE");
	}
}
