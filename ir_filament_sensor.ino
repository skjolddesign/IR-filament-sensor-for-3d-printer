/*   - PROJECT:
 *   Arduino (Digispark) 3D printer filament sensor. Switch outputPin 1, dependent on ir beam interruption on A1:
 *   
 *   - IMAGE:
 *   https://github.com/skjolddesign/IR-filament-sensor-for-3d-printer/blob/master/IMG_0820b.JPG
 *   
 *   - CODE:
 *   https://github.com/skjolddesign/IR-filament-sensor-for-3d-printer
 *   
 *   - 3D Design
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
    IR LED Cathode   => GND (the big base inside LED)
    Detector Anode   => GND
    Detector Cathode => Analog
    Analog => R2 3.3k ohms resistor => 5V (R2 is Pullup resistor)
    Detector conducts to GND when sees IR light. Higher R2 gives higher sensitiviy.
    
*/

// Defines (you can change these):
#define DEBUG_TX_RX_PIN   4  // serial debug pin 4 Digispark (yes we use same pin for TX and RX)
#define analogInPin      A1  // Digispark A1/P2 is floating and good for use. (some pins may have resistors on them)
#define outputPin         1  // Digispark Pin 1 has built in led

// Trigger value is used to decide OK/EMPTY filament. Use serial debug pin to see sensor value.
// IMPORTAINT! If powered by 3.3V vs 5V, the IR LED power is lower, detector resistanse will rise, and sensorValue will rise.
// 5.0V: air=30, transparent PLA=40, yellow PLA=600
// 3.3V: air=47, transparent PLA=68, yellow PLA=700   
#define triggerValue     54  // (35 for 5V, 54 for 3.3V).


//Digispark serial with one pin only:
//#include <TinyPinChange.h> 
#include <SoftSerial.h>
SoftSerial mySoftSerial(DEBUG_TX_RX_PIN, DEBUG_TX_RX_PIN, false); //true allows to connect to a regular RS232 without RS232 line driver


int sensorValue = 0;        
unsigned int detections = 0; // used to delay filament EMPTY.

void setup() {
  mySoftSerial.begin(9600); // After mySoftSerial.begin(), the serial port is in rxMode by default
  mySoftSerial.txMode(); //Before sending a message, switch to txMode
  pinMode(outputPin, outputPin);
  digitalWrite(outputPin,LOW);
}

void loop() {
  // get val and print
  sensorValue = analogRead(analogInPin);
  mySoftSerial.print("sensor = ");
  mySoftSerial.println(sensorValue);
  
  // decide FILAMENT OK or EMPTY
  if(sensorValue>=triggerValue){ 
      digitalWrite(outputPin,HIGH); // HIGH = FILAMENT OK
      detections=0; //reset detection
  }
    else{
      detections ++;
  }

  // EMPTY detection delay: (10x100ms delay = 1000ms detection time)
    if(detections>=10){ 
    digitalWrite(outputPin,LOW); // LOW = FILAMENT EMPTY
    detections=20; //hold value here so it wont roll over.
  }
  
  
  
  delay(100);
}
