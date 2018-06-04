#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <zconf.h>
#include "FileBuffer.h"

#define NUM_THREADS	4

void *BusyWork(void *t)
{
    srand((unsigned int)time(NULL));
    int i;
    long tid;
    double *result=malloc(sizeof(double));
    tid = (long)t;
    printf("Thread %ld starting...\n",tid);
    for (i=0; i<rand(); i++)
    {
        *result = *result + sin(i) * tan(i);
    }
    printf("Thread %ld done. Result = %e\n",tid,result);
    pthread_exit((void*)result);
}
void *threadFunction()
{
    //init
    //decide to read
}
int main (int argc, char *argv[])
{
    Deq* dequeue = initializeDequeue();
    for (int i = 0; i < 10; ++i)
    {
        char str[10];
        sprintf(str, "%d", rand());
        FileBuff* buff = initializeFileBuff(str);
        addNode(dequeue,buff);
    }
    printDequeue(dequeue);
    pop(dequeue);
    pop(dequeue);
    pop(dequeue);
    printDequeue(dequeue);
//    long timeOne = (unsigned long)time(NULL);
//    sleep(2);
//    long timeTwo = (unsigned long)time(NULL);
//    printf("Has past %f",difftime(timeOne,timeTwo
//    ));

}
//
//    pthread_t thread[NUM_THREADS];
//    pthread_attr_t attr;
//    int rc;
//    long t;
//    void *status;
//    /* Initialize and set thread detached attribute */
//    pthread_attr_init(&attr);
//    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
//
//    for(t=0; t<NUM_THREADS; t++) {
//        printf("Main: creating thread %ld\n", t);
//        rc = pthread_create(&thread[t], &attr, BusyWork, (void *)t);
//        if (rc) {
//            printf("ERROR; return code from pthread_create() is %d\n", rc);
//            exit(-1);
//        }
//    }
//
//    /* Free attribute and wait for the other threads */
//    pthread_attr_destroy(&attr);
//    for(t=0; t<NUM_THREADS; t++) {
//        rc = pthread_join(thread[t], &status);
//        if (rc) {
//            printf("ERROR; return code from pthread_join() is %d\n", rc);
//            exit(-1);
//        }
//        printf("Main: completed join with thread %ld having a status of %e\n",t,*((double *)status));
//    }
//
//    printf("Main: program completed. Exiting.\n");
//    pthread_exit(NULL);
//}