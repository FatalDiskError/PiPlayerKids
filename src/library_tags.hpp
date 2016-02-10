#ifndef LIBRARY_TAGS_HPP_INCLUDED
#define LIBRARY_TAGS_HPP_INCLUDED

namespace library {
	class LibraryTags
	{
		public:
			static constexpr const char* TAG_DEFINITION = "<?xml version=\"1.0\" encoding=\"utf-8\"?>";
			static constexpr const char* TAG_BRACE_OPEN = "<";
			static constexpr const char* TAG_BRACE_CLOSE = ">";
			static constexpr const char* TAG_CLOSING = "/";
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
			static constexpr const char* DEFAULT_ATTRIBUTES_FILES = "rfid=\"\" current_file=\"1\" timestamp=\"0\"";
	};
}

#endif
