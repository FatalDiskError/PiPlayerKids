#include <iostream>
#include <boost/filesystem.hpp>
#include "mp3lib.hpp"
using namespace std;
using namespace boost::filesystem;

namespace mp3lib {
	Mp3Lib::Mp3Lib(string dir)
	{
		#ifndef SILENT_LIB_CORE
		cout << "Mp3Lib::Mp3Lib(" << dir << ")" << endl;
		#endif

		_path = FileBrowser::getPath(dir);
		_pSeries = NULL;

		#ifndef SILENT_LIB_CORE
		cout << "Mp3Lib::init(void)" << endl;
		#endif

		vector<path> subDirs = FileBrowser::getSubDirectories(_path);

		#ifndef SILENT_LIB_INFO
		cout << "Mp3Lib " << _path << " is parsing series" << endl;
		#endif
		for(vector<path>::iterator it = subDirs.begin(); it!=subDirs.end(); ++it)
		{
			Series *series = new Series(*it);
			_series.push_back(series);
		}
		_seriesIterator = _series.begin();
		#ifndef SILENT_LIB_INFO
		cout << "Mp3Lib " << _path << " parsing series done" << endl;
		#endif
	}

	Mp3Lib::~Mp3Lib(void)
	{
		#ifndef SILENT_LIB_CORE
		cout << "Mp3Lib::~Mp3Lib(void)" << endl;
		#endif
	}

	/*
	 * ####################
	 * ###    SERIES    ###
	 * ####################
	 */
	void Mp3Lib::showSeries(void)
	{
		cout << (*_seriesIterator)->getSeriesName() << endl;
	}

	void Mp3Lib::nextSeries(void)
	{
		++_seriesIterator;
		if(_seriesIterator==_series.end())
		{
			_seriesIterator = _series.begin();
		}
		showSeries();
	}

	void Mp3Lib::previousSeries(void)
	{
		if(_seriesIterator==_series.begin())
		{
			_seriesIterator = _series.end();
		}
		--_seriesIterator;
		showSeries();
	}

	void Mp3Lib::enterSeries(void)
	{
		_pSeries = *_seriesIterator;
		cout << ">>> enter " << _pSeries->getSeriesName() << endl;
	}

	void Mp3Lib::exitSeries(void)
	{
		cout << "<<< exit " << _pSeries->getSeriesName() << endl;
		_pSeries = NULL;
	}

	/*
	 * ####################
	 * ###   EPISODES   ###
	 * ####################
	 */
	void Mp3Lib::showEpisode(void)
	{
		if(_pSeries==NULL)
		{
			return;
		}
		_pSeries->showEpisode();
	}

	void Mp3Lib::nextEpisode(void)
	{
		if(_pSeries==NULL)
		{
			return;
		}
		_pSeries->nextEpisode();
	}

	void Mp3Lib::previousEpisode(void)
	{
		if(_pSeries==NULL)
		{
			return;
		}
		_pSeries->previousEpisode();
	}

	void Mp3Lib::enterEpisode(void)
	{
		if(_pSeries==NULL)
		{
			return;
		}
		_pSeries->enterEpisode();
	}

	void Mp3Lib::exitEpisode(void)
	{
		if(_pSeries==NULL)
		{
			return;
		}
		_pSeries->exitEpisode();
	}

	/*
	 * ####################
	 * ###     MP3      ###
	 * ####################
	 */
	void Mp3Lib::showMp3(void)
	{
		if(_pSeries==NULL)
		{
			return;
		}
		_pSeries->showMp3();
	}

	void Mp3Lib::nextMp3(void)
	{
		if(_pSeries==NULL)
		{
			return;
		}
		_pSeries->nextMp3();
	}

	void Mp3Lib::previousMp3(void)
	{
		if(_pSeries==NULL)
		{
			return;
		}
		_pSeries->previousMp3();
	}

	void Mp3Lib::playMp3(void)
	{
		if(_pSeries==NULL)
		{
			return;
		}
		_pSeries->playMp3();
	}

	void Mp3Lib::pauseMp3(void)
	{
		if(_pSeries==NULL)
		{
			return;
		}
		_pSeries->pauseMp3();
	}

/*
	string Mp3Lib::getPath(void)
	{
		return _path.generic_string();
	}

	vector<Series*>* Mp3Lib::getSeries(void)
	{
		return &_series;
	}

	void Mp3Lib::testlib(void)
	{
		cout << "|-- " << getPath() << endl;

		list<Series*>::iterator series;
		list<Episode*>::iterator episodes;
		list<string>::iterator mp3Files;
		for(series = getSeries()->begin(); series != getSeries()->end(); ++series)
		{
			cout << "       |--" << (*series)->getPath() << " ### " << (*series)->getCover() << endl;
			for(episodes = (*series)->getEpisodes()->begin(); episodes != (*series)->getEpisodes()->end(); ++episodes)
			{
				cout << "              |--" << (*episodes)->getPath() << " ### " << (*episodes)->getCover() << endl;
				for(mp3Files = (*episodes)->getMp3Files()->begin(); mp3Files != (*episodes)->getMp3Files()->end(); ++mp3Files)
				{
					cout << "                     |--" << *mp3Files << endl;
				}
			}
		}
	}
*/
}
