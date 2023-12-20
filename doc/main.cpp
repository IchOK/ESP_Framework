#include <Arduino.h>
#include <driver/gpio.h>

#define LED_COUNT 1
#define LED_PIN GPIO_NUM_18


void setup () {
  Serial.begin (115200);
//  Serial0.begin (115200);
//  Serial1.begin (115200);
  delay(5000);

  Serial.println ("Hello - Serial");
//  Serial0.println ("Hello - Serial0");
//  Serial1.println ("Hello - Serial1");
}

void loop () {
  Serial.print ("x");
//  Serial0.print ("0");
//  Serial1.print ("1");
  delay (1000);
}