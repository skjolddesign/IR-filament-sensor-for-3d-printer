/*   - PROJECT:
 *   Arduino (Digispark) 3D printer filament sensor. Switch output 1, dependent on ir beam interruption on A1:
 *   https://github.com/skjolddesign/IR-filament-sensor-for-3d-printer
 *   https://www.tinkercad.com/things/2x7NXTsgyG6-ir-filament-sensor
 
 *   
 *   - DIGISPARK PINS: 
 *   https://hackster.imgix.net/uploads/attachments/779313/digispark-pinout_QeJ1n4FNv3.png?auto=compress%2Cformat&w=1280&h=960&fit=max 
 *   
 *   - IR OBSTACLE SENSOR TUTORIAL:
 *   https://create.arduino.cc/projecthub/muhammed-shameel-k-v/simple-obstacle-sensor-with-arduino-988ae3
 *
 *   - SENSOR
 *   https://www.ebay.com/itm/10PCS-TCRT5000L-TCRT5000-Reflective-Optical-Sensor-Infrared-IR-Switch-infrared/200942957928?hash=item2ec9223568:g:GiwAAOSw2~Jbkhyj
 
    
    - WIRING:
    IR LED Anode => R1 470 ohms resistor => 5v (update: R470 is to strong, use a litle higher value)
    IR Cathode   => GND (den store basen inni LED)
    PhotoDiode Anode   => GND
    PhotoDiode Cathode => Analog
    Analog => R2 3.3k ohms resistor => 5V (R2 er da Pullup resistor)
    Dette gir AD 38 i 1cm luft.
    Detektor leder til GND ved IR lys. Høyere R2 gir mer følsomhet.
*/

//Digispark serial with one pin only:
#include <TinyPinChange.h>
#include <SoftSerial.h>
#define DEBUG_TX_RX_PIN         4  // debug on pin 4 Digispark
SoftSerial mySoftSerial(DEBUG_TX_RX_PIN, DEBUG_TX_RX_PIN, false); //true allows to connect to a regular RS232 without RS232 line driver
 

const int analogInPin = A1;  // Floating Analog on  A1=P2 Digispark.
int sensorValue = 0;        
#define output 1 // Pin 1 has built in led on Digispark
unsigned int detection = 0; // used to delay filament EMPTY.

void setup() {
  //Serial.begin(9600);
  mySoftSerial.begin(9600); // After MyDbgSerial.begin(), the serial port is in rxMode by default
  mySoftSerial.txMode(); //Before sending a message, switch to txMode
  pinMode(output, OUTPUT);
  digitalWrite(output,LOW);
}

void loop() {
  // get val and print
  sensorValue = analogRead(analogInPin);
  mySoftSerial.print("sensor = ");
  mySoftSerial.println(sensorValue);
  
  // decide FILAMENT OK or EMPTY
  if(sensorValue>=35){ // (air=30, transparent PLA=40, yellow PLA=600)
      digitalWrite(output,HIGH); // HIGH = FILAMENT OK
      detection=0; //reset detection
  }
    else{
      detection ++;
  }

  // EMPTY detection delay: (20x100ms delay = 2000ms detection time)
    if(detection>=20){ 
    digitalWrite(output,LOW); // LOW = FILAMENT EMPTY
    detection=20; //hold value here so it wont roll over.
  }
  
  
  
  delay(100);
}
