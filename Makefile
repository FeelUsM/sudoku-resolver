sudoku : main.o defs.o evaluation.o
	g++ *.o -o sudoku
main.cpp evaluation.cpp : interface.h
defs.cpp : defs.h
defs.h interface.h : common.h

clean : 
	rm sudoku *.o
