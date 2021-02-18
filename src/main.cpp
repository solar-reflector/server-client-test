#include <WebSocketsClient.h>

WebSocketsClient webSocket;

const char *SSID = "network ssid";
const char *PASS = "network password";
const char *SERVER = "server address";

long t = 0;

void sendMessage()
{
  String message = "{ \"topic\": \"db\", \"mode\": \"normal\", \"data\": " + (String)floor(random(100, 250)) + " }";
  webSocket.sendTXT(message);
  Serial.printf("Data sent to web server: %s\n", message.c_str());
}

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_DISCONNECTED:
    Serial.printf("Disconnected\n");
    break;
  case WStype_CONNECTED:
    Serial.printf("Connected to WebSocket\n");
    sendMessage();
    break;
  case WStype_TEXT:
    Serial.printf("Message Recived: %s\n", payload);
    break;
  default:
    break;
  }
}

void StartWiFi()
{
  Serial.printf("\nConnecting to internet");
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println(WiFi.localIP());
}

void setup(void)
{
  Serial.begin(115200);
  StartWiFi();

  webSocket.begin(SERVER, 8080);
  webSocket.onEvent(webSocketEvent);
}

void loop()
{
  webSocket.loop();
  if (webSocket.isConnected() && millis() - t > 30000)
  {
    sendMessage();
    t = millis();
  }
}