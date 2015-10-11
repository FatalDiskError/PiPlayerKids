#ifndef RFID_HPP_INCLUDED
#define RFID_HPP_INCLUDED

#include <map>
#include <string>
//#include <sstream>

using namespace std;
using namespace console;

namespace rfid
{
	class Rfid
	{
		public:
			Rfid(Console**);
			~Rfid(void);
			
		private:
			Console** _pLinkToConsole;
			//ostringstream outStream;
			//ostringstream logStream;
	};
}

#endif
