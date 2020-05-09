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
	//���ϵͳ�˵�
	printf("----------�˵�----------\n");
	printf("1����ӿ�\n");
	printf("2����ѯ��\n");
	printf("3���ϻ�\n");
	printf("4���»�\n");
	printf("5����ֵ\n");
	printf("6���˷�\n");
	printf("7����ѯͳ��\n");
	printf("8��ע����\n");
	printf("0���˳�\n");
	printf("��ѡ��˵����ţ�0~7����\n");
}

void add()
{
	printf("---��ӿ�---\n");
	Card card;
	char aName[32] = {0};
	printf("������Ҫ��ӵĿ���\n");
	scanf("%s", &aName);
	while (strlen(aName) > 18 ) {
		printf("���Ź���������������\n");
		scanf("%s", &aName);
	}
	strncpy(card.aName, aName, strlen(aName)+1);//��\0Ҳ�������룬��������ַ������ֿ�ָ��
	char aPwd[32] = { 0 };
	printf("�����뿨������\n");
	scanf("%s", &aPwd);
	while (strlen(aPwd) > 8) {
		printf("�������������������\n");
		scanf("%s", &aPwd);
	}
	strncpy(card.aPwd, aPwd, strlen(aPwd)+1);//��\0Ҳ�������룬��������ַ������ֿ�ָ��
	printf("�������ֵ���\n");
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
	printf("����\t����\t״̬\t���\n");
	printf("%s\t%s\t%d\t%0.1f\n", 
		card.aName, card.aPwd, card.nStatus, card.fBalance);
	addCardInfo(card);//��ӿ���Ϣ���ļ�
}

void query()
{
	char aName[32] = { 0 };
	printf("������Ҫ���ҵĿ���\n");
	scanf("%s", &aName);
	while (strlen(aName) > 18) {
		printf("������������������\n");
		scanf("%s", &aName);
	}
	int length = 0;
	Card* card=queryCardsInfo(aName,&length);
	if (length == 0) {
		printf("û�д˿�����Ϣ\n");
		return;
	}
	printf("����\t״̬\t���\t�ۼ�ʹ��\tʹ�ô���\t�ϴ�ʹ��ʱ��\n");
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
	printf("�����뿨��\n");
	scanf("%s", &aName);
	while (strlen(aName) > 18) {
		printf("���Ź���������������\n");
		scanf("%s", &aName);
	}
	char aPwd[32] = { 0 };
	printf("����������\n");
	scanf("%s", &aPwd);
	while (strlen(aPwd) > 8) {
		printf("�������������������\n");
		scanf("%s", &aPwd);
	}
	LogonInfo logoinfo;
	result = doLogon(aName, aPwd,&logoinfo);
	switch (result)
	{
	case FALSE:
		printf("�ϻ�ʧ��\n");
		break;
	case TRUE:
		printf("�ϻ��ɹ�\n");
		printf("����\t���\t��¼ʱ��\n");
		char a[21];
		timeToString(logoinfo.tLogon, a);
		printf("%s\t%.1f\t%s\n", 
			logoinfo.aCardName, logoinfo.fBalance, a);
		break;
	case ONUSE:
		printf("�ÿ����ڱ�ʹ��\n");
		break;
	case UNENOUUGHMONEY:
		printf("�ÿ����㣬�޷�ʹ��\n");
		break;
	}
}

void settle()
{
	int result;
	char aName[32] = { 0 };
	printf("�����뿨��\n");
	scanf("%s", &aName);
	while (strlen(aName) > 18) {
		printf("���Ź���������������\n");
		scanf("%s", &aName);
	}
	char aPwd[32] = { 0 };
	printf("����������\n");
	scanf("%s", &aPwd);
	while (strlen(aPwd) > 8) {
		printf("�������������������\n");
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
		printf("�»��ɹ�\n");
		printf("����\t����\t���\t�ϻ�ʱ��\t\t�»�ʱ��\n");
		printf("%s\t%.1f\t%.1f\t%s\t%s\n",
			settleInof.aCardName, settleInof.fAmount, settleInof.fBalance, a, b);
		break;
	}
	case FALSE:
		printf("�»�ʧ�ܣ����鿨״̬����Ϣ�����Ƿ�����");
		break;
	case UNENOUUGHMONEY:
		printf("����");
	}
}

void addMoney()
{
	int result;
	char aName[32] = { 0 };
	printf("�����뿨��\n");
	scanf("%s", &aName);
	while (strlen(aName) > 18) {
		printf("���Ź���������������\n");
		scanf("%s", &aName);
	}
	char aPwd[32] = { 0 };
	printf("����������\n");
	scanf("%s", &aPwd);
	while (strlen(aPwd) > 8) {
		printf("�������������������\n");
		scanf("%s", &aPwd);
	}
	float total=0;
	printf("�������ֵ���\n");
	scanf("%f", &total);
	MoneyInfo money;
	money.fMoney = total;
	result=doAddMoney(aName, aPwd, &money);
	switch (result)
	{
	case TRUE:{
		printf("����\t��ֵ���\t���\n");
		printf("%s\t%.1f\t\t%.1f\n",
			money.aCardName,money.fMoney,money.fBalance);
		break;
	}
	case FALSE:
		printf("��ֵʧ��\n"); 
		break;
	}
}

void refundMoney()
{
	int result;
	char aName[32] = { 0 };
	printf("�����뿨��\n");
	scanf("%s", &aName);
	while (strlen(aName) > 18) {
		printf("���Ź���������������\n");
		scanf("%s", &aName);
	}
	char aPwd[32] = { 0 };
	printf("����������\n");
	scanf("%s", &aPwd);
	while (strlen(aPwd) > 8) {
		printf("�������������������\n");
		scanf("%s", &aPwd);
	}
	MoneyInfo money;
	result = doRefundMoney(aName, aPwd, &money);
	switch (result)
	{
	case TRUE: {
		printf("�˷ѳɹ�\n");
		printf("����\t�˷ѽ��\t���\n");
		printf("%s\t%.1f\t\t%.1f\n",
			money.aCardName, money.fMoney, money.fBalance);
		break;
	}
	case FALSE:
		printf("�˷�ʧ��\n");
		break;
	
	case UNENOUUGHMONEY:
		printf("����\n");
		break;
	}
}

void annul()
{
	char aName[32] = { 0 };
	printf("�����뿨��\n");
	scanf("%s", &aName);
	while (strlen(aName) > 18) {
		printf("���Ź���������������\n");
		scanf("%s", &aName);
	}
	char aPwd[32] = { 0 };
	printf("����������\n");
	scanf("%s", &aPwd);
	while (strlen(aPwd) > 8) {
		printf("�������������������\n");
		scanf("%s", &aPwd);
	}
	Card card;
	strcpy(card.aName, aName);
	strcpy(card.aPwd, aPwd);
	int resutl = annulCard(&card);
	switch (resutl)
	{
	case FALSE:
		printf("ע��ʧ��\n");
		break;
	case TRUE: 
		printf("ע���ɹ�\n");
		break;
	case UNENOUUGHMONEY:
		printf("���㣬���ȳ�ֵ");
		break;
	}
}

void exitApp()
{	
	release();
}
void statistics() {
	printf("----------��ѯͳ�Ʋ˵�----------\n");
	printf("1����Ӫҵ��ͳ��\n");
	printf("2��ȫ���û������\n");
	printf("3�����û�����\n");
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