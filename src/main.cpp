#include <iostream>
#include <cstring>

#include "console.hpp"
#include "library.hpp"
#include "player.hpp"
#include "rfid.hpp"

using namespace std;
using namespace console;
using namespace library;
using namespace player;
using namespace rfid;

void start(char*);
void printHelp(bool = false);
void resetLibrary(char*);
void scanLibrary(void);

enum Params
{
	HELP, RESET, SCAN
};
struct ParamsMap : map<Params, string>
{
	ParamsMap()
	{
		this->operator[](HELP) = "-h";
		this->operator[](RESET) = "-reset";
		this->operator[](SCAN) = "-scan";
	};
	~ParamsMap(){};
};
static ParamsMap paramsMap;

int main(int argc, char **argv)
{
	char* pEndChar = strrchr(argv[0], '/');
	int endPos = pEndChar-argv[0]+1;
	char workingDir[endPos];
	strncpy(workingDir, argv[0], endPos);

	if(argc==1)
	{
		start(workingDir);
	}
	else if(argc==2)
	{
		string param = string(argv[1]);
		
		if(param == paramsMap[HELP])
			printHelp();
		else if(param == paramsMap[RESET])
			resetLibrary(workingDir);
		else if(param == paramsMap[SCAN])
			scanLibrary();
	}
	else
	{
		printHelp(true);
		return -1;
	}
	
	return 0;
}

void start(char* pWorkingDir)
{
	Console* _pConsole = new Console(VERTICAL_LAYOUT);
	Library* _pLibrary = new Library(pWorkingDir, &_pConsole);
	Player* _pPlayer = new Player(&_pConsole);
	Rfid* _pRfid = new Rfid(&_pConsole);

	//delete _pRfid;
	//delete _pPlayer;
	//delete _pLibrary;
	int inputChar = _pConsole->waitForChar();
	_pConsole->printLog(to_string(inputChar));
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

void resetLibrary(char* pWorkingDir)
{
}

void scanLibrary(void)
{
}
