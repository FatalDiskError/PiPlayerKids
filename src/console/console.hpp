#ifndef CONSOLE_HPP_INCLUDED
#define CONSOLE_HPP_INCLUDED

#include <iostream>
#include <string>
#include <sstream>

using namespace std;

namespace console
{
	class Console
	{
		public:
			Console() {};
			~Console(void) {};
			virtual void printOut(string, int = 0) =0;
			virtual void printOut(ostringstream*, int = 0) =0;
			virtual void printLog(string) =0;
			virtual void printLog(ostringstream*) =0;
			virtual int waitForChar(void) =0;
			
		private:
			
		protected:
			const string OUT_PREFIX = "> ";
			const string LOG_PREFIX = "> ";
	};
}
#endif
