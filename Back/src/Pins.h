#ifndef PINS_H
#define PINS_H

// Servo Pin
#define SERVO 15

// Ultrasonic Sensor Pins
#define ECHO1 2
#define TRIG1 4
#define ECHO2 19
#define TRIG2 18
#define ECHO3 16
#define TRIG3 17
#define ECHO4 26
#define TRIG4 27

// I2C Pins
#define SDA 33
#define SCL 32

//time
#define SDA_time 21
#define SCL_time 22

// HX711 Pins
#define SCK 14
#define DT 12

// Arrays for Ultrasonic Sensor Pins
const int trigPins[] = {TRIG1, TRIG2, TRIG3, TRIG4};
const int echoPins[] = {ECHO1, ECHO2, ECHO3, ECHO4};

#endif // PINS_H
