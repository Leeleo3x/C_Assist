#include "C_Assist.h"

void menulist()
{
	int ky,y;
	int cmd;
	textbackground(0);
	clrscr();
	do
	{
		system("cls");
		textmode(C80);
		textbackground(8);
		textcolor(WHITE);
		window(1,1,80,25);
		clrscr();
		gotoxy(3,3);
		cprintf("1:Change Code Style\r\n");
		gotoxy(3,5);
		cprintf("2:Unfinshed!\r\n");
		gotoxy(3,7);
		cprintf("3:Unfinshed!\r\n");
		gotoxy(3,9);
		cprintf("4:Unfinshed!\r\n");
		gotoxy(3,11);
		cprintf("PRESS ESC TO QUIT...\r\n");
		y = 1;
		upBar(y);
		y+=2;
		cmd = 1;
		do
		{
			ky = bioskey(0);
			switch(ky)
			{
				case Key_Esc:
				{
					cmd = 0;
					ky = Key_Enter;
					break;
				}
				case Key_Down:
				{
					if (y<9)
					{
						upBar(y);
						y+=2;
						cmd = (cmd+1)%6;
					}
					break;
				}
				case Key_Up:
				{
					if (y>3)
					{
						downBar(y);
						y-=2;
						cmd = (cmd+1)%6;
					}
					break;
				}		
			}
		}while (ky != Key_Enter);
		switch(cmd)
		{
			case 0:
			{
				return;
				break;
			}
			case 1:
			{
				changeStyle();
				break;
			}
		}
	}while (1);
}

void upBar(int y)
{
	int i;
	typedef struct texel_struct
	{
		/* data */
		unsigned char ch;
		unsigned char attr;
	}texel;
	texel t;
	for (i=1; i<=22; i++)
	{
		gettext(i,y,i,y,&t);
		t.attr = 15;
		puttext(i,y,i,y,&t);
		gettext(i,y+2,i,y+2,&t);
		t.attr = 4;
		puttext(i,y+2,i,y+2,&t);
	}
	return;
}

void downBar(int y)
{
	int i;
	typedef struct texel_struct
	{
		/* data */
		unsigned char ch;
		unsigned char attr;
	}texel;
	texel t;
	for (i=1;i<=22;i++)
	{
		gettext(i,y,i,y,&t);
		t.attr = 15;
		puttext(i,y,i,y,&t);
		gettext(i,y-2,i,y-2,&t);
		t.attr = 4;
		puttext(i,y-2,i,y-2,&t);
	}
	return;
}

void changeStyle()
{	
	FILE *file;
	int key,x,y;
	char *fileName;
	char *buf;
	system("cls");
	cputs("Please input file name:");
	scanf("%s",fileName);
	system("cls");
	file = fopen(fileName,"r");
	textmode(C80);
	textcolor(WHITE);
	window(1,1,80,80);
	gotoxy(25,1);
	cputs("PRESS ESC TO QUIT...\n");
	gotoxy(1,1);
	while (!feof(file)) putch(fgetc(file));
	key = bioskey(0);
	while (1)
	{
		key = bioskey(0);
		x = wherex();
		y = wherey();
		switch(key)
		{
			case Key_Left:
				if (x>1) gotoxy(x-1,y);
				break;
			case Key_Right:
				if (x<25) gotoxy(x+1,y);
				break;
			case Key_Up:
				if (y>1) gotoxy(x,y-1);
				break;
			case Key_Down:
				if (y<80) gotoxy(x,y+1);
				break;
			case Key_Enter:
				if (y<80) gotoxy(1,y+1);
				break;
			case Key_Delete:
				if (x>1){
					gotoxy(x-1,y);
					putch(' ');
					gotoxy(x-1,y);
				}
				break;
			case Key_Esc:
				return;
			default:
				putch(key);	
		}
		
	}
	return;
}

int main()
{
	menulist();
	return 0;
}
