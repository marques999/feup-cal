#include <iostream>
#include <fstream>
#include <conio.h>
#include "Menu.h"
#include "Graph.h"

using namespace std;

void helloWorld()
{
	cout << "Hello world!" << endl;
	system("pause");
}

void helloHell()
{
	cout << "HELL0A S9V0AS9GFS0A9GS" << endl;
	system("pause");
}

void exportGraph()
{
	cout << "Exporting raph as html.." << endl;
}

void settingsMenu()
{
	Menu settingsMenu("Settings");

	settingsMenu.setWidth(40);
	settingsMenu.addItem('1', "Create room");
	settingsMenu.addItem('2', "Remove room");
	settingsMenu.addSeparator();
	settingsMenu.addItem('3', "Create connection");
	settingsMenu.addItem('4', "Remove connection");
	settingsMenu.addSeparator();
	settingsMenu.addItem('5', "Disable radiador");
	settingsMenu.addSeparator();
	settingsMenu.addItem('6', "<- Back");
	settingsMenu.addHandler('6', nullptr);
	settingsMenu.run();
}

void mainMenu()
{
	Menu mainMenu("Sistema de Aquecimento Central");

	mainMenu.setWidth(40);
	mainMenu.addItem('1', "View House");
	mainMenu.addItem('2', "Edit House");
	mainMenu.addItem('3', "Export graph");
	mainMenu.addItem('4', "Reset");
	mainMenu.addSeparator();
	mainMenu.addItem('5', "Exit");
	mainMenu.addHandler('1', helloWorld);
	mainMenu.addHandler('2', settingsMenu);
	mainMenu.addHandler('3', exportGraph);
	mainMenu.addHandler('5', nullptr);
	mainMenu.run();
}

int main()
{
	Graph<int> gf;

	gf.addVertex(4);
	gf.addVertex(2);
	gf.addVertex(3);
	gf.addEdge(2, 3, 3.0);
	gf.addEdge(3, 4, 2.0);
	gf.addEdge(2, 4, 1.0);
	
	vector<int> v = gf.topologicalOrder();
	
	for (const int &x : v)
	{
		cout << x << endl;
	}


	//mainMenu();

	return 0;
}