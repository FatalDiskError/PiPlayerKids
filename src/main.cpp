#include <typeinfo>
#include <iostream>
#include <sstream>
#include <cstring>
#include <string>

#include <sigc++/sigc++.h>
//#include <Signal.h>

#if CONSOLE_TYPE == stdout
	#include "console/stdout_console.hpp"
	#define TEST "STD"
#elif CONSOLE_TYPE == ncurse
	#include "console/ncurse_console.hpp"
	#define TEST "NCURSES"
#endif

#include "library.hpp"
#include "rfid.hpp"

using namespace std;
using namespace console;
using namespace rfid;
using namespace library;
using namespace sigc;
//using namespace Gallant;

void start(string);
void printHelp(bool = false);
void resetLibrary(string);
void scanLibrary(void);

enum class Params
{
	HELP, RESET, SCAN
};
struct ParamsMap : map<Params, string>
{
	ParamsMap()
	{
		this->operator[](Params::HELP) = "-h";
		this->operator[](Params::RESET) = "-reset";
		this->operator[](Params::SCAN) = "-scan";
	};
	~ParamsMap(){};
};
static ParamsMap paramsMap;

ostringstream _outStream;
ostringstream _logStream;

int main(int argc, char **argv)
{
	string applicationPath = argv[0];
	
	if(argc==1)
	{
		start(applicationPath);
	}
	else if(argc==2)
	{
		string param = argv[1];
		
		if(param == paramsMap[Params::HELP])
			printHelp();
		else if(param == paramsMap[Params::RESET])
			resetLibrary(applicationPath);
		else if(param == paramsMap[Params::SCAN])
			scanLibrary();
	}
	else
	{
		printHelp(true);
		return -1;
	}
	
	return 0;
}

void start(string applicationPath)
{
	Console* _pConsole;
	
	#if CONSOLE_TYPE == stdout
		_pConsole = new StdOutConsole();
	#elif CONSOLE_TYPE == ncurse
		_pConsole = new NcurseConsole(VERTICAL_LAYOUT);
	#endif
	_logStream << "CONSOLE_TYPE: " << CONSOLE_TYPE;
	_pConsole->printLog(&_logStream);
	
	Rfid* _pRfid = new Rfid(&_pConsole);
	Library* _pLibrary = new Library(applicationPath, &_pConsole);
	_pLibrary->logOutRfidMap();
	
	/*
	 * libsigc++ with slots
	 * 
	 */
	signal<void, string> rfidSignal;
	rfidSignal.connect(_pLibrary->setEpisodeSlot);
	
	signal<void, Library::Navigation> navigationSignal;
	navigationSignal.connect(_pLibrary->navigateSlot);

	/*
	 * pbhogan_signals
	 * 
	Signal1<string> rfidSignal;
	rfidSignal.Connect(_pLibrary, &Library::setEpisode);
	
	Signal1<Library::Navigation> navigationSignal;
	navigationSignal.Connect(_pLibrary, &Library::navigate);
	 */
	
	bool isLoop = true;
	while(isLoop)
	{
		int charCode = _pConsole->waitForChar();
		#ifdef XML_LOG_OUTPUT
			_logStream << "charCode: " << charCode << " - char: " << (char)charCode;
			_pConsole->printLog(&_logStream);
		#endif
		
		switch((char)charCode)
		{
			case 'c': //99
			case 'q': //113
				isLoop = false;
				break;
			case 'p': //112
				navigationSignal(Library::Navigation::PLAY_PAUSE);
				break;
			case 'n': //110
				navigationSignal(Library::Navigation::NEXT);
				break;
			case 'p': //112
				navigationSignal(Library::Navigation::PREVIOUS);
				break;
			case 'r': //114
				navigationSignal(Library::Navigation::RESET);
				break;
			case '1': //49
				rfidSignal("x11111111");
				break;
			case '2': //50
				rfidSignal("x12121212");
				break;
			case '3': //51
				rfidSignal("x13131313");
				break;
			case '4': //52
				rfidSignal("xxxxxxxxx");
				break;
			case '5': //53
				rfidSignal("x21212121");
				break;
			case '6': //54
				rfidSignal("x22222222");
				break;
			/*
			case '7': //55
				_pPlayer->playFile("drei_fragezeichen_kids/01_gefaehrlich_raetsel/placeholder_0_drei_fragezeichen_kids.mp3", 0);
				break;
			case '8': //56
				_pPlayer->playFile("wieso_weshalb_warum/ritter/placeholder_0_wieso_weshalb_warum.mp3", 0);
				break;
			case '9': //57
				_pPlayer->playFile("benjamin_bluemchen/als_wetterelefant/placeholder_0_benjamin_bluemchen.mp3", 0);
				break;
			*/
			case '0': //58
				rfidSignal("x99999999");
				break;
		}
	}
	
	//delete _pRfid;
	//delete _pPlayer;
	delete _pLibrary;
	delete _pConsole;
}

void printHelp(bool isArgumentWrong)
{
	if(isArgumentWrong)
	{
		cout << "Wrong number of arguments. Use \"pi_player_kids -h\" for details" << endl;
	}
	cout << "pi_player_kids [-h|-scan|-reset]" << endl;
	cout << "   -h       This help." << endl;
	cout << "   -reset   Delete current library. WARNING: all RFID codes will be deleted!" << endl;
	cout << "   -scan    Scan for new files. Reorder files for episodes. Assign RFID codes to episodes." << endl;
}

void resetLibrary(string applicationPath)
{
}

void scanLibrary(void)
{
}
