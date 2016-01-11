#include "rfid.hpp"

using namespace std;
using namespace console;

namespace rfid
{
	Rfid::Rfid(Console** ppConsole=NULL)
	{
		_pLinkToConsole=ppConsole;
		(*_pLinkToConsole)->printLog("constructing player");
		
		init_rfid();
	}
	
	Rfid::~Rfid(void)
	{
		(*_pLinkToConsole)->printLog("destructing rfid");
		release_rfid();
	}
	
	void Rfid::listen(void)
	{
		(*_pLinkToConsole)->printLog("start Rfid::listen(void)");
		
		uint8_t SN[10];
		uint8_t SN_len=0;
		
		uint8_t use_gpio=1;
		uint8_t gpio=18;
		
		uint16_t CType=0;
		
		int tmp, i;
		char status;
		char str[255];
		char *p;
		char sn_str[23];
		
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
			for(tmp=0; tmp<SN_len; tmp++)
			{
				sprintf(p, "%02x", SN[tmp]);
				p+=2;
			}
			*(p++)=0;
			
			/*
			if(use_gpio){
				bcm2835_gpio_write(gpio, HIGH);
			}
			*/
			
			_outStream << "Tag found ";
			_outStream << "[type: " << setfill('0') << setw(4) << setbase(16) << CType << ", ";
			_outStream << "SNlen: " << setbase(10) << int(SN_len) << ", ";
			_outStream << "SN: " << int(SN) << "] ";
			_outStream << "SN: " << sn_str;
			(*_pLinkToConsole)->printOut(&_outStream);
			
			string serial(sn_str);
			
			if(serial == "db9a8e0b")
			{
				(*_pLinkToConsole)->printLog("stop Rfid::listen(void)");
				return;
			}
			else if(serial == "c45b133b")
			{
				(*_pLinkToConsole)->printLog("stop Rfid::listen(void)");
				(*_pLinkToConsole)->printLog("### shutdown ###");
				system("sudo shutdown -h -P now");
				return;
			}
			rfidSignal(serial);
			
			p=str;
			PcdHalt();
			if (use_gpio){
				bcm2835_gpio_write(gpio, LOW);
			}
		}
	}
	
	void Rfid::init_rfid()
	{
		if(geteuid()!=0 || getenv("FAKEROOTKEY"))
		{
			(*_pLinkToConsole)->printOut("You need to be root to properly run this program");
			throw "You need to be root to properly run this program";
		}
		if(!bcm2835_init())
		{
			(*_pLinkToConsole)->printOut("Not able to initialize BCM2835");
			throw "Not able to initialize BCM2835";
		}
		
		bcm2835_spi_begin();
		bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);	// default
		bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);					// default
		bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32);	// default
		bcm2835_spi_chipSelect(BCM2835_SPI_CS0);					// default
		bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);	// default
		
		/*
		uid_t uid=500;
		setuid(uid);
		*/
		
		InitRc522();
	}
	
	void Rfid::release_rfid()
	{
		bcm2835_spi_end();
		bcm2835_close();
	}
}
