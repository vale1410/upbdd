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

test.o: src/test.cpp backend.h impStore.h bddStore.h input.h $(TEST_H)
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

andStore.o: andStore.h src/andStore.cpp  
	$(CC) -o bin/andStore.o -c src/andStore.cpp 

backend.o: backend.h src/backend.cpp 
	$(CC) -o bin/backend.o -c src/backend.cpp 

testImp.o: testImp.h src/testImp.cpp 
	$(CC) -o bin/testImp.o -c src/testImp.cpp 

testBdd.o: testBdd.h src/testBdd.cpp 
	$(CC) -o bin/testBdd.o -c src/testBdd.cpp 

testInput.o: testInput.h src/testInput.cpp 
	$(CC) -o bin/testInput.o -c src/testInput.cpp 

testProblem.o: testProblem.h src/testProblem.cpp 
	$(CC) -o bin/testProblem.o -c src/testProblem.cpp 

common.h: src/common.h
imp.h: src/imp.h
bdd.h: src/bdd.h src/common.h
impStore.h: src/impStore.h src/common.h
bddStore.h: src/bddStore.h src/impStore.h
andStore.h: src/andStore.h src/impStore.h
backend.h: src/backend.h src/bdd.h src/imp.h src/impStore.h
input.h: src/input.h src/common.h

testImp.h: src/testImp.h 
testBdd.h: src/testBdd.h 
testInput.h: src/testInput.h 
testProblem.h: src/testProblem.h 

clean:
	rm -fr bin/*
