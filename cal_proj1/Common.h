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

class CustomException
{
public:

	CustomException(const string &s)
	{
		this->args = s;
	}

	virtual string str() const = 0;

protected:

	string args;
};

class MessageException
{
public:

	MessageException()
	{
	}

	virtual string str() const = 0;
};

class SourceRoomNotFound : public CustomException
{
public:

	SourceRoomNotFound(const string &s) : CustomException(s)
	{
	}

	string str() const
	{
		return "ERROR: source room " + args + " not found.";
	}
};

class DestinationRoomNotFound : public CustomException
{
public:

	DestinationRoomNotFound(const string &s) : CustomException(s)
	{
	}

	string str() const
	{
		return "ERROR: destination room " + args + " not found.";
	}
};

class TargetRoomNotFound : public CustomException
{
public:

	TargetRoomNotFound(const string &s) : CustomException(s)
	{
	}

	string str() const
	{
		return "ERROR: target room " + args + " not found.";
	}
};


class BoilerNoConnections : public MessageException
{
public:

	BoilerNoConnections() : MessageException()
	{
	}

	string str() const
	{
		return "ERROR: boiler must be connected to at least one room.";
	}
};

class RoomNotConnected : public CustomException
{
public:

	RoomNotConnected(const string &s) : CustomException(s)
	{
	}

	string str() const
	{
		return "ERROR: " + args + " is not connected to any source (boiler or room).";
	}
};

class InvalidParameter : public MessageException
{
public:

	InvalidParameter() : MessageException()
	{
	}

	string str() const
	{
		return "ERROR: you have entered an invalid value.";
	}
};

class FileIOException : public CustomException
{
public:

	FileIOException(const string &s) : CustomException(s)
	{
	}

	string str() const
	{
		return "ERROR: file I/O exception occured while trying to access " + args + ".";
	}
};

#endif /* __COMMON_H_ */