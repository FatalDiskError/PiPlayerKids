#ifndef LIBRARY_TAGS_HPP_INCLUDED
#define LIBRARY_TAGS_HPP_INCLUDED

namespace library {
	class LibraryTags
	{
		public:
			static constexpr const char* TAG_LIBRARY = "library";
			static constexpr const char* TAG_SERIES = "series";
			static constexpr const char* TAG_EPISODE = "episode";
			static constexpr const char* TAG_FILES = "files";
			static constexpr const char* TAG_FILE = "file";
			static constexpr const char* TAG_TITLE = "title";
			static constexpr const char* TAG_COVER = "cover";
			
			static constexpr const char* ATTRIBUTE_RFID = "rfid";
			static constexpr const char* ATTRIBUTE_CURRENT_FILE = "current_file";
			static constexpr const char* ATTRIBUTE_TIMESTAMP = "timestamp";
	};
}

#endif
