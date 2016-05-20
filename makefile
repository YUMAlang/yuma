LexAnalysis.o: exception.hpp LexAnalysis.cpp LexAnalysis.hpp
	g++ -g -Wall -static -c LexAnalysis.cpp -o LexAnalysis.o

SyntAnalysis.o: SyntAnalysis.cpp SyntAnalysis.hpp
	g++ -g -Wall -static -c SyntAnalysis.cpp -o SyntAnalysis.o

Executor.o: Executor.cpp Executor.hpp
	g++ -g -Wall -static -c Executor.cpp -o Executor.o

program: program.cpp LexAnalysis.o SyntAnalysis.o Executor.o
	g++ -g -Wall -static program.cpp LexAnalysis.o SyntAnalysis.o Executor.o -o program

clear: 
	rm *.o; rm program

save: 
	(cat *.hpp ; cat *.cpp) > saving
