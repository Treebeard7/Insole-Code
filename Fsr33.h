#include "Arduino.h"
#ifndef Fsr33_h
#define Fsr33_h

class Fsr33 {
 
 
  public:
    Fsr33(int attachTo, int c, String a);
    int counter;
    String Name;
    int pin;
    int resetcounter1;
    boolean standardForce();
    boolean peripheralForce();
    String getPart();
    
    
};
   

#endif 
