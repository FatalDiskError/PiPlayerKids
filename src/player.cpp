#define XML_OUT_OUTPUT
#define XML_LOG_OUTPUT
#define USE_DEFAULT_DEVICE

#include <iostream>
#include <string>

//#include <Signal.h> // https://github.com/pbhogan/Signals
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
		
		//int deviceId = getDeviceId();
		//bool init = BASS_Init(deviceId, DEFAULT_FREQUENCY, 0, 0, NULL);
		
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		
		int deviceId = -1, numDevices, numActiveDevices = 0;
		BASS_DEVICEINFO deviceInfo;
		for (numDevices = 0; BASS_GetDeviceInfo(numDevices, &deviceInfo); numDevices++)
		{
			if (deviceInfo.flags & BASS_DEVICE_DEFAULT)
				deviceId = numDevices;
				
			if (deviceInfo.flags & BASS_DEVICE_ENABLED)
				numActiveDevices++;
			
			#ifdef XML_OUT_OUTPUT
				_outStream << numDevices;
				_outStream << " [" << ((deviceInfo.flags & BASS_DEVICE_DEFAULT) ? "x" : " ") << "]";
				_outStream << " [" << ((deviceInfo.flags & BASS_DEVICE_ENABLED) ? "act" : "   ") << "]";
				_outStream << " Device: " << deviceInfo.name;
				(*_pLinkToConsole)->printOut(&_outStream);
			#endif
		}
		
		#ifdef XML_OUT_OUTPUT
			_outStream << "Active devices: " << numActiveDevices << " of " << numDevices;
			(*_pLinkToConsole)->printOut(&_outStream);
		#endif
		
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		
		bool init = BASS_Init(deviceId, DEFAULT_FREQUENCY, 0, 0, NULL);
		#ifdef XML_LOG_OUTPUT
			_logStream << "BASS_Init [" << deviceId << "]";
			_logStream << " " << (init ? "true" : "false");
			_logStream << " [error: " << BASS_ErrorGetCode() << "]";
			(*_pLinkToConsole)->printLog(&_logStream);
		#endif
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
			
			_outStream << "please select your deviceId (0-" << (deviceCount-1) << "): ";
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
		_logStream << "fileName [" << fileName << "] ";
		_logStream << "timestamp [" << timestamp << "] ";
		_logStream << "streamHandle [" << streamHandle << "]";
		(*_pLinkToConsole)->printLog(&_logStream);
		
		/*
		if(streamHandle != 0){
			BASS_ChannelStop(streamHandle);
			_logStream << "BASS_ChannelStop [" << streamHandle << "]";
			(*_pLinkToConsole)->printLog(&_logStream);
		}
		
		string path = "/home/pi/projects/PiPlayerKids/bin/library/";
		fileName.insert(0, path);
		
		streamHandle = BASS_StreamCreateFile(false, fileName.c_str(), 0, 0, BASS_STREAM_AUTOFREE);
		#ifdef XML_LOG_OUTPUT
			_logStream << "BASS_StreamCreateFile [" << fileName << "]";
			_logStream << " " << streamHandle;
			_logStream << " [error: " << BASS_ErrorGetCode() << "]";
			(*_pLinkToConsole)->printLog(&_logStream);
		#endif
		
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
