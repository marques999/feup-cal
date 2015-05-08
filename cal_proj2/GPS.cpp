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
	try
	{
		readDistritos();
	}
	catch (FileIOException &e)
	{
		UI::DisplayMessage(e.str());
	}

	distrito = -1;
	concelho = -1;
	rua = -1;
}

GPS& GPS::instance()
{
	static GPS gps;
	return gps;
}

void GPS::GUIMenu()
{
	GUIDistrito();
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
		throw FileIOException(concelhosFilename);
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

bool GPS::readRuas(unsigned vectorIndex)
{
	ifstream in;
	Rua currentEntry;

	char ruasFilename[128];

	sprintf_s(ruasFilename, "Data/%d/%d.txt", distrito, vectorIndex);
	in.open(ruasFilename);

	if (!in.is_open() || in.eof() || in.bad())
	{
		throw FileIOException(ruasFilename);
	}

	while (!in.eof())
	{
		getline(in, currentEntry.nome);

		if (currentEntry.nome.empty())
		{
			break;
		}

		getline(in, currentEntry.localidade);

		if (currentEntry.nome.empty())
		{
			break;
		}

		getline(in, currentEntry.codPostal);

		if (currentEntry.nome.empty())
		{
			break;
		}

		ruas.push_back(currentEntry);
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
		throw FileIOException("index.txt");
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

vector<Rua> GPS::findMatch(const vector<Rua> &v, const string &s)
{
	vector<Rua> matchVector;

	unsigned minimumDistance = 100;
	unsigned matchIndex = -1;

	for (unsigned i = 0; i < v.size(); i++)
	{
		unsigned newDistance = LevenshteinDistance(v[i].nome, s);

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
			if (i != matchIndex && LevenshteinDistance(v[i].nome, s) == minimumDistance)
			{
				matchVector.push_back(v[i]);
			}
		}
	}

	return matchVector;
}

void GPS::GUITable()
{
	vector<string> tableLabel = { " Nome", " Localidade", " Codigo" };

	const int rowCount = 3;
	const int tableLength[rowCount] = { 36, 12, 10 };

	UI::DisplayTable(rowCount, tableLabel, tableLength);

	for (const Rua &r : ruas)
	{
		vector<string> tableRow(rowCount);

		tableRow[0] = r.nome;
		tableRow[1] = r.localidade;
		tableRow[2] = r.codPostal;

		UI::DisplayTableRow(rowCount, tableRow, tableLength);
	}
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

bool operator==(const Rua &r1, const Rua &r2)
{
	return r1.nome == r2.nome && r1.localidade == r2.localidade;
}

unsigned GPS::index(const vector<string> &v, const string &s)
{
	for (unsigned i = 0; i < v.size(); i++)
	{
		if (v[i] == s)
		{
			return i;
		}
	}

	return -1;
}

unsigned GPS::index(const vector<Rua> &v, const string &s)
{
	for (unsigned i = 0; i < v.size(); i++)
	{
		if (v[i].nome == s)
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

unsigned GPS::indexRua(const string &ruaEscolhida)
{
	return index(ruas, ruaEscolhida);
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
		string concelhoEscolhido = GUISelect(concelhos, strSelecionarConcelho);

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

	GUIRua();
}

void GPS::GUIDistrito()
{
	while (true)
	{
		string distritoEscolhido = GUISelect(distritos, strSelecionarDistrito);

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
	return GUISelectAux(distritos, s, strSelecionarDistrito);
}

unsigned GPS::seleccionarConcelho(const string &s)
{
	return GUISelectAux(concelhos, s, strSelecionarConcelho);
}

unsigned GPS::seleccionarRua(const string &s)
{
	vector<string> nomesRuas;

	for (const Rua &r : ruas)
	{
		nomesRuas.push_back(r.nome);
	}

	return GUISelectAux(nomesRuas, s, strSelecionarRua);
}

void GPS::GUIRua()
{
	if (concelho == -1)
	{
		return;
	}

	readRuas(concelho);

	while (true)
	{
		string ruaEscolhida = GUISelectRua();

		if (!ruaEscolhida.empty())
		{
			unsigned vectorIndex = indexRua(ruaEscolhida);

			if (vectorIndex == -1)
			{
				concelho = seleccionarRua(ruaEscolhida);
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

		UI::DisplayFrame(strNavigationBar);

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
				return index(v, previousMatches[vectorIndex]);
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
	string userInput;

	char c;

	do
	{
		UI::ClearConsole();
		UI::DisplayFrame(prompt);
		printf(strSearchFormat, userInput.c_str());

		vector<string> index = findMatch(v, userInput);

		if (!index.empty())
		{
			UI::DisplayBox(index);
		}

		UI::DisplayFrame(strNavigationBar);
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
			if (userInput.size() > 0)
			{
				printf("\b \b");
				userInput.pop_back();
			}
			break;
		}

		default:
		{
			if (c >= 32 && c <= 255)
			{
				userInput.push_back(c);
				putc(c, stdout);
			}
			break;
		}
		}
	} while (c != '\r');

	return userInput;
}

string GPS::GUISelectRua()
{
	string userInput;

	char c;

	do
	{
		UI::ClearConsole();
		UI::DisplayFrame(strSelecionarRua);
		printf(strSearchFormat, userInput.c_str());

		vector<Rua> index = findMatch(ruas, userInput);

		if (!index.empty())
		{
			GUITable();
		}

		UI::DisplayFrame(strNavigationBar);
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
			if (userInput.size() > 0)
			{
				printf("\b \b");
				userInput.pop_back();
			}

			break;
		}

		default:
		{
			if (c >= 32 && c <= 255)
			{
				userInput.push_back(c);
				putc(c, stdout);
			}

			break;
		}
		}
	} while (c != '\r');

	return userInput;
}