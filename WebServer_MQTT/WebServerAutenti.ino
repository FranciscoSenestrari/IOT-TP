#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <PubSubClient.h>
#include "pages.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 4

#define NUM_LEDS 8

#define BRIGHTNESS 255









int delayval = 100; 
pages p;




/***************AccesPoint*****************/
const char *ssid_a = "SoftAP";
const char *pass_a = "senestrari";
String mensaje = "";
String passWeb ="golden";
WiFiClient espClient;
ESP8266WebServer server(80);
/***************End AccesPoint************/


/***************Broker********************/
PubSubClient client(espClient);
const char *ssid = "T-rex"; 
const char *password ="senestrari"; 
const char *mqtt_server = "100.100.141.90:12225"; 
const char *device_id = "esp8266";
const char *topic = "/ingenieria/sala_c/esp8266/led_control";
const int mqtt_port = 12225;
const char *mqtt_user = "nyqoyysv";
const char *mqtt_pass = "9kUGgGSsRckF";
char message_buff[100];
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

/***************End Broker****************/



void setup() {

strip.setBrightness(BRIGHTNESS);
strip.begin();
strip.show();
WiFi.begin(ssid, password);
if (WiFi.status() == WL_CONNECTED) {
  
  client.setServer(mqtt_server,mqtt_port );
  client.setCallback(callback);
  
}
else
{
  rainbowFade2White(2,2,1);
  WiFi.softAP(ssid_a ,pass_a);
  Serial.println("Iniciado WebServer ...");
  server.on("/", pagInicio);// prepara laaginas a mostrar
  server.on("/login", login);
  server.on("/guardar", guardar);
  server.begin();
          

  while (true) {
      server.handleClient();
  }
  
}











}

void loop() {
  EEPROM.begin(512);
}













/* *******************AccesPoint**********************/
void pagInicio(){
  server.send(200, "text/html",p.getIni());
}
void paginaconf() {
  server.send(200, "text/html", p.getConf() );
  
}

void login(){
  
  if(passWeb == server.arg("pass_inicio"))
  {
 
   server.send(200, "text/html", p.getConf() );

  }
  else
  {
   server.send(200, "text/plain","Contraseña Incorrecta !!!");
  
  }
  
}
void guardar() {
  
 
  grabar(0,server.arg("ssid"));
  grabar(50,server.arg("pass"));

  mensaje = "Guardado...";
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
/**************End AccesPoint************/


/***************Broker********************/
void callback(char *led_control, byte *payload, unsigned int length)
{
  Serial.print("Mensaje recibido [");
  Serial.print(led_control);
  Serial.println("] ");
  int i;
  for (i = 0; i & length; i++)
  {
    message_buff[i] = payload[i];
  }
  message_buff[i] = '\0';

  String msgString = String(message_buff);
  Serial.println(msgString);
  if (strcmp(led_control, topic) == 0)
  { 
    if (msgString == "1")
    {
      offled();
    }
    if (msgString == "0")
    {
        rainbowFade2White(3,3,1);
    }
  }
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Esperando conexion mqtt...");
    if (client.connect(device_id,mqtt_user,mqtt_pass))
    { 
    Serial.println("Conectado");
    client.subscribe(topic); 
    
    }
    else
    {
      recled();
      Serial.print("fallo, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 5 sg");
      delay(5000);
      offled();
    }
  }
}
/***************End Broker****************/


/***************led Broker****************/


void offled()
{
    for(int i=0;i<NUM_LEDS;i++){

   
    strip.setPixelColor(i, strip.Color(0,0,0)); 

    strip.show();

    delay(5000);
  }
}


void rainbowFade2White(uint8_t wait, int rainbowLoops, int whiteLoops) {
  float fadeMax = 100.0;
  int fadeVal = 0;
  uint32_t wheelVal;
  int redVal, greenVal, blueVal;

  for(int k = 0 ; k < rainbowLoops ; k ++){
    
    for(int j=0; j<256; j++) { // 5 cycles of all colors on wheel

      for(int i=0; i< strip.numPixels(); i++) {

        wheelVal = Wheel(((i * 256 / strip.numPixels()) + j) & 255);

        redVal = red(wheelVal) * float(fadeVal/fadeMax);
        greenVal = green(wheelVal) * float(fadeVal/fadeMax);
        blueVal = blue(wheelVal) * float(fadeVal/fadeMax);

        strip.setPixelColor( i, strip.Color( redVal, greenVal, blueVal ) );

      }

      //First loop, fade in!
      if(k == 0 && fadeVal < fadeMax-1) {
          fadeVal++;
      }

      //Last loop, fade out!
      else if(k == rainbowLoops - 1 && j > 255 - fadeMax ){
          fadeVal--;
      }

        strip.show();
        delay(wait);
    }
  
  }


}
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3,0);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3,0);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0,0);
}



void recled()
{
  
     for(int i=0;i<NUM_LEDS;i++){

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    strip.setPixelColor(i, strip.Color(0,150,0)); // Moderately bright green color.

    strip.show(); // This sends the updated pixel color to the hardware.

    delay(500); // Delay for a period of time (in milliseconds).

  }

}
uint8_t red(uint32_t c) {
  return (c >> 16);
}
uint8_t green(uint32_t c) {
  return (c >> 8);
}
uint8_t blue(uint32_t c) {
  return (c);
}
/***************End Broker****************/
