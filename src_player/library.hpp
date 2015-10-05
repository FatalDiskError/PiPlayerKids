#include <fstream>
#include <vector>
#include <string>

#include <console.hpp>

using namespace std;
using namespace console;

namespace library {
	class Library
	{
		private:
			const string LIBRARY_FILE_NAME = "./library/library.xml"

			Console** _pLinkToConsole;
			File* _pCurrentFile;
			Episode* _pCurrentEpisode;
			vector<Series*> _pSeries;

			fstream openLibraryFile(void);
			void closeLibraryFile(fstream);
			void parseLibraryFile(void);
			void seekSeries(fstream, long);
			void seekEpisodes(fstream, long);
			void seekFiles(fstream, long);
		public:
			typedef struct
			{
				string name;
				int timestamp;
			} File;
			typedef struct
			{
				string name;
				string rfidSerialNumber;
				vector<string> files;
			} Episode;
			typedef struct
			{
				string name;
				vector<Episode> episodes;
			} Series;
			
			Library(Console**);
			~Library(void);
			File* getFile(string);
			File* getNextFile();
			void writeCurrentFile(string, string);
			void writeCurrentTime(string, string);
	};
}
