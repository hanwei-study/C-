#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS
#include"menu.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"model.h"
#include"service.h"
#include"global.h"
#include"tool.h"
#include"card_service.h"
void outputMenu() {
	//输出系统菜单
	printf("----------菜单----------\n");
	printf("1、添加卡\n");
	printf("2、查询卡\n");
	printf("3、上机\n");
	printf("4、下机\n");
	printf("5、充值\n");
	printf("6、退费\n");
	printf("7、查询统计\n");
	printf("8、注销卡\n");
	printf("0、退出\n");
	printf("请选择菜单项编号（0~7）：\n");
}

void add()
{
	printf("---添加卡---\n");
	Card card;
	char aName[32] = {0};
	printf("请输入要添加的卡号\n");
	scanf("%s", &aName);
	while (strlen(aName) > 18 ) {
		printf("卡号过长，请重新输入\n");
		scanf("%s", &aName);
	}
	strncpy(card.aName, aName, strlen(aName)+1);//把\0也拷贝进入，避免输出字符串出现空指针
	char aPwd[32] = { 0 };
	printf("请输入卡的密码\n");
	scanf("%s", &aPwd);
	while (strlen(aPwd) > 8) {
		printf("密码过长，请重新输入\n");
		scanf("%s", &aPwd);
	}
	strncpy(card.aPwd, aPwd, strlen(aPwd)+1);//把\0也拷贝进入，避免输出字符串出现空指针
	printf("请输入充值金额\n");
	scanf("%f", &card.fBalance);
	card.fTotalUse = card.fBalance;
	card.nDel = 0;
	card.nUseCount = 0;
	card.nStatus = 0;
	card.tStart = card.tEnd = card.tLast = time(NULL);
	struct tm* startTime = localtime(&card.tStart);
	struct tm* endTime = localtime(&card.tEnd);
	endTime->tm_year = startTime->tm_year + 1;
	card.tEnd = mktime(endTime);
	printf("卡号\t密码\t状态\t余额\n");
	printf("%s\t%s\t%d\t%0.1f\n", 
		card.aName, card.aPwd, card.nStatus, card.fBalance);
	addCardInfo(card);//添加卡信息到文件
}

void query()
{
	char aName[32] = { 0 };
	printf("请输入要查找的卡号\n");
	scanf("%s", &aName);
	while (strlen(aName) > 18) {
		printf("输入有误，请重新输入\n");
		scanf("%s", &aName);
	}
	int length = 0;
	Card* card=queryCardsInfo(aName,&length);
	if (length == 0) {
		printf("没有此卡的信息\n");
		return;
	}
	printf("卡号\t状态\t余额\t累计使用\t使用次数\t上次使用时间\n");
	int i;
	for (i = 0; i < length; i++) {
		char a[21];
		timeToString(card[i].tLast, a);
		printf("%s\t%d\t%0.1f\t%0.1f\t\t%d\t\t%s",
			card[i].aName,card[i].nStatus,card[i].fBalance,card[i].fTotalUse,card[i].nUseCount,a);
		printf("\n");
	}
}

void logon()
{
	int result;
	char aName[32] = { 0 };
	printf("请输入卡号\n");
	scanf("%s", &aName);
	while (strlen(aName) > 18) {
		printf("卡号过长，请重新输入\n");
		scanf("%s", &aName);
	}
	char aPwd[32] = { 0 };
	printf("请输入密码\n");
	scanf("%s", &aPwd);
	while (strlen(aPwd) > 8) {
		printf("密码过长，请重新输入\n");
		scanf("%s", &aPwd);
	}
	LogonInfo logoinfo;
	result = doLogon(aName, aPwd,&logoinfo);
	switch (result)
	{
	case FALSE:
		printf("上机失败\n");
		break;
	case TRUE:
		printf("上机成功\n");
		printf("卡号\t余额\t登录时间\n");
		char a[21];
		timeToString(logoinfo.tLogon, a);
		printf("%s\t%.1f\t%s\n", 
			logoinfo.aCardName, logoinfo.fBalance, a);
		break;
	case ONUSE:
		printf("该卡正在被使用\n");
		break;
	case UNENOUUGHMONEY:
		printf("该卡余额不足，无法使用\n");
		break;
	}
}

void settle()
{
	int result;
	char aName[32] = { 0 };
	printf("请输入卡号\n");
	scanf("%s", &aName);
	while (strlen(aName) > 18) {
		printf("卡号过长，请重新输入\n");
		scanf("%s", &aName);
	}
	char aPwd[32] = { 0 };
	printf("请输入密码\n");
	scanf("%s", &aPwd);
	while (strlen(aPwd) > 8) {
		printf("密码过长，请重新输入\n");
		scanf("%s", &aPwd);
	}
	SettleInfo settleInof;
	result = doSettle(aName, aPwd, &settleInof);
	switch (result)
	{
	case TRUE: 
	{
		char a[21];
		timeToString(settleInof.tStart, a);
		char b[21];
		timeToString(settleInof.tEnd , b);
		printf("下机成功\n");
		printf("卡号\t消费\t余额\t上机时间\t\t下机时间\n");
		printf("%s\t%.1f\t%.1f\t%s\t%s\n",
			settleInof.aCardName, settleInof.fAmount, settleInof.fBalance, a, b);
		break;
	}
	case FALSE:
		printf("下机失败，请检查卡状态或卡信息输入是否有误");
		break;
	case UNENOUUGHMONEY:
		printf("余额不足");
	}
}

void addMoney()
{
	int result;
	char aName[32] = { 0 };
	printf("请输入卡号\n");
	scanf("%s", &aName);
	while (strlen(aName) > 18) {
		printf("卡号过长，请重新输入\n");
		scanf("%s", &aName);
	}
	char aPwd[32] = { 0 };
	printf("请输入密码\n");
	scanf("%s", &aPwd);
	while (strlen(aPwd) > 8) {
		printf("密码过长，请重新输入\n");
		scanf("%s", &aPwd);
	}
	float total=0;
	printf("请输入充值金额\n");
	scanf("%f", &total);
	MoneyInfo money;
	money.fMoney = total;
	result=doAddMoney(aName, aPwd, &money);
	switch (result)
	{
	case TRUE:{
		printf("卡号\t充值金额\t余额\n");
		printf("%s\t%.1f\t\t%.1f\n",
			money.aCardName,money.fMoney,money.fBalance);
		break;
	}
	case FALSE:
		printf("充值失败\n"); 
		break;
	}
}

void refundMoney()
{
	int result;
	char aName[32] = { 0 };
	printf("请输入卡号\n");
	scanf("%s", &aName);
	while (strlen(aName) > 18) {
		printf("卡号过长，请重新输入\n");
		scanf("%s", &aName);
	}
	char aPwd[32] = { 0 };
	printf("请输入密码\n");
	scanf("%s", &aPwd);
	while (strlen(aPwd) > 8) {
		printf("密码过长，请重新输入\n");
		scanf("%s", &aPwd);
	}
	MoneyInfo money;
	result = doRefundMoney(aName, aPwd, &money);
	switch (result)
	{
	case TRUE: {
		printf("退费成功\n");
		printf("卡号\t退费金额\t余额\n");
		printf("%s\t%.1f\t\t%.1f\n",
			money.aCardName, money.fMoney, money.fBalance);
		break;
	}
	case FALSE:
		printf("退费失败\n");
		break;
	
	case UNENOUUGHMONEY:
		printf("余额不足\n");
		break;
	}
}

void annul()
{
	char aName[32] = { 0 };
	printf("请输入卡号\n");
	scanf("%s", &aName);
	while (strlen(aName) > 18) {
		printf("卡号过长，请重新输入\n");
		scanf("%s", &aName);
	}
	char aPwd[32] = { 0 };
	printf("请输入密码\n");
	scanf("%s", &aPwd);
	while (strlen(aPwd) > 8) {
		printf("密码过长，请重新输入\n");
		scanf("%s", &aPwd);
	}
	Card card;
	strcpy(card.aName, aName);
	strcpy(card.aPwd, aPwd);
	int resutl = annulCard(&card);
	switch (resutl)
	{
	case FALSE:
		printf("注销失败\n");
		break;
	case TRUE: 
		printf("注销成功\n");
		break;
	case UNENOUUGHMONEY:
		printf("余额不足，请先充值");
		break;
	}
}

void exitApp()
{	
	release();
}
void statistics() {
	printf("----------查询统计菜单----------\n");
	printf("1、总营业额统计\n");
	printf("2、全体用户总余额\n");
	printf("3、总用户数量\n");
	int result = 0;
	scanf("%d", &result);
	switch (result)
	{
		case 1:
			total();
			break;
		case 2:
			totalBalance();
			break;
		case 3:
			activeCard();
			break;
	}
}