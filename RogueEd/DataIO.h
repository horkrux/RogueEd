#ifndef _DATAIO_H_
#define _DATAIO_H_

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

#include <unordered_map>
#include <fstream>
#include <iostream>
using namespace std;

typedef struct
{
	unsigned int offset;// = 0;
	unsigned int size;// = 0;
	int unknown;// = -1;
	unsigned short flags;// = 0;
	unsigned short folder_length;// = 0;
} FileInfo;


class DataIO
{
public:
	static DataIO* getInstance();

	void shutdown();

	int read(string);
	int write();
	int getFileContents(const string, vector<char>*);
	unordered_map<string, FileInfo> getHashMap();

private:

	DataIO();
	DataIO(const DataIO&);
	~DataIO();

	static DataIO* single;

	unordered_map<string, FileInfo> m_hashMap;
	ifstream m_header;
	ifstream m_archive;

	vector<char> m_archive_contents;
};

#endif