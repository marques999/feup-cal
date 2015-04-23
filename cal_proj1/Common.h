/*!
 * \file Common.h
 *
 * FEUP_CAL1415_2MIEIC02_D
 * \author Diogo Marques
 * \author Jose Taveira
 * \author Vitor Esteves
 *
 * \date Abril 2015
 *
 */

#ifndef __COMMON_H_
#define __COMMON_H_

#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <functional>
#include <iostream>
#include <iomanip>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#ifdef _MSC_VER
#define _sprintf sprintf_s
#else
#define _sprintf sprintf
#endif

using namespace std;

class SourceRoomNotFound
{
public:

	SourceRoomNotFound(const string &s)
	{
		this->sourceRoom = s;
	}

	string str()
	{
		return "ERROR: source room " + sourceRoom + " not found.";
	}

private:

	string sourceRoom;
};

class DestinationRoomNotFound
{
public:

	DestinationRoomNotFound(const string &s)
	{
		this->destinationRoom = s;
	}

	string str()
	{
		return "ERROR: destination room " + destinationRoom + " not found.";
	}

private:

	string destinationRoom;
};

class InvalidParameter
{
	string param;

public:

	InvalidParameter(const string &p)
	{
		param = p;
	}

	string str()
	{
		return "ERROR: invalid parameter " + param + ".";
	}
};

class FileIOException
{
	string fileName;

public:

	FileIOException(const string &s)
	{
		fileName = s;
	}

	string str()
	{
		return "ERROR: file I/O exception occured while trying to access " + fileName + ".";
	}
};

#endif /* __COMMON_H_ */