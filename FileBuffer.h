//
// Created by mcstarioni on 03.06.2018.
//
#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifndef PRACTICE_FILEBUFFER_H
#define PRACTICE_FILEBUFFER_H
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
    FileBuff *buff = (FileBuff*)malloc(sizeof(FileBuff));
    buff->path = (char*)malloc(sizeof(path));
    strcpy(buff->path,path);
    buff->filePtr = NULL;
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
void toTop(Deq* dequeue, DeqNode *temp)
{
    temp->prev->next = temp->next;
    temp->next->prev = temp->prev;
    addNode(dequeue,temp->buff);
}
FileBuff *getByPath(Deq *dequeue,char *path)
{
    DeqNode *temp = dequeue->head;
    while(temp->next != dequeue->tail)
    {
        temp = temp->next;
        if(strcmp(temp->buff->path,path) == 0)
        {
            return temp->buff;
        }
    }
    return NULL;
}

void freeFileBuff(FileBuff  *buff)
{
    buff->path = "";
    fclose(buff->filePtr);
}
void allocFileBuff(FileBuff *buff, char *path)
{
    buff->filePtr = fopen(path,"a+");
}
void doFileBuff(FileDeq *fileDeq,char *path, void (*functionPtr)(FileBuff *,void*),void *arg)
{
    FileBuff *result;
    pthread_mutex_lock(&fileDeq->lock);
    FileBuff *temp = getByPath(fileDeq->dequeue,path);
    if(temp != NULL)
    {
        pthread_mutex_lock(&temp->lock);
        pthread_mutex_unlock(&fileDeq->lock);
        (*functionPtr)(temp,arg);
        pthread_mutex_unlock(&temp->lock);
    }
    else
    {
        FileBuff *lastBuff = pop(fileDeq->dequeue);
        pthread_mutex_lock(&lastBuff->lock);
        pthread_mutex_unlock(&fileDeq->lock);
        freeFileBuff(lastBuff);
        allocFileBuff(lastBuff,path);
        (*functionPtr)(lastBuff,arg);
        pthread_mutex_unlock(&lastBuff->lock);
    }
    //TODO: FileBuff creation

}
void writeToFile(FileBuff *fileBuff,void *arg)
{
    fprintf(fileBuff->filePtr,"New text %s\n",(char*)arg);
}
#endif //PRACTICE_FILEBUFFER_H
