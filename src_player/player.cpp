#include "console.hpp"
#include "kidsplayer.hpp"

using namespace std;
using namespace player;
using namespace console;

namespace player
{
	KidsPlayer::KidsPlayer(Console** ppConsole=NULL)
	{
		_pLinkToConsole=ppConsole;
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
