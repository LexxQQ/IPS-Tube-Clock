/*
	Name:       IPS Tube Clock.ino
	Created:	08.10.2021 1:27:04
	Author:     ELEMENTS\VitaliyN
*/

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include "Settings.h"

#pragma region init time
#include <time.h>                       // time() ctime()
#include <sys/time.h>                   // struct timeval
#include <coredecls.h>                  // settimeofday_cb()
#include <TZ.h>

//#define DST_MN	TZ_Europe_Kiev * 60	// 180	// use 60mn for summer time in some countries  

time_t nowTime;
struct tm* nowTimeInfo;
#pragma endregion

#pragma region init TFT
#if defined(ARDUINO_FEATHER_ESP32) // Feather Huzzah32
#define TFT_CS         14
#define TFT_RST        15
#define TFT_DC         32

#elif defined(ESP8266)
#define TFT0_CS         1U	// Chip selection
#define TFT1_CS         3U	// Chip selection
#define TFT2_CS         5U	// Chip selection
#define TFT3_CS         4U	// Chip selection
#define TFT4_CS         0U	// Chip selection
#define TFT5_CS         2U	// Chip selection

#define TFT_DC         D0	// Data|Command
#define TFT_RST        A0	// Reset

// CLK	PIN_SPI_SCK
// MOSI	PIN_SPI_MOSI

#else
// For the breakout board, you can use any 2 or 3 pins.
// These pins will also work for the 1.8" TFT shield.
#define TFT_CS        10
#define TFT_RST        9 // Or set to -1 and connect to Arduino RESET pin
#define TFT_DC         8
#endif

// OPTION 1 (recommended) is to use the HARDWARE SPI pins, which are unique to each board and not reassignable.
// For Arduino Uno: MOSI = pin 11 and SCLK = pin 13.
// This is the fastest mode of operation and is required if using the breakout board's microSD card.

// For 1.44" and 1.8" TFT with ST7735 use:
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

// For 1.14", 1.3", 1.54", 1.69", and 2.0" TFT with ST7789:
//Adafruit_ST7789 tft0 = Adafruit_ST7789(TFT0_CS, TFT_DC, TFT_RST);
//Adafruit_ST7789 tft1 = Adafruit_ST7789(TFT1_CS, TFT_DC, TFT_RST);
Adafruit_ST7789 tft2 = Adafruit_ST7789(TFT2_CS, TFT_DC, TFT_RST);
//Adafruit_ST7789 tft3 = Adafruit_ST7789(TFT3_CS, TFT_DC, TFT_RST);
//Adafruit_ST7789 tft4 = Adafruit_ST7789(TFT4_CS, TFT_DC, TFT_RST);
//Adafruit_ST7789 tft5 = Adafruit_ST7789(TFT5_CS, TFT_DC, TFT_RST);

// OPTION 2 lets you interface the display using ANY TWO or THREE PINS,
// tradeoff being that performance is not as fast as hardware SPI above.
//#define TFT_MOSI 11  // Data out
//#define TFT_SCLK 13  // Clock out

// For ST7735-based displays, we will use this call
//Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

// OR for the ST7789-based displays, we will use this call
//Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);  
#pragma endregion

#pragma region timers
long timeDateChange = 0;
const long timeDateChangeIntervalMillis = 1 * 1000L; // sec * 1000L
#pragma endregion

float p = 3.1415926;

void setup(void) {
	Serial.begin(9600);
	Serial.print(F("Hello! ST77xx TFT Test"));
	//InitTime();
	InitPorts();

	// initializer 1.14" 240x135 TFT:
	//tft0.init(240, 240);           // Init ST7789 240x135

	// SPI speed defaults to SPI_DEFAULT_FREQ defined in the library, you can override it here
	// Note that speed allowable depends on chip and quality of wiring, if you go too fast, you
	// may end up with a black screen some times, or all the time.
	//tft0.setSPISpeed(40000000);

	Serial.println(F("Initialized"));

	/*uint16_t time = millis();
	tft0.fillScreen(ST77XX_BLACK);
	time = millis() - time;
	
	Serial.println(time, DEC);
	delay(500);*/
	
	Serial.println("done");
	delay(1000);
}

void loop() {	
	if (millis() - timeDateChange > timeDateChangeIntervalMillis) {
		timeDateChange = millis();
		/* code */
		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

		//digitalWrite(TFT0_CS, !digitalRead(TFT0_CS));
		//digitalWrite(TFT1_CS, !digitalRead(TFT1_CS));
		digitalWrite(TFT2_CS, !digitalRead(TFT2_CS));
		digitalWrite(TFT3_CS, !digitalRead(TFT3_CS));
		digitalWrite(TFT4_CS, !digitalRead(TFT4_CS));
		//digitalWrite(TFT5_CS, !digitalRead(TFT5_CS));

		
	}
}

void InitTime() {
	nowTime = time(nullptr);
	nowTimeInfo = localtime(&nowTime);

	// nowTimeInfo->tm_year
}

void InitPorts() {
	pinMode(LED_BUILTIN, OUTPUT);
	//pinMode(TFT0_CS, OUTPUT);
	//pinMode(TFT1_CS, OUTPUT);
	pinMode(TFT2_CS, OUTPUT);
	pinMode(TFT3_CS, OUTPUT);
	pinMode(TFT4_CS, OUTPUT);
	//pinMode(TFT5_CS, OUTPUT);
}