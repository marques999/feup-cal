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

	size_t len = 0;
	size_t i = 1;

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

size_t KMPSearch(const string &text, const string &pattern)
{
	size_t M = pattern.size();
	size_t N = text.size();
	size_t i = 0; // index for text[]
	size_t j = 0; // index for pattern[]

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
	const size_t len1 = s1.size();
	const size_t len2 = s2.size();
	const size_t mlen = min(len1, len2 + 1);

	vector<size_t> col(len2 + 1);
	vector<size_t> prevCol(len2 + 1);

	for (size_t i = 0; i < prevCol.size(); i++)
	{
		prevCol[i] = i;
	}

	for (size_t i = 0; i < mlen; i++)
	{
		col[0] = i + 1;

		for (size_t j = 0; j < len2; j++)
		{
			col[j + 1] = MIN3(prevCol[1 + j] + 1, col[j] + 1, prevCol[j] + (compareChars(s1[i], s2[i]) ? 0 : 1));
		}

		col.swap(prevCol);
	}

	return prevCol[len2];
}

int LongestCommonSubstring(const string& str1, const string& str2)
{
	if (str1.empty() || str2.empty())
	{
		return 0;
	}

	vector<size_t> curr(str2.size());
	vector<size_t> prev(str1.size());
	vector<size_t> swap;

	size_t maxSubstr = 0;

	for (size_t i = 0; i < str1.size(); ++i)
	{
		for (size_t j = 0; j < str2.size(); ++j)
		{
			if (str1[i] != str2[j])
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