#ifndef BILLING_SERVICE_H
#endif // !BILLING_SERVICE_H
#ifndef BILLING_SERVICE_H
#define BILLING_SERVICE_H
#include"model.h"
int addBilling(Billing billing);
void initBillingList();
void releaseBillingList();
int getBilling();
Billing* queryBilling(const char* pName, int *pIndex);
lpBillingNode getBillingList();
#endif // !BILLING_SERVICE_H