#pragma once

#include "Room.h"
#include "Graph.h"
#include "graphviewer.h"
#include <iostream>
#include <map>

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
	void displayInfo();
	void enableRoom();
	void addConnection();
	void addConnectionGraphViewer(pair<unsigned, unsigned> edge);
	void addRoom();
	void addRoomGraphViewer(unsigned vertexId, const Room& room);
	void removeRoom();
	void removeRoomGraphViewer(Vertex<Room>* &room);
	void changeTemperature();
	void changeTemperatureGraphViewer(Vertex<Room>* &room);
	void removeConnection();
	void removeConnectionGraphViewer(pair<unsigned, unsigned> edge);
	void resetFlow();
	void addBoiler();

private:
	
	map<unsigned, Room> rooms;
	map<unsigned, pair<unsigned, unsigned> > connections;

	const double defaultWeight = 70.0;

	char* formatRoom(const Room &r) const;
	unsigned getPositionX(int i);
	unsigned getPositionY(int j);
	unsigned matrixWidth;
	unsigned matrixHeight;
	
	unsigned matrixID;
	vector<vector<bool> > matrix;
	void setVertexColor(unsigned vertexId, double roomTemperature) const;
	bool roomExists(const string &s) const;
	unsigned nextID;
	Vertex<Room>* getRoom(const string &s) const;
	GraphViewer *gv;
	Graph<Room> g;
};
