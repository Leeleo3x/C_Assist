/*
 *
 *  Filename:  C_Assist.h
 *  Created:  03/08/2014
 *  Author:  Leo Lee , leeleo3x@gmail.com
 *
 */

#include <stdio.h>
#include <math.h>
//#include "graphics.h"
//#include <conio.h>
#include <stdlib.h>
//#include <dos.h>
#include <string.h>

#define MAX_LINE_LENGTH 128
#define MAX_LINE_SUM 500
extern void * imagep;
extern int mouseLeftFlag;

/* objects module */
void GraphInit();
void buttonPressed(int x,int y,int length,int width);
void buttonWithText(int x,int y,int length,int width,char *text);
void drawMouse();
/* mouse moudule */
void initmouse();
void createMouse();
int leftPress();
int rightPress();
int runMouse(int *x,int *y);
