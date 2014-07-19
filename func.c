/*
 * =====================================================================================
 *
 *       Filename:  func.c
 *
 *    Description:  所有用到的函数的定义
 *
 *        Version:  1.0
 *        Created:  2014年06月13日 21时12分59秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  geekgao, gaopu333@gmail.com
 *        Company:  Class 1302 of Software Engineer
 *
 * =====================================================================================
 */

#include "finance.h"
#include <stdio.h>
#include <termio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
//======================================================================================
void menu1();
void menu2();
void login(char user[]);				//将登陆的用户的用户名放到user指向的空间
void add(const char username[]);
void look(const char username[]);
void edit(const char username[]);
void export(const char username[]);
int change_password(const char username[]);
int del_user(const char username[]);
void creat_user();
int login_with_password(char username[]);
User_list *creat_User_list();
Data_list *creat_Data_list();
void mygets(char str[],int count);		//控制输入的字符个数为count个,包括'\0'
void clear_User_list(User_list *head);
void clear_Data_list(Data_list *head);
int read_data_to_list(const char username[],Data_list *head);
int read_user_to_list(User_list *head);		//根据返回值判断是否成功
int write_data_to_file(const char username[],Data_list *head);
int write_user_to_file(User_list *head);	//返回值表示写入文件的数据个数
void print_in_out(Data_list *node);
void print_type(Data_list *node);
enum In_out select_in_out();
enum Type select_type(enum In_out tmp);
void lock(char str[]);
void data_sort(const char username[]);
int timecmp(char str1[],char str2[]);	//str1和str2是函数ctime返回的字符串去掉'\n'那个格式的字符串
int monstr_to_int(char str[]);
int intcmp(const void *a,const void *b);
void moneysort(Data_list *head,int count,int type);		//约定type为1按升序排序，为0按降序排序
void timesort(Data_list *head,int count,int type);
void set_time(int year,int month,int mday);
void input_time(char str1[],char str2[]);
//======================================================================================

void menu2()
{
	printf("*************************************************************\n");
	printf("\t\t\t个人财务管理系统\n");
	printf("*************************************************************\n");
	printf("\t\t\t(1)记一笔\n");
	printf("\t\t\t(2)查看财务报表\n");
	printf("\t\t\t(3)编辑信息\n");
	printf("\t\t\t(4)数据排序\n");
	printf("\t\t\t(5)导出账本到文件\n");
	printf("\t\t\t(6)修改密码\n");
	printf("\t\t\t(7)删除账户\n");
	printf("\t\t\t(8)退出系统\n");
	printf("*************************************************************\n");
	printf("\t\t\t    请选择:");
}
void menu1()
{
	printf("*************************************************************\n");
	printf("\t\t\t个人财务管理系统\n");
	printf("*************************************************************\n");
	printf("\t\t\t(1)登陆系统\n");
	printf("\t\t\t(2)创建账户\n");
	printf("\t\t\t(3)退出系统\n");
	printf("*************************************************************\n");
	printf("\t\t\t    请选择:");
}
void login(char user[])
{
	int sel = 0;

	while (1)
	{
		system("clear");
		menu1();
		scanf("%d",&sel);
		while (getchar() != '\n')	;
		system("clear");
		switch(sel)
		{
			case 1:
				if (login_with_password(user)){
					return ;
				}
				break;
			case 2:
				creat_user();
				break;
			case 3:
				exit(0);
				break;
			default:
				break;
		}
	}
}
void add(const char username[])
{
	Data_list tmp;
	FILE *fp;
	time_t timep;
	int sel = 0,flag = 0;
	char name[22]=".";
	long filelength = 0;

	strcat(name,username);
	fp = fopen(name,"a+");
	if (fp == NULL) {
		printf("文件打开失败!程序将返回.\n");
		printf("按回车继续.\n");
		while (getchar() != '\n')	;
		return ;
	}

	fseek(fp,0L,SEEK_END);
	filelength = ftell(fp);
	rewind(fp);

	tmp.next = NULL;
	time(&timep);
	strcpy(tmp.time,ctime(&timep));
	tmp.time[24]= 0;				//ctime()返回的字符串后面自带一个回车，这里把那个回车去掉
	tmp.in_out = select_in_out();
	if (tmp.in_out == back) {
		fclose(fp);
		if (!filelength) {
			remove(name);
		}
		return ;
	}
	tmp.type = select_type(tmp.in_out);
	if (tmp.type == back) {
		fclose(fp);
		if (!filelength) {
			remove(name);
		}
		return ;
	}

	printf("请输入钱数:\n");
	while (1)
	{
		flag = scanf("%lf",&tmp.money);
		while (getchar() != '\n')	;
		if (flag == 1 && tmp.money >= 0) {
			break;
		}
	}
	
	fwrite(&tmp,sizeof(Data_list),1,fp);
	fclose(fp);
	printf("%s恭喜您，本次记账成功.\n",username);
	printf("按回车返回.\n");
	while (getchar() != '\n')	;
}
void look(const char username[])
{
	Data_list *head = creat_Data_list(),*p = head,*tmp = NULL;
	int sel = 0,count = 1,i = 0,sel_all = 0;
	double in_money = 0,out_money = 0;
	char nowtime[26],time1[26],time2[26];
	time_t timep;

	if (!read_data_to_list(username,head)) {
		printf("还没有账本!程序将返回.\n");
		printf("按回车继续.\n");
		while (getchar() != '\n')	;
		return ;
	}

	while (1)
	{
		printf("请选择是(1)查看全部(2)查看支出(3)查看收入(4)退出:\n");
		scanf("%d",&sel);
		while (getchar() != '\n')	;
		if (sel >= 1 && sel <= 3) {
			break;
		}
		if (sel == 4) {	
			clear_Data_list(head);
			return ;
		}
	}
	if (sel == 1) {
		sel_all = 1;
	}
	//留下符合要求的节点
	while (p->next)
	{
		if ((sel == 2 && p->next->in_out == in) || (sel == 3 && p->next->in_out == out)) {
			tmp = p->next;
			p->next = p->next->next;
			free(tmp);
		}
		else {
			p = p->next;
		}
	}

	printf("请选择是(1)查看全部(2)查看本月(3)查看本年(4)自定义时间段(5)退出:\n");
	while (1)
	{
		scanf("%d",&sel);
		while (getchar() != '\n')	;
		if (sel >= 1 && sel <= 4) {
			break;
		}
		if (sel == 5) {	
			clear_Data_list(head);
			return ;
		}
	}
	p = head;
	time(&timep);
	strcpy(nowtime,ctime(&timep));
	//留下符合要求的节点
	switch (sel)
	{
		case 1:
			break;
		case 2:
			while (p->next)
			{
				//月份的三个字母在456位置
				for (i = 4;i <= 6 && (nowtime[i] == p->next->time[i]);i++)	;	
				if (i <= 6) {
					tmp = p->next;
					p->next = p->next->next;
					free(tmp);
				}
				else {
					p = p->next;
				}
			}
			break;
		case 3:
			while (p->next)
			{
				for (i = 20;i <= 23 && (nowtime[i] == p->next->time[i]);i++)	;
				if (i <= 23) {
					tmp = p->next;
					p->next = p->next->next;
					free(tmp);
				}
				else {
					p = p->next;
				}
			}
			break;
		case 4:
			input_time(time1,time2);
			while (p->next)
			{
				if (timecmp(p->next->time,time1) == 1 || timecmp(time2,p->next->time) == 1) {
					tmp = p->next;
					p->next = p->next->next;
					free(tmp);
				}
				else {
					p = p->next;
				}
			}
		default:
			break;
	}

	p = head->next;
	printf("编号\t时间\t\t\t\t收入/支出\t类别\t\t钱数\n");
	while (p)
	{
		printf("(%d)\t",count);
		printf("%s\t",p->time);
		print_in_out(p);
		printf("\t\t");
		print_type(p);
		printf("\t");
		printf("%.2f元\n",p->money);
		if (p->in_out == in) {
			in_money += p->money;
		}
		else if (p->in_out == out) {
			out_money += p->money;
		}
		count++;
		p = p->next;
		
	}
	printf("支出%.2lf元\n",out_money);
	printf("收入%.2lf元\n",in_money);
	if (sel_all) {
		printf("净收入%.2lf元\n",in_money-out_money);
	}

	clear_Data_list(head);
	printf("按回车程序将返回.\n");
	while (getchar() != '\n')	;
}
void edit(const char username[])
{
	Data_list *head = creat_Data_list(),*p,*tmp;
	int count = 0,num = 0,num2[1000] = {0},sel = 0,flag = 0,one_menu = 0,two_menu = 0,three_menu = 0,i = 0,j = 0,t = 0;
	char name[22] = ".",ch = 0,nowtime[26],time1[26],time2[26],y_n = 0;
	time_t timep;

	if (!read_data_to_list(username,head)) {
		printf("还没有任何数据！程序将返回.\n");
		printf("按回车继续.\n");
		while (getchar() != '\n')	;
		return ;
	}
	
	while (1)
	{
		printf("请选择功能项:\n");
		printf("(1)编辑条目(2)删除条目(3)退出\n");
		scanf("%d",&one_menu);
		while (getchar() != '\n')	;
		if (one_menu >= 1 && one_menu <= 3) {
			break;
		}
	}
	
	switch (one_menu)
	{
	  case 1:
		printf("编号\t时间\t\t\t\t收入/支出\t类别\t\t钱数\n");
		p = head->next;
		while (p)
		{
			count++;
			printf("(%d)\t",count);
			printf("%s\t",p->time);
			print_in_out(p);
			printf("\t\t");
			print_type(p);
			printf("\t");
			printf("%.2f元\n",p->money);
			p = p->next;
		}
		while (1)
		{
			printf("请选择编辑哪个编号的信息:\n");
			scanf("%d",&num);
			while (getchar() != '\n')	;
			if (num >= 1 && num <= count) {
				break;
			}
		}

		p = head;
		count = 0;
		while (p->next)
		{
			count++;
			if (count == num) {
				break;
			}
			p = p->next;
		}
	    while (1)
		{
			printf("请选择功能项:\n");
			printf("(1)修改支出/收入(2)修改类别(3)修改钱数(4)退出\n");
			scanf("%d",&two_menu);
			while (getchar() != '\n')	;
			if (two_menu >= 1 && two_menu <= 4) {
				break;
			}
		}
		switch (two_menu)
		{
			case 1:
				p->next->in_out = select_in_out();
				if (p->next->in_out == back) {
				  	clear_Data_list(head);
					return ;
				}
				p->next->type = select_type(p->next->in_out);
				if (p->next->type == back) {
				  	clear_Data_list(head);
					return ;
				}
					break;
			case 2:
				p->next->type = select_type(p->next->in_out);
				if (p->next->type == back) {
				  	clear_Data_list(head);
					return ;
				}
					break;
			case 3:
				printf("请输入钱数:\n");
				while (1)
				{
					flag = scanf("%lf",&p->next->money);
					while (getchar() != '\n')	;
					if (flag == 1 && p->next->money >= 0) {
						break;
					}
				}
				break;
			case 4:
					clear_Data_list(head);
					return ;
			default:
					break;
			}
		
		break;
	  case 2:
	   	 while (1)
		{
			printf("请选择功能项:\n");
			printf("(1)按序号删除(2)按时间段删除(3)退出\n");
			scanf("%d",&two_menu);
			while (getchar() != '\n')	;
			if (two_menu >= 1 && two_menu <= 3) {
				break;
			}
		}
		switch (two_menu)
		{
			case 1:
			  	p = head->next;
				printf("编号\t时间\t\t\t\t收入/支出\t类别\t\t钱数\n");
				while (p)
				{
					count++;
					printf("(%d)\t",count);
					printf("%s\t",p->time);
					print_in_out(p);
					printf("\t\t");
					print_type(p);
					printf("\t");
					printf("%.2f元\n",p->money);
					p = p->next;
				}
					printf("请选择删除哪些编号的信息(逗号隔开):\n");
					num = 0;
					while (num < 1000)
					{
						scanf("%d",&num2[num]);
						ch = getchar();
						if (ch == '\n') {
							break;
						}
						if (num2[num]) {	//编号肯定是从0开始
						  	num++;
						}
					}
					//用快排给序号数组排序
					qsort(num2,num+1,sizeof(int),intcmp);
					p = head;
					count = 0;
					i = 0;
					while (p->next)
					{
						count++;
						if (count == num2[i]) {
							tmp = p->next;
							p->next = p->next->next;
							free(tmp);
							i++;
						}
						else {
						  	p = p->next;
						}
						if (i == num+1) {
						  	break;
						}
					}
				break;
			case 2:
				while (1)
				{
					printf("请选择:\n");
					printf("(1)删除本月(2)删除本年(3)自定义时间段(4)退出\n");
					scanf("%d",&three_menu);
					while (getchar() != '\n')	;
					if (three_menu >= 1 && three_menu <= 4) {
						break;
					}
				}
				p = head;
				time(&timep);
				strcpy(nowtime,ctime(&timep));
				switch (three_menu)
				{
					case 1:
						while (p->next)
						{
							//月份的三个字母在456位置
							for (i = 4;i <= 6 && (nowtime[i] == p->next->time[i]);i++)	;	
							if (i > 6) {
								tmp = p->next;
								p->next = p->next->next;
								free(tmp);
							}
							else {
								p = p->next;
							}
						}
						break;
					case 2:
						while (p->next)
						{
							for (i = 20;i <= 23 && (nowtime[i] == p->next->time[i]);i++)	;
							if (i > 23) {
								tmp = p->next;
								p->next = p->next->next;
								free(tmp);
							}
							else {
								p = p->next;
							}
						}
						break;
					case 3:
						input_time(time1,time2);
						while (p->next)
						{
							if (timecmp(time1,p->next->time) == 1 && timecmp(p->next->time,time2) == 1) {
								tmp = p->next;
								p->next = p->next->next;
								free(tmp);
							}
							else {
								p = p->next;
							}
						}
						break;
					case 4:
						clear_Data_list(head);
						return ;
					default:
						break;
				}
				break;
			case 3:
				clear_Data_list(head);
				return ;
			default:
				break;
		}
		break;
	  case 3:
		clear_Data_list(head);
		return ;
	  default:
		break;
	}
	printf("请确认是否使以上操作生效？(Y确认)");
	y_n = getchar();
	while (getchar() != '\n')		;
	if (y_n != 'y' && y_n != 'Y') {
		clear_Data_list(head);
		printf("操作取消，程序将返回.\n");
		printf("按回车继续.\n");
		while (getchar() != '\n')		;
		return ;
	}

	count = write_data_to_file(username,head);
	if (!count) {
	  	remove(strcat(name,username));
	}
	clear_Data_list(head);
	printf("操作成功!程序将返回.\n");
	printf("按回车继续.\n");
	while (getchar() != '\n')		;
}
void export(const char username[])
{
	FILE *fp;
	char newfile[26];
	Data_list *head = creat_Data_list(),*p;
	int count = 1;
	double in_money = 0,out_money = 0;

	if (!read_data_to_list(username,head)) {
		printf("还没有账单,请先创建账单.\n");
		printf("按回车继续.\n");
		while (getchar() != '\n')	;
		return ;
	}

	strcpy(newfile,username);
	strcat(newfile,"_data");

	
	if (!(fp = fopen(newfile,"w"))) {
		printf("创建文件失败!程序将返回.");
		printf("按回车继续.\n");
		while (getchar() != '\n')	;
		return ;
	}

	fprintf(fp,"编号\t时间\t\t\t\t\t\t收入/支出\t类别\t\t钱数\n");
	p = head->next;
	while (p)
	{
		fprintf(fp,"%d\t\t",count++);
		fprintf(fp,"%s\t",p->time);
		switch(p->in_out)
		{
			case in:
				fprintf(fp,"收入\t\t");
				in_money += p->money;
				break;
			case out:
				fprintf(fp,"支出\t\t");
				out_money += p->money;
				break;
		}
		switch(p->type)
		{
			case clothes:
				fprintf(fp,"衣服饰品\t");
				break;
			case food:
				fprintf(fp,"食品水酒\t");
				break;
			case home:
				fprintf(fp,"居家物业\t");
				break;
			case traffic:
				fprintf(fp,"行车交通\t");
				break;
			case communication:
				fprintf(fp,"交流通讯\t");
				break;
			case entertainment:
				fprintf(fp,"休闲娱乐\t");
				break;
			case study:
				fprintf(fp,"学习进修\t");
				break;
			case social:
				fprintf(fp,"人情往来\t");
				break;
			case medical:
				fprintf(fp,"医疗保险\t");
				break;
			case other_out:
				fprintf(fp,"其他杂项\t");
				break;
			case occupation:
				fprintf(fp,"职业收入\t");
				break;
			case other_in:
				fprintf(fp,"其他收入\t");
				break;
		}
		fprintf(fp,"%.2f元\n",p->money);
		p = p->next;
	}
	fprintf(fp,"支出:%.2lf元\n",out_money);
	fprintf(fp,"收入:%.2lf元\n",in_money);
	fprintf(fp,"净收入:%.2lf元",in_money - out_money);
	fclose(fp);
	clear_Data_list(head);
	printf("%s恭喜您，导出到文件成功!\n",username);
	printf("账本文件名为:%s\n",newfile);
	printf("按回车程序将返回.\n");
	while (getchar() != '\n')	;
}
int change_password(const char username[])
{
	User_list *head = creat_User_list(),*p = head->next;
	char name[21],password[50],*str,y_n = 0;
	int count = 1,flag = 0,err = 0;
	
	if (!read_user_to_list(head)) {
		printf("读取账户数据出错!程序将返回.\n");
		printf("按回车继续.\n");
		while (getchar() != '\n')	;
		return ;
	}

	strcpy(name,username);
	lock(name);
	system("stty -echo");
	while (1)
	{
		if (count++ > 3) {
			clear_User_list(head);
			system("stty echo");
			printf("对不起，三次尝试错误，程序将退出以保护用户数据！\n");
			printf("按回车继续.\n");
			while (getchar() != '\n')	;
			exit(0);
		}
		printf("请输入旧密码(三次机会)：\n");
		mygets(password,50);
		lock(password);
		p = head->next;
		while (p)
		{
			if (!strcmp(password,p->password) && !strcmp(name,p->name)) {
				flag = 1;
				break;
			}
			p = p->next;
		}
		if (flag) {
			break;
		}
	}
	printf("请输入新的密码(截取前16个字符，只能含有英文或数字):\n");

	str = p->password;
	do
	{
		if (err == 1) {
			printf("对不起，输入不符合要求!\n");
			printf("请重新年输入:\n");
		}
		err = 0;
		mygets(p->password,17);

		if (!strlen(p->password)) {
			err = 1;
			continue;
		}
		while (*str)
		{
			if (!isalpha(*str) && !isalnum(*str)) {
				err = 1;
				str = p->password;
				break;
			}
			str++;
		}
	}while (err);
	system("stty echo");

	lock(p->password);
	printf("确认更改密码吗？(Y确认)\n");
	y_n = getchar();
	while (getchar() != '\n')	;
	if (y_n != 'y' && y_n != 'Y') {
		clear_User_list(head);
		printf("您取消了更改密码，程序将返回.\n");
		printf("按回车继续.\n");
		while (getchar() != '\n')	;
		return 0;
	}

	write_user_to_file(head);
	clear_User_list(head);
	printf("密码已经成功更改！请重新登陆！\n");
	printf("按回车继续.\n");
	while (getchar() != '\n')	;

	return 1;
}
int del_user(const char username[])
{
	User_list *head = creat_User_list(),*p,*tmp;
	FILE *fp;
	char name[22] = ".",sel = 0,lockname[21],password[50];
	int count = 0,flag = 0;
	
	if (!read_user_to_list(head)) {
		printf("读取用户信息出错!程序将返回\n");
		printf("按回车继续.\n");
		while (getchar() != '\n')	;
		return 0;
	}
	
	strcpy(lockname,username);
	lock(lockname);
	p = head;
	while (p->next)
	{
		if(!strcmp(p->next->name,lockname)) {
			printf("请输入密码，您有3次机会，请珍惜：\n");
			while (count < 3)
			{
				printf("密码:");
				system("stty -echo");
				mygets(password,50);
				system("stty echo");
				lock(password);
				if (!strcmp(password,p->next->password)) {
					tmp = p->next;
					p->next = p->next->next;
					free(tmp);
					flag = 1;//标记成功输入密码
					printf("\n");
					break;
				}
				printf("\n");
				count++;
				if (count == 3) {
					printf("您输入的密码不正确!程序将退出!\n");
					printf("按回车继续.");
					while (getchar() != '\n')	;
					exit(1);
				}
			}
		}
		if (flag) {
			break;
		}
		p = p->next;
	}

	printf("确认要删除账户吗？(Y确认)");
	sel = getchar();
	while (getchar() != '\n')	;
	if (sel != 'y' && sel != 'Y') {
		printf("取消删除账户，程序将返回.\n");
		printf("按回车继续.\n");
		while (getchar() != '\n')	;
		return 0;
	}
	
	strcat(name,username);
	fp =fopen(name,"r");
	if (fp) {
		printf("当前账户存在账本数据，是否将账本信息导出到文件后再删除账户？(Y确认)\n");
		sel = getchar();
		while (getchar() != '\n')	;
		if (sel == 'y' || sel == 'Y') {
			export(username);
		}
		fclose(fp);
	}
	remove(name);

	count = write_user_to_file(head);
	if (!count) {
		remove(".user");
	}
	clear_User_list(head);
	return 1;
}
void creat_user()
{
	FILE *fp;
	User_list *p = NULL,*head = creat_User_list(),tmp;
	int err = 0,had_file = 0;
	char *str = tmp.name,true_name[21];

	//如果文件存在	
	if (read_user_to_list(head)) {
		had_file = 1;
	}
	fp = fopen(".user","a+");
	if (fp == NULL) {
		printf("文件打开失败!程序将返回.\n");
		printf("按回车继续.\n");
		while (getchar() != '\n')	;
		return ;
	}

	printf("请输入新的用户名(截取前20个字符,只能含有英文或数字):\n");
	do
	{
		if (err == 1) {
			printf("对不起，已经含有此用户名!\n");
			printf("请重新年输入:\n");
		}
		if (err == 2) {
			printf("对不起，输入不符合要求!\n");
			printf("请重新年输入:\n");
		}
		err = 0;
		mygets(tmp.name,21);
		if (!strlen(tmp.name)) {
			err = 2;
			continue;
		}
		while (*str)
		{
			if (!isalpha(*str) && !isalnum(*str)) {
				err = 2;
				str = tmp.name;
				break;
			}
			str++;
		}

		if (had_file && !err) {
			p = head->next;
			while (p)
			{
				if (!strcmp(p->name,tmp.name)) {
					err = 1;
					break;
				}
				p = p->next;
			}
		}
	}while (err);
	str = tmp.password;
	printf("请输入密码(截取前16个字符，只能含有英文或数字):\n");

	system("stty -echo");
	do
	{
		if (err == 2) {
			printf("对不起，输入不符合要求!\n");
			printf("请重新年输入:\n");
		}
		err = 0;
		mygets(tmp.password,17);

		if (!strlen(tmp.password)) {
			err = 2;
			continue;
		}
		while (*str)
		{
			if (!isalpha(*str) && !isalnum(*str)) {
				err = 2;
				str = tmp.password;
				break;
			}
			str++;
		}
	}while (err);
	system("stty echo");
	
	strcpy(true_name,tmp.name);
	lock(tmp.password);
	lock(tmp.name);
	fwrite(&tmp,sizeof(User_list),1,fp);
	fclose(fp);
	clear_User_list(head);

	printf("%s恭喜您，您的账户创建成功!\n",true_name);
	printf("按回车返回.\n");
	while (getchar() != '\n')	;
}
int login_with_password(char username[])
{
	User_list *head = creat_User_list(),*p = NULL;
	int err = 1,count = 1;
	char name[50],password[50];
	//如果文件存在	
	if (!read_user_to_list(head)) {
		printf("对不起，还没有任何用户被创建或者文件打开出错!\n");
		printf("按回车返回.\n");
		while (getchar() != '\n');
		return 0;
	}

	printf("您有5次机会输入，请珍惜：\n");
	do
	{
		if (count++ >5) {
			printf("对不起，您已经尝试达到五次，程序将退出!\n");
			exit(0);
		}
		if (err == 2) {
			printf("用户名或密码错误，请重新输入!\n");
		}
		printf("请输入用户名:");
		mygets(name,50);
		strcpy(username,name);
		lock(name);
		printf("请输入密码:");
		system("stty -echo");
		mygets(password,50);
		lock(password);
		printf("\n");
		system("stty echo");
		
		p = head->next;
		while (p)
		{
			if (!strcmp(name,p->name) && !strcmp(password,p->password)) {
				err = 0;
				break;
			}
			p = p->next;
			err = 2;
		}
	}while (err);

	clear_User_list(head);
	return 1;
}
User_list *creat_User_list()
{
	User_list *head;

	head = (User_list *)malloc(sizeof(User_list));
	head->next = NULL;

	return head;
}
Data_list *creat_Data_list()
{
	Data_list *head;

	head = (Data_list *)malloc(sizeof(Data_list));
	head->next = NULL;

	return head;
}
void mygets(char str[],int count)
{
	char ch;
	int i = 0;
	int flag = 0;		//标记输入的字符个数是否超过界线
	ch = getchar();
	for (;i<count;i++)
	{
		str[i] = ch;
		if (str[i] == '\n') {
			break;
		}
		if (i == count-1 && str[i] != '\n') {
			flag = 1;
			break;
		}

		ch = getchar();
	}
	str[i] = '\0';
	if (flag) {
		while (getchar() != '\n')	;
	}		//如果输入超过限定的长度，则将后面的字符吸收掉
}
void clear_User_list(User_list *head)
{
	User_list *p;

	while (head)
	{
		p = head;
		head = head->next;
		free(p);
	}
}
void clear_Data_list(Data_list *head)
{
	
	Data_list *p;

	while (head)
	{
		p = head;
		head = head->next;
		free(p);
	}
}
int read_data_to_list(const char username[],Data_list *head)
{
	FILE *fp;
	long filelength = 0;
	int times = 0;
	Data_list *p = head->next,*prev = head;
	char filename[22] = ".";

	strcat(filename,username);
	fp = fopen(filename,"r");
	if (fp == NULL) {
		return 0;
	}
	fseek(fp,0L,SEEK_END);
	filelength = ftell(fp);
	rewind(fp);
	times = filelength / sizeof(Data_list);
	if (!times) {
		return 0;
	}
	while (times--)
	{
		p = (Data_list *)malloc(sizeof(Data_list));
		fread(p,sizeof(Data_list),1,fp);
		p->next = prev->next;
		prev->next = p;
		prev = p;
	}

	fclose(fp);

	return 1;
}
int read_user_to_list(User_list *head)
{
	FILE *fp;
	long filelength = 0;
	int times = 0;
	User_list *p = head->next,*prev = head;

	fp = fopen(".user","r");
	if (fp == NULL) {
		return 0;
	}
	fseek(fp,0L,SEEK_END);
	filelength = ftell(fp);
	rewind(fp);
	times = filelength / sizeof(User_list);
	if (!times) {
		return 0;
	}
	while (times--)
	{
		p = (User_list *)malloc(sizeof(User_list));
		fread(p,sizeof(User_list),1,fp);
		p->next = prev->next;
		prev->next = p;
		prev = p;
	}

	fclose(fp);

	return 1;

}
void print_in_out(Data_list *node)
{
	switch(node->in_out)
	{
		case in:
			printf("收入");
			break;
		case out:
			printf("支出");
			break;
	}
}
void print_type(Data_list *node)
{
	switch(node->type)
	{
		case clothes:
			printf("衣服饰品");
			break;
		case food:
			printf("食品水酒");
			break;
		case home:
			printf("居家物业");
			break;
		case traffic:
			printf("行车交通");
			break;
		case communication:
			printf("交流通讯");
			break;
		case entertainment:
			printf("休闲娱乐");
			break;
		case study:
			printf("学习进修");
			break;
		case social:
			printf("人情往来");
			break;
		case medical:
			printf("医疗保险");
			break;
		case other_out:
			printf("其他杂项");
			break;
		case occupation:
			printf("职业收入");
			break;
		case other_in:
			printf("其他收入");
			break;
	}
}
enum In_out select_in_out()
{
	int sel;

	while (1)
	{
		printf("请选择是(1)支出还是(2)收入(3)退出:\n");
		scanf("%d",&sel);
		while (getchar() != '\n')	;
		switch(sel)
		{
			case 1:
				return out;
			case 2:
				return in;
			case 3:
				return back;
			default:
				sel = 0;
				break;
		}
	}
}
enum Type select_type(enum In_out tmp)
{
	int sel;

	while (1)
	{
		printf("请选择类别:\n");
		switch(tmp)
		{
			case out:
				printf("(01)衣服饰品\t(02)食品水酒\t(03)居家物业\n");
				printf("(04)行车交通\t(05)交流通讯\t(06)休闲娱乐\n");
				printf("(07)学习进修\t(08)人情往来\t(09)医疗保险\n");
				printf("(10)其他杂项\t(11)退出\n");
				break;
			case in:
				printf("(1)职业收入\t(2)其他收入\t(3)退出\n");
				break;
			default:
				printf("收入/支出类型出错.\n");
				break;
		}
		scanf("%d",&sel);
		while (getchar() != '\n')	;
		switch(tmp)
		{
			case out:
				switch(sel)
				{
					case  1:
						return clothes;
					case  2:
						return food;
					case  3:
						return home;
					case  4:
						return traffic;
					case  5:
						return communication;
					case  6:
						return entertainment;
					case  7:
						return study;
					case  8:
						return social;
					case  9:
						return medical;
					case 10:
						return other_in;
					case 11:
						return back;
					default:
						sel = 0;
						break;
				}
				break;
			case in:
				switch(sel)
				{
					case 1:
						return occupation;
					case 2:
						return other_out;
					case 3:
						return back;
					default:
						sel = 0;
						break;
				}
				break;
			default:
				break;
		}
	}
}
void lock(char str[])
{
	while (*str)
	{
		*str += 30;
		str++;
	}
}
void data_sort(const char username[])
{
	Data_list *head = creat_Data_list(),*p;
	int sel = 0,count = 0;

	if (!read_data_to_list(username,head)) {
		clear_Data_list(head);
		printf("还没任何账本数据!程序将返回.\n");
		printf("按回车继续.\n");
		while (getchar() != '\n')	;
		return ;
	}
	p = head->next;
	while (p)
	{
		count++;
		p = p->next;
	}
	while (1)
	{
		printf("请选择:\n");
		printf("(1)按时间升序排序\t(2)按时间降序排序\n");
		printf("(3)按钱数升序排序\t(4)按钱数降序排序\n");
		printf("(5)退出\n");
		scanf("%d",&sel);
		while (getchar() != '\n')	;
		switch (sel)
		{
			case 1:
				timesort(head,count,1);
				break;
			case 2:
				timesort(head,count,0);
				break;
			case 3:
				moneysort(head,count,1);
				break;
			case 4:
				moneysort(head,count,0);
				break;
			case 5:
				clear_Data_list(head);
				return ;
			default:
				break;
		}
		if (sel >= 1 && sel <= 4) {
			break;
		}
	}
	write_data_to_file(username,head);
	printf("排序成功!程序将返回.\n");
	printf("按回车继续.\n");
	while (getchar() != '\n')		;
}
int timecmp(char s1[],char s2[])
{
	int year1 = 0,month1 = 0,mday1 = 0,hour1 = 0,minutes1 = 0,sec1 = 0;
	int year2 = 0,month2 = 0,mday2 = 0,hour2 = 0,minutes2 = 0,sec2 = 0;
	int i = 1;
	char smonth[4],syear[5],smday[3],shour[3],sminutes[3],ssec[3],*p;
	char str1[26],str2[26];

	strcpy(str1,s1);
	strcpy(str2,s2);

	p = str1;
	strsep(&p," ");
	strcpy(smonth,strsep(&p," "));
	strcpy(smday,strsep(&p," "));
	strcpy(shour,strsep(&p,":"));
	strcpy(sminutes,strsep(&p,":"));
	strcpy(ssec,strsep(&p," "));
	strcpy(syear,strsep(&p,"\0"));

	year1 = atoi(syear);
	month1 = monstr_to_int(smonth);
	mday1 = atoi(smday);
	hour1 = atoi(shour);
	minutes1 = atoi(sminutes);
	sec1 = atoi(ssec);
	
	p =str2;
	strsep(&p," ");
	strcpy(smonth,strsep(&p," "));
	strcpy(smday,strsep(&p," "));
	strcpy(shour,strsep(&p,":"));
	strcpy(sminutes,strsep(&p,":"));
	strcpy(ssec,strsep(&p," "));
	strcpy(syear,strsep(&p,"\0"));

	year2 = atoi(syear);
	month2 = monstr_to_int(smonth);
	mday2 = atoi(smday);
	hour2 = atoi(shour);
	minutes2 = atoi(sminutes);
	sec2 = atoi(ssec);
	
	if (year1 < year2) {
		return 1;
	}
	else if (year1 > year2) {
		return -1;
	}
	
	if (month1 < month2) {
		return 1;
	}
	else if (month1 > month2) {
		return -1;
	}

	if (mday1 < mday2) {
		return 1;
	}
	else if (mday1 >mday2) {
		return -1;
	}

	if (hour1 < hour2) {
		return 1;
	}
	else if (hour1 > hour2) {
		return -1;
	}

	if (minutes1 < minutes2) {
		return 1;
	}
	else if (minutes1 > minutes2) {
		return -1;
	}
	if (sec1 < sec2) {
		return 1;
	}
	else if (sec1 >sec2) {
		return -1;
	}

	return 0;
}
int monstr_to_int(char str[])
{
	if (!strcmp(str,"Jan")) {
		return 1;
	}
	else if (!strcmp(str,"Feb")) {
		return 2;
	}
	else if (!strcmp(str,"Mar")) {
		return 3;
	}
	else if (!strcmp(str,"Apr")) {
		return 4;
	}
	else if (!strcmp(str,"May")) {
		return 5;
	}
	else if (!strcmp(str,"Jun")) {
		return 6;
	}
	else if (!strcmp(str,"Jul")) {
		return 7;
	}
	else if (!strcmp(str,"Aug")) {
		return 8;
	}
	else if (!strcmp(str,"Sep")) {
		return 9;
	}
	else if (!strcmp(str,"Oct")) {
		return 10;
	}
	else if (!strcmp(str,"Nov")) {
		return 11;
	}
	else if (!strcmp(str,"Dec")) {
		return 12;
	}
	else {
		printf("月份错误!\n");
		return 0;
	}
}
int write_data_to_file(const char username[],Data_list *head)
{
	char filename[22] = ".";
	FILE *fp;
	Data_list *p = head->next;
	int count = 0;

	strcat(filename,username);
	fp =fopen(filename,"w");

	while (p)
	{
		fwrite(p,sizeof(Data_list),1,fp);
		count++;
		p = p->next;
	}
	fclose(fp);
	return count;
}
int write_user_to_file(User_list *head)
{
	FILE *fp;
	User_list *p = head->next;
	int count = 0;
	
	fp = fopen(".user","w");
	while (p)
	{
		fwrite(p,sizeof(User_list),1,fp);
		count++;
		p = p->next;
	}
	fclose(fp);
	return count;
}
int intcmp(const void *a,const void *b)
{
	return (*(int *)a) - (*(int *)b);
}
void moneysort(Data_list *head,int count,int type)
{
	int i,j,flag = 0;	//flag来区分是否需要交换数据
	Data_list *p = head->next,t,*q;

	for (i = 1;i <= count-1;i++)
	{
		p = head->next;
		for (j = 0;j < count - i;j++)
		{
			flag = 0;
			if (type) {
				if (p->money > p->next->money) {
					flag = 1;
				}
			}
			else {
				if (p->money < p->next->money) {
					flag = 1;
				}
			}
			if (flag) {
				q = p->next;

				t = *(p->next);
				*(p ->next) = *p;
				*p = t;
					
				q->next = p->next;
				p->next = q;
			}
			p =p->next;
		}
	}
}
void timesort(Data_list *head,int count,int type)
{
	int i,j,flag = 0;	//flag来区分是否需要交换数据
	Data_list *p = head->next,t,*q;

	for (i = 1;i <= count-1;i++)
	{
		p = head->next;
		for (j = 0;j < count - i;j++)
		{
			flag = 0;
			if (type) {
				if (timecmp(p->time,p->next->time) == -1) {
					flag = 1;
				}
			}
			else {
				if (timecmp(p->time,p->next->time) == 1) {
					flag = 1;
				}
			}
			if (flag) {
				q = p->next;

				t = *(p->next);
				*(p ->next) = *p;
				*p = t;
					
				q->next = p->next;
				p->next = q;
			}
			p =p->next;
		}
	}
}
void set_time(int year,int month,int mday)
{
	time_t t;
	struct tm *timep;
	
	time(&t);
	timep = localtime(&t);		//这一步是因为tm结构体中的其他成员我想让系统给它赋值
	timep->tm_year = year - 1900;
	timep->tm_mon = month - 1;
	timep->tm_mday = mday;
	timep->tm_hour = 0;
	timep->tm_min = 0;
	timep->tm_sec = 0;
	
	t = mktime(timep);
	stime(&t);
}
void input_time(char str1[],char str2[])
{
	int year1,month1,mday1;
	int year2,month2,mday2;
	int *p1 = &year1,*p2 = &month1,*p3 = &mday1, i = 1;
	time_t nowtime,t;

	printf("请输入起始日期:\n");
	for (i = 1;i <= 2;i++)
	{
		printf("年份:");
		while (1)
		{
			if (scanf("%d",p1)) {
				break;
			}
				while (getchar() != '\n')	;
		}
		printf("月份:");
		while (1)
		{
			while (getchar() != '\n')	;
			if (scanf("%d",p2) && *p2 >= 1 && *p2 <= 12) {
				break;
			}
		}
		printf("日期:");
		while (1)
		{
			while (getchar() != '\n')	;
			if (scanf("%d",p3) && *p3 >= 1 && *p3 <= 31) {
				break;
			}
		}
		while (getchar() != '\n')	;
		time(&nowtime);
		if (i == 1) {
			set_time(*p1,*p2,*p3);
		}
		else {
			set_time(*p1,*p2,*p3+1);
		}
		time(&t);
		if (i == 1) {
			strcpy(str1,ctime(&t));
		}
		else {
			strcpy(str2,ctime(&t));
		}
		stime(&nowtime);
		if (i == 1) {
			printf("请输入截止日期:\n");
		}
		p1 = &year2;
		
		p2 = &month2;
		p3 = &mday2;
	}
}
