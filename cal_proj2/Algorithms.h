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

unsigned KMPSearch(const string &text, const string &pattern);
unsigned LevenshteinDistance(const string& s1, const string& s2);
int LongestCommonSubstring(const string& s1, const string& s2);

#endif /* __ALGORITHMS_H_ */