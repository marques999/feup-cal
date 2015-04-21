#include <string>

#include "Room.h"

using namespace std;

class Pipe
{
public:

	Pipe(pair<Room*, Room*> &r) : connection(r), caudal(70)
	{
	}

	Room& getSource() 
	{
		return *(connection.first); 
	}

	Room& getDestination() 
	{
		return *(connection.second); 
	}

	unsigned getCaudal() 
	{
		return caudal; 
	}

private:

	pair<Room*, Room*> connection;
	unsigned caudal;
};