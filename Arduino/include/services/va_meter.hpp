#pragma once

#include <Wire.h>
#include "../lib/ina219/INA219.h"
#include "serviceInterface.hpp"

struct INA219Uuid {
    const char* shuntvoltageUuid;
    const char* busvoltageUuid;
    const char* current_mAUuid;
    const char* power_mWUuid;
};

class VAMeter final : public ServiceInterface {
    // Attributes
    BLEFloatCharacteristic shuntvoltageChar;
    BLEFloatCharacteristic busvoltageChar;
    BLEFloatCharacteristic current_mAChar;
    BLEFloatCharacteristic power_mWChar;

    INA219 sensor;

public:
    VAMeter(uint8_t addr, const INA219Uuid& uuid) : shuntvoltageChar(uuid.shuntvoltageUuid, BLERead | BLENotify),
    busvoltageChar(uuid.busvoltageUuid, BLERead | BLENotify),
    current_mAChar(uuid.current_mAUuid, BLERead | BLENotify),
    power_mWChar(uuid.power_mWUuid, BLERead | BLENotify),
    sensor(addr)
    { }
    ~VAMeter() override = default;

    void begin() override {
        this->sensor.begin();

        // set init value
        this->update();
    }

    void addCharacteristic(BLEService &service) override {
        service.addCharacteristic(shuntvoltageChar);
        service.addCharacteristic(busvoltageChar);
        service.addCharacteristic(current_mAChar);
        service.addCharacteristic(power_mWChar);
    }

    void update() override {
        this->shuntvoltageChar.writeValue(this->sensor.getShuntVoltage_mV());
        this->busvoltageChar.writeValue(this->sensor.getBusVoltage_V());
        this->current_mAChar.writeValue(this->sensor.getCurrent_mA());
        this->power_mWChar.writeValue(this->sensor.getPower_mW());
    }
};