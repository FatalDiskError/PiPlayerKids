#include <fstream>
#include <vector>
#include <string>

#include <library_tags.hpp>
#include <console.hpp>

using namespace std;
using namespace console;

namespace library {
	Library::Library(Console** ppConsole=NULL)
	{
		_pLinkToConsole=ppConsole;
		parseLibraryFile();
	}

	Library::~Library(void){}

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
	
	void Library::parseLibraryFile(void)
	{
		fstream libraryFileStream = openLibraryFile();
		seekSeries(libraryFileStream, 0);
		closeLibraryFile(libraryFileStream);
	}
	
	void Library::seekSeries(fstream libraryFileStream, long seekStartPos)
	{
		
		//find series-open-tag
		//find series-close-tag
		//seek episodes in between
		//seek series
	}
	
	void Library::seekEpisodes(fstream libraryFileStream, long seekStartPos)
	{
	}
	
	void Library::seekFiles(fstream libraryFileStream, long seekStartPos)
	{
	}
	
	File* Library::getFile(string rfidSerialNumber)
	{
	}
	
	File* Library::getNextFile()
	{
	}
	
	void Library::writeCurrentFile(string rfidSerialNumber, string name)
	{
	}
	
	void Library::writeCurrentTime(string rfidSerialNumber, string timestamp)
	{
	}
}
