#include <string>

using namespace std;

class Room
{
public:

	Room() : name(""), temperature(20.0)
	{
	}

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

	bool isEnabled() const
	{
		return enabled;
	}

	void toggleStatus()
	{
		enabled = !enabled;
	}

	double getTemperature() const
	{
		return temperature;
	}

	void setTemperature(double newTemperature)
	{
		temperature = newTemperature;
	}

	bool operator==(const Room &rhs) const
	{
		return name == rhs.name;
	}

private:

	string name;
	bool enabled;
	double temperature;
};