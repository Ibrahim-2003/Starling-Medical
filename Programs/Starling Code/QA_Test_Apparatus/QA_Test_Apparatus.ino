// Ibrahim Al-Akash
// Starling Medical
// QA Testing Apparatus V1
// 2-16-24

// Include the RTC Library:
#include <DS3231.h>

// Include the SD library:
#include <SPI.h>
#include <SD.h>

// Include the Flow Sensor Library
#include <Arduino.h>
#include <FlowSensor.h>

// Pin Layout
/*
SD CARD
--------
4: CS
11: MOSI
13: CLK
12: MISO

2: Flowmeter PIN
7: LED PIN
9: RELAY PIN
A0: Photoresistor PIN
A5: RTC SCL
A4: RTC SDA
*/

// Initiate the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);

unsigned long start_time;
unsigned long temp_time;
unsigned int setup_time = 10; // seconds (Duration system allows for tester to complete setup before running experiment)
unsigned int void_duration = 1; // seconds (Duration of urination simulation)
unsigned int cycle_duration = 10; // seconds (A cycle duration is the duration between each void)
unsigned int test_duration = 60; // seconds (Test duration is the entire duration of the experiment)
unsigned int cycle_count; // calculated later number of cycles to run during test
String current_time;
String log_filename;

#define LED_PIN_1 7
#define LED_PIN_2 5
#define LED_PIN_3 4
#define RELAY_PIN 9
#define SD_CS_PIN 4
#define PHOTORESISTOR_PIN A0
#define type YFS201
#define FLOWSENSOR_PIN 2 // pin -> interrupt pin 

FlowSensor Sensor(type, FLOWSENSOR_PIN);

void setup()
{
  // Setup pin modes
  pinMode(LED_PIN_1, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(PHOTORESISTOR_PIN, INPUT);

  // Setup Serial connection
  Serial.begin(9600);

  // Initialize SD card
  SD.begin(4);

  // Initialize the real-time clock object
  rtc.begin();

  // Initialize flowmeter counter
  Sensor.begin(count);

  // Generate log file
  log_filename = rtc.getDateStr();
  log_filename += rtc.getTimeStr();
  log_filename += ".csv";
  File dataFile = SD.open(log_filename.c_str(), FILE_WRITE);

  // Wait for setup delay period to end and then proceed to running
  while(delta_time(start_time) < setup_time){}

  // Record the start time
  start_time = rtc.getUnixTime(rtc.getTime());
  Serial.print("Start Time: ");
  Serial.println(rtc.getTimeStr());
  dataFile.print("Start Time: ");
  dataFile.println(rtc.getTimeStr());

  // Initialize CSV header names
  dataFile.println("Time (s)\tLED Status (On/Off)\tPhotoresistor (V)\tPump Status (On/Off)\tFlow Rate (L/min)");
  dataFile.close();

  // Run test
  while(delta_time(start_time) < test_duration){
    // While the test is ongoing, run a void simulation at each cycle duration
    cycle_count = test_duration / cycle_duration;
    for(int i = 0; i < cycle_count; i++){
      // Activate LED and pump and log the data
      log_data(log_filename, start_time, false, read_photoresistor(), false, read_flowmeter());
      activate(void_duration, log_filename, start_time);
      temp_time = rtc.getUnixTime(rtc.getTime());
      
      // Wait for the cycle duration time to pass before moving onto the next cycle
      while(delta_time(temp_time) < cycle_duration){
        log_data(log_filename, start_time, false, read_photoresistor(), false, read_flowmeter());
        delay(1000);
      }
    }
  }
  // Mark conclusion of test in SD card data log
  dataFile = SD.open(log_filename.c_str(), FILE_WRITE);
  dataFile.print("-\t-\t-\t-\t-\nTEST COMPLETED\nEND TIME: ");
  dataFile.println(rtc.getTimeStr());
  dataFile.close();
  while(1){}
}



void loop()
{
  
}

void activate(unsigned int void_duration, String log_filename, unsigned long start_time)
{
  // Turn on LED
  digitalWrite(LED_PIN_1, HIGH);
  temp_time = rtc.getUnixTime(rtc.getTime());
  log_data(log_filename, start_time, true, read_photoresistor(), false, read_flowmeter());
  digitalWrite(RELAY_PIN, HIGH);

  // Keep LED turned on until the void duration has passed
  while(delta_time(temp_time) < void_duration){
    log_data(log_filename, start_time, true, read_photoresistor(), true, read_flowmeter());
    delay(1000);
  }

  // Turn off LED after void duration has passed
  digitalWrite(LED_PIN_1, LOW);
}

void log_data(String log_filename, unsigned long start_time, bool LED_STATUS, int PHOTORESISTOR_READING, bool PUMP_STATUS, double FLOWRATE)
{
  // Record measurements to SD card data log
  File dataFile = SD.open(log_filename.c_str(), FILE_WRITE);
  dataFile.print(delta_time(start_time));
  dataFile.print("\t");
  dataFile.print(LED_STATUS);
  dataFile.print("\t");
  dataFile.print(PHOTORESISTOR_READING);
  dataFile.print("\t");
  dataFile.print(PUMP_STATUS);
  dataFile.print("\t");
  dataFile.println(FLOWRATE);
  dataFile.close();
}

void count()
{
	Sensor.count();
}

int read_photoresistor(void){
  return analogRead(PHOTORESISTOR_PIN);
}

float read_flowmeter(void){
  Sensor.read();
  float flow_rate = Sensor.getFlowRate_m();
  if(isnan(flow_rate)){
    return 0.0;
  }
  return Sensor.getFlowRate_m();
}

long delta_time(long initial_time){
  return rtc.getUnixTime(rtc.getTime()) - start_time;
}
