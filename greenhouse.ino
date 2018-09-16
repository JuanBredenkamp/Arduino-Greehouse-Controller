/*      Project : Greehouse
         Author : Juan Bredenkamp
        Created : 20 Jun 2018  (Updated 18 Jul 2018)
    Description : Greenhouse Automation
        Version : v.1
		
	Arduino Pin                                    Board Pin

		Ground  ----------------------------------   Ground
		Pin 2   ----------------------------------   Switch1
		Pin 4   ----------------------------------   Greenled
		Pin 5   ----------------------------------   Redled
		Pin 3   ----------------------------------   Buzzer
		Pin 8   ----------------------------------   DHT1PIN
		Pin 9   ----------------------------------   DHT2PIN
		Pin 10  ----------------------------------   SoilSens
		Pin 23  ----------------------------------   Relay1
		Pin 25  ----------------------------------   Relay2
		Pin 27  ----------------------------------   Relay3
		Pin 29  ----------------------------------   Relay4
  
	Cant Use Arduino Pin

		Pin 4    ---- SS for SD Card
		Pin 10   ---- SS for Ethernet Controller W5100
		Pin 11   ---- SPI bus (through the ICSP header) MOSI
		Pin 12   ---- SPI bus (through the ICSP header) MISO
		Pin 13   ---- SPI bus (through the ICSP header) SCK
		Pin 50   ---- Duemilanove MISO
		Pin 51   ---- Duemilanove MOSI
		Pin 52   ---- Duemilanove SCK
		Pin 52   ---- SS Hardware
		
*/

//Libraries
#include <DHT.h>
#include <SPI.h>
#include <Wire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "U8glib.h"

//Constants
#define Switch1 2
#define Greenled 4
#define Redled 5
#define Buzzer 3
#define DHT1PIN 8
#define DHT2PIN 9
#define SoilSens 10
#define relay1 23
#define relay2 25
#define relay3 27
#define relay4 29
#define DHT2TYPE DHT22

int maxtemp= 35;
int mintemp= 2;
int switchcheckcount = 0;

OneWire oneWire(SoilSens);
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);
DHT dht1(DHT1PIN, DHT2TYPE);
DHT dht2(DHT2PIN, DHT2TYPE);
DallasTemperature sensors(&oneWire);


void setup(){  
	Serial.begin(9600);
	delay(1000);
	pinMode(Buzzer, OUTPUT);
	digitalWrite(Buzzer, HIGH);
	pinMode(Switch1, INPUT_PULLUP);
	pinMode(Greenled, OUTPUT);
	pinMode(Redled, OUTPUT);
	pinMode(relay1, OUTPUT);
	pinMode(relay2, OUTPUT);
	pinMode(relay3, OUTPUT);
	pinMode(relay4, OUTPUT);
	dht1.begin();
	dht2.begin();
	sensors.begin();
	BootingLcd();
	digitalWrite(relay1, HIGH);
	digitalWrite(relay2, HIGH);
	digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
	digitalWrite(Buzzer, LOW);
	digitalWrite(Greenled, HIGH);
  delay(2000);
 
}

void loop(){
	TempIndoor();
	TempOutdoor();
	TempSoil();
   
}

void BootingLcd(){
	u8g.firstPage();
	do {
		u8g.setFont(u8g_font_helvB18);
		u8g.drawStr( 2, 40, "  Booting");
	}
	
	while( u8g.nextPage());
		delay(100);
}
	
void TempIndoor(){
	float h1 = dht1.readHumidity();
	float t1 = dht1.readTemperature();
	if (isnan(t1) || isnan(h1)) {
		Serial.println("Failed to read from Indoor Sensor");
	}

	else {
		Serial.print("Indoor Humidity: "); 
		Serial.print(h1);
		Serial.print(" %");
		Serial.print(" Temperature: "); 
		Serial.print(t1);
		Serial.println(" *C");
	}

	if(t1 > maxtemp){
        digitalWrite(Buzzer, HIGH);
        digitalWrite(Redled, HIGH);
        digitalWrite(Greenled, LOW);
        digitalWrite(relay2, LOW);
        Serial.print("!! Indoor Temperature to High !!");
        Serial.println("");
        u8g.firstPage();
		
  	do{
		u8g.setFont(u8g_font_unifont);
		u8g.drawStr( 2,10, "Indoor Temp");
		u8g.drawStr(2,55,"! High Alert !");
	}
	
	while( u8g.nextPage());
		Delay5Sec();
	}

	else if (t1 < mintemp){
		digitalWrite(Buzzer, HIGH);
        digitalWrite(Redled, HIGH);
        digitalWrite(Greenled, LOW);
        digitalWrite(relay3, LOW);
        Serial.print("!! Indoor Temperature to Low !!");
        Serial.println("");
        u8g.firstPage();
		
	do{
		u8g.setFont(u8g_font_unifont);
		u8g.drawStr(2,10, "Indoor Temp");
		u8g.drawStr(2,55,"! Low Alert !");
	}
	
	while( u8g.nextPage());
		Delay5Sec();
	}
	
	else{
		digitalWrite(Buzzer, LOW);
		digitalWrite(Redled, LOW);
		digitalWrite(Greenled, HIGH);
		digitalWrite(relay2, HIGH);
		digitalWrite(relay3, HIGH);
	}
	
	u8g.firstPage();
	
	do{
		u8g.setFont(u8g_font_unifont);
		u8g.drawStr(2,10, "Indoor:  Temp");
        u8g.setFont(u8g_font_helvB18);
		u8g.drawStr(90,55,"\260C");
		u8g.setPrintPos(2, 55);
		u8g.print(t1);
	}
	
	while( u8g.nextPage());
		Delay5Sec();
		u8g.firstPage();
		
	do{
		u8g.setFont(u8g_font_unifont);
		u8g.drawStr(2,10, "Indoor:  Humd");
		u8g.setFont(u8g_font_helvB18);
		u8g.drawStr(90,55,"%");
		u8g.setPrintPos(2, 55);
		u8g.print(h1);
	}
	
	while( u8g.nextPage());
		Delay5Sec();
}

void TempOutdoor(){
    float h2 = dht2.readHumidity();
    float t2 = dht2.readTemperature();
	if (isnan(t2) || isnan(h2)) {
		Serial.println("Failed to read from Outdoor Sensor");
	}
	
	else {
		Serial.print("Outdoor Humidity: "); 
		Serial.print(h2);
		Serial.print(" %");
		Serial.print(" Temperature: "); 
		Serial.print(t2);
		Serial.println(" *C");
	}
	
	u8g.firstPage();
	
	do{
		u8g.setFont(u8g_font_unifont);
		u8g.drawStr(2,10, "Outdoor: Temp");
		u8g.setFont(u8g_font_helvB18);
		u8g.drawStr(90,55,"\260C");
		u8g.setPrintPos(2, 55);
		u8g.print(t2);
	}
	
	while( u8g.nextPage());
		Delay5Sec();
		u8g.firstPage();
		
	do{
		u8g.setFont(u8g_font_unifont);
		u8g.drawStr(2,10, "Outdoor: Humd");
		u8g.setFont(u8g_font_helvB18);
		u8g.drawStr(90,55,"%");
		u8g.setPrintPos(2, 55);
		u8g.print(h2);
	}
	
	while( u8g.nextPage());
		Delay5Sec();
	}

void TempSoil(){
	sensors.requestTemperatures();
	float t3 = (sensors.getTempCByIndex(0));
	Serial.print("Soil Temperature: "); 
	Serial.print(t3);
	Serial.println(" *C");
	u8g.firstPage(); 
	
	do{
		u8g.setFont(u8g_font_unifont);
		u8g.drawStr(2,10, "Soil:    Temp");
		u8g.setFont(u8g_font_helvB18);
		u8g.drawStr(90,55,"\260C");
		u8g.setPrintPos(2, 55);
		u8g.print(t3);
	}
	
	while( u8g.nextPage());
	Delay5Sec();
}

void Switchread(){
	int Switch1Val = digitalRead(Switch1);
	if (Switch1Val == HIGH){
		digitalWrite(relay1, HIGH);
	}
	
	else{
		digitalWrite(relay1, LOW);
	}
}

void Delay5Sec(){
	switchcheckcount = 0;
	while(switchcheckcount < 50){
		switchcheckcount++;
		Switchread();     
		delay(100);
	}
}
