#include <stdio.h>
#include <string.h>
#include <stdbool.h>

struct Route
{
    char name[50];
    int distance;
    int duration;
};

struct User
{
    char username[50];
    char password[50];
    char role[50];
};

struct Route routes[100];
struct User users;

void header(char title[])
{
    printf("\e[1;1H\e[2J");
    printf("========================================\n");
    printf("%s\n", title);
    printf("========================================\n");
}

void pressToContinue()
{
    printf("Press ENTER key to continue...");
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
    getchar();
}

void login()
{
    char username[50];
    char password[50];
    char role[50];
    char buffer[1024];
    int found = 0;

    FILE *file = fopen("csv/users.csv", "r");
    if (file == NULL)
    {
        printf("Could not open users.csv\n");
        return;
    }

    do
    {
        header("Login");
        printf("Type your username: ");
        scanf(" %[^\n]", username);
        printf("Type your password: ");
        scanf(" %[^\n]", password);

        rewind(file); // Go back to the start of the file
        while (fgets(buffer, sizeof(buffer), file))
        {
            char username[50], password[50], role[50];

            // Assuming username and password are separated by a comma
            sscanf(buffer, "%[^,],%[^,],%s", username, password, role);

            if (strcmp(username, username) == 0 && strcmp(password, password) == 0)
            {
                found = 1;
                break;
            }
        }

        if (found)
        {
            printf("role: %s\n", role);
            strcpy(users.username, username);
            strcpy(users.password, password);
            strcpy(users.role, role);
            printf("Login success!\n");
            printf("Welcome, %s!\n", users.username);
            pressToContinue();
            break;
        }
        else
        {
            printf("Login failed!\n");
            pressToContinue();
        }
    } while (true);

    fclose(file);
}

int mainMenu(int choice, char role[])
{
    header("Transportation Route Management System");

    printf("Role: %s\n", role);

    if (strcmp(role, "ADMIN") == 0)
    {
        printf("1. Add Route\n");
        printf("2. Search Route\n");
        printf("3. Display Routes\n");
        printf("4. Sort Routes by Distance\n");
        printf("5. Delete Route by Name\n");
        printf("6. Delete All Routes\n");
        printf("7. Exit\n");
    }
    else
    {
        printf("1. Search Route\n");
        printf("2. Display Routes\n");
        printf("3. Sort Routes by Distance\n");
        printf("4. Exit\n");
    }

    printf("Type your choice: ");
    scanf("%d", &choice);

    return choice;
}

void addRoute()
{
    char startRoute[50];
    char endRoute[50];
    char routeName[50];
    char addAgain;
    int distance;
    int duration;
    bool isAgain = false;

    do
    {
        startRoute[0] = '\0';
        endRoute[0] = '\0';
        routeName[0] = '\0';
        header("Add Route");
        printf("Type the start route: ");
        scanf(" %[^\n]", &startRoute);
        printf("Type the end route: ");
        scanf(" %[^\n]", &endRoute);

        strcat(routeName, startRoute);
        strcat(routeName, " - ");
        strcat(routeName, endRoute);

        printf("Type the distance (meter): ");
        scanf("%d", &distance);
        printf("Type the duration (minutes): ");
        scanf("%d", &duration);

        for (int i = 0; i < 100; i++)
        {
            if (strcmp(routes[i].name, routeName) == 0)
            {
                printf("Route already exist!\n\n");
                break;
            }

            if (strcmp(routes[i].name, "") == 0)
            {
                strcpy(routes[i].name, routeName);
                routes[i].distance = distance;
                routes[i].duration = duration;
                printf("Route added!\n");
                break;
            }

            if (i == 99)
            {
                printf("Route is full!\n");
                break;
            }
        }

        printf("Add another route? (Y/N): ");
        scanf("%s", &addAgain);

        if (addAgain == 'Y' || addAgain == 'y')
        {
            isAgain = true;
        }
        else
        {
            isAgain = false;
        }
    } while (isAgain);
}

void deleteRoute()
{
    header("Delete Route");

    char routeName[50];
    char deleteAgain;
    bool isAgain = false;

    do
    {
        printf("Type the route name to delete: ");
        scanf(" %[^\n]", &routeName);

        for (int i = 0; i < 100; i++)
        {
            if (strcmp(routes[i].name, routeName) == 0)
            {
                strcpy(routes[i].name, "");
                routes[i].distance = 0;
                routes[i].duration = 0;
                printf("Route deleted!\n");
                break;
            }

            if (i == 99)
            {
                printf("Route not found!\n");
                break;
            }
        }

        printf("Delete another route? (Y/N): ");
        scanf("%s", &deleteAgain);

        if (deleteAgain == 'Y' || deleteAgain == 'y')
        {
            isAgain = true;
        }
        else
        {
            isAgain = false;
        }
    } while (isAgain);
}

void deleteAllRoutes()
{
    header("Delete All Routes");

    for (int i = 0; i < 100; i++)
    {
        strcpy(routes[i].name, "");
        routes[i].distance = 0;
        routes[i].duration = 0;
    }

    printf("All routes deleted!\n");
    printf("\n");
    pressToContinue();
}

int searchRoute(char keyword[])
{
    int routeFound = 0;

    for (int i = 0; i < 100; i++)
    {
        if (strcmp(routes[i].name, "") != 0)
        {
            if (strstr(routes[i].name, keyword) != NULL)
            {
                printf("Route Name: %s\n", routes[i].name);
                printf("Distance (meter): %d\n", routes[i].distance);
                printf("Duration (minutes): %d\n", routes[i].duration);
                printf("\n");
                routeFound++;
            }
        }
    }

    if (routeFound == 0)
    {
        printf("Route with keyword '%s' not found!\n", keyword);
    }

    return routeFound;
}

void displayRoutes()
{
    header("Display Routes");
    for (int i = 0; i < 100; i++)
    {
        if (strcmp(routes[i].name, "") != 0)
        {
            printf("Route Name: %s\n", routes[i].name);
            printf("Distance (meter): %d\n", routes[i].distance);
            printf("Duration (minutes): %d\n", routes[i].duration);
            printf("\n");
        }
    }

    pressToContinue();
}

void sortRoutes()
{
    struct Route temp;

    header("Sort Routes by Distance");
    char ascDesc;
    printf("Sort by Ascending (A) or Descending (D): ");
    scanf("%s", &ascDesc);

    for (int i = 0; i < 100; i++)
    {
        for (int j = i + 1; j < 100; j++)
        {
            if (strcmp(routes[i].name, "") != 0 && strcmp(routes[j].name, "") != 0)
            {
                if (ascDesc == 'A' || ascDesc == 'a')
                {
                    if (routes[i].distance > routes[j].distance)
                    {
                        temp = routes[i];
                        routes[i] = routes[j];
                        routes[j] = temp;
                    }
                }
                else if (ascDesc == 'D' || ascDesc == 'd')
                {
                    if (routes[i].distance < routes[j].distance)
                    {
                        temp = routes[i];
                        routes[i] = routes[j];
                        routes[j] = temp;
                    }
                }
                else
                {
                    printf("Invalid input!\n");
                }
            }
        }
    }

    printf("Routes sorted!\n");
    pressToContinue();
}

int main()
{
    bool isRunning = true;
    char routeName[50];
    int choice;

    login();

    do
    {
        choice = mainMenu(choice, users.role);

        switch (choice)
        {
        case 1:
            if (strcmp(users.role, "ADMIN") == 0)
            {
                addRoute();
                break;
            }
            else
            {
                header("Search Route");
                printf("Type the route name to search: ");
                scanf(" %[^\n]", &routeName);

                int index = searchRoute(routeName);
                printf("Found %d route(s)!\n", index);

                printf("\n");
                pressToContinue();
            }
            break;
        case 2:
            if (strcmp(users.role, "ADMIN") == 0)
            {
                header("Search Route");
                printf("Type the route name to search: ");
                scanf(" %[^\n]", &routeName);

                int index = searchRoute(routeName);
                printf("Found %d route(s)!\n", index);

                printf("\n");
                pressToContinue();
            }
            else
            {
                displayRoutes();
            }
            break;
        case 3:
            if (strcmp(users.role, "ADMIN") == 0)
            {
                displayRoutes();
            }
            else
            {
                sortRoutes();
            }
            break;
        case 4:
            if (strcmp(users.role, "ADMIN") == 0)
            {
                sortRoutes();
            }
            else
            {
                printf("Exiting program...\nThank you for using the Program!\n");
                pressToContinue();
                isRunning = false;
            }
            break;
        case 5:
            if (strcmp(users.role, "ADMIN") == 0)
            {
                deleteRoute();
            }
            else
            {
                printf("Invalid input!\n");
            }
            break;
        case 6:
            if (strcmp(users.role, "ADMIN") == 0)
            {
                deleteAllRoutes();
            }
            else
            {
                printf("Invalid input!\n");
            }
            break;
        case 7:
            printf("Exiting program...\nThank you for using the Program!\n");
            pressToContinue();
            isRunning = false;
            break;
        }
    } while (isRunning);

    return 0;
}