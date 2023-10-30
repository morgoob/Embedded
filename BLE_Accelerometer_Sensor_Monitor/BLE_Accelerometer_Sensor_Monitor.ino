#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL343.h>
#include <DFRobot_RGBLCD1602.h>
#include <ArduinoBLE.h>

#define ADXL343_SCK D13
#define ADXL343_MISO D12
#define ADXL343_MOSI D11
#define ADXL343_CS D10
#define IRAS2 D6 // LEFT
#define IRAS1 D5 // FRONT
#define IRAS0 D4 // RIGHT
#define REDLED D3

/* For SPI */
Adafruit_ADXL343 accel = Adafruit_ADXL343(ADXL343_SCK, ADXL343_MISO, ADXL343_MOSI, ADXL343_CS, 12345);

/* RGB LCD Screen*/
DFRobot_RGBLCD1602 lcd(/*lcdCols*/16,/*lcdRows*/2);  //16 characters and 2 lines of show

/* Buffer for printing to LCD */
char buffer[32];

/* Constants to determine if the accelerometer is moving */
const float X = 0.04;
const float Y = 0.04;
const float Z = 9.49;
const float VARY_FACTOR = 0.5;

/* BLE */
const char* SERVICE_UUID = "3096261e-325a-42ff-b1a2-0401d482e2f9";
const char* COORD_UUID = "45b976c8-b6fb-4c21-90db-22ab4abd5b5c";
const char* MOVEMENT_UUID = "afc5d7f8-ae89-4ed6-b18c-a79e5a835ff2";
const char* OBSTACLE_UUID = "b2f99546-23be-4168-815d-87ae87ec00b7";

BLEService accelerometerService(SERVICE_UUID);
BLEStringCharacteristic coordCharacteristic(COORD_UUID, BLENotify, 32);
BLEStringCharacteristic movementCharacteristic(MOVEMENT_UUID, BLENotify, 32);
BLEStringCharacteristic obstacleCharacteristic(OBSTACLE_UUID, BLENotify, 32);

char* IRAS_state(bool bit){
  if(!bit) return((char*) "yes");
  return((char*) "no");
}

bool moving(float coordinate, float expected){
  if((expected + VARY_FACTOR <= coordinate) || (expected - VARY_FACTOR >= coordinate)) return true;
  return false;
}

void _print(float x, float y, float z){
  lcd.setCursor(0, 0);
  sprintf(buffer, "%.1f %.1f %.1f", x, y, z);
  lcd.print(buffer);
  lcd.setCursor(0, 1); //second row 
  if(moving(x, X) || moving(y, Y) || moving(z, Z)) {
    sprintf(buffer, "%-16s", "Moving: Yes");
  } else {
    sprintf(buffer, "%-16s", "Moving: No");
  }
  lcd.print(buffer);
}

void setup() {
  lcd.init();
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Accelerometer Test"); Serial.println("");

  /* Pin Initialization */
  pinMode(REDLED, OUTPUT);
  pinMode(IRAS0, INPUT);
  pinMode(IRAS1, INPUT);
  pinMode(IRAS2, INPUT);
  digitalWrite(REDLED, HIGH); // active low LED
  
  /* Initialise the sensor */
  if(!accel.begin())
  {
    /* There was a problem detecting the ADXL343 ... check your connections */
    Serial.println("Ooops, no ADXL343 detected ... Check your wiring!");
    while(1);
  }
  
  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL343_RANGE_16_G);
  
  /* Display some basic information on this sensor */
  accel.printSensorDetails();

  /* BLE initialization */
  if (!BLE.begin()) {
    Serial.println("starting Bluetooth® Low Energy module failed!");
    while (1);
  }

  BLE.setLocalName("moclloyd Lab 5.4");
  BLE.setAdvertisedService(accelerometerService);
  accelerometerService.addCharacteristic(coordCharacteristic);
  accelerometerService.addCharacteristic(movementCharacteristic);
  accelerometerService.addCharacteristic(obstacleCharacteristic);
  BLE.addService(accelerometerService);
  BLE.advertise();
}

void loop() {
  digitalWrite(REDLED, (!digitalRead(IRAS2) || !digitalRead(IRAS1) || !digitalRead(IRAS0))); // turn LED on if any of the three IRAS senses something
  
  BLEDevice central = BLE.central();
  
  /* Get a new sensor event */
  sensors_event_t event;
  accel.getEvent(&event);

  float x = event.acceleration.x;
  float y = event.acceleration.y;
  float z = event.acceleration.z;

  sprintf(buffer, "(%.1f, %.1f, %.1f)", x, y, z);
  coordCharacteristic.setValue(buffer);
  
  if(moving(x, X) || moving(y, Y) || moving(z, Z)) {
    sprintf(buffer, "Moving: Yes");
  } else {
    sprintf(buffer, "Moving: No");
  }
  movementCharacteristic.setValue(buffer);
  
  sprintf(buffer, "L: %s, F: %s, R: %s", IRAS_state(digitalRead(IRAS2)), IRAS_state(digitalRead(IRAS1)), IRAS_state(digitalRead(IRAS0))); // LEFT FRONT RIGHT
  obstacleCharacteristic.setValue(buffer);
  
  _print(x, y, z);
  
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: "); Serial.print(x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(z); Serial.print("  ");Serial.println("m/s^2 ");

  delay(250);
}
