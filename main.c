#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define ESCAPE 033
#define BACKSPACE 074 // <
#define ENTER 044 // $
#define BACKLINE 0136 // ^

struct letter {
    size_t index;
    char content;
    char pad[7];
};

void backspace(char *src, struct letter *dest, size_t srclen) {
    int y,x;
    getyx(stdscr,y,x);
    char tbrestored;
    tbrestored = mvinch(y,x-1) & A_CHARTEXT;
    for (size_t i=0;i<srclen;i++) {
        if (tbrestored==dest[i].content) {
            src[dest[i].index] = dest[i].content;
            dest[i].content = 0;
            dest[i].index = 0;
            move(0,0);
            printw("%s", src);
            move(y,x);
            mvaddch(y,x-1,'*');
            move(y,x-1);
            refresh();
            break;
        }
    }
    refresh();
}

void enter(size_t *al, int *linepos, char **src, int row) {
    int y,x;
    getyx(stdscr, y,x);
    if (y<row-1) {
        (*al)++;
        move(0,0);
        printw("%s", src[y]);
        move(y+1,linepos[y]);
        refresh();
        linepos[y-1] = x;
    }
}

void backline(size_t *al, int *linepos, char **src) {
    int y,x;
    getyx(stdscr, y,x);
    if (y>1) {
        (*al)--;
        move(0,0);
        printw("%s", src[y-2]);
        move(y-1,linepos[y-2]);
        refresh();
        linepos[y-1] = x;
    }
}

void type(char ch, char *src, struct letter *dest, size_t srclen) {
    int y,x;
    for (size_t i=0;i<srclen;i++) {
        if (ch==src[i]) {
            addch(ch);
            src[i] = '.';
            int y,x;
            getyx(stdscr, y,x);
            move(0,0);
            printw("%s", src);
            move(y,x);
            dest[i].content = ch;
            dest[i].index = i;
            refresh();
            getyx(stdscr, y,x);
            break;
        }
    }
    refresh();
}

int main(void) {
    size_t line_length = 196;
    char line[line_length+2];

    printf("type the line you want to variate: ");
    fgets(line, line_length+2, stdin);

    size_t real_length = 0;
    for (size_t i=0;i<line_length+2;i++)
    {
        if (line[i] != '\n')
        {
            real_length++;
        }
        else
        {
            break;
        }
    }

    int last_key = ENTER;

    initscr(); raw(); noecho();

    unsigned int row, col;
    getmaxyx(stdscr, row, col);

    char *source[row-1];

    for (size_t i=0;i<row-1;i++) {
        source[i] = malloc((real_length + 1) * sizeof(char));
    }
    for (size_t i=0;i<row-1;i++) {
        for (size_t k=0;k<real_length;k++) {
            source[i][k] = line[k];
        }
    }
    for (size_t i=0;i<row-1;i++) {
        source[i][real_length] = 0;
    }

    struct letter (*page[row-1])[real_length];
    for (size_t i=0;i<row-1;i++) {
        page[i] = malloc(sizeof(struct letter[real_length]));
    }
    for (size_t i=0;i<row-1;i++) {
        for (size_t k=0;k<real_length;k++) {
            (*page[i])[k].index = 0;
            (*page[i])[k].content = 0;
        }
    }

    int linepos[row-1];
    for (size_t i=0;i<row-1;i++) {
        linepos[i] = 0;
    }

    printw("%s\n", source[0]);

    int y,x;
    getyx(stdscr, y,x);
    size_t  activeline = 0;

    while (last_key != ESCAPE) {
        last_key = getch();
        switch (last_key) {
            case ESCAPE:
                endwin();
                break;
            case BACKSPACE:
                backspace(source[activeline],
                        *page[activeline], real_length);
                break;
            case ENTER:
                enter(&activeline, linepos, source, row);
                break;
            case BACKLINE:
                backline(&activeline, linepos, source);
                break;
            default:
                type(last_key, source[activeline],
                        *page[activeline], real_length);
                break;
        }
    }

    for (size_t i=0;i<row-1;i++) {
        free(source[i]);
        free(page[i]);
    }

    return EXIT_SUCCESS;
}
