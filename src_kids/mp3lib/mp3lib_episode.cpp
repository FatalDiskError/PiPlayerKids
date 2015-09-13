#include <iostream>
#include <boost/filesystem.hpp>
#include <fmod.hpp>
#include "mp3lib.hpp"
#include "../sha256/sha256.hpp"
//#include "common.h"

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

	int FMOD_Main(void)
	{
		FMOD::System     *system;
		FMOD::Sound      *sound, *sound_to_play;
		FMOD::Channel    *channel = 0;
		FMOD_RESULT       result;
		unsigned int      version;
		void             *extradriverdata = 0;
		int               numsubsounds;

		Common_Init(&extradriverdata);

		/*
			Create a System object and initialize.
		*/
		result = FMOD::System_Create(&system);
		ERRCHECK(result);

		result = system->getVersion(&version);
		ERRCHECK(result);

		if (version < FMOD_VERSION)
		{
			Common_Fatal("FMOD lib version %08x doesn't match header version %08x", version, FMOD_VERSION);
		}

		result = system->init(32, FMOD_INIT_NORMAL, extradriverdata);
		ERRCHECK(result);

		/*
			This example uses an FSB file, which is a preferred pack format for fmod containing multiple sounds.
			This could just as easily be exchanged with a wav/mp3/ogg file for example, but in this case you wouldnt need to call getSubSound.
			Because getNumSubSounds is called here the example would work with both types of sound file (packed vs single).
		*/
		result = system->createStream(Common_MediaPath("wave_vorbis.fsb"), FMOD_LOOP_NORMAL | FMOD_2D, 0, &sound);
		ERRCHECK(result);

		result = sound->getNumSubSounds(&numsubsounds);
		ERRCHECK(result);

		if (numsubsounds)
		{
			sound->getSubSound(0, &sound_to_play);
			ERRCHECK(result);
		}
		else
		{
			sound_to_play = sound;
		}

		/*
			Play the sound.
		*/
		result = system->playSound(sound_to_play, 0, false, &channel);
		ERRCHECK(result);

		/*
			Main loop.
		*/
		do
		{
			Common_Update();

			if (Common_BtnPress(BTN_ACTION1))
			{
				bool paused;
				result = channel->getPaused(&paused);
				ERRCHECK(result);
				result = channel->setPaused(!paused);
				ERRCHECK(result);
			}

			result = system->update();
			ERRCHECK(result);

			{
				unsigned int ms = 0;
				unsigned int lenms = 0;
				bool         playing = false;
				bool         paused = false;

				if (channel)
				{
					result = channel->isPlaying(&playing);
					if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE))
					{
						ERRCHECK(result);
					}

					result = channel->getPaused(&paused);
					if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE))
					{
						ERRCHECK(result);
					}

					result = channel->getPosition(&ms, FMOD_TIMEUNIT_MS);
					if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE))
					{
						ERRCHECK(result);
					}

					result = sound_to_play->getLength(&lenms, FMOD_TIMEUNIT_MS);
					if ((result != FMOD_OK) && (result != FMOD_ERR_INVALID_HANDLE))
					{
						ERRCHECK(result);
					}
				}

				Common_Draw("==================================================");
				Common_Draw("Play Stream Example.");
				Common_Draw("Copyright (c) Firelight Technologies 2004-2015.");
				Common_Draw("==================================================");
				Common_Draw("");
				Common_Draw("Press %s to toggle pause", Common_BtnStr(BTN_ACTION1));
				Common_Draw("Press %s to quit", Common_BtnStr(BTN_QUIT));
				Common_Draw("");
				Common_Draw("Time %02d:%02d:%02d/%02d:%02d:%02d : %s", ms / 1000 / 60, ms / 1000 % 60, ms / 10 % 100, lenms / 1000 / 60, lenms / 1000 % 60, lenms / 10 % 100, paused ? "Paused " : playing ? "Playing" : "Stopped");
			}

			Common_Sleep(50);
		} while (!Common_BtnPress(BTN_QUIT));

		/*
			Shut down
		*/
		result = sound->release();  /* Release the parent, not the sound that was retrieved with getSubSound. */
		ERRCHECK(result);
		result = system->close();
		ERRCHECK(result);
		result = system->release();
		ERRCHECK(result);

		Common_Close();

		return 0;
	}
}
