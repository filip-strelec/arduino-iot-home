

 #include <ESP8266WiFi.h>
 #include <Adafruit_Sensor.h>
#include <DHT.h>
//CONFIG FOR DHT (temp and humidity)
#define DHTPIN 14     // Digital pin connected to the DHT sensor
#define DHTTYPE    DHT22     // DHT 22 (AM2302)
DHT dht(DHTPIN, DHTTYPE);


//define etanoll
float ETH_PPM;

//pm 2.5, 10 config

int pin2 = 5;
int pin1 = 4;
unsigned long duration1;
unsigned long duration2;

unsigned long starttime;
unsigned long sampletime_ms = 30000;//sampe 1s ;
unsigned long lowpulseoccupancy1 = 0;
unsigned long lowpulseoccupancy2 = 0;
float ratio1 = 0;
float ratio2 = 0;
float concentration1 = 0;
float concentration2 = 0;

//LPG CONFIG
 float LPG_PPM;

 //network config
const char* ssid = "E49253";
const char* password = "pass";

//1st lightbulb config 
int relay1Pin = 13; // GPIO13
int valueLed1 = LOW;

//2nd lightbulb config 
int relay2Pin = 0; // 
int valueLed2 = LOW;

//3rd lightbulb config 
int relay3Pin = 2; // 
int valueLed3 = LOW;

int relayHeatingPin = 12;
int valueHeating = LOW;







WiFiServer server(80);
 
void setup() {
  
  Serial.begin(115200);
  delay(10);
  dht.begin();
 
  pinMode(relay1Pin, OUTPUT);
  digitalWrite(relay1Pin, LOW);

  


  pinMode(relayHeatingPin, OUTPUT);
  digitalWrite(relayHeatingPin, LOW);



  pinMode(relay2Pin, OUTPUT);
  digitalWrite(relay2Pin, LOW);




  pinMode(relay3Pin, OUTPUT);
  digitalWrite(relay3Pin, LOW);

  
 
  // Connect to WiFi network
  // Serial.println();
  // Serial.println();
   Serial.print("Connecting to ");
   Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    // Serial.print(".");
  }
  // Serial.println("");
  // Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println(F("Server started"));
 
  // Print the IP address
  Serial.print(F("URL: "));
  Serial.print(F("http://"));
  Serial.print(WiFi.localIP());
  Serial.println(F("/"));

    starttime = millis();//get the current time; (for pm sensor)
 
}

void initPmCheck(){
 duration1 = pulseIn(pin1, LOW);
  duration2 = pulseIn(pin2, LOW);
  lowpulseoccupancy1 = lowpulseoccupancy1+duration1;
  lowpulseoccupancy2 = lowpulseoccupancy2+duration2;
//Serial.print(millis()-starttime);

  if ((millis()-starttime) > sampletime_ms)//if the sampel time == 30s
  {
    ratio1 = lowpulseoccupancy1/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration1 = 1.1*pow(ratio1,3)-3.8*pow(ratio1,2)+520*ratio1+0.62; // using spec sheet curve

    ratio2 = lowpulseoccupancy2/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration2 = 1.1*pow(ratio2,3)-3.8*pow(ratio2,2)+520*ratio2+0.62; // 

       
//     Serial.print("concentrationPM10 = ");
//     Serial.print(concentration1);
//     Serial.print(" pcs/0.01cf  -  ");

//     Serial.print("concentration2.5 = ");
//     Serial.print(concentration2);
//     Serial.print(" pcs/0.01cf  -  ");
//  Serial.println("_ ");
       lowpulseoccupancy1 = 0;
    lowpulseoccupancy2 = 0;
    starttime = millis();

     
  }
    

      
 
  }

  //alcohol MQ-3 sensor

  void alcoholCheck(){


   float sensor_volt;
    float RS_gas; // Get value of RS in a GAS
    float ratio; // Get ratio RS_GAS/RS_air
    
 
    int sensorValue = analogRead(A0);
 
    sensor_volt=(float)sensorValue/1024*5.0;
 
    RS_gas = (5.0-sensor_volt)/sensor_volt; // omit *RL /*-Replace the name "R0" with the value of R0 in the demo of First Test -*/
 
    ratio = RS_gas/0.21;  // ratio = RS/R0

for(int i = 0 ; i < 20 ; i++)
   {
      ETH_PPM =ETH_PPM + 0.037283 + (5.334452 - 0.197283)/(1 + pow((ratio/0.2771443), 3.229137));
   }

   

ETH_PPM = ETH_PPM/20;
if(ETH_PPM < 0.05) {
  ETH_PPM = 0;
  }
  

  }

//FOR MQ-6 sensor
//void gasCheck(){ 
//
//   float sensor_volt;
//    float RS_gas; // Get value of RS in a GAS
//    float ratio; // Get ratio RS_GAS/RS_air
//   
//    float R0 = 0.75;
//    int sensorValue = analogRead(A0);
// 
//    sensor_volt=(float)sensorValue/1024*5.0;
// 
//    RS_gas = (5.0-sensor_volt)/sensor_volt; // omit *RL /*-Replace the name "R0" with the value of R0 in the demo of First Test -*/
// 
//    ratio = RS_gas/R0;  // ratio = RS/R0
//   
//    LPG_PPM = 106.0382 + (14304.77 - 106.0382)/(1 + pow((ratio/0.4002885),2.885229) );
// 
//}
 
void loop() {
 initPmCheck();
 alcoholCheck();
// gasCheck();

  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
 
  // Wait until the client sends some data

//   Serial.println(F("new client"));
// IS MAKING PROBLEMS
  // while(!client.available()){
  //   delay(1);
    
    
  // }
 
  // Read the first line of the request
  String request = client.readStringUntil('\r');
  // Serial.println(request);
  client.flush();
 
  // Match the request


// change FIRST LIGHT
  if (request.indexOf("/led1") != -1)  {
      

    if(valueLed1 == LOW){
        valueLed1 = HIGH;
       digitalWrite(relay1Pin, valueLed1);
      }
      else{
          valueLed1 = LOW;
    digitalWrite(relay1Pin, valueLed1);
 
    }
  }


else if (request.indexOf("/led2") != -1)  {
    

    if(valueLed2 == LOW){
        valueLed2 = HIGH;
       digitalWrite(relay2Pin, valueLed2);
      }
      else{
          valueLed2 = LOW;
    digitalWrite(relay2Pin, valueLed2);
 
    }

  }



 else if (request.indexOf("/led3") != -1)  {

//  Serial.println("led3");
//    Serial.println(valueLed3);

    if(valueLed3 == LOW){
        valueLed3 = HIGH;
       digitalWrite(relay3Pin, valueLed3);
      }
      else{
          valueLed3 = LOW;
    digitalWrite(relay3Pin, valueLed3);
 
    }
    

  }
  

 else if (request.indexOf("/heat") != -1)  {
    

    if(valueHeating == LOW){
        valueHeating = HIGH;
       digitalWrite(relayHeatingPin, valueHeating);
      }
      else{
          valueHeating = LOW;
    digitalWrite(relayHeatingPin, valueHeating);
 
    }
  }



 else if (request.indexOf("/turnOff") != -1)  {
  valueHeating = LOW;
       digitalWrite(relayHeatingPin, valueHeating);
            valueLed3 = LOW;
    digitalWrite(relay3Pin, valueLed3);
            valueLed2 = LOW;
    digitalWrite(relay2Pin, valueLed2);
         valueLed1 = LOW;
    digitalWrite(relay1Pin, valueLed1);
 

   
  }




  else {
    
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
  client.println(F("HTTP/1.1 200 OK"));
  client.println(F("Content-Type: application/json"));
  client.println(F("Access-Control-Allow-Origin: *"));
  client.println(F(""));
//
client.print(F("{\"temperatura\":\""));
client.print(dht.readTemperature());
client.print(F("\", \"vlaga\":\""));
client.print(dht.readHumidity());
client.print(F("\", \"PM:2.5\":\""));
client.print(concentration2);
client.print(F("\", \"PM:10\":\""));
client.print(concentration1);
client.print(F("\", \"Alcohol-PPM\":\""));
client.print(ETH_PPM);

client.print(F("\", \"led1\":"));

  if(valueLed1 == HIGH) {
    client.print(F("\"On"));
  } else {
    client.print(F("\"Off"));
  }

  client.print(F("\", \"led2\":"));

  if(valueLed2 == HIGH) {
    client.print(F("\"On"));
  } else {
    client.print(F("\"Off"));
  }

client.print(F("\", \"led3\":"));

  if(valueLed3 == HIGH) {
    client.print(F("\"On"));
  } else {
    client.print(F("\"Off"));
  }






client.print(F("\", \"heat\":"));

  if(valueHeating == HIGH) {
    client.print(F("\"On\""));
  } else {
    client.print(F("\"Off\""));
  }
  client.print(F("}"));



  
 
  delay(1);
  // Serial.println("Client disonnected");
  // Serial.println("");

}
 
