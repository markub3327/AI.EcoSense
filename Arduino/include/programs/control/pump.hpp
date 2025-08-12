#pragma once

#include <ArduinoBLE.h>

#include "../../../lib/Task/src/periodicTask.hpp"
#include "../../../lib/Task/src/delayedTask.hpp"
#include "../../services/datetime.hpp"
#include "../../services/water_flow.hpp"
#include "../../pins.hpp"

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
        this->waterFlow.update();
        // Serial.print("Water flow: ");
        // Serial.println(this->waterFlow.getValue());

        // Watering expert system
        if (this->watering_running) {
            if (this->waterFlow.getValue() < 0.03) {
                this->watering_running = false;
                this->stopPump();
            }
        } else {
            if ((dateTime.getHour() == 6 && dateTime.getMinute() <= 15) ||
                (dateTime.getHour() == 18 && dateTime.getMinute() <= 15)) {
                this->watering_15_min.begin();
                this->watering_running = true;
            }
        }
        this->watering_15_min.run();

        // logging the state of the pump
        this->pumpSpeedChar.writeValue(this->speed);
    }

    // Watering expert system
    bool watering_running = false;

    // General
    BLEByteCharacteristic pumpSpeedChar;
    _watering_X_min watering_15_min;
    uint8_t speed;   // in percentage (0% - stop, 100% full speed)
    WaterFlow waterFlow;
    DateTime& dateTime;

public:
    template<typename... Args>
    explicit Pump(DateTime& dateTime, Args&&... args) : PeriodicTask(std::forward<Args>(args)...),
        pumpSpeedChar("19B11A01-F8F2-537E-4F6C-D104768A1215", BLERead | BLENotify),
        watering_15_min(this, 15),
        speed(0),
        waterFlow(3, "19B11A02-F8F2-537E-4F6C-D104768A1215"),
        dateTime(dateTime)
    { }

    void addCharacteristic(BLEService& service) {
        service.addCharacteristic(this->pumpSpeedChar);
        this->waterFlow.addCharacteristic(service);
    }

    void begin() override {
        pinMode(PUMP_PIN, OUTPUT);

        this->waterFlow.begin();
    }
};