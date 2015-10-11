#include <iostream>
#include <boost/filesystem.hpp>
#include "mp3lib.hpp"
using namespace std;
using namespace boost::filesystem;

namespace mp3lib {
	Mp3Lib::Series::Series(path p)
	{
		#ifndef SILENT_SERIES_CORE
		cout << "  Series::Series(" << p << ")" << endl;
		#endif
		_path = p;
		_pEpisode = NULL;

		#ifndef SILENT_SERIES_CORE
		cout << "  Series::init(void)" << endl;
		#endif

		vector<path> subDirs = FileBrowser::getSubDirectories(_path);

		#ifndef SILENT_SERIES_INFO
		cout << "  Series " << _path << " is parsing episodes" << endl;
		#endif
		for(vector<path>::iterator it = subDirs.begin(); it!=subDirs.end(); ++it)
		{
			Episode *episode = new Episode(*it);
			_episodes.push_back(episode);
		}
		_episodesIterator = _episodes.begin();
		#ifndef SILENT_SERIES_INFO
		cout << "  Series " << _path << " parsing episodes done" << endl;
		#endif

		//FileBrowser::browseFolders(_path, this, COVER);
		//FileBrowser::browseFolders(_path, this, CURRENT_PATH);
	}

	Mp3Lib::Series::~Series(void)
	{
		#ifndef SILENT_SERIES_CORE
		cout << "  Series::~Series(void)" << endl;
		#endif
	}

	string Mp3Lib::Series::getSeriesName(void)
	{
		return _path.filename().generic_string();
	}

	/*
	 * ####################
	 * ###   EPISODES   ###
	 * ####################
	 */
	void Mp3Lib::Series::showEpisode(void)
	{
		cout << (*_episodesIterator)->getEpisodeName() << endl;
	}

	void Mp3Lib::Series::nextEpisode(void)
	{
		++_episodesIterator;
		if(_episodesIterator==_episodes.end())
		{
			_episodesIterator = _episodes.begin();
		}
		showEpisode();
	}

	void Mp3Lib::Series::previousEpisode(void)
	{
		if(_episodesIterator==_episodes.begin())
		{
			_episodesIterator = _episodes.end();
		}
		--_episodesIterator;
		showEpisode();
	}

	void Mp3Lib::Series::enterEpisode(void)
	{
		_pEpisode = *_episodesIterator;
		cout << ">>> enter " << _pEpisode->getEpisodeName() << endl;
	}

	void Mp3Lib::Series::exitEpisode(void)
	{
		cout << "<<< exit " << _pEpisode->getEpisodeName() << endl;
		_pEpisode = NULL;
	}

	/*
	 * ####################
	 * ###     MP3      ###
	 * ####################
	 */
	void Mp3Lib::Series::showMp3(void)
	{
		if(_pEpisode==NULL)
		{
			return;
		}
		_pEpisode->showMp3();
	}

	void Mp3Lib::Series::nextMp3(void)
	{
		if(_pEpisode==NULL)
		{
			return;
		}
		_pEpisode->nextMp3();
	}

	void Mp3Lib::Series::previousMp3(void)
	{
		if(_pEpisode==NULL)
		{
			return;
		}
		_pEpisode->previousMp3();
	}

	void Mp3Lib::Series::playMp3(void)
	{
		if(_pEpisode==NULL)
		{
			return;
		}
		_pEpisode->playMp3();
	}

	void Mp3Lib::Series::pauseMp3(void)
	{
		if(_pEpisode==NULL)
		{
			return;
		}
		_pEpisode->pauseMp3();
	}
}
