// Basic demo for plotting accelerometer readings from Adafruit MSA301/311

#include <Wire.h>
#include <Adafruit_MSA301.h>
#include <Adafruit_Sensor.h>

//
// Comment/Uncomment as needed for specific MSA being used:
//
// Adafruit_MSA301 msa;
Adafruit_MSA311 msa;

void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MSA301/311 test!");

  // Try to initialize!
  if (! msa.begin()) {
    Serial.println("Failed to find MSA301/311 chip");
    while (1) { delay(10); }
  }
  Serial.println("MSA301/311 Found!");

}

void loop() {

  /* Get a new sensor event, normalized */
  sensors_event_t event;
  msa.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2), with commas in between */
  Serial.print(event.acceleration.x);
  Serial.print(", "); Serial.print(event.acceleration.y);
  Serial.print(", "); Serial.print(event.acceleration.z);
  Serial.println();

  delay(10);
}