#include "route.h" // Include our custom header file


// --- Main Program Logic ---

int main() {
    int distance = 0;
    int origin_location_ID = 113;
    int origin_index,i,destination_index;
    int destination_location_ID = 100;
    char routefile[] = "locations.csv";
    int record_count = 0;

    Data *Route = createArray(routefile,&record_count);
    // Display the list
    displayList(Route, record_count);

    printf("%d\n",record_count);

    int min_distance = record_count; // Start with the maximum possible stops

    for (int i = 0; i < record_count; i++) {
        if (Route[i].location_ID == origin_location_ID) {

            for (int j = 0; j < record_count; j++) {
                if (Route[j].location_ID == destination_location_ID) {

                    // One-way Circular Math:
                    // This formula always calculates the distance MOVING FORWARD
                    int current_dist = (j - i + record_count) % record_count;

                    // We still want the "shortest" because there are multiple
                    // instances of the same ID in your CSV.
                    if (current_dist < min_distance) {
                        min_distance = current_dist;
                    }
                }
            }
        }
    }

    printf("The shortest distance in this one-way loop is %d km.\n", min_distance);

    free(Route);
    Route = NULL;

    return 0;
}
