#include <Arduino.h>
#include <WiFi.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include "letters.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

#define CLK_PIN   18 // or SCK
#define DATA_PIN  19 // or MOSI
#define CS_PIN    5 // or SS

MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
//speed: more value moves slower 
#define SPEED_TIME  100
#define PAUSE_TIME  0
#define MAX_MESG  20

const char* ssid = "BeInTech-Ventures";
const char* password = "BI15711588";
bool changed = false;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
unsigned long t;
unsigned int cur;
unsigned int last;
String day;
// Global variables

void setup(void)
{
  Serial.begin(115200);
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
  }

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(100);
  timeClient.begin();
  delay(50);
  timeClient.setTimeOffset(6*3600);
  delay(50);
  timeClient.forceUpdate();
  delay(200);
  String date = timeClient.getFormattedDate();
  delay(50);
  unsigned long epochTime = timeClient.getEpochTime();
  delay(50);
  unsigned long epochDay = epochTime/86400;
  delay(50);
  t = epochDay-18072;
  Serial.println(t);
  Serial.println(date);
  day = "Connected Home: " + String(t) + " days";
  char *c =(char*) day.c_str();
  P.begin(3);
  P.setInvert(false);
  P.setZone(0, 0, 3);
  P.displayZoneText(0, c, PA_CENTER, SPEED_TIME, 0, PA_PRINT, PA_SCROLL_LEFT);
}

void loop(void)
{
  P.displayAnimate();
  if (P.getZoneStatus(0))
  {
    P.setTextEffect(0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
    P.displayReset(0);
  }
  
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
    cur=timeClient.getHours();
    Serial.print("Current: ");
    Serial.print(cur);
    Serial.println();
    if(cur==0 && last==23){
      delay(2000);
      ESP.restart();
    }
    last = cur;
    Serial.print("Last: ");
    Serial.print(last);
    Serial.println();
    delay(10);
}