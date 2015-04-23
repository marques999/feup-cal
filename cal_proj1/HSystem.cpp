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
	pipes.clear();
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

	UI::DisplayFrame("PIPES");
	UI::DisplayTable(rowCount, tableLabel, tableLength);

	for (auto &p : pipes)
	{
		vector<string> tableRow(rowCount);

		tableRow[0] = UI::Format(p.first, 8);
		tableRow[1] = roomName(p.second.source());
		tableRow[2] = roomName(p.second.dest());
		tableRow[3] = UI::FormatWeight(p.second.weight(), 1);

		UI::DisplayTableRow(rowCount, tableRow, tableLength);
	}

	UI::PauseConsole();
}

void HSystem::displayRooms() const
{
	const int rowCount = 6;
	const int tableLength[rowCount] = { 8, 24, 5, 5, 16, 12 };

	vector<string> tableLabel = { " Room", " Name", "  X", "  Y", " Temperature", " Status" };

	UI::DisplayFrame("ROOMS");
	UI::DisplayTable(rowCount, tableLabel, tableLength);

	for (auto &r : rooms)
	{
		vector<string> tableRow(rowCount);

		tableRow[0] = UI::Format(r.first, 8);
		tableRow[1] = r.second.getName();
		tableRow[2] = UI::Format(r.second.getX(), 4);
		tableRow[3] = UI::Format(r.second.getY(), 4);
		tableRow[4] = UI::FormatTemperature(r.second.getTemperature());
		tableRow[5] = r.second.isEnabled() ? "Enabled" : "Disabled";

		UI::DisplayTableRow(rowCount, tableRow, tableLength);
	}

	UI::PauseConsole();
}

string HSystem::roomName(unsigned id) const
{
	return rooms.find(id)->second.getName();
}

void HSystem::readEdge(ifstream &fin)
{
	unsigned edgeId;

	fin.read((char*)&edgeId, sizeof(unsigned));

	Pipe newPipe(fin);

	map<unsigned, Room>::const_iterator srcRoom = rooms.find(newPipe.source());
	map<unsigned, Room>::const_iterator dstRoom = rooms.find(newPipe.dest());

	if (newPipe.dest() == 0 || srcRoom == rooms.end() || dstRoom == rooms.end())
	{
		return;
	}

	g.addEdge(srcRoom->second, dstRoom->second, newPipe.weight());
	addConnectionGraphViewer(newPipe);
}

void HSystem::readVertex(ifstream &fin)
{
	unsigned vertexId;

	fin.read((char*)&vertexId, sizeof(unsigned));

	if (vertexId != 0)
	{
		Room newRoom(fin);

		g.addVertex(vertexId, newRoom);
		addRoomGraphViewer(vertexId, newRoom);
	}
}

void HSystem::writeEdge(unsigned edgeId, ofstream &fout) const
{
	if (pipes.find(edgeId) == pipes.end())
	{
		return;
	}

	Pipe currentEdge = pipes.at(edgeId);
	map<unsigned, Room>::const_iterator srcRoom = rooms.find(currentEdge.source());
	map<unsigned, Room>::const_iterator dstRoom = rooms.find(currentEdge.dest());

	if (currentEdge.dest() == 0 || srcRoom == rooms.end() || dstRoom == rooms.end())
	{
		return;
	}

	fout.write((char*)&edgeId, sizeof(unsigned));
	pipes.at(edgeId).write(fout);
}

void HSystem::writeVertex(unsigned vertexId, ofstream &fout) const
{
	if (rooms.find(vertexId) != rooms.end())
	{
		if (vertexId != 0)
		{
			fout.write((char*)&vertexId, sizeof(unsigned));
			rooms.at(vertexId).write(fout);
		}
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

	if (in.eof())
	{
		return;
	}

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

	if (in.eof())
	{
		return;
	}

	for (size_t i = 0; !in.eof() && i < numberEdges; i++)
	{
		readEdge(in);
	}

	in.close();
}

void HSystem::saveGraph(const string &filename) const
{
	ofstream out;

	out.open(filename);

	if (!out.is_open() || out.fail())
	{
		throw FileIOException(filename);
	}

	size_t numberVertices = g.getNumVertex() - 1;

	out.write((char*)&numberVertices, sizeof(size_t));

	vector<Vertex<Room>* > vertices = g.getVertices();

	for (auto &v : vertices)
	{
		if (v->id != 0)
		{
			out.write((char*)&(v->id), sizeof(unsigned));
			v->info.write(out);
		}
	}

	size_t numberEdges = pipes.size();

	out.write((char*)&numberEdges, sizeof(size_t));

	for (auto &e : pipes)
	{
		writeEdge(e.first, out);
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
	UI::DisplayMessage(resetWeightSuccess);
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

	cout << endl << prompt;

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
			cout << "ERROR: invalid value entered.";
		}
	}

	stringstream ss(input);

	if (!(ss >> val) || ss.rdbuf()->in_avail() != 0)
	{
		cout << "ERROR: invalid value entered.";
	}
	else
	{
		if (validator(val))
		{
			success = true;
		}
		else
		{
			cout << "ERROR: invalid value entered.";
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
	string roomName = readValue<string>(promptSourceName);

	if (getRoom(roomName) != nullptr)
	{
		UI::DisplayMessage(roomExists);
	}
	else
	{
		double roomTemperature = readValue<double>(promptTemperature, [](double t)
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
				UI::DisplayMessage(addRoomSuccess);
			}
		}
		else
		{
			UI::DisplayMessage(addRoomFail);
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
	string roomName = readValue<string>(promptRoomName);

	if (getRoom(roomName) == nullptr)
	{
		throw SourceRoomNotFound(roomName);
	}

	Room newRoom(roomName);

	int vertexId = g.removeVertex(newRoom);

	if (vertexId == -1)
	{
		throw SourceRoomNotFound(roomName);
	}

	map<unsigned, Room>::const_iterator it = rooms.find(vertexId);

	if (vertexId == 0)
	{
		UI::DisplayMessage(radiatorIsBoiler);
	}
	else
	{
		if (it != rooms.end())
		{
			removePosition(it->second.getX(), it->second.getY());
		}

		removeRoomGraphViewer(vertexId);
		UI::DisplayMessage(removeRoomSuccess);
	}
}

void HSystem::disableRoom()
{
	string roomName = readValue<string>(promptRoomDisable);
	Vertex<Room>* selectedRoom = getRoom(roomName);

	if (selectedRoom == nullptr)
	{
		throw SourceRoomNotFound(roomName);
	}

	if (selectedRoom->id == 0)
	{
		UI::DisplayMessage(radiatorIsBoiler);
	}
	else
	{
		if (selectedRoom->info.isEnabled())
		{
			gv->setVertexColor(selectedRoom->id, "DARK_GRAY");
			gv->rearrange();
			selectedRoom->info.disable();
			UI::DisplayMessage(radiatorDisableSuccess);
		}
		else
		{
			UI::DisplayMessage(radiatorDisableFail);
		}
	}
}

void HSystem::enableRoom()
{
	string roomName = readValue<string>(promptRoomEnable);
	Vertex<Room>* selectedRoom = getRoom(roomName);

	if (selectedRoom == nullptr)
	{
		throw SourceRoomNotFound(roomName);
	}

	if (selectedRoom->id == 0)
	{
		UI::DisplayMessage(radiatorIsBoiler);
	}
	else
	{
		if (selectedRoom->info.isEnabled())
		{
			UI::DisplayMessage(radiatorEnableFail);
		}
		else
		{
			setVertexColor(selectedRoom->id, selectedRoom->info.getTemperature());
			selectedRoom->info.enable();
			gv->rearrange();
			UI::DisplayMessage(radiatorEnableSuccess);
		}
	}
}

char* HSystem::formatRoom(const Room &r) const
{
	const size_t stringLength = r.getName().length() + 1;
	char* buffer = new char[stringLength + 10];
	sprintf(buffer, "%s (%.1fC)", r.getName().c_str(), r.getTemperature());
	return buffer;
}

void HSystem::changeTemperature()
{
	string roomName = readValue<string>(promptRoomName);
	Vertex<Room>* selectedRoom = getRoom(roomName);

	if (selectedRoom == nullptr)
	{
		throw SourceRoomNotFound(roomName);
	}

	if (selectedRoom->id == 0)
	{
		UI::DisplayMessage(radiatorIsBoiler);
	}
	else
	{
		double roomTemperature = readValue<double>(promptNewTemperature, [](double t)
		{
			return (t >= 0.0 && t <= 100.0);
		});

		selectedRoom->info.setTemperature(roomTemperature);
		changeTemperatureGraphViewer(selectedRoom);
		UI::DisplayMessage(temperatureSuccess);
	}
}

void HSystem::addConnection()
{
	string srcName = readValue<string>(promptSourceName);
	Vertex<Room>* srcRoom = getRoom(srcName);

	if (srcRoom == nullptr)
	{
		throw SourceRoomNotFound(srcName);
	}

	string dstName = readValue<string>(promptDestinationName);
	Vertex<Room>* dstRoom = getRoom(dstName);

	if (dstRoom == nullptr)
	{
		throw DestinationRoomNotFound(dstName);
	}

	for (auto &e : pipes)
	{
		if (e.second.source() == srcRoom->id && e.second.dest() == dstRoom->id)
		{
			UI::DisplayMessage(roomConnectFail);
			return;
		}
	}

	if (dstRoom->id == 0)
	{
		UI::DisplayMessage(roomConnectBoiler);
	}
	else
	{
		if (g.addEdge(srcRoom->info, dstRoom->info, defaultWeight))
		{
			if (!g.isDAG())
			{
				g.removeEdge(srcRoom->info, dstRoom->info);
				UI::DisplayMessage(connectionHasCycles);
			}
			else
			{
				addConnectionGraphViewer(Pipe(srcRoom->id, dstRoom->id, defaultWeight));
				UI::DisplayMessage(roomConnectSuccess);
			}
		}
		else
		{
			UI::DisplayMessage(unknownError);
		}
	}
}

void HSystem::removeConnection()
{
	string srcName = readValue<string>(promptSourceName);
	Vertex<Room>* srcRoom = getRoom(srcName);

	if (srcRoom == nullptr)
	{
		throw SourceRoomNotFound(srcName);
	}

	string dstName = readValue<string>(promptDestinationName);
	Vertex<Room>* dstRoom = getRoom(dstName);

	if (dstRoom == nullptr)
	{
		throw DestinationRoomNotFound(dstName);
	}

	if (g.removeEdge(srcRoom->info, dstRoom->info))
	{
		removeConnectionGraphViewer(srcRoom->id, dstRoom->id);
		UI::DisplayMessage(roomDisconnectSuccess);
	}
	else
	{
		UI::DisplayMessage(roomDisconnectFail);
	}
}

void HSystem::drawFloorplan(int x, int y)
{
	UI::ClearConsole();
	UI::DisplayFrame("POSITION ROOM");

	for (int i = 0; i < matrixHeight; i++)
	{
		cout << "   \t\t\t\xba";

		for (int j = 0; j < matrixWidth; j++)
		{
			if (i == y && j == x)
			{
				cout << " X \xba";
			}
			else
			{
				cout << (matrix[i][j] ? " R \xba" : "   \xba");
			}
		}

		cout << "\n   \t\t\t\xcd";

		for (int k = 0; k < matrixWidth; k++)
		{
			cout << "\xcd\xcd\xcd\xce";
		}

		cout << "\xcd\n";
	}
}

bool HSystem::findPosition()
{
	bool foundPosition = false;

	for (roomPositionY = 0; roomPositionY < matrixHeight; roomPositionY++)
	{
		for (roomPositionX = 0; roomPositionX < matrixWidth; roomPositionX++)
		{
			if (!matrix[roomPositionY][roomPositionX])
			{
				return true;
			}
		}
	}

	return false;
}

bool HSystem::positionRoom()
{
	if (!findPosition())
	{
		return false;
	}

	while (true)
	{
		drawFloorplan(roomPositionX, roomPositionY);

		switch (_getch())
		{
		case 'w':
		{
			if (roomPositionY > 0 && !matrix[roomPositionY - 1][roomPositionX])
			{
				roomPositionY--;
			}

			break;
		}
		case 's':
		{
			if (roomPositionY < matrixHeight - 1 && !matrix[roomPositionY + 1][roomPositionX])
			{
				roomPositionY++;
			}

			break;
		}
		case 'a':
		{
			if (roomPositionX > 0 && !matrix[roomPositionY][roomPositionX - 1])
			{
				roomPositionX--;
			}

			break;
		}
		case 'd':
		{
			if (roomPositionX < matrixWidth - 1 && !matrix[roomPositionY][roomPositionX + 1])
			{
				roomPositionX++;
			}

			break;
		}
		case '0':
		{
			matrix[roomPositionY][roomPositionX] = true;
			return true;
		}
		}
	}
}

Vertex<Room>* HSystem::getRoom(const string &s) const
{
	return g.getVertex(Room(s));
}

void HSystem::addConnectionGraphViewer(const Pipe &pipe) // FALTA WEIGHT!!!
{
	int currentID = nextID;

	if (rooms.find(pipe.source()) != rooms.end() && rooms.find(pipe.dest()) != rooms.end())
	{
		pipes[currentID] = pipe;
		gv->addEdge(currentID, pipe.source(), pipe.dest(), EdgeType::DIRECTED);
		gv->setEdgeLabel(currentID, UI::FormatWeight(pipe.weight(), 1));
		gv->rearrange();
		nextID++;
	}
}

void HSystem::addRoomGraphViewer(unsigned vertexId, const Room& room)
{
	rooms[vertexId] = room;
	gv->addNode(vertexId, convertPositionX(room.getX()), convertPositionY(room.getY()));
	gv->setVertexLabel(vertexId, formatRoom(room));

	if (room.isEnabled())
	{
		setVertexColor(vertexId, room.getTemperature());
	}
	else
	{
		gv->setVertexColor(vertexId, "DARK_GRAY");
	}

	gv->rearrange();
}

void HSystem::changeTemperatureGraphViewer(Vertex<Room>* &room)
{
	if (rooms.find(room->id) != rooms.end())
	{
		rooms[room->id].setTemperature(room->info.getTemperature());
		gv->setVertexLabel(room->id, formatRoom(room->info));
		setVertexColor(room->id, room->info.getTemperature());
		gv->rearrange();
	}
}

void HSystem::removeRoomGraphViewer(unsigned vertexId)
{
	map<unsigned, Pipe>::iterator currentPipe = pipes.begin();
	map<unsigned, Room>::iterator currentRoom = rooms.find(vertexId);

	if (currentRoom != rooms.end())
	{
		rooms.erase(currentRoom);
		gv->removeNode(vertexId);
		gv->rearrange();

		while (currentPipe != pipes.end())
		{
			if (currentPipe->second.source() == vertexId || currentPipe->second.dest() == vertexId)
			{
				currentPipe = pipes.erase(currentPipe);
			}
			else
			{
				currentPipe++;
			}
		}
	}
}

void HSystem::changeWeightGraphViewer(unsigned edgeId, unsigned weight)
{
	if (pipes.find(edgeId) != pipes.end())
	{
		pipes[edgeId].change(weight);
		gv->setEdgeLabel(edgeId, UI::FormatWeight(weight, 1));
		gv->rearrange();
	}
}

void HSystem::removeConnectionGraphViewer(unsigned src, unsigned dst)
{
	for (auto &e : pipes)
	{
		if (e.second.source() == src && e.second.dest() == dst)
		{
			gv->removeEdge(e.first);
			gv->rearrange();
			pipes.erase(e.first);
			break;
		}
	}
}
