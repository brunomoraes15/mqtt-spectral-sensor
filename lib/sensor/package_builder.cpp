#include "package_builder.h"

String Package::build_packgage(const sensor_sample& sample){
    String csv_pkg;
    csv_pkg += String(sample.timestamp_ms) + ",";
    csv_pkg += String(sample.sensor_id) + ",";
    csv_pkg += String(sample.mac_address) + ",";
    csv_pkg += String(sample.again) + ",";
    csv_pkg += String(sample.astep) + ",";
    csv_pkg += String(sample.int_time) + ",";
    csv_pkg += String(sample.f1) + ",";
    csv_pkg += String(sample.f2) + ",";
    csv_pkg += String(sample.f3) + ",";
    csv_pkg += String(sample.f4) + ",";
    csv_pkg += String(sample.f5) + ",";
    csv_pkg += String(sample.f6) + ",";
    csv_pkg += String(sample.f7) + ",";
    csv_pkg += String(sample.f8) + ",";
    csv_pkg += String(sample.clear) + ",";
    csv_pkg += String(sample.nir) + ",";
}