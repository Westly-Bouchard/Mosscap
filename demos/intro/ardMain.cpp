#include <mosscap/defaults.h>
#include <sim/TankSim.h>
#include <Arduino.h>
#include <Motor.h>

std::unique_ptr<SimulatorBase> simInit();

#define FORWARD_PIN 41
#define TURN_LEFT_PIN 42
#define TURN_RIGHT_PIN 43
#define BACKWARD_PIN 44

Motor left(11, 10, 12);
Motor right(21, 20, 22);

void run(int pwm) {
    left.run(pwm);
    right.run(pwm);
}

void turn(int pwm) {
    left.run(-pwm);
    right.run(pwm);
}

void setup() {
    pinMode(FORWARD_PIN, INPUT);
    pinMode(BACKWARD_PIN, INPUT);
    pinMode(TURN_LEFT_PIN, INPUT);
    pinMode(TURN_RIGHT_PIN, INPUT);
}

void loop() {
    if (digitalRead(FORWARD_PIN)) {
        run(200);
    } else if (digitalRead(BACKWARD_PIN)) {
        run(-200);
    } else if (digitalRead(TURN_LEFT_PIN)) {
        turn(200);
    } else if (digitalRead(TURN_RIGHT_PIN)) {
        turn(-200);
    } else {
        run(0);
    }
}

std::unique_ptr<SimulatorBase> simInit() {
    // Set rendering scale
    Renderer::setScale(2.0);

    // Configuration of the robot
    WMRConfig config = defaultRobot;
    config.showEncoderTelemetry = false;
    config.showTOFTelemetry = false;

    // Create robot
    auto robot = std::make_unique<TankSim>(config);
    robot->setPose(1, 1, 0);

    TOFConfig tofConfig{0, 0, 0};
    tofConfig.boundingBox = BoundingBox(3, 3);

    robot->registerTOF(tofConfig);

    robot->registerLeftMotor(defaultMotor, 10);
    robot->registerRightMotor(defaultMotor, 20);

    robot->registerLeftEncoder(1500, 20);
    robot->registerRightEncoder(1500, 21);

    ArduinoRuntime::getInstance().createButton("Forward", 41);
    ArduinoRuntime::getInstance().createButton("Backward", 44);
    ArduinoRuntime::getInstance().createButton("Left", 42);
    ArduinoRuntime::getInstance().createButton("Right", 43);

    return robot;
}