/*
 * =====================================================================================
 *
 *       Filename:  C_Assist.h
 *
 *    Description:  The head file for C_Assist;
 *
 *        Version:  1.0
 *        Created:  03/08/2014 21:14:25
 *       Revision:  none
 *       Compiler:  watcom
 *
 *         Author:  Leo Lee (), leeleo3x@gmail.com
 *   Organization:  ZJU
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <math.h>
#include "graphics.h"
#include <conio.h>
#include <stdlib.h>
#include <dos.h>
#include <string.h>

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
