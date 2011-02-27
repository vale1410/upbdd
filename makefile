#
# makefile for the upbdd project.
#

CC = g++ -Wall -g 

all: start

start: upbdd
	./bin/start

upbdd: bdd.o imp.o impStore.o common.h
	$(CC) bin/impStore.o bin/bdd.o bin/imp.o -o bin/start

imp.o: src/imp.cpp common.h 
	$(CC) -o bin/imp.o -c src/imp.cpp 

bdd.o: src/bdd.cpp imp.o impStore.o common.h  
	$(CC) -o bin/bdd.o -c src/bdd.cpp 

impStore.o: src/impStore.cpp imp.o common.h
	$(CC) -o bin/impStore.o -c src/impStore.cpp 

common.h: src/common.h
clean:
	rm -fr bin/*
