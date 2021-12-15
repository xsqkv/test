#ifdef __linux__
	#include <termios.h>
	#include <unistd.h>
	#include <sys/ioctl.h>
#endif

#ifdef __linux__
	struct termios old_tio, new_tio;//GLOBAL SETTINGS VARIABLES
#endif

class onexit
{
	~onexit()
	{
		#ifdef __linux__
		system("clear");
		#else
		system("cls");
		#endif

		#ifdef __linux__
			tcsetattr(STDIN_FILENO,TCSANOW,&old_tio);//Set old settings
		#endif

		exit(1);
	}
};