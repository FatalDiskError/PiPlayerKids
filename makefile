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

BASS_PATH = /usr/lib/bass_arm
BASS_INCLUDE_PATH = -I$(BASS_PATH)
BASS_LIBRARY_PATH = -L$(BASS_PATH) -L$(BASS_PATH)/hardfp
BASS_LIBS = -lbass -lm
BASS_LINKER_FLAGS = -Wl,-rpath,$(BASS_PATH):$(BASS_PATH)/hardfp

BCM2835_PATH = /usr/lib/bcm2835-1.45/src
BCM2835_INCLUDE_PATH = -I$(BCM2835_PATH)
BCM2835_LIBRARY_PATH = -L$(BCM2835_PATH)
BCM2835_LIBS = -lbcm2835

RFID_PATH = ../libraries/rfid
RFID_INCLUDE_PATH = -I$(RFID_PATH)
RFID_LIBRARY_PATH = -L$(RFID_PATH)
#RFID_LIBS =

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
COMPILER_INCLUDE_PATH = $(BOOST_INCLUDE_PATH) $(BASS_INCLUDE_PATH) $(BCM2835_INCLUDE_PATH) $(RFID_INCLUDE_PATH) $(RAPIDXML_INCLUDE_PATH) $(LIBSIG_INCLUDE_PATH)

###########################
### LINKER LIBRARY PATH ###
###########################
LINKER_LIBRARY_PATH = $(BOOST_LIBRARY_PATH) $(BASS_LIBRARY_PATH) $(BCM2835_LIBRARY_PATH) $(RFID_LIBRARY_PATH) $(RAPIDXML_LIBRARY_PATH) $(LIBSIG_LIBRARY_PATH)

########################
### LINKER LIBRARIES ###
########################
LINKER_LIBS = $(BOOST_LIBS) $(BASS_LIBS) $(BCM2835_LIBS) $(NCURSES_LIBS)

####################
### LINKER FLAGS ###
####################
LINKER_FLAGS = $(BASS_LINKER_FLAGS)


#################
### COMPILING ###
#################
PLAYER_FILES = main library player rfid
PLAYER_OBJECTS = $(PLAYER_FILES:%=obj/%.o)

start: $(PLAYER_FILES) console rfidlib linking

%: src/%.cpp
	g++ $(COMPILER_FLAGS) -g -c $< -o obj/$@.o $(COMPILER_INCLUDE_PATH)

console:
	g++ $(COMPILER_FLAGS) -g -c src/console/$(CONSOLE_TYPE)_console.cpp -o obj/console.o $(COMPILER_INCLUDE_PATH)

rfidlib:
	g++ $(COMPILER_FLAGS) -g -c $(RFID_PATH)/rc522.c -o $(RFID_PATH)/obj/rc522.o $(COMPILER_INCLUDE_PATH)
	g++ $(COMPILER_FLAGS) -g -c $(RFID_PATH)/rfid.c -o $(RFID_PATH)/obj/rfid.o $(COMPILER_INCLUDE_PATH)

linking:
	g++ $(COMPILER_FLAGS) -g $(PLAYER_OBJECTS) obj/console.o -o bin/player $(RFID_PATH)/obj/rc522.o $(RFID_PATH)/obj/rfid.o $(LINKER_LIBRARY_PATH) $(LINKER_LIBS) $(LINKER_FLAGS)


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
