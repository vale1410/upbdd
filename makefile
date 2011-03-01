#
# makefile for the upbdd project.
#

CC = g++ -Wall -g

all: start

start: upbdd
	./bin/start

upbdd: bdd.o imp.o impStore.o bddStore.o backend.o
	$(CC) src/test.cpp bin/bddStore.o bin/impStore.o bin/bdd.o bin/imp.o bin/backend.o -o bin/start

imp.o: src/imp.cpp common.h imp.h
	$(CC) -o bin/imp.o -c src/imp.cpp 

bdd.o: src/bdd.cpp imp.o common.h  
	$(CC) -o bin/bdd.o -c src/bdd.cpp 

impStore.o: src/impStore.cpp imp.o common.h impStore.h
	$(CC) -o bin/impStore.o -c src/impStore.cpp 

bddStore.o: src/bddStore.cpp bdd.o common.h bddStore.h
	$(CC) -o bin/bddStore.o -c src/bddStore.cpp 

backend.o: src/backend.cpp impStore.o bddStore.o common.h backend.h
	$(CC) -o bin/backend.o -c src/backend.cpp 

common.h: src/common.h
backend.h: src/backend.h
impStore.h: src/impStore.h
bddStore.h: src/bddStore.h
imp.h: src/imp.h

clean:
	rm -fr bin/*
