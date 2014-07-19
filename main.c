/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2014年06月13日 21时12分17秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  geekgao, gaopu333@gmail.com
 *        Company:  Class 1302 of Software Engineer
 *
 * =====================================================================================
 */

#include <stdio.h>
#include "finance.h"
#include <stdlib.h>
int main()
{
	char username[21];
	int sel = 0;

LOOP:
	login(username);
	
	//登陆成功到达下面的界面
	while (1)
	{
		system("clear");
		printf("%s,你好:\n",username);
		menu2();
		sel = 0;
		scanf("%d",&sel);
		while (getchar() != '\n')	;
		system("clear");
		switch(sel)
		{
			case 1:
				add(username);
				break;
			case 2:
				look(username);
				break;
			case 3:
				edit(username);
				break;
			case 4:
				data_sort(username);
				break;
			case 5:
				export(username);
				break;
			case 6:
				if (change_password(username)) {
					goto LOOP;
				}
				break;
			case 7:
				if (del_user(username)) {
					goto LOOP;
				}
				break;
			case 8:
				exit(0);
				break;
			default:
				break;
		}
	}

	return 0;
}






