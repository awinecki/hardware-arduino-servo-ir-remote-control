#include <Arduino.h>
#include <Servo.h>
#include <IRremote.h>

// Define pins
int IR_RECV_PIN = 6;
int SERVO_DATA_PIN = 9;

// Init servo & IR
Servo servo;
IRrecv irrecv(IR_RECV_PIN);
decode_results ir_results;

int servo_position = 90;
int servo_max = 170;
int servo_min = 5;
int micro_step = 12;
int loop_delay = 15;
boolean direction_up = true;

void setup() {
  // Serial.begin(9600);
  servo.attach(SERVO_DATA_PIN);
  irrecv.enableIRIn();
}

void loop() {

  if (irrecv.decode(&ir_results)) {

    // Print out IR received HEX code
    // Serial.println(ir_results.value, HEX);

    // "ON" pressed
    if (ir_results.value == 0xFF02FD) {
      direction_up = true;
      servo_position += micro_step;
    }

    // "OFF" pressed
    if (ir_results.value == 0xFF9867) {
      direction_up = false;
      servo_position -= micro_step;
    }

    // Holding down any button (ON / OFF is always fired first, then this
    // if still holding the button down
    if (ir_results.value == 0xFFFFFFFF) {
      if (direction_up) {
        servo_position += micro_step;
      } else {
        servo_position -= micro_step;
      }
    }

    irrecv.resume(); // Receive the next value
  }

  if (servo_position > servo_max) {
    servo_position = servo_max;
  } else if (servo_position < servo_min) {
    servo_position = servo_min;
  }

  servo.write(servo_position);
  delay(loop_delay);
}

