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
const char* tableTop = "ษอออออออออออออออออออออออออออออออออออออออออออออออออป";
const char* tableBottom = "ศอออออออออออออออออออออออออออออออออออออออออออออออออผ";
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

		size_t maximumIndex = v.size() < 10 ? v.size() : 10;

		for (size_t i = 0; i < maximumIndex; i++)
		{
			printf("%s%-40sบ\n", paddingLeft, v[i].c_str());
		}

		UI::Display(boxBottom);
	}

	void ClearConsole()
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

	void DisplayTable(int c, const vector<string> &labels, const int length[])
	{
		string frameTop;
		string frameBottom;
		stringstream ss;

		frameTop.push_back('\xc9');
		frameBottom.push_back('\xc8');
		ss << '\xba';

		for (int i = 0; i < c; i++)
		{
			for (int j = 0; j < length[i]; j++)
			{
				frameTop.push_back('\xcd');
				frameBottom.push_back('\xcd');
			}

			ss << left << setw(length[i]) << labels[i];
			ss << right << '\xba';

			if (i == c - 1)
			{
				frameTop.push_back('\xbb');
				frameBottom.push_back('\xbc');
			}
			else
			{
				frameTop.push_back('\xcb');
				frameBottom.push_back('\xca');
			}
		}

		UI::Display(tableTop);
		UI::Display(tableBottom);
	}

	void DisplayTableRow(int c, const vector<string> &labels, const int length[])
	{
		stringstream ss;

		ss << " ";

		for (int i = 0; i < c; i++)
		{
			ss << left << setw(length[i] - 1) << labels[i];

			if (i != c - 1)
			{
				ss << right << "  ";
			}
		}

		UI::Display(ss.str().c_str());
	}

	void DisplayFrame(const char* s)
	{
		int stringLength = strlen(s);
		int stringBegin = (stringLength % 2 == 0) ? (23 - (stringLength / 2)) : (22 - (stringLength / 2));
		int stringEnd = 22 + (stringLength / 2);

		printf("\n\t\tษอออออออออออออออออออออออออออออออออออออออออออออออป\n\t\tบ ");

		for (int i = 0; i < stringBegin; i++)
		{
			putc(' ', stdout);
		}

		printf(s);

		for (int i = stringEnd; i < 45; i++)
		{
			putc(' ', stdout);
		}

		printf("บ\n\t\tศอออออออออออออออออออออออออออออออออออออออออออออออผ\n\n");
	}

	void DisplayMessage(const string &msg)
	{
		cout << "\n" << msg << "\n" << "Press any key to continue...";
		cin.get();
		cin.clear();

		if (cin.rdbuf()->in_avail() != 0)
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}

	void PauseConsole()
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