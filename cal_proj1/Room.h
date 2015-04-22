/*!
 * \file Room.h
 *
 * FEUP_CAL1415_2MIEIC02_D
 * \author Diogo Marques
 * \author Jose Taveira
 * \author Vitor Esteves
 *
 * \date Abril 2015
 *
 */

#ifndef __ROOM_H_
#define __ROOM_H_

#include "Common.h"

using namespace std;

class Room
{
public:

	Room() : name(""), temperature(20.0), enabled(true)
	{
	}

	Room(ifstream &fin)
	{
		read(fin);
	}

	Room(const string &s) : name(s), temperature(20.0), enabled(true)
	{
	}

	Room(const string &s, const double t) : name(s), temperature(t), enabled(true)
	{

	}

	string getName() const
	{
		return name;
	}

	int getX() const
	{
		return positionX;
	}

	int getY() const
	{
		return positionY;
	}

	void setPosition(int x, int y)
	{
		positionX = x;
		positionY = y;
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

	void read(ifstream &fin)
	{
		getline(fin, name);
		fin.read((char*)&temperature, sizeof(double));
		fin.read((char*)&positionX, sizeof(int));
		fin.read((char*)&positionY, sizeof(int));
		fin.read((char*)&enabled, sizeof(unsigned char));
	}

	void write(ofstream &fout) const
	{
		fout << name << endl;
		fout.write((char*)&temperature, sizeof(double));
		fout.write((char*)&positionX, sizeof(int));
		fout.write((char*)&positionY, sizeof(int));
		fout.write((char*)&enabled, sizeof(unsigned char));
	}

private:

	string name;
	bool enabled;
	int positionX;
	int positionY;
	double temperature;
};

#endif /* __ROOM_H_ */