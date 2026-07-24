#include "package_builder.h"

String Payload::build_payload(const sensor_sample& sample){
    String csv_payload;
    csv_payload += String(sample.timestamp_ms) + ",";
    csv_payload += String(sample.sensor_id) + ",";
    csv_payload += String(sample.mac_address) + ",";
    csv_payload += String(sample.again) + ",";
    csv_payload += String(sample.astep) + ",";
    csv_payload += String(sample.int_time) + ",";
    csv_payload += String(sample.f1) + ",";
    csv_payload += String(sample.f2) + ",";
    csv_payload += String(sample.f3) + ",";
    csv_payload += String(sample.f4) + ",";
    csv_payload += String(sample.f5) + ",";
    csv_payload += String(sample.f6) + ",";
    csv_payload += String(sample.f7) + ",";
    csv_payload += String(sample.f8) + ",";
    csv_payload += String(sample.clear) + ",";
    csv_payload += String(sample.nir);

    return csv_payload;
}