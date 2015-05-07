/*!
 * \file Rua.h
 *
 * FEUP_CAL1415_2MIEIC01_D
 * \author Diogo Marques
 * \author Jose Taveira
 * \author Vitor Esteves
 *
 * \date Maio 2015
 *
 */

#ifndef __RUA_H_
#define __RUA_H_

#include "Common.h"

struct Rua
{
	string nome;
	string localidade;
	string codPostal;

	void read(ifstream &in)
	{

	}
};

#endif __RUA_H_