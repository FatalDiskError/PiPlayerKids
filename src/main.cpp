#include <iostream>
#include <cstring>
#include <string>

#include "console.hpp"
#include "library.hpp"
#include "player.hpp"
#include "rfid.hpp"

using namespace std;
using namespace console;
using namespace library;
using namespace player;
using namespace rfid;

void start(string);
void printHelp(bool = false);
void resetLibrary(string);
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
	string applicationPath = argv[0];
	
	if(argc==1)
	{
		start(applicationPath);
	}
	else if(argc==2)
	{
		string param = argv[1];
		
		if(param == paramsMap[HELP])
			printHelp();
		else if(param == paramsMap[RESET])
			resetLibrary(applicationPath);
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

void start(string applicationPath)
{
	Console* _pConsole = new Console(VERTICAL_LAYOUT);
	Library* _pLibrary = new Library(applicationPath, &_pConsole);
	
	Player* _pPlayer = new Player(&_pConsole);
	Rfid* _pRfid = new Rfid(&_pConsole);
	
	_pLibrary->logOutRfidMap();
	
	_pLibrary->getEpisode("x11111111");
	_pLibrary->getEpisode("x12121212");
	_pLibrary->getEpisode("x13131313");
	_pLibrary->getEpisode("xxxxxxxxx");
	_pLibrary->getEpisode("x21212121");
	_pLibrary->getEpisode("x22222222");
	
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
