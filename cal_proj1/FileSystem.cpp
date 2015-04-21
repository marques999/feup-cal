#include <fstream>
#include <string>
#include <vector>
#include <iostream>

using namespace std;
/*
class File
{

private:

	FILE* fp;
	string filename;

public:

	File(const string& fn) : filename(fn)
	{
		fp = nullptr;
	}

	__forceinline void open(const char* mode)
	{
		fp = fopen(filename.c_str(), mode);
	}

	size_t size()
	{
		if (fp == nullptr)
		{
			return;
		}
		
		size_t sz;

		fseek(fp, 0L, 2);
		sz = ftell(fp);
		fseek(fp, 0L, 0);

		return sz;
	}

	void reset()
	{
		if (fp != nullptr)
		{
			fseek(fp, 0L, 0);
		}
	}

	template<typename T>
	__forceinline T read()
	{
		if (fp == nullptr)
		{
			return;
		}

		T readValue;

		if (fread((char*)&readValue, sizeof(T), 1, fp) != 0)
		{
			return readValue;
		}

		throw exception();
	}

	template<typename T>
	__forceinline void readArray(vector<T> &v)
	{
		if (fp == nullptr)
		{
			return;
		}

		size_t arraySize;

		fread((char*)&arraySize, sizeof(size_t), 1, fp);

		for (size_t i = 0; i < arraySize; i++)
		{
			v.push_back(read(fp));
		}
	}

	template<typename T>
	__forceinline void write(const T &v)
	{
		if (fp == nullptr)
		{
			return;
		}

		fwrite((char*)v, sizeof(T), fp);
	}

	template<typename T>
	__forceinline void writeArray(const vector<T> &v)
	{
		if (fp == nullptr)
		{
			return;
		}

		size_t vectorSize = v.size();

		fwrite((char*)&vectorSize, sizeof(vectorSize), fp);

		for (auto &b : v)
		{
			write<T>(fp, v);
		}
	}
};
*/