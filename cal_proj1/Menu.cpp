#include "HSystem.h"
#include "Menu.h"
#include "Exceptions.h"
HSystem* hSystem = nullptr;

__forceinline void hSystem_addRoom()
{
	if (hSystem != nullptr)
	{
		hSystem->addRoom();
	}
}

__forceinline void hSystem_addConnection()
{
	if (hSystem != nullptr)
	{
		hSystem->addConnection();
	}
}

__forceinline void hSystem_removeRoom()
{
	if (hSystem != nullptr)
	{
		hSystem->removeRoom();
	}
}

__forceinline void hSystem_removeConnection()
{
	if (hSystem != nullptr)
	{
		hSystem->removeEdge();
	}
}

__forceinline void hSystem_enableRadiator()
{
	if (hSystem != nullptr)
	{
		hSystem->enableRoom();
	}
}

__forceinline void hSystem_disableRadiator()
{
	if (hSystem != nullptr)
	{
		hSystem->disableRoom();
	}
}

void settingsMenu()
{
	Menu settingsMenu("Settings");

	settingsMenu.setWidth(40);
	settingsMenu.addItem('1', "Create room");
	settingsMenu.addHandler('1', hSystem_addRoom);
	settingsMenu.addItem('2', "Remove room");
	settingsMenu.addHandler('2', hSystem_removeRoom);
	settingsMenu.addSeparator();
	settingsMenu.addItem('3', "Create connection");
	settingsMenu.addHandler('3', hSystem_addConnection);
	settingsMenu.addItem('4', "Remove connection");
	settingsMenu.addHandler('4', hSystem_removeConnection);
	settingsMenu.addSeparator();
	settingsMenu.addItem('5', "Disable radiator");
	settingsMenu.addHandler('5', hSystem_disableRadiator);
	settingsMenu.addItem('6', "Enable radiator");
	settingsMenu.addHandler('6', hSystem_enableRadiator);
	settingsMenu.addSeparator();
	settingsMenu.addItem('7', "<- Back");
	settingsMenu.addHandler('7', nullptr);

	try 
	{
		settingsMenu.run();
	}
	catch (SourceRoomNotFound &srnf)
	{
		cout << endl << srnf << endl;
		system("pause");
	}
	catch (DestinationRoomNotFound &drnf)
	{
		cout << endl << drnf << endl;
		system("pause");
	}
}

void mainMenu()
{
	Menu mainMenu("Sistema de Aquecimento Central");

	mainMenu.setWidth(40);
	mainMenu.addItem('1', "View House");
	mainMenu.addItem('2', "Edit House");
	mainMenu.addHandler('2', settingsMenu);
	mainMenu.addItem('3', "Export graph");
	mainMenu.addItem('4', "Reset");
	mainMenu.addSeparator();
	mainMenu.addItem('5', "Exit");
	mainMenu.addHandler('5', nullptr);
	mainMenu.run();
}

int main()
{
	hSystem =new  HSystem();
	mainMenu();
	return 0;
}