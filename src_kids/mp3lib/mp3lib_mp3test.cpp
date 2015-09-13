#include <iostream>
#include <boost/filesystem.hpp>
#include "mp3lib.hpp"
using namespace std;
using namespace boost::filesystem;

namespace mp3lib {
	Mp3Lib::Mp3Test::Mp3Test(void)
	{
		cout << a << "\t" << c << endl;
		cout << b << "\t" << c << endl;
	}

	void Mp3Lib::Mp3Test::output(int in){
		int tmp = a;
		a = b;
		b = a + tmp;

		c += in;

		cout << b << "\t" << c << endl;
	}
}
