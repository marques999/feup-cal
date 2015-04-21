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

void HSystem::initialize()
{
	gv = new GraphViewer(1280, 720, true);
	gv->setBackground("background.jpg");
	gv->createWindow(1280, 720);
	gv->defineVertexColor("blue");
	gv->defineEdgeColor("black");
	
	Room roomCaldeira("Caldeira", 20.0);

	g.addVertex(roomCaldeira),
	gv->addNode(0);
	gv->setVertexColor(0, "DARK_GRAY");
	gv->setVertexLabel(0, "Caldeira");
	gv->rearrange();
	nextID = 0;
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

void HSystem::addRoomGraphViewer(unsigned vertexId, const Room& room)
{
	gv->addNode(vertexId);
	gv->setVertexLabel(vertexId, room.getName());
	setVertexColor(vertexId, room.getTemperature());
	gv->rearrange();
}

template <typename T>
T readValue(const string& prompt, function<bool(T)> validator)
{
	bool success = false;
	T val = T();

	while (!success)
	{
		std::cout << prompt;

		std::string input;
		std::getline(std::cin, input);

		if (std::cin.fail())
		{
			if (std::cin.eof())
			{
				cin.clear();
			}
			else
			{
				cin.clear();
				cout << "\nERROR: invalid value, please try again...\n";
				continue;
			}
		}

		std::stringstream ss(input);

		if (!(ss >> val) || ss.rdbuf()->in_avail() != 0)
		{
			cout << "\nERROR: invalid value, please try again...\n";
		}
		else
		{
			if (validator(val))
			{
				success = true;
			}
			else
			{
				cout << "\nERROR: invalid value, please try again...\n";
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
	// não pode ativar nem desativar a caldeira
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
		cout << "\nINFORMATION: caldeira status can't be changed!\n";
		system("pause");
		return;
	}

	gv->setVertexColor(disabledRoom->id, "DARK_GRAY");
	gv->rearrange();

	cout << "\nINFORMATION: room radiator disabled successfully!\n";
	system("pause");
}

void HSystem::enableRoom()
{
	// não pode ativar nem desativar a caldeira
	string roomName = readValue<string>("\nEnter the room name to be enabled: ", [](const string& s)
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
		cout << "\nINFORMATION: caldeira status can't be changed!\n";
		system("pause");
		return;
	}

	setVertexColor(enabledRoom->id, enabledRoom->info.getTemperature());
	gv->rearrange();

	cout << "\nINFORMATION: room radiator enabled successfully!\n";
	system("pause");
}

void HSystem::removeEdge()
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
		cout << "\nINFORMATION: room added successfully!\n";
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
	string srcName = readValue<string>("\nEnter the source room name: ");
	Vertex<Room>* srcRoom = getRoom(srcName);

	if (srcRoom == nullptr)
	{
		throw SourceRoomNotFound(srcName);
	}

	string dstName = readValue<string>("Enter the destinationr room name:");
	Vertex<Room>* dstRoom = getRoom(dstName);

	if (dstRoom == nullptr)
	{
		throw DestinationRoomNotFound(dstName);
	}

	if (g.addEdge(srcRoom->info, dstRoom->info, 70.0))
	{
		int currentID = nextID;
		gv->addEdge(currentID, srcRoom->id, dstRoom->id, EdgeType::DIRECTED);
		gv->setEdgeThickness(currentID, 7);
		gv->setEdgeLabel(currentID, "70.0");
		gv->rearrange();
		nextID++;
		cout << "\nINFORMATION: room added successfully!\n";
		system("pause");
	}
	else
	{
		throw RoomExists();
	}
}