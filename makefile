LexAnalysis.o: LexAnalysis.cpp LexAnalysis.hpp
	g++ -g -Wall -static -c LexAnalysis.cpp -o LexAnalysis.o

POLIZ.o: POLIZ.cpp POLIZ.hpp
	g++ -g -Wall -static -c POLIZ.cpp -o POLIZ.o

syntAnalysis.o: syntAnalysis.cpp syntAnalysis.hpp
	g++ -g -Wall -static -c syntAnalysis.cpp -o syntAnalysis.o

program: program.cpp POLIZ.o LexAnalysis.o syntAnalysis.o 
	g++ -g -Wall -static program.cpp POLIZ.o LexAnalysis.o syntAnalysis.o -o program

clear: 
	rm *.o; rm program

save: 
	(cat LexAnalysis.hpp ; cat LexAnalysis.cpp ; cat program.cpp) > saving
