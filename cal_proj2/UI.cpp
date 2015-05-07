/*!
 * \file UI.cpp
 *
 * FEUP_CAL1415_2MIEIC01_D
 * \author Diogo Marques
 * \author Jose Taveira
 * \author Vitor Esteves
 *
 * \date Maio 2015
 *
 */

#include "UI.h"

const char* boxTop = "ษอออออออออออออออออออออออออออออออออออออออออป";
const char* boxBottom = "ศอออออออออออออออออออออออออออออออออออออออออผ";
const char* paddingLeft = "                  บ ";

namespace UI
{
	void Display(const char* s)
	{
		printf("%*s\n", 40 + (strlen(s) / 2), s);
	}

	void DisplayBox(const vector<string> &v)
	{
		UI::Display(boxTop);

		for (const string &s : v)
		{
			printf("%s%-40sบ\n", paddingLeft, s.c_str());
		}

		UI::Display(boxBottom);
	}

	static void ClearConsole()
	{
		system("cls");
	}

	template <typename T>
	static T readValue(const string& prompt)
	{
		string input;
		T val = T();

		cout << prompt;
		getline(cin, input);

		if (cin.fail())
		{
			cin.clear();

			if (!cin.eof())
			{
				throw InvalidParameter();
			}
		}

		stringstream ss(input);

		if (!(ss >> val) || ss.rdbuf()->in_avail() != 0)
		{
			throw InvalidParameter();
		}

		return val;
	}

	static string readString(const string& prompt)
	{
		string input;

		cout << prompt;
		getline(cin, input);

		if (cin.fail())
		{
			cin.clear();

			if (!cin.eof())
			{
				throw InvalidParameter();
			}
		}

		return input;
	}

	static void DisplayFrame(const char* s)
	{
		int stringLength = strlen(s);
		int stringBegin = (stringLength % 2 == 0) ? (23 - (stringLength / 2)) : (22 - (stringLength / 2));
		int stringEnd = 22 + (stringLength / 2);

		printf("\n\t\tษอออออออออออออออออออออออออออออออออออออออออออออออป\n\t\tบ ");

		for (int i = 0; i < stringBegin; i++)
		{
			putc(' ', stdout);
		}

		puts(s);

		for (int i = stringEnd; i < 45; i++)
		{
			putc(' ', stdout);
		}

		printf("บ\n\t\tศอออออออออออออออออออออออออออออออออออออออออออออออผ\n\n");
	}

	static void DisplayMessage(const string &msg)
	{
		cout << "\n" << msg << "\n" << "Press any key to continue...";
		cin.get();
		cin.clear();

		if (cin.rdbuf()->in_avail() != 0)
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}

	static void PauseConsole()
	{
		printf("Press any key to continue...");
		cin.get();
		cin.clear();

		if (cin.rdbuf()->in_avail() != 0)
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}

	string Format(unsigned n, unsigned w)
	{
		ostringstream os;
		os << setw(w) << right << fixed << n;
		return os.str();
	}
}