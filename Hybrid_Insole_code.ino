#include <Arduino.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <Fsr33.h>


#define SECRET_SSID "ORBI72"
#define SECRET_PASS "blackcar899"

const char* ssid     = "ORBI72";
const char* password = "blackcar899";
const char* standardforce = "/trigger/sfm/with/key/c1yTRHdDNP5x2irnUgpuy0";
const char* peripheralforce = "/trigger/pfm/with/key/c1yTRHdDNP5x2irnUgpuy0";
const char* googlesheets = "/trigger/add_data/with/key/c1yTRHdDNP5x2irnUgpuy0";
const char* server = "maker.ifttt.com";

int keyIndex = 0;
char sssid[] = SECRET_SSID;        
char spass[] = SECRET_PASS; 
int status = WL_IDLE_STATUS;                    
 

boolean transmitting;
String input = "";
String readValue;
int duration;
const int motordisk = 2;
int sfcounter = 0;
int pfcounter = 0;
boolean newData;

Fsr33 fsr_heel(7, 0, "Heel"); //I have not finalized the analog input pins
Fsr33 fsr_metleft(8, 0, "Left Metatarsel");
Fsr33 fsr_metright(9, 0, "Right Metatarsel");
Fsr33 fsr_toepad1(10, 0, "Big Toe");
Fsr33 fsr_toepad3(11, 0, "Middle Toe");
Fsr33 fsr_toepad4(12, 0, "Small Toe");
Fsr33 fsr_edgeleft(13, 0, "Left Edge");
Fsr33 fsr_edgeright (6, 0, "Right Edge");
Fsr33 fsr_edgetoepad1(5, 0, "Big Toe Tip");
Fsr33 fsr_edgetoepad3(4, 0, "Middle Toe Tip");
Fsr33 fsr_edgetoepad5(3, 0, "Small Toe Tip");

Fsr33 standard[] = {fsr_heel, fsr_metleft, fsr_metright, fsr_toepad1, fsr_toepad3, fsr_toepad4}; 
Fsr33 peripheral[] = {fsr_edgeleft, fsr_edgeright, fsr_edgetoepad1, fsr_edgetoepad3, fsr_edgetoepad5};




void setup() {
  //while (! Serial) {delay(1);}
 
  Serial.begin(9600);
  init1();
  pinMode(motordisk, OUTPUT);
  Serial.println("DPN Insole Prototype");
  Serial.println("Testing v. 1");
  Serial.println("Please enter a command...");
  Serial.println();


  
}

void loop() {

    while (Serial.available()) {
    if (Serial.available()) {
      String str = Serial.readString();
     

      if (str.equalsIgnoreCase("begin"))
      {
       
        Serial.println("Enter duration (min.)");
        Serial.flush();
        delay(5000);
        beginMethod();

        if (newData == true) {
        duration = 12*(readValue.toInt());
        //duration =  duration*6;
        Serial.println(duration);
        newData=false;
        }
        
        for (int i = 1; i <= duration; i++) {    
          
          for (int j = 0; j < 6; j++) {
            if (!standard[j].standardForce()) {
              String sname = standard[j].getPart();
          
              //init();
              smakeIFTTTRequest(sname);
              googlesheetsadd(sname);
            }
  
            }
          

          for (int k = 0; k < 5; k++) {
            if (peripheral[k].peripheralForce()) {
              String pname = peripheral[k].getPart();
              //init();
              pmakeIFTTTRequest(pname);
            }
          }
       
          delay(5000); 
               
        }

   
        Serial.flush();
      }

      if (str.equalsIgnoreCase("test sensation")) {
        delay(5000);
        testSensation();
        Serial.flush();
      }

      if (str.equalsIgnoreCase("stop")) 
      {
        exit(1);
      }
      
    }
  
  }
  
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
  String remp = input;
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
