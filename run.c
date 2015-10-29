#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/sysinfo.h>

int NUM_THREADS =  4;

clock_t begin, end;
double time_spent;

long counter = 0;
long number_incircle=0;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

struct pitem
{
    long tosses;
    long thread;
    long tosses_per_thread;
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
    long tosses_per_thread=0;
    double pi_estimate=0;

    pi=(struct pitem*)pitems;
    tid = pi->thread;
    tosses = pi->tosses;
    tosses_per_thread = pi->tosses_per_thread;
    while(tosses_per_thread>0)
    {
        x = randfrom(-1.0,1.0);
        y = randfrom(-1.0,1.0);
        distance_squared = x * x + y * y;
        if(distance_squared<=1)
        {
            pthread_mutex_lock( &mutex1 );
            number_incircle++;
            pthread_mutex_unlock( &mutex1 ); 
        }
        tosses_per_thread--;
    }
    //printf("Job:#%d Done! Thread No.%ld!\n", counter,tid);
    //printf("distance_squared: %f\n",distance_squared);
    free(pi);
    pthread_exit(NULL);
}

int main( int argc, char *argv[] )  
{
    begin = clock();
    NUM_THREADS = get_nprocs();
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
        int l =NUM_THREADS;
        long tosses_per_thread=0;
        int exp = 0;
        double x,y;
        double distance_squared;

        tosses_per_thread = tosses/NUM_THREADS;
        exp =tosses%NUM_THREADS;

            for(t=0; t<l; t++)
            {
                ptr_pitem = (struct pitem *)malloc(sizeof(struct pitem));
                pitem.thread = t;
                ptr_pitem->thread = t;
                ptr_pitem->tosses = tosses;
                ptr_pitem->tosses_per_thread = tosses_per_thread;

                rc = pthread_create(&threads[t], NULL, tosse, ptr_pitem);
                if (rc)
                {
                    //printf("ERROR; return code from pthread_create() is %d\n", rc);
                    exit(-1);
                }
            }
            for(t=0; t<l; t++)
            {
                rc = pthread_join(threads[t], &status);
                if (rc)
                {
                    //printf("ERROR; return code from pthread_join() is %d\n", rc);
                    exit(-1);
                }
                //printf("Main: completed join with thread %ld having a status of %ld\n",t,(long)status);
            }
        while(exp>0)
        {
            x = randfrom(-1.0,1.0);
            y = randfrom(-1.0,1.0);
            distance_squared = x * x + y * y;
            if(distance_squared<=1)
            {
                number_incircle++;
            }
            exp--;
        }
                
        pi_estimate = 4*(((double)number_incircle)/((double)tosses));
        //printf("Number of tosses: %u\n",tosses);
        //printf("Number in circle: %u\n",number_incircle);
        //printf("Pi Estimate: %f\n",pi_estimate);
        printf("%.15f\n",pi_estimate);
        end = clock();
        time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
        //printf("Time Elapsed: %fs\n",time_spent);
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
