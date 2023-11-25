#include "FontBuilder.h"

FontBuilder::FontBuilder()
{

}

FontBuilder::FontBuilder(const FontBuilder& cpy)
{

}

FontBuilder::~FontBuilder()
{

}

int FontBuilder::ReadDXTKFont(string filename)
{
	ifstream fontFile = ifstream(filename, ios::in | ios::binary | ios::ate);

	if (fontFile)
	{
		
	}
	else {
		return 1;
	}

	return 0;
}