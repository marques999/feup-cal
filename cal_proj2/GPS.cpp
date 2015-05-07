#include "GPS.h"
#include "UI.h"
#include "Algorithms.h"

GPS::GPS()
{
}

GPS& GPS::instance()
{
	static GPS INSTANCE;
	return INSTANCE;
}

bool GPS::read()
{
	ifstream in;
	//ifstream aux;
//	string auxEntry;
	string currentEntry;

	//	aux.open("Data/Portugal_files.txt");
	in.open("Data/Portugal.txt");

	if (!in.is_open() || in.eof() || in.bad())
	{
		return false;
	}

	//	if (!aux.is_open() || aux.eof() || aux.bad())
	//	{
	//		return false;
	//	}

	while (!in.eof())
	{
		getline(in, currentEntry);
		//	getline(in, auxEntry);

		if (currentEntry.empty())
		{
			break;
		}

		distritos.push_back(currentEntry);
	}

	in.close();

	return false;
}

void GPS::selecionarDistrito()
{
	concelhos.clear();
}

vector<string> GPS::findMatch(const vector<string> &v, const string &s)
{
	vector<string> matchVector;

	unsigned minimumDistance = 100;
	unsigned matchIndex = -1;

	for (unsigned i = 0; i < v.size(); i++)
	{
		unsigned newDistance = LevenshteinDistance(v[i], s);

		if (newDistance < minimumDistance)
		{
			matchIndex = i;
			minimumDistance = newDistance;
		}
	}

	if (matchIndex != -1)
	{
		matchVector.push_back(v[matchIndex]);

		for (unsigned i = 0; i < v.size(); i++)
		{
			if (i != matchIndex && LevenshteinDistance(v[i], s) == minimumDistance)
			{
				matchVector.push_back(v[i]);
			}
		}
	}

	return matchVector;
}

#include <Windows.h>

void setEcho(bool enable)
{
#ifdef WIN32

	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;

	GetConsoleMode(hStdin, &mode);

	if (!enable)
	{
		mode &= ~ENABLE_ECHO_INPUT;
	}
	else
	{
		mode |= ENABLE_ECHO_INPUT;
	}

	SetConsoleMode(hStdin, mode);

#else

	struct termios tty;

	tcgetattr(STDIN_FILENO, &tty);

	if (!enable)
	{
		tty.c_lflag &= ~ECHO;
	}
	else
	{
		tty.c_lflag |= ECHO;
	}

	(void) tcsetattr(STDIN_FILENO, TCSANOW, &tty);

#endif
}

string GPS::enterText()
{
	setEcho(false);
	string passwd;

	char c;

	do
	{
		system("cls");
		cout << passwd << '\xdb' << endl;

		vector<string> index = findMatch(distritos, passwd);
		cout << endl;
		if (!index.empty())
		{
			UI::DisplayBox(index);
		}

		c = _getch();

		switch (c)
		{
		case 0:
		{
			_getch();
			break;
		}
		case '\b':
		{
			if (passwd.size() > 0)
			{
				cout << "\b \b";
				passwd.pop_back();
			}
			break;
		}
		default:
		{
			if (c >= 32 && c <= 255)
			{
				passwd.push_back(c);
				cout << c;
			}
			break;
		}
		}
	} while (c != '\r');
	return passwd;
}