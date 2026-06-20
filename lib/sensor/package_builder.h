#pragma once

#include <Arduino.h>
#include "sensor.h"

class Package {
    public:
    static String build_packgage(const sensor_sample& sample);
};