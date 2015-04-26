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

	Pipe() : _src(0), _dst(0)
	{
	}

	Pipe(unsigned src, unsigned dst) : _src(src), _dst(dst)
	{
	}

	Pipe(ifstream &fin)
	{
		read(fin);
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

	void read(ifstream &fin)
	{
		fin.read((char*)&_src, sizeof(unsigned));
		fin.read((char*)&_dst, sizeof(unsigned));
	}

	void write(ofstream &fout) const
	{
		fout.write((char*)&_src, sizeof(unsigned));
		fout.write((char*)&_dst, sizeof(unsigned));
	}

private:

	unsigned _src;
	unsigned _dst;
};

#endif /* __PIPE_H_ */