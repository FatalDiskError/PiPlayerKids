#ifndef LIBRARY_HPP_INCLUDED
#define LIBRARY_HPP_INCLUDED

#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <cstring>

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <rapidxml_print.hpp>
#include <sigc++/sigc++.h>

#include "library_tags.hpp"
#include "player.hpp"
#include "console/console.hpp"

using namespace std;
using namespace player;
using namespace console;
using namespace rapidxml;
using namespace sigc;

namespace library {
	class Library
	{
		public:
			enum class Navigation
			{
				PLAY_PAUSE, NEXT, BACK, RESET
			};
			
			Library(string, Console**);
			~Library(void);
			
			void logOutRfidMap(void);
			
			/*
			 * libsigc++ slots
			 */
			slot<void, Navigation> navigateSlot;
			slot<void, string> episodeSlot;
			slot<void> nextFileSlot;
			
		private:
			Player* _pPlayer;
			Console** _pLinkToConsole;
			ostringstream _outStream;
			ostringstream _logStream;
			
			const string DIRECTORY_DIVIDER = "/";
			const string LIBRARY_FILE_NAME = "/library/library.xml";
			string _libraryPath;
			map<string, xml_node<>*> _rfidMap;
			
			file<>* _pXmlFile;
			xml_document<> _doc;
			xml_node<>* _pCurrentEpisodeFiles;
			xml_node<>* _pCurrentFile;
			
			/*
			 * libsigc++ signals
			signal<void, string, int> playSignal;
			signal<int> playPauseSignal;
			 */
			
			
			void parseLibraryFile(void);
			void parseSeries(xml_node<>*);
			void parseSeriesNode(xml_node<>*);
			void parseEpisodes(xml_node<>*);
			void parseEpisodeNode(xml_node<>*);
			
			void setFile(int);
			void playFile(void);
			
			void playPause();
			void nextFile();
			void previousFile();
			void resetFiles();
			
			void setEpisode(string);
			void navigate(Navigation);
			
			int getChildCount(xml_node<>*);
			xml_node<>* getChildAt(xml_node<>*, int);
			int getCurrentFileIndex(void);
			void setCurrentFileIndex(int);
			double getCurrentTimestamp(void);
			void setCurrentTimestamp(double);
	};
}
#endif
