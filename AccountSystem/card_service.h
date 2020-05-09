#ifndef CARD_SERVICE_H
#define CARD_SERVICE_H
#include"model.h"
#include"card_service.h"
#include"global.h"
#include<string.h>
#include<stdlib.h>
#include"card_file.h"
int addCard(Card card);    // ����addCard()����
Card* queryCard(const char* pName);//����queryCard()����
Card* queryCards(const char* pName, int* index);
int getCard();
float doTotal();
float doTotalBalance();
Card* checkCard(const char * pName, const char * pPwd,int * pIndex);
int initCardList();
void releaseCardList();
#endif // !SERVICE_H

