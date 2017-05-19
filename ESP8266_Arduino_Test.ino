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
#define SSID "Tilgin-sj46gmTmbqsu"
#define PASS "eexoQH2udFMsg"
#define PING_IP "www.google.com" //google.com
SoftwareSerial ESP8266(2, 3);
int error_counter = 0;

void setup()
{
  Serial.begin(115200);
  ESP8266.begin(9600);
  ESP8266.setTimeout(1000);

  Serial.println(" ");
  Serial.println("                     ...");
  Serial.println(" ");
  Serial.println("          ESP8266-01 Module Test Code");
  Serial.println(" ");
  Serial.println("                     ...");
  Serial.println("");
  Serial.println("");
  delay(2000);
  Serial.println("....Initializing Module Connection Sequence....");
  Serial.println("                     ...");
  Serial.println(" ");
  Serial.println(" ");
  delay(2000);

  // Test if the module is ready:
  ESP8266.println("AT+RST");
  Serial.println("Trying to reset ESP8266 module...");
  ESP8266.flush();
  if(ESP8266.find("OK"))
  {
    Serial.println("Module is ready.");
  }
  else
  {
    Serial.println("Module have no response!");
    while(1);
  }
  delay(5000);

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
  Serial.println("Setting the module for single connection mode.");
  ESP8266.println("AT+CIPMUX=0");
}

void loop()
{
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += PING_IP;
  cmd += "\",80";
  ESP8266.println(cmd);
  Serial.println(cmd);
  delay(1000);

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
    Serial.println("Connection timeout!");
    error_counter++;
    delay(1000);
    return;
  }
  if(error_counter > 10)
  {
    Serial.println("Connection problem!");
    while(1);
  }

  ESP8266.print(cmd);
  delay(2000);

  while (ESP8266.available())
  {
    char c = ESP8266.read();
    Serial.write(c);

    if(c == '\r')
    {
      Serial.print('\n');
    }
  }

  Serial.println("====");
  delay(1000);
}

boolean connectWiFi()
{
  // Disconnect from current AP:
  ESP8266.println("AT+CWQAP");
  delay(2000);

  // Change module mode to 1:
  ESP8266.println("AT+CWMODE=1");
  delay(500);
  ESP8266.println("AT+RST");
  delay(5000);

  // Prepare connection command:
  String cmd = "AT+CWJAP=\"";
  cmd += SSID;
  cmd += "\",\"";
  cmd += PASS;
  cmd += "\"";

  // Connection command:
  String temporaryString = SSID;
  temporaryString = "Trying to connect " + temporaryString;
  Serial.println(temporaryString);
  delay(100);
  ESP8266.println(cmd);
  delay(10000);

  // Check WiFi connection status of the ESP8266 module:
  if(ESP8266.find("OK"))
  {
    Serial.println("Connected to WiFi.");
    return true;
  }
  else
  {
    Serial.println("Can not connect to the WiFi.");
    return false;
  }
}
