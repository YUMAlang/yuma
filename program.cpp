#include <stdio.h>
#include "Executor.hpp"

using namespace std;

int main(int argc, char** argv)
{
    FILE* fs;

    if(argc == 1)
	fs = stdin;
    else if(argc == 2)
	{
	    if(!(fs = fopen(argv[1], "r")))
		return 1;
	}
    else
	return 1;

    Executor ex;

    try
	{
	    ex.san.TryGetLexemes(fs);
	    ex.san.process();
	    // ex.san.poliz.print();
	    // cout << "\n";
	    ex.execute();
	}
    catch(Exception& ex)
	{
	    cerr << "Please, God, NO! " << ex.what() << endl;
	    return 1;
	}
    catch(...)
	{
	    cerr << "Wow! An unknown error here!" << endl;
	    return 1;
	}

    return 0;
}
