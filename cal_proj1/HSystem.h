#pragma once

#include "Room.h"
#include "Graph.h"
#include "graphviewer.h"
#include <iostream>

class HSystem
{
public:

	HSystem()
	{
		nextID = 0;
		initialize();
	}

	~HSystem()
	{
	}

	void initialize();
	void disableRoom();
	void enableRoom();
	void addConnection();
	void addRoom();
	void addRoomGraphViewer(unsigned vertexId, const Room& room);
	void removeRoom();
	void removeEdge();

private:

	void setVertexColor(unsigned vertexId, double roomTemperature) const;
	bool roomExists(const string &s) const;
	unsigned nextID;
	Vertex<Room>* getRoom(const string &s) const;
	GraphViewer *gv;
	Graph<Room> g;
};
