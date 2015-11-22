#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <iostream>
#include <string>

//#include <Signal.h> // https://github.com/pbhogan/Signals
#include <bass.h>

#include "console.hpp"

using namespace std;
using namespace console;

namespace player {
	class Player
	{
		public:
			Player(Console**);
			~Player(void);
			void playFile(string, int);
		private:
			Console** _pLinkToConsole;
			ostringstream _outStream;
			ostringstream _logStream;
			
			const int DEFAULT_DEVICE_ID = -1;
			const int CHAR_CODE_0 = 48;
			const int DEFAULT_FREQUENCY = 44100;
			
			HSTREAM streamHandle;
			
			int getDeviceId(void);
	};
}

#endif
