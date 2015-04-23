/*!
* \file Graph.h
*
* FEUP_CAL1415_2MIEIC02_D
* \author Diogo Marques
* \author Jose Taveira
* \author Vitor Esteves
*
* \date Abril 2015
*
*/

#ifndef GRAPH_H_
#define GRAPH_H_

#include <vector>
#include <queue>
#include <list>
#include <limits>

#include "Vertex.h"

using namespace std;

const int NOT_VISITED = 0;
const int BEING_VISITED = 1;
const int DONE_VISITED = 2;
const int INT_INFINITY = INT_MAX;

template <class T>
class Graph
{
	vector<Vertex<T> *> vertices;
	void dfs(Vertex<T> *v, vector<T> &res) const;

	int numCycles;
	void dfsVisit(Vertex<T> *v);
	void dfsVisit();
	void getPathTo(Vertex<T> *origin, list<T> &res);

protected:

	unsigned nextID = 0;

public:

	int addVertex(const T &in);
	bool addVertex(unsigned vertexId, const T &in);
	bool addEdge(const T &sourc, const T &dest, double w);
	int removeVertex(const T &in);
	bool removeEdge(const T &sourc, const T &dest);

	vector<T> dfs() const;
	vector<T> bfs(Vertex<T> *v) const;

	int maxNewChildren(Vertex<T> *v, T &inf) const;
	vector<Vertex<T> * > getVertices() const;
	int getNumVertex() const;

	Vertex<T>* getVertex(const T &v) const;
	void resetIndegrees();
	void resetWeights(double weight);
	vector<Vertex<T>*> getSources() const;
	int getNumCycles();
	bool isDAG();
	vector<T> topologicalOrder();
	vector<T> getPath(const T &origin, const T &dest);

	void unweightedShortestPath(const T &v);
};

template <class T>
int Graph<T>::getNumVertex() const
{
	return vertices.size();
}

template <class T>
vector<Vertex<T>* > Graph<T>::getVertices() const
{
	return vertices;
}

template <class T>
void Graph<T>::resetWeights(double weight)
{
	for (Vertex<T>* v : vertices)
	{
		for (Edge<T> &e : v->adj)
		{
			e.weight = weight;
		}
	}
}

template <class T>
int Graph<T>::addVertex(const T &in)
{
	for (Vertex<T>* &v : vertices)
	{
		if (v->info == in)
		{
			return -1;
		}
	}

	Vertex<T>* newVertex = new Vertex<T>(in);

	newVertex->id = nextID++;
	vertices.push_back(newVertex);

	return newVertex->id;
}

template <class T>
bool Graph<T>::addVertex(unsigned vertexId, const T &in)
{
	for (Vertex<T>* &v : vertices)
	{
		if (v->info == in)
		{
			return false;
		}
	}

	Vertex<T>* newVertex = new Vertex<T>(vertexId, in);

	nextID = vertexId + 1;
	vertices.push_back(newVertex);

	return true;
}

template <class T>
int Graph<T>::removeVertex(const T &in)
{
	typename vector<Vertex<T>*>::iterator it = vertices.begin();
	typename vector<Vertex<T>*>::iterator ite = vertices.end();

	for (; it != ite; it++)
	{
		if ((*it)->info == in)
		{
			Vertex<T> *v = *it;

			vertices.erase(it);

			typename vector<Vertex<T>*>::iterator it1 = vertices.begin();
			typename vector<Vertex<T>*>::iterator it1e = vertices.end();

			for (; it1 != it1e; it1++)
			{
				(*it1)->removeEdgeTo(v);
			}

			typename vector<Edge<T> >::iterator it2 = v->adj.begin();
			typename vector<Edge<T> >::iterator it2e = v->adj.end();

			for (; it2 != it2e; it2++)
			{
				it2->dest->indegree--;
			}

			unsigned id = v->id;

			delete v;

			return id;
		}
	}

	return -1;
}

template <class T>
bool Graph<T>::addEdge(const T &src, const T &dst, double w)
{
	typename vector<Vertex<T>*>::iterator it = vertices.begin();
	typename vector<Vertex<T>*>::iterator ite = vertices.end();

	Vertex<T> *vS;
	Vertex<T> *vD;

	int found = 0;

	while (found != 2 && it != ite)
	{
		if ((*it)->info == src)
		{
			vS = *it;
			found++;
		}

		if ((*it)->info == dst)
		{
			vD = *it;
			found++;
		}

		it++;
	}

	if (found != 2)
	{
		return false;
	}

	vD->indegree++;
	vS->addEdge(vD, w);

	return true;
}

template <class T>
bool Graph<T>::removeEdge(const T &src, const T &dst)
{
	typename vector<Vertex<T>*>::iterator it = vertices.begin();
	typename vector<Vertex<T>*>::iterator ite = vertices.end();

	Vertex<T> *vS;
	Vertex<T> *vD;

	int found = 0;

	while (found != 2 && it != ite)
	{
		if ((*it)->info == src)
		{
			vS = *it;
			found++;
		}

		if ((*it)->info == dst)
		{
			vD = *it;
			found++;
		}

		it++;
	}

	if (found != 2)
	{
		return false;
	}

	return vS->removeEdgeTo(vD);
}

template <class T>
vector<T> Graph<T>::dfs() const
{
	for (Vertex<T>* v : vertices)
	{
		v->visited = false;
	}

	vector<T> res;

	for (Vertex<T>* v : vertices)
	{
		if (v->visited == false)
		{
			dfs(v, res);
		}
	}

	return res;
}

template <class T>
void Graph<T>::dfs(Vertex<T> *v, vector<T> &res) const
{
	v->visited = true;
	res.push_back(v->info);

	for (Edge<T> &e : v->adj)
	{
		if (e.dest->visited == false)
		{
			dfs(e.dest, res);
		}
	}
}

template <class T>
vector<T> Graph<T>::bfs(Vertex<T> *v) const
{
	vector<T> res;
	queue<Vertex<T> *> q;

	q.push(v);
	v->visited = true;

	while (!q.empty())
	{
		Vertex<T> *v1 = q.front();

		q.pop();
		res.push_back(v1->info);

		for (const Edge<T> &e : v1->adj)
		{
			if (e.dest->visited == false)
			{
				e.dest->visited = true;
				q.push(e.dest);
			}
		}
	}

	return res;
}

template <class T>
Vertex<T>* Graph<T>::getVertex(const T &src) const
{
	for (Vertex<T>* v : vertices)
	{
		if (v->info == src)
		{
			return v;
		}
	}

	return nullptr;
}

template<class T>
void Graph<T>::resetIndegrees()
{
	for (Vertex<T>* v : vertices)
	{
		v->indegree = 0;
	}

	for (Vertex<T>* v : vertices)
	{
		for (const Edge<T> &e : v->adj)
		{
			e.dest->indegree++;
		}
	}
}

template<class T>
vector<Vertex<T>*> Graph<T>::getSources() const
{
	vector< Vertex<T>* > buffer;

	for (Vertex<T>* vertex : vertices)
	{
		if (vertex->indegree == 0)
		{
			buffer.push_back(vertex);
		}
	}

	return buffer;
}

template <class T>
int Graph<T>::getNumCycles()
{
	numCycles = 0;
	dfsVisit();

	return numCycles;
}

template <class T>
bool Graph<T>::isDAG()
{
	return !getNumCycles();
}

template <class T>
void Graph<T>::dfsVisit()
{
	for (Vertex<T>* &v : vertices)
	{
		v->visited = false;
	}

	for (Vertex<T>* &v : vertices)
	{
		if (v->visited == false)
		{
			dfsVisit(v);
		}
	}
}

template <class T>
void Graph<T>::dfsVisit(Vertex<T> *v)
{
	v->processing = true;
	v->visited = true;

	for (const Edge<T> &e : v->adj)
	{
		if (e.dest->processing)
		{
			numCycles++;
		}

		if (!e.dest->visited)
		{
			dfsVisit(e.dest);
		}
	}

	v->processing = false;
}

template<class T>
vector<T> Graph<T>::topologicalOrder()
{
	vector<T> res;

	if (!isDAG())
	{
		return res;
	}

	resetIndegrees();

	queue<Vertex<T>*> q;
	vector<Vertex<T>*> sources = getSources();

	while (!sources.empty())
	{
		q.push(sources.back());
		sources.pop_back();
	}

	while (!q.empty())
	{
		Vertex<T>* v = q.front();

		q.pop();
		res.push_back(v->info);

		for (const Edge<T> &e : v->adj)
		{
			e.dest->indegree--;

			if (e.dest->indegree == 0)
			{
				q.push(e.dest);
			}
		}
	}

	if (res.size() != vertices.size())
	{
		while (!res.empty())
		{
			res.pop_back();
		}
	}

	resetIndegrees();

	return res;
}

template<class T>
void Graph<T>::unweightedShortestPath(const T &src)
{
	for (Vertex<T>* v : vertices)
	{
		v->path = nullptr;
		v->dist = INT_MAX;
	}

	Vertex<T>* v = getVertex(src);

	v->dist = 0;

	queue<Vertex<T>* > q;

	q.push(v);

	while (!q.empty())
	{
		v = q.front();
		q.pop();

		for (const Edge<T> &e : v->adj)
		{
			if (e.dest->dist == INT_MAX)
			{
				e.dest->dist = v->dist + 1;
				e.dest->path = v;
				q.push(e.dest);
			}
		}
	}
}

template<class T>
void Graph<T>::getPathTo(Vertex<T> *dst, list<T> &res)
{
	res.push_back(dst->info);

	if (dst->path != nullptr)
	{
		getPathTo(dst->path, res);
	}
}

template<class T>
vector<T> Graph<T>::getPath(const T &src, const T &dst)
{
	unweightedShortestPath(src);

	list<T> buffer;
	Vertex<T>* v = getVertex(dst);

	buffer.push_front(v->info);

	while (v->path->info != origin)
	{
		v = v->path;
		buffer.push_front(v->info);
	}

	buffer.push_front(v->path->info);

	vector<T> res;

	while (!buffer.empty())
	{
		res.push_back(buffer.front());
		buffer.pop_front();
	}

	return res;
}

#endif /* __GRAPH_H_ */