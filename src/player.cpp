#define USE_DEFAULT_DEVICE

#include "player.hpp"

using namespace std;
using namespace console;

namespace player {
	void CALLBACK endOfFileCallback(HSYNC, DWORD, DWORD, void*);
	/***************
	 * CONSTRUCTOR *
	 ***************/
	Player::Player(Console** ppConsole=NULL)
	{
		_pLinkToConsole=ppConsole;
		(*_pLinkToConsole)->printLog("constructing player");

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

	void Player::playFile(string fileName, double timestamp)
	{
		_logStream << "fileName [" << fileName << "] ";
		_logStream << "timestamp [" << timestamp << "] ";
		_logStream << "_streamHandle [" << _streamHandle << "]";
		(*_pLinkToConsole)->printLog(&_logStream);

		if(_streamHandle != 0){
			BASS_ChannelStop(_streamHandle);
			_logStream << "BASS_ChannelStop [" << _streamHandle << "]";
			(*_pLinkToConsole)->printLog(&_logStream);

			BASS_StreamFree(_streamHandle);
			_logStream << "BASS_StreamFree [" << _streamHandle << "]";
			(*_pLinkToConsole)->printLog(&_logStream);
		}

		string path = "/home/pi/projects/PiPlayerKids/bin/library/";
		fileName.insert(0, path);

		_streamHandle = BASS_StreamCreateFile(false, fileName.c_str(), 0, 0, BASS_STREAM_PRESCAN | BASS_STREAM_AUTOFREE);
		#ifdef XML_LOG_OUTPUT
			_logStream << "BASS_StreamCreateFile [" << fileName << "]";
			_logStream << " " << _streamHandle;
			_logStream << " [error: " << BASS_ErrorGetCode() << "]";
			(*_pLinkToConsole)->printLog(&_logStream);
			/*
			 * BASS_StreamCreateFile ERROR CODES
			 *
			BASS_ERROR_INIT			BASS_Init has not been successfully called.
			BASS_ERROR_NOTAVAIL		Only decoding channels (BASS_STREAM_DECODE) are allowed when using the "no sound" device. The BASS_STREAM_AUTOFREE flag is also unavailable to decoding channels.
			BASS_ERROR_ILLPARAM		The length must be specified when streaming from memory.
			BASS_ERROR_FILEOPEN		The file could not be opened.
			BASS_ERROR_FILEFORM		The file's format is not recognised/supported.
			BASS_ERROR_CODEC		The file uses a codec that is not available/supported. This can apply to WAV and AIFF files, and also MP3 files when using the "MP3-free" BASS version.
			BASS_ERROR_FORMAT		The sample format is not supported by the device/drivers. If the stream is more than stereo or the BASS_SAMPLE_FLOAT flag is used, it could be that they are not supported.
			BASS_ERROR_SPEAKER		The specified SPEAKER flags are invalid. The device/drivers do not support them, they are attempting to assign a stereo stream to a mono speaker or 3D functionality is enabled.
			BASS_ERROR_MEM			There is insufficient memory.
			BASS_ERROR_NO3D			Could not initialize 3D support.
			BASS_ERROR_UNKNOWN		Some other mystery problem!
			 */
		#endif

		BASS_ChannelSetSync(_streamHandle, BASS_SYNC_END, 0, endOfFileCallback, this);
		bool play = BASS_ChannelPlay(_streamHandle, true); // 2nd param: bool restart

		QWORD position_byte = BASS_ChannelSeconds2Bytes(_streamHandle, timestamp-5);
		bool seek = BASS_ChannelSetPosition(_streamHandle, position_byte, BASS_POS_BYTE);
		#ifdef XML_LOG_OUTPUT
			_logStream << "BASS_ChannelPlay [" << _streamHandle << "]";
			_logStream << " " << (play ? "true" : "false");
			_logStream << " [error: " << BASS_ErrorGetCode() << "]";
			(*_pLinkToConsole)->printLog(&_logStream);
			/*
			 * BASS_ChannelPlay ERROR CODES
			 *
			BASS_ERROR_HANDLE	handle is not a valid channel.
			BASS_ERROR_START	The output is paused/stopped, use BASS_Start to start it.
			BASS_ERROR_DECODE	The channel is not playable; it is a "decoding channel".
			BASS_ERROR_BUFLOST	Should not happen... check that a valid window handle was used with BASS_Init.
			BASS_ERROR_NOHW		No hardware voices are available (HCHANNEL only). This only occurs if the sample was loaded/created with the BASS_SAMPLE_VAM flag and BASS_VAM_HARDWARE is set in the sample's VAM mode, and there are no hardware voices available to play it.
			 */
		#endif
	}

	void Player::endOfFile()
	{
		completedSignal();
	}

	void CALLBACK endOfFileCallback(HSYNC handle, DWORD channel, DWORD data, void* pTarget)
	{
		Player* pPlayer = static_cast<Player*>(pTarget);
		pPlayer->endOfFile();
	}

	double Player::playPauseFile(void)
	{
		if(_streamHandle != 0)
		{
			DWORD active = BASS_ChannelIsActive(_streamHandle);
			#ifdef XML_LOG_OUTPUT
				_logStream << "BASS_ChannelIsActive [" << _streamHandle << "]";
				_logStream << " active: " << active;
				_logStream << " [error: " << BASS_ErrorGetCode() << "]";
				(*_pLinkToConsole)->printLog(&_logStream);
				/*
				 * BASS_ChannelIsActive RETURN VALUES
				 *
				BASS_ACTIVE_STOPPED		The channel is not active, or handle is not a valid channel.
				BASS_ACTIVE_PLAYING		The channel is playing (or recording).
				BASS_ACTIVE_PAUSED		The channel is paused.
				BASS_ACTIVE_STALLED		Playback of the stream has been stalled due to a lack of sample data. The playback will automatically resume once there is sufficient data to do so.
				 */
			#endif

			if(active == BASS_ACTIVE_PLAYING)
			{
				bool paused = BASS_ChannelPause(_streamHandle);
				#ifdef XML_LOG_OUTPUT
					_logStream << "BASS_ChannelPause [" << _streamHandle << "]";
					_logStream << " " << (paused ? "true" : "false");
					_logStream << " [error: " << BASS_ErrorGetCode() << "]";
					(*_pLinkToConsole)->printLog(&_logStream);
					/*
					 * BASS_ChannelPause ERROR CODES
					 *
					BASS_ERROR_NOPLAY	The channel is not playing (or handle is not a valid channel).
					BASS_ERROR_DECODE	The channel is not playable; it is a "decoding channel".
					BASS_ERROR_ALREADY	The channel is already paused.
					 */
				#endif

				QWORD position_byte = BASS_ChannelGetPosition(_streamHandle, BASS_POS_BYTE);
				double position_time = BASS_ChannelBytes2Seconds(_streamHandle, position_byte);
				return position_time;
			}
			else if(active == BASS_ACTIVE_STOPPED || active == BASS_ACTIVE_PAUSED)
			{
				bool play = BASS_ChannelPlay(_streamHandle, false);
				#ifdef XML_LOG_OUTPUT
					_logStream << "BASS_ChannelPlay [" << _streamHandle << "]";
					_logStream << " " << (play ? "true" : "false");
					_logStream << " [error: " << BASS_ErrorGetCode() << "]";
					(*_pLinkToConsole)->printLog(&_logStream);
					/*
					 * BASS_ChannelPlay ERROR CODES
					 *
					BASS_ERROR_HANDLE	handle is not a valid channel.
					BASS_ERROR_START	The output is paused/stopped, use BASS_Start to start it.
					BASS_ERROR_DECODE	The channel is not playable; it is a "decoding channel".
					BASS_ERROR_BUFLOST	Should not happen... check that a valid window handle was used with BASS_Init.
					BASS_ERROR_NOHW		No hardware voices are available (HCHANNEL only). This only occurs if the sample was loaded/created with the BASS_SAMPLE_VAM flag and BASS_VAM_HARDWARE is set in the sample's VAM mode, and there are no hardware voices available to play it.
					 */
				#endif
				return -1;
			}
		}
		return -1;
	}

	double Player::stopFile(void)
	{
		if(_streamHandle != 0)
		{
			DWORD active = BASS_ChannelIsActive(_streamHandle);
			#ifdef XML_LOG_OUTPUT
				_logStream << "BASS_ChannelIsActive [" << _streamHandle << "]";
				_logStream << " active: " << active;
				_logStream << " [error: " << BASS_ErrorGetCode() << "]";
				(*_pLinkToConsole)->printLog(&_logStream);
				/*
				 * BASS_ChannelIsActive RETURN VALUES
				 *
				BASS_ACTIVE_STOPPED		The channel is not active, or handle is not a valid channel.
				BASS_ACTIVE_PLAYING		The channel is playing (or recording).
				BASS_ACTIVE_PAUSED		The channel is paused.
				BASS_ACTIVE_STALLED		Playback of the stream has been stalled due to a lack of sample data. The playback will automatically resume once there is sufficient data to do so.
				 */
			#endif

			if(active == BASS_ACTIVE_PLAYING)
			{
				bool paused = BASS_ChannelPause(_streamHandle);
				#ifdef XML_LOG_OUTPUT
					_logStream << "BASS_ChannelPause [" << _streamHandle << "]";
					_logStream << " " << (paused ? "true" : "false");
					_logStream << " [error: " << BASS_ErrorGetCode() << "]";
					(*_pLinkToConsole)->printLog(&_logStream);
					/*
					 * BASS_ChannelPause ERROR CODES
					 *
					BASS_ERROR_NOPLAY	The channel is not playing (or handle is not a valid channel).
					BASS_ERROR_DECODE	The channel is not playable; it is a "decoding channel".
					BASS_ERROR_ALREADY	The channel is already paused.
					 */
				#endif

				QWORD position_byte = BASS_ChannelGetPosition(_streamHandle, BASS_POS_BYTE);
				double position_time = BASS_ChannelBytes2Seconds(_streamHandle, position_byte);
				return position_time;
			}
		}
		return -1;
	}
}
