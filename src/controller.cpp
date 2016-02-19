#include "controller.hpp"

using namespace std;
using namespace console;
using namespace rfid;
using namespace library;

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
	}
	Controller::~Controller(void)
	{
		(*_pLinkToConsole)->printLog("destructing library");
	}

	void Controller::execute(string serial)
	{
		if(serial == _controllerCodeMap[ControllerCode::PLAY_PAUSE])
		{
			(*_pLinkToConsole)->printLog("### PLAY_PAUSE ###");
			navigateSignal(Library::Navigation::PLAY_PAUSE);
		}
		else if(serial == _controllerCodeMap[ControllerCode::NEXT])
		{
			(*_pLinkToConsole)->printLog("### NEXT ###");
			navigateSignal(Library::Navigation::NEXT);
		}
		else if(serial == _controllerCodeMap[ControllerCode::BACK])
		{
			(*_pLinkToConsole)->printLog("### BACK ###");
			navigateSignal(Library::Navigation::BACK);
		}
		else if(serial == _controllerCodeMap[ControllerCode::RESET])
		{
			(*_pLinkToConsole)->printLog("### RESET ###");
			navigateSignal(Library::Navigation::RESET);
		}
		else if(serial == _controllerCodeMap[ControllerCode::QUIT])
		{
			(*_pLinkToConsole)->printLog("### QUIT ###");
			rfidStatusSignal(Rfid::RfidStatusCode::NORMAL_END);
			/*
			exit(1);
			 */
		}
		else if(serial == _controllerCodeMap[ControllerCode::SHUTDOWN])
		{
			rfidStatusSignal(Rfid::RfidStatusCode::SHUTDOWN_END);
			/*
			(*_pLinkToConsole)->printLog("### SHUTDOWN ###");
			system("sudo shutdown -h -P now");
			exit(2);
			 */
		}
		else
		{
			episodeSignal(serial);
		}
	}
}
