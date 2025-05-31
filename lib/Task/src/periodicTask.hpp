//
// Created by Martin Kubovčík on 31/05/2025.
//

#pragma once

#include "taskInterface.hpp"

class PeriodicTask : public TaskInterface {
    // timing for weak up and sleep
    unsigned long period;
    unsigned long previousMillis;

public:
    explicit PeriodicTask(const unsigned long period) : period(period), previousMillis(0) {}
    ~PeriodicTask() override = default;

    void run() override {
        unsigned long currentMillis = millis();

        // counter-overflow detection:
        if (currentMillis < previousMillis) {
            previousMillis = currentMillis;
            Serial.println("Task detected counter overflow.");
            this->setState(WAITING);
        }
        // if the period have passed, run the job:
        else if (currentMillis - previousMillis >= this->period) {
            previousMillis = currentMillis;
            this->setState(READY);
            this->update();
        } else {
            this->setState(WAITING);
        }
    }
};
