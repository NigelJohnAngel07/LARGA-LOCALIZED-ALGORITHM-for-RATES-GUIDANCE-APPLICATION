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

    printf("LARGA: LOCALIZED ALGORITHM for RATES GUIDANCE APPLICATION\n");
    printf("Select Route");

    Route *RouteSelection = loadRouteOptions("route_selection.csv", &totalRoutes);

    if (RouteSelection == NULL) return 1;

    printf("\nChoose a route index: ");
    scanf("%d", &choice);
    while ((ch = getchar()) != '\n' && ch != EOF);

    strcpy(file, RouteSelection[choice].location_file);
    Data *Route = createArray(file, &record_count);

    displayList(Route, record_count);
    printf("Origin: ");
    scanf("%d", &origin_index);
    while ((ch = getchar()) != '\n' && ch != EOF);
    printf("Destination: ");
    scanf("%d", &destination_index);
    while ((ch = getchar()) != '\n' && ch != EOF);

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

