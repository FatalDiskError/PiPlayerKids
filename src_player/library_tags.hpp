#include <string>
namespace library {
	class LibraryTags
	{
		public:
			static const string TAG_LIBRARY_OPEN = "<library>"
			static const string TAG_LIBRARY_CLOSE = "</library>"
			static const string TAG_SERIES_OPEN = "<series>"
			static const string TAG_SERIES_CLOSE = "</series>"
			static const string TAG_EPISODE_OPEN = "<episode>"
			static const string TAG_EPISODE_CLOSE = "</episode>"
			static const string TAG_FILES_OPEN = "<files>"
			static const string TAG_FILES_CLOSE = "</files>"
			static const string TAG_FILE_OPEN = "<file>"
			static const string TAG_FILE_CLOSE = "</file>"
			static const string TAG_TITLE_OPEN = "<title>"
			static const string TAG_TITLE_CLOSE = "</title>"
			static const string TAG_RFID_OPEN = "<rfid>"
			static const string TAG_RFID_CLOSE = "</rfid>"
	};
}
