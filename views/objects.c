/*
 * =====================================================================================
 *
 *       Filename:  objects.c
 *
 *    Description:
 *
 *        Version:  1.0
 *        Created:  03/08/2014 21:19:12
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Leo Lee (), leeleo3x@gmail.com
 *   Organization:
 *
 * =====================================================================================
 */
#include "../C_Assist.h"

#define MSIZE 5
/*
 * Name : void buttonDown(int x,int y,int width,int length)
 * Parameters : x-the coordinate x of the button
 *              y-the coordinate y of the button
 *              width-the width of the button
 *              length-the length of the button
 * Ruturn Value : NONE
 * Usage : draw a buttonDown on the screen
 */

void buttonDown(int x,int y,int length,int width)
{
    setcolor(DARKGRAY);
    line(x,y,x+length,y);
    line(x+length,y,x+length,y+width);
    setcolor(WHITE);
    line(x,y,x,y+width);
    line(x,y+width,x+length,y+width);
}

/*
 * Name : void buttonUp(int x,int y,int width,int length)
 * Parameters : x-the coordinate x of the button
 *              y-the coordinate y of the button
 *              width-the width of the button
 *              length-the length of the button
 * Ruturn Value : NONE
 * Usage : draw a button up on the screen
 */
void buttonUp(int x,int y,int length,int width)
{
    setcolor(DARKGRAY);
    line(x,y,x,y+width);
    line(x,y+width,x+length,y+width);
    setcolor(WHITE);
    line(x,y,x+length,y);
    line(x+length,y,x+length,y+width);
    setcolor(BLACK);
}

/*
 * Name : void buttonPressed(int x,int y,int length,int width)
 * Parameters : x-the coordinate x of the button
 *              y-the coordinate y of the button
 *              width-the width of the button
 *              length-the length of the button
 * Ruturn Value : NONE
 * Usage : show the motion of pressing button
 */

void buttonPressed(int x, int y,int length,int width)
{
    buttonDown(x,y,length,width);
    delay(3000);
    buttonUp(x,y,length,width);
}

/*
 * Name : void buttonWithText(int x,int y,int length,int width,char *text)
 * Parameters : x-the coordinate x of the button
 *              y-the coordinate y of the button
 *              width-the width of the button
 *              length-the length of the button
 * Ruturn Value : NONE
 * Usage : draw a button with text
 */

void buttonWithText(int x,int y,int length,int width,char *text)
{
    setfillstyle(SOLID_FILL,LIGHTGRAY);
    bar(x,y,x+length,y+width);
    buttonUp(x,y,length,width);
    setcolor(YELLOW);
    settextstyle(1,0,1);
    outtextxy(x+3,y+width/2-7,text);
}

/*
 * Name : void drawMouse()
 * Parameters : NONE
 * Return Value : NONE
 * Usage : create a mouse image
 */
void drawMouse()
{
    setcolor(YELLOW);
    rectangle(100-MSIZE,100-MSIZE,100+MSIZE,100+MSIZE);
    line(100-MSIZE,100-MSIZE,100+MSIZE,100+MSIZE);
    line(100-MSIZE,100+MSIZE,100+MSIZE,100-MSIZE);
}
