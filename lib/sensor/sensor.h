#pragma once

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AS7341.h>
#include <stdint.h>

typedef struct sensor_sample {
    char     mac_address[18];
    uint8_t  sensor_id;
    uint64_t timestamp_ms;
    uint16_t f1, f2, f3, f4, f5, f6, f7, f8;
    uint16_t nir;
    uint16_t clear;
    uint16_t atime;
    uint16_t astep;
    uint16_t again;
    uint16_t int_time;
} sensor_sample;

class Sensor {
public:

    Sensor(uint8_t sensor_id, TwoWire* wire, uint8_t sda, uint8_t scl);
    bool begin();
    bool read(sensor_sample& sample);

private:
    uint8_t _sensor_id;
    uint8_t _sda, _scl;
    TwoWire* _wire;
    Adafruit_AS7341 _sensor;

    uint16_t _atime;
    uint16_t _astep;
    uint16_t _again;
};

extern Sensor sensor;