#pragma once

#include <ArduinoBLE.h>

#include "../../lib/Task/src/periodicTask.hpp"
#include "../../lib/Task/src/delayedTask.hpp"
#include "../sensors/water_flow.hpp"
#include "../pins.hpp"

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

        this->waterFlow.update();
        // Serial.print("Water flow: ");
        // Serial.println(this->waterFlow.getValue());

        // logging the state of the pump
        this->pumpSpeedChar.writeValue(this->speed);
    }

    BLEByteCharacteristic pumpSpeedChar;
    _watering_X_min watering_5_min;
    _watering_X_min watering_10_min;
    uint8_t speed;   // in percentage (0% - stop, 100% full speed)
    WaterFlow waterFlow;


public:
    template<typename... Args>
    explicit Pump(Args&&... args) : PeriodicTask(std::forward<Args>(args)...),
        pumpSpeedChar("19B11A01-F8F2-537E-4F6C-D104768A1215", BLERead | BLENotify),
        watering_5_min(this, 5),
        watering_10_min(this, 10),
        speed(0),
        waterFlow(3, "19B11A02-F8F2-537E-4F6C-D104768A1215")
    { }

    void addCharacteristic(BLEService& service) {
        service.addCharacteristic(pumpSpeedChar);
        waterFlow.addCharacteristic(service);
    }

    void begin() override {
        pinMode(PUMP_PIN, OUTPUT);

        this->waterFlow.begin();

        // Pump programs init
        this->watering_10_min.begin();
    }
};