/*!
 * \file Vertex.h
 *
 * FEUP_CAL1415_2MIEIC02_D
 * \author Diogo Marques
 * \author Jose Taveira
 * \author Vitor Esteves
 *
 * \date Abril 2015
 *
 */

#ifndef __VERTEX_H_
#define __VERTEX_H_

#include "Common.h"
#include "Edge.h"

template <class T>
struct Vertex
{
	Vertex(const T &in) : info(in), visited(false), processing(false), indegree(0), dist(0), id(0)
	{
		path = 0;
	}

	Vertex(unsigned id, const T &in) : info(in), visited(false), processing(false), indegree(0), dist(0), id(id)
	{
		path = 0;
	}

	T info;
	Vertex<T>* path;
	Vertex<T>* clone() const;
	vector<Edge<T> > adj;

	bool visited;
	bool processing;
	unsigned id;
	int indegree;
	double dist;

	void addEdge(unsigned edgeId, Vertex<T> *dst, double w);
	bool removeEdgeTo(Vertex<T> *dst);
};

template <class T>
Vertex<T>* Vertex<T>::clone() const
{
	Vertex<T>* v = new Vertex<T>(this->id, this->info);

	v->adj = this->adj;
	v->visited = this->visited;
	v->indegree = this->indegree;
	v->dist = this->dist;
	v->processing = this->processing;
	v->path = 0;

	return v;
}

template <class T>
bool Vertex<T>::removeEdgeTo(Vertex<T> *dst)
{
	dst->indegree--;

	vector<Edge<T> >::iterator it = adj.begin();
	vector<Edge<T> >::iterator ite = adj.end();

	while (it != ite)
	{
		if (it->dest->info == dst->info)
		{
			adj.erase(it);

			return true;
		}

		it++;
	}

	return false;
}

template <class T>
void Vertex<T>::addEdge(unsigned edgeId, Vertex<T> *dst, double w)
{
	adj.push_back(Edge<T>(edgeId, dst, w));
}

#endif /* __VERTEX_H_ */