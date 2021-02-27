/*DPN Insole Microcontroller Code

   Author: Rohan Sanda
   Year: 2019 (edited 2021)

   Description: This is the script for the Arduino MKR 1010 WiFi microcontroller on the insoles.
   The script uses the custom library I creted called FSR33 that is used to help store values
   created by the force sensitive resistor networks on the insoles. The code provides the
   full implementation for the WiFi functionality and foot placement detection algorithms. It
   is meant to be used with the Webhooks tool from IFTTT.

   NOTE ALL PIN NUMBERS ARE BASED OFF THE ARDUINO MKR 1010 WIFI BOARD
*/


#include <Arduino.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include "Fsr33.h"

/*Network information*/
#define SECRET_SSID "XXXXXXXX" //Wifi network and password
#define SECRET_PASS "XXXXXXXX"

const char* ssid     = "XXXXXXXXX";
const char* password = "XXXXXXXXX";
const char* standardforce = "/trigger/sfm/with/key/XXXXXXXXXXXXXXXXXXX";    //IFTTT API Link
const char* peripheralforce = "/trigger/pfm/with/key/XXXXXXXXXXXXXXXXXXXX";
const char* googlesheets = "/trigger/add_data/with/key/XXXXXXXXXXXXXXXXXX";
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

/*The Fsr constructor from the header file takes in twp parameters:
   @param1 = analogIn pin on board
   @param2 = anatomical feature the sensor monitors

   The public constructor will initialize each Fsr sensor.
*/
Fsr33 fsr_heel(7, "Heel");
Fsr33 fsr_metleft(8, "Left Metatarsel");
Fsr33 fsr_metright(9, "Right Metatarsel");
Fsr33 fsr_toepad1(10, "Big Toe");
Fsr33 fsr_toepad3(11, "Middle Toe");
Fsr33 fsr_toepad4(12, "Small Toe");
Fsr33 fsr_edgeleft(13, "Left Edge");
Fsr33 fsr_edgeright (6, "Right Edge");
Fsr33 fsr_edgetoepad1(5, "Big Toe Tip");
Fsr33 fsr_edgetoepad3(4, "Middle Toe Tip");
Fsr33 fsr_edgetoepad5(3, "Small Toe Tip");


/*Group the fsr objects into two arrays, one for periperal sensors and one for standard points of contact.*/
Fsr33 standard[] = { fsr_heel, fsr_metleft, fsr_metright, fsr_toepad1, fsr_toepad3, fsr_toepad4 };
Fsr33 peripheral[] = { fsr_edgeleft, fsr_edgeright, fsr_edgetoepad1, fsr_edgetoepad3, fsr_edgetoepad5 };





void setup() {
  Serial.begin(9600);
  init1();

  pinMode(motordisk, OUTPUT);

  Serial.println("DPN Insole Prototype");
  Serial.println("Testing v. 1");
  Serial.println("Please enter a command...");
  Serial.println();
}


void loop() {

  /*Currently, controls occur through the Serial port as an app has not yet been built to facillitate 
   * user remote control. If the efficacy of the design is demonstrated through the prototype and 
   * and remaining resources exist, I will look into creating one.*/
  while (Serial.available()) {
    if (Serial.available()) {
      String str = Serial.readString();

      /*Begin testing for improper positions. Use this most often. */
      if (str.equalsIgnoreCase("begin")) {

        Serial.println("Enter duration (min.)");
        Serial.flush();
        delay(3000);
        beginMethod();

        /*In the prototyping phase, I only wanted the program to run for a certain amount of time
         * to conserve energy. In future implementations the program will constantly run, and have
         * some sort of power-saving controls.
         */
        if (newData) {
          duration = 12 * (readValue.toInt());        //checks foot position 12 times per minute.
          Serial.println(duration);
          newData = false;
        }

        for (int i = 1; i <= duration; i++) {
          for (int j = 0; j < 6; j++) {
            if (standard[j].standardForce()) {        //check each standard fsr for improper positions
              String sname = standard[j].getPart();    //if so, then find the part that's misplaced and trigger an alert

              smakeIFTTTRequest(sname);
              googlesheetsadd(sname);
            }
          }

          for (int k = 0; k < 5; k++) {               //check each peripheral fsr for unwanted pressure
            if (peripheral[k].peripheralForce()) {    //if so, then get the misplaced part and trigger an alert
              String pname = peripheral[k].getPart();
           
              pmakeIFTTTRequest(pname);
              googlesheetsadd(pname);
            }
          }

          delay(5000);

        }
        Serial.flush();
      }


      /*Initializes the test sensation function with the motor disks. Can 
       * be used by the patient to see if their sensation is getting worse during 
       * the day. In the future, I will implement this function into one large control
       * function so that it can be called simultaneously while the main pressure 
       * checking algorithms are running.*/
      if (str.equalsIgnoreCase("test sensation")) {
        delay(3000);
        testSensation();
        Serial.flush();
      }

      if (str.equalsIgnoreCase("stop")) {
        exit(1);
      }
    }
  }
}

/* See above for more details.*/
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

/* Starts reads the string inputed in the Serial terminal*/
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
  Serial.print(server);;
  WiFiClient client;
  int retries = 5;
  while (!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if (!!!client.connected()) {
    Serial.println("Failed to connect...");
  }

  Serial.println(standardforce);

  String jsonObject_sf = String("{\"value1\":\"") + sname  + "\"}";

  client.println(String("POST ") + standardforce + " HTTP/1.1");
  client.println(String("Host: ") + server);
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject_sf.length());
  client.println();
  client.println(jsonObject_sf);

  int timeout = 5 * 10; // 5 seconds
  while (!!!client.available() && (timeout-- > 0)) {
    delay(100);
  }
  if (!!!client.available()) {
    Serial.println("No response...");
  }
  while (client.available()) {
    Serial.write(client.read());
  }

  Serial.println("\nclosing connection");
  client.stop();
}

void pmakeIFTTTRequest(String pname) {
  Serial.print("Connecting to ");
  Serial.print(server);

  WiFiClient client;
  int retries = 5;
  while (!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if (!!!client.connected()) {
    Serial.println("Failed to connect...");
  }

  Serial.println(peripheralforce);

  // Temperature in Celsius
  String jsonObject_pf = String("{\"value1\":\"") + pname  + "\"}";



  client.println(String("POST ") + peripheralforce + " HTTP/1.1");
  client.println(String("Host: ") + server);
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject_pf.length());
  client.println();
  client.println(jsonObject_pf);

  int timeout = 5 * 10; // 5 seconds
  while (!!!client.available() && (timeout-- > 0)) {
    delay(100);
  }
  if (!!!client.available()) {
    Serial.println("No response...");
  }
  while (client.available()) {
    Serial.write(client.read());
  }

  Serial.println("\nclosing connection");
  client.stop();
}

void googlesheetsadd(String aname) {
  Serial.print("Connecting to ");
  Serial.print(server);

  WiFiClient client;
  int retries = 5;
  while (!!!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();
  if (!!!client.connected()) {
    Serial.println("Failed to connect...");
  }

  Serial.println(googlesheets);

  // Temperature in Celsius
  String jsonObject_sf = String("{\"value1\":\"") + aname  + "\"}";


  client.println(String("POST ") + googlesheets + " HTTP/1.1");
  client.println(String("Host: ") + server);
  client.println("Connection: close\r\nContent-Type: application/json");
  client.print("Content-Length: ");
  client.println(jsonObject_sf.length());
  client.println();
  client.println(jsonObject_sf);

  int timeout = 5 * 10; // 5 seconds
  while (!!!client.available() && (timeout-- > 0)) {
    delay(100);
  }
  if (!!!client.available()) {
    Serial.println("No response...");
  }
  while (client.available()) {
    Serial.write(client.read());
  }

  Serial.println("\nclosing connection");
  client.stop();
}
