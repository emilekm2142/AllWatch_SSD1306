#pragma once
//Uncomment lines that do not fulfill the preceding questions.

//Do we USE SSD1306 or SSH1106?
#define Renderer_OLED

//Do we use SSD1306?
//#define Renderer_SSD1306

//Do we use SSH1106?
#define Renderer_SSH1106


//comment if you do not wish to add any default wifi network
#define APPEND_DEFAULT_WIFI
//set the values accordingly
#define DEFAULT_WIFI_SSID  "test"
#define DEFAULT_WIFI_PASSWORD  "123456789"

 //uncomment if you intend to use a RX or TX pin as a button
//#define USE_TX_RX_AS_GPIO
//Set buttons	//Set buttons
#define PIN_DOWN  1	
#define PIN_UP  3
//#define INVERT
