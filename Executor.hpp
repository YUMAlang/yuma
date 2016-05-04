#include <stack>
#include "LexAnalysis.hpp"
#include "syntAnalysis.hpp"
#include "POLIZ.hpp"
#include "string"

using namespace std;

class Executor
{
	Synt_analyzer san;
	void execute(vector<Lexeme> code);
	void get2ops(stack<double>& st, double& op1, double& op2);
	void get2ops(stack<string>& st, string& op1, string& op2);
};

enum type_of_stack { num, str };
