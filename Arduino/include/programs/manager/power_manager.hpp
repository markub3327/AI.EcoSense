#pragma once

#include <ArduinoBLE.h>

#include "../../../lib/Task/src/messageQueue.hpp"
#include "services/va_meter.hpp"


class PowerManager final : public PeriodicTask {
    VAMeter ina1 = {0x40, {
        "19B11D01-F8F2-537E-4F6C-D104768A1215",
        "19B11D02-F8F2-537E-4F6C-D104768A1215",
        "19B11D03-F8F2-537E-4F6C-D104768A1215",
        "19B11D04-F8F2-537E-4F6C-D104768A1215"
    }};
    VAMeter ina2 = {0x41, {
        "19B11E01-F8F2-537E-4F6C-D104768A1215",
        "19B11E02-F8F2-537E-4F6C-D104768A1215",
        "19B11E03-F8F2-537E-4F6C-D104768A1215",
        "19B11E04-F8F2-537E-4F6C-D104768A1215"
    }};
    VAMeter ina3 = {0x42, {
        "19B11F01-F8F2-537E-4F6C-D104768A1215",
        "19B11F02-F8F2-537E-4F6C-D104768A1215",
        "19B11F03-F8F2-537E-4F6C-D104768A1215",
        "19B11F04-F8F2-537E-4F6C-D104768A1215"
    }};

    void update() override {
        ina1.update();
        ina2.update();
        ina3.update();
    }

public:
    template<typename... Args>
    explicit PowerManager(Args&&... args) : PeriodicTask(std::forward<Args>(args)...)
    { }

    void addCharacteristic(BLEService& service) {
        ina1.addCharacteristic(service);
        ina2.addCharacteristic(service);
        ina3.addCharacteristic(service);
    }

    void begin() override {
        ina1.begin();
        ina2.begin();
        ina3.begin();
    }
};