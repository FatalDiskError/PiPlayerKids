#ifndef LIBRARY_HPP_INCLUDED
#define LIBRARY_HPP_INCLUDED

#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
//#include <boost/signals2.hpp>
//#include <sigc++/sigc++.h>
#include <Signal.h>

#include "library_tags.hpp"
#include "player.hpp"
#include "console.hpp"

using namespace std;
using namespace player;
using namespace console;
using namespace rapidxml;
//using namespace boost::signals2;
//using namespace sigc;
using namespace Gallant;

namespace library {
	class Library
	{
		public:
			enum class Navigation
			{
				NEXT, PREVIOUS, RESET
			};
			
			Library(string, Player**, Console**);
			~Library(void);
			void setEpisode(string);
			void navigate(Navigation);
			
			void logOutRfidMap(void);
			//signal<void (string, int)> playSignal;
			//signal<void, string, int> playSignal;
			//slot<void, string> setEpisodeSlot;
			//slot<void, Navigation> navigateSlot;
			Signal2<string, int> playSignal;
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
