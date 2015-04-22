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
		hSystem->removeConnection();
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

__forceinline void hSystem_changeTemperature()
{
	if (hSystem != nullptr)
	{
		hSystem->changeTemperature();
	}
}

__forceinline void hSystem_resetFlow()
{
	if (hSystem != nullptr)
	{
		hSystem->resetFlow();
	}
}

__forceinline void hSystem_displayInfo()
{
	if (hSystem != nullptr)
	{
		hSystem->displayInfo();
	}
}

void settingsMenu()
{
	Menu settingsMenu("Settings");

	settingsMenu.setWidth(40);
	settingsMenu.addItem('1', "Create room");
	settingsMenu.addHandler('1', hSystem_addRoom);
	settingsMenu.addItem('2', "Change temperature");
	settingsMenu.addHandler('2', hSystem_changeTemperature);
	settingsMenu.addItem('3', "Remove room");
	settingsMenu.addHandler('3', hSystem_removeRoom);
	settingsMenu.addSeparator();
	settingsMenu.addItem('4', "Create connection");
	settingsMenu.addHandler('4', hSystem_addConnection);
	settingsMenu.addItem('5', "Remove connection");
	settingsMenu.addHandler('5', hSystem_removeConnection);
	settingsMenu.addSeparator();
	settingsMenu.addItem('6', "Disable radiator");
	settingsMenu.addHandler('6', hSystem_disableRadiator);
	settingsMenu.addItem('7', "Enable radiator");
	settingsMenu.addHandler('7', hSystem_enableRadiator);
	settingsMenu.addItem('8', "Reset flow");
	settingsMenu.addHandler('8', hSystem_resetFlow);
	settingsMenu.addItem('9', "Display information");
	settingsMenu.addHandler('9', hSystem_displayInfo);
	settingsMenu.addSeparator();
	settingsMenu.addItem('0', "<- Back");
	settingsMenu.addHandler('0', nullptr);

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