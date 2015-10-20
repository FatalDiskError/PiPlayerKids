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
			Library(string, Console**);
			~Library(void);
			void setEpisode(string);
			void nextFile();
			void previousFile();
			
			void logOutRfidMap(void);
			
		private:
			Console** _pLinkToConsole;
			ostringstream _outStream;
			ostringstream _logStream;
			
			const string DIRECTORY_DIVIDER = "/";
			const string LIBRARY_FILE_NAME = "/library/library.xml";
			string _libraryPath;
			map<string, xml_node<>*> rfidMap;
			
			xml_document<> _doc;
			xml_node<>* _pCurrentEpisodeFiles;
			xml_node<>* _pCurrentFile;
			
			signal<void (string, int)> playSignal;
			
			void parseLibraryFile(void);
			void parseSeries(xml_node<>*);
			void parseSeriesNode(xml_node<>*);
			void parseEpisodes(xml_node<>*);
			void parseEpisodeNode(xml_node<>*);
			
			void setFile();
			
			int getChildCount(xmlnode<>*);
			xmlnode<>* getChildAt(xmlnode<>*, int);
			int getCurrentFileIndex(void);
			void setCurrentFileIndex(int);
			int getCurrentTimestamp(void);
			void setCurrentTimestamp(int);
	};
}

#endif
