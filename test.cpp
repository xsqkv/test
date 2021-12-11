#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cmath>

#ifdef __linux__
	#include <termios.h>
	#include <unistd.h>
	#include <sys/ioctl.h>
#else
	#include <windows.h>
	#include <conio.h>
#endif

using namespace std;

struct Task
{
	public:
	Task(string Question="",vector<string> Answers={},int RightAnswer=0)
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

#ifdef __linux__
	struct termios old_tio, new_tio;//GLOBAL SETTINGS VARIABLES
#endif

int w,h;

void setcur(int x,int y) {printf("\033[%d;%dH",y+1,x+1);}

void write(string text,int y,bool selected=false)
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

class onexit
{
	~onexit()
	{
		clear();
		#ifdef __linux__
			tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);//Set old settings
		#endif
		exit(1);
	}
};

bool isAnswer(string text)
{
	return text[text.size()-1] == ' ';
}

int main(int argc,char** argv)
{
	#ifdef _WIN32
		setlocale(LC_ALL, "ru-RU.utf8");
	#endif

	#ifdef __linux__
		struct winsize buff;
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &buff);//PUT SIZE INTO VARIABLE
		w = buff.ws_col;//CONSOLE BUFFER WIDTH
		h = buff.ws_row;//CONSOLE BUFFER HEIGHT
	#else
		CONSOLE_SCREEN_BUFFER_INFO csbi;
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
		w = csbi.srWindow.Right - csbi.srWindow.Left + 1;
		h = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
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

	const string title = "Welcome to this test on the topic of the operator of electronic computers!";
	const string start = "START TEST";
	const string exit = "EXIT";

	clear();//CLEAR SCREEN

	write(title,0);//TITLE
	
	write(start,h/2-1,true);//START BUON
	write(exit,h/2+1);//EXIT BUTTON

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
			write(start,h/2-1,true);
			write(exit,h/2+1);
		}
		else//EXIT BUTTON
		{
			write(start,h/2-1);
			write(exit,h/2+1,true);
		}
	}
	
	ifstream in;
	in.open("data");
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
		write(tasks[i].question,h/3);//WRITE QUESTION
		for(int j = 0;j<tasks[i].answers.size();j++)
		{
			int l = tasks[i].answers.size();
			if(idx % 4 == j)
			{
				write(tasks[i].answers[j],h/2+j+1,true);
			}
			else
			{
				write(tasks[i].answers[j],h/2+j+1,false);
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
					write(tasks[i].answers[j],h/2+j+1,true);
				}
				else
				{
					write(tasks[i].answers[j],h/2+j+1,false);
				}
			}

		}
	}

	clear();
	int percent = (right/(double)tasks.size())*100.0;
	write("Ваш результат: "+to_string(percent)+"%\n",h/2);
	int mark = floor(5*(right/(double)tasks.size())+0.5);
	write("Ваша оценка: "+to_string(mark)+"\n",h/2+2);

	//Ваш результат 4/10 (40%)

	//    Ваша оценка 2.5

	//  ВЫЙТИ        ЗАНОВО
	
#ifdef __linux__
	tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);//Set old settings
#endif

	for(int i=0;i<3;i++) cout << endl;
	cout << "Press enter for exit..."<<endl;
	while(getchar()!=10);
	clear();

	return 0;
}