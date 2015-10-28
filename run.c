#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1000
#define MEGEXTRA 1000000

pthread_attr_t attr;

void *tosse(void *threadid)
{
   long tid;
   tid = (long)threadid;
   printf("Hello World! It's me, thread #%ld!\n", tid);
   pthread_exit(NULL);
}

int main( int argc, char *argv[] )  
{
	long tossesAmount = 2;
    int rc = 0;
    long t = 0;
   	if( argc == 2 )
   	{
        char * e;
        tossesAmount = strtol(argv[1],&e,0);
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
