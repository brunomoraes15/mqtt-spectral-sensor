#include "sensor_manager.h"

static const SensorConfig SENSOR_TABLE[] = {
    {1, &Wire,  21, 22},
    {2, &Wire1, 18, 19},
};

static const uint8_t SENSOR_TABLE_SIZE = sizeof(SENSOR_TABLE) / sizeof(SENSOR_TABLE[0]);

SensorManager sensor_manager;

SensorManager::SensorManager() : _sensor_count(0) {
    for (uint8_t i = 0; i < MAX_SENSORS; i++) {
        _sensors[i] = nullptr;
        _available[i] = false;
    }
}

static void scan_bus(TwoWire* wire, const char* label) {
    Serial.print("[SensorManager] Scanning ");
    Serial.println(label);
    uint8_t found = 0;
    for (uint8_t addr = 0x08; addr <= 0x77; addr++) {
        wire->beginTransmission(addr);
        if (wire->endTransmission() == 0) {
            Serial.print("  -> device ACKed at 0x");
            Serial.println(addr, HEX);
            found++;
        }
    }
    if (found == 0) {
        Serial.println("  -> no devices found");
    }
}

void SensorManager::begin() {
    _sensor_count = SENSOR_TABLE_SIZE < MAX_SENSORS ? SENSOR_TABLE_SIZE : MAX_SENSORS;

    for (uint8_t i = 0; i < _sensor_count; i++) {
        const SensorConfig& cfg = SENSOR_TABLE[i];

        cfg.wire->begin(cfg.sda, cfg.scl);
        scan_bus(cfg.wire, cfg.sensor_id == 1 ? "Wire (sensor 1)" : "bus (sensor >1)");

        _sensors[i] = new Sensor(cfg.sensor_id, cfg.wire, cfg.sda, cfg.scl);

        if (_sensors[i]->begin()) {
            _available[i] = true;
            Serial.print("[SensorManager] Sensor ");
            Serial.print(cfg.sensor_id);
            Serial.println(" initialized");
        } else {
            _available[i] = false;
            Serial.print("[SensorManager] Sensor ");
            Serial.print(cfg.sensor_id);
            Serial.println(" failed to initialize - excluded from sweeps");
        }
    }
}

bool SensorManager::read_all(sensor_sample out[], uint8_t max_out, uint8_t& count) {
    count = 0;
    bool any_available = false;

    for (uint8_t i = 0; i < _sensor_count && count < max_out; i++) {
        if (!_available[i]) continue;
        any_available = true;

        sensor_sample sample;
        if (_sensors[i]->read(sample)) {
            out[count++] = sample;
        } else {
            Serial.print("[SensorManager] Read failed for sensor ");
            Serial.println(_sensors[i]->id());
        }
    }

    return any_available;
}

bool SensorManager::is_available(uint8_t index) const {
    if (index >= _sensor_count) return false;
    return _available[index];
}