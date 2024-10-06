#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <time.h>

#include <SPI.h>
#include <SD.h>

const int chipSelect = 4;

Adafruit_BMP280 bmp;    // usa la interface I2C
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

    /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  bmp_temp->printSensorDetails();

}

void loop() {
  // make a string for assembling the data to log:
  String dataString = "{\n";

  struct tm preFormatedNow = {};
  preFormatedNow.tm_year = 2024 - 1900; // Años desde 1900
  preFormatedNow.tm_mon = 9;             // Mes (9 = octubre, ya que enero es 0)
  preFormatedNow.tm_mday = 1;            // Día del mes
  preFormatedNow.tm_hour = 12;            // Hora
  preFormatedNow.tm_min = 30;             // Minuto
  preFormatedNow.tm_sec = 0; 
  time_t now = mktime(&preFormatedNow);

  // time_t now = time(NULL);
  tm timeinfo = *gmtime(&now);
  char buf[32];

  float tempReaded = 0;
  float pressReaded = 0;
  sensors_event_t temp_event, pressure_event;

  bmp_temp->getEvent(&temp_event);
  bmp_pressure->getEvent(&pressure_event);


  // read three sensors and append to the string:
  dataString += "   \"Temperature\": \"" + String(temp_event.temperature) + "\",\n";
  dataString += "   \"Pression\": \"" + String(pressure_event.pressure) + "\",\n";

  strftime(buf, sizeof(buf), "%FT%TZ", &timeinfo);
  String dateString = String(buf);
  dataString += "   \"DateTime\": \"";
  dataString += dateString;
  dataString += "\"\n";

  dataString += "},\n";

  Serial.println(dataString);

  // File dataFile = SD.open("otro.txt", FILE_WRITE);
  // File dataFile = SD.open("otro.txt", O_APPEND);
  File dataFile = SD.open("otro.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    // dataFile.println(String(dataString));
    dataFile.close();
    
    Serial.println();
    // print to the serial port too:
    Serial.println(dataString);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening otro.txt");
  }
  delay(10000);
}
