#include <map>
#include <string>
#include <sstream>

namespace kidsplayer {
	enum PlayerParams
	{
		HELP, SCAN, RESET
	};

	struct ParamsMap : map<PlayerParams, string>
	{
		ParamsMap()
		{
			this->operator[](HELP) = "-h";
			this->operator[](SCAN) = "-scan";
			this->operator[](RESET) = "-reset";
		};
		~ParamsMap(){};
	};
	static ParamsMap playerParamsMap;
	
	class KidsPlayer
	{
		private:
			Console* _pConsole;
			Mp3lib* _pLibrary;
			ostringstream outStream;
			ostringstream logStream;

		public:
			KidsPlayer(void);
			~KidsPlayer(void);
			void start(void);
			void help(bool);
			void scan(void);
			void reset(void);
	};
}
