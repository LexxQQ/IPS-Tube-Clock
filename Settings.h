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

#define TZ		3		// (utc+) TZ in hours