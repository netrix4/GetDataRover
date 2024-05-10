
#include <Wire.h>
// #include <SPI.h>
#include <Adafruit_BMP280.h>

#include <SPI.h>
#include <SD.h>
const int chipSelect = 4;


Adafruit_BMP280 bmp; // use I2C interface
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.println(F("BMP280 Sensor And uSD card event test"));

  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1) delay(10);
  }

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  bmp_temp->printSensorDetails();

  Serial.println("BMP280 and uSD card initialized.");
}

void loop() {
  float tempReaded = 0;
  float pressReaded = 0;
  String header = " === Mediciones Obtenidas ===";
  sensors_event_t temp_event, pressure_event;


  bmp_temp->getEvent(&temp_event);
  bmp_pressure->getEvent(&pressure_event);

  Serial.println(header);
  Serial.print(F("Temperature = "));
  
  tempReaded = temp_event.temperature;
  Serial.print(tempReaded);
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  
  pressReaded = pressure_event.pressure;
  Serial.print(pressReaded);
  Serial.println(" hPa");
  Serial.println(" ============================ \n");


  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // delay(150);
    // if the file is available, write to it:
  if (dataFile) {
    // bmp_temp->getSensor(&sensorBMPInfo);
    // Serial.print(sensorBMPInfo.type);

    dataFile.println(header);
    dataFile.print("Temperature = ");
    dataFile.print(tempReaded);
    dataFile.println(" *C");

    dataFile.print("Pressure = ");
    dataFile.print(pressReaded);
    dataFile.println(" hPa");

    dataFile.println(" ============================ \n");
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }

  Serial.println();
  delay(5500);
}
