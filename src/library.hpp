#ifndef LIBRARY_HPP_INCLUDED
#define LIBRARY_HPP_INCLUDED

#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <Signal.h> // https://github.com/pbhogan/Signals

#include "library_tags.hpp"
#include "console.hpp"

using namespace std;
using namespace console;
using namespace rapidxml;

namespace library {
	class Library
	{
		public:
			enum class Navigation
			{
				NEXT, PREVIOUS, RESET
			};
			
			Library(string, Console**);
			~Library(void);
			void setEpisode(string);
			void navigate(Navigation);
			
			void logOutRfidMap(void);
			Gallant::Signal2<string, int> playSignal;
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
			
			
			void parseLibraryFile(void);
			void parseSeries(xml_node<>*);
			void parseSeriesNode(xml_node<>*);
			void parseEpisodes(xml_node<>*);
			void parseEpisodeNode(xml_node<>*);
			
			void setFile();
			void nextFile();
			void previousFile();
			void resetFiles();
			
			int getChildCount(xml_node<>*);
			xml_node<>* getChildAt(xml_node<>*, int);
			int getCurrentFileIndex(void);
			void setCurrentFileIndex(int);
			int getCurrentTimestamp(void);
			void setCurrentTimestamp(int);
	};
}
#endif
