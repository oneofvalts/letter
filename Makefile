CFLAGS=-lncurses -Wextra -Wpadded -O0 -g3

build:
	cc main.c -o letter $(CFLAGS)
run:
	./letter
debug:
	gdb letter
valgrind:
	valgrind --tool=memcheck -s --track-origins=yes --leak-check=full --show-leak-kinds=all ./letter
