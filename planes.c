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
#include <sys/types.h>
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
	printf("Wait2..\n");
	planes = malloc(MAX_PLANES * sizeof(P));;

	runways[0] = RUNWAY_AVAILABLE;
	runways[1] = RUNWAY_AVAILABLE;
	runways[2] = RUNWAY_AVAILABLE;
	

	/* Initializes all of the mutexes we'll be using. */
	if (pthread_mutex_init(&mlock1,NULL) !=0){
		printf("\n Failed to create mlock1.");
		return -1;
	}

	if (pthread_mutex_init(&mlock2,NULL) !=0){
		printf("\n Failed to create mlock2.");
		return -1;
	}

	if (pthread_mutex_init(&mlock3,NULL) !=0){
		printf("\n Failed to create mlock3.");
		return -1;
	}
	
	/*Finish initializing*/

	//Ensures that an amount of planes is entered. And only that.
	if (argc != 2){
		printf("Please enter an amount of planes and only that..\n");
		return -1;
	}

	//Sets numPlanes to number entered.
	numPlanes = atoi(argv[1]);

	//Makes sure the user isn't trying to be a smartass.
	if (numPlanes <= 0){
		printf("You need to enter one or more planes.\n");
		return -1;
	}
	
	/*Creates and allocates arrays we will be using*/
	pthread_t *listofThreads;
	listofThreads = (pthread_t *)malloc((numPlanes+2)*sizeof(*listofThreads));
	pthread_attr_init(&pthread_custom_attr);
	planes = (P *)malloc((numPlanes + 2) * sizeof(P));
	

	int i;
	for ( i = 0; i < numPlanes; i++){
		planes[i].id = i;
		planes[i].status = INCOMING;
		planes[i].fuel = 1000;//Unused. Changed quickly, only set to avoid NULL.

		//Creates thread to fly the planes
		pthread_create(&listofThreads[i], &pthread_custom_attr, flyingPlane, (void *)(planes + i));
	}

	//Creates a thread to decrement fuel
	pthread_create(&listofThreads[i+1], NULL, lowerFuel, (void *)(void *)(planes+numPlanes));


	/*Syncing all of the threads and planes..*/
	void *status;
	for (i=0; i<numPlanes; i++){
	
		pthread_join(listofThreads[i],&status);
	}
	pthread_join(listofThreads[numPlanes+1], &status);

	//DESTROY DESTROY DESTROYYY
	free(planes);	

	pthread_mutex_destroy(&mlock1);
	pthread_mutex_destroy(&mlock2);
	pthread_mutex_destroy(&mlock3);

	return 1;
}

void *flyingPlane(void *aplane){
	
	int chooseRunway, current, i, j, lowFuel, l, m;
	//Creates a plane variable
	P *plane = (P *) aplane;
	srand(time(NULL)+(plane->id));
	int arrivalTime = rand()%10 + 3;
	plane->status = INCOMING;

	//Sets random fuel
	plane->fuel = rand()%100 + 10; 
	printf("The plane ID: %d arrived at the time: %d. The level of fuel is: %d\n", plane->id, (int)arrivalTime, plane->fuel);


	plane->status = WAITING;
	sleep(3);//Waits a fixed amount of time

	printf(" Plane: %d wants to land and has %d fuel left..\n", plane->id, plane->fuel);


	//Chance of equipment failing. Need better engineers..
	if(rand()%5 == 0){
		pthread_mutex_lock(&mlock1);
		pthread_mutex_lock(&mlock2);
		pthread_mutex_lock(&mlock3);
		//Gets set. Will be used later.
		emergency = 1;
		chooseRunway = 0;
		//Always uses runway 0 in emergencies
		runways[0] = plane->id;

		plane->status = ELANDING;

		printf("Plane: %d has equipment failure and is trying to land.\n", plane->id);
		sleep(rand()%3 + 1);


	}


	else{

		chooseRunway = -1;
		while( chooseRunway == -1){
			lowFuel = 0;
			l = 0;

			//Actively checks for fuel and lowfuel.
			for( i = 0; i< numPlanes; i++){
				if( (  (planes[i].fuel) <= plane->fuel) && (planes[i].status == WAITING) ){
					lowFuel = 1;
				} 
			}

			//Low fuel planes go next.
			if (lowFuel == 1){
				for (l = 0; l<SIZE; l++){
					if(runways[l] == RUNWAY_AVAILABLE){
						chooseRunway = l;
						break;
					}
				}
			}
		}
		
			/*Locking mutexes depending on runway*/
			if(chooseRunway == 0){
				pthread_mutex_lock(&mlock1);
			}

			if(chooseRunway == 1){
				pthread_mutex_lock(&mlock2);
			}
	
			if(chooseRunway == 2){
				pthread_mutex_lock(&mlock3);
			}				
			/*End locking*/



		plane->status = LANDING;

		printf("Plane: %d is trying to land.\n\n", plane->id);
		runways[chooseRunway] = plane->id;
		sleep(rand()%5 + 2);

	}

	printf("Plane: %d just parked.\n\n", plane->id);
	plane->status = LANDED;

	runways[chooseRunway] = RUNWAY_AVAILABLE;

	/*Unlocking of mutexes*/
	if(chooseRunway == 0) {
		pthread_mutex_unlock(&mlock1);
		if(emergency == 1){
			emergency = 0;
			pthread_mutex_unlock(&mlock2);
			pthread_mutex_unlock(&mlock3);
		}
	}
	
	if(chooseRunway == 1){
		pthread_mutex_unlock(&mlock2);
	}
	
	if(chooseRunway == 2){
		pthread_mutex_unlock(&mlock3);
	}
	/*End unlocking*/

	pthread_exit(NULL);
																																																																
}

void *lowerFuel(void *aplane){

	int arrivalTime, k, parked, chooseRunway, lowest, l, m;

	int current = 0;
	while(1){
		parked = 0;
		current++;
		current = current % numPlanes; //Allows random planes to be the "current".
		planes[current].id = current;
		for( l = 0; l < numPlanes; l++){
			if(planes[l].status!= LANDED){
				parked = 1;//Means not all planes have landed
				break;
			}
		}

		if(parked != 1){ //All planes landed.
			printf("Every plane landed!\n\n");
			exit(0);
		}

		//Slows down the decrement, otherwise it'll decrement really quickly.
		sleep(2);
		if(planes[current].status!=LANDED){
			planes[current].fuel = planes[current].fuel - 1;
			printf("\nPlane %d fuel is: %d\n",planes[current].id, planes[current].fuel);
			if(planes[current].fuel < 0){
				printf("Good job, idiot. Plane %d just crashed. The program will now end.\n", current);
				exit(0);
			}

		}

	}

	pthread_exit(NULL);
}


