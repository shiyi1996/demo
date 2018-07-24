/*************************************************************************
	> File Name: wait.c
	> Author: 师毅
	> Mail:  shiyi19960604@gmail.com
	> Created Time: 2015年07月27日 星期一 18时17分07秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

int i=0;
pthread_mutex_t mutex;
pthread_cond_t cond;

void* thread_b(void *arg)
{
    while(i<20)
    {
        pthread_mutex_lock(&mutex);
        if(i%2==0)
        {
            pthread_cond_signal(&cond);
        }
        else
            printf("b 奇数 %d\n",i);
        pthread_mutex_unlock(&mutex);
        sleep(1);
        i++;
    }
}

void* thread_a(void *arg)
{
    while(i<20)
    {
        pthread_mutex_lock(&mutex);
        if(i%2)
        {
            pthread_cond_wait(&cond,&mutex);
        }
        printf("a  偶数 %d\n",i);
        pthread_mutex_unlock(&mutex);
        sleep(1);
    }
}

int main()
{
    pthread_t thid;
    pthread_create(&thid,NULL,(void*)thread_a,NULL);
    pthread_create(&thid,NULL,(void*)thread_b,NULL);
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&cond,NULL);
    
    sleep(50);
    return 0;
}

