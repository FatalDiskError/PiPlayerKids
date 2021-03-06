#ifndef RFID_HPP_INCLUDED
#define RFID_HPP_INCLUDED

#include <iostream>
#include <iomanip>
#include <string>
#include <functional>

#include <unistd.h>
#include <sys/wait.h>

#include <sigc++/sigc++.h>

#include "rfid.h"
#include "rc522.h"
#include "bcm2835.h"
#include "console/console.hpp"

using namespace std;
using namespace console;
using namespace sigc;

namespace rfid
{
	class Rfid
	{
		public:
			enum class RfidStatusCode
			{
				NONE, NORMAL_END, SHUTDOWN_END
			};

			Rfid(Console**);
			~Rfid(void);

			RfidStatusCode listen(void);
			string listenOnce(void);

			/*
			 * libsigc++ slots
			 */
			sigc::slot<void, RfidStatusCode> rfidStatusSlot;

			/*
			 * libsigc++ signals
			 */
			sigc::signal<void, string> rfidSignal;

		private:
			Console** _pLinkToConsole;
			ostringstream _outStream;
			ostringstream _logStream;
			RfidStatusCode _statusCode;

			void endRfid(RfidStatusCode);

			void init_rfid(void);
			void release_rfid(void);
	};
}
#endif
