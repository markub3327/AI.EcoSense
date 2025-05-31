#pragma once

#include <ArduinoBLE.h>

#include "../../lib/Task/src/periodicTask.hpp"
#include "../pins.hpp"


class Pump final : public PeriodicTask {
    BLEService *service;
    BLEByteCharacteristic pumpChar;
    uint8_t speed;

    void setPump(const int value) {
        this->speed = value;
        analogWrite(PUMP_PIN, this->speed);
    }

    void stopPump() {
        this->speed = 0;
        analogWrite(PUMP_PIN, this->speed);
    }

    void update() override {
    }

public:
    template<typename... Args>
    explicit Pump(Args&&... args) : PeriodicTask(std::forward<Args>(args)...), service(nullptr), pumpChar("19B11A01-F8F2-537E-4F6C-D104768A1215", BLEWrite), speed(0)
    { }

    void begin() override {
        if (this->service)
            this->service->addCharacteristic(pumpChar);

        pinMode(PUMP_PIN, OUTPUT);

        // Pump test
        this->setPump(200);
        delay(15000);
        this->stopPump();
    }

    void setService(BLEService &service) { this->service = &service; }
};