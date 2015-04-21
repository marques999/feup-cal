#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>

using namespace std;

class SourceRoomNotFound
{
public:

	SourceRoomNotFound(const string &s)
	{
		this->sourceRoom = s;
	}

	friend ostream& operator<<(ostream &os, const SourceRoomNotFound &e)
	{
		cout << "\nERRROR: source room " << e.sourceRoom << " does not exist.\n";
		return os;
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

	friend ostream& operator<<(ostream &os, const DestinationRoomNotFound &e)
	{
		cout << "\nERRROR: destination room " << e.destinationRoom << " does not exist.\n";
		return os;
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


#endif