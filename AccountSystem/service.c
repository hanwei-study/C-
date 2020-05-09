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
	//判断卡号密码是否正确
	int index = 0;
	Card* card=checkCard(pName, pPwd,&index);
	if (card == NULL) {
		return FALSE;
	}
	//判断能否上机
	if (card->nStatus != 0) {
		return ONUSE;//已经上机
	}
	if (card->fBalance <= 0) {
		return UNENOUUGHMONEY;//余额不足
	}
	//更新卡信息
	card->nStatus = 1;
	card->nUseCount++;
	card->tLast = time(NULL);
	updateCard2(card, CARDPATH, index);
	//添加消费
	strcpy(billing.aCardName, pName);
	billing.fAmount = 0;
	billing.nStatus = 0;
	billing.nDel = 0;
	billing.tStart = time(NULL);
	billing.tEnd = time(NULL);
	//添加登录信息
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
	//判断卡号密码是否正确
	int index = 0;
	Card* card = checkCard(pName, pPwd, &index);
	if (card == NULL) {
		return FALSE;
	}
	//判断能否下机
	if (card->nStatus != 1) {
		return FALSE;//没有上机
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
	//更新卡信息
	card->fBalance = fBalance;
	card->nStatus = 0;
	card->tLast = time(NULL);
	updateCard2(card, CARDPATH, index);
	//更新消费信息
	pBilling->fAmount = fBalance;
	pBilling->nStatus = 1;
	pBilling->nDel = 1;
	pBilling->tEnd = time(NULL);
	updateBilling(pBilling, BILLINGPATH, positon);
	//记录下机信息
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
	//查找充值卡
	Card* pCard = checkCard(pName, pPwd, &index);
	if (pCard == NULL||pCard->nStatus==2||pCard->nStatus==3)
	{
		return FALSE;
	}
	//更新卡
	pCard->fBalance += pMoneyInfo->fMoney;
	pCard->fTotalUse += pMoneyInfo->fMoney;
	pCard->tLast = time(NULL);	
	updateCard2(pCard, CARDPATH, index);
	//记录充值信息
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
	//查找退费卡
	Card* pCard = checkCard(pName, pPwd, &index);
	if (pCard == NULL || pCard->nStatus!=0)
	{
		return FALSE;
	}
	if (pCard->fBalance <= 0) {
		return UNENOUUGHMONEY;
	}
	float total = pCard->fBalance;
	//更新卡
	pCard->fBalance = 0;
	pCard->fTotalUse -= total;
	pCard->tLast = time(NULL);
	if (updateCard2(pCard, CARDPATH, index) == FALSE) {
		return FALSE;
	}
	//记录退费信息
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
	//查找注销卡
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
	printf("请输入卡号\n");
	scanf("%s", &aName);
	while (strlen(aName) > 18) {
		printf("卡号过长，请重新输入\n");
		scanf("%s", &aName);
	}
	printf("请输入开始时间，格式2000-8-10 18:18\n");
	getchar();
	char a[21];
	gets(a);
	time_t start=stringToTime(a);
	printf("请输入结束时间，格式2000-8-10 18:18\n");
	char b[21];
	gets(b);
	time_t end = stringToTime(b);
	lpBillingNode p = getBillingList();
	if (end == -1 || start == -1) {
		printf("输入有误\n");
		return;
	}
	printf("卡号\t金额\t时间\n");
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
	printf("总营业额是%f", result);
}
void totalBalance() {
	float result = doTotalBalance();
	printf("用户总余额是%f", result);
}
void activeCard() {
	int count = getCardCount2(CARDPATH);
	printf("总用户数量为%d", count);
}