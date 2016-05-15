#ifndef _EXELIB_H_
#define _EXELIB_H_

#include "SyntAnalysis.hpp"
#include <vector>
#include <string>
#include <stack>

class Executor
{
    stack<Value> args;

public:
    Synt_analyzer san;

    void execute();
};

#endif
