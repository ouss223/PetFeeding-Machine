#ifndef PINS_H
#define PINS_H

// Servo Pin
#define SERVO 15

// Ultrasonic Sensor Pins
#define ECHO1 2
#define TRIG1 4

#define ECHO3 16
#define TRIG3 17

// I2C Pins
#define SDA 33
#define SCL 32

// time
#define SDA_time 21
#define SCL_time 22

// HX711 Pins
#define SCK 14
#define DT 12

#define SCK1 18
#define DT1 19

#define SCK2 26
#define DT2 25

// Arrays for Ultrasonic Sensor Pins
const int trigPins[] = {TRIG1, TRIG3};
const int echoPins[] = {ECHO1, ECHO3};

#endif // PINS_H
