#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clearNewline(char *str)
{
    int len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
        str[len - 1] = '\0';
}

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void waitForUser()
{
    printf("\nPress Enter to continue...");
    getchar();
}

// Add Bus
void addBus()
{
    FILE *f = fopen("bus.txt", "a");
    if (!f)
    {
        printf("Cannot open bus.txt\n");
        return;
    }

    char id[10], name[50], routeId[10];
    printf("Bus ID: ");
    fgets(id, sizeof(id), stdin);
    clearNewline(id);
    printf("Bus Name: ");
    fgets(name, sizeof(name), stdin);
    clearNewline(name);
    printf("Route ID: ");
    fgets(routeId, sizeof(routeId), stdin);
    clearNewline(routeId);
    fprintf(f, "%s %s %s\n", id, name, routeId);
    fclose(f);
    printf("Bus added.\n");
}

// Update Bus
void updateBus()
{
    FILE *f = fopen("bus.txt", "r");
    if (!f)
    {
        printf("No bus file\n");
        return;
    }
    FILE *temp = fopen("temp.txt", "w");
    char id[10], bid[10], name[50], routeId[10];
    int found = 0;
    printf("Enter Bus ID: ");
    fgets(id, sizeof(id), stdin);
    clearNewline(id);

    while (fscanf(f, "%s %s %s", bid, name, routeId) == 3)
    {
        if (strcmp(bid, id) == 0)
        {
            found = 1;
            printf("New Name: ");
            fgets(name, sizeof(name), stdin);
            clearNewline(name);
            printf("New Route ID: ");
            fgets(routeId, sizeof(routeId), stdin);
            clearNewline(routeId);
        }
        fprintf(temp, "%s %s %s\n", bid, name, routeId);
    }

    fclose(f);
    fclose(temp);
    remove("bus.txt");
    rename("temp.txt", "bus.txt");
    printf(found ? "Updated.\n" : "Not found.\n");
}

// Delete Bus
void deleteBus()
{
    FILE *f = fopen("bus.txt", "r");
    if (!f)
    {
        printf("No bus file\n");
        return;
    }
    FILE *temp = fopen("temp.txt", "w");
    char id[10], bid[10], name[50], routeId[10];
    int found = 0;
    printf("Bus ID to delete: ");
    fgets(id, sizeof(id), stdin);
    clearNewline(id);

    while (fscanf(f, "%s %s %s", bid, name, routeId) == 3)
    {
        if (strcmp(bid, id) != 0)
            fprintf(temp, "%s %s %s\n", bid, name, routeId);
        else
            found = 1;
    }

    fclose(f);
    fclose(temp);
    remove("bus.txt");
    rename("temp.txt", "bus.txt");
    printf(found ? "Deleted.\n" : "Not found.\n");
}

// Add Route
void addRoute()
{
    FILE *f = fopen("route.txt", "a");
    if (!f)
    {
        printf("Cannot open route.txt\n");
        return;
    }

    char id[10], name[50], stops[200];
    printf("Route ID: ");
    fgets(id, sizeof(id), stdin);
    clearNewline(id);
    printf("Route Name: ");
    fgets(name, sizeof(name), stdin);
    clearNewline(name);
    printf("Stops (Stop1-Time1;Stop2-Time2): ");
    fgets(stops, sizeof(stops), stdin);
    clearNewline(stops);
    fprintf(f, "%s %s %s\n", id, name, stops);
    fclose(f);
    printf("Route added.\n");
}

// Update Route
void updateRoute()
{
    FILE *f = fopen("route.txt", "r");
    if (!f)
    {
        printf("No route file\n");
        return;
    }
    FILE *temp = fopen("temp.txt", "w");
    char id[10], rid[10], name[50], stops[200];
    int found = 0;
    printf("Route ID to update: ");
    fgets(id, sizeof(id), stdin);
    clearNewline(id);

    while (fscanf(f, "%s %s %[^\n]", rid, name, stops) == 3)
    {
        if (strcmp(rid, id) == 0)
        {
            found = 1;
            printf("New Name: ");
            fgets(name, sizeof(name), stdin);
            clearNewline(name);
            printf("New Stops: ");
            fgets(stops, sizeof(stops), stdin);
            clearNewline(stops);
        }
        fprintf(temp, "%s %s %s\n", rid, name, stops);
    }

    fclose(f);
    fclose(temp);
    remove("route.txt");
    rename("temp.txt", "route.txt");
    printf(found ? "Updated.\n" : "Not found.\n");
}

// Delete Route
void deleteRoute()
{
    FILE *f = fopen("route.txt", "r");
    if (!f)
    {
        printf("No route file\n");
        return;
    }
    FILE *temp = fopen("temp.txt", "w");
    char id[10], rid[10], name[50], stops[200];
    int found = 0;
    printf("Route ID to delete: ");
    fgets(id, sizeof(id), stdin);
    clearNewline(id);

    while (fscanf(f, "%s %s %[^\n]", rid, name, stops) == 3)
    {
        if (strcmp(rid, id) != 0)
            fprintf(temp, "%s %s %s\n", rid, name, stops);
        else
            found = 1;
    }

    fclose(f);
    fclose(temp);
    remove("route.txt");
    rename("temp.txt", "route.txt");
    printf(found ? "Deleted.\n" : "Not found.\n");
}

// View All Buses
void viewAllBuses()
{
    FILE *f = fopen("bus.txt", "r");
    if (!f)
    {
        printf("No bus data.\n");
        return;
    }

    char id[10], name[50], routeId[10];
    printf("\n--- All Buses ---\n");
    while (fscanf(f, "%s %s %s", id, name, routeId) == 3)
        printf("ID: %s, Name: %s, Route ID: %s\n", id, name, routeId);
    fclose(f);
}

// View All Routes
void viewAllRoutes()
{
    FILE *f = fopen("route.txt", "r");
    if (!f)
    {
        printf("No route data.\n");
        return;
    }

    char id[10], name[50], stops[200];
    printf("\n--- All Routes ---\n");
    while (fscanf(f, "%s %s %[^\n]", id, name, stops) == 3)
        printf("ID: %s, Name: %s, Stops: %s\n", id, name, stops);
    fclose(f);
}

// Filter Buses by Route Name
void filterBusByRouteName()
{
    char rname[50], rid[10], name[50], stops[200];
    printf("Enter Route Name: ");
    fgets(rname, sizeof(rname), stdin);
    clearNewline(rname);

    FILE *rf = fopen("route.txt", "r");
    int found = 0;
    while (fscanf(rf, "%s %s %[^\n]", rid, name, stops) == 3)
    {
        if (strcmp(name, rname) == 0)
        {
            found = 1;
            break;
        }
    }
    fclose(rf);

    if (!found)
    {
        printf("Route not found.\n");
        return;
    }

    FILE *bf = fopen("bus.txt", "r");
    char id[10], busname[50], routeId[10];
    printf("Buses for route '%s':\n", rname);
    while (fscanf(bf, "%s %s %s", id, busname, routeId) == 3)
    {
        if (strcmp(routeId, rid) == 0)
            printf("ID: %s, Name: %s\n", id, busname);
    }
    fclose(bf);
}

// Filter Buses by Station Name
void filterBusByStation()
{
    char station[50], rid[10], name[50], stops[200];
    printf("Enter Station Name: ");
    fgets(station, sizeof(station), stdin);
    clearNewline(station);

    FILE *rf = fopen("route.txt", "r");
    char matchedRoutes[100][10];
    int count = 0;

    while (fscanf(rf, "%s %s %[^\n]", rid, name, stops) == 3)
    {
        if (strstr(stops, station))
        {
            strcpy(matchedRoutes[count++], rid);
        }
    }
    fclose(rf);

    if (count == 0)
    {
        printf("No routes found with this station.\n");
        return;
    }

    FILE *bf = fopen("bus.txt", "r");
    char id[10], busname[50], routeId[10];
    printf("Buses passing through '%s':\n", station);
    while (fscanf(bf, "%s %s %s", id, busname, routeId) == 3)
    {
        for (int i = 0; i < count; i++)
        {
            if (strcmp(routeId, matchedRoutes[i]) == 0)
            {
                printf("ID: %s, Name: %s\n", id, busname);
            }
        }
    }
    fclose(bf);
}

// Menu
int main()
{
    int ch;
    while (1)
    {
        clearScreen();
        printf("\n--- Bus Route Management ---\n");
        printf("1. Add Bus\n2. Update Bus\n3. Delete Bus\n");
        printf("4. Add Route\n5. Update Route\n6. Delete Route\n");
        printf("7. View All Buses\n8. View All Routes\n");
        printf("9. Filter Bus by Route Name\n10. Filter Bus by Station Name\n");
        printf("0. Exit\nChoice: ");
        scanf("%d", &ch);
        getchar(); // clear newline

        clearScreen();
        switch (ch)
        {
        case 1:
            addBus();
            break;
        case 2:
            updateBus();
            break;
        case 3:
            deleteBus();
            break;
        case 4:
            addRoute();
            break;
        case 5:
            updateRoute();
            break;
        case 6:
            deleteRoute();
            break;
        case 7:
            viewAllBuses();
            break;
        case 8:
            viewAllRoutes();
            break;
        case 9:
            filterBusByRouteName();
            break;
        case 10:
            filterBusByStation();
            break;
        case 0:
            exit(0);
        default:
            printf("Invalid.\n");
        }
        waitForUser();
    }
    return 0;
}
