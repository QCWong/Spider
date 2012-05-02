OBJECT = nodeURL.o spider.o 
all : main
.c.o:
	g++ -c -g -o $@ $<

main : $(OBJECT) main.cpp 
	g++ -g $(OBJECT) main.cpp -o spider

clean :
	rm -f *.o 
