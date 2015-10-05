#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include "rfid.h"
#include "rc522.h"
#include "bcm2835.h"
using namespace std;

int init_rfid();

int main(int argc, char **argv)
{
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

	cout << " > Waiting for RFID card" << endl;
	
	for(;;)
	{
		status = find_tag(&CType);
		/*
		switch(status)
		{
			case TAG_NOTAG:
				cout << "TAG_NOTAG" << endl;
				break;
			case TAG_OK:
				cout << "TAG_OK" << endl;
				break;
			case TAG_COLLISION:
				cout << "TAG_COLLISION" << endl;
				break;
			case TAG_ERR:
				cout << "TAG_ERR" << endl;
				break;
			case TAG_ERRCRC:
				cout << "TAG_ERRCRC" << endl;
				break;
			default:
				cout << "unknown status" << endl;
				break;
		}
		*/

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
		*(p++)='[';
		for(tmp=0; tmp<SN_len; tmp++)
		{
			sprintf(p, "%02x", SN[tmp]);
			p+=2;
		}
		*(p++)=']';
		*(p++)=0;

		/*
		if(use_gpio){
			bcm2835_gpio_write(gpio, HIGH);
		}
		*/
		
		cout << " > Found tag: type=" << setfill('0') << setw(4) << setbase(16) << CType;
		cout << " SNlen=" << setbase(10) << int(SN_len);
		cout << " SN=" << int(SN);
		cout << " SN=" << sn_str << endl;

		switch (CType) {
		case 0x4400:
			num_blocks=0x0f;
			block_size=4;
			break;
		case 0x0400:
			/*
			PcdHalt();
			if (use_gpio){
				bcm2835_gpio_write(gpio, LOW);
			}
			continue;
			*/
			/*
			16 sectors, 64 blocks, 1024 bytes:
				each sector has 4 blocks
				each block has 16 bytes
			*/
			num_blocks=0x3f;
			block_size=1;
			break;
		default:
			break;
		}
		p=str;
		/*
		sprintf(p,"%s",fmem_path);
		p+=strlen(p);
		for (tmp=0;tmp<SN_len;tmp++) {
			sprintf(p,"%02x",SN[tmp]);
			p+=2;
		}
		sprintf(p,".txt");
		if ((fmem_str=fopen(str,"r"))!=NULL) {
			fclose(fmem_str);
			PcdHalt();
			if (use_gpio) bcm2835_gpio_write(gpio, LOW);
			continue;
		}
		if ((fmem_str=fopen(str,"w"))==NULL) {
			syslog(LOG_DAEMON|LOG_ERR,"Cant open file for write: %s",str);
			PcdHalt();
			if (use_gpio) bcm2835_gpio_write(gpio, LOW);
			continue;
		}
		*/
		for (i=0; i<num_blocks; i+=block_size) {
			/*
			//16 bytes á 8 bit
			read_tag_str(i,str);
			//fprintf(fmem_str,"%02x: %s\n",i,str);
			cout << " > " << setfill('0') << setw(2) << setbase(16) << i;
			cout << "=" << str << endl;
			*/
		}
			
		PcdHalt();
		if (use_gpio){
			bcm2835_gpio_write(gpio, LOW);
		}
	}

	bcm2835_spi_end();
	bcm2835_close();
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
