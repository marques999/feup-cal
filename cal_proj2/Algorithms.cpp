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

static vector<int> KMPTable(const string &pattern)
{
	vector<int> lps(pattern.size());

	unsigned len = 0;
	unsigned i = 1;

	lps[0] = 0;

	while (i < pattern.size())
	{
		if (pattern[i] == pattern[len])
		{
			len++;
			lps[i] = len;
			i++;
		}
		else if (len != 0)
		{
			len = lps[len - 1];
		}
		else
		{
			lps[i] = 0;
			i++;
		}
	}

	return lps;
}

unsigned KMPSearch(const string &text, const string &pattern)
{
	unsigned M = pattern.size();
	unsigned N = text.size();
	unsigned i = 0;
	unsigned j = 0;

	vector<int> lps = KMPTable(pattern);

	while (i < N)
	{
		if (pattern[j] == text[i])
		{
			j++;
			i++;
		}

		if (j == M)
		{
			return (i - j);
		}

		if (pattern[j] != text[i])
		{
			if (j != 0)
			{
				j = lps[j - 1];
			}
			else
			{
				i++;
			}
		}
	}

	return N;
}

#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

bool compareChars(char a, char b)
{
	if (a >= 32 && a <= 127)
	{
		return toupper(a) == toupper(b);
	}

	switch (a)
	{
	case '\xc6': case '\xc7': case '\xb5': case '\xa0':
		return 'A' == toupper(b);
	case '\x82': case '\x88': case '\x90': case '\xd2':
		return 'E' == toupper(b);
	case '\xa2': case '\xe0': case '\xe4': case '\xe5':
		return 'O' == toupper(b);
	case '\x80': case '\x87':
		return 'C' == toupper(b);
	}

	return false;
}

unsigned LevenshteinDistance(const string& s1, const string& s2)
{
	const unsigned len1 = s1.size();
	const unsigned len2 = s2.size();
	const unsigned mlen = min(len1, len2 + 1);

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
			col[j + 1] = MIN3(prevCol[1 + j] + 1, col[j] + 1, prevCol[j] + (compareChars(s1[i], s2[i]) ? 0 : 1));
		}

		col.swap(prevCol);
	}

	return prevCol[len2];
}

int LongestCommonSubstring(const string& s1, const string& s2)
{
	if (s1.empty() || s2.empty())
	{
		return 0;
	}

	vector<unsigned> curr(s2.size());
	vector<unsigned> prev(s1.size());
	vector<unsigned> swap;

	unsigned maxSubstr = 0;

	for (unsigned i = 0; i < s1.size(); ++i)
	{
		for (unsigned j = 0; j < s2.size(); ++j)
		{
			if (s1[i] != s2[j])
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

				maxSubstr = std::max(maxSubstr, curr[j]);
			}
		}

		swap = curr;
		curr = prev;
		prev = swap;
	}

	return maxSubstr;
}