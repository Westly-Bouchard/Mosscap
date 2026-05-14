#include "arduino/ArduinoRuntime.h"

#include "arduino/Motor.h"

Motor lM(10);
Motor rM(11);

void setup() {
    pinMode(1, INPUT);
}

void loop() {
    if (digitalRead(1)) {
        lM.run(-150);
        rM.run(150);
    } else {
        lM.run(0);
        rM.run(0);
    }
}