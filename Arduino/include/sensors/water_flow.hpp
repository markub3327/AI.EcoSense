//
// Created by Martin Kubovčík on 15/07/2025.
//

#pragma once

#include "sensorInterface.hpp"

class WaterFlow final : public SensorInterface {
    // wrapper function
    static void pulse(WaterFlow *ptr) {
        ptr->pulseCount++; // Increment pulse count safely from ISR
    }

    uint8_t pin;
    BLEFloatCharacteristic characteristic;
    volatile uint32_t pulseCount = 0;
public:
    template<typename... Args>
    WaterFlow(const uint8_t pin, Args&&... args) : pin(pin), characteristic(std::forward<Args>(args)..., BLERead | BLENotify)
    {  }
    ~WaterFlow() override = default;

    void begin() override {
        attachInterrupt(digitalPinToInterrupt(pin), pulse, RISING, this);

        // set init value
        this->update();
    }

    void addCharacteristic(BLEService &service) override {
        service.addCharacteristic(characteristic);
    }

    void update() override {
        noInterrupts(); // Disable interrupts to read safely
        auto count = pulseCount;
        this->pulseCount = 0;
        interrupts();

        // Read sensor
        this->characteristic.writeValue(static_cast<float>(count) / 5880.0f);
    }

    float getValue() {
        return this->characteristic.value();
    }
};