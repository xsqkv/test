#include <iostream>
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <termios.h>
#include <signal.h>
#include <stdlib.h>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

struct Task
{
	public:
	Task(string Question,vector<string> Answers,int RightAnswer)
	{
		question=Question;
		answers=Answers;
		rightAnswer=RightAnswer;
	}
	Task()
	{

	}
	string question;
	vector<string> answers;
	int rightAnswer;
};

void setcur(int x,int y)
{
	printf("\033[%d;%dH",y+1,x+1);
}

void write(int x,int y,string text,bool selected=false)
{
	if(selected)
	{
		printf("%s","\033[7m");
	}
	printf("\033[%d;%dH",y+1,x+1);;//TITLE
	printf("%s",text.c_str());
	printf("%s","\033[0m");
}

struct termios old_tio, new_tio;//GLOBAL SETTINGS VARIABLES

void clear() {system("clear");}//CLEAR SCREEN

void exit(int s)//FUNCTION FOR EXIT EVENT
{
	system("clear");
	tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);//Set old settings
	exit(1);
}

bool isAnswer(string text)
{
	return text[text.size()-1] == ' ';
}

int main(int argc,char** argv)
{
	int right = 0;
	int wrong = 0;

	struct sigaction sigIntHandler;         //
	sigIntHandler.sa_handler = exit;		//
	sigemptyset(&sigIntHandler.sa_mask);	//CATCH EXIT EVENT
	sigIntHandler.sa_flags = 0;				//
	sigaction(SIGINT, &sigIntHandler, NULL);//

	struct winsize buff;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &buff);//PUT SIZE INTO VARIABLE

	int w = buff.ws_col;//CONSOLE BUFFER WIDTH
	int h = buff.ws_row;//CONSOLE BUFFER HEIGHT

	char ch = 0;//CHAR FOR CATCHING ENTER
	unsigned int idx = pow(2,16)+1;//INDEX OF BUTTON

	string title = "Welcome to this test on the topic of the operator of electronic computers!";
	string start = "START TEST";
	string exit = "EXIT";

	clear();//CLEAR SCREEN

	write(w/2-(title.size()/2),0,title);//START BUTTON
	write(w/2-(start.size()/2),(h/2)-1,start,true);//START BUTTON
	write(w/2-(exit.size()/2),(h/2)+1,exit);//EXIT BUTTON

	tcgetattr(STDIN_FILENO,&old_tio);//Get settings in variable
	new_tio=old_tio;//Copy old settings to new
	new_tio.c_lflag &=(~ICANON & ~ECHO);//Set new flags
	tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);//Set new settings

	for(;;)//USER SELECT
	{
		ch = getchar();//CATCH SELECT
		if(ch == 10)
		{
			if(idx % 2 == 1)//IF SELECTED ENTER
			{
				break;
			}
			else//IF SELECTED EXIT
			{
				clear();
				tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);//Set old settings
				return 0;
			}
		}
		if (ch == '\033')//CATCH ARROW
		{
			getchar(); // skip the [
			switch(getchar())
			{
				case 'A'://ARROW UP
					idx++;
					break;
				case 'B'://ARROW DOWN
					idx--;
					break;
			}
		}
		if(idx % 2 == 1)//START BUTTON
		{
			write(w/2-(start.size()/2),(h/2)-1,start,true);
			write(w/2-(exit.size()/2),(h/2)+1,exit,false);
		}
		else//EXIT BUTTON
		{
			write(w/2-(start.size()/2),(h/2)-1,start,false);
			write(w/2-(exit.size()/2),(h/2)+1,exit,true);
		}
	}
	
	ifstream in("data");
	string line;
	int tasks = 0;
	while(getline(in,line))//COUNT LINES
	{
		if(line.size() == 0)
		{
			++tasks;
		}
	}
	++tasks;
	cout << tasks << endl;
	pause();
	Task task[tasks];
	
    if (in.is_open())//FILL TASKS
    {
		int idx=0;
		int count = 0;
        while (getline(in, line))
        {
			if(count==0)
			{
				task[idx].question=line;
			}
            if(isAnswer(line))
			{
				task[idx].rightAnswer = count-1;
			}
			else
			{
				task[idx].answers[count-1] = line;
			}
			if(line == "\n")
			{
				count=0;
				idx++;
			}
			count++;
        }
    }
    in.close();     // закрываем файл

	for(int i=0;i<tasks;i++)
	{
		clear();//CLS
		int l = task[i].answers.size();
		write(w/2-(task[i].question.size()/2),h/3,task[i].question);//WRITE QUESTION
		for(int j = 0;j<task[i].answers.size();j++)
		{
			write(w/2-(task[i].answers[j].size()/2),(h/2)+i+1,task[i].answers[j]);//WRITE ANSWERS
		}
		idx = pow(2,16)+3;
		for(;;)
		{
			ch = getchar();//CATCH SELECT
			if(ch == 10)//IF ENTER PRESSED
			{
				if(idx % 4 == 3)
				{
					break;
				}
				else if(idx % 4 == 2)
				{
					return 0;
				}
				else if(idx % 4 == 1)
				{
					return 0;
				}
				else if(idx % 4 == 0)
				{
					return 0;
				}
			}
			if (ch == '\033')//CATCH ARROW
			{
				getchar(); // skip the [
				switch(getchar())
				{
					case 'A'://ARROW UP
						idx++;
						break;
					case 'B'://ARROW DOWN
						idx--;
						break;
				}
			}
			for(int j = 0;j<task[i].answers.size();j++)
			{
				if(idx % 4 == 3)//1 BUTTON
				{
					write(w/2-(task[i].answers[j].size()/2),(h/2)-1,task[i].answers[j],true);
					write(w/2-(exit.size()/2),(h/2)+1,exit,false);
				}
				else if(idx % 4 == 2)//2 BUTTON
				{
					write(w/2-(start.size()/2),(h/2)-1,start,true);
					write(w/2-(exit.size()/2),(h/2)+1,exit,false);
				}
				else if(idx % 4 == 1)//3 BUTTON
				{
					write(w/2-(start.size()/2),(h/2)-1,start,true);
					write(w/2-(exit.size()/2),(h/2)+1,exit,false);
				}
				else if(idx % 4 == 0)//4 BUTTON
				{
					write(w/2-(start.size()/2),(h/2)-1,start,true);
					write(w/2-(exit.size()/2),(h/2)+1,exit,false);
				}
			}
		}
	}

	//Ваш результат 4/10 (40%)

	//    Ваша оценка 2.5

	//  ВЫЙТИ        ЗАНОВО
	tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);//Set old settings

	//clear();
	
	return 0;
}