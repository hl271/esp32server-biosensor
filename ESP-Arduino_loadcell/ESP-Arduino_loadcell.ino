
#include <SoftwareSerial.h>

#include "HX711.h"
#define calibration_factor -7050.0 //This value is obtained using the SparkFun_HX711_Calibration sketch
#define LOADCELL_DOUT_PIN  8
#define LOADCELL_SCK_PIN  9
HX711 scale;

SoftwareSerial espSerial(5, 6);
float flt;
void setup(){
  Serial.begin(115200);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0

  espSerial.begin(115200);
  delay(2000);
}
void loop()
{
  Serial.print("Reading: ");
  Serial.print(scale.get_units(), 1); //scale.get_units() returns a float
  Serial.print(" kg"); //You can change this to kg but you'll need to refactor the calibration_factor
  Serial.println();
  flt = scale.get_units();
  espSerial.println(flt);
  delay(1000);
}
