#include "route.h" // Include our custom header file


// --- Main Program Logic ---

int main() {
    int distance = 0;
    int origin_location_ID;
    int origin_index,i,destination_index,ch,route_index;
    int destination_location_ID;
    char file[50];
    int record_count = 0;
    int totalRoutes = 0;
    int choice;
    float fare;
    int valid = 0;//for error handling
    int maxOption = 2;


    printf("LARGA: LOCALIZED ALGORITHM for RATES GUIDANCE APPLICATION\n\n");
    printf("Select Route");

    Route *RouteSelection = loadRouteOptions("route_selection.csv", &totalRoutes);

    if (RouteSelection == NULL) return 1;

    while (!valid) {
        printf("\nChoose a route index (0-%d): ", maxOption);
        if (scanf("%d", &choice) == 1) {
            if (choice < 0) {
                printf("Invalid input. The index cannot be negative.\n");
            }
            else if (choice > maxOption) {
                printf("Invalid input. Please choose an index between 0 and %d.\n", maxOption);
            }
            else {
                valid = 1;
            }
        } else {
            printf("Invalid input. Please enter a number.\n");
        }
        while ((ch = getchar()) != '\n' && ch != EOF);
    }

    strcpy(file, RouteSelection[choice].location_file);
    Data *Route = createArray(file, &record_count);

    displayList(Route, record_count);



    while (1) {
        printf("Origin (0-21): ");

        if (scanf("%d", &origin_index) == 1) {
            while ((ch = getchar()) != '\n' && ch != EOF);
            if (origin_index >= 0 && origin_index <= 21) {
                break;
            } else {
                printf("Invalid input. Please choose an index between 0 and 21.\n");
            }
        } else {
            printf("Invalid input. Please enter a number.\n");
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
    }

    while (1) { // Loop until valid input
        printf("Destination (0-21): ");

        // 1. Attempt to read an integer
        if (scanf("%d", &destination_index) == 1) {

            // 2. Immediately clear the buffer (removes '\n')
            while ((ch = getchar()) != '\n' && ch != EOF);

            // 3. STRICT RANGE CHECK: 0 to 21
            if (destination_index >= 0 && destination_index <= 21) {
                break; // Input is perfect. Exit the loop.
            } else {
                printf("Invalid input. Please choose an index between 0 and 21.\n");
            }

        } else {
            // 4. Input was text/garbage
            printf("Invalid input. Please enter a number.\n");

            // Clear the garbage text from the buffer
            while ((ch = getchar()) != '\n' && ch != EOF);
        }
    }

    origin_location_ID = Route[origin_index].location_ID;
    destination_location_ID = Route[destination_index].location_ID;

    free(Route);

    strcpy(file, RouteSelection[choice].route_file);

    Route = createArray(file,&record_count);

    distance = calculateMinDistance(Route, record_count, origin_location_ID, destination_location_ID);
    fare = calculateFare(distance);

    displayReceipt(Route, origin_index, destination_index,distance, fare);

    free(Route);
    free(RouteSelection);
    Route = NULL;
    RouteSelection = NULL;

    return 0;
}

