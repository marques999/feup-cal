/*!
 * \file UI.h
 *
 * FEUP_CAL1415_2MIEIC01_D
 * \author Diogo Marques
 * \author Jose Taveira
 * \author Vitor Esteves
 *
 * \date Maio 2015
 *
 */

#ifndef __UI_H_
#define __UI_H_

#include "Common.h"

namespace UI
{
	/**
	* @brief clears console sceren
	*/
	inline void ClearConsole()
	{
		system("cls");
	}

	/*
	* @brief prints "press any key" message and waits for user input
	*/
	inline void PauseConsole()
	{
		printf("Press any key to continue...");
		cin.get();
		cin.clear();

		if (cin.rdbuf()->in_avail() != 0)
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}

	/**
	* @brief formats a fixed digit number
	* @param n value
	* @param w number of digits (field width)
	* @return returns a string containing the formatted value
	*/
	inline string Format(unsigned n, unsigned w)
	{
		ostringstream os;
		os << setw(w) << right << fixed << n;
		return os.str();
	}

	/**
	* @brief displays text centered on screen
	* @param s string to be displayed
	*/
	inline void Display(const char* s)
	{
		printf("%*s\n", 40 + (strlen(s) / 2), s);
	}

	/**
	* @brief displays a brief error or informative message
	* @param s string to be displayed
	*/
	inline void DisplayMessage(const string &msg)
	{
		cout << "\n" << msg << "\n" << "Press any key to continue...";
		cin.get();
		cin.clear();

		if (cin.rdbuf()->in_avail() != 0)
		{
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}

	/**
	* @brief displays vector contents as a list inside a rectangular box
	* @param v a vector of strings containing the elements to be displayed
	*/
	void DisplayBox(const vector<string> &v, size_t begin);

	/**
	* @brief displays a frame (or header)
	* @param s string to be displayed
	*/
	void DisplayFrame(const char* s);

	/**
	* @brief displays table header
	* @param c number of columns
	* @param labels array containing the labels
	* @param length array containing the width of each column (should be the same size as c)
	*/
	void DisplayTable(int c, const vector<string> &labels, const int length[]);

	/**
	* @brief displays a table row
	* @param c number of columns (same as DisplayTable())
	* @param labels array containing the row values
	* @param length array containing the width of each cell (should be the same size as c)
	*/
	void DisplayTableRow(int c, const vector<string> &labels, const int length[]);
}

#endif /* __UI_H_ */