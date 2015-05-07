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
#include "Rua.h"

class GPS
{
public:

	static GPS& instance();
	string GUISelect(const vector<string> &v, const char* prompt);
	unsigned GUISelectAux(const vector<string> &v, const string &s, const char* prompt);

	unsigned seleccionarConcelho(const string &s);
	unsigned seleccionarDistrito(const string &s);
	unsigned seleccionarRua(const string &s);

	void GUIConcelho();
	void GUIDistrito();
	void GUIRua();

	unsigned index(const vector<string> &v, const string &s);
	unsigned indexConcelho(const string &concelhoEscolhido);
	unsigned indexDistrito(const string &distritoEscolhido);

	bool readDistritos();
	bool readConcelhos(unsigned vectorIndex);
	bool readRuas(unsigned vectorIndex);

private:

	GPS();

	vector<string> findMatch(const vector<string> &v, const string &s);
	vector<string> distritos;
	vector<string> concelhos;
	vector<Rua> ruas;

	unsigned distrito;
	unsigned concelho;
	string rua;
};

#endif /* __GPS_H_ */