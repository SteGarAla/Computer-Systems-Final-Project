// Name: Steven Garcia-Alamilla
// Date: 04/27/24
// Class/Assignment: Final Project

#include "csapp.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


typedef struct {
    int connfd;
    char hostname[MAXLINE];
    char port[MAXLINE];
} thread_args;

// Monty Hall game logic (Watching YT video helps with understanding function)
int monty_hall(int switch_doors) {
    int doors[3] = {0, 0, 1};  // Initialize doors (0 for goat, 1 for car) (not random right now but will be randomized)
    int contestant_choice = rand() % 3; // assign a door for the user
    int revealed_door, switch_choice;

    // Shuffling items behind door
    // essentially we are just going through each door and assigning it 
    for (int i = 0; i < 3; i++) {
        int swap_idx = rand() % 3;
        int temp = doors[i];
        doors[i] = doors[swap_idx];
        doors[swap_idx] = temp;
    }

    // Revealing a door.
    do {
        revealed_door = rand() % 3;
    // while the door is CONTESTANT DOOR OR DOOR WITH PRIZE, keep randomizing. The whole point is to reveal a door that is not users or prize...
    } while (doors[revealed_door] == 1 || revealed_door == contestant_choice);

    // If contestant decides to switch, we need to find the door that is neither users current door or door that was previously removed
    if (switch_doors) {
        for (int i = 0; i < 3; i++) {
            if (i != contestant_choice && i != revealed_door) {
                switch_choice = i;
                break;
            }
        }
        // users door is updated
        contestant_choice = switch_choice; // Perform the switch
    }
    // now we just return if their choice led them to winninng or losing ;(
    return doors[contestant_choice] == 1;
}

// Simulate multiple games function
// I failed to use n amount of threads to run n amount of simulations. Big numbers would crash the program.
void simulate(int num_games, int switch_doors, char* result) {
    int wins = 0;
    for (int i = 0; i < num_games; i++) {
        // for each win we will increment
        wins += monty_hall(switch_doors);
    }
    // get the win percentage
    float winPercent = ((float)wins / num_games) * 100.0;
    // only going to show 2 decimal places because you would need to run a HUGE amount of games to get MORE THAN 3 decimal places of 6's
    sprintf(result, "Out of %d games, you won %d times. \n Your win percentage is: %.2f%%!!!\n", num_games, wins, winPercent);
}

// I was able to find a way to use threads to my advantage, even though my inital idea of using n threads to run n games failed, i think threads function is pretty efficicent.
// I also didnt need to create a serverfunction but rather just ended up adding what would of been usesd there inside of here.
void *thread(void *vargp) {
    thread_args *args = (thread_args*) vargp;
    // make sure we are connected to the correct client
    int connfd = args->connfd;
    printf("Connected to (%s, %s)\n", args->hostname, args->port);

    // running in detached mode
    pthread_detach(pthread_self());
    char buffer[MAXLINE], results[MAXLINE];

    while (1) {
        
        memset(buffer, 0, MAXLINE);
        if (read(connfd, buffer, MAXLINE) == 0) {
            printf("(%s, %s) disconnected\n", args->hostname, args->port);
            close(connfd);
            free(args);
            return NULL;
        }
        // users choice was to play
        if (buffer[0] == 'P') {
            int num_games, switch_doors;
            sscanf(buffer + 2, "%d,%d", &num_games, &switch_doors);
            simulate(num_games, switch_doors, results);
            write(connfd, results, strlen(results));
            // users choice is to learn more, we will send this to the client side.
            // the text was taken from Wolfram MathWorld.
        } else if (buffer[0] == 'L') {
            strcpy(results, "The Monty Hall problem is named for its similarity to the Let's Make a Deal television game show hosted by Monty Hall. The problem is stated as follows. Assume that a room is equipped with three doors. Behind two are goats, and behind the third is a shiny new car. You are asked to pick a door, and will win whatever is behind it. Let's say you pick door 1. Before the door is opened, however, someone who knows what's behind the doors (Monty Hall) opens one of the other two doors, revealing a goat, and asks you if you wish to change your selection to the third door (i.e., the door which neither you picked nor he opened). The Monty Hall problem is deciding whether you do. \n \n The correct answer is that you do want to switch. If you do not switch, you have the expected 1/3 chance of winning the car, since no matter whether you initially picked the correct door, Monty will show you a door with a goat. But after Monty has eliminated one of the doors for you, you obviously do not improve your chances of winning to better than 1/3 by sticking with your original choice. If you now switch doors, however, there is a 2/3 chance you will win the car (counterintuitive though it seems) be (Monty Hall Paradox -- From Wolfram MathWorld) .\n");
            write(connfd, results, strlen(results));
        // user wishes to exit the program.
        } else if (buffer[0] == 'E') {
            printf("(%s, %s) disconnected\n", args->hostname, args->port);
            close(connfd);
            free(args);
            return NULL;
        }
    }
}

int main(int argc, char **argv) {
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
    pthread_t tid;

    if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(0);
    }

    listenfd = Open_listenfd(argv[1]);
    while (1) {
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

        // unique value for every thread
        thread_args *args = malloc(sizeof(thread_args));
        args->connfd = connfd;
        Getnameinfo((SA *)&clientaddr, clientlen, args->hostname, MAXLINE, args->port, MAXLINE, 0);
        // creates the threads
        pthread_create(&tid, NULL, thread, args);
    }
}
