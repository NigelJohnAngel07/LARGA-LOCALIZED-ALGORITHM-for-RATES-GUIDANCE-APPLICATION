#ifndef ROUTE_H_INCLUDED
#define ROUTE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Constants for buffer sizes
#define MAX_LINE_LENGTH 256
#define LOCATION_NAME_MAX 100

// Define the structure once here
typedef struct {
    int location_ID;
    char location_name[LOCATION_NAME_MAX];
} Data;

typedef struct {
    char location_file [50];
    char route_file [50];
} Route;

// Function Prototypes (Declarations)
// These tell the compiler what the functions look like

Route* loadRouteOptions(char *masterFile, int *count);
Data* createArray(char routefile[50], int *count);
void displayList(Data *Route, int total_records);
int calculateMinDistance(Data *Route, int record_count, int origin_ID, int dest_ID);
float calculateFare(int distance);
void displayReceipt(Data *Route, int origin_idx, int dest_idx, int distance, float fare);
float round25cent(float value);

#endif // ARRAY_H_INCLUDED
