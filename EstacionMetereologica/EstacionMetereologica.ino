#include <Wire.h>
#include <Adafruit_BMP280.h>
#include <ArduinoJson.h>
#include <time.h>

#include <SPI.h>
// #include <SD.h>




// const int chipSelect = 4;





Adafruit_BMP280 bmp;    // usa la interface I2C
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

void setup() {
  Serial.begin(9600);
  // Serial.println(F("BMP280 Sensor And uSD card event test"));
  Serial.println(F("BMP280 Sensor event test"));

  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1) delay(10);
  }






  // if (!SD.begin(chipSelect)) {
  //   Serial.println("Card failed, or not present");
  //   // don't do anything more:
  //   while (1);
  // }







  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  // bmp_temp->printSensorDetails();

  // Serial.println("BMP280 and uSD card initialized.");
  Serial.println("BMP280 inicializado.\n\n");
  // struct tm preFormatedNow = {};
  //   preFormatedNow.tm_year = 2024 - 1900; // Años desde 1900
  //   preFormatedNow.tm_mon = 9;             // Mes (9 = octubre, ya que enero es 0)
  //   preFormatedNow.tm_mday = 2;            // Día del mes
  //   preFormatedNow.tm_hour = 0;            // Hora
  //   preFormatedNow.tm_min = 0;             // Minuto
  //   preFormatedNow.tm_sec = 0; 
  // time_t now = mktime(&preFormatedNow);
}

void loop() {

  // Aqui cada que entra el ciclo se puede incrementar una hora simulada con el codigo comentado de arriba

  JsonDocument doc;
  time_t now = time(NULL);
  tm timeinfo = *gmtime(&now);
  char buf[32];

  float tempReaded = 0;
  float pressReaded = 0;
  int contador = 0;   // Aqui definimos el numero de mediciones por iteracion
  String header = " === Mediciones Obtenidas ===";
  sensors_event_t temp_event, pressure_event;

  // Measurments will take 1 second and will be 5 times because of the file lenght
  while (contador < 5){

    bmp_temp->getEvent(&temp_event);
    bmp_pressure->getEvent(&pressure_event);

    Serial.println(header);

    Serial.print("Temperatura = ");
    
    tempReaded = temp_event.temperature;
    // doc["TemperaturaMed"] = tempReaded;
    doc["TemperaturaMed"]["Magnitud"] = tempReaded;
    doc["TemperaturaMed"]["Unidades"] = "C";
    strftime(buf, sizeof(buf), "%FT%TZ", &timeinfo);
    doc["TemperaturaMed"]["FechaHora"] = buf;

    Serial.print(tempReaded);
    Serial.println(" *C");

    Serial.print("Presion = ");
    
    pressReaded = pressure_event.pressure;

    // doc["PresionMed"] = pressReaded;
    doc["PresionMed"]["Magnitud"] = pressReaded;
    doc["PresionMed"]["Unidades"] = "hPa";
    // char buf[32];
    strftime(buf, sizeof(buf), "%FT%TZ", &timeinfo);
    doc["PresionMed"]["FechaHora"] = buf;

    Serial.print(pressReaded);
    Serial.println(" hPa");

    Serial.println(" ============================ \n");

    serializeJson(doc, Serial);
    Serial.println("\n");







    // File dataFile = SD.open("datalog.txt", FILE_WRITE);

    // // if the file is available, write to it:
    // if (dataFile) {

    //   dataFile.println(header);
    //   dataFile.print("Medicion numero: ");
    //   dataFile.println(contador+1);
    //   dataFile.print("Temperatura = ");
    //   dataFile.print(tempReaded);
    //   dataFile.println(" *C");

    //   dataFile.print("Presion = ");
    //   dataFile.print(pressReaded);
    //   dataFile.println(" hPa");

    //   dataFile.println(" ============================ \n");
    //   dataFile.close();

    //   Serial.print("Medicion ");
    //   Serial.print(contador+1);
    //   Serial.println(" lograda.");

    // }

    // // if the file isn't open, pop up an error:
    // else {
    //   Serial.println("error opening datalog.txt");
    // }










    delay(1000);  // Al sensor le tomara un segundo medir una vez, 5s medir 5 veces
    contador ++;
  }

    // Serial.println("Esperando una hora");
    // delay(3600000);  // una hora
    Serial.println("5 mediciones hechas, esperando 10s\n\n");
    delay(10000);  // 10 segundos

}
