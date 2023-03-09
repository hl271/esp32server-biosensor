// this sample code provided by www.programmingboss.com
#define RXp2 16
#define TXp2 17
String a;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXp2, TXp2);
}
void loop() {
//    Serial.println("Message Received: ");
//    while(Serial2.available()) {
      a = Serial2.readStringUntil('\n');
      Serial.print("a is: ");
      Serial.println(a);
//    }
    
}
