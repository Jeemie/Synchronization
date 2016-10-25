/*
	Code By:
		Jimmy Tran
		Connor Smith
		Matthew Jackman
*/
#include <stdlib.h>
#include <stdio.h>
#include "plane.h"
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>
#define MAX_PLANES 25 
#define MAX_RUNWAY 3 
#define RUNWAY_AVAILABLE -1
#define HAS_PARKED 10
#define INCOMING 0
#define WAITING 1
#define LANDING 2
#define ELANDING 3
#define LANDED 4
#define CRASHED 5




int main(int argc, const char* argv[]){


	pthread_attr_t pthread_custom_attr;


	runways[0] = RUNWAY_AVAILABLE;
	runways[1] = RUNWAY_AVAILABLE;
	runways[2] = RUNWAY_AVAILABLE;


	/* Initializes all of the semaphores we'll be using */
	if (sem_init(&lock1, 0,1) !=0){
		printf("\n Failed to create lock1.");
		return -1;
	}

	if (sem_init(&lock2, 0,1) !=0){
		printf("\n Failed to create lock2.");
		return -1;
	}

	if (sem_init(&lock3, 0,1) !=0){
		printf("\n Failed to create lock3.");
		return -1;
	}
	//Finish initializing all of the semaphores we're using


	//Makes sure that the amount of planes wanted IS put in and nothing else.
	if (argc != 2){
		printf("Please enter an amount of planes and only that..\n");
		return -1;
	}


	//Sets numPlanes to be equal to number entered.
	numPlanes = atoi(argv[1]);

	//Checks to make sure user isn't being a smartass.
	if (numPlanes <= 0){
		printf("You need to enter one or more planes.\n");
		return -1;
	}
	
	pthread_t *listofThreads; //Creates an array of threads 
	listofThreads = (pthread_t *)malloc((numPlanes+2)*sizeof(*listofThreads)); //Allocates them
	pthread_attr_init(&pthread_custom_attr); //Inits the custom attribute
	planes = (P *)malloc((numPlanes + 2) * sizeof(P)); //Allocates an array of planes. Extra 2 spaces to store 2 threads.



	
	int i;
	for ( i = 0; i < numPlanes; i++){
		planes[i].id = i;
		planes[i].status = INCOMING;
		planes[i].fuel = 1000; //Set so there isn't a null value. Not used. Changed immediately.


		//Thread that "flies" planes
		pthread_create(&listofThreads[i], &pthread_custom_attr, flyingPlane, (void *)(planes + i));
	}

	//Another thread that decrements fuel
	pthread_create(&listofThreads[i+1], NULL, lowerFuel, (void *)(void *)(planes+numPlanes)); 


	void *status;

	//Syncronizes all of the threads/planes
	for (i=0; i<numPlanes; i++){
	
		pthread_join(listofThreads[i],&status);
	}
	pthread_join(listofThreads[numPlanes+1], &status);

	free(planes);	

	sem_destroy(&lock1);
	sem_destroy(&lock2);
	sem_destroy(&lock3);

	return 1;
}

void *flyingPlane(void *aplane){
	
	int chooseRunway, current, i, j, lowFuel, l, m;
	P *plane = (P *) aplane;
	srand(time(NULL)+(plane->id));
	int arrivalTime = rand()%10 + 3;
	plane->status = INCOMING;
	plane->fuel = rand()%100 + 10;
	printf("The plane ID: %d arrived at the time: %d. The level of fuel is: %d\n", plane->id, (int)arrivalTime, plane->fuel);


	plane->status = WAITING;
	sleep(3); //Makes it wait a fixed amount of time.

	printf(" Plane: %d wants to land and has %d fuel left..\n", plane->id, plane->fuel);


	//Chance of equipmenting failing. Need better engineers.
	if(rand()%5 == 0){
		sem_wait(&lock1); //It locks all the semaphores so that it can enter on it's own.
		sem_wait(&lock2);
		sem_wait(&lock3);
		emergency = 1; //Sets the value to 1. Used later in the function.

		chooseRunway = 0; //Makes it choose runway 0.
		runways[0] = plane->id;

		plane->status = ELANDING; //EMergency Landing

		printf("Plane: %d has equipment failure and is trying to land.\n", plane->id);
		sleep(rand()%3 + 1);


	}


	else{

		chooseRunway = -1;
		while( chooseRunway == -1){
			lowFuel = 0;
			l = 0;

			for( i = 0; i< numPlanes; i++){
				if( (  (planes[i].fuel) <= plane->fuel) && (planes[i].status == WAITING) ){//Checks for low fuel
					lowFuel = 1;
				} 
			}

			if (lowFuel == 1){ //Makes that plane land next
				for (l = 0; l<SIZE; l++){
					if(runways[l] == RUNWAY_AVAILABLE){
						chooseRunway = l;
						break;
					}
				}
			}
		}
		
				
			if(chooseRunway == 0){
				sem_wait(&lock1);//locks the runway
			}

			if(chooseRunway == 1){
				sem_wait(&lock2); //locks the runway
			}
	
			if(chooseRunway == 2){
				sem_wait(&lock3); //locks the runway
			}				


		plane->status = LANDING;

		printf("Plane: %d is trying to land.\n\n", plane->id);
		runways[chooseRunway] = plane->id; //planes goes into that runway
		sleep(rand()%5 + 2); //landing time

	}

	printf("Plane: %d just parked.\n\n", plane->id);
	plane->status = LANDED;

	runways[chooseRunway] = RUNWAY_AVAILABLE;
	if(chooseRunway == 0) {
		sem_post(&lock1);
		if(emergency == 1){ //If it was an emergency landing, unlock all other semaphores as well.
			emergency = 0;
			printf("Emergency reset..\n\n");
			sem_post(&lock2);
			sem_post(&lock3);
		}
	}
	
	if(chooseRunway == 1){
		sem_post(&lock2); //unlocks
	}
	
	if(chooseRunway == 2){
		sem_post(&lock3); //unlocks
	}

	pthread_exit(NULL);
																																																																
}

void *lowerFuel(void *aplane){

	int arrivalTime, k, parked, chooseRunway, lowest, l, m;

	int current = 0;
	while(1){
		parked = 0;
		current++;
		current = current % numPlanes; //Allows planes to randomly land
		planes[current].id = current;
		for( l = 0; l < numPlanes; l++){
			if(planes[l].status!= LANDED){
				parked = 1; //Means hasn't parked
				break;
			}
		}

		if(parked != 1){ //If all are parked..
			printf("Every plane landed!\n\n");
			exit(0);
		}

		sleep(2); //Slows down the decrement, otherwise it'll decrement really quickly.
		if(planes[current].status!=LANDED){
			planes[current].fuel = planes[current].fuel - 1;
			printf("\nPlane %d fuel is: %d\n",planes[current].id, planes[current].fuel);
			if(planes[current].fuel < 0){
				printf("Good job. Plane %d just crashed.\n", current);
				exit(0);
			}

		}

	}

	pthread_exit(NULL);
}


