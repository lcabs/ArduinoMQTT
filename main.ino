/*
 Reconnecting MQTT example - non-blocking

 This sketch demonstrates how to keep the client connected
 using a non-blocking reconnect function. If the client loses
 its connection, it attempts to reconnect every 5 seconds
 without blocking the main loop.

*/

//#include <Buzzer.h>
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

// Update these with values suitable for your hardware/network.
byte mac[]    = {  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };
IPAddress ip(192, 168, 18, 169);
IPAddress server(192, 168, 18, 168);
//Buzzer buzzer(27);

int SerialPrint = 1;
long lastReconnectAttempt = 0;

void callback(char* topic, byte* payload, unsigned int length);
void subscribetoAll();
void checkspayload();

void callback(char* topic, byte* payload, unsigned int length)
{
  String payloadmsg;
  for (int i=0;i<length;i++) 
  {
    payloadmsg += (char)payload[i];
    payloadmsg[length] = '\0';
  }
  if (SerialPrint == 1) 
  {
    Serial.print("topic received = ");
    Serial.println(topic);
    Serial.print("payload received = ");
    Serial.println(payloadmsg);
  }
  checkspayload(payloadmsg);
}

EthernetClient ethClient;
PubSubClient client(ethClient);

boolean reconnect() 
{
  if (client.connect("ARDUINO","Arduino","12345")) 
  {
    Serial.println("Arduino On!");
    subscribetoAll();
    client.publish("lcabs1993/arduino","Arduino On!");    // Once connected, publish an announcement...
  
  }
  return client.connected();
}

void subscribetoAll()
{
  Serial.println("Subscribed to all.");
  client.subscribe("lcabs1993/broadcast");
  client.subscribe("lcabs1993/arduino");
  Serial.println("Subscribed to all.");
}

void checkspayload(String x)
  {
    if (x == "buzzeron")
      {
        Serial.println("BUZZER ATIVADO");
      }  
    if (x == "buzzeroff")
      {
        Serial.println("BUZZER DESATIVADO");
      }  
}


void setup()
{
  Serial.begin(9600);
  Serial.println("Serial on!");
  client.setServer(server, 1883);
  client.setCallback(callback);
  Ethernet.begin(mac, ip);
  delay(1500);
  lastReconnectAttempt = 0;
}

void loop()
{
  if (!client.connected()) 
  {
    Serial.println("Client disconnected.");
    long now = millis();
    if (now - lastReconnectAttempt > 1000) 
    {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (reconnect()) 
      {
        lastReconnectAttempt = 0;
      }
    }
  } else 
  {
  // Client connected 
  client.loop();
  }
}
