/*!
 * \file Algorithms.cpp
 *
 * FEUP_CAL1415_2MIEIC01_D
 * \author Diogo Marques
 * \author Jose Taveira
 * \author Vitor Esteves
 *
 * \date Maio 2015
 *
 */

#include "Algorithms.h"

static bool compare(char a, char b)
{
	if (a >= 32 && a <= 127)
	{
		return toupper(a) == toupper(b);
	}

	switch (a)
	{
	case '\xc6': case '\xc7': // ãÃ
	case '\xa0': case '\xb5': // áÁ
		return 'A' == toupper(b);
	case '\x82': case '\x88':
	case '\x90': case '\xd2':
		return 'E' == toupper(b);
	case '\xa1': case '\xd6': // íÍ
		return 'I' == toupper(b);
	case '\xa2': case '\xe0': // óÓ
	case '\xe4': case '\xe5': // õÕ
	case '\x93': case '\xe2': // ôÔ
		return 'O' == toupper(b);
	case '\xa3': case '\xe9': // úÚ
		return 'U' == toupper(b);
	case '\x80': case '\x87': // çÇ
		return 'C' == toupper(b);
	}

	return false;
}

unsigned LevenshteinDistance(const string& s1, const string& s2)
{
	const unsigned len1 = s1.size();
	const unsigned len2 = s2.size();
	const unsigned mlen = min(len1, len2);

	vector<unsigned> col(len2 + 1);
	vector<unsigned> prevCol(len2 + 1);

	for (unsigned i = 0; i < prevCol.size(); i++)
	{
		prevCol[i] = i;
	}

	for (unsigned i = 0; i < mlen; i++)
	{
		col[0] = i + 1;

		for (unsigned j = 0; j < len2; j++)
		{
			col[j + 1] = MIN3(prevCol[1 + j] + 1, col[j] + 1, prevCol[j] + (compare(s1[i], s2[i]) ? 0 : 1));
		}

		col.swap(prevCol);
	}

	return prevCol[len2];
}

unsigned LongestCommonSubstring(const string& s1, const string& s2)
{
	if (s1.empty() || s2.empty())
	{
		return 0;
	}

	vector<unsigned> curr(s2.size());
	vector<unsigned> prev(s1.size());
	vector<unsigned> swap;

	unsigned index = 0;

	for (unsigned i = 0; i < s1.size(); ++i)
	{
		for (unsigned j = 0; j < s2.size(); ++j)
		{
			if (!compare(s1[i], s2[j]))
			{
				curr[j] = 0;
			}
			else
			{
				if (i == 0 || j == 0)
				{
					curr[j] = 1;
				}
				else
				{
					curr[j] = 1 + prev[j - 1];
				}

				index = max(index, curr[j]);
			}
		}

		swap = curr;
		curr = prev;
		prev = swap;
	}

	return index;
}