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
			enum class ControllerCode
			{
				PLAY_PAUSE, NEXT, BACK, RESET, QUIT, SHUTDOWN
			};

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
			struct ControllerCodeMap : map<ControllerCode, string>
			{
				ControllerCodeMap()
				{
					this->operator[](ControllerCode::PLAY_PAUSE) = "3d15d424";
					this->operator[](ControllerCode::NEXT) = "3da74c24";
					this->operator[](ControllerCode::BACK) = "2fca1021";
					this->operator[](ControllerCode::RESET) = "c45b133b";
					this->operator[](ControllerCode::QUIT) = "fb20f60b";
					this->operator[](ControllerCode::SHUTDOWN) = "db9a8e0b";
				};
				~ControllerCodeMap(){};
			};
			ControllerCodeMap _controllerCodeMap;

			Console** _pLinkToConsole;
			ostringstream _outStream;
			ostringstream _logStream;

			void execute(string);
	};
}
#endif
