#pragma once
String OPEN_WEATHER_MAP_APP_ID = "a1d8ddb5d4eb3a32e72c2ccfd620ce9d";	// Sign up here to get an API key: https://docs.thingpulse.com/how-tos/openweathermap-key/
/*
  Go to https://openweathermap.org/find?q= and search for a location. Go through the
  result set and select the entry closest to the actual location you want to display
  data for. It'll be a URL like https://openweathermap.org/city/2657896. The number
  at the end is what you assign to the constant below.
*/
String OPEN_WEATHER_MAP_LOCATION_ID = "711660";	// Boryspil, UA
// String OPEN_WEATHER_MAP_LOCATION_ID = "361291";	// Hurghada, EG
// String OPEN_WEATHER_MAP_LOCATION_ID = "349340";	// Sharm el Sheikh, EG


// Pick a language code from this list:
// Arabic - ar, Bulgarian - bg, Catalan - ca, Czech - cz, German - de, Greek - el,
// English - en, Persian (Farsi) - fa, Finnish - fi, French - fr, Galician - gl,
// Croatian - hr, Hungarian - hu, Italian - it, Japanese - ja, Korean - kr,
// Latvian - la, Lithuanian - lt, Macedonian - mk, Dutch - nl, Polish - pl,
// Portuguese - pt, Romanian - ro, Russian - ru, Swedish - se, Slovak - sk,
// Slovenian - sl, Spanish - es, Turkish - tr, Ukrainian - ua, Vietnamese - vi,
// Chinese Simplified - zh_cn, Chinese Traditional - zh_tw.
String OPEN_WEATHER_MAP_LANGUAGE = "ua";

//#include <TZ.h>
//#define Timezone			TZ_Europe_Kiev		// (utc+) TZ in hours //  in 
#define Timezone			PSTR("EET-2EEST,M3.5.0/3,M10.5.0/4")		// (utc+) TZ in hours //  in 

#define TFT_BRIGHTNESS	128 // 0-255

//				D0 - RESET
//				D1 - D/C
#define TFT0_CS	D2
//				D5 - SCK
//				D7 - SDA (MOSI)

//#define test0	A0
#define test1	D3 //+
//#define test2	D6
//#define test3	D8