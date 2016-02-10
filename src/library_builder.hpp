#ifndef LIBRARY_BUILDER_HPP_INCLUDED
#define LIBRARY_BUILDER_HPP_INCLUDED

#include <sstream>
#include <iostream>
#include <cstring>

#include <map>
#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sys/stat.h>

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <rapidxml_print.hpp>

#include <boost/filesystem.hpp>

#include "app_params.hpp"
#include "console/console.hpp"
#include "global_exit.hpp"
#include "library_tags.hpp"

using namespace std;
using namespace app_params;
using namespace console;
using namespace global_exit;
using namespace rapidxml;
using namespace boost::filesystem;

namespace library {
	enum FileExtensions
	{
		CURRENT_PATH, PREV_PATH, MP3, JPG
	};
	struct FileExtensionsMap : map<FileExtensions, string>
	{
		FileExtensionsMap()
		{
			this->operator[](CURRENT_PATH) =	".";
			this->operator[](PREV_PATH) =		"..";
			this->operator[](MP3) =				".mp3";
			this->operator[](JPG) =				".jpg";
		};
		~FileExtensionsMap(){};
	};
	static FileExtensionsMap fileExtensionMap;

	enum FileNames
	{
		LIBRARY, COVER, DEFAULT_COVER
	};
	struct FileNamesMap : map<FileNames, string>
	{
		FileNamesMap()
		{
			this->operator[](LIBRARY) =			"library.xml";
			this->operator[](COVER) =			"cover.jpg";
			this->operator[](DEFAULT_COVER) =	"no_cover.jpg";
		};
		~FileNamesMap(){};
	};
	static FileNamesMap fileNamesMap;

	class LibraryBuilder
	{
		public:
			LibraryBuilder(path, Console**);
			~LibraryBuilder(void);

			void buildLibraryFile(Operations);
		private:
			Console** _pLinkToConsole;
			ostringstream _outStream;
			ostringstream _logStream;

			const string LIBRARY_FILE_PATH =		"library";
			const string LIBRARY_FILE_NAME =		"library.xml";
			const string DOT =						".";
			const string BACKUP_FILE_EXTENSION =	".bak";

			vector<xml_node<char>*> _nodeStorage;
			vector<const char*> _valueStorage;

			path _libraryPath;
			path _libraryFile;
			ofstream _libraryFileStream;

			file<>* _pLibraryXmlFile;
			xml_document<> _libraryDoc;

			xml_node<>* getExistingNode(string, xml_node<>*, const char*);

			void parseSeriesFolders(xml_node<>*);
			void parseEpisodesFolders(xml_node<>*, path);
			void parseFilesFolders(xml_node<>*, path);

			xml_node<>* writeSeriesNode(string);
			xml_node<>* writeEpisodeNode(string);
			xml_node<>* writeFileNode(string);

			path backupFile(void);
			void createBackupFileName(path&, int);

			void writeOpenTag(string, int, bool=true, string="");
			void writeCloseTag(string, int=0);

			void tracePath(string, path);
			path relativeTo(path, path);
			path getPath(string);
			vector<path> getSubDirectories(path);
			vector<path> getFiles(path, FileExtensions=CURRENT_PATH);
			//path getFile(path, FileNames);
			bool conditional_check(path, FileExtensions);
			//bool conditional_check(path, FileNames);
	};
}
#endif
