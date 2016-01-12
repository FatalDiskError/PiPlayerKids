#include "controller.hpp"

using namespace std;
using namespace console;
using namespace library;
using namespace sigc;

namespace controller {
	/***************
	 * CONSTRUCTOR *
	 ***************/
	Controller::Controller(Console** ppConsole=NULL)
	{
		_pLinkToConsole = ppConsole;
		(*_pLinkToConsole)->printLog("constructing library");
		
		/*
		 * libsigc++ slots
		 */
		rfidSlot = mem_fun(this, &Controller::execute);
		
		/*
		 * libsigc++ signals
		playSignal.connect(_pPlayer->playFileSlot);
		playPauseSignal.connect(_pPlayer->playPauseSlot);
		 */
	}
	Controller::~Controller(void)
	{
		(*_pLinkToConsole)->printLog("destructing library");
	}

	void Controller::execute(string serial)
	{
		if(serial == "3d15d424")
		{
			(*_pLinkToConsole)->printLog("### PLAY_PAUSE ###");
			navigateSignal(Library::Navigation::PLAY_PAUSE);
		}
		else if(serial == "3da74c24")
		{
			(*_pLinkToConsole)->printLog("### NEXT ###");
			navigateSignal(Library::Navigation::NEXT);
		}
		else if(serial == "2fca1021")
		{
			(*_pLinkToConsole)->printLog("### BACK ###");
			navigateSignal(Library::Navigation::BACK);
		}
		else if(serial == "c45b133b")
		{
			(*_pLinkToConsole)->printLog("### RESET ###");
			navigateSignal(Library::Navigation::RESET);
		}
		else if(serial == "db9a8e0b")
		{
			(*_pLinkToConsole)->printLog("### shutdown ###");
			system("sudo shutdown -h -P now");
		}
		else
		{
			episodeSignal(serial);
		}
	}
}
