all: shell.c
	gcc shell shell.c parse.c

run: shell
./shell
