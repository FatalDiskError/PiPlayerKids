#include <curses.h>

using namespace std;

namespace console {
	enum ConsoleLayout
	{
		HORIZONTAL_LAYOUT, VERTICAL_LAYOUT, NO_LOG_LAYOUT
	};

	class Console
	{
		private:
			const int X_OFFSET = 2;
			const int Y_OFFSET = 2;
			const char BORDER_CHAR = '#';
			const char EMPTY_CHAR = ' ';
			const string OUT_PREFIX = "> ";
			const string LOG_PREFIX = "> ";
			
			ConsoleLayout layout;
			WINDOW *window;
			int dimensionX, dimensionY;
			int xOut, yOut, xOutMin, yOutMin, xOutMax, yOutMax, outLineLength;
			int xLog, yLog, xLogMin, yLogMin, xLogMax, yLogMax, logLineLength;
			
			void drawDot(char, int, int);
			void drawHLine(char, int);
			void drawVLine(char, int);
			void drawBorder(char);
			void clearOutLine(void);
			void clearLogLine(void);
		public:
			Console(ConsoleLayout);
			~Console(void);
			void printOut(string);
			void printLog(string);
	};
}
