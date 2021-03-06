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

	/**
	* @brief displays a frame (or header)
	* @param s string to be displayed
	*/
	void DisplayFrame(const string &s);

	/**
	* @brief displays a frame (or header), doesn't clear console screen
	* @param s string to be displayed
	*/
	void DisplayFrameNoCls(const string &s);

	/**
	* @brief displays text centered on screen
	* @param s string to be displayed
	*/
	void Display(const string &s);

	/**
	* @brief displays a brief error or informative message
	* @param s string to be displayed
	*/
	void DisplayMessage(const string &s);

	/**
	* @brief displays table header
	* @param c number of columns
	* @param labels array containing the labels
	* @param length array containing the width of each column (should be the same size as c)
	*/
	void DisplayTable(int c, const vector<string> &labels, const int length[]);

	/**
	* @brief displays a table row
	* @param c number of columns (same as DisplayTable())
	* @param labels array containing the row values
	* @param length array containing the width of each cell (should be the same size as c)
	*/
	void DisplayTableRow(int c, const vector<string> &labels, const int length[]);

	/**
	* @brief formats a fixed digit number
	* @param n value
	* @param w number of digits (field width)
	* @return returns a string containing the formatted value
	*/
	string Format(unsigned n, unsigned w);

	/**
	* @brief formats a floating point number
	* @param n value
	* @param w number of precision digits (field width)
	* @return returns a string containing the formatted value
	*/
	string FormatWeight(double n);

	/**
	* @brief formats a floating point number as a temperature value
	* @param n value
	* @param w number of digits (field width)
	* @return returns a string containing the formatted value
	*/
	string FormatTemperature(double n);
}

#endif /* __UI_H_ */