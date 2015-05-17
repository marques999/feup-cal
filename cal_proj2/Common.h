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

class CustomException
{
public:

	CustomException(const string& s)
	{
		this->args = s;
	}

	virtual const char* str() const = 0;

protected:

	string args;
};

class MessageException
{
public:

	MessageException()
	{
	}

	virtual const char* str() const = 0;
};

class InvalidParameter : public MessageException
{
public:

	InvalidParameter() : MessageException()
	{
	}

	const char* str() const
	{
		return "ERROR: you have entered an invalid value.";
	}
};

class FileIOException : public CustomException
{
public:

	FileIOException(const char* s) : CustomException(s)
	{
	}

	const char* str() const
	{
		string s = "ERROR: a file I/O occured when trying to access " + args + ".";

		return s.c_str();
	}
};

#endif /* __COMMON_H_ */