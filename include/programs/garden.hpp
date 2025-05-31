#pragma once

#include <ArduinoBLE.h>

#include "../../lib/Task/src/messageQueue.hpp"
#include "sensors/soil_moisure.hpp"
#include "sensors/meteo.hpp"
#include "pins.hpp"


class Garden final : public PeriodicTask {
    BLEService service = {"19B10000-F8F2-537E-4F6C-D104768A1215"};
    MessageQueue<int> soilMoistureMsg;

    SoilMoisture soilMoistureChar[2] = {
    {SOIL_MOISTURE_1, "19B11B01-F8F2-537E-4F6C-D104768A1215"},
    {SOIL_MOISTURE_2, "19B11B02-F8F2-537E-4F6C-D104768A1215"}
    };

    Meteo meteoChar = {"19B11C01-F8F2-537E-4F6C-D104768A1215", "19B11C02-F8F2-537E-4F6C-D104768A1215", "19B11C03-F8F2-537E-4F6C-D104768A1215"};

    void update() override {
        this->meteoChar.update();
        for (auto & it : this->soilMoistureChar) {
            it.update();
            soilMoistureMsg.send(it.getValue());
        }
    }

public:
    template<typename... Args>
    explicit Garden(Args&&... args) : PeriodicTask(std::forward<Args>(args)...)//, bleConnected(nullptr)
    { }

    void begin() override {
      for (auto & it : this->soilMoistureChar) {
        it.addCharacteristic(this->service);
        it.begin();
      }

      meteoChar.addCharacteristic(this->service);
      meteoChar.begin();
    }

    BLEService& getService() { return this->service; }
};