#include <chrono>
#include <iostream>
#include <string>
#include <functional>
#include <limits>
#include <vector>
#include <algorithm>

using namespace std;

uint64_t getCurrentTime()
{
	return (uint64_t)chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()).count();
}

unsigned getTimeDiff(uint64_t curTime, uint64_t prevTime)
{
	return unsigned(curTime - prevTime);
}

void clearConsole()
{
	system("cls");
}

void pauseConsole(const string& message)
{
	cout << message << endl;
	cin.get();
	cin.clear();

	if (cin.rdbuf()->in_avail() != 0)
	{
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

bool stringStartsWith(const string& str, const string& prefix)
{
	return !str.compare(0, prefix.size(), prefix);
}

__forceinline string &stringToLower(string &s)
{
	transform(s.begin(), s.end(), s.begin(), tolower);

	return s;
}

__forceinline string &stringToUpper(string &s)
{
	transform(s.begin(), s.end(), s.begin(), toupper);

	return s;
}

__forceinline string &stringTrimLeft(string &s)
{
	s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));

	return s;
}

__forceinline string &stringTrimRight(string &s)
{
	s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());

	return s;
}

__forceinline string &stringTrim(string &s)
{
	return stringTrimLeft(stringTrimRight(s));
}

template<typename T>
__forceinline void insertionSort(vector<T> &v)
{
	for (auto &i = v.start(); i != v.end(); ++i)
	{
		rotate(upper_bound(start, i, *i), i, next(i));
	}
}