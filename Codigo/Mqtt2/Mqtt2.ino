
#include <ESP8266WiFi.h> 
#include <PubSubClient.h> 
#include <DHT.h>       // including the library of DHT11 temperature and humidity sensor




// Update these with values suitable for your network.
const int trigPin = 2;  //D4
const int echoPin = 0;  //D3
long duration;
int distance;
const char* ssid = "Jhordy";
const char* password = "19971997";
const char* mqtt_server = "broker.hivemq.com"; /// MQTT Broker
int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() { 
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT);
pinMode(LED_BUILTIN, OUTPUT); // Initialize the BUILTIN_LED pin as an output
 Serial.begin(115200);
 // Start up the library


 
 
 setup_wifi();
 client.setServer(mqtt_server, mqtt_port);
 client.setCallback(callback);

 Serial.println("Connected ");
 Serial.print("MQTT Server ");
 Serial.print(mqtt_server);
 Serial.print(":");
 Serial.println(String(mqtt_port)); 
 Serial.print("ESP8266 IP ");
 Serial.println(WiFi.localIP()); 
 Serial.println("Modbus RTU Master Online");


 
}

void setup_wifi() {

 delay(1000);
 // We start by connecting to a WiFi network
 Serial.println();
 Serial.print("Conectandose a ");
 Serial.println(ssid);

 WiFi.begin(ssid, password);

 while (WiFi.status() != WL_CONNECTED) {
 delay(500);
 Serial.print(".");
 }

 Serial.println("");
 Serial.println("WiFi connected");
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
 Serial.print("Message arrived [");
 Serial.print(topic);
 Serial.print("] ");
 for (int i = 0; i<length; i++) {
 Serial.print((char)payload[i]);
 }
 Serial.println();
 // Switch on the LED if an 1 was received as first character
if ((char)payload[0] == '1') {
digitalWrite(LED_BUILTIN, LOW); // Turn the LED on (Note that LOW is the voltage level
// but actually the LED is on; this is because
// it is acive low on the ESP-01)
} else {
digitalWrite(LED_BUILTIN, HIGH); // Turn the LED off by making the voltage HIGH
}
 
}
void reconnect() {
 // Loop until we're reconnected
 while (!client.connected()) {
 Serial.print("Attempting MQTT connection...");
 // Attempt to connect
 if (client.connect("Esp")) {

 Serial.println("connected");
 client.publish("jordy","Connected!");
 Serial.print("Subscribido!");
 client.subscribe("tec");
 } else {
 Serial.print("failed, rc=");
 Serial.print(client.state());
 Serial.println(" try again in 5 seconds");
 // Wait 5 seconds before retrying
 delay(5000);
 }
 }
}
void loop() {
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);
 
 //char temperaturenow [15];
 //dtostrf(distance,2, 1, temperaturenow); //// convert float to char 
 char cadena[16];
sprintf(cadena, "%d", distance);
 client.publish("jordy", cadena); /// send char 
 
 if (!client.connected()) {
 reconnect();
 Serial.println("Desconectado");

 }
 client.loop();

 delay(10000); 
}
