#include <iostream>
#include <dirent.h>
#include <boost/locale.hpp>
#include "mp3lib.hpp"
using namespace std;
using namespace boost::locale;

namespace mp3lib {
	FileBrowser::FileBrowser(void){}

	FileBrowser::~FileBrowser(void){}

	void FileBrowser::browseFolders(string containerDir, IBrowseable *pBrowseable, FileExtensions fileExtension)
	{
		DIR *pDir;
		struct dirent *pDirEntry;

		pDir = opendir(containerDir.c_str());
		if(pDir != NULL)
		{
			while((pDirEntry = readdir(pDir)))
			{
				string fileName = pDirEntry->d_name;
				switch(fileExtension)
				{
					case CURRENT_PATH:
						if (pDirEntry->d_type == isFolder)
						{
							if(fileName != fileExtensionMap[CURRENT_PATH] && fileName != fileExtensionMap[PREV_PATH])
							{
								pBrowseable->parseFolder(fileName);
							}
						}
						break;
					case MP3:
						if (pDirEntry->d_type == isFile)
						{
							string smallFileName = to_lower(fileName);
							int searchLength = fileExtensionMap[MP3].size();
							int pos = smallFileName.size() - searchLength;

							if(pos > 0 && smallFileName.compare(pos, searchLength, fileExtensionMap[MP3]) == 0)
							{
								cout << "  ##  adding mp3: " << fileName << "  ###  " << smallFileName << endl;
								pBrowseable->parseFolder(fileName);
							}
						}
						break;
					case COVER:
						if( ICoverBrowseable* pCoverBrowseable = dynamic_cast<ICoverBrowseable*>(pBrowseable))
						{
							if (pDirEntry->d_type == isFile)
							{
								int searchLength = fileExtensionMap[COVER].size();
								if(fileName.compare(0, searchLength, fileExtensionMap[COVER]) == 0)
								{
									pCoverBrowseable->addCover(fileName);
								}
							}
						}
						break;
				}
			}
			(void)closedir(pDir);
		}
		else
		{
			perror(string("Couldn't open the directory: " + containerDir).c_str());
		}
	}
}
