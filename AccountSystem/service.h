#ifndef SERVICE_H
#define SERVICE_H
#include"model.h"
int addCardInfo(Card card);    // ÉùÃ÷addCard()º¯Êý
Card* queryCardsInfo(const char* pName, int* index);
int doLogon(const char * pName, const char * pPwd, LogonInfo* pLogonInfo);
void release();
int doSettle(const char * pName, const char * pPwd, SettleInfo* pSettleInfo);
float getAmount(time_t tStart);
int doAddMoney(const char * pName, const char * pPwd, MoneyInfo* pMoneyInfo);
int doRefundMoney(const char * pName, const char * pPwd, MoneyInfo* pMoneyInfo);
int annulCard(Card* pCard);
void search();
void total();
void totalBalance();
void activeCard();
#endif // !SERVICE_H
