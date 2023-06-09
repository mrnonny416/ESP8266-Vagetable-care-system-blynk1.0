
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <WidgetRTC.h>
#include "./file.h"
#include <NTPClient.h> //https://randomnerdtutorials.com/esp8266-nodemcu-date-time-ntp-client-server-arduino/
#include <WiFiUdp.h>

char auth[] = AUTHENTICAION_TOKEN;
char ssid[] = WIFI_SSID;
char pass[] = WIFI_PASSWORD;
char server[] = SERVER_URL;
int port = SERVER_PORT;

WidgetRTC rtc;

const int weekly[7] = {1, 2, 3, 4, 5, 6, 7};

// Define light Port
const int light1 = D0;
const int light2 = D1;
const int light3 = D2;
const int light4 = D3;
// Define Water Port
const int water1 = D5;
const int water2 = D6;
const int water3 = D7;
const int water4 = D8;

const int humidity = A0;

TimeInputParam *Timer[8] = {NULL}; // global

bool timer_manual;

bool manual[8]; // global

static unsigned long last_display_time = 0;

int virtual_port[8] = {11, 12, 13, 14, 15, 16, 17, 18};

// Code Timer-----------------
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  pinMode(light1, OUTPUT);
  pinMode(light2, OUTPUT);
  pinMode(light3, OUTPUT);
  pinMode(light4, OUTPUT);
  pinMode(water1, OUTPUT);
  pinMode(water2, OUTPUT);
  pinMode(water3, OUTPUT);
  pinMode(water4, OUTPUT);

  digitalWrite(light1, HIGH);
  digitalWrite(light2, HIGH);
  digitalWrite(light3, HIGH);
  digitalWrite(light4, HIGH);
  digitalWrite(water1, HIGH);
  digitalWrite(water2, HIGH);
  digitalWrite(water3, HIGH);
  digitalWrite(water4, HIGH);

  Blynk.begin(auth, ssid, pass, server, port); // ใช้เป็น Server Blynk ฟรี

  // timer-----------------------------
  timeClient.begin();
  timeClient.setTimeOffset(25200); //+7 Thailand
}
void loop()
{
  Blynk.run();
  timeClient.update();
  if (millis() - last_display_time >= 5000)
  {
    Serial.println(timeClient.getDay());
    if (timer_manual)
    { // timer start
      for (int i = 0; i < 8; i++)
      {
        if (Timer[i] != NULL and Timer[i]->isWeekdaySelected(weekly[weekday()]))
        {
          if (Timer[i]->hasStartTime() and Timer[i]->getStartHour() < timeClient.getHours() or (Timer[i]->getStartHour() == timeClient.getHours() and Timer[i]->getStartMinute() <= timeClient.getMinutes()))
          {
            manual[i] = true;
          }
          else
          {
            manual[i] = false;
          }
          if (Timer[i]->hasStopTime() and Timer[i]->getStopHour() < timeClient.getHours() or (Timer[i]->getStopHour() == timeClient.getHours() and Timer[i]->getStopMinute() <= timeClient.getMinutes()))
          {
            manual[i] = false;
          }
          Blynk.virtualWrite(virtual_port[i], manual[i]);
        }
      }
    } // timer end
    digitalWrite(light1, !manual[0]);
    digitalWrite(light2, !manual[1]);
    digitalWrite(light3, !manual[2]);
    digitalWrite(light4, !manual[3]);
    digitalWrite(water1, !manual[4]);
    digitalWrite(water2, !manual[5]);
    digitalWrite(water3, !manual[6]);
    digitalWrite(water4, !manual[7]);

    int val = analogRead(humidity);
    Blynk.virtualWrite(V0, 1024 - val);
    Serial.println(1024 - val);
    Blynk.setProperty(V0, "color", val > 500 and val < 710 ? "#09D6FF" : "#FFAD09"); // 50%~69% good for plate

    last_display_time = millis();
  }
}

BLYNK_CONNECTED()
{
  rtc.begin();
}

BLYNK_WRITE(V1)
{
  if (Timer[0] != NULL)
    delete Timer[0];
  Timer[0] = new TimeInputParam(param);
}
BLYNK_WRITE(V2)
{
  if (Timer[1] != NULL)
    delete Timer[1];
  Timer[1] = new TimeInputParam(param);
}
BLYNK_WRITE(V3)
{
  if (Timer[2] != NULL)
    delete Timer[2];
  Timer[2] = new TimeInputParam(param);
}
BLYNK_WRITE(V4)
{
  if (Timer[3] != NULL)
    delete Timer[3];
  Timer[3] = new TimeInputParam(param);
}
BLYNK_WRITE(V5)
{
  if (Timer[4] != NULL)
    delete Timer[4];
  Timer[4] = new TimeInputParam(param);
}
BLYNK_WRITE(V6)
{
  if (Timer[5] != NULL)
    delete Timer[5];
  Timer[5] = new TimeInputParam(param);
}
BLYNK_WRITE(V7)
{
  if (Timer[6] != NULL)
    delete Timer[6];
  Timer[6] = new TimeInputParam(param);
}
BLYNK_WRITE(V8)
{
  if (Timer[7] != NULL)
    delete Timer[7];
  Timer[7] = new TimeInputParam(param);
}
//-----------Switch-----------

BLYNK_WRITE(V10)
{
  switch (param.asInt())
  {
  case 1:
  {
    timer_manual = true;
    Serial.println("Item 1 Selected");
    Blynk.setProperty(V1, "color", "#FF8C09");
    Blynk.setProperty(V2, "color", "#FF8C09");
    Blynk.setProperty(V3, "color", "#7D7D7D");
    Blynk.setProperty(V4, "color", "#7D7D7D");
    Blynk.setProperty(V5, "color", "#09C3FF");
    Blynk.setProperty(V6, "color", "#09C3FF");
    Blynk.setProperty(V7, "color", "#09C3FF");
    Blynk.setProperty(V8, "color", "#09C3FF");
    Blynk.setProperty(V11, "color", "#7D7D7D");
    Blynk.setProperty(V12, "color", "#7D7D7D");
    Blynk.setProperty(V13, "color", "#7D7D7D");
    Blynk.setProperty(V14, "color", "#7D7D7D");
    Blynk.setProperty(V15, "color", "#7D7D7D");
    Blynk.setProperty(V16, "color", "#7D7D7D");
    Blynk.setProperty(V17, "color", "#7D7D7D");
    Blynk.setProperty(V18, "color", "#7D7D7D");
    break;
  }
  case 2:
  {
    timer_manual = false;
    Serial.println("Item 2 Selected");
    Blynk.setProperty(V1, "color", "#7D7D7D");
    Blynk.setProperty(V2, "color", "#7D7D7D");
    Blynk.setProperty(V3, "color", "#7D7D7D");
    Blynk.setProperty(V4, "color", "#7D7D7D");
    Blynk.setProperty(V5, "color", "#7D7D7D");
    Blynk.setProperty(V6, "color", "#7D7D7D");
    Blynk.setProperty(V7, "color", "#7D7D7D");
    Blynk.setProperty(V8, "color", "#7D7D7D");
    Blynk.setProperty(V11, "color", "#FF8C09");
    Blynk.setProperty(V12, "color", "#FF8C09");
    Blynk.setProperty(V13, "color", "#FF8C09");
    Blynk.setProperty(V14, "color", "#FF8C09");
    Blynk.setProperty(V15, "color", "#09C3FF");
    Blynk.setProperty(V16, "color", "#09C3FF");
    Blynk.setProperty(V17, "color", "#09C3FF");
    Blynk.setProperty(V18, "color", "#09C3FF");
    break;
  }
  }
}

//-----------Light 1-----------
BLYNK_WRITE(V11)
{
  if (param.asInt() == 1)
  {
    Serial.print("Light 1 : On");
    manual[0] = true;
  }
  else
  {
    Serial.print("Light 1 : Off");
    manual[0] = false;
  }
}
//-----------Light 2-----------
BLYNK_WRITE(V12)
{
  if (param.asInt() == 1)
  {
    Serial.print("Light 2 : On");
    manual[1] = true;
  }
  else
  {
    Serial.print("Light 2 : Off");
    manual[1] = false;
  }
}
//-----------Light 3-----------
BLYNK_WRITE(V13)
{
  if (param.asInt() == 1)
  {
    Serial.print("Light 3 : On");
    manual[2] = true;
  }
  else
  {
    Serial.print("Light 3 : Off");
    manual[2] = false;
  }
}
//-----------Light 4-----------
BLYNK_WRITE(V14)
{
  if (param.asInt() == 1)
  {
    Serial.print("Light 4 : On");
    manual[3] = true;
  }
  else
  {
    Serial.print("Light 4 : Off");
    manual[3] = false;
  }
}
//-----------Water 1-----------
BLYNK_WRITE(V15)
{
  if (param.asInt() == 1)
  {
    Serial.print("Water 1 : On");
    manual[4] = true;
  }
  else
  {
    Serial.print("Water 1 : Off");
    manual[4] = false;
  }
}
//-----------Water 2-----------
BLYNK_WRITE(V16)
{
  if (param.asInt() == 1)
  {
    Serial.print("Water 2 : On");
    manual[5] = true;
  }
  else
  {
    Serial.print("Water 2 : Off");
    manual[5] = false;
  }
}
//-----------Water 3-----------
BLYNK_WRITE(V17)
{
  if (param.asInt() == 1)
  {
    Serial.print("Water 3 : On");
    manual[6] = true;
  }
  else
  {
    Serial.print("Water 3 : Off");
    manual[6] = false;
  }
}
//-----------Water 4-----------
BLYNK_WRITE(V18)
{
  if (param.asInt() == 1)
  {
    Serial.print("Water 4 : On");
    manual[7] = true;
  }
  else
  {
    Serial.print("Water 4 : Off");
    manual[7] = false;
  }
}
