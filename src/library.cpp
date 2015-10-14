#define XML_LOG_OUTPUT

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>

#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>

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
	/*
	void Library::parseNode(xml_node<>* pParent, string tagName)
	{
		for(xml_node<>* pNode = pParent->first_node(tagName); pNode; pNode = pNode->next_sibling())
		{
			_outStream << "Name of node is: ";
			_outStream << pNode->name() << endl;
			(*_pLinkToConsole)->printOut(&_outStream);
			
			parseSeriesNode(pSeriesNode);
		}
	}
	*/
	
	void Library::parseLibraryFile(void)
	{
		_logStream << "opening " << _libraryPath;
		(*_pLinkToConsole)->printLog(&_logStream);
		
		file<> xmlFile(_libraryPath.c_str());
		xml_document<> doc;
		doc.parse<0>(xmlFile.data());

		xml_node<>* pLibrary = doc.first_node(LibraryTags::TAG_LIBRARY);
		_outStream << "+-- " << pLibrary->name();
		(*_pLinkToConsole)->printOut(&_outStream);

		parseSeries(pLibrary);
	}
	
	void Library::parseSeries(xml_node<>* pLibrary)
	{
		for(xml_node<>* pSeriesNode = pLibrary->first_node(LibraryTags::TAG_SERIES); pSeriesNode; pSeriesNode = pSeriesNode->next_sibling())
		{
			#ifdef XML_LOG_OUTPUT
			_outStream << "| +-- " << pSeriesNode->name();
			#endif
			parseSeriesNode(pSeriesNode);
		}
	}
	
	void Library::parseSeriesNode(xml_node<>* pSeries)
	{
		xml_node<>* pTitle = pSeries->first_node(LibraryTags::TAG_TITLE);

		#ifdef XML_LOG_OUTPUT
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
	
	void Library::parseEpisodes(xml_node<>* pSeries)
	{
		for(xml_node<>* pEpisode = pSeries->first_node(LibraryTags::TAG_EPISODE); pEpisode; pEpisode = pEpisode->next_sibling())
		{
			#ifdef XML_LOG_OUTPUT
			_outStream << "| | +-- " << pEpisode->name();
			#endif
			parseEpisodeNode(pEpisode);
		}
	}
	
	void Library::parseEpisodeNode(xml_node<>* pEpisode)
	{
		#ifdef XML_LOG_OUTPUT
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
		
		#ifdef XML_LOG_OUTPUT
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
	void Library::parseFiles(xml_node<>* pFiles)
	{
		#ifdef XML_LOG_OUTPUT
		xml_attribute<>* pRfid = pFiles->first_attribute(LibraryTags::ATTRIBUTE_RFID);
		xml_attribute<>* pCurrentFile = pFiles->first_attribute(LibraryTags::ATTRIBUTE_CURRENT_FILE);
		xml_attribute<>* pTimestamp = pFiles->first_attribute(LibraryTags::ATTRIBUTE_TIMESTAMP);
		
		int i = 1;
		char chr = (char)pCurrentFile->value();
		int currentFile = (int)chr;
		_outStream << "<" << currentFile << "/" << chr << "> " << pFiles->name() << " @ " << pRfid->value() << ": ";
		for(xml_node<>* pFile = pFiles->first_node(LibraryTags::TAG_FILE); pFile; pFile = pFile->next_sibling())
		{
			if(i != 1)
			{
				_outStream << ", ";
			}

			if(i == currentFile)
			{
				_outStream << "[" << pFile->value() << "]";
			}
			else
			{
				_outStream << pFile->value();
			}
			_outStream << " (" << i << ")";
			i++;
		}
		(*_pLinkToConsole)->printOut(&_outStream);
		#endif
	}
	
	Library::File* Library::getFile(string rfidSerialNumber)
	{
		map<string, xml_node<>*>::iterator it;
		it = rfidMap.find(rfidSerialNumber);
		if(it != rfidMap.end())
		{
			_logStream << "rfid [" << rfidSerialNumber << "] found";
			parseFiles(it->second);
		}
		else
		{
			_logStream << "rfid [" << rfidSerialNumber << "] not found";
		}
		(*_pLinkToConsole)->printLog(&_logStream);
		
		(*_pLinkToConsole)->printOut("[c]ontinue, [n]ext, [p]revious: ");
		bool isLoop = true;
		while(isLoop)
		{
			int charCode = (*_pLinkToConsole)->waitForChar();
			_logStream << "charCode: " << charCode << " - char: " << (char)charCode;
			(*_pLinkToConsole)->printLog(&_logStream);
			
			switch((char)charCode)
			{
				case 'c': //99
					isLoop = false;
					break;
				case 'n': //110
					break;
				case 'p': //112
					break;
			}
		}
		return NULL;
	}
	
	Library::File* Library::getNextFile()
	{
		return NULL;
	}
	
	void Library::writeCurrentFile(string rfidSerialNumber, string name)
	{
	}
	
	void Library::writeCurrentTime(string rfidSerialNumber, string timestamp)
	{
	}
	
	/*************
	 * DEBUGGING *
	 *************/
	void Library::logOutRfidMap(void)
	{
		(*_pLinkToConsole)->printLog("|--- rfid map:");
		for(map<string, xml_node<>*>::iterator it=rfidMap.begin(); it!=rfidMap.end(); ++it)
		{
			_logStream << "|  rfid [" << it->first << "]: " << it->second;
			(*_pLinkToConsole)->printLog(&_logStream);
		}
		(*_pLinkToConsole)->printLog("|---------");
	}
}
