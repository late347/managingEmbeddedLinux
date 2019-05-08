#include <wiringPi.h>          
#include <lcd.h>                
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <unistd.h>
#include <arpa/inet.h>

//USING WIRINGPI PIN NUMBERS HERE
#define LCD_RS  25               //Register select pin
#define LCD_E   24               //Enable Pin
#define LCD_D4  23               //Data pin 4
#define LCD_D5  22               //Data pin 5
#define LCD_D6  21               //Data pin 6
#define LCD_D7  14               //Data pin 7

int main(void){
	/* code */
    int lcd;
    wiringPiSetup();
    lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);

    if (lcd){
        printf ("lcdInit failed! \n");//check that lcd initialize successfully
        return -1 ;
    }else{
        printf("lcd Successfull!\n");
    }


    int n;
    struct ifreq ifr;
    char iface[] = "wlan0"; //Change this to the network of your choice (eth0, wlan0, etc.)

	for (int j = 0; j < 36; j++) {
		lcdClear(lcd);
		n = socket(AF_INET, SOCK_DGRAM, 0);
		ifr.ifr_addr.sa_family = AF_INET;
		strncpy(ifr.ifr_name, iface, IFNAMSIZ - 1);
		ioctl(n, SIOCGIFADDR, &ifr);
		close(n);

		lcdPosition(lcd, 0, 0);
		lcdPrintf(lcd, "IP Address: ");

		lcdPosition(lcd, 0, 1);
		lcdPrintf(lcd, ("%s - %s\n", iface, inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr)));

		delay(5000);
	}
	lcdClear(lcd);



	return 0;
}