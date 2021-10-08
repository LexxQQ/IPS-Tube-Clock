/*
	Name:       IPS Tube Clock.ino
	Created:	08.10.2021 1:27:04
	Author:     ELEMENTS\VitaliyN
*/

#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
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
Adafruit_ST7789 tft0 = Adafruit_ST7789(TFT0_CS, TFT_DC, TFT_RST);
//Adafruit_ST7789 tft1 = Adafruit_ST7789(TFT1_CS, TFT_DC, TFT_RST);
//Adafruit_ST7789 tft2 = Adafruit_ST7789(TFT2_CS, TFT_DC, TFT_RST);
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
	InitPorts();

	// Use this initializer if using a 1.8" TFT screen:
	//tft0.initR(INITR_BLACKTAB);      // Init ST7735S chip, black tab

	// OR use this initializer if using a 1.8" TFT screen with offset such as WaveShare:
	// tft0.initR(INITR_GREENTAB);      // Init ST7735S chip, green tab

	// OR use this initializer (uncomment) if using a 1.44" TFT:
	//tft0.initR(INITR_144GREENTAB); // Init ST7735R chip, green tab

	// OR use this initializer (uncomment) if using a 0.96" 160x80 TFT:
	//tft0.initR(INITR_MINI160x80);  // Init ST7735S mini display

	// OR use this initializer (uncomment) if using a 1.3" or 1.54" 240x240 TFT:
	//tft0.init(240, 240);           // Init ST7789 240x240

	// OR use this initializer (uncomment) if using a 1.69" 280x240 TFT:
	//tft0.init(240, 280);           // Init ST7789 280x240

	// OR use this initializer (uncomment) if using a 2.0" 320x240 TFT:
	//tft0.init(240, 320);           // Init ST7789 320x240

	// OR use this initializer (uncomment) if using a 1.14" 240x135 TFT:
	tft0.init(240, 240);           // Init ST7789 240x135

	// SPI speed defaults to SPI_DEFAULT_FREQ defined in the library, you can override it here
	// Note that speed allowable depends on chip and quality of wiring, if you go too fast, you
	// may end up with a black screen some times, or all the time.
	//tft0.setSPISpeed(40000000);

	Serial.println(F("Initialized"));

	uint16_t time = millis();
	tft0.fillScreen(ST77XX_BLACK);
	time = millis() - time;

	Serial.println(time, DEC);
	delay(500);

	// large block of text
	tft0.fillScreen(ST77XX_BLACK);
	testdrawtext("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", ST77XX_WHITE);
	delay(1000);

	// tft print function!
	tftPrintTest();
	delay(4000);

	// a single pixel
	tft0.drawPixel(tft0.width() / 2, tft0.height() / 2, ST77XX_GREEN);
	delay(500);

	// line draw test
	testlines(ST77XX_YELLOW);
	delay(500);

	// optimized lines
	testfastlines(ST77XX_RED, ST77XX_BLUE);
	delay(500);

	testdrawrects(ST77XX_GREEN);
	delay(500);

	testfillrects(ST77XX_YELLOW, ST77XX_MAGENTA);
	delay(500);

	tft0.fillScreen(ST77XX_BLACK);
	testfillcircles(10, ST77XX_BLUE);
	testdrawcircles(10, ST77XX_WHITE);
	delay(500);

	testroundrects();
	delay(500);

	testtriangles();
	delay(500);

	mediabuttons();
	delay(500);

	Serial.println("done");
	delay(1000);
}

void loop() {
	tft0.invertDisplay(true);
	delay(500);
	tft0.invertDisplay(false);
	delay(500);

	if (millis() - timeDateChange > timeDateChangeIntervalMillis) {
		timeDateChange = millis();
		/* code */
		/*digitalWrite(TFT0_CS, !digitalRead(TFT0_CS));
		digitalWrite(TFT1_CS, !digitalRead(TFT1_CS));
		digitalWrite(TFT2_CS, !digitalRead(TFT2_CS));
		digitalWrite(TFT3_CS, !digitalRead(TFT3_CS));
		digitalWrite(TFT4_CS, !digitalRead(TFT4_CS));
		digitalWrite(TFT5_CS, !digitalRead(TFT5_CS));*/

		digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
	}
}

void InitTime() {
	nowTime = time(nullptr);
	nowTimeInfo = localtime(&nowTime);

	// nowTimeInfo->tm_year
}

void InitPorts() {
	pinMode(LED_BUILTIN, OUTPUT);
	/*pinMode(TFT0_CS, OUTPUT);
	pinMode(TFT1_CS, OUTPUT);
	pinMode(TFT2_CS, OUTPUT);
	pinMode(TFT3_CS, OUTPUT);
	pinMode(TFT4_CS, OUTPUT);
	pinMode(TFT5_CS, OUTPUT);*/
}

void testlines(uint16_t color) {
	tft0.fillScreen(ST77XX_BLACK);
	for (int16_t x = 0; x < tft0.width(); x += 6) {
		tft0.drawLine(0, 0, x, tft0.height() - 1, color);
		delay(0);
	}
	for (int16_t y = 0; y < tft0.height(); y += 6) {
		tft0.drawLine(0, 0, tft0.width() - 1, y, color);
		delay(0);
	}

	tft0.fillScreen(ST77XX_BLACK);
	for (int16_t x = 0; x < tft0.width(); x += 6) {
		tft0.drawLine(tft0.width() - 1, 0, x, tft0.height() - 1, color);
		delay(0);
	}
	for (int16_t y = 0; y < tft0.height(); y += 6) {
		tft0.drawLine(tft0.width() - 1, 0, 0, y, color);
		delay(0);
	}

	tft0.fillScreen(ST77XX_BLACK);
	for (int16_t x = 0; x < tft0.width(); x += 6) {
		tft0.drawLine(0, tft0.height() - 1, x, 0, color);
		delay(0);
	}
	for (int16_t y = 0; y < tft0.height(); y += 6) {
		tft0.drawLine(0, tft0.height() - 1, tft0.width() - 1, y, color);
		delay(0);
	}

	tft0.fillScreen(ST77XX_BLACK);
	for (int16_t x = 0; x < tft0.width(); x += 6) {
		tft0.drawLine(tft0.width() - 1, tft0.height() - 1, x, 0, color);
		delay(0);
	}
	for (int16_t y = 0; y < tft0.height(); y += 6) {
		tft0.drawLine(tft0.width() - 1, tft0.height() - 1, 0, y, color);
		delay(0);
	}
}

void testdrawtext(char* text, uint16_t color) {
	tft0.setCursor(0, 0);
	tft0.setTextColor(color);
	tft0.setTextWrap(true);
	tft0.print(text);
}

void testfastlines(uint16_t color1, uint16_t color2) {
	tft0.fillScreen(ST77XX_BLACK);
	for (int16_t y = 0; y < tft0.height(); y += 5) {
		tft0.drawFastHLine(0, y, tft0.width(), color1);
	}
	for (int16_t x = 0; x < tft0.width(); x += 5) {
		tft0.drawFastVLine(x, 0, tft0.height(), color2);
	}
}

void testdrawrects(uint16_t color) {
	tft0.fillScreen(ST77XX_BLACK);
	for (int16_t x = 0; x < tft0.width(); x += 6) {
		tft0.drawRect(tft0.width() / 2 - x / 2, tft0.height() / 2 - x / 2, x, x, color);
	}
}

void testfillrects(uint16_t color1, uint16_t color2) {
	tft0.fillScreen(ST77XX_BLACK);
	for (int16_t x = tft0.width() - 1; x > 6; x -= 6) {
		tft0.fillRect(tft0.width() / 2 - x / 2, tft0.height() / 2 - x / 2, x, x, color1);
		tft0.drawRect(tft0.width() / 2 - x / 2, tft0.height() / 2 - x / 2, x, x, color2);
	}
}

void testfillcircles(uint8_t radius, uint16_t color) {
	for (int16_t x = radius; x < tft0.width(); x += radius * 2) {
		for (int16_t y = radius; y < tft0.height(); y += radius * 2) {
			tft0.fillCircle(x, y, radius, color);
		}
	}
}

void testdrawcircles(uint8_t radius, uint16_t color) {
	for (int16_t x = 0; x < tft0.width() + radius; x += radius * 2) {
		for (int16_t y = 0; y < tft0.height() + radius; y += radius * 2) {
			tft0.drawCircle(x, y, radius, color);
		}
	}
}

void testtriangles() {
	tft0.fillScreen(ST77XX_BLACK);
	uint16_t color = 0xF800;
	int t;
	int w = tft0.width() / 2;
	int x = tft0.height() - 1;
	int y = 0;
	int z = tft0.width();
	for (t = 0; t <= 15; t++) {
		tft0.drawTriangle(w, y, y, x, z, x, color);
		x -= 4;
		y += 4;
		z -= 4;
		color += 100;
	}
}

void testroundrects() {
	tft0.fillScreen(ST77XX_BLACK);
	uint16_t color = 100;
	int i;
	int t;
	for (t = 0; t <= 4; t += 1) {
		int x = 0;
		int y = 0;
		int w = tft0.width() - 2;
		int h = tft0.height() - 2;
		for (i = 0; i <= 16; i += 1) {
			tft0.drawRoundRect(x, y, w, h, 5, color);
			x += 2;
			y += 3;
			w -= 4;
			h -= 6;
			color += 1100;
		}
		color += 100;
	}
}

void tftPrintTest() {
	tft0.setTextWrap(false);
	tft0.fillScreen(ST77XX_BLACK);
	tft0.setCursor(0, 30);
	tft0.setTextColor(ST77XX_RED);
	tft0.setTextSize(1);
	tft0.println("Hello World!");
	tft0.setTextColor(ST77XX_YELLOW);
	tft0.setTextSize(2);
	tft0.println("Hello World!");
	tft0.setTextColor(ST77XX_GREEN);
	tft0.setTextSize(3);
	tft0.println("Hello World!");
	tft0.setTextColor(ST77XX_BLUE);
	tft0.setTextSize(4);
	tft0.print(1234.567);
	delay(1500);
	tft0.setCursor(0, 0);
	tft0.fillScreen(ST77XX_BLACK);
	tft0.setTextColor(ST77XX_WHITE);
	tft0.setTextSize(0);
	tft0.println("Hello World!");
	tft0.setTextSize(1);
	tft0.setTextColor(ST77XX_GREEN);
	tft0.print(p, 6);
	tft0.println(" Want pi?");
	tft0.println(" ");
	tft0.print(8675309, HEX); // print 8,675,309 out in HEX!
	tft0.println(" Print HEX!");
	tft0.println(" ");
	tft0.setTextColor(ST77XX_WHITE);
	tft0.println("Sketch has been");
	tft0.println("running for: ");
	tft0.setTextColor(ST77XX_MAGENTA);
	tft0.print(millis() / 1000);
	tft0.setTextColor(ST77XX_WHITE);
	tft0.print(" seconds.");
}

void mediabuttons() {
	// play
	tft0.fillScreen(ST77XX_BLACK);
	tft0.fillRoundRect(25, 10, 78, 60, 8, ST77XX_WHITE);
	tft0.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_RED);
	delay(500);
	// pause
	tft0.fillRoundRect(25, 90, 78, 60, 8, ST77XX_WHITE);
	tft0.fillRoundRect(39, 98, 20, 45, 5, ST77XX_GREEN);
	tft0.fillRoundRect(69, 98, 20, 45, 5, ST77XX_GREEN);
	delay(500);
	// play color
	tft0.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_BLUE);
	delay(50);
	// pause color
	tft0.fillRoundRect(39, 98, 20, 45, 5, ST77XX_RED);
	tft0.fillRoundRect(69, 98, 20, 45, 5, ST77XX_RED);
	// play color
	tft0.fillTriangle(42, 20, 42, 60, 90, 40, ST77XX_GREEN);
}