#define XML_OUT_OUTPUT
#define XML_LOG_OUTPUT

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <signals2.hpp>

#include "library.hpp"
#include "library_tags.hpp"
#include "console.hpp"

using namespace std;
using namespace rapidxml;
using namespace console;

namespace library {
	/***************
	 * CONSTRUCTOR *
	 ***************/
	Library::Library(string applicationPath, Console** ppConsole=NULL)
	{
		_pLinkToConsole = ppConsole;
		(*_pLinkToConsole)->printLog("constructing library");
		
		size_t lastDirectoryDividerPos = applicationPath.rfind(DIRECTORY_DIVIDER);
		if (lastDirectoryDividerPos!=string::npos)
		{
			_libraryPath = applicationPath.substr(0, lastDirectoryDividerPos);
			_libraryPath.append(LIBRARY_FILE_NAME);
		}
		parseLibraryFile();
	}
	
	Library::~Library(void)
	{
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
		
		file<> xmlFile(_libraryPath.c_str());
		_doc.parse<0>(xmlFile.data());
		xml_node<>* pLibrary = _doc.first_node(LibraryTags::TAG_LIBRARY);
		
		#ifdef XML_OUT_OUTPUT
		_outStream << "+-- " << pLibrary->name();
		(*_pLinkToConsole)->printOut(&_outStream);
		#endif
		
		parseSeries(pLibrary);
	}

	// Series
	void Library::parseSeries(xml_node<>* pLibrary)
	{
		for(xml_node<>* pSeriesNode = pLibrary->first_node(LibraryTags::TAG_SERIES); pSeriesNode; pSeriesNode = pSeriesNode->next_sibling())
		{
			#ifdef XML_OUT_OUTPUT
			_outStream << "| +-- " << pSeriesNode->name();
			#endif
			parseSeriesNode(pSeriesNode);
		}
	}
	void Library::parseSeriesNode(xml_node<>* pSeries)
	{
		xml_node<>* pTitle = pSeries->first_node(LibraryTags::TAG_TITLE);

		#ifdef XML_OUT_OUTPUT
		_outStream << " [" << pTitle->name() << ": " << pTitle->value() << "]";
		(*_pLinkToConsole)->printOut(&_outStream);
		/*
		xml_node<>* pCover = pSeries->first_node(LibraryTags::TAG_COVER);
		_outStream << pCover->name() << ": " << pCover->value() << endl;
		(*_pLinkToConsole)->printOut(&_outStream);
		*/
		#endif

		parseEpisodes(pSeries);
	}

	// Episodes
	void Library::parseEpisodes(xml_node<>* pSeries)
	{
		for(xml_node<>* pEpisode = pSeries->first_node(LibraryTags::TAG_EPISODE); pEpisode; pEpisode = pEpisode->next_sibling())
		{
			#ifdef XML_OUT_OUTPUT
			_outStream << "| | +-- " << pEpisode->name();
			#endif
			parseEpisodeNode(pEpisode);
		}
	}
	void Library::parseEpisodeNode(xml_node<>* pEpisode)
	{
		#ifdef XML_OUT_OUTPUT
		xml_node<>* pTitle = pEpisode->first_node(LibraryTags::TAG_TITLE);
		
		_outStream << " [" << pTitle->name() << ": " << pTitle->value() << "]";
		(*_pLinkToConsole)->printOut(&_outStream);
		/*
		xml_node<>* pCover = pEpisode->first_node(LibraryTags::TAG_COVER);
		_outStream << pCover->name() << ": " << pCover->value() << endl;
		(*_pLinkToConsole)->printOut(&_outStream);
		*/
		#endif
		
		xml_node<>* pFiles = pEpisode->first_node(LibraryTags::TAG_FILES);
		
		#ifdef XML_OUT_OUTPUT
		_outStream << "| | | +-- " << pFiles->name() << " [";
		
		for (xml_attribute<>* pAttribute = pFiles->first_attribute(); pAttribute; pAttribute = pAttribute->next_attribute())
		{
			_outStream << pAttribute->name() << ": " << pAttribute->value() << " - ";
		}
		
		_outStream << "]";
		(*_pLinkToConsole)->printOut(&_outStream);
		#endif
		
		xml_attribute<>* pRfid = pFiles->first_attribute(LibraryTags::ATTRIBUTE_RFID);
		
		string rfidSerialNumber = pRfid->value();
		if(!rfidSerialNumber.empty())
		{
			rfidMap[rfidSerialNumber] = pFiles;
		}
	}
	
	/*******************
	 * FILE OPERATIONS *
	 *******************/
	// Files
	void Library::setFile()
	{
		int currentFileIndex = getCurrentFileIndex();
		xmlnode<>* pFile = getChildAt(_pCurrentEpisodeFiles, currentFileIndex-1);
		if(pFile != NULL)
		{
			_logStream << "file [index " << currentFileIndex << "] found";
			(*_pLinkToConsole)->printLog(&_logStream);
			
			_pCurrentFile = pFile;
			
			int timestamp = getCurrentTimestamp();
			playSignal(_pCurrentFile->value(), timestamp);
		}
		else
		{
			_logStream << "file [index " << currentFileIndex << "] not found";
			(*_pLinkToConsole)->printLog(&_logStream);
		}
	}
	
	void Library::setEpisode(string rfidSerialNumber)
	{
		map<string, xml_node<>*>::iterator it;
		it = rfidMap.find(rfidSerialNumber);
		if(it != rfidMap.end())
		{
			_logStream << "rfid [" << rfidSerialNumber << "] found";
			(*_pLinkToConsole)->printLog(&_logStream);
			
			_pCurrentEpisodeFiles = it->second;
			
			setFile();
		}
		else
		{
			_logStream << "rfid [" << rfidSerialNumber << "] not found";
			(*_pLinkToConsole)->printLog(&_logStream);
		}
		return;
	}
	
	void Library::nextFile()
	{
		if(_pCurrentEpisodeFiles == NULL)
		{
			return;
		}
		
		int nextIndex = getCurrentFileIndex()++;
		if(nextIndex <= getChildCount())
		{
			setCurrentFileIndex(nextIndex);
			setFile();
		}
	}
	
	void Library::previousFile()
	{
		if(_pCurrentEpisodeFiles == NULL)
		{
			return;
		}
		
		int previousIndex = getCurrentFileIndex()--;
		if(previousIndex > 0)
		{
			setCurrentFileIndex(previousIndex);
			setFile();
		}
	}
	
	/*************
	 * DEBUGGING *
	 *************/
	void Library::logOutRfidMap(void)
	{
		#ifdef XML_LOG_OUTPUT
		(*_pLinkToConsole)->printLog("|--- rfid map:");
		for(map<string, xml_node<>*>::iterator it=rfidMap.begin(); it!=rfidMap.end(); ++it)
		{
			_logStream << "|  rfid [" << it->first << "]: " << it->second;
			(*_pLinkToConsole)->printLog(&_logStream);
		}
		(*_pLinkToConsole)->printLog("|---------");
		#endif
	}
	
	
	/***************
	 * XML HELPERS *
	 ***************/
	int Library::getChildCount(xmlnode<>* pNode)
	{
		int i = 0;
		for (xmlnode<>* pChild = pNode->first_node(); pChild != NULL; pChild = pChild->next_sibling())
		{
			i++;
		} 
		return i;
	}
	
	xmlnode<>* Library::getChildAt(xmlnode<>* pNode, int index)
	{
		int i = 0;
		for (xmlnode<>* pChild = pNode->first_node(); pChild != NULL; pChild = pChild->next_sibling())
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
		xml_attribute<>* pCurrentFile = _pCurrentEpisodeFiles->first_attribute(LibraryTags::ATTRIBUTE_CURRENT_FILE);
		return stoi(pCurrentFile->value());
	}
	
	void Library::setCurrentFileIndex(int index)
	{
		xml_attribute<>* pCurrentFile = _pCurrentEpisodeFiles->first_attribute(LibraryTags::ATTRIBUTE_CURRENT_FILE);
		string newIndex = to_string(index);
		const char* text = _doc.allocate_string(newIndex.c_str(), strlen(newIndex.c_str()));
		pCurrentFile->value(text);
	}
	
	int Library::getCurrentTimestamp(void)
	{
		xml_attribute<>* pTimestamp = _pCurrentEpisodeFiles->first_attribute(LibraryTags::ATTRIBUTE_TIMESTAMP);
		return = stoi(pTimestamp->value());
	}
	
	void Library::setCurrentTimestamp(int timestamp)
	{
		xml_attribute<>* pTimestamp = _pCurrentEpisodeFiles->first_attribute(LibraryTags::ATTRIBUTE_TIMESTAMP);
		string newTimestamp = to_string(timestamp);
		const char* text = _doc.allocate_string(newTimestamp.c_str(), strlen(newTimestamp.c_str()));
		pTimestamp->value(text);
	}
}
