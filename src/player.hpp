#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <iostream>
#include <string>

#include <sigc++/sigc++.h>
#include <bass.h>

#include "console/console.hpp"

using namespace std;
using namespace console;
using namespace sigc;

namespace player {
	class Player
	{
		public:
			Player(Console**);
			~Player(void);
			/*
			 * libsigc++ slots
			 */
			slot<void, string, int> playFileSlot;
			slot<int> playPauseSlot;
			/*
			 * libsigc++ signals
			 */
			signal<void> completedSignal;
			
			void endOfFile(void);
			
		private:
			Console** _pLinkToConsole;
			ostringstream _outStream;
			ostringstream _logStream;
			
			const int DEFAULT_DEVICE_ID = -1;
			const int CHAR_CODE_0 = 48;
			const int DEFAULT_FREQUENCY = 44100;
			
			HSTREAM _streamHandle;
			
			int getDeviceId(void);
			void playFile(string, int);
			int playPause(void);
	};
}
#endif
