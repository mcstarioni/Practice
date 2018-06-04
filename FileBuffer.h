//
// Created by mcstarioni on 03.06.2018.
//
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef PRACTICE_FILEBUFFER_H
#define PRACTICE_FILEBUFFER_H
#define ACTION_REMOVE 0
#define ACTION_ASCEND 1
typedef struct FileBufferStructure FileBuff;
typedef struct FileDequeue FileDeq;
typedef struct DequeueNode DeqNode;
typedef struct Dequeue Deq;

struct FileBufferStructure
{
    char *path;
    FILE *filePtr;
    pthread_mutex_t lock;
};
struct FileDequeue
{
    FileBuff **buff;
    Deq *dequeue;
    int sizeOfBuff,numberOfFiles,isFull;
    pthread_mutex_t lock;
};
struct DequeueNode
{
    FileBuff *buff;
    DeqNode *prev;
    DeqNode *next;
};
struct Dequeue
{
    DeqNode *head;
    DeqNode *tail;
};
FileBuff* initializeFileBuff(char *path)
{
    printf("creation 2 0\n");
    FileBuff *buff = (FileBuff*)malloc(sizeof(FileBuff));
    printf("creation 2 1\n");
    buff->path = (char*)malloc(sizeof(path));
    printf("creation 2 2\n");
    strcpy(buff->path,path);
    buff->filePtr = fopen(path,"a+");
    pthread_mutex_init(&buff->lock,NULL);
    return buff;
}
DeqNode *initializeDeqNode(DeqNode *prev,DeqNode *next, FileBuff *buff)
{
    DeqNode* deqNode = (DeqNode*)malloc(sizeof(DeqNode));
    deqNode->prev = prev;
    deqNode->next = next;
    deqNode->buff = buff;
    return deqNode;
}

Deq *initializeDequeue()
{
    Deq *dequeue = (Deq*)malloc(sizeof(Deq));
    dequeue->head = initializeDeqNode(NULL,NULL,NULL);
    dequeue->tail = initializeDeqNode(NULL,NULL,NULL);
    dequeue->head->next = dequeue->tail;
    dequeue->tail->prev = dequeue->head;
    return dequeue;
}
void addNode(Deq *deq, FileBuff *buff)
{
    DeqNode* temp = initializeDeqNode(deq->tail->prev,deq->tail,buff);
    deq->tail->prev->next = temp;
    deq->tail->prev = temp;
}
FileBuff* pop(Deq* dequeue)
{
    DeqNode *temp = dequeue->head->next;
    temp->next->prev = dequeue->head;
    dequeue->head->next = temp->next;
    return temp->buff;
}
void printDequeue(Deq* dequeue)
{
    DeqNode* temp = dequeue->head;
    int i = 0;
    while(temp->next != dequeue->tail)
    {
        temp = temp->next;
        printf("%d. %s\n",i,temp->buff->path);
        i++;
    }
}
FileDeq *initializeFileDequeue(int sizeOfBuff)
{
    FileDeq * fileDeq = (FileDeq*)malloc(sizeof(FileDeq));
    fileDeq->isFull = 0;
    fileDeq->sizeOfBuff = sizeOfBuff;
    fileDeq->numberOfFiles = 0;
    fileDeq->buff = (FileBuff**)malloc(sizeof(FileBuff*)*sizeOfBuff);
    pthread_mutex_init(&fileDeq->lock,NULL);
    return fileDeq;
}
void ascendNode(Deq* dequeue, DeqNode *temp)
{
    temp->prev->next = temp->next;
    temp->next->prev = temp->prev;
    addNode(dequeue,temp->buff);
}
void removeNode(DeqNode *temp)
{
    temp->next->prev = temp->prev;
    temp->prev->next = temp->next;
    free(temp);
}
FileBuff *actionByPath(Deq *dequeue,char *path,int action)
{
    DeqNode *temp = dequeue->head;
    while(temp->next != dequeue->tail)
    {
        temp = temp->next;
        if(strcmp(temp->buff->path,path) == 0)
        {
            (action)?ascendNode(dequeue,temp):removeNode(temp);
            return temp->buff;
        }
    }
    return NULL;
}

void freeFileBuff(FileDeq *fileDeq, char *path)
{
    printf("destruction");
    pthread_mutex_lock(&fileDeq->lock);
    FileBuff *buff = actionByPath(fileDeq->dequeue,path,ACTION_REMOVE);
    fclose(buff->filePtr);
    pthread_mutex_destroy(&buff->lock);
    free(buff->path);
    fileDeq->numberOfFiles--;
    pthread_mutex_unlock(&fileDeq->lock);
}
void doFileBuff(FileDeq *fileDeq,char *path, void (*functionPtr)(FileBuff *,void*),void *arg)
{
    printf("doFileBuff: 1\n");
    FileBuff *buff;
    pthread_mutex_lock(&fileDeq->lock);
    printf("doFileBuff: 2\n");
    if(fileDeq->numberOfFiles == fileDeq->sizeOfBuff)
    {
        buff = actionByPath(fileDeq->dequeue, path,ACTION_ASCEND);
        if (buff != NULL) {
            pthread_mutex_lock(&buff->lock);
            pthread_mutex_unlock(&fileDeq->lock);
            (*functionPtr)(buff, arg);
            pthread_mutex_unlock(&buff->lock);
        } else {
            buff = pop(fileDeq->dequeue);
            pthread_mutex_lock(&buff->lock);
            freeFileBuff(fileDeq,path);
            pthread_mutex_unlock(&buff->lock);
            free(buff);
            buff = initializeFileBuff(path);
            pthread_mutex_lock(&buff->lock);
            addNode(fileDeq->dequeue,buff);
            pthread_mutex_unlock(&fileDeq->lock);
            (*functionPtr)(buff, arg);
            pthread_mutex_unlock(&buff->lock);
        }
    }
    else
    {
        printf("creation 1\n");
        pthread_mutex_lock(&fileDeq->lock);
        buff = initializeFileBuff(path);
        printf("creation 3\n");
        addNode(fileDeq->dequeue,buff);
        (*functionPtr)(buff, arg);
        fileDeq->numberOfFiles++;
        pthread_mutex_unlock(&fileDeq->lock);
    }

}
#endif //PRACTICE_FILEBUFFER_H
