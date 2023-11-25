#pragma once

#include <string>
#include <fstream>
using namespace std;

class FontBuilder
{
public:
	FontBuilder();
	FontBuilder(const FontBuilder&);
	~FontBuilder();

private:
	int ReadDXTKFont(string);
};