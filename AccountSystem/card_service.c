#include"card_service.h"
#include"model.h"
#include"global.h"
#include<string.h>
#include<stdlib.h>
#include"card_file.h"
#include"billing_service.h"
lpCardNode cardList = NULL;
int initCardList()
{
	lpCardNode head;
	head = (lpCardNode)malloc(sizeof(CardNode));
	if (head != NULL) {
		head->next = NULL;
		cardList = head;
		return TRUE;
	}
	return FALSE;
}
void releaseCardList()
{
	lpCardNode p = NULL;
	while(cardList != NULL) {
		p = cardList;
		cardList = cardList->next;
		free(p);
	}
}
Card* checkCard(const char * pName, const char * pPwd,int * pIndex)
{	

	if (getCard() == FALSE) {
		return FALSE;
	}
	lpCardNode cardNode = cardList->next;
	while (cardNode != NULL) {
		if ((strcmp(cardNode->data.aName, pName) == 0) && (strcmp(cardNode->data.aPwd, pPwd)==0)&&cardNode->data.nDel==0) {
			return &cardNode->data;
		}
		(*pIndex)++;
		cardNode = cardNode->next;
	}
	return NULL;
}
int addCard(Card card)
{
	char *pPath = CARDPATH;
	if (TRUE == saveCard2(&card,pPath)) {
		return TRUE;
	}	
	return FALSE;
}

Card * queryCard(const char * pName)
{
	lpCardNode p = NULL;
	if (getCard() == FALSE) {
		return FALSE;
	}
	if (cardList->next != NULL) {
		p = cardList->next;
		while (p != NULL) {
			if ((strcmp(p->data.aName, pName)==0)&&(p->data.nDel==0)) {
				return &p->data;
			}
			p = p->next;
		}
	}
	return NULL;
}

Card * queryCards(const char * pName, int * index)
{
	lpCardNode p = NULL;
	if (getCard() == FALSE) {
		return FALSE;
	}
	if (cardList->next != NULL) {
		Card* pCard = (Card*)malloc(sizeof(Card));
		p = cardList->next;
		while (p != NULL) {
			if ((strstr(p->data.aName, pName)!= NULL)) {
				pCard[*index] = p->data;
				(*index)++;
				pCard = (Card*)realloc(pCard, ((*index)+1) * sizeof(Card));
			}
			p = p->next;
		}
		if (index == 0) {
			return NULL;
		}
		return pCard;
	}
	return NULL;
}

int getCard()
{
	int nCount = 0;
	int i = 0;
	Card* pCard = NULL;
	lpCardNode node = NULL;
	char *pPath = CARDPATH;
	nCount = getCardCount2(pPath);
	pCard = (Card*)malloc(sizeof(Card)*nCount);
	if (cardList!=NULL) {
		releaseCardList();
	}
	initCardList();
	if (pCard == NULL) {
		return FALSE;
	}
	if (FALSE == readCard2(pCard, pPath)) {
		free(pCard);
		pCard = NULL;
		return FALSE;
	}
	lpCardNode cur = NULL;
	for (i = 0, node = cardList; i < nCount; i++) {
		//·ÖÅäÄÚ´æ
		cur = (lpCardNode)malloc(sizeof(CardNode));
		if (cur == NULL) {
			free(pCard);
			return FALSE;
		}
		memset(cur, 0, sizeof(CardNode));
		cur->data = pCard[i];
		cur->next = NULL;
		node->next = cur;
		node = cur;
	}
	free(pCard);
	pCard = NULL;
	return TRUE;
}
float doTotal() {
	if (getCard() == FALSE) {
		return 0;
	}
	float result = 0;
	lpCardNode p=cardList->next;
	while (p!=NULL)
	{
		result += p->data.fTotalUse;
		p = p->next;
	}
	return result;
	
}
float doTotalBalance() {
	if (getCard() == FALSE) {
		return 0;
	}
	float result = 0;
	lpCardNode p = cardList->next;
	while (p != NULL)
	{
		result += p->data.fBalance;
		p = p->next;
	}
	return result;
}