#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <TM1637Display.h>

// Update these with values suitable for your network.
const char* ssid = "HomeNet 0105";//put your wifi ssid here
const char* password = "euroeuro1";//put your wifi password here
const char* mqtt_server = "io.adafruit.com"; //const char* mqtt_server = "iot.eclipse.org";
//const char *topic = "pixi3cr/f/c1";
const char *mqtt_username = "pixi3cr";
const char *mqtt_password = "674fd6eac2cd414397e1826be1106988";

const int timereset = 100000;

String MQTT_to_Holding[20];  // Array recepcion valores MQTT // Array reception MQTT values

const int CLK = 4; //Set the CLK pin connection to the display
const int DIO = 2; //Set the DIO pin connection to the display

TM1637Display display(CLK, DIO); //set up the 4-Digit Display.
WiFiClient espClient;
PubSubClient client(espClient);
//long lastMsg = 0;
//char msg[50];
//int nove = 0;
int value = 0;
void setup_wifi() {
   delay(100);
  // We start by connecting to a WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(500);
      Serial.print(".");
    }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void callback(char *topic, byte *payload, unsigned int length) {
String string;
int string1;
int res1;
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
display.showNumberDec(0, true);  // Expect: 0000
delay(500);
  int cantidad=0 ;   // cantidad de registros  //  number of records
  for (int i = 0; i < length; i++) {
  string+=((char)payload[i]);   
  res1+=i;   
  }

  Serial.println(string);  // Imprime mensajes MQTT Recibidos // Prints MQTT messages received
  string1 = string.toInt();  
 display.showNumberDec(string1,false,4,0); //Display the numCounter value;       Serial.println(test - '0');
Serial.println(res1);

if (res1 >= timereset) {
Serial.println(res1);
  delay(5000);
 display.showNumberHexEx(0xff, 0, true);        // Expect: f1Af
  delay(5000);
   Serial.println("Reset..");
    ESP.restart();
}

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) 
  {
 String client_id = "esp8266-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
      if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
          Serial.println("Public mqtt broker connected");
      client.subscribe("pixi3cr/f/c1");
       client.subscribe("pixi3cr/f/v1");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 6 seconds before retrying
      delay(6000);
    }
  }
} //end reconnect()

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  display.setBrightness(0x0a); //set the diplay to maximum brightness
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.setCallback(callback);
  client.loop();
}
