/*!
 * \file UI.h
 *
 * FEUP_CAL1415_2MIEIC02_D
 * \author Diogo Marques
 * \author Jose Taveira
 * \author Vitor Esteves
 *
 * \date Abril 2015
 *
 */

#ifndef __UI_H_
#define __UI_H_

#include "Common.h"

namespace UI
{
	/**
	* @brief clears console sceren
	*/
	void ClearConsole();

	/*
	* @brief prints "press any key" message and waits for user input
	*/
	void PauseConsole();

	template <typename T>
	T readValue(const string& prompt);

	string readString(const string& prompt);

	/**
	* @brief displays a frame (or header)
	* @param s string to be displayed
	*/
	void DisplayFrame(const char* s);

	/**
	* @brief displays text centered on screen
	* @param s string to be displayed
	*/
	void Display(const char* s);

	void DisplayBox(const vector<string> &v);

	/**
	* @brief displays a brief error or informative message
	* @param s string to be displayed
	*/
	void DisplayMessage(const string &s);

	/**
	* @brief formats a fixed digit number
	* @param n value
	* @param w number of digits (field width)
	* @return returns a string containing the formatted value
	*/
	string Format(unsigned n, unsigned w);
}

#endif /* __UI_H_ */