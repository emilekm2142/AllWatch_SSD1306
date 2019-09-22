// SettingsManager.h
#define WiFiPath "WiFi.list"
#ifndef _SETTINGSMANAGER_h
#define _SETTINGSMANAGER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
#include "LinkedList.h"
#include "EEPROM.h"
#include <ESP8266WiFi.h>
#include "FS.h"
#include <ESP8266HTTPClient.h>
#include <Hash.h>
//#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "Dependency.h"
#include "ExtraPeripheralsManager.h"
class BuiltInApplication;
class ApplicationDataHolder {
public:
	char* name;
	std::function<BuiltInApplication*()> creatingFunction;
	bool showOnAppsList = true;
	int iconHeight = NULL;
	int iconWidth = NULL;
	const unsigned char* icon = NULL;
	ApplicationDataHolder() {

	}
	ApplicationDataHolder(char* name, std::function<BuiltInApplication*()>  creatingFunction, bool show=true) {
		this->name = name;
		this->creatingFunction = creatingFunction;
		this->showOnAppsList = show;
	}
	BuiltInApplication* getApplication() {
		return creatingFunction();
	}
};

class SettingsManager//:public Dependency
{
	//https://arduino.stackexchange.com/questions/13545/using-progmem-to-store-array-of-structs
private:
	
	
 protected:

	 class WatchHttpClient
	 {
	 protected:
		 SettingsManager* parent;
	 public:
		 bool _Connect() {
			 parent->wifiManager->ConnectToFirstFittingWiFiNetwork();
			 return parent->wifiManager->WiFiConnected();
		 }
		 void _Disconnect() {
			 parent->wifiManager->Disconnect();
		 }
		 WatchHttpClient(SettingsManager* sm) {
			 parent = sm;
		 }
		 HTTPClient* MakeGetRequest(char* url) {
			 
			 if (!parent->wifiManager->WiFiConnected()) {
				bool success = _Connect();
				if (!success) {
					Serial.println(F("Error... No network available!")); return nullptr
						;
				}
			 }
			 HTTPClient* http = new HTTPClient();

			 http->begin(url);
			 http->GET();

			 return http;
		 }
		 void EndRequest(HTTPClient* r) {
			 r->end();
			 _Disconnect();
		 }

	 };
	 class AppsManager {
		
		
	 public:
		 SettingsManager* parent;
		 LinkedList<ApplicationDataHolder*>* builtInApps = new LinkedList<ApplicationDataHolder*>();
		 BuiltInApplication* currentApplication=NULL;
		 AppsManager(SettingsManager* sm) {
			 parent = sm;
		 }
		 ApplicationDataHolder* getBuiltInApplicationByName(char* name) {
			 for (int i = 0; i < builtInApps->size(); i++) {
				 if (strcmp(builtInApps->get(i)->name, name) == 0) {
					 return builtInApps->get(i);
				 }
			 }
		 }
		 void RedirectStreamOfApps(Print& s) {
			 auto appsDir = parent->SPIFFS->openDir("/apps");
			 while (appsDir.next()) {
				 Serial.println(appsDir.fileName());
				 s.println(appsDir.fileName());
			 }
		 }
		 void RegisterApplication(char* name, std::function<BuiltInApplication*()> creatingFunction, int iconWidth=NULL, int iconHeight=NULL, const unsigned char* icon=NULL, bool show=true) {
			 char filePath[32];
			 snprintf_P(filePath,
				 32,
				 "/apps/%s",
				 name
			 );
			 if (!parent->SPIFFS->exists(filePath)) {
				 Serial.println("creating file: ");
				 Serial.println(filePath);
				auto f =  parent->SPIFFS->open(filePath, "a+");
				f.close();
			 }
			 auto m = new ApplicationDataHolder(name, creatingFunction, show);
			 m->iconHeight = iconHeight;
			 m->iconWidth = iconWidth;
			 m->icon = icon;
			 this->builtInApps->add(m);


		 }
		 void DeleteApplication(char* name) {
			 char filePath[32];
			 snprintf_P(filePath,
				 32,
				 "/apps/%s",
				 name
			 );
			 if (parent->SPIFFS->exists(filePath)) {
				  parent->SPIFFS->remove(filePath);
				 
			 }

		 }
		 fs::File GetConfigForApplication(char* appName, char* mode="a+") {
			 char filePath[32];

			 snprintf_P(filePath,
				 32,
				 "/apps/%s",
				 appName
			 );
			 auto f = parent->SPIFFS->open(filePath, mode);
			 Serial.println();
			 Serial.println(filePath);
			 if (!f) { Serial.println("File was not opened"); }
			 Serial.print("opening file with size: ");
			 Serial.println(f.size());
			 return f;
		 }
		 
		 bool GetKeyFromConfig(char* appName, char* key, char* buffer) {
			 Serial.printf((char*)F("Requested keyL %s"), key);
			 auto f = GetConfigForApplication(appName);
			 while (f.available()) {
				 char keyBuffer[20];
				 char valueBuffer[40];
				 for (int i = 0; i < 40; i++) valueBuffer[i] = '\0';
				 for (int i = 0; i < 20; i++) keyBuffer[i] = '\0';
				 auto keyLength = f.readBytesUntil('=', keyBuffer, 20);
				 keyBuffer[keyLength] = '\0';
				 auto valueLength = f.readBytesUntil('\n', valueBuffer,40);
				 Serial.println("valueBuffer");
				 Serial.println(valueBuffer);

				 Serial.println("keyBuffer");
				 Serial.println(keyBuffer);
				 Serial.println(key);
				 Serial.println(strcmp(key, keyBuffer));

				 valueBuffer[valueLength] = '\0';
				 if (strcmp(key, keyBuffer) == 0) {
					 Serial.println("im going to return: ");
					 Serial.println(valueBuffer);
					 strcpy(buffer, valueBuffer);
					 return true;
				 }
				
			 }
			 return false;
		 }
		 bool KeyExists(char* appName, char* key) {
			 auto f = GetConfigForApplication(appName);
			 while (f.available()) {
				 char keyBuffer[20];
				 char valueBuffer[20];
				 auto keyLength = f.readBytesUntil('=', keyBuffer, 20);
				 auto valueLength = f.readBytesUntil('\n', valueBuffer, 20);
				 if (strcmp(key, keyBuffer) == 0) {
					 return true;
				 }

			 }
			 return false;
		 }
		 void AppendKeyToConfig(char* appName, char* key, char* value) {
			 auto f = GetConfigForApplication(appName);
	
			 f.print(key);
			 f.print('=');
			 f.print(value);
			 f.print('\n');
			 Serial.print("Key added: "); Serial.println(key);
			 f.flush();
			 f.close();
		 }
		 void UpdateKeyInConfig(char* appName, char* key, char* value) {
			 RemoveKeyInConfig(appName, key);
			 AppendKeyToConfig(appName, key, value);
		 }
		 void RemoveKeyInConfig(char* appName, char* key ) {
			 auto f = GetConfigForApplication(appName);
			 char filePath[32];

			 snprintf_P(filePath,
				 32,
				 "/apps/%s",
				 appName
			 );
			 char filePathN[32];

			 snprintf_P(filePathN,
				 32,
				 "/apps/N%s",
				 appName
			 );
			 auto newF = parent->SPIFFS->open(filePath, "a+");
			 while (f.available()) {
				 char keyBuffer[20];
				 char valueBuffer[20];
				 auto keyLength = f.readBytesUntil('=', keyBuffer, 20);
				 auto valueLength = f.readBytesUntil('\n', valueBuffer, 20);
				 if (strcmp(key, keyBuffer) != 0) {
					 
					 newF.print(keyBuffer);
					 newF.print('=');
					 newF.print(valueBuffer);
					 newF.print('\n');


				 }

			 }
			
			 f.close();
			 newF.close();
			 this->parent->SPIFFS->remove(filePath);
			 parent->SPIFFS->rename(filePathN, filePath);
		 }
	 };
	 class WiFiManager {
	 public:
		 const char* filename = "/WiFi.list";
		 SettingsManager* parent;
		 WiFiManager(SettingsManager* parent) {
			 this->parent = parent;
		 }
		 int ScanNetworks() {
			 return parent->w->scanNetworks();
		 }
		 String GetNetwork(int i ) {
			 return parent->w->SSID(i);
		 }
		 int GetNetworkStrength(int i) {
			 return parent->w->RSSI(i);
		 }
		 void AppendWiFiNetwork(fs::FS* SPIFFS, char* ssid, char* password) {
			 Serial.println(ssid);
			 auto f = parent->SPIFFS->open(filename, "a+");
			 if (!f) {
				 Serial.println("error opening");
			 }
			 f.print(ssid);
			 f.print('\r');
			 f.print(password);
			 f.print('\n');

			 f.close();
		 }
		 void ClearList() {
			 this->parent->SPIFFS->remove(filename);
			 auto newF = parent->SPIFFS->open(filename, "a+");
			 newF.close();
		 }
		 void RemoveWiFiNetwork(char* name) {
			
			 char filePathN[32];

			 snprintf_P(filePathN,
				 32,
				 "%sN",
				 filename
			 );
			 auto newF = parent->SPIFFS->open(filePathN, "a+");
			 auto oldF = parent->SPIFFS->open(filename, "r");

			 while (oldF.available()) {
				 char nameBuffer[80];
				 char passBuffer[80];
				 auto nameLength = oldF.readBytesUntil('\r', nameBuffer, 80);
				 auto passLength = oldF.readBytesUntil('\n', passBuffer, 80);
				 nameBuffer[nameLength] = '\0';
				 passBuffer[passLength] = '\0';

				 if (strcmp(nameBuffer, name) == 0) {
					
				 }
				 else {
					 newF.print(nameBuffer);
					 newF.print('\r');
					 newF.print(passBuffer);
					 newF.print('\n');
				 }

			 }


			 oldF.close();
			 newF.close();
			 this->parent->SPIFFS->remove(filename);
			 parent->SPIFFS->rename(filePathN, filename);
		 }
		 bool IsTheNetworkNearby(String& ssid, int networksAmount) {
			 for (int i = 0; i < networksAmount; i++) {
				 if (parent->w->SSID(i) == ssid) return true;
			 }
			 return false;
		 }
		 bool ConnectToFirstFittingWiFiNetwork() {
			 auto f = parent->SPIFFS->open(filename, "r");
			 Serial.println(f.size());
			 int networksAmount = parent->w->scanNetworks();

			 while (f.available()) {
				 auto n = f.readStringUntil('\r');
				 if (IsTheNetworkNearby(n, networksAmount)) {
					 auto p = f.readStringUntil('\n');
					 if (ConnectToWifiSync((char*)n.c_str(), (char*)p.c_str())) return true;
				 }
				 else {
					 f.readStringUntil('\n');
				 }


			 }
			 return false;



		 }
		 std::pair<String,String> GetWiFiNetworkAtIndex(int order) {
			 auto f = parent->SPIFFS->open(filename, "r");
			 Serial.println(f.size());
			 int i = 0;
			 int s = 0;
			 while (i <= order) {
				 if (f.read() == '\n')
					 i++;
				 s++;
				 if (s >= f.size()) {
					 Serial.println("no such network. Exiting");
					 return std::make_pair(String(), String());
				 }

			 }
			 auto n = f.readStringUntil('\r');
			 auto p = f.readStringUntil('\n');
			 Serial.println(n);
			 Serial.println(p);
			
			
			 
			 f.close();
			 return std::make_pair(n, p);

		 }
		 bool ConnectToWiFiUsingSavedCredentials(char* ssid) {
			 auto f = parent->SPIFFS->open(filename, "r");
			 Serial.println(f.size());
			 int i = 0;
			 int s = 0;
			 while (f.available()) {
				 char n[100];
				 char p[100];
				 int nl=f.readBytesUntil('\r', n, 100);
				 int pl=f.readBytesUntil('\n', p, 100);
				 n[nl] = '\0';
				 p[pl] = '\0';
				 if (strcmp(n,ssid)==0) {
					int status= ConnectToWifiSync(ssid, p);
					if (status) {

						f.close();
						return true;
					}
				 }
				 

			 }
			
			 



			 f.close();
			 return false;
		 }
		 bool IsNetworkSaved(fs::FS* SPIFFS, char* ssid) {

		 }
		 int SavedNetworksAmount(fs::FS* SPIFFS) {

		 }
		 void TryToConnectToWifi(char* ssid, char* pass) {
			 parent->w->forceSleepWake();
			 ConnectToFirstFittingWiFiNetwork();

		 }
		 void ConnectToWifiAsync(char* ssid, char* pass) {
			 parent->w->begin(ssid, pass);
		 }
		 bool ConnectToWifiSync(char* ssid, char* pass) {
			 ConnectToWifiAsync(ssid, pass);
			 long startTime = millis();

			 while (!WiFiConnected()) {
				 if (millis() - startTime > 5000) return false;
				 delay(1);
			 }
			 return true;
		 }
		 void Disconnect() {
			 parent->w->disconnect();
			 long startTime = millis();
			 while (WiFiConnected()) {
				 if (millis() - startTime > 5000) return;
				 delay(1);
			 }
		 }
		 bool WiFiConnected() {
			 return parent->w->isConnected();
		 }
	 };
	 bool isSettingsServerOpened = false;

	 

 public:
	 void TestSaveWiFi() {
		
		 Serial.println("testing");
		 this->wifiManager->ConnectToFirstFittingWiFiNetwork();
		
	 }
	 
	 AsyncWebServer* server =nullptr;
	 
	 fs::FS* SPIFFS;
	 ESP8266WiFiClass* w;
	 WatchHttpClient* http = new WatchHttpClient(this);
	 AppsManager* appsManager = new AppsManager(this);
	 WiFiManager* wifiManager = new WiFiManager(this);
	 ExtraPeripheralsManager* extraPeripheralsManager;
	 TimeKepper* tk;
	 SettingsManager(ESP8266WiFiClass* WiFi, fs::FS* SPIFFS, TimeKepper* tk, ExtraPeripheralsManager* e) {
		 extraPeripheralsManager = e;
		 this->SPIFFS = SPIFFS;
		 this->tk = tk;
		// SPIFFS->format();
		 SPIFFS->begin();
		
		 w = WiFi;
		WiFi->mode(WIFI_OFF);
		WiFi->forceSleepBegin();
		w->setAutoReconnect(false);
		





	 }
	// DependenciesHolder* dependencies;
	 void init() {
		
		// auto d = SPIFFS.openDir("asda");
		 
	 }
	 
	 bool WiFiConnected() {
		 return w->isConnected();
	 }
	 void CloseNetwork() {
		 w->softAPdisconnect();
		 w->enableAP(false);
		
	 }
	 void CloseSettingsServer() {
		 if (server != nullptr)
		 delete server;
		 server = nullptr;
	 }
	 void CreateNetwork() {
		 w->forceSleepWake();
		 int st = w->softAP("Watch");
		 delay(1000);
		 Serial.println("new network made");
		 Serial.printf("Status: %d", st);		 
	 }
	 void OpenSettingsServer() {
		 server = new AsyncWebServer(80);
		 server->on("/addWifi", HTTP_GET, [this](AsyncWebServerRequest *request) {
			 if (request->hasParam("ssid") && request->hasParam("password")) {
				 this->wifiManager->AppendWiFiNetwork(this->SPIFFS, (char*)request->getParam("ssid")->value().c_str(), (char*)request->getParam("password")->value().c_str());


				 request->send(200, "text/plain", "OK");
			 }
			 request->send(200, "text/plain", "ERROR");
		 });
		 server->on("/requestWiFi", HTTP_GET, [this](AsyncWebServerRequest *request) {
			 //request->send()
			 Serial.println("requestWifi");
			 request->send(200, "text/plain", "OK");
			 request->send(*SPIFFS, wifiManager->filename, "text/plain");

		 });
		 server->on("/removeAllNetworks", HTTP_GET, [this](AsyncWebServerRequest *request) {
			 wifiManager->ClearList();
			 request->send(*SPIFFS, wifiManager->filename, "text/plain");
		 });
		 server->on("/removeWiFiNetwork", HTTP_GET, [this](AsyncWebServerRequest *request) {
			 wifiManager->RemoveWiFiNetwork((char*)request->getParam("name")->value().c_str());
			 request->send(*SPIFFS, wifiManager->filename, "text/plain");
		 });
		 server->on("/requestApps", HTTP_GET, [this](AsyncWebServerRequest *request) {
			 AsyncResponseStream *response = request->beginResponseStream("text/html");
			 appsManager->RedirectStreamOfApps(*response);
			 request->send(response);
		 });
		 server->on("/updateAppConfig", HTTP_POST, [this](AsyncWebServerRequest *request) {
			 if (request->hasParam("name"))
				 Serial.println((char*)request->getParam("name")->value().c_str());
			 auto f = appsManager->GetConfigForApplication((char*)request->getParam("name")->value().c_str(), "w");
			 f.print((char*)request->getParam("cfg")->value().c_str());
			 f.close();

			 char filePath[32];

			 snprintf_P(filePath,
				 32,
				 "/apps/%s",
				 (char*)request->getParam("name")->value().c_str()
			 );
			 request->send(*SPIFFS, filePath, "text/plain");

		 });
		 server->on("/requestAppSettings", HTTP_GET, [this](AsyncWebServerRequest *request) {
			 if (request->hasParam("name")) {

				 char filePath[32];

				 snprintf_P(filePath,
					 32,
					 "/apps/%s",
					 (char*)request->getParam("name")->value().c_str()
				 );
				 request->send(*SPIFFS, filePath, "text/plain");
			 }



		 });
		 server->on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
			 //request->send()
			 request->send(*SPIFFS, "/index.html", "text/html");


		 });
		 server->serveStatic("/", *SPIFFS, "/");
		 server->begin();
		 // server.begin();

		 isSettingsServerOpened = true;
	 }
	 String header;
	 void SettingsOnLoop() {
		
	 }
	 void SyncTime() {
		 //https://time-watch-service.herokuapp.com/datetime
		 HTTPClient http;
		 char buffer[] = "https://time-watch-service.herokuapp.com/datetime";
		 http.begin(buffer);
		 http.addHeader("Content-Type", "text/plain");
		 int code = http.GET();
		 auto s = http.getStream();
		 char buff[10];
		 int read;
		 read = s.readBytesUntil('\n', buff, 10);
		 buff[read] = '\0';
		 int year = atoi(buff);

		 read = s.readBytesUntil('\n', buff, 10);
		 buff[read] = '\0';
		 int month = atoi(buff);

		 read = s.readBytesUntil('\n', buff, 10);
		 buff[read] = '\0';
		 int day = atoi(buff);

		 read = s.readBytesUntil('\n', buff, 10);
		 buff[read] = '\0';
		 int hour = atoi(buff);

		 read = s.readBytesUntil('\n', buff, 10);
		 buff[read] = '\0';
		 int minute = atoi(buff);

		 read = s.readBytesUntil('\n', buff, 10);
		 buff[read] = '\0';
		 int second = atoi(buff);

		 tk->SetDateTime(year, month, day, hour, minute, second);

	 }
	 //Na kartce
	 void CreateMobileAppliation(char* name,  char* packageName) {
		 char path[256] = "Apps/";
		 strcat(path, name);
		auto f= SPIFFS->open(&path[0], "wb");
		f.write(sizeof(packageName));
		f.write((uint8_t*)packageName, sizeof(packageName));
		Serial.println("Wrote:");
		f.close();
		Serial.println("wrote:");
		auto f2 = SPIFFS->open(&path[0], "rb");
		for (int i = 0; i < f2.size(); i++)
			Serial.print(f2.read());
	 }

};


#endif

