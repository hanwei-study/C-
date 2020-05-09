#include"service.h"
#include"card_service.h"
#include"billing_service.h"
#include"billing_file.h"
#include"money_file.h"
#include"card_file.h"
#include<stdlib.h>
#include<string.h>
int addCardInfo(Card card) {
	return addCard(card);
}

Card * queryCardsInfo(const char * pName, int * index)
{
	return queryCards(pName, index);
}

int doLogon(const char * pName, const char * pPwd, LogonInfo* pLogonInfo)
{	
	Billing billing;
	//�жϿ��������Ƿ���ȷ
	int index = 0;
	Card* card=checkCard(pName, pPwd,&index);
	if (card == NULL) {
		return FALSE;
	}
	//�ж��ܷ��ϻ�
	if (card->nStatus != 0) {
		return ONUSE;//�Ѿ��ϻ�
	}
	if (card->fBalance <= 0) {
		return UNENOUUGHMONEY;//����
	}
	//���¿���Ϣ
	card->nStatus = 1;
	card->nUseCount++;
	card->tLast = time(NULL);
	updateCard2(card, CARDPATH, index);
	//�������
	strcpy(billing.aCardName, pName);
	billing.fAmount = 0;
	billing.nStatus = 0;
	billing.nDel = 0;
	billing.tStart = time(NULL);
	billing.tEnd = time(NULL);
	//��ӵ�¼��Ϣ
	strcpy(pLogonInfo->aCardName, card->aName);
	pLogonInfo->fBalance = card->fBalance;
	pLogonInfo->tLogon = time(NULL);
	return addBilling(billing);
}

void release()
{
	releaseCardList();
	releaseBillingList();
}

int doSettle(const char * pName, const char * pPwd, SettleInfo * pSettleInfo)
{
	//�жϿ��������Ƿ���ȷ
	int index = 0;
	Card* card = checkCard(pName, pPwd, &index);
	if (card == NULL) {
		return FALSE;
	}
	//�ж��ܷ��»�
	if (card->nStatus != 1) {
		return FALSE;//û���ϻ�
	}
	Billing* pBilling = NULL;
	int positon=0;
	pBilling = queryBilling(pName, &positon);
	if (pBilling == NULL) {
		return FALSE;
	}
	float dbAmount = getAmount(pBilling->tStart);
	float fBalance = card->fBalance - dbAmount;
	if (fBalance < 0) {
		return UNENOUUGHMONEY;
	}
	//���¿���Ϣ
	card->fBalance = fBalance;
	card->nStatus = 0;
	card->tLast = time(NULL);
	updateCard2(card, CARDPATH, index);
	//����������Ϣ
	pBilling->fAmount = fBalance;
	pBilling->nStatus = 1;
	pBilling->nDel = 1;
	pBilling->tEnd = time(NULL);
	updateBilling(pBilling, BILLINGPATH, positon);
	//��¼�»���Ϣ
	strcpy(pSettleInfo->aCardName, pName);
	pSettleInfo->fAmount = dbAmount;
	pSettleInfo->fBalance = fBalance;
	pSettleInfo->tEnd = time(NULL);
	pSettleInfo->tStart = pBilling->tStart;
	return TRUE;
}

float getAmount(time_t tStart)
{
	time_t tEnd = time(NULL);
	int nSec=time(NULL) - tStart;
	int nMinutes = nSec / 60+1;
	int nCount;
	if (nMinutes%UNIT == 0) {
		nCount = nMinutes / UNIT;
	}else {
		nCount = nMinutes / UNIT + 1;
	}
	float dbAmount = nCount * CHARGE;
	return dbAmount;
}

int doAddMoney(const char * pName, const char * pPwd, MoneyInfo * pMoneyInfo)
{
	Money sMoney;
	int index = 0;
	//���ҳ�ֵ��
	Card* pCard = checkCard(pName, pPwd, &index);
	if (pCard == NULL||pCard->nStatus==2||pCard->nStatus==3)
	{
		return FALSE;
	}
	//���¿�
	pCard->fBalance += pMoneyInfo->fMoney;
	pCard->fTotalUse += pMoneyInfo->fMoney;
	pCard->tLast = time(NULL);	
	updateCard2(pCard, CARDPATH, index);
	//��¼��ֵ��Ϣ
	strcpy(sMoney.aCardName, pCard->aName);
	sMoney.tTime = time(NULL);
	sMoney.nStatus = 0;
	sMoney.fMoney = pMoneyInfo->fMoney;
	sMoney.nDel = 0;
	if (saveMoney(&sMoney, MONEYPATH)==FALSE) {
		return FALSE;
	}
	pMoneyInfo->fBalance = pCard->fBalance;
	strcpy(pMoneyInfo->aCardName, pName);
	return TRUE;
}

int doRefundMoney(const char * pName, const char * pPwd, MoneyInfo * pMoneyInfo)
{
	Money sMoney;
	int index = 0;
	//�����˷ѿ�
	Card* pCard = checkCard(pName, pPwd, &index);
	if (pCard == NULL || pCard->nStatus!=0)
	{
		return FALSE;
	}
	if (pCard->fBalance <= 0) {
		return UNENOUUGHMONEY;
	}
	float total = pCard->fBalance;
	//���¿�
	pCard->fBalance = 0;
	pCard->fTotalUse -= total;
	pCard->tLast = time(NULL);
	if (updateCard2(pCard, CARDPATH, index) == FALSE) {
		return FALSE;
	}
	//��¼�˷���Ϣ
	strcpy(sMoney.aCardName, pCard->aName);
	sMoney.tTime = time(NULL);
	sMoney.nStatus = 1;
	sMoney.fMoney = total;
	sMoney.nDel = 0;
	if (saveMoney(&sMoney, MONEYPATH) == FALSE) {
		return FALSE;
	}	
	pMoneyInfo->fMoney = total;
	pMoneyInfo->fBalance = 0;
	strcpy(pMoneyInfo->aCardName, pName);
	return TRUE;
}

int annulCard(Card * pCard)
{	
	int index = 0;
	//����ע����
	Card* curCard = checkCard(pCard->aName, pCard->aPwd, &index);
	if (curCard == NULL || curCard->nStatus != 0)
	{
		return FALSE;
	}
	if (curCard->fBalance < 0) {
		return UNENOUUGHMONEY;
	}
	curCard->nDel = 1;
	curCard->nStatus = 2;
	curCard->tLast = time(NULL);
	pCard->fTotalUse = curCard->fTotalUse - curCard->fBalance;
	pCard->fBalance = curCard->fBalance;
	curCard->fTotalUse = curCard->fTotalUse - curCard->fBalance;
	curCard->fBalance = 0;
	if (updateCard2(curCard, CARDPATH, index) == FALSE) {
		return FALSE;
	}
	return TRUE;
}
void search() {
	char aName[32] = { 0 };
	printf("�����뿨��\n");
	scanf("%s", &aName);
	while (strlen(aName) > 18) {
		printf("���Ź���������������\n");
		scanf("%s", &aName);
	}
	printf("�����뿪ʼʱ�䣬��ʽ2000-8-10 18:18\n");
	getchar();
	char a[21];
	gets(a);
	time_t start=stringToTime(a);
	printf("���������ʱ�䣬��ʽ2000-8-10 18:18\n");
	char b[21];
	gets(b);
	time_t end = stringToTime(b);
	lpBillingNode p = getBillingList();
	if (end == -1 || start == -1) {
		printf("��������\n");
		return;
	}
	printf("����\t���\tʱ��\n");
	while (p!=NULL)
	{
		if(strcmp(p->data.aCardName, aName) 
			&& (end > p->data.tEnd) 
			&& (start < p->data.tEnd)){
			timeToString(p->data.tEnd,a);
			printf("%s\t%.1f\t%s\n",p->data.aCardName,p->data.fAmount,a);
		}
		p = p->next;
	}
}
void total() {
	float result=doTotal();
	printf("��Ӫҵ����%f", result);
}
void totalBalance() {
	float result = doTotalBalance();
	printf("�û��������%f", result);
}
void activeCard() {
	int count = getCardCount2(CARDPATH);
	printf("���û�����Ϊ%d", count);
}