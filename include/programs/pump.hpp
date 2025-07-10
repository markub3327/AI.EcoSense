#pragma once

#include <ArduinoBLE.h>

#include "../../lib/Task/src/periodicTask.hpp"
#include "../pins.hpp"
#include "delayedTask.hpp"

constexpr uint8_t WATERING_SPEEED = 98;

class Pump final : public PeriodicTask {

    class _watering_X_min final : public DelayedTask {
        Pump *pump;
        void update() override {
            this->pump->stopPump();
        }
    public:
        _watering_X_min(Pump *pump, const uint8_t minutes) : DelayedTask(minutes * 60000), pump(pump) {}
        void begin() override {
            this->pump->setPump(WATERING_SPEEED);
            this->start();
        }
    };

    void setPump(const int speed) {
        this->speed = speed;
        analogWrite(PUMP_PIN, map(speed, 0, 100, 0, 255));
    }

    void stopPump() {
        this->setPump(0);
    }

    void update() override {
        this->watering_10_min.run();

        // logging the state of the pump
        this->pumpSpeedChar.writeValue(this->speed);
    }

    BLEService *service;
    BLEByteCharacteristic pumpSpeedChar;
    _watering_X_min watering_5_min;
    _watering_X_min watering_10_min;
    uint8_t speed;   // in percentage (0% - stop, 100% full speed)

public:
    template<typename... Args>
    explicit Pump(Args&&... args) : PeriodicTask(std::forward<Args>(args)...),
        service(nullptr),
        pumpSpeedChar("19B11A01-F8F2-537E-4F6C-D104768A1215", BLERead | BLENotify),
        watering_5_min(this, 5),
        watering_10_min(this, 10)
    { }

    void begin() override {
        if (this->service)
            this->service->addCharacteristic(pumpSpeedChar);

        pinMode(PUMP_PIN, OUTPUT);

        // Pump programs init
        this->watering_10_min.begin();
    }

    void setService(BLEService &service) { this->service = &service; }
};