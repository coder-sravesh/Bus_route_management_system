#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STOPS 5

typedef struct
{
    int busID;
    char busName[50];
    int routeID;
} Bus;

typedef struct
{
    int routeID;
    char routeName[50];
    char stops[MAX_STOPS][100]; // Format: "StopName - Timing"
} Route;

void addBus(FILE *busFile);
void updateBus(FILE *busFile);
void deleteBus(FILE *busFile);
void addRoute(FILE *routeFile);
void updateRoute(FILE *routeFile);
void deleteRoute(FILE *routeFile);
void filterByRouteID(FILE *busFile);
void filterByStopName(FILE *busFile, FILE *routeFile);
void viewAllBuses(FILE *busFile);
void viewAllRoutes(FILE *routeFile);

void showMenu(FILE *busFile, FILE *routeFile)
{
    int choice;
    do
    {
        system("clear");
        printf("=========================================\n");
        printf("  Bus Route Management System\n");
        printf("  Developed by Sravesh (2025)\n");
        printf("=========================================\n\n");
        printf("1. Add Bus\n2. Update Bus\n3. Delete Bus\n");
        printf("4. Add Route\n5. Update Route\n6. Delete Route\n");
        printf("7. Filter Bus by Route ID\n8. View all buses\n9. View all routes\n10. Filter Bus by Stop Name\n");
        printf("9. Exit\nChoice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
            system("clear");
            addBus(busFile);
            break;
        case 2:
            system("clear");
            updateBus(busFile);
            break;
        case 3:
            system("clear");
            deleteBus(busFile);
            break;
        case 4:
            system("clear");
            addRoute(routeFile);
            break;
        case 5:
            system("clear");
            updateRoute(routeFile);
            break;
        case 6:
            system("clear");
            deleteRoute(routeFile);
            break;
        case 7:
            system("clear");
            filterByRouteID(busFile);
            break;
        case 8:
            system("clear");
            viewAllBuses(busFile);
            break;
        case 9:
            system("clear");
            viewAllRoutes(routeFile);
            break;
        case 10:
            system("clear");
            filterByStopName(busFile, routeFile);
            break;
        case 11:
            return;
        default:
            printf("Invalid choice\n");
        }

        printf("\nContinue? (y/n): ");
    } while (getchar() == 'y');
}

void viewAllBuses(FILE *busFile)
{
    rewind(busFile);
    char line[128];
    int found = 0;
    printf("\n--- All Buses ---\n");
    while (fgets(line, sizeof(line), busFile))
    {
        char id[10], name[50], routeId[10];
        if (sscanf(line, "%[^,],%[^,],%s", id, name, routeId) == 3)
        {
            printf("\n\nID: %s\n Bus Name: %s\n Route ID: %s\n", id, name, routeId);
            found = 1;
        }
    }
    if (!found)
        printf("No buses found.\n");
}

void viewAllRoutes(FILE *routeFile)
{
    rewind(routeFile);
    char line[512];
    int found = 0;
    printf("\n--- All Routes ---\n");
    while (fgets(line, sizeof(line), routeFile))
    {
        char id[10], name[50], stops[400];
        if (sscanf(line, "%[^,],%[^,],%[^\n]", id, name, stops) == 3)
        {
            printf("Route Name: %s, ID: %s\n", name, id);
            char *stop = strtok(stops, ";");
            int i = 1;
            while (stop != NULL)
            {
                char stopName[50], timing[50];
                if (sscanf(stop, "%[^-]-%[^\n]", stopName, timing) == 2)
                {
                    printf("  Stop %d: %s - %s\n", i++, stopName, timing);
                }
                stop = strtok(NULL, ";");
            }
            found = 1;
        }
    }
    if (!found)
        printf("No routes found.\n");
}

// Add Bus
void addBus(FILE *busFile)
{
    char id[10], name[50], routeId[10];
    printf("Enter bus in format:\n  ID (e.g. B1)\n  Name (e.g. City Express)\n  Route ID (e.g. R101)\n");

    printf("ID: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;
    printf("Name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;
    printf("Route ID: ");
    fgets(routeId, sizeof(routeId), stdin);
    routeId[strcspn(routeId, "\n")] = 0;

    fprintf(busFile, "%s,%s,%s\n", id, name, routeId);
    printf("Bus added.\n");
}

// Update Bus
void updateBus(FILE *busFile)
{
    int id, found = 0;
    printf("Enter Bus ID to update: ");
    scanf("%d", &id);

    FILE *temp = fopen("temp.txt", "w");
    rewind(busFile);
    Bus b;
    char line[100];

    while (fgets(line, sizeof(line), busFile))
    {
        sscanf(line, "%d,%49[^,],%d", &b.busID, b.busName, &b.routeID);
        if (b.busID == id)
        {
            found = 1;
            printf("New Name: ");
            getchar();
            fgets(b.busName, 50, stdin);
            b.busName[strcspn(b.busName, "\n")] = 0;
            printf("New Route ID: ");
            scanf("%d", &b.routeID);
        }
        fprintf(temp, "%d,%s,%d\n", b.busID, b.busName, b.routeID);
    }
    fclose(busFile);
    fclose(temp);
    remove("bus.txt");
    rename("temp.txt", "bus.txt");
    busFile = fopen("bus.txt", "a+");
    printf(found ? "Bus updated.\n" : "Bus not found.\n");
}

// Delete Bus
void deleteBus(FILE *busFile)
{
    int id, found = 0;
    printf("Enter Bus ID to delete: ");
    scanf("%d", &id);
    FILE *temp = fopen("temp.txt", "w");
    rewind(busFile);
    Bus b;
    char line[100];

    while (fgets(line, sizeof(line), busFile))
    {
        sscanf(line, "%d,%49[^,],%d", &b.busID, b.busName, &b.routeID);
        if (b.busID != id)
            fprintf(temp, "%d,%s,%d\n", b.busID, b.busName, b.routeID);
        else
            found = 1;
    }
    fclose(busFile);
    fclose(temp);
    remove("bus.txt");
    rename("temp.txt", "bus.txt");
    busFile = fopen("bus.txt", "a+");
    printf(found ? "Bus deleted.\n" : "Bus not found.\n");
}

// Add Route
void addRoute(FILE *routeFile)
{
    char id[10], name[50], stops[400];
    printf("Enter route in format:\n  ID (e.g. R101)\n  Name (e.g. City Loop)\n  Stops in Stop-Timing;Stop-Timing format (e.g. Central-10AM;Market-10:30AM)\n");

    printf("ID: ");
    fgets(id, sizeof(id), stdin);
    id[strcspn(id, "\n")] = 0;
    printf("Name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;
    printf("Stops: ");
    fgets(stops, sizeof(stops), stdin);
    stops[strcspn(stops, "\n")] = 0;

    fprintf(routeFile, "%s,%s,%s\n", id, name, stops);
    printf("Route added.\n");
}

// Update Route
void updateRoute(FILE *routeFile)
{
    int id, found = 0;
    printf("Enter Route ID to update: ");
    scanf("%d", &id);
    FILE *temp = fopen("temp.txt", "w");
    rewind(routeFile);
    char line[512];

    while (fgets(line, sizeof(line), routeFile))
    {
        Route r;
        char *token = strtok(line, ",");
        if (!token)
            continue;
        r.routeID = atoi(token);
        if (r.routeID == id)
        {
            found = 1;
            printf("New Route Name: ");
            getchar();
            fgets(r.routeName, 50, stdin);
            r.routeName[strcspn(r.routeName, "\n")] = 0;
            printf("Update stops? (y/n): ");
            char ch = getchar();
            getchar();
            if (ch == 'y')
            {
                for (int i = 0; i < MAX_STOPS; i++)
                {
                    printf("Stop %d (Name - Time): ", i + 1);
                    fgets(r.stops[i], 100, stdin);
                    r.stops[i][strcspn(r.stops[i], "\n")] = 0;
                }
            }
            else
            {
                for (int i = 0; i < MAX_STOPS; i++)
                    strcpy(r.stops[i], strtok(NULL, ","));
            }
            fprintf(temp, "%d,%s", r.routeID, r.routeName);
            for (int i = 0; i < MAX_STOPS; i++)
                fprintf(temp, ",%s", r.stops[i]);
            fprintf(temp, "\n");
        }
        else
        {
            fprintf(temp, "%d", r.routeID);
            for (int i = 0; i < 6; i++)
            {
                token = strtok(NULL, ",");
                if (token)
                    fprintf(temp, ",%s", token);
            }
            fprintf(temp, "\n");
        }
    }
    fclose(routeFile);
    fclose(temp);
    remove("route.txt");
    rename("temp.txt", "route.txt");
    routeFile = fopen("route.txt", "a+");
    printf(found ? "Route updated.\n" : "Route not found.\n");
}

// Delete Route
void deleteRoute(FILE *routeFile)
{
    int id, found = 0;
    printf("Enter Route ID to delete: ");
    scanf("%d", &id);
    FILE *temp = fopen("temp.txt", "w");
    rewind(routeFile);
    char line[512];
    while (fgets(line, sizeof(line), routeFile))
    {
        if (atoi(strtok(line, ",")) != id)
            fprintf(temp, "%s", line);
        else
            found = 1;
    }
    fclose(routeFile);
    fclose(temp);
    remove("route.txt");
    rename("temp.txt", "route.txt");
    routeFile = fopen("route.txt", "a+");
    printf(found ? "Route deleted.\n" : "Route not found.\n");
}

// Filter Bus by Route ID
void filterByRouteID(FILE *busFile)
{
    int id, found = 0;
    printf("Enter Route ID: ");
    scanf("%d", &id);
    rewind(busFile);
    Bus b;
    char line[100];
    while (fgets(line, sizeof(line), busFile))
    {
        sscanf(line, "%d,%[^,],%d", &b.busID, b.busName, &b.routeID);
        if (b.routeID == id)
        {
            printf("Bus ID: %d | Name: %s | Route ID: %d\n", b.busID, b.busName, b.routeID);
            found = 1;
        }
    }
    if (!found)
        printf("No buses found.\n");
}

// Filter Bus by Stop Name
void filterByStopName(FILE *busFile, FILE *routeFile)
{
    char stop[50];
    int matchIDs[100], count = 0;
    printf("Enter Stop Name: ");
    getchar();
    fgets(stop, 50, stdin);
    stop[strcspn(stop, "\n")] = 0;

    rewind(routeFile);
    char line[512];
    while (fgets(line, sizeof(line), routeFile))
    {
        char *tok = strtok(line, ",");
        int routeID = atoi(tok);
        tok = strtok(NULL, ",");
        for (int i = 0; i < MAX_STOPS; i++)
        {
            tok = strtok(NULL, ",");
            if (tok && strstr(tok, stop))
            {
                matchIDs[count++] = routeID;
                break;
            }
        }
    }

    rewind(busFile);
    Bus b;
    char bline[100];
    int found = 0;
    while (fgets(bline, sizeof(bline), busFile))
    {
        sscanf(bline, "%d,%[^,],%d", &b.busID, b.busName, &b.routeID);
        for (int i = 0; i < count; i++)
        {
            if (b.routeID == matchIDs[i])
            {
                printf("Bus ID: %d | Name: %s | Route ID: %d\n", b.busID, b.busName, b.routeID);
                found = 1;
            }
        }
    }
    if (!found)
        printf("No buses found.\n");
}

// Main
int main()
{
    FILE *busFile = fopen("bus.txt", "a+");
    FILE *routeFile = fopen("route.txt", "a+");
    if (!busFile || !routeFile)
    {
        printf("File error.\n");
        return 1;
    }

    showMenu(busFile, routeFile);

    fclose(busFile);
    fclose(routeFile);
    return 0;
}
