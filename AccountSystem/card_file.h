#ifndef CARD_FILE_H
#define CARD_FILE_H
#include"model.h"
#include"tool.h"
#include"global.h"
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int saveCard2(const Card * pCard, const char * pPath);
int readCard2(Card * pCard, const char * pPath);
int getCardCount2(const char * pPath);
int updateCard2(const Card * pCard, const char * pPath, int nIndex);
#endif // !CARD_FILE_H
