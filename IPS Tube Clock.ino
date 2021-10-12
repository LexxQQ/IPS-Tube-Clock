/*
	Name:       IPS Tube Clock.ino
	Created:	08.10.2021 1:27:04
	Author:     ELEMENTS\VitaliyN
*/

#include "Settings.h"
#include "Images.h"
//#include "iv-2.h"
#include "in-14.h"
//#include "z568m.h"

// Include the jpeg decoder library
#include <TJpg_Decoder.h>

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
#include "Free_Fonts.h" // Include the header file attached to this sketch
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

		uint8_t sec0 = nowTimeInfo->tm_sec / 10;
		uint8_t sec1 = nowTimeInfo->tm_sec % 10;

		TJpgDec.drawJpg(0, 0, GetDigitImage(sec0), GetDigitSize(sec0));
		/*tft.setFreeFont(FSB9);
		tft.drawString(GetDateString(nowTimeInfo), 0, 0);*/

		TJpgDec.drawJpg(135, 0, GetDigitImage(sec1), GetDigitSize(sec1));
		/*tft.setFreeFont(FM9);
		tft.drawString(GetTimeString(nowTimeInfo), 135, 0);*/
	}
}

#pragma region tft_output
// This next function will be called during decoding of the jpeg file to
// render each block to the TFT.  If you use a different TFT library
// you will need to adapt this function to suit.
bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
	if (y >= tft.height()) return 0; // Stop further decoding as image is running off bottom of screen	
	tft.pushImage(x, y, w, h, bitmap); // This function will clip the image block rendering automatically at the TFT boundaries

									   // This might work instead if you adapt the sketch to use the Adafruit_GFX library
									   // tft.drawRGBBitmap(x, y, bitmap, w, h);	
	return 1; // Return 1 to decode next block
}
#pragma endregion

#pragma region configModeCallback
// gets called when WiFiManager enters configuration mode
void configModeCallback(WiFiManager* myWiFiManager) {
	Serial.println("Entered config mode");
	Serial.println(WiFi.softAPIP());
	// if you used auto generated SSID, print it
	Serial.println(myWiFiManager->getConfigPortalSSID());

	// ticker.attach(0.2, tick); // entered config mode, make led toggle faster
}
#pragma endregion

#pragma region InitWifi
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

	/*tft.pushImage(200, 10, 24, 19, wifi_ok_24x19);*/
	TJpgDec.drawJpg(214, 2, wifi_ok_jpg_24x19, sizeof(wifi_ok_jpg_24x19));
}
#pragma endregion

#pragma region InitPorts
void InitPorts() {
	pinMode(LED_BUILTIN, OUTPUT);
}
#pragma endregion

#pragma region InitTft
void InitTft() {
	// The jpeg image can be scaled by a factor of 1, 2, 4, or 8
	TJpgDec.setJpgScale(1);

	// The byte order can be swapped (set true for TFT_eSPI)
	/*TJpgDec.setSwapBytes(true);*/

	// The decoder must be given the exact name of the rendering function above
	TJpgDec.setCallback(tft_output);

	analogWrite(LED_BUILTIN, TFT_BRIGHTNESS); // 0-255

	tft.begin();     // initialize a ST7789 chip
	tft.setSwapBytes(true); // Swap the byte order for pushImage() - corrects endianness

	// tft.setRotation(1);

	tft.fillScreen(TFT_BLACK);

	// tft.pushImage(200, 10, 24, 19, girls_bmp_240x240);
	TJpgDec.drawJpg(0, 0, girls_jpg_240x240, sizeof(girls_jpg_240x240));

	/*tft.pushImage(200, 10, 24, 19, wifi_nok_24x19);*/
	TJpgDec.drawJpg(214, 2, wifi_nok_jpg_24x19, sizeof(wifi_nok_jpg_24x19));
}
#pragma endregion

#pragma region RefreshTime
void RefreshTime() {
	// configTime(TZ_SEC, DST_SEC, "europe.pool.ntp.org", "time.nist.gov");	// Get time from network time service
	// configTime(TZ_Etc_GMTp3, "europe.pool.ntp.org", "time.nist.gov");	// Get time from network time service
	configTime(Timezone, "europe.pool.ntp.org", "time.nist.gov");	// Get time from network time service

	nowTime = time(nullptr);
	nowTimeInfo = localtime(&nowTime);
}
#pragma endregion

#pragma region GetDigitImage
const uint8_t* GetDigitImage(uint8_t digit) {
	switch (digit)
	{
	case 0:
		return dig_0_jpg;

	case 1:
		return dig_1_jpg;

	case 2:
		return dig_2_jpg;

	case 3:
		return dig_3_jpg;

	case 4:
		return dig_4_jpg;

	case 5:
		return dig_5_jpg;

	case 6:
		return dig_6_jpg;

	case 7:
		return dig_7_jpg;

	case 8:
		return dig_8_jpg;

	case 9:
		return dig_9_jpg;

	default:
		return dig_0_jpg;
		break;
	}
}
#pragma endregion

#pragma region GetDigitSize
int GetDigitSize(uint8_t digit) {
	switch (digit)
	{
	case 0:
		return sizeof(dig_0_jpg);

	case 1:
		return sizeof(dig_1_jpg);

	case 2:
		return sizeof(dig_2_jpg);

	case 3:
		return sizeof(dig_3_jpg);

	case 4:
		return sizeof(dig_4_jpg);

	case 5:
		return sizeof(dig_5_jpg);

	case 6:
		return sizeof(dig_6_jpg);

	case 7:
		return sizeof(dig_7_jpg);

	case 8:
		return sizeof(dig_8_jpg);

	case 9:
		return sizeof(dig_9_jpg);

	default:
		return sizeof(dig_0_jpg);
		break;
	}
}
#pragma endregion

#pragma region GetTimeString
String GetTimeString(struct tm* timeInfo) {
	String result = "";

	if (timeInfo->tm_hour < 10) {
		result += "0";
	}
	result += String(timeInfo->tm_hour);

	result += ":";

	if (timeInfo->tm_min < 10) {
		result += "0";
	}
	result += String(timeInfo->tm_min);

	result += ":";

	if (timeInfo->tm_sec < 10) {
		result += "0";
	}
	result += String(timeInfo->tm_sec);

	return result;
}
#pragma endregion

#pragma region GetDateString
String GetDateString(struct tm* timeInfo) {
	String result = "";

	if (timeInfo->tm_mday < 10) {
		result += "0";
	}
	result += String(timeInfo->tm_mday);

	result += ".";

	if ((1 + timeInfo->tm_mon) < 10) {
		result += "0";
	}
	result += String(1 + timeInfo->tm_mon);

	result += ".";

	if (timeInfo->tm_year < 10) {
		result += "0";
	}
	result += String(1900 + timeInfo->tm_year);

	return result;
}
#pragma endregion