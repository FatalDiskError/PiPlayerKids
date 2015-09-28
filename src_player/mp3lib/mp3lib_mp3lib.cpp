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

		_libraryBasePath = FileBrowser::getPath(dir);
		_libraryFilePath = FileBrowser::getFile(_libraryBasePath, LIBRARY);
		_pSeries = NULL;

		#ifndef SILENT_LIB_CORE
		cout << "Mp3Lib::init(void)" << endl;
		#endif
	}

	Mp3Lib::~Mp3Lib(void)
	{
		#ifndef SILENT_LIB_CORE
		cout << "Mp3Lib::~Mp3Lib(void)" << endl;
		#endif
	}
	
	void Mp3Lib::resetLibrary(void)
	{
		#ifndef SILENT_LIB_INFO
		cout << "Mp3Lib " << _libraryBasePath << " is reseting library" << endl;
		#endif
		remove(_libraryFilePath);
	}
	
	void Mp3Lib::loadLibrary(void)
	{
	}
	
	void Mp3Lib::writeLibrary(void)
	{
	}
	
	void Mp3Lib::parseSeries(void)
	{
		vector<path> subDirs = FileBrowser::getSubDirectories(_libraryBasePath);

		#ifndef SILENT_LIB_INFO
		cout << "Mp3Lib " << _libraryBasePath << " is parsing series" << endl;
		#endif
		for(vector<path>::iterator it = subDirs.begin(); it!=subDirs.end(); ++it)
		{
			Series *series = new Series(*it);
			_series.push_back(series);
		}
		_seriesIterator = _series.begin();
		#ifndef SILENT_LIB_INFO
		cout << "Mp3Lib " << _libraryBasePath << " parsing series done" << endl;
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
}
