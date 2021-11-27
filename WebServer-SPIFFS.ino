/*************************************************************************************************
  ESP32 Web Server
  Ejemplo de creación de Web server con acceso a SPIFFS
  Basándose en los ejemplos de:
  https://github.com/khoih-prog/WiFiWebServer
  https://lastminuteengineers.com/creating-esp32-web-server-arduino-ide/
  https://github.com/luisllamasbinaburo/ESP32-Examples
  https://www.luisllamas.es/esp8266-esp32/
**************************************************************************************************/
//************************************************************************************************
// Librerías
//************************************************************************************************
#include <WiFi.h>
#include <SPIFFS.h>
#include <WebServer.h>
//************************************************************************************************
// Variables globales
//************************************************************************************************
// SSID & Password
const char* ssid = "ESP32";  // Enter SSID here
const char* password = "12345678";  //Enter Password here
IPAddress local_ip(192,168,1,1);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);

WebServer server(80);  // Object of WebServer(HTTP port, 80 is defult)

uint8_t LED1pin = 2;
bool LED1status = LOW;
bool PARK1status = LOW;
bool PARK2status = LOW;
bool PARK3status = LOW;
bool PARK4status = LOW;

#define RXD2 16
#define TXD2 17
int recibido;

//************************************************************************************************
// Configuración
//************************************************************************************************
void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
  pinMode(LED1pin, OUTPUT);
  if (!SPIFFS.begin()) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  Serial.print("SSID: ");
  Serial.println(ssid);
  Serial.print("password: ");
  Serial.println(password);

  server.on("/", handle_OnConnect); // página de inicio
  server.on("/led1on", handle_led1on); // handler al activar led
  server.on("/led1off", handle_led1off); // handler al desactivar led
  server.onNotFound([]() {                  // si el cliente solicita una uri desconocida
    if (!HandleFileRead(server.uri()))      // enviar archivo desde SPIFF, si existe
      handleNotFound();             // sino responder con un error 404 (no existe)
  });
  //server.onNotFound(handle_NotFound); //si no se encuentra el uri, responder con error 404 (no existe)

  server.begin(); // iniciar servidor
  Serial.println("HTTP server started");
  delay(100);
}
//************************************************************************************************
// loop principal
//************************************************************************************************
void loop() {
  server.handleClient(); // escuchar solicitudes de clientes
   if(Serial2.available()){
     recibido = Serial2.read();
   }
   Serial.println(recibido);
  //******************************LOGICA DEL SERIAL***********************************************
  if (recibido == 0){
    PARK1status = LOW;
    PARK2status = LOW; 
    PARK3status = LOW; 
    PARK4status = LOW;  
  } 
  if (recibido & 1){
    PARK1status = HIGH; 
  }
  else
  {
    PARK1status = LOW;
  }
  if (recibido & 2){
    PARK2status = HIGH; 
  }
  else
  {
    PARK2status = LOW;
  }
  if (recibido & 4){
    PARK3status = HIGH; 
  }
  else
  {
    PARK3status = LOW;
  }
  if (recibido & 8){
    PARK4status = HIGH;  
  }
  else
  {
    PARK4status = LOW;
  }
  //******************************CONTROL LED AZUL************************************************
  if (LED1status)
  {
    digitalWrite(LED1pin, HIGH);
  }
  else
  {
    digitalWrite(LED1pin, LOW);
  }
}
//************************************************************************************************
// Handler de página de inicio
//************************************************************************************************
void handle_OnConnect() {
  handle_led1off(); // inicia LED apagado, por defecto
}
//************************************************************************************************
// Handler de led1on
//************************************************************************************************
void handle_led1on() {
  LED1status = HIGH;
  Serial.println("GPIO2 Status: ON");
  server.send(200, "text/html", SendHTML(LED1status, PARK1status, PARK2status, PARK3status, PARK4status)); //responde con un OK (200) y envía HTML
}
//************************************************************************************************
// Handler de led1off
//************************************************************************************************
void handle_led1off() {
  LED1status = LOW;
  Serial.println("GPIO2 Status: OFF");
  server.send(200, "text/html", SendHTML(LED1status, PARK1status, PARK2status, PARK3status, PARK4status)); //responde con un OK (200) y envía HTML
}
//************************************************************************************************
// Procesador de HTML
//************************************************************************************************
String SendHTML(uint8_t led1stat, uint8_t park1stat, uint8_t park2stat, uint8_t park3stat, uint8_t park4stat) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>LED Control</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #3498db;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #3498db;}\n";
  ptr += ".button-on:active {background-color: #2980b9;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>Parqueo-matic &#128664</h1>\n";
  ptr += "<h3>Web Server (AP)</h3>\n";
  //BLUE LED ESP32
  if (led1stat == HIGH)
  {
    ptr += "<p>LED1 Status: ON</p><a class=\"button button-off\" href=\"/led1off\">OFF</a>\n";
  }
  if (led1stat == LOW)
  {
    ptr += "<p>LED1 Status: OFF</p><a class=\"button button-on\" href=\"/led1on\">ON</a>\n";
  }
  //PARKING LOT 1 
  if (park1stat == HIGH)
  {
    ptr += "\n";
    ptr += "<p>PARKING LOT 1 Status: AVAILABLE</p>\n";
  }
  if (park1stat == LOW)
  {
    ptr += "\n";
    ptr += "<p>PARKING LOT 1 Status: UNAVAILABLE</p>\n";
  }

  //PARKING LOT 2 
  if (park2stat == HIGH)
  {
    ptr += "\n";
    ptr += "<p>PARKING LOT 2 Status: AVAILABLE</p>\n";
  }
  if (park2stat == LOW)
  {
    ptr += "\n";
    ptr += "<p>PARKING LOT 2 Status: UNAVAILABLE</p>\n";
  }
  
  //PARKING LOT 3 
  if (park3stat == HIGH)
  {
    ptr += "\n";
    ptr += "<p>PARKING LOT 3 Status: AVAILABLE</p>\n";
  }
  if (park3stat == LOW)
  {
    ptr += "\n";
    ptr += "<p>PARKING LOT 3 Status: UNAVAILABLE</p>\n";
  }
  
  //PARKING LOT 4 
  if (park4stat == HIGH)
  {
    ptr += "\n";
    ptr += "<p>PARKING LOT 4 Status: AVAILABLE</p>\n";
  }
  if (park4stat == LOW)
  {
    ptr += "\n";
    ptr += "<p>PARKING LOT 4 Status: UNAVAILABLE</p>\n";
  }
  
  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
void handleNotFound() {
  server.send(404, "text/plain", "Not found");
}
//************************************************************************************************
// Obtener el tipo de contenido del archivo
//************************************************************************************************
String GetContentType(String filename)
{
  if (filename.endsWith(".htm")) return "text/html";
  else if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/x-pdf";
  else if (filename.endsWith(".zip")) return "application/x-zip";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}
//************************************************************************************************
// Enviar al servidor el archivo desde SPIFFS
//************************************************************************************************
void ServeFile(String path)
{
  File file = SPIFFS.open(path, "r");
  size_t sent = server.streamFile(file, GetContentType(path));
  file.close();
}
//************************************************************************************************
// Enviar al servidor el archivo desde SPIFFS
//************************************************************************************************
void ServeFile(String path, String contentType)
{
  File file = SPIFFS.open(path, "r");
  size_t sent = server.streamFile(file, contentType);
  file.close();
}
//************************************************************************************************
// Handler de not found
//************************************************************************************************
bool HandleFileRead(String path)
{
  if (path.endsWith("/")) path += "index.html";
  Serial.println("handleFileRead: " + path);

  if (SPIFFS.exists(path))
  {
    ServeFile(path);
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;
}
