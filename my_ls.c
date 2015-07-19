/*===============================================================
*   Copyright (C) 2015 All rights reserved.
*   
*   文件名称：my_ls.c
*   创 建 者：师毅
*   创建日期：2015年07月19日
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
#include <string.h>
#include <time.h>
int main(int argc, char **argv)
{
    char filename[20];
    struct stat buf;
    
    if(argc!=2)
    {
        printf("参数不合法\n");
        exit(1);
    }
    
    strcpy(filename, argv[1]);
    
    if(stat(filename,&buf) == -1)
    {
        printf("error: stat\n");
        exit(2);
    }

    printf("设备编号：%d\n",buf.st_dev);
    printf("i-node节点编号：%d\n",buf.st_ino);
    printf("文件类型及存储权限：%o\n",buf.st_mode);
    printf("硬链接数目：%d\n",buf.st_nlink);
    printf("用户id：%d\n",buf.st_uid);
    printf("组id：%d\n",buf.st_gid);
    printf("设备编号：%d\n",buf.st_rdev);
    printf("文件大小：%d\n",buf.st_size);
    printf("I/O缓存区大小：%d\n",buf.st_blksize);
    printf("占用文件区块个数：%d\n",buf.st_blocks);
    printf("最近访问时间：%s",ctime(&buf.st_atime));
    printf("最近修改时间：%s",ctime(&buf.st_mtime));//一般在调用utime/write时才会改变
    printf("最近更改时间：%s",ctime(&buf.st_ctime));//文件所有者/所属组/文件权限 被更改时更新

    return 0;
}
