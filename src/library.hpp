#ifndef LIBRARY_HPP_INCLUDED
#define LIBRARY_HPP_INCLUDED

#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

#include "library_tags.hpp"
#include "console.hpp"

using namespace std;
using namespace rapidxml;
using namespace console;

namespace library {
	class Library
	{
		public:
			typedef struct
			{
				string name;
				int timestamp;
			} File;
			typedef struct
			{
				string name;
				string rfidSerialNumber;
				vector<string> files;
			} Episode;
			typedef struct
			{
				string name;
				vector<Episode> episodes;
			} Series;
			
			Library(char*, Console**);
			~Library(void);
			File* getFile(string);
			File* getNextFile();
			void writeCurrentFile(string, string);
			void writeCurrentTime(string, string);
			
		private:
			static constexpr const char* LIBRARY_FILE_NAME = "library/library.xml";
			char* _pLibraryPath;
			//const string LIBRARY_FILE_NAME = "./library/library.xml";

			Console** _pLinkToConsole;
			ostringstream outStream;
			ostringstream logStream;
			
			File* _pCurrentFile;
			Episode* _pCurrentEpisode;
			vector<Series*> _pSeries;
			
			map<string, xml_node<>*> rfidMap;

			void parseLibraryFile(void);
			//void parseNode(xml_node<>*, string);
			void parseSeries(xml_node<>*);
			void parseSeriesNode(xml_node<>*);
			void parseEpisodes(xml_node<>*);
			void parseEpisodeNode(xml_node<>*);
			void parseFiles(xml_node<>*);
			void parseFileNode(xml_node<>*);
	};
}

#endif
