SHELL = ./report_time.sh

all: start

clear:
	rm obj/*.o
	clear

##########################
### CUSTOM DEFINITIONS ###
##########################
CONSOLE_STDOUT = stdout
CONSOLE_NCURSE = ncurse
CONSOLE_TYPE = $(CONSOLE_STDOUT)

XML_OUT_OUTPUT = true
XML_LOG_OUTPUT = true


####################
### CONSOLE LIBS ###
####################
ifeq ($(CONSOLE_TYPE), $(CONSOLE_STDOUT))
NCURSES_LIBS =
else
ifeq ($(CONSOLE_TYPE), $(CONSOLE_NCURSE))
NCURSES_LIBS = -lncurses
else
	$(error no console lib defined)
endif
endif

######################
### COMPILER FLAGS ###
######################
COMPILER_FLAGS = -std=c++11 -march=armv6 -mfloat-abi=hard -mfpu=vfp -DCONSOLE_TYPE=$(CONSOLE_TYPE)

ifeq ($(XML_OUT_OUTPUT), true)
COMPILER_FLAGS += -DXML_OUT_OUTPUT
endif
ifeq ($(XML_LOG_OUTPUT), true)
COMPILER_FLAGS += -DXML_LOG_OUTPUT
endif

#################
### LIBRARIES ###
#################
BOOST_PATH = /usr/include/boost
BOOST_INCLUDE_PATH = -I$(BOOST_PATH)/filesystem
# -I$(BOOST_PATH)/signals2
BOOST_LIBRARY_PATH = -L/usr/lib/
BOOST_LIBS = -lboost_filesystem
#-lboost_locale -lboost_system

LIBEV_PATH = /usr/include/libev
LIBEV_INCLUDE_PATH = -I$(LIBEV_PATH)
LIBEV_LIBRARY_PATH = -L$(LIBEV_PATH)
#LIBEV_LIBS = -llibev

#PBHOGAN_SIGNALS_PATH = ../libraries/pbhogan_signals
#PBHOGAN_SIGNALS_INCLUDE_PATH = -I$(PBHOGAN_SIGNALS_PATH)
#PBHOGAN_SIGNALS_LIBRARY_PATH = -L$(PBHOGAN_SIGNALS_PATH)
#PBHOGAN_SIGNALS_LIBS = 

BASS_PATH = ../libraries/bass_arm
BASS_INCLUDE_PATH = -I$(BASS_PATH)
BASS_LIBRARY_PATH = -L$(BASS_PATH) -L$(BASS_PATH)/hardfp
BASS_LIBS = -lbass -lm
BASS_LINKER_FLAGS = -Wl,-rpath,$(BASS_PATH):$(BASS_PATH)/hardfp

RFID_PATH = ../libraries/bcm2835-1.45/src
RFID_INCLUDE_PATH = -I$(RFID_PATH)
RFID_LIBRARY_PATH = -L$(RFID_PATH)
RFID_LIBS = -lbcm2835

RAPIDXML_PATH = ../libraries/rapidxml-1.13
RAPIDXML_INCLUDE_PATH = -I$(RAPIDXML_PATH)
RAPIDXML_LIBRARY_PATH = -L$(RAPIDXML_PATH)
#RAPIDXML_LIBS = 

#LIBSIG_PATH = /usr/include/sigc++-2.0
#-I$(LIBSIG_PATH)
LIBSIG_INCLUDE_PATH = `pkg-config sigc++-2.0 --cflags`
LIBSIG_LIBRARY_PATH = `pkg-config sigc++-2.0 --libs`
#LIBSIG_LIBS = 


#############################
### COMPILER INCLUDE PATH ###
#############################
PLAYER_INCLUDE_PATH = $(BOOST_INCLUDE_PATH) $(BASS_INCLUDE_PATH) $(RFID_INCLUDE_PATH) $(RAPIDXML_INCLUDE_PATH)
# $(PBHOGAN_SIGNALS_INCLUDE_PATH)
# $(LIBSIG_INCLUDE_PATH)
# $(LIBEV_INCLUDE_PATH)

#############################
### COMPILER LIBRARY PATH ###
#############################
PLAYER_LIBRARY_PATH = $(BOOST_LIBRARY_PATH) $(BASS_LIBRARY_PATH) $(RFID_LIBRARY_PATH) $(RAPIDXML_LIBRARY_PATH)
# $(PBHOGAN_SIGNALS_LIBRARY_PATH)
# $(LIBSIG_LIBRARY_PATH)
# $(LIBEV_LIBRARY_PATH)

########################
### LINKER LIBRARIES ###
########################
PLAYER_LIBS = $(BOOST_LIBS) $(BASS_LIBS) $(RFID_LIBS) $(NCURSES_LIBS)

########################
### LINKER LIBRARIES ###
########################
PLAYER_LINKER_FLAGS = $(BASS_LINKER_FLAGS)


#################
### COMPILING ###
#################
PLAYER_FILES = main library player rfid
PLAYER_OBJECTS = $(PLAYER_FILES:%=obj/%.o)

start: $(PLAYER_FILES) console linking

%: src/%.cpp
	g++ $(COMPILER_FLAGS) -g -c $< -o obj/$@.o $(PLAYER_INCLUDE_PATH)

console:
	g++ $(COMPILER_FLAGS) -g -c src/console/$(CONSOLE_TYPE)_console.cpp -o obj/console.o $(PLAYER_INCLUDE_PATH)
#	g++ $(COMPILER_FLAGS) -g -c src/console/ncurse_console.cpp -o obj/ncurse_console.o $(PLAYER_INCLUDE_PATH)
#	g++ $(COMPILER_FLAGS) -g -c src/console/stdout_console.cpp -o obj/stdout_console.o $(PLAYER_INCLUDE_PATH)

linking:
	g++ $(COMPILER_FLAGS) -g $(PLAYER_OBJECTS) obj/console.o -o bin/player $(PLAYER_LIBRARY_PATH) $(PLAYER_LIBS) $(PLAYER_LINKER_FLAGS)
# $(BASS_INCLUDE_PATH)


############
### HELP ###
############
# -g - turn on debugging (so GDB gives more friendly output)
# -Wall - turns on most warnings
# -O or -O2 - turn on optimizations
# -o <name> - name of the output file
# -c - output an object file (.o)
# -I<include path> - specify an include directory (.h / .hpp)
# -L<library path> - specify a lib directory (lib___.so / lib___.a)
# -l<library> - link with library lib<library>.a
# -Wl,aaa,bbb,ccc - passes a comma-separated list of tokens as a space-separated list of arguments to the linker (becomes: ld aaa bbb ccc)
