/*************************************************************************
	> File Name: my_shell.c
	> Author: 师毅
	> Mail:  shiyi19960604@gmail.com
	> Created Time: 2015年07月23日 星期四 20时23分07秒
 ************************************************************************/

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<dirent.h>

#define none 0
#define in_redirect 1
#define out_redirect 2
#define have_pipe 3

void print();//输出提示信息
void get_cmd(char *);//获取命令
void explain_cmd(char *,int *,char a[][256] );//解释命令
void do_cmd(int , char a[][256]);//执行命令
int  find_cmd(char *);//寻找命令执行

int main(int argc, char **argv)
{
    int i;
    int argcount;
    char arglist[100][256];

    char *buf = (char *)malloc(sizeof(char)*256);
    if(buf == NULL)
    {
        printf("分配内存于buf失败\n");
        exit(1);
    }

    while(1)
    {
        argcount =0;
        print();
        get_cmd(buf);
        if(strcmp(buf,"quit\n")==0 || strcmp(buf,"exit\n")==0)
        {
            break;
        }
        explain_cmd(buf,&argcount,arglist);

        do_cmd(argcount,arglist);
    }
    if(buf!=NULL)
    {
        free(buf);
    }

    return 0;

}

int find_cmd(char *cmd)
{
    DIR *dp;
    struct dirent *dir;
    char* path[] = {"./","/bin","/usr/bin",NULL};
    int i=0;
    if(strncmp(cmd,"./",2)==0)
    {
        cmd += 2;
    }

    while(path[i] != NULL)
    {
        if((dp = opendir(path[i])) == NULL)
        {
            printf("error: can't open %s\n",path[i]);
        }
        while((dir = readdir(dp))!=NULL)
        {
            if(strcmp(dir->d_name,cmd)==0)
            {
                closedir(dp);
                return 1;
            }
        }
        i++;
        closedir(dp);
    }
    return 0;
}

void do_cmd(int argcount, char arg[][256])
{
    int i,j;
    int fd;
    int background = 0;
    int flag = 0;
    int how=0;
    char *arglist[argcount+1];
    char *file;
    char *arg_temp[256];
    pid_t pid;

    for(i=0;i<argcount;i++)
    {
        arglist[i] = (char*)arg[i];
    }
    arglist[argcount] = NULL;

    for(i=0;i<argcount;i++)
    {    
        if(strncmp(arglist[i],"&",1)==0)
        {
            if(i == argcount -1 )
            {
                background = 1;
                arglist[i]=NULL;
                argcount--;
                break;
            }
            else
            {
                printf("Wrong command\n");
                return ;
            }
        }
    }
    for(i=0;i<argcount;i++)
    {
        if(strcmp(arglist[i],">") == 0)
        {
            flag++;
            how = out_redirect;
            if(arglist[i+1]==NULL || i==0)
            {
                flag++;
            }
        }
        if(strcmp(arglist[i],"<") == 0)
        {
            flag++;
            how = in_redirect;
            if(arglist[i+1]==NULL || i==0)
            {
                flag++;
            }
        }
        if(strcmp(arglist[i],"|") == 0 )
        {
            flag++;
            how = have_pipe;
            if(arglist[i+1]==NULL || i==0)
            {
                flag++;
            }
        }
    }

    if(flag>1)
    {
        printf("参数不合法\n");
        return;
    }

    if(how == out_redirect || how == in_redirect)
    {
        for(i=0;arglist[i]!=NULL;i++)
        {
            if(strcmp(arglist[i],">")==0 || strcmp(arglist[i],"<")==0)
            {
                file = arglist[i+1];
            }
        }
    }

    if(how == have_pipe)
    {
        for(i=0;arglist[i]!=NULL;i++)
        {
            if(strcmp(arglist[i],"|")==0)
            {
                break;
            }
        }
        for(j=i+1;arglist[j]!=NULL;j++)
        {
            arg_temp[j-i-1]=arglist[j];
        }
        arg_temp[j-i-1] = arglist[j];
    }

    pid = fork();
    if(pid == -1)
    {
        printf("创建进程失败\n");
        return;
    }

    if(pid == 0)
    {
        if(how == none)
        {
            if(!find_cmd(arglist[0]))
            {
                printf("%s 不存在\n",arglist[0]);
                return;
            }
            execvp(arglist[0],(char **)arglist);
            exit(1);
        }
        else if(how == out_redirect)
        {
            if(!find_cmd(arglist[0]))
            {
                printf("%s 不存在",arglist[0]);
                return;
            }
            fd  = open(file,O_RDWR|O_CREAT|O_TRUNC,0644);
            dup2(fd,1);
            printf("**%d**\n",fd);
            execvp(arglist[0],(char **)arglist);
            puts("aaa");
            exit(1);
        }
        else if(how == in_redirect)
        {
            if(!find_cmd(arglist[0]))
            {
                printf("%s 不存在",arglist[0]);
                return;
            }
            fd = open(file,0);
            dup2(fd,0);
            execvp(arglist[0],(char **)arglist);
            exit(1);
        }
        else if(how == have_pipe)
        {
            if(!find_cmd(arglist[0]))
            {
                printf("%s 不存在",arglist[0]);
                return;
            }
            //...
            return ;
        }
    }
    
    if(background==1)
    {
        printf("[%d 进程后台执行]\n",pid);
        sleep(1);
        return ;
    }

    if(waitpid(pid,&flag,0)==-1)
    {
        printf("error:wait for the child\n");
    }

}

void explain_cmd(char *buf, int* argcount, char arglist[][256])
{
    char *p = buf;
    char *q = buf;
    int num;

    while(1)
    {
        if(p[0] == '\n')
        {
            break;
        }
        else
        {
            while(p[0]==' ')
                p++;
            q = p;
            num = 0;
            while(*q!=' ' && *q!='\n')
            {
                q++;
                num++;
            }
            strncpy(arglist[*argcount],p,num+1);
            arglist[*argcount][num] = '\0';
            (*argcount)++;
            p=q;
        }
    }
}

void get_cmd(char *buf)
{
    char ch;
    int len = 0;

    ch = getchar();

    while(ch!='\0' && ch!='\n')
    {
        buf[len++] = ch;
        ch = getchar();
    }

    if(len > 256)
    {
        printf("the cmd was too long\n");
        exit(1);
    }
    buf[len++] = '\n';
    buf[len]='\0';
}

void print()
{
    printf("shiyi$$:");
}
