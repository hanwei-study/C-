#include "billing_service.h"
#include"billing_file.h"
#include"global.h"
#include<stdlib.h>
#include<string.h>
lpBillingNode billingList;
int addBilling(Billing billing)
{
	return saveBilling(&billing, BILLINGPATH);
}

void initBillingList()
{
	lpBillingNode head;
	head = (lpBillingNode)malloc(sizeof(BillingNode));
	if (head != NULL) {
		head->next = NULL;
		billingList = head;
	}
}

void releaseBillingList()
{
	lpBillingNode p = NULL;
	while (billingList != NULL) {
		p = billingList;
		billingList = billingList->next;
		free(p);
	}
}

int getBilling()
{
	int nCount = 0;
	int i = 0;
	Billing* pBilling = NULL;
	lpBillingNode node = NULL;
	char *pPath = BILLINGPATH;
	nCount = getBillingCount(pPath);
	pBilling = (Billing*)malloc(sizeof(Billing)*nCount);
	if (billingList != NULL) {
		releaseBillingList();
	}
	initBillingList();
	if (pBilling == NULL) {
		return FALSE;
	}
	if (FALSE == readBilling(pBilling, pPath)) {
		free(pBilling);
		pBilling = NULL;
		return FALSE;
	}
	lpBillingNode cur = NULL;
	for (i = 0, node = billingList; i < nCount; i++) {
		//·ÖÅäÄÚ´æ
		cur = (lpBillingNode)malloc(sizeof(BillingNode));
		if (cur == NULL) {
			free(pBilling);
			return FALSE;
		}
		memset(cur, 0, sizeof(BillingNode));
		cur->data = pBilling[i];
		cur->next = NULL;
		node->next = cur;
		node = cur;
	}
	free(pBilling);
	pBilling = NULL;
	return TRUE;
}

Billing * queryBilling(const char * pName, int * pIndex)
{
	if (getBilling() == FALSE) {
		return FALSE;
	}
	lpBillingNode billingNode = billingList->next;
	while (billingNode != NULL) {
		
		if ((strcmp(billingNode->data.aCardName, pName) == 0)&&(billingNode->data.nDel==0)) {
			return &billingNode->data;
		}
		(*pIndex)++;
		billingNode = billingNode->next;
	}
	return NULL;
}
lpBillingNode getBillingList() {
	getBilling();
	return billingList;
}