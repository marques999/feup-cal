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

class Room
{
public:

	Room() : _name(""), _temperature(20.0), _enabled(true), _x(0), _y(0)
	{
	}

	Room(const string &s) : _name(s), _temperature(20.0), _enabled(true), _x(0), _y(0)
	{
	}

	Room(const string &s, const double t) : _name(s), _temperature(t), _enabled(true), _x(0), _y(0)
	{
	}

	Room(ifstream &fin)
	{
		read(fin);
	}

	~Room()
	{
	}

	const string& getName() const
	{
		return _name;
	}

	int getX() const
	{
		return _x;
	}

	int getY() const
	{
		return _y;
	}

	void setPosition(int x, int y)
	{
		_x = x;
		_y = y;
	}

	bool isEnabled() const
	{
		return _enabled;
	}

	void enable()
	{
		_enabled = true;
	}

	void disable()
	{
		_enabled = false;
	}

	double getTemperature() const
	{
		return _temperature;
	}

	void setTemperature(double newTemperature)
	{
		_temperature = newTemperature;
	}

	bool operator==(const Room &rhs) const
	{
		return _name == rhs._name;
	}

	void read(ifstream &fin)
	{
		getline(fin, _name);
		fin.read((char*)&_temperature, sizeof(double));
		fin.read((char*)&_x, sizeof(int));
		fin.read((char*)&_y, sizeof(int));
		fin.read((char*)&_enabled, sizeof(unsigned char));
	}

	void write(ofstream &fout) const
	{
		fout << _name << endl;
		fout.write((char*)&_temperature, sizeof(double));
		fout.write((char*)&_x, sizeof(int));
		fout.write((char*)&_y, sizeof(int));
		fout.write((char*)&_enabled, sizeof(unsigned char));
	}

private:

	string _name;
	bool _enabled;
	int _x;
	int _y;
	double _temperature;
};

#endif /* __ROOM_H_ */