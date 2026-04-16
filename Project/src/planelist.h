#ifndef _PLANELIST_H
#define _PLANELIST_H

#include "airplane.h"

void init_planelist(void); // Inits global plane list
void add_airplane(airplane *plane); // Adds new airplane to global plane list
void remove_airplane(airplane *plane); // Removes airplane from global plane list
int is_flightid_taken(char *id); // Check if a flight ID is already taken by another plane
void _destructor(void *data); // Helper destructor for airplane data in the alist

#endif // _PLANELIST_H