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
	Library::Library(char* pWorkingDir, Console** ppConsole=NULL)
	{
		char libraryPath[strlen(pWorkingDir) + strlen(LIBRARY_FILE_NAME)];
		strcpy(libraryPath, pWorkingDir);
		strcat(libraryPath, LIBRARY_FILE_NAME);
		_pLibraryPath = libraryPath;
		
		_pLinkToConsole=ppConsole;
		parseLibraryFile();
	}

	Library::~Library(void){}

	void Library::parseLibraryFile(void)
	{
		/*
		ifstream libXmlFile LIBRARY_FILE_NAME;
		vector<char> buffer((istreambuf_iterator<char>(libXmlFile)), istreambuf_iterator<char>());
		buffer.push_back('\0');
		doc.parse<0>(&buffer[0]);
		*/

		//cout << _pLibraryPath << endl;
		//file<> xmlFile(LIBRARY_FILE_NAME);
		file<> xmlFile("./bin/library/library.xml");
		xml_document<> doc;
		doc.parse<0>(xmlFile.data());

		xml_node<>* pLibrary = doc.first_node(LibraryTags::TAG_LIBRARY);
		outStream << pLibrary->name();
		(*_pLinkToConsole)->printOut(&outStream);

		parseSeries(pLibrary);
	}

	/*
	void Library::parseNode(xml_node<>* pParent, string tagName)
	{
		for(xml_node<>* pNode = pParent->first_node(tagName); pNode; pNode = pNode->next_sibling())
		{
			outStream << "Name of node is: ";
			outStream << pNode->name() << endl;
			(*_pLinkToConsole)->printOut(&outStream);
			
			parseSeriesNode(pSeriesNode);
		}
	}
	*/
	
	void Library::parseSeries(xml_node<>* pLibrary)
	{
		for(xml_node<>* pSeriesNode = pLibrary->first_node(LibraryTags::TAG_SERIES); pSeriesNode; pSeriesNode = pSeriesNode->next_sibling())
		{
			outStream << "  " << pSeriesNode->name();
			parseSeriesNode(pSeriesNode);
		}
	}
	
	void Library::parseSeriesNode(xml_node<>* pSeries)
	{
		xml_node<>* pTitle = pSeries->first_node(LibraryTags::TAG_TITLE);
		outStream << " [" << pTitle->name() << ": " << pTitle->value() << "]";
		(*_pLinkToConsole)->printOut(&outStream);

		/*
		xml_node<>* pCover = pSeries->first_node(LibraryTags::TAG_COVER);
		outStream << pCover->name() << ": " << pCover->value() << endl;
		(*_pLinkToConsole)->printOut(&outStream);
		*/

		parseEpisodes(pSeries);
	}
	
	void Library::parseEpisodes(xml_node<>* pSeries)
	{
		for(xml_node<>* pEpisode = pSeries->first_node(LibraryTags::TAG_EPISODE); pEpisode; pEpisode = pEpisode->next_sibling())
		{
			outStream << "    " << pEpisode->name();
			parseEpisodeNode(pEpisode);
		}
	}
	
	void Library::parseEpisodeNode(xml_node<>* pEpisode)
	{
		xml_node<>* pTitle = pEpisode->first_node(LibraryTags::TAG_TITLE);
		xml_node<>* pRfid = pEpisode->first_node(LibraryTags::TAG_RFID);
		outStream << " [" << pTitle->name() << ": " << pTitle->value() << " / " << pRfid->name() << ": " << pRfid->value() << "]";
		(*_pLinkToConsole)->printOut(&outStream);

		string rfidSerialNumber = pRfid->value();
		if(!rfidSerialNumber.empty())
		{
			rfidMap[rfidSerialNumber] = pEpisode;
		}
		
		/*
		xml_node<>* pCover = pEpisode->first_node(LibraryTags::TAG_COVER);
		outStream << pCover->name() << ": " << pCover->value() << endl;
		(*_pLinkToConsole)->printOut(&outStream);
		*/

		parseFiles(pEpisode);
	}
	
	void Library::parseFiles(xml_node<>* pEpisode)
	{
		xml_node<>* pFiles = pEpisode->first_node(LibraryTags::TAG_FILES);
		outStream << "      " << pFiles->name() << " [";
		
		for (xml_attribute<>* pAttribute = pFiles->first_attribute(); pAttribute; pAttribute = pAttribute->next_attribute())
		{
			outStream << pAttribute->name() << ": " << pAttribute->value() << " - ";
		}
		
		outStream << "]";
		(*_pLinkToConsole)->printOut(&outStream);
		
		for(xml_node<>* pFile = pFiles->first_node(LibraryTags::TAG_FILE); pFile; pFile = pFile->next_sibling())
		{
			parseFileNode(pFile);
		}
	}
	
	void Library::parseFileNode(xml_node<>* pFile)
	{
		//outStream << "   " << pFile->name() << ": " << pFile->value() << endl;
		//(*_pLinkToConsole)->printOut(&outStream);
	}










	Library::File* Library::getFile(string rfidSerialNumber)
	{
		map<string, xml_node<>*>::iterator it;
		it = rfidMap.find(rfidSerialNumber);
		if(it != rfidMap.end())
		{
			logStream << rfidMap.size();
			(*_pLinkToConsole)->printLog(&logStream);
		}
		return NULL;
	}
	
	Library::File* Library::getNextFile()
	{
		return NULL;
	}
	
	void Library::writeCurrentFile(string rfidSerialNumber, string name)
	{
		for(map<string, xml_node<>*>::iterator it=rfidMap.begin(); it!=rfidMap.end(); ++it)
		{
			outStream << "### " << it->first << ": " << it->second;
			(*_pLinkToConsole)->printOut(&outStream);
		}
	}
	
	void Library::writeCurrentTime(string rfidSerialNumber, string timestamp)
	{
	}
}
