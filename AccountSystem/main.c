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
		outputMenu();//��ʾ�˵�
		scanf("%d", &nSelection);
		switch (nSelection)
		{
		case 1:
			add();//��ӿ�
			break;
		case 2:
			query();//��ѯ��
			break;
		case 3:
			logon();//�ϻ�
			break;
		case 4:
			settle();//�»�
			break;
		case 5:
			addMoney();//��ֵ
			break;
		case 6:
			refundMoney();//�˿�
			break;
		case 7: {
			statistics();//��ѯͳ��
			break;
		}
		case 8:
			annul();//ע����
			break;
		case 0:
			exitApp();//�˳�app
			break;
		default:
			printf("����Ĳ˵��������");
			break;
		}
		printf("\n");
	} while (nSelection!=0);
	return 0;
}
