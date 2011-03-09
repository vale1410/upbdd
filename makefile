#
# makefile for the upbdd project.
#

CC = g++ -Wall -g
#CC = g++ -O3

all: start

start: upbdd
	./bin/start

upbdd: test.o bdd.o imp.o impStore.o bddStore.o backend.o input.o
	$(CC) bin/test.o bin/bddStore.o bin/impStore.o bin/bdd.o bin/imp.o bin/backend.o bin/input.o -o bin/start

test.o: src/test.cpp backend.h impStore.h bddStore.h input.h
	$(CC) -o bin/test.o -c src/test.cpp  

input.o: input.h src/input.cpp 
	$(CC) -o bin/input.o -c src/input.cpp  

imp.o: imp.h src/imp.cpp 
	$(CC) -o bin/imp.o -c src/imp.cpp 

bdd.o: bdd.h src/bdd.cpp 
	$(CC) -o bin/bdd.o -c src/bdd.cpp 

impStore.o: impStore.h src/impStore.cpp  
	$(CC) -o bin/impStore.o -c src/impStore.cpp 

bddStore.o: bddStore.h src/bddStore.cpp  
	$(CC) -o bin/bddStore.o -c src/bddStore.cpp 

backend.o: backend.h src/backend.cpp 
	$(CC) -o bin/backend.o -c src/backend.cpp 

common.h: src/common.h
imp.h: src/imp.h
bdd.h: src/bdd.h src/common.h
impStore.h: src/impStore.h src/common.h
bddStore.h: src/bddStore.h src/impStore.h
backend.h: src/backend.h src/bdd.h src/imp.h src/impStore.h
input.h: src/input.h src/common.h

clean:
	rm -fr bin/*
