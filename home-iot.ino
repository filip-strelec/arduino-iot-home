

 #include <ESP8266WiFi.h>
 
const char* ssid = "E49253";
const char* password = "EVW32C0S00011471";
 
int led1Pin = 13; // GPIO13
int valueLed1 = LOW;




WiFiServer server(80);
 
void setup() {
  Serial.begin(115200);
  delay(10);
 
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
 
}
 
void loop() {
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
//  client.println("<!DOCTYPE HTML>");
//  client.println("<html>");
// 
//  client.print("Led pin: ");
// 
//  if(value == HIGH) {
//    client.print("On");
//  } else {
//    client.print("Off");
//  }
//  client.println("</html>");

client.print("{\"temperatura\":\"20\", \"vlaga\":\"2\", \"led1\":");
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
 
