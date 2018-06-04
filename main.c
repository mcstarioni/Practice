#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <zconf.h>
#include "FileBuffer.h"

#define NUM_THREADS	4
#define SIZE_OF_BUFF 5
FileDeq *fileDeq;
void writeToFile(FileBuff *buff,void *args)
{
    printf("writing\n");
    fprintf(buff->filePtr,"/Hello world! %s\n",(char*)args);
}
void *threadFunction(void *args)
{
    printf("thread start\n");
    doFileBuff(fileDeq,"1.txt",&writeToFile,"Thread one");
    freeFileBuff(fileDeq,"1.txt");
    int res = 20;
    pthread_exit(&res);
}

int main (int argc, char *argv[])
{
    FileDeq *deq = initializeFileDequeue(SIZE_OF_BUFF);

    pthread_t thread[NUM_THREADS];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_create(&thread[1], &attr, threadFunction, NULL);
    pthread_attr_destroy(&attr);
    printf("main\n");
    void **a;
    int res = pthread_join(thread[1], a);
    printf("main\n");
    pthread_exit(NULL);
}
//    Deq* dequeue = initializeDequeue();
//    for (int i = 0; i < 10; ++i)
//    {
//        char str[10];
//        sprintf(str, "%d", rand());
//        FileBuff* buff = initializeFileBuff(str);
//        addNode(dequeue,buff);
//    }
//    printDequeue(dequeue);
//    pop(dequeue);
//    pop(dequeue);
//    pop(dequeue);
//    printDequeue(dequeue);
//    long timeOne = (unsigned long)time(NULL);
//    sleep(2);
//    long timeTwo = (unsigned long)time(NULL);
//    printf("Has past %f",difftime(timeOne,timeTwo
//    ));
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