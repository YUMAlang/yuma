#include <iostream>
#include <stdio.h>
#include "syntAnalysis.hpp"

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
  
  Synt_analyzer i;
  
  try
  {
	i.TryGetLexemes(fs);
  }
  catch (const char *s)
	{
	  cerr << "Oops: " << s << endl;
	  return 1;
	}
	i.process();
	
  cout << "Wow! It really works!" << endl;
  
  return 0;
}
