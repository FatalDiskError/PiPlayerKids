#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "../bcm2835/bcm2835-1.45/src/bcm2835.h"
#include "rc522.h"
using namespace std;

int init_rfid();

int main(int argc, char **argv)
{
	/*
	uint8_t buff[MAXRLEN];
	uint8_t SN[7];
	char status,tmp;
	*/
	
	uint8_t use_gpio=1;
	uint8_t gpio=18;
	uint32_t spi_speed=5000;

	if(init_rfid())
	{
		return 1;
	}

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

		if(select_tag_sn(SN,&SN_len)!=TAG_OK)
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

		if(use_gpio){
			bcm2835_gpio_write(gpio, HIGH);
		}
		
		if (find_config_param(sn_str,str,sizeof(str),1)>0) {
			child=fork();
			if (child==0) {
				fclose(stdin);
				freopen("","w",stdout);
				freopen("","w",stderr);
				execl("/bin/sh","sh","-c",str,NULL);
			} else if (child>0) {
				i=6000;
				do {
					usleep(10000);
					tmp=wait3(NULL,WNOHANG,NULL);
					i--;
				} while (i>0 && tmp!=child);

				if (tmp!=child) {
					kill(child,SIGKILL);
					wait3(NULL,0,NULL);
				}
			}else{
				syslog(LOG_DAEMON|LOG_ERR,"Can't run child process! (%s %s)\n",sn_str,str);
			}

		}else{
			syslog(LOG_DAEMON|LOG_INFO,"New tag: type=%04x SNlen=%d SN=%s\n",CType,SN_len,sn_str);

			if (save_mem) {
				switch (CType) {
				case 0x4400:
					max_page=0x0f;
					page_step=4;
					break;
				case 0x0400:
					PcdHalt();
					if (use_gpio) bcm2835_gpio_write(gpio, LOW);
					continue;
					max_page=0x3f;
					page_step=1;
					break;
				default:
					break;
				}
				p=str;
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
				for (i=0;i<max_page;i+=page_step) {
					read_tag_str(i,str);
					fprintf(fmem_str,"%02x: %s\n",i,str);
				}
				fclose(fmem_str);
			}
		}
		PcdHalt();
		if (use_gpio) bcm2835_gpio_write(gpio, LOW);
	}
	/*
	for(;;)
	{
		tmp = PcdRequest(PICC_REQIDL, buff);
		if(tmp==MI_OK)
		{
			status = PcdAnticoll(PICC_ANTICOLL1, buff);
			if(status==MI_OK)
			{
				printf("Card sn1 -> 0x_|%02x|%02x|%02x|%02x|_\n", buff[0], buff[1], buff[2], buff[3]);
				if(buff[0]==0x88)
				{
					memcpy(SN, &buff[1], 3);
					status = PcdSelect(PICC_ANTICOLL1, buff);
					status = PcdAnticoll(PICC_ANTICOLL2, buff);
					if(status==MI_OK)
					{
						memcpy(&SN[3], buff, 4);
						printf("SN -> 0x");
						for(tmp=0; tmp<7; tmp++)
						{
							printf("%02x", SN[tmp]);
						}
						printf("\n");
						
						status = PcdSelect(PICC_ANTICOLL2, buff);
						PcdRead(0, buff);
						for(tmp=0; tmp<16; tmp++)
						{
							printf("%02x",buff[tmp]);
							if(((tmp+1) % 4)==0)
							{
								printf(" ");
							}
						}
						printf("\n");

						PcdRead(4, buff);
						for(tmp=0; tmp<16; tmp++)
						{
							printf("%02x",buff[tmp]);
							if(((tmp+1) % 4)==0)
							{
								printf(" ");
							}
						}
						printf("\n");

						PcdRead(8, buff);
						for(tmp=0; tmp<16; tmp++)
						{
							printf("%02x", buff[tmp]);
							if(((tmp+1) % 4)==0)
							{
								printf(" ");
							}
						}
						printf("\n");

						PcdRead(0x0c, buff);
						for(tmp=0; tmp<16; tmp++)
						{
							printf("%02x", buff[tmp]);
							if(((tmp+1) % 4)==0)
							{
								printf(" ");
							}
							buff[tmp] = 0;
						}
						printf("\n");
						PcdHalt();
					}
				}
			}
		}else{ }
	}
	*/

	bcm2835_spi_end();
	bcm2835_close();
	return 0;

	/*
	// second try - not working
	// Send a byte to the slave and simultaneously read a byte back from the slave
	// If you tie MISO to MOSI, you should read back what was sent
	uint8_t send_data = 0x23;
	uint8_t read_data = bcm2835_spi_transfer(send_data);
	printf("Sent to SPI: 0x%02X. Read back from SPI: 0x%02X.\n", send_data, read_data);
	if (send_data != read_data)
	  printf("Do you have the loopback from MOSI to MISO connected?\n");
	bcm2835_spi_end();
	bcm2835_close();
	*/
	/*
	// first try
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
	*/
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
