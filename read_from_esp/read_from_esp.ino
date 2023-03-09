//#define RXp2 16
//#define TXp2 17
//void setup() {
//// Open serial communications and wait for port to open:
//Serial.begin(115200);
//Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
////while (!Serial) {
////}
////; // wait for serial port to connect. Needed for native USB port only
//}
//
//void loop() { // run over and over
////if (Serial.available()) {
////Serial.write(Serial.read());
//  Serial.println(Serial2.readString());
//}
// this sample code provided by www.programmingboss.com
#define RXp2 16
#define TXp2 17
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
}
void loop() {

    Serial.println(Serial2.readString());
}
