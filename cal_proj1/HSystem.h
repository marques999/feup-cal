/*!
 * \file HSystem.h
 *
 * FEUP_CAL1415_2MIEIC02_D
 * \author Diogo Marques
 * \author Jose Taveira
 * \author Vitor Esteves
 *
 * \date Abril 2015
 *
 */

#ifndef __HSYSTEM_H_
#define __HSYSTEM_H_

#include "Common.h"
#include "edgetype.h"
#include "Graph.h"
#include "Pipe.h"
#include "graphviewer.h"
#include "Room.h"
#include "UI.h"

class HSystem
{
public:

	HSystem();
	~HSystem();

	void initialize();
	void reset();
	void loadGraph(const string &filename);
	void saveGraph(const string &filename) const;
	void readEdge(ifstream &fin);
	void readVertex(ifstream &fin);
	void writeEdge(ofstream &fout) const;
	void writeVertex(unsigned vertexId, const Room &room, ofstream &fout) const;

	void disableRoom();
	void displayRooms() const;
	void displayConnections() const;
	void enableRoom();
	void addConnection();
	void addConnectionGraphViewer(unsigned src, unsigned dst);
	void addRoom();
	void addRoomGraphViewer(unsigned vertexId, const Room& room);
	void removeRoom();
	void removeRoomGraphViewer(Vertex<Room>* &room);
	void changeTemperature();
	void changeTemperatureGraphViewer(Vertex<Room>* &room);
	void removeConnection();
	void removeConnectionGraphViewer(unsigned src, unsigned dst);
	void resetFlow();
	void addBoiler();

private:

	map<unsigned, Room> rooms;
	map<unsigned, Pipe> pipes;

	const double defaultWeight = 70.0;

	char* formatRoom(const Room &r) const;

	string roomName(unsigned id) const;

	int roomPositionX;
	int roomPositionY;
	unsigned convertPositionX(int x);
	unsigned convertPositionY(int y);
	
	void drawFloorplan(int x, int y);
	bool positionRoom();
	unsigned matrixWidth;
	unsigned matrixHeight;

	vector<vector<bool> > matrix;
	void setVertexColor(unsigned vertexId, double roomTemperature) const;
	void removePosition(int x, int y);
	void updateMatrix();
	unsigned nextID;
	Vertex<Room>* getRoom(const string &s) const;
	GraphViewer *gv;
	Graph<Room> g;
};

#endif /* __HSYSTEM_H_ */