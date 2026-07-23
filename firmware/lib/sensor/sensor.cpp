#include "sensor.h"
#include <WiFi.h>
#include "net_tools.h"

Sensor::Sensor(
    uint8_t sensor_id, 
    TwoWire* wire, 
    uint8_t sda, 
    uint8_t scl
) {
    _sensor_id = sensor_id; 
    _wire =  wire;
    _sda  =  sda;
    _scl  =  scl;
    _atime = 29;
    _astep = 599;
    _again = AS7341_GAIN_64X;
};

Sensor sensor(1, &Wire, 21, 22);

bool Sensor::begin(){
    _wire->begin(_sda, _scl);
    if (!_sensor.begin(0x39, _wire)) return false;

    _sensor.setATIME(_atime);
    _sensor.setASTEP(_astep);
    _sensor.setGain((as7341_gain_t)_again);

    return true;
};

bool Sensor::read(sensor_sample& sample){
    if (!_sensor.readAllChannels())  return false;
    sample.sensor_id = _sensor_id;
    sample.timestamp_ms = millis();
    strcpy(sample.mac_address, WiFi.macAddress().c_str());

    sample.f1 =   _sensor.getChannel(AS7341_CHANNEL_415nm_F1);
    sample.f2 =   _sensor.getChannel(AS7341_CHANNEL_445nm_F2);
    sample.f3 =   _sensor.getChannel(AS7341_CHANNEL_480nm_F3);
    sample.f4 =   _sensor.getChannel(AS7341_CHANNEL_515nm_F4);
    sample.f5 =   _sensor.getChannel(AS7341_CHANNEL_555nm_F5);
    sample.f6 =   _sensor.getChannel(AS7341_CHANNEL_590nm_F6);
    sample.f7 =   _sensor.getChannel(AS7341_CHANNEL_630nm_F7);
    sample.f8 =   _sensor.getChannel(AS7341_CHANNEL_680nm_F8);
    sample.clear= _sensor.getChannel(AS7341_CHANNEL_CLEAR);
    sample.nir =  _sensor.getChannel(AS7341_CHANNEL_NIR);

    sample.atime = _atime;
    sample.astep = _astep;
    sample.again = _again;

    sample.int_time =
        (_atime + 1) *
        (_astep + 1) *
        2.78;

    return true;

};