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

const char* boxTop = "浜様様様様様様様様様様様様様様様様様様様様�";
const char* boxBottom = "藩様様様様様様様様様様様様様様様様様様様様�";
const char* tableTop = "浜様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様融";
const char* tableBottom = "藩様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様様夕";
const char* paddingLeft = "                  � ";

namespace UI
{
	void DisplayBox(const vector<string> &v, unsigned index)
	{
		const unsigned numberEntries = v.size() - index;
		const unsigned maximumIndex = numberEntries < 10 ? index + numberEntries : index + 10;

		UI::Display(boxTop);

		for (unsigned i = index; i < maximumIndex; i++)
		{
			printf("%s%-40s�\n", paddingLeft, v[i].c_str());
		}

		UI::Display(boxBottom);
	}

	void DisplayTable(unsigned c, const vector<string> &labels, const unsigned length[])
	{
		string frameTop;
		string frameBottom;
		stringstream ss;

		frameTop.push_back('\xc9');
		frameBottom.push_back('\xc8');
		ss << '\xba';

		for (unsigned i = 0; i < c; i++)
		{
			for (unsigned j = 0; j < length[i]; j++)
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
		UI::Display(ss.str().c_str());
		UI::Display(tableBottom);
	}

	void DisplayTableRow(unsigned c, const vector<string> &labels, const unsigned length[])
	{
		stringstream ss;

		ss << " ";

		for (unsigned i = 0; i < c; i++)
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
		const unsigned stringLength = strlen(s);
		const unsigned stringBegin = (stringLength % 2 == 0) ? (23 - (stringLength / 2)) : (22 - (stringLength / 2));
		const unsigned stringEnd = 22 + (stringLength / 2);

		printf("\n\t\t浜様様様様様様様様様様様様様様様様様様様様様様様�\n\t\t� ");

		for (unsigned i = 0; i < stringBegin; i++)
		{
			putchar(' ');
		}

		printf(s);

		for (unsigned i = stringEnd; i < 45; i++)
		{
			putchar(' ');
		}

		printf("�\n\t\t藩様様様様様様様様様様様様様様様様様様様様様様様�\n\n");
	}
}