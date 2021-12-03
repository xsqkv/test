#include <iostream>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

void setcur(int x,int y)
{
	printf("\033[%d;%dH",x+1,y+1);
}
void getcur(int *x,int *y)
{
	printf("\033[6n");
   	scanf("\033[%d;%dR", x, y);
}

int main()
{
	struct winsize buff;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &buff);
	int w = buff.ws_col;
	int h = buff.ws_row;
	
	return 0;
}
