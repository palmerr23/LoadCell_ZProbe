/*
 Example using the SparkFun HX711 breakout board with a scale
 Modified from the SparkFun Electronics examples and modified Bodge HX711 libraries
 Date: Dec 2015 RP

 Arduino pin 
 5V -> VCC
 GND -> GND
 
 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.
*/

#include "HX711i.h"
void tare_int(void);

#define SERIAL_ON 1            // debug and calibration mode

#define CLK  6                 // HX711 clock (pin OK for MEGA or NANO)
#define DOUT  5                // HX711 data out (pin OK for MEGA or NANO)

#define TARE_PIN 3             // tare (ZERO THE READING)when this pin goes high
#define PROBE_OUT 4            // z-probe signal to RAMPS or other printer controller (pin OK for MEGA or NANO)
#define LED 13                 // LED to flash (pin OK for MEGA or NANO)

// use SERIAL_ON mode to find a good number for calibration_factor - aiming for around 500 units for a firm z-probe contact
long calibration_factor = 50L; // Any reasonable number for long int arithmetic 
#define PROBE_HVAL 1500        // something representing a firm contact 
#define HYSTERESIS 50          // stop chatter - increase if light flickers

HX711 scale(DOUT, CLK);        // use default gain of 128

void setup() {
#ifdef SERIAL_ON
  Serial.begin(38400);
  Serial.println("HX711 calibration sketch");
#endif  

  scale.set_scale(calibration_factor);    
  scale.get_units(3);             // some throw away some readings to stablise
  scale.tare(3);	                // Reset the normalised reading to 0 "at rest" (set OFFSET)i.e. TARE mode on scale

  pinMode(PROBE_OUT, OUTPUT);     // this is the output for Z-probe to RAMPS
  digitalWrite(PROBE_OUT, LOW);   // start with z-probe not in contact

  pinMode(LED, OUTPUT);          // indicator LED
  digitalWrite(LED, LOW); 

  pinMode(TARE_PIN, INPUT_PULLUP);        // signal from RAMPS to triger zero set
  
 attachInterrupt(digitalPinToInterrupt(TARE_PIN), tare_int, RISING);
 interrupts();
}

volatile boolean tare_now = false;

void tare_int(void){   // interrupt routine to trigger zero setting
  tare_now = true;
}

long reading, trigger_point, i = 0;
boolean probe_hit = false;
boolean led_val = LOW;

void loop() {
 
  if (tare_now) {
    #ifdef SERIAL_ON
       Serial.println("**************** Tare ****************");
    #endif 
     tare_now = false;
     scale.tare(3);                  // Reset the 'at rest' reading to 0 
  }

  reading = scale.get_units(1);
  
#ifdef SERIAL_ON
  Serial.print(i);
  Serial.print(", ");
  Serial.print(reading);
  Serial.print(", ");
  Serial.println(probe_hit);
  i++;
#endif  

// state variable machine - stay high or low until reading changes by more than HYSTERESIS

switch (probe_hit) {
    case false:
     if (reading >= PROBE_HVAL) 
          probe_hit = true;
          led_val = LOW;        //ON
          break;
          
    case true:
      if (reading < (PROBE_HVAL - HYSTERESIS)) 
          probe_hit = false;
          led_val = HIGH;         // OFF
          break;
    }
 digitalWrite(PROBE_OUT, probe_hit);    // set the output pin
 digitalWrite(LED, led_val);            // and the LED
  
}
