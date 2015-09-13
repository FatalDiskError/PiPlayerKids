#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../bcm2835/bcm2835-1.45/src/bcm2835.h"
using namespace std;

int main(int argc, char **argv){
    if (geteuid() == 0 && !getenv("FAKEROOTKEY")){
		if (!bcm2835_init()){
			return 1;
		}else{
			cout << " > init() OK" << endl;
		}
		
		if (!bcm2835_close()){
			return 1;
		}else{
			cout << " > close() OK" << endl;
		}
    }else{
		cout << " > You need to be root to properly run this test program" << endl;
		return 1;
    }
    return 0;
}
