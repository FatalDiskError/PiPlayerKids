#include "library.hpp"

using namespace std;
using namespace app_params;
using namespace global_exit;
using namespace player;
using namespace console;
using namespace rapidxml;
using namespace boost::filesystem;
using namespace sigc;

namespace library {
	/***************
	 * CONSTRUCTOR *
	 ***************/
	Library::Library(path applicationPath, Console** ppConsole=NULL)
	{
		_pLinkToConsole = ppConsole;
		(*_pLinkToConsole)->printLog("constructing library");

		_pPlayer = new Player(ppConsole);

		/*
		 * libsigc++ slots
		 */
		navigateSlot = mem_fun(this, &Library::navigate);
		episodeSlot = mem_fun(this, &Library::setEpisode);
		nextFileSlot = mem_fun(this, &Library::nextFile);

		/*
		 * libsigc++ signals
		 */
		_pPlayer->completedSignal.connect(nextFileSlot);

		_libraryPath = applicationPath;
		_libraryPath /= LIBRARY_FILE_PATH;
		if(!exists(_libraryPath) || !is_directory(_libraryPath))
		{
			GlobalExit::exit(ErrorCode::EXIT_ERROR_DIRECTORY_NOT_FOUND, "_libraryPath");
		}

		_libraryFile = _libraryPath;
		_libraryFile /= LIBRARY_FILE_NAME;
		if(!exists(_libraryFile) || !is_regular_file(_libraryFile))
		{
			GlobalExit::exit(ErrorCode::EXIT_ERROR_FILE_NOT_FOUND, "_libraryFile");
		}

		parseLibraryFile();
	}
	Library::~Library(void)
	{
		stopFile();
		saveLibraryFile();
		(*_pLinkToConsole)->printLog("destructing library");
	}

	/*****************
	 * PARSE LIBRARY *
	 *****************/
	// Library
	void Library::parseLibraryFile(void)
	{
		_logStream << "opening " << _libraryPath;
		(*_pLinkToConsole)->printLog(&_logStream);

		// open library.xml-file
		_pLibraryXmlFile = new file<>(_libraryFile.c_str());
		_libraryDoc.parse<parse_full>(_pLibraryXmlFile->data());
		// get <library>-node
		xml_node<>* pLibrary = _libraryDoc.first_node(LibraryTags::TAG_LIBRARY);

		#ifdef XML_OUT_OUTPUT
			_outStream << "+-+ " << pLibrary->name();
			_outStream << " [numSeries: " << getChildCount(pLibrary) << "]";
			(*_pLinkToConsole)->printOut(&_outStream);
		#endif
		// parse <library>-node
		parseSeries(pLibrary);
	}
	void Library::saveLibraryFile(void)
	{
		_logStream << "saving " << _libraryFile.filename();
		(*_pLinkToConsole)->printLog(&_logStream);

		string data;
		print(back_inserter(data), _libraryDoc);
		ofstream newLibraryFile;
		newLibraryFile.open(_libraryFile.c_str());
		newLibraryFile << data;
		newLibraryFile.close();
	}

	// Series
	void Library::parseSeries(xml_node<>* pLibrary)
	{
		// iterate <series>-nodes within <library>-node
		for(xml_node<>* pSeriesNode = pLibrary->first_node(LibraryTags::TAG_SERIES); pSeriesNode; pSeriesNode = pSeriesNode->next_sibling())
		{
			#ifdef XML_OUT_OUTPUT
				_outStream << "| +-+ " << pSeriesNode->name();
			#endif
			// parse <series>-node
			parseSeriesNode(pSeriesNode);
		}
	}
	void Library::parseSeriesNode(xml_node<>* pSeries)
	{
		// get <title>-node of series
		xml_node<>* pTitle = pSeries->first_node(LibraryTags::TAG_TITLE);

		#ifdef XML_OUT_OUTPUT
			_outStream << " [";
			_outStream << pTitle->name() << ": " << pTitle->value() << ", ";
			_outStream << "numEpisodes: " << (getChildCount(pSeries)-2);
			_outStream << "]";
			(*_pLinkToConsole)->printOut(&_outStream);
		#endif
		// parse episodes
		parseEpisodes(pSeries);
	}

	// Episodes
	void Library::parseEpisodes(xml_node<>* pSeries)
	{
		// iterate <episode>-nodes within <series>-node
		for(xml_node<>* pEpisode = pSeries->first_node(LibraryTags::TAG_EPISODE); pEpisode; pEpisode = pEpisode->next_sibling())
		{
			#ifdef XML_OUT_OUTPUT
				_outStream << "| | +-+ " << pEpisode->name();
			#endif
			// parse <episode>-node
			parseEpisodeNode(pEpisode);
		}
	}
	void Library::parseEpisodeNode(xml_node<>* pEpisode)
	{
		#ifdef XML_OUT_OUTPUT
			xml_node<>* pTitle = pEpisode->first_node(LibraryTags::TAG_TITLE);

			_outStream << " [" << pTitle->name() << ": " << pTitle->value() << "]";
			(*_pLinkToConsole)->printOut(&_outStream);
		#endif
		// get <files>-node of episode
		xml_node<>* pFiles = pEpisode->first_node(LibraryTags::TAG_FILES);

		#ifdef XML_OUT_OUTPUT
			_outStream << "| | | +-- " << pFiles->name() << " [";
			_outStream << "numFiles: " << getChildCount(pFiles);

			for (xml_attribute<>* pAttribute = pFiles->first_attribute(); pAttribute; pAttribute = pAttribute->next_attribute())
			{
				_outStream << " - " << pAttribute->name() << ": " << pAttribute->value();
			}

			_outStream << "]";
			(*_pLinkToConsole)->printOut(&_outStream);
		#endif
		// get [rfid]-attribute from <files>-node
		xml_attribute<>* pRfid = pFiles->first_attribute(LibraryTags::ATTRIBUTE_RFID);

		// store <files>-node by rfid-serial-number
		string rfidSerialNumber = pRfid->value();
		if(!rfidSerialNumber.empty())
		{
			_rfidMap[rfidSerialNumber] = pFiles;
		}
	}

	/*******************
	 * FILE OPERATIONS *
	 *******************/
	// Files
	void Library::setEpisode(string rfidSerialNumber)
	{
		// search rfid-serial-number in map
		map<string, xml_node<>*>::iterator it;
		it = _rfidMap.find(rfidSerialNumber);
		// entry found
		if(it != _rfidMap.end())
		{
			#ifdef XML_LOG_OUTPUT
				_logStream << "found <files> with rfid [" << rfidSerialNumber << "]";
				(*_pLinkToConsole)->printLog(&_logStream);
			#endif
			// save state of current episode
			stopFile();

			// set current-episode-files to <files>-node of found rfid-serial-number
			_pCurrentEpisodeFiles = it->second;

			// set file for current episode with index of current file from xml
			setFile(getCurrentFileIndex());

			// play current file
			playFile();
		}
		// entry not found
		else
		{
			_logStream << "not found <files> with rfid [" << rfidSerialNumber << "]";
			(*_pLinkToConsole)->printLog(&_logStream);
		}
		return;
	}

	void Library::setFile(int currentFileIndex)
	{
		// get <file>-node at current-file-index
		xml_node<>* pFile = getChildAt(_pCurrentEpisodeFiles, currentFileIndex-1);
		// <file>-node found
		if(pFile != NULL)
		{
			#ifdef XML_LOG_OUTPUT
				_logStream << "found <file> at index [" << currentFileIndex << "]";
				(*_pLinkToConsole)->printLog(&_logStream);
			#endif
			// set current-file to found <file>-node
			_pCurrentFile = pFile;
		}
		// <file>-node not found
		else
		{
			_logStream << "not found <file> at index [" << currentFileIndex << "]";
			(*_pLinkToConsole)->printLog(&_logStream);
		}
	}

	void Library::playFile()
	{
		if(_pCurrentFile == NULL)
		{
			return;
		}

		// play current file at given timestamp from xml
		double timestamp = getCurrentTimestamp();
		_outStream << "play from pos: " << timestamp;
		(*_pLinkToConsole)->printOut(&_outStream);

		_pPlayer->playFile(_pCurrentFile->value(), timestamp);
	}

	void Library::stopFile()
	{
		if(_pCurrentEpisodeFiles == NULL || _pCurrentFile == NULL)
		{
			return;
		}

		double timestamp = _pPlayer->stopFile();
		if(timestamp >= 0)
		{
			setCurrentTimestamp(timestamp);
		}

		_outStream << "current pos: " << timestamp;
		(*_pLinkToConsole)->printOut(&_outStream);
	}

	void Library::playPause()
	{
		if(_pCurrentEpisodeFiles == NULL || _pCurrentFile == NULL)
		{
			return;
		}

		double timestamp = _pPlayer->playPauseFile();
		if(timestamp >= 0)
		{
			setCurrentTimestamp(timestamp);
		}

		_outStream << "current pos: " << timestamp;
		(*_pLinkToConsole)->printOut(&_outStream);
	}

	void Library::nextFile()
	{
		if(_pCurrentEpisodeFiles == NULL)
		{
			return;
		}
		setCurrentTimestamp(0);


		int nextIndex = getCurrentFileIndex();
		++nextIndex;
		if(nextIndex <= getChildCount(_pCurrentEpisodeFiles))
		{
			setCurrentFileIndex(nextIndex);
			setFile(nextIndex);
			playFile();
		}
		else
		{
			resetFiles();
		}
	}

	void Library::previousFile()
	{
		if(_pCurrentEpisodeFiles == NULL)
		{
			return;
		}
		setCurrentTimestamp(0);

		int previousIndex = getCurrentFileIndex();
		--previousIndex;
		if(previousIndex > 0)
		{
			setCurrentFileIndex(previousIndex);
			setFile(previousIndex);
			playFile();
		}
	}

	void Library::resetFiles()
	{
		if(_pCurrentEpisodeFiles == NULL)
		{
			return;
		}

		setCurrentFileIndex(1);
		setCurrentTimestamp(0);
		setFile(1);
		playFile();
		stopFile();
	}

	void Library::navigate(Navigation op)
	{
		switch(op)
		{
			case Navigation::PLAY_PAUSE:
				playPause();
				break;
			case Navigation::NEXT:
				nextFile();
				break;
			case Navigation::BACK:
				previousFile();
				break;
			case Navigation::RESET:
				resetFiles();
				break;
		}
	}


	/***************
	 * XML HELPERS *
	 ***************/
	int Library::getChildCount(xml_node<>* pNode)
	{
		int i = 0;
		for (xml_node<>* pChild = pNode->first_node(); pChild != NULL; pChild = pChild->next_sibling())
		{
			i++;
		}
		return i;
	}

	xml_node<>* Library::getChildAt(xml_node<>* pNode, int index)
	{
		int i = 0;
		for (xml_node<>* pChild = pNode->first_node(); pChild != NULL; pChild = pChild->next_sibling())
		{
			if(i == index)
			{
				return pChild;
			}
			i++;
		}
		return NULL;
	}

	int Library::getCurrentFileIndex(void)
	{
		// get [current_file]-attribute of <files>-node
		xml_attribute<>* pCurrentFile = _pCurrentEpisodeFiles->first_attribute(LibraryTags::ATTRIBUTE_CURRENT_FILE);
		// parse string to int
		return stoi(pCurrentFile->value());
	}

	void Library::setCurrentFileIndex(int index)
	{
		xml_attribute<>* pCurrentFile = _pCurrentEpisodeFiles->first_attribute(LibraryTags::ATTRIBUTE_CURRENT_FILE);
		string newIndex = to_string(index);
		const char* text = _libraryDoc.allocate_string(newIndex.c_str());
		pCurrentFile->value(text);
	}

	double Library::getCurrentTimestamp(void)
	{
		// get [timestamp]-attribute of <files>-node
		xml_attribute<>* pTimestamp = _pCurrentEpisodeFiles->first_attribute(LibraryTags::ATTRIBUTE_TIMESTAMP);
		// parse string to int
		return stod(pTimestamp->value());
	}

	void Library::setCurrentTimestamp(double timestamp)
	{
		xml_attribute<>* pTimestamp = _pCurrentEpisodeFiles->first_attribute(LibraryTags::ATTRIBUTE_TIMESTAMP);
		string newTimestamp = to_string(timestamp);
		const char* text = _libraryDoc.allocate_string(newTimestamp.c_str());
		pTimestamp->value(text);
	}
}
