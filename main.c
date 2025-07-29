#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>

#include "include/util.h"


#include "include/pilot.h"


/*--------------------------------------------------------------------------------------------------------------------*/

#define NUMBEROFKART 20
#define MILLI_PER_MINUTE 1000

/*--------------------------------------------------------------------------------------------------------------------*/


typedef struct kart {
    int kartId;
    int lapNbr;
    char *pilote;
    char *team;
    float bestLapTime;
    float s1, s2, s3;
    int isOut;
    int isPitting;
} Kart;


typedef enum enumEventType {
    event_ERROR,
    event_START,
    event_S1,
    event_S2,
    event_S3,
    event_PIT_START,
    event_PIT_END,
    event_OUT,
    event_END
  } EventType;



typedef enum enumRaceType {
    race_ERROR,
    race_P1,
    race_P2,
    race_P3,
    race_Q1_SPRINT,
    race_Q2_SPRINT,
    race_Q3_SPRINT,
    race_SPRINT,
    race_Q1_GP,
    race_Q2_GP,
    race_Q3_GP,
    race_GP,
    race_FINISHED,
    race_MAX
  } RaceType;


typedef struct structProgramOptions {
    int raceNumber;
    RaceType raceType;
    int duration;
    int laps;
    int sleepIndex;
    bool verbose;
} ProgramOptions;


typedef struct structEventRace {
    int id;
    int number;
    RaceType type;
    int car;
    int lap;
    EventType event;
    uint32_t timestamp; // time in milliseconds
} EventRace;

/*--------------------------------------------------------------------------------------------------------------------*/


Driver const *currentRacers = DRIVER_LIST_CRASH;


/*--------------------------------------------------------------------------------------------------------------------*/



int init(void) {

    srand(time(NULL));
    //init the random number generator

    Kart kars[20];

    for (int i = 0; i < 20; i++) {
        kars[i].kartId = DRIVER_LIST[i].number;
        kars[i].pilote = strdup(DRIVER_LIST[i].name);
        kars[i].team = strdup(DRIVER_LIST[i].team);


        kars[i].lapNbr = 0;
        kars[i].bestLapTime = 9999;
        kars[i].isOut = 0;
        kars[i].isPitting = 0;
    }





}
/*--------------------------------------------------------------------------------------------------------------------*/

int getRandomTime(int minTime, int maxTime) {
    return minTime + (unsigned int)((rand() * rand())) % (maxTime - minTime + 1);
    //rand() * rand() to incread the scop and reasign to unsigned int to avoid overflow
    //% give a number between maxTime and minTime

}

/*--------------------------------------------------------------------------------------------------------------------*/

int genTimeCore(ProgramOptions *pParms) {
  uint32_t maxRaceTime;
  //non sign int




  if (pParms->raceType == race_P1 || pParms->raceType == race_P2 || pParms->raceType == race_P3) {//si c'est practice
    maxRaceTime = 60 * MILLI_PER_MINUTE;//1000
  } else if (pParms->raceType == race_Q1_GP) {
    maxRaceTime = 18 * MILLI_PER_MINUTE;
  } else if (pParms->raceType == race_Q2_GP) {
    maxRaceTime = 15 * MILLI_PER_MINUTE;
  } else if (pParms->raceType == race_Q3_GP) {
    maxRaceTime = 12 * MILLI_PER_MINUTE;
  } else if (pParms->raceType == race_Q1_SPRINT) {
    maxRaceTime = 12 * MILLI_PER_MINUTE;
  } else if (pParms->raceType == race_Q2_SPRINT) {
    maxRaceTime = 10 * MILLI_PER_MINUTE;
  } else if (pParms->raceType == race_Q3_SPRINT) {
    maxRaceTime = 8 * MILLI_PER_MINUTE;
  } else {
    maxRaceTime = 0;
  }

  printf("maxRaceTime 🏃🏃 = %d\n", maxRaceTime);

  if (!maxRaceTime) {
    printf("no max run time, the event is a GP i guess!\n");
  }

  if (pParms->verbose) {
    printf("verbose info:");//on affiche les infos, ou limité par un le nombre de tour ou limité par le nombre de minute
    if (maxRaceTime == 0) {
      printf("INFO: the race events generation will be limited to %d laps\n", pParms->laps);
    } else {
      printf("INFO: the race events generation will be limited to %d minutes\n", maxRaceTime / MILLI_PER_MINUTE);
    }
  }
    printf("START timestamp 0🏁🏁🏁\n");

    int result = getRandomTime(25000, 45000);

    printf("------------>%i\n",result);


}

/*--------------------------------------------------------------------------------------------------------------------*/


void changeDriverTheme(Driver const **target) {
    clear();
    mvprintw(1, 2, "Select a theme for the pilots :\n");
    mvprintw(2, 2, "---------------------------");



    char *choices[] = {
        "1. F1 pilote (défault)",
        "2. Crash CTR pilote",
        "3. Gendarme de Saint-Tropez pilote",
    };

    int highlight = 0; // position
    int key;
    int choice = -1;

    while (1) {
        clear();
        printLogo();
        mvprintw(9, 2, "Please make a selection\n"); //x,y + txt to show

        for (int i = 0; i < sizeof(choices) / sizeof(choices[0]); i++) {
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
                if (highlight < 0) highlight = sizeof(choices) / sizeof(choices[0])-1;
                break;
            case KEY_DOWN:
                highlight++;
                if (highlight > sizeof(choices) / sizeof(choices[0])-1) highlight = 0;
                break;
            case 10: //10=enter
                choice = highlight;
                break;
        }

        if (choice != -1) {break;}


    }

    endwin();



    refresh();
    getch();



    switch (choice) {
        case 0:
            *target = DRIVER_LIST;
            break;
        case 1:
            *target = DRIVER_LIST_CRASH;
            break;
        case 2:
            *target = DRIVER_TROPEZ;
            break;
        default:
            break;
    }
}




/*--------------------------------------------------------------------------------------------------------------------*/

void showAllDrivers(const Driver* drivers) {
    clear();
    mvprintw(1, 2, "List of pilote :\n");
    mvprintw(2, 2, "---------------------------");

    for (int i = 0; i < NUMBEROFKART; i++) {
        mvprintw(4 + i, 2, "%2d. %-20s  |  %-30s",
                 drivers[i].number,
                 drivers[i].name,
                 drivers[i].team);
    }


    refresh();
    getch();
}




int mainMenu(void) {
    setenv("TERM", "xterm", 1);


    initscr(); // init
    printLogo();
    noecho(); //don't show typed char
    curs_set(0);//cursor
    keypad(stdscr, TRUE);
    clear();



    char *choices[] = {
        "1. Start a new GP Week-end",
        "2. Continue the current weekend",
        "3. Show the result for the current weekend",
        "4. Show the result for all weekends",
        "5. Show tracks",
        "6. Show current loaded racers",
        "7. [FUN] Change racers name",
        "99. Exit"
    };

    int highlight = 0; // position
    int key;
    int choice = -1;

    while (1) {
        clear();
        printLogo();
        mvprintw(9, 2, "Please make a selection\n"); //x,y + txt to show

        for (int i = 0; i < sizeof(choices) / sizeof(choices[0]); i++) {
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
                if (highlight < 0) highlight = sizeof(choices) / sizeof(choices[0])-1;
                break;
            case KEY_DOWN:
                highlight++;
                if (highlight > sizeof(choices) / sizeof(choices[0])-1) highlight = 0;
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
            showAllDrivers(currentRacers);
            mainMenu();
            break;
        case 6:
            changeDriverTheme(&currentRacers);
            mainMenu();
            break;
        case 7:
            printf("byebye!\n");
            return 0;

        default:
            printf("Invalid selection\n");
    }

    return 0;
}


/*--------------------------------------------------------------------------------------------------------------------*/






/*--------------------------------------------------------------------------------------------------------------------*/

int main(void) {


    init();
    ProgramOptions options;







    // typedef struct structProgramOptions {
    //     int raceNumber;
    //     RaceType raceType;
    //     int duration;
    //     int laps;
    //     int sleepIndex;
    //     bool verbose;
    // } ProgramOptions;
    options.raceType =1;
    options.verbose =1;
    genTimeCore(&options);
    options.raceType =0;
    genTimeCore(&options);

    while (1) {


    }

    //mainMenu();



    return 0;
}
