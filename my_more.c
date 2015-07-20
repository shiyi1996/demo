/*===============================================================
*   Copyright (C) 2015 All rights reserved.
*   
*   文件名称：my_more.c
*   创 建 者：师毅
*   创建日期：2015年07月20日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define PAGENUM 10

void do_more(FILE *fp);
int see_more(FILE *fp, int sum, int now);

int main( int argc, char **argv )
{
    FILE *fp;
    if( argc == 1 )
    {
        do_more( stdin );
    }
    else
    {
        while( argc--)
        {
            if( (fp = fopen(*++argv,"r")) != NULL )
            {   
                do_more(fp);
                fclose(fp);
            }
            else
            {
                exit(1);
            }
        }
    }
    return 0;
}

void do_more(FILE *fp)
{
    FILE *fp_tty;
    char line[512];
    int page_num = 0;
    int rel;
    int fd;
    int page_now;
    int page_sum;

    fd = fileno( fp );
    rel = lseek(fd,0,SEEK_END);
    printf("%d\n",rel);
    page_sum = lseek(fd,0,SEEK_CUR);
    rel = lseek(fd,0,SEEK_SET);
    printf("%d\n",rel);


    fp_tty = fopen("/dev/tty","r");
    if( fp_tty == NULL )
    {
        exit(1);
    }

    while( fgets(line,512,fp) )
    {
        page_now = lseek(fd, 0, SEEK_CUR);
        printf("%d\n",page_now);
        printf("*****%d\n",fseek(fp,0,SEEK_CUR));
        if( page_num == PAGENUM )
        {
            rel = see_more(fp_tty, page_sum, page_now);
            if( rel == 0 )
            {
                break;
            }
            else
            {
                page_num -= rel;
            }
        }

        if( fputs( line, stdout ) == EOF)
        {
            exit(2);
        }
        page_num++;
    }
}

int see_more(FILE *fp, int sum,int now)
{
    int rel;
    printf("now %d, sum %d",now , sum);
    printf("\033[7m --更多-- %.2f%% \033[0m",(sum - now)*100.0/sum);

    while( (rel = getc(fp)) != EOF )
    {
        if(rel == 'q')
            return 0;
        if(rel == ' ')
            return PAGENUM;
        if(rel == '\n')
            return 1;
    }

    return 0;
}
