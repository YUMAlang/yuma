#include <iostream>
#include <stdio.h>
#include "getlex.hpp"

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
  
  char s [256];
  Interpreter i;
  
  while ( fgets(s, sizeof(s), fs) )
    {
      try
	{
	  i.GetLex(s);
	}
      catch (const char *s)
	{
	  cerr << "Oops: " << s << endl;
	  return 1;
	}
    }

  i.PrintLex();
  return 0;
}
