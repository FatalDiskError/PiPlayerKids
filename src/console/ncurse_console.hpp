#ifndef NCURSE_CONSOLE_HPP_INCLUDED
#define NCURSE_CONSOLE_HPP_INCLUDED

//#include <sstream>
#include <curses.h>
#include "console.hpp"

using namespace std;

namespace console
{
	enum ConsoleLayout
	{
		HORIZONTAL_LAYOUT, VERTICAL_LAYOUT, NO_LOG_LAYOUT
	};

	class NcurseConsole : public Console
	{
		public:
			NcurseConsole(ConsoleLayout);
			~NcurseConsole(void);
			void printOut(string, int = 0);
			void printOut(ostringstream*, int = 0);
			void printLog(string);
			void printLog(ostringstream*);
			int waitForChar(void);
			
		private:
			const int X_OFFSET = 2;
			const int Y_OFFSET = 2;
			const char BORDER_CHAR = '#';
			const char EMPTY_CHAR = ' ';
			
			ConsoleLayout layout;
			WINDOW *window;
			int dimensionX, dimensionY;
			int xOut, yOut, xOutMin, yOutMin, xOutMax, yOutMax, outLineLength, yOutPrev;
			int xLog, yLog, xLogMin, yLogMin, xLogMax, yLogMax, logLineLength;
			
			void drawDot(char, int, int);
			void drawHLine(char, int);
			void drawVLine(char, int);
			void drawBorder(char);
			void clearOutLine(void);
			void clearLogLine(void);
	};
}
#endif
