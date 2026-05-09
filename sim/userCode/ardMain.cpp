#include "arduino/ArduinoRuntime.h"

#include "arduino/Motor.h"

Motor m(28);

void setup() {
    pinMode(1, INPUT);
}

void loop() {
    if (digitalRead(1)) {
        m.run(200);
    } else {
        m.run(0);
    }
}