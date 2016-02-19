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
#include <boost/filesystem.hpp>
#include <sigc++/sigc++.h>

#include "app_params.hpp"
#include "global_exit.hpp"
#include "console/console.hpp"
#include "library_tags.hpp"
#include "player.hpp"

using namespace std;
using namespace app_params;
using namespace global_exit;
using namespace player;
using namespace console;
using namespace rapidxml;
using namespace boost::filesystem;
using namespace sigc;

namespace library {
	class Library
	{
		public:
			enum class Navigation
			{
				PLAY_PAUSE, NEXT, BACK, RESET
			};

			Library(path, Console**);
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

			const string LIBRARY_FILE_PATH =	"library";
			const string LIBRARY_FILE_NAME =	"library.xml";

			path _libraryPath;
			path _libraryFile;
			file<>* _pLibraryXmlFile;
			xml_document<> _libraryDoc;

			map<string, xml_node<>*> _rfidMap;

			xml_node<>* _pCurrentEpisodeFiles;
			xml_node<>* _pCurrentFile;

			void parseLibraryFile(void);
			void saveLibraryFile(void);
			void parseSeries(xml_node<>*);
			void parseSeriesNode(xml_node<>*);
			void parseEpisodes(xml_node<>*);
			void parseEpisodeNode(xml_node<>*);

			void setFile(int);
			void playFile(void);

			void playPause(void);
			void stopFile(void);
			void nextFile(void);
			void previousFile(void);
			void resetFiles(void);

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
