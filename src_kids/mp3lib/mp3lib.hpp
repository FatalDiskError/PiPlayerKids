#define SILENT_LIB_CORE
#define SILENT_LIB_INFO
#define SILENT_SERIES_CORE
#define SILENT_SERIES_INFO
#define SILENT_EPISODE_CORE
#define xSILENT_EPISODE_INFO
#define SILENT_FILEBROWSER
#ifndef MP3LIB
#define MP3LIB
#include <string>
#include <map>
#include <boost/filesystem.hpp>

using namespace std;
using namespace boost::filesystem;

namespace mp3lib {
	enum FileExtensions
	{
		CURRENT_PATH, PREV_PATH, MP3, JPG, COVER, DEFAULT_COVER
	};

	struct EnumMap : map<FileExtensions, string>
	{
		EnumMap()
		{
			this->operator[](CURRENT_PATH) = ".";
			this->operator[](PREV_PATH) = "..";
			this->operator[](MP3) = ".mp3";
			this->operator[](JPG) = ".jpg";
			this->operator[](COVER) = "cover.jpg";
			this->operator[](DEFAULT_COVER) = "no_cover.jpg";
		};
		~EnumMap(){};
	};
	static EnumMap fileExtensionMap;



	class Mp3Lib
	{
		private:
			class FileBrowser
			{
				private:
					FileBrowser(void);
					~FileBrowser(void);
				public:
					static path getPath(string dir);
					static vector<path> getSubDirectories(path p);
					static vector<path> getFiles(path p, FileExtensions fileExtension);
					static bool conditional_check(path p, FileExtensions fileExtension);
			};

			class Episode
			{
				private:
					path _coverFile;
					path _path;
					string _hash;
					vector<path> _mp3Files;
					vector<path>::iterator _mp3FilesIterator;
					path* _pMp3File;
				public:
					Episode(path p);
					~Episode(void);
					string getEpisodeName(void);

					// mp3
					void showMp3(void);
					void nextMp3(void);
					void previousMp3(void);
					void playMp3(void);
					void pauseMp3(void);
			};

			class Series
				{
					private:
						path _coverFile;
						path _path;
						vector<Episode*> _episodes;
						vector<Episode*>::iterator _episodesIterator;
						Episode* _pEpisode;
					public:
						Series(path p);
						~Series(void);
						string getSeriesName(void);

						// episodes
						void showEpisode(void);
						void nextEpisode(void);
						void previousEpisode(void);
						void enterEpisode(void);
						void exitEpisode(void);
						// mp3
						void showMp3(void);
						void nextMp3(void);
						void previousMp3(void);
						void playMp3(void);
						void pauseMp3(void);
				};

			path _path;
			vector<Series*> _series;
			vector<Series*>::iterator _seriesIterator;
			Series* _pSeries;
		public:
			Mp3Lib(string dir);
			~Mp3Lib(void);
			// series
			void showSeries(void);
			void nextSeries(void);
			void previousSeries(void);
			void enterSeries(void);
			void exitSeries(void);
			// episodes
			void showEpisode(void);
			void nextEpisode(void);
			void previousEpisode(void);
			void enterEpisode(void);
			void exitEpisode(void);
			// mp3
			void showMp3(void);
			void nextMp3(void);
			void previousMp3(void);
			void playMp3(void);
			void pauseMp3(void);
	};
}
#endif
