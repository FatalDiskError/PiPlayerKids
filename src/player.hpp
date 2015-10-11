#ifndef PLAYER_HPP_INCLUDED
#define PLAYER_HPP_INCLUDED

#include <map>
#include <string>
//#include <sstream>

using namespace std;
using namespace console;

namespace player
{
	class Player
	{
		private:
			Console** _pLinkToConsole;
			//ostringstream outStream;
			//ostringstream logStream;

		public:
			Player(Console**);
			~Player(void);
	};
}

#endif
