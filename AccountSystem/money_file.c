#include"money_file.h"
#include"global.h"
#include<stdio.h>
int saveMoney(const Money * pMoney, const char * pPath)
{
	FILE* fp = NULL;
	//打开二进制文件
	fp = fopen(pPath, "ab+");
	if (fp == NULL) {
		return FALSE;
	}
	//将数据写进二进制文件
	fwrite(pMoney, sizeof(Money), 1, fp);
	//关闭文件
	fclose(fp);
	return TRUE;
}
