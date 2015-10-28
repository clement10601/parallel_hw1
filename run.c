#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS     8

#define N 1000
#define MEGEXTRA 1000000

long counter = 0;
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
   x = randfrom(-1.0,1.0);
   y = randfrom(-1.0,1.0);
   struct pitem *pi;
   pi=(struct pitem*)pitems;
   long tid=0;
   tid = pi->thread;

   pthread_mutex_lock( &mutex1 );
   counter++;
   printf("Counter value: %d\n",counter);
   pthread_mutex_unlock( &mutex1 );

   printf("Hello World! It's me, thread #%ld!\n", tid);
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
