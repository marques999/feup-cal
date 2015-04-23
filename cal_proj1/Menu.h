/*!
 * \file Menu.h
 *
 * FEUP_CAL1415_2MIEIC02_D
 * \author Diogo Marques
 * \author Jose Taveira
 * \author Vitor Esteves
 *
 * \date Abril 2015
 *
 */

#ifndef __MENU_H_
#define __MENU_H_

#include "Common.h"

class HSystem;

typedef void(*MenuFunction)();

class Menu
{
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
				UI::Display(menuSeparator);
			}
			else
			{
				ss << left << "\xba " << it->first << ". " << setw(_width - 7) << it->second << setw(2) << " \xba";
				UI::Display(ss.str());
			}
		}
	}

	__forceinline void addSeparator()
	{
		_menu.push_back(make_pair(' ', string()));
	}

	__forceinline void displayMenu()
	{
		UI::Display(menuTop);
		DisplayMenuTitle();
		UI::Display(menuSeparator);
		DisplayMenuItems();
		UI::Display(menuBottom);
	}

	__forceinline void DisplayMenuTitle()
	{
		stringstream ss;

		ss << left << "\xba " << setw(_width - 4) << _title << setw(2) << " \xba";
		UI::Display(ss.str());
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
			UI::ClearConsole();
			cout << "\n\n\n\n";
			displayMenu();

			map<char, MenuFunction>::const_iterator it = _func.find(_getch());

			if (it != _func.end())
			{
				if (it->second != nullptr)
				{
					try 
					{
						it->second();
					}
					catch (SourceRoomNotFound &srnf)
					{
						UI::DisplayMessage(srnf.str());
					}
					catch (DestinationRoomNotFound &drnf)
					{
						UI::DisplayMessage(drnf.str());
					}
					catch (InvalidParameter &ip)
					{
						UI::DisplayMessage(ip.str());
					}
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

#endif /* __MENU_H_ */