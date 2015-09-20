#include <iostream>
#include <boost/filesystem.hpp>
#include "mp3lib.hpp"
#include "../sha256/sha256.hpp"

using namespace std;
using namespace boost::filesystem;

namespace mp3lib {
	Mp3Lib::Episode::Episode(path p)
	{
		#ifndef SILENT_EPISODE_CORE
		cout << "    Episode::Episode(" << p << ")" << endl;
		#endif
		_path = p;
		_hash = sha256(p.generic_string());
		_pMp3File = NULL;

		#ifndef SILENT_EPISODE_CORE
		cout << "    Episode::init(void)" << endl;
		#endif

		vector<path> mp3s = FileBrowser::getFiles(_path, MP3);

		#ifndef SILENT_EPISODE_INFO
		cout << "    Episode " << _hash << " (" << _path << ") is parsing mp3s" << endl;
		#endif
		for(vector<path>::iterator it = mp3s.begin(); it!=mp3s.end(); ++it)
		{
			#ifndef SILENT_EPISODE_INFO
			cout << "      Mp3 " << *it << endl;
			#endif
			_mp3Files.push_back(*it);
		}
		_mp3FilesIterator = _mp3Files.begin();
		#ifndef SILENT_SERIES_INFO
		cout << "    Episode " << _path << " parsing mp3s done" << endl;
		#endif

		//FileBrowser::browseFolders(_path, this, COVER);
		//FileBrowser::browseFolders(_path, this, MP3);
	}

	Mp3Lib::Episode::~Episode(void)
	{
		#ifndef SILENT_EPISODE_CORE
		cout << "    Episode::~Episode(void)" << endl;
		#endif
	}

	string Mp3Lib::Episode::getEpisodeName(void)
	{
		return _path.filename().generic_string();
	}

	/*
	 * ####################
	 * ###     MP3      ###
	 * ####################
	 */
	void Mp3Lib::Episode::showMp3(void)
	{
		cout << _mp3FilesIterator->filename() << endl;
		//cout << *_mp3FilesIterator << endl;
	}

	void Mp3Lib::Episode::nextMp3(void)
	{
		++_mp3FilesIterator;
		if(_mp3FilesIterator==_mp3Files.end())
		{
			_mp3FilesIterator = _mp3Files.begin();
		}
		showMp3();
	}

	void Mp3Lib::Episode::previousMp3(void)
	{
		if(_mp3FilesIterator==_mp3Files.begin())
		{
			_mp3FilesIterator = _mp3Files.end();
		}
		--_mp3FilesIterator;
		showMp3();
	}

	void Mp3Lib::Episode::playMp3(void)
	{
		_pMp3File = &(*_mp3FilesIterator);
		cout << "|> play " << *_pMp3File << endl;
	}

	void Mp3Lib::Episode::pauseMp3(void)
	{
		cout << "|| pause " << *_pMp3File << endl;
	}
}
