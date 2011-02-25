# makefile for the upbdd project.
#
#

all: upbdd

upbdd: src/bdd.cpp
	g++ -Wall -g src/bdd.cpp -o bin/start

start: bin/start
	./bin/start

clean:
	rm -fr bin/*
