CC = gcc
FLAGS = -Wall -Wextra -g

gol: gol.c
	$(CC) gol.c $(FLAGS) -o gol

run: gol
	./gol
