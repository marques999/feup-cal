/*!
* \file UI.h
*
* FEUP_AEDA1415_2MIEIC03_D
* \author Carlos Soares
* \author Diogo Marques
* \author Fabio Carneiro
*
* \date Dezembro 2014
*
*/

#ifndef __MENU_H_
#define __MENU_H_

#include <string>
#include <iomanip>
#include <conio.h>
#include <map>
#include <sstream>
#include <vector>

using namespace std;

namespace UI
{
	/**
	* @brief displays a frame (or header)
	* @param s string to be displayed
	* @return void
	*/
	void DisplayFrame(string s);

	/**
	* @brief displays text centered on screen
	* @param s string to be displayed
	* @return void
	*/
	void Display(string s);

	/**
	* @brief displays a brief error or informative message
	* @param s string to be displayed
	* @return void
	*/
	void DisplayMessage(string s);

	/**
	* @brief displays the table header
	* @param c the number of columns
	* @param labels an array containing the labels
	* @param length an array containing the width of each column (should be the same size as c)
	* @return void
	*/
	void DisplayTable(int c, vector<string> labels, int length[]);

	/**
	* @brief displays a table row
	* @param c the number of columns (same as DisplayTable())
	* @param labels an array containing the row values
	* @param length an array containing the width of each cell (should be the same size as c)
	* @return void
	*/
	void DisplayTableRow(int c, vector<string> labels, int length[]);

	/**
	* @brief formats the price in a user-friendly way (fixed point)
	* @param p number to be formatted
	* @return a string containing the formatted value
	*/
	string FormatPrice(double p);

	/**
	* @brief formats ratings in a user-friendly way (fixed point)
	* @param p number to be formatted
	* @return a string containing the formatted value
	*/
	string FormatRating(double r);

	/**
	* @brief formats a fixed digit number (eg. the tax number)
	* @param p number to be formatted
	* @param r number of digits
	* @return a string containing the formatted value
	*/
	string Format(unsigned n, unsigned w);
}

class HSystem;

typedef void(*MenuFunction)();

class Menu
{
private:

	void Display(const string &s)
	{
		cout << right << setw(40 + (s.size() / 2)) << setfill(' ') << s << endl;
	}

public:

	Menu(const string &title) : _width(80), _title(title)
	{
	}

	void setWidth(unsigned w)
	{
		_width = w;

		menuTop.push_back('\xc9');
		menuBottom.push_back('\xc8');
		menuSeparator.push_back('\xcc');

		for (unsigned i = 1; i < _width - 1; i++)
		{
			menuTop.push_back('\xcd');
			menuBottom.push_back('\xcd');
			menuSeparator.push_back('\xcd');
		}

		menuTop.push_back('\xbb');
		menuBottom.push_back('\xbc');
		menuSeparator.push_back('\xb9');
	}

	~Menu()
	{
	}

	void DisplayMenuItems()
	{
		vector<pair<char, string> >::const_iterator it;
		stringstream ss;

		for (it = _menu.begin(); it != _menu.end(); it++)
		{
			ss.clear();
			ss.str(string());

			if (it->second == string())
			{
				Display(menuSeparator);
			}
			else
			{
				ss << left << "\xba " << it->first << ". " << setw(_width - 7) << it->second << setw(2) << " \xba";
				Display(ss.str());
			}
		}
	}

	__forceinline void addSeparator()
	{
		_menu.push_back(make_pair(' ', string()));
	}

	__forceinline void displayMenu()
	{
		Display(menuTop);
		DisplayMenuTitle();
		Display(menuSeparator);
		DisplayMenuItems();
		Display(menuBottom);
	}

	__forceinline void DisplayMenuTitle()
	{
		stringstream ss;

		ss << left << "\xba " << setw(_width - 4) << _title << setw(2) << " \xba";
		Display(ss.str());
	}

	__forceinline void addItem(char c, const string& desc)
	{
		_menu.push_back(make_pair(c, desc));
	}

	__forceinline void addHandler(char c, MenuFunction func)
	{
		_func.insert(make_pair(c, func));
	}

	void run()
	{
		while (true)
		{
			system("cls");
			displayMenu();

			char c = _getch();

			map<char, MenuFunction>::const_iterator it = _func.find(c);

			if (it != _func.end())
			{
				if (it->second != nullptr)
				{
					it->second();
				}
				else
				{
					break;
				}
			}
		}
	}

private:

	unsigned _width;

	string _title;
	string menuTop;
	string menuBottom;
	string menuSeparator;
	
	vector<pair<char, string> >_menu;
	map<char, MenuFunction> _func;
};

#endif