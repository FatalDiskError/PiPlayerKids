#include <iostream>
#include <iterator>
#include <algorithm>
#include "mp3lib.hpp"
#include <boost/filesystem.hpp>
using namespace std;
using namespace boost::filesystem;

namespace mp3lib {
	Mp3Lib::FileBrowser::FileBrowser(void){
		throw "No instances of Mp3Lib::FileBrowser::FileBrowser allowed.";
	}

	Mp3Lib::FileBrowser::~FileBrowser(void){}

	path Mp3Lib::FileBrowser::getPath(string dir)
	{
		path p(dir);
		try
		{
			if(exists(p))
			{
				#ifndef SILENT_FILEBROWSER
				cout << "FileBrowser::getPath(" << dir << ") is path " << p << endl;
				#endif
				return p;
			}
		}
		catch (const filesystem_error& ex)
		{
			cout << ex.what() << endl;
		}
	}

	vector<path> Mp3Lib::FileBrowser::getSubDirectories(path p)
	{
		#ifndef SILENT_FILEBROWSER
		cout << "FileBrowser::getSubDirectories(" << p << ") is forwarded to FileBrowser::getFiles(" << p << ")" << endl;
		#endif
		return getFiles(p, CURRENT_PATH);
	}

	vector<path> Mp3Lib::FileBrowser::getFiles(path p, FileExtensions fileExtension)
	{
		try
		{
			if(exists(p))
			{
				if(is_directory(p))
				{
					#ifndef SILENT_FILEBROWSER
					cout << endl << "FileBrowser::getFiles(" << p << ") contains:" << endl;
					#endif

					vector<path> ret;
					for(directory_iterator di(p); di!=directory_iterator(); di++){
						if(conditional_check(di->path(), fileExtension))
						{
							#ifndef SILENT_FILEBROWSER
							cout << " adding ";
							#endif
							ret.push_back(di->path());
						}
						else
						{
							#ifndef SILENT_FILEBROWSER
							cout << "        ";
							#endif
						}
						#ifndef SILENT_FILEBROWSER
						cout << di->path() << " - " << di->path().filename() << endl;
						#endif
					}
					#ifndef SILENT_FILEBROWSER
					cout << "FileBrowser::getFiles(" << p << ") end" << endl << endl;
					#endif
					return ret;
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
	}

	bool Mp3Lib::FileBrowser::conditional_check(path p, FileExtensions fileExtension)
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
			case COVER:
				return is_regular_file(p) && p.filename()==fileExtensionMap[COVER];
				break;
			case DEFAULT_COVER:
				break;
		}
		return false;
	}
}
