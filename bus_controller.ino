#include <SIM900.h>
#include <inetGSM.h>
#include <ArduinoJson.h>
#include "gps.h"
boolean started = false;
int i=0;
char msg[2000];
char inSerial[2000];
InetGSM inet;
char tempMsg[500];
String message;

GPSGSM gps;
char lon[15];
char lat[15];
char alt[15];
char time[20];
char vel[15];
char msg1[5];
char msg2[5];
char stat;

String dateGPS="", latitudeGPS="", longitudeGPS="", speedGPS="", altitudeGPS="";
void setup()
{
	startDevice();
	if(started){
		Serial.println("started");   
		if(startSAPBR() == 1){
			int32_t time = millis();
			
			getdata(tempMsg,500,"1");

			time = millis() - time;
	        time = time/1000;
	        Serial.println("done!");
	        Serial.print(time); Serial.println(" s");
		}
	}
	//Serial.println(message);
  if(startgps() == 1){
     postdata("BUSway",dateGPS, altitudeGPS.toInt(), 30, "on", "closed");
  } 
}
int buscount=0;
void loop()
{
     serialhwread();
     serialswread();
     buscount += 1;
  if(startgps() == 1){
    postdata("BUSway " +String(buscount),dateGPS, altitudeGPS.toInt(), 30, "on", "closed");
  }

}

void serialhwread()
{
	i=0;
     if (Serial.available() > 0) {
          while (Serial.available() > 0) {
               inSerial[i]=(Serial.read());
               delay(10);
               i++;
          }

          inSerial[i]='\0';
          if(!strcmp(inSerial,"/END")) {
               Serial.println("_");
               inSerial[0]=0x1a;
               inSerial[1]='\0';
               gsm.SimpleWriteln(inSerial);
          }
          //Send a saved AT command using serial port.
          if(!strcmp(inSerial,"TEST")) {
               Serial.println("SIGNAL QUALITY");
               gsm.SimpleWriteln("AT+CSQ");
          }
          //Read last message saved.
          if(!strcmp(inSerial,"MSG")) {
               Serial.println(msg);
          } else {
               Serial.println(inSerial);
               gsm.SimpleWriteln(inSerial);
          }
          inSerial[0]='\0';
     }
}

void serialswread()
{
     gsm.SimpleRead();
}

