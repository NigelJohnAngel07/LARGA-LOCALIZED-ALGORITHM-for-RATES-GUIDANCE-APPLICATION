#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- Definitions and Structs ---
#define MAX_LINE_LENGTH 256
#define LOCATION_NAME_MAX 100

typedef struct {
    int location_ID;
    char location_name[LOCATION_NAME_MAX];
} Data;

// --- Helper: Create CSV Data (Unique Only) ---
void create_csv_file() {
    FILE *f = fopen("locations.csv", "w");
    if (f == NULL) {
        perror("Error creating CSV file");
        exit(1);
    }
    // Updated count to 23 (Unique locations only)
    fprintf(f, "23\n");
    fprintf(f, "100,Mintal Elementary School\n");
    fprintf(f, "101,Mintal Catholic Church\n");
    fprintf(f, "102,Trese Catalunan Pequeno\n");
    fprintf(f, "103,Crossing Catalunan Pequeno\n");
    fprintf(f, "104,Dayrit Farm\n");
    fprintf(f, "105,Reyes Residence\n");
    fprintf(f, "106,Cal's Repair Shop\n");
    fprintf(f, "107,Ulas\n");
    fprintf(f, "108,DXSS\n");
    fprintf(f, "109,Tahimik Avenue\n");
    fprintf(f, "110,Matina Crossing\n");
    fprintf(f, "111,ABS-CBN junction\n");
    fprintf(f, "112,SM City\n");
    fprintf(f, "113,Ecoland Terminal\n");
    fprintf(f, "114,Almendras Gym\n");
    fprintf(f, "115,Roxas Avenue\n");
    fprintf(f, "116,Mindanao Fruit(?)\n");
    fprintf(f, "117,Bangkerohan\n");
    fprintf(f, "118,Ma-a Crossing\n");
    fprintf(f, "119,Tulip Drive\n");
    fprintf(f, "120,La Suerte Gallera\n");
    fprintf(f, "121,Matina Crossing\n");
    fprintf(f, "122,Tahimik Avenue\n");
    fclose(f);
}

// --- Helper: Read CSV to Array ---
Data* createArray(char routefile[50], int *count) {
    FILE *file = fopen(routefile, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    char line[MAX_LINE_LENGTH];

    // Read array size
    if (!fgets(line, sizeof(line), file)) {
        fclose(file);
        return NULL;
    }
    int array_size = atoi(line);

    Data *Route = malloc(array_size * sizeof(Data));
    if (Route == NULL) {
        fclose(file);
        return NULL;
    }

    int lines_read = 0;
    while (fgets(line, sizeof(line), file) && lines_read < array_size) {
        char *id_str = strtok(line, ",");
        char *name = strtok(NULL, "\n");

        if (id_str && name) {
            // Clean up windows newlines if present
            size_t len = strlen(name);
            if (len > 0 && name[len - 1] == '\r') name[len - 1] = '\0';

            Route[lines_read].location_ID = atoi(id_str);
            strncpy(Route[lines_read].location_name, name, LOCATION_NAME_MAX - 1);
            Route[lines_read].location_name[LOCATION_NAME_MAX - 1] = '\0';

            lines_read++;
        }
    }

    fclose(file);
    *count = lines_read;
    return Route;
}

void displayList(Data *Route, int total_records) {
    printf("\n--- List Contents ---\n");
    printf("| Index | Location ID | Location Name                          |\n");
    printf("|-------|-------------|----------------------------------------|\n");
    for (int i = 0; i < total_records; i++) {
        printf("| %5d | %11d | %-38s |\n", i, Route[i].location_ID, Route[i].location_name);
    }
    printf("----------------------------------------------------------------\n");
}

// --- Main Program ---
int main() {
    create_csv_file();

    char routefile[] = "locations.csv";
    int record_count = 0;
    Data *Route = createArray(routefile, &record_count);

    displayList(Route, record_count);

    int origin_index, destination_index;

    printf("\n--- Route Calculator ---\n");

    // 1. Get Origin Index
    printf("Enter Origin Index (0 to %d): ", record_count - 1);
    scanf("%d", &origin_index);

    // 2. Get Destination Index
    printf("Enter Destination Index (0 to %d): ", record_count - 1);
    scanf("%d", &destination_index);

    // 3. Validate Inputs
    if (origin_index < 0 || origin_index >= record_count ||
        destination_index < 0 || destination_index >= record_count) {
        printf("\nError: Invalid Index entered. Please run the program again.\n");
        free(Route);
        return 0;
    }

    // 4. Calculate Distance
    int distance = (destination_index - origin_index + record_count) % record_count;

    // 5. Calculate Fare (UPDATED)
    float fare = 0.0;
    char choice;

    if (distance <= 4) {
        fare = 13.0;
    } else {
        // 13 pesos for first 4km + 1.75 pesos for every km after
        fare = 13.0 + ((distance - 4) * 2);
    }

    // Discount Logic
    printf("Are you a senior citizen / PWD / student? (y/n): ");
    scanf(" %c", &choice);

    if (choice == 'y' || choice == 'Y') {
        fare = fare * 0.80; // 20% discount
        printf("Discount applied (20%% off).\n");
    }

    // 6. Final Output
    printf("\n----------------------------------------------------------------\n");
    printf("Origin:         [%d] %s (ID: %d)\n",
           origin_index, Route[origin_index].location_name, Route[origin_index].location_ID);
    printf("Destination:    [%d] %s (ID: %d)\n",
           destination_index, Route[destination_index].location_name, Route[destination_index].location_ID);
    printf("Distance:       %d km\n", distance);
    printf("Total Fare:     Php %.2f\n", fare);
    printf("----------------------------------------------------------------\n");

    free(Route);
    return 0;
}
