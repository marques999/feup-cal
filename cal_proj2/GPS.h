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

#include "Algorithms.h"
#include "Rua.h"
#include "UI.h"

class GPS
{
public:

	static GPS& instance();

	void GUIMenu();

private:

	GPS();

	void GUIConcelho();
	void GUIDistrito();
	void GUIRua();
	void GUITable(const vector<Rua> &v, unsigned index);
	void GUIInfo() const;
	bool GUISelectRua(string &userInput);
	bool GUISelect(const vector<string> &v, string &userInput, const char* prompt);
	void GUINavigate(unsigned &vectorIndex, unsigned vectorSize) const;
	void GUISwitchPage(unsigned &vectorIndex, unsigned vectorSize) const;
	void GUIInsertChar(string &userInput, char c, unsigned &vectorIndex) const;
	void GUIRemoveChar(string &userInput, unsigned &vectorIndex) const;
	
	template<class T>
	unsigned GUISelectAux(const vector<T> &v, const T &s, const char* prompt);

	vector<string> findMatch(const vector<string> &v, const string &s);
	vector<Rua> findMatch(const vector<Rua> &v, const string &s);
	vector<string> distritos;
	vector<string> concelhos;
	vector<Rua> ruas;

	bool read(const char* filename, vector<string> &v);
	bool readDistritos();
	bool readConcelhos(unsigned vectorIndex);
	bool readRuas(unsigned vectorIndex);

	const char* strSelecionarConcelho = "SELECIONAR CONCELHO";
	const char* strSelecionarDistrito = "SELECIONAR DISTRITO";
	const char* strSelecionarRua = "SELECIONAR RUA";
	const char* strSearchFormat = "\n                Pesquisar: %s\xdb\n\n";
	const char* strNavigationBar = "<ENTER> validar selec\x87\xc6o    <ESC> voltar";

	template<class T>
	unsigned index(const vector<T> &v, const T &s);

	unsigned indexConcelho(const string &concelhoEscolhido);
	unsigned indexDistrito(const string &distritoEscolhido);
	unsigned indexRua(const string &ruaEscolhida);
	unsigned seleccionarConcelho(const string &s);
	unsigned seleccionarDistrito(const string &s);
	unsigned seleccionarRua(const string &s);
	unsigned distrito;
	unsigned concelho;
	unsigned rua;
};

#endif /* __GPS_H_ */