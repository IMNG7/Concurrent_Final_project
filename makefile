
#File Name: makefile
#Description: Makefile for the Lab0 for ECEN 5033
#Author: Nitik Gupta

DEPS = main.o treiber.o M_S_Queue.o Elimination.o
SRC = main.cpp treiber.cpp M_S_Queue.cpp Elimination.cpp
INCLUDES = treiber.h M_S_Queue.h Elimination.h
CC = g++
LIBS = -pthread -fopenmp
CFLAGS = -g -O3

all: mysort

mysort: ${DEPS}
	g++ -o mysort -fopenmp -g main.cpp treiber.cpp M_S_Queue.cpp Elimination.cpp

main.o: main.cpp
	g++ -o main.o -fopenmp -g -c main.cpp

treiber.o: treiber.cpp treiber.h
	g++ -o treiber.o -fopenmp -g -c treiber.cpp

M_S_Queue.o: M_S_Queue.cpp M_S_Queue.h
	g++ -o M_S_Queue.o -fopenmp -g -c M_S_Queue.cpp

Elimination.o: Elimination.cpp Elimination.h
	g++ -o Elimination.o -fopenmp -g -c Elimination.cpp

clean :
	rm mysort *.o
