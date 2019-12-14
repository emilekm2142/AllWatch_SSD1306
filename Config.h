#pragma once
#define WATCH_VERSION_NUMBER 1

//Uncomment lines that do not fulfill the preceding questions.

//#define BME
#define BMP


//Do we USE SSD1306 or SSH1106?
#define Renderer_OLED

//Do we use SSD1306?
//#define Renderer_SSD1306
#define SSD1306_PIN_SDA 0 //D5 on wemos d1 mini
#define SSD1306_PIN_SCL 2 //D3 on wemos d1 mini

//Do we use SSH1106?
#define Renderer_SSH1106
#define SSH1106_PIN_SDA 0 //D5 on wemos d1 mini
#define SSH1106_PIN_SCL 2 //D3 on wemos d1 mini

//comment if you do not wish to add any default wifi network
#define APPEND_DEFAULT_WIFI
//set the values accordingly
#define DEFAULT_WIFI_SSID  "VM2828377"
#define DEFAULT_WIFI_PASSWORD  "qjf4vbhqXpNr"


//uncomment if you intend to use a RX or TX pin as a button


//#define USE_TX_RX_AS_GPIO
#define PIN_DOWN  1	
#define PIN_UP  3
//uncomment if you want to inverse HIGH to LOW
//#define INVERT

#define RTC_AVAILABLE
#define RTC_SDA 0
#define RTC_SCL 2

#define DEBOUNCE_TIME 200

//#define OTA_AVAILABLE
//#define START_WITH_OTA_PORT
#define OTA_WIFI_SSID "VM2828377"
#define OTA_WIFI_PASSWORD "qjf4vbhqXpNr"

#define TEMPERATURE_AVAILABLE
#define HOME_SCREEN_TEMPERATURE_READINGS

#define DISPLAY_SECONDS
#define ENERGY_SAVING
//#define WAKE_UP_FROM_SLEEP_AUTOMATICALLY

//#define RUN_TESTS
#define TESTS_SSID "VM2828377"
#define TESTS_PASSWORD  "qjf4vbhqXpNr"

#define BUZZER
#define BUZZER_PIN 4

//#define SHUTDOWN_ON_LOW_VOLTAGE