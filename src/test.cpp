#include <iostream>
#include <string>
#include <fstream>
#include <cmath>

#include "lib/task.h"
#include "lib/events.h"
#include "lib/termi0.h"
#include "lib/test.h"

#ifdef _WIN32
	#include <windows.h>
	#include <conio.h>
#endif

using namespace std;

int main(int argc,char** argv)
{
	#ifdef _WIN32
		setlocale(LC_ALL, "ru-RU.utf8");
	#endif

	#ifdef __linux__
		tcgetattr(STDIN_FILENO,&old_tio);//Get settings in variable
		new_tio=old_tio;//Copy old settings to new
		new_tio.c_lflag &=(~ICANON & ~ECHO);//Set new flags
		tcsetattr(STDIN_FILENO,TCSANOW,&new_tio);//Set new settings
	#endif

	double right = 0;

	char ch = 0;//CHAR FOR CATCHING ENTER
	long long idx = pow(2,16)+1;//INDEX OF BUTTON

	clear();//CLEAR SCREEN

	cwrite(title,0);//TITLE
	
	cwrite(start,h/2-1,true);//START BUON
	cwrite(exit_,h/2+1);//EXIT BUTTON

	for(;;)//USER SELECT
	{
		#ifdef __linux__
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
						idx--;
						break;
					case 'B'://ARROW DOWN
						idx++;
						break;
				}
			}
		#else
			ch = _getch();
			if(ch == '\r')
			{
				if(idx % 2 == 1)//IF SELECTED ENTER
				{
					break;
				}
				else//IF SELECTED EXIT
				{
					clear();
					//tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);//Set old settings
					return 0;
				}
			}
			switch (_getch())
			{
				case 72://ARROW UP
					idx--;
					break;
				case 80://ARROW DOWN
					idx++;
					break;
			}
		#endif
		
		if(idx % 2 == 1)//START BUTTON	
		{
			cwrite(start,h/2-1,true);
			cwrite(exit_,h/2+1);
		}
		else//EXIT BUTTON
		{
			cwrite(start,h/2-1);
			cwrite(exit_,h/2+1,true);
		}
	}
	
	ifstream in;
	in.open("data",ifstream::in);
	string line;
	vector<Task> tasks;
	idx=0;
    if (in.is_open())//FILL TASKS
    {
		int count = 0;
        while (getline(in, line))
        {
			if(line.size() == 0)
			{
				count=0;
				idx++;
				continue;
			}
			if(count==0)
			{
				tasks.push_back(Task(line,{},0));
				count++;
				continue;
			}
            if(isAnswer(line))
			{
				tasks[idx].rightAnswer = count-1;//Add index of right number
				int len = line.size();
				tasks[idx].answers.push_back(line.substr(0,len-1));//Add answer
			}	
			else
			{
				tasks[idx].answers.push_back(line);//Add answer
			}
			count++;
        }
    }
    in.close();//Close File
	idx = pow(2,16);
	for(int i = 0;i < tasks.size();i++)
	{
		clear();//CLS
		int l = tasks[i].answers.size();
		cwrite(tasks[i].question,h/3);//cwrite QUESTION
		for(int j = 0;j<tasks[i].answers.size();j++)//cwrite ANSWERS
		{
			int l = tasks[i].answers.size();
			if(idx % 4 == j)
			{
				cwrite(tasks[i].answers[j],h/2+j+1,true);
			}
			else
			{
				cwrite(tasks[i].answers[j],h/2+j+1,false);
			}
		}
		for(;;)
		{
			#if __linux__
				ch = getchar();//CATCH SELECT
				if(ch == 10)//IF ENTER PRESSED
				{
					for(int j = 0;j<tasks[i].answers.size();j++)
					{
						if(idx % 4 == tasks[i].rightAnswer)
						{
							right++;
							break;
						}
					}
					break;
				}
				if (ch == '\033')//CATCH ARROW
				{
					getchar(); // skip the [
					switch(getchar())
					{
						case 'A'://ARROW UP
							idx--;
							break;
						case 'B'://ARROW DOWN
							idx++;
							break;
					}
				}
			#else
				ch = _getch();//CATCH SELECT
				if(ch == '\r')//IF ENTER PRESSED
				{
					for(int j = 0;j<tasks[i].answers.size();j++)
					{
						if(idx % 4 == tasks[i].rightAnswer)
						{
							right++;
							break;
						}
					}
					break;
				}
				switch(_getch())
				{
					case 72://ARROW UP
						idx--;
						break;
					case 80://ARROW DOWN
						idx++;
						break;
				}
			#endif
			for(int j = 0;j<tasks[i].answers.size();j++)
			{
				int l = tasks[i].answers.size();
				if(idx % 4 == j)//1 BUTTON
				{
					cwrite(tasks[i].answers[j],h/2+j+1,true);
				}
				else
				{
					cwrite(tasks[i].answers[j],h/2+j+1,false);
				}
			}
		}
	}

	clear();

	int percent = (right/(double)tasks.size())*100.0;
	cwrite("Ваш результат: "+to_string(percent)+"%\n",h/2);
	int mark = floor(5*(right/(double)tasks.size())+0.5);
	cwrite("Ваша оценка: "+to_string(mark)+"\n",h/2+2);
	
	#ifdef __linux__
		tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);//Set old settings
	#endif

	for(int i=0;i<3;i++) cout << endl;

	cout << "Press enter for exit..."<<endl;

	while(getchar()!=10);
	
	clear();

	return 0;
}