#pragma once

#include "sensorInterface.hpp"

class SoilMoisture final : public SensorInterface {
    uint8_t pin;
    BLEUnsignedIntCharacteristic characteristic;

public:
    template<typename... Args>
    SoilMoisture(const uint8_t pin, Args&&... args) : pin(pin), characteristic(std::forward<Args>(args)..., BLERead | BLENotify)
    {  }
    ~SoilMoisture() override = default;

    void begin() override {
        // set init value
        this->update();
    }

    void addCharacteristic(BLEService &service) override {
        service.addCharacteristic(characteristic);
    }

    void update() override {
        // Read sensor
        this->characteristic.writeValue(analogRead(this->pin));
    }

    uint16_t getValue() {
        return this->characteristic.value();
    }
};