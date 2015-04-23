/*!
 * \file Pipe.h
 *
 * FEUP_CAL1415_2MIEIC02_D
 * \author Diogo Marques
 * \author Jose Taveira
 * \author Vitor Esteves
 *
 * \date Abril 2015
 *
 */

#ifndef __PIPE_H_
#define __PIPE_H_

#include "Common.h"

class Pipe
{
public:

	Pipe() : _src(0), _dst(0), _weight(0.0)
	{
	}

	Pipe(ifstream &fin)
	{
		read(fin);
	}

	Pipe(unsigned src, unsigned dst) : _src(src), _dst(dst)
	{
	}

	Pipe(unsigned src, unsigned dst, double weight) : _src(src), _dst(dst), _weight(weight)
	{
	}

	~Pipe()
	{
	}

	unsigned source() const
	{
		return _src;
	}

	unsigned dest() const
	{
		return _dst;
	}

	double weight() const
	{
		return _weight;
	}

	void change(unsigned weight)
	{
		_weight = weight;
	}

	void read(ifstream &fin)
	{
		fin.read((char*)&_src, sizeof(unsigned));
		fin.read((char*)&_dst, sizeof(unsigned));
		fin.read((char*)&_weight, sizeof(double));
	}

	void write(ofstream &fout) const
	{
		fout.write((char*)&_src, sizeof(unsigned));
		fout.write((char*)&_dst, sizeof(unsigned));
		fout.write((char*)&_weight, sizeof(double));
	}

private:

	unsigned _src;
	unsigned _dst;
	double _weight;
};

#endif /* __PIPE_H_ */