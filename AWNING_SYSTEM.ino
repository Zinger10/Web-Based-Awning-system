// Import required libraries
#include "WiFi.h"
#include "ESPAsyncWebSrv.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>

// Replace with your network credentials
const char* ssid = "LWIFI";
const char* password = "wenrui123.";

#define DHTPIN 27             // Digital pin connected to the DHT sensor
#define LDRPIN 32             // Analog pin connected to the LDR sensor
#define HANGER_ON 5          // Digital pin connected to the red led indicating hanger pulled out
#define HANGER_OFF 18         // Digital pin connected to the green led indicating hanger pulled back
#define HANGER_ON_BUTTON 13   // Digital pin connected to the button to pull out the hanger
#define HANGER_OFF_BUTTON 26  // Digital pin connected to the button to pull back the hanger
#define lamp 19               // Digital pin connected to the yelllow led indicating the lamp
#define toggle_lamp 25        // Digital pin connected to the toggle to swithc on/off the lamp

#define DHTTYPE    DHT11     // DHT 11


DHT dht(DHTPIN, DHTTYPE);

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Function to read Temperature
String readDHTTemperature() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  //float t = dht.readTemperature(true);
  // Check if any reads failed and exit early (to try again).
  if (isnan(t)) {    
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(t);
    return String(t);
  }
}

// Function to read Humidity
String readDHTHumidity() {
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Failed to read from DHT sensor!");
    return "--";
  }
  else {
    Serial.println(h);
    return String(h);
  }
}

// Function to read LDR
String readLDRValue() {
  // Read the LDR sensor value 
  int ldrValue = analogRead(LDRPIN);
  int mappedValue = map(ldrValue, 0, 4095, 0, 100); // map the ldr value into 0% - 100%)
  if (isnan(mappedValue)) {
    Serial.println("Failed to read from LDR sensor!");
    return "--";
  }
  else {
    Serial.println(mappedValue);
    return String(mappedValue);
  }
}

// Function to read Hanger pulled out button status
bool readButtonState(){
  return digitalRead(HANGER_ON_BUTTON) == LOW; // LOW when the button is released.
}

// Function to read Hanger pulled back button status
bool readButtonStateOff(){
  return digitalRead(HANGER_OFF_BUTTON) == LOW; // LOW when the button is released.
}

// Function to read lamp status
bool readLampToggleState() {
  return digitalRead(toggle_lamp) == LOW; // LOW when the button is released.
}


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .dht-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }


  </style>
</head>
<script>

function Led1() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("ledStatusOff").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "/Led1", true); // Endpoint to turn on the LED for 5 seconds
      xhttp.send();
}

function Led2() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("ledStatus").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "/Led2", true); // Endpoint to turn on the LED for 5 seconds
      xhttp.send();
}



function Led3() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById("lampStatus").innerHTML = this.responseText;
        }
      };
      xhttp.open("GET", "Led3", true); // Endpoint to turn on the LED for 5 seconds
      xhttp.send();
}



setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperature").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperature", true);
  xhttp.send();
}, 5000 ) ;


setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("humidity").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/humidity", true);
  xhttp.send();
}, 5000 ) ;

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ldr").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/ldr", true); // Assuming you added an endpoint for LDR ("/ldr")
  xhttp.send();
}, 1000 ) ;

setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ledStatus").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/ledStatus", true);
  xhttp.send();
}, 1000);

setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("ledStatusOff").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/ledStatusOff", true);
  xhttp.send();
}, 1000);

setInterval(function () {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("lampStatus").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/lampStatus", true);
  xhttp.send();
}, 1000);

</script>
<body>
  <h2>ESP32 Clothes Hanger System</h2>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="dht-labels">Temperature</span> 
    <span id="temperature">%TEMPERATURE%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fas fa-tint" style="color:#00add6;"></i> 
    <span class="dht-labels">Humidity</span>
    <span id="humidity">%HUMIDITY%</span>
    <sup class="units">&percnt;</sup>
  </p>

  <p>
    <i class="fas fa-sun" style="color:#FFD700;"></i> 
    <span class="dht-labels">Light Intensity</span>
    <span id="ldr">%LDR%</span>
    <sup class="units">&percnt;</sup>
  </p>

  <p>
    <i class="fas fa-lightbulb" style="color:#ff0000;"></i> 
    <span class="dht-labels">Awning OFF</span>
    <span id="ledStatusOff">%LEDOFF%</span>
    <br>
     <button onclick="Led1()">On/Off</button>
  </p>
  <p>
    <i class="fas fa-lightbulb" style="color:#00ff00;"></i> 
    <span class="dht-labels">Awning ON</span>
    <span id="ledStatus">%LED%</span>
    <br>
     <button onclick="Led2()">On/Off</button>
  </p>

  <p>
    <i class="fas fa-lightbulb" style="color:#ffcc00;"></i> 
    <span class="dht-labels">LAMP Status</span>
    <span id="lampStatus">%LAMP_LED%</span>
    <br>
    <button onclick="Led3()">LED On</button>
    <button onclick="Led3()">LED Off</button>
  </p>


</body>

</html>)rawliteral";

// Replaces placeholder with DHT values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATURE"){
    return readDHTTemperature();
  }
  else if(var == "HUMIDITY"){
    return readDHTHumidity();
  }
  else if(var == "LDR"){
    return String(readLDRValue());
  }
  else if(var == "LED"){
    return digitalRead(HANGER_ON) == HIGH ? "ON" : "OFF";
  }
  else if(var == "LEDOFF"){
    return digitalRead(HANGER_OFF) == HIGH ? "ON" : "OFF";
  }
  else if(var == "LAMP_LED"){
  return digitalRead(lamp) == HIGH ? "ON" : "OFF";
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  dht.begin();
  
  pinMode(LDRPIN, INPUT);

  pinMode(HANGER_ON, OUTPUT);
  digitalWrite(HANGER_ON, LOW);
  pinMode(HANGER_ON_BUTTON, INPUT_PULLUP);

  pinMode(HANGER_OFF, OUTPUT);
  digitalWrite(HANGER_OFF, LOW);
  pinMode(HANGER_OFF_BUTTON, INPUT_PULLUP);

  pinMode(lamp, OUTPUT);
  digitalWrite(lamp, LOW);
  pinMode(toggle_lamp, INPUT_PULLUP);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTTemperature().c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", readDHTHumidity().c_str());
  });

  server.on("/ldr", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", String(readLDRValue()).c_str());
  });

  server.on("/ledStatus", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", digitalRead(HANGER_ON) == HIGH ? "ON" : "OFF");
  });
  server.on("/ledStatusOff", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", digitalRead(HANGER_OFF) == HIGH ? "ON" : "OFF");
  });
  server.on("/lampStatus", HTTP_GET, [](AsyncWebServerRequest *request){
  request->send_P(200, "text/plain", digitalRead(lamp) == HIGH ? "ON" : "OFF");
  });

 //Handle the request to turn on LED 1
  server.on("/Led1", HTTP_GET, [](AsyncWebServerRequest *request){
  if (digitalRead(HANGER_OFF) == LOW )
  {
  digitalWrite(HANGER_OFF, HIGH); // Turn on the LED
  request->send(200, "text/plain", "LED turned on"); 
  }
  else
  {
    digitalWrite(HANGER_OFF, LOW); // Turn on the LED
  request->send(200, "text/plain", "LED turned off"); 
  }
  // Respond to the request
  
  }); 
  

  //Handle the request to turn on LED 2
  server.on("/Led2", HTTP_GET, [](AsyncWebServerRequest *request){
  if (digitalRead(HANGER_ON) == LOW )
  {
  digitalWrite(HANGER_ON, HIGH); // Turn on the LED
  request->send(200, "text/plain", "LED turned on"); 
  }
  else
  {
    digitalWrite(HANGER_ON, LOW); // Turn on the LED
  request->send(200, "text/plain", "LED turned off"); 
  }
  // Respond to the request
  
  }); 
  
  

  // Handle the request to turn on the LED3
 server.on("/Led3", HTTP_GET, [](AsyncWebServerRequest *request){
  if (digitalRead(lamp) == LOW )
  {
  digitalWrite(lamp, HIGH); // Turn on the LED
  request->send(200, "text/plain", "LED turned on"); 
  }
  else
  {
    digitalWrite(lamp, LOW); // Turn on the LED
  request->send(200, "text/plain", "LED turned off"); 
  }
  // Respond to the request
  
  }); 
  
  
  // Start server
  server.begin();
}
 
void loop(){
  // Check the Hanger_ON state
  if (readButtonState()) {
    digitalWrite(HANGER_ON, HIGH);  // Turn on the LED
  } else {
    digitalWrite(HANGER_ON, LOW);   // Turn off the LED
  }

  // Check the Hanger_OFF state
  if (readButtonStateOff()) {
    digitalWrite(HANGER_OFF, HIGH);  // Turn on the second LED
  } else {
    digitalWrite(HANGER_OFF, LOW);   // Turn off the second LED
  }

  // Check the button state for the lamp LED
  if (readLampToggleState()) {
    // Toggle the LED status only if the button is pressed.
    digitalWrite(lamp, !digitalRead(lamp));  // Toggle the lamp LED state
  }
  delay(10);
}