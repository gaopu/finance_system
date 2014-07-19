/*
 * =====================================================================================
 *
 *       Filename:  finance.h
 *
 *    Description:  这里面包含的是结构体的定义
 *
 *        Version:  1.0
 *        Created:  2014年06月13日 20时13分54秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  geekgao, gaopu333@gmail.com
 *        Company:  Class 1302 of Software Engineer
 *
 * =====================================================================================
 */
#ifndef _FINANCE_H_
#define _FINANCE_H_


typedef struct User_list
{
	char name[21];
	char password[17];
	struct User_list *next;
}User_list;


enum In_out{in,out};
enum Type{clothes,food,home,traffic,communication,entertainment,
			study,social,medical,other_out,occupation,other_in,back};	//back只是辅助用来实现返回功能


typedef struct Data_list
{
	char time[26];		//录入信息的时间，大小是考虑到时间格式如下Mon Jun 16 11:05:43 2014\n
	enum In_out in_out;		//标记是支出还是收入
	enum Type type;			//标记各种类型
	double money;
	struct Data_list *next;
}Data_list;

#endif
