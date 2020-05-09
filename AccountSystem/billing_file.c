#include "billing_file.h"
#include <stdio.h>
#include"global.h"
//将计费信息保存到文件中
int saveBilling(const Billing * pBilling, const char * pPath)
{
	FILE* fp = NULL;
	//打开二进制文件
	fp = fopen(pPath, "ab+");
	if (fp == NULL) {
		return FALSE;
	}
	//将数据写进二进制文件
	fwrite(pBilling,sizeof(Billing), 1, fp);
	//关闭文件
	fclose(fp);
	return TRUE;
}

int readBilling(Billing * pBilling, const char * pPath)
{
	FILE *fp = NULL;
	if ((fp = fopen(pPath, "r")) == NULL) {
		return FALSE;
	}
	int i = 0;
	while (!feof(fp)) {
		fread(&pBilling[i], sizeof(Billing), 1, fp);
		i++;
	}
	fclose(fp);
	return TRUE;
}

int getBillingCount(const char * pPath)
{	
	int nCount = 0;
	FILE *fp = NULL;
	if ((fp = fopen(pPath, "r")) == NULL) {
		return nCount;
	}
	Billing b;
	while (!feof(fp)) {
		fread(&b, sizeof(Billing), 1, fp);
		nCount++;
	}
	fclose(fp);
	return nCount;
}

int updateBilling(Billing * pBilling, char * pPath, int index)
{
	FILE *fp = NULL;
	//读写方式打开文件
	fp = fopen(pPath, "rb+");
	if (fp == NULL) {
		return FALSE;
	}
	int count = 0 ;
	Billing billing;
	long lPosition = 0;
	fseek(fp,lPosition, 0);
	//遍历文件，找到记录
	while (!feof(fp)&&(count<index))
	{
		fread(&billing, sizeof(Billing), 1, fp);
		count++;
		lPosition = ftell(fp);
	}
	fseek(fp, lPosition, 0);
	fwrite(pBilling, sizeof(Billing), 1, fp);
	fclose(fp);
	return TRUE;
}
