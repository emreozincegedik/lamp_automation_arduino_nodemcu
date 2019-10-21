#include <ESP8266WiFi.h>
#include <WiFiClient.h>
 
//ESP Web Server Library to host a web page
#include <ESP8266WebServer.h> 
// Include the I2C Liquid Crystal Display Library                     

//---------------------------------------------------------------
//Our HTML webpage contents in program memory
const char MAIN_page[] PROGMEM = R"=====(
  <!DOCTYPE html>
  <html>
  <body>
  <center>
  Ciclk to turn <a href="ledOn">Lamp ON</a><br>
  Ciclk to turn <a href="ledOff">Lamp OFF</a><br>
  Ciclk to turn <a href="ledToggle">Lamp Toggle</a><br>
  </center>  
  </body>
  </html>
)=====";
//---------------------------------------------------------------
 
 
//Static IP address configuration
IPAddress staticIP(192, 168, 1, 50); //ESP static ip
IPAddress gateway(192, 168, 1, 1);   //IP Address of your WiFi Router (Gateway)
IPAddress subnet(255, 255, 255, 0);  //Subnet mask
IPAddress dns(192, 168, 1, 1);  //DNS
 
 
//const char* deviceName = "circuits4you.com";
 
//On board LED Connected to GPIO2
#define LED D3
#define LED2 D4  
#define input D5
 
//SSID and Password of your WiFi router

const char* ssid = "Your_SSID_HERE";
const char* password = "YOUR_PASSWORD_HERE";

 
//Declare a global object variable from the ESP8266WebServer class.
ESP8266WebServer server(80); //Server on port 80
 
//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================

void handleRoot() {
  if (!server.authenticate("emre", "Bywzen0lNlH7FiuMTJsE")) {
    return server.requestAuthentication();
  }
 Serial.println("You called root page");
 Serial.println(digitalRead(input));
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}
 
void handleLEDon() { 
  if (!server.authenticate("emre", "Bywzen0lNlH7FiuMTJsE")) {
    return server.requestAuthentication();
  }
 Serial.println("Lamp on page");
 if (!digitalRead(input))
 {
   if (digitalRead(LED))
   {
     
 digitalWrite(LED,LOW); //LED is connected in reverse 
 digitalWrite(LED2,HIGH); //LED is connected in reverse 
   }
   else
 {
   digitalWrite(LED,HIGH); //LED off
    digitalWrite(LED2,LOW);
 }
 }
 
 
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s+"<center class='state'>Lamp is ON</center>"); //Send ADC value only to client ajax request
 
Serial.print(digitalRead(input));
}
 
void handleLEDoff() { 
  if (!server.authenticate("emre", "Bywzen0lNlH7FiuMTJsE")) {
    return server.requestAuthentication();
  }
    Serial.println("Lamp off page");
  if (digitalRead(input))
  {
    if (digitalRead(LED2))
    {
       digitalWrite(LED,HIGH); //LED off
    digitalWrite(LED2,LOW);
    }
    else
  {
    digitalWrite(LED,LOW); //LED is connected in reverse 
 digitalWrite(LED2,HIGH); //LED is connected in reverse 
  }
   
    
    }
  
   String s = MAIN_page; //Read HTML contents
    server.send(200, "text/html", s+"<center class='state'>Lamp is OFF</center>"); //Send ADC value only to client ajax request
 
  
 
Serial.print(digitalRead(input));
}
void handleLEDtoggle(){
  if (!server.authenticate("emre", "Bywzen0lNlH7FiuMTJsE")) {
    return server.requestAuthentication();
  }
  Serial.println("Lamp toggle page");
  
 String s = MAIN_page; //Read HTML contents
  if (digitalRead(LED))
  {
  digitalWrite(LED,LOW); //LED off
  digitalWrite(LED2,HIGH);
  
    /* code */
  }
  else
  {
  digitalWrite(LED,HIGH); //LED is connected in reverse
  digitalWrite(LED2,LOW);
  
  }
  if (!digitalRead(input))
  {
  server.send(200, "text/html", s+"<center class='state'>Lamp is OFF</center>"); //Send ADC value only to client ajax request
    
  }
  else
  {
  server.send(200, "text/html", s+"<center class='state'>Lamp is ON</center>"); //Send ADC value only to client ajax request
    /* code */
  }
  
  
  //Serial.print(digitalRead(input));
  }
//==============================================================
//                  SETUP
//==============================================================
void setup(void){
  Serial.begin(9600);
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  //Onboard LED port Direction output
  pinMode(LED,OUTPUT); 
  pinMode(LED2,OUTPUT); 
  //Power on LED state off
  digitalWrite(LED,HIGH);
  digitalWrite(LED2,LOW),
 
  WiFi.disconnect();  //Prevent connecting to wifi based on previous configuration
  
  //WiFi.hostname(deviceName);      // DHCP Hostname (useful for finding device for static lease)
  WiFi.config(staticIP, gateway, subnet, dns);
  WiFi.begin(ssid, password);
 
  WiFi.mode(WIFI_STA); //WiFi mode station (connect to wifi router only
  
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

 
  server.on("/", handleRoot);      //Which routine to handle at root location. This is display page
  server.on("/ledOn", handleLEDon); //as Per  <a href="ledOn">, Subroutine to be called
  server.on("/ledOff", handleLEDoff);
  server.on("/ledToggle", handleLEDtoggle);
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}
//==============================================================
//                     LOOP
//==============================================================
void loop(void){
  server.handleClient();          //Handle client requests
  
}