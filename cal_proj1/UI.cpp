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

		cout << "\n\t\t-------------------------------------------------\n";
		cout << "\t\t| ";

		for (int i = 0; i < begin; i++)
		{
			cout << " ";
		}

		cout << s;

		for (int i = end; i < 45; i++)
		{
			cout << " ";
		}

		cout << "|\n";
		cout << "\t\t-------------------------------------------------\n\n";
	}

	void DisplayFrameNoCls(const string &s)
	{
		int begin = (s.size() % 2 == 0) ? (23 - (s.size() / 2)) : (22 - (s.size() / 2));
		int end = 22 + (s.size() / 2);

		cout << "\n\t\t-------------------------------------------------\n";
		cout << "\t\t| ";

		for (int i = 0; i < begin; i++)
		{
			cout << " ";
		}

		cout << s;

		for (int i = end; i < 45; i++)
		{
			cout << " ";
		}

		cout << "|\n";
		cout << "\t\t-------------------------------------------------\n\n";
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
		stringstream frame_begin;
		stringstream frame_end;
		stringstream ss;
		frame_begin << "-";
		frame_end << "-";
		ss << "|";
		for (int i = 0; i < c; i++)
		{
			for (int j = 0; j < length[i]; j++)
			{
				frame_begin << "-";
				frame_end << "-";
			}
			ss << left << setw(length[i]) << labels[i];
			ss << right << "|";
			frame_begin << "-";
			frame_end << "-";
		}
		UI::Display(frame_begin.str());
		UI::Display(ss.str());
		UI::Display(frame_end.str());
	}

	void DisplayTableRow(int c, const vector<string> &labels, const int length[])
	{
		stringstream ss;

		ss << "  ";

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
		os << setw(w) << left << fixed << setprecision(0) << n;
		return os.str();
	}
}