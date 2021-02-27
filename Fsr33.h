#include "Arduino.h"
#ifndef Fsr33_h
#define Fsr33_h

class Fsr33 {


  public:
    Fsr33(int attachTo, String a);
    int counter = 0;
    String Name;
    int pin;
    int resetcounter1;
    boolean standardForce();
    boolean peripheralForce();
    String getPart();

};


#endif
