#include <Arduino.h>
#include <SPI.h>
#include <WiFiNINA.h>


//Fsr pins
int fsr_heel = A0;
int fsr_metleft = A1;
int fsr_metright = A2;
int fsr_toepad1 = A3;
int fsr_toepad3 = A4;
//int fsr_toepad4 = A5;
int fsr_edgeleft = A5;
int fsr_edgeright = A6;
//Fsr counters
int counter1;
int counter2;
int counter3;
int counter4;
int counter5;
int counter6;
int counter7;
int resetcounter1;


#define SECRET_SSID "ORBI72"
#define SECRET_PASS "blackcar899"

const char* ssid     = "ORBI72";
const char* password = "blackcar899";
const char* standardforce = "/trigger/sfm/with/key/c1yTRHdDNP5x2irnUgpuy0";
const char* peripheralforce = "/trigger/pfm/with/key/c1yTRHdDNP5x2irnUgpuy0";
const char* googlesheets = "/trigger/add_data/with/key/c1yTRHdDNP5x2irnUgpuy0";
const char* server = "maker.ifttt.com";

int keyIndex = 0;
char sssid[] = SECRET_SSID;        // your network SSID (name)
char spass[] = SECRET_PASS; 
int status = WL_IDLE_STATUS;                     // the Wifi radio's status
 

boolean transmitting;
String input = "";
String readValue;
int duration;
const int motordisk = 10;
int sfcounter = 0;
int pfcounter = 0;
boolean newData;


int returnCounter(int pin) {
  if (pin == fsr_heel) {
    return counter1;
}
 else if (pin == fsr_metleft) {
  return counter2;
 }
 else if (pin == fsr_metright) {
  return counter3;
 }
 else if (pin == fsr_toepad1) {
  return counter4;
 }
 else if (pin == fsr_toepad3) {
  return counter5;
 }
 else if (pin == fsr_edgeleft) {
  return counter6;
 }
 else  {
  return counter7;
 }
}

void incrementCounter(int pin) {
if (pin == fsr_heel) {
     counter1++;
}
 else if (pin == fsr_metleft) {
   counter2++;
 }
 else if (pin == fsr_metright) {
   counter3++;
 }
 else if (pin == fsr_toepad1) {
   counter4++;
 }
 else if (pin == fsr_toepad3) {
   counter5++;
 }
 else if (pin == fsr_edgeleft) {
   counter6++;
 }
 else  {
   counter7++;
 }
}

void zeroCounter(int pin) {
if (pin == fsr_heel) {
     counter1 = 0;
}
 else if (pin == fsr_metleft) {
   counter2 = 0;
 }
 else if (pin == fsr_metright) {
   counter3 = 0;
 }
 else if (pin == fsr_toepad1) {
   counter4 = 0;
 }
 else if (pin == fsr_toepad3) {
   counter5 = 0;
 }
 else if (pin == fsr_edgeleft) {
   counter6 = 0;
 }
 else  {
   counter7 = 0;
 }
}

String getName(int pin) {
  String aaa;
if (pin == fsr_heel) {
    aaa = "Left Heel";
}
 else if (pin == fsr_metleft) {
  aaa =  "Left left metatarsel";
 }
 else if (pin == fsr_metright) {
  aaa = "Left right metatarsel";
 }
 else if (pin == fsr_toepad1) {
  aaa = "Left first toepad1";
 }
 else if (pin == fsr_toepad3) {
  aaa = "Left third toepad" ;
 }
 else if (pin == fsr_edgeleft) {
  aaa =  "Left left edge";
 }
 else  {
  aaa = "Left right edge";
 }
 return aaa;
}

boolean teststandardfsr(int a) {
  int b = a;
 
  int fsrReading = analogRead(b);
  if (fsrReading > 20) {
    return false;
  }
  else {  
    incrementCounter(b);
  }
    if (returnCounter(b) >= 5) {
       Serial.println("WARNING! Incorrect foot positioning at ");
       //Serial.print(Name);
       zeroCounter(b);
       return true;
      }
      else {
        return false;
      }
     return false;
  }

boolean testperipheralfsr(int j) {
  int k = j;
 
  int fsrReading1 = analogRead(k);
  if (fsrReading1 < 20) {
    return false;
  }
  else {  
    incrementCounter(k);
  }
    if (returnCounter(k) >= 5) {
       Serial.println("WARNING! Incorrect foot positioning at ");
       //Serial.print(Name);
       zeroCounter(k);
       return true;
      }
      else {
        return false;
      }
     return false;
  }


void hardreset() {
counter1 = 0;
counter2 = 0;
counter3 = 0;
counter4 = 0;
counter5 = 0;
counter6 = 0;
counter7 = 0;
}





void setup() {
  while (! Serial) {delay(1);}
 
  Serial.begin(9600);
  init1();
  pinMode(motordisk, OUTPUT);
  
  Serial.println("DPN Insole Prototype");
  Serial.println("Testing v. 1");
  
  Serial.println();  
}

void loop() {

      resetcounter1++;
       if (teststandardfsr(fsr_heel)){
      String sname = getName(fsr_heel);
      smakeIFTTTRequest(sname);
      googlesheetsadd(sname);
     }
     if (teststandardfsr(fsr_metleft)){
      String sname = getName(fsr_metleft);
      smakeIFTTTRequest(sname);
      googlesheetsadd(sname);
     }
      if (teststandardfsr(fsr_metright)){
      String sname = getName(fsr_metright);
      smakeIFTTTRequest(sname);
      googlesheetsadd(sname);
     }
     if (teststandardfsr(fsr_toepad1)){
      String sname = getName(fsr_toepad1);
      smakeIFTTTRequest(sname);
      googlesheetsadd(sname);
     }
      if (teststandardfsr(fsr_toepad3)){
      String sname = getName(fsr_toepad3);
      smakeIFTTTRequest(sname);
      googlesheetsadd(sname);
     }

     if (testperipheralfsr(fsr_edgeleft)) {
      String pname = getName(fsr_edgeleft);
      pmakeIFTTTRequest(pname);
      googlesheetsadd(pname);
     }
      if (testperipheralfsr(fsr_edgeright)) {
      String pname = getName(fsr_edgeright);
      pmakeIFTTTRequest(pname);
      googlesheetsadd(pname);
     }
     if ((resetcounter1 % 7) == 0) {
      hardreset();
     }

delay(5000);

  
}

void testSensation () {
  Serial.println("Initiating Pulse Mechanism");
  delay(500);
  Serial.println();
  delay (3000);
  
  digitalWrite(motordisk, HIGH);
  Serial.println("ON");
  delay(1000);
  Serial.println();
  
  digitalWrite(motordisk, LOW);
  Serial.println("OFF");
  delay(1000);
  Serial.println();

  digitalWrite(motordisk, HIGH);
  Serial.println("ON");
  delay(1000);
  Serial.println();

  digitalWrite(motordisk, LOW);
  Serial.println("OFF");
  delay(1000);
  Serial.println();

   digitalWrite(motordisk, HIGH);
  Serial.println("ON");
  delay(1000);
  Serial.println();

  digitalWrite(motordisk, LOW);
  Serial.println("OFF");
  delay(1000);
  Serial.println();
  
  Serial.println("Pulse Mechanism Complete");
  Serial.flush();
  
}

void beginMethod() {
  if (Serial.available() > 0) {
    readValue = Serial.readString();
    newData = true;
  }
}

void init1() {
  Serial.print("Connecting to: "); 
  Serial.print(ssid);
    while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WEP network, SSID: ");
    Serial.println(sssid);
    status = WiFi.begin(sssid, keyIndex, spass);

    // wait 10 seconds for connection:
    delay(1000);
  }

  // once you are connected :
  Serial.print("You're connected to the network");
}

void smakeIFTTTRequest(String sname) {
  Serial.print("Connecting to "); 
  Serial.print(server);
  String temp = sname;
  WiFiClient client;
  int retries = 5;
  while(!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if(!!!client.connected()) {
    Serial.println("Failed to connect...");
  }
  
  Serial.print("Request resource: "); 
  Serial.println(standardforce);

  // Temperature in Celsius
  String jsonObject_sf = String("{\"value1\":\"") + temp  + "\"}";
                      
 
                      
  client.println(String("POST ") + standardforce + " HTTP/1.1");
  client.println(String("Host: ") + server); 
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject_sf.length());
  client.println();
  client.println(jsonObject_sf);
  
  int timeout = 5 * 10; // 5 seconds             
  while(!!!client.available() && (timeout-- > 0)){
    delay(100);
  }
  if(!!!client.available()) {
    Serial.println("No response...");
  }
  while(client.available()){
    Serial.write(client.read());
  }
  
  Serial.println("\nclosing connection");
  client.stop(); 
}

void pmakeIFTTTRequest(String pname) {
  Serial.print("Connecting to "); 
  Serial.print(server);
  String remp = pname;
  WiFiClient client;
  int retries = 5;
  while(!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if(!!!client.connected()) {
    Serial.println("Failed to connect...");
  }
  
  Serial.print("Request resource: "); 
  Serial.println(peripheralforce);

  // Temperature in Celsius
  String jsonObject_pf = String("{\"value1\":\"") + remp  + "\"}";
                      
 
                      
  client.println(String("POST ") + peripheralforce + " HTTP/1.1");
  client.println(String("Host: ") + server); 
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject_pf.length());
  client.println();
  client.println(jsonObject_pf);
        
  int timeout = 5 * 10; // 5 seconds             
  while(!!!client.available() && (timeout-- > 0)){
    delay(100);
  }
  if(!!!client.available()) {
    Serial.println("No response...");
  }
  while(client.available()){
    Serial.write(client.read());
  }
  
  Serial.println("\nclosing connection");
  client.stop(); 
}

void googlesheetsadd(String aname) {
  Serial.print("Connecting to "); 
  Serial.print(server);
  String temp = aname;
  WiFiClient client;
  int retries = 5;
  while(!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if(!!!client.connected()) {
    Serial.println("Failed to connect...");
  }
  
  Serial.print("Request resource: "); 
  Serial.println(googlesheets);

  // Temperature in Celsius
  String jsonObject_sf = String("{\"value1\":\"") + temp  + "\"}";
                      
 
                      
  client.println(String("POST ") + googlesheets + " HTTP/1.1");
  client.println(String("Host: ") + server); 
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject_sf.length());
  client.println();
  client.println(jsonObject_sf);
  
  int timeout = 5 * 10; // 5 seconds             
  while(!!!client.available() && (timeout-- > 0)){
    delay(100);
  }
  if(!!!client.available()) {
    Serial.println("No response...");
  }
  while(client.available()){
    Serial.write(client.read());
  }
  
  Serial.println("\nclosing connection");
  client.stop(); 
}
