#include <iostream>
#include <stdio.h>
#include "Executor.hpp"

using namespace std;

int main (int argc, char **argv)
{
  FILE* fs;
  
  if (argc == 1)
    fs = stdin;
  else if (argc == 2)
    {
    if (! (fs = fopen(argv[1], "r")) )
      return 1;
    }
  else return 1;
  
  Executor ex;
  Synt_analyzer i;
  
  try
  {
	ex.san.TryGetLexemes(fs);
	ex.san.process();
	ex.execute();
  }
  catch (...)
	{
	  return 1;
	}
  
  return 0;
}
