/*!
 * \file GPS.cpp
 *
 * FEUP_CAL1415_2MIEIC01_D
 * \author Diogo Marques
 * \author Jose Taveira
 * \author Vitor Esteves
 *
 * \date Maio 2015
 *
 */

#include "GPS.h"
#include "UI.h"
#include "Algorithms.h"

GPS::GPS()
{
	readDistritos();
	distrito = -1;
	concelho = -1;
	rua = string();
}

GPS& GPS::instance()
{
	static GPS INSTANCE;
	return INSTANCE;
}

bool GPS::readConcelhos(unsigned vectorIndex)
{
	ifstream in;
	string currentEntry;

	char concelhosFilename[128];

	sprintf_s(concelhosFilename, "Data/%d.txt", vectorIndex);
	in.open(concelhosFilename);

	if (!in.is_open() || in.eof() || in.bad())
	{
		return false;
	}

	while (!in.eof())
	{
		getline(in, currentEntry);

		if (currentEntry.empty())
		{
			break;
		}

		concelhos.push_back(currentEntry);
	}

	return true;
}

bool GPS::readDistritos()
{
	ifstream in;
	string currentEntry;

	in.open("Data/index.txt");

	if (!in.is_open() || in.eof() || in.bad())
	{
		return false;
	}

	while (!in.eof())
	{
		getline(in, currentEntry);

		if (currentEntry.empty())
		{
			break;
		}

		distritos.push_back(currentEntry);
	}

	return true;
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

unsigned GPS::index(const vector<string> &v, const string &s)
{
	for (unsigned i = 0; i < distritos.size(); i++)
	{
		if (v[i] == s)
		{
			return i;
		}
	}

	return -1;
}

unsigned GPS::indexConcelho(const string &concelhoEscolhido)
{
	return index(concelhos, concelhoEscolhido);
}

unsigned GPS::indexDistrito(const string &distritoEscolhido)
{
	return index(distritos, distritoEscolhido);
}

void GPS::GUIConcelho()
{
	if (distrito == -1)
	{
		return;
	}

	readConcelhos(distrito);

	while (true)
	{
		string concelhoEscolhido = GUISelect(concelhos, "SELECCIONAR CONCELHO");

		if (!concelhoEscolhido.empty())
		{
			unsigned vectorIndex = indexConcelho(concelhoEscolhido);

			if (vectorIndex == -1)
			{
				concelho = seleccionarConcelho(concelhoEscolhido);
			}
			else
			{
				concelho = vectorIndex;
			}

			if (concelho != -1)
			{
				break;
			}
		}
		else
		{
			//	throw InvalidParameter();
		}
	}
}


void GPS::GUIDistrito()
{
	while (true)
	{
		string distritoEscolhido = GUISelect(distritos, "SELECCIONAR DISTRITO");

		if (!distritoEscolhido.empty())
		{
			unsigned vectorIndex = indexDistrito(distritoEscolhido);

			if (vectorIndex == -1)
			{
				distrito = seleccionarDistrito(distritoEscolhido);
			}
			else
			{
				distrito = vectorIndex;
			}

			if (distrito != -1)
			{
				break;
			}
		}
		else
		{
			// throw
		}
	}

	GUIConcelho();
}

// nome da rua, avenida ou praça deve começar pelo nome respetivo
// caso contrario, se começar por uma letra do alfabeto sera entendido como uma localidade
// caso contrario, se começar por um numero sera entendido como codigo-postal

unsigned GPS::seleccionarDistrito(const string &s)
{
	return GUISelectAux(distritos, s, "SELECCIONAR DISTRITO");
}

unsigned GPS::seleccionarConcelho(const string &s)
{
	return GUISelectAux(concelhos, s, "SELECCIONAR CONCELHO");
}

void GPS::GUIRua()
{
	// ler ruas

	const int rowCount = 3;
	const int tableLength[rowCount] = { 24, 12, 8 };

	for (const Rua &r : ruas)
	{
		vector<string> tableRow(rowCount);

		tableRow[0] = r.nome;
		tableRow[1] = r.localidade;
		tableRow[2] = r.codPostal;

		UI::DisplayTableRow(rowCount, tableRow, tableLength);
	}

	UI::PauseConsole();
}

//allow page down!

unsigned GPS::GUISelectAux(const vector<string> &v, const string &s, const char* prompt)
{
	vector<string> previousMatches = findMatch(v, s);

	const unsigned vectorSize = previousMatches.size();
	unsigned vectorIndex = 0;

	while (true)
	{
		UI::ClearConsole();
		UI::DisplayFrame(prompt);

		for (size_t i = 0; i < vectorSize; i++)
		{
			if (i == vectorIndex)
			{
				printf("                -> %s\n", previousMatches[i].c_str());
			}
			else
			{
				printf("                   %s\n", previousMatches[i].c_str());
			}
		}

		UI::DisplayFrame("<ENTER> validar selec\x87\xc6o    <ESC> voltar");

		int c = _getch();

		if (c == 0xe0)
		{
			switch (_getch())
			{
			case 72:
			{
				if (vectorIndex > 0)
				{
					vectorIndex--;
				}
				else
				{
					vectorIndex = vectorSize - 1;
				}

				break;
			}
			case 80:
			{
				if (vectorIndex < vectorSize - 1)
				{
					vectorIndex++;
				}
				else
				{
					vectorIndex = 0;
				}

				break;
			}
			}
		}
		else
		{
			if (c == 0x0d || c == 0x0a)
			{
				return vectorIndex;
			}

			if (c == 0x1b)
			{
				return -1;
			}
		}
	}
}

string GPS::GUISelect(const vector<string> &v, const char* prompt)
{
	setEcho(false);
	string passwd;

	char c;

	do
	{
		UI::ClearConsole();
		UI::DisplayFrame(prompt);
		printf("                Pesquisar: %s\xdb\n\n", passwd.c_str());

		vector<string> index = findMatch(v, passwd);

		if (!index.empty())
		{
			UI::DisplayBox(index);
		}

		UI::DisplayFrame("<ENTER> validar selec\x87\xc6o    <ESC> voltar");
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