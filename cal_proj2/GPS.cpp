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

void GPS::GUIInfo() const
{
	if (distrito == -1 || concelho == -1 || rua == -1)
	{
		return;
	}

	UI::ClearConsole();
	UI::DisplayFrame("DETALHES DA MORADA");

	vector<string> detalhesMorada;

	detalhesMorada.push_back(ruas[rua].nome);
	detalhesMorada.push_back(ruas[rua].codPostal + '\t' + ruas[rua].localidade);
	detalhesMorada.push_back("Concelho: " + concelhos[concelho]);
	detalhesMorada.push_back("Distrito: " + distritos[distrito]);

	UI::DisplayBox(detalhesMorada, 0);
	UI::PauseConsole();
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

		ostringstream os;
		
		os << currentEntry.nome << " " << currentEntry.localidade << " " << currentEntry.codPostal;
		currentEntry.toString = os.str();
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
			if (i != matchIndex && LevenshteinDistance(v[i], s) - minimumDistance)
			{
				matchVector.push_back(v[i]);
			}
		}
	}

	return matchVector;
}

vector<Rua> GPS::findMatch(const vector<Rua> &v, const string &s)
{
	vector<Rua> matchVector(v);

	make_heap(matchVector.begin(), matchVector.end(), [&s](const Rua &r1, const Rua &r2)
	{
		return LongestCommonSubstring(r1.toString, s) < LongestCommonSubstring(r2.toString, s);
	});

	return matchVector;
}

void GPS::GUITable(const vector<Rua> &v)
{
	vector<string> tableLabel = { " Nome", " Localidade", " Codigo" };

	const unsigned columnCount = 3;
	const unsigned rowCount = v.size() < 10 ? v.size() : 10;
	const int tableLength[columnCount] = { 36, 12, 10 };

	UI::DisplayTable(columnCount, tableLabel, tableLength);

	for (unsigned i = 0; i < rowCount; i++)
	{
		vector<string> tableRow(columnCount);

		tableRow[0] = v[i].nome;
		tableRow[1] = v[i].localidade;
		tableRow[2] = v[i].codPostal;

		UI::DisplayTableRow(columnCount, tableRow, tableLength);
	}
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
		if (v[i].toString == s)
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

	concelhos.clear();
	readConcelhos(distrito);

	while (true)
	{
		string concelhoEscolhido;

		if (!GUISelect(concelhos, concelhoEscolhido, strSelecionarConcelho))
		{
			return;
		}

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
				GUIRua();
			}
		}
	}
}

void GPS::GUIDistrito()
{
	while (true)
	{
		string distritoEscolhido;

		if (!GUISelect(distritos, distritoEscolhido, strSelecionarDistrito))
		{
			return;
		}

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
				GUIConcelho();
			}
		}
	}
}

void GPS::GUIRua()
{
	if (concelho == -1)
	{
		return;
	}

	ruas.clear();
	readRuas(concelho);

	while (true)
	{
		string ruaEscolhida;

		if (!GUISelectRua(ruaEscolhida))
		{
			return;
		}

		if (!ruaEscolhida.empty())
		{
			unsigned vectorIndex = indexRua(ruaEscolhida);

			if (vectorIndex == -1)
			{
				rua = seleccionarRua(ruaEscolhida);
			}
			else
			{
				rua = vectorIndex;
			}

			if (rua != -1)
			{
				GUIInfo();
			}
		}
	}
}

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

void GPS::GUINavigate(unsigned &vectorIndex, unsigned vectorSize) const
{
	int c = _getch();

	if (c == 72)
	{
		if (vectorIndex > 0)
		{
			vectorIndex--;
		}
		else
		{
			vectorIndex = vectorSize - 1;
		}
	}
	else if (c == 80)
	{
		if (vectorIndex < vectorSize - 1)
		{
			vectorIndex++;
		}
		else
		{
			vectorIndex = 0;
		}
	}
	else
	{
	}
}

void GPS::GUISwitchPage(unsigned &vectorIndex, unsigned vectorSize) const
{
	int c = _getch();

	if (c == 73)
	{
		if (vectorIndex < 10)
		{
			vectorIndex = 0;
		}
		else
		{
			vectorIndex -= 10;
		}
	}
	else if (c == 81)
	{
		if (vectorIndex + 10 < vectorSize)
		{
			vectorIndex += 10;
		}
	}
	else
	{
	}
}

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
			GUINavigate(vectorIndex, vectorSize);
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

bool GPS::GUISelect(const vector<string> &v, string &s, const char* prompt)
{
	int c;
	unsigned vectorIndex = 0;

	do
	{
		vector<string> matchedVector = findMatch(v, s);

		UI::ClearConsole();
		UI::DisplayFrame(prompt);
		printf(strSearchFormat, s.c_str());

		if (!matchedVector.empty())
		{
			UI::DisplayBox(matchedVector, vectorIndex);
		}

		UI::DisplayFrame(strNavigationBar);
		c = _getch();

		switch (c)
		{
		case 0:
			_getch();
			break;
		case 0x1b:
			return false;
			break;
		case 0xe0:
			GUISwitchPage(vectorIndex, matchedVector.size());
			break;
		case '\b':
			GUIRemoveChar(s, vectorIndex);
			break;
		default:
			GUIInsertChar(s, c, vectorIndex);
			break;
		}
	} while (c != '\r');

	return true;
}

bool GPS::GUISelectRua(string &userInput)
{
	int c;
	unsigned vectorIndex = 0;

	do
	{
		vector<Rua> matchedVector = findMatch(ruas, userInput);

		UI::ClearConsole();
		UI::DisplayFrame(strSelecionarRua);
		printf(strSearchFormat, userInput.c_str());

		if (!matchedVector.empty())
		{
			GUITable(matchedVector);
		}

		UI::DisplayFrame(strNavigationBar);
		c = _getch();

		switch (c)
		{
		case 0x00:
			_getch();
			break;
		case 0x1b:
			return false;
			break;
		case 0xe0:
			GUISwitchPage(vectorIndex, matchedVector.size());
			break;
		case '\b':
			GUIRemoveChar(userInput, vectorIndex);
			break;
		default:
			GUIInsertChar(userInput, c, vectorIndex);
			break;
		}
	} while (c != '\r');

	return true;
}

inline void GPS::GUIInsertChar(string &userInput, char c, unsigned &vectorIndex) const
{
	if (c >= 32 && c <= 255)
	{
		userInput.push_back(c);
		putc(c, stdout);
		vectorIndex = 0;
	}
}

inline void GPS::GUIRemoveChar(string &userInput, unsigned &vectorIndex) const
{
	if (userInput.size() > 0)
	{
		printf("\b \b");
		userInput.pop_back();
		vectorIndex = 0;
	}
}