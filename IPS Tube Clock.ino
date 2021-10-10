/*
	Name:       IPS Tube Clock.ino
	Created:	08.10.2021 1:27:04
	Author:     ELEMENTS\VitaliyN
*/

#include "Settings.h"
//#include "Images.h"
#include "iv-2.h"

#pragma region time
#include <time.h>                       // time() ctime()
#include <sys/time.h>                   // struct timeval
#include <coredecls.h>                  // settimeofday_cb()

#define Timezone_min		Timezone * 60	// 180	// use 60mn for summer time in some countries  
#define DaylightOffset_sec	Timezone_min * 60

time_t nowTime;
struct tm* nowTimeInfo;
#pragma endregion

#pragma region wifi
#include <ESPWiFi.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager  
#pragma endregion

#pragma region TFT
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();
#pragma endregion

#pragma region timers
long timeDateChange = 0;
const long timeDateChangeIntervalMillis = 1 * 1000L; // sec * 1000L
#pragma endregion

void setup(void) {
	Serial.begin(9600);
	Serial.print(F("IPS Tube Clock - sutup..."));

	InitPorts();

	InitTft();

	InitWifi();

	Serial.println("done");
	delay(1000);
}

uint16_t digit = 1;

void loop() {
	if (millis() - timeDateChange > timeDateChangeIntervalMillis) {
		timeDateChange = millis();
		/* code */
		RefreshTime();

		Serial.print(nowTimeInfo->tm_hour);
		Serial.print(":");
		Serial.print(nowTimeInfo->tm_min);
		Serial.print(":");
		Serial.println(nowTimeInfo->tm_sec);

		if (digit++ >= 9) {
			digit = 1;
		}

		tft.pushImage(0, 0, 135, 240, getDigitImage(digit));



	}
}

// gets called when WiFiManager enters configuration mode
void configModeCallback(WiFiManager* myWiFiManager) {
	Serial.println("Entered config mode");
	Serial.println(WiFi.softAPIP());
	// if you used auto generated SSID, print it
	Serial.println(myWiFiManager->getConfigPortalSSID());

	// ticker.attach(0.2, tick); // entered config mode, make led toggle faster
}

void InitWifi() {
	/*WiFi.begin(WIFI_SSID, WIFI_PWD);*/
	WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
	delay(1000);
	WiFi.mode(WIFI_STA);        //Only Station No AP, This line hides the viewing of ESP as wifi hotspot
	WiFi.setAutoReconnect(true);
	// WiFi.begin(ssid, password);     //Connect to your WiFi router
	Serial.println("Connecting to WIFI...");
	//while (WiFi.status() != WL_CONNECTED) {
	//	delay(500);
	//	Serial.print(".");
	//}

	//Serial.println("");
	//Serial.print("Connected to ");
	//Serial.println(ssid);
	//Serial.print("IP address: ");
	//Serial.println(WiFi.localIP());  //IP address assigned to your ESP

	WiFiManager wm;	// Local intialization. Once its business is done, there is no need to keep it around
	//if (digitalRead(MainButtonPin) == LOW) {
	//	wm.resetSettings();	// reset settings
	//}
	wm.setAPCallback(configModeCallback); // set callback that gets called when connecting to previous WiFi fails, and enters Access Point mode
	wm.setTimeout(120);

	// This is sometimes necessary, it is still unknown when and why this is needed but it may solve some race condition or bug in esp SDK/lib
	wm.setCleanConnect(true); // disconnect before connect, clean connect

	// fetches ssid and pass and tries to connect
	// if it does not connect it starts an access point with the specified name
	// and goes into a blocking loop awaiting configuration
	if (!wm.autoConnect("Lexus_IPS_Tube_Clock", "1122334455")) {
		// This is sometimes necessary, it is still unknown when and why this is needed but it may solve some race condition or bug in esp SDK/lib
		wm.setCleanConnect(true); // disconnect before connect, clean connect

		Serial.println("failed to connect and hit timeout");
		delay(1000);
		ESP.restart(); // reset and try again, or maybe put it to deep sleep
		delay(1000);
	}

	Serial.println("connected... yeey :)"); // if you get here you have connected to the WiFi

	//int counter = 0;
	//while (WiFi.status() != WL_CONNECTED) {
	//	delay(500);
	//	//Serial.print(".");
	//	display.clear();
	//	display.drawString(64, 10, "Connecting to WiFi");
	//	display.drawXbm(46, 30, 8, 8, counter % 3 == 0 ? activeSymbole : inactiveSymbole);
	//	display.drawXbm(60, 30, 8, 8, counter % 3 == 1 ? activeSymbole : inactiveSymbole);
	//	display.drawXbm(74, 30, 8, 8, counter % 3 == 2 ? activeSymbole : inactiveSymbole);
	//	display.display();

	//	counter++;
	//}	
}

void RefreshTime() {
	// configTime(TZ_SEC, DST_SEC, "europe.pool.ntp.org", "time.nist.gov");	// Get time from network time service
	// configTime(TZ_Etc_GMTp3, "europe.pool.ntp.org", "time.nist.gov");	// Get time from network time service
	configTime(Timezone, "europe.pool.ntp.org", "time.nist.gov");	// Get time from network time service

	nowTime = time(nullptr);
	nowTimeInfo = localtime(&nowTime);
}

void InitPorts() {
	pinMode(LED_BUILTIN, OUTPUT);
}

void InitTft() {
	analogWrite(LED_BUILTIN, TFT_BRIGHTNESS); // 0-255

	tft.begin();     // initialize a ST7789 chip
	tft.setSwapBytes(true); // Swap the byte order for pushImage() - corrects endianness

	tft.fillScreen(TFT_BLACK);

	/*tft.pushImage(0, 0, 240, 240, girls);*/
}

const uint16_t* getDigitImage(uint16_t digit) {
	switch (digit)
	{
	/*case 0:
		return dig_0;*/

	case 1:
		return dig_1;

	case 2:
		return dig_2;

	case 3:
		return dig_3;

	case 4:
		return dig_4;

	case 5:
		return dig_5;

	case 6:
		return dig_6;

	case 7:
		return dig_7;

	case 8:
		return dig_8;

	case 9:
		return dig_9;

	default:
		return dig_8;
		break;
	}
}