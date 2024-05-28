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
unsigned int cycle_duration = 9; // seconds (A cycle duration is the duration between each void)
unsigned int test_duration = 60; // seconds (Test duration is the entire duration of the experiment)
String current_time;
String log_filename;
Time t;
bool PUMP_STATUS = false;
bool LED_STATUS = false;

#define LED_PIN 7
#define RELAY_PIN 9
#define SD_CS_PIN 4
#define PHOTORESISTOR_PIN A0
#define type YFS201
#define FLOWSENSOR_PIN 2 // pin -> interrupt pin 

FlowSensor Sensor(type, FLOWSENSOR_PIN);

void setup()
{
  // Setup pin modes
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(PHOTORESISTOR_PIN, INPUT);

  // Setup Serial connection
  Serial.begin(9600);

  // Initialize SD card
  SD.begin(4);
  while (!Serial) {
    ; // wait for the serial port to connect. Needed for native USB port only
  }
  Serial.print("Initializing SD card...");
  while (!SD.begin(4)) {
    Serial.println("initialization failed!");
  }
  Serial.println("initialization done.");

  // Initialize the real-time clock object
  rtc.begin();

  // Initialize flowmeter counter
  Sensor.begin(count);

  // Generate log file
  log_filename = "LOG.txt";
  File dataFile = SD.open(log_filename.c_str(), FILE_WRITE);

  // Wait for the setup delay period to end and then proceed to running
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
  Serial.println("File Created");

  while(delta_time(start_time) < test_duration){
    // While the test is ongoing, run a void simulation at each cycle duration
    temp_time = rtc.getUnixTime(rtc.getTime());
    unsigned long end_time = temp_time + void_duration;
    while(delta_time(temp_time) < cycle_duration){
      // Log data for bookkeeping
      log_data(log_filename, start_time, LED_STATUS, analogRead(PHOTORESISTOR_PIN), PUMP_STATUS, read_flowmeter());
      // Run void simulation
      activate(void_duration, log_filename, start_time);
      delay(1000);
      log_data(log_filename, start_time, LED_STATUS, analogRead(PHOTORESISTOR_PIN), PUMP_STATUS, read_flowmeter());
      temp_time = rtc.getUnixTime(rtc.getTime());
      // Calculate remaining time until the next cycle
      unsigned long next_cycle_time = start_time + ((delta_time(temp_time) / cycle_duration) + 1) * cycle_duration;
      unsigned long remaining_time = next_cycle_time - rtc.getUnixTime(rtc.getTime());
      // Wait for the remaining time using the RTC
      while (delta_time(rtc.getUnixTime(rtc.getTime())) < remaining_time) {
        delay(1000);
        log_data(log_filename, start_time, LED_STATUS, analogRead(PHOTORESISTOR_PIN), PUMP_STATUS, read_flowmeter());
      }
      temp_time = rtc.getUnixTime(rtc.getTime());
    }
  }
  // Mark the conclusion of the test in the SD card data log
  dataFile = SD.open(log_filename.c_str(), FILE_WRITE);
  dataFile.print("-");
  dataFile.print("\t");
  dataFile.print("-");
  dataFile.print("\t");
  dataFile.print("-");
  dataFile.print("\t");
  dataFile.print("-");
  dataFile.print("\t");
  dataFile.println("-");
  dataFile.println("TEST COMPLETED");
  dataFile.print("END TIME: ");
  dataFile.println(rtc.getTimeStr());
  dataFile.close();
  Serial.println("TEST COMPLETED");
  while(1){}
  delay(100);
}

void loop()
{
  
}

void activate(unsigned int void_duration, String log_filename, unsigned long start_time)
{
  // Turn on the LED
  digitalWrite(LED_PIN, HIGH);
  temp_time = rtc.getUnixTime(rtc.getTime());
  LED_STATUS = true;
  log_data(log_filename, start_time, LED_STATUS, analogRead(PHOTORESISTOR_PIN), PUMP_STATUS, read_flowmeter());

  // Keep the LED turned on until the void duration has passed
  unsigned long end_time = temp_time + void_duration;
  while (rtc.getUnixTime(rtc.getTime()) < end_time) {
    // Activate the water pump
    if (!PUMP_STATUS) {
      digitalWrite(RELAY_PIN, HIGH);
    }
    PUMP_STATUS = true;
    log_data(log_filename, start_time, LED_STATUS, analogRead(PHOTORESISTOR_PIN), PUMP_STATUS, read_flowmeter());
    delay(1000); // Delay for 1 second
  }

  // Turn off the LED after the void duration has passed
  digitalWrite(LED_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);
  LED_STATUS = false;
  PUMP_STATUS = false;
  log_data(log_filename, start_time, LED_STATUS, analogRead(PHOTORESISTOR_PIN), PUMP_STATUS, read_flowmeter());
}

void log_data(String log_filename, unsigned long start_time, bool LED_STATUS, int PHOTORESISTOR_READING, bool PUMP_STATUS, double FLOWRATE)
{
  // Record measurements to the SD card data log
  File dataFile = SD.open("LOG.txt", FILE_WRITE);
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

float read_flowmeter(void){
  Sensor.read();
  float flow_rate = Sensor.getFlowRate_m();
  if (isnan(flow_rate)){
    return 0.0;
  }
  return flow_rate;
}

long delta_time(long initial_time){
  return rtc.getUnixTime(rtc.getTime()) - start_time;
}
