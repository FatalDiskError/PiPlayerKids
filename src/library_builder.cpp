#include "library_builder.hpp"

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
		 * /

		/ *
		 * libsigc++ signals
		completedSignal.connect(pNodeLibrary->nextFileSlot);
		_pPlayer->completedSignal.connect(nextFileSlot);
		 * /

		/ *
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
			_logStream << "append to " << _libraryFile.filename();
			(*_pLinkToConsole)->printLog(&_logStream);

			_pLibraryXmlFile = new file<>(_libraryFile.c_str());
			_libraryDoc.parse<parse_full>(_pLibraryXmlFile->data());

			xml_node<>* pNodeLibrary = _libraryDoc.first_node(LibraryTags::TAG_LIBRARY);
			parseSeriesFolders(pNodeLibrary);

		}
		else if(operation == Operations::FULL)
		{
			_logStream << "full create " << _libraryFile.filename();
			(*_pLinkToConsole)->printLog(&_logStream);

			xml_node<>* pNodeLibrary = writeEmptyLibraryNode();
			parseSeriesFolders(pNodeLibrary);
		}
		else if(operation == Operations::RFID)
		{
			_logStream << "add rfid to " << _libraryFile.filename();
			(*_pLinkToConsole)->printLog(&_logStream);


		}

		writeFile();

		_logStream << "build " << _libraryFile.filename() << " done";
		(*_pLinkToConsole)->printLog(&_logStream);
	}

	void LibraryBuilder::writeFile(void)
	{
		_logStream << "write " << _libraryFile.filename();
		(*_pLinkToConsole)->printLog(&_logStream);

		string data;
		print(back_inserter(data), _libraryDoc);

		ofstream newLibraryFile;
		newLibraryFile.open(_libraryFile.c_str());
		newLibraryFile << data;
		newLibraryFile.close();
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

	xml_node<>* LibraryBuilder::getExistingNode(string nodeName, xml_node<>* pNodeParent, const char* tagType)
	{
		for(xml_node<>* pNode = pNodeParent->first_node(tagType); pNode; pNode = pNode->next_sibling())
		{
			if(tagType == LibraryTags::TAG_FILE)
			{
				if(pNode->value() == nodeName)
				{
					return pNode;
				}
			}
			else if(tagType == LibraryTags::TAG_EPISODE || tagType == LibraryTags::TAG_SERIES)
			{
				xml_node<>* pNodeTitle = pNode->first_node(LibraryTags::TAG_TITLE);
				if(pNodeTitle->value() == nodeName)
				{
					return pNode;
				}
			}
		}
		return NULL;
	}

	void LibraryBuilder::parseSeriesFolders(xml_node<>* pNodeLibrary)
	{
		vector<path> vecSeries = getFiles(_libraryPath);
		sort(vecSeries.begin(), vecSeries.end());
		for(vector<path>::iterator itSeries = vecSeries.begin(); itSeries!=vecSeries.end(); ++itSeries)
		{
			path pathSeries = *itSeries;
			//tracePath("pathSeries", pathSeries);

			string seriesName = pathSeries.filename().string();
			xml_node<>* pNodeSeries = getExistingNode(seriesName, pNodeLibrary, LibraryTags::TAG_SERIES);
			if(pNodeSeries)
			{
				// found series
				// update/write episodes
				// update/write files
				_logStream << "found\t |-series " << seriesName;
				(*_pLinkToConsole)->printLog(&_logStream);
			}
			else
			{
				// write series
				// write episodes
				// write files
				_logStream << "write\t |-series " << seriesName;
				(*_pLinkToConsole)->printLog(&_logStream);

				pNodeSeries = writeSeriesNode(seriesName);
				pNodeLibrary->append_node(pNodeSeries);
			}

			parseEpisodesFolders(pNodeSeries, pathSeries);
		}
	}

	void LibraryBuilder::parseEpisodesFolders(xml_node<>* pNodeSeries, path pathSeries)
	{
		vector<path> vecEpisodes = getFiles(pathSeries);
		sort(vecEpisodes.begin(), vecEpisodes.end());
		for(vector<path>::iterator itEpisodes = vecEpisodes.begin(); itEpisodes!=vecEpisodes.end(); ++itEpisodes)
		{
			path pathEpisode = *itEpisodes;
			//tracePath("pathEpisode", pathEpisode);

			string episodeName = pathEpisode.filename().string();
			xml_node<>* pNodeEpisode = getExistingNode(episodeName, pNodeSeries, LibraryTags::TAG_EPISODE);
			if(pNodeEpisode)
			{
				// found episode
				_logStream << "found\t |  |-episode " << episodeName;
				(*_pLinkToConsole)->printLog(&_logStream);

			}
			else
			{
				// write episode
				_logStream << "write\t |  |-episode " << episodeName;
				(*_pLinkToConsole)->printLog(&_logStream);

				pNodeEpisode = writeEpisodeNode(episodeName);
				pNodeSeries->append_node(pNodeEpisode);

				xml_node<>* pNodeFiles = pNodeEpisode->first_node(LibraryTags::TAG_FILES);
				parseFilesFolders(pNodeFiles, pathEpisode);
			}
		}
	}

	void LibraryBuilder::parseFilesFolders(xml_node<>* pNodeFiles, path pathEpisodes)
	{
		vector<path> vecFiles = getFiles(pathEpisodes, MP3);
		sort(vecFiles.begin(), vecFiles.end());
		for(vector<path>::iterator itFiles = vecFiles.begin(); itFiles!=vecFiles.end(); ++itFiles)
		{
			path pathFile = *itFiles;
			//tracePath("pathFile", pathFile);

			path pathRelativeFile = relativeTo(_libraryPath, pathFile);
			//tracePath("pathRelativeFile", pathRelativeFile);

			string fileName = pathRelativeFile.string();

			_logStream << "write\t |  |  |-file " << fileName;
			(*_pLinkToConsole)->printLog(&_logStream);

			// write file
			xml_node<>* pNodeFile = writeFileNode(fileName);
			pNodeFiles->append_node(pNodeFile);
		}
	}

	xml_node<>* LibraryBuilder::writeEmptyLibraryNode(void)
	{
		xml_node<>* pNodeDeclaration = _libraryDoc.allocate_node(node_declaration);
		pNodeDeclaration->append_attribute(_libraryDoc.allocate_attribute(LibraryTags::DECLARATION_VERSION, LibraryTags::DECLARATION_VERSION_VALUE));
		pNodeDeclaration->append_attribute(_libraryDoc.allocate_attribute(LibraryTags::DECLARATION_ENCODING, LibraryTags::DECLARATION_ENCODING_VALUE));
		_libraryDoc.append_node(pNodeDeclaration);

		xml_node<>* pNodeLibrary = _libraryDoc.allocate_node(node_element, LibraryTags::TAG_LIBRARY);
		_libraryDoc.append_node(pNodeLibrary);

		return pNodeLibrary;
	}

	xml_node<>* LibraryBuilder::writeSeriesNode(string seriesName)
	{
		xml_node<>* pNodeSeries = _libraryDoc.allocate_node(node_element, LibraryTags::TAG_SERIES);
		xml_node<>* pNodeTitle = _libraryDoc.allocate_node(node_element, LibraryTags::TAG_TITLE);
		xml_node<>* pNodeCover = _libraryDoc.allocate_node(node_element, LibraryTags::TAG_COVER);

		pNodeSeries->append_node(pNodeTitle);
		pNodeSeries->append_node(pNodeCover);

		const char* title = _libraryDoc.allocate_string(seriesName.c_str());
		pNodeTitle->value(title);

		return pNodeSeries;
	}

	xml_node<>* LibraryBuilder::writeEpisodeNode(string episodeName)
	{
		xml_node<>* pNodeEpisode = _libraryDoc.allocate_node(node_element, LibraryTags::TAG_EPISODE);
		xml_node<>* pNodeTitle = _libraryDoc.allocate_node(node_element, LibraryTags::TAG_TITLE);
		xml_node<>* pNodeCover = _libraryDoc.allocate_node(node_element, LibraryTags::TAG_COVER);
		xml_node<>* pNodeFiles = _libraryDoc.allocate_node(node_element, LibraryTags::TAG_FILES);

		pNodeEpisode->append_node(pNodeTitle);
		pNodeEpisode->append_node(pNodeCover);
		pNodeEpisode->append_node(pNodeFiles);

		const char* title = _libraryDoc.allocate_string(episodeName.c_str());
		pNodeTitle->value(title);

		pNodeFiles->append_attribute(_libraryDoc.allocate_attribute(LibraryTags::ATTRIBUTE_RFID, ""));
		pNodeFiles->append_attribute(_libraryDoc.allocate_attribute(LibraryTags::ATTRIBUTE_CURRENT_FILE, "1"));
		pNodeFiles->append_attribute(_libraryDoc.allocate_attribute(LibraryTags::ATTRIBUTE_TIMESTAMP, "0"));

		return pNodeEpisode;
	}

	xml_node<>* LibraryBuilder::writeFileNode(string fileName)
	{
		xml_node<>* pNodeFile = _libraryDoc.allocate_node(node_element, LibraryTags::TAG_FILE);

		const char* filePath = _libraryDoc.allocate_string(fileName.c_str());
		pNodeFile->value(filePath);

		return pNodeFile;
		//node_cdata
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
		}
		catch (const filesystem_error& ex)
		{
			GlobalExit::exit(ErrorCode::EXIT_ERROR_FILESYSTEM, ex.what());
		}
		return ret;
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
}
