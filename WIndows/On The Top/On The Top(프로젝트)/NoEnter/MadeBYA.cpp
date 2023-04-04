#include <stdio.h>
#include <conio.h>
#include "GameTool.h"
#include "Console.h"
#include "Main.h"
void MadeBY()
{
	Console();
	Gotoxy(2, 1); setColor(White, Black); printf("Made By DaLae");
	Gotoxy(2, 2); setColor(White, Black); printf("Made By ÀÌÇÑ¼ö");
	Gotoxy(2, 3); setColor(White, Black); printf("Blog : http://blog.naver.com/dalae37");
	Gotoxy(2, 4); setColor(White, Black); printf("Thanks To");
	Gotoxy(12, 4); setColor(Yellow, Black); printf("ZEROPEN");
	getch();
	MainA();
}