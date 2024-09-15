// Name: Steven Garcia-Alamilla
// Date: 04/27/24
// Class/Assignment: Final Project

#include "csapp.h"
#include <stdbool.h>

// fucntion that will print doors. last minute add in.
void print_doors() {
    printf("\nChoose a door:\n");
    printf("  ______     ______     ______   \n");
    printf(" |      |   |      |   |      |  \n");
    printf(" |  [1] |   |  [2] |   |  [3] |  \n");
    printf(" |      |   |      |   |      |  \n");
    printf(" |      |   |      |   |      |  \n");
    printf(" |      |   |      |   |      |  \n");
    printf(" |______|   |______|   |______|  \n\n");
}


int main(int argc, char **argv) {
    int clientfd, n;
    char *host, *port, buffer[MAXLINE];

    if (argc != 3) {
        fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
        exit(0);
    }

    host = argv[1];
    port = argv[2];
    clientfd = Open_clientfd(host, port);

    while (true) {
        printf("Enter P to play, L for instructions, E to exit: ");
        fgets(buffer, MAXLINE, stdin);
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character
        char choice = buffer[0];

        // user wants to play
        if (choice == 'P' || choice == 'p') {
            // rather then actually printing it we will just use our helper function
            print_doors();
            printf("Select a door (1, 2, or 3): ");
            fgets(buffer, MAXLINE, stdin);
            int initial_choice = atoi(buffer);

            printf("How many games would you like to simulate? ");
            fgets(buffer, MAXLINE, stdin);
            int num_games = atoi(buffer);

            printf("Do you want to switch doors after one is revealed? Enter 1 for yes, 0 for no: ");
            fgets(buffer, MAXLINE, stdin);
            int switch_doors = atoi(buffer);

            // formats this and will send this over to the server side
            sprintf(buffer, "P,%d,%d", num_games, switch_doors);
            // writing it to server
            Write(clientfd, buffer, strlen(buffer));
        // user wants to learn more
        } else if (choice == 'L' || choice == 'l') {
            sprintf(buffer, "L");
            Write(clientfd, buffer, strlen(buffer));
        // user wants to exit the program
        } else if (choice == 'E' || choice == 'e') {
            sprintf(buffer, "E");
            Write(clientfd, buffer, strlen(buffer));
            break;
        // users choice was none of the ones above
        } else {
            printf("Invalid choice. Please try again.\n");
            continue;
        }

        // Wait for response from the server
        n = Read(clientfd, buffer, MAXLINE);
        buffer[n] = '\0';
        printf("Monty Hall: %s\n", buffer);
    }
        
    Close(clientfd);
    return 0;
}
