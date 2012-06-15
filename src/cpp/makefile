#
# makefile for the upbdd project.
#

CC = g++ -Wall -g
TEST_L = bin/testBdd.o bin/testImp.o bin/testInput.o bin/testProblem.o
TEST_O = testBdd.o testImp.o testInput.o testProblem.o
TEST_H = testBdd.h testImp.h testInput.h testProblem.h
#CC = g++ -O3

all: test

test: test.o bdd.o imp.o impStore.o bddStore.o andStore.o backend.o input.o $(TEST_O)
	$(CC) $(TEST_L) bin/test.o bin/andStore.o bin/bddStore.o bin/impStore.o bin/bdd.o bin/imp.o bin/backend.o bin/input.o -o bin/test
	./bin/test

test.o: test.cpp backend.h impStore.h bddStore.h input.h $(TEST_H)
	$(CC) -o bin/test.o -c test.cpp  

input.o: input.h input.cpp 
	$(CC) -o bin/input.o -c input.cpp  

imp.o: imp.h imp.cpp 
	$(CC) -o bin/imp.o -c imp.cpp 

bdd.o: bdd.h bdd.cpp 
	$(CC) -o bin/bdd.o -c bdd.cpp 

impStore.o: impStore.h impStore.cpp  
	$(CC) -o bin/impStore.o -c impStore.cpp 

bddStore.o: bddStore.h bddStore.cpp  
	$(CC) -o bin/bddStore.o -c bddStore.cpp 

andStore.o: andStore.h andStore.cpp  
	$(CC) -o bin/andStore.o -c andStore.cpp 

backend.o: backend.h backend.cpp 
	$(CC) -o bin/backend.o -c backend.cpp 

testImp.o: testImp.h testImp.cpp 
	$(CC) -o bin/testImp.o -c testImp.cpp 

testBdd.o: testBdd.h testBdd.cpp 
	$(CC) -o bin/testBdd.o -c testBdd.cpp 

testInput.o: testInput.h testInput.cpp 
	$(CC) -o bin/testInput.o -c testInput.cpp 

testProblem.o: testProblem.h testProblem.cpp 
	$(CC) -o bin/testProblem.o -c testProblem.cpp 

common.h: common.h
imp.h: imp.h
bdd.h: bdd.h common.h
impStore.h: impStore.h common.h
bddStore.h: bddStore.h impStore.h
andStore.h: andStore.h impStore.h
backend.h: backend.h bdd.h imp.h impStore.h
input.h: input.h common.h

testImp.h: testImp.h 
testBdd.h: testBdd.h 
testInput.h: testInput.h 
testProblem.h: testProblem.h 

clean:
	rm -fr bin/*
