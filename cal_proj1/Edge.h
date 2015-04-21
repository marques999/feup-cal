#ifndef EDGE_H_
#define EDGE_H_

template<class T>
struct Vertex;

template <class T>
struct Edge
{
	Edge(Vertex<T> *d, double w) : dest(d), weight(w)
	{
	}

	Vertex<T> *dest;
	double weight;
};

#endif