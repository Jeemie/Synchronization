#include <pthread.h>
#include <semaphore.h>

#ifndef PLANE_H
#define PLANE_H
#define MAX_RUNWAY 3 
#define RUNWAY_UNAVAILABLE -1
#define HAS_PARKED 10

#define MAX_PLANES 25
typedef struct plane{
	int fuel;
	int status;
	int id;
}P;


int runways[3]; //3 runways that the problem specifies
int SIZE = (sizeof(runways)/sizeof(int));
sem_t lock1;
sem_t lock2;
sem_t lock3; 

pthread_mutex_t mlock1;
pthread_mutex_t mlock2;
pthread_mutex_t mlock3;
int numPlanes;
P *planes;
int emergency = 0;
void *lowerFuel(void *aplane);
      	  
sem_t full;     	  /* keeps track of full spots */
sem_t empty;    	  /* keeps track of empty spots */
sem_t mutex;  

void *flyingPlane(void *aplane);
void *lowerFuel(void *aplane);
//P makeplane();
//void makingplanes(P *planes);
//void meltingsteelbeams(P **planes);


#endif
