#include <map>
#include <string>
//#include <sstream>

namespace player {
	class Player
	{
		private:
			Console** _pLinkToConsole;
			//ostringstream outStream;
			//ostringstream logStream;

		public:
			Player(Console**);
			~Player(void);
			void start(void);
			void help(bool);
			void scan(void);
			void reset(void);
	};
}
