/*===============================================================
*   Copyright (C) 2015 All rights reserved.
*   
*   文件名称：my_ls.c
*   创 建 者：师毅
*   创建日期：2015年07月20日
*   描    述：
*
*   更新日志：
*
================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <grp.h>
#include <pwd.h>
#include <sys/stat.h>

#define PARAM_NONE 0 //ls
#define PARAM_A 1 //-a
#define PARAM_L 2 //-l
#define PARAM_R 4 //-r
#define MAXROELEN 80 //一行最大显示长度

int maxlen;
int leave = MAXROELEN;

int file_i = 0;
int file_num = 0;
char file_name[1000][100];

void quick_sort(int l, int r, int num[], char name[][50])
{
    int i,j,pos;
    pos = num[l];
    i = l;
    j = r;

    if(l >= r)
    {
        return;
    }

    while( i<j )
    {
        while( strcmp(name[num[j]], name[pos]) > 0 && i<j)
        {
            j--;
        }
        if(i<j)
        {
            num[i] = num[j];
        }

        while( strcmp(name[num[i]], name[pos]) < 0 && i<j )
        {
            i++;
        }
        if(i<j)
        {
            num[j] = num[i];
        }
    }

    num[i] = pos;

    quick_sort(i+1, r, num, name);
    quick_sort(l, i-1, num, name);
}


void display_attribute( struct stat buf, char *name)
{
    int i, t;
    char str[10];
    char buf_time[30];
    struct passwd *pwd = (struct passwd*)malloc( sizeof(struct passwd) );
    struct group *grp = (struct group*)malloc( sizeof(struct group) );

    if( S_ISLNK( buf.st_mode ) )//链接文件
    {
        printf("l");
    }
    else if( S_ISDIR( buf.st_mode ) )//目录文件
    {
        printf("d");
    }
    else if( S_ISREG( buf.st_mode ) )//一般文件
    {
        printf("-");
    }
    else if( S_ISCHR( buf.st_mode ) )//字符设备文件
    {
        printf("c");
    }
    else if( S_ISBLK( buf.st_mode ) )//块设备文件
    {
        printf("b");
    }

    for( i=0;i<9;i++)
    {
        str[i] = '-';
    }

    for( i=0;i<3;i++)
    {
        t = buf.st_mode % 8;
        if( t >= 4)
        {
            str[i*3+2] = 'r';
            t -= 4;
        }
        if( t >= 2 )
        {
            str[i*3+1] = 'w';
            t -= 2;
        }
        if( t == 1)
        {
            str[i*3] = 'x';
        }
        buf.st_mode /= 8;
    }    

    for( i=8;i>=0;i--)
    {
        printf("%c",str[i]);//打印文件权限

    }

    printf(" %2d ",(int)buf.st_nlink);//打印文件硬链接数量

    pwd = getpwuid( buf.st_uid );
    grp = getgrgid( buf.st_gid );
    printf("%-8s", pwd->pw_name);//打印文件所有者
    printf("%-8s", grp->gr_name);//打印文件所属组

    printf("%-6d",(int)buf.st_size);//打印文件大小

    strcpy( buf_time, ctime(&buf.st_mtime) );    	
    buf_time[ strlen(buf_time)-1 ] = '\0';
    printf(" %s",buf_time);//打印文件修改日期

    printf("  %s\n",name);//打印文件名

}

//打印目录下文件名
void display_single( char *name )
{
    int len ;

    if( leave < maxlen )
    {
        printf("\n");
        leave = MAXROELEN;
    }
    printf("%s",name);
	len = maxlen - strlen(name);

	if(len>leave)
	{
		printf("\n");
		leave = MAXROELEN;
	}
	else
	{
	    while(len>0)
	    {
	        printf(" ");
	        len--;
		}
    	leave -= maxlen + 2;
  	}

}

//对目录中的文件操作
int display_dir(int flag, char *path)
{
	int i,j,len = strlen(path);
    DIR *dir;
    struct dirent *buf;
    int count = 0;
    char filename[256][50];
	int num[256];

    dir = opendir(path);
    if(dir == NULL)
    {
        printf("error : opendir\n");
        exit(1);
    }
	i=0;
    while( (buf = readdir(dir)) != NULL )
    {
		if( (flag & PARAM_A) || buf->d_name[0]!='.' )
		{
        	if( maxlen < strlen(buf->d_name) )
        	{
           		maxlen = strlen(buf->d_name);
        	}

			strncpy(filename[i], path, len);
			filename[i][len] = '\0';
			strcat(filename[i], buf->d_name);
			filename[i][strlen(buf->d_name) + len] = '\0';
		
			num[i]=i;
        	i++;
		}
    }
    closedir(dir);
    
    if(i > 256)
    {
        printf("error： filenum > 256\n");
        exit(1);
    }
    
	quick_sort(0, i-1, num, filename);

	for(j=0; j<i; j++)
	{
		display(flag, filename[ num[j]]);
	}
	
}   

int display( int flag, char *path)
{
    int i,j;
    struct stat buf;
    char name[15];

    for(i=0,j=0; i<strlen(path); i++)
    {
        if(path[i] == '/' && i!=strlen(path)-1)
        {
            j=0;
            continue;
        }
        name[j++] = path[i];
    }
    name[j] = '\0';

    if( lstat(path, &buf) == -1 )
    {
        printf("error : lstat\n");
        exit(3);
    }

    switch(flag)
    {
        case PARAM_NONE:
            if( name[0] != '.')
            {
                display_single( name );
            }
            break;
        case PARAM_A:
            display_single( name );
            break;
        case PARAM_L:
            if( name[0] != '.')
            {
                display_attribute( buf,name );
            }
            break;
        case PARAM_A | PARAM_L:
            display_attribute( buf, name );
            break;
    }
}

int display_r(int flag,char *path)
{
	struct stat p;
	struct dirent *buf;
	DIR *dir;
	int len;

    dir = opendir(path);
    if(dir == NULL)
    {
        printf("error : opls -Rendir\n");
        exit(1);
    }

    while( (buf = readdir(dir)) != NULL )
    {
		if( (flag & PARAM_A) || buf->d_name[0]!='.' )
		{

			if( stat(buf->d_name, &p) == -1 )
			{			
				len = strlen(path);
				strncpy(file_name[file_num], path, len);
				if(file_name[file_num][len -1]!='/')
				{
					strcpy(file_name[file_num], buf->d_name);
					file_name[file_num][len ]='/';
					len ++;
				}
				file_name[file_num][len]='\0';
				strcat(file_name[file_num], buf->d_name);
				file_name[file_num++][strlen(buf->d_name) + len] = '\0';			
			}
		}	
	}
		//path[strlen(path)-1] = '\0';


}

int main( int argc, char *argv[] )
{
    int i, j, k, num;
    char cmd[50];
    int flag_param = PARAM_NONE;
    char path[100];
    struct stat buf;


    i = 1; j = num = 0;
    while( i < argc )
    {
        if( argv[i][0] == '-' )
        {
            num++;
            for( k=1; k < strlen(argv[i]); k++)
            {
                cmd[j++] = argv[i][k];
            }
        }
        i++;
    }

    for( i=0; i<j; i++)
    {
        if( cmd[i] == 'l')
        {
            flag_param |= PARAM_L;
        }
        else if( cmd[i] == 'a')
        {
            flag_param |= PARAM_A;
        }
        else if( cmd[i] == 'R')
        {
            flag_param |= PARAM_R;
        }
        else
        {
            printf("不合法参数 -%c\n",cmd[i]);
            exit( 1 );
        }
    }    
    
    //没有文件或目录参数
    if( num+1 ==argc )    
    {
        strcpy( path, "./");
        path[2] = '\0';
        if( flag_param & PARAM_R)
        {
            strcpy(file_name[file_num++],"./");
            file_name[file_num-1][3]='\0';
        }
        else
        {
            display_dir( flag_param, path);
        }
    }
    else
    {
        i=1;

        while( i < argc )
        {
            if( argv[i][0] != '-' )
            {
                strcpy( path, argv[i]);
                if( stat( path, &buf) == -1 )
                {
                    printf("error : %s not a dir or file\n",argv[i]);
                    exit(2);
                }

                if( S_ISREG( buf.st_mode) )//文件
                {        
                    display( flag_param, path );
                }
                else//目录
                {
                    if( path[strlen(argv[i])-1] != '/' )
                    {
                        path[strlen(argv[i])] = '/';
                        path[strlen(argv[i])+1] = '\0';
                    }
                    else
                    {
                        path[strlen(argv[i])] = '\0';
                    }

        			if( flag_param & PARAM_R)
        			{
            			strcpy(file_name[file_num++],path);
						file_name[file_num-1][strlen(path)] = '\0';
        			}
        			else
        			{
         			    display_dir( flag_param, path);
        			}
                }
            }
            i++;
        }
    }
    if( flag_param & PARAM_R)
    {
		i = 0;
		while( i < file_num )
		{
			printf("%s:\n",file_name[i]);
			display_dir(flag_param,path);
			display_r(flag_param,file_name[i++]);
			printf("\n");
		}
    }
    puts("");
    return 0;
}

