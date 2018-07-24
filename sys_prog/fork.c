/*===============================================================
*   Copyright (C) 2015 All rights reserved.
*   
*   文件名称：fork.c
*   创 建 者：师毅
*   创建日期：2015年07月22日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/param.h>
#include <time.h>

int main()
{
    pid_t pid;
    char *msg;
    int i;

    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork();
    if(pid > 0 )
    {
        printf("first father : %d\n",getpid());
        exit(0);
    }

    setsid();

    pid = fork();
    
    if(pid >0)
    {
        printf("second father: %d\n",getpid());
        exit(0);
    }
    for(i=0;i<NOFILE;close(i++));
    
    chdir("/");
 
    umask(0);

    signal(SIGCHLD, SIG_IGN);

    time_t now;    

    while(1)
    {
        time(&now);
        syslog(LOG_USER | LOG_INFO ,"time : \t%s\t\n",ctime(&now));
    }

    return 0;

}   
