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
#include <ESP8266httpUpdate.h>
#include "TimeKepper.h"
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

#include "FS.h"
#include "Config.h"
#include <ESP8266HTTPClient.h>
#include <Hash.h>
//#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

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
			 bool foundNetwork = parent->wifiManager->ConnectToFirstFittingWiFiNetwork();
			 if (foundNetwork) {
				 int wt = millis();
				 while (WiFi.status()!=WL_CONNECTED)
				 {
					 Serial.println(WiFi.status());
					 // Serial.println("waiting to connect!");
					 delay(400);
				 }
				 Serial.println("...");
			 }
			 else
			 {
				 Serial.println("No such network!");
			 }
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
			 Serial.printf("\n Sending request: %s \n", url);
			 http->begin(url);
			 http->setTimeout(5000);
			 int code = http->GET();
		 	if (code<0)
		 	{
				delay(1000);
				code = http->GET();
				if (code < 0) { return nullptr; }
			
		 	}
			 Serial.printf("code: %d", code);
			 return http;
		 }
		 void EndRequest(HTTPClient* r, bool disconnect=true) {
			 r->end();
			 if (disconnect) _Disconnect();
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
	 	void DeleteConfigFile(const char* name)
		 {
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
	 	 void MakeConfigFile(const char* name)
		 {
			 char filePath[32];
			 snprintf_P(filePath,
				 32,
				 "/apps/%s",
				 name
			 );
			 if (!parent->SPIFFS->exists(filePath)) {
				 Serial.println("creating file: ");
				 Serial.println(filePath);
				 auto f = parent->SPIFFS->open(filePath, "a+");
				 f.close();
			 }
		 }
		 void RegisterApplication(char* name, std::function<BuiltInApplication*()> creatingFunction, int iconWidth=NULL, int iconHeight=NULL, const unsigned char* icon=NULL, bool show=true) {
			 MakeConfigFile(name);
			 auto m = new ApplicationDataHolder(name, creatingFunction, show);
			 m->iconHeight = iconHeight;
			 m->iconWidth = iconWidth;
			 m->icon = icon;
			 this->builtInApps->add(m);


		 }
		 void DeleteApplication(char* name) {
			
			 DeleteConfigFile(name);
		 }
		 fs::File GetConfigForApplication(const char* appName, const char* mode="a+") {
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
				 valueBuffer[valueLength] = '\0';
				 keyBuffer[keyLength] = '\0';
				 Serial.println(keyBuffer);
				 if (strcmp(key, keyBuffer) == 0) {
					 return true;
				 }

			 }
			 return false;
		 }
		 void AppendKeyToConfig(const char* appName, const char* key, const char* value) {
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
				 keyBuffer[keyLength] = '\0';
				 valueBuffer[valueLength] = '\0';
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
	 private:
		 bool persistentConnectionStatus = false;
		 char persistentConnectionSSID[25];
		 char persistentConnectionPassword[25];
	 	 void SetPersistentConnection(const char* SSID, const char* password)
	 	 {
			 strcpy(persistentConnectionSSID,SSID);
			 strcpy(persistentConnectionPassword,password);
	 	 	 
	 	 }
	 	void SavePersistentConnectionDetailsToFile(const char* SSID, const char* password)
	 	 {
			this->parent->appsManager->DeleteConfigFile("WiFiPersistent");
			this->parent->appsManager->MakeConfigFile("WiFiPersistent");
			this->parent->appsManager->AppendKeyToConfig("WiFiPersistent", "SSID", SSID);
			this->parent->appsManager->AppendKeyToConfig("WiFiPersistent", "pswd", password);
	 	 }
	 	void LoadPersistentNetworkFromFile()
	 	 {
		 	 //TODO: ta czesc
	 	 }
	 public:
		 const char* filename = "/WiFi.list";
		 SettingsManager* parent;
		 WiFiManager(SettingsManager* parent) {
			 this->parent = parent;
		 }
		 int ScanNetworks() {
			 return parent->w->scanNetworks();
		 }
	 	String GetIp()
		 {
			return parent->w->localIP().toString();
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
					 Serial.println("Found! "); Serial.println(n);
					 auto p = f.readStringUntil('\n');
					 ConnectToWifiAsync((char*)n.c_str(), (char*)p.c_str());
				 	 return true;
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
			 auto f = parent->SPIFFS->open(filename, "r");
			 Serial.println(f.size());
			 int i = 0;
			 int s = 0;
			 while (f.available()) {
				 char n[100];
				 char p[100];
				 int nl = f.readBytesUntil('\r', n, 100);
				 int pl = f.readBytesUntil('\n', p, 100);
				 n[nl] = '\0';
				 p[pl] = '\0';
				 if (strcmp(n, ssid) == 0) {
					
					 

						 f.close();
						 return true;
					 
				 }


			 }





			 f.close();
			 return false;
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
				 delay(50);
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
		//WiFi->mode(WIFI_OFF);
		//WiFi->forceSleepBegin();
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
	 bool OTASerialEnabled = false;
	void SetupOTA()
	 {
		ArduinoOTA.onStart([]() {
			String type;
			if (ArduinoOTA.getCommand() == U_FLASH) {
				type = "sketch";
			}
			else { // U_SPIFFS
				type = "filesystem";
			}

			// NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
			Serial.println("Start updating " + type);
		});
		ArduinoOTA.onEnd([]() {
			Serial.println("\nEnd");
		});
		ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
			Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
		});
		ArduinoOTA.onError([](ota_error_t error) {
			Serial.printf("Error[%u]: ", error);
			if (error == OTA_AUTH_ERROR) {
				Serial.println("Auth Failed");
			}
			else if (error == OTA_BEGIN_ERROR) {
				Serial.println("Begin Failed");
			}
			else if (error == OTA_CONNECT_ERROR) {
				Serial.println("Connect Failed");
			}
			else if (error == OTA_RECEIVE_ERROR) {
				Serial.println("Receive Failed");
			}
			else if (error == OTA_END_ERROR) {
				Serial.println("End Failed");
			}
		});
		ArduinoOTA.begin();
		OTASerialEnabled = true;
	 }
	
	void InstallUpdate()
	 {
		//remove old settings interface
		this->SPIFFS->rmdir("s");
		//download new settings interface
		auto request = this->http->MakeGetRequest("http://watch-service-server.herokuapp.com/update/get/settings");
		auto requestStream = request->getStream();
		//parse the stream
		while (requestStream.available())
		{
			//for each line
			char filename[50];
			int filenameLen = requestStream.readBytesUntil('\n', filename,50);
			filename[filenameLen] = '\0';
			
			//download the file
			Serial.printf("Downloading: %s \n", filename);
			char fAddr[200];
			sprintf(fAddr, "http://watch-service-server.herokuapp.com/update/get/settings/%s", filename,201);
			auto rsp = this->http->MakeGetRequest(fAddr);
			//create file
			char path[201];
			sprintf(path, "/s/%s", filename);
			Serial.printf("Saving to path: %s \n", path);

			auto targetFile = this->SPIFFS->open(path, "w+");
			rsp->writeToStream((Stream*)&targetFile);
			targetFile.close();
			http->EndRequest(rsp,false);
		}
		http->EndRequest(request,false);
		Serial.println("downloading new .bin");
		ESPhttpUpdate.update("http://watch-service-server.herokuapp.com/update/get");
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
		 CloseSettingsServer();
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
			 request->send(*SPIFFS, "/s/index.html", "text/html");


		 });
		 server->serveStatic("/apps", *SPIFFS, "/apps");
		 server->serveStatic("/", *SPIFFS, "/s");
		 server->begin();
		

		 isSettingsServerOpened = true;
	 }
	 String header;
	 void SettingsOnLoop() {
		if (OTASerialEnabled) ArduinoOTA.handle();
		
	 }
	 int SyncTime() {
		 //https://time-watch-service.herokuapp.com/datetime
		 auto rsp = http->MakeGetRequest("http://time-watch-service.herokuapp.com/datetime");
		 if (rsp == nullptr) { return -1; }
		 auto s = rsp->getStream();
		 char buff[10];
		 int read;
		 read = s.readBytesUntil('\n', buff, 10);
		 buff[read] = '\0';
		 Serial.println(buff);
		 int year = atoi(buff);

		 read = s.readBytesUntil('\n', buff, 10);
		 buff[read] = '\0';
		 Serial.println(buff);

		 int month = atoi(buff);

		 read = s.readBytesUntil('\n', buff, 10);
		 buff[read] = '\0';
		 Serial.println(buff);

		 int day = atoi(buff);

		 read = s.readBytesUntil('\n', buff, 10);
		 buff[read] = '\0';
		 Serial.println(buff);

		 int hour = atoi(buff);

		 read = s.readBytesUntil('\n', buff, 10);
		 buff[read] = '\0';
		 Serial.println(buff);

		 int minute = atoi(buff);

		 read = s.readBytesUntil('\n', buff, 10);
		 buff[read] = '\0';
		 Serial.println(buff);

	 	int second = atoi(buff);

		 tk->SetDateTime(year, month, day, hour, minute, second);
		 http->EndRequest(rsp, false);
		 return 0;
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
	 void SaveAlarmOne(int day, int hour, int minute, bool writeToRTC=true)
	 {
		 appsManager->DeleteConfigFile("AlarmOne");
		 auto fileHandle = this->SPIFFS->open("/AlarmOne", "w+");
		 fileHandle.write(hour);
	
		 fileHandle.write(minute);
	
		 fileHandle.close();
		 tk->SetAlarmOne(day, hour, minute);
	 }
	void DeleteAlarmOne()
	 {
		
		tk->DeleteAlarmOne();
	 }

	bool IsItTheTimeToTriggerAlarmOne()
	 {
	

	 	
		if (this->SPIFFS->exists("/AlarmOne")) {
			auto fileHandle = this->SPIFFS->open("/AlarmOne", "r");
			uint8_t hour, minute;
			fileHandle.read(&hour, sizeof(uint8_t));
			fileHandle.read(&minute, sizeof(uint8_t));
			Serial.println((uint8_t)hour);
			Serial.println((uint8_t)minute);
			fileHandle.close();
			auto now = tk->GetCurrentTime();
			return now.Hour() == (uint8_t)hour && now.Minute() == (uint8_t)minute;
			
		}
		return false;
	 }
};


#endif

