/*!
 * \file Menu.cpp
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
#include "Menu.h"

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

__forceinline void hSystem_displayRooms()
{
	if (hSystem != nullptr)
	{
		hSystem->displayRooms();
	}
}

__forceinline void hSystem_displayConnections()
{
	if (hSystem != nullptr)
	{
		hSystem->displayConnections();
	}
}

__forceinline void hSystem_reset()
{
	if (hSystem != nullptr)
	{
		hSystem->reset();
	}
}

__forceinline void hSystem_load()
{
	if (hSystem != nullptr)
	{
		hSystem->loadGraph("this.graph");
	}
}

void adminMenu()
{
	Menu adminMenu("View House");

	adminMenu.setWidth(40);
	adminMenu.addItem('1', "xxxx");
	adminMenu.addItem('2', "yyyy");
	adminMenu.addSeparator();
	adminMenu.addItem('3', "Display rooms");
	adminMenu.addHandler('3', hSystem_displayRooms);
	adminMenu.addItem('4', "Display connections");
	adminMenu.addHandler('4', hSystem_displayConnections);
	adminMenu.addSeparator();
	adminMenu.addItem('5', "<- Back");
	adminMenu.addHandler('5', nullptr);
	adminMenu.run();
}

void settingsMenu()
{
	Menu settingsMenu("Edit House");

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
	settingsMenu.addSeparator();
	settingsMenu.addItem('9', "<- Back");
	settingsMenu.addHandler('9', nullptr);

	settingsMenu.run();
}

void mainMenu()
{
	Menu mainMenu("Sistema de Aquecimento Central");

	mainMenu.setWidth(40);
	mainMenu.addItem('1', "View House");
	mainMenu.addHandler('1', adminMenu);
	mainMenu.addItem('2', "Edit House");
	mainMenu.addHandler('2', settingsMenu);
	mainMenu.addSeparator();
	mainMenu.addItem('3', "Load graph");
	mainMenu.addHandler('3', hSystem_load);
	mainMenu.addItem('4', "Save graph");
	mainMenu.addHandler('4', nullptr);
	mainMenu.addItem('5', "Reset");
	mainMenu.addHandler('5', hSystem_reset);
	mainMenu.addSeparator();
	mainMenu.addItem('6', "Exit");
	mainMenu.addHandler('6', nullptr);
	mainMenu.run();
}

int main()
{
	hSystem = new HSystem();
	mainMenu();
	return 0;
}