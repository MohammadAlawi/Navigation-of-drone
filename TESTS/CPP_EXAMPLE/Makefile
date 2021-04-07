# *****************************************************
# Variables to control Makefile operation
 
CC = g++
CFLAGS = -Wall -g
 
# ****************************************************
# Targets needed to bring the executable up to date
 
main: main.o  MyLibrary.o
	cc -o		mainyeee	main.o	MyLibrary.o
 
# The main.o target can be written more simply
 
main.o:			main.cpp	MyLibrary.cpp	MyLibrary.hpp
	cc	main.cpp

MyLibrary.o:	main.cpp	MyLibrary.cpp	MyLibrary.hpp
	cc	MyLibrary.cpp	