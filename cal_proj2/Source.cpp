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

#include "Common.h"
#include "GPS.h"

int main()
{
	GPS gps = GPS::instance();

	gps.read();
	gps.enterText();

	return 0;
}