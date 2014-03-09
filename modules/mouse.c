#include "../C_Assist.h"

#define MSIZE 5

void * imagep;
int mouseLeftFlag = 0;

/*
 * Name : void initmouse()
 * Parameters : NONE
 * Return Value : NONE
 * Usage : initialize the mouse module
 */

void initmouse()
{
    union REGS ireg,oreg;
    int ret;
    ireg.w.ax = 0;
    int86(0x33,&ireg,&oreg);
    ret = oreg.w.ax;
    if (ret != 0xFFFF)
    {
        printf("Mouse is not installed.");
        exit(1);
    }
   ireg.w.ax = 0x0007; /* set horizontal range */
   ireg.w.cx = 0;
   ireg.w.dx = 800-1;
   int86(0x33, &ireg,&oreg);
   ireg.w.ax = 0x0008; /* set vertical range */
   ireg.w.cx = 0;
   ireg.w.dx = 600-1;
   int86(0x33, &ireg,&oreg);
}

/*
 * Name : void getMousePosition(int *x,int *y)
 * Parameters : x,y-the pointer of the coordinate of the mouse
 * Return Value : NONE;
 * Usage : fetch the position of the mouse
 */
void getMousePosition(int *x,int *y)
{
    union REGS ireg,oreg;
    ireg.w.ax = 3;
    int86(0x33,&ireg,&oreg);
    *x = oreg.w.cx;
    *y = oreg.w.dx;
}

/*
 * Name : int leftPress()
 * Parameters : NONE
 * Return Value : 1-left pressed
 *                0-not yet
 * Usage : exam if the mouse left button pressed
 */
int leftPress()
{
    union REGS ireg,oreg;
    ireg.w.ax = 3;
    int86(0x33,&ireg,&oreg);
    delay(30);
    if(oreg.w.bx & 1)
    {
        mouseLeftFlag = 1;
    }
    return oreg.w.bx & 1;
}

/*
 * Name : int rightPress()
 * Parameters : NONE
 * Return Value : 1-right pressed
 *                0-not yet
 * Usage : exam if the mouse right button pressed
 *
 */
int rightPress()
{
    union REGS ireg,oreg;
    ireg.w.ax = 3;
    int86(0x33,&ireg,&oreg);
    return oreg.w.bx&2?1:0;
}

/*
 * Name : void createMouse()
 * Parameters : NONE
 * Return Value : NONE
 * Usage : draw a mouse icorn on screen
 */
void createMouse()
{
    unsigned long gsize;
    initmouse();
    drawMouse();
    gsize = imagesize(100-MSIZE,100-MSIZE,100+MSIZE,100+MSIZE);
    imagep = malloc(gsize);
    getimage(100-MSIZE,100-MSIZE,100+MSIZE,100+MSIZE,imagep);
    cleardevice();
}

/*
 * Name : int runMouse(int *x,int *y)
 * Parameters : x,y-the coordinate of the mouse position
 * Return Value 0-no click event happened
 *              1-right button was clicked
 *              2-left button was clicked
 * Usage : create the current image of the mouse,
 *         get the current status of the mouse
 */
int runMouse(int *xx,int *yy)
{
    int x,y;
    getMousePosition(&x,&y);
    if (*xx != x || *yy != y)
    {
        putimage(*xx,*yy,imagep,XOR_PUT);
        *xx = x;
        *yy = y;
        putimage(x,y,imagep,XOR_PUT);
    }
    if (leftPress() == 1 || mouseLeftFlag == 1)
    {
        return 1;
    }
    if (rightPress() == 1)
    {
        return 2;
    }
    return 0;
}
