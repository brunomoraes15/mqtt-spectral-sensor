#pragma once

#include <Arduino.h>
#include <Wire.h>
#include "sensor.h"

#define MAX_SENSORS 4

struct SensorConfig {
    uint8_t  sensor_id;
    TwoWire* wire;
    uint8_t  sda;
    uint8_t  scl;
};

class SensorManager {
public:
    SensorManager();

    void begin();

    bool read_all(sensor_sample out[], uint8_t max_out, uint8_t& count);

    uint8_t sensor_count() const { return _sensor_count; }
    bool is_available(uint8_t index) const;

private:
    Sensor*  _sensors[MAX_SENSORS];
    bool     _available[MAX_SENSORS];
    uint8_t  _sensor_count;
};

extern SensorManager sensor_manager;