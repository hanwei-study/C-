#include "card_file.h"
#define CARDCHARNUM 256

int saveCard2(const Card * pCard, const char * pPath)
{
	FILE* fp = NULL;
	//打开二进制文件
	fp = fopen(pPath, "ab+");
	if (fp == NULL) {
		return FALSE;
	}
	//将数据写进二进制文件
	fwrite(pCard, sizeof(Card), 1, fp);
	//关闭文件
	fclose(fp);
	return TRUE;
}

int readCard2(Card * pCard, const char * pPath)
{
	FILE *fp = NULL;
	if ((fp = fopen(pPath, "r")) == NULL) {
		return FALSE;
	}
	int i = 0;
	while (!feof(fp)) {
		fread(&pCard[i], sizeof(Card), 1, fp);
		i++;
	}
	fclose(fp);
	return TRUE;
}

int getCardCount2(const char * pPath)
{
	int nCount = 0;
	FILE *fp = NULL;
	if ((fp = fopen(pPath, "r")) == NULL) {
		return nCount;
	}
	Card card;
	while (!feof(fp)) {
		fread(&card, sizeof(Card), 1, fp);
		nCount++;
	}
	fclose(fp);
	return nCount-1;
}


int updateCard2(const Card * pCard, const char * pPath, int nIndex)
{
	FILE *fp = NULL;
	//读写方式打开文件
	fp = fopen(pPath, "rb+");
	if (fp == NULL) {
		return FALSE;
	}
	int count = 0;
	Card billing;
	long lPosition = 0;
	fseek(fp, lPosition, 0);
	//遍历文件，找到记录
	while (!feof(fp) && (count < nIndex))
	{
		fread(&billing, sizeof(Card), 1, fp);
		count++;
		lPosition = ftell(fp);
	}
	fseek(fp, lPosition, 0);
	fwrite(pCard, sizeof(Card), 1, fp);
	fclose(fp);
	return TRUE;
}
