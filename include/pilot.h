//
// Created by Bib's on 22-07-25.
//

#ifndef PILOT_H
#define PILOT_H

typedef struct {
    int number;
    const char* name;
    const char* team;
} Driver;


typedef struct {
    char name[50];
    char circuit[100];
    int laps;
    int hasSprint;
    int sprintLaps;
} GrandPrix;

extern const GrandPrix GP_LIST[];

extern const Driver DRIVER_LIST[];
extern const Driver DRIVER_LIST_CRASH[];
extern const Driver DRIVER_TROPEZ[];




#endif //PILOT_H
