


 #include <ESP8266WiFi.h>
 #include <Adafruit_Sensor.h>
#include <DHT.h>
//CONFIG FOR DHT (temp and humidity)
#define DHTPIN 14     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT22     // DHT 22 (AM2302)

DHT dht(DHTPIN, DHTTYPE);

//pm 2.5, 10 config

int pin2 = 4;
int pin1 = 5;
unsigned long duration1;
unsigned long duration2;

unsigned long starttime;
unsigned long sampletime_ms = 3000;//sampe 1s ;
unsigned long lowpulseoccupancy1 = 0;
unsigned long lowpulseoccupancy2 = 0;
float ratio1 = 0;
float ratio2 = 0;
float concentration1 = 0;
float concentration2 = 0;

 //network config
const char* ssid = "E49253";
const char* password = "EVW32C0S00011471";

//1st lightbulb config 
int led1Pin = 13; // GPIO13
int valueLed1 = LOW;






WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
  dht.begin();
 
  pinMode(led1Pin, OUTPUT);
  digitalWrite(led1Pin, LOW);
 
  // Connect to WiFi network
  Serial.println();
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
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");

    starttime = millis();//get the current time; (for pm sensor)
 
}

void initPmCheck(){
 duration1 = pulseIn(pin1, LOW);
  duration2 = pulseIn(pin2, LOW);
  lowpulseoccupancy1 = lowpulseoccupancy1+duration1;
  lowpulseoccupancy2 = lowpulseoccupancy2+duration2;
Serial.print(millis()-starttime);

  if ((millis()-starttime) > sampletime_ms)//if the sampel time == 30s
  {
    ratio1 = lowpulseoccupancy1/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration1 = 1.1*pow(ratio1,3)-3.8*pow(ratio1,2)+520*ratio1+0.62; // using spec sheet curve

    ratio2 = lowpulseoccupancy2/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration2 = 1.1*pow(ratio2,3)-3.8*pow(ratio2,2)+520*ratio2+0.62; // 

       
    Serial.print("concentration1 = ");
    Serial.print(concentration1);
    Serial.print(" pcs/0.01cf  -  ");

    Serial.print("concentration2 = ");
    Serial.print(concentration2);
    Serial.print(" pcs/0.01cf  -  ");

    

     
  }
    

      
    lowpulseoccupancy1 = 0;
    lowpulseoccupancy2 = 0;
    starttime = millis();
  }



 
void loop() {
 initPmCheck();
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
    
    
  }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // Match the request


// change FIRST LIGHT
  if (request.indexOf("/led1") != -1)  {
    if(valueLed1 == LOW){
        valueLed1 = HIGH;
       digitalWrite(led1Pin, valueLed1);
      }
      else{
          valueLed1 = LOW;
    digitalWrite(led1Pin, valueLed1);
 
    }
  }
//  if (request.indexOf("/OFF") != -1)  {
//    digitalWrite(led1Pin, LOW);
//        Serial.println("NEJDEMOO!");
//
//    valueLed1 = LOW;
//  }
 
// Set ledPin according to the request
//digitalWrite(ledPin, value);
 
  // Return the response
//
//  
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/json");
  client.println(""); //  do not forget this one

client.print("{\"temperatura\":\"");
client.print(dht.readTemperature());
client.print("\", \"vlaga\":\"");
client.print(dht.readHumidity());
client.print("\", \"led1\":");

  if(valueLed1 == HIGH) {
    client.print("\"On\"");
  } else {
    client.print("\"Off\"");
  }
  client.print("}");




  
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");

}
 
