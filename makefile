OBJECT = nodeURL.o Spider.o 

all : main

nodeURL.o : nodeURL.cpp nodeURL.h 
	g++ -c -g nodeURL.cpp -o nodeURL.o

Spider.o : Spider.cpp Spider.h
	g++ -c -g Spider.cpp -o Spider.o

main : $(OBJECT) main.cpp 
	g++ -g $(OBJECT) main.cpp -o main

clean :
	rm -f *.o 
