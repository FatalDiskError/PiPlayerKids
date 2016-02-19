#include <typeinfo>
#include <iostream>
#include <sstream>
#include <cstring>
#include <string>

#include <sigc++/sigc++.h>
#include <boost/filesystem.hpp>

#if CONSOLE_TYPE == stdout
	#include "console/stdout_console.hpp"
	#define TEST "STD"
#elif CONSOLE_TYPE == ncurse
	#include "console/ncurse_console.hpp"
	#define TEST "NCURSES"
#endif

#include "app_params.hpp"
#include "controller.hpp"
#include "library.hpp"
#include "library_builder.hpp"
#include "rfid.hpp"

using namespace std;
using namespace app_params;
using namespace console;
using namespace controller;
using namespace library;
using namespace rfid;
using namespace sigc;
using namespace boost::filesystem;

void start(path);
void scanLibrary(path, Operations);
void printHelp(bool = false);

ostringstream _outStream;
ostringstream _logStream;

int main(int argc, char **argv)
{
	path applicationPath = path(argv[0]).parent_path();

	if(argc==1)
	{
		start(applicationPath);
	}
	else if(argc==2)
	{
		string param = argv[1];

		if(param == paramsMap[Params::HELP])
		{
			printHelp();
		}
		else if(param == paramsMap[Params::SCAN])
		{
			scanLibrary(applicationPath, Operations::RFID);
		}
		else
		{
			printHelp(true);
			return -1;
		}
	}
	else if(argc==3)
	{
		string param = argv[1];
		string operation = argv[2];

		if(param == paramsMap[Params::SCAN])
		{
			if(operation == operationsMap[Operations::APPEND])
				scanLibrary(applicationPath, Operations::APPEND);
			else if(operation == operationsMap[Operations::FULL])
				scanLibrary(applicationPath, Operations::FULL);
			else if(operation == operationsMap[Operations::RFID])
				scanLibrary(applicationPath, Operations::RFID);
			else
			{
				printHelp(true);
				return -1;
			}
		}
		else
		{
			printHelp(true);
			return -1;
		}
	}
	else
	{
		printHelp(true);
		return -1;
	}

	return 0;
}

void start(path applicationPath)
{
	Console* _pConsole;
	#if CONSOLE_TYPE == stdout
		_pConsole = new StdOutConsole();
	#elif CONSOLE_TYPE == ncurse
		_pConsole = new NcurseConsole(VERTICAL_LAYOUT);
	#endif
	_logStream << "CONSOLE_TYPE: " << CONSOLE_TYPE;
	_pConsole->printLog(&_logStream);

	Controller* _pController = new Controller(&_pConsole);
	Library* _pLibrary = new Library(applicationPath, &_pConsole);
	Rfid* _pRfid = new Rfid(&_pConsole);

	_pController->episodeSignal.connect(_pLibrary->episodeSlot);
	_pController->navigateSignal.connect(_pLibrary->navigateSlot);
	_pController->rfidStatusSignal.connect(_pRfid->rfidStatusSlot);
	_pRfid->rfidSignal.connect(_pController->rfidSlot);

	Rfid::RfidStatusCode statusCode = _pRfid->listen();

	delete _pController;
	delete _pLibrary;
	delete _pRfid;

	if(statusCode == Rfid::RfidStatusCode::NONE)
	{
		_pConsole->printLog("### END BY PROGRAMM ###");
		delete _pConsole;
	}
	else if(statusCode == Rfid::RfidStatusCode::NORMAL_END)
	{
		_pConsole->printLog("### END BY USER ###");
		delete _pConsole;
		exit(0);
	}
	else if(statusCode == Rfid::RfidStatusCode::SHUTDOWN_END)
	{
		_pConsole->printLog("### END BY USER ###");
		_pConsole->printLog("### SHUTDOWN ###");
		delete _pConsole;
		system("sudo shutdown -h -P now");
		exit(2);
	}
}

void scanLibrary(path applicationPath, Operations operation)
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
	LibraryBuilder* _pLibraryBuilder = new LibraryBuilder(applicationPath, &_pConsole);

	_pLibraryBuilder->buildLibraryFile(operation, _pRfid);

	delete _pLibraryBuilder;
	delete _pRfid;
	delete _pConsole;
}

void printHelp(bool isArgumentWrong)
{
	if(isArgumentWrong)
	{
		cout << "Wrong number of arguments. Use \"pi_player_kids -h\" for details" << endl;
	}
	cout << "pi_player_kids [-h|-scan [-new|-full|-rfid]]" << endl;
	cout << "   -h        This help." << endl;
	cout << "   -scan     Scan for files. Default behaviour is [-scan -new]." << endl;
	cout << "             Current library-file is backuped before each scan." << endl;
	cout << "      -append   Scan only for new series/episodes and append in current library." << endl;
	cout << "      -full     Rescan all files. Directories within library-path represents series, sub-directories episodes." << endl;
	cout << "      -rfid     Assign RFID-codes to episodes." << endl;
}
