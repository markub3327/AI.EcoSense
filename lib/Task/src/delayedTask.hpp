//
// Created by Martin Kubovčík on 31/05/2025.
//

#pragma once

#include "periodicTask.hpp"

class DelayedTask : public PeriodicTask {
public:
    template<typename... Args>
    DelayedTask(const unsigned long period) : PeriodicTask(period) {}

    void run() override {
        PeriodicTask::run();
        if (this->getState() == READY) {
            this->setState(PAUSE);
        }
    }

    void start() {
        this->setState(READY);
    }
};