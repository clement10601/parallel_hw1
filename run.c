#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1000
#define MEGEXTRA 1000000

double randfrom(double min, double max) 
{
    double range = (max - min); 
    double div = RAND_MAX / range;
    return min + (rand() / div);
}

void *tosse(void *threadid)
{
   double x,y;
   //x = randfrom(-1.0,1.0);
   //x = randfrom(-1.0,1.0);
   long tid;
   tid = (long)threadid;
   printf("Hello World! It's me, thread #%ld!\n", tid);
   pthread_exit(NULL);
}

int main( int argc, char *argv[] )  
{
	long tossesAmount = 1;
    int rc = 0;
    long t = 0;
    printf("%d",tossesAmount);
   	if( argc == 2 )
   	{
        printf("%d",tossesAmount);
      	tossesAmount = (long)argv[1];
        printf("%d",tossesAmount);
        pthread_t threads[tossesAmount];

      	for(t=0; t<tossesAmount; t++)
		{
      		rc = pthread_create(&threads[t], NULL, tosse, (void *)t);
      		if (rc)
			{
         		printf("ERROR; return code from pthread_create() is %d\n", rc);
         		exit(-1);
      		}
		}
        pthread_exit(NULL);
   	}
   	else if( argc > 2 )
   	{
      	printf("Too many arguments.\n");
   	}
   	else
   	{
      	printf("One argument expected.\n");
   	}
}
