#include <Arduino.h>
#include <AccelStepper.h>
#define MotorInterfaceType 4

// put function declarations here:
AccelStepper z(MotorInterfaceType, 5, 7, 6, 8);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);
  z.setMaxSpeed(1000.0);
  z.setAcceleration(50.0);
  z.setSpeed(200);
}

void loop() {
  // put your main code here, to run repeatedly:
  z.moveTo(2038);
  z.run();
}
