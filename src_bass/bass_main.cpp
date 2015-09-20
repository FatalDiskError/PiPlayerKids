#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <curses.h>
#include <unistd.h>
#include "bass.h"

using namespace std;

int dimensionX, dimensionY;
int xMin, yMin, xMax, yMax;
int yLog, yLogMin, yLogMax;

void drawDot(char cr, int y, int x)
{
	move(y, x);
	addch(cr);
}

void drawHLine(char cr, int y)
{
	for(int x=0; x<dimensionX; ++x)
		drawDot(cr, y, x);
}

void drawVLine(char cr, int x)
{
	for(int y=0; y<dimensionY; ++y)
		drawDot(cr, y, x);
}

void drawBorder(char cr)
{
	drawHLine(cr, 0);
	drawHLine(cr, dimensionY-1);
	
	drawVLine(cr, 0);
	drawVLine(cr, dimensionX-1);
}

void printLog(string log)
{
	move(yLog, xMin);
	printw((" > " + log).c_str());
	++yLog;
	if(yLog > yLogMax)
		yLog = yLogMin;
}

int main(int argc, char **argv)
{
	int x, y;
	WINDOW *wnd = initscr();				// init screen
	cbreak();								// get keys without <ENTER>
	noecho();								// don't echo keystrokes
	getmaxyx(wnd, dimensionY, dimensionX);	// get max rows and columns
	clear();								// clear screen
	refresh();								// update screen
	
	xMin = 3;
	xMax = dimensionX - xMin;
	yMin = 2;
	yMax = dimensionY - yMin;

	drawBorder('#');

	x = xMin;
	y = yMin;
	
	int device = -1, numDevices, numActiveDevices = 0;
	BASS_DEVICEINFO deviceInfo;
	for (numDevices = 0; BASS_GetDeviceInfo(numDevices, &deviceInfo); numDevices++)
	{
		if (deviceInfo.flags & BASS_DEVICE_DEFAULT)
			device = numDevices;
			
		if (deviceInfo.flags & BASS_DEVICE_ENABLED)
			numActiveDevices++;
		
		move(y, x);
		printw("%i [%c] [%s] Device: %s",
			numDevices, 
			(deviceInfo.flags & BASS_DEVICE_DEFAULT) ? 'x' : ' ',
			(deviceInfo.flags & BASS_DEVICE_ENABLED) ? "act" : "   ", 
			deviceInfo.name
		);
		++y;
	}
	
	move(y, x);
	printw("Active devices: %i of %i", numActiveDevices, numDevices);
	
	++y;
	++y;
	drawHLine('#', y);
	++y;
	++y;
	move(y, x);
	printw("Log:");
	yLogMin = yLog = y+1;
	yLogMax = yMax;
	
	int frequency = 44100;
	float volume = 0.1;
	string file = "./bin/1.mp3";
	HSTREAM streamHandle;
	bool success;
	ostringstream log;
	
	/* Initialize output device */
	success = BASS_Init(device, frequency, 0, 0, NULL);
	log << "BASS_Init(" << device << ", " << frequency << ", 0, 0, NULL) ";
	log << (success ? "success" : "failed") << " with errorCode: " << BASS_ErrorGetCode();
	printLog(log.str());
	log.str("");
	log.clear();
	
	/*
	// does not work on raspi
	success = BASS_SetVolume(volume);
	log2 << "BASS_SetVolume(" << volume << ") ";
	log2 << (success ? "success" : "failed") << " with errorCode: " << BASS_ErrorGetCode();
	printLog(log2.str());
	*/
	
	streamHandle = BASS_StreamCreateFile(FALSE, file.c_str(), 0, 0, 0);
	log << "BASS_StreamCreateFile(FALSE, " << file << ", 0, 0, 0) ";
	log << "streamHandle: " << streamHandle << " with errorCode: " << BASS_ErrorGetCode();
	printLog(log.str());
	log.str("");
	log.clear();
	
	success = BASS_ChannelPlay(streamHandle, true);
	log << "BASS_ChannelPlay(" << streamHandle << ", true) ";
	log << (success ? "success" : "failed") << " with errorCode: " << BASS_ErrorGetCode();
	printLog(log.str());
	log.str("");
	log.clear();
	
	for(int i=0; i<10; i++){
		usleep(200000);
		log << "wait " << i;
		printLog(log.str());
		log.str("");
		log.clear();
	}

	/*
	"initscr is normally the first curses routine to call when initializing a
	program. The initscr code determines the terminal type and initializes all curses
	data structures. initscr also causes the first call to refresh to clear
	the screen."

	"The cbreak routine disables line buffering and
	erase/kill character-processing (interrupt and flow control characters
	are unaffected), making characters typed by the user immediately avail-
	able to the program."

	"The printw, wprintw, mvprintw and mvwprintw routines are analogous to
	printf [see printf(3S)]. In effect, the string that would be output by
	printf is output instead as though waddstr were used on the given window."

	"The getch, wgetch, mvgetch and mvwgetch, routines read a character from
	the window. In no-delay mode, if no input is waiting, the value ERR is
	returned. In delay mode, the program waits until the system passes text
	through to the program. Depending on the setting of cbreak, this is
	after one character (cbreak mode), or after the first newline (nocbreak
	mode). In half-delay mode, the program waits until a character is typed
	or the specified timeout has been reached."

	"A program should always call endwin before exiting or escaping from
	curses mode temporarily. This routine restores tty modes, moves the cur-
	sor to the lower left-hand corner of the screen and resets the terminal
	into the proper non-visual mode. "

	"A program using these routines must be linked with the -lncurses option,
	or (if it has been generated) with the debugging library -lncurses_g.
	(Your system integrator may also have installed these libraries under the
	names -lcurses and -lcurses_g.) The ncurses_g library generates trace
	logs (in a file called 'trace' in the current directory) that describe
	curses actions."	
	*/
	getch();
	
	BASS_Free();
	endwin();
	return 0;
}