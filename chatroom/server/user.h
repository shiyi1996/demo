/*************************************************************************
	> File Name: user.h
	> Author: 师毅
	> Mail:  shiyi19960604@gmail.com
	> Created Time: 2015年08月03日 星期一 15时49分15秒
 ************************************************************************/

#ifndef _USER_H
#define _USER_H

typedef struct User
{
        int id;
        char name[20];
        char pass[20];
}user_t;

typedef struct News
{
    int type;
    char buf[200];
	char from_name[20];
	char to_name[20];
	user_t user;
}new_t;


//根据用户名查找用户
int find_user_id(char name[], user_t *buf);

//根据id查找用户
int find_user_name(int id, user_t *buf);

//增加用户
int add_user(user_t *buf);

//创建id号
int make_id();

#endif
