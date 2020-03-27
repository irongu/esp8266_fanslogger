#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <gfxfont.h>

#include <Adafruit_ST7735.h>
#include <Adafruit_ST7789.h>
#include <Adafruit_ST77xx.h>
#include <SPI.h>

#define TFT_CS         4
#define TFT_RST        16                                            
#define TFT_DC         5
#define TFT_MOSI 13  // Data out
#define TFT_SCLK 14  // Clock out
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
//Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

/*
D0 = GPIO16
D1 = GPIO5
D2 = GPIO4
D3 = GPIO0
D4 = GPIO2
D5 = GPIO14
D6 = GPIO12
D7 = GPIO13
D8 = GPIO15
D9 = GPIO3
D10 = GPIO1
 */
//SDA - D2
//SCL - D1
//VCC - 3V
//GND - G
//本代码禁止商用，若有商用，法律追究.
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <Shifty.h>
#include <Wire.h>
#include <DS3231M.h>
#include <Time.h>
#include <TimeLib.h>

DS3231M_Class DS3231M;
unsigned int localPort = 2390;  
const char* ssid = "irongu";  //bazhegaile
const char* password = "172.16.3.199";
String uid = "326251695";//546195 326251695

DynamicJsonDocument jsonBuffer(400);
WiFiClient client;
IPAddress timeServerIP;
const char* ntpServerName = "time.windows.com";
const int NTP_PACKET_SIZE = 48;
byte packetBuffer[ NTP_PACKET_SIZE];

long fans=0;
long view=0;
long articleview=0;
long likes=0;

long start_fans=0;
long start_view=0;
long start_articleview=0;
long start_likes=0;

unsigned char change=0;
long added=0;
unsigned char firstrun=0;

WiFiUDP udp;
void setup() 
{
  Serial.begin(115200);
  for(char i=0;i<3;i++)Serial.println("");
  tft.init(135, 240);           // Init ST7789 240x135
  tft.setRotation(0);
  tft.setTextWrap(false);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE,ST77XX_BLACK);
  tft.setTextSize(1);
 /* while (!DS3231M.begin()) // Initialize RTC communications
  { 
    Serial.println("Unable to find DS3231M. Checking again in 1 second.");
    delay(1000);
  }*/
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);//获取名称密码
  tft.setCursor(0,0);
  tft.println("connecting WIFI");
  while(WiFi.status()!=WL_CONNECTED)
  {  //连接WIFI
    delay(500);
    Serial.print(".");
  }
  Serial.println(""); 
  Serial.print("SSID:"); 
  Serial.println(ssid);  
  Serial.println("WiFi connected");  
  for(char i=0;i<18;i++)Serial.print("-");  
  Serial.println("");  
  Serial.print("User:");  
  Serial.println(uid);
  
  tft.setCursor(0,0);
  tft.print("SSID:");
  tft.print(ssid);
  tail_cover();
  tft.println("WiFi connected");
  for(char i=0;i<18;i++)tft.print("-");
  tft.println("");
  tft.print("User:");
  tft.println(uid);
  for(char i=0;i<18;i++)tft.print("-");
  tft.println("");
  udp.begin(localPort);
  delay(2000);
}

void loop() 
{  
  if(WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    //http.begin("http://api.bilibili.com/x/relation/stat?vmid=326251695");  //zhe de UID gai cheng ni de
    String url = "http://api.bilibili.com/x/relation/stat?vmid=";
    url += uid;
    http.begin(url);
    int httpCode=http.GET(); //获取数据
    if(httpCode == 200)
    {
      String resBuff = http.getString();
      //Serial.println(resBuff);
      DeserializationError error = deserializeJson(jsonBuffer, resBuff); 
      if(error)
      {
        Serial.println("json error");
        Serial.println(error.c_str());
        while(1);
      }
      JsonObject root = jsonBuffer.as<JsonObject>();
      long fans_new = root["data"]["follower"];
      if(fans_new!=fans)
      {
        added=fans_new-fans;
        fans=fans_new;
        Serial.print("Fans:");        
        Serial.println(fans); 
        if(firstrun==0)start_fans=fans_new;       
        if(firstrun==1)
        {
          Serial.print("change:");
          Serial.println(added);
          change=1;
        }
      }
      tft.print("Fans:");
      tft.println(fans);
      tft.print("change:");
      tft.print(added);
      tail_cover();     
    } 
    //http.begin("http://api.bilibili.com/x/space/upstat?mid=326251695");  //zhe de UID gai cheng ni de
    url = "http://api.bilibili.com/x/space/upstat?mid=";
    url += uid;
    http.begin(url);
    //http.begin("http://api.bilibili.com/x/space/upstat?mid=546195");
    httpCode=http.GET(); //获取数据
    if(httpCode == 200)
    {
      String resBuff = http.getString();
      DeserializationError error = deserializeJson(jsonBuffer, resBuff); 
      if(error)
      {
        Serial.println("json error");
        Serial.println(error.c_str());
        while(1);
      }
      JsonObject root = jsonBuffer.as<JsonObject>();
      // long code = root["code"];
      long view_new = root["data"]["archive"]["view"];
      if(view_new!=view)
      {
        added=view_new-view;
        view=view_new;
        Serial.print("View:");
        Serial.println(view);
        if(firstrun==0)start_view=view_new;
        if(firstrun==1)
        {
          Serial.print("change:");
          Serial.println(added);
          change=1;
        }
      }
      tft.print("View:");
      tft.print(view);
//      tft.print("change:");
//      tft.print(added);
      tail_cover();
      long articleview_new = root["data"]["article"]["view"];
      if(articleview_new!=articleview)
      {
        added=articleview_new-articleview;
        articleview=articleview_new;
        Serial.print("articleView:");
        Serial.println(articleview);
        if(firstrun==0)start_articleview=articleview_new;
        if(firstrun==1)
        {
          Serial.print("change:");
          Serial.println(added);
          change=1;
        }
      }
      tft.print("articleView:");
      tft.print(articleview);
//      tft.print("change:");
//      tft.print(added);
      tail_cover();
      long likes_new = root["data"]["likes"];
      if(likes_new!=likes)
      {
        added=likes_new-likes;
        likes=likes_new;
        Serial.print("Likes:");
        Serial.println(likes);
        if(firstrun==0)start_likes=likes_new;
        if(firstrun==1)
        {
          Serial.print("change:");
          Serial.println(added);
          change=1;
        }
      } 
      tft.print("Likes:");
      tft.println(likes);
      tft.print("change:");
      tft.print(added);
      tail_cover(); 
      for(char i=0;i<18;i++)tft.print("-");
      tft.println(""); 

      tft.println("Fans added:");
      tft.println(fans-start_fans);
      tft.println("View added:");
      tft.println(view-start_view);
      tft.println("ArticleView added:");
      tft.println(articleview-start_articleview);
      tft.println("Likes added:");
      tft.println(likes-start_likes);
      if(firstrun==0)
      {
        tft.println("From Time:");
        showTime();
        for(char i=0;i<18;i++)tft.print("-");
        tft.println("");
      }
      if(change==1||firstrun==0)
      {
        if(firstrun==1)
        {
          tft.println("");
          tft.println("");
          for(char i=0;i<18;i++)tft.print("-");
          tft.println("");
        }
        showTime();
        for(char i=0;i<18;i++)Serial.print("-");
        Serial.println("");
        change=0;
        firstrun=1;
      } 
      tft.setCursor(0,0);
      for(char i=0;i<5;i++)tft.println("");     
      delay(10000);
      //delay(10000);     
    } 
  }
}

void showTime(){
  WiFi.hostByName(ntpServerName, timeServerIP); 
  sendNTPpacket(timeServerIP);
  delay(1000);
  int cb = udp.parsePacket();
  if (!cb) {
    Serial.println("no packet yet");
    tft.print("no packet yet");
    tail_cover();
  }
  else {
    udp.read(packetBuffer, NTP_PACKET_SIZE);
    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
    unsigned long secsSince1900 = highWord << 16 | lowWord;
    const unsigned long seventyYears = 2208988800UL;
    unsigned long epoch = secsSince1900 - seventyYears;
    //Serial.print(((epoch  % 86400L) / 3600)+8);
    //Serial.print(':');
    
    //Serial.println((epoch  % 3600) / 60);
    epoch=epoch+8*60*60;
    Serial.print(year(epoch));
    Serial.print("-");
    Serial.print(month(epoch));
    Serial.print("-");
    Serial.print(day(epoch));
    Serial.print(" ");
    Serial.print(hour(epoch));
    Serial.print(F(":"));
    if (minute(epoch) < 10 ) {
      Serial.print('0');
    }
    Serial.print(minute(epoch));
    Serial.print(F(":"));
    if ( second(epoch) < 10 ) {
      Serial.print('0');
    }
    Serial.print(second(epoch));
    Serial.println(F(" "));

    tft.print(year(epoch));
    tft.print("-");
    tft.print(month(epoch));
    tft.print("-");
    tft.print(day(epoch));
    tft.print(" ");
    tft.print(hour(epoch));
    tft.print(F(":"));
    if (minute(epoch) < 10 ) {
      tft.print('0');
    }
    tft.print(minute(epoch));
    tft.print(F(":"));
    if ( second(epoch) < 10 ) {
      tft.print('0');
    }
    tft.print(second(epoch));
    tft.println(F(" "));
  }
}

void sendNTPpacket(IPAddress& address){
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  packetBuffer[0] = 0b11100011;
  packetBuffer[1] = 0;
  packetBuffer[2] = 6;
  packetBuffer[3] = 0xEC; 
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  udp.beginPacket(address, 123);
  udp.write(packetBuffer, NTP_PACKET_SIZE);
  udp.endPacket();
}

void tail_cover()
{
  for(char i=0;i<10;i++)tft.print(" ");
  tft.println("");
}
