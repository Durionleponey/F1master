#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <poll.h>    // poll() pour surveiller 20 pipes


#include "include/util.h"


#include "include/pilot.h"


/*--------------------------------------------------------------------------------------------------------------------*/

#define NUMBEROFKART 20
#define NUMBEROFRACE 24
#define NUMBEROFSECTOR 3

#define MILLI_PER_MINUTE 1000

/*--------------------------------------------------------------------------------------------------------------------*/


typedef struct kart {
    int kartId;
    int piloteNumber;
    int lapNbr;
    int isPitting;
    float bestLapTime;
    float s1, s2, s3;
    bool isOut;
} Kart;


Kart karts[NUMBEROFKART];

/*--------------------------------------------------------------------------------------------------------------------*/



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
    char gpname[40];
    int trackNumber;
    RaceType raceType;
    bool special;
    int laps;
    int speedfactor;
    bool verbose;
} ProgramOptions;

ProgramOptions options;

/*--------------------------------------------------------------------------------------------------------------------*/


void printProgramOptions(const ProgramOptions *opts) {
    printf("Grand Prix name: %s\n", opts->gpname);
    printf("Track number: %d\n", opts->trackNumber);
    printf("Race type: %s\n", opts->raceType == true ? "SPRINT" : "ENDURANCE");
    printf("Special: %s\n", opts->special ? "Yes" : "No");
    printf("Laps: %d\n", opts->laps);
    printf("Speed factor: %d\n", opts->speedfactor);
    printf("Verbose: %s\n", opts->verbose ? "Enabled" : "Disabled");
}





/*--------------------------------------------------------------------------------------------------------------------*/



/*--------------------------------------------------------------------------------------------------------------------*/


Driver const *currentRacers = DRIVER_LIST_CRASH;


/*--------------------------------------------------------------------------------------------------------------------*/



int init(void) {

    srand(time(NULL));
    //init the random number generator

    Kart kars[20];

    for (int i = 0; i < 20; i++) {
        kars[i].lapNbr = 0;
        kars[i].bestLapTime = 9999;
        kars[i].isOut = false;
        kars[i].isPitting = 0;
        kars[i].s1 =0;
        kars[i].s2 =0;
        kars[i].s3 =0;

        kars[i].piloteNumber = currentRacers[i].number;


    }

    // typedef struct kart {
    //     int kartId;
    //     int piloteNumber;
    //     int lapNbr;
    //     int isPitting;
    //     float bestLapTime;
    //     float s1, s2, s3;
    //     bool isOut;
    // } Kart;



}
/*--------------------------------------------------------------------------------------------------------------------*/

int getRandomTime(int minTime, int maxTime) {
    return minTime + (unsigned int)((rand() * rand())) % (maxTime - minTime + 1);
    //rand() * rand() to incread the scop and reasign to unsigned int to avoid overflow
    //% give a number between maxTime and minTime

}

/*--------------------------------------------------------------------------------------------------------------------*/

int genTimeCore(ProgramOptions *pParms, int fd[2], int id) {
  uint32_t maxRaceTime;
  //non sign int
  int totaltimestamp =0;
  int sector =0;

    printProgramOptions(&options);
  //
  // if (pParms->raceType == race_P1 || pParms->raceType == race_P2 || pParms->raceType == race_P3) {//si c'est practice
  //   maxRaceTime = 60 * MILLI_PER_MINUTE;//1000
  // } else if (pParms->raceType == race_Q1_GP) {
  //   maxRaceTime = 18 * MILLI_PER_MINUTE;
  // } else if (pParms->raceType == race_Q2_GP) {
  //   maxRaceTime = 15 * MILLI_PER_MINUTE;
  // } else if (pParms->raceType == race_Q3_GP) {
  //   maxRaceTime = 12 * MILLI_PER_MINUTE;
  // } else if (pParms->raceType == race_Q1_SPRINT) {
  //   maxRaceTime = 12 * MILLI_PER_MINUTE;
  // } else if (pParms->raceType == race_Q2_SPRINT) {
  //   maxRaceTime = 10 * MILLI_PER_MINUTE;
  // } else if (pParms->raceType == race_Q3_SPRINT) {
  //   maxRaceTime = 8 * MILLI_PER_MINUTE;
  // } else {
  //   maxRaceTime = 0;
  // }

  //printf("maxRaceTime 🏃🏃 = %d\n", maxRaceTime);

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


    // karts[0].s1=30;
    // karts[0].s2=60;
    // karts[0].s3=90;
    //sleep(3);





    if (pParms->special){}



    karts[id].lapNbr=pParms->laps;

    //printf("number of lap that will be done --> %i\n\n\n",numberOfTour);


    while (karts[id].lapNbr) {

        karts[id].s1 = 0;
        karts[id].s2 = 0;
        write(fd[1], &karts[id], sizeof(karts[id]));


        //printf("START LAP timestamp 0🏁🏁🏁\n");


        for (int i=1; i<NUMBEROFSECTOR+1; i++) {


            //printf("START SECTOR %i🏁🏁🏁\n",i);


            //printf("--->%i\n",pParms->speedfactor);
            float sector = getRandomTime(25000, 45000);
            float realtime = sector;
            sector= sector/pParms->speedfactor;

            //printf("seconde ---> %f\n",(sector));

            usleep(sector*1000);

            switch (i) {
                case 1:
                    //printf("seconde s1 ---> %f\n",(realtime));
                    karts[id].s1 = realtime;
                    karts[id].s3 = 0;
                    write(fd[1], &karts[id], sizeof(karts[id]));
                    break;
                case 2:
                    //printf("seconde s2 ---> %f\n",(realtime));
                    karts[id].s2 = realtime;
                    write(fd[1], &karts[id], sizeof(karts[id]));
                    break;
                case 3:
                    //printf("seconde s3---> %f\n",(realtime));
                    karts[id].s3 = realtime;
                    write(fd[1], &karts[id], sizeof(karts[id]));
                    break;


            }



        }



        //printf("🥳🥳LAP COMPLETED ! left--> %i\n\n\n",numberOfTour);
        karts[id].lapNbr--;

    }

    //printf("race complete!");
    close(fd[1]);



}

/*--------------------------------------------------------------------------------------------------------------------*/


int animation(char event[]){

    clear();



const char *art[] = {

};
const int art_rows = sizeof art / sizeof *art;
const int art_cols = 62;

int max_y, max_x;
getmaxyx(stdscr, max_y, max_x);
int start_y = (max_y - art_rows) / 2;
int start_x = (max_x - art_cols) / 2;


mvprintw(start_y + art_rows + 2, start_x+14, "Strating %s ..... Ready ? [Touch a key to continue] ", event);



    getch();

endwin();
return 0;
}

/*--------------------------------------------------------------------------------------------------------------------*/



void changeDriverTheme(Driver const **target) {
    clear();




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
        mvprintw(9, 2, "Please select your pilotes\n"); //x,y + txt to show

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
    refresh();

    endwin();
    refresh();








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


/*--------------------------------------------------------------------------------------------------------------------*/

void showAllTracks(void) {
    clear();
    mvprintw(1, 2, "List of tracks :\n");
    mvprintw(2, 2, "---------------------------");

    for (int i = 0; i < NUMBEROFRACE; i++) {
        mvprintw(4 + i, 2, "%d .%10s. %10s | number of laps: %i,Sprint Laps: %d",
                i,
                 GP_LIST[i].name,
                 GP_LIST[i].circuit,
                 GP_LIST[i].laps,
                 GP_LIST[i].sprintLaps);
    }


    refresh();
    getch();
}

/*--------------------------------------------------------------------------------------------------------------------*/



int trackSelection(void) {
    setenv("TERM", "xterm", 1);

    clear();





    int highlight = 0; // position
    int key;
    int choice = -1;


    while (1) {
        clear();
        //printLogo();
        mvprintw(1, 2, "Please chose the track for the week end\n"); //x,y + txt to show

        for (int i = 0; i < NUMBEROFRACE; i++) {
            if (i == highlight) {
                attron(A_REVERSE);
            }
            mvprintw(3 + i, 4, "%s10 | %s", GP_LIST[i].name, GP_LIST[i].circuit);
            if (i == highlight) {
                attroff(A_REVERSE);
            }
        }

        key = getch(); // wait for input

        switch (key) {
            case KEY_UP:
                highlight--;
                if (highlight < 0) highlight = NUMBEROFRACE -1;
                break;
            case KEY_DOWN:
                highlight++;
                if (highlight > NUMBEROFRACE-1) highlight = 0;
                break;
            case 10: //10=enter
                choice = highlight;
                break;
        }

        if (choice != -1) {break;}


    }

    endwin();

    //options.raceNumber = choice;

    options.laps = GP_LIST[choice].laps;



    return choice;
}

/*--------------------------------------------------------------------------------------------------------------------*/


int setGPname(void){



    printLogo2();

    printf("Please choose the GP name: [40 char max]\n");

    printf("\n>>");
    scanf("%40s", options.gpname);
    return 0;





}


/*--------------------------------------------------------------------------------------------------------------------*/

bool weekendTypeSelection(void) {
    setenv("TERM", "xterm", 1);

    clear();






    char *choices[] = {
        "1. Classic",
        "2. With Sprints",
    };

    int highlight = 0; // position
    int key;
    int choice = -1;

    while (1) {
        clear();
        printLogo();
        mvprintw(9, 2, "Please select the type of WEEK END\n"); //x,y + txt to show

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

    bool a;

    if (choice){
    a = true;
    }else{
    a =false;
    }

    endwin();

    return choice;
}

/*--------------------------------------------------------------------------------------------------------------------*/



int speedfactorchanger(void) {
    setenv("TERM", "xterm", 1);

    clear();






    char *choices[] = {
        "1. 1x speed (défault)",
        "2. 2x speed (fast)",
        "3. 3x speed (very fast)",
        "4. 5x speed (lighning speed)",
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

    choice++;

    if (choice == 4){choice =5;}

    return choice;
}


/*--------------------------------------------------------------------------------------------------------------------*/


void displayPractice(void)
{
    printf("\033[H\033[J");

    printf("%-3s | %-20s | %-11s | %-11s | %-11s | %-5s | %-5s\n",
           "#", "Pilote", "S1", "S2", "S3", "Laps", "Équipe");
    puts("-------------------------------------------------------------------------------------------------------------");

    for (int i = 0; i < NUMBEROFKART; ++i) {
        printf("%-3d | %-20s | %8.3f\" | %8.3f\" | %8.3f\" | %-5d | %-35s\n",
               currentRacers[i].number,
               currentRacers[i].name,
               karts[i].s1 / 1000.0,
               karts[i].s2 / 1000.0,
               karts[i].s3 / 1000.0,
               karts[i].lapNbr,
               currentRacers[i].team);
    }
}



/*--------------------------------------------------------------------------------------------------------------------*/

int lauchTheEvent(void) {

    printf("helllo\n");
    //while (1) {}

    int fd[NUMBEROFKART][2];


    for (int i =0; i<NUMBEROFKART;i++) {

        pipe(fd[i]);


        pid_t pid = fork();

        if (pid > 0) {
            close(fd[i][1]);

            int fl = fcntl(fd[i][0], F_GETFL);
            fcntl(fd[i][0], F_SETFL, fl | O_NONBLOCK);
        }

        srand(time(NULL) ^ getpid());//regen random for each children


        if (pid == 0) {
            printf("kidsss\n");
            close(fd[i][0]);



            genTimeCore(&options,fd[i],i);
            _exit(EXIT_SUCCESS);
        }

        //parent

        close(fd[i][1]);


    }


    printf("parent\n");

    //displayPractice();

    struct pollfd p[NUMBEROFKART];
    for (int i = 0; i < NUMBEROFKART; ++i) {
        p[i].fd     = fd[i][0];
        p[i].events = POLLIN;
    }


    while (1) {

        usleep(2000);


        int ready = poll(p, NUMBEROFKART, -1);
        if (ready == -1) {
            perror("poll");
            break;
        }

        for (int i = 0; i < NUMBEROFKART; ++i) {
            if (p[i].fd == -1) continue;

            if (p[i].revents & POLLIN) {

                ssize_t n;
                while ((n = read(fd[i][0], &karts[i], sizeof karts[i])) == sizeof karts[i]) {
                }

                if (n == 0) {
                    close(fd[i][0]);
                    p[i].fd = -1;
                }
                else if (n == -1 && errno != EAGAIN) {
                    perror("read");
                }
            }
        }

        displayPractice();
    }


    //genTimeCore(&options);
    close(fd[0]);

}


/*--------------------------------------------------------------------------------------------------------------------*/

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
        "8. [TIME SAVING] Speed factor",
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
            //setGPname();
            options.special = weekendTypeSelection();
            options.trackNumber = trackSelection();
            options.speedfactor = speedfactorchanger();
            //changeDriverTheme(&currentRacers);
            animation("Practice");

            options.speedfactor = 5;

            lauchTheEvent();





        case 1:
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            showAllTracks();
            mainMenu();
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
            options.speedfactor = speedfactorchanger();
            mainMenu();
            return 0;
        case 8:
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
    //ProgramOptions options;


    // typedef struct structProgramOptions {
    //     int raceNumber;
    //     RaceType raceType;
    //     int duration;
    //     int laps;
    //     int sleepIndex;
    //     bool verbose;
    // } ProgramOptions;
    options.speedfactor =5;


    options.raceType =1;
    options.verbose =1;
    //genTimeCore(&options);
    options.raceType =0;



    //genTimeCore(&options);



    mainMenu();



    return 0;
}
