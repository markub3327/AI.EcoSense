#pragma once

#include <Arduino_HS300x.h>
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
        if (!HS300x.begin()) {
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
        // HS300x
        auto t1 = HS300x.readTemperature();
        auto h = HS300x.readHumidity();
        this->tempChar.writeValue(t1);
        this->humidityChar.writeValue(h);

        // Serial.print("Temp1: ");
        // Serial.println(t1);
        // Serial.print("Humidity: ");
        // Serial.println(h);

        // LPS22HB
        auto t2 = BARO.readTemperature();
        auto p = BARO.readPressure() * 10.0f;
        this->temp2Char.writeValue(t2);
        this->pressureChar.writeValue(p);    // convert to hPa

        // Serial.print("Temp2: ");
        // Serial.println(t2);
        // Serial.print("Pressure: ");
        // Serial.println(p);
    }
};