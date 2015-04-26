/*!
 * \file Edge.h
 *
 * FEUP_CAL1415_2MIEIC02_D
 * \author Diogo Marques
 * \author Jose Taveira
 * \author Vitor Esteves
 *
 * \date Abril 2015
 *
 */

#ifndef __EDGE_H_
#define __EDGE_H_

template<class T>
struct Vertex;

template <class T>
struct Edge
{
	Edge() : dest(nullptr), id(0)
	{
	}

	Edge(Vertex<T> *dw) : dest(d), id(0)
	{
	}

	Edge(unsigned n, Vertex<T> *d) : dest(d), id(n)
	{
	}

	Vertex<T> *dest;

	unsigned id;
};

#endif /* __EDGE_H_ */