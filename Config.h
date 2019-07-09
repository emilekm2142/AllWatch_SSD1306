#pragma once
//Uncomment lines that do not fulfill the preceding questions.

//Do we USE SSD1306 or SSH1106?
#define Renderer_OLED

//Do we use SSD1306?
#define Renderer_SSD1306

//Do we use SSH1106?
//#define Renderer_SSH1106


//Set buttons
#define PIN_DOWN  4
#define PIN_UP  5


//comment if you do not wish to add any default wifi network
#define APPEND_DEFAULT_WIFI
//set the values accordingly
#define DEFAULT_WIFI_SSID  "test"
#define DEFAULT_WIFI_PASSWORD  "123456789"
