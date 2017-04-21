/**************************************************/
/*              ESP8266 Module Test               */
/*  By: Umur Ozhan SENGUL                         */
/*  Under GPLv3                                   */
/**************************************************/
/////////////////////
//    LIBRARIES    //
/////////////////////
#include <SoftwareSerial.h>

////////////////////////////
//    GLOBAL VARIABLES    //
////////////////////////////
#define SSID "<SSID>"
#define PASS "<Password>"
#define DST_IP "66.249.93.147" //google.com
SoftwareSerial ESP8266(2, 3);

void setup()
{
  Serial.begin(115200);
  ESP8266.begin(9600);
  ESP8266.setTimeout(1000);

  while(!Serial);
  while(!ESP8266);
  Serial.println("ESP8266 Demo on UNO");

  while(ESP8266.available() > 0)
  {
    ESP8266.read();
    delay(1000);
    // Test if the module is ready:
    ESP8266.println("AT+RST");
    Serial.println("Resetting module");
    ESP8266.flush();

    if(ESP8266.find("Ready")||ESP8266.find("ready"))
    {
      Serial.println("Module is ready");
    }
    else
    {
      Serial.println("Module have no response.");
      while(1);
    }
    delay(1000);

    //Connect to the WiFi:
    boolean connected=false;
    for(int i=0;i<5;i++)
    {
      if(connectWiFi())
      {
        connected = true;
        break;
      }
    }
    if (!connected)
    {
      while(1);
    }
    delay(5000);
    // Set the single connection mode:
    ESP8266.println("AT+CIPMUX=0");
  }
}

void loop()
{
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += DST_IP;
  cmd += "\",80";
  ESP8266.println(cmd);
  Serial.println(cmd);

  if(ESP8266.find("Error"))
  {
    return;
  }
  cmd = "GET / HTTP/1.0\r\n\r\n";
  ESP8266.print("AT+CIPSEND=");
  ESP8266.println(cmd.length());

  if(ESP8266.find(">"))
  {
    Serial.print(">");
  }
  else
  {
    ESP8266.println("AT+CIPCLOSE");
    Serial.println("connect timeout");
    delay(1000);
    return;
  }

  ESP8266.print(cmd);
  delay(2000);

  while (ESP8266.available())
  {
    char c = ESP8266.read();
    Serial.write(c);

    if(c=='\r')
    {
      Serial.print('\n');
    }
  }

  Serial.println("====");
  delay(1000);
}

boolean connectWiFi()
{
  ESP8266.println("AT+CWMODE=1");
  String cmd="AT+CWJAP=\"";
  cmd += SSID;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";

  ESP8266.println(cmd);
  Serial.println(cmd);
  delay(2000);

  if(ESP8266.find("OK"))
  {
    Serial.println("OK, Connected to WiFi.");
    return true;
  }
  else
  {
    Serial.println("Can not connect to the WiFi.");
    return false;
  }
}
