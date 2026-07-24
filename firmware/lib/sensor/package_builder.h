#pragma once

#include <Arduino.h>
#include "sensor.h"

class Payload {
    public:
    static String build_payload(const sensor_sample& sample);
};