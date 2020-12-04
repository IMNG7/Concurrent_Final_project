
#File Name: makefile
#Description: Makefile for the Lab0 for ECEN 5033
#Author: Nitik Gupta

DEPS = main.o treiber.o M_S_Queue.o
CC = g++
LIBS = -pthread
CFLAGS = -g -O3

all: mysort

mysort: ${DEPS}
	${CC} ${LIBS} ${CFLAGS} ${DEPS} -o $@ 

*.o: *.cpp *.h
	${CC} ${LIBS} ${CFLAGS} -c *.cpp

clean :
	rm mysort *.o
