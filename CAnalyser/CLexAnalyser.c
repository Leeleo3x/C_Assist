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
 *       Compiler:  watcom
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
int lineBra[6][1000] ;
int static_iden_number = 0;
int stackDepth = 0;


struct stackStruct
{
    char * identiferName;
    int identiferType;
    int currentDepth;
    struct stackStruct * pre;
};

struct stackStruct * stackTail = NULL;
struct normalNode * normalHead, * normalTail = NULL;
struct errorNode * errorHead, * errorTail = NULL;
struct identiferNode * idenHead, * idenTail = NULL;

void stackPush(const char * identiferName, int identiferType)
{
    struct stackStruct * tmp;
    tmp = (struct stackStruct *) malloc(sizeof(struct stackStruct));
    tmp->identiferName = (char *) malloc(sizeof(char) * strlen(identiferName));
    tmp->identiferType = identiferType;
    tmp->currentDepth = stackDepth;
    tmp->pre = stackTail;
    stackTail = tmp;  
}

void stackPop()
{
    while (stackTail != NULL && stackTail->currentDepth > stackDepth){
        struct stackStruct * tmp;
        tmp = stackTail;
        stackTail = stackTail->pre;
        free(tmp->identiferName);
        free(tmp);
    }
}

int stackGetIdentiferType(const char * identiferName)
{
    struct stackStruct * tmp = stackTail;
    while (tmp != NULL) {
        if (strcmp(identiferName, tmp->identiferName) == 0) {
            return tmp->identiferType;
        }
    }
    return 0;
}

void initialize()
{
    char str[30];
    freopen("key","r",stdin);
    keySum = 0;
    while (scanf("%s",str) != EOF)
    {
        keyArr[keySum] = (char *) malloc(sizeof(char));
        strcpy(keyArr[keySum++], str);
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

    normalHead = (struct normalNode *) malloc(sizeof(struct normalNode));
    errorHead = (struct errorNode *) malloc(sizeof(struct errorNode));
    idenHead = (struct identiferNode *) malloc(sizeof(struct identiferNode));

    normalHead->next = NULL;
    normalHead->pre = NULL;
    errorHead->next = NULL;
    idenHead->next = NULL;

    normalTail = normalHead;
    errorTail = errorHead;
    idenTail = idenHead;
}

void createNewNode(const char *content,const char *describe,int type,int addr,int line)
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
    temp->content = (char *) malloc(sizeof(char) * strlen(content));
    temp->describe = (char *) malloc(sizeof(char) * strlen(describe));
    strcpy(temp->content, content);
    strcpy(temp->describe, describe);
    temp->type = type;
    temp->addr = addr;
    temp->line = line;
    temp->next = NULL;
}

void createNewError(const char *content,const char *describe,int type,int line)
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
    temp->content = (char *) malloc(sizeof(char));
    temp->describe = (char *) malloc(sizeof(char));
    strcpy(temp->content, content);
    strcpy(temp->describe, describe);
    temp->type = type;
    temp->line = line;
    temp->next = NULL;
}

int createNewIden(const char * content,const char * describe,int type,int addr,int line)
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
    if (flag == 0) {
        addr_temp = ++static_iden_number;
    } else {
        return addr_temp;
    }
    temp->content = (char *) malloc(sizeof(char) * strlen(content));
    temp->describe = (char *) malloc(sizeof(char) * strlen(describe));
    strcpy(temp->content, content);
    strcpy(temp->describe, describe);
    temp->type = type;
    temp->addr = addr;
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

int identiferDefineCheck(struct normalNode * p)
{
    while (p && p->type == COMMA) p = p->pre;
    switch (p->type) {
        case CHAR : return CHAR;
        case ENUM : return ENUM;
        case FLOAT : return FLOAT;
        case LONG : return LONG;
        case SHORT : return SHORT;
        case SIGNED : return SIGNED;
        case UNSIGNED : return UNSIGNED;
    }
    return 0;
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

void scanner(const char * filename)
{
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
            if (seekKey != -1)
            {
                createNewNode(word,KEY_DESC,seekKey,-1,line);
            }
            else
            {
                int identifer = identiferDefineCheck(normalHead);
                if (identifer) {
                    addr_tmp = createNewIden(word, IDENTIFER_DEFINATION_DESC, identifer, -1, line);
                    createNewNode(word, IDENTIFER_DEFINATION_DESC, identifer, addr_tmp, line);
                    stackPush(word, identifer);
                }
                else {
                    identifer = stackGetIdentiferType(word);
                    if (identifer) {
                        addr_tmp = createNewIden(word,IDENTIFER_DESC, identifer, -1, line);
                        createNewNode(word, IDENTIFER_DESC, identifer, addr_tmp, line);
                    } else {
                        createNewNode(word, IDENTIFER_DESC, -1, -1, line);
                        createNewError(word, UNDEFINED_IDENTIFER, UNDEFINED_IDENTIFER_NUM, line); 
                    }
                }
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
                }
                word = (char *)malloc(sizeof(char)*(i+1));
                array[i] = '\0';
                strcpy(word,array);
                preProcess(word,line);
                fseek(infile,-1L,SEEK_CUR);
            }
            else if (ch == '-')
            {
                array[i++] = ch;
                ch = fgetc(infile);
                if (ch >= '0' && ch <= '9')
                {
                    int flag = 0;
                    int flag2 = 0;
                    while (ch>='0' && ch<='9')
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
                    word = (char *) malloc(sizeof(char) * (i+1));
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
                        if (flag == 0)
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
                else if (ch == '>')
                {
                    createNewNode("->",OPE_DESC,ARROW,-1,line);
                }
                else if (ch == '-')
                {
                    createNewNode("--",OPE_DESC,SELF_SUB,-1,line);
                }
                else if (ch == '=')
                {
                    createNewNode("--",OPE_DESC,SELF_SUB,-1,line);
                }
                else
                {
                    createNewNode("-",OPE_DESC,SUB,-1,line);
                    fseek(infile,-1L,SEEK_CUR);
                }
            }
            else if (ch == '+')
            {
                ch = fgetc(infile);
                if (ch == '+')
                {
                    createNewNode("++",OPE_DESC,SELF_ADD,-1,line);
                }
                else if (ch == '=')
                {
                    createNewNode("+=",OPE_DESC,COMPLETE_ADD,-1,line);
                }
                else
                {
                    createNewNode("+",OPE_DESC,ADD,-1,line);
                    fseek(infile,-1L,SEEK_CUR);
                }
            }
            else if (ch == '*')
            {
                ch = fgetc(infile);
                if (ch == '=')
                {
                    createNewNode("*=",OPE_DESC,COMPLETE_MUL,-1,line);
                }
                else
                {
                    createNewNode("*",OPE_DESC,MUL,-1,line);
                    fseek(infile,-1L,SEEK_CUR);
                }
            }
            else if (ch == '^')
            {
                ch = fgetc(infile);
                if (ch == '=')
                {
                    createNewNode("^=",OPE_DESC,COMPLETE_BYTE_XOR,-1,line);
                }
                else
                {
                    createNewNode("^",OPE_DESC,BYTE_XOR,-1,line);
                    fseek(infile,-1L,SEEK_CUR);
                }
            }
            else if (ch == '%')
            {
                ch = fgetc(infile);
                if (ch == '=')
                {
                    createNewNode("%=",OPE_DESC,COMPLETE_MOD,-1,line);
                }
                else
                {
                    createNewNode("%",OPE_DESC,MOD,-1,line);
                    fseek(infile,-1L,SEEK_CUR);
                }
            }
            else if (ch == '&')
            {
                ch = fgetc(infile);
                if (ch == '=')
                {
                    createNewNode("&=",OPE_DESC,COMPLETE_BYTE_AND,-1,line);
                }
                else if (ch == '&')
                {
                    createNewNode("&&",OPE_DESC,AND,-1,line);
                }
                else
                {
                    createNewNode("&",OPE_DESC,BYTE_AND,-1,line);
                    fseek(infile,-1L,SEEK_CUR);
                }
            }
            else if (ch == '~')
            {
                ch = fgetc(infile);
                if (ch == '=')
                {
                    createNewNode("~=",OPE_DESC,COMPLETE_COMPLEMENT,-1,line);
                }
                else
                {
                    createNewNode("~",OPE_DESC,COMPLEMENT,-1,line);
                    fseek(infile,-1L,SEEK_CUR);
                }
            }
            else if (ch == '!')
            {
                ch = fgetc(infile);
                if (ch == '=')
                {
                    createNewNode("!=",OPE_DESC,NOT_EQUAL,-1,line);
                }
                else
                {
                    createNewNode("!",OPE_DESC,NOT,-1,line);
                    fseek(infile,-1L,SEEK_CUR);
                }
            }
            else if (ch == '<')
            {
                ch = fgetc(infile);
                if (ch == '<')
                {
                    createNewNode("<<",OPE_DESC,LEFT_MOVE,-1,line);
                }
                else if (ch == '=')
                {
                    createNewNode("<=",OPE_DESC,LES_EQUAL,-1,line);
                }
                else
                {
                    createNewNode("<",OPE_DESC,LES_THAN,-1,line);
                    fseek(infile,-1L,SEEK_CUR);
                }
            }
            else if (ch == '>')
            {
                ch = fgetc(infile);
                if (ch == '>')
                {
                    createNewNode(">>",OPE_DESC,RIGHT_MOVE,-1,line);
                }
                else if (ch == '=')
                {
                    createNewNode(">=",OPE_DESC,GRT_EQUAL,-1,line);
                }
                else
                {
                    createNewNode(">",OPE_DESC,GRT_THAN,-1,line);
                    fseek(infile,-1L,SEEK_CUR);
                }
            }
            else if (ch == '|')
            {
                ch = fgetc(infile);
                if (ch == '|')
                {
                    createNewNode("||",OPE_DESC,OR,-1,line);
                }
                else
                {
                    createNewNode("|",OPE_DESC,BYTE_OR,-1,line);
                    fseek(infile,-1L,SEEK_CUR);
                }
            }
            else if (ch == '=')
            {
                ch = fgetc(infile);
                if (ch == '=')
                {
                    createNewNode("==",OPE_DESC,EQUAL,-1,line);
                }
                else
                {
                    createNewNode("=",OPE_DESC,ASG,-1,line);
                    fseek(infile,-1L,SEEK_CUR);
                }
            }
            else if (ch == '(')
            {
                leftSmall++;
                lineBra[0][leftSmall] = line;
                createNewNode("(",CLE_OPE_DESC,LEFT_BRA,-1,line);
            }
            else if (ch == ')')
            {
                rightSmall++;
                lineBra[1][rightSmall] = line;
                createNewNode(")",CLE_OPE_DESC,RIGHT_BRA,-1,line);
            }
            else if (ch == '[')
            {
                leftMiddle++;
                lineBra[2][leftMiddle] = line;
                createNewNode("[",CLE_OPE_DESC,LEFT_INDEX,-1,line);
            }
            else if (ch == ']')
            {
                leftMiddle++;
                lineBra[3][rightMiddle] = line;
                createNewNode("]",CLE_OPE_DESC,RIGHT_INDEX,-1,line);
            }
            else if (ch == '{')
            {
                leftBig++;
                lineBra[4][leftBig] = line;
                createNewNode("{",CLE_OPE_DESC,L_BOUNDER,-1,line);
            }
            else if (ch == '}')
            {
                leftBig++;
                lineBra[5][rightBig] = line;
                createNewNode("}",CLE_OPE_DESC,R_BOUNDER,-1,line);
            }
            else if (ch == '.')
            {
                createNewNode(".",CLE_OPE_DESC,POINTER,-1,line);
            }
            else if (ch == ',')
            {
                createNewNode(",",CLE_OPE_DESC,COMMA,-1,line);
            }
            else if (ch == ';')
            {
                createNewNode(";",CLE_OPE_DESC,SEMI,-1,line);
            }
            else
            {
                char temp[2];
                temp[0] = ch;
                temp[1] = '\0';
                createNewError(temp,CHAR_ERROR,CHAR_ERROR_NUM,line);
            }
        }
        ch = fgetc(infile);
    }
}

void BraMappingError()
{
    if (leftSmall != rightSmall)
    {
        int i = (leftSmall > rightSmall) ? (leftSmall-rightSmall) : (rightSmall - leftSmall);
        int flag = (leftSmall>rightSmall) ? 1 : 0;
        if (flag)
        {
            while (i--)
            {
                createNewError(_NULL,LEFT_BRA_ERROR,LEFT_BRA_ERROR_NUM,lineBra[0][i+1]);
            }
        }
        else
        {
            while (i--)
            {
                createNewError(_NULL,RIGHT_BRA_ERROR,RIGHT_BRA_ERROR_NUM,lineBra[1][i+1]);
            }
        }
    }
    if (leftMiddle != rightMiddle)
    {
        int i = (leftMiddle > rightMiddle) ? (leftMiddle - rightMiddle) : (rightMiddle - leftMiddle);
        int flag = (leftMiddle > rightMiddle) ? 1 : 0;
        if (flag)
        {
            while (i--)
            {
                createNewError(_NULL,LEFT_INDEX_ERROR,LEFT_INDEX_ERROR_NUM,lineBra[2][i+1]);
            }
        }
        else
        {
            while (i--)
            {
                createNewError(_NULL,RIGHT_INDEX_ERROR,RIGHT_INDEX_ERROR_NUM,lineBra[3][i+1]);
            }
        }
    }
    if (leftBig != rightBig)
    {
        int i = (leftBig>rightBig) ? (leftBig - rightBig) : (rightBig - leftBig); //different with origin file
        int flag = (leftBig>rightBig) ? 1 : 0;
        if (flag)
        {
            while (i--)
            {
                createNewError(_NULL,L_BOUNDER_ERROR,L_BOUNDER_ERROR_NUM,lineBra[4][i+1]);
            }
        }
        else
        {
            while (i--)
            {
                createNewError(_NULL,R_BOUNDER_ERROR,R_BOUNDER_ERROR_NUM,lineBra[5][i+1]);
            }
        }
    }
}

void work(char * file)
{
    scanner(file);
}
int main()
{
    initialize();
    scanner("test.c");

    freopen("test.out","w",stdout);
    struct normalNode * p = normalHead;
    for (; p; p = p->next) {
        printf("%s %s %d %d\n", p->content,  p->describe, p->addr, p->line);
    }
    printf("*********\n");
    struct errorNode * error = errorHead;
    for (; error; error = errorHead->next) {
        printf("%s %s %d %d\n", error->content, error->describe, error->type, error->line);
    }
    printf("*********\n");
    struct identiferNode * iden = idenHead;
    for (; iden; iden = iden->next) {
        printf("%s %s %d %d\n", iden->content, iden->describe, iden->type, iden->line);
    }
}
