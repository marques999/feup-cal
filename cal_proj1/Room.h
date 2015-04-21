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

	string getName() const
	{
		return name;
	}

	double getTemperature() const
	{
		return temperature;
	}

	bool operator==(const Room &rhs) const
	{
		return name == rhs.name;
	}

private:

	string name;
	double temperature;
};