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

extern const Driver DRIVER_LIST[];
extern const Driver DRIVER_LIST_CRASH[];
extern const Driver DRIVER_TROPEZ[];



#endif //PILOT_H
