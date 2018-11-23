#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <Adafruit_NeoPixel.h>
#include "config.h"



#define PIN            2


#define NUMPIXELS      8


Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

int delayval = 100; // delay for half a second

config_t conf;


conf.essid = "Test-iot";
conf.pass = "";


const char *ssid = "Test-iot";
const char *pass = "";

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
"</form>";
"<input class='boton' type='submit' value='Mostrar Eprom'/><br><br>"
String paginafin = "</body> </html>";




WiFiClient espClient;
ESP8266WebServer server(80);
void setup() {

  WiFi.softAP(conf.essid, conf.pass);
  Serial.println("Iniciado WebServer ...");
  server.on("/", paginaconf);// prepara laaginas a mostrar
  server.on("/guardar", guardar);   
  server.on("/mostrar", leer(0);
  server.begin();

  pixels.begin(); 
  while (true) {
      server.handleClient();//asd
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
  
  Serial.println(server.arg("ssid"));
  grabar(0,server.arg("ssid"));
  Serial.println(server.arg("pass"));
  grabar(50,server.arg("pass"));

  mensaje = "Configuracion Guardada...";
 for(int i=0;i<NUMPIXELS;i++){


    pixels.setPixelColor(i, pixels.Color(0,150,0));

    pixels.show(); 

    delay(delayval);

  }
  paginaconf();
}
void grabar(int addr, String a) {
  int tamano = a.length(); 
  char inchar[50]; 
  a.toCharArray(inchar, tamano+1);
  for (int i = 0; i < tamano; i++) {
    EEPROM.write(addr+i, inchar[i]);
  }
  for (int i = tamano; i < 50; i++) {
    EEPROM.write(addr+i, 255);
  }
  EEPROM.commit();
}

String leer(int addr) {
   byte lectura;
   String strlectura;
   for (int i = addr; i < addr+50; i++) {
      lectura = EEPROM.read(i);
      if (lectura != 255) {
        strlectura += (char)lectura;
      }
   }
   return strlectura;
}

void loop() {
  // put your main code here, to run repeatedly:
  EEPROM.begin(512);
}
