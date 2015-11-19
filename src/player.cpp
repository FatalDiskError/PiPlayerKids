#define XML_OUT_OUTPUT
#define XML_LOG_OUTPUT
#define USE_DEFAULT_DEVICE

#include <iostream>
#include <string>

#include <Signal.h> // https://github.com/pbhogan/Signals
#include <bass.h>

#include "player.hpp"
#include "console.hpp"

using namespace std;
using namespace console;

namespace player {
	/***************
	 * CONSTRUCTOR *
	 ***************/
	Player::Player(Console** ppConsole=NULL)
	{
		_pLinkToConsole=ppConsole;
		(*_pLinkToConsole)->printLog("constructing player");
		
		int deviceId = getDeviceId();
		bool init = BASS_Init(deviceId, DEFAULT_FREQUENCY, 0, 0, NULL);
		#ifdef XML_LOG_OUTPUT
			_logStream << "BASS_Init [" << deviceId << "]";
			_logStream << " " << (init ? "true" : "false");
			_logStream << " [error: " << BASS_ErrorGetCode() << "]";
			(*_pLinkToConsole)->printLog(&_logStream);
		#endif
		
		/*
		float volume = 0.1;
		bool vol = BASS_SetVolume(volume);
		#ifdef XML_LOG_OUTPUT
			_logStream << "BASS_SetVolume [" << volume << "]";
			_logStream << " " << (vol ? "true" : "false");
			_logStream << " [error: " << BASS_ErrorGetCode() << "]";
			(*_pLinkToConsole)->printLog(&_logStream);
		#endif
		*/
	}
	
	Player::~Player(void)
	{
		(*_pLinkToConsole)->printLog("destructing player");
		BASS_Free();
	}
	
	int Player::getDeviceId(void)
	{
		return -1;
		#ifdef USE_DEFAULT_DEVICE
			return DEFAULT_DEVICE_ID;
		#else
			int deviceCount = 0;
			BASS_DEVICEINFO info;
			for (int deviceInfoId = 0; BASS_GetDeviceInfo(deviceInfoId, &info); deviceInfoId++)
			{
				_outStream << "BASS_GetDeviceInfo [" << deviceInfoId << "] is ";
				
				if (info.flags & BASS_DEVICE_ENABLED)
				{
					deviceCount++;
					_outStream << "enabled  [";
				}
				else
				{
					_outStream << "disabled [";
				}
				_outStream << info.name << "]";
				(*_pLinkToConsole)->printOut(&_outStream);
			}
			
			#ifdef XML_LOG_OUTPUT
				_logStream << "number of devices: " << deviceCount;
				(*_pLinkToConsole)->printLog(&_logStream);
			#endif
			
			_outStream << "please select your device (0-" << (deviceCount-1) << "): ";
			(*_pLinkToConsole)->printOut(&_outStream);
			
			int deviceId = DEFAULT_DEVICE_ID;
			while(deviceId < 0 || deviceId > deviceCount)
			{
				int charCode = _pConsole->waitForChar();
				deviceId = charCode - CHAR_CODE_0;
			}
			return deviceId;
		#endif
	}
	
	void Player::playFile(string fileName, int timestamp)
	{
		string path = "/home/pi/projects/PiPlayerKids/bin/library/";
		fileName.insert(0, path);
		
		#ifdef XML_LOG_OUTPUT
			_logStream << "playFile(" << fileName << ", " << timestamp << ")";
			_logStream << " [" << fileName.c_str() << "]";
			(*_pLinkToConsole)->printLog(&_logStream);
		#endif
		
		streamHandle = BASS_StreamCreateFile(FALSE, fileName.c_str(), 0, 0, 0);
		#ifdef XML_LOG_OUTPUT
			_logStream << "BASS_StreamCreateFile [" << fileName << "]";
			_logStream << " " << streamHandle;
			_logStream << " [error: " << BASS_ErrorGetCode() << "]";
			(*_pLinkToConsole)->printLog(&_logStream);
		#endif
		
		/*
		bool play = BASS_ChannelPlay(streamHandle, true);
		#ifdef XML_LOG_OUTPUT
			_logStream << "BASS_ChannelPlay [" << streamHandle << "]";
			_logStream << " " << (play ? "true" : "false");
			_logStream << " [error: " << BASS_ErrorGetCode() << "]";
			(*_pLinkToConsole)->printLog(&_logStream);
		#endif
		*/
	}
}
