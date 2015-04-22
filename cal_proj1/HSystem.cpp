/*!
 * \file HSystem.cpp
 *
 * FEUP_CAL1415_2MIEIC02_D
 * \author Diogo Marques
 * \author Jose Taveira
 * \author Vitor Esteves
 *
 * \date Abril 2015
 *
 */

#include "HSystem.h"

using namespace std;

HSystem::HSystem()
{
	nextID = 0;
	initialize();
}

HSystem::~HSystem()
{
	if (gv)
	{
		delete gv;
	}
}

void HSystem::initialize()
{
	matrixWidth = 6;
	matrixHeight = 6;
	matrix = vector<vector<bool> >(matrixHeight, vector<bool>(matrixWidth, false));

	gv = new GraphViewer(700, 600, false);
	gv->setBackground("background.png");
	gv->createWindow(700, 600);
	gv->defineVertexColor("blue");
	gv->defineEdgeColor("black");

	addBoiler();
	nextID = 0;
}

void HSystem::reset()
{
	gv->closeWindow();
	rooms.clear();
	connections.clear();
	g = Graph<Room>();
	initialize();
}

unsigned HSystem::convertPositionX(int x)
{
	double xCoord = 100 + (600 / (double)matrixWidth) * x + ((600 / (double)matrixWidth) * 0.5);
	return (unsigned)round(xCoord);
}

unsigned HSystem::convertPositionY(int y)
{
	double yCoord = (600 / matrixHeight) * y + ((600 / matrixHeight) * 0.5);
	return (unsigned)round(yCoord);
}

void HSystem::displayConnections() const
{
	const int rowCount = 4;
	const int tableLength[rowCount] = { 8, 24, 24, 12 };

	vector<string> tableLabel = { " ID", " From", " To", " Quantity" };

	UI::DisplayFrame("CONNECTIONS");
	UI::DisplayTable(rowCount, tableLabel, tableLength);

	for (auto &c : connections)
	{
		vector<string> tableRow(rowCount);

		tableRow[0] = UI::Format(c.first, 8);
		tableRow[1] = roomName(c.second.first);
		tableRow[2] = roomName(c.second.second);
		tableRow[3] = "0";

		UI::DisplayTableRow(rowCount, tableRow, tableLength);
	}

	UI::DisplayMessage("Press any key to continue...");
}

void HSystem::displayRooms() const
{
	const int rowCount = 4;
	const int tableLength[rowCount] = { 8, 32, 12, 12 };

	vector<string> tableLabel = { " Room", " Name", " Temperature", " Status" };

	UI::DisplayFrame("ROOMS");
	UI::DisplayTable(rowCount, tableLabel, tableLength);

	for (auto &r : rooms)
	{
		vector<string> tableRow(rowCount);

		tableRow[0] = UI::Format(r.first, 8);
		tableRow[1] = r.second.getName();
		tableRow[2] = UI::Format(r.second.getTemperature(), 8);
		tableRow[3] = r.second.isEnabled() ? "Enabled" : "Disabled";

		UI::DisplayTableRow(rowCount, tableRow, tableLength);
	}

	UI::DisplayMessage("Press any key to continue...");
}

string HSystem::roomName(unsigned id) const
{
	return rooms.find(id)->second.getName();
}

void HSystem::readVertex(ifstream &fin)
{
	int vertexId;

	fin.read((char*)&vertexId, sizeof(unsigned));

	if (vertexId != 0)
	{
		Room newRoom(fin);

		g.addVertex(vertexId, newRoom);
		addRoomGraphViewer(vertexId, newRoom);
	}
}

void HSystem::writeVertex(unsigned vertexId, const Room &room, ofstream &fout) const
{
	if (vertexId < rooms.size())
	{
		fout.write((char*)vertexId, sizeof(unsigned));
		room.write(fout);
	}
}

void HSystem::loadGraph(const string &filename)
{
	ifstream in;
	Room emptyRoom;

	in.open(filename);

	if (!in.is_open() || in.fail())
	{
		throw FileIOException(filename);
	}

	if (in.eof())
	{
		return;
	}

	size_t numberVertices;

	in.read((char*)&numberVertices, sizeof(size_t));

	for (size_t i = 0; !in.eof() && i < numberVertices; i++)
	{
		readVertex(in);
	}

	updateMatrix();

	if (in.eof())
	{
		return;
	}

	size_t numberEdges;

	in.read((char*)&numberEdges, sizeof(size_t));

	for (size_t i = 0; !in.eof() && i < numberEdges; i++)
	{
	}

	in.close();
}

void HSystem::saveGraph(const string &filename) const
{
	ofstream out;
	Room emptyRoom;

	out.open(filename);

	if (!out.is_open() || out.fail())
	{
		throw FileIOException(filename);
	}

	size_t numberVertices = g.getNumVertex();

	out.write((char*)&numberVertices, sizeof(size_t));

	vector<Vertex<Room>* > vertices = g.getVertices();

	for (auto &v : vertices)
	{
		out.write((char*)&(v->id), sizeof(unsigned));
		v->info.write(out);
	}

	size_t numberEdges = connections.size();

	out.write((char*)&numberEdges, sizeof(size_t));

	for (auto &e : connections)
	{
		map<unsigned, Room>::const_iterator srcRoom = rooms.find(e.second.first);
		map<unsigned, Room>::const_iterator dstRoom = rooms.find(e.second.second);

		if (srcRoom == rooms.end() || dstRoom == rooms.end())
		{
			continue;
		}

		out.write((char*)&(e.first), sizeof(unsigned));
		out.write((char*)&(e.second.first), sizeof(unsigned));
		out.write((char*)&(e.second.second), sizeof(unsigned));
	}

	out.close();
}

void HSystem::addBoiler()
{
	Room roomCaldeira("Caldeira", 20.0);

	rooms[0] = roomCaldeira;
	g.addVertex(roomCaldeira);
	gv->addNode(0, 50, 300);
	gv->setVertexColor(0, "DARK_GRAY");
	gv->setVertexLabel(0, formatRoom(roomCaldeira));
	gv->rearrange();
}

void HSystem::resetFlow()
{
	g.resetWeights(defaultWeight);
	UI::DisplayMessage("INFORMATION: weights reset successfully!");
	saveGraph("this.graph");
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

void HSystem::updateMatrix()
{
	for (auto &r : rooms)
	{
		if (r.first)
		{
			matrix[r.second.getY()][r.second.getX()] = true;
		}
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
				cout << "ERROR: invalid value, please try again...";
				continue;
			}
		}

		stringstream ss(input);

		if (!(ss >> val) || ss.rdbuf()->in_avail() != 0)
		{
			cout << "ERROR: invalid value, please try again...";
		}
		else
		{
			if (validator(val))
			{
				success = true;
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
	string roomName = readValue<string>("\nEnter source room name: ");

	if (getRoom(roomName) != nullptr)
	{
		UI::DisplayMessage("ERROR: a room with the same name already exists.");
	}
	else
	{
		double roomTemperature = readValue<double>("Enter the room temperature:", [](double t)
		{
			return (t >= 0.0 && t <= 42.0);
		});

		Room newRoom(roomName, roomTemperature);

		if (positionRoom())
		{
			newRoom.setPosition(roomPositionX, roomPositionY);

			int vertexId = g.addVertex(newRoom);

			if (vertexId != -1)
			{
				addRoomGraphViewer(vertexId, newRoom);
				UI::DisplayMessage("INFORMATION: room added successfully!");
			}
		}
		else
		{
			UI::DisplayMessage("ERROR: house is currently full, please remove a room first.");
		}
	}
}

void HSystem::removePosition(int x, int y)
{
	if (x >= 0 && x < matrixWidth && y >= 0 && y < matrixHeight)
	{
		matrix[y][x] = false;
	}
}

void HSystem::removeRoom()
{
	string roomName = readValue<string>("\nEnter source room name: ");

	if (getRoom(roomName) == nullptr)
	{
		throw SourceRoomNotFound(roomName);
	}

	Room newRoom(roomName);

	int vertexId = g.removeVertex(newRoom);

	if (vertexId == -1)
	{
		return;
	}

	map<unsigned, Room>::const_iterator it = rooms.find(vertexId);

	if (it != rooms.end())
	{
		removePosition(it->second.getX(), it->second.getY());
	}

	gv->removeNode(vertexId);
	gv->rearrange();
	UI::DisplayMessage("INFORMATION: room removed successfully!");
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
		UI::DisplayMessage("ERROR: boiler status can't be changed.");
	}
	else
	{
		gv->setVertexColor(disabledRoom->id, "DARK_GRAY");
		gv->rearrange();
		UI::DisplayMessage("INFORMATION: room radiator disabled successfully!");
	}
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

	Vertex<Room>* selectedRoom = getRoom(roomName);

	if (selectedRoom == nullptr)
	{
		throw SourceRoomNotFound(roomName);
	}

	if (selectedRoom->id == 0)
	{
		UI::DisplayMessage("ERROR: boiler status can't be changed.");
	}
	else
	{
		setVertexColor(selectedRoom->id, selectedRoom->info.getTemperature());
		gv->rearrange();
		UI::DisplayMessage("INFORMATION: room radiator enabled successfully!");
	}
}

void HSystem::changeTemperature()
{
	string roomName = readValue<string>("\nEnter target room name: ", [](const string& s)
	{
		return s != "";
	});

	Vertex<Room>* selectedRoom = getRoom(roomName);

	if (selectedRoom == nullptr)
	{
		throw SourceRoomNotFound(roomName);
	}

	if (selectedRoom->id == 0)
	{
		UI::DisplayMessage("ERROR: boiler status can't be changed.");
	}
	else
	{
		double roomTemperature = readValue<double>("Enter the new room temperature:", [](double t)
		{
			return (t >= 0.0 && t <= 100.0);
		});

		selectedRoom->info.setTemperature(roomTemperature);
		changeTemperatureGraphViewer(selectedRoom);
		UI::DisplayMessage("INFORMATION: room temperature successfully changed!");
	}
}

void HSystem::drawFloorplan(int x, int y)
{
	UI::ClearConsole();
	UI::DisplayFrame("POSITION ROOM");

	for (int i = 0; i < matrix.size(); i++)
	{
		cout << "\t\t\t\t|";

		for (int j = 0; j < matrix[i].size(); j++)
		{
			if (i == y && j == x)
			{
				cout << " X |";
			}
			else
			{
				cout << (matrix[i][j] ? " R |" : "   |");
			}
		}

		cout << "\n\t\t\t\t";

		for (int k = 0; k < 4 * matrixWidth + 1; k++)
		{
			cout << "-";
		}

		cout << "\n";
	}
}

bool HSystem::positionRoom()
{
	roomPositionX = 0;
	roomPositionY = 0;

	while (matrix[roomPositionY][roomPositionX] && roomPositionX < matrixWidth)
	{
		roomPositionX++;
	}

	while (matrix[roomPositionY][roomPositionX] && roomPositionY < matrixHeight)
	{
		roomPositionY++;
	}

	if (matrix[roomPositionY][roomPositionX])
	{
		return false;
	}

	while (true)
	{
		drawFloorplan(roomPositionX, roomPositionY);

		switch (_getch())
		{
		case 'w':
			if (roomPositionY > 0 && !matrix[roomPositionY - 1][roomPositionX])
			{
				roomPositionY--;
			}
			break;
		case 's':
			if (roomPositionY < matrixHeight - 1 && !matrix[roomPositionY + 1][roomPositionX])
			{
				roomPositionY++;
			}
			break;
		case 'a':
			if (roomPositionX > 0 && !matrix[roomPositionY][roomPositionX - 1])
			{
				roomPositionX--;
			}
			break;
		case 'd':
			if (roomPositionX < matrixWidth - 1 && !matrix[roomPositionY][roomPositionX + 1])
			{
				roomPositionX++;
			}
			break;
		case '0':
			matrix[roomPositionY][roomPositionX] = true;
			return true;
		}
	}
}

Vertex<Room>* HSystem::getRoom(const string &s) const
{
	return g.getVertex(Room(s));
}

void HSystem::addConnection()
{
	string srcName = readValue<string>("\nEnter source room name: ");
	Vertex<Room>* srcRoom = getRoom(srcName);

	if (srcRoom == nullptr)
	{
		throw SourceRoomNotFound(srcName);
	}

	string dstName = readValue<string>("Enter destination room name: ");
	Vertex<Room>* dstRoom = getRoom(dstName);

	if (dstRoom == nullptr)
	{
		throw DestinationRoomNotFound(dstName);
	}

	for (auto &e : connections)
	{
		if (e.second.first == srcRoom->id && e.second.second == dstRoom->id)
		{
			UI::DisplayMessage("ERROR: rooms are already connected, please choose another pair.");
			return;
		}
	}

	if (dstRoom->id == 0)
	{
		UI::DisplayMessage("ERROR: destination room is the boiler, please choose another one.");
	}
	else
	{
		if (g.addEdge(srcRoom->info, dstRoom->info, defaultWeight))
		{
			if (!g.isDAG())
			{
				g.removeEdge(srcRoom->info, dstRoom->info);
				UI::DisplayMessage("ERROR: connections between rooms must not create cycles.");
			}
			else
			{
				addConnectionGraphViewer(make_pair(srcRoom->id, dstRoom->id));
				UI::DisplayMessage("INFORMATION: rooms were connected successfully!");
			}
		}
		else
		{
			UI::DisplayMessage("ERROR: what have you done???");
		}
	}
}

void HSystem::removeConnection()
{
	string srcName = readValue<string>("\nEnter source room name: ");
	Vertex<Room>* srcRoom = getRoom(srcName);

	if (srcRoom == nullptr)
	{
		throw SourceRoomNotFound(srcName);
	}

	string dstName = readValue<string>("Enter destination room name: ");
	Vertex<Room>* dstRoom = getRoom(dstName);

	if (dstRoom == nullptr)
	{
		throw DestinationRoomNotFound(dstName);
	}

	if (g.removeEdge(srcRoom->info, dstRoom->info))
	{
		removeConnectionGraphViewer(make_pair(srcRoom->id, dstRoom->id));
		UI::DisplayMessage("INFORMATION: rooms disconnected successfully!");
	}
	else
	{
		UI::DisplayMessage("ERROR: rooms are not connected to each other.");
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
	gv->addNode(vertexId, convertPositionX(room.getX()), convertPositionY(room.getY()));
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