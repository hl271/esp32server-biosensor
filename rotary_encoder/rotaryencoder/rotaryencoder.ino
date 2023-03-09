//#include <SoftwareSerial.h>
 int counter = 0; 
 int aState;
 int aLastState;  
 int angle = 0;
// SoftwareSerial espSerial(5, 6);
void setup() {
  Serial.begin (9600);
//  espSerial.begin(115200);
 
  pinMode(2, INPUT);           // set pin to input
  pinMode(3, INPUT);           // set pin to input
  
  digitalWrite(2, HIGH);       // turn on pullup resistors
  digitalWrite(3, HIGH);       // turn on pullup resistors
 
 
  //Setting up interrupt
  //A rising pulse from encodenren activated ai0(). AttachInterrupt 0 is DigitalPin nr 2 on moust Arduino.
  attachInterrupt(0, ai0, RISING);
  
  //B rising pulse from encodenren activated ai1(). AttachInterrupt 1 is DigitalPin nr 3 on moust Arduino.
  attachInterrupt(1, ai1, RISING);
  
}

 void loop() { 
  
   aState = digitalRead(2); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a Pulse has occured
   if (aState != aLastState){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise
     if (digitalRead(3) != aState) { 
       counter ++;
     } else {
       counter --;
     }
//     Serial.print("Position: ");
//     Serial.println(counter);
//     Serial.print("Angle: ");
     angle = counter % 360;
     
     Serial.println(angle);
   }
     
//   espSerial.println(angle);
   aLastState = aState; // Updates the previous state of the outputA with the current state
 }
 void ai0() {
  // ai0 is activated if DigitalPin nr 2 is going from LOW to HIGH
  // Check pin 3 to determine the direction
  if(digitalRead(3)==LOW) {
  counter++;
  }else{
  counter--;
  }
  }
 void ai1() {
  // ai0 is activated if DigitalPin nr 3 is going from LOW to HIGH
  // Check with pin 2 to determine the direction
  if(digitalRead(2)==LOW) {
  counter--;
  }else{
  counter++;
  }
  }
