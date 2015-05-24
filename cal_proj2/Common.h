/*!
 * \file Common.h
 *
 * FEUP_CAL1415_2MIEIC01_D
 * \author Diogo Marques
 * \author Jose Taveira
 * \author Vitor Esteves
 *
 * \date Maio 2015
 *
 */

#ifndef __COMMON_H_
#define __COMMON_H_

#include <conio.h>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class FileIOException
{
public:

	FileIOException(const string& s)
	{
		this->args = s;
	}

	string str() const
	{
		return "ERROR: a file I/O occured when trying to access " + args + ".";
	}

protected:

	string args;
};

class InvalidParameter
{
public:

	InvalidParameter()
	{
	}

	string str() const
	{
		return "ERROR: you have entered an invalid value.";
	}
};

#endif /* __COMMON_H_ */