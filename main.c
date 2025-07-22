#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "include/util.h"


int mainMenu(void) {
    setenv("TERM", "xterm", 1);


    initscr(); // init
    noecho(); //don't show typed char
    curs_set(0);//cursor
    keypad(stdscr, TRUE);
    clear();

    printLogo();

    char *choices[] = {
        "1. Start a new GP Week-end",
        "2. Continue the current weekend",
        "3. Show the result for the current weekend",
        "4. Show the result for all weekends",
        "5. Show track infos",
        "6. Show racers infos",
        "99. Exit"
    };

    int highlight = 0; // position
    int key;
    int choice = -1;

    while (1) {
        clear();
        printLogo();
        mvprintw(9, 2, "Please make a selection\n"); //x,y + txt to show

        for (int i = 0; i < 7; i++) {
            if (i == highlight) {
                attron(A_REVERSE); // selection effect
            }
            mvprintw(11 + i, 4, "%s", choices[i]);//print element
            if (i == highlight) {
                attroff(A_REVERSE);
            }
        }

        key = getch(); // wait for input

        switch (key) {
            case KEY_UP:
                highlight--;
                if (highlight < 0) highlight = 6;
                break;
            case KEY_DOWN:
                highlight++;
                if (highlight > 6) highlight = 0;
                break;
            case 10: //10=enter
                choice = highlight;
                break;
        }

        if (choice != -1) {break;}


    }

    endwin();

    switch (choice) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            break;
        case 5:
            break;
        case 6:
            printf("byebye!\n");
            return 0;
        default:
            printf("Invalid selection\n");
    }

    return 0;
}

int main(void) {

    mainMenu();

    return 0;
}
