/*
 * =====================================================================================
 *
 *       Filename:  CLexAnalyser.c
 *
 *    Description:  C Lex Analyser
 *
 *        Version:  1.0
 *        Created:  03/09/2014 17:52:00
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Leo Lee (), leeleo3x@gmail.com
 *   Organization:  ZJU
 *
 * =====================================================================================
 */
#include "CLexAnalyser.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

char keyArr[40][30],limitArr[20][30],operaArr[40][30];
int keySum,limitSum,operaSum;

struct normalNode * normalHead, * normalTail = NULL;
struct errorNode * errorHead, * errorTail = NULL;
struct idenNode * idenHead, * idenTail = NULL;

void initialize()
{
    char str[30];
    freopen("key","r",stdin);
    keySum = 0;
    while (scanf("%s",str) != EOF)
    {
        strcpy(keyArr[keySum++],str);
    }
    fclose(stdin);
    freopen("limit","r",stdin);
    limitSum = 0;
    while (scanf("%s",str) != EOF)
    {
        strcpy(limitArr[limitSum++],str);
    }
    fclose(stdin);
    freopen("opera","r",stdin);
    operaSum = 0;
    while (scanf("%s",str) != EOF)
    {
        strcpy(operaArr[operaSum++],str);
    }
}

void createNewNode(char *content,char *describe,int type,int addr,int line)
{
    struct normalNode * temp = (struct normalNode *) malloc(sizeof(struct normalNode));
    if (normalTail == NULL)
    {
        normalTail = temp;
        normalHead = temp;
    }
    else
    {
        normalTail->next = temp;
        normalTail = temp;
    }
    strcpy(temp->content,content);
    strcpy(temp->describe,describe);
    temp->type = type;
    temp->addr = addr;
    temp->line = line;
    temp->next = NULL;
}

void createNewError(char *content,char *describe,int type,int line)
{}
int main()
{
    char str[30];
    freopen("key","r",stdin);
    //initialize();
    return 0;
}
