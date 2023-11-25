#include "DataIO.h"

DataIO::DataIO() {}
DataIO::DataIO(const DataIO& cpy) {}

DataIO::~DataIO() 
{
	vector<char>().swap(m_archive_contents);
}

DataIO* DataIO::single = NULL;

DataIO* DataIO::getInstance()
{
	if (single == NULL) {
		single = new DataIO();
	}
	return single;
}

int DataIO::read(string filename)
{
	m_header = ifstream(filename, ios::in | ios::binary | ios::ate);
	if (m_header)
	{
		size_t len_header = m_header.tellg();
		vector<char> contents_header(len_header);
		m_header.seekg(0, ios::beg);
		m_header.read(&contents_header[0], len_header);
		m_header.close();

		int num_folders = len_header / 32;

		vector<string> header_names(num_folders);

		for (int i = 0; i < num_folders; i++)
		{
			header_names[i] = contents_header.data() + i * 32;
		}

		filename.replace(filename.size() - 3, 3, "DAT");
		m_archive = ifstream(filename, ios::in | ios::binary | ios::ate);

		if (m_archive)
		{
			size_t len_archive = m_archive.tellg();
			m_archive_contents = vector<char>(len_archive);
			m_archive.seekg(0, ios::beg);
			m_archive.read(&m_archive_contents[0], len_archive);
			m_archive.close();

			char* contents_data = m_archive_contents.data();

			int offset_info = *(int*)contents_data;
			int info_size = *(int*)(contents_data + 4);
			int curr_offset = 0;
			vector<int> folder_length;
			string folder;
			vector<string> folder_structure;
			string file_name = header_names[0] + "/";

			//string file_name = header_names[0] + "/";

			while (curr_offset < info_size)
			{
				FileInfo fileInfo = *(FileInfo*)(contents_data + offset_info + curr_offset);

				if (fileInfo.flags & 0x80 && fileInfo.folder_length)
				{
					folder_structure.push_back(contents_data + offset_info + curr_offset + 16);
					folder_length.push_back(fileInfo.folder_length);

					for (int i = 0; i < folder_structure.size(); i++)
					{
						folder_length[i] -= 32;
					}

					curr_offset += 32;
					continue;
				}

				if (folder_structure.size() > 0)
				{
					for (int i = 0; i < folder_structure.size(); i++)
					{
						file_name += folder_structure[i];
						file_name += "/";

						folder_length[i] -= 32;
					}

					for (int i = folder_structure.size() - 1; i > -1; i--) 
					{
						if (!folder_length[i])
						{
							folder_length.pop_back();
							folder_structure.pop_back();
						}
					}
				}

				file_name += (contents_data + offset_info + curr_offset + 16);
				m_hashMap[file_name] = fileInfo;

				//file_name = header_names[0] + "/";

				curr_offset += 32;
				file_name = header_names[0] + "/";
			}

			OutputDebugStringA("hoho");

		}
	}
	



	return 1;
}

int DataIO::write() { return 0; }

int DataIO::getFileContents(const string name, vector<char>* contents)
{
	if (!m_archive_contents.size())
		return 1;

	try {
		FileInfo info = m_hashMap.at(name);

		vector<char>::iterator begin = m_archive_contents.begin() + info.offset;
		vector<char>::iterator end = m_archive_contents.begin() + info.offset + info.size;
		*contents = vector<char>(begin, end);

		return 0;
	}
	catch (out_of_range)
	{
		return 1;
	}

	return 1;
}

unordered_map<string, FileInfo> DataIO::getHashMap()
{
	return m_hashMap;
}

void DataIO::shutdown()
{
	if (single)
		delete single;
}