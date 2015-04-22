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

class RoomExists
{
public:

	RoomExists()
	{
	}

	friend ostream& operator<<(ostream &os, const RoomExists &e)
	{
		cout << "\nERRROR: a room with the same name already exists\n";
		return os;
	}
};

class PermissoesInsuficientes {
	string param;

public:

	PermissoesInsuficientes(const string &n) {
		param = n;
	}

	friend ostream& operator<<(ostream &os, const PermissoesInsuficientes &e) {
		os << "ERROR: permission denied - can't access " << e.param << " from other developers.\n";
		return os;
	}
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
		return ("ERROR: invalid parameter " + param + ".");
	}
};

class FileIOException
{
	string fileName;

public:

	FileIOException(const string &fn)
	{
		fileName = fn;
	}

	friend ostream& operator<<(ostream &os, const FileIOException &e)
	{
		os << "\nERROR: file I/O exception occured while trying to read/write " << e.fileName << "\n";
		return os;
	}
};

string to_upper(const string &s);

#endif /* __COMMON_H_ */
