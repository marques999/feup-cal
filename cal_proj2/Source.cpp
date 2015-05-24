/*!
 * \file Source.cpp
 *
 * FEUP_CAL1415_2MIEIC01_D
 * \author Diogo Marques
 * \author Jose Taveira
 * \author Vitor Esteves
 *
 * \date Maio 2015
 *
 */

#include "GPS.h"

int main()
{
	GPS gps = GPS::instance();

	try
	{
		gps.GUIMenu();
	}
	catch (CustomException &e)
	{
		UI::DisplayMessage(e.str());
	}
	catch (MessageException &e)
	{
		UI::DisplayMessage(e.str());
	}

	return 0;
}