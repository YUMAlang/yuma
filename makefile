LexAnalysis.o: LexAnalysis.cpp LexAnalysis.hpp
	g++ -g -Wall -static -c LexAnalysis.cpp -o LexAnalysis.o

POLIZ.o: POLIZ.cpp POLIZ.hpp
	g++ -g -Wall -static -c POLIZ.cpp -o POLIZ.o

syntAnalysis.o: SyntAnalysis.cpp syntAnalysis.hpp
	g++ -g -Wall -static -c SyntAnalysis.cpp -o syntAnalysis.o

Executor.o: Executor.cpp Executor.hpp
	g++ -g -Wall -static -c Executor.cpp -o Executor.o

program: program.cpp POLIZ.o LexAnalysis.o syntAnalysis.o Executor.o
	g++ -g -Wall -static program.cpp POLIZ.o LexAnalysis.o syntAnalysis.o Executor.o -o program

clear: 
	rm *.o; rm program

save: 
	(cat LexAnalysis.hpp ; cat LexAnalysis.cpp ; cat program.cpp) > saving
