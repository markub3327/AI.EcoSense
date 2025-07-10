#pragma once

#include <Arduino_HTS221.h>
#include <Arduino_LPS22HB.h>
#include "sensorInterface.hpp"

class Meteo final : public SensorInterface {
    BLEFloatCharacteristic tempChar, temp2Char;
    BLEFloatCharacteristic humidityChar;
    BLEFloatCharacteristic pressureChar;

public:
    Meteo(const char* tempUuid, const char* temp2Uuid, const char* humUuid, const char* presUuid)
    : tempChar(tempUuid, BLERead | BLENotify),
      temp2Char(temp2Uuid, BLERead | BLENotify),
      humidityChar(humUuid, BLERead | BLENotify),
      pressureChar(presUuid, BLERead | BLENotify)
    { }
    ~Meteo() override = default;

    void begin() override {
        if (!HTS.begin()) {
          Serial.println("Failed to initialize humidity temperature sensor!");
        }

        if (!BARO.begin()) {
          Serial.println("Failed to initialize pressure sensor!");
        }

        // set init value
        this->update();
    }

    void addCharacteristic(BLEService &service) override {
        service.addCharacteristic(tempChar);
        service.addCharacteristic(temp2Char);
        service.addCharacteristic(humidityChar);
        service.addCharacteristic(pressureChar);
    }

    void update() override {
        // HTS221
        this->tempChar.writeValue(HTS.readTemperature());
        this->humidityChar.writeValue(HTS.readHumidity());

        // LPS22HB
        this->temp2Char.writeValue(BARO.readTemperature());
        this->pressureChar.writeValue(BARO.readPressure() * 10.0f);    // convert to hPa
    }
};