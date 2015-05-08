/*!
 * \file Algorithms.h
 *
 * FEUP_CAL1415_2MIEIC01_D
 * \author Diogo Marques
 * \author Jose Taveira
 * \author Vitor Esteves
 *
 * \date Maio 2015
 *
 */

#ifndef __ALGORITHMS_H_
#define __ALGORITHMS_H_

#include "Common.h"

size_t KMPSearch(const string &text, const string &pattern);
int LongestCommonSubstring(const string& str1, const string& str2);
unsigned LevenshteinDistance(const string& s1, const string& s2);

#endif /* __ALGORITHMS_H_ */