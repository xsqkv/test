#include <iostream>

#ifdef __linux__
	#include <unistd.h>
	#include <sys/ioctl.h>
#else
	#include <windows.h>
	#include <conio.h>
#endif

using namespace std;

#ifdef __linux__
	struct winsize buff;
    auto a = ioctl(STDOUT_FILENO, TIOCGWINSZ, &buff);//PUT SIZE INTO VARIABLE
    int	w = buff.ws_col;//CONSOLE BUFFER WIDTH
    int	h = buff.ws_row;//CONSOLE BUFFER HEIGHT
#else
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	auto a = GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	int w = csbi.srWindow.Right - csbi.srWindow.Left + 1;
	int h = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#endif

void cwrite(string text,int y,bool selected=false)
{
	if(selected)
	{
		printf("%s","\033[7m");
	}
	printf("\033[%d;%dH",y+1,(w/2-(text.size()/2))+1);
	printf("%s",text.c_str());
	printf("%s","\033[0m");
}

void clear() //CLEAR SCREEN
{
	#ifdef __linux__
		system("clear");
	#else
		system("cls");
	#endif
}