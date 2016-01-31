#ifndef GLOBAL_EXIT_HPP_INCLUDED
#define GLOBAL_EXIT_HPP_INCLUDED

#include <string>
#include <iterator>

namespace global_exit {
	class Error
	{
		public:
			Error(int errorCode, string errorText)
			{
				_errorCode = errorCode;
				_errorText = errorText;
			}
			
			int getErrorCode(void)
			{
				return _errorCode;
			}
			string getErrorText(void)
			{
				return _errorText;
			}
			
		private:
			int _errorCode;
			string _errorText;
	};
	
	enum ErrorCode
	{
		EXIT_OK,
		EXIT_ERROR,
		EXIT_ERROR_UNKNOWN,
		EXIT_ERROR_FILESYSTEM,
		EXIT_ERROR_DIRECTORY_NOT_FOUND,
		EXIT_ERROR_FILE_NOT_FOUND,
		EXIT_ERROR_INDEX_OUT_OF_BOUNDS
	};
	struct ErrorMap : map<ErrorCode, Error*>
	{
		ErrorMap()
		{
			this->operator[](EXIT_OK) =							new Error(0, "EXIT_OK");
			this->operator[](EXIT_ERROR) =						new Error(-1, "EXIT_ERROR");
			this->operator[](EXIT_ERROR_UNKNOWN) =				new Error(-2, "EXIT_ERROR_UNKNOWN");
			this->operator[](EXIT_ERROR_FILESYSTEM) =			new Error(-3, "EXIT_ERROR_FILESYSTEM");
			this->operator[](EXIT_ERROR_DIRECTORY_NOT_FOUND) =	new Error(-4, "EXIT_ERROR_DIRECTORY_NOT_FOUND");
			this->operator[](EXIT_ERROR_FILE_NOT_FOUND) =		new Error(-5, "EXIT_ERROR_FILE_NOT_FOUND");
			this->operator[](EXIT_ERROR_INDEX_OUT_OF_BOUNDS) =	new Error(-6, "EXIT_ERROR_INDEX_OUT_OF_BOUNDS");
		};
		~ErrorMap(){};
	};
	static ErrorMap errorMap;
	
	class GlobalExit
	{
		public:
			static void exit(ErrorCode errorCode, string errorText="")
			{
				//map<ErrorCode, Error>::iterator it;
				ErrorMap::iterator it = errorMap.find(errorCode);
				
				if(it == errorMap.end())
				{
					cerr << "Unexpected exit with unknown error (" << errorCode << "): " << errorText << endl;
					std::exit(-1000);
				}
				else
				{
					Error* pErr = it->second;
					cerr << "Exit (" << pErr->getErrorCode() << " | " << pErr->getErrorText() << ") ";
					if(errorText.length() > 0)
						cerr << errorText;
					cerr << endl;
					
					std::exit(pErr->getErrorCode());
				}
			}
		private:
	};
}

#endif
