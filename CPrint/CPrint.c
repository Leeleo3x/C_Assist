/*
 *  Filename: CodeRebuild.c
 *  Create: 05/11/2014
 *  Author: Momoco
 *
 */

#include "../C_Assist.h"

int currentX, currentY;
int textViewPosX, textViewPosY;
int textViewWidth, textViewHeight;
int currentLine;
int indentNum;

void printCode(int beginX, int beginY, int width, int height, struct normalNode * normalPoint);
void printError(int beginX, int beginY,int width,int height, struct errorNode * errorPoint);
void printTextWithColor(const char * text, int color);
void printInclude();
void printDefine();
int getColorWithType(int type);

int textWidth(const char * str)
{
    return (strlen(str) * charWidth);
}

void printContent(int beginX, int beginY, int width, int height, struct normalNode * normalPoint, struct errorNode * errorPoint) 
{
    int codeHeight = height * 7 / 10;
    int errorHeight = height - codeHeight;
    
    printCode(beginX, beginY, width, codeHeight, normalPoint);
    printError(beginX, beginY+codeHeight, width, errorHeight, errorPoint);
}

void printCode(int beginX, int beginY, int width, int height, struct normalNode * normalPoint)
{    
    char str[10];

    indentNum = 0;

    textViewPosX = beginX + borderWidth;
    textViewPosY = beginY;
    textViewWidth = width - borderWidth;
    textViewHeight = height;
    currentX = textViewPosX;
    currentY = textViewPosY;

    setfillstyle(SOLID_FILL, LIGHTGRAY);
    bar(beginX, beginY, beginX + borderWidth, beginY + height);
     
    normalPoint = normalPoint->next;
    currentLine = 0;
    while (normalPoint) {
        while (normalPoint->line > currentLine) {
            currentY = currentY + charHeight;
            currentX = textViewPosX;
            currentLine ++;

            setcolor(DARKGRAY);
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
            itoa(currentLine, str, 10);
            outtextxy(currentX - borderWidth, currentY, str);
        }
        if (normalPoint->type == SPACE) {
            printTextWithColor(" ", WHITE);
        }
        else if (normalPoint->type == MACRO_VAL) {
            printDefine();
            printTextWithColor(normalPoint->content, CYAN);
        } 
        else if (normalPoint->type == HEADER) {
            printInclude();
            printTextWithColor(normalPoint->content, CYAN);
        }
        else if (normalPoint->addr != -1) {
            printTextWithColor(normalPoint->content, WHITE);
        }
        else {
            printf("%s", normalPoint->content);
            printTextWithColor(normalPoint->content, getColorWithType(normalPoint->type));
        }
        normalPoint = normalPoint->next;
    }
}

void printError(int beginX, int beginY,int width,int height, struct errorNode * errorPoint)
{
    char str[10];
    errorPoint = errorPoint->next; 
    currentX = beginX;
    currentY = beginY;

    while (errorPoint) {
        printTextWithColor("error", RED);
        printTextWithColor("(line: ", YELLOW);
        itoa(errorPoint->line, str, 10);
        printTextWithColor(str, YELLOW);
        printTextWithColor(")", YELLOW);
        if (errorPoint->type == UNUSED_IDENTIFIER_NUM || errorPoint->type == UNDEFINED_IDENTIFIER_NUM) {
            printTextWithColor(" '", YELLOW);
            printTextWithColor(errorPoint->content, YELLOW);
            printTextWithColor("'", YELLOW);
        }
        printTextWithColor(" ", WHITE);
        printTextWithColor(errorPoint->describe, WHITE);  
        currentY = currentY + charHeight; 
        currentX = beginX;
        errorPoint = errorPoint->next;
    } 
}

void printTextWithColor(const char * text, int color)
{
    int width = textWidth(text);
    
    if (width + currentX > textViewPosX + textViewWidth) {
        currentY = currentY + charHeight;
        currentX = textViewPosX;
    }

    setcolor(color);
    settextstyle(DEFAULT_FONT, HORIZ_DIR, 1);
    outtextxy(currentX, currentY, text);
    
    currentX = currentX + width;
}

int getColorWithType(int type)
{
    if (type <= 32 && type >= 1) {
        switch (type) {
            case AUTO :
            case CHAR :
            case CONST :
            case ENUM :
            case EXTERN :
            case FLOAT :
            case INT :
            case LONG :
            case REGISTER :
            case SHORT :
            case SIGNED :
            case STATIC :
            case STRUCT :
            case TYPEDEF :
            case UNION :
            case UNSIGNED :
            case VOID :
            case VOLATILE :
                return YELLOW;
            default :
                return LIGHTGREEN; 
        }
    } 
    else if (type == HEADER) {
        return CYAN;
    } 
    else {
        if (type == CHAR_VAL || type == STRING_VAL || type == SIN_QUE || type == DOU_QUE)
            return RED;
    } 
    return WHITE;
}

void printInclude()
{
    printTextWithColor("#include", RED);
}

void printDefine()
{
    printTextWithColor("#define", RED);
}

