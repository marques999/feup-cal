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
	Edge(Vertex<T> *d, double w) : dest(d), weight(w), id(0)
	{
	}

	Edge(unsigned n, Vertex<T> *d, double w) : dest(d), weight(w), id(n)
	{
	}

	Vertex<T> *dest;

	unsigned id;
	double temperature;
	double weight;
};

#endif /* __EDGE_H_ */