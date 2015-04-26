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

#ifdef _WIN32
#include "dirent.h"
#else
#include <dirent.h>
#endif

HSystem::HSystem()
{
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
	roomPositionX = 0;
	roomPositionY = 0;
	matrix = vector<vector<bool> >(matrixHeight, vector<bool>(matrixWidth, false));
	gv = new GraphViewer(700, 600, false);
	initializeWindow();
	addBoiler();
}

void HSystem::initializeWindow()
{
	gv->setBackground("background.png");
	gv->createWindow(700, 600);
	gv->defineVertexColor("blue");
	gv->defineEdgeColor("black");
	gv->rearrange();
}

void HSystem::reset()
{
	for (auto& pipe : pipes)
	{
		gv->removeEdge(pipe.first);
	}

	for (auto& room : rooms)
	{
		gv->removeNode(room.first);
	}

	rooms.clear();
	pipes.clear();
	g = Graph<Room>();
	matrix.clear();
	matrix = vector<vector<bool> >(matrixHeight, vector<bool>(matrixWidth, false));
	roomPositionX = 0;
	roomPositionY = 0;
	addBoiler();
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
	const int rowCount = 5;
	const int tableLength[rowCount] = { 8, 24, 24 };

	vector<string> tableLabel = { " ID", " From", " To" };

	UI::DisplayFrame("PIPES");
	UI::DisplayTable(rowCount, tableLabel, tableLength);

	for (auto &p : pipes)
	{
		Pipe currentPipe = p.second;
		vector<string> tableRow(rowCount);

		tableRow[0] = UI::Format(p.first, 7);
		tableRow[1] = roomName(currentPipe.source());
		tableRow[2] = roomName(currentPipe.dest());
		UI::DisplayTableRow(rowCount, tableRow, tableLength);
	}

	UI::PauseConsole();
}

void HSystem::displayRooms() const
{
	const int rowCount = 6;
	const int tableLength[rowCount] = { 8, 24, 5, 5, 13, 12 };

	Room lowestTemperature = rooms.at(findLowestTemperature());
	Room highestTemperature = rooms.at(findHighestTemperature());
	string displayString;
	vector<string> tableLabel = { " Room", " Name", "  X", "  Y", " Temperature", " Status" };

	UI::DisplayFrame("ROOMS");
	UI::DisplayTable(rowCount, tableLabel, tableLength);

	for (auto &r : rooms)
	{
		Room currentRoom = r.second;
		vector<string> tableRow(rowCount);

		tableRow[0] = UI::Format(r.first, 7);
		tableRow[1] = currentRoom.getName();
		tableRow[2] = UI::Format(currentRoom.getX(), 2);
		tableRow[3] = UI::Format(currentRoom.getY(), 2);
		tableRow[4] = UI::FormatTemperature(currentRoom.getTemperature());
		tableRow[5] = currentRoom.isEnabled() ? "Enabled" : "Disabled";
		UI::DisplayTableRow(rowCount, tableRow, tableLength);
	}

	cout << endl;
	displayString += "Lowest: ";
	displayString += lowestTemperature.getName();
	displayString += " (";
	displayString += UI::FormatTemperature(lowestTemperature.getTemperature());
	displayString += ")";
	UI::Display(displayString);
	displayString.clear();
	displayString += "Highest: ";
	displayString += highestTemperature.getName();
	displayString += " (";
	displayString += UI::FormatTemperature(highestTemperature.getTemperature());
	displayString += ")";
	UI::Display(displayString);
	cout << endl;
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

	g.addEdge(edgeId, srcRoom->second, dstRoom->second);
	addConnectionGraphViewer(edgeId, newPipe);
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

bool HSystem::loadGraph(const string &filename)
{
	reset();

	ifstream in;
	Room emptyRoom;

	in.open(filename);

	if (!in.is_open() || in.fail())
	{
		throw FileIOException(filename);
	}

	if (in.eof())
	{
		return false;
	}

	size_t numberVertices;

	in.read((char*)&numberVertices, sizeof(size_t));

	if (in.eof())
	{
		return false;
	}

	for (size_t i = 0; !in.eof() && i < numberVertices; i++)
	{
		readVertex(in);
	}

	updateMatrix();

	if (in.eof())
	{
		return false;
	}

	size_t numberEdges;

	in.read((char*)&numberEdges, sizeof(size_t));

	if (in.eof())
	{
		return false;
	}

	for (size_t i = 0; !in.eof() && i < numberEdges; i++)
	{
		readEdge(in);
	}

	in.close();

	return true;
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
	Room roomCaldeira("Caldeira", defaultTemperature, defaultWeight);

	rooms[0] = roomCaldeira;
	g.addVertex(roomCaldeira);
	gv->addNode(0, 50, 300);
	gv->setVertexColor(0, "DARK_GRAY");
	gv->setVertexLabel(0, formatRoom(roomCaldeira));
	gv->rearrange();
}

void HSystem::resetFlow()
{
	vector<Vertex<Room>* > vertices = g.getVertices();

	for (auto &v : vertices)
	{
		v->info.setCaudal(defaultWeight);
		cout << "caudal: " << v->info.getCaudal() << endl;
	}

	resetFlowGraphViewer();
	UI::DisplayMessage(resetWeightSuccess);
}

void HSystem::resetFlowGraphViewer()
{
	for (auto &r : rooms)
	{
		r.second.setCaudal(defaultWeight);
		gv->setVertexLabel(r.first, formatRoom(r.second));
	}
}

void HSystem::setVertexColor(unsigned vertexId, double roomTemperature) const
{
	if (vertexId == 0)
	{
		gv->setVertexColor(vertexId, "DARK_GRAY");
	}
	else
	{
		if (roomTemperature < 10.0)
		{
			gv->setVertexColor(vertexId, "MAGENTA");
		}
		else if (roomTemperature < 15.0)
		{
			gv->setVertexColor(vertexId, "BLUE");
		}
		else if (roomTemperature < 20.0)
		{
			gv->setVertexColor(vertexId, "CYAN");
		}
		else if (roomTemperature < 25.0)
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
T readValue(const string& prompt)
{
	string input;
	T val = T();

	cout << endl << prompt;
	getline(cin, input);

	if (cin.fail())
	{
		cin.clear();

		if (!cin.eof())
		{
			throw InvalidParameter();
		}
	}

	stringstream ss(input);

	if (!(ss >> val) || ss.rdbuf()->in_avail() != 0)
	{
		throw InvalidParameter();
	}

	return val;
}

string readString(const string& prompt)
{
	string input;

	cout << endl << prompt;
	getline(cin, input);

	if (cin.fail())
	{
		cin.clear();

		if (!cin.eof())
		{
			throw InvalidParameter();
		}
	}

	return input;
}

void HSystem::findBest(unsigned vertexId, double novaTemp)
{
	double currentWeight = 5.0;

	int weightIndex = 0;

	if (rooms.find(vertexId) == rooms.end())
	{
		return;
	}

	Vertex<Room>* targetRoom = g.getVertex(rooms.at(vertexId));

	double tempAntes = targetRoom->info.getTemperature();
	double QAntes = targetRoom->info.getCaudal();

	double worstTemperature = 0.0;

	map<unsigned, double> bestPath;
	map<unsigned, double> currentPath;

	while (true)
	{
		double tempAdicional = calculateWaterTemperature(tempAntes, QAntes, novaTemp, currentWeight);
		
		currentPath = dijkstra(targetRoom, tempAdicional, currentWeight, worstTemperature);
		currentWeight += 5.0;
	}

	currentWeight -= 5.0;

	vector<Vertex<Room>* > vertices = g.getVertices();

	// escolhe m�nimo das m�ximas varia��es

	for (auto &v : vertices)
	{
		// altera caudal das divis�es atravessadas
	}

	updateGraphViewer();

	UI::DisplayMessage("INFORMATION: room temperature successfully changed to match target.");
}

const string promptTargetRoom = "Enter target room name:";
const string promptTargetTemperature = "Enter target room temperature: ";

void HSystem::findBestMenu()
{
	string targetName = readString(promptTargetRoom);
	Vertex<Room>* targetRoom = getRoom(targetName);

	if (targetRoom == nullptr)
	{
		throw TargetRoomNotFound(targetName);
	}

	if (targetRoom->id == 0)
	{
		throw BoilerNoConnections();
	}

	double novaTemp = readValue<double>(promptTargetTemperature);

	if (!validateTemperature(novaTemp))
	{
		throw InvalidParameter();
	}

	double tempAntes = targetRoom->info.getTemperature();
	double QAntes = targetRoom->info.getCaudal();
	double tempAdicional = calculateWaterTemperature(tempAntes, QAntes, novaTemp, 15);
	double delta = 0.0;

	cout << "Temperatura adicional: " << tempAdicional << endl;
	dijkstra(targetRoom, tempAdicional, 15, delta);

	//findBest(targetRoom->id, targetTemperature);

	cout << "pior diferenca: " << delta << endl;
	UI::PauseConsole();

}

void HSystem::updateGraphViewer()
{
	vector<Vertex<Room> *> vertices = g.getVertices();

	for (auto &v : vertices)
	{
		if (rooms.find(v->id) != rooms.end())
		{
			rooms[v->id].setCaudal(v->info.getCaudal());
			rooms[v->id].setTemperature(v->info.getTemperature());
			changeTemperatureGraphViewer(v);
		}
	}
}

void HSystem::loadGraph()
{
	vector<string> foundFiles;
	size_t vectorIndex = 0;
	DIR* dirp = opendir(".");
	struct dirent* dentry;

	while ((dentry = readdir(dirp)) != NULL)
	{
		if (strlen(dentry->d_name) > 6 && strcmp(dentry->d_name + strlen(dentry->d_name) - 6, ".graph") == 0)
		{
			foundFiles.push_back(dentry->d_name);
		}
	}

	while (true)
	{
		UI::ClearConsole();
		UI::DisplayFrame("LOAD GRAPH");

		for (size_t i = 0; i < foundFiles.size(); i++)
		{
			if (i == vectorIndex)
			{
				UI::Display("-> " + foundFiles[i]);
			}
			else
			{
				UI::Display("   " + foundFiles[i]);
			}
		}

		cout << endl;
		UI::Display(pressEnterLoad);

		int c = _getch();

		if (c == 0xe0)
		{
			switch (_getch())
			{
			case 72:
			{
				if (vectorIndex > 0)
				{
					vectorIndex--;
				}
				break;
			}
			case 80:
			{
				if (vectorIndex < foundFiles.size() - 1)
				{
					vectorIndex++;
				}
				break;
			}
			}
		}
		else if (c == 0x0d || c == 0x0a)
		{
			closedir(dirp);

			if (loadGraph(foundFiles[vectorIndex]))
			{
				UI::DisplayMessage(graphLoadSuccess);
			}
			else
			{
				UI::DisplayMessage(graphLoadFail);
			}

			break;
		}
	}
}

void HSystem::saveGraph() const
{
	string graphFilename;
	char userChoice;
	ifstream in;

	graphFilename = readString(promptFilename);
	graphFilename += ".graph";
	in.open(graphFilename);

	if (in.is_open() || in.good())
	{
		in.close();
		userChoice = readValue<char>(graphSaveOverwrite);

		if (userChoice != 'y')
		{
			return;
		}
	}

	saveGraph(graphFilename);
	UI::DisplayMessage(graphSaveSuccess);
}

void HSystem::addRoom()
{
	string roomName = readString(promptSourceName);

	if (getRoom(roomName) != nullptr)
	{
		UI::DisplayMessage(roomExists);
	}
	else
	{
		double roomTemperature = readValue<double>(promptTemperature);

		if (!validateTemperature(roomTemperature))
		{
			throw InvalidParameter();
		}

		Room newRoom(roomName, roomTemperature, defaultWeight);

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
	string roomName = readString(promptRoomName);

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
	string roomName = readString(promptRoomDisable);
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
			selectedRoom->info.disable();
			disableRoomGraphViewer(selectedRoom->id);
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
	string roomName = readString(promptRoomEnable);
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
			selectedRoom->info.enable();
			enableRoomGraphViewer(selectedRoom->id, selectedRoom->info.getTemperature());
			UI::DisplayMessage(radiatorEnableSuccess);
		}
	}
}

char* HSystem::formatRoom(const Room &r) const
{
	char* buffer = new char[r.getName().length() + 32];
	sprintf(buffer, "%s (%.1fC, %.1f%)", r.getName().c_str(), r.getTemperature(), r.getCaudal());
	return buffer;
}

void HSystem::changeTemperature()
{
	string roomName = readString(promptRoomName);
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
		double roomTemperature = readValue<double>(promptNewTemperature);

		if (!validateTemperature(roomTemperature))
		{
			throw InvalidParameter();
		}

		selectedRoom->info.setTemperature(roomTemperature);
		changeTemperatureGraphViewer(selectedRoom);
		UI::DisplayMessage(temperatureSuccess);
	}
}

void HSystem::addConnection()
{
	string srcName = readString(promptSourceName);
	Vertex<Room>* srcRoom = getRoom(srcName);

	if (srcRoom == nullptr)
	{
		throw SourceRoomNotFound(srcName);
	}

	string dstName = readString(promptDestinationName);
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
		int edgeId = g.addEdge(srcRoom->info, dstRoom->info);

		if (edgeId != -1)
		{
			if (!g.isDAG())
			{
				g.removeEdge(srcRoom->info, dstRoom->info);
				UI::DisplayMessage(connectionHasCycles);
			}
			else
			{
				addConnectionGraphViewer(edgeId, Pipe(srcRoom->id, dstRoom->id));
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
	string srcName = readString(promptSourceName);
	Vertex<Room>* srcRoom = getRoom(srcName);

	if (srcRoom == nullptr)
	{
		throw SourceRoomNotFound(srcName);
	}

	string dstName = readString(promptDestinationName);
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

	string firstColumn;

	for (int k = 0; k < matrixWidth; k++)
	{
		if (k == 0)
		{
			firstColumn += "\xc9\xcd\xcd\xcd\xcb";
		}
		else if (k == matrixWidth - 1)
		{
			firstColumn += "\xcd\xcd\xcd\xbb";
		}
		else
		{
			firstColumn += "\xcd\xcd\xcd\xcb";
		}
	}

	UI::Display(firstColumn);

	for (int i = 0; i < matrixHeight; i++)
	{
		string oddColumn;
		string evenColumn;

		oddColumn += '\xba';

		for (int j = 0; j < matrixWidth; j++)
		{
			if (i == y && j == x)
			{
				oddColumn += " X \xba";
			}
			else
			{
				oddColumn += (matrix[i][j] ? " R \xba" : "   \xba");
			}
		}

		UI::Display(oddColumn);

		evenColumn += (i == matrixHeight - 1 ? '\xc8' : '\xcc');

		for (int k = 0; k < matrixWidth; k++)
		{
			if (k == matrixWidth - 1)
			{
				evenColumn += "\xcd\xcd\xcd";
				evenColumn += (i == matrixHeight - 1 ? '\xbc' : '\xb9');
			}
			else if (i == matrixHeight - 1)
			{
				evenColumn += "\xcd\xcd\xcd";
				evenColumn += (k == matrixWidth - 1 ? '\xbc' : '\xca');
			}
			else
			{
				evenColumn += "\xcd\xcd\xcd\xce";
			}
		}

		UI::Display(evenColumn);
	}

	UI::Display(pressEnterPlace);
}

bool HSystem::findPosition()
{
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

		int c = _getch();

		if (c == 0xe0)
		{
			switch (_getch())
			{
			case 72:
			{
				if (roomPositionY > 0 && !matrix[roomPositionY - 1][roomPositionX])
				{
					roomPositionY--;
				}

				break;
			}
			case 80:
			{
				if (roomPositionY < matrixHeight - 1 && !matrix[roomPositionY + 1][roomPositionX])
				{
					roomPositionY++;
				}

				break;
			}
			case 75:
			{
				if (roomPositionX > 0 && !matrix[roomPositionY][roomPositionX - 1])
				{
					roomPositionX--;
				}

				break;
			}
			case 77:
			{
				if (roomPositionX < matrixWidth - 1 && !matrix[roomPositionY][roomPositionX + 1])
				{
					roomPositionX++;
				}

				break;
			}
			}
		}
		else
		{
			if (c == 0x0d || c == 0x0a)
			{
				matrix[roomPositionY][roomPositionX] = true;
				break;
			}
		}
	}

	return true;
}

bool HSystem::validateGraph() const
{
	vector<Vertex<Room>* > vertices = g.getVertices();

	if (vertices.at(0)->adj.empty())
	{
		throw BoilerNoConnections();
	}

	for (Vertex<Room>* &v : vertices)
	{
		if (v->id != 0 && v->indegree == 0)
		{
			throw RoomNotConnected(v->info.getName());
		}
	}

	return true;
}

Vertex<Room>* HSystem::getRoom(const string &s) const
{
	return g.getVertex(Room(s));
}

void HSystem::addConnectionGraphViewer(unsigned edgeId, const Pipe &pipe)
{
	if (rooms.find(pipe.source()) != rooms.end() && rooms.find(pipe.dest()) != rooms.end())
	{
		pipes[edgeId] = pipe;
		gv->addEdge(edgeId, pipe.source(), pipe.dest(), EdgeType::DIRECTED);
		gv->rearrange();
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

void HSystem::changeWeightGraphViewer(unsigned vertexId, double weight)
{
	if (rooms.find(vertexId) != rooms.end())
	{
		rooms[vertexId].setCaudal(weight);
		gv->setVertexLabel(vertexId, formatRoom(rooms.at(vertexId)));
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

void HSystem::disableRoomGraphViewer(unsigned vertexId)
{
	if (rooms.find(vertexId) != rooms.end())
	{
		rooms[vertexId].disable();
		gv->setVertexColor(vertexId, "DARK_GRAY");
		gv->rearrange();
	}
}

void HSystem::enableRoomGraphViewer(unsigned vertexId, double temperature)
{
	if (rooms.find(vertexId) != rooms.end())
	{
		rooms[vertexId].enable();
		setVertexColor(vertexId, temperature);
		gv->rearrange();
	}
}

bool HSystem::validateTemperature(double temperature) const
{
	return (temperature >= minimumTemperature && temperature <= maximumTemperature);
}

// FUN��O COST
double HSystem::calculateRoomTemperature(double TempAntes, double QAntes, double TempAdicional, double QAdicional) const
{
	return (TempAntes * QAntes + TempAdicional * QAdicional) / (QAntes + QAdicional);
}

double HSystem::calculateWaterTemperature(double TempAntes, double QAntes, double NovaTemp, double QAdicional) const
{
	return (QAntes * TempAntes - NovaTemp * QAntes - NovaTemp * QAdicional) / -QAdicional;
}

map<unsigned, double> HSystem::dijkstra(Vertex<Room>* &dst, double tempAdicional, double QAdicional, double &worstTemperature) ////////
{
	map<unsigned, double> res;
	queue<Vertex<Room>* > q;

	if (!g.isDAG())
	{
		return res;
	}

	g.resetIndegrees();
	g.resetDists();

	if (g.getVertices().empty())
	{
		return res;
	}
	
	Vertex<Room>* boilerRoom = g.getVertex(rooms.at(0));

	boilerRoom->dist = 100.0,
	q.push(boilerRoom);
	

	while (!q.empty())
	{
		Vertex<Room>* v = q.front();

		q.pop();

		cout << "Visitou vertice " << v->id << endl;
#undef max
	
		for (const Edge<Room> &e : v->adj)
		{
			Vertex<Room>* w = e.dest;

			e.dest->indegree--;

			double tempAntes = w->info.getTemperature();
			double QAntes = w->info.getCaudal();
			double novaTemp = calculateRoomTemperature(tempAntes, QAntes, tempAdicional, QAdicional);
			
			cout << "Nova temepratura para o vertice " << w->id << ": " << novaTemp << endl;
			
			double deltaTemperatura = abs(tempAntes - novaTemp);
			
			if (deltaTemperatura < w->dist)
			{
				if (deltaTemperatura > worstTemperature)
				{
					worstTemperature = deltaTemperatura;
				}

				cout << "nova variacao minima ! " << deltaTemperatura << endl;
				w->dist = deltaTemperatura;
				res[w->id] = novaTemp;
				w->path = v;
			}

			if (w->indegree == 0)
			{
				q.push(w);
			}
		}
	}
	
	g.resetIndegrees();

	return res;
}

unsigned HSystem::findLowestTemperature() const
{
	unsigned roomId = -1;
	double lowestTemperature = maximumTemperature;

	for (auto &r : rooms)
	{
		if (r.second.getTemperature() < lowestTemperature)
		{
			lowestTemperature = r.second.getTemperature();
			roomId = r.first;
		}
	}

	return roomId;
}

unsigned HSystem::findHighestTemperature() const
{
	unsigned roomId = -1;
	double highestTemperature = minimumTemperature;

	for (auto &r : rooms)
	{
		if (r.second.getTemperature() > highestTemperature)
		{
			highestTemperature = r.second.getTemperature();
			roomId = r.first;
		}
	}

	return roomId;
}