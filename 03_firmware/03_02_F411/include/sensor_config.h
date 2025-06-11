#ifndef SENSOR_CONFIG_H
#define SENSOR_CONFIG_H

#include <Arduino.h>

bool initSensorComm(SensorType sensorType);
void configSensor(SensorType sensorType);

#endif
