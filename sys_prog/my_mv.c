/*===============================================================
*   Copyright (C) 2015 All rights reserved.
*   
*   文件名称：my_mv.c
*   创 建 者：师毅
*   创建日期：2015年07月19日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
    char oldpath[20];
    char nowpath[20];

    if(argc < 3)
    {
        printf("error : 参数不合法\n");
        exit(1);
    }

    strcpy(oldpath, argv[1]);
    strcpy(nowpath, argv[2]);

    if(rename(oldpath, nowpath) == -1)
    {
        printf("error: rename\n");
        exit(1);
    }

    return 0;
}
