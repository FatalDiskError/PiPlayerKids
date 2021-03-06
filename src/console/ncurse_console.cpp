#include <iostream>
#include <string>
#include <sstream>
//#include <curses.h>
//#include "console.hpp"
#include "ncurse_console.hpp"

using namespace std;

namespace console
{
	NcurseConsole::NcurseConsole(ConsoleLayout consoleLayout) : Console()
	{
		layout = consoleLayout;
		
		window = initscr();							// init screen
		cbreak();									// get keys without <ENTER>
		noecho();									// don't echo keystrokes
		getmaxyx(window, dimensionY, dimensionX);	// get max rows and columns
		clear();									// clear screen
		refresh();									// update screen
		
		int middle;
		if(layout==HORIZONTAL_LAYOUT){
			middle = dimensionY / 2;
			drawHLine(BORDER_CHAR, middle);
			
			xOutMin = X_OFFSET;
			xOutMax = dimensionX - X_OFFSET;
			yOutMin = Y_OFFSET;
			yOutMax = middle - Y_OFFSET;
			
			xLogMin = X_OFFSET;
			xLogMax = dimensionX - X_OFFSET;
			yLogMin = middle + Y_OFFSET;
			yLogMax = dimensionY - 1 - Y_OFFSET;
		}else if(layout==VERTICAL_LAYOUT){
			middle = dimensionX / 2;
			drawVLine(BORDER_CHAR, middle);

			xOutMin = X_OFFSET;
			xOutMax = middle + 1 - X_OFFSET;
			yOutMin = Y_OFFSET;
			yOutMax = dimensionY - Y_OFFSET;

			xLogMin = middle + X_OFFSET;
			xLogMax = dimensionX - X_OFFSET;
			yLogMin = Y_OFFSET;
			yLogMax = dimensionY - 1 - Y_OFFSET;
		}else{
			xOutMin = X_OFFSET;
			xOutMax = dimensionX - X_OFFSET;
			yOutMin = Y_OFFSET;
			yOutMax = dimensionY - Y_OFFSET;
			
			xLogMin = 0;
			xLogMax = 0;
			yLogMin = 0;
			yLogMax = 0;
		}
		
		xOut = xOutMin;
		yOut = yOutMin;
		yOutPrev = yOut;
		outLineLength = xOutMax - xOutMin - OUT_PREFIX.length();
		
		xLog = xLogMin;
		yLog = yLogMin;
		logLineLength = xLogMax - xLogMin - LOG_PREFIX.length();
		
		drawBorder(BORDER_CHAR);
		move(yOut, xOut);
		
		printLog("constructing console");
	}

	NcurseConsole::~NcurseConsole(void)
	{
		printOut("destructing console. press any key to continue");
		getch();

		clear();
		refresh();
		echo();
		nocbreak();
		endwin();
	}

	void NcurseConsole::drawDot(char cr, int y, int x)
	{
		move(y, x);
		addch(cr);
	}

	void NcurseConsole::drawHLine(char cr, int y)
	{
		for(int x=0; x<dimensionX; ++x)
			drawDot(cr, y, x);
	}

	void NcurseConsole::drawVLine(char cr, int x)
	{
		for(int y=0; y<dimensionY; ++y)
			drawDot(cr, y, x);
	}

	void NcurseConsole::drawBorder(char cr)
	{
		drawHLine(cr, 0);
		drawHLine(cr, dimensionY-1);
		
		drawVLine(cr, 0);
		drawVLine(cr, dimensionX-1);
	}

	void NcurseConsole::clearOutLine(void)
	{
		for(int x=xOutMin; x<xOutMax; ++x)
			drawDot(EMPTY_CHAR, yOut, x);
	}

	void NcurseConsole::clearLogLine(void)
	{
		for(int x=xLogMin; x<xLogMax; ++x)
			drawDot(EMPTY_CHAR, yLog, x);
	}

	void NcurseConsole::printOut(string out, int lineOffset)
	{
		string line;
		int pos = 0;
		yOut += lineOffset;
		if(yOut > yOutMax)
		{
			yOut = yOutMin + (yOut - yOutMax);
		}
		else if(yOut < yOutMin)
		{
			yOut = yOutMax + (yOut - yOutMin);
		}
		
		while(pos<out.length()){
			clearOutLine();
			if(pos==0){
				move(yOut, xOutMin);
				printw(OUT_PREFIX.c_str());
			}
			move(yOut, xOutMin + OUT_PREFIX.length());
			line = out.substr(pos, outLineLength);
			printw(line.c_str());
			pos += outLineLength;
			
			++yOut;
			if(yOut > yOutMax)
				yOut = yOutMin;
		}
		refresh();
	}
	
	void NcurseConsole::printOut(ostringstream* out, int lineOffset)
	{
		printOut(out->str(), lineOffset);
		out->str("");
		out->clear();
	}
	
	void NcurseConsole::printLog(string log)
	{
		int outY, outX;
		getyx(window, outY, outX);
		
		if(layout==NO_LOG_LAYOUT){
			return;
		}
		
		string line;
		int pos = 0;
		
		while(pos<log.length()){
			clearLogLine();
			if(pos==0){
				move(yLog, xLogMin);
				printw(LOG_PREFIX.c_str());
			}
			move(yLog, xLogMin + LOG_PREFIX.length());
			line = log.substr(pos, logLineLength);
			printw(line.c_str());
			pos += logLineLength;
			
			++yLog;
			if(yLog > yLogMax)
				yLog = yLogMin;
		}
		move(outY, outX);
		refresh();
	}
	
	void NcurseConsole::printLog(ostringstream* log)
	{
		printLog(log->str());
		log->str("");
		log->clear();
	}
	
	int NcurseConsole::waitForChar(void)
	{
		return getch();
	}
}
