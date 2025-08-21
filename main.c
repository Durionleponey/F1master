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
#include <semaphore.h>
#include <sys/wait.h>
#include <iso646.h>



/*--------------------------------------------------------------------------------------------------------------------*/

#include "include/util.h"
#include "include/pilot.h"
#include "include/shared_memory.h"

/*--------------------------------------------------------------------------------------------------------------------*/

#define NUMBEROFKART 20
#define NUMBEROFRACE 24
#define NUMBEROFSECTOR 3

#define SECOND_PER_MINUTE 60
#define TIME_FOR_PRACTICE 60

#define TIME_FOR_QUALIF1 18
#define TIME_FOR_QUALIF2 15
#define TIME_FOR_QUALIF3 12


#define TIME_FOR_QUALIF1SPECIAL 12
#define TIME_FOR_QUALIF2SPECIAL 10
#define TIME_FOR_QUALIF3SPECIAL 8

// Q1 : 12 minutes, Q2 : 10 minutes, Q3 : 8 minutes

#define CHANCETOGOPITING 10
#define CHANCETOGOOUT 1000
#define SECONDLOSTINPIT 25

#define BLOCK_SIZE 4096

#define INFINITY 999999

#define SPEEDFACTOR 50

/*--------------------------------------------------------------------------------------------------------------------*/


typedef struct kart {
    int kartId;
    int piloteNumber;
    int lapNbr;
    int isPitting;
    float lapTime;
    float bestLapTime;
    float s1, s2, s3;
    float bs1, bs2, bs3;
    int stepDone;
    int isOut;
} Kart;


Kart karts[NUMBEROFKART];




typedef enum enumRaceType {
    Essaie1,//friday//60
    Essaie2,//60
    Essaie3,//60
    Qualif1,
    Qualif2,
    Qualif3,
    GrandP,
    SpecialEssaie1,//60
    SpecialQualifi1,//12
    SpecialQualifi2,//10
    SpecialQualifi3,//8
    SpecialSprint,//100KM
    SpecialQualifi4,//temp normal
    SpecialQualifi5,
    SpecialQualifi6,
    SpecialGrandP//course normal

  } RaceType;





typedef struct structProgramOptions {
    char gpname[40];
    int trackNumber;
    RaceType raceType;
    bool special;
    int laps;
    int time_left;
    int speedfactor;
    bool verbose;
} ProgramOptions;

ProgramOptions options;


typedef struct structEventBest{

    float sector_best[3];
    int sector_best_car_id[3];
    float best_lap;
    int best_lap_car_id;
    int time_left;
    int qualifiedKard[15];
    sem_t semaphore[4];

} EventBest;

//EventBest eventBest;



int finalPostion[20];





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


void ensure_file_exists(const char *path) {
    int fd = open(path, O_CREAT | O_RDWR, 0666);
    if (fd != -1) close(fd);
}

/*--------------------------------------------------------------------------------------------------------------------*/


Driver const *currentRacers = DRIVER_LIST_CRASH;

EventBest *data;

/*--------------------------------------------------------------------------------------------------------------------*/

int resetChildAndMemory(void) {

    (*data).best_lap=INFINITY;//sorry but (*data) it's more logic for me
    (*data).sector_best[0]=INFINITY;
    (*data).sector_best[1]=INFINITY;
    (*data).sector_best[2]=INFINITY;


    for (int i = 0; i < NUMBEROFKART; ++i) {

        karts[i].lapNbr      = 0;
        karts[i].lapTime = karts[i].bestLapTime = INFINITY;
        karts[i].isOut       =0;
        karts[i].isPitting   = 0;
        karts[i].s1 = karts[i].s2 = karts[i].s3 = 0;
        karts[i].bs1 = karts[i].bs2 = karts[i].bs3 = INFINITY;
        karts[i].stepDone    = 0;
        karts[i].piloteNumber = currentRacers[i].number;
    }



}


/*--------------------------------------------------------------------------------------------------------------------*/


int init(void) {



    char *filenameforsharedmemory = "sharedmemory";

    ensure_file_exists(filenameforsharedmemory);


    //shard memory stuff

    char *charedMemory;

    //create shared memory
    get_shared_block(filenameforsharedmemory, BLOCK_SIZE);

    //charedMemory = attach_memory_block(filenameforsharedmemory, BLOCK_SIZE);
    //*charedMemory != because * mean go to address en write

    data = (EventBest *)attach_memory_block(filenameforsharedmemory, sizeof(EventBest));

    (*data).best_lap=INFINITY;//sorry but (*data) it's more logic for me
    (*data).sector_best[0]=INFINITY;
    (*data).sector_best[1]=INFINITY;
    (*data).sector_best[2]=INFINITY;




    //printf("parent --> %f",((*data).best_lap));




    srand(time(NULL));
    //init the random number generator



   for (int i = 0; i < NUMBEROFKART; ++i) {

        karts[i].lapNbr      = 0;
       karts[i].lapTime = karts[i].bestLapTime = INFINITY;
       karts[i].isOut       =0;
       karts[i].isPitting   = 0;
       karts[i].s1 = karts[i].s2 = karts[i].s3 = 0;
       karts[i].bs1 = karts[i].bs2 = karts[i].bs3 = INFINITY;
       karts[i].stepDone    = 0;
       karts[i].piloteNumber = currentRacers[i].number;
        }

    //semaphore stuff

    sem_init(&((*data).semaphore[0]), 1, 1);
    sem_init(&((*data).semaphore[1]), 1, 1);
    sem_init(&((*data).semaphore[2]), 1, 1);
    sem_init(&((*data).semaphore[3]), 1, 1);


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

int genTimeCore(ProgramOptions *pParms, int fd[2],int id) {
  uint32_t maxRaceTime;
  //non sign int
  int totaltimestamp =0;
  int sector =0;
    float timeinpit;

    //printProgramOptions(&options);




    if (pParms->special){}

    int substractTour =1;

    switch (options.raceType) {
        case Essaie1:
        case Essaie2:
        case Essaie3:
        case Qualif1:
        case Qualif2:
        case Qualif3:
            karts[id].lapNbr = INFINITY;
            substractTour =0;
            break;
        case GrandP:
            karts[id].lapNbr=pParms->laps;
            break;
    }



    //karts[id].lapNbr=pParms->laps;

    //printf("number of lap that will be done --> %i\n\n\n",numberOfTour);

    karts[id].lapTime=INFINITY;


    while (karts[id].lapNbr && !karts[id].isOut && (*data).time_left) {
        //printf("aaaaaaaaaaaaaaaaaaaa");
        //printf("enfant --> %f\n",((*data).best_lap));



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

                    if (karts[id].lapTime < karts[id].bestLapTime) {
                        karts[id].bestLapTime = karts[id].lapTime;

                        sem_wait(&((*data).semaphore[3]));

                        if (karts[id].lapTime < (*data).best_lap) {
                            (*data).best_lap = karts[id].lapTime;
                            (*data).best_lap_car_id = id;
                        }
                        sem_post(&((*data).semaphore[3]));

                    }


                    karts[id].lapTime = realtime;
                    //printf("realti---> %f\n",realtime);
                    //printf("karts1---> %f\n",karts[id].bs1);
                    if (realtime < karts[id].bs1) {
                        //printf("realtime: %f\n",realtime);
                        //printf("karts[id].bs1): %f\n",karts[id].bs1);
                        //printf("realtime < karts[id].bs1%s\n",(realtime < karts[id].bs1) ? "OUI" : "NON");
                        karts[id].bs1 = realtime;
                        //printf("changement\n");
                    }

                    sem_wait(&((*data).semaphore[0]));


                    if (realtime < (*data).sector_best[0]) {
                        (*data).sector_best[0] = realtime;
                        (*data).sector_best_car_id[0] = id;


                    }

                    sem_post(&((*data).semaphore[0]));
                    karts[id].s3 = 0;
                    karts[id].stepDone++;
                    write(fd[1], &karts[id], sizeof(karts[id]));
                    break;
                case 2:
                    //printf("seconde s2 ---> %f\n",(realtime));
                    karts[id].s2 = realtime;
                    karts[id].lapTime += realtime;
                    if (realtime < karts[id].bs2) {
                        karts[id].bs2 = realtime;
                    }

                    sem_wait(&((*data).semaphore[1]));

                    if (realtime < (*data).sector_best[1]) {
                        (*data).sector_best[1] = realtime;
                        (*data).sector_best_car_id[1] = id;

                    }

                    sem_post(&((*data).semaphore[1]));



                    karts[id].stepDone++;
                    write(fd[1], &karts[id], sizeof(karts[id]));
                    break;
                case 3:
                    //pitting stuff
                    if (getRandomTime(1, CHANCETOGOPITING) == 1) {

                        karts[id].isPitting = true;
                        write(fd[1], &karts[id], sizeof(karts[id]));

                        sleep(SECONDLOSTINPIT/options.speedfactor);

                        realtime+=SECONDLOSTINPIT*1000;

                    }


                    //printf("seconde s3---> %f\n",(realtime));
                    karts[id].isPitting = false;
                    karts[id].s3 = realtime;
                    karts[id].lapTime += realtime;
                    if (realtime < karts[id].bs3) {
                        karts[id].bs3 = realtime;

                    }

                    sem_wait(&((*data).semaphore[2]));

                    if (realtime < (*data).sector_best[2]) {
                        (*data).sector_best[2] = realtime;
                        (*data).sector_best_car_id[2] = id;

                    }

                    sem_post(&((*data).semaphore[2]));
                    karts[id].stepDone++;
                    write(fd[1], &karts[id], sizeof(karts[id]));
                    break;


            }

        }
        //printf("🥳🥳LAP COMPLETED ! left--> %i\n\n\n",numberOfTour);
        if (substractTour){karts[id].lapNbr--;}

        //
        if (getRandomTime(1, CHANCETOGOOUT) == 1) {
            karts[id].isOut = true;
            write(fd[1], &karts[id], sizeof(karts[id]));
        }


    }

    //printf("race complete!");
    close(fd[1]);

    return 0;



}

/*--------------------------------------------------------------------------------------------------------------------*/


int animation(){

    char event[100];

    switch (options.raceType) {
        case Essaie1:
            strcpy(event, "Essaie 1");
            break;
        case Essaie2:
            strcpy(event, "Essaie 2");
            break;
        case Essaie3:
            strcpy(event, "Essaie 3");
            break;
        case Qualif1:
            strcpy(event, "Qualification 1");
            break;
        case Qualif2:
            strcpy(event, "Qualification 2");
            break;
        case Qualif3:
            strcpy(event, "Qualification 3");
            break;
        case GrandP:
            strcpy(event, "GrandPrix Race");
            break;
        case SpecialEssaie1:
            strcpy(event, "Essai (week-end sprint)");
            break;
        case SpecialQualifi1:
            strcpy(event, "Qualification Sprint - Q1");
            break;
        case SpecialQualifi2:
            strcpy(event, "Qualification Sprint - Q2");
            break;
        case SpecialQualifi3:
            strcpy(event, "Qualification Sprint - Q3");
            break;
        case SpecialSprint:
            strcpy(event, "Course Sprint");
            break;
        case SpecialQualifi4:
            strcpy(event, "Qualification Course - Q1");
            break;
        case SpecialQualifi5:
            strcpy(event, "Qualification Course - Q2");
            break;
        case SpecialQualifi6:
            strcpy(event, "Qualification Course - Q3");
            break;
        case SpecialGrandP:
            strcpy(event, "Grand Prix Race (week-end sprint)");
            break;
    }

    initscr(); // init
    printLogo();
    noecho(); //don't show typed char
    curs_set(0);//cursor
    keypad(stdscr, TRUE);





const char *art[] = {

};
const int art_rows = sizeof art / sizeof *art;
const int art_cols = 62;

int max_y, max_x;
getmaxyx(stdscr, max_y, max_x);
int start_y = (max_y - art_rows) / 2;
int start_x = (max_x - art_cols) / 2;


mvprintw(start_y + art_rows + 2, start_x+14, "Strating %s ..... Ready ? touch a key to continue.", event);
    mvprintw(start_y + art_rows + 3, start_x+14, "Name of the Cup: %s", options.gpname);
    mvprintw(start_y + art_rows + 4, start_x+14, "Name of the GP: %s", GP_LIST[options.trackNumber].name);
    mvprintw(start_y + art_rows + 5, start_x+14, "Name of the track: %s", GP_LIST[options.trackNumber].circuit);



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

    printf("Please choose the SAISON name: [40 char max]\n");

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
        "1. 1x speed (normal very very boring)",
        "2. 2x speed (a bit faster but still very boring)",
        "3. 5x speed (faster but still boring)",
        "4. 10x speed ('''fast''')",
        "5. 50x speed (fast)",
        "6. 100x speed (very fast)",
        "7. 250x speed (very very fast)"
    };

    int highlight = 0; // position
    int key;
    int choice = -1;

    while (1) {
        clear();
        printLogo();
        mvprintw(9, 2, "Please select the speed\n"); //x,y + txt to show

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
            return 1;
        case 1:
            return 2;
        case 2:
            return 5;
        case 3:
            return 10;
        case 4:
            return 50;
        case 5:
            return 100;
        case 6:
            return 250;
    }



    return 1;
}

/*--------------------------------------------------------------------------------------------------------------------*/

static int cmp_step_done(const void *a, const void *b) {
    if (options.raceType == GrandP) {

        const int ia = *(const int *)a;
        const int ib = *(const int *)b;

        return karts[ib].stepDone -karts[ia].stepDone;

    }else {
        const int ia = *(const int *)a;
        const int ib = *(const int *)b;

        return karts[ia].bestLapTime -karts[ib].bestLapTime;

    }

}

/*--------------------------------------------------------------------------------------------------------------------*/



int saveEventType() {

    char filepath[100];
    snprintf(filepath, sizeof(filepath), "F1master/%s/%s-%s.f1master", options.gpname, options.gpname, GP_LIST[options.trackNumber].name);

    FILE *file = fopen(filepath,"a");

    if (file == NULL) {
        perror("fopen");
    }

    fprintf(file, "%i\n", options.special);



    fclose(file);

    return 0;
}



/*--------------------------------------------------------------------------------------------------------------------*/

int saveEvent(int top[]) {

    char filepath[100];
    snprintf(filepath, sizeof(filepath), "F1master/%s/%s-%s.f1master", options.gpname, options.gpname, GP_LIST[options.trackNumber].name);

    FILE *file = fopen(filepath,"a");

    if (file == NULL) {
        perror("fopen");
    }


    for (int i = 0; i < NUMBEROFKART; ++i) {
        fprintf(file, "%i,", currentRacers[top[i]].number);
    }

    fprintf(file,"%f,",(*data).best_lap);

    for (int i = 0; i < 3; ++i) {
        fprintf(file, "%f,",(*data).sector_best[i]);
    }



    fprintf(file, "\n");



    fclose(file);
    return 0;
}


/*--------------------------------------------------------------------------------------------------------------------*/

int createAfile(void) {


    char command[100];

    system("mkdir F1master");

    snprintf(command, sizeof(command), "mkdir F1master/%s", options.gpname);


    system(command);

    char filepath[100];
    snprintf(filepath, sizeof(filepath), "F1master/%s/%s-%s.f1master", options.gpname, options.gpname, GP_LIST[options.trackNumber].name);

    FILE *file = fopen(filepath,"w");

    if (file == NULL) {
        perror("errrrrror");
    }else {

        fclose(file);

    }

}
/*--------------------------------------------------------------------------------------------------------------------*/

int setTimeForTherace() {

    switch (options.raceType) {
        case Essaie1:
        case Essaie2:
        case Essaie3:
        case SpecialEssaie1:
            (*data).time_left = TIME_FOR_PRACTICE * SECOND_PER_MINUTE;
            break;
        case Qualif1:
        case SpecialQualifi4:
            (*data).time_left = TIME_FOR_QUALIF1 * SECOND_PER_MINUTE;
            break;
        case Qualif2:
        case SpecialQualifi5:
            (*data).time_left = TIME_FOR_QUALIF2 * SECOND_PER_MINUTE;
            break;
        case Qualif3:
        case SpecialQualifi6:
            (*data).time_left = TIME_FOR_QUALIF3 * SECOND_PER_MINUTE;
            break;
        case GrandP:
        case SpecialGrandP:
            (*data).time_left = 2*TIME_FOR_PRACTICE * SECOND_PER_MINUTE;
            break;
        case SpecialQualifi1:
            (*data).time_left = TIME_FOR_QUALIF1SPECIAL * SECOND_PER_MINUTE;
            break;
        case SpecialQualifi2:
            (*data).time_left = TIME_FOR_QUALIF1SPECIAL * SECOND_PER_MINUTE;
            break;
        case SpecialQualifi3:
            (*data).time_left = TIME_FOR_QUALIF1SPECIAL * SECOND_PER_MINUTE;
            break;


    }
    return 0;
};



/*--------------------------------------------------------------------------------------------------------------------*/




void displayPractice(int readytosave)//need to update the name to display event
{

    int ii;
    int order[NUMBEROFKART];




    for (int i = 0; i < NUMBEROFKART; ++i) {
        order[i] = i;
    }

    float tdiff = 0.0f;




    qsort(order, NUMBEROFKART, sizeof(int), cmp_step_done);


    printf("\033[H\033[J");

    printf("%-3s |%-4s | %-20s | %-9s | %-9s | %-9s | %-9s | %-9s | %-9s | %-9s | %-9s | %-5s | %-2s | %-1s\n",
           "#", "Num", "Pilote", "S1", "S2", "S3", "BS1", "BS2", "BS3", "BLaps", "TDiff", "Laps","Pit","Crash");
    puts("-------------------------------------------------------------------------------------------------------------------------------------------------------");

    for (int i = 0; i < NUMBEROFKART; ++i) {
        ii = order[i];





        if (options.raceType != GrandP) {

            if (i == 0) {
                tdiff = 0.0f;
            }else {
                tdiff = (karts[ii].bestLapTime - karts[order[i-1]].bestLapTime) / 1000.0f;


            }

        }else {

            if (i == 0) {
                tdiff = 0.0f;
            } else {
                int tourDiff = karts[order[0]].stepDone - karts[ii].stepDone;
                if (tourDiff > 0) {
                    tdiff = tourDiff * (karts[order[0]].bestLapTime / 3000.0f);
                } else {
                    tdiff = (karts[ii].bestLapTime - karts[order[0]].bestLapTime) / 1000.0f;
                }
            }

        }



        if (!karts[ii].lapNbr)karts[ii].isOut = true;


        printf("%-3d | %-3d | %-20s | %8.3f\" | %8.3f\" | %8.3f\" | %8.3f\" | %8.3f\" | %8.3f\" | %8.3f\" | %8.3f\" | %-5d | %-5s | %-5s\n",
            i+1,
               currentRacers[ii].number,
               currentRacers[ii].name,
               karts[ii].s1 / 1000.0,
               karts[ii].s2 / 1000.0,
               karts[ii].s3 / 1000.0,
               karts[ii].bs1 / 1000.0,
               karts[ii].bs2 / 1000.0,
               karts[ii].bs3 / 1000.0,
               karts[ii].bestLapTime / 1000.0,
               tdiff,
               karts[ii].lapNbr,
               karts[ii].isPitting ? "🛠️" : "🟢",
               karts[ii].isOut ? "❌" : "🏎️");
    }
    //currentRacers[ii].team

    printf("\n");
    printf("best S1: %8.3f\" by %s\n",((*data).sector_best[0]/1000),currentRacers[(*data).sector_best_car_id[0]].name);
    printf("best S2: %8.3f\" by %s\n",((*data).sector_best[1]/1000),currentRacers[(*data).sector_best_car_id[1]].name);
    printf("best S3: %8.3f\" by %s\n\n",((*data).sector_best[2]/1000),currentRacers[(*data).sector_best_car_id[2]].name);
    printf("best Lap: %7.3f\" by %s\n\n",((*data).best_lap/1000),currentRacers[(*data).best_lap_car_id].name);

    int secondes = (*data).time_left;
    int minutes = secondes / 60;
    int reste_secondes = secondes % 60;
    printf("Time left: %02d:%02d\n\n", minutes, reste_secondes);





    if (readytosave) {
        //printf("💾💾💾💾💾/n");
        saveEvent(order);


    }



}

/*--------------------------------------------------------------------------------------------------------------------*/


void reap_children_nonblock(void) {
    int status;
    pid_t pid;
    /* Récolte tous les enfants terminés sans bloquer */
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
    }
}


/*--------------------------------------------------------------------------------------------------------------------*/

void loadPostion(void) {

    char filepath[100];
    snprintf(filepath, sizeof(filepath), "F1master/%s/%s-%s.f1master", options.gpname, options.gpname, GP_LIST[options.trackNumber].name);

    FILE *patate = fopen(filepath, "r");
    if (!patate) {
        perror("🥔🥔🥔🥔🥔");
        return;
    }

    char buffer[1024];


    //fgets(buffer, sizeof(buffer), patate);//read first line of the file


    int numberOfF1toloadByLigne = 20;
    int stopReading = 5;//lign 4 in the .F1master is the result of q1

    int i = 1;

    //yes it's bad but i'm alone and i have to study my network security stuff so..


    while (fgets(buffer, sizeof(buffer), patate)) {
        if (i == stopReading) {
            int k = 0;
            char *tok = strtok(buffer, ",");
            while (tok && k < numberOfF1toloadByLigne) {

                if (k>=15) {
                    finalPostion[k] = (int)strtol(tok, NULL, 10);


                }
                tok = strtok(NULL, ",");
                k++;
            }

            break;

        }
        i++;

    }

    rewind(patate);





    stopReading++;//lign 4 in the .F1master is the result of q1

    i = 1;


    while (fgets(buffer, sizeof(buffer), patate)) {
        if (i == stopReading) {
            int k = 0;
            char *tok = strtok(buffer, ",");
            while (tok && k < numberOfF1toloadByLigne) {

                if (k>=10 && k<15) {
                    finalPostion[k] = (int)strtol(tok, NULL, 10);


                }
                tok = strtok(NULL, ",");
                k++;
            }

            break;

        }
        i++;

    }





    rewind(patate);
    stopReading++;//lign 4 in the .F1master is the result of q1

    i = 1;


    while (fgets(buffer, sizeof(buffer), patate)) {
        if (i == stopReading) {
            int k = 0;
            char *tok = strtok(buffer, ",");
            while (tok && k < numberOfF1toloadByLigne) {

                if (k<10) {
                    finalPostion[k] = (int)strtol(tok, NULL, 10);


                }
                tok = strtok(NULL, ",");
                k++;
            }

            break;

        }
        i++;

    }



    //printf("%s", buffer);


}
/*--------------------------------------------------------------------------------------------------------------------*/


 void loadQualified(void) {

    char filepath[100];
    snprintf(filepath, sizeof(filepath), "F1master/%s/%s-%s.f1master", options.gpname, options.gpname, GP_LIST[options.trackNumber].name);

    FILE *patate = fopen(filepath, "r");
    if (!patate) {
        perror("🥔🥔🥔🥔🥔");
        return;
    }

    char buffer[1024];


    //fgets(buffer, sizeof(buffer), patate);//read first line of the file

    int i = 0;
    int numberOfF1toload = 15;


    int stopReading = 4;//lign 4 in the .F1master is the result of q1


    if (options.raceType == Qualif3) {stopReading = 5;numberOfF1toload = 10;} //result of q2

    while (fgets(buffer, sizeof(buffer), patate)) {
        if (i == stopReading) {
            int k = 0;
            char *tok = strtok(buffer, ",");
            while (tok && k < numberOfF1toload) {
                (*data).qualifiedKard[k++] = (int)strtol(tok, NULL, 10);
                tok = strtok(NULL, ",");
            }


            break;

        }
        i++;

    }

    fclose(patate);


    //printf("%s", buffer);


}
/*--------------------------------------------------------------------------------------------------------------------*/

int lauchTheEvent(void) {


    switch (options.raceType) {
        case Qualif2:
        case Qualif3:
            loadQualified();
            break;
        default:
            break;
    }




    animation();

    resetChildAndMemory();
    setTimeForTherace();

    int readytosave = 0;


    //printf("helllo\n");
    //while (1) {}

    int fd[NUMBEROFKART][2];
    for (int i = 0; i < NUMBEROFKART; ++i) {
        fd[i][0] = fd[i][1] = -1;
    }

    int qualifiedflag = 0;

    int numberOfRunningkart = 15;

    if (options.raceType == Qualif3) numberOfRunningkart = 10;




    for (int i =0; i<NUMBEROFKART;i++) {
        qualifiedflag = 0;

        int iSave;




        if (options.raceType == GrandP) {

            iSave = i;







            switch (finalPostion[i]) {
                case 1:  i = 0;  break;
                case 2:  i = 1;  break;
                case 3:  i = 2;  break;
                case 4:  i = 3;  break;
                case 10: i = 4;  break;
                case 11: i = 5;  break;
                case 14: i = 6;  break;
                case 16: i = 7;  break;
                case 18: i = 8;  break;
                case 20: i = 9;  break;
                case 22: i = 10; break;
                case 23: i = 11; break;
                case 24: i = 12; break;
                case 27: i = 13; break;
                case 31: i = 14; break;
                case 44: i = 15; break;
                case 55: i = 16; break;
                case 63: i = 17; break;
                case 77: i = 18; break;
                case 81: i = 19; break;
            }








        }



        switch (options.raceType) {
            case Qualif2:
            case Qualif3:

                int a = currentRacers[i].number;
                //printf("🥔--->%i\n",a);

                for (int i=0; i<numberOfRunningkart; i++) {


                    //printf("%i\n",(*data).qualifiedKard[i]);



                    if ((a) == (*data).qualifiedKard[i]) {
                        qualifiedflag = 1;
                        break;

                    }

                }
                break;

            default:
                qualifiedflag = 1;

        }

        //printf("%i\n",qualifiedflag);

        if (!qualifiedflag){continue;}

        pipe(fd[i]);



        pid_t pid = fork();

        if (pid > 0) {
            close(fd[i][1]);

            int fl = fcntl(fd[i][0], F_GETFL);
            fcntl(fd[i][0], F_SETFL, fl | O_NONBLOCK);

        }

        srand(time(NULL) ^ getpid());//regen random for each children


        if (pid == 0) {
            //printf("kidsss\n");
            close(fd[i][0]);



            genTimeCore(&options,fd[i],i);
            _exit(EXIT_SUCCESS);
        }

        //parent



        close(fd[i][1]);

        if (options.raceType == GrandP) {
            i = iSave;

        }





    }


    //printf("parent\n");



    struct pollfd p[NUMBEROFKART];
    int pcount = 0;
    for (int i = 0; i < NUMBEROFKART; ++i) {
        if (fd[i][0] != -1) {
            p[pcount].fd = fd[i][0];
            p[pcount].events = POLLIN;
            pcount++;
        }
    }

    pid_t pidTime = fork();

    if (pidTime == 0) {

        while ((*data).time_left) {
            sleep((1));
            (*data).time_left-=(1*options.speedfactor);

            if ((*data).time_left < 0) {
                (*data).time_left = 0;
            }
        }

        _exit(0);

    }



 while ((*data).time_left)
{
    for (int i = 0; i < NUMBEROFKART; ++i) {
        fcntl(fd[i][0], F_SETFL, O_NONBLOCK);
        p[i].fd     = fd[i][0];
        p[i].events = POLLIN;
    }

    while ((*data).time_left) {
        sleep(1);

        int ready = poll(p, NUMBEROFKART, 0);
        if (ready == -1) {
            perror("poll");
            break;
        }

        if (ready > 0) {
            for (int i = 0; i < NUMBEROFKART; ++i) {
                if (p[i].fd == -1) continue;
                if (p[i].revents & POLLIN) {
                    ssize_t n;
                    while ((*data).time_left && (n = read(fd[i][0],
                                     &karts[i],
                                     sizeof karts[i])) == sizeof karts[i]) {
                        if ((*data).time_left < 0) {
                            (*data).time_left = 0;
                        }
                    }

                    if (n == 0) {
                        close(fd[i][0]);
                        p[i].fd = -1;
                    } else if (n == -1 && errno != EAGAIN) {
                        perror("read");
                    }
                }
            }
        }


        reap_children_nonblock();
        displayPractice(readytosave);
        //printf("aaaaaaaa%i\n",(*data).time_left);
        if ((*data).time_left < 0) {
            (*data).time_left = 0;
        }
    }


}//






    readytosave =1;
    //genTimeCore(&options);
    displayPractice(readytosave);
    close(fd[0]);
    return 0;

}


int eventLauncher(void) {


    switch (options.raceType) {
        case Essaie1:
            options.raceType = Essaie1;
            lauchTheEvent();
        case Essaie2:
            options.raceType = Essaie2;
            lauchTheEvent();
        case Essaie3:
            options.raceType = Essaie3;
            lauchTheEvent();
        case Qualif1:
            options.raceType = Qualif1;
            //loadEliminated();
            //options.speedfactor = 20;
            lauchTheEvent();
        case Qualif2:
            options.raceType = Qualif2;

            lauchTheEvent();
        case Qualif3:
            options.raceType = Qualif3;

            lauchTheEvent();
        case GrandP:
            //options.speedfactor =1;
            options.raceType = GrandP;
            loadPostion();



            printf("Final starting position:\n\n");


            for (int i = 0; i < 20; i++) {

                printf("%i-->%i\n",i+1,finalPostion[i]);

            }

            sleep(5);
            //
            //
            // while (1){}



            lauchTheEvent();
            break;

        case SpecialEssaie1:
            options.raceType = SpecialEssaie1;
            //while (1){}
            lauchTheEvent();
        case SpecialQualifi1:
            options.raceType = SpecialQualifi1;
            lauchTheEvent();
        case SpecialQualifi2:
            options.raceType = SpecialQualifi2;
            lauchTheEvent();
        case SpecialQualifi3:
            options.raceType = SpecialQualifi3;
            lauchTheEvent();
        case SpecialSprint:
            options.raceType = SpecialSprint;
            lauchTheEvent();
        case SpecialQualifi4:
            options.raceType = SpecialQualifi4;
            lauchTheEvent();
        case SpecialQualifi5:
            options.raceType = SpecialQualifi5;
            lauchTheEvent();
        case SpecialQualifi6:
            options.raceType = SpecialQualifi6;
            lauchTheEvent();
        case SpecialGrandP:
            options.raceType = SpecialGrandP;
            lauchTheEvent();
            break;


    }
}



/*--------------------------------------------------------------------------------------------------------------------*/


int setRacetype(void) {


    char filepath[100];
    snprintf(filepath, sizeof(filepath), "F1master/%s/%s-%s.f1master", options.gpname, options.gpname, GP_LIST[options.trackNumber].name);

    FILE *patate = fopen(filepath, "r");
    if (!patate) {
        perror("🥔🥔🥔🥔🥔");
        return;
    }

    char buffer[1024];


    //fgets(buffer, sizeof(buffer), patate);//read first line of the file

    int i = 0;




    while (fgets(buffer, sizeof(buffer), patate)) {

        i++;



        }


    fclose(patate);



    return i-1;

    }







    //printf("%s", buffer);







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
        "2. Continue a weekend",
        "3. Show results for a Saison",
        "4. Show tracks",
        "5. Show current loaded racers",
        "6. Exit"
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
            //pommedeterre
            options.raceType = Essaie1;
            setGPname();
            //strcpy(options.gpname, "robin");//to kick
            options.trackNumber = trackSelection();
            createAfile();
            //options.special = weekendTypeSelection();


            options.special=GP_LIST[options.trackNumber].hasSprint;

            options.special = 1;

            if (options.special){options.raceType = SpecialEssaie1;}
            saveEventType();
            //options.laps = 30;//to kick
            options.speedfactor = speedfactorchanger();
            //options.speedfactor = 200;
            changeDriverTheme(&currentRacers);
            //options.speedfactor = SPEEDFACTOR;
            //(*data).time_left = TIME_FOR_PRACTICE * SECOND_PER_MINUTE;

            eventLauncher();

            mainMenu();


         case 1:
            //pommedeterre
            setGPname();
            //strcpy(options.gpname, "robin");//to kick
            options.trackNumber = trackSelection();
            options.special=GP_LIST[options.trackNumber].hasSprint;
            if (options.special){options.raceType = SpecialEssaie1;}//yep now the number at the top of the file is useless but i'have to study my cisco stuff
            //options.laps = 30;//to kick
            options.speedfactor = speedfactorchanger();
            //options.speedfactor = 200;
            changeDriverTheme(&currentRacers);
            //options.speedfactor = SPEEDFACTOR;
            //(*data).time_left = TIME_FOR_PRACTICE * SECOND_PER_MINUTE;

            options.raceType = setRacetype();

            eventLauncher();
            mainMenu();
            break;

        case 2:
            printf("comming soon... meaby...");
            mainMenu();
            break;
        case 3:
            //displayResultOfAGP();
            showAllTracks();
            mainMenu();
            break;

        case 4:
            showAllDrivers(currentRacers);
            mainMenu();
            break;
        case 5:

            printf("byebye !\n");
            exit(0);

        default:
            printf("Invalid selection\n");
    }

    return 0;
}





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
    //options.speedfactor =100;


    options.raceType =1;
    options.verbose =1;
    //genTimeCore(&options);
    options.raceType =0;



    //genTimeCore(&options);




    mainMenu();


    sem_destroy(&((*data).semaphore));



    return 0;
}
