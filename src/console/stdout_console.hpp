#ifndef STDOUT_CONSOLE_HPP_INCLUDED
#define STDOUT_CONSOLE_HPP_INCLUDED

//#include <sstream>
#include "console.hpp"

using namespace std;

namespace console
{
	class StdOutConsole : public Console
	{
		public:
			StdOutConsole();
			~StdOutConsole(void);
			void printOut(string, int = 0);
			void printOut(ostringstream*, int = 0);
			void printLog(string);
			void printLog(ostringstream*);
			int waitForChar(void);
			
		private:
			const string OUT_PREFIX = "> ";
			const string LOG_PREFIX = "> ";
	};
}
#endif
