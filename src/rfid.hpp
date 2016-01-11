#ifndef RFID_HPP_INCLUDED
#define RFID_HPP_INCLUDED

#include <iostream>
#include <iomanip>
#include <string>

#include <unistd.h>
#include <sys/wait.h>

#include <sigc++/sigc++.h>

#include "rfid.h"
#include "rc522.h"
#include "bcm2835.h"
#include "console/console.hpp"

using namespace std;
using namespace console;
//using namespace sigc;

namespace rfid
{
	class Rfid
	{
		public:
			Rfid(Console**);
			~Rfid(void);
			
			void listen(void);
			
			/*
			 * libsigc++ signals
			 */
			sigc::signal<void, string> rfidSignal;
			
		private:
			Console** _pLinkToConsole;
			ostringstream _outStream;
			ostringstream _logStream;
			
			void init_rfid(void);
			void release_rfid(void);
	};
}
#endif
