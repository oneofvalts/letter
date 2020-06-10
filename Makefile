CFLAGS=-lncurses -Wextra -Wpadded -O0 -g3

build:
	cc main.c $(CFLAGS)
run:
	./a.out
debug:
	gdb a.out
valgrind:
	valgrind --tool=memcheck -s --track-origins=yes --leak-check=full --show-leak-kinds=all ./a.out
