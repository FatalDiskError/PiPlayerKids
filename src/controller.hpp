#ifndef CONTROLLER_HPP_INCLUDED
#define CONTROLLER_HPP_INCLUDED

#include <sigc++/sigc++.h>

#include "console/console.hpp"
#include "library.hpp"

using namespace std;
using namespace console;
using namespace library;
using namespace sigc;

namespace controller
{
	class Controller
	{
		public:
			Controller(Console**);
			~Controller(void);
			
			/*
			 * libsigc++ slots
			 */
			slot<void, string> rfidSlot;
			/*
			 * libsigc++ signals
			 */
			signal<void, Library::Navigation> navigateSignal;
			signal<void, string> episodeSignal;
			
		private:
			Console** _pLinkToConsole;
			ostringstream _outStream;
			ostringstream _logStream;
			
			void execute(string);
	};
}
#endif
