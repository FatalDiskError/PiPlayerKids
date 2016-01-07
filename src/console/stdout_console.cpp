//#include <iostream>
//#include <string>
//#include <sstream>
//#include "console.hpp"
#include "stdout_console.hpp"

using namespace std;

namespace console
{
	StdOutConsole::StdOutConsole() : Console()
	{
		printLog("constructing console");
	}

	StdOutConsole::~StdOutConsole(void)
	{
		printOut("destructing console. press any key to continue");
	}
	
	void StdOutConsole::printOut(string out, int lineOffset)
	{
		cout << OUT_PREFIX << "OUT: " << out << endl;
	}
	
	void StdOutConsole::printOut(ostringstream* out, int lineOffset)
	{
		printOut(out->str(), lineOffset);
		out->str("");
		out->clear();
	}
	
	void StdOutConsole::printLog(string log)
	{
		cout << LOG_PREFIX << "LOG: " << log << endl;
	}
	
	void StdOutConsole::printLog(ostringstream* log)
	{
		printLog(log->str());
		log->str("");
		log->clear();
	}
	
	int StdOutConsole::waitForChar(void)
	{
		string userInput;
		getline(cin, userInput);
		
		int charCode = 0;
		if(userInput.length() > 0){
			charCode = userInput.at(0);
		}
		return charCode;
	}
}
