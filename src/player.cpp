#include "console.hpp"
#include "player.hpp"

using namespace std;
using namespace console;

namespace player
{
	Player::Player(Console** ppConsole=NULL)
	{
		_pLinkToConsole=ppConsole;
	}
	
	Player::~Player(void)
	{
		delete _pLinkToConsole;
	}
}
