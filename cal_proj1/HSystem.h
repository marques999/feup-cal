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
	void initializeWindow();
	void reset();
	void loadGraph();
	void saveGraph() const;
	bool loadGraph(const string &filename);
	void saveGraph(const string &filename) const;
	void readEdge(ifstream &fin);
	void readVertex(ifstream &fin);
	void writeEdge(unsigned edgeId, ofstream &fout) const;
	void writeVertex(unsigned vertexId, ofstream &fout) const;

	void displayRooms() const;
	void displayConnections() const;
	
	void disableRoom();
	void disableRoomGraphViewer(unsigned vertexId);
	void enableRoom();
	void enableRoomGraphViewer(unsigned vertexId, double temperature);
	void addConnection();
	void addConnectionGraphViewer(const Pipe& pipe);
	void addRoom();
	void addRoomGraphViewer(unsigned vertexId, const Room& room);
	void removeRoom();
	void removeRoomGraphViewer(unsigned vertexId);
	void changeTemperature();
	void changeTemperatureGraphViewer(Vertex<Room>* &room);
	void changeWeight(); // is it needed?
	void changeWeightGraphViewer(unsigned edgeId, unsigned weight);
	void removeConnection();
	void removeConnectionGraphViewer(unsigned src, unsigned dst);
	void resetFlow();
	void resetFlowGraphViewer();
	void addBoiler();
	bool validateGraph() const;
private:

	int matrixWidth;
	int matrixHeight;
	int roomPositionX;
	int roomPositionY;

	double calculateTemperature(const Room &room, const Edge<Room> &pipe, double t, double q)
	{
		return (room.getTemperature() * pipe.weight + t*q) / (pipe.weight + q);
	}

	GraphViewer *gv;
	Graph<Room> g;
	map<unsigned, Room> rooms;
	map<unsigned, Pipe> pipes;
	vector<vector<bool> > matrix;

	const double minimumTemperature = 0.0;
	const double maximumTemperature = 100.0;
	const double defaultTemperature = 20.0;
	const double defaultWeight = 70.0;

	const string promptRoomDisable = "Enter the room name to be disabled: ";
	const string promptRoomEnable = "Enter the room name to be enabled: ";
	const string radiatorIsBoiler = "ERROR: boiler status can't be changed by the user.";
	const string radiatorDisableFail = "ERROR: radiator already disabled, nothing to do...";
	const string radiatorDisableSuccess = "INFORMATION: room radiator disabled successfully!";
	const string radiatorEnableFail = "ERROR: radiator already enabled, nothing to do...";
	const string radiatorEnableSuccess = "INFORMATION: room radiator enabled successfully!";
	const string addRoomSuccess = "INFORMATION: room added successfully!";
	const string addRoomFail = "ERROR: house is currently full, please remove a room first.";
	const string removeRoomSuccess = "INFORMATION: room removed successfully!";
	const string roomExists = "ERROR: a room with the same name already exists.";
	const string promptRoomName = "Enter room name: ";
	const string promptDestinationName = "Enter destination room name: ";
	const string promptSourceName = "Enter source room name: ";
	const string promptTemperature = "Enter the room temperature: ";
	const string promptNewTemperature = "Enter the new room temperature: ";
	const string temperatureSuccess = "INFORMATION: room temperature successfully changed!";
	const string promptFilename = "Please enter a filename(extension will be added automatically): ";
	const string pressEnterLoad = "Press <enter> to load the selected file...";
	const string pressEnterPlace = "Press <enter> to place the room at the chosen location...";
	const string graphLoadSuccess = "INFORMATION: graph successfully loaded from file.";
	const string graphLoadFail = "ERROR: reached end of file prematurely, graph might not be displayed correctly.";
	const string graphSaveSuccess = "INFORMATION: graph successfully written to file.";
	const string graphSaveOverwrite = "QUESTION: A file with that name already exists.\nDo you want to overwrite it ? (y/N) : ";
	const string roomConnectSuccess = "INFORMATION: rooms were connected successfully!";
	const string roomConnectFail = "ERROR: rooms are already connected, please choose another pair.";
	const string roomDisconnectSuccess = "INFORMATION: rooms were disconnected successfully!";
	const string roomDisconnectFail = "ERROR: rooms are not connected to each other.";
	const string roomConnectBoiler = "ERROR: destination room is the boiler, please choose another one.";
	const string connectionHasCycles = "ERROR: connections between rooms must not create cycles.";
	const string unknownError = "ERROR: what have you done???";
	const string resetWeightSuccess = "INFORMATION: weights reset successfully!";

	char* formatRoom(const Room &r) const;

	string roomName(unsigned id) const;

	unsigned convertPositionX(int x);
	unsigned convertPositionY(int y);
	void drawFloorplan(int x, int y);
	bool positionRoom();
	bool findPosition();
	bool validateTemperature(double temperature) const;
	
	void updateMatrix();
	void setVertexColor(unsigned vertexId, double roomTemperature) const;
	void removePosition(int x, int y);

	unsigned findHighestTemperature() const;
	unsigned findLowestTemperature() const;
	unsigned nextID;

	Vertex<Room>* getRoom(const string &s) const;
};

#endif /* __HSYSTEM_H_ */