#ifndef _EXLIB_H_
#define _EXLIB_H_

#include <stack>
#include "LexAnalysis.hpp"
#include "syntAnalysis.hpp"
#include "POLIZ.hpp"
#include <string>

using namespace std;

struct Executor
{
	Synt_analyzer san;
	void execute();
	void get2ops(stack<double>& st, double& op1, double& op2);
	void get2ops(stack<string>& st, string& op1, string& op2);
};

enum type_of_stack { num_stack, str_stack };

#endif
