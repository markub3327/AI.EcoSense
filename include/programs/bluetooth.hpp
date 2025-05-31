#pragma once

#include <ArduinoBLE.h>
#include "../lib/Task/src/periodicTask.hpp"

enum BLEStatus {
    INITIALIZING = 0,
    ADVERTISING = 1,
    CONNECTED   = 2,
    FAILED      = 3,
};

class Bluetooth final : public PeriodicTask {
    BLEDevice central;
    BLEStatus status;

    bool connected() const { return central.connected(); }

    void update() override {
        if (this->status == INITIALIZING) {
            // start advertising
            BLE.advertise();
            Serial.println("Bluetooth® device active, waiting for connections...");
            this->status = ADVERTISING;
        } else if (this->status == ADVERTISING) {
            // wait for a Bluetooth® Low Energy central
            central = BLE.central();

            // if a central is connected to the peripheral:
            if (central) {
                Serial.print("Connected to central: ");
                // print the central's BT address:
                Serial.println(central.address());
                // turn on the LED to indicate the connection:
                analogWrite(LED_BUILTIN, 10);

                if (this->connected())
                    this->status = CONNECTED;
            }
        } else if (this->status == CONNECTED) {
            if (!this->connected())
            {
                // when the central disconnects, turn off the LED:
                analogWrite(LED_BUILTIN, 0);
                Serial.print("Disconnected from central: ");
                Serial.println(central.address());
                this->status = ADVERTISING;
            }
        }
    }

public:
    template<typename... Args>
    explicit Bluetooth(Args&&... args) : PeriodicTask(std::forward<Args>(args)...), status(INITIALIZING) {}

    void begin() override {
        // begin initialization
        if (!BLE.begin()) {
            Serial.println("starting BLE failed!");
            this->status = FAILED;
        }
        BLE.setDeviceName("AIGarden");
        BLE.setLocalName("AIGarden");
    }

    void addService(BLEService &service) const {
        if (this->status == INITIALIZING) {
            BLE.setAdvertisedService(service);
            BLE.addService(service);
        }
    }

    BLEStatus getStatus() const { return this->status; }
};