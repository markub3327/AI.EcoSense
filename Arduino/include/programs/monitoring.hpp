#pragma once

#include <ArduinoBLE.h>

#include "../../lib/Task/src/messageQueue.hpp"
#include "sensors/soil_moisure.hpp"
#include "sensors/meteo.hpp"
#include "pins.hpp"


class Monitoring final : public PeriodicTask {
    MessageQueue<int> soilMoistureMsg;

    SoilMoisture soilMoistureChar[6] = {
    {SOIL_MOISTURE_1, "19B11B01-F8F2-537E-4F6C-D104768A1215"},
    {SOIL_MOISTURE_2, "19B11B02-F8F2-537E-4F6C-D104768A1215"},
    {SOIL_MOISTURE_3, "19B11B03-F8F2-537E-4F6C-D104768A1215"},
    {SOIL_MOISTURE_4, "19B11B04-F8F2-537E-4F6C-D104768A1215"},
    {SOIL_MOISTURE_5, "19B11B05-F8F2-537E-4F6C-D104768A1215"},
    {SOIL_MOISTURE_6, "19B11B06-F8F2-537E-4F6C-D104768A1215"}
    };

    Meteo meteoChar = {
        "19B11C01-F8F2-537E-4F6C-D104768A1215",
        "19B11C02-F8F2-537E-4F6C-D104768A1215",
        "19B11C03-F8F2-537E-4F6C-D104768A1215",
        "19B11C04-F8F2-537E-4F6C-D104768A1215"
    };

    void update() override {
        this->meteoChar.update();
        for (auto & it : this->soilMoistureChar) {
            it.update();
            soilMoistureMsg.send(it.getValue());
        }
    }

public:
    template<typename... Args>
    explicit Monitoring(Args&&... args) : PeriodicTask(std::forward<Args>(args)...)
    { }

    void addCharacteristic(BLEService& service) {
        for (auto & it : this->soilMoistureChar) {
            it.addCharacteristic(service);
        }
        meteoChar.addCharacteristic(service);
    }

    void begin() override {
      for (auto & it : this->soilMoistureChar) {
        it.begin();
      }
      meteoChar.begin();
    }
};