#include "rfid.hpp"
#include "console/console.hpp"

using namespace std;
using namespace console;

namespace rfid
{
	Rfid::Rfid(Console** ppConsole=NULL)
	{
		_pLinkToConsole=ppConsole;
	}
	
	Rfid::~Rfid(void)
	{
		delete _pLinkToConsole;
	}
}
