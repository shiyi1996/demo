/*************************************************************************
	> File Name: temp.c
	> Author: 师毅
	> Mail:  shiyi19960604@gmail.com
	> Created Time: 2015年07月24日 星期五 14时12分29秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>
#include<time.h>

int* thread(void *a)
{   
    while(1)
    {
        printf("压死！\n");

        sleep(1);
    }
      
    return NULL;
}

int main()
{
    pthread_t thid;

    if( pthread_create(&thid,NULL,(void*)thread,NULL) != 0 )
    {
        printf("Error!\n");
        exit(1);
    }

    while(1)
    {
            printf("大你！\n");
            sleep(1);
	}
    return 0;
}
