#include <Starling_MLX90393.h>
#include <HX711.h>

// Ibrahim Al-Akash
// Starling Medical


#include <SPI.h>
#include <SD.h>

Starling_MLX90393 sensor = Starling_MLX90393();
#define MLX90393_CS 10
//#define SD_PIN 4

float elapsed_time, previous_time, mass;
unsigned long index;


const int HX711_dout = 3; //mcu > HX711 dout pin
const int HX711_sck = 2; //mcu > HX711 sck pin
HX711 scale;

void setup(void)
{
  Serial.begin(115200);

  // LOAD CELL
  scale.begin(HX711_dout, HX711_sck);  

  /* Wait for serial on USB platforms. */
  while (!Serial) {
      delay(10);
  }

  // MAGNETOMETER
//  Serial.println(F("Starting Adafruit MLX90393 Magnetic Elastomer Sensor"));
  if (! sensor.begin_I2C(0x0C)) {          // hardware I2C mode, can pass in address & alt Wire
    Serial.println(F("No sensor found ... check your wiring?"));
    while (1) { delay(10); }
  }

  sensor.setGain(MLX90393_GAIN_2_5X);
  sensor.enableTemperatureCompensation(true);
  sensor.enableZSeriesConnection(false);

  // Set resolution, per axis
  sensor.setResolution(MLX90393_X, MLX90393_RES_16);
  sensor.setResolution(MLX90393_Y, MLX90393_RES_16);
  sensor.setResolution(MLX90393_Z, MLX90393_RES_16);

  // Set oversampling
  sensor.setOversampling(MLX90393_OSR_2);

  // Set digital filtering
  sensor.setFilter(MLX90393_FILTER_0);

  delay(1000);

  Serial.print(F("UNITS: "));
  Serial.println(scale.get_units(10));

  Serial.println(F("\nEmpty the scale, press a key to continue"));
  while(!Serial.available());
  while(Serial.available()) Serial.read();

  scale.tare();
  Serial.print(F("UNITS: "));
  Serial.println(scale.get_units(10));


  Serial.println(F("\nPut 10 gram in the scale, press a key to continue"));
  delay(1000);
  while(!Serial.available());
  while(Serial.available()) Serial.read();

  scale.calibrate_scale(10, 5);
  Serial.print(F("UNITS: "));
  Serial.println(scale.get_units(10));

  Serial.println(F("\nScale is calibrated, press a key to continue"));
  // Serial.println(scale.get_scale());
  // Serial.println(scale.get_offset());
  while(!Serial.available());
  while(Serial.available()) Serial.read();

  index = 1;
  Serial.println(F("Sample\tTime\tX\tY\tZ\tMass"));
}

void loop(void) {
    index = auto_logger_mass(0.001f,index,scale);
}

unsigned long auto_logger_mass(float time_delay, unsigned long index, HX711 scale){
  elapsed_time = (millis() - previous_time)/1000;
  float x, y, z, mass;
  bool button_state;
//  mass = scale.get_units(10);

if (scale.is_ready()) mass = scale.get_units() ;
    

  // get X Y and Z data at once
  if (sensor.readData(&x, &y, &z)) {
      Serial.print(index); Serial.print('\t');
      Serial.print(elapsed_time, 4); Serial.print('\t');
      Serial.print(x, 4); Serial.print('\t');
      Serial.print(y, 4); Serial.print('\t');
      Serial.print(z, 4); Serial.print('\t');
      Serial.println(mass, 4);
      index = index + 1;
  } else {
      Serial.println("Unable to read XYZ data from the sensor.");
  }

  delay(time_delay*1000);
  if(index == (pow(2,32) - 1)){ // If the index reaches the maximum limit of the unsigned long (2^32 - 1) enter a while loop forever so user can stop the program
    while(true){}
    }
  return index;
}
