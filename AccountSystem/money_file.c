#include"money_file.h"
#include"global.h"
#include<stdio.h>
int saveMoney(const Money * pMoney, const char * pPath)
{
	FILE* fp = NULL;
	//�򿪶������ļ�
	fp = fopen(pPath, "ab+");
	if (fp == NULL) {
		return FALSE;
	}
	//������д���������ļ�
	fwrite(pMoney, sizeof(Money), 1, fp);
	//�ر��ļ�
	fclose(fp);
	return TRUE;
}
