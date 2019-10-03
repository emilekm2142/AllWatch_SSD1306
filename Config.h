#pragma once
#define WATCH_VERSION_NUMBER 1

//Uncomment lines that do not fulfill the preceding questions.




//Do we USE SSD1306 or SSH1106?
#define Renderer_OLED

//Do we use SSD1306?
#define Renderer_SSD1306
#define SSD1306_PIN_SDA 14 //D5 on wemos d1 mini
#define SSD1306_PIN_SCL 0 //D3 on wemos d1 mini
//Do we use SSH1106?
//#define Renderer_SSH1106


//comment if you do not wish to add any default wifi network
#define APPEND_DEFAULT_WIFI
//set the values accordingly
#define DEFAULT_WIFI_SSID  "test"
#define DEFAULT_WIFI_PASSWORD  "123456789"


//uncomment if you intend to use a RX or TX pin as a button


//#define USE_TX_RX_AS_GPIO
#define PIN_DOWN  1	
#define PIN_UP  3
//uncomment if you want to inverse HIGH to LOW
//#define INVERT

//#define RTC_AVAILABLE

#define DEBOUNCE_TIME 50


//#define START_WITH_OTA_PORT
#define OTA_WIFI_SSID "test"
#define OTA_WIFI_PASSWORD "123456789"