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

char str[10];
char x[10];
char y[10];

int* thread()
{
    int i;
    
    while(1)
    {
        system("clear");
        for(i=0;i<10;i++)
        {
            printf("\033[%d;%dH%c\n",x[i],y[i],str[i]);
            x[i]++;
        }
        sleep(1);
    }
      
    return NULL;
}

int main()
{
    pthread_t thid;
    
    int i;
    char c='\0';

    system("stty -icanon");
    system("stty -echo");

    srand((unsigned)time(NULL));

    for(i=0;i<10;i++)
    {
        str[i] = 'a' + rand()%26;
        x[i] = 2;
        y[i] = i*3+3;
    }

    if( pthread_create(&thid,NULL,(void*)thread,(void*)&i)
       != 0 )
    {
        printf("Error!\n");
        exit(1);
    }
    while(1)
    {
        c = getchar();
        if(c!='\0')
        {
            for(i=0;i<10;i++)
            {
                if(c == str[i])
                {
                    str[i] = 'a'+rand()%26;
                    x[i] = 2;
                    c='\0';
                    break;
                }
            }
        }
        for(i=0;i<10;i++)
        {
            if(x[i]>=20)
            {
                break;
            }
        }
    }
    system("stty icanonon");
    system("stty echo");

    return 0;
}
