/*************************************************************************
	> File Name: raise.c
	> Author: 师毅
	> Mail:  shiyi19960604@gmail.com
	> Created Time: 2015年07月30日 星期四 09时25分01秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>

void handler(int sigid)
{
    printf("asassaasas: %d\n",sigid);
}

int main()
{
    signal(SIGINT,handler);

    raise(SIGINT);
    return 0;
}

