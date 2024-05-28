/*
 * PressureSensorTest
 *
 * Fetch and print values from a Honeywell 
 * TruStability HSC Pressure Sensor over SPI
 * 
 * The sensor values used in this demo are 
 * for a -15 to 15 psi gauge pressure sensor. 
 * 
 */

#include <HoneywellTruStabilitySPI.h>

#define SLAVE_SELECT_PIN 9
#define BUTTON 3
TruStabilityPressureSensor sensor( SLAVE_SELECT_PIN, -5.0, 5.0 );

float mass = 0.0f;
unsigned long t;

void setup() {
  Serial.begin(115200); // start Serial communication
  SPI.begin(); // start SPI communication
  sensor.begin(); // run sensor initialization
  pinMode(BUTTON, INPUT);
  t = millis();
}

void loop() {
//  readMass();
// the sensor returns 0 when new data is ready
  bool activated = false;
  if( sensor.readSensor() == 0 ) {
    Serial.print(millis() - t); Serial.print('\t');
    Serial.print( sensor.pressure(), 6); Serial.print('\t');
    Serial.println(mass, 4);
  }
  
  delay( 100 ); // Slow down sampling to 10 Hz. This is just a test.
  if(!activated){
    if(digitalRead(BUTTON)){
      while(digitalRead(BUTTON)){
        }
      activated = true;
      mass += 10.0f;
      return;
      }
    }else{
      if(!digitalRead(BUTTON)){
        activated = false;
        return;
        }
      }
}



void readMass(void){
  if( sensor.readSensor() == 0 ) {
    float p = sensor.pressure();
  mass = -2014.3882135158285*p + -119.00032521053421;
  Serial.print(p,6); Serial.print('\t');
  Serial.println(mass,4);
  }
  
  }
