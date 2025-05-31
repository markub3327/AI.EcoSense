//
// Created by Martin Kubovčík on 31/05/2025.
//

#pragma once

#include <ArduinoBLE.h>

class SensorInterface {

public:
    virtual ~SensorInterface() = default;

    virtual void begin() = 0;

    virtual void addCharacteristic(BLEService &service) = 0;

    virtual void update() = 0;
};