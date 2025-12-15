#include "route.h"

Route* loadRouteOptions(char *masterFile, int *count) {
    FILE *file = fopen(masterFile, "r");
    if (file == NULL) {
        perror("Error opening master file");
        return NULL;
    }

    char line[150];
    // Read the first line to get the number of routes (e.g., "3")
    if (fgets(line, sizeof(line), file)) {
        *count = atoi(line);
    } else {
        fclose(file);
        return NULL;
    }

    // Allocate memory for the array of structs
    Route *routeList = malloc((*count) * sizeof(Route));
    if (routeList == NULL) {
        fclose(file);
        return NULL;
    }

    printf("\n--- Available Jeepney Routes ---\n");

    int i = 0;
    while (i < *count && fgets(line, sizeof(line), file)) {
        // Remove trailing newline
        line[strcspn(line, "\n")] = 0;

        // 1. Extract Route Name (for printing)
        char *routeName = strtok(line, ",");

        // 2. Extract Location File (store in struct)
        char *locFile = strtok(NULL, ",");

        // 3. Extract Route File (store in struct)
        char *rteFile = strtok(NULL, ",");

        if (routeName && locFile && rteFile) {
            printf("[%d] %s\n", i, routeName);

            // Copy strings into the struct array
            strncpy(routeList[i].location_file, locFile, 49);
            strncpy(routeList[i].route_file, rteFile, 49);

            // Ensure null termination
            routeList[i].location_file[49] = '\0';
            routeList[i].route_file[49] = '\0';

            i++;
        }
    }

    fclose(file);
    return routeList;
}

Data* createArray(char routefile[50], int *count) {
    FILE *file = fopen(routefile, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    char line[MAX_LINE_LENGTH];

    // 1. Determine Array Size
    // Usually, the first line of a CSV might contain the count,
    // or you need to count the lines manually first.
    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return NULL;
    }
    int array_size = atoi(line); // Assumes first line is the number of entries

    // 2. Allocate Memory
    Data *Route = malloc(array_size * sizeof(Data));
    if (Route == NULL) {
        fclose(file);
        return NULL;
    }

    int lines_read = 0;
    int index;
    while (fgets(line, sizeof(line), file) && lines_read < array_size) {
        char *id_str = strtok(line, ",");
        char *name = strtok(NULL, "\n");

        if (id_str && name) {
            // Cleanup trailing carriage return for Windows compatibility
            size_t len = strlen(name);
            if (len > 0 && name[len - 1] == '\r') name[len - 1] = '\0';

            Route[lines_read].location_ID = atoi(id_str);
            strncpy(Route[lines_read].location_name, name, LOCATION_NAME_MAX - 1);
            Route[lines_read].location_name[LOCATION_NAME_MAX - 1] = '\0';

            lines_read++;
        }
    }

    fclose(file);
    *count = lines_read; // Return the actual number read via pointer
    return Route; // Return the pointer to the array
}

void displayList(Data *Route, int total_records) {
    if (Route == NULL || total_records == 0) {
        printf("The list is empty.\n");
        return;
    }

    printf("\n--- List Contents ---\n");
    printf("| Index | Location ID | Location Name                          |\n");
    printf("|-------|-------------|----------------------------------------|\n");

    for (int i = 0; i < total_records; i++) {
        printf("| %5d | %11d | %-38s |\n",
               i,
               Route[i].location_ID,
               Route[i].location_name);
    }

    printf("----------------------------------------------------------------\n");
}

int calculateMinDistance(Data *Route, int record_count, int origin_ID, int dest_ID) {
    // Initialize with a value larger than any possible distance
    int min_distance = record_count + 1;

    for (int i = 0; i < record_count; i++) {
        if (Route[i].location_ID == origin_ID) {
            for (int j = 0; j < record_count; j++) {
                if (Route[j].location_ID == dest_ID) {
                    // One-way Circular Math
                    int current_dist = (j - i + record_count) % record_count;

                    // Update min if current is shorter
                    if (current_dist < min_distance) {
                        min_distance = current_dist;
                    }
                }
            }
        }
    }

    // If 'found' is still 0, it means one or both IDs weren't in the list
    return min_distance;
}

float calculateFare(int distance) {
    float fare = 0.0;
    char choice;

    // 1. Calculate Regular Fare
    if (distance <= 4) {
        fare = 13.00;
    } else {
        fare = 13.00 + ((distance - 4) * 1.8);
    }

    // 2. Discount Logic
    printf("Are you a student / elderly / disabled? (y/n): ");
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        // Apply 20% discount
        fare = fare * 0.80;

        printf("Discounted fare applied.\n");
    }

    fare = round(fare * 4) / 4;

    return fare;
}

void displayReceipt(Data *Route, int origin_index, int destination_index, int distance, float fare) {
    printf("\n================================================================\n");
    printf("                       JEEPNEY TRIP RECEIPT                     \n");
    printf("----------------------------------------------------------------\n");

    // Display Origin Details
    printf("Origin:         [%d] %-30s (ID: %d)\n",
           origin_index,
           Route[origin_index].location_name,
           Route[origin_index].location_ID);

    // Display Destination Details
    printf("Destination:    [%d] %-30s (ID: %d)\n",
           destination_index,
           Route[destination_index].location_name,
           Route[destination_index].location_ID);

    printf("----------------------------------------------------------------\n");
    printf("Distance:       %d stops (approx. %d km)\n", distance, distance);
    printf("Total Fare:     PHP %.2f\n", fare);
    printf("================================================================\n");
    printf("              Thank you for riding! Keep safe!                  \n");
    printf("================================================================\n\n");
}
