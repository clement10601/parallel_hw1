#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS     8

#define N 1000
#define MEGEXTRA 1000000

long counter = 0;
long number_incircle=0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

struct pitem
{
    long tosses;
    long thread;
};

double randfrom(double min, double max) 
{
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

void *tosse(void *pitems)
{
    double x,y;
    double distance_squared;
    struct pitem *pi;
    long tid=0;
    long tosses=0;
    double pi_estimate=0;

    pi=(struct pitem*)pitems;
    tid = pi->thread;
    tosses = pi->tosses;
    x = randfrom(-1.0,1.0);
    y = randfrom(-1.0,1.0);
    distance_squared = x * x + y * y;
    pthread_mutex_lock( &mutex1 );
    counter++;
    printf("Job:#%d Done! Thread No.%ld!\n", counter,tid);
    printf("distance_squared: %f\n",distance_squared);
    if(distance_squared<=1)
    {
        printf("incircle!\n");
        number_incircle++;
    }
    else
    {
        printf("not incircle!\n");
    }
    pthread_mutex_unlock( &mutex1 );
    free(pi);
    pthread_exit(NULL);
}

int main( int argc, char *argv[] )  
{
    int prgstat=0;
	long tosses = 0;
    struct pitem pitem; 
    struct pitem* ptr_pitem;
    int rc = 0;
    long t = 0;
    double pi_estimate=0;

    void *status;

   	if( argc == 2 )
   	{
        char *e;
        tosses = strtol(argv[1],&e,0);
        pthread_t threads[NUM_THREADS];

        pitem.tosses = tosses;
      	for(t=0; t<NUM_THREADS; t++)
		{
            ptr_pitem = (struct pitem *)malloc(sizeof(struct pitem));
            pitem.thread = t;
            ptr_pitem->thread = t;
            ptr_pitem->tosses = tosses;
      		rc = pthread_create(&threads[t], NULL, tosse, ptr_pitem);
      		if (rc)
			{
         		printf("ERROR; return code from pthread_create() is %d\n", rc);
         		exit(-1);
      		}
		}
        for(t=0; t<NUM_THREADS; t++) 
        {
            rc = pthread_join(threads[t], &status);
            if (rc) 
            {
                printf("ERROR; return code from pthread_join() is %d\n", rc);
                exit(-1);
            }
            printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);
        }
        pi_estimate = 4*number_incircle/tosses;
        printf("number of tosses: %u\n",tosses);
        printf("number in circle: %u\n",number_incircle);
        printf("Pi Estimate: %f\n",pi_estimate);
        pthread_exit(NULL);
        prgstat=0;
   	}
   	else if( argc > 2 )
   	{
      	printf("Too many arguments.\n");
        prgstat=1;        
   	}
   	else
   	{
      	printf("One argument expected.\n");
        prgstat=1;
   	}
    return prgstat;
}
