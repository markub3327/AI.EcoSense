#pragma once

#include <Arduino_HTS221.h>
#include <Arduino_LPS22HB.h>
#include "sensorInterface.hpp"

class Meteo final : public SensorInterface {
    BLEFloatCharacteristic tempChar;
    BLEFloatCharacteristic humidityChar;
    BLEFloatCharacteristic pressureChar;

public:
    Meteo(const char* tempUuid, const char* humUuid, const char* presUuid)
    : tempChar(tempUuid, BLERead | BLENotify),
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
    service.addCharacteristic(humidityChar);
    service.addCharacteristic(pressureChar);
  }

  void update() override {
    this->tempChar.writeValue(HTS.readTemperature());
    this->humidityChar.writeValue(HTS.readHumidity());
    this->pressureChar.writeValue(BARO.readPressure() * 10.0f);    // convert to hPa
  }
};