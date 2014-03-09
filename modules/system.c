/*
 * =====================================================================================
 *
 *       Filename:  system.c
 *
 *    Description:  the module for system initialization
 *
 *        Version:  1.0
 *        Created:  03/09/2014 12:31:12
 *       Revision:  none
 *       Compiler:  watcom
 *
 *         Author:  Leo Lee (), leeleo3x@gmail.com
 *   Organization:  ZJU
 *
 * =====================================================================================
 */
#include "../C_Assist.h"

/*
 * Name : void GraphInit()
 * Parameters : NONE
 * Return Value : NONE
 * Usage : Initialize the graphic,
 *         The program will break down if there is any wrong during initializing
 */

void GraphInit()
{
    int GraphDriver = DETECT;
    int GraphMode = VESA_800x600x8bit;
    int ErrorCode;
    initgraph(&GraphDriver,&GraphMode,"");
    /*ErrorCode = graphresult();
    int (ErrorCode != grOk)
    {
        printf("Graphics System Error: %s\n",grapherrormsg(ErrorCode));
        exit(1);
    }*/
}

