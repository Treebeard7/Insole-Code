
#include "Arduino.h"
#include "Fsr33.h"

/*Constructor function*/
Fsr33::Fsr33(int attachTo, String a) {
  pinMode(attachTo, INPUT);
  pin = attachTo;
  resetcounter1 = 0;
  Name = a;
}

/*Iterative function that checks if there are any improperly placed feet
   Returns true if there is an improperly placed foot part on a standard
   force sensor. Future versions of the code will implement better algorithsm.
*/
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

/*Same purpose as above but for peripheral force sensors that have slightly different
 * thresholds than above. 
 */
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
