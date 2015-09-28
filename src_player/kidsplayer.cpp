//#include <iostream>
#include "console.hpp"
#include "kidsplayer.hpp"

using namespace std;
using namespace player;
using namespace console;

int main(int argc, char **argv)
{
	KidsPlayer* _pKidsPlayer = new KidsPlayer();

	if(argc==1)
	{
		_pKidsPlayer->start();
	}
	else if(argc==2)
	{
		string param = string(argv[1]);
		
		if(param == playerParamsMap[HELP])
			_pKidsPlayer->help();
		else if(param == playerParamsMap[SCAN])
			_pKidsPlayer->scan();
		else if(param == playerParamsMap[RESET])
			_pKidsPlayer->reset();
	}
	else
	{
		_pKidsPlayer->help(true);
		delete _pKidsPlayer;
		return -1;
	}
	
	delete _pKidsPlayer;
	return 0;
}

namespace kidsplayer
{
	KidsPlayer::KidsPlayer(void)
	{
		_pConsole = new Console(NO_LOG_LAYOUT);
		_pLibrary = new Mp3lib("./library/");
	}
	
	KidsPlayer::~KidsPlayer(void)
	{
		delete _pConsole;
	}
	
	void KidsPlayer::start(void)
	{
		
	}

	void KidsPlayer::help(bool isArgumentWrong=false)
	{
		if(isArgumentWrong)
		{
			_pConsole->printOut("Wrong number of arguments. Use \"pi_player_kids -h\" for details");
		}
		_pConsole->printOut("pi_player_kids [-h|-scan|-reset]");
		_pConsole->printOut("   -h       This help.");
		_pConsole->printOut("   -scan    Scan for new files. Reorder files for episodes. Assign RFID codes to episodes.");
		_pConsole->printOut("   -reset   Delete current library. WARNING: all RFID codes will be deleted!");
	}

	void KidsPlayer::scan(void)
	{
	}

	void KidsPlayer::reset(void)
	{
		_pConsole->printOut("Do you really want to reset the library? This will delete all RFID codes! No mp3/cover files will be deleted)");
		_pConsole->printOut("[y/n]");
		_pLibrary->resetLibrary();
	}
}
