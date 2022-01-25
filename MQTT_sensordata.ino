#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <Arduino_MKRENV.h>
const char* ssid = "IOT";
const char* password = "Arduino2020";
const char* mqtt_server = "172.16.12.10";

WiFiClient espClient;
PubSubClient client(espClient);


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
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


void reconnect() 
  {
        // Loop until we're reconnected
    while (!client.connected()) 
      {
        Serial.print("Attempting MQTT connection...");
          // Create a random client ID
        String clientId = "ESP8266Client-";
        clientId += String(random(0xffff), HEX);
            // Attempt to connect
        if (client.connect(clientId.c_str())) 
          {
            Serial.println("connected");
          } 
        else 
          {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
                // Wait 5 seconds before retrying
            delay(5000);
          }
      }
  }

void setup() 
  {
      if (!ENV.begin()) 
        {
          Serial.println("Failed to initialize MKR ENV shield!");
          while (1);
        }

      Serial.begin(115200);
      setup_wifi();
      client.setServer(mqtt_server, 1883);
  }

unsigned long previousMillis = 0;
int interval = 1000;


void loop() 
{
      // read all the sensor values
  float humidity    = ENV.readHumidity();
  float pressure    = ENV.readPressure();
  float illuminance = ENV.readIlluminance();
  float uva         = ENV.readUVA();
  float uvb         = ENV.readUVB();
  float uvIndex     = ENV.readUVIndex();

  Serial.print("Humidity    = ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("Pressure    = ");
  Serial.print(pressure);
  Serial.println(" kPa");

  Serial.print("Illuminance = ");
  Serial.print(illuminance);
  Serial.println(" lx");

  Serial.print("UVA         = ");
  Serial.println(uva);

  Serial.print("UVB         = ");
  Serial.println(uvb);

  Serial.print("UV Index    = ");
  Serial.println(uvIndex);

    // print an empty line
  Serial.println();

    // wait 1 second to print again
  delay(1000);
  if (!client.connected()) 
    {
      reconnect();
    }
  client.loop();

  int temperature = int(ENV.readTemperature());
  float temp = temperature ;
  char tempC[5];
  sprintf(tempC, "%.2f", temp);
  client.publish("temperatur", tempC);
  
  int lluminance = int(ENV.readIlluminance());
  float lys = lluminance ;
  char lux[5];
  sprintf(lux, "%.2f", lys);
  client.publish("lysstyrke", lux);  

  int Humidity = int(ENV.readHumidity());
  float humD = Humidity ;
  char hum[5];
  sprintf(hum, "%.2f", humD);
  client.publish("fuktighet", hum);

  int Pressure = int(ENV.readPressure());
  float trykk = Pressure ;
  char hPa[5];
  sprintf(hPa, "%.2f", trykk);
  client.publish("lufttrykk", hPa);
  
}
