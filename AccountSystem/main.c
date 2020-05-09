#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS
#include "billing_file.h"
#include "global.h"
#include"menu.h"
int main() {
	initCardList();
	int nSelection = -1;
	do
	{
		outputMenu();//显示菜单
		scanf("%d", &nSelection);
		switch (nSelection)
		{
		case 1:
			add();//添加卡
			break;
		case 2:
			query();//查询卡
			break;
		case 3:
			logon();//上机
			break;
		case 4:
			settle();//下机
			break;
		case 5:
			addMoney();//充值
			break;
		case 6:
			refundMoney();//退款
			break;
		case 7: {
			statistics();//查询统计
			break;
		}
		case 8:
			annul();//注销卡
			break;
		case 0:
			exitApp();//退出app
			break;
		default:
			printf("输入的菜单编号有误");
			break;
		}
		printf("\n");
	} while (nSelection!=0);
	return 0;
}
