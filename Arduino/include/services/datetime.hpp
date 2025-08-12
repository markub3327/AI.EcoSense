//
// Created by Martin Kubovčík on 18/07/2025.
//

#pragma once

#include "serviceInterface.hpp"


class DateTime final : public ServiceInterface {
    BLECharacteristic characteristic; // Time (HH:MM:SS) characteristic;
    uint8_t receivedTime[7];

public:
    template<typename... Args>
    DateTime(Args&&... args) : characteristic(std::forward<Args>(args)..., BLEWrite, 7)
    {  }
    ~DateTime() override = default;

    void begin() override {
    }

    void addCharacteristic(BLEService &service) override {
        service.addCharacteristic(characteristic);
    }

    uint8_t getDay() const { return this->receivedTime[2]; }
    uint8_t getMonth() const { return this->receivedTime[1]; }
    uint8_t getYear() const { return this->receivedTime[0]; }

    uint8_t getHour() const { return this->receivedTime[3]; }
    uint8_t getMinute() const { return this->receivedTime[4]; }
    uint8_t getSecond() const { return this->receivedTime[5]; }

    uint8_t getDayOfWeek() const { return this->receivedTime[6]; }

    void update() override {
        // Check if the time characteristic has been written to by the central
        if (characteristic.written()) {
            characteristic.readValue(receivedTime, sizeof(receivedTime));

            // Serial.print("Received time: ");
            // Serial.print(receivedTime[2]);
            // Serial.print(".");
            // Serial.print(receivedTime[1]);
            // Serial.print(".");
            // Serial.print(receivedTime[0] + 2000);
            // Serial.print("  ");

            // Serial.print(receivedTime[3]);
            // Serial.print(":");
            // Serial.print(receivedTime[4]);
            // Serial.print(":");
            // Serial.println(receivedTime[5]);
        }
    }
};