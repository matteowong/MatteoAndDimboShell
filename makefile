all: shell.c
	gcc -o shell shell.c parse.c

run: shell
	./shell
