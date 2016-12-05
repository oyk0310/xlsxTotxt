CC=gcc -g -rdynamic
CXX=g++

CPPS=xlsxTotxt.c
OBJ=xlsxTotxt
CINC = -I. -I /usr/local/include/libxml2 
CFLAGS=-g -O0 -Wall -o
CLIBS=-L. -lxml2
 
$(OBJ):$(CPPS)
	$(CXX) $(CPPS) $(CLIBS) $(CINC) $(CFLAGS) $(OBJ) 

clean:
	rm -f $(OBJ)

