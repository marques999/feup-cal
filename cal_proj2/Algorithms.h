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

#define MIN3(a, b, c) ((a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c)))

unsigned LevenshteinDistance(const string& s1, const string& s2);
unsigned LongestCommonSubstring(const string& s1, const string& s2);

#endif /* __ALGORITHMS_H_ */