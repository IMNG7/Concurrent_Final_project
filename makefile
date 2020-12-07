
#File Name: makefile
#Description: Makefile for the Lab0 for ECEN 5033
#Author: Nitik Gupta

DEPS = main.o treiber.o M_S_Queue.o Elimination.o sgl_stack_queue.o Flat_Combining.o
SRC = main.cpp treiber.cpp M_S_Queue.cpp Elimination.cpp sgl_stack_queue.cpp Flat_Combining.cpp
INCLUDES = treiber.h M_S_Queue.h Elimination.h Basket_Queue.h sgl_stack_queue.h Flat_Combining.h
CC = g++
LIBS = -pthread -fopenmp -latomic
CFLAGS = -g -O3

all: mysort

mysort: ${DEPS}
	g++ -o mysort -pthread -fopenmp -g ${SRC}

main.o: main.cpp
	g++ -o main.o -pthread -fopenmp -g -c main.cpp

treiber.o: treiber.cpp treiber.h
	g++ -o treiber.o -fopenmp -g -c treiber.cpp

M_S_Queue.o: M_S_Queue.cpp M_S_Queue.h
	g++ -o M_S_Queue.o -fopenmp -g -c M_S_Queue.cpp

sgl_stack_queue.o: sgl_stack_queue.cpp sgl_stack_queue.h
	g++ -o sgl_stack_queue.o -pthread -fopenmp -g -c sgl_stack_queue.cpp

Elimination.o: Elimination.cpp Elimination.h
	g++ -o Elimination.o -fopenmp -g -c Elimination.cpp

Flat_Combining.o: Flat_Combining.cpp Flat_Combining.h
	g++ -o Flat_Combining.o -pthread -fopenmp -g -c Flat_Combining.cpp

#Basket_Queue.o: Basket_Queue.cpp Basket_Queue.h
#	g++ -o Basket_Queue.o -fopenmp -g -c Basket_Queue.cpp

clean :
	rm mysort *.o
