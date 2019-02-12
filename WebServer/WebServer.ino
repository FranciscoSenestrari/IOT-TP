#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>



#define PIN            2


#define NUMPIXELS      8


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 100; // delay for half a second
const char *ssid = "Post";
const char *pass = "senestrari";
String mensaje = "";
String pagina = "<!DOCTYPE html>"
"<html>"
"<head>"
"<title>Tutorial Eeprom</title>"
"<meta charset='UTF-8'>"
"</head>"
"<body>"
"</form>"
"<form action='guardar' method='get'>"
"SSID:<br><br>"
"<input class='input1' name='ssid' type='text'><br>"
"PASSWORD:<br><br>"
"<input class='input1' name='pass' type='password'><br><br>"
"<input class='boton' type='submit' value='GUARDAR'/><br><br>"
"</form>"
"<input class='boton' type='submit' value='Mostrar'/><br><br>";
String paginafin = "</body> </html>";




WiFiClient espClient;
ESP8266WebServer server(80);
void setup() {

  WiFi.softAP(ssid, pass);
  Serial.println("Iniciado WebServer ...");
  server.on("/", paginaconf);// prepara laaginas a mostrar
  server.on("/guardar", guardar);
  server.begin();

  pixels.begin(); 
  while (true) {
      server.handleClient();
  }

}
void paginaconf() {
  server.send(200, "text/html", pagina );
  for(int i=0;i<NUMPIXELS;i++){


    pixels.setPixelColor(i, pixels.Color(255,255,255));

    pixels.show(); 

    delay(delayval);
  }
}

void guardar() {
  
 
  grabar(0,server.arg("ssid"));
  grabar(50,server.arg("pass"));

  mensaje = "Guardado...";
 for(int i=0;i<NUMPIXELS;i++){
    pixels.setPixelColor(i, pixels.Color(0,150,0));
    pixels.show(); 
    delay(delayval);
  }
  paginaconf();
}

void grabar(int addr, String a) {
  int tamano = a.length(); 
  char charin[50]; 
  a.toCharArray(charin, tamano+1);
  for (int i = 0; i < tamano; i++) {
    EEPROM.write(addr+i, charin[i]);
  }
  for (int i = tamano; i < 50; i++) {
    EEPROM.write(addr+i, 255);
  }
  EEPROM.commit();
}

void loop() {
  EEPROM.begin(512);
}
