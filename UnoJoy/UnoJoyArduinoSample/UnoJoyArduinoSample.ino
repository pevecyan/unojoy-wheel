
#include "UnoJoy.h"


bool isCalibrationMode = false;
int steeringMin = 0;
int steeringMax = 1024;
int pedal1Min = 0;
int pedal1Max = 0;
int pedal2Min = 0;
int pedal2Max = 0;

void setup(){
  setupPins();
  setupUnoJoy();
  //Serial.begin(9600);
}

void loop(){

  if (isCalibrationMode){
     dataForController_t controllerData = getCalibrationModeData();
     setControllerData(controllerData); 
     handleCalibration();
  }
  else { 
    // Always be getting fresh data
    dataForController_t controllerData = getControllerData();
    setControllerData(controllerData); 
  }
}

void setupPins(void){
  // Set all the digital pins as inputs
  // with the pull-up enabled, except for the 
  // two serial line pins
  for (int i = 22; i <= 32; i+=2){
    pinMode(i, INPUT);
    digitalWrite(i, HIGH);
  }
  pinMode(A8, INPUT);
  pinMode(A10, INPUT);
  pinMode(A11, INPUT);

}

dataForController_t getControllerData(void){
  int button1 = !digitalRead(22);
  int button2 = !digitalRead(24);
  int button3 = !digitalRead(26);
  int button4 = !digitalRead(28);
  int button5 = !digitalRead(30);
  int button6 = !digitalRead(32);
  
  // Set up a place for our controller data
  //  Use the getBlankDataForController() function, since
  //  just declaring a fresh dataForController_t tends
  //  to get you one filled with junk from other, random
  //  values that were in those memory locations before
  dataForController_t controllerData = getBlankDataForController();
  // Since our buttons are all held high and
  //  pulled low when pressed, we use the "!"
  //  operator to invert the readings from the pins
  
  controllerData.triangleOn = button1;
  controllerData.circleOn = button2;
  controllerData.squareOn = button3;
  controllerData.crossOn = button4;
  controllerData.dpadUpOn = button5;
  controllerData.dpadDownOn = button6;
  
  // Set the analog sticks
  //  Since analogRead(pin) returns a 10 bit value,
  //  we need to perform a bit shift operation to
  //  lose the 2 least significant bits and get an
  //  8 bit number that we can use  
 
  controllerData.leftStickX = getSteering() >> 2;
  controllerData.rightStickX = getPedal1() >> 2;
  controllerData.rightStickY = getPedal2() >> 2;

  //int sensorValue = analogRead(A1);
  //Serial.println(digitalRead(40));
  // And return the data!

  if (button1 && button2 && button3 && button4 && button5 && button6){
      isCalibrationMode = true;
      steeringMin = 512;
      steeringMax = 512;
      pedal1Min = 512;
      pedal1Max = 512;
      pedal2Min = 512;
      pedal2Max = 512;
  }
  
  return controllerData;
}


dataForController_t getCalibrationModeData(){
    int button1 = !digitalRead(22);
    int button2 = !digitalRead(24);
    int button3 = !digitalRead(26);
    int button4 = !digitalRead(28);
    int button5 = !digitalRead(30);
    int button6 = !digitalRead(32);
  
    dataForController_t controllerData = getBlankDataForController();

    controllerData.triangleOn = 1;
    controllerData.circleOn = 1;
    controllerData.squareOn = 1;
    controllerData.crossOn = 1;
    controllerData.dpadUpOn = 1;
    controllerData.dpadDownOn = 1;

    controllerData.leftStickX = getSteering() >> 2;
    controllerData.rightStickX = getPedal1() >> 2;
    controllerData.rightStickY = getPedal2() >> 2;

    if (button1 && button2 && button3 && button4 && button5 && !button6){
        isCalibrationMode = false;
    }

    return controllerData;
}

void handleCalibration(){
    int steeringData = analogRead(A8);
    
    if (steeringData < steeringMin ) steeringMin = steeringData;
    if (steeringData > steeringMax ) steeringMax = steeringData;


    int pedal1Data = analogRead(A10);

    if(pedal1Data < pedal1Min) pedal1Min = pedal1Data;
    if(pedal1Data > pedal1Max) pedal1Max = pedal1Data;


    int pedal2Data = analogRead(A11);

    if(pedal2Data < pedal2Min) pedal2Min = pedal2Data;
    if(pedal2Data > pedal2Max) pedal2Max = pedal2Data;
}


int getSteering(){
   float steering = analogRead(A8);
   steering = min(max(steering, steeringMin), steeringMax);
   steering = ((steering - (steeringMin-5)) / ((steeringMax+5) - (steeringMin-5))) * 1024;
   int steeringData = (int)steering;
   return steeringData; 
}

int getPedal1(){
   float pedal = analogRead(A10);
   pedal = min(max(pedal, pedal1Min), pedal1Max);
   pedal = ((pedal - (pedal1Min-5)) / ((pedal1Max+5) - (pedal1Min-5))) * 1024;
   int pedalData = (int)pedal;
   return pedalData; 
}

int getPedal2(){
   float pedal = analogRead(A11);
   pedal = min(max(pedal, pedal2Min), pedal2Max);
   pedal = ((pedal - (pedal2Min-5)) / ((pedal2Max+5) - (pedal2Min-5))) * 1024;
   int pedalData = (int)pedal;
   return pedalData; 
}



