#include <string>

using namespace std;

class Room
{
public:

	Room(const string &s) : name(s), temperature(20.0)
	{
	}

	Room(const string &s, const double t) : name(s), temperature(t)
	{
	}

	string getName()
	{
		return name;
	}

	double getTemperature()
	{
		return temperature;
	}
	
	bool operator==(const Room &r)
	{
		return name == r.name;
	}

private:

	string name;
	double temperature;
};