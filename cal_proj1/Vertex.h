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

	typename vector<Edge<T> >::iterator it = find_if(adj.begin(), adj.end(), [](const Vertex* &v) { v->info == dst->info });

	if (it == adj.end())
	{
		return false;
	}

	adj.erase(it);

	return true;
}

template <class T>
void Vertex<T>::addEdge(Vertex<T> *dst, double w)
{
	adj.push_back(Edge<T>(dst, w));
}

#endif