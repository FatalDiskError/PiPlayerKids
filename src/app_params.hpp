#ifndef MAIN_HPP_INCLUDED
#define MAIN_HPP_INCLUDED

#include <map>
#include <string>

using namespace std;

namespace app_params
{
	enum class Params
	{
		HELP, SCAN
	};
	struct ParamsMap : map<Params, string>
	{
		ParamsMap()
		{
			this->operator[](Params::HELP) =	"-h";
			this->operator[](Params::SCAN) =	"-scan";
		};
		~ParamsMap(){};
	};
	static ParamsMap paramsMap;
	
	enum class Operations
	{
		APPEND, FULL, RFID
	};
	struct OperationsMap : map<Operations, string>
	{
		OperationsMap()
		{
			this->operator[](Operations::APPEND) =	"-append";
			this->operator[](Operations::FULL) =	"-full";
			this->operator[](Operations::RFID) =	"-rfid";
		};
		~OperationsMap(){};
	};
	static OperationsMap operationsMap;
}
#endif
