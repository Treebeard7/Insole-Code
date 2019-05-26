
#include "Arduino.h"
#include "Fsr33.h"

Fsr33::Fsr33(int attachTo, int c, String a)
{
  pinMode(attachTo, INPUT);
  pin = attachTo;
  resetcounter1 = 0;
  counter = c;
  Name = a;
}

boolean Fsr33::standardForce() {
int fsrReading = analogRead(pin);
resetcounter1++;
      if (fsrReading > 250) {
        return false;
      }  
      else {
        counter++;
      } 
      if (counter >= 5 && ((resetcounter1 <= 4) && (resetcounter1 >= 2))) {
       Serial.println("WARNING! Incorrect foot positioning at ");
       Serial.print(Name);
       counter = 0;
       resetcounter1 = 0;
       return true;
      }
       else if (counter >= 5 && resetcounter1 >= 5) {
        counter = 0;
        resetcounter1 = 0;
       }
       
      else {
        return false;
      }
     return false;
    }

boolean Fsr33::peripheralForce()  {
  int fsrReading1 = analogRead(pin);
  resetcounter1++;
      if (fsrReading1 > 250) {
        counter++;
      } else {
        return false;
      }
      if (counter >= 5 && ((resetcounter1 <= 4) && (resetcounter1 >= 2))) {
       Serial.println("WARNING! Dangerous Foot-Shoe Friction at ");
       Serial.print(Name);
       counter = 0;
       resetcounter1 = 0;
       return true;
      } else {
        return false;
      }
      return false;
      }
      

String Fsr33::getPart() {
  return Name;
}
