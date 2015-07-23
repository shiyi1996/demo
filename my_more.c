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
#include <fcntl.h>
#define PAGENUM 10

void do_more(int fd);
int see_more(FILE  *fp, int sum, int now);

int main( int argc, char **argv )
{
    int fd;
    if( argc == 1 )
    {
        return 0;
    }
    else
    {
        while( argc-->1 )
        {
            if( (fd = open(*++argv,O_RDONLY)) != -1 )
            {   
                do_more(fd);
                close(fd);
            }
            else
            {
                printf("Error: can't open file\n");
                exit(1);
            }
        }
    }
    return 0;
}

void do_more(int fd)
{
    char str[256];
    char *p = str;
    int page_num = 0;
    int rel;
    int page_now;
    int page_sum;
    FILE *fp_tty;

    rel = lseek(fd,0,SEEK_END);
    page_sum = lseek(fd,0,SEEK_CUR);
    rel = lseek(fd,0,SEEK_SET);


    fp_tty = fopen("/dev/tty","r");
    if( fp_tty == NULL )
    {
        exit(1);
    }

    while( page_now<page_sum )
    {
        while(read(fd,str,1))
        {
            if(str[0] =='\n')
            {
                printf("\n");
                break;
            }
            printf("%c",str[0]);
        }
        page_now = lseek(fd, 0, SEEK_CUR);
        if( page_num == PAGENUM )
        {
            rel = see_more(fp_tty, page_sum, page_now);
            printf("\033[1A\033[K");
            if( rel == 0 )
            {
                break;
            }
            else
            {
                page_num -= rel;
            }
        }
        page_num++;
    }
}

int see_more(FILE *fp, int sum,int now)
{
    int rel;
    printf("\033[7m --更多-- %.2f%% \033[0m",now*100.0/sum);

    while( (rel = getc(fp)) != EOF )
    {
        if(rel == 'q')
        {
            return 0;
		}
        if(rel == ' ')
        {
	    	return PAGENUM;
		}
        if(rel == '\n')
        {   
			return 1;
		}
    }
    
    return 0;
}
