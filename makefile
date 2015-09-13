all: rfid

clear:
	rm *.o
	clear

# ++++++++++++++++
# +++   KIDS   +++
# ++++++++++++++++

KIDS_INCLUDE_PATH = -Ic:/boost/boost_1_58_0/include/boost-1_58 -Ic:/PROGRA~2/FMODSO~1/FMODST~1/api/lowlevel/inc
KIDS_LIBRARY_PATH = -Lc:/boost/boost_1_58_0/lib/ -Lc:/PROGRA~2/FMODSO~1/FMODST~1/api/lowlevel/lib/
KIDS_LIBS = -lboost_filesystem-mgw49-mt-1_58 -lboost_locale-mgw49-mt-1_58 -lboost_system-mgw49-mt-1_58 -lfmod64

MP3LIB = mp3lib_mp3lib mp3lib_series mp3lib_episode mp3lib_filebrowser
MP3LIB_OBJ = $(MP3LIB:%=%.o)
MP3LIB_TARGET = $(MP3LIB:%=kids_%)

kids: kids_main kids_sha256 $(MP3LIB)
	g++ -std=c++11 kids_main.o sha256.o $(MP3LIB_OBJ) -o bin/kids $(KIDS_INCLUDE_PATH) $(KIDS_LIBRARY_PATH) $(KIDS_LIBS)

kids_main: src_kids/kids_main.cpp
	g++ -std=c++11 -c src_kids/kids_main.cpp $(KIDS_INCLUDE_PATH)

kids_sha256: src_kids/sha256/sha256.cpp src_kids/sha256/sha256.hpp
	g++ -std=c++11 -c src_kids/sha256/sha256.cpp

%: src_kids/mp3lib/%.cpp src_kids/mp3lib/mp3lib.hpp
	g++ -std=c++11 -c $< $(KIDS_INCLUDE_PATH)

# ++++++++++++++++
# +++   FMOD   +++
# ++++++++++++++++

FMOD_INCLUDE_PATH = -I../libraries/fmod/api/lowlevel/inc
FMOD_LIBRARY_PATH = -L../libraries/fmod/api/lowlevel/lib/arm/
FMOD_LIBS = -lfmod

fmod: fmod_common_platform fmod_common fmod_main
	g++ -std=c++11 play_stream.o common_platform.o common.o -o bin/fmod $(FMOD_INCLUDE_PATH) $(FMOD_LIBRARY_PATH) $(FMOD_LIBS)

fmod_main: src_fmod/play_stream.cpp
	g++ -std=c++11 -c src_fmod/play_stream.cpp $(FMOD_INCLUDE_PATH)

fmod_common: src_fmod/common.cpp
	g++ -std=c++11 -c src_fmod/common.cpp $(FMOD_INCLUDE_PATH)

fmod_common_platform: src_fmod/common_platform.cpp
	g++ -std=c++11 -c src_fmod/common_platform.cpp $(FMOD_INCLUDE_PATH)

# ++++++++++++++++
# +++   BASS   +++
# ++++++++++++++++

BASSPATH = ../libraries/bass_arm
BASSPATH := $(shell cd $(BASSPATH); pwd)
BASS_INCLUDE_PATH = -I$(BASSPATH)
BASS_LIBRARY_PATH = -L$(BASSPATH) -L$(BASSPATH)/hardfp
#BASS_INCLUDE_PATH = -I../libraries/bass_arm
#BASS_LIBRARY_PATH = -L../libraries/bass_arm
BASS_LIBS = -lbass -lm -lncurses
BASS_FLAGS = -Wl,-rpath,$(BASSPATH):$(BASSPATH)/hardfp

bass: bass_main
	g++ -std=c++11 bass_main.o -o bin/bass $(BASS_INCLUDE_PATH) $(BASS_LIBRARY_PATH) $(BASS_LIBS) $(BASS_FLAGS)

bass_main: src_bass/bass_main.cpp
	g++ -std=c++11 -c src_bass/bass_main.cpp $(BASS_INCLUDE_PATH)

# ++++++++++++++++
# +++   RFID   +++
# ++++++++++++++++
RFID_INCLUDE_PATH = -Ibcm2835/bcm2835-1.45/src
RFID_LIBRARY_PATH = -Lbcm2835/bcm2835-1.45/src
RFID_LIBS = -lbcm2835

rfid: rfid_main
	g++ -std=c++11 rfid_main.o -o bin/rfid $(RFID_INCLUDE_PATH) $(RFID_LIBRARY_PATH) $(RFID_LIBS)

rfid_main: src_rfid/rfid_main.cpp
	g++ -std=c++11 -c src_rfid/rfid_main.cpp $(RFID_INCLUDE_PATH)

rfid_bcm2835: bcm2835/bcm2835-1.45/src/bcm2835.c bcm2835/bcm2835-1.45/src/bcm2835.h
	g++ -std=c++11 -c bcm2835/bcm2835-1.45/src/bcm2835.c

# -g - turn on debugging (so GDB gives more friendly output)
# -Wall - turns on most warnings
# -O or -O2 - turn on optimizations
# -o <name> - name of the output file
# -c - output an object file (.o)
# -I<include path> - specify an include directory
# -L<library path> - specify a lib directory (header)
# -l<library> - link with library lib<library>.a
