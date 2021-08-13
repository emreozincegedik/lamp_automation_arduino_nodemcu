#include <ESP8266WiFi.h>
#include <WiFiClient.h>

//ESP Web Server Library to host a web page
#include <ESP8266WebServer.h>

#include <IRremoteESP8266.h>
#include <IRsend.h>
const uint16_t kIrLed = 3; // ESP8266 GPIO pin to use. Recommended: 4 (D2).

IRsend irsend(kIrLed); // Set the GPIO to be used to sending the message.
uint8_t klimaOff22[13] = {0xC3, 0x77, 0xE0, 0x00, 0x60, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x05, 0x9F};

uint8_t klimaOn22[13] = {0xC3, 0x77, 0xE0, 0x00, 0x60, 0x00, 0x20, 0x00, 0x00, 0x20, 0x00, 0x05, 0xBF};

uint8_t klimaOff16[13] = {0xC3, 0x47, 0xE0, 0x00, 0x60, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x05, 0x6F};

uint8_t klimaOn16[13] = {0xC3, 0x47, 0xE0, 0x00, 0x60, 0x00, 0x20, 0x00, 0x00, 0x20, 0x00, 0x05, 0x8F};

bool klimaStatus = false;
bool klimaStatus16 = false;

bool state;
//---------------------------------------------------------------
//Our HTML webpage contents in program memory
const char MAIN_page[] PROGMEM = R"=====(
  <!DOCTYPE html>
  <html>
  <body>
  <center>
  Click to turn <a href="ledOn">Lamp ON</a><br>
  Click to turn <a href="ledOff">Lamp OFF</a><br>
  Click to turn <a href="ledToggle">Lamp Toggle</a><br>

  <br>
  Click to turn <a href="klimaOff22">Klima Off 22</a><br>
  Click to turn <a href="klimaOn22">Klima On 22</a><br>
  Click to turn <a href="klimaToggle22">Klima Toggle 22</a><br>
  <br>
  Click to turn <a href="klimaOff16">Klima Off 16</a><br>
  Click to turn <a href="klimaOn16">Klima On 16</a><br>
  Click to turn <a href="klimaToggle16">Klima Toggle 16</a><br>
  <br>

  <a href="ledOn"><button>Lambayi Ac</button></a>
  <a href="ledOff"><button>Lambayi Kapat</button></a>
  <a href="ledToggle"><button>Lambayi Ac/Kapat</button></a>
  </center>  
  </body>
  </html>
)=====";

//---------------------------------------------------------------

//Static IP address configuration
IPAddress staticIP(192, 168, 1, 50); //ESP static ip
IPAddress gateway(192, 168, 1, 1);   //IP Address of your WiFi Router (Gateway)
IPAddress subnet(255, 255, 255, 0);  //Subnet mask
IPAddress dns(192, 168, 1, 1);       //DNS

//const char* deviceName = "circuits4you.com";

//On board LED Connected to GPIO2
#define LED D5
int pinState = LOW;
#define input A0
int read;

//SSID and Password of your WiFi router

// const char *ssid = "SUPERONLINE-WiFi_7562";
// const char *password = "UNCCVEHEX7R3";

// const char *ssid = "TTNET_ZyXEL_3HWM";
// const char *password = "cicek-nesle1962";
const char *ssid = "SUPERONLINE_WiFi_9467";
const char *password = "mert1995ST";

//Declare a global object variable from the ESP8266WebServer class.
ESP8266WebServer server(80); //Server on port 80

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================

String lastLine(bool output)
{
  String a = "";
  switch (output)
  {
  case true:
    a = "ON";
    break;
  case false:
    a = "OFF";
    break;

  default:
    break;
  }
  return "<center class='state'>Lamp is " + a + "</center>";
  // return "<center class='state'>Lamp is <span id='output'>" + a + "</span></center>";
}

void handleRoot()
{
  if (!server.authenticate("emre", "Bywzen0lNlH7FiuMTJsE"))
  {
    return server.requestAuthentication();
  }
  //Serial.println("You called root page");
  String s = MAIN_page;                                          //Read HTML contents
  server.send(200, "text/html", s + lastLine(digitalRead(LED))); //Send web page
}

void handleLEDon()
{
  if (!server.authenticate("emre", "Bywzen0lNlH7FiuMTJsE"))
  {
    return server.requestAuthentication();
  }
  //Serial.println("Lamp on page");
  digitalWrite(LED, HIGH);
  state = true;

  String s = MAIN_page;                                          //Read HTML contents
  server.send(200, "text/html", s + lastLine(digitalRead(LED))); //Send ADC value only to client ajax request
}

void handleLEDoff()
{
  if (!server.authenticate("emre", "Bywzen0lNlH7FiuMTJsE"))
  {
    return server.requestAuthentication();
  }
  //Serial.println("Lamp off page");

  String s = MAIN_page; //Read HTML contents
  digitalWrite(LED, LOW);
  state = false;
  server.send(200, "text/html", s + lastLine(digitalRead(LED))); //Send ADC value only to client ajax request
}
void toggleFunc()
{
  //Serial.print("Lamp toggle page ");

  String s = MAIN_page; //Read HTML contents
  if (state)
  {
    state = false;
  }
  else
  {
    state = true;
  }
  digitalWrite(LED, state);
  // digitalWrite(LED, !digitalRead(LED));
  delay(100);
  server.send(200, "text/html", s + lastLine(state)); //Send ADC value only to client ajax request
  //Serial.println(state);
}
void handleLEDtoggle()
{
  if (!server.authenticate("emre", "Bywzen0lNlH7FiuMTJsE"))
  {
    return server.requestAuthentication();
  }
  toggleFunc();
  // if (digitalRead(LED))
  // {
  //   digitalWrite(LED, HIGH);                           //LED off
  //   server.send(200, "text/html", s + lastLine("ON")); //Send ADC value only to client ajax request
  // //Serial.println("(ON)");
  //   /* code */
  // }
  // else
  // {
  //   digitalWrite(LED, LOW); //LED is connected in reverse
  //   server.send(200, "text/html", s + lastLine("OFF"));
  //   //Serial.println("(OFF)");
  // }

  ////Serial.print(digitalRead(input));
}

void handleKlimaOff16()
{
  if (!server.authenticate("emre", "Bywzen0lNlH7FiuMTJsE"))
  {
    return server.requestAuthentication();
  }
  //Serial.println("Klima Off Page");
  String s = MAIN_page;
  for (size_t i = 0; i < 3; i++)
  {
    irsend.sendElectraAC(klimaOff16);
    delay(300);
  }

  klimaStatus = false;
  server.send(200, "text/html", s + "<center class='state'>Klima is OFF</center>");
}
void handleKlimaOn16()
{
  if (!server.authenticate("emre", "Bywzen0lNlH7FiuMTJsE"))
  {
    return server.requestAuthentication();
  }
  //Serial.println("Klima On Page");
  String s = MAIN_page;
  for (size_t i = 0; i < 3; i++)
  {
    irsend.sendElectraAC(klimaOn16);
    delay(300);
  }

  klimaStatus = true;
  server.send(200, "text/html", s + "<center class='state'>Klima is ON</center>");
}
void handleKlimaToggle16()
{
  if (!server.authenticate("emre", "Bywzen0lNlH7FiuMTJsE"))
  {
    return server.requestAuthentication();
  }
  //Serial.println("Klima Toggle Page");
  String s = MAIN_page;
  String statusStr = "Off";
  for (size_t i = 0; i < 3; i++)
  {
    if (klimaStatus16)
    {
      irsend.sendElectraAC(klimaOff16);
      statusStr = "OFF";
    }
    else
    {
      irsend.sendElectraAC(klimaOn16);
      statusStr = "ON";
    }
    delay(300);
  }

  klimaStatus16 = !klimaStatus16;

  server.send(200, "text/html", s + "<center class='state'>Klima is " + statusStr + "</center>");
}
void handleKlimaOn22()
{
  if (!server.authenticate("emre", "Bywzen0lNlH7FiuMTJsE"))
  {
    return server.requestAuthentication();
  }
  //Serial.println("Klima On Page");
  String s = MAIN_page;
  for (size_t i = 0; i < 3; i++)
  {
    irsend.sendElectraAC(klimaOn22);
    delay(300);
  }

  klimaStatus = true;
  server.send(200, "text/html", s + "<center class='state'>Klima is ON</center>");
}
void handleKlimaOff22()
{
  if (!server.authenticate("emre", "Bywzen0lNlH7FiuMTJsE"))
  {
    return server.requestAuthentication();
  }
  //Serial.println("Klima Off Page");
  String s = MAIN_page;
  for (size_t i = 0; i < 3; i++)
  {
    irsend.sendElectraAC(klimaOff22);
    delay(300);
  }

  klimaStatus = false;
  server.send(200, "text/html", s + "<center class='state'>Klima is OFF</center>");
}
void handleKlimaToggle22()
{
  if (!server.authenticate("emre", "Bywzen0lNlH7FiuMTJsE"))
  {
    return server.requestAuthentication();
  }
  //Serial.println("Klima Toggle Page");
  String s = MAIN_page;
  String statusStr = "Off";
  for (size_t i = 0; i < 3; i++)
  {
    if (klimaStatus)
    {
      irsend.sendElectraAC(klimaOff22);
      statusStr = "OFF";
    }
    else
    {
      irsend.sendElectraAC(klimaOn22);
      statusStr = "ON";
    }
    delay(300);
  }

  klimaStatus = !klimaStatus;

  server.send(200, "text/html", s + "<center class='state'>Klima is " + statusStr + "</center>");
}

//==============================================================
//                  SETUP
//==============================================================

void setup(void)
{
  //Serial.begin(9600);
  WiFi.begin(ssid, password); //Connect to your WiFi router
  //Serial.println("yes");

  // pinMode(input, INPUT_PULLUP);
  read = analogRead(input);
  //Onboard LED port Direction output
  pinMode(LED, OUTPUT);
  //Power on LED state off
  digitalWrite(LED, LOW);

  // attachInterrupt(digitalPinToInterrupt(input), handleLEDtoggle, CHANGE);

  state = false;

  irsend.begin();

  WiFi.disconnect(); //Prevent connecting to wifi based on previous configuration

  //WiFi.hostname(deviceName);      // DHCP Hostname (useful for finding device for static lease)
  WiFi.config(staticIP, gateway, subnet, dns);
  WiFi.begin(ssid, password);

  WiFi.mode(WIFI_STA); //WiFi mode station (connect to wifi router only

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    //Serial.print(".");
  }

  //If connection successful show IP address in //Serial monitor
  //Serial.println("");
  //Serial.print("Connected to ");
  //Serial.println(ssid);
  //Serial.print("IP address: ");
  //Serial.println(WiFi.localIP()); //IP address assigned to your ESP

  server.on("/", handleRoot);       //Which routine to handle at root location. This is display page
  server.on("/ledOn", handleLEDon); //as Per  <a href="ledOn">, Subroutine to be called
  server.on("/ledOff", handleLEDoff);
  server.on("/ledToggle", handleLEDtoggle);
  server.on("/klimaOn22", handleKlimaOn22);
  server.on("/klimaOff22", handleKlimaOff22);
  server.on("/klimaToggle16", handleKlimaToggle16);
  server.on("/klimaOn16", handleKlimaOn16);
  server.on("/klimaOff16", handleKlimaOff16);
  server.on("/klimaToggle22", handleKlimaToggle22);

  server.begin(); //Start server
  //Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP
//==============================================================

void loop(void)
{
  // //Serial.print("abs: ");
  // //Serial.println(abs(read - analogRead(input)));
  if (abs(read - analogRead(input)) > 500)
  {
    toggleFunc();
  }
  read = analogRead(input);

  delay(100);
  // //Serial.println(analogRead(input));
  server.handleClient(); //Handle client requests
}