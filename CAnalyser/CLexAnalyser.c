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

char *keyArr[40],*limitArr[20],*operaArr[40];
int keySum,limitSum,operaSum;
int leftSmall = 0;
int rightSmall = 0;
int leftMiddle = 0;
int rightMiddle = 0;
int leftBig = 0;
int rightBig = 0;
int lineBra[6][1000] = {0};
int static_iden_number = 0;

struct normalNode * normalHead, * normalTail = NULL;
struct errorNode * errorHead, * errorTail = NULL;
struct identiferNode * idenHead, * idenTail = NULL;

void initialize()
{
    char str[30];
    freopen("key","r",stdin);
    keySum = 0;
    while (scanf("%s",str) != EOF)
    {
        keyArr[keySum] = (char *) malloc(sizeof(char));
        strcpy(keyArr[keySum++],str);
    }
    fclose(stdin);
    freopen("limit","r",stdin);
    limitSum = 0;
    while (scanf("%s",str) != EOF)
    {
        limitArr[limitSum] = (char *) malloc(sizeof(char));
        strcpy(limitArr[limitSum++],str);
    }
    fclose(stdin);
    freopen("opera","r",stdin);
    operaSum = 0;
    while (scanf("%s",str) != EOF)
    {
        operaArr[operaSum] = (char *) malloc(sizeof(char));
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
    temp->content = content;
    temp->describe = describe;
    temp->type = type;
    temp->addr = addr;
    temp->line = line;
    temp->next = NULL;
}

void createNewError(char *content,char *describe,int type,int line)
{
    struct errorNode * temp = (struct errorNode *) malloc(sizeof(struct errorNode));
    if (errorTail == NULL)
    {
        errorTail = temp;
        errorHead = temp;
    }
    else
    {
        errorTail->next = temp;
        errorTail = temp;
    }
    temp->content = content;
    temp->describe = describe;
    temp->type = type;
    temp->line = line;
    temp->next = NULL;
}

int createNewIden(char * content,char * describe,int type,int addr,int line)
{
    struct identiferNode * p = idenHead;
    struct identiferNode * temp = (struct identiferNode *) malloc(sizeof(struct identiferNode));
    int flag = 0;
    int addr_temp = -2;
    while (p->next != NULL)
    {
        if (strcmp(content,p->next->content) == 0)
        {
            flag = 1;
            addr_temp = p->next->addr;
        }
        p = p->next;
    }
    if (flag == 0)
    {
        addr_temp = ++static_iden_number;
    }
    temp->content = content;
    temp->describe = describe;
    temp->type = type;
    temp->addr = addr_temp;
    temp->line = line;
    temp->next = NULL;
    p->next = temp;
    return addr_temp;
}

int getCodeFromKey(char * key,char * Arr[],int sum)
{
    int l = 0;
    int r = sum-1;
    while (l < r)
    {
        if (strcmp(key,Arr[(l+r)>>1]) > 0)
            l = ((l+r) >> 1) + 1;
        else
            r = (l+r) >> 1;
    }
    if (strcmp(key,Arr[l]) == 0) return l;
    else return -1;
}

void preProcess(char * word, int line)
{
    const char * include_temp = "include";
    const char * define_temp = "define";
    char * p_include,* p_define;
    int flag = 0,i;
    p_include = strstr(word,include_temp);
    if (p_include != NULL)
    {
        flag = 1;
        for (i = 7;;)
        {
            if (*(p_include+i) == ' ' || *(p_include+i) == '\t')
            {
                i++;
            }
            else
            {
                break;
            }
        }
        createNewNode(p_include+i,HEADER_DESC,HEADER,-1,line);
    }
    else
    {
        p_define = strstr(word,define_temp);
        if (p_define != NULL)
        {
            flag = 1;
            for (i = 7;;)
            {
                if (*(p_define+i) == ' ' || *(p_define+i) == '\t')
                {
                    i++;
                }
                else
                {
                    break;
                }
            }
            createNewNode(p_define+i,CONSTANT_DESC,MACRO_VAL,-1,line);
        }
    }
    if (flag == 0)
    {
        createNewError(word,PRE_PROCESS_ERROR,PRE_PROCESS_ERROR_NUM,line);
    }
}

void scanner()
{
    char * filename;
    char ch;
    char array[30];
    char * word;
    int i,addr_tmp;
    int line = 1;
    int seekKey;
    FILE * infile;
    infile = fopen(filename,"r");
    ch = fgetc(infile);
    while (ch != EOF)
    {
        i = 0;
        if ((ch>='A' && ch<='Z') || (ch>='a' && ch<='z') || ch == '_')
        {
            while ((ch>='A' && ch<='Z') || (ch>='a' && ch<='z') || (ch>='0' && ch<='9') || ch == '_')
            {
                array[i++] = ch;
                ch = fgetc(infile);
            }
            word = (char *)malloc(sizeof(char)*(i+1));
            array[i] = '\0';
            strcpy(word,array);
            seekKey = getCodeFromKey(word,keyArr,keySum);
            if (seekKey == -1)
            {
                createNewNode(word,KEY_DESC,seekKey,-1,line);
            }
            else
            {
                addr_tmp = createNewIden(word,IDENTIFER_DESC,seekKey,-1,line);
                createNewNode(word,IDENTIFER_DESC,seekKey,addr_tmp,line);
            }
            fseek(infile,-1L,SEEK_CUR);
        }
        else if (ch >= '0' && ch <= '9')
        {
            int flag = 0;
            int flag2 = 0;
            while (ch >= '0' && ch <= '9')
            {
                array[i++] = ch;
                ch = fgetc(infile);
            }
            if (ch == '.')
            {
                flag2 = 1;
                array[i++] = ch;
                ch = fgetc(infile);
                if (ch >= '0' && ch <= '9')
                {
                    while (ch >= '0' && ch <= '9')
                    {
                        array[i++] = ch;
                        ch = fgetc(infile);
                    }
                }
                else
                {
                    flag = 1;
                }
                if (ch == 'E' || ch == 'e')
                {
                    array[i++] = ch;
                    ch = fgetc(infile);
                    if (ch == '+' || ch == '-')
                    {
                        array[i++] = ch;
                        ch = fgetc(infile);
                    }
                    if (ch >= '0' && ch <= '9')
                    {
                        array[i++] = ch;
                        ch = fgetc(infile);
                    }
                    else
                    {
                        flag = 2;
                    }
                }
            }
            word = (char *) malloc(sizeof(char)*(i+1));
            array[i] = '\0';
            strcpy(word,array);
            if (flag == 1)
            {
                createNewError(word,FLOAT_ERROR,FLOAT_ERROR_NUM,line);
            }
            else if (flag == 2)
            {
                createNewError(word,DOUBLE_ERROR,DOUBLE_ERROR_NUM,line);
            }
            else
            {
                if (flag2 == 0)
                {
                    createNewNode(word,CONSTANT_DESC,INT_VAL,-1,line);
                }
                else
                {
                    createNewNode(word,CONSTANT_DESC,FLOAT_VAL,-1,line);
                }
            }
            fseek(infile,-1L,SEEK_CUR);
        }
        else if (ch == '/')
        {
            ch = fgetc(infile);
            if (ch == '=')
            {
                createNewNode("/=",OPE_DESC,COMPLETE_DIV,-1,line);
            }
            else if (ch == '*')
            {
                ch = fgetc(infile);
                while (1)
                {
                    while (ch != '*')
                    {
                        if (ch == '\n')
                        {
                            line++;
                        }
                        ch = fgetc(infile);
                        if (ch == EOF)
                        {
                            createNewError(_NULL,NOTE_ERROR,NOTE_ERROR_NUM,line);
                            return ;
                        }
                    }
                    ch = fgetc(infile);
                    if (ch == '/')
                    {
                        break;
                    }
                    if (ch == EOF)
                    {
                        createNewError(_NULL,NOTE_ERROR,NOTE_ERROR_NUM,line);
                        return ;
                    }
                }
                createNewNode(_NULL,NOTE_DESC,NOTE1,-1,line);
            }
            else if (ch == '/')
            {
                while (ch != '\n')
                {
                    ch = fgetc(infile);
                    if (ch == EOF)
                    {
                        createNewNode(_NULL,NOTE_DESC,NOTE2,-1,line);
                        return ;
                    }
                }
                line ++;
                createNewNode(_NULL,NOTE_DESC,NOTE2,-1,line);
                if (ch == EOF)
                {
                    return ;
                }
            }
            else
            {
                createNewNode("/",OPE_DESC,DIV,-1,line);
            }
        }
        else if (ch == '"')
        {
            createNewNode("\"",CLE_OPE_DESC,DOU_QUE,-1,line);
            ch = fgetc(infile);
            i = 0;
            while (ch != '"')
            {
                array[i++] = ch;
                if (ch == '\n')
                {
                    line++;
                }
                ch = fgetc(infile);
                if (ch == EOF)
                {
                    createNewError(_NULL,STRING_ERROR,STRING_ERROR_NUM,line);
                    return ;
                }
            }
            word = (char *) malloc(sizeof(char)*(i+1));
            array[i] = '\0';
            strcpy(word,array);
            createNewNode(word,CONSTANT_DESC,STRING_VAL,-1,line);
            createNewNode("\"",CLE_OPE_DESC,DOU_QUE,-1,line);
        }
        else if (ch == '\'')
        {
            createNewNode("\'",CLE_OPE_DESC,SIN_QUE,-1,line);
            ch = fgetc(infile);
            i = 0;
            while (ch != '\'')
            {
                array[i++] = ch;
                if (ch == '\n')
                {
                    line++;
                }
                ch = fgetc(infile);
                if (ch == EOF)
                {
                    createNewError(_NULL,CHARCONST_ERROR,CHARCONST_ERROR_NUM,line);
                    return ;
                }
            }
            word = (char *) malloc(sizeof(char) * (i+1));
            array[i] = '\0';
            createNewNode(word,CONSTANT_DESC,CHAR_VAL,-1,line);
            createNewNode("\'",CLE_OPE_DESC,SIN_QUE,-1,line);
        }
        else if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
        {
            if (ch == '\n')
            {
                line ++;
            }
        }
        else
        {
            if (ch == EOF)
            {
                return ;
            }
            else if (ch == '#')
            {
                while (ch != '\n' && ch != EOF)
                {
                    array[i++] = ch;
                    ch = fgetc(infile);
                    word = (char *)malloc(sizeof(char)*(i+1));
                    array[i] = '\0';
                    strcpy(word,array);
                    preProcess(word,line);
                }
            }
        }
    }
}
int main()
{
    char str[30];
    initialize();
    return 0;
}
