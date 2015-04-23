/*!
 * \file UI.cpp
 *
 * FEUP_CAL1415_2MIEIC02_D
 * \author Diogo Marques
 * \author Jose Taveira
 * \author Vitor Esteves
 *
 * \date Abril 2015
 *
 */

#include "UI.h"

namespace UI
{
	void ClearConsole()
	{
		system("cls");
	}

	void DisplayFrame(const string &s)
	{
		ClearConsole();

		int begin = (s.size() % 2 == 0) ? (23 - (s.size() / 2)) : (22 - (s.size() / 2));
		int end = 22 + (s.size() / 2);

		cout << "\n\t\tษอออออออออออออออออออออออออออออออออออออออออออออออป\n\t\tบ ";

		for (int i = 0; i < begin; i++)
		{
			cout << " ";
		}

		cout << s;

		for (int i = end; i < 45; i++)
		{
			cout << " ";
		}

		cout << "บ\n\t\tศอออออออออออออออออออออออออออออออออออออออออออออออผ\n\n";
	}

	void DisplayFrameNoCls(const string &s)
	{
		int begin = (s.size() % 2 == 0) ? (23 - (s.size() / 2)) : (22 - (s.size() / 2));
		int end = 22 + (s.size() / 2);

		cout << "\n\t\tษอออออออออออออออออออออออออออออออออออออออออออออออป\n\t\tบ ";

		for (int i = 0; i < begin; i++)
		{
			cout << " ";
		}

		cout << s;

		for (int i = end; i < 45; i++)
		{
			cout << " ";
		}

		cout << "บ\n\t\tศอออออออออออออออออออออออออออออออออออออออออออออออผ\n\n";
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
		cout << "Press any key to continue...";
		cin.get();
		cin.clear();

		if (cin.rdbuf()->in_avail() != 0)
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}

	void Display(const string &s)
	{
		cout << right << setw(40 + (s.size() / 2)) << setfill(' ') << s << endl;
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

		UI::Display(frameTop);
		UI::Display(ss.str());
		UI::Display(frameBottom);
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

		UI::Display(ss.str());
	}

	string Format(unsigned n, unsigned w)
	{
		ostringstream os;
		os << setw(w) << left << fixed << n;
		return os.str();
	}

	string FormatWeight(double n, unsigned w)
	{
		ostringstream os;
		os << setw(w + 3) << left << fixed << setprecision(w) << n;
		return os.str();
	}

	string FormatTemperature(double n)
	{
		ostringstream os;
		os << setw(5) << fixed << setprecision(1) << n << "C";
		return os.str();
	}
}