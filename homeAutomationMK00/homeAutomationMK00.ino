#include <ESP8266WiFi.h>
WiFiClient client;
WiFiServer server(80);
const char* ssid = "meeraclemicro";
const char* password = "fuckumini";
String  command = ""; 

// Set Relay Pins
const int light1 = D1;
const int light2 = D2;
const int fan1 = D5;
const int fan2 = D6;

IPAddress local_IP(192, 168, 0, 250);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

void setup()
{
  Serial.begin(115200);
  pinMode(light1, OUTPUT);
  pinMode(light2, OUTPUT);
  pinMode(fan1, OUTPUT);
  pinMode(fan2, OUTPUT);

  digitalWrite(light1, 1);
  digitalWrite(light2, 1);
  digitalWrite(fan1, 1);
  digitalWrite(fan2, 1);
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());  server.begin();
}

void loop()
{
  client = server.available();
  if (!client) return;
  command = checkClient ();
  Serial.println(command);

  if (command == "l1on"  )        {
    digitalWrite(light1, 0);
    Serial.println("light1 is ON");
  }
  else if (command == "l1off" )     {
    digitalWrite(light1, 1);
    Serial.println("light1 is OFF");
  }
  else if (command == "l2on"  )        {
    digitalWrite(light2, 0);
    Serial.println("light2 is ON");
  }
  else if (command == "l2off" )     {
    digitalWrite(light2, 1);
    Serial.println("light2 is OFF");
  }
  else if (command == "f1on"  )     {
    digitalWrite(fan1, 0);
    Serial.println("fan1 is ON");
  }
  else if (command == "f1off" )   {
    digitalWrite(fan1, 1);
    Serial.println("fan1 is OFF");
  }
  else if (command == "f2on"  )     {
    digitalWrite(fan2, 0);
    Serial.println("fan2 is ON");
  }
  else if (command == "f2off" )   {
    digitalWrite(fan2, 1);
    Serial.println("fan2 is OFF");
  }
  sendBackEcho(command); 
  command = "";
}

String checkClient (void)
{
  while (!client.available()) delay(1);
  String request = client.readStringUntil('\r');
  Serial.println(request);
  request.remove(0, 5);
  request.remove(request.length() - 9, 9);
  return request;
}

void sendBackEcho(String echo)
{
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  //  client.println("<!DOCTYPE HTML>");
  //  client.println("<html>");
  client.println(echo);
  //  client.println("</html>");
  //  client.flush(); // No need
  //  client.stop(); // No need
  delay(1);
}
