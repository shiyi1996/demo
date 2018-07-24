/*===============================================================
*   Copyright (C) 2015 All rights reserved.
*   
*   文件名称：my_chmod.c
*   创 建 者：师毅
*   创建日期：2015年07月18日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>

int main(int argc,char **argv)
{
    int mode;//权限
    int temp = 0;//临时变量

    char *filename;//文件名

    if(argc<3)//判断命令是否合法
    {
        printf("error : chmod <mode> <filename>\n");
        exit(0);
    }    

    mode = atoi(argv[1]);//获取权限值

    if(mode>777 || mode<0)//判断权限值是否合法
    {
        printf("mode参数不合规范\n");
        exit(0);
    }

    while( mode>0 )//将权限值转化为八进制数
    {
        temp = temp*8 + mode%10;
        mode/=10;
    }

    mode = temp;
    filename = argv[2];//获取文件名

    if( chmod(filename,mode) == -1 )//执行修改权限操作，并判断是否修改成功
    {
        printf("权限修改失败\n");
        exit(1);
    }

    return 0;
}
