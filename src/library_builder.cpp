#include "library_builder.hpp"
#include "library.hpp"

using namespace std;
using namespace app_params;
using namespace console;
using namespace global_exit;
using namespace boost::filesystem;

namespace library {
	/***************
	 * CONSTRUCTOR *
	 ***************/
	LibraryBuilder::LibraryBuilder(path applicationPath, Console** ppConsole=NULL)
	{
		_pLinkToConsole = ppConsole;
		(*_pLinkToConsole)->printLog("constructing LibraryBuilder");
		
		/*
		 * libsigc++ slots
		navigateSlot = mem_fun(this, &Library::navigate);
		episodeSlot = mem_fun(this, &Library::setEpisode);
		nextFileSlot = mem_fun(this, &Library::nextFile);
		 */
		
		/*
		 * libsigc++ signals
		completedSignal.connect(pLibrary->nextFileSlot);
		_pPlayer->completedSignal.connect(nextFileSlot);
		 */
		
		/*
		 * libsigc++ signals
		playSignal.connect(_pPlayer->playFileSlot);
		playPauseSignal.connect(_pPlayer->playPauseSlot);
		 */
		
		_libraryPath = applicationPath;
		_libraryPath /= LIBRARY_FILE_PATH;
		if(!exists(_libraryPath) || !is_directory(_libraryPath))
			GlobalExit::exit(ErrorCode::EXIT_ERROR_DIRECTORY_NOT_FOUND, "_libraryPath");
		tracePath("_libraryPath", _libraryPath);
		
		_libraryFile = _libraryPath;
		_libraryFile /= LIBRARY_FILE_NAME;
		if(!exists(_libraryFile) || !is_regular_file(_libraryFile))
			GlobalExit::exit(ErrorCode::EXIT_ERROR_FILE_NOT_FOUND, "_libraryFile");
		tracePath("_libraryFile", _libraryFile);
	}
	
	LibraryBuilder::~LibraryBuilder(void)
	{
		(*_pLinkToConsole)->printLog("destructing LibraryBuilder");
	}
	
	/*****************
	 * BUILD LIBRARY *
	 *****************/
	// Library
	void LibraryBuilder::buildLibraryFile(Operations operation)
	{
		_logStream << "build " << _libraryFile.filename();
		(*_pLinkToConsole)->printLog(&_logStream);
		
		backupFile();
		
		if(operation == Operations::APPEND)
		{
			// tricky
			_libraryFileStream.open(_libraryFile.string(), ofstream::out | ofstream::app);
			
			long pos = _libraryFileStream.tellp();
			
			_logStream << "pos " << pos;
			(*_pLinkToConsole)->printLog(&_logStream);
			
			_libraryFileStream.seekp (pos-11);
			_libraryFileStream << "xxx";
			
		}
		else if(operation == Operations::FULL)
		{
			_libraryFileStream.open(_libraryFile.string(), ofstream::out | ofstream::trunc);
			
			// write default xml-definition
			_libraryFileStream << LibraryTags::TAG_DEFINITION << endl;
			
			// write library-tag
			writeOpenTag(LibraryTags::TAG_LIBRARY, 0);
			writeSeries();
			writeCloseTag(LibraryTags::TAG_LIBRARY, 0);
		}
		
		_logStream << "build " << _libraryFile.filename() << " done";
		(*_pLinkToConsole)->printLog(&_logStream);
	}
	
	void LibraryBuilder::writeSeries(bool checkExistance)
	{
		vector<path> series = getSubDirectories(_libraryPath);
		sort(series.begin(), series.end());
		for(vector<path>::iterator itSeries = series.begin(); itSeries!=series.end(); ++itSeries)
		{
			// write series-tag
			writeOpenTag(LibraryTags::TAG_SERIES, 1);
			path pathSeries = *itSeries;
			
			/*
			_logStream << "+--add series      : +--" << pathSeries.filename();
			(*_pLinkToConsole)->printLog(&_logStream);
			 */
			
			// write series title
			writeOpenTag(LibraryTags::TAG_TITLE, 2, false);
			_libraryFileStream << pathSeries.filename().string();
			writeCloseTag(LibraryTags::TAG_TITLE);
			
			// write series cover
			writeOpenTag(LibraryTags::TAG_COVER, 2, false);
			writeCloseTag(LibraryTags::TAG_COVER);
			
			vector<path> episodes = getSubDirectories(pathSeries);
			sort(episodes.begin(), episodes.end());
			for(vector<path>::iterator itEpisodes = episodes.begin(); itEpisodes!=episodes.end(); ++itEpisodes)
			{
				// write episode-tag
				writeOpenTag(LibraryTags::TAG_EPISODE, 2);
				path pathEpisode = *itEpisodes;
				
				/*
				_logStream << "|  +--add episode  : |  +--" << pathEpisode.filename();
				(*_pLinkToConsole)->printLog(&_logStream);
				 */
				
				// write episode title
				writeOpenTag(LibraryTags::TAG_TITLE, 3, false);
				_libraryFileStream << pathEpisode.filename().string();
				writeCloseTag(LibraryTags::TAG_TITLE);
				
				// write episode cover
				writeOpenTag(LibraryTags::TAG_COVER, 3, false);
				writeCloseTag(LibraryTags::TAG_COVER);
				
				// write files-tag
				writeOpenTag(LibraryTags::TAG_FILES, 3);
				//rfid="" current_file="1" timestamp="0"
				vector<path> files = getFiles(pathEpisode, FileExtensions::MP3);
				sort(files.begin(), files.end());
				for(vector<path>::iterator itFiles = files.begin(); itFiles!=files.end(); ++itFiles)
				{
					// write file-tag
					writeOpenTag(LibraryTags::TAG_FILE, 4, false);
					path pathFile = *itFiles;
					
					/*
					_logStream << "|  |  +--add file  : |  |  +--" << pathFile.filename();
					(*_pLinkToConsole)->printLog(&_logStream);
					 */
					
					_libraryFileStream << relativeTo(_libraryPath, pathFile).string();
					writeCloseTag(LibraryTags::TAG_FILE);
				}
				writeCloseTag(LibraryTags::TAG_FILES, 3);
				
				writeCloseTag(LibraryTags::TAG_EPISODE, 2);
			}
			
			writeCloseTag(LibraryTags::TAG_SERIES, 1);
		}
	}
	
	path LibraryBuilder::backupFile(void)
	{
		_logStream << "backup " << _libraryFile.filename();
		(*_pLinkToConsole)->printLog(&_logStream);
		
		int backupCounter = 0;
		path backupFile;
		
		do{
			createBackupFileName(backupFile, backupCounter);
			++backupCounter;
		}while(exists(backupFile));
		
		copy(_libraryFile, backupFile);
		return backupFile;
	}
	
	void LibraryBuilder::createBackupFileName(path& target, int counter)
	{
		target = _libraryFile;
		target+=DOT;
		target+=to_string(counter);
		target+=BACKUP_FILE_EXTENSION;
	}
	
	void LibraryBuilder::writeOpenTag(string tag, int tabs, bool lineBreak)
	{
		while(tabs>0){
			_libraryFileStream << "\t";
			--tabs;
		}
		_libraryFileStream << LibraryTags::TAG_BRACE_OPEN << tag << LibraryTags::TAG_BRACE_CLOSE;
		if(lineBreak)
			_libraryFileStream << endl;
	}
	
	void LibraryBuilder::writeCloseTag(string tag, int tabs)
	{
		while(tabs>0){
			_libraryFileStream << "\t";
			--tabs;
		}
		_libraryFileStream << LibraryTags::TAG_BRACE_OPEN << LibraryTags::TAG_CLOSING << tag << LibraryTags::TAG_BRACE_CLOSE << endl;
	}
	
	
	
	
	
	
	void LibraryBuilder::tracePath(string name, path p)
	{
		_outStream << name << ": " << p << endl;
		//_outStream << name << ": root_name      : " << p.root_name() << endl;
		//_outStream << name << ": root_directory : " << p.root_directory() << endl;
		//_outStream << name << ": root_path      : " << p.root_path() << endl;
		_outStream << name << ": relative_path  : " << p.relative_path() << endl;
		_outStream << name << ": stem           : " << p.stem() << endl;
		
		_outStream << name << ": parent_path    : " << p.parent_path() << endl;
		_outStream << name << ": filename       : " << p.filename() << endl;
		_outStream << name << ": extension      : " << extension(p) << endl;
		
		_outStream << name << ": is_absolute    : " << p.is_absolute() << endl;
		_outStream << name << ": is_relative    : " << p.is_relative() << endl;
		
		_outStream << name << ": exists         : " << exists(p) << endl;
		_outStream << name << ": is_directory   : " << is_directory(p) << endl;
		_outStream << name << ": is_regular_file: " << is_regular_file(p) << endl;
		_outStream << name << ": canonical      : " << canonical(p) << endl;
		_outStream << name << ": absolute       : " << absolute(p) << endl;
		(*_pLinkToConsole)->printOut(&_outStream);
	}
	
	path LibraryBuilder::relativeTo(path from, path to)
	{
		path::const_iterator fromIter = from.begin();
		path::const_iterator toIter = to.begin();
		
		while (fromIter != from.end() && toIter != to.end() && (*toIter) == (*fromIter))
		{
			++toIter;
			++fromIter;
		}
		
		path finalPath;
		while (fromIter != from.end())
		{
			finalPath /= "..";
			++fromIter;
		}
		
		while (toIter != to.end())
		{
			finalPath /= *toIter;
			++toIter;
		}
		
		return finalPath;
	}
	
	path LibraryBuilder::getPath(string dir)
	{
		path p(dir);
		try
		{
			if(exists(p))
			{
				return p;
			}
		}
		catch (const filesystem_error& ex)
		{
			GlobalExit::exit(ErrorCode::EXIT_ERROR_FILESYSTEM, ex.what());
		}
		
		GlobalExit::exit(ErrorCode::EXIT_ERROR_DIRECTORY_NOT_FOUND, dir);
		return NULL;
	}
	
	vector<path> LibraryBuilder::getSubDirectories(path p)
	{
		return getFiles(p, CURRENT_PATH);
	}
	
	vector<path> LibraryBuilder::getFiles(path p, FileExtensions fileExtension)
	{
		vector<path> ret;
		try
		{
			for(directory_iterator di(p); di!=directory_iterator(); di++){
				if(conditional_check(di->path(), fileExtension))
				{
					ret.push_back(di->path());
				}
			}
			/*
			if(exists(p))
			{
				if(is_directory(p))
				{
				}
				else
				{
					cout << p << " is not a directory" << endl;
				}
			}
			else
			{
				cout << p << " does not exist" << endl;
			}
			 */
		}
		catch (const filesystem_error& ex)
		{
			GlobalExit::exit(ErrorCode::EXIT_ERROR_FILESYSTEM, ex.what());
		}
		return ret;
	}
	
	path LibraryBuilder::getFile(path p, FileNames fileNames)
	{
		try
		{
			if(exists(p))
			{
				if(is_directory(p))
				{
					for(directory_iterator di(p); di!=directory_iterator(); di++){
						if(conditional_check(di->path(), fileNames))
						{
							return di->path();
						}
					}
				}
				else
				{
					cout << p << " is not a directory" << endl;
				}
			}
			else
			{
				cout << p << " does not exist" << endl;
			}
		}
		catch (const filesystem_error& ex)
		{
			cout << ex.what() << endl;
		}
		return NULL;
	}

	bool LibraryBuilder::conditional_check(path p, FileExtensions fileExtension)
	{
		switch(fileExtension)
		{
			case CURRENT_PATH:
				return is_directory(p);
				break;
			case MP3:
				return is_regular_file(p) && p.extension()==fileExtensionMap[MP3];
				break;
			case JPG:
				return is_regular_file(p) && p.extension()==fileExtensionMap[JPG];
				break;
		}
		return false;
	}

	bool LibraryBuilder::conditional_check(path p, FileNames fileName)
	{
		switch(fileName)
		{
			case LIBRARY:
				return is_regular_file(p) && p.filename()==fileNamesMap[LIBRARY];
				break;
			case COVER:
				return is_regular_file(p) && p.filename()==fileNamesMap[COVER];
				break;
			case DEFAULT_COVER:
				break;
		}
		return false;
	}
}
