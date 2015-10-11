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

	/*
	fstream Library::openLibraryFile(void)
	{
		fstream libraryFileStream;
		libraryFileStream.exceptions(failbit | badbit);
		try
		{
			libraryFileStream.open(LIBRARY_FILE_NAME, in | out | app);
			return fstream libraryFileStream;;
		}
		catch(failure e)
		{
			perror("unable to load library file");
			exit(-1);
		}
	}
	
	void Library::closeLibraryFile(fstream libraryFileStream)
	{
		if(libraryFileStream.is_open())
		{
			libraryFileStream.close();
		}
	}
	*/
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
		outStream << "Name of library node is: ";
		outStream << pLibrary->name() << endl;
		(*_pLinkToConsole)->printOut(&outStream);

		parseSeries(pLibrary);

		/*
		xml_node<>* node = doc.first_node(LibraryTags::TAG_SERIES);
		outStream << "Node " << LibraryTags::TAG_SERIES << " has value ";
		outStream << node->value() << endl;
		(*_pLinkToConsole)->printOut(&outStream);
		
		for (xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute())
		{
			outStream << "Node foobar has attribute ";
			outStream << attr->name() << " ";
			outStream << "with value ";
			outStream << attr->value() << endl;
			(*_pLinkToConsole)->printOut(&outStream);
		}
		*/
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
			outStream << "Name of series node is: ";
			outStream << pSeriesNode->name() << endl;
			(*_pLinkToConsole)->printOut(&outStream);
			
			parseSeriesNode(pSeriesNode);
		}
	}
	
	void Library::parseSeriesNode(xml_node<>* pSeries)
	{
		xml_node<>* pTitle = pSeries->first_node(LibraryTags::TAG_TITLE);
		outStream << "Name of title node is: ";
		outStream << pTitle->name() << endl;
		(*_pLinkToConsole)->printOut(&outStream);
		
		outStream << "Value of title node is: ";
		outStream << pTitle->value() << endl;
		(*_pLinkToConsole)->printOut(&outStream);
		
		xml_node<>* pCover = pSeries->first_node(LibraryTags::TAG_COVER);
		outStream << "Name of cover node is: ";
		outStream << pCover->name() << endl;
		(*_pLinkToConsole)->printOut(&outStream);
		
		outStream << "Value of cover node is: ";
		outStream << pCover->value() << endl;
		(*_pLinkToConsole)->printOut(&outStream);

		parseEpisodes(pSeries);
	}
	
	void Library::parseEpisodes(xml_node<>* pSeries)
	{
		for(xml_node<>* pEpisode = pSeries->first_node(LibraryTags::TAG_EPISODE); pEpisode; pEpisode = pEpisode->next_sibling())
		{
			outStream << "Name of episode node is: ";
			outStream << pEpisode->name() << endl;
			(*_pLinkToConsole)->printOut(&outStream);
			
			parseEpisodeNode(pEpisode);
		}
	}
	
	void Library::parseEpisodeNode(xml_node<>* pEpisode)
	{
		xml_node<>* pTitle = pEpisode->first_node(LibraryTags::TAG_TITLE);
		outStream << "Name of title node is: ";
		outStream << pTitle->name() << endl;
		(*_pLinkToConsole)->printOut(&outStream);
		
		outStream << "Value of title node is: ";
		outStream << pTitle->value() << endl;
		(*_pLinkToConsole)->printOut(&outStream);
		
		xml_node<>* pCover = pEpisode->first_node(LibraryTags::TAG_COVER);
		outStream << "Name of cover node is: ";
		outStream << pCover->name() << endl;
		(*_pLinkToConsole)->printOut(&outStream);
		
		outStream << "Value of cover node is: ";
		outStream << pCover->value() << endl;
		(*_pLinkToConsole)->printOut(&outStream);
		
		xml_node<>* pRfid = pEpisode->first_node(LibraryTags::TAG_RFID);
		outStream << "Name of rfid node is: ";
		outStream << pRfid->name() << endl;
		(*_pLinkToConsole)->printOut(&outStream);
		
		outStream << "Value of rfid node is: ";
		outStream << pRfid->value() << endl;
		(*_pLinkToConsole)->printOut(&outStream);

		parseFiles(pEpisode);
	}
	
	void Library::parseFiles(xml_node<>* pEpisode)
	{
		xml_node<>* pFiles = pEpisode->first_node(LibraryTags::TAG_FILES);
		outStream << "Name of files node is: ";
		outStream << pFiles->name() << endl;
		(*_pLinkToConsole)->printOut(&outStream);
		
		for (xml_attribute<>* pAttribute = pFiles->first_attribute(); pAttribute; pAttribute = pAttribute->next_attribute())
		{
			outStream << "Node files has attribute ";
			outStream << pAttribute->name() << " ";
			outStream << "with value ";
			outStream << pAttribute->value() << endl;
			(*_pLinkToConsole)->printOut(&outStream);
		}
		
		for(xml_node<>* pFile = pFiles->first_node(LibraryTags::TAG_FILE); pFile; pFile = pFile->next_sibling())
		{
			parseFileNode(pFile);
		}
	}
	
	void Library::parseFileNode(xml_node<>* pFile)
	{
		outStream << "Name of file node is: ";
		outStream << pFile->name() << endl;
		(*_pLinkToConsole)->printOut(&outStream);
		
		outStream << "Value of file node is: ";
		outStream << pFile->value() << endl;
		(*_pLinkToConsole)->printOut(&outStream);
	}
	
	Library::File* Library::getFile(string rfidSerialNumber)
	{
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
}
