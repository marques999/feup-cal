#ifndef VERTEX_H_
#define VERTEX_H_

#include <vector>
#include "Edge.h"

template <class T>
struct Vertex
{
	Vertex(const T &in) : info(in), visited(false), processing(false), indegree(0), dist(0)
	{
		path = 0;
	}

	Vertex(int id, const T &in) : info(in), visited(false), processing(false), indegree(0), dist(0), id(0)
	{
		path = 0;
	}

	T info;
	Vertex* path;
	vector<Edge<T> > adj;

	bool visited;
	bool processing;
	int id;
	int indegree;
	int dist;

	void addEdge(Vertex<T> *dst, double w);
	bool removeEdgeTo(Vertex<T> *dst);
};

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
void Vertex<T>::addEdge(Vertex<T> *dst, double w)
{
	adj.push_back(Edge<T>(dst, w));
}

#endif