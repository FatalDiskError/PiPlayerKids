#include "console.hpp"
#include "rfid.hpp"

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
