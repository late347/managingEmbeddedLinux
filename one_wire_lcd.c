#include <wiringPi.h>
#include <lcd.h>
#include <stdio.h>              //Needed for the printf function below
#include <stdlib.h>
#include <string.h>
//USE WIRINGPI PIN NUMBERS
#define LCD_RS  25               //Register select pin
#define LCD_E   24               //Enable Pin
#define LCD_D4  23               //Data pin 4
#define LCD_D5  22               //Data pin 5
#define LCD_D6  21               //Data pin 6
#define LCD_D7  14               //Data pin 7

//#define LED_OUT 0
#define BUT_IN 1 // configure as pullup, input pin 

//global variables
FILE *fp1;
char buffer[255]={0};
char sensortemp[13]={0}; //result from sensorfile, the temperaute as char array
char sensorFile[] = "/sys/bus/w1/devices/10-0008031ba6c8/w1_slave"; 
char readytemp[13]={0};

int flip();
int flip(int i){
    return 1 - i;
};


void readTemp();
void readTemp(){
    FILE *fp2 = fopen(sensorFile, "r");
   // delay(1000); //wait to allow temp to update itself into the file
    if(fp2 == NULL){
    	printf("file doesnt exist, error\n");
    }else{
    	printf("opening sensorfile for temp measurement...\n");

    	while(fgets(buffer, sizeof(buffer), fp2)); //read that file into buffer
    	char *pointer0 = strstr(buffer, "t="); //find the actual temperature
    	pointer0 += 2; //advance pointer to poitn at number (temp)
    	
    	
    	for(int i=0; (*pointer0 != '\n'); pointer0++,i++){
    		*(sensortemp+i) = *pointer0;
    	}
    	printf("temp was : %s\n", sensortemp );

    	float c = atof(sensortemp);
    	c = c / 1000.0;
    	int res = snprintf(readytemp,13,"%.2f",c);
		printf("readytemp was : %s\n", readytemp);
    	
    }

}





int main(){
	/*every fileread triggers temp measurement, use 1 sec delay for safety?*/
    int lcd;

    wiringPiSetup();
    lcd = lcdInit (2, 16, 4, LCD_RS, LCD_E, LCD_D4, LCD_D5, LCD_D6, LCD_D7, 0, 0, 0, 0);
    if (lcd){//check that lcd initialize successfully
        printf ("lcdInit failed! \n");
        return -1 ;
    }
    else{
        printf("lcd Successfull!\n");
    }
    fp1 = fopen(sensorFile, "r");
    if(fp1 == NULL){
    	printf("file doesnt exist, error\n");
    }else{
    	printf("file exists...\n");
    }
    delay(2000);



    pinMode(BUT_IN, INPUT);
    pullUpDnControl(BUT_IN, PUD_UP);  //pull up to 3.3V,make GPIO1 a stable level
  

    while(1){
        
        if(digitalRead(BUT_IN) == 0){ //indicate that button has pressed down
            //GET TEMP MEASUREMENT
            //delay 1 sec
      	    lcdClear(lcd);
            readTemp();
            delay(2000);
            
            lcdPuts(lcd,readytemp);
memset(readytemp,0,sizeof(readytemp));
memset(sensortemp,0,sizeof(sensortemp));
     

        }else{
        	delay(50);
        	//maybe smaller delay for granularity
        }
    }

 
    return 0;
}
