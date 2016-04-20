getlex.o: getlex.cpp getlex.hpp
	g++ -g -Wall -c getlex.cpp -o getlex.o

program: program.cpp getlex.o
	g++ -g -Wall program.cpp getlex.o -o program

clear: 
	rm *.o; rm program

save: 
	(cat getlex.hpp ; cat getlex.cpp ; cat program.cpp) > saving
