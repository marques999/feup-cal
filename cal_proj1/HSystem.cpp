#include <iostream>
#include <fstream>
#include <conio.h>
#include <functional>
#include "edgetype.h"
#include "Exceptions.h"
#include "HSystem.h"
#include "Menu.h"
#include "Graph.h"

using namespace std;

unsigned HSystem::getPositionX(int i)
{
	return 40 + (600 / matrixWidth) * i + ((600 / matrixWidth) * 0.5);
}

void HSystem::displayInfo()
{
	system("cls");
	cout << "ROOMS:\n\n";

	for (auto &r : rooms)
	{
		cout << r.first << " | " << r.second.getName() << " | " << r.second.getTemperature();
		cout << endl;
	}

	cout << "\nCONNECTIONS:\n\n";

	for (auto &c : connections)
	{
		cout << c.first << " | from Room " << c.second.first << " to Room " << c.second.second;
		cout << endl;
	}

	system("pause");
}

unsigned HSystem::getPositionY(int j)
{
	return (600 / matrixHeight) * j + ((600 / matrixHeight) * 0.5);
}

void HSystem::initialize()
{
	matrixWidth = 6;
	matrixID = 0;
	matrixHeight = 6;

	matrix.reserve(matrixHeight);

	for (vector<bool> &v : matrix)
	{
		v.reserve(matrixWidth);
	}

	gv = new GraphViewer(600, 600, true);
	gv->setBackground("background.jpg");
	gv->createWindow(600, 600);
	gv->defineVertexColor("blue");
	gv->defineEdgeColor("black");
	
	addBoiler();

	nextID = 0;
}

void HSystem::addBoiler()
{
	Room roomCaldeira("Caldeira", 20.0);

	rooms[0] = roomCaldeira;
	g.addVertex(roomCaldeira),
	gv->addNode(0, 20, 300);
	gv->setVertexColor(0, "DARK_GRAY");
	gv->setVertexLabel(0, formatRoom(roomCaldeira));
	gv->rearrange();
}

void HSystem::resetFlow()
{
	g.resetWeights(defaultWeight);
	cout << "\nINFORMATION: weights reset successfully!\n";
	system("pause");
}

void HSystem::setVertexColor(unsigned vertexId, double roomTemperature) const
{
	if (roomTemperature <= 10.0)
	{
		gv->setVertexColor(vertexId, "MAGENTA");
	}
	else if (roomTemperature <= 15.0)
	{
		gv->setVertexColor(vertexId, "BLUE");
	}
	else if (roomTemperature <= 20.0)
	{
		gv->setVertexColor(vertexId, "CYAN");
	}
	else if (roomTemperature <= 25.0)
	{
		gv->setVertexColor(vertexId, "YELLOW");
	}
	else if (roomTemperature < 30.0)
	{
		gv->setVertexColor(vertexId, "ORANGE");
	}
	else
	{
		gv->setVertexColor(vertexId, "RED");
	}
}



template <typename T>
T readValue(const string& prompt, function<bool(T)> validator)
{
	bool success = false;
	T val = T();

	while (!success)
	{
		cout << prompt;

		string input;
		getline(std::cin, input);

		if (cin.fail())
		{
			if (cin.eof())
			{
				cin.clear();
			}
			else
			{
				cin.clear();
				cout << "\nERROR: invalid value, please try again...";
				continue;
			}
		}

		stringstream ss(input);

		if (!(ss >> val) || ss.rdbuf()->in_avail() != 0)
		{
			cout << "\nERROR: invalid value, please try again...";
		}
		else
		{
			if (validator(val))
			{
				success = true;
			}
			else
			{
				cout << "\nERROR: invalid value, please try again...";
			}
		}
	}

	return val;
}

template <typename T>
T readValue(const string& prompt)
{
	return readValue<T>(prompt, [](T) { return true; });
}

void HSystem::addRoom()
{
	string roomName = readValue<string>("\nEnter the room name: ", [](const string& s)
	{
		return s != "";
	});

	double roomTemperature = readValue<double>("Enter the room temperature:", [](double t)
	{
		return (t >= 0.0 && t <= 42.0);
	});

	Room newRoom(roomName, roomTemperature);
	
	int vertexId = g.addVertex(newRoom);

	if (vertexId == -1)
	{
		throw RoomExists();
	}

	addRoomGraphViewer(vertexId, newRoom);
		
	cout << "\nINFORMATION: room added successfully!\n";
	system("pause");
}

void HSystem::removeRoom()
{
	string roomName = readValue<string>("\nEnter the room name: ", [](const string& s)
	{
		return s != "";
	});

	Room newRoom(roomName);

	if (!roomExists(roomName))
	{
		throw SourceRoomNotFound(roomName);
	}

	int vertexId = g.removeVertex(newRoom);

	if (vertexId == -1)
	{
		return;
	}

	gv->removeNode(vertexId);
	gv->rearrange();
	cout << "\nINFORMATION: room removed successfully!\n";
	system("pause");
}

void HSystem::disableRoom()
{
	string roomName = readValue<string>("\nEnter the room name to be disabled: ", [](const string& s)
	{
		return s != "";
	});

	Vertex<Room>* disabledRoom = getRoom(roomName);

	if (disabledRoom == nullptr)
	{
		throw SourceRoomNotFound(roomName);
	}

	if (disabledRoom->id == 0)
	{
		cout << "\nERROR: boiler status can't be changed!\n";
		system("pause");
		return;
	}

	gv->setVertexColor(disabledRoom->id, "DARK_GRAY");
	gv->rearrange();

	cout << "\nINFORMATION: room radiator disabled successfully!\n";
	system("pause");
}

char* HSystem::formatRoom(const Room &r) const
{
	const size_t stringLength = r.getName().length() + 1;
	char* buffer = new char[stringLength + 10];

	sprintf(buffer, "%s (%.1fC)", r.getName().c_str(), r.getTemperature());

	return buffer;
}

void HSystem::enableRoom()
{
	string roomName = readValue<string>("\nEnter the room to be enabled: ", [](const string& s)
	{
		return s != "";
	});

	Vertex<Room>* enabledRoom = getRoom(roomName);

	if (enabledRoom == nullptr)
	{
		throw SourceRoomNotFound(roomName);
	}

	if (enabledRoom->id == 0)
	{
		cout << "\ERROR: boiler status can't be changed!\n";
		system("pause");
		return;
	}

	setVertexColor(enabledRoom->id, enabledRoom->info.getTemperature());
	gv->rearrange();

	cout << "\nINFORMATION: room radiator enabled successfully!\n";
	system("pause");
}

void HSystem::changeTemperature()
{
	string roomName = readValue<string>("\nEnter the room name: ", [](const string& s)
	{
		return s != "";
	});

	Vertex<Room>* roomPointer = getRoom(roomName);

	if (roomPointer == nullptr)
	{
		throw SourceRoomNotFound(roomName);
	}

	if (roomPointer->id == 0)
	{
		cout << "\nERROR: boiler status can't be changed!\n";
		system("pause");
		return;
	}

	double roomTemperature = readValue<double>("Enter the new room temperature:", [](double t)
	{
		return (t >= 0.0 && t <= 100.0);
	});

	roomPointer->info.setTemperature(roomTemperature);
	changeTemperatureGraphViewer(roomPointer);

	cout << "\nINFORMATION: room temperature successfully changed!\n";
	system("pause");
}

void HSystem::removeConnection()
{
	string srcName = readValue<string>("\nEnter the source room name: ");
	Vertex<Room>* srcRoom = getRoom(srcName);

	if (srcRoom == nullptr)
	{
		throw SourceRoomNotFound(srcName);
	}

	string dstName = readValue<string>("Enter the destination room name:");
	Vertex<Room>* dstRoom = getRoom(dstName);

	if (dstRoom == nullptr)
	{
		throw DestinationRoomNotFound(dstName);
	}

	if (g.removeEdge(srcRoom->info, dstRoom->info))
	{
		removeConnectionGraphViewer(make_pair(srcRoom->id, dstRoom->id));
		cout << "\nINFORMATION: room removed successfully!\n";
		system("pause");
	}
	else
	{
		throw RoomExists();
	}
}


bool HSystem::roomExists(const string &s) const
{
	Room newRoom(s);

	return g.getVertex(newRoom) != nullptr;
}

Vertex<Room>* HSystem::getRoom(const string &s) const
{
	return g.getVertex(Room(s));
}

void HSystem::addConnection()
{
	string srcName = readValue<string>("\nEnter source room: ");
	Vertex<Room>* srcRoom = getRoom(srcName);

	if (srcRoom == nullptr)
	{
		throw SourceRoomNotFound(srcName);
	}

	string dstName = readValue<string>("Enter destination room:");
	Vertex<Room>* dstRoom = getRoom(dstName);

	if (dstRoom == nullptr)
	{
		throw DestinationRoomNotFound(dstName);
	}

	if (g.addEdge(srcRoom->info, dstRoom->info, defaultWeight))
	{
		if (!g.isDAG())
		{
			g.removeEdge(srcRoom->info, dstRoom->info);
			cout << "\nERROR: connections between rooms must not create cycles!\n";
			system("pause");
		}
		else 
		{
			addConnectionGraphViewer(make_pair(srcRoom->id, dstRoom->id));
			cout << "\nINFORMATION: room added successfully!\n";
			system("pause");
		}
	}
	else
	{
		throw RoomExists();
	}
}

void HSystem::addConnectionGraphViewer(pair<unsigned, unsigned> edge)
{
	int currentID = nextID;

	if (edge.first < rooms.size() && edge.second < rooms.size())
	{
		connections[currentID] = edge;
		gv->addEdge(currentID, edge.first, edge.second, EdgeType::DIRECTED);
		gv->setEdgeLabel(currentID, "70.0");
		gv->rearrange();
		nextID++;
	}
}

void HSystem::addRoomGraphViewer(unsigned vertexId, const Room& room)
{
	rooms[vertexId] = room;
	gv->addNode(vertexId);
	gv->setVertexLabel(vertexId, formatRoom(room));
	setVertexColor(vertexId, room.getTemperature());
	gv->rearrange();
}

void HSystem::changeTemperatureGraphViewer(Vertex<Room>* &room)
{
	rooms.at(room->id).setTemperature(room->info.getTemperature());
	gv->setVertexLabel(room->id, formatRoom(room->info));
	setVertexColor(room->id, room->info.getTemperature());
	gv->rearrange();
}

void HSystem::removeRoomGraphViewer(Vertex<Room>* &room)
{
	if (room->id < rooms.size())
	{
		rooms.erase(room->id);
		gv->removeNode(room->id);
		gv->rearrange();

	/*	for (auto &e : connections)
		{
			if (e.second.first == room->id)
			{
				connections.erase(e.first);
			}
		}
		*/
	}
}

void HSystem::removeConnectionGraphViewer(pair<unsigned, unsigned> edge)
{
	if (edge.first < rooms.size() && edge.second < rooms.size())
	{
		for (auto &e : connections)
		{
			if (e.second == edge)
			{
				gv->removeEdge(e.first);
				gv->rearrange();
				connections.erase(e.first);
				break;
			}
		}
	}
}
