#include <iostream>
#include <sstream>
#include <vector>
#include <iomanip>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <signal.h>
#include <sys/wait.h>

#include "bass.h"
#include "console.hpp"
#include "rfid.h"
#include "rc522.h"
#include "bcm2835.h"

using namespace std;
using namespace console;

ostringstream out;
ostringstream log;
Console *con = new Console(VERTICAL_LAYOUT);
HSTREAM streamHandle;
bool success;	
int currentEpisodeIndex = 0;
int currentFileIndex = 0;
vector<vector<string>> episodes = {
	{"./bin/2.mp3"},
	{"./bin/3.mp3"},
	{"./bin/4.mp3"},
	{"./bin/5.mp3"},
	{"./bin/6_1.mp3","./bin/6_2.mp3","./bin/6_3.mp3","./bin/6_4.mp3","./bin/6_5.mp3"}
};
vector<string> files = {
	"./bin/2.mp3",
	"./bin/3.mp3",
	"./bin/4.mp3",
	"./bin/5.mp3",
	"./bin/6.mp3"
};
vector<string> codes = {
	"39877302",
	"fb9eb50b",
	"d856fb0e",
	"dbac170b",
	"4933a102"
};

int init_rfid();
void printOut(ostringstream *out);
void printLog(ostringstream *log);
void playFile(void);
void CALLBACK playNextFile(HSYNC handle, DWORD channel, DWORD data, void *user);


int main(int argc, char **argv)
{
	// ### BASS ###
	int device = -1, numDevices, numActiveDevices = 0;
	BASS_DEVICEINFO deviceInfo;
	for (numDevices = 0; BASS_GetDeviceInfo(numDevices, &deviceInfo); numDevices++)
	{
		if (deviceInfo.flags & BASS_DEVICE_DEFAULT)
			device = numDevices;
			
		if (deviceInfo.flags & BASS_DEVICE_ENABLED)
			numActiveDevices++;
		
		out << numDevices;
		out << " [" << ((deviceInfo.flags & BASS_DEVICE_DEFAULT) ? "x" : " ") << "]";
		out << " [" << ((deviceInfo.flags & BASS_DEVICE_ENABLED) ? "act" : "   ") << "]";
		out << " Device: " << deviceInfo.name;
		printOut(&out);
	}
	
	out << "Active devices: " << numActiveDevices << " of " << numDevices;
	printOut(&out);
	
	/*
	for(int i=0; i<15; i++){
		usleep(20000);
		out << "wait " << i;
		printOut(&out);
	}
	*/
	
	int frequency = 44100;
	float volume = 0.1;
	string file = "./bin/1.mp3";

	/* Initialize output device */
	success = BASS_Init(device, frequency, 0, 0, NULL);
	log << "BASS_Init(" << device << ", " << frequency << ", 0, 0, NULL) ";
	log << (success ? "success" : "failed") << " with errorCode: " << BASS_ErrorGetCode();
	printLog(&log);
	
	/*
	// does not work on raspi
	success = BASS_SetVolume(volume);
	log2 << "BASS_SetVolume(" << volume << ") ";
	log2 << (success ? "success" : "failed") << " with errorCode: " << BASS_ErrorGetCode();
	printLog(log2.str());
	*/

	// ### RFID ###
	uid_t uid=500;
	uint8_t SN[10];
	uint16_t CType=0;
	uint8_t SN_len=0;
	char status;
	int tmp, i;
	char str[255];
	char *p;
	char sn_str[23];

	int num_blocks=0;
	uint8_t block_size=0;
	
	uint8_t use_gpio=1;
	uint8_t gpio=18;
	uint32_t spi_speed=5000;

	if(init_rfid())
	{
		return 1;
	}

	setuid(uid);
	InitRc522();
	
	out << "Waiting for RFID card" << endl;
	printOut(&out);
	
	for(;;)
	{
		status = find_tag(&CType);

		if(status==TAG_NOTAG)
		{
			usleep(200000);
			continue;
		}
		else if((status!=TAG_OK) && (status!=TAG_COLLISION))
		{
			continue;
		}

		if(select_tag_sn(SN, &SN_len)!=TAG_OK)
		{
			continue;
		}

		p=sn_str;
		//*(p++)='[';
		for(tmp=0; tmp<SN_len; tmp++)
		{
			sprintf(p, "%02x", SN[tmp]);
			p+=2;
		}
		//*(p++)=']';
		*(p++)=0;

		/*
		if(use_gpio){
			bcm2835_gpio_write(gpio, HIGH);
		}
		*/
		// log <<  << endl;
		// printLog(&log);
		
		log << "Found tag: type=" << setfill('0') << setw(4) << setbase(16) << CType;
		log << " SNlen=" << setbase(10) << int(SN_len);
		log << " SN=" << int(SN);
		log << " SN=" << sn_str << endl;
		printLog(&log);
		
		for(int i=0; i<codes.size(); i++)
		{
			if(codes[i]==sn_str){
				currentFileIndex = 0;
				currentEpisodeIndex = i;
				
				log << "Known code. Start playing " << episodes[currentEpisodeIndex][currentFileIndex] << endl;
				printLog(&log);
				
				playFile();
				break;
			}
		}

		p=str;
		PcdHalt();
		if (use_gpio){
			bcm2835_gpio_write(gpio, LOW);
		}
	}
	
	/*
	streamHandle = BASS_StreamCreateFile(FALSE, file.c_str(), 0, 0, 0);
	log << "BASS_StreamCreateFile(FALSE, " << file << ", 0, 0, 0) ";
	log << "streamHandle: " << streamHandle << " with errorCode: " << BASS_ErrorGetCode();
	printLog(&log);
	
	success = BASS_ChannelPlay(streamHandle, true);
	log << "BASS_ChannelPlay(" << streamHandle << ", true) ";
	log << (success ? "success" : "failed") << " with errorCode: " << BASS_ErrorGetCode();
	printLog(&log);
	*/
	
	/*
	for(int i = 0; i<files.size(); i++){
		usleep(5000000);
		
		streamHandle = BASS_StreamCreateFile(FALSE, files[i].c_str(), 0, 0, 0);
		log << "BASS_StreamCreateFile(FALSE, " << files[i] << ", 0, 0, 0) ";
		log << "streamHandle: " << streamHandle << " with errorCode: " << BASS_ErrorGetCode();
		printLog(&log);
		
		success = BASS_ChannelPlay(streamHandle, true);
		log << "BASS_ChannelPlay(" << streamHandle << ", true) ";
		log << (success ? "success" : "failed") << " with errorCode: " << BASS_ErrorGetCode();
		printLog(&log);
	}
	*/

	/*
	"initscr is normally the first curses routine to call when initializing a
	program. The initscr code determines the terminal type and initializes all curses
	data structures. initscr also causes the first call to refresh to clear
	the screen."

	"The cbreak routine disables line buffering and
	erase/kill character-processing (interrupt and flow control characters
	are unaffected), making characters typed by the user immediately avail-
	able to the program."

	"The printw, wprintw, mvprintw and mvwprintw routines are analogous to
	printf [see printf(3S)]. In effect, the string that would be output by
	printf is output instead as though waddstr were used on the given window."

	"The getch, wgetch, mvgetch and mvwgetch, routines read a character from
	the window. In no-delay mode, if no input is waiting, the value ERR is
	returned. In delay mode, the program waits until the system passes text
	through to the program. Depending on the setting of cbreak, this is
	after one character (cbreak mode), or after the first newline (nocbreak
	mode). In half-delay mode, the program waits until a character is typed
	or the specified timeout has been reached."

	"A program should always call endwin before exiting or escaping from
	curses mode temporarily. This routine restores tty modes, moves the cur-
	sor to the lower left-hand corner of the screen and resets the terminal
	into the proper non-visual mode. "

	"A program using these routines must be linked with the -lncurses option,
	or (if it has been generated) with the debugging library -lncurses_g.
	(Your system integrator may also have installed these libraries under the
	names -lcurses and -lcurses_g.) The ncurses_g library generates trace
	logs (in a file called 'trace' in the current directory) that describe
	curses actions."	
	*/
	//getch();
	
	bcm2835_spi_end();
	bcm2835_close();

	BASS_Free();
	endwin();
	return 0;
}

int init_rfid() {
    if(geteuid()!=0 || getenv("FAKEROOTKEY"))
	{
		cerr << " > You need to be root to properly run this test program" << endl;
		return 1;
    }
	if(!bcm2835_init())
	{
		cerr << " > Not able to initialize BCM2835" << endl;
		return 1;
	}
	
	bcm2835_spi_begin();
	bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);	// default
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);					// default
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32);	// default
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);					// default
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);	// default

	InitRc522();
	return 0;
}

void printOut(ostringstream *out)
{
	con->printOut(out->str());
	out->str("");
	out->clear();
}

void printLog(ostringstream *log)
{
	con->printLog(log->str());
	log->str("");
	log->clear();
}

void playFile(void)
{
	if(streamHandle != 0){
		BASS_ChannelStop(streamHandle);
		log << "BASS_ChannelStop(" << streamHandle << ")" << endl;
		printLog(&log);
	}
				
	string file = episodes[currentEpisodeIndex][currentFileIndex];
	streamHandle = BASS_StreamCreateFile(FALSE, file.c_str(), 0, 0, BASS_STREAM_AUTOFREE);
	log << "BASS_StreamCreateFile(FALSE, " << file << ", 0, 0, BASS_STREAM_AUTOFREE) ";
	log << "streamHandle: " << streamHandle << " with errorCode: " << BASS_ErrorGetCode();
	printLog(&log);
	
	BASS_ChannelSetSync(streamHandle, BASS_SYNC_END, 0, playNextFile, 0);
	success = BASS_ChannelPlay(streamHandle, FALSE);
	log << "BASS_ChannelPlay(" << streamHandle << ", true) ";
	log << (success ? "success" : "failed") << " with errorCode: " << BASS_ErrorGetCode();
	printLog(&log);
}

void CALLBACK playNextFile(HSYNC handle, DWORD channel, DWORD data, void *user)
{
	BASS_StreamFree(channel);
	
	++currentFileIndex;
	if(currentFileIndex<episodes[currentEpisodeIndex].size())
	{
		playFile();
	}
}
