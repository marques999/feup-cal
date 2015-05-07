/*!
* \file GPS.h
*
* FEUP_CAL1415_2MIEIC01_D
* \author Diogo Marques
* \author Jose Taveira
* \author Vitor Esteves
*
* \date Maio 2015
*
*/

#ifndef __GPS_H_
#define __GPS_H_

#include "Common.h"

class GPS
{
public:

	static GPS& instance();
	string enterText();
	bool read();
	void selecionarDistrito();

private:

	GPS();

	vector<string> findMatch(const vector<string> &v, const string &s);
	
	vector<string> distritos;
	vector<string> concelhos;
	vector<string> ruas;
};

#endif /* __GPS_H_ */