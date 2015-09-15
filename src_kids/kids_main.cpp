#include <iostream>
#include <stdlib.h>
#include "mp3lib/mp3lib.hpp"
using namespace std;
using namespace mp3lib;

void start_player(string);

int main(int argc, char **argv)
{
	if(argc==1)
	{
		start_player("./library");
	}
	else if(argc==2)
	{
		start_player(string(argv[1]));
	}
	else
	{
		perror("wrong number of arguments. Use \"pi_player_kids [path_to_lib]\"");
		return -1;
	}
	return 0;
}

void start_player(string path_to_lib)
{
	Mp3Lib *mp3 = new Mp3Lib(path_to_lib);

	cout << "01 "; mp3->showSeries();
	cout << "02 "; mp3->showSeries();
	cout << "03 "; mp3->nextSeries();
	cout << "04 "; mp3->nextSeries();
	cout << "05 "; mp3->nextSeries();
	cout << "06 "; mp3->nextSeries();
	cout << "07 "; mp3->nextSeries();
	cout << "08 "; mp3->nextSeries();
	cout << "09 "; mp3->previousSeries();
	cout << "10 "; mp3->previousSeries();
	cout << "11 "; mp3->previousSeries();
	cout << "12 "; mp3->previousSeries();
	cout << "13 "; mp3->previousSeries();
	cout << "14 "; mp3->previousSeries();
	cout << "15 "; mp3->previousSeries();
	cout << "16 "; mp3->nextSeries();
	cout << "17 "; mp3->nextSeries();
	cout << "18   "; mp3->enterSeries();
	/*cout << "xx "; mp3->nextSeries();*/
	cout << "19     "; mp3->showEpisode();
	cout << "20     "; mp3->nextEpisode();
	cout << "21     "; mp3->nextEpisode();
	cout << "22     "; mp3->nextEpisode();
	cout << "23     "; mp3->nextEpisode();
	cout << "24     "; mp3->nextEpisode();
	cout << "25     "; mp3->nextEpisode();
	cout << "26     "; mp3->nextEpisode();
	cout << "27     "; mp3->nextEpisode();
	cout << "28     "; mp3->previousEpisode();
	cout << "29     "; mp3->previousEpisode();
	cout << "30     "; mp3->previousEpisode();
	cout << "31     "; mp3->previousEpisode();
	cout << "32     "; mp3->previousEpisode();
	cout << "32     "; mp3->previousEpisode();
	cout << "33   "; mp3->exitSeries();
	cout << "34 "; mp3->showSeries();
	cout << "35 "; mp3->nextSeries();
	cout << "36 "; mp3->nextSeries();
	cout << "37   "; mp3->enterSeries();
	cout << "38     "; mp3->showEpisode();
	cout << "39     "; mp3->nextEpisode();
	cout << "40     "; mp3->nextEpisode();
	cout << "41     "; mp3->nextEpisode();
	cout << "42     "; mp3->nextEpisode();
	cout << "43     "; mp3->nextEpisode();
	cout << "44     "; mp3->nextEpisode();
	cout << "45     "; mp3->nextEpisode();
	cout << "46     "; mp3->nextEpisode();
	cout << "47     "; mp3->nextEpisode();
	cout << "48     "; mp3->nextEpisode();
	cout << "49     "; mp3->nextEpisode();
	cout << "50     "; mp3->nextEpisode();
	cout << "51       "; mp3->enterEpisode();
	cout << "52         "; mp3->showMp3();
	cout << "53         "; mp3->nextMp3();
	cout << "54         "; mp3->nextMp3();
	cout << "55         "; mp3->nextMp3();
	cout << "56         "; mp3->nextMp3();
	cout << "57         "; mp3->nextMp3();
	cout << "58         "; mp3->nextMp3();
	cout << "59         "; mp3->nextMp3();
	cout << "60         "; mp3->nextMp3();
	cout << "61           "; mp3->playMp3();
	cout << "62         "; mp3->previousMp3();
	cout << "63         "; mp3->previousMp3();
	cout << "64         "; mp3->previousMp3();
	cout << "65         "; mp3->previousMp3();
	cout << "66         "; mp3->previousMp3();
	cout << "67         "; mp3->previousMp3();
	cout << "68         "; mp3->previousMp3();
	cout << "69           "; mp3->playMp3();
	cout << "70         "; mp3->nextMp3();
	cout << "71           "; mp3->playMp3();
	//mp3->testlib();
}
